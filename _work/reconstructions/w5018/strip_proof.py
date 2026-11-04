#!/usr/bin/env python3
"""Prove the post-link strip normalization (w5018).

The shipped .so's section header table is bfd-normalized (SHF_INFO_LINK on
.rel.plt, INIT/FINI_ARRAY sh_entsize=4, unaligned NOBITS .bss sh_offset) but
its .shstrtab is in gold Stringpool order.  Neither `gold -s` alone nor
`gold + bfd strip` (without reorder) reproduces that combination.

Usage: python3 strip_proof.py <repo-root> <build-dir> <original.so>
"""
import os
import shlex
import struct
import subprocess
import sys


def hdr(path):
    d = open(path, "rb").read()
    shoff = struct.unpack_from("<I", d, 0x20)[0]
    ents = struct.unpack_from("<H", d, 0x2E)[0]
    n = struct.unpack_from("<H", d, 0x30)[0]
    si = struct.unpack_from("<H", d, 0x32)[0]
    sh = [struct.unpack_from("<IIIIIIIIII", d, shoff + i * ents) for i in range(n)]
    st = sh[si]
    shstr = d[st[4]:st[4] + st[5]]
    names = []
    for s in sh:
        e = shstr.find(b"\0", s[0])
        names.append(shstr[s[0]:e].decode())
    return d, {n: s for n, s in zip(names, sh)}, shstr


def main():
    root, build, orig = sys.argv[1], sys.argv[2], sys.argv[3]
    cmd = subprocess.run(["ninja", "-t", "commands", "libgof2hdaa.so"],
                         cwd=build, capture_output=True, text=True).stdout.strip().splitlines()[-1]
    parts = cmd.split(" && ")
    gold = shlex.split(parts[1])
    strip = shlex.split(parts[2])
    reorder = shlex.split(parts[3])
    ri = next(i for i, x in enumerate(reorder) if x.endswith("reorder_shstrtab.py"))
    reorder = reorder[:ri + 1]
    oi = gold.index("-o")
    scratch = "/dev/shm/w5018/stripproof"
    os.makedirs(scratch, exist_ok=True)
    O, om, ost = hdr(orig)
    variants = {}

    def link(tag, extra=(), strip_it=True, reorder_it=True):
        un = os.path.join(scratch, tag + ".unstripped.so")
        so = os.path.join(scratch, tag + ".so")
        t = list(gold) + list(extra)
        oi2 = t.index("-o")
        t[oi2 + 1] = un
        subprocess.run(t, check=True, capture_output=True)
        if strip_it:
            subprocess.run([strip[0], "--strip-all", "-o", so, un], check=True)
            if reorder_it:
                subprocess.run(reorder + [un, so], check=True)
        else:
            so = un
        return so

    variants["gold -s"] = link("gold_s", ["-s"], strip_it=False)
    variants["gold -s + bfd strip"] = link("gold_s_bfd", ["-s"])
    variants["gold + bfd strip"] = link("gold_bfd", [], reorder_it=False)
    variants["gold + bfd strip + reorder"] = link("gold_bfd_reorder", [])
    for tag, p in variants.items():
        d, m, st = hdr(p)
        diffs = []
        for n in om:
            if n not in m:
                diffs.append(n + ":missing")
            elif om[n] != m[n]:
                a, b = om[n], m[n]
                fields = ["name", "type", "flags", "addr", "offset", "size",
                          "link", "info", "align", "entsize"]
                diffs.append(n + ":" + ",".join(
                    f"{fields[i]}={a[i]:x}/{b[i]:x}" for i in range(10) if a[i] != b[i]))
        print(f"{tag:32} shstrtab {'SAME' if st == ost else 'DIFF'}  "
              f"hdrs {'SAME' if not diffs else 'DIFF'}")
        for x in diffs[:4]:
            print("   ", x)


if __name__ == "__main__":
    main()
