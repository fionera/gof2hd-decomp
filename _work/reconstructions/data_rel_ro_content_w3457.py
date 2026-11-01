#!/usr/bin/env python3
"""Attribute raw .data.rel.ro differences to their owning input sections.

The verify gate's `data: exact N of M exported objects` is relocation-normalized.
This script compares the raw section bytes and uses a gold -Map plus an unstripped
link (for symbol names) to say which input section owns each differing word and
which section/address it points at.

Usage (from the repo root):
  python3 _work/reconstructions/data_rel_ro_content_w3457.py \
      --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
      --ours cmake-build-match/libgof2hdaa.so \
      --map /tmp/w3457/data.map \
      --unstripped /tmp/w3457/unstripped.so

The map/unstripped pair is produced by the build's own gold command with only
`-Map=...` added and the final strip removed; see the companion note.
"""
import argparse
import collections
import os
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.verify.elf import (  # noqa: E402
    Elf32, SHT_REL, R_ARM_ABS32, R_ARM_GLOB_DAT, R_ARM_RELATIVE,
)


def parse_map(path):
    """Return [(section, addr, size, owner)] for allocated map rows."""
    rows = []
    cur = None
    for line in Path(path).read_text(errors="replace").splitlines():
        m = re.match(r"^ (\.\S+)\s*$", line)
        if m:
            cur = m.group(1)
            continue
        m = re.match(r"^ (\.\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+(.+))?$", line)
        if m:
            owner = (m.group(4) or "").strip()
            if owner:
                rows.append((m.group(1), int(m.group(2), 16), int(m.group(3), 16), owner))
            cur = None
            continue
        m = re.match(r"^ \s+0x([0-9a-f]+)\s+0x([0-9a-f]+)(?:\s+(.+))?$", line)
        if m and cur:
            owner = (m.group(3) or "").strip()
            if owner:
                rows.append((cur, int(m.group(1), 16), int(m.group(2), 16), owner))
            cur = None
    return rows


def owner_at(rows, addr):
    for name, a, size, owner in rows:
        if a <= addr < a + size:
            return name, a, size, owner
    return None


def relocs(elf):
    names = [s["name"] for s in elf._symbols(11)]
    out = {}
    for sec in elf.sections:
        if sec["type"] != SHT_REL or sec["name"] == ".rel.plt":
            continue
        for off in range(sec["offset"], sec["offset"] + sec["size"], 8):
            r_offset, r_info = struct.unpack_from("<II", elf.data, off)
            idx = r_info >> 8
            out[r_offset] = (r_info & 0xff, idx, names[idx] if idx < len(names) else "?")
    return out


def sec_of(elf, value):
    for sec in elf.sections:
        if sec["addr"] and sec["addr"] <= value < sec["addr"] + sec["size"]:
            return sec["name"]
    return None


def load_symbols(path, nm):
    text = subprocess.check_output([nm, "-n", "-C", path], text=True, errors="replace")
    syms = []
    for line in text.splitlines():
        parts = line.split(None, 2)
        if len(parts) != 3:
            continue
        try:
            value = int(parts[0], 16)
        except ValueError:
            continue
        syms.append((value, parts[2]))
    syms.sort()
    return syms


def sym_at(syms, value):
    best = None
    for addr, name in syms:
        if addr <= value:
            best = (addr, name)
        else:
            break
    return best[1] if best else "?"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--orig", required=True)
    ap.add_argument("--ours", required=True)
    ap.add_argument("--map", required=True)
    ap.add_argument("--unstripped", required=True)
    ap.add_argument("--nm", default="nm")
    args = ap.parse_args()

    eo, eu = Elf32(args.orig), Elf32(args.ours)
    so = {s["name"]: s for s in eo.sections}
    su = {s["name"]: s for s in eu.sections}
    ro, ru = so[".data.rel.ro"], su[".data.rel.ro"]
    if ro["addr"] != ru["addr"] or ro["size"] != ru["size"]:
        raise SystemExit(".data.rel.ro size/base differ; raw-word attribution is not aligned")
    base, size = ro["addr"], ro["size"]
    od = eo.data[ro["offset"]:ro["offset"] + size]
    ud = eu.data[ru["offset"]:ru["offset"] + size]
    diffs = [i for i in range(size) if od[i] != ud[i]]
    words = sorted(set((base + i) // 4 * 4 for i in diffs))
    print(f".data.rel.ro base={base:#x} size={size:#x} diff_bytes={len(diffs)} "
          f"words={len(words)} first={base + diffs[0]:#x}" if diffs else
          f".data.rel.ro base={base:#x} size={size:#x} diff_bytes=0")

    rlo, rlu = relocs(eo), relocs(eu)
    type_pairs = collections.Counter((rlo.get(w, (None,))[0], rlu.get(w, (None,))[0])
                                     for w in words)
    print("reloc type pairs:", dict(type_pairs))

    rows = parse_map(args.map)
    syms = load_symbols(args.unstripped, args.nm)
    by = collections.Counter()
    by_bytes = collections.Counter()
    examples = collections.defaultdict(list)
    for word in words:
        off = word - base
        ow = struct.unpack_from("<I", od, off)[0]
        uw = struct.unpack_from("<I", ud, off)[0]
        nb = sum(1 for j in range(4) if off + j < size and od[off + j] != ud[off + j])
        owner = owner_at(rows, word)
        target = owner_at(rows, uw)
        owner_name = os.path.basename(owner[3]).replace(".marked", "") if owner else "?"
        target_name = os.path.basename(target[3]).replace(".marked", "") if target else "?"
        key = (owner_name, owner[0] if owner else "?",
               sec_of(eu, uw), target_name, uw - ow)
        by[key] += 1
        by_bytes[key] += nb
        if len(examples[key]) < 3:
            examples[key].append((word, ow, uw))

    print("\nby owner / owner-section / target-section / target-owner / delta:")
    for key, count in by_bytes.most_common():
        print(f"{count:4d} bytes {by[key]:4d} words {key}")
        for word, ow, uw in examples[key]:
            print(f"    {word:#08x} {ow:#08x} -> {uw:#08x} {sym_at(syms, uw)}")

    print("\nby owner totals:")
    totals = collections.Counter()
    for key, count in by_bytes.items():
        totals[key[0]] += count
    for name, count in totals.most_common():
        print(f"{count:4d} {name}")


if __name__ == "__main__":
    main()
