import subprocess, sys, struct
OBJ='/home/debian/GalaxyOnFire2/gof2-decomp/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS='/home/debian/GalaxyOnFire2/gof2-decomp/cmake-build-match/libgof2hdaa.so'
def sym(path,name):
    out=subprocess.check_output([OBJ,'-T',path],text=True)
    for l in out.splitlines():
        p=l.split()
        if len(p)>=6 and p[-1]==name:
            return int(p[0],16), int(p[4],16)
    return None
def textsec(path):
    out=subprocess.check_output([OBJ,'-h',path],text=True)
    lines=out.splitlines()
    for i,l in enumerate(lines):
        if l.split() and l.split()[1:2]==['.text']:
            p=l.split()
            # idx name size vma lma fileoff algn
            return int(p[3],16), int(p[4],16), int(p[5],16)
    raise SystemExit('no .text')
def data(path,addr,size):
    f=open(path,'rb').read()
    sz,vma,off=textsec(path)
    return f[off+(addr-vma):off+(addr-vma)+size]
name=sys.argv[1]
a=sym(ORIG,name); b=sym(OURS,name)
if not a or not b:
    print('missing',a,b); sys.exit(1)
x=data(ORIG,a[0],a[1]); y=data(OURS,b[0],b[1])
n=min(len(x),len(y))
print(name,'orig',a,'ours',b,'diff',sum(p!=q for p,q in zip(x[:n],y[:n])),'sizes',len(x),len(y))
