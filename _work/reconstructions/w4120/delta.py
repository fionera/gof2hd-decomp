import subprocess,sys
NM='/home/debian/GalaxyOnFire2/wt/w4120/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
def load(p):
    d={}
    out=subprocess.run([NM,'-nD','--defined-only',p],capture_output=True,text=True).stdout
    for ln in out.splitlines():
        q=ln.split(maxsplit=2)
        if len(q)==3 and q[1] in 'tTwW': d[q[2]]=int(q[0],16)
    return d
a=load('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'); b=load(sys.argv[1])
common=set(a)&set(b)
seq=sorted(common,key=lambda n:a[n])
last=None
for n in seq:
    d=b[n]-a[n]
    if d!=last:
        print(f'{a[n]:#08x} delta {d:+#x}  {n}')
        last=d
