#!/usr/bin/env python3
"""Generic synthetic instrumented-PGO replay for one function in one TU."""
import os, re, sys, json, struct, subprocess, difflib
REPO='/home/debian/GalaxyOnFire2/wt/w10514'
BUILD=os.path.join(REPO,'cmake-build-match')
NDK=os.path.join(REPO,'.cache/ndk/android-ndk-r18b')
PROFDATA=os.path.join(NDK,'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
OBJDUMP=os.path.join(NDK,'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump')
VERIFY=os.path.join(REPO,'tools/verify/verify.py')
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
SCRATCH=os.environ.get('W10414_SCRATCH','/dev/shm/w10514/run')
PREFIX_OFF=['-mllvm','-profile-guided-section-prefix=false']
os.makedirs(SCRATCH,exist_ok=True)

def run(cmd,**kw):
    return subprocess.run(cmd,shell=isinstance(cmd,str),check=True,capture_output=True,text=True,**kw)

def compile_cmd(tu):
    out=run(['ninja','-C',BUILD,'-t','commands']).stdout.split('\n')
    for c in out:
        if 'clang++' in c and ' -c ' in c and c.rstrip().endswith(tu):
            return c
    raise SystemExit('compile line not found for '+tu)

def link_cmd():
    out=run(['ninja','-C',BUILD,'-t','commands','libgof2hdaa.so']).stdout.split('\n')
    for c in out:
        if 'ld.gold' in c and '-shared' in c:
            return c
    raise SystemExit('link command not found')

def obj_path(tu):
    return os.path.join(BUILD,'CMakeFiles/gof2.dir','./'+tu+'.obj')

def instrument_ir(cmd,path):
    m=re.search(r'-o (\S+)',cmd)
    c=cmd.replace(m.group(1),path).replace(' -c ',' -fprofile-instr-generate -emit-llvm -S -c ')
    run(c,cwd=BUILD)

def parse_meta(ir,mangle):
    txt=open(ir).read()
    m=re.search(r'^define [^\n]*@%s\(.*?\n\}'%re.escape(mangle),txt,re.S|re.M)
    if not m: raise SystemExit('function not in IR: '+mangle)
    fn=m.group(0)
    pl=re.search(r'^@__profd_%s = .*$'%re.escape(mangle),txt,re.M).group(0)
    ints=re.findall(r'i64 (-?\d+)',pl)
    fhash=int(ints[1]) & 0xFFFFFFFFFFFFFFFF; ncounters=int(re.search(r'i32 (\d+), \[2 x i16\]',pl).group(1))
    cur=None; lab_ctr={}
    for ln in fn.split('\n'):
        lm=re.match(r'^; <label>:(\d+):',ln)
        if lm: cur=int(lm.group(1)); continue
        cm=re.search(r'@__profc_%s, i32 0, i32 (\d+)\)'%re.escape(mangle),ln)
        if cm and cur is not None and cur not in lab_ctr and 'load i64' in ln:
            lab_ctr[cur]=int(cm.group(1))
    return dict(fhash=fhash,ncounters=ncounters,lab_ctr=lab_ctr,fn=fn,txt=txt)

def write_profile(mangle,fhash,ncounters,vals,path):
    with open(path,'w') as f:
        f.write('%s\n# Func Hash:\n%d\n# Num Counters:\n%d\n# Counter Values:\n'%(mangle,fhash,ncounters))
        for v in vals: f.write('%d\n'%v)

def merge(text,data): run([PROFDATA,'merge',text,'-o',data])

def compile_use(cmd,profdata,obj,extra=()):
    m=re.search(r'-o (\S+)',cmd)
    c=cmd.replace(m.group(1),obj)
    flags=list(extra)+['-fprofile-instr-use=%s'%profdata]
    c=c.replace(' -c ',' '+' '.join(flags)+' -c ',1)
    run(c,cwd=BUILD)

def link_with(repls,so):
    lc=link_cmd()
    for canon,obj in repls:
        lc=lc.replace(canon,obj)
    lc=lc.replace(os.path.join(BUILD,'libgof2hdaa.unstripped.so'),so+'.un')
    lc=lc.replace(os.path.join(BUILD,'libgof2hdaa.so'),so)
    run(lc,cwd=BUILD)

def sym(path,name):
    for l in run([OBJDUMP,'-T',path]).stdout.split('\n'):
        p=l.split()
        if len(p)>=6 and p[-1]==name: return int(p[0],16),int(p[4],16)
    return None

def text_section(path):
    for l in run([OBJDUMP,'-h',path]).stdout.split('\n'):
        p=l.split()
        if len(p)>5 and p[1]=='.text': return int(p[3],16),int(p[4],16),int(p[5],16)
    raise SystemExit('no .text')

def text_bytes(path,addr,size):
    d=open(path,'rb').read(); sz,vma,off=text_section(path)
    return d[off+addr-vma:off+addr-vma+size]

def disasm(path,addr,size):
    out=run([OBJDUMP,'-d','--no-show-raw-insn','--start-address=%d'%addr,'--stop-address=%d'%(addr+size),path]).stdout
    rows=[]
    for ln in out.split('\n'):
        m=re.match(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$',ln)
        if m: rows.append((int(m.group(1),16),m.group(2),m.group(3).strip()))
    return rows

def measure(so,mangle):
    a,b=sym(ORIG,mangle),sym(so,mangle)
    if not a or not b: return None
    x,y=text_bytes(ORIG,*a),text_bytes(so,*b); n=min(len(x),len(y))
    raw=sum(p!=q for p,q in zip(x[:n],y[:n]))
    oi=[(r[0],r[1]+' '+r[2]) for r in disasm(ORIG,*a)]
    ui=[(r[0],r[1]+' '+r[2]) for r in disasm(so,*b)]
    oidx={ad:i for i,(ad,_) in enumerate(oi)}; uidx={ad:i for i,(ad,_) in enumerate(ui)}
    def norm(t,idx):
        def rep(m):
            ad=int(m.group(1),16); return 'I%d'%idx[ad] if ad in idx else 'X'
        t=re.sub(r'\b([0-9a-f]{4,8})\b',rep,t); t=re.sub(r'<[^>]*>','',t)
        return re.sub(r'\s+',' ',t).strip()
    ot=[norm(t,oidx) for _,t in oi]; ut=[norm(t,uidx) for _,t in ui]
    sm=difflib.SequenceMatcher(None,ot,ut,autojunk=False)
    aligned=sum(max(i2-i1,j2-j1) for tag,i1,i2,j1,j2 in sm.get_opcodes() if tag!='equal')
    return dict(raw=raw,aligned=aligned,size=len(y),orig_size=len(x))

def verify_so(so,tag):
    bd=os.path.join(SCRATCH,'verify_'+tag); os.makedirs(bd,exist_ok=True)
    run([sys.executable,VERIFY,'--build-dir',bd,'--original',ORIG,'--ours',so,'--objdump',OBJDUMP])
    r=json.load(open(os.path.join(bd,'report.json')))['summary']
    return r

def obj_text_sections(path):
    d=open(path,'rb').read()
    e_shoff,=struct.unpack_from('<I',d,0x20)
    e_shentsize,e_shnum,e_shstrndx=struct.unpack_from('<HHH',d,0x2e)
    secs=[]
    for i in range(e_shnum):
        off=e_shoff+i*e_shentsize
        name,typ,flags,addr,offset,size=struct.unpack_from('<IIIIII',d,off)
        secs.append((name,typ,offset,size))
    shstr=secs[e_shstrndx]; st=d[shstr[2]:shstr[2]+shstr[3]]
    out={}
    for s in secs:
        end=st.find(b'\0',s[0]); n=st[s[0]:end].decode('latin1')
        if s[1]!=8 and n: out[n]=d[s[2]:s[2]+s[3]]
    return out

class Prof:
    def __init__(self,tu,mangle):
        self.tu=tu; self.mangle=mangle
        self.cmd=compile_cmd(tu)
        ir=os.path.join(SCRATCH,os.path.basename(tu)+'.instr.ll')
        instrument_ir(self.cmd,ir)
        self.meta=parse_meta(ir,mangle)
        self.meta['cmd']=self.cmd
    def build(self,vals,tag,extra=()):
        txt=os.path.join(SCRATCH,tag+'.proftext'); dat=os.path.join(SCRATCH,tag+'.profdata')
        write_profile(self.mangle,self.meta['fhash'],self.meta['ncounters'],vals,txt)
        merge(txt,dat)
        obj=os.path.join(SCRATCH,tag+'.obj')
        compile_use(self.cmd,dat,obj,PREFIX_OFF+list(extra))
        so=os.path.join(SCRATCH,tag+'.so')
        link_with([(obj_path(self.tu),obj)],so)
        m=measure(so,self.mangle)
        canon=obj_text_sections(obj_path(self.tu)); b=obj_text_sections(obj)
        others=[n for n in canon if n.startswith('.text.') and self.mangle not in n]
        m['other_same']=sum(1 for n in others if n in b and b[n]==canon[n])
        m['other_total']=len(others)
        m['so']=so
        return m

if __name__=='__main__':
    # smoke: uniform profile is a no-op
    tu,mangle=sys.argv[1],sys.argv[2]
    p=Prof(tu,mangle)
    print('fhash',p.meta['fhash'],'ncounters',p.meta['ncounters'])
    vals=[1]*p.meta['ncounters']
    m=p.build(vals,'uniform')
    print(m)
