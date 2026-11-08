import subprocess, re, sys, collections, hashlib
ORIG=sys.argv[1]
RE='/home/debian/GalaxyOnFire2/wt/w10118/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf'
OD='/home/debian/GalaxyOnFire2/wt/w10118/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'
out=subprocess.check_output([RE,'-sW',ORIG],text=True)
syms=[]
for l in out.splitlines():
    p=l.split()
    if len(p)>=8 and p[3]=='FUNC' and p[6]!='UND':
        try: addr=int(p[1],16); size=int(p[2],0)
        except: continue
        syms.append((addr,size,p[7]))
byaddr=collections.defaultdict(list)
for a,s,n in syms: byaddr[a].append((s,n))
# groups of >1 symbol at same address, excluding pure C1/C2 and D1/D2 alias pairs
def alias_pair(names):
    if len(names)!=2: return False
    a,b=sorted(names)
    # C1/C2 same rest, D1/D2 same rest
    for x,y in (('C1','C2'),('D1','D2')):
        if x in a and y in b:
            return a.replace(x,y)==b
    return False
groups=[(a,v) for a,v in byaddr.items() if len(v)>1]
nonalias=[(a,v) for a,v in groups if not alias_pair([n for s,n in v])]
print('FUNC symbols:',len(syms),'distinct addresses:',len(byaddr))
print('multi-symbol addresses:',len(groups),'non-C1C2/D1D2 groups:',len(nonalias))
for a,v in nonalias[:40]:
    print(hex(a), v)
# body hashes at distinct addresses
# get .text bytes
sec=subprocess.check_output([RE,'-SW',ORIG],text=True)
m=re.search(r'\[\s*\d+\]\s+\.text\s+PROGBITS\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)',sec)
base=int(m.group(1),16); off=int(m.group(2),16); size=int(m.group(3),16)
data=open(ORIG,'rb').read()[off:off+size]
bodies=collections.defaultdict(list)
for a,s,n in syms:
    if s==0: continue
    if base<=a and a+s<=base+size:
        bodies[data[a-base:a-base+s]].append((a,s,n))
dups={h:v for h,v in bodies.items() if len(v)>1}
# exclude alias groups: same address
realdups={h:v for h,v in dups.items() if len({a for a,s,n in v})>1}
print('distinct-address identical-body groups:',len(realdups))
tot=0
for h,v in sorted(realdups.items(), key=lambda kv:-len(kv[1]))[:20]:
    tot+=len(v)
    print('size',v[0][1],'count',len(v),'addrs',[hex(a) for a,s,n in v[:5]],'names',[n for a,s,n in v[:3]])
print('total funcs in distinct-address dup groups:',sum(len(v) for v in realdups.values()))
