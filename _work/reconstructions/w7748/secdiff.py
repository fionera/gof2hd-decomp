#!/usr/bin/env python3
"""Per-section differing-byte counts between the original and our linked .so."""
import sys, os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'verify'))
from elf import Elf32
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'cmake-build-match', 'libgof2hdaa.so')
of = Elf32(ORIG); oe = Elf32(OURS)
a = {s['name']: s for s in of.sections}; b = {s['name']: s for s in oe.sections}
for n in a:
    if n not in b:
        print(n, 'missing'); continue
    sa, sb = a[n], b[n]
    ba = of.bytes_at(sa['addr'], sa['size']) if sa['addr'] else b''
    bb = oe.bytes_at(sb['addr'], sb['size']) if sb['addr'] else b''
    d = sum(x != y for x, y in zip(ba, bb)) + abs(len(ba) - len(bb))
    if d or sa['size'] != sb['size']:
        print('%-16s size %d/%d diffbytes %d' % (n, sa['size'], sb['size'], d))
