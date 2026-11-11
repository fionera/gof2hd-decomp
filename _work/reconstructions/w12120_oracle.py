#!/usr/bin/env python3
import subprocess, sys, re, difflib, os, json
ROOT='/home/debian/GalaxyOnFire2/wt/w12120'
NDK=ROOT+'/.cache/ndk/android-ndk-r18b'
OBJ=NDK+'/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
SCRATCH='/home/debian/GalaxyOnFire2/scratch/w12120'

def compile_cmd(src):
    d=json.load(open(ROOT+'/cmake-build-match/compile_commands.json'))
    for e in d:
        if e['file'].endswith(src):
            return e['command']
    raise SystemExit('no cmd for '+src)

def build(src, outobj):
    cmd=compile_cmd(src)
    cmd=re.sub(r'-o \S+\.obj', '-o '+outobj, cmd)
    subprocess.check_call(cmd, shell=True, cwd=ROOT, stderr=subprocess.DEVNULL)

def sym_dyn(path,name):
    out=subprocess.check_output([OBJ,'-T',path],text=True)
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name:
            return int(p[0],16),int(p[4],16)
    return None

def sym_obj(path,name):
    out=subprocess.check_output([OBJ,'-t',path],text=True)
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name:
            return int(p[0],16),int(p[4],16)
    return None

def relocs(path, secname):
    out=subprocess.check_output([OBJ,'-r','-j',secname,path],text=True)
    r={}
    for l in out.splitlines():
        m=re.match(r'^([0-9a-f]+)\s+\S+\s+(\S+)\s*$',l)
        if m:
            r[int(m.group(1),16)]=m.group(2)
    return r

def sec_of(path,name):
    out=subprocess.check_output([OBJ,'-t',path],text=True)
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name:
            return p[3]  # section name field? check
    return None

def disasm_obj(path,name):
    out=subprocess.check_output([OBJ,'-t',path],text=True)
    ndx=None
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name:
            ndx=p[3]
            break
    secname=None
    if ndx is not None and ndx.startswith('.'):
        secname=ndx
    elif ndx is not None and ndx.isdigit():
        h=subprocess.check_output([OBJ,'-h',path],text=True)
        for hl in h.splitlines():
            hm=re.match(r'^\s*(\d+)\s+(\S+)',hl)
            if hm and hm.group(1)==ndx:
                secname=hm.group(2); break
    if secname is None:
        secname='.text.'+name
    out=subprocess.check_output([OBJ,'-d','-j',secname,path],text=True)
    ins=[]; pools=[]
    for l in out.splitlines():
        f=l.split('\t')
        if len(f)<3: continue
        m=re.match(r'\s*([0-9a-f]+):',f[0])
        if not m: continue
        a=int(m.group(1),16); t=' '.join(x.strip() for x in f[2:]).strip(); by=f[1].strip()
        if t.startswith('.word'):
            if pools and pools[-1][1]==a: pools[-1]=(pools[-1][0],a+4)
            else: pools.append((a,a+4))
        else: ins.append((a,t,by))
    rl=relocs(path,secname)
    return ins, rl, pools

def disasm(path,addr,size):
    out=subprocess.check_output([OBJ,'-d','--start-address=%d'%addr,'--stop-address=%d'%(addr+size),path],text=True)
    ins=[]
    for l in out.splitlines():
        f=l.split('\t')
        if len(f)<3: continue
        m=re.match(r'\s*([0-9a-f]+):',f[0])
        if not m: continue
        a=int(m.group(1),16); t=' '.join(x.strip() for x in f[2:]).strip(); by=f[1].strip()
        if t: ins.append((a,t,by))
    return ins

BRANCH=re.compile(r'^(b|bl|blx|bx|b[a-z]{1,2}|cbz|cbnz|tbb|tbh)(\.w|\.n)?$')
def norm(ins, rl=None):
    idx={a:i for i,(a,t,by) in enumerate(ins)}
    out=[]
    for a,t,by in ins:
        sym=None
        m=re.search(r'<([^>]+)>',t)
        if m: sym=m.group(1)
        t=re.sub(r'<[^>]*>','',t).strip()
        parts=t.split()
        op=parts[0]
        if BRANCH.match(op) and len(parts)>=2:
            tgt=parts[-1]
            if op.startswith('bl'):
                nm=None
                try:
                    ad=int(tgt,16)
                    if rl is not None and a in rl: nm=rl[a]
                except ValueError: pass
                if nm is None and sym: nm=sym
                nm=(nm or 'UNKNOWN')
                nm=re.sub(r'@@.*$','',nm); nm=re.sub(r'\+0x[0-9a-f]+$','',nm); nm=nm.replace('@plt','')
                out.append('CALL SYM:'+nm); continue
            out.append(re.sub(r'\s+',' ',' '.join(parts[:-1])).strip()+' B:'+by.replace(' ','').lower()); continue
        t=re.sub(r'\s+',' ',(' '.join(parts))).strip()
        t=re.sub(r';\s*[0-9a-f]+.*$','; POOL',t)
        t=re.sub(r'\([0-9a-f]+ ?\)$','(POOL)',t)
        t=re.sub(r'; \(adr [^)]*\)','; POOL',t)
        out.append(t)
    return out

def score(src, name, verbose=True):
    outobj=SCRATCH+'/t.o'
    build(src,outobj)
    a=sym_dyn(ORIG,name)
    b=sym_obj(outobj,name)
    ui,rl,pools=disasm_obj(outobj,name)
    oi=disasm(ORIG,*a)
    def inp(ad, base):
        off=ad-base
        return any(lo<=off<hi for lo,hi in pools)
    oi=[(ad,t,by) for ad,t,by in oi if not inp(ad,a[0])]
    ui=[(ad,t,by) for ad,t,by in ui if not inp(ad,0)]
    ot=norm(oi); ut=norm(ui,rl)
    sm=difflib.SequenceMatcher(None,ot,ut,autojunk=False)
    cats=[]
    for tag,i1,i2,j1,j2 in sm.get_opcodes():
        if tag=='equal': continue
        cats.append((tag,i1,i2,j1,j2))
    d=sum(max(i2-i1,j2-j1) for _,i1,i2,j1,j2 in cats)
    print('%-60s osize=%d usize=%d uinsn=%d oinsn=%d diff=%d'%(name,a[1],b[1] if b else -1,len(ui),len(oi),d))
    if verbose:
        for tag,i1,i2,j1,j2 in cats:
            print('  ',tag)
            for k in range(i1,i2): print('    O',ot[k])
            for k in range(j1,j2): print('    U',ut[k])
    return d

if __name__=='__main__':
    score(sys.argv[1],sys.argv[2])
