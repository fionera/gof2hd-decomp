import json,sys,struct,itertools
d=json.load(open(sys.argv[1]))
F=['iPadHD','n9','iPad','iPadLarge','retinaDisplay']
combos=sorted(d)
words=sorted({int(k)&~3 for v in d.values() for k in v})
def val(c,w):
    st=d[c]
    if not all(str(w+j) in st for j in range(4)):
        if any(str(w+j) in st for j in range(4)): return ('B',tuple(st.get(str(w+j)) for j in range(4)))
        return None
    return sum(st[str(w+j)]<<(8*j) for j in range(4))
def chain(w):
    best=None
    for perm in itertools.permutations([0,1,3,4,2]):
        rem=list(combos); out=[]; ok=True
        for vi in perm:
            if len({val(c,w) for c in rem})<=1: break
            sel=[c for c in rem if c[vi]=='1']
            vs={val(c,w) for c in sel}
            if len(vs)==1:
                out.append((F[vi],vs.pop())); rem=[c for c in rem if c[vi]=='0']
            elif len(sel)==0: continue
            else: ok=False; break
        if not ok: continue
        vs={val(c,w) for c in rem}
        if len(vs)!=1: continue
        r=(len(out),out,vs.pop())
        if best is None or r[0]<best[0]: best=r
    return best
res={}
for w in words:
    c=chain(w)
    res[w]=c
    print('%03x'%w, c if c else 'NOCHAIN')
json.dump({str(k):v for k,v in res.items()},open(sys.argv[2],'w'))
