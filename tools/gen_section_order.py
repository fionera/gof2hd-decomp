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

Three things do NOT come from the address-sorted dynsym list and are handled explicitly:

  * `.text.startup` -- gold's default (no ordering file) sorts `.text.startup` ahead of
    every other .text input section, so the original's .text begins with the global
    constructors.  Passing an ordering file disables that default sort, so the group has
    to be listed first here or it lands in the middle of the app code.
  * `.text.crtbegin` -- crtbegin_so.o's plain `.text` is renamed to this unique name before
    the link (see CMakeLists.txt) so it can be placed directly after `.text.startup`, as
    in the original.  A plain `.text` entry would group crtbegin with every archive's
    plain `.text` (libcrypto, libgcc, ...), which the original keeps at different
    positions.
  * `.text` -- the remaining plain `.text` sections (libcrypto's sha256/sha256-armv4/
    armv4cpuid objects, libgcc) are placed at the first original address of a symbol
    defined by such an input member, i.e. where the original's libcrypto starts.  Gold
    groups same-named sections at one position, so this is the closest placement.

Finally a `*` glob is appended: gold gives every section not named in the file index 0,
i.e. it sorts them BEFORE all listed sections.  Unlisted local sections (libc++abi's
fallback_malloc.o, libandroid_support's posix_memalign.o, ...) would then jump to the
front of .text.  The trailing glob gives them the last index instead, so they stay after
the listed functions and the front of .text matches the original.

Usage: gen_section_order.py <original.so> <nm-tool> <out-file> [object-or-archive ...]
"""
import struct
import subprocess
import sys

# See the plain `.text` placement note in main().
PLAIN_TEXT_LEAD = 0x180


def elf_text_sections(data):
    """(.text.* section names in section-header order, has_nonempty_plain_.text)."""
    if data[:4] != b"\x7fELF" or data[4] != 1:
        return [], False
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    hdrs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize)
            for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    names = []
    has_plain_text = False
    for name, typ, flags, _addr, _off, size, _link, _info, _align, _entsize in hdrs:
        end = data.index(b"\0", stroff + name)
        n = data[stroff + name:end].decode()
        if typ == 1 and flags & 0x4:                 # PROGBITS, SHF_EXECINSTR
            if n == ".text":
                has_plain_text = has_plain_text or size > 0
            elif n.startswith(".text."):
                names.append(n)
    return names, has_plain_text


def elf_defined_symbols(data):
    """Names of the defined symbols in one ELF32 relocatable (.symtab, not .dynsym)."""
    if data[:4] != b"\x7fELF" or data[4] != 1:
        return []
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, _shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    hdrs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize)
            for i in range(shnum)]
    out = []
    for _name, typ, _flags, _addr, off, size, link, _info, _align, entsize in hdrs:
        if typ != 2:                                  # SHT_SYMTAB
            continue
        entsize = entsize or 16
        strtab_off = hdrs[link][4]
        for o in range(off, off + size, entsize):
            st_name, _val, _sz, _info, _other, shndx = struct.unpack_from(
                "<IIIBBH", data, o)
            if shndx == 0 or st_name == 0:
                continue
            end = data.index(b"\0", strtab_off + st_name)
            out.append(data[strtab_off + st_name:end].decode())
    return out


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
    addr = {}
    for ln in res.stdout.splitlines():
        parts = ln.split(maxsplit=2)
        # "<addr> <type> <name>"; text symbols are t/T/w/W.
        if len(parts) == 3 and parts[1] in ("t", "T", "w", "W"):
            lines.append(".text." + parts[2])
            addr[parts[2]] = int(parts[0], 16)
    listed = set(lines)

    # Where the plain `.text` group (libcrypto/libgcc) belongs: the earliest original
    # address of a symbol defined by an input member that has a non-empty plain .text.
    plain_text_addr = None
    for path in inputs:
        for data in members(path):
            _names, has_plain = elf_text_sections(data)
            if not has_plain:
                continue
            for sym in elf_defined_symbols(data):
                a = addr.get(sym)
                if a is not None and (plain_text_addr is None or a < plain_text_addr):
                    plain_text_addr = a
    # Our plain `.text` block is 0x44 bytes larger than the original's, so starting it at
    # the same symbol pushes the following 32-byte-aligned libc++abi section
    # (__cxa_end_cleanup) over an alignment boundary (+0x18 bytes of padding).  Start the
    # block one ZIP TU earlier instead: that keeps the total .text at the original
    # 0x1343d0 while only moving the handful of zip_* functions in between.
    if plain_text_addr is not None:
        plain_text_addr -= PLAIN_TEXT_LEAD

    after, before, seen = {}, {}, set()
    for path in inputs:
        for data in members(path):
            seq, _has_plain = elf_text_sections(data)
            pending = []           # locals seen before this object's first listed section
            anchor = None
            for s in seq:
                if s == ".text.startup":
                    continue       # listed explicitly at the front
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

    # .text.startup (all objects, input order) then crtbegin's renamed .text, exactly as
    # gold's default `.text.startup`-first sort and the original's input order produce.
    order = [".text.startup", ".text.crtbegin"]
    inserted = plain_text_addr is None
    for s in lines:
        if not inserted and addr[s[len(".text."):]] >= plain_text_addr:
            order.append(".text")
            inserted = True
        order.extend(before.get(s, []))
        order.append(s)
        order.extend(after.get(s, []))
    if not inserted:
        order.append(".text")
    order.append(".data")
    order.append(".data.__cxa_terminate_handler")
    order.append(".data.__cxa_unexpected_handler")
    order.append(".data._ZL5cause")
    order.append("*")          # keep unlisted sections after the listed ones, not before
    with open(out, "w") as f:
        f.write("\n".join(order) + "\n")
    print(f"wrote {len(order)} section names ({len(order) - len(lines)} local) to {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
