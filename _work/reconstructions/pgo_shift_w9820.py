#!/usr/bin/env python3
"""Shifted-rank PGO replay for Status::missionCompleted (wave 151, w9820).

w9720 established that a 1432 B rank-profile build still gives
`byte_exact 4493` because the target's call sites appear in a different
order, which permutes five `.rel.plt` slots.  This harness searches a
*shifted* rank profile: the switch-case counter is `rank*K + off` instead
of `rank-1`.  Raising `off` makes the shared `isInList(ii)`/`setWon` tails
cold enough that MachineBlockPlacement leaves them at the end of the
function, restoring the original's first-occurrence call order

    [getClientImage, isInList(i), getIngredientList,
     getQuantityList, isInList(ii), setWon]           (canonical)

and therefore a byte-identical `.rel.plt`.  It does NOT restore the
original's switch-case order or its 1432 B size; see the note.

Usage:
  python3 pgo_shift_w9820.py order <so> ...
  python3 pgo_shift_w9820.py shift <base> <off> <k4> [c28] [latch] [--extra FLAG ...]
  python3 pgo_shift_w9820.py grid <file>     # lines: base off k4 c28 latch

Scratch: /dev/shm/w9820/run (override with W9620_SCRATCH).
"""
import os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pgo_combine_w9620 as h

SIX = ['_ZN7Mission14getClientImageEv',
       '_ZN4Item8isInListEiP5ArrayIPS_E',
       '_ZN9BluePrint17getIngredientListEv',
       '_ZN9BluePrint15getQuantityListEv',
       '_ZN4Item8isInListEiiP5ArrayIPS_E',
       '_ZN7Mission6setWonEb']
CANON = [0, 1, 2, 3, 4, 5]


def order(so):
    a = h.sym(so, h.MANGLE)
    seen = []
    for r in h.disasm(so, *a):
        if r[1].startswith('bl'):
            nm = r[2].split('<')[-1].split('@')[0] if '<' in r[2] else r[2]
            if nm in SIX and nm not in seen:
                seen.append(nm)
    return [SIX.index(x) for x in seen]


def section(path, name):
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    secs = [struct.unpack_from('<IIIIII', d, shoff + i * shentsize)
            for i in range(shnum)]
    st = d[secs[shstrndx][4]:secs[shstrndx][4] + secs[shstrndx][5]]
    for nm, ty, fl, ad, of, sz in secs:
        if st[nm:st.find(b'\0', nm)].decode() == name:
            return d, of, sz
    raise SystemExit('no section ' + name)


def relplt_diff(so):
    a = section(h.ORIG, '.rel.plt')
    b = section(so, '.rel.plt')
    if a[2] != b[2]:
        return -1
    return sum(1 for i in range(0, a[2], 8)
               if a[0][a[1] + i:a[1] + i + 8] != b[0][b[1] + i:b[1] + i + 8])


def make_vals(meta, base, off, k4, c28, latch, K=1):
    vals = [base] * meta['ncounters']
    seen = set()
    for v in meta['ir_order']:
        ctr = meta['case_ctr'].get(v)
        if ctr is None or ctr in seen:
            continue
        seen.add(ctr)
        vals[ctr] = meta['rank'].get(v, 1) * K + off
    for k in (6, 7, 12):
        vals[k] = latch
    vals[28] = c28
    vals[4] = k4
    vals[23] = 0
    return vals


def run(meta, base, off, k4, c28, latch, extra=(), K=1):
    vals = make_vals(meta, base, off, k4, c28, latch, K)
    m = h.build(meta, vals, 'shift', extra)
    m['order'] = order(m['so'])
    m['relplt'] = relplt_diff(m['so'])
    return m


def report(tag, m):
    print('%s size=%d raw=%d aligned=%d eager=%s relplt=%d order=%s %s'
          % (tag, m['size'], m['raw'], m['aligned'], m['eager'], m['relplt'],
             m['order'], 'MATCH' if m['order'] == CANON else ''), flush=True)


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return
    cmd = sys.argv[1]
    if cmd == 'order':
        for p in sys.argv[2:]:
            print(os.path.basename(p), order(p), 'relplt_diffs', relplt_diff(p))
        return
    meta = h.get_meta()
    if cmd == 'shift':
        a = sys.argv[2:]
        extra = []
        if '--extra' in a:
            i = a.index('--extra')
            extra = a[i + 1:]
            a = a[:i]
        base, off, k4 = int(a[0]), int(a[1]), int(a[2])
        c28 = int(a[3]) if len(a) > 3 else 28
        latch = int(a[4]) if len(a) > 4 else 0
        report('base=%d off=%d k4=%d c28=%d latch=%d' % (base, off, k4, c28, latch),
               run(meta, base, off, k4, c28, latch, extra))
    elif cmd == 'grid':
        for line in open(sys.argv[2]):
            p = line.split()
            if not p:
                continue
            base, off, k4, c28, latch = [int(x) for x in p[:5]]
            report('base=%d off=%d k4=%d c28=%d latch=%d' % (base, off, k4, c28, latch),
                   run(meta, base, off, k4, c28, latch))


if __name__ == '__main__':
    main()
