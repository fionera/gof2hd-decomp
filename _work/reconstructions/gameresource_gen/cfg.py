"""Print the basic-block structure of a code range of an original function, with the
resource entries each block adds (used to recover the if/else structure of the tail)."""
import sys
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from extract import *  # noqa

name, lo, hi = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
out, it = extract(name)
st, sz = func_range(name)
insns, datas = decode(st, sz)
# branch targets
islands = set()
for a, b in zip(insns, insns[1:]):
    if a.mnemonic in ('b', 'b.w') and a.operands[0].imm == b.address and b.address != a.address + a.size:
        islands.add(a.address)
leaders = {lo}
for i in insns:
    if i.address in islands:
        continue
    if lo <= i.address < hi and (i.mnemonic.startswith('b') and i.mnemonic not in ('bic', 'bics', 'bl', 'blx')
                                 or i.mnemonic in ('cbz', 'cbnz')):
        t = i.operands[-1].imm
        leaders.add(t)
leaders = sorted(x for x in leaders if lo <= x < hi)
recs = [r for r in out if lo <= r['addr'] < hi]
bi = 0
for k, L in enumerate(leaders):
    E = leaders[k + 1] if k + 1 < len(leaders) else hi
    rs = [r for r in recs if L <= r['addr'] < E]
    print('== block %x..%x' % (L, E))
    for r in rs:
        print('   ' + fmt(r)[:160])
