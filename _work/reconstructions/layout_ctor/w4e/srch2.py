#!/usr/bin/env python3
# srch2.py TAG ORDERFILE OVFILE CHOFILE SEED : hill-climb order + per-field choices in search worktree
import subprocess,random,sys,os,json
S='/tmp/claude-1000/-home-debian-GalaxyOnFire2/58744cff-beee-4dd7-9dbb-ba064b0911f1/scratchpad/w4e'
W=os.environ.get('SW','/home/debian/GalaxyOnFire2/wt/w4e-search')
tag,of,ov,chf,seed=sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],int(sys.argv[5])
random.seed(seed)
CH=json.load(open(S+'/chains.json'))
tmpo=S+'/s_%s.ord'%tag; tmpc=S+'/s_%s.cho'%tag; cpp=S+'/s_%s.cpp'%tag
base=W+'/src/game/ui/Layout.cpp'
T=open(S+'/Layout.base.cpp').read()
a=T.index('Layout::Layout() {'); b=T.index('\n}\n',a)+3
def ev(order,cho):
    open(tmpo,'w').write(' '.join('%x'%x for x in order)); json.dump(cho,open(tmpc,'w'))
    r=subprocess.run('python3 %s/flat.py %s %s %s > %s'%(S,tmpo,ov,tmpc,cpp),shell=True)
    open(base,'w').write(T[:a]+open(cpp).read().rstrip('\n')+'\n'+T[b:])
    r=subprocess.run('cmake --build cmake-build-match --target link_so 2>&1 | grep -c "error:"; WT=%s python3 %s/bin/pct.py _ZN6LayoutC2Ev'%(W,S),shell=True,cwd=W,capture_output=True,text=True).stdout.split()
    try:
        if r[0]!='0': return -1
        return float(r[1])
    except Exception: return -1
A=[int(x,16) for x in open(of).read().split()]
C=json.load(open(chf)) if os.path.exists(chf) else {}
best=ev(A,C); print('start',best,flush=True)
it=0
while True:
    it+=1
    o=list(A); c=json.loads(json.dumps(C)); m=random.random(); desc=''
    if m<0.6:
        i=random.randrange(len(o)); k=random.choice([-1,1,-2,2,-3,3,-5,5,-8,8,-13,13,-21,21])
        j=max(0,min(len(o)-1,i+k)); x=o.pop(i); o.insert(j,x); desc='mv %x %d'%(x,k)
    elif m<0.85:
        k=random.choice([kk for kk in CH if len(CH[kk])>1])
        c.setdefault(k,{})['o']=random.choice(CH[k]); desc='ord %s %s'%(k,c[k]['o'])
    else:
        k='%x'%random.choice(o)
        cur=c.get(k,{}).get('hd')
        c.setdefault(k,{})['hd']='i' if cur in (None,'f') and random.random()<0.5 else ('f' if cur!='f' else 'i'); desc='hd %s %s'%(k,c[k]['hd'])
    s=ev(o,c)
    if s>=best:
        if s>best: print(it,'improve',s,desc,flush=True)
        best=s; A=o; C=c
        open(of+'.best','w').write(' '.join('%x'%x for x in A)); json.dump(C,open(chf+'.best','w'))
    if it%50==0: print(it,'cur',best,flush=True)
