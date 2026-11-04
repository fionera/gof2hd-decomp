#!/usr/bin/env python3
"""Contiguity/adjacency witness for .rodata/.data/.data.rel.ro/.bss (w5018).

For each section, map every defined strong data symbol in each object to its
original address.  Check that each object's symbols form one contiguous block
(no other object's symbols interleaved) and that the block sequence equals
tools/source_order.txt.

Usage: python3 data_adjacency_witness.py <repo-root> <original.so> <build-dir>
"""
import difflib
import os
import struct
import sys

sys.path.insert(0, os.path.join(sys.argv[1], "tools", "verify"))
from elf import Elf32  # noqa: E402


def obj_syms(path):
    d = open(path, "rb").read()
    shoff = struct.unpack_from("<I", d, 0x20)[0]
    ents = struct.unpack_from("<H", d, 0x2E)[0]
    n = struct.unpack_from("<H", d, 0x30)[0]
    si = struct.unpack_from("<H", d, 0x32)[0]
    sh = [struct.unpack_from("<IIIIIIIIII", d, shoff + i * ents) for i in range(n)]
    st = sh[si]
    stb = d[st[4]:st[4] + st[5]]

    def sn(x):
        e = stb.find(b"\0", x)
        return stb[x:e].decode("utf8", "replace")

    names = [sn(s[0]) for s in sh]
    symsec = next(i for i, s in enumerate(sh) if s[1] == 2)
    symsh = sh[symsec]
    strtab = sh[symsh[6]]
    sb = strtab[4]

    def cstr(x):
        e = d.find(b"\0", sb + x)
        return d[sb + x:e].decode("utf8", "replace")

    out = []
    for off in range(symsh[4], symsh[4] + symsh[5], symsh[9] or 16):
        st_name, st_value, st_size = struct.unpack_from("<III", d, off)
        info = d[off + 12]
        shndx = struct.unpack_from("<H", d, off + 14)[0]
        if shndx == 0 or shndx >= 0xFF00:
            continue
        out.append((cstr(st_name), st_value, st_size, info >> 4, info & 0xF, names[shndx]))
    return out


def main():
    root, orig_path, build = sys.argv[1], sys.argv[2], sys.argv[3]
    orig = Elf32(orig_path)
    dyn = {}
    for s in orig._symbols(11):
        if s["name"] and s["shndx"] != 0:
            dyn.setdefault(s["name"], s)
    srcs = [l.strip() for l in open(os.path.join(root, "tools/source_order.txt"))
            if l.strip() and not l.startswith("#")]
    for secname in [".rodata", ".data", ".data.rel.ro", ".bss"]:
        rows = []
        for pos, src in enumerate(srcs):
            p = os.path.join(build, "CMakeFiles/gof2.dir/./" + src + ".obj")
            addrs = []
            for nm, val, size, bind, typ, sec in obj_syms(p):
                if bind != 1:
                    continue
                if secname == ".rodata" and not sec.startswith(".rodata"):
                    continue
                if secname == ".data" and not (sec == ".data" or
                                               (sec.startswith(".data.") and
                                                not sec.startswith(".data.rel.ro"))):
                    continue
                if secname == ".data.rel.ro" and not sec.startswith(".data.rel.ro"):
                    continue
                if secname == ".bss" and not sec.startswith(".bss"):
                    continue
                if nm in dyn:
                    addrs.append((dyn[nm]["value"], nm, size))
            if addrs:
                addrs.sort()
                rows.append((pos, src, addrs))
        allpts = []
        for pos, src, addrs in rows:
            for a, nm, sz in addrs:
                allpts.append((a, pos, src, nm, sz))
        allpts.sort()
        blocks = []
        for a, pos, src, nm, sz in allpts:
            if not blocks or blocks[-1][0] != pos:
                blocks.append([pos, src, []])
            blocks[-1][2].append((a, nm, sz))
        inter = [b for b in blocks if len({x[0] for x in b[2]}) != 1 or b[2] != sorted(b[2])]
        seq = [src for pos, src, addrs in blocks]
        ref = [s for s in srcs if s in set(seq)]
        print(f"{secname}: objects {len(rows)} blocks {len(blocks)} "
              f"contiguous {len(blocks) == len(rows)} order_match {ref == seq}")
        if ref != seq:
            for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, ref, seq, autojunk=False).get_opcodes():
                if tag != "equal":
                    print("  ", tag, ref[i1:i2][:6], seq[j1:j2][:6])


if __name__ == "__main__":
    main()
