#!/usr/bin/env python3
"""Restore gold's Stringpool .shstrtab order after a bfd strip.

Usage: reorder_shstrtab.py <gold-unstripped.so> <stripped.so>

The bfd strip rewrites .shstrtab into section-header order.  Gold's own
Stringpool order is still present in the unstripped gold output; this
script filters that order down to the sections that survive the strip and
rewrites the stripped file's .shstrtab (and every sh_name) to match it.
"""
import struct
import sys


def parse(path):
    data = bytearray(open(path, "rb").read())
    shoff = struct.unpack_from("<I", data, 0x20)[0]
    shentsize = struct.unpack_from("<H", data, 0x2E)[0]
    shnum = struct.unpack_from("<H", data, 0x30)[0]
    shstrndx = struct.unpack_from("<H", data, 0x32)[0]
    shstr = shoff + shstrndx * shentsize
    stroff = struct.unpack_from("<I", data, shstr + 16)[0]
    strsize = struct.unpack_from("<I", data, shstr + 20)[0]
    strtab = bytes(data[stroff:stroff + strsize])
    sections = []
    for i in range(shnum):
        base = shoff + i * shentsize
        nameoff = struct.unpack_from("<I", data, base)[0]
        end = strtab.index(b"\0", nameoff)
        sections.append((i, base, strtab[nameoff:end].decode()))
    return data, shoff, shentsize, shnum, shstr, stroff, strsize, strtab, sections


def find_name_offset(strtab, name):
    needle = name.encode()
    pos = 0
    while True:
        pos = strtab.find(needle, pos)
        if pos < 0:
            raise SystemExit("section name %r not found in gold .shstrtab" % name)
        if pos + len(needle) < len(strtab) and strtab[pos + len(needle)] == 0:
            return pos
        pos += 1


def main():
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    gold_path, strip_path = sys.argv[1], sys.argv[2]
    _, _, _, _, _, _, _, gold_str, gold_secs = parse(gold_path)
    data, shoff, shentsize, shnum, shstr, stroff, strsize, _, secs = parse(strip_path)

    # Relative order of the surviving section names in gold's Stringpool.
    ordered = []
    for _i, _base, name in secs:
        if not name:
            continue
        ordered.append((find_name_offset(gold_str, name), name))
    ordered.sort()

    # Rebuild the string table with gold's suffix sharing.
    out = bytearray(b"\0")
    offsets = {}
    for _off, name in ordered:
        needle = name.encode()
        shared = None
        for prev, prev_off in offsets.items():
            prev_bytes = prev.encode()
            if len(prev_bytes) >= len(needle) and prev_bytes.endswith(needle):
                shared = prev_off + len(prev_bytes) - len(needle)
                break
        if shared is None:
            offsets[name] = len(out)
            out += needle + b"\0"
        else:
            offsets[name] = shared

    if len(out) != strsize:
        raise SystemExit("reordered .shstrtab size %d != %d" % (len(out), strsize))
    data[stroff:stroff + strsize] = out
    for _i, base, name in secs:
        struct.pack_into("<I", data, base, offsets[name] if name else 0)
    open(strip_path, "wb").write(data)


if __name__ == "__main__":
    main()
