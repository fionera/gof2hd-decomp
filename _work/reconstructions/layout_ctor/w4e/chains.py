import json,itertools,struct,sys
S='/tmp/claude-1000/-home-debian-GalaxyOnFire2/58744cff-beee-4dd7-9dbb-ba064b0911f1/scratchpad/w4e'
d=json.load(open(S+'/lc/lay_orig.json'))
F=['iPadHD','n9','iPad','iPadLarge','retinaDisplay']
SH={'iPadHD':'H','n9':'N','iPad':'P','iPadLarge':'L','retinaDisplay':'R'}
combos=sorted(d)
BYTES={0x284,0x285,0x286}
def val(c,w,n):
    st=d[c]
    if not all(str(w+j) in st for j in range(n)): return None
    return sum(st[str(w+j)]<<(8*j) for j in range(n))
def chain_ok(w,n,perm):
    # pure chain: first set flag in perm decides; none set -> default
    res={}
    for c in combos:
        v=val(c,w,n)
        k=next((f for f in perm if c[F.index(f)]=='1'),None)
        if k in res and res[k]!=v: return False
        res[k]=v
    return True
order=[int(x,16) for x in open(sys.argv[1]).read().split()]
out={}
for w in order:
    n=1 if w in BYTES else 4
    oks=[]
    for perm in itertools.permutations(F):
        if chain_ok(w,n,perm): oks.append(''.join(SH[f] for f in perm))
    out['%x'%w]=oks
    # summarize: pairwise precedence constraints that hold in all oks
    cons=[]
    for a in 'HNPLR':
        for b in 'HNPLR':
            if a<b and oks:
                if all(o.index(a)<o.index(b) for o in oks): cons.append(a+'<'+b)
                elif all(o.index(a)>o.index(b) for o in oks): cons.append(b+'<'+a)
    print('%3x %3d %s'%(w,len(oks),' '.join(cons) if oks else 'NOCHAIN'))
json.dump(out,open(S+'/chains.json','w'))
