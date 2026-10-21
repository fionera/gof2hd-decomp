#!/usr/bin/env python3
"""Exported data-object parity: compare every OBJECT symbol defined in both .so files.

Bytes compare literally, except words that carry a dynamic relocation (ABS32/GLOB_DAT against a
symbol, or RELATIVE): those compare by the symbol / object(+offset) / function they point at, since
two links place things at different addresses. .bss objects (no file bytes) compare by size only.

Usage: datadiff.py <original.so> <ours.so> [--top N] [--json out]
       datadiff.py <original.so> <ours.so> --sections [--objdump OBJDUMP] [--top N]

--sections (phase 2, local data): section sizes, then the original's .rodata strings (>=4 printable
chars) and 16-byte binary chunks that occur nowhere in ours (plus our strings absent from the
original -- usually invented placeholders). With --objdump, every gap is attributed to the
original functions that reach it PC-relatively (ldr rX,[pc] + add rX,pc), i.e. the bodies whose
literals/tables are missing or wrong.
"""
import argparse, bisect, json, re, struct, subprocess, sys, os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "verify"))
from elf import Elf32, SHT_REL, SHT_DYNSYM  # noqa: E402

R_ARM_ABS32, R_ARM_GLOB_DAT, R_ARM_RELATIVE = 2, 21, 23


