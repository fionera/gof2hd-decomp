#!/usr/bin/env python3
"""Classify every verify row with pct == 100.0 but byte == False.

The verifier normalizes inter-function branches by symbol and PIC literal-pool words by the
logical datum they name, so a 100% row means the instruction stream is structurally identical
and only raw bytes differ.  This script separates the differing bytes into instruction words
and literal-pool words, resolves the PIC `ldr rX,[pc,#N] / add rX,pc` destination, and prints
the section-pair / target histograms used by `literal_pool_sweep_w3436.md`.

Usage:
    python3 _work/reconstructions/literal_pool_sweep_w3436.py [--json OUT]

Run after `cmake --build cmake-build-match --target verify` (or `check`).  The original is
read from `../_work/bins/android_2.0.16_libgof2hdaa.so` relative to the repo root.
"""
import argparse
import collections
import json
import os
import re
import struct
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "verify"))
import verify  # noqa: E402
from elf import Elf32  # noqa: E402

ORIG = ROOT.parent / "_work" / "bins" / "android_2.0.16_libgof2hdaa.so"
OURS = ROOT / "cmake-build-match" / "libgof2hdaa.so"
REPORT = ROOT / "cmake-build-match" / "verify" / "report.json"
OBJDUMP = (ROOT / ".cache" / "ndk" / "android-ndk-r18b" / "toolchains" /
           "arm-linux-androideabi-4.9" / "prebuilt" / "linux-x86_64" / "bin" /
           "arm-linux-androideabi-objdump")


def addr_range(elf):
    mapped = [s for s in elf.sections if s["addr"]]
    lo = next((s["addr"] for s in mapped if s["name"] == ".plt"),
              min(s["addr"] for s in mapped))
    return lo, max(s["addr"] + s["size"] for s in mapped)


def make_resolvers(eo, eu):
    fo, fu = eo.functions(), eu.functions()
    common = set(fo) & set(fu)
    ro = verify.Resolver(fo, common, eo.plt_map(), addr_range(eo))
    ru = verify.Resolver(fu, common, eu.plt_map(), addr_range(eu))
    common_objects = {o[2] for o in eo.objects()} & {o[2] for o in eu.objects()}
    ro.attach_data(eo, common_objects)
    ru.attach_data(eu, common_objects)
    imports = ({v for k, v in eo.got_map().values() if k == "sym"} &
               {v for k, v in eu.got_map().values() if k == "sym"})
    ro.imports = ru.imports = imports
    return fo, fu, ro, ru


def analyze(blob, thumb, base, resolver):
    """Return (instruction lines, pool byte set, jump-table byte set, PIC destinations)."""
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
        f.write(blob)
        tmp = f.name
    try:
        lines = verify._objdump_lines(OBJDUMP, tmp, thumb)
        jtab = set()
        if thumb:
            for _ in range(2):
                pool = verify._pool_bytes(lines, thumb)
                jtab = verify._jump_tables(lines, blob, pool)
                if not pool and not jtab:
                    break
                clean = bytearray(blob)
                for b in pool | jtab:
                    if b < len(clean):
                        clean[b] = 0x00 if (b & 1) == 0 else 0xbf
                with open(tmp, "wb") as f:
                    f.write(clean)
                lines = verify._objdump_lines(OBJDUMP, tmp, thumb)
    finally:
        os.unlink(tmp)

    pool = set()
    pic = {}
    for i, (addr, insn) in enumerate(lines):
        if addr in pool or addr in jtab:
            continue
        pm = verify.PCREL_LOAD.match(insn)
        if not pm:
            continue
        mnem, reg, imm = pm.group(1), pm.group(2), int(pm.group(3) or 0)
        tgt = ((addr + (4 if thumb else 8)) & ~3) + imm
        width = 8 if (mnem == "ldrd" or (mnem == "vldr" and reg.startswith("d"))) else 4
        pool.update(range(tgt, tgt + width))
        for laddr, later in lines[i + 1:i + 17]:
            if re.match(rf"^add(?:\.[nw])?\s+{reg},\s*pc$", later):
                if tgt + 4 <= len(blob):
                    delta = struct.unpack_from("<I", blob, tgt)[0]
                    pic[tgt] = (base + laddr + (4 if thumb else 8) + delta) & 0xFFFFFFFF
                break
            lm = re.match(r"^([a-z]+)\S*\s+(\w+)", later)
            if lm and lm.group(2) == reg and not re.match(
                    r"^(str|cmp|cmn|tst|teq|push|stm|vst|b)", lm.group(1)):
                break
    return lines, pool, jtab, pic


def branch_info(insn, ioff, base, size, resolver):
    parts = insn.split(None, 1)
    if len(parts) != 2 or not verify.BRANCH.match(parts[0]):
        return None
    ms = list(verify.HEX.finditer(parts[1]))
    if not ms:
        return None
    t = int(ms[-1].group(0), 16)
    if t < size:
        return ("intra", f"loc_{t:x}")
    addr = (base + t) & 0xFFFFFFFF
    if parts[0].startswith("blx"):
        addr = (t - ((ioff + 4) & ~3) + ((base + ioff + 4) & ~3)) & 0xFFFFFFFF
    name = resolver.resolve(addr)
    return ("sym", name) if name else ("abs", f"#{addr:#x}")


