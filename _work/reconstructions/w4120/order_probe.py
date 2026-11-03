import struct, subprocess, sys, os
from collections import defaultdict

ORIG='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
NM='/home/debian/GalaxyOnFire2/wt/w4120/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'

# original exported function addresses
addr={}
out=subprocess.run([NM,'-nD','--defined-only',ORIG],capture_output=True,text=True).stdout
for ln in out.splitlines():
    p=ln.split(maxsplit=2)
    if len(p)==3 and p[1] in 'tTwW':
        addr[p[2]]=int(p[0],16)

def symtab(path):
    d=open(path,'rb').read()
    shoff,=struct.unpack_from('<I',d,0x20)
    shentsize,shnum,shstrndx=struct.unpack_from('<HHH',d,0x2e)
    hdrs=[struct.unpack_from('<IIIIIIIIII',d,shoff+i*shentsize) for i in range(shnum)]
    stroff=hdrs[shstrndx][4]
    def name(off):
        e=d.index(b'\0',stroff+off); return d[stroff+off:e].decode()
    secnames=[name(h[0]) for h in hdrs]
    syms=[]
    for h in hdrs:
        nm,typ,flags,a,off,size,link,info,align,ent=h
        if typ!=2: continue
        ent=ent or 16
        st=hdrs[link][4]
        for o in range(off,off+size,ent):
            st_name,val,sz,sinfo,other,shndx=struct.unpack_from('<IIIBBH',d,o)
            if not st_name: continue
            e=d.index(b'\0',st+st_name); n=d[st+st_name:e].decode()
            if shndx==0: continue
            syms.append((n,sinfo&0xf,sinfo>>4,shndx,sz))
    return d,hdrs,secnames,syms

objs=[l.strip() for l in open('/home/debian/GalaxyOnFire2/wt/w4120/cmake-build-match/link_order.rsp') if l.strip()]
# simulate: for each object, function sections in section-header order
seen_weak=set()
seq=[]  # (name, obj)
per_obj=[]
for o in objs:
    d,hdrs,secnames,syms=symtab(o)
    # map section index -> sym name for STT_FUNC in .text.*
    funcs=[]
    for n,typ,bind,shndx,sz in syms:
        if typ==2 and shndx<len(secnames) and secnames[shndx].startswith('.text.'):
            funcs.append((hdrs[shndx][4],n,bind,sz))  # sh_offset as source order proxy
    funcs.sort()
    objseq=[]
    for off,n,bind,sz in funcs:
        if n not in addr: continue
        if bind==2:  # STB_WEAK
            if n in seen_weak: continue
            seen_weak.add(n)
        objseq.append(n)
    per_obj.append((o,objseq))
    seq.extend(objseq)

# original order of these symbols
origseq=[n for n in sorted(addr,key=lambda x:addr[x]) if n in set(seq)]
# compare adjacent pairs and exact position
pos={n:i for i,n in enumerate(seq)}
opos={n:i for i,n in enumerate(origseq)}
common=set(seq)&set(origseq)
same_pos=sum(1 for n in common if pos[n]==opos[n])
adj=sum(1 for i in range(len(seq)-1) if seq[i] in opos and seq[i+1] in opos and opos[seq[i+1]]==opos[seq[i]]+1)
print('objects',len(objs),'exported funcs in simulation',len(seq),'common',len(common))
print('same relative position',same_pos,'adjacent-correct pairs',adj,'of',len(seq)-1)
# per object: is its own sequence a contiguous increasing run in original order?
bad=[]
for o,s in per_obj:
    if len(s)<2: continue
    ok=all(opos[s[i]]<opos[s[i+1]] for i in range(len(s)-1))
    if not ok:
        # count inversions
        inv=sum(1 for i in range(len(s)) for j in range(i+1,len(s)) if opos[s[i]]>opos[s[j]])
        bad.append((inv,len(s),o,s))
print('objects with out-of-order exported funcs:',len(bad))
for inv,n,o,s in sorted(bad,reverse=True)[:20]:
    print(f'  {inv:4d} inversions / {n:3d}  {os.path.basename(o)}')
