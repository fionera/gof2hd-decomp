import re,sys,struct,subprocess
sys.path.insert(0,'/home/debian/GalaxyOnFire2/wt/w7727/tools/verify')
from elf import Elf32
OBJDUMP='/home/debian/GalaxyOnFire2/wt/w7727/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'

def getfun(path,name):
    e=Elf32(path); f=e.functions()[name]; return e,f

def dis(path,start,stop):
    out=subprocess.check_output([OBJDUMP,'-d','--start-address=%#x'%start,'--stop-address=%#x'%stop,path],text=True)
    ins=[]
    for line in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f ]+)\t(\S+)\s*(.*)',line)
        if m:
            ins.append((int(m.group(1),16),m.group(2).strip(),m.group(3),m.group(4)))
    return ins

def pools(e,ins):
    out=[]
    for addr,raw,op,args in ins:
        m=re.match(r'(ldr|ldrd|vldr|ldrb|ldrh|ldrsb|ldrsh)(\.\w+)?\s+(\w+),\s*\[pc(?:,\s*#(-?\d+))?\]',op+' '+args)
        if m:
            imm=int(m.group(4) or 0)
            target=((addr+4)&~3)+imm
            b=e.bytes_at(target,4)
            val=struct.unpack('<I',b)[0] if len(b)==4 else None
            fv=struct.unpack('<f',b)[0] if len(b)==4 else None
            out.append((addr,op+' '+args,target,val,fv))
    return out

def tbhs(e,ins):
    out=[]
    for addr,raw,op,args in ins:
        if op in ('tbh','tbb'):
            base=(addr+4)&~3
            ents=[]
            for k in range(40):
                b=e.bytes_at(base+2*k,2)
                if len(b)<2: break
                v=struct.unpack('<H',b)[0]
                tgt=base+2*v
                ents.append((k,base+2*k,v,tgt))
            out.append((addr,args,base,ents))
    return out

if __name__=='__main__':
    path=sys.argv[1]; name=sys.argv[2]
    e,f=getfun(path,name)
    print('fun',name,hex(f['value']),f['size'])
    ins=dis(path,f['value'],f['value']+f['size'])
    print('insns',len(ins))
    print('=== TBH ===')
    for addr,args,base,ents in tbhs(e,ins):
        print('tbh @%x base=%x n=%d'%(addr,base,len(ents)))
        print('  ', ' '.join('%d:%x'%(k,tgt) for k,_,v,tgt in ents[:24]))
    print('=== POOLS ===')
    for addr,txt,target,val,fv in pools(e,ins):
        print('%08x %-28s -> %08x %08x %r'%(addr,txt,target,val,fv))
