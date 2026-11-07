import re,sys,struct
sys.path.insert(0,'/home/debian/GalaxyOnFire2/wt/w7727/tools/verify')
from elf import Elf32
OBJDUMP='/home/debian/GalaxyOnFire2/wt/w7727/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
def dis(path,start,stop):
    out=subprocess.check_output([OBJDUMP,'-d','--start-address=%#x'%start,'--stop-address=%#x'%stop,path],text=True)
    ins=[]
    for line in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f ]+)\t(\S+)\s*(.*)',line)
        if m: ins.append((int(m.group(1),16),m.group(3),m.group(4)))
    return ins
import subprocess
def analyze(path,name):
    e=Elf32(path); f=e.functions()[name]
    ins=dis(path,f['value'],f['value']+f['size'])
    res=[]
    for i,(addr,op,args) in enumerate(ins):
        if op in ('tbh','tbb'):
            base=addr+4
            # find preceding cmp to get range: search back for 'cmp rX, #N' where reg matches
            m=re.search(r'\[pc,\s*(\w+)',args)
            if not m:
                print('NOMATCH',hex(addr),repr(op),repr(args)); continue
            reg=m.group(1)
            n=None
            for j in range(i-1,max(0,i-6),-1):
                a2,o2,ar2=ins[j]
                mm=re.match(r'cmp\s+%s,\s*#(\d+)'%reg,o2+' '+ar2)
                if mm: n=int(mm.group(1))+1; break
            if n is None: n=16
            ents=[]
            for k in range(n):
                b=e.bytes_at(base+2*k,2); v=struct.unpack('<H',b)[0]; ents.append(base+2*v)
            # pattern: default = most common target
            from collections import Counter
            c=Counter(ents); dflt=c.most_common(1)[0][0]
            pat=''.join('.' if t==dflt else chr(ord('A')+ (sorted(set(ents)-{dflt}).index(t))) for t in ents)
            res.append((addr,base,n,pat,ents,dflt))
    return res
for path,label in [(sys.argv[1],'ORIG'),(sys.argv[2],'OURS')]:
    print('====',label)
    for addr,base,n,pat,ents,dflt in analyze(path,'_ZN11LevelScript7processEi'):
        print('tbh %08x n=%2d pat=%s'%(addr,n,pat))
