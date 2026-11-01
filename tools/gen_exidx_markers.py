#!/usr/bin/env python3
"""Give every object's empty plain `.text` a unique name so gold keeps its exidx sentinel.

Gold's `fix_exidx_coverage` emits an `EXIDX_CANTUNWIND` entry for every text input section
that has no `.ARM.exidx` companion.  With `-ffunction-sections` every object still carries an
empty plain `.text`; the original link (no ordering file) kept one sentinel per TU, but our
`--section-ordering-file` lists `.text` once, so all 272 empty sections collapse into a single
ordering index and only 6 sentinels survive.

This script makes renamed copies of every link input:

  * each gof2 object (read from the gold response file produced by gen_link_order.py) is
    copied, and a zero-size `.text` is renamed to `.text.tuend.o<NNN>`.  The marker is
    normally forced to `sh_addralign=1` so it adds no padding between 2-aligned Thumb
    functions; KIPlayer.cpp and Explosion.cpp are the exception (align 4) because their
    successor TU (ListItem.cpp / Status.cpp) starts on a 4-byte boundary in the original
    binary.  Aligning those two markers reproduces that boundary without touching `.text`
    size or `.rodata`;
  * each archive is unpacked and re-packed, with every member's zero-size `.text` renamed to
    `.text.tuend.a<AA>m<MMM>` with alignment 1.

The copies are written under <out-dir>; `link_order.rsp` and `archives.rsp` in that directory
list them, and gen_section_order.py inserts each marker after its owner's last `.text.*`
section.

Usage: gen_exidx_markers.py <out-dir> <objcopy> <ar> <link-order.rsp> <archive>...
"""
import os
import shutil
import struct
import subprocess
import sys


# libcrypto's three plain-.text objects are each one section.  Give them unique names so
# the ordering file can place them individually: the original interleaves them with the
# zip TUs and orders armv4cpuid.o before sha256-armv4.o, which gold's same-name `.text`
# grouping cannot express.
CRYPTO_TEXT_RENAME = {
    "armv4cpuid.o": ".text.crypto_armv4cpuid",
    "sha256.o": ".text.crypto_sha256",
    "sha256-armv4.o": ".text.crypto_sha256_armv4",
}


def elf_sections(data):
    """[(name, type, flags, size, align, header_offset)] for an ELF32 relocatable."""
    if data[:4] != b"\x7fELF" or data[4] != 1:
        raise ValueError("not an ELF32 file")
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    hdrs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize)
            for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    out = []
    for i, (name, typ, flags, _addr, _off, size, _link, _info, align, _entsize) in enumerate(hdrs):
        end = data.index(b"\0", stroff + name)
        out.append((data[stroff + name:end].decode(), typ, flags, size, align,
                    shoff + i * shentsize))
    return out


def has_empty_plain_text(path):
    data = open(path, "rb").read()
    return any(n == ".text" and size == 0 for n, _t, _f, size, _a, _o in elf_sections(data))


def patch_align(path, name, align):
    data = bytearray(open(path, "rb").read())
    for n, _t, _f, _s, _a, off in elf_sections(data):
        if n == name:
            struct.pack_into("<I", data, off + 32, align)   # sh_addralign
            open(path, "wb").write(data)
            return
    raise SystemExit(f"{path}: section {name} not found")


def rename_empty_text(objcopy, src, dst, marker, align=1):
    shutil.copyfile(src, dst)
    if has_empty_plain_text(dst):
        subprocess.run([objcopy, "--rename-section", f".text={marker}", dst],
                       check=True, capture_output=True)
        patch_align(dst, marker, align)
        return True
    return False


def ar_members(data):
    """[(member-name, body)] of an ar archive."""
    if not data.startswith(b"!<arch>\n"):
        raise ValueError("not an ar archive")
    out = []
    pos = 8
    longnames = b""
    while pos + 60 <= len(data):
        hdr = data[pos:pos + 60]
        name = hdr[:16].strip()
        size = int(hdr[48:58].strip())
        body = data[pos + 60:pos + 60 + size]
        if name == b"//":
            longnames = body
        elif name not in (b"/", b"/SYM64/"):
            if name.startswith(b"/") and name[1:].isdigit():
                off = int(name[1:])
                end = longnames.index(b"/\n", off)
                name = longnames[off:end]
            else:
                name = name.rstrip(b"/")
            out.append((name.decode(), body))
        pos += 60 + size + (size & 1)
    return out


def main() -> int:
    outdir, objcopy, ar, rsp, archives = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5:]
    objdir = os.path.join(outdir, "objects")
    ardir = os.path.join(outdir, "archives")
    for d in (objdir, ardir):
        os.makedirs(d, exist_ok=True)

    objs = [ln.strip() for ln in open(rsp) if ln.strip()]
    copies, markers = [], 0
    for i, obj in enumerate(objs):
        dst = os.path.join(objdir, f"o{i:03d}_{os.path.basename(obj)}")
        # KIPlayer/Explosion/ApplicationManager are followed by a TU whose first
        # function is 4-aligned in the original (ListItem/Status/Engine); align the
        # boundary marker to 4 to match.
        align = 4 if os.path.basename(obj) in ("KIPlayer.cpp.obj", "Explosion.cpp.obj",
                                                "ApplicationManager.cpp.obj") else 1
        if rename_empty_text(objcopy, obj, dst, f".text.tuend.o{i:03d}", align):
            markers += 1
        copies.append(dst)
    with open(os.path.join(outdir, "link_order.rsp"), "w") as f:
        f.write("\n".join(copies) + "\n")

    arch_copies = []
    for ai, arch in enumerate(archives):
        members = ar_members(open(arch, "rb").read())
        mdir = os.path.join(ardir, f"a{ai:03d}")
        os.makedirs(mdir, exist_ok=True)
        member_paths = []
        for mi, (name, body) in enumerate(members):
            src = os.path.join(mdir, f"m{mi:03d}_{name}")
            open(src, "wb").write(body)
            dst = src + ".marked"
            if rename_empty_text(objcopy, src, dst, f".text.tuend.a{ai:03d}m{mi:03d}"):
                markers += 1
            else:
                for key, newname in CRYPTO_TEXT_RENAME.items():
                    if key in name:
                        subprocess.run([objcopy, "--rename-section",
                                        f".text={newname}", dst],
                                       check=True, capture_output=True)
                        break
            member_paths.append(dst)
        dst_arch = os.path.join(ardir, os.path.basename(arch))
        subprocess.run([ar, "rcs", dst_arch] + member_paths, check=True, capture_output=True)
        arch_copies.append(dst_arch)
    with open(os.path.join(outdir, "archives.rsp"), "w") as f:
        f.write("\n".join(arch_copies) + "\n")

    print(f"exidx markers: {markers} empty .text sections renamed "
          f"({len(objs)} objects, {len(archives)} archives) -> {outdir}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
