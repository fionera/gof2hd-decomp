#!/usr/bin/env python3
"""Per-record `.ARM.extab` diff map between the original and our build.

Usage:
  python3 _work/reconstructions/extab_record_map.py [--ours PATH] [--orig PATH]

Prints every differing EHABI record ranked by differing bytes, with the
function name (resolved from the original's .dynsym when exported, otherwise
from the nearest containing exported symbol), record length and LSDA row
counts.  Records are matched by (function VMA, occurrence index).

This is the tool used by extab-largest-record-wave156-w10219.md.
"""
import argparse
import collections
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import extab_lsda_decode as E

OBJ = os.path.join(
    os.path.dirname(HERE), "..", ".cache", "ndk", "android-ndk-r18b",
    "toolchains", "arm-linux-androideabi-4.9", "prebuilt", "linux-x86_64",
    "bin", "arm-linux-androideabi-objdump")
ORIG = "/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so"
OURS = os.path.join(os.path.dirname(HERE), "..", "cmake-build-match", "libgof2hdaa.so")


def dynsym(path):
    out = subprocess.check_output([OBJ, "-T", path], text=True)
    syms = []
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 6 and p[2] == "DF" and p[3] == ".text":
            try:
                syms.append((int(p[0], 16), int(p[4], 16), p[-1]))
            except ValueError:
                pass
    syms.sort()
    return syms


def name_of(syms, fn):
    for a, sz, n in syms:
        if a == fn:
            return n
    best = None
    for a, sz, n in syms:
        if a <= fn < a + sz:
            best = (a, sz, n)
    if best:
        return "%s+%#x" % (best[2], fn - best[0])
    return "?"


def records(path):
    data, secs, entries = E.parse(path)
    et = secs[".ARM.extab"]
    out = []
    for r in entries:
        if r["kind"] != "EXT":
            continue
        off = et["off"] + (r["extab"] - et["addr"])
        blob = data[off:off + r["reclen"]]
        try:
            rows = len(E.decode_lsda(data, secs, r["extab"], r["reclen"])["rows"])
        except Exception:
            rows = -1
        out.append(dict(fn=r["fn"], addr=r["extab"], len=r["reclen"],
                        blob=blob, rows=rows))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--orig", default=ORIG)
    ap.add_argument("--ours", default=OURS)
    args = ap.parse_args()
    syms = dynsym(args.orig)
    ra, rb = records(args.orig), records(args.ours)
    ga, gb = collections.defaultdict(list), collections.defaultdict(list)
    for r in ra:
        ga[r["fn"]].append(r)
    for r in rb:
        gb[r["fn"]].append(r)
    rows = []
    for fn in sorted(ga):
        for k in range(min(len(ga[fn]), len(gb.get(fn, [])))):
            a, b = ga[fn][k], gb[fn][k]
            n = min(len(a["blob"]), len(b["blob"]))
            nd = sum(1 for p, q in zip(a["blob"][:n], b["blob"][:n]) if p != q)
            nd += abs(len(a["blob"]) - len(b["blob"]))
            if nd:
                rows.append((nd, fn, k, a, b))
    rows.sort(reverse=True, key=lambda x: x[0])
    print("%6s %-62s %3s %6s %6s %4s %4s" %
          ("diff", "function", "k", "olen", "len", "orow", "urow"))
    for nd, fn, k, a, b in rows:
        print("%6d %-62s %3d %6d %6d %4d %4d" %
              (nd, name_of(syms, fn), k, a["len"], b["len"], a["rows"], b["rows"]))
    print("TOTAL diff bytes %d in %d records" %
          (sum(r[0] for r in rows), len(rows)))


if __name__ == "__main__":
    main()
