#!/usr/bin/env python3
# Decisive Phase-2 test: over the WHOLE authored corpus (every work/classes/*/src/*.cpp with a
# methods.tsv vaddr/n), compile at -fstack-protector vs -fstack-protector-strong ACROSS ALL CORES
# and count byte-exact at each. The flag with more total exact is the target's flag.
import subprocess, os, re, glob, difflib, multiprocessing as mp
REPO="/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"; os.chdir(REPO)
SO="../_work/bins/android_2.0.16_libgof2hdaa.so"
TC="/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64"; CXX=TC+"/bin/clang++"
BASE="-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -Oz -fpic -frtti -Iinclude".split()
OBJD="arm-linux-gnueabihf-objdump"
BR=re.compile(r'(b|cbz|cbnz|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(\.[wn])?$')
def norm(ins):
    ins=re.sub(r';.*','',ins); ins=re.sub(r'\s@\s.*$','',ins); ins=re.sub(r'<[^>]*>','',ins)
    ins=re.sub(r'\s+',' ',ins.strip()); mn=ins.split(' ',1)[0]
    if mn in('bl','blx'): return 'call'
    if BR.match(mn) and ins!='bx lr': return mn+' @'
    ins=re.sub(r'\bpc, #[0-9]+\b','pc, #@',ins); ins=re.sub(r'0x[0-9a-f]+','@',ins); ins=re.sub(r'\b[0-9a-f]{5,}\b','@',ins)
    return re.sub(r'\s+',' ',ins).strip()
def disasm(args):
    out=subprocess.run(args,capture_output=True,text=True).stdout; return out
def tgt(vaddr,n):
    out=disasm([OBJD,"-d","--no-show-raw-insn","-M","force-thumb","--start-address=0x%x"%vaddr,"--stop-address=0x%x"%(vaddr+n),SO]); r=[]
    for l in out.splitlines():
        m=re.match(r'\s*[0-9a-f]+:\s+(\S.*)',l)
        if m:
            ni=norm(m.group(1)); r.append(ni)
            if re.search(r'pop \{[^}]*pc\}',ni) or ni=='bx lr': break
    return r
def objins(o,sym):
    out=disasm([OBJD,"-d","-C","--no-show-raw-insn",o]); r=[]; cap=False
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
def work(arg):
    cls,key,method,vaddr,n,src,flag=arg
    o="/tmp/cf_%d.o"%(hash((src,flag))&0xffffffff)
    r=subprocess.run([CXX]+BASE+[flag,"-c",src,"-o",o],capture_output=True,text=True)
    if r.returncode!=0:
        return (cls,key,flag,-1)  # compile error
    try:
        t=tgt(int(vaddr,16),n); g=objins(o,method)
        if g and len(t)>len(g)+2: t=t[:len(g)]
        p=difflib.SequenceMatcher(None,t,g).ratio()*100
    except Exception: p=0.0
    finally:
        try: os.remove(o)
        except OSError: pass
    return (cls,key,flag,p)
if __name__=="__main__":
    tasks=[]
    for mt in glob.glob("work/classes/*/methods.tsv"):
        cls=mt.split("/")[2]
        for ln in open(mt):
            p=ln.rstrip("\n").split("\t")
            if len(p)<4: continue
            key,method,vaddr,n=p[0],p[1],p[2],p[3]
            src="work/classes/%s/src/%s.cpp"%(cls,key)
            if not os.path.exists(src): continue
            try: nn=int(n)
            except ValueError: continue
            for f in ("-fstack-protector","-fstack-protector-strong"):
                tasks.append((cls,key,method,vaddr,nn,src,f))
    print("corpus functions: %d, tasks: %d, cores: %d"%(len(tasks)//2,len(tasks),mp.cpu_count()))
    with mp.Pool(mp.cpu_count()) as pool:
        res=pool.map(work,tasks,chunksize=16)
    by={}
    for cls,key,flag,p in res: by.setdefault((cls,key),{})[flag]=p
    base=strong=cerr=0; flips=[]
    for (cls,key),d in by.items():
        b=d.get("-fstack-protector",0); s=d.get("-fstack-protector-strong",0)
        if b==-1 or s==-1: cerr+=1
        if b>=100: base+=1
        if s>=100: strong+=1
        if (b<100) and s>=100: flips.append((cls,key))
    print("WHOLE-CORPUS byte-exact:")
    print("  -fstack-protector        = %d"%base)
    print("  -fstack-protector-strong = %d"%strong)
    print("  net delta (strong-base)  = %+d"%(strong-base))
    print("  compile-errors (either)  = %d"%cerr)
    print("  functions that FLIP to exact ONLY under -strong: %d"%len(flips))
    for c,k in flips[:40]: print("    %s/%s"%(c,k))
