#!/usr/bin/env python3
"""Read the original 64-entry g_ips_shipTypeMap and compare it to Level.cpp.

Usage:
  python3 _work/reconstructions/g_ips_shipTypeMap_64.py [--source src/game/world/Level.cpp]

The original table is at VMA/file offset 0x1fc3d0 in
../_work/bins/android_2.0.16_libgof2hdaa.so (`.rodata` is VMA == file offset).
"""
import argparse
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ORIGINAL = ROOT.parent / "_work" / "bins" / "android_2.0.16_libgof2hdaa.so"
TABLE_VMA = 0x1FC3D0
TABLE_WORDS = 64


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--source", default=str(ROOT / "src/game/world/Level.cpp"))
    args = ap.parse_args()

    data = ORIGINAL.read_bytes()
    original = list(struct.unpack_from(f"<{TABLE_WORDS}I", data, TABLE_VMA))
    print("original g_ips_shipTypeMap[64]:")
    print(original)

    src = Path(args.source).read_text()
    m = re.search(
        r"static const uint32_t g_ips_shipTypeMap\[(\d+)\] = \{(.*?)\n    \};",
        src,
        re.S,
    )
    if not m:
        print("source table not found")
        return 1
    n = int(m.group(1))
    source = [int(x.strip()) for x in m.group(2).replace("\n", " ").split(",") if x.strip()]
    print(f"source g_ips_shipTypeMap[{n}]:")
    print(source)
    print("match:", n == TABLE_WORDS and source == original)
    return 0 if n == TABLE_WORDS and source == original else 1


if __name__ == "__main__":
    raise SystemExit(main())
