#!/usr/bin/env python3
"""Generate a gold --section-ordering-file that reproduces the original .so's .text
function order.

With -ffunction-sections every function `SYM` lands in its own section `.text.SYM`.
The original .so (stripped, only .dynsym) exposes its function symbols with addresses;
sorting those by address gives the original's function layout order. Emitting the
corresponding `.text.SYM` names in that order lets gold place our functions in the
exact same sequence, so inter-function bl/blx and GOT/PLT relative offsets line up --
the prerequisite for whole-.so byte/hash identity while keeping one-class-per-file source.

Local (static / internal-linkage) functions are not in the original's .dynsym, so their
position cannot be read from it. The original was linked without an ordering file: its
.text is plain input order, i.e. each object's functions in source order. So when our
objects are given, every local function section is slotted directly after the section
that precedes it in its own object (or before the first listed one that follows it).

Usage: gen_section_order.py <original.so> <nm-tool> <out-file> [object-or-archive ...]
"""
import struct
import subprocess
import sys


def elf_text_sections(data):
    """Names of the .text.* sections of one ELF32 relocatable, in section-header order."""
    if data[:4] != b"\x7fELF" or data[4] != 1:
        return []
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    hdrs = [struct.unpack_from("<IIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    names = []
    for name, typ, flags, _addr, _off, _size in hdrs:
        end = data.index(b"\0", stroff + name)
        n = data[stroff + name:end].decode()
        if typ == 1 and flags & 0x4 and n.startswith(".text."):   # PROGBITS, SHF_EXECINSTR
            names.append(n)
    return names


def members(path):
    """Yield the ELF payloads of an object file or of every member of an ar archive."""
    data = open(path, "rb").read()
    if not data.startswith(b"!<arch>\n"):
        yield data
        return
    pos = 8
    while pos + 60 <= len(data):
        name = data[pos:pos + 16].strip()
        size = int(data[pos + 48:pos + 58].strip())
        body = data[pos + 60:pos + 60 + size]
        if name not in (b"/", b"//"):
            yield body
        pos += 60 + size + (size & 1)


def main() -> int:
    orig, nm, out, inputs = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4:]
    # -n: sort by address; -D: dynamic symbols (all the stripped original exposes).
    res = subprocess.run([nm, "-nD", "--defined-only", orig],
                         capture_output=True, text=True)
    lines = []
    for ln in res.stdout.splitlines():
        parts = ln.split(maxsplit=2)
        # "<addr> <type> <name>"; text symbols are t/T/w/W.
        if len(parts) == 3 and parts[1] in ("t", "T", "w", "W"):
            lines.append(".text." + parts[2])
    listed = set(lines)
    after, before, seen = {}, {}, set()
    for path in inputs:
        for data in members(path):
            seq = elf_text_sections(data)
            pending = []           # locals seen before this object's first listed section
            anchor = None
            for s in seq:
                if s in listed:
                    if pending:
                        before.setdefault(s, []).extend(pending)
                        pending = []
                    anchor = s
                elif s not in seen:
                    seen.add(s)
                    if anchor is None:
                        pending.append(s)
                    else:
                        after.setdefault(anchor, []).append(s)
    order = []
    for s in lines:
        order.extend(before.get(s, []))
        order.append(s)
        order.extend(after.get(s, []))
    with open(out, "w") as f:
        f.write("\n".join(order) + "\n")
    print(f"wrote {len(order)} section names ({len(order) - len(lines)} local) to {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
