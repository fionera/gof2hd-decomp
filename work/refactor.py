#!/usr/bin/env python3
# Portability refactor engine: convert a class's offset-soup -> real named-field struct.
# Merges Ghidra's recovered layout (work/runtime/target/layouts.tsv: authoritative pointer/scalar
# typing) with the corpus's accessor usage, injects a real struct into class.h, and rewrites simple
# accessor(IDENT, OFF) calls to IDENT->field. Gate: the refactored work/ source must compile to the
# SAME armv7 .text as the known-good src/recovered/ offset form (byte-exact preserved) -> safe.
# Unconverted complex accessors keep working via the existing helpers (partial conversion is safe).
# Usage: python3 work/refactor.py <Class> [<Class> ...]
import subprocess, re, os, sys, glob
REPO=os.path.dirname(os.path.dirname(os.path.abspath(__file__))); os.chdir(REPO)
TC="/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64"
AFLAGS="-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti"

# 1) Ghidra layouts: struct -> [(off,size,gtype,name)]
LAY={}
cur=None
for ln in open("work/runtime/target/layouts.tsv"):
    p=ln.rstrip("\n").split("\t")
    if p[0]=="#STRUCT": cur=p[1]; LAY[cur]=[]
    elif cur and len(p)>=4:
        LAY[cur].append((int(p[0]),int(p[1]),p[2],p[3]))

def ctype_of(gtype, accsizes):
    g=gtype.strip()
    if g.endswith("*"): return g.replace(" *","*"), True          # pointer (Ghidra)
    if g in ("float",): return "float", False
    if g in ("double",): return "double", False
    if g in ("uchar","byte","undefined1","bool","uint8_t","char"): return "uint8_t", False
    if g in ("ushort","undefined2","uint16_t","short"): return "uint16_t", False
    if g in ("uint","undefined4","dword","int","uint32_t","undefined"): return "int", False
    if g in ("undefined8","longlong","ulonglong"): return "long long", False
    # fall back to accessor-derived size
    if 8 in accsizes: return "long long", False
    if 2 in accsizes: return "uint16_t", False
    if 1 in accsizes: return "uint8_t", False
    return "int", False

ACC=re.compile(r'\b(i32|u32|u8|u16|f32|pp|P|C|F\s*<\s*([^>]*?)\s*>)\s*\(\s*([A-Za-z_]\w*)\s*,\s*(0[xX][0-9a-fA-F]+|\d+)\s*\)')
ACCSZ={"i32":4,"u32":4,"f32":4,"pp":4,"P":4,"C":1,"u8":1,"u16":2}
def acc_ptr(kind,tparam):
    if kind in ("pp","P"): return True
    if kind=="F" and tparam and "*" in tparam: return True
    return False

def refactor(cls):
    sd="work/classes/%s/src"%cls
    if not os.path.isdir(sd): return (cls,"no dir")
    srcs=glob.glob(sd+"/*.cpp")
    # 2) accessor usage across methods: off -> (sizes set, ptr?)
    acc={}
    for f in srcs:
        for m in ACC.finditer(open(f).read()):
            kind=m.group(1).split("<")[0].strip() if m.group(1).startswith("F") else m.group(1)
            kind="F" if m.group(1).startswith("F") else kind
            off=int(m.group(4),16) if m.group(4).lower().startswith("0x") else int(m.group(4))
            e=acc.setdefault(off,[set(),False])
            e[0].add(ACCSZ.get(kind,4));
            if acc_ptr(kind,m.group(2)): e[1]=True
    # 3) merge: Ghidra fields are authoritative spans; accessor offsets INSIDE a span are sub-field
    #    byte-accesses -> keep the helper (don't add a field). Only field-START offsets convert.
    fields={}  # off -> [ctype, name, size32]
    spans=[]
    for off,size,gtype,name in LAY.get(cls,[]):
        ct,ptr=ctype_of(gtype, acc.get(off,[set()])[0])
        sz=4 if ptr else size
        nm=name if (name and not name.startswith("field")) else "f_%x"%off
        fields[off]=[ct,nm,sz]; spans.append((off,off+sz))
    overlaps=lambda o,sz: any(not(e<=o or s>=o+sz) for s,e in spans)
    for off,(sizes,ptr) in sorted(acc.items()):
        if off in fields:
            if ptr and not fields[off][0].endswith("*"): fields[off]=["void*",fields[off][1],4]
            continue
        if ptr: ct,sz="void*",4
        elif 8 in sizes: ct,sz="long long",8
        elif 2 in sizes: ct,sz="uint16_t",2
        elif 1 in sizes: ct,sz="uint8_t",1
        else: ct,sz="int",4
        if overlaps(off,sz): continue       # word-over-bytes or sub-field access -> keep helper
        fields[off]=[ct,"f_%x"%off,sz]; spans.append((off,off+sz))
    if not fields: return (cls,"no fields")
    # 4) ordered fields + padding (armv7 layout). 32-bit sizes used for offset math.
    SZ32={"uint8_t":1,"uint16_t":2,"int":4,"unsigned":4,"float":4,"void*":4,"long long":8,"double":8}
    offs=sorted(fields); lines=[]; cur=0; off2name={}
    for off in offs:
        ct,nm,sz=fields[off]
        if off<cur: continue                       # overlapped by previous -> keep helper (byte-exact)
        if off>cur: lines.append("    unsigned char _pad_%x[%d];"%(cur,off-cur))
        decl = ct.replace("*","* ")+nm if ct.endswith("*") else ct+" "+nm
        lines.append("    %s; // 0x%x"%(decl,off))
        off2name[off]=nm
        cur=off+(4 if ct.endswith("*") else SZ32.get(ct,4))
    struct_body="\n".join(lines)
    return (cls, "ok", off2name, struct_body, len(off2name))

