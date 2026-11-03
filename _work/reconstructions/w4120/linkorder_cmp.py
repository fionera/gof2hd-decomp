import struct,subprocess,os
NM='/home/debian/GalaxyOnFire2/wt/w4120/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
def nm(p):
    d={}; out=subprocess.run([NM,'-nD','--defined-only',p],capture_output=True,text=True).stdout
    for ln in out.splitlines():
        q=ln.split(maxsplit=2)
        if len(q)==3 and q[1] in 'tTwW': d[q[2]]=int(q[0],16)
    return d
def dynsym_index(p):
    d=open(p,'rb').read(); shoff,=struct.unpack_from('<I',d,0x20); shentsize,shnum,shstrndx=struct.unpack_from('<HHH',d,0x2e)
    hdrs=[struct.unpack_from('<IIIIIIIIII',d,shoff+i*shentsize) for i in range(shnum)]; stroff=hdrs[shstrndx][4]
    # find .dynsym
    for h in hdrs:
        name,typ,flags,addr,off,size,link,info,align,ent=h
        e=d.index(b'\0',stroff+name); n=d[stroff+name:e].decode()
        if n=='.dynsym':
            st=hdrs[link][4]; out={}
            for i,o in enumerate(range(off,off+size,ent)):
                st_name,val,sz,sinfo,other,shndx=struct.unpack_from('<IIIBBH',d,o)
                if not st_name: continue
                e=d.index(b'\0',st+st_name); out.setdefault(d[st+st_name:e].decode(),i)
            return out
addr=nm(ORIG); idx=dynsym_index(ORIG)
objs=[l.strip() for l in open('/home/debian/GalaxyOnFire2/wt/w4120/cmake-build-match/link_order.rsp') if l.strip()]
rows=[]
for pos,o in enumerate(objs):
    out=subprocess.run([NM,'--defined-only','-g',o],capture_output=True,text=True).stdout
    syms=[p for p in (ln.split() for ln in out.splitlines()) if len(p)==3]
    strong=[addr[p[2]] for p in syms if p[1]=='T' and p[2] in addr]
    first=min((idx[p[2]] for p in syms if p[2] in idx),default=None)
    rows.append((pos,min(strong) if strong else None,first,o))
# current order = link_order.rsp order; compare to min-addr and first-mention
cur=[r[3] for r in rows]
byaddr=[r[3] for r in sorted(rows,key=lambda r:(r[1] is None,r[1] or 0,r[0]))]
bydyn=[r[3] for r in sorted(rows,key=lambda r:(r[2] is None,r[2] or 0,r[0]))]
print('current==byaddr',cur==byaddr,'current==bydyn',cur==bydyn)
for i,(a,b) in enumerate(zip(cur,bydyn)):
    if a!=b:
        print('first diff at',i,os.path.basename(a),'vs',os.path.basename(b)); break
