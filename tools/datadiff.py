#!/usr/bin/env python3
"""Exported data-object parity: compare every OBJECT symbol defined in both .so files.

Bytes compare literally, except words that carry a dynamic relocation (ABS32/GLOB_DAT against a
symbol, or RELATIVE): those compare by the symbol / object(+offset) / function they point at, since
two links place things at different addresses. .bss objects (no file bytes) compare by size only.

Usage: datadiff.py <original.so> <ours.so> [--top N] [--json out]
"""
import argparse, bisect, json, struct, sys, os
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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("original"); ap.add_argument("ours")
    ap.add_argument("--top", type=int, default=30); ap.add_argument("--json")
    a = ap.parse_args()
    o, u = Side(a.original), Side(a.ours)
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
