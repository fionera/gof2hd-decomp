#!/usr/bin/env python3
# flat.py ORDERFILE [overrides.json] -> flat ternary Layout::Layout body
import json,struct,sys,re
S='/tmp/claude-1000/-home-debian-GalaxyOnFire2/58744cff-beee-4dd7-9dbb-ba064b0911f1/scratchpad/w4e'
d=json.load(open(S+'/lc/lay_orig.json'))
F=['iPadHD','n9','iPad','iPadLarge','retinaDisplay']
combos=sorted(d)
names={}
for l in open(S+'/layout_rec.txt'):
    m=re.match(r'\s*(\d+) \|\s+(.*)\s(\S+)$',l.rstrip())
    if m:
        off=int(m.group(1)); nm=m.group(3)
        if off not in names and not nm.startswith('_pad') and nm not in ('Layout',): names[off]=nm
names[0x238]='field_0x238_blk16.a'; names[0x23c]='field_0x238_blk16.b'; names[0x240]='field_0x238_blk16.c'; names[0x244]='field_0x238_blk16.d'
FLOATS={0x90,0xdc,0xe0,0xe4,0xe8,0x1e0,0x2f8}
BYTES={0x284,0x285,0x286}
def val(c,w,n):
    st=d[c]
    if not all(str(w+j) in st for j in range(n)): return None
    return sum(st[str(w+j)]<<(8*j) for j in range(n))
def lit(w,v):
    if v is None: return None
    if w in FLOATS:
        f=struct.unpack('<f',struct.pack('<I',v))[0]
        s=repr(f)
        # shortest float repr
        for p in range(1,10):
            t='%.*f'%(p,f) if abs(f)>=1e-3 else '%.*g'%(p,f)
            t=t.rstrip('0') if '.' in t else t
            if t.endswith('.'): t+='0'
            if struct.unpack('<f',struct.pack('<f',float(t)))[0]==f: s=t; break
        if 'e' not in s and '.' not in s: s+='.0'
        return s+'f'
    if w in BYTES: return str(v)
    return str(struct.unpack('<i',struct.pack('<I',v))[0])
DEF=['iPadHD','n9','iPadLarge','retinaDisplay','iPad']
def f32(x): return struct.unpack('<f',struct.pack('<f',x))[0]
def asnum(w,x):
    if w in FLOATS: return struct.unpack('<f',struct.pack('<I',x))[0]
    return struct.unpack('<i',struct.pack('<I',x))[0]
def hdarm(w,n,cs):
    global HDF
    hv=val(cs[0],w,n)
    if not HDF or w in BYTES or hv is None: return lit(w,hv)
    ip=val('00100',w,n)
    if ip is None: return lit(w,hv)
    a=asnum(w,ip); h=asnum(w,hv)
    ok = (f32(a*1.40625)==h) if w in FLOATS else (int(f32(a*1.40625))==h)
    if ok:
        l=lit(w,ip)
        if l in ('0','0.0f'): return '0.0f'
        if l=='1.0f': return '1.40625f'
        return '%s * 1.40625f'%l
    if w in FLOATS: return lit(w,hv)
    return '%d.0f'%h
def tree(cs,w,n,vs):
    if vs and vs[0]=='iPadHD' and len({val(c,w,n) for c in cs})>1:
        a=hdarm(w,n,[c for c in cs if c[0]=='1'])
        b=tree([c for c in cs if c[0]=='0'],w,n,vs[1:])
        return 'Globals::iPadHD ? %s : %s'%(a,b if '?' not in b else '('+b+')')
    vals={val(c,w,n) for c in cs}
    if len(vals)==1: return lit(w,vals.pop())
    v=vs[0]; i=F.index(v)
    a=tree([c for c in cs if c[i]=='1'],w,n,vs[1:])
    b=tree([c for c in cs if c[i]=='0'],w,n,vs[1:])
    if a==b: return a
    return ('Globals::%s ? %s : %s' if b and not '?' in b else 'Globals::%s ? %s : (%s)')%(v,a if '?' not in a else '('+a+')',b)
ov=json.load(open(sys.argv[2])) if len(sys.argv)>2 else {}
CH=json.load(open(S+'/chains.json'))
LONG={'H':'iPadHD','N':'n9','P':'iPad','L':'iPadLarge','R':'retinaDisplay'}
PREF=__import__('os').environ.get('PREF','HNLRP')
def kd(a,b):
    return sum(1 for i in range(5) for j in range(i+1,5) if b.index(a[i])>b.index(a[j]))
def bestorder(k):
    oks=CH.get(k,[])
    if not oks: return DEF
    o=min(oks,key=lambda x:(kd(x,PREF),x))
    return [LONG[c] for c in o]
order=[int(x,16) for x in open(sys.argv[1]).read().split()]
out=[]
TAILSTART=int(__import__('os').environ.get('TAIL','1e8'),16)
CHO=json.load(open(sys.argv[3])) if len(sys.argv)>3 else {}
intail=False
for w in order:
    if w==TAILSTART: intail=True
    n=1 if w in BYTES else 4
    k='%x'%w
    c=CHO.get(k,{})
    HDF=(c.get('hd','i' if intail else 'f')=='f')
    if k in ov and ov[k].startswith('='):
        e=ov[k]
        if HDF and 'HDF' in ov.get(k+'_f',''): e=ov[k+'_f']
        out.append('    this->%s %s;'%(names[w],e)); continue
    if 'o' in c: vs=[LONG[x] for x in c['o']]
    else: vs=ov.get(k,bestorder(k))
    if isinstance(vs,str): vs=vs.split(',')
    e=tree(combos,w,n,vs)
    if e is None or 'None' in e:
        out.append('    // TODO partial %x'%w); continue
    out.append('    this->%s = %s;'%(names[w],e))
print('Layout::Layout() {\n'+'\n'.join(out)+'\n}')
