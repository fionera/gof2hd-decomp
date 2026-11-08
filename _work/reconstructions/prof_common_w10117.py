#!/usr/bin/env python3
"""Common helpers for the w10117 sample/instrumented PGO replay."""
import json, os, re, struct, subprocess, sys, difflib
ROOT='/home/debian/GalaxyOnFire2/wt/w10117'
BUILD=os.path.join(ROOT,'cmake-build-match')
NDK=os.path.join(ROOT,'.cache/ndk/android-ndk-r18b')
PROFDATA=os.path.join(NDK,'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
OBJDUMP=os.path.join(NDK,'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump')
VERIFY=os.path.join(ROOT,'tools/verify/verify.py')
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
SCRATCH='/dev/shm/w10117'
os.makedirs(SCRATCH,exist_ok=True)

def run(cmd,**kw):
    return subprocess.run(cmd,shell=isinstance(cmd,str),check=True,capture_output=True,text=True,**kw)

def compile_cmd(tu):
    for e in json.load(open(os.path.join(BUILD,'compile_commands.json'))):
        if e['file'].endswith(tu):
            return e['command']
    raise SystemExit('no compile cmd for '+tu)

def link_parts():
    text=open(os.path.join(BUILD,'build.ninja')).read()
    m=re.search(r'\n  COMMAND = (cd .*?libgof2hdaa\.so[^\n]*)\n',text)
    return m.group(1).split(' && ')

def sym(path,name):
    for l in run([OBJDUMP,'-T',path]).stdout.split('\n'):
        p=l.split()
        if len(p)>=6 and p[-1]==name: return int(p[0],16),int(p[4],16)
    return None

def text_section(path):
    for l in run([OBJDUMP,'-h',path]).stdout.split('\n'):
        p=l.split()
        if len(p)>5 and p[1]=='.text': return int(p[3],16),int(p[5],16)
    raise SystemExit('no .text')

def disasm(path,addr,size):
    out=run([OBJDUMP,'-d','--no-show-raw-insn','--start-address=%d'%addr,
             '--stop-address=%d'%(addr+size),path]).stdout
    rows=[]
    for ln in out.split('\n'):
        m=re.match(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$',ln)
        if m: rows.append((int(m.group(1),16),m.group(2),m.group(3).strip()))
    return rows

def text_bytes(path,addr,size):
    d=open(path,'rb').read(); vma,off=text_section(path)
    return d[off+addr-vma:off+addr-vma+size]

def measure(so,name):
    a,b=sym(ORIG,name),sym(so,name)
    if not a or not b: return None
    x,y=text_bytes(ORIG,*a),text_bytes(so,*b)
    n=min(len(x),len(y))
    raw=sum(p!=q for p,q in zip(x[:n],y[:n]))
    oi=[(r[0],r[1]+' '+r[2]) for r in disasm(ORIG,*a)]
    ui=[(r[0],r[1]+' '+r[2]) for r in disasm(so,*b)]
    oidx={ad:i for i,(ad,_) in enumerate(oi)}; uidx={ad:i for i,(ad,_) in enumerate(ui)}
    def norm(t,idx):
        def rep(m):
            ad=int(m.group(1),16)
            return 'I%d'%idx[ad] if ad in idx else 'X'
        t=re.sub(r'\b([0-9a-f]{4,8})\b',rep,t); t=re.sub(r'<[^>]*>','',t)
        return re.sub(r'\s+',' ',t).strip()
    ot=[norm(t,oidx) for _,t in oi]; ut=[norm(t,uidx) for _,t in ui]
    sm=difflib.SequenceMatcher(None,ot,ut,autojunk=False)
    aligned=sum(max(i2-i1,j2-j1) for tag,i1,i2,j1,j2 in sm.get_opcodes() if tag!='equal')
    return dict(raw=raw,aligned=aligned,size=len(y),orig_size=len(x),so=so)

def relink():
    for part in link_parts():
        subprocess.run(part,shell=True,check=True,cwd=ROOT,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
    return os.path.join(BUILD,'libgof2hdaa.so')

def compile_tu(tu,extra=(),emit_ir=None,obj=None):
    cc=compile_cmd(tu)
    m=re.search(r'-o (\S+)',cc)
    out=obj or m.group(1)
    c=cc.replace(m.group(1),out)
    if emit_ir:
        c=c.replace(' -c ',' -S -emit-llvm -c ')
    c=c.replace(' -c ',' '+' '.join(extra)+' -c ',1)
    subprocess.run(c,shell=True,check=True,cwd=BUILD,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
    return out

def verify(so,tag):
    bd=os.path.join(SCRATCH,'verify_'+tag); os.makedirs(bd,exist_ok=True)
    run([sys.executable,VERIFY,'--build-dir',bd,'--original',ORIG,'--ours',so,'--objdump',OBJDUMP])
    r=json.load(open(os.path.join(bd,'report.json')))['summary']
    return r['byte_exact'],r['linked_exact']
