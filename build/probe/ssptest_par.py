#!/usr/bin/env python3
# Parallel -fstack-protector vs -strong blast-radius test. Compiles every matches.tsv source with
# each flag ACROSS ALL CORES (multiprocessing) and counts exacts. Run in the guest.
import subprocess, os, sys, re, multiprocessing as mp
REPO="/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"; os.chdir(REPO)
SO="../_work/bins/android_2.0.16_libgof2hdaa.so"
TC="/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64"; CXX=TC+"/bin/clang++"
BASE="-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti -Iinclude".split()
OBJD="arm-linux-gnueabihf-objdump"

entries=[]
for ln in open("matches.tsv"):
    if ln.startswith("#") or not ln.strip(): continue
    name,vaddr,n,obj,sym,status=ln.rstrip("\n").split("\t")
    src="src/"+obj[len("build/obj/"):].rsplit(".o",1)[0]+".cpp"
    if os.path.exists(src): entries.append((name,vaddr,int(n),sym,src,status))

BR=re.compile(r'(b|cbz|cbnz|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(\.[wn])?$')
def norm(ins):
    ins=re.sub(r';.*','',ins); ins=re.sub(r'\s@\s.*$','',ins); ins=re.sub(r'<[^>]*>','',ins)
    ins=re.sub(r'\s+',' ',ins.strip()); mn=ins.split(' ',1)[0]
    if mn in('bl','blx'): return 'call'
    if BR.match(mn) and ins!='bx lr': return mn+' @'
    ins=re.sub(r'\bpc, #[0-9]+\b','pc, #@',ins); ins=re.sub(r'0x[0-9a-f]+','@',ins); ins=re.sub(r'\b[0-9a-f]{5,}\b','@',ins)
    return re.sub(r'\s+',' ',ins).strip()
def tgt_ins(vaddr,n):
    out=subprocess.run([OBJD,"-d","--no-show-raw-insn","-M","force-thumb","--start-address=0x%x"%vaddr,"--stop-address=0x%x"%(vaddr+n),SO],capture_output=True,text=True).stdout
    r=[]
    for l in out.splitlines():
        m=re.match(r'\s*[0-9a-f]+:\s+(\S.*)',l)
        if m:
            ni=norm(m.group(1)); r.append(ni)
            if re.search(r'pop \{[^}]*pc\}',ni) or ni=='bx lr': break
    return r
def obj_ins(o,sym):
    out=subprocess.run([OBJD,"-d","-C","--no-show-raw-insn",o],capture_output=True,text=True).stdout
    r=[]; cap=False
    for l in out.splitlines():
        h=re.match(r'[0-9a-f]+ <(.+)>:',l)
        if h: cap=sym in h.group(1); continue
        if cap:
            if not l.strip(): cap=False; continue
            m=re.match(r'\s*[0-9a-f]+:\s+(\S.*)',l)
            if m:
                ni=norm(m.group(1))
                if re.match(r'\.(word|short|hword|byte|inst|long)\b',ni): continue
                r.append(ni)
                if re.search(r'pop \{[^}]*pc\}',ni) or ni=='bx lr': cap=False
    return r
import difflib
def pct_of(vaddr,n,o,sym):
    t=tgt_ins(vaddr,n); g=obj_ins(o,sym)
    if g and len(t)>len(g)+2: t=t[:len(g)]
    return difflib.SequenceMatcher(None,t,g).ratio()*100

def work(arg):
    name,vaddr,n,sym,src,status,flag=arg
    o="/tmp/p_%d_%s.o"%(os.getpid(),re.sub(r'\W','_',name)[:30])
    r=subprocess.run([CXX]+BASE+[flag,"-c",src,"-o",o],capture_output=True,text=True)
    if r.returncode!=0: return (name,status,flag,None)
    try: p=pct_of(int(vaddr,16),n,o,sym)
    except Exception: p=None
    finally:
        try: os.remove(o)
        except OSError: pass
    return (name,status,flag,p)

if __name__=="__main__":
    tasks=[(n,v,nb,sym,src,st,f) for (n,v,nb,sym,src,st) in entries for f in ("-fstack-protector","-fstack-protector-strong")]
    with mp.Pool(mp.cpu_count()) as pool:
        res=pool.map(work,tasks,chunksize=8)
    by={}
    for name,status,flag,p in res: by.setdefault(name,{})[flag]=(status,p)
    base_ok=strong_ok=0; reg=[]
    for name,d in by.items():
        st=d["-fstack-protector"][0]; need=100 if st=="exact" else 90
        b=d["-fstack-protector"][1]; s=d["-fstack-protector-strong"][1]
        if b and b>=need: base_ok+=1
        if s and s>=need: strong_ok+=1
        if b and b>=need and (not s or s<need): reg.append((name,round(s or 0,1)))
    print("of %d sources (parallel x%d):"%(len(by),mp.cpu_count()))
    print("  -fstack-protector        ok = %d"%base_ok)
    print("  -fstack-protector-strong ok = %d"%strong_ok)
    print("  regressed by -strong = %d"%len(reg))
    for nm,s in reg[:60]: print("    %-34s strong%%=%s"%(nm,s))
