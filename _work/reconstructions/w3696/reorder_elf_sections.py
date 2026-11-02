#!/usr/bin/env python3
"""Permute an ELF32 relocatable's `.text.*` section headers into a desired order.

Gold uses the section-header order of each relocatable input as its input order.
This script leaves every non-`.text.*` section in place and only permutes the
`.text.*` header entries among their existing slots, then rewrites every section
index reference that can point at one of those slots (ELF e_shstrndx, sh_link,
REL/RELA/GROUP sh_info, symtab st_shndx, and GROUP payload indices).

Usage:
  reorder_elf_sections.py INPUT.o OUTPUT.o SECTION_ORDER.txt
"""
import struct
import sys


def parse(path):
    data = bytearray(open(path, "rb").read())
    e_shoff, = struct.unpack_from("<I", data, 0x20)
    e_shentsize, = struct.unpack_from("<H", data, 0x2e)
    e_shnum, = struct.unpack_from("<H", data, 0x30)
    e_shstrndx, = struct.unpack_from("<H", data, 0x32)
    hdrs = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        hdrs.append(bytearray(data[off:off + e_shentsize]))
    shstr_off = struct.unpack_from("<I", hdrs[e_shstrndx], 0x10)[0]

    def name(h):
        no, = struct.unpack_from("<I", h, 0)
        end = data.find(b"\0", shstr_off + no)
        return data[shstr_off + no:end].decode()

    return data, hdrs, name, e_shoff, e_shentsize, e_shstrndx


def reorder(path, out, order):
    data, hdrs, name, e_shoff, e_shentsize, e_shstrndx = parse(path)
    pos = {n: i for i, n in enumerate(order)}
    slots = [i for i, h in enumerate(hdrs) if name(h).startswith(".text.")]
    old = [hdrs[i] for i in slots]
    new = sorted(old, key=lambda h: pos.get(name(h), 10 ** 9))

    id_to_old = {id(h): i for i, h in enumerate(hdrs)}
    remap = {i: i for i in range(len(hdrs))}
    for slot, h in zip(slots, new):
        remap[id_to_old[id(h)]] = slot
    for slot, h in zip(slots, new):
        hdrs[slot] = h

    struct.pack_into("<H", data, 0x32, remap[e_shstrndx])

    for h in hdrs:
        typ, = struct.unpack_from("<I", h, 4)
        link, = struct.unpack_from("<I", h, 0x18)
        info, = struct.unpack_from("<I", h, 0x1c)
        if link < len(hdrs):
            struct.pack_into("<I", h, 0x18, remap.get(link, link))
        if typ in (4, 9, 17):  # RELA, REL, GROUP
            if info < len(hdrs):
                struct.pack_into("<I", h, 0x1c, remap.get(info, info))

    for h in hdrs:
        typ, = struct.unpack_from("<I", h, 4)
        if typ not in (2, 11):  # SYMTAB, DYNSYM
            continue
        off, = struct.unpack_from("<I", h, 0x10)
        size, = struct.unpack_from("<I", h, 0x14)
        entsize, = struct.unpack_from("<I", h, 0x24)
        entsize = entsize or 16
        for o in range(off, off + size, entsize):
            shndx, = struct.unpack_from("<H", data, o + 14)
            if shndx and shndx < len(hdrs):
                struct.pack_into("<H", data, o + 14, remap.get(shndx, shndx))

    for h in hdrs:
        typ, = struct.unpack_from("<I", h, 4)
        if typ != 17:  # GROUP
            continue
        off, = struct.unpack_from("<I", h, 0x10)
        size, = struct.unpack_from("<I", h, 0x14)
        for o in range(off + 4, off + size, 4):
            idx, = struct.unpack_from("<I", data, o)
            if idx < len(hdrs):
                struct.pack_into("<I", data, o, remap.get(idx, idx))

    for i, h in enumerate(hdrs):
        data[e_shoff + i * e_shentsize:e_shoff + (i + 1) * e_shentsize] = h
    open(out, "wb").write(data)


def main():
    if len(sys.argv) != 4:
        raise SystemExit(__doc__)
    order = [ln.strip() for ln in open(sys.argv[3]) if ln.strip()]
    reorder(sys.argv[1], sys.argv[2], order)


if __name__ == "__main__":
    main()
