#!/usr/bin/env python3
"""Synthetic instrumented-PGO replay for Status::missionCompleted (wave 156, w10214).

Builds a hand-written `.profdata` (counter vector) for one function, compiles
its TU with `-fprofile-instr-use`, relinks the whole .so with the exact gold
command, and reports raw / aligned / size / .rel.plt parity.

The counter layout comes from the `-fprofile-instr-generate` IR (see
`pgo_combine_w9620.get_meta()`); `case_ctr` maps a switch case value to the
counter that instruments the switch edge to that case's block.

Usage:
  python3 prof_instr_w10214.py flat BASE [G1 G2 G3]      # flat case groups
  python3 prof_instr_w10214.py gradient                  # flat groups + source-order gradient
  python3 prof_instr_w10214.py consistent K              # flow-consistent counts
  python3 prof_instr_w10214.py consgrad K                # consistent + source-order gradient

Scratch: /dev/shm/w10214/run (override with W9620_SCRATCH).
"""
import os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
os.environ.setdefault('W9620_SCRATCH', '/dev/shm/w10214/run')
import pgo_combine_w9620 as H

G1 = [167, 174, 8, 0, 11, 13, 14]
G2 = [150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184, 189]
G3 = [156, 157, 160, 163, 164, 165, 168, 169, 171, 172]
# source order of the original jump-table bodies (base-150 table then base-8 table)
SRC = [167, 174, 8, 0, 11, 13, 14, 150, 151, 152, 153, 154, 155, 158, 162,
       166, 170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172]


def flat(meta, base, g1=4, g2=3, g3=2, latch=1, c28=1):
    v = [base] * meta['ncounters']
    for g, val in zip((G1, G2, G3), (g1, g2, g3)):
        for c in g:
            v[meta['case_ctr'][c]] = val
    for k in (6, 7, 12):
        v[k] = latch
    v[28] = c28
    return v


def gradient(meta, g1=4000, g2=3000, g3=2000):
    """Flat group ratios with a tiny per-case gradient in source order."""
    v = [1] * meta['ncounters']
    for c in G1:
        v[meta['case_ctr'][c]] = g1
    for i, c in enumerate(G2):
        v[meta['case_ctr'][c]] = g2 + (len(G2) - i)
    for i, c in enumerate(G3):
        v[meta['case_ctr'][c]] = g3 + (len(G3) - i)
    for k in (6, 7, 12):
        v[k] = 1
    return v


def consistent(meta, K=1000, g1=4, g2=3, g3=2):
    """Flow-consistent counts: N=1 call, K loop iterations, no filtered missions."""
    v = [0] * meta['ncounters']
    v[0] = v[1] = v[2] = v[3] = 1
    v[4] = K
    v[5] = 0
    v[6] = v[7] = 0
    for k in (8, 9, 10, 11):
        v[k] = K
    v[12] = K
    for c in G1:
        v[meta['case_ctr'][c]] = g1
    for c in G2:
        v[meta['case_ctr'][c]] = g2
    for c in G3:
        v[meta['case_ctr'][c]] = g3
    v[115] = K - len(G1) * g1 - len(G2) * g2 - len(G3) * g3
    return v


def consgrad(meta, K=100000, g1=4000, g2=3000, g3=2000):
    v = [0] * meta['ncounters']
    v[0] = v[1] = v[2] = v[3] = 1
    v[4] = K
    v[5] = 0
    v[6] = v[7] = 0
    for k in (8, 9, 10, 11):
        v[k] = K
    v[12] = K
    for c in G1:
        v[meta['case_ctr'][c]] = g1
    for i, c in enumerate(G2):
        v[meta['case_ctr'][c]] = g2 + (len(G2) - i)
    for i, c in enumerate(G3):
        v[meta['case_ctr'][c]] = g3 + (len(G3) - i)
    s = len(G1) * g1 + sum(g2 + (len(G2) - i) for i in range(len(G2))) \
        + sum(g3 + (len(G3) - i) for i in range(len(G3)))
    v[115] = K - s
    return v


def _section(path, name):
    d = open(path, 'rb').read()
    e_shoff, = struct.unpack_from('<I', d, 0x20)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    secs = [struct.unpack_from('<IIIIII', d, e_shoff + i * e_shentsize)
            for i in range(e_shnum)]
    shstr = secs[e_shstrndx]
    st = d[shstr[4]:shstr[4] + shstr[5]]
    for s in secs:
        end = st.find(b'\0', s[0])
        if st[s[0]:end].decode('latin1') == name:
            return d[s[4]:s[4] + s[5]]
    return None


def relplt_diff(so):
    a = _section(H.ORIG, '.rel.plt')
    b = _section(so, '.rel.plt')
    if a is None or b is None:
        return None
    return sum(x != y for x, y in zip(a, b)), len(a), len(b)


def main():
    meta = H.get_meta()
    cmd = sys.argv[1]
    extra = []
    if '--extra' in sys.argv:
        i = sys.argv.index('--extra')
        extra = sys.argv[i + 1:]
        sys.argv = sys.argv[:i]
    if cmd == 'flat':
        base = int(sys.argv[2])
        g = [int(x) for x in sys.argv[3:6]] or [4, 3, 2]
        vals = flat(meta, base, *g)
    elif cmd == 'gradient':
        vals = gradient(meta)
    elif cmd == 'consistent':
        vals = consistent(meta, int(sys.argv[2]))
    elif cmd == 'consgrad':
        vals = consgrad(meta, int(sys.argv[2]))
    else:
        raise SystemExit(__doc__)
    r = H.build(meta, vals, 'instr_' + cmd, extra)
    rp = relplt_diff(r['so'])
    print('%s raw=%d aligned=%d size=%d eager=%s other=%d/%d relplt=%s'
          % (cmd, r['raw'], r['aligned'], r['size'], r['eager'],
             r['other_same'], r['other_total'], rp))


if __name__ == '__main__':
    main()
