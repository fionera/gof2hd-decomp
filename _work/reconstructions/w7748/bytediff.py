#!/usr/bin/env python3
"""Raw differing-byte count for one function (same-offset comparison)."""
import sys, os
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'verify'))
from elf import Elf32
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'cmake-build-match', 'libgof2hdaa.so')
name = sys.argv[2] if len(sys.argv) > 2 else '_ZN11LevelScript7processEi'
of = Elf32(ORIG); oe = Elf32(OURS)
o = of.functions()[name]; u = oe.functions()[name]
ob = of.bytes_at(o['value'], o['size']); ub = oe.bytes_at(u['value'], u['size'])
print('sizes', len(ob), len(ub), 'diffbytes', sum(a != b for a, b in zip(ob, ub)))
