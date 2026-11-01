#!/usr/bin/env python3
"""Attribute .rodata content differences to the owning input section/TU.

Usage:
  python3 _work/reconstructions/rodata_content_map_round2.py \
      --map /tmp/w3438_rodata.map \
      --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
      --ours cmake-build-match/libgof2hdaa.so

The map must come from a scratch gold relink with `-Map` (the shipped .so is
stripped, so `nm` cannot name the input sections).  The output is the table in
rodata_content_map_round2.md: differing bytes per input section, the owner
object, and the first verbatim occurrence of that section's bytes in the
original .rodata (NONE means the section's content itself differs, not just its
address).
"""
import argparse
import os
import re
from collections import defaultdict
from pathlib import Path

RO = 0x1C9E10
RS = 0x41B67


def parse_map(path):
    lines = Path(path).read_text().splitlines()
    start = next(i for i, l in enumerate(lines)
                 if re.match(r'^\.rodata\s+0x[0-9a-f]+\s+0x[0-9a-f]+$', l))
    end = next(i for i in range(start + 1, len(lines))
               if lines[i] and not lines[i].startswith(' '))
    out = []
    cur_name = None
    for line in lines[start + 1:end]:
        m = re.match(r'^ (\.rodata\S*)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+(.+))?$', line)
        if m:
            name, addr, size, owner = m.group(1), int(m.group(2), 16), int(m.group(3), 16), (m.group(4) or '').strip()
            out.append((name, addr, size, owner or '** merge'))
            cur_name = name
            continue
        m = re.match(r'^ (\.rodata\S*)\s*$', line)
        if m:
            cur_name = m.group(1)
            continue
        m = re.match(r'^ \s*(\*\* merge \w+(?: \w+)?)\s*$', line)
        if m:
            cur_name = m.group(1)
            continue
        m = re.match(r'^ \s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+(.+))?$', line)
        if m:
            addr, size, owner = int(m.group(1), 16), int(m.group(2), 16), (m.group(3) or '').strip()
            out.append((cur_name or '?', addr, size, owner or '** merge'))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--map', default='/tmp/w3438_rodata.map')
    ap.add_argument('--orig', default='../_work/bins/android_2.0.16_libgof2hdaa.so')
    ap.add_argument('--ours', default='cmake-build-match/libgof2hdaa.so')
    ap.add_argument('--ro-base', type=lambda s: int(s, 0), default=RO)
    ap.add_argument('--ro-size', type=lambda s: int(s, 0), default=RS)
    a = ap.parse_args()

    orig = Path(a.orig).read_bytes()[a.ro_base:a.ro_base + a.ro_size]
    ours = Path(a.ours).read_bytes()[a.ro_base:a.ro_base + a.ro_size]
    secs = parse_map(a.map)

    rows = []
    for name, addr, size, owner in secs:
        lo, hi = addr - a.ro_base, addr - a.ro_base + size
        if lo < 0 or hi > a.ro_size:
            continue
        diff = sum(1 for i in range(lo, hi) if orig[i] != ours[i])
        if not diff:
            continue
        body = ours[lo:hi]
        occ = []
        pos = 0
        while True:
            p = orig.find(body, pos)
            if p < 0:
                break
            occ.append(a.ro_base + p)
            pos = p + 1
        rows.append((diff, size, name, os.path.basename(owner), addr, occ[:5]))

    rows.sort(reverse=True)
    print('output .rodata %#x size %#x, input sections %d, differing %d'
          % (a.ro_base, a.ro_size, len(secs), len(rows)))
    print(f"{'bytes':>6} {'size':>6} {'section':<42} {'owner':<44} {'ours':>9} {'orig occurrence(s)'}")
    for diff, size, name, owner, addr, occ in rows:
        print(f'{diff:6d} {size:6d} {name:<42} {owner:<44} {addr:#09x} '
              f'{",".join(hex(x) for x in occ[:3]) or "NONE"}')

    print('\nTotals by owner:')
    by = defaultdict(lambda: [0, 0, 0])
    for diff, size, name, owner, addr, occ in rows:
        by[owner][0] += diff
        by[owner][1] += size
        by[owner][2] += 1
    for owner, (diff, size, count) in sorted(by.items(), key=lambda kv: -kv[1][0]):
        print(f'{diff:6d} {size:6d} {count:3d} {owner}')


if __name__ == '__main__':
    main()