def apply(cls, off2name, body):
    ch="work/classes/%s/src/class.h"%cls; t=open(ch).read()
    if "@portable-fields" in t: return False     # already converted (idempotent)
    # skip if any .cpp defines its OWN `struct <cls> { ... }` (injecting into class.h would redefine it)
    for f in glob.glob("work/classes/%s/src/*.cpp"%cls):
        if re.search(r'\bstruct\s+%s\s*\{'%re.escape(cls), open(f).read()): return False
    body="    // @portable-fields\n"+body
    # inject fields into `struct <cls> { ... }` (or define a forward-declared one)
    m=re.search(r'\bstruct\s+%s\s*\{'%re.escape(cls), t)
    if m: t=t[:m.end()]+"\n"+body+"\n"+t[m.end():]
    else:
        m=re.search(r'\bstruct\s+%s\s*;'%re.escape(cls), t)
        if not m: return False
        t=t[:m.start()]+("struct %s {\n%s\n};"%(cls,body))+t[m.end():]
    open(ch,"w").write(t)
    # rewrite simple accessor(IDENT, OFF) -> IDENT->field for converted offsets
    def repl(m2):
        kind=m2.group(1); ident=m2.group(3)
        off=int(m2.group(4),16) if m2.group(4).lower().startswith("0x") else int(m2.group(4))
        nm=off2name.get(off)
        return "%s->%s"%(ident,nm) if nm else m2.group(0)
    for f in glob.glob("work/classes/%s/src/*.cpp"%cls):
        s=open(f).read(); s2=ACC.sub(repl,s)
        if s2!=s: open(f,"w").write(s2)
    return True

def textsec(o):
    subprocess.run(["orb","run","arm-linux-gnueabihf-objcopy","-O","binary","--only-section=.text",o,o+".bin"],capture_output=True)
    try: return open(o+".bin","rb").read()
    except: return b""
def verify(cls):
    sd="work/classes/%s/src"%cls; rd="src/recovered/%s"%cls
    ok=True; checked=0
    cmds=[]
    for f in glob.glob(sd+"/*.cpp"):
        b=os.path.basename(f); rf=rd+"/"+b
        # match reassembly's include setup (-Iinclude; class.h found via quote-include) so verify
        # catches exactly what the full build would (e.g. struct redefinitions).
        new_o="/tmp/n_%s_%s.o"%(cls,b); cmd_n="%s/bin/clang++ %s -Iinclude -c %s -o %s"%(TC,AFLAGS,f,new_o)
        if os.path.exists(rf):
            old_o="/tmp/o_%s_%s.o"%(cls,b); cmd_o="%s/bin/clang++ %s -Iinclude -c %s -o %s"%(TC,AFLAGS,rf,old_o)
            cmds.append((b,cmd_n,cmd_o,new_o,old_o)); checked+=1
        else:
            cmds.append((b,cmd_n,None,new_o,None))
    script=" ; ".join(c[1]+(" ; "+c[2] if c[2] else "") for c in cmds)
    try: r=subprocess.run(["orb","run","bash","-lc",script],capture_output=True,text=True,timeout=180)
    except subprocess.TimeoutExpired: return (False,"timeout",checked)
    if r.returncode!=0: return (False,"compile error",checked)
    for b,_,cmd_o,new_o,old_o in cmds:
        if old_o:
            if textsec(new_o)!=textsec(old_o): return (False,"DIFF %s"%b,checked)
    return (True,"ok",checked)

def process(cls):
    sd="work/classes/%s/src"%cls
    if not os.path.isdir(sd): return (cls,"skip","no dir",0)
    backup={f:open(f).read() for f in glob.glob(sd+"/*.cpp")+[sd+"/class.h"] if os.path.exists(f)}
    r=refactor(cls)
    if r[1]!="ok": return (cls,"skip",r[1],0)
    _,_,off2name,body,nf=r
    if not apply(cls,off2name,body): return (cls,"skip","done/no-struct",0)
    ok,why,chk=verify(cls)
    if ok: return (cls,"OK","%d fields, %d verified"%(nf,chk),nf)
    for f,c in backup.items(): open(f,"w").write(c)   # restore (no git race)
    return (cls,"REVERT",why,0)

def commit(names):
    if not names: return
    subprocess.run(["git","add"]+["work/classes/"+c for c in names],capture_output=True)
    subprocess.run(["git","commit","-q","-m","portability wave: +%d classes -> named-field structs (byte-exact verified)"%len(names)],capture_output=True)

if __name__=="__main__":
    import concurrent.futures as cf
    classes=sys.argv[1:]
    if classes==["all"]: classes=sorted(os.path.basename(d) for d in glob.glob("work/classes/*") if os.path.isdir(d))
    npass=nfail=nskip=0; pending=[]
    # fewer workers (orb dispatch stalls under heavy parallel load); commit every 25 -> durable,
    # idempotent re-run resumes after any stall.
    with cf.ThreadPoolExecutor(max_workers=5) as ex:
        futs={ex.submit(process,c):c for c in classes}
        for fut in cf.as_completed(futs):
            cls,st,why,nf=fut.result()
            if st=="OK": npass+=1; pending.append(cls)
            elif st=="REVERT": nfail+=1
            else: nskip+=1
            print("%-26s %-7s %s"%(cls,st,why),flush=True)
            if len(pending)>=25: commit(pending); pending=[]
    commit(pending)
    print("\n== %d converted, %d reverted, %d skipped =="%(npass,nfail,nskip))
