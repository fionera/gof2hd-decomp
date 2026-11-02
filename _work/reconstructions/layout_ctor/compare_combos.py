#!/usr/bin/env python3
"""Compare the `this` stores of the original and a candidate Layout::C1 for all 32 flag combos.

Usage: python3 _work/reconstructions/layout_ctor/compare_combos.py <candidate.so>
"""
import sys, itertools
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools/verify'))
sys.path.insert(0, str(ROOT / '_work/reconstructions/layout_ctor'))
import emu

F = ['iPadHD', 'n9', 'iPad', 'iPadLarge', 'retinaDisplay']
ORIG = ROOT.parent / '_work/bins/android_2.0.16_libgof2hdaa.so'

def run(so, flags):
    emu.e = emu.V.Elf32(str(so))
    emu.got = emu.e.got_map()
    return emu.run('_ZN6LayoutC1Ev', flags)

def main(ours):
    combos = [''.join(b) for b in itertools.product('01', repeat=5)]
    bad = 0
    bytes_ = set()
    words = set()
    for c in combos:
        fl = {F[i]: int(c[i]) for i in range(5)}
        o = run(ORIG, dict(fl))
        u = run(ours, dict(fl))
        if o != u:
            bad += 1
            diff = [k for k in set(o) | set(u) if o.get(k) != u.get(k)]
            print('BAD', c, len(diff), sorted(diff)[:10])
        else:
            bytes_.update(o.keys())
            words.update(k & ~3 for k in o)
    print('bad combos', bad, 'of', len(combos), 'bytes', len(bytes_), 'words', len(words))

if __name__ == '__main__':
    main(sys.argv[1])
