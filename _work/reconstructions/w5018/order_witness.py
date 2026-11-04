#!/usr/bin/env python3
"""Independent link-order witnesses for libgof2hdaa.so (w5018).

For each of .text/.rodata/.data/.data.rel.ro/.bss, map every defined strong
symbol in each relocatable object to its address in the shipped original
.dynsym, then sort objects by the lowest such address.  The resulting object
sequence must equal tools/source_order.txt filtered to the objects that have a
witness in that section.

Usage: python3 order_witness.py <repo-root> <original.so> <build-dir>
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
    rows = []
    for pos, src in enumerate(srcs):
        p = os.path.join(build, "CMakeFiles/gof2.dir/./" + src + ".obj")
        addrs = {".text": [], ".rodata": [], ".data": [], ".data.rel.ro": [], ".bss": []}
        for name, val, size, bind, typ, sec in obj_syms(p):
            if bind not in (1, 2):
                continue
            if sec.startswith(".text") and not (bind == 1 and typ == 2):
                continue
            if not sec.startswith(".text") and bind != 1:
                continue
            if name not in dyn:
                continue
            a = dyn[name]["value"] & ~1
            if sec.startswith(".text"):
                key = ".text"
            elif sec.startswith(".rodata"):
                key = ".rodata"
            elif sec.startswith(".data.rel.ro"):
                key = ".data.rel.ro"
            elif sec.startswith(".data"):
                key = ".data"
            elif sec.startswith(".bss"):
                key = ".bss"
            else:
                continue
            addrs[key].append(a)
        rows.append((pos, src, addrs))
    for sec in [".text", ".rodata", ".data", ".data.rel.ro", ".bss"]:
        keyed = []
        for pos, src, addrs in rows:
            if addrs[sec]:
                keyed.append((min(addrs[sec]), pos, src))
        keyed.sort()
        order = [src for _, _, src in keyed]
        ref = [s for s in srcs if s in set(order)]
        sm = difflib.SequenceMatcher(None, ref, order, autojunk=False)
        print(f"{sec}: {len(order)} objects  order_match={ref == order}  ratio={sm.ratio():.6f}")
        if ref != order:
            for tag, i1, i2, j1, j2 in sm.get_opcodes():
                if tag != "equal":
                    print("  ", tag, ref[i1:i2][:6], order[j1:j2][:6])


if __name__ == "__main__":
    main()
