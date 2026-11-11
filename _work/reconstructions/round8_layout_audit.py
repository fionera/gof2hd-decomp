#!/usr/bin/env python3
"""Round-8 build-provenance audit: per-TU object layout vs source_order.txt.

Reconstructs the original stripped .so's per-TU layout from a gold -Map of the
canonical build and compares every per-TU section chunk with the original.

Usage:
  python3 _work/reconstructions/round8_layout_audit.py \
      --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
      --map /dev/shm/w11320/gold.map \
      --map-so /dev/shm/w11320/map.so \
      --source-order tools/source_order.txt

The map is produced by re-running the canonical link command with
`-Map=<file>` and an unstripped `-o` target.  See
_work/reconstructions/build-provenance-round8-build-system.md §9.
"""
import argparse
import json
import os
import re
import struct
import subprocess
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(REPO, "tools", "verify"))
from elf import Elf32  # noqa: E402

SEC_HDR = re.compile(r"^(\S+)\s+0x([0-9a-fA-F]+)\s+0x([0-9a-fA-F]+)\s*$")
NAMEONLY = re.compile(r"^ (\S+)\s*$")
CONT = re.compile(r"^ (\S+)\s*(?:0x([0-9a-fA-F]+)\s+0x([0-9a-fA-F]+))?\s+(/\S+)\s*$")
CONT2 = re.compile(r"^\s+0x([0-9a-fA-F]+)\s+0x([0-9a-fA-F]+)\s+(/\S+)\s*$")
SYM = re.compile(r"^\s+0x([0-9a-fA-F]+)(?:\s+0x([0-9a-fA-F]+))?\s+(\S.*?)\s*$")
MERGE = re.compile(r"^ \*\* (.*)$")


def parse_map(path):
    """Return a list of input-section contributions in link order.

    Each contribution: {outsec, insec, addr, size, obj, syms:[(addr,size,name)]}.
    `** merge strings` / `** merge constants` blocks have obj=None.
    """
    outsec = None
    pending = None
    cur = None
    out = []
    for line in open(path, errors="replace"):
        line = line.rstrip("\n")
        if not line.strip():
            continue
        m = SEC_HDR.match(line)
        if m and not line.startswith(" "):
            outsec = m.group(1)
            pending = None
            cur = None
            continue
        m = MERGE.match(line)
        if m:
            pending = "** " + m.group(1)
            cur = None
            continue
        m = NAMEONLY.match(line)
        if m:
            pending = m.group(1)
            cur = None
            continue
        m = CONT.match(line)
        if m:
            name, addr, size, obj = m.groups()
            if addr is None:
                pending = name
                continue
            cur = dict(outsec=outsec, insec=name, addr=int(addr, 16),
                       size=int(size, 16), obj=obj, syms=[])
            out.append(cur)
            pending = None
            continue
        m = CONT2.match(line)
        if m:
            addr, size, obj = m.groups()
            cur = dict(outsec=outsec, insec=pending or outsec,
                       addr=int(addr, 16), size=int(size, 16),
                       obj=obj, syms=[])
            out.append(cur)
            pending = None
            continue
        m = SYM.match(line)
        if m and cur is not None:
            addr, size, name = m.groups()
            cur["syms"].append((int(addr, 16),
                                int(size, 16) if size else None, name))
    return out


def obj_to_src(obj):
    marker = "/CMakeFiles/gof2.dir/./"
    i = obj.find(marker)
    if i < 0:
        return None
    p = obj[i + len(marker):]
    return p[:-4] if p.endswith(".obj") else p


def read_at(elf, vaddr, size):
    for s in elf.sections:
        if s["addr"] and s["addr"] <= vaddr < s["addr"] + s["size"]:
            off = s["offset"] + (vaddr - s["addr"])
            return elf.data[off:off + size]
    return b""


def diff_bytes(a, b):
    n = min(len(a), len(b))
    return sum(x != y for x, y in zip(a[:n], b[:n])) + abs(len(a) - len(b))


