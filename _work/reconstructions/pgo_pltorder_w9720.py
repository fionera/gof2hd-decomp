#!/usr/bin/env python3
"""PLT-order scorer for the Status::missionCompleted PGO replay (wave 150, w9720).

The 14 "downstream one-byte literal-pool diffs" of a 1432 B profile build are
NOT literal-pool artefacts in the target: they are a 5-entry `.rel.plt`
permutation caused by the order in which the profiled target's call sites
appear in its `.text` section.  gold allocates PLT slots in first-relocation
order, so a different call order shifts the PLT stubs and every `blx <plt>`
and every `add r0, pc` GOT literal in the callers.

This tool reports the first-occurrence order of the six PLT-relevant callees
in the target and compares it with the original's order (which is also the
canonical build's order, because `.rel.plt` is byte-identical there).

Usage:
  python3 pgo_pltorder_w9720.py order <so> [<so> ...]
  python3 pgo_pltorder_w9720.py sweep4 <k4> <k11> [extra flags ...]
  python3 pgo_pltorder_w9720.py thresholds

Scratch: /dev/shm/w9720/run (override with W9620_SCRATCH).
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pgo_combine_w9620 as h

SIX = ['_ZN7Mission14getClientImageEv',
       '_ZN4Item8isInListEiP5ArrayIPS_E',
       '_ZN9BluePrint17getIngredientListEv',
       '_ZN9BluePrint15getQuantityListEv',
       '_ZN4Item8isInListEiiP5ArrayIPS_E',
       '_ZN7Mission6setWonEb']


def order(path):
    a = h.sym(path, h.MANGLE)
    seen = []
    for r in h.disasm(path, *a):
        if r[1].startswith('bl'):
            nm = r[2].split('<')[-1].split('@')[0] if '<' in r[2] else r[2]
            if nm in SIX and nm not in seen:
                seen.append(nm)
    return seen


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    cmd = sys.argv[1]
    ref = order(h.ORIG)
    print('original order:', [SIX.index(x) for x in ref])
    if cmd == 'order':
        for p in sys.argv[2:]:
            o = order(p)
            print(os.path.basename(p), [SIX.index(x) for x in o],
                  'MATCH' if o == ref else 'DIFF')
    elif cmd == 'sweep4':
        meta = h.get_meta()
        k4, k11 = int(sys.argv[2]), int(sys.argv[3])
        extra = sys.argv[4:]
        vals = h.base_vector(12, 28, 0, meta, {4: k4, 11: k11})
        tag = 'p4_%d_%d' % (k4, k11)
        m = h.build(meta, vals, tag, extra)
        o = order(m['so'])
        print(tag, 'raw', m['raw'], 'aligned', m['aligned'], 'size', m['size'],
              'eager', m['eager'], 'plt', [SIX.index(x) for x in o],
              'MATCH' if o == ref else 'DIFF')
    elif cmd == 'thresholds':
        meta = h.get_meta()
        for extra in (['-mllvm', '-max-uses-for-sinking=0'],
                      ['-mllvm', '-max-uses-for-sinking=1'],
                      ['-mllvm', '-sink-freq-percent-threshold=0']):
            vals = h.base_vector(12, 28, 0, meta)
            tag = 'th_' + extra[-1].replace('=', '').replace('-', '')
            m = h.build(meta, vals, tag, extra)
            o = order(m['so'])
            print(extra[-1], 'raw', m['raw'], 'aligned', m['aligned'],
                  'size', m['size'], 'eager', m['eager'],
                  'plt', [SIX.index(x) for x in o],
                  'MATCH' if o == ref else 'DIFF')


if __name__ == '__main__':
    main()
