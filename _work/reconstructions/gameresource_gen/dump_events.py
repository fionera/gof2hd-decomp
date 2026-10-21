import sys
sys.path.insert(0, __file__.rsplit('/', 1)[0])
from armdis import *
name = sys.argv[1]
st, sz = func_range(name)
insns, datas = decode(st, sz)
it = Interp(insns, datas, plt_map(), got_map())
ev = it.run()
print('# insns', len(insns), 'unknown', sorted(it.unknown), file=sys.stderr)
for e in ev:
    if e[0] == 'call':
        print('%x call %s %s %s -> %r' % (e[4], e[1], e[2], [x for x in e[3] if x is not None] if e[1] not in ('_Znwj',) else '', e[5]))
    elif e[0] == 'field':
        print('%x field obj%d+%d = %r %s' % (e[4], e[1], e[2], e[3], e[5] if len(e) > 5 else ''))
    elif e[0] == 'slot':
        print('%x slot sp+%d = obj%d' % (e[3], e[1], e[2]))
    else:
        print('%x %s' % (e[-1], e[:-1]))
