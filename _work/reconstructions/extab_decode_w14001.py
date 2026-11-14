#!/usr/bin/env python3
"""Correct ARM EHABI LSDA decoder + per-record diff."""
import re, struct, subprocess, bisect, sys

ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = '/home/debian/GalaxyOnFire2/wt/w14001/cmake-build-match/libgof2hdaa.so'

def sections(path):
    out = subprocess.run(['readelf','-SW',path],capture_output=True,text=True).stdout
    secs={}
    for line in out.splitlines():
        m=re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)',line)
        if m: secs[m.group(1)]={'addr':int(m.group(3),16),'off':int(m.group(4),16),'size':int(m.group(5),16)}
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
    ex=secs['.ARM.exidx']; n=ex['size']//8
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

def uleb(b,p):
    r=0;s=0
    while True:
        x=b[p];p+=1;r|=(x&0x7f)<<s;s+=7
        if not x&0x80: return r,p

def readenc(b,p,fmt):
    f=fmt&0x0f
    if f==0x00: return struct.unpack_from('<I',b,p)[0],p+4
    if f==0x01: return uleb(b,p)
    if f==0x02: return struct.unpack_from('<H',b,p)[0],p+2
    if f==0x03: return struct.unpack_from('<I',b,p)[0],p+4
    if f==0x04: return struct.unpack_from('<Q',b,p)[0],p+8
    if f==0x09: p=(p+3)&~3; return struct.unpack_from('<I',b,p)[0],p+4
    if f==0x0a: p=(p+3)&~3; return struct.unpack_from('<I',b,p)[0],p+4
    if f==0x0b: p=(p+7)&~7; return struct.unpack_from('<Q',b,p)[0],p+8
    raise ValueError('fmt %x'%f)

def decode(data, secaddr, off, size):
    rec=data[off:off+size]; vaddr=secaddr+off
    w0=struct.unpack_from('<I',rec,0)[0]
    d={}
    if w0 & 0x80000000:  # compact model
        d['model']='compact'; d['pers']=(w0>>24)&0x0f; lsda_off=4
    else:
        d['model']='generic'; d['pers']=prel31(w0,vaddr)
        w1=struct.unpack_from('<I',rec,4)[0]
        nwords=(w1>>24)&0xff
        d['unwind_words']=nwords
        lsda_off=8+4*nwords
    d['lsda_off']=lsda_off
    p=lsda_off
    d['lp_enc']=rec[p]; p+=1
    if d['lp_enc']!=0xff:
        _,p=readenc(rec,p,d['lp_enc'])
    d['tt_enc']=rec[p]; p+=1
    if d['tt_enc']!=0xff:
        tt_off,p=uleb(rec,p); d['tt_off']=tt_off
    d['cs_enc']=rec[p]; p+=1
    cs_len,p=uleb(rec,p); d['cs_len']=cs_len
    cs_start=p; cs_end=p+cs_len
    trips=[]
    while p<cs_end:
        st,p=readenc(rec,p,d['cs_enc'])
        ln,p=readenc(rec,p,d['cs_enc'])
        lpv,p=readenc(rec,p,d['cs_enc'])
        act,p=uleb(rec,p)
        trips.append((st,ln,lpv,act))
    d['trips']=trips
    d['action_off']=p
    d['action_bytes']=bytes(rec[p:]) if d['tt_enc']==0xff else None
    return d,rec

def main():
    so,do,ro,oo=parse(ORIG); sn,dn,rn,on=parse(OURS)
    fno=funcs(ORIG); fnn=funcs(OURS)
    def size_at(offs,e):
        i=bisect.bisect_left(offs,e)
        return offs[i+1]-e if i+1<len(offs) else None
    diffs=[]
    for fa,ea in rn:
        if ea is None: continue
        sz=size_at(on,ea)
        if sz is None: continue
        oa=so['.ARM.extab']['off']+(ea-so['.ARM.extab']['addr'])
        ob=sn['.ARM.extab']['off']+(ea-sn['.ARM.extab']['addr'])
        nd=sum(1 for x,y in zip(do[oa:oa+sz],dn[ob:ob+sz]) if x!=y)
        if nd: diffs.append((nd,fa,ea,sz))
    diffs.sort(reverse=True)
    print('records=%d totaldiff=%d'%(len(diffs),sum(d[0] for d in diffs)))
    for nd,fa,ea,sz in diffs:
        name=fnn.get(fa&~1,'?')
        oa=so['.ARM.extab']['off']+(ea-so['.ARM.extab']['addr'])
        ob=sn['.ARM.extab']['off']+(ea-sn['.ARM.extab']['addr'])
        d1,r1=decode(do,so['.ARM.extab']['addr'],oa,sz)
        d2,r2=decode(dn,sn['.ARM.extab']['addr'],ob,sz)
        print('\n=== fn=0x%06x extab=0x%06x size=%d diff=%d %s'%(fa,ea,sz,nd,name))
        for k in ('model','pers','unwind_words','lsda_off','lp_enc','tt_enc','cs_enc','cs_len'):
            if k in d1 or k in d2:
                v1,v2=d1.get(k),d2.get(k)
                print('  %-12s orig=%s ours=%s%s'%(k,hex(v1) if isinstance(v1,int) else v1,hex(v2) if isinstance(v2,int) else v2,'' if v1==v2 else '  <<<DIFF'))
        t1,t2=d1['trips'],d2['trips']
        print('  trips orig=%d ours=%d'%(len(t1),len(t2)))
        ndt=0
        for i in range(min(len(t1),len(t2))):
            if t1[i]!=t2[i]:
                ndt+=1
                if ndt<=20: print('    trip %3d orig=%s ours=%s'%(i,t1[i],t2[i]))
        if ndt>20: print('    ... %d differing trips'%ndt)
        a1,a2=d1['action_bytes'],d2['action_bytes']
        if a1 is not None and a2 is not None:
            print('  action bytes orig=%s'%(a1.hex() if a1 else '(none)'))
            print('  action bytes ours=%s'%(a2.hex() if a2 else '(none)'))
        pos=[i for i in range(sz) if r1[i]!=r2[i]]
        print('  diff record offsets: %s'%pos)

main()
