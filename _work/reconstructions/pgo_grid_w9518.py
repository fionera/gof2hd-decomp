#!/usr/bin/env python3
"""Grid search for a full-TU `-fprofile-instr-use` counter vector that makes
`Status::missionCompleted` exactly 1432 B (the original size) and, optionally,
reproduces the original's source-order switch layout.

Uses the helpers in `pgo_mechanisms_w9518.py`.  The switch vector is the w9305
winning family `[1, A, A, low*5, B*12, C*10]` (IR case order), with every
non-case counter set to `base`.

  python3 pgo_grid_w9518.py            # the 750-point grid
  python3 pgo_grid_w9518.py --quick    # 4 known size-1432 hits only
"""
import os, re, subprocess, sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import pgo_mechanisms_w9518 as H

SRC_ORDER = [167, 174, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184,
             189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172]


def objsize(obj):
    out = subprocess.run([H.OBJDUMP, '-t', obj], capture_output=True, text=True).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) >= 6 and p[-1] == H.MANGLE:
            return int(p[4], 16)
    return None


def order_of(obj):
    out = subprocess.run(['python3', os.path.join(H.REPO, '_work/reconstructions/jumptable_w9319.py'), obj],
                         capture_output=True, text=True).stdout
    m = re.search(r'base=150 N=39.*?\n\s*phys: \[([^\]]*)\]', out, re.S)
    return [int(x) for x in m.group(1).split(',')] if m else None


def main():
    quick = '--quick' in sys.argv
    os.makedirs(H.SCRATCH, exist_ok=True)
    cmd = H.compile_cmd()
    ir = os.path.join(H.SCRATCH, 'status_instr.ll')
    H.instrument_ir(cmd, ir)
    fhash, ncounters, case_ctr, ir_order = H.parse_switch(ir)

    if quick:
        points = [(5, 6, 6, 5, 16), (6, 3, 3, 2, 3), (6, 3, 3, 2, 16), (6, 5, 5, 4, 12)]
    else:
        points = [(A, low, C + 1, C, base)
                  for A in (2, 3, 4, 5, 6)
                  for low in (1, 2, 3, 4, 5, 6)
                  for C in (1, 2, 3, 4, 5)
                  for base in (0, 1, 2, 4, 8)]

    hits = 0
    for (A, low, B, C, base) in points:
        weights = [1, A, A, low, low, low, low, low] + [B] * 12 + [C] * 10
        vals = H.make_counters(weights, ir_order, case_ctr, ncounters, base, False)
        tag = 'g_%d_%d_%d_%d_b%d' % (A, low, B, C, base)
        txt = os.path.join(H.SCRATCH, tag + '.proftext')
        dat = os.path.join(H.SCRATCH, tag + '.profdata')
        H.write_profile(fhash, ncounters, vals, txt)
        H.merge(txt, dat)
        obj = os.path.join(H.SCRATCH, tag + '.obj')
        H.compile_variant(cmd, obj, (), dat)
        if objsize(obj) == 1432:
            hits += 1
            o = order_of(obj)
            src = o is not None and o[:24] == SRC_ORDER
            print('SIZE1432  A=%d low=%d B=%d C=%d base=%d  src_order=%s'
                  % (A, low, B, C, base, src), flush=True)
    print('done: %d points, %d size-1432 hits' % (len(points), hits))


if __name__ == '__main__':
    main()
