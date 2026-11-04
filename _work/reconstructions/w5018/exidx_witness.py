#!/usr/bin/env python3
"""Independent .ARM.exidx link-order witness (w5018).

Each exidx entry's word0 is a prel31 pointer to the function it covers.  Map
those function addresses to the defining object's strong .text symbols, then
check (a) the object sequence equals tools/source_order.txt and (b) each
object's exidx function addresses are strictly ascending.

Usage: python3 exidx_witness.py <repo-root> <original.so> <build-dir>
"""
import os
import struct
import sys

sys.path.insert(0, os.path.join(sys.argv[1], "tools", "verify"))
from elf import Elf32  # noqa: E402


def obj_funcs(path):
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

    out = set()
    for off in range(symsh[4], symsh[4] + symsh[5], symsh[9] or 16):
        st_name, st_value, st_size = struct.unpack_from("<III", d, off)
        info = d[off + 12]
        shndx = struct.unpack_from("<H", d, off + 14)[0]
        if shndx == 0 or shndx >= 0xFF00:
            continue
        if (info >> 4) == 1 and (info & 0xF) == 2 and names[shndx].startswith(".text"):
            out.add(cstr(st_name))
    return out


def main():
    root, orig_path, build = sys.argv[1], sys.argv[2], sys.argv[3]
    orig = Elf32(orig_path)
    dyn = {}
    for s in orig._symbols(11):
        if s["name"] and s["shndx"] != 0:
            dyn.setdefault(s["name"], s["value"] & ~1)
    sec = next(s for s in orig.sections if s["name"] == ".ARM.exidx")
    fns = []
    for i in range(sec["size"] // 8):
        off = sec["offset"] + i * 8
        w0, w1 = struct.unpack_from("<II", orig.data, off)
        x = w0 & 0x7FFFFFFF
        if x & (1 << 30):
            x -= 1 << 31
        fns.append((sec["addr"] + i * 8 + x) & 0xFFFFFFFF)
    srcs = [l.strip() for l in open(os.path.join(root, "tools/source_order.txt"))
            if l.strip() and not l.startswith("#")]
    owner = {}
    for pos, src in enumerate(srcs):
        p = os.path.join(build, "CMakeFiles/gof2.dir/./" + src + ".obj")
        for name in obj_funcs(p):
            if name in dyn:
                owner[dyn[name]] = pos
    seq = []
    per = {}
    mapped = 0
    for a in fns:
        if a not in owner:
            continue
        mapped += 1
        pos = owner[a]
        if not seq or seq[-1] != pos:
            seq.append(pos)
        per.setdefault(pos, []).append(a)
    ref = [pos for pos in range(len(srcs)) if pos in set(seq)]
    nonasc = sum(1 for v in per.values() if v != sorted(v))
    print(f"exidx entries mapped {mapped}  objects {len(per)}  "
          f"object_order_matches_source_order {ref == seq}  "
          f"objects_with_nonascending_addrs {nonasc}")


if __name__ == "__main__":
    main()
