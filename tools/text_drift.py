#!/usr/bin/env python3
"""text_drift.py -- report where our .text layout diverges from the original.

Both .so files expose their functions in .dynsym with addresses.  Sorting the original's
exports by address gives the original layout; the same for ours.  For every pair of
consecutive exports we compare the *gap* (bytes between the end of one function and the
start of the next).  A gap difference means the local (static / internal-linkage) code
between those two exports differs in size, or an object's section order differs.

Usage: text_drift.py [--original ORIG] [--ours OURS] [--objdump OD] [--min N]
"""
import argparse
import re
import subprocess

PAT = re.compile(r"^([0-9a-f]{8}) \S+\s+DF\s+\.text\s+([0-9a-f]{8})\s+\S+\s+(\S+)$")


def syms(objdump, path):
    out = subprocess.run([objdump, "-T", path], capture_output=True, text=True).stdout
    d = {}
    for line in out.splitlines():
        m = PAT.match(line.strip())
        if m:
            d[m.group(3)] = (int(m.group(1), 16), int(m.group(2), 16))
    return d


def demangle(names):
    if not names:
        return {}
    out = subprocess.run(["c++filt"] + names, capture_output=True, text=True).stdout
    return dict(zip(names, out.splitlines()))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--original", default="../_work/bins/android_2.0.16_libgof2hdaa.so")
    ap.add_argument("--ours", default="cmake-build-match/libgof2hdaa.so")
    ap.add_argument("--objdump", default=".cache/ndk/android-ndk-r18b/toolchains/"
                    "arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/"
                    "arm-linux-androideabi-objdump")
    ap.add_argument("--min", type=int, default=1,
                    help="only report gap deltas with |delta| >= N (default 1)")
    a = ap.parse_args()
    o = syms(a.objdump, a.original)
    u = syms(a.objdump, a.ours)
    rows = sorted((o[k][0], k) for k in o if k in u)
    dem = demangle([k for _, k in rows])
    drift = 0
    shown = 0
    print(f"{'orig_gap':>8} {'our_gap':>8} {'delta':>6}  after")
    for i in range(len(rows) - 1):
        a0, k = rows[i]
        a1, k1 = rows[i + 1]
        b, s = u[k]
        b1, s1 = u[k1]
        go = a1 - (a0 + o[k][1])
        gu = b1 - (b + s)
        d = gu - go
        drift += d
        if abs(d) >= a.min:
            shown += 1
            print(f"{go:8d} {gu:8d} {d:+6d}  {a0:#x} {dem[k]}")
    print(f"\n{shown} gap mismatches of {len(rows) - 1}; cumulative drift {drift:+d} B")
    print(f"orig .text end {hex(rows[-1][0] + o[rows[-1][1]][1])}, "
          f"ours {hex(u[rows[-1][1]][0] + u[rows[-1][1]][1])}")


if __name__ == "__main__":
    main()
