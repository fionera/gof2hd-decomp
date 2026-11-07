import re,sys,subprocess,struct
OBJDUMP='/home/debian/GalaxyOnFire2/wt/w7727/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
def dis(path,start,stop):
    out=subprocess.check_output([OBJDUMP,'-d','--start-address=%#x'%start,'--stop-address=%#x'%stop,path],text=True)
    return out
def parse(out):
    insns=[]
    for line in out.splitlines():
        m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f ]+)\s+(\S+)\s*(.*)',line)
        if m:
            addr=int(m.group(1),16); raw=m.group(2).strip(); op=m.group(3); args=m.group(4)
            insns.append((addr,raw,op,args))
    return insns
if __name__=='__main__':
    out=dis(sys.argv[1],int(sys.argv[2],0),int(sys.argv[3],0))
    for i in parse(out):
        print('%08x %-10s %-8s %s'%i)
