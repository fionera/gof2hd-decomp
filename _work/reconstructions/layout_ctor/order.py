#!/usr/bin/env python3
"""order.py SO SYM -> field store order of the default (all flags 0) path of the original ctor,
with fields that path never writes inserted after their predecessor on the n9 path."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import emu
BYTES = (0x284, 0x285, 0x286)
def order(fl):
    emu.run(sys.argv[1], fl); seen = []
    for p, o, n in emu.run.last.trace:
        if isinstance(o, int):
            k = o if o in BYTES else o & ~3
            if k not in seen: seen.append(k)
    return seen
z = dict(iPadHD=0, n9=0, iPad=0, iPadLarge=0, retinaDisplay=0)
seen = order(z)
for other in (dict(z, n9=1), dict(z, iPadHD=1)):
    o = order(other)
    for i, k in enumerate(o):
        if k not in seen:
            seen.insert(seen.index(o[i - 1]) + 1 if i else 0, k)
print(' '.join('%x' % x for x in seen))
