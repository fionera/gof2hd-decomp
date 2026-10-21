#!/usr/bin/env python3
"""pic_strings.py -- list the PIC data references of one function of an ELF (default: original .so).

For every `ldr rX,[pc,#N]` ... `add rX, pc` pair the target is add_addr + 4 + pool_word.  The target
is resolved to a GOT symbol, or read as a NUL-terminated string.  With --c the strings are printed
as ready-to-paste C string literals (multi-line, split after each '\\n').

usage: pic_strings.py <mangled> [--so path] [--c]
"""
import re, struct, subprocess, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[3] / "tools" / "verify"))
from elf import Elf32  # noqa: E402

ROOT = Path(__file__).resolve().parents[3]
OBJDUMP = ROOT / ".cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump"
ORIG = "/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so"


def c_literal(b):
    out, cur = [], ""
    for c in b:
        ch = chr(c)
        if ch == "\n":
            cur += "\\n"; out.append(cur); cur = ""; continue
        cur += {"\t": "\\t", "\r": "\\r", '"': '\\"', "\\": "\\\\"}.get(ch, ch)
    if cur:
        out.append(cur)
    return "\n".join(f'"{s}"' for s in out)


def main():
    args = sys.argv[1:]
    so = ORIG
    if "--so" in args:
        i = args.index("--so"); so = args[i + 1]; del args[i:i + 2]
    want_c = "--c" in args
    args = [a for a in args if a != "--c"]
    name = args[0]
    e = Elf32(so)
    f = e.functions()[name]
    start, size = f["value"], f["size"]
    got = e.got_map()
    dis = subprocess.run([str(OBJDUMP), "-d", "-M", "force-thumb", f"--start-address={start}",
                          f"--stop-address={start + size}", so], capture_output=True, text=True).stdout
    regs = {}
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
            slot = got.get(tgt)
            if slot:
                print(f"{addr:#x} got {slot[0]}:{slot[1] if slot[0]=='sym' else hex(slot[1])}")
                continue
            b = e.bytes_at(tgt, 65536)
            z = b.find(b"\0")
            s = b[:z]
            if want_c:
                print(f"// {addr:#x} -> {tgt:#x} len {z}\n{c_literal(s)}")
            else:
                print(f"{addr:#x} -> {tgt:#x} len {z}: {s[:80]!r}")


if __name__ == "__main__":
    main()
