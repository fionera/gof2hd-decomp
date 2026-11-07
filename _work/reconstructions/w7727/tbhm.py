import re,sys,struct,subprocess
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
def analyze(path,label):
    e=Elf32(path); f=e.functions()['_ZN11LevelScript7processEi']
    ins=dis(path,f['value'],f['value']+f['size'])
    print('====',label)
    for i,(addr,op,args) in enumerate(ins):
        if op in ('tbh','tbb'):
            m=re.search(r'\[pc, (\w+)',args); reg=m.group(1)
            n=None; cmpins=None
            for j in range(i-1,max(0,i-8),-1):
                a2,o2,ar2=ins[j]
                mm=re.match(r'cmp\s+%s,\s*#(\d+)'%reg,o2+' '+ar2)
                if mm: n=int(mm.group(1))+1; cmpins=(a2,o2,ar2); break
            ctx=' ; '.join('%s %s'%(o,a) for _,o,a in ins[max(0,i-5):i])
            print('tbh %08x n=%s cmp=%s'%(addr,n,cmpins))
            print('   ctx:',ctx)
analyze(sys.argv[1],'ORIG'); analyze(sys.argv[2],'OURS')
