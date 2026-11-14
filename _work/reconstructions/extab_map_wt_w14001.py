#!/usr/bin/env python3
"""Per-function .ARM.extab (LSDA) record comparison: original vs ours."""
import re, struct, subprocess, sys, bisect

ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = '/home/debian/GalaxyOnFire2/wt/w14001/cmake-build-match/libgof2hdaa.so'

def sections(path):
    out = subprocess.run(['readelf','-SW',path],capture_output=True,text=True).stdout
    secs={}
    for line in out.splitlines():
        m=re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)',line)
        if m:
            secs[m.group(1)]={'addr':int(m.group(3),16),'off':int(m.group(4),16),'size':int(m.group(5),16)}
    return secs

def funcs(path):
    out = subprocess.run(['readelf','-sW','--dyn-syms',path],capture_output=True,text=True).stdout
    m={}
    for line in out.splitlines():
        p=line.split()
        if len(p)>=8 and p[3]=='FUNC':
            try: a=int(p[1],16)
            except ValueError: continue
            if a: m.setdefault(a & ~1, p[7])
    return m

def prel31(v, base):
    v &= 0x7fffffff
    if v & 0x40000000: v -= 0x80000000
    return base + v

def parse(path):
    secs=sections(path); data=open(path,'rb').read()
    ex=secs['.ARM.exidx']; et=secs['.ARM.extab']
    n=ex['size']//8
    recs=[]
    for i in range(n):
        o=ex['off']+i*8
        w0,w1=struct.unpack_from('<II',data,o)
        fa=prel31(w0,ex['addr']+i*8)
        if w1==1: recs.append((fa,None)); continue
        ea=prel31(w1,ex['addr']+i*8+4)
        recs.append((fa,ea))
    offs=sorted({e for _,e in recs if e is not None})
    return secs,data,recs,offs

so,do,ro,oo=parse(ORIG)
sn,dn,rn,on=parse(OURS)
print('exidx entries',len(ro),len(rn),'extab offs',len(oo),len(on),'same set',oo==on)
fn=funcs(OURS); fo=funcs(ORIG)

# record size from offset list
def size_at(offs,e):
    i=bisect.bisect_left(offs,e)
    if i+1<len(offs): return offs[i+1]-e
    return None

diffs=[]
for fa,ea in rn:
    if ea is None: continue
    sz=size_at(on,ea)
    if sz is None: continue
    oa=so['.ARM.extab']['off']+(ea-so['.ARM.extab']['addr'])
    ob=sn['.ARM.extab']['off']+(ea-sn['.ARM.extab']['addr'])
    a=do[oa:oa+sz]
    b=dn[ob:ob+sz]
    nd=sum(1 for x,y in zip(a,b) if x!=y)
    if nd: diffs.append((nd,fa,ea,sz,fn.get(fa & ~1,'?')))
diffs.sort(reverse=True)
print('records with differing bytes:',len(diffs),'total differing bytes:',sum(d[0] for d in diffs))
for nd,fa,ea,sz,name in diffs[:25]:
    print(f'  {nd:4d}/{sz:4d} B  fn=0x{fa:06x} extab=0x{ea:06x}  {name}')
