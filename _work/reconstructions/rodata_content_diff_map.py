#!/usr/bin/env python3
"""Reproduce the .rodata content-diff measurements in rodata_content_diff_map.md.

Usage:
  python3 rodata_content_diff_map.py [--orig ORIG.so] [--ours OURS.so]
                                     [--unstripped UNSTRIPPED.so] [--map MAP]

Defaults are the paths used by worker w3428 (run from the repo root):
  orig       ../_work/bins/android_2.0.16_libgof2hdaa.so
  ours       cmake-build-match/libgof2hdaa.so
  unstripped /tmp/w3428_rodata_unstripped.so   (scratch relink with .symtab)
  map        /tmp/w3428_rodata.map             (scratch relink -Map)

Both binaries have .rodata at file offset 0x1c9e10, size 0x41b67, so file
offsets equal VMAs inside .rodata.
"""
import argparse, difflib, re, subprocess, sys
from collections import Counter
from pathlib import Path

RO = 0x1C9E10
RS = 0x41B67
NM = (".cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/"
      "prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm")


def load(path):
    return Path(path).read_bytes()[RO:RO + RS]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--orig", default="../_work/bins/android_2.0.16_libgof2hdaa.so")
    ap.add_argument("--ours", default="cmake-build-match/libgof2hdaa.so")
    ap.add_argument("--unstripped", default="/tmp/w3428_rodata_unstripped.so")
    ap.add_argument("--map", default="/tmp/w3428_rodata.map")
    a = ap.parse_args()

    O, U = load(a.orig), load(a.ours)

    diff = [i for i in range(RS) if O[i] != U[i]]
    runs = 0
    prev = -2
    for i in diff:
        if i != prev + 1:
            runs += 1
        prev = i
    print("== raw diff ==")
    print(f"differing bytes {len(diff)} / {RS} ({100.0*len(diff)/RS:.2f}%)")
    if not diff:
        print("runs 0  (byte-identical)")
        return
    print(f"runs {runs}  first VMA {RO+diff[0]:#x}  last VMA {RO+diff[-1]:#x}")

    start = diff[0] & ~3
    end = (diff[-1] | 3) + 1
    ow = [O[i:i + 4] for i in range(start, end, 4)]
    uw = [U[i:i + 4] for i in range(start, end, 4)]
    co, cu = Counter(ow), Counter(uw)
    print("\n== word multiset over the diff region ==")
    print(f"region {RO+start:#x}..{RO+end:#x}  words {len(ow)}")
    print(f"common {sum((co & cu).values())}  only-orig "
          f"{sum((co - cu).values())}  only-ours {sum((cu - co).values())}")

    sm = difflib.SequenceMatcher(None, ow, uw, autojunk=False)
    ops = sm.get_opcodes()
    eq = sum(i2 - i1 for t, i1, i2, j1, j2 in ops if t == "equal")
    de = sum(i2 - i1 for t, i1, i2, j1, j2 in ops if t == "delete")
    ins = sum(j2 - j1 for t, i1, i2, j1, j2 in ops if t == "insert")
    rep = sum(min(i2 - i1, j2 - j1) for t, i1, i2, j1, j2 in ops if t == "replace")
    uneq = 0
    for t, i1, i2, j1, j2 in ops:
        if t == "replace":
            for k in range(min(i2 - i1, j2 - j1)):
                if ow[i1 + k] != uw[j1 + k]:
                    uneq += 1
    print(f"align: equal {eq}  delete {de}  insert {ins}  replace {rep} "
          f"(unequal {uneq}); moved {sum((co & cu).values()) - eq}")

    print("\n== per-symbol exact-content check ==")
    try:
        out = subprocess.check_output([NM, "-S", "-an", a.unstripped], text=True)
    except (OSError, subprocess.CalledProcessError) as e:
        print(f"skipped (nm failed: {e})")
        return
    syms = []
    for line in out.splitlines():
        m = re.match(r"([0-9a-fA-F]{8,16})\s+([0-9a-fA-F]+)\s+([A-Za-z])\s+(.+)$",
                     line.strip())
        if m:
            addr, size, typ, name = int(m.group(1), 16), int(m.group(2), 16), m.group(3), m.group(4)
            if RO <= addr < RO + RS:
                syms.append((addr, size, typ, name))
    syms.sort()
    ndiff = nnot = 0
    for addr, size, typ, name in syms:
        lo, hi = addr - RO, addr - RO + size
        if not any(O[x] != U[x] for x in range(lo, hi)):
            continue
        ndiff += 1
        ours = U[lo:hi]
        occ = []
        pos = 0
        while True:
            p = O.find(ours, pos)
            if p < 0:
                break
            occ.append(RO + p)
            pos = p + 1
        if not occ:
            nnot += 1
            print(f"  NOT-EXACT {name}  ours {addr:#x} size {size}")
    print(f"differing symbols {ndiff}; no verbatim copy in original {nnot}")


if __name__ == "__main__":
    main()