def classify():
    eo, eu = Elf32(ORIG), Elf32(OURS)
    fo, fu, ro, ru = make_resolvers(eo, eu)
    rows = json.loads(REPORT.read_text())["rows"]
    targets = [r for r in rows if r["pct"] == 100.0 and not r["byte"]]
    out = []
    for r in targets:
        name = r["name"]
        o, u = fo[name], fu[name]
        if o["size"] != u["size"]:
            out.append(dict(name=name, size=o["size"], err="size"))
            continue
        size = o["size"]
        bo = eo.bytes_at(o["value"], size)
        bu = eu.bytes_at(u["value"], size)
        lo, po, jo, pico = analyze(bo, o["thumb"], o["value"], ro)
        lu, pu, ju, picu = analyze(bu, u["thumb"], u["value"], ru)

        def cmap(pool, ins):
            m = {}
            for w in pool:
                m[w] = ("pool", w & ~3)
            for i, (a, _insn) in enumerate(ins):
                nxt = ins[i + 1][0] if i + 1 < len(ins) else size
                for x in range(a, min(nxt, size)):
                    m.setdefault(x, ("insn", a))
            return m

        mo, mu = cmap(po, lo), cmap(pu, lu)
        runs = []
        x = 0
        while x < size:
            if bo[x] != bu[x]:
                y = x
                while y < size and bo[y] != bu[y]:
                    y += 1
                runs.append((x, y))
                x = y
            else:
                x += 1

        diffs = []
        for a, b in runs:
            kinds = set()
            for x in range(a, b):
                kinds.add(mo.get(x, ("?", x))[0])
                kinds.add(mu.get(x, ("?", x))[0])
            d = dict(start=a, end=b)
            if kinds == {"pool"}:
                w = a & ~3
                d["kind"] = "pool"
                d["word"] = w
                d["ov"] = struct.unpack_from("<I", bo, w)[0]
                d["uv"] = struct.unpack_from("<I", bu, w)[0]
                if w in pico or w in picu:
                    d["pic"] = True
                    do, du = pico.get(w), picu.get(w)
                    d["odest"], d["udest"] = do, du
                    d["os"] = eo.section_of(do) if do else None
                    d["us"] = eu.section_of(du) if du else None
                else:
                    d["pic"] = False
                    d["os"] = eo.section_of(d["ov"])
                    d["us"] = eu.section_of(d["uv"])
            elif kinds == {"insn"}:
                d["kind"] = "insn"
                a0 = mo.get(a, ("insn", a))[1]
                d["ioff"] = a0
                d["oins"] = next((insn for aa, insn in lo if aa == a0), "")
                d["uins"] = next((insn for aa, insn in lu if aa == a0), "")
                d["obr"] = branch_info(d["oins"], a0, o["value"], size, ro)
                d["ubr"] = branch_info(d["uins"], a0, u["value"], size, ru)
            else:
                d["kind"] = "mixed"
                d["kinds"] = sorted(kinds)
            diffs.append(d)
        out.append(dict(name=name, size=size, addr_delta=u["value"] - o["value"], diffs=diffs))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--json", help="write full per-function records here")
    args = ap.parse_args()
    res = classify()
    if args.json:
        Path(args.json).write_text(json.dumps(res, indent=1))

    kind_runs = collections.Counter()
    kind_funcs = collections.Counter()
    pool_runs = collections.Counter()
    pool_funcs = collections.Counter()
    insn_runs = collections.Counter()
    same = moved = 0
    for r in res:
        if r.get("addr_delta") == 0:
            same += 1
        else:
            moved += 1
        ks, ps, iks = set(), set(), set()
        for d in r.get("diffs", []):
            kind_runs[d["kind"]] += 1
            ks.add(d["kind"])
            if d["kind"] == "pool":
                k = (d.get("os"), d.get("us"))
                pool_runs[k] += 1
                ps.add(k)
            elif d["kind"] == "insn":
                k = (d.get("obr"), d.get("ubr"))
                insn_runs[k] += 1
                iks.add(k)
        for k in ks:
            kind_funcs[k] += 1
        for k in ps:
            pool_funcs[k] += 1
    print(f"pct100 non-byte functions: {len(res)}  (same address {same}, moved {moved})")
    print("runs by kind:", dict(kind_runs))
    print("functions by kind:", dict(kind_funcs))
    print("pool runs by (orig section, ours section):")
    for k, v in pool_runs.most_common():
        print(f"  {str(k):40s} {v}")
    print("pool functions by (orig section, ours section):")
    for k, v in pool_funcs.most_common():
        print(f"  {str(k):40s} {v}")
    print("instruction runs by resolved target pair (top 20):")
    for k, v in insn_runs.most_common(20):
        print(f"  {str(k)[:110]:110s} {v}")


if __name__ == "__main__":
    main()
