#!/usr/bin/env python3
"""Order the link's object files the way the original link received them.

gold emits .dynsym (and so .dynstr/.hash) in input order: each object's symbols in the order it
first defines or references them. The original's .dynsym therefore encodes its object link order
(the LOCAL_SRC_FILES order of the original build). Each of our objects is placed by the earliest
.dynsym index (in the original) of any global symbol it defines; objects that define none keep
their relative order at the end. Writes a gold response file (one path per line).

Usage: gen_link_order.py <original.so> <nm-tool> <out.rsp> <object>...
"""
import os
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "verify"))
from elf import Elf32, SHT_DYNSYM  # noqa: E402


def main() -> int:
    orig, nm, out, objs = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4:]
    index = {}
    for i, s in enumerate(Elf32(orig)._symbols(SHT_DYNSYM)):
        if s["name"] and s["shndx"] != 0:
            index.setdefault(s["name"], i)
    keyed = []
    for pos, o in enumerate(objs):
        res = subprocess.run([nm, "--defined-only", "-g", o], capture_output=True, text=True)
        best = min((index[p[2]] for p in (ln.split() for ln in res.stdout.splitlines())
                    if len(p) == 3 and p[2] in index), default=None)
        keyed.append((best is None, best if best is not None else 0, pos, o))
    keyed.sort()
    with open(out, "w") as f:
        f.write("\n".join(o for *_, o in keyed) + "\n")
    print(f"wrote link order for {len(objs)} objects to {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
