import sys,re,struct,subprocess,os
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from parse_funcs import funcs
NM='/home/debian/GalaxyOnFire2/wt/w4120/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
FILT='/usr/bin/c++filt'

def nm_addrs(p):
    d={}
    out=subprocess.run([NM,'-nD','--defined-only',p],capture_output=True,text=True).stdout
    for ln in out.splitlines():
        q=ln.split(maxsplit=2)
        if len(q)==3 and q[1] in 'tTwW': d[q[2]]=int(q[0],16)
    return d

def symtab_section_order(path):
    d=open(path,'rb').read(); shoff,=struct.unpack_from('<I',d,0x20); shentsize,shnum,shstrndx=struct.unpack_from('<HHH',d,0x2e)
    hdrs=[struct.unpack_from('<IIIIIIIIII',d,shoff+i*shentsize) for i in range(shnum)]; stroff=hdrs[shstrndx][4]
    def name(off):
        e=d.index(b'\0',stroff+off); return d[stroff+off:e].decode()
    secnames=[name(h[0]) for h in hdrs]; out=[]
    for h in hdrs:
        nm_,typ,flags,a,off,size,link,info,align,ent=h
        if typ!=2: continue
        ent=ent or 16; st=hdrs[link][4]
        for o in range(off,off+size,ent):
            st_name,val,sz,sinfo,other,shndx=struct.unpack_from('<IIIBBH',d,o)
            if not st_name or shndx==0 or shndx>=len(hdrs): continue
            e=d.index(b'\0',st+st_name); n=d[st+st_name:e].decode()
            if (sinfo&0xf)!=2: continue
            out.append((hdrs[shndx][4],n,sinfo>>4,sz,secnames[shndx]))
    out.sort()
    return out

def demangle(names):
    if not names: return []
    r=subprocess.run([FILT]+names,capture_output=True,text=True)
    return r.stdout.splitlines()

def norm(s):
    s=re.sub(r'\s+','',s)
    s=s.replace('*','').replace('&','')
    s=s.replace('const','')
    s=s.replace('AbyssEngine::','').replace('~','')
    return s

def src_sig(decl):
    # strip leading return type: find last 'Class::method(' or 'name('
    k=decl.find('(')
    if k<0: return None
    pre=decl[:k]
    m=re.search(r'([A-Za-z_]\w*(?:::[A-Za-z_]?~?\w*)+)\s*$',pre)
    if m: return m.group(1)
    m=re.search(r'([A-Za-z_]\w*)\s*$',pre)
    return m.group(1) if m else None

def main():
    orig=nm_addrs('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
    src,obj=sys.argv[1],sys.argv[2]
    blocks=funcs(src)
    syms=symtab_section_order(obj)
    # global funcs with original addr, dedupe mangled name
    cand=[]; seen=set()
    for off,n,bind,sz,sec in syms:
        if bind==1 and n in orig and n not in seen:
            seen.add(n); cand.append((n,orig[n]))
    dem=demangle([n for n,_ in cand])
    byname={}; seenaddr=set()
    for (n,a),d in zip(cand,dem):
        if a in seenaddr: continue
        seenaddr.add(a)
        base=d[:d.find('(')] if '(' in d else d
        byname.setdefault(norm(base),[]).append((a,n,d))
    exported=[]
    used=set()
    for bi,(a,b,decl) in enumerate(blocks):
        sig=src_sig(decl)
        if not sig: continue
        key=norm(sig)
        lst=byname.get(key)
        if not lst: continue
        # pick first unused
        for a2,n2,d2 in lst:
            if n2 not in used:
                used.add(n2); exported.append((bi,a,b,a2,d2)); break
    print(f'{os.path.basename(src)}: {len(exported)} exported blocks, {len(seenaddr)} unique-addr global funcs')
    if len(exported)!=len(seenaddr):
        usedn={e[4] for e in exported}
        for (n,a),d in zip(cand,dem):
            if d not in usedn: print('    unmatched sym',d)
        for bi,(a,b,decl) in enumerate(blocks):
            sig=src_sig(decl)
            if sig and not any(e[0]==bi for e in exported): print('    unmatched src',sig)
    # write reordered
    text=open(src).read()
    slots=[e[0] for e in exported]
    sorted_blocks=sorted(exported,key=lambda e:e[3])
    mapping={slot:blk for slot,blk in zip(slots,sorted_blocks)}
    out=[]; pos=0
    for bi,(a,b,decl) in enumerate(blocks):
        out.append(text[pos:a])
        if bi in mapping:
            _,aa,bb,addr,dm=mapping[bi]
            out.append(text[aa:bb])
        else:
            out.append(text[a:b])
        pos=b
    out.append(text[pos:])
    new=''.join(out)
    if '--write' in sys.argv:
        open(src,'w').write(new); print('wrote',src)
    else:
        open('/dev/shm/w4120/reordered_'+os.path.basename(src),'w').write(new)

if __name__=='__main__': main()
