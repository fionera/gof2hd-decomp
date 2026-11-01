#!/usr/bin/env python3
"""Reconstruct within-TU function order from .ARM.extab.

The original was linked without a section-ordering file, so its .text order is
its compile order.  With -ffunction-sections each function's .ARM.extab record
is emitted in the object's function order, and the section-ordering file lists
only .text* sections, so the final .ARM.extab order is the *source* function
order.  This script sorts the extab records by extab address, maps each function
to its owning object, and checks whether the sequence is ascending in .text
address.

Usage (from the repo root):
  python3 _work/reconstructions/w3560/extab_order.py \
      ../_work/bins/android_2.0.16_libgof2hdaa.so \
      /tmp/ours_unstripped.so \
      cmake-build-match/link_order.rsp

The unstripped .so is produced by running the gold link command without the
final `strip --strip-all` (see the note's reproduction section).
"""
import difflib
import os
import struct
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from symtab import parse  # noqa: E402


def sections(path):
    data = open(path, 'rb').read()
    shoff = struct.unpack_from('<I', data, 0x20)[0]
    shentsize = struct.unpack_from('<H', data, 0x2e)[0]
    shnum = struct.unpack_from('<H', data, 0x30)[0]
    shstrndx = struct.unpack_from('<H', data, 0x32)[0]
    out = []
    for i in range(shnum):
        off = shoff + i * shentsize
        v = struct.unpack_from('<IIIIIIIIII', data, off)
        out.append(dict(name=v[0], type=v[1], flags=v[2], addr=v[3], offset=v[4],
                        size=v[5], link=v[6], info=v[7], align=v[8], entsize=v[9]))
    st = out[shstrndx]
    for s in out:
        s['sname'] = data[st['offset'] + s['name']:data.index(b'\0', st['offset'] + s['name'])].decode()
    return data, {s['sname']: s for s in out}


def prel31(x, addr):
    v = x & 0x7fffffff
    if v & 0x40000000:
        v -= 0x80000000
    return (addr + v) & 0xffffffff


def extab_sequence(path):
    """[(extab_addr, function_addr)] in extab-address order."""
    data, secs = sections(path)
    exidx = secs['.ARM.exidx']
    rows = []
    for i in range(exidx['size'] // 8):
        w0, w1 = struct.unpack_from('<II', data, exidx['offset'] + i * 8)
        fn = prel31(w0, exidx['addr'] + i * 8)
        if w1 == 1 or (w1 & 0x80000000):
            continue  # cantunwind / inline compact
        rows.append((prel31(w1, exidx['addr'] + i * 8 + 4), fn))
    rows.sort()
    return [fn for _, fn in rows]


def main():
    orig_so, ours_unstripped, rsp = sys.argv[1], sys.argv[2], sys.argv[3]
    addr_name = {}
    for s in parse(ours_unstripped):
        if s['typ'] == 2 and s['name'] and s['shndx'] != 0:
            addr_name.setdefault(s['value'] & ~1, s['name'])
    owner = {}
    for obj in [l.strip() for l in open(rsp) if l.strip()]:
        for s in parse(obj):
            if s['bind'] in (1, 2) and s['name'] and s['shndx'] != 0:
                owner.setdefault(s['name'], os.path.basename(obj))

    def by_tu(seq):
        out = defaultdict(list)
        for f in seq:
            out[owner.get(addr_name.get(f, ''), '?')].append(f)
        return out

    for label, path in (('ORIG', orig_so), ('OURS', ours_unstripped)):
        seq = extab_sequence(path)
        groups = by_tu(seq)
        bad = [(tu, fs) for tu, fs in groups.items() if fs != sorted(fs)]
        print(f"{label}: extab records {len(seq)}  TUs {len(groups)}  not-address-sorted {len(bad)}")
        if label == 'OURS':
            ranked = []
            for tu, fs in bad:
                inv = sum(1 for i in range(len(fs)) for j in range(i + 1, len(fs)) if fs[i] > fs[j])
                ranked.append((inv, tu, len(fs), fs))
            for inv, tu, n, fs in sorted(ranked, reverse=True)[:25]:
                first = next((i for i in range(n - 1) if fs[i] > fs[i + 1]), None)
                print(f"  {tu:40} funcs {n:4} inversions {inv:5} first@{first}")
                if first is not None:
                    for k in range(max(0, first - 1), min(n, first + 3)):
                        print(f"      {fs[k]:#x} {addr_name.get(fs[k], '?')}")

    o = extab_sequence(orig_so)
    u = extab_sequence(ours_unstripped)
    sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
    blocks = [b for b in sm.get_matching_blocks() if b.size]
    print(f"function-level: {len(o)}/{len(u)} ratio {sm.ratio():.6f} "
          f"matched {sum(b.size for b in blocks)} gaps {len(blocks) - 1}")


if __name__ == '__main__':
    main()
