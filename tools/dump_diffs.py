#!/usr/bin/env python3
"""Batch side-by-side diff dumper for fleet workers.

Reads mangled names (one per line, or "name<TAB>anything" — first field wins)
from a pool file, runs tools/verify/verify.py --show for each, and writes one
<mangled>.txt per function into the output directory. Workers get these dumps
instead of Ghidra MCP access (both sides' full disassembly is in the dump).

Usage:
  python3 tools/dump_diffs.py --pool pool.txt --out dumps/dir \
      [--build-dir cmake-build-match/verify] [--original ../_work/bins/...] \
      [--ours cmake-build-match/libgof2hdaa.so]
"""
import argparse, os, subprocess, sys

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NDK_OBJDUMP = os.path.join(
    REPO, ".cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/"
    "prebuilt/darwin-x86_64/bin/arm-linux-androideabi-objdump")

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--pool", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--build-dir", default=os.path.join(REPO, "cmake-build-match/verify"))
    ap.add_argument("--original", default=os.path.join(REPO, "../_work/bins/android_2.0.16_libgof2hdaa.so"))
    ap.add_argument("--ours", default=os.path.join(REPO, "cmake-build-match/libgof2hdaa.so"))
    ap.add_argument("--objdump", default=NDK_OBJDUMP)
    args = ap.parse_args()

    names = []
    for line in open(args.pool):
        line = line.strip()
        if line and not line.startswith("#"):
            names.append(line.split()[0])
    os.makedirs(args.out, exist_ok=True)
    fail = 0
    for n in names:
        out = subprocess.run(
            [sys.executable, os.path.join(REPO, "tools/verify/verify.py"),
             "--build-dir", args.build_dir, "--original", args.original,
             "--ours", args.ours, "--objdump", args.objdump, "--show", n],
            capture_output=True, text=True)
        path = os.path.join(args.out, n + ".txt")
        with open(path, "w") as f:
            f.write(out.stdout)
            if out.returncode != 0:
                f.write("\n[dump_diffs] verify.py stderr:\n" + out.stderr)
        if out.returncode != 0:
            fail += 1
            print(f"WARN {n}: verify.py exit {out.returncode}", file=sys.stderr)
    print(f"dumped {len(names) - fail}/{len(names)} to {args.out}")

if __name__ == "__main__":
    main()
