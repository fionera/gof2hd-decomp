#!/usr/bin/env python3
"""Measure object/TU interleaving in the current match build.

Usage (from the repo root, after `cmake --build cmake-build-match --target link_so`):
    python3 _work/reconstructions/w3613/tu_interleave.py

Two metrics are printed because the older "144 / 195" figure counts every exported
symbol, including the many weak inline copies (Array/String/Matrix ctors) that every
TU emits.  The strong-only metric (STB_GLOBAL STT_FUNC, `T`) is the meaningful one for
TU membership: a weak copy can legitimately live in many objects.
"""
import json
import os
import subprocess
import sys
from collections import Counter, defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__)))))
sys.path.insert(0, os.path.join(ROOT, "tools", "verify"))
sys.path.insert(0, os.path.join(ROOT, "_work", "reconstructions", "w3560"))
from elf import Elf32  # noqa: E402
from symtab import parse  # noqa: E402

ORIG = os.path.join(ROOT, "..", "_work", "bins", "android_2.0.16_libgof2hdaa.so")
RSP = os.path.join(ROOT, "cmake-build-match", "link_order.rsp")
CXXFILT = os.path.join(
    ROOT, ".cache", "ndk", "android-ndk-r18b", "toolchains",
    "arm-linux-androideabi-4.9", "prebuilt", "linux-x86_64", "bin",
    "arm-linux-androideabi-c++filt")


def demangle(names):
    if not names:
        return {}
    out = subprocess.run([CXXFILT], input="\n".join(names), text=True,
                         capture_output=True, check=True).stdout.splitlines()
    if len(out) != len(names):
        raise SystemExit("c++filt line-count mismatch")
    return dict(zip(names, out))


def main():
    e = Elf32(ORIG)
    funcs = e.functions()
    objs = [ln.strip() for ln in open(RSP) if ln.strip()]

    obj_all, obj_strong = {}, {}
    for o in objs:
        allf, strong = [], []
        for s in parse(o):
            if not s["name"] or s["shndx"] == 0 or s["typ"] != 2:
                continue
            if s["name"] in funcs:
                allf.append(s["name"])
                if s["bind"] == 1:
                    strong.append(s["name"])
        obj_all[o] = allf
        obj_strong[o] = strong

    def interleaved(obj_funcs):
        rng = {}
        for o, fs in obj_funcs.items():
            if fs:
                rng[o] = (min(funcs[n]["value"] for n in fs),
                          max(funcs[n]["value"] + funcs[n]["size"] for n in fs))
        bad = []
        for o, (lo, hi) in rng.items():
            foreign = []
            for p, fs in obj_funcs.items():
                if p == o:
                    continue
                for n in fs:
                    a = funcs[n]["value"]
                    if lo <= a < hi:
                        foreign.append((a, os.path.basename(p), n))
            if foreign:
                bad.append((o, lo, hi, sorted(foreign)))
        return rng, bad

    rng_all, bad_all = interleaved(obj_all)
    rng_strong, bad_strong = interleaved(obj_strong)
    print(f"objects linked: {len(objs)}")
    print(f"objects with exported funcs (all): {len(rng_all)}; "
          f"range contains another object's func: {len(bad_all)}")
    print(f"objects with strong exported funcs: {len(rng_strong)}; "
          f"range contains another object's strong func: {len(bad_strong)}")
    for o, lo, hi, foreign in sorted(bad_strong, key=lambda x: -len(x[3])):
        print(f"  STRONG {os.path.basename(o)} {lo:#x}-{hi:#x} "
              f"foreign {len(foreign)}")
        for a, p, n in foreign[:8]:
            print(f"    {a:#x} {p} {n}")

    # Dump attribution: which original TUs are represented by more than one of our
    # objects?  HD is the app witness, SD the engine witness.
    j = json.load(open(os.path.join(ROOT, "_work", "original_layout",
                                    "original_layout.json")))["tu_symbols"]
    sym2tu = {}
    for dump in ("gof2hd_1.1.19_ios", "gof2sd_1.0.1_ios"):
        for tu, v in j[dump].items():
            for s in v["symbols"]:
                sym2tu.setdefault(s[1:] if s.startswith("_") else s, tu)
    names = sorted({n for fs in obj_all.values() for n in fs})
    dem = demangle(names)

    def tu_of(n):
        d = dem.get(n, n)
        for suffix in (" (.cold.1)", " (.cold.2)", " (.cold.3)"):
            if d.endswith(suffix):
                d = d[:-len(suffix)]
        return sym2tu.get(d)

    tu_objs = defaultdict(Counter)
    for o, fs in obj_strong.items():
        for n in fs:
            tu = tu_of(n)
            if tu:
                tu_objs[tu][os.path.basename(o)] += 1
    print("original TUs represented by >1 object (strong funcs):")
    for tu, c in sorted(tu_objs.items(), key=lambda kv: (-len(kv[1]), kv[0])):
        if len(c) > 1:
            print(f"  {tu}: {len(c)} objects, {sum(c.values())} funcs")
            for o, n in c.most_common():
                print(f"    {n:3} {o}")

    print("objects with no strong exported funcs:")
    for o in objs:
        if not obj_strong[o]:
            print(f"  {os.path.basename(o)} (all function symbols: {len(obj_all[o])})")


if __name__ == "__main__":
    main()
