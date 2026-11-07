#!/usr/bin/env python3
"""Score _ZN11LevelScript7processEi against the original linked .so.

Usage: python3 score.py [ours.so] [mangled]
Assumes the repo root is two levels up from this file (worktree layout).
"""
import sys, os, tempfile
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'verify'))
import verify as V
from elf import Elf32
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'cmake-build-match', 'libgof2hdaa.so')
OBJ = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump')
tempfile.tempdir = os.environ.get('W7748_TMP', '/dev/shm/w7748')
os.makedirs(tempfile.tempdir, exist_ok=True)
of = Elf32(ORIG); oe = Elf32(OURS)
orig = of.functions(); ours = oe.functions(); common = set(orig) & set(ours)
def ar(e):
    mapped = [s for s in e.sections if s['addr']]
    lo = next((s['addr'] for s in mapped if s['name'] == '.plt'), min(s['addr'] for s in mapped))
    return lo, max(s['addr'] + s['size'] for s in mapped)
orr = V.Resolver(orig, common, of.plt_map(), ar(of))
our = V.Resolver(ours, common, oe.plt_map(), ar(oe))
co = {o[2] for o in of.objects()} & {o[2] for o in oe.objects()}
for r, e in ((orr, of), (our, oe)):
    r.attach_data(e, co)
imp = ({v for k, v in of.got_map().values() if k == 'sym'} &
       {v for k, v in oe.got_map().values() if k == 'sym'})
orr.imports = our.imports = imp
name = sys.argv[2] if len(sys.argv) > 2 else '_ZN11LevelScript7processEi'
o, u = orig[name], ours[name]
ob = of.bytes_at(o['value'], o['size']); ub = oe.bytes_at(u['value'], u['size'])
oraw, on = V.disassemble(OBJ, ob, o['thumb'], o['value'], orr)
uraw, un = V.disassemble(OBJ, ub, u['thumb'], u['value'], our)
on, un = V.reconcile(on, un)
print('size', o['size'], u['size'], 'insns', len(on), len(un))
print('pct', V.score(on, un))
