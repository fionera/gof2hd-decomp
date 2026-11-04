#!/usr/bin/env python3
import subprocess,re,sys,struct,hashlib
def readelf(args,path): return subprocess.check_output(['readelf']+args+[path],text=True,errors='replace')
def sections(path):
    out=readelf(['-SW'],path); d={}; order=[]
    for l in out.splitlines():
        m=re.match(r'\s*\[\s*(\d+)\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+(\S*)\s+(\d+)\s+(\d+)\s+(\d+)',l)
        if m:
            d[m.group(2)]=dict(idx=int(m.group(1)),type=m.group(3),addr=int(m.group(4),16),off=int(m.group(5),16),size=int(m.group(6),16),es=int(m.group(7),16),fl=m.group(8),lk=int(m.group(9)),inf=int(m.group(10)),al=int(m.group(11)))
            order.append(m.group(2))
    return d,order
def phdrs(path):
    out=readelf(['-lW'],path); rows=[]
    for l in out.splitlines():
        m=re.match(r'\s*(LOAD|DYNAMIC|NOTE|GNU_STACK|GNU_RELRO|ARM_EXIDX|PHDR)\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+)\s+(\S+)\s+(0x[0-9a-f]+)',l)
        if m: rows.append(m.groups())
    return rows
def dynamic(path):
    out=readelf(['-dW'],path); rows=[]
    for l in out.splitlines():
        m=re.match(r'\s*0x([0-9a-f]+)\s+\(([^)]+)\)\s*(.*)',l)
        if m: rows.append((m.group(2),m.group(3).strip()))
    return rows
def notes(path):
    out=readelf(['-nW'],path); rows=[]
    for l in out.splitlines():
        if 'Android' in l or 'GNU' in l or 'Owner' in l: rows.append(l.strip())
    return rows
def main():
    O=sys.argv[1]; M=sys.argv[2]
    so,soo=sections(O); sm,smo=sections(M)
    od=open(O,'rb').read(); md=open(M,'rb').read()
    print('file size',len(od),len(md),'sha',hashlib.sha256(od).hexdigest()[:16],hashlib.sha256(md).hexdigest()[:16])
    print('section count',len(soo),len(smo))
    print('%-22s %10s %10s %10s %10s %8s %8s'%('section','orig_size','our_size','orig_addr','our_addr','diffB','hdr'))
    for n in soo:
        a=so[n]; b=sm.get(n)
        if not b: print('%-22s missing'%n); continue
        diff=''
        if a['size']==b['size']:
            da=od[a['off']:a['off']+a['size']]; db=md[b['off']:b['off']+b['size']]
            diff=sum(x!=y for x,y in zip(da,db))
        else: diff='size!'
        hdr='same' if all(a[k]==b[k] for k in ['type','addr','size','es','fl','lk','inf','al']) else 'DIFF'
        print('%-22s %10x %10x %10x %10x %8s %8s'%(n,a['size'],b['size'],a['addr'],b['addr'],diff,hdr))
    for n in smo:
        if n not in so: print('%-22s EXTRA size %x addr %x'%(n,sm[n]['size'],sm[n]['addr']))
    print('\nphdrs orig:'); [print(' ',r) for r in phdrs(O)]
    print('phdrs ours:'); [print(' ',r) for r in phdrs(M)]
    print('\ndynamic orig:'); [print(' ',r) for r in dynamic(O)]
    print('dynamic ours:'); [print(' ',r) for r in dynamic(M)]
    print('\nnotes orig:'); [print(' ',r) for r in notes(O)]
    print('notes ours:'); [print(' ',r) for r in notes(M)]
if __name__=='__main__': main()
