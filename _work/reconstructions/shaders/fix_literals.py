#!/usr/bin/env python3
"""fix_literals.py -- replace a function's string literals with the original binary's.

Pairs the PIC string references of <mangled> in the original .so with those of our build, in
instruction order, and checks that our sequence equals the sequence of string literals found in
the C++ function body.  If so, rewrites the body: short strings inline; strings longer than 64
bytes (the embedded GLSL sources) become file-static `static const char *const kName = ...;`
constants (multi-line, split after each \\n) inserted before the first `namespace AbyssEngine {`.

usage: fix_literals.py <mangled> <source.cpp> <Class::Method> [--ours path] [--names vsh,fsh,...] [--dry]
"""
import re, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import pic_strings as ps  # noqa: E402
import struct, subprocess  # noqa: E402
from elf import Elf32  # noqa: E402

OURS = str(Path(__file__).resolve().parents[3] / "cmake-build-match/libgof2hdaa.so")


def strings(so, name):
    e = Elf32(so)
    f = e.functions()[name]
    start, size = f["value"], f["size"]
    got = e.got_map()
    dis = subprocess.run([str(ps.OBJDUMP), "-d", "-M", "force-thumb", f"--start-address={start}",
                          f"--stop-address={start + size}", so], capture_output=True, text=True).stdout
    regs, out = {}, []
    for line in dis.splitlines():
        m = re.match(r"\s*([0-9a-f]+):\s+(?:[0-9a-f]{4} ?)+\s+(\S+)\s*(.*)", line)
        if not m:
            continue
        addr, mnem, ops = int(m.group(1), 16), m.group(2), m.group(3)
        lm = re.match(r"(\w+), \[pc, #(\d+)\]", ops)
        if mnem.startswith("ldr") and lm:
            pool = ((addr + 4) & ~3) + int(lm.group(2))
            regs[lm.group(1)] = struct.unpack("<I", e.bytes_at(pool, 4))[0]
            continue
        am = re.match(r"(\w+), pc$", ops)
        if mnem == "add" and am and am.group(1) in regs:
            tgt = (addr + 4 + regs.pop(am.group(1))) & 0xFFFFFFFF
            if tgt in got:
                continue
            b = e.bytes_at(tgt, 65536)
            z = b.find(b"\0")
            if z >= 0 and e.section_of(tgt) == ".rodata":
                out.append(b[:z])
    return out


LIT = re.compile(r'(?<![uUL\w])"((?:[^"\\\n]|\\.)*)"')


def unescape(s):
    return s.encode().decode("unicode_escape").encode("latin-1")


def body_span(src, qual):
    i = src.find(qual + "(")
    if i < 0:
        raise SystemExit(f"{qual}( not found")
    j = src.index("{", i)
    depth = 0
    for k in range(j, len(src)):
        depth += {"{": 1, "}": -1}.get(src[k], 0)
        if depth == 0:
            return j, k + 1
    raise SystemExit("unbalanced")


def main():
    a = sys.argv[1:]
    ours = OURS
    names = []
    dry = "--dry" in a
    a = [x for x in a if x != "--dry"]
    if "--ours" in a:
        i = a.index("--ours"); ours = a[i + 1]; del a[i:i + 2]
    if "--names" in a:
        i = a.index("--names"); names = a[i + 1].split(","); del a[i:i + 2]
    mangled, path, qual = a
    orig = strings(ps.ORIG, mangled)
    mine = strings(ours, mangled)
    src = open(path).read()
    b0, b1 = body_span(src, qual)
    body = src[b0:b1]
    lits = list(LIT.finditer(body))
    vals = [unescape(m.group(1)) for m in lits]
    if vals != mine:
        print("source literals != our binary order:\n src ", vals, "\n bin ", mine)
        raise SystemExit(1)
    if len(orig) != len(mine):
        print(f"count differs: orig {len(orig)} ours {len(mine)}")
        for x in orig:
            print("  orig", x[:70])
        raise SystemExit(1)
    consts = []
    out, pos, ni = [], 0, 0
    for m, o in zip(lits, orig):
        out.append(body[pos:m.start()])
        if len(o) > 64:
            nm = names[ni] if ni < len(names) else f"kShaderSource{ni}"
            ni += 1
            consts.append(f"static const char *const {nm} =\n" +
                          "\n".join("    " + l for l in ps.c_literal(o).split("\n")) + ";\n")
            out.append(nm)
        else:
            out.append(ps.c_literal(o).replace("\n", " "))
        pos = m.end()
    out.append(body[pos:])
    new = src[:b0] + "".join(out) + src[b1:]
    if consts:
        k = new.find("namespace AbyssEngine {")
        new = new[:k] + "\n".join(consts) + "\n" + new[k:]
    if dry:
        print("".join(out)); print("\n".join(consts)[:2000])
    else:
        open(path, "w").write(new)
        print(f"rewrote {len(lits)} literals ({len(consts)} long) in {qual}")


if __name__ == "__main__":
    main()
