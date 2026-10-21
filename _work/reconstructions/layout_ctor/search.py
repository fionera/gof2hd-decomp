import subprocess,random,sys,os
S='/tmp/claude-1000/-home-debian-GalaxyOnFire2/58744cff-beee-4dd7-9dbb-ba064b0911f1/scratchpad'
W='/home/debian/GalaxyOnFire2/wt/w4c-search'; R='/home/debian/GalaxyOnFire2/wt/w4c/_work/reconstructions/layout_ctor/'
style=sys.argv[1]; fa=sys.argv[2]; fb=sys.argv[3]
OA=S+'/emu/os_a.txt'; OB=S+'/emu/os_b.txt'
def ev(a,b):
    open(OA,'w').write(' '.join('%x'%x for x in a)); open(OB,'w').write(' '.join('%x'%x for x in b))
    cmd='python3 {R}gen.py {R}lay_orig.json {S}/w4c/lay_layout.txt {OA} {st} {R}hd_region.txt {OB} > {S}/emu/ctor_s.cpp && python3 {S}/emu/install_s.py {S}/emu/ctor_s.cpp'.format(R=R,S=S,OA=OA,OB=OB,st=style)
    subprocess.run(cmd,shell=True,check=True)
    r=subprocess.run('cmake --build cmake-build-match --target link_so 2>&1 | grep -E "error|FAILED"; python3 %s/emu/pct_s.py _ZN6LayoutC2Ev'%S,shell=True,cwd=W,capture_output=True,text=True).stdout
    try: return float(r.split()[0])
    except: return -1
A=[int(x,16) for x in open(fa).read().split()]; B=[int(x,16) for x in open(fb).read().split()]
best=ev(A,B); print('start',best,flush=True)
random.seed(int(sys.argv[4]) if len(sys.argv)>4 else 1)
it=0
while True:
    it+=1
    which=random.random()<0.5
    o=list(A if which else B)
    i=random.randrange(len(o)); k=random.choice([-3,-2,-1,1,2,3,-6,6,-12,12,-24,24])
    j=max(0,min(len(o)-1,i+k)); x=o.pop(i); o.insert(j,x)
    s=ev(o,B) if which else ev(A,o)
    if s>=best:
        if s>best: print(it,'improve',s,'AB'[0 if which else 1],'%x %d'%(x,k),flush=True)
        best=s
        if which: A=o
        else: B=o
        open(fa,'w').write(' '.join('%x'%x for x in A)); open(fb,'w').write(' '.join('%x'%x for x in B))
    elif it%100==0: print(it,best,flush=True)
