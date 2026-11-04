#!/usr/bin/env python3
"""Independent .rel.plt link-order witness + rotation analysis (w5018).

Gold assigns PLT slots in first-reference order.  Walk the relocatable objects
in tools/source_order.txt order, collecting PC24/THM_CALL/CALL/JUMP24/
THM_JUMP24 relocations in section order, and record the first object that
references each symbol.  Compare that object sequence with source_order.

Then print the exact .rel.plt slot table around the known 6-symbol rotation and
the first call site of each rotated symbol in both binaries.

Usage: python3 relplt_witness.py <repo-root> <original.so> <ours.so> <build-dir>
"""
import os
import re
import struct
import subprocess
import sys

sys.path.insert(0, os.path.join(sys.argv[1], "tools", "verify"))
from elf import Elf32  # noqa: E402

TARGETS = [
    "_ZN10StarSystem20switchPlanetForIntroEv",
    "_ZN5Level20switchSkyboxForIntroEv",
    "_ZN21ParticleSystemManager5resetEv",
    "_ZN9PlayerEgo18startSmokeEmissionEv",
    "_ZN13PlayerStation10setVisibleEb",
    "_ZN9PlayerEgo17deleteHackingGameEv",
]


def obj_relocs(path):
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

    syms = []
    for off in range(symsh[4], symsh[4] + symsh[5], symsh[9] or 16):
        st_name, st_value, st_size = struct.unpack_from("<III", d, off)
        info = d[off + 12]
        shndx = struct.unpack_from("<H", d, off + 14)[0]
        syms.append((cstr(st_name), info >> 4, info & 0xF, shndx))
    out = []
    for i, s in enumerate(sh):
        if s[1] != 9:  # SHT_REL
            continue
        for off in range(s[4], s[4] + s[5], 8):
            r_off, r_info = struct.unpack_from("<II", d, off)
            typ = r_info & 0xFF
            idx = r_info >> 8
            if idx == 0 or idx >= len(syms):
                continue
            nm, bind, typ2, shndx = syms[idx]
            if typ in (1, 10, 28, 29, 30):
                out.append((r_off, nm))
    return out


def relplt(path):
    e = Elf32(path)
    names = [s["name"] for s in e._symbols(11)]
    sec = next(s for s in e.sections if s["name"] == ".rel.plt")
    out = []
    for i, off in enumerate(range(sec["offset"], sec["offset"] + sec["size"], 8)):
        r_off, r_info = struct.unpack_from("<II", e.data, off)
        out.append((i, names[r_info >> 8]))
    return out


def pltmap(path):
    e = Elf32(path)
    names = [s["name"] for s in e._symbols(11)]
    sec = next(s for s in e.sections if s["name"] == ".rel.plt")
    plt = next(s for s in e.sections if s["name"] == ".plt")["addr"]
    m = {}
    for i, off in enumerate(range(sec["offset"], sec["offset"] + sec["size"], 8)):
        r_off, r_info = struct.unpack_from("<II", e.data, off)
        m[names[r_info >> 8]] = plt + 0x14 + i * 12
    return m


def callsites(path, targets):
    m = pltmap(path)
    out = subprocess.run(
        ["llvm-objdump", "-d", "--triple=thumbv7-none-linux-androideabi",
         "--start-address=0x137d18", "--stop-address=0x145ca8", path],
        capture_output=True, text=True).stdout
    res = {}
    for line in out.splitlines():
        mm = re.match(r"\s+([0-9a-f]+):.*blx\s+0x([0-9a-f]+)", line)
        if not mm:
            continue
        addr, tgt = int(mm.group(1), 16), int(mm.group(2), 16)
        for t in targets:
            if m.get(t) == tgt:
                res.setdefault(t, []).append(addr)
    return res


def main():
    root, orig_path, ours_path, build = sys.argv[1:5]
    srcs = [l.strip() for l in open(os.path.join(root, "tools/source_order.txt"))
            if l.strip() and not l.startswith("#")]
    seen = {}
    seq = []
    for pos, src in enumerate(srcs):
        p = os.path.join(build, "CMakeFiles/gof2.dir/./" + src + ".obj")
        for _, nm in obj_relocs(p):
            if nm not in seen:
                seen[nm] = pos
                seq.append(pos)
    objseq = []
    for pos in seq:
        if not objseq or objseq[-1] != pos:
            objseq.append(pos)
    ref = [pos for pos in range(len(srcs)) if pos in set(objseq)]
    print(f"first-reference object sequence matches source_order: {ref == objseq}")

    A = relplt(orig_path)
    B = relplt(ours_path)
    diffs = [i for i in range(len(A)) if A[i][1] != B[i][1]]
    print(f".rel.plt slots {len(A)}  differing slots {diffs}")
    for i in diffs:
        print(f"  {i}: orig {A[i][1]}  ours {B[i][1]}")
    ca = callsites(orig_path, TARGETS)
    cb = callsites(ours_path, TARGETS)
    print("\nfirst call site (function offset, LevelScript::process @ 0x137d18):")
    for t in TARGETS:
        print(f"  {t}")
        print(f"    orig {[hex(x) for x in ca.get(t, [])]} -> {[hex(x - 0x137D18) for x in ca.get(t, [])]}")
        print(f"    ours {[hex(x) for x in cb.get(t, [])]} -> {[hex(x - 0x137D18) for x in cb.get(t, [])]}")


if __name__ == "__main__":
    main()
