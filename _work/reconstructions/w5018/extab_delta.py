#!/usr/bin/env python3
"""Attribute the .ARM.extab size delta to individual LSDA records (w5018).

Each .ARM.exidx entry points (via a prel31 word) at an .ARM.extab LSDA record.
Sort the unique record addresses and take successive differences as record
sizes; compare the size of the record owned by each function between the
original and our build.  The sum of the per-function deltas must equal the
whole-section size delta.

Usage: python3 extab_delta.py <original.so> <ours.so>
"""
import struct
import sys

sys.path.insert(0, "tools/verify")
from elf import Elf32  # noqa: E402


def entries(path):
    e = Elf32(path)
    sec = next(s for s in e.sections if s["name"] == ".ARM.exidx")
    out = []
    for i in range(sec["size"] // 8):
        off = sec["offset"] + i * 8
        w0, w1 = struct.unpack_from("<II", e.data, off)

        def sx(x):
            x &= 0x7FFFFFFF
            return x - (1 << 31) if x & (1 << 30) else x

        fn = (sec["addr"] + i * 8 + sx(w0)) & 0xFFFFFFFF
        if w1 == 1:
            tgt = None
        elif w1 & 0x80000000:
            tgt = None
        else:
            tgt = (sec["addr"] + i * 8 + 4 + sx(w1)) & 0xFFFFFFFF
        out.append((fn, tgt))
    return out, e


def main():
    A, eO = entries(sys.argv[1])
    B, eU = entries(sys.argv[2])
    names = {s["value"] & ~1: s["name"] for s in eO._symbols(11) if s["name"]}
    fa = {fn: t for fn, t in A if t}
    fb = {fn: t for fn, t in B if t}

    def sizes(m):
        addrs = sorted(set(m.values()))
        d = {}
        for i, a in enumerate(addrs):
            d[a] = addrs[i + 1] - a if i + 1 < len(addrs) else None
        return d

    sa, sb = sizes(fa), sizes(fb)
    total = 0
    print(f"{'function':60} {'orig_off':>10} {'orig_sz':>8} {'our_sz':>8} {'delta':>6}")
    for fn in sorted(fa):
        if fn not in fb:
            continue
        a, b = fa[fn], fb[fn]
        if sa.get(a) != sb.get(b):
            d = sb[b] - sa[a]
            total += d
            print(f"{names.get(fn, hex(fn)):60} {a:#10x} {sa[a]:8d} {sb[b]:8d} {d:+6d}")
    print(f"{'net':60} {'':10} {'':8} {'':8} {total:+6d}")
    ext_o = next(s for s in eO.sections if s["name"] == ".ARM.extab")["size"]
    ext_u = next(s for s in eU.sections if s["name"] == ".ARM.extab")["size"]
    print(f".ARM.extab size original {ext_o:#x}  ours {ext_u:#x}  delta {ext_u - ext_o:+d}")


if __name__ == "__main__":
    main()