class Side:
    def __init__(self, path):
        self.e = Elf32(path)
        self.objs = {n: (v, sz) for v, sz, n in self.e.objects()}
        self.funcs = {}
        for n, f in self.e.functions().items():
            self.funcs.setdefault(f["value"], n)
        self.objlist = sorted((v, sz, n) for n, (v, sz) in self.objs.items())
        self.starts = [o[0] for o in self.objlist]
        names = [s["name"] for s in self.e._symbols(SHT_DYNSYM)]
        self.rel = {}
        for sec in self.e.sections:
            if sec["type"] != SHT_REL or sec["name"] == ".rel.plt":
                continue
            for off in range(sec["offset"], sec["offset"] + sec["size"], 8):
                r_off, info = struct.unpack_from("<II", self.e.data, off)
                t, idx = info & 0xFF, info >> 8
                if t in (R_ARM_ABS32, R_ARM_GLOB_DAT) and idx:
                    self.rel[r_off] = ("sym", names[idx])
                elif t == R_ARM_RELATIVE:
                    self.rel[r_off] = ("rel", None)

    def name_addr(self, a):
        if (a & ~1) in self.funcs:
            return "fn:" + self.funcs[a & ~1]
        i = bisect.bisect_right(self.starts, a) - 1
        if i >= 0:
            v, sz, n = self.objlist[i]
            if v <= a < v + max(sz, 1):
                return f"obj:{n}+{a - v}"
        sec = self.e.section_of(a)
        if sec == ".rodata":
            b = self.e.bytes_at(a, 256)
            z = b.find(b"\0")
            if z > 0 and all(32 <= c < 127 for c in b[:z]):
                return "str:" + b[:z].decode()
        return f"local:{sec}"

    def norm(self, name):
        v, sz = self.objs[name]
        if self.e.section_of(v) == ".bss":
            return ["bss"] * ((sz + 3) // 4)
        b = self.e.bytes_at(v, sz)
        out, i = [], 0
        while i < sz:
            r = self.rel.get(v + i)
            if r and i + 4 <= sz:
                if r[0] == "sym":
                    out.append("sym:" + r[1])
                else:
                    out.append(self.name_addr(struct.unpack_from("<I", b, i)[0]))
                i += 4
            else:
                out.append(b[i:i + 4].hex())
                i += 4
        return out


def _sec(e, name):
    s = next((x for x in e.sections if x["name"] == name), None)
    return (e.data[s["offset"]:s["offset"] + s["size"]], s["addr"]) if s else (b"", 0)


_STR = re.compile(rb"[\x20-\x7e\t\n\r]{4,}\x00")


def pic_xrefs(side, objdump):
    """(target address, function) for every `ldr rX,[pc,#n]` ... `add rX,pc` pair in .text."""
    text = next(s for s in side.e.sections if s["name"] == ".text")
    out = subprocess.run([objdump, "-d", "-M", "force-thumb", "-j", ".text", side.e.path],
                         capture_output=True, text=True).stdout if hasattr(side.e, "path") else ""
    funcs = sorted((f["value"] & ~1, n) for n, f in side.e.functions().items())
    starts = [f[0] for f in funcs]
    ldr = re.compile(r"^\s*([0-9a-f]+):\s.*\tldr(?:\.w)?\s+(\w+), \[pc, #-?\d+\]\s+; \(([0-9a-f]+)")
    add = re.compile(r"^\s*([0-9a-f]+):\s.*\tadd\s+(\w+), pc$")
    regs, refs = {}, []
    for line in out.splitlines():
        m = ldr.match(line)
        if m:
            a = int(m.group(3), 16)
            if text["addr"] - 0x100000 <= a < text["addr"] + text["size"] + 0x100000:
                try:
                    regs[m.group(2)] = struct.unpack("<I", side.e.bytes_at(a, 4))[0]
                except Exception:  # noqa: BLE001
                    pass
            continue
        m = add.match(line)
        if m and m.group(2) in regs:
            at = int(m.group(1), 16)
            t = (regs.pop(m.group(2)) + at + 4) & 0xFFFFFFFF
            i = bisect.bisect_right(starts, at) - 1
            refs.append((t, funcs[i][1] if i >= 0 else "?"))
    refs.sort()
    return refs


def section_report(o, u, objdump, top):
    for name in (".rodata", ".data.rel.ro", ".data", ".bss"):
        so = next((x for x in o.e.sections if x["name"] == name), None)
        su = next((x for x in u.e.sections if x["name"] == name), None)
        if so and su:
            print(f"{name:<14} orig {so['size']:>7}  ours {su['size']:>7}  delta {su['size'] - so['size']:>+7}")
    orod, oaddr = _sec(o.e, ".rodata")
    urod, _ = _sec(u.e, ".rodata")
    refs = pic_xrefs(o, objdump) if objdump else []
    keys = [r[0] for r in refs]

    def who(a, n):
        if not refs:
            return ""
        j, k = bisect.bisect_left(keys, a - 96), bisect.bisect_right(keys, a + n)
        fns = sorted({f for _, f in refs[j:k]})
        return "  <- " + ", ".join(fns[:3]) + (" ..." if len(fns) > 3 else "")

    mask = bytearray(len(orod))
    miss_s = []
    for m in _STR.finditer(orod):
        mask[m.start():m.end()] = b"\x01" * (m.end() - m.start())
        if m.group(0) not in urod:
            miss_s.append((m.start() + oaddr, m.group(0)))
    print(f"\n.rodata strings missing from ours: {len(miss_s)} ({sum(len(x) for _, x in miss_s)} B)")
    for a, st in sorted(miss_s, key=lambda r: -len(r[1]))[:top]:
        print(f"  {a:#x} {len(st):>5}  {st[:60]!r}{who(a, len(st))}")
    extra = [m.group(0) for m in _STR.finditer(urod) if m.group(0) not in orod]
    print(f"\nour .rodata strings absent from the original: {len(extra)} ({sum(map(len, extra))} B)")
    for st in sorted(extra, key=len, reverse=True)[:top]:
        print(f"  {len(st):>5}  {st[:70]!r}")
    runs, i = [], 0
    while i + 16 <= len(orod):
        if mask[i] or i % 4:
            i += 1
            continue
        w = orod[i:i + 16]
        if not any(mask[i:i + 16]) and w.count(0) != 16 and w not in urod:
            if runs and runs[-1][1] == i:
                runs[-1][1] = i + 16
            else:
                runs.append([i, i + 16])
            i += 16
        else:
            i += 4
    print(f"\n.rodata binary chunks missing from ours: {len(runs)} runs ({sum(b - a for a, b in runs)} B)")
    for a, b in sorted(runs, key=lambda r: r[0] - r[1])[:top]:
        print(f"  {a + oaddr:#x} {b - a:>6}  {orod[a:a + 16].hex()}{who(a + oaddr, b - a)}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("original"); ap.add_argument("ours")
    ap.add_argument("--top", type=int, default=30); ap.add_argument("--json")
    ap.add_argument("--sections", action="store_true", help="phase-2 local-data gap report")
    ap.add_argument("--objdump", help="NDK objdump: attribute gaps to original functions")
    a = ap.parse_args()
    o, u = Side(a.original), Side(a.ours)
    if a.sections:
        o.e.path = a.original
        section_report(o, u, a.objdump, a.top)
        return
    common = sorted(set(o.objs) & set(u.objs))
    rows, exact = [], 0
    for n in common:
        on, un = o.norm(n), u.norm(n)
        ok = on == un and o.objs[n][1] == u.objs[n][1]
        exact += ok
        if not ok:
            diff = sum(1 for x, y in zip(on, un) if x != y) + abs(len(on) - len(un))
            rows.append(dict(name=n, osize=o.objs[n][1], usize=u.objs[n][1], words_diff=diff,
                             section=o.e.section_of(o.objs[n][0])))
    print(f"data objects: common {len(common)}  exact {exact}  differ {len(rows)}  "
          f"orig-only {len(set(o.objs) - set(u.objs))}  ours-only {len(set(u.objs) - set(o.objs))}")
    for r in sorted(rows, key=lambda r: -r["words_diff"])[:a.top]:
        print(f"{r['words_diff']:>6}w {r['osize']:>7} {r['usize']:>7} {r['section']:<14} {r['name']}")
    if a.json:
        json.dump(rows, open(a.json, "w"), indent=0)


if __name__ == "__main__":
    main()
