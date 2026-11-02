#!/usr/bin/env python3
"""Report exported-function address matches and the exact-address prefix.

Usage:
  addr_match.py ORIGINAL.so OURS.so [...]
"""
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
NM = ROOT / ".cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm"


def syms(path):
    out = {}
    r = subprocess.run([str(NM), "-nD", "--defined-only", path],
                       capture_output=True, text=True)
    for ln in r.stdout.splitlines():
        p = ln.split(maxsplit=2)
        if len(p) == 3 and p[1] in "tTwW":
            out[p[2]] = int(p[0], 16) & ~1
    return out


def main():
    orig = syms(sys.argv[1])
    for path in sys.argv[2:]:
        ours = syms(path)
        common = set(orig) & set(ours)
        same = sum(1 for n in common if orig[n] == ours[n])
        rows = sorted((orig[n], n) for n in common)
        prefix = 0
        for a, n in rows:
            if ours[n] != a:
                break
            prefix += 1
        first = next(((a, n, ours[n]) for a, n in rows if ours[n] != a), None)
        print(f"{path}: {same}/{len(common)} same; prefix {prefix}; first {first}")


if __name__ == "__main__":
    main()
