#!/usr/bin/env python3
"""Prove the gold --build-id algorithm from the shipped artifact (w5018).

Gold's --build-id=sha1 writes a 20-byte SHA-1 of the *whole output file* with
the 20-byte build-id descriptor zeroed.  The hash is taken over the unstripped
gold output (including .symtab/.strtab/.debug_*), so it cannot be recovered
from the stripped .so.

Usage: python3 buildid_proof.py <unstripped.so> [<stripped.so> ...]
"""
import hashlib
import struct
import sys

sys.path.insert(0, "tools/verify")
from elf import Elf32  # noqa: E402


def note(path):
    e = Elf32(path)
    sec = next(s for s in e.sections if s["name"] == ".note.gnu.build-id")
    off = sec["offset"]
    namesz, descsz, ntype = struct.unpack_from("<III", e.data, off)
    return e.data, off, namesz, descsz, ntype


def buildid(path):
    d, off, namesz, descsz, ntype = note(path)
    return d[off + 16:off + 16 + descsz].hex()


def sha1_zero_desc(path):
    d, off, namesz, descsz, ntype = note(path)
    b = bytearray(d)
    for i in range(off + 16, off + 16 + descsz):
        b[i] = 0
    return hashlib.sha1(bytes(b)).hexdigest()


def main():
    for p in sys.argv[1:]:
        d, off, namesz, descsz, ntype = note(p)
        print(p)
        print(f"  note: namesz={namesz} descsz={descsz} ntype={ntype} size={0x24 if descsz == 20 else '?'}")
        print(f"  build-id          {buildid(p)}")
        print(f"  sha1(file)        {hashlib.sha1(d).hexdigest()}")
        print(f"  sha1(zero desc)   {sha1_zero_desc(p)}")
        print(f"  match             {buildid(p) == sha1_zero_desc(p)}")


if __name__ == "__main__":
    main()
