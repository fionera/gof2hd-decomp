import subprocess,sys
NM='/home/debian/GalaxyOnFire2/wt/w4120/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
def load(p):
    d={}
    out=subprocess.run([NM,'-nD','--defined-only',p],capture_output=True,text=True).stdout
    for ln in out.splitlines():
        q=ln.split(maxsplit=2)
        if len(q)==3 and q[1] in 'tTwW': d[q[2]]=(int(q[0],16),q[1])
    return d
a=load('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
b=load(sys.argv[1])
common=set(a)&set(b)
print('common',len(common),'orig',len(a),'ours',len(b))
same=[n for n in common if a[n][0]==b[n][0]]
print('same addr',len(same),'of',len(common))
# order comparison
oa=sorted(common,key=lambda n:a[n][0]); ob=sorted(common,key=lambda n:b[n][0])
print('same order sequence', oa==ob)
# print first 20 mismatched by original addr
mis=sorted([n for n in common if a[n][0]!=b[n][0]],key=lambda n:a[n][0])
print('first mismatches:')
for n in mis[:30]:
    print(f'  orig {a[n][0]:#08x} ours {b[n][0]:#08x} {n}')
# same-addr by object? 