def map_symtab(map_so):
    """addr -> [mangled names] from the unstripped map output."""
    out = {}
    txt = subprocess.check_output(["readelf", "-sW", map_so], text=True)
    for line in txt.splitlines():
        m = re.match(r"\s*\d+:\s+([0-9a-f]+)\s+(0x[0-9a-f]+|\d+)\s+FUNC\s+"
                     r"\S+\s+\S+\s+\S+\s+(.+)$", line)
        if m:
            out.setdefault(int(m.group(1), 16) & ~1, []).append(m.group(3).strip())
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--original", required=True)
    ap.add_argument("--map", required=True)
    ap.add_argument("--map-so", required=True)
    ap.add_argument("--source-order", required=True)
    args = ap.parse_args()

    orig = Elf32(args.original)
    maps = Elf32(args.map_so)
    srcs = [l.strip() for l in open(args.source_order)
            if l.strip().startswith("src/")]
    tu_index = {s: i for i, s in enumerate(srcs)}
    contribs = parse_map(args.map)
    gof2 = [c for c in contribs if obj_to_src(c["obj"])]
    for c in gof2:
        c["src"] = obj_to_src(c["obj"])

    print("== per-section per-TU chunks ==")
    for sec in [".text", ".rodata", ".data.rel.ro", ".data", ".bss",
                ".init_array", ".fini_array", ".ARM.exidx", ".ARM.extab"]:
        xs = [c for c in gof2 if c["outsec"] == sec and c["size"] > 0]
        if not xs:
            continue
        total = sum(c["size"] for c in xs)
        db = 0
        bad = 0
        for c in xs:
            if sec == ".bss":  # NOBITS: no file bytes to compare
                continue
            d = diff_bytes(read_at(orig, c["addr"], c["size"]),
                           read_at(maps, c["addr"], c["size"]))
            db += d
            bad += bool(d)
        # contiguity and TU monotonicity
        runs = []
        for c in xs:
            if not runs or runs[-1] != c["src"]:
                runs.append(c["src"])
        idx = [tu_index[s] for s in runs]
        mono = all(a < b for a, b in zip(idx, idx[1:]))
        interleaved = len(runs) != len(set(runs))
        diff_s = "n/a" if sec == ".bss" else f"{db:7d}"
        print(f"{sec:14s} contribs {len(xs):5d} TUs {len(set(c['src'] for c in xs)):3d} "
              f"bytes {total:8d} diff {diff_s:>7s} diff_contribs {bad:3d} "
              f"monotonic {mono} interleaved {interleaved}")

    print("\n== per-TU .text diff (only nonzero) ==")
    funcs = [c for c in gof2 if c["outsec"] == ".text" and c["size"] > 0]
    for s in srcs:
        xs = [c for c in funcs if c["src"] == s]
        if not xs:
            continue
        db = sum(diff_bytes(read_at(orig, c["addr"], c["size"]),
                            read_at(maps, c["addr"], c["size"])) for c in xs)
        if db:
            print(f"{s:60s} funcs {len(xs):4d} bytes {sum(c['size'] for c in xs):7d} diff {db:6d}")

    print("\n== .init_array ==")
    for c in [c for c in gof2 if c["outsec"] == ".init_array"]:
        a = read_at(orig, c["addr"], c["size"])
        b = read_at(maps, c["addr"], c["size"])
        ptr = struct.unpack("<I", a)[0] & ~1 if len(a) == 4 else None
        print(f"{c['addr']:#x} {c['src']:55s} -> {ptr and hex(ptr)} diff {diff_bytes(a, b)}")

    print("\n== exported data objects (original .dynsym) ==")
    bysec = {}
    for a, s, n in orig.objects():
        bysec.setdefault(orig.section_of(a), []).append((a, s, n))
    for sec in [".rodata", ".data.rel.ro", ".data", ".bss"]:
        xs = sorted(bysec.get(sec, []))
        seq = []
        for a, s, n in xs:
            cand = [c for c in gof2 if c["outsec"] == sec
                    and c["addr"] <= a < c["addr"] + max(c["size"], 1)]
            if cand:
                seq.append((tu_index[cand[0]["src"]], cand[0]["src"]))
        br = sum(1 for x, y in zip(seq, seq[1:]) if y[0] < x[0])
        print(f"{sec:14s} exported {len(xs):4d} mapped {len(seq):4d} "
              f"monotonic {br == 0} breaks {br}")

    print("\n== TU sequences ==")
    for sec in [".text", ".rodata", ".data.rel.ro", ".data", ".bss"]:
        xs = [c for c in gof2 if c["outsec"] == sec and c["size"] > 0]
        runs = []
        for c in xs:
            if not runs or runs[-1] != c["src"]:
                runs.append(c["src"])
        idx = [tu_index[s] for s in runs]
        br = [(a, b) for a, b in zip(idx, idx[1:]) if b < a]
        print(f"{sec:14s} runs {len(runs):3d} breaks {len(br)}")
        for a, b in br[:5]:
            print(f"   {srcs[a]} -> {srcs[b]}")


if __name__ == "__main__":
    main()
