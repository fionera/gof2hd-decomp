#!/usr/bin/env python3
import os,sys,re,json,subprocess
sys.path.insert(0,'/dev/shm/w10117')
import prof_common as C

def write_sample(path,mangled,start,lines,total=1000,head=0):
    with open(path,'w') as f:
        f.write('%s:%d:%d\n'%(mangled,total,head))
        for off,cnt in lines:
            f.write(' %d: %d\n'%(off,cnt))

def sample_build(tu,mangled,start,lines,extra=(),tag='s',obj=None):
    prof=os.path.join(C.SCRATCH,tag+'.prof')
    write_sample(prof,mangled,start,lines)
    o=C.compile_tu(tu,['-fprofile-sample-use='+prof]+list(extra),obj=obj)
    so=C.relink()
    return C.measure(so,mangled)

def parse_instr(ir,mangled):
    txt=open(ir).read()
    # find __profd_ symbol
    m=re.search(r'^@__profd_%s = .*$'%re.escape(mangled),txt,re.M)
    if not m:
        # static / local name variants
        m=re.search(r'^@__profd_[^=]*%s[^=]* = .*$'%re.escape(mangled),txt,re.M)
    if not m: raise SystemExit('no __profd for '+mangled)
    pl=m.group(0)
    ints=re.findall(r'i64 (-?\d+)',pl)
    fhash=int(ints[1]) if len(ints)>1 else int(ints[0])
    mm=re.search(r'i32 (\d+), \[2 x i16\]',pl)
    if not mm: mm=re.search(r'i32 (\d+), \[',pl)
    n=int(mm.group(1))
    return fhash,n

def write_proftext(path,mangled,fhash,n,vals):
    with open(path,'w') as f:
        f.write('%s\n# Func Hash:\n%d\n# Num Counters:\n%d\n# Counter Values:\n'%(mangled,fhash & ((1<<64)-1),n))
        for v in vals: f.write('%d\n'%v)

def instr_build(tu,mangled,vals=None,extra=(),tag='i',obj=None):
    ir=os.path.join(C.SCRATCH,tag+'.instr.ll')
    C.compile_tu(tu,['-fprofile-instr-generate','-mllvm','-enable-name-compression=false'],emit_ir=ir,obj=ir)
    fhash,n=parse_instr(ir,mangled)
    if vals is None: vals=[1]*n
    txt=os.path.join(C.SCRATCH,tag+'.proftext'); dat=os.path.join(C.SCRATCH,tag+'.profdata')
    write_proftext(txt,mangled,fhash,n,vals)
    C.run([C.PROFDATA,'merge',txt,'-o',dat])
    o=C.compile_tu(tu,['-fprofile-instr-use='+dat,'-mllvm','-profile-guided-section-prefix=false']+list(extra),obj=obj)
    so=C.relink()
    return C.measure(so,mangled),fhash,n

if __name__=='__main__':
    cmd=sys.argv[1]
    if cmd=='sample':
        tu,mg,start,span=sys.argv[2],sys.argv[3],int(sys.argv[4]),int(sys.argv[5])
        extra=[]
        if '--extra' in sys.argv:
            i=sys.argv.index('--extra'); extra=sys.argv[i+1:]; sys.argv=sys.argv[:i]
        lines=[(i,4) for i in range(span)]
        r=sample_build(tu,mg,start,lines,extra,tag='samp')
        print('sample all-lines raw=%d aligned=%d size=%d'%(r['raw'],r['aligned'],r['size']))
    elif cmd=='sample-lines':
        tu,mg,start=sys.argv[2],sys.argv[3],int(sys.argv[4])
        spec=sys.argv[5]
        extra=[]
        if '--extra' in sys.argv:
            i=sys.argv.index('--extra'); extra=sys.argv[i+1:]; sys.argv=sys.argv[:i]
        lines=[(int(a.split(':')[0]),int(a.split(':')[1])) for a in spec.split(',')]
        r=sample_build(tu,mg,start,lines,extra,tag='samp')
        print('sample lines raw=%d aligned=%d size=%d'%(r['raw'],r['aligned'],r['size']))
    elif cmd=='instr':
        tu,mg=sys.argv[2],sys.argv[3]
        extra=[]
        if '--extra' in sys.argv:
            i=sys.argv.index('--extra'); extra=sys.argv[i+1:]; sys.argv=sys.argv[:i]
        r,fh,n=instr_build(tu,mg,None,extra,tag='instr')
        print('instr uniform raw=%d aligned=%d size=%d hash=%d n=%d'%(r['raw'],r['aligned'],r['size'],fh,n))
