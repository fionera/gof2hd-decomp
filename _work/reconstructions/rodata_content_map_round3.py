#!/usr/bin/env python3
"""Map original .rodata VMA -> owning symbol/TU (round 3, w3450).

For every .rodata/.data symbol in every gof2 object file, read the symbol's
bytes and search the original .rodata for the exact byte string.  Print the
VMA, size, symbol, owner object and input section for every hit inside the
region of interest.

Usage (from the repo root, after a normal build):
  python3 _work/reconstructions/rodata_content_map_round3.py \
      [--orig ../_work/bins/android_2.0.16_libgof2hdaa.so] \
      [--build cmake-build-match] [--lo 0x1fc3d0] [--hi 0x1fdd14]

Caveat: the search reports the *first* occurrence of the symbol's bytes.  For
small tables whose content also appears inside another table (e.g.
kAgentMeshes inside the second half of our 64-byte jumpgateMeshes) the first
hit can be a coincidental overlap; cross-check the size and section.
"""
import argparse
import glob
import os
import re
import struct
import subprocess


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--orig", default="../_work/bins/android_2.0.16_libgof2hdaa.so")
    ap.add_argument("--build", default="cmake-build-match")
    ap.add_argument("--lo", default="0x1fc3d0")
    ap.add_argument("--hi", default="0x1fdd14")
    a = ap.parse_args()
    lo, hi = int(a.lo, 16), int(a.hi, 16)
    orig = open(a.orig, "rb").read()

    entries = []
    pat = os.path.join(a.build, "CMakeFiles", "gof2.dir", "**", "*.obj")
    for obj in glob.glob(pat, recursive=True):
        secs = {}
        for line in subprocess.check_output(["objdump", "-h", obj], text=True).splitlines():
            m = re.match(r"\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)", line)
            if m:
                secs[m.group(1)] = (int(m.group(5), 16), int(m.group(2), 16))
        if not any(s.startswith(".rodata") for s in secs):
            continue
        data = open(obj, "rb").read()
        for line in subprocess.check_output(["objdump", "-t", obj], text=True).splitlines():
            m = re.match(r"([0-9a-f]+)\s+\S+\s+\S+\s+(\S+)\s+([0-9a-f]+)\s+(\S+)", line)
            if not m:
                continue
            val, sec, size, name = int(m.group(1), 16), m.group(2), int(m.group(3), 16), m.group(4)
            if sec not in secs or size <= 0:
                continue
            if not (sec.startswith(".rodata") or sec.startswith(".data")):
                continue
            off, _ = secs[sec]
            blob = data[off + val:off + val + size]
            i = orig.find(blob)
            if i < 0:
                continue
            entries.append((i, size, name, os.path.basename(obj), sec))

    seen, out = set(), []
    for e in entries:
        k = (e[0], e[1], e[2])
        if k in seen:
            continue
        seen.add(k)
        out.append(e)
    out.sort()
    print("Original .rodata 0x%x..0x%x" % (lo, hi))
    print("%-10s %5s %-52s %-28s %s" % ("vma", "size", "symbol", "owner", "section"))
    for off, size, name, obj, sec in out:
        if lo <= off < hi:
            print("0x%08x %5d %-52s %-28s %s" % (off, size, name[:52], obj[:28], sec))


if __name__ == "__main__":
    main()
