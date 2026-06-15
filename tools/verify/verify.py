#!/usr/bin/env python3
"""Function-level ASM validation: compare our ARM build output against the original
libgof2hdaa.so, function by function, and print a match-% report.

Pipeline (all ARM tooling runs in OrbStack; objdiff can't read ARMv7 so we use
arm-linux-gnueabihf-objdump directly):
  1. compile each src/*.cpp with the matching NDK r18b toolchain  (build_objs.sh)
  2. for each base .o, delink the matching originals out of the .so   (delink.py)
  3. disassemble both, normalize, fuzzy-match per symbol             (asmdiff.py)
  4. print a table sorted worst-first + write report.json

Usage:
  verify.py [--build-dir DIR] [--only REGEX] [--no-build]
  verify.py --show <mangled-symbol>        # side-by-side diff of one function
"""
import argparse
import json
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
sys.path.insert(0, HERE)
import asmdiff       # noqa: E402
import delink as delinker  # noqa: E402  (local delink() below shadows the module name)

DEFAULT_BUILD = os.path.join(REPO, "cmake-build-match", "verify")


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, **kw)


def delink(base_o, target_o):
    run([sys.executable, os.path.join(HERE, "delink.py"),
         "--base", base_o, "--out", target_o],
        stdout=subprocess.DEVNULL)


def find_base_objects(build_dir):
    base_root = os.path.join(build_dir, "base")
    objs = []
    for dirpath, _, files in os.walk(base_root):
        for f in files:
            if f.endswith(".o"):
                p = os.path.join(dirpath, f)
                objs.append((os.path.relpath(p, base_root)[:-2], p))  # (unit, path)
    return sorted(objs)


def collect(build_dir, only=None):
    """Returns (rows, units_count). Delinks + diffs every base object."""
    rows = []
    target_root = os.path.join(build_dir, "target")
    only_re = re.compile(only) if only else None
    for unit, base_o in find_base_objects(build_dir):
        target_o = os.path.join(target_root, unit + ".o")
        os.makedirs(os.path.dirname(target_o), exist_ok=True)
        try:
            delink(base_o, target_o)
            tf = asmdiff.disassemble(target_o)
            bf = asmdiff.disassemble(base_o)
        except subprocess.CalledProcessError as e:
            print(f"  ! {unit}: tooling error ({e})", file=sys.stderr)
            continue
        for r in asmdiff.compare(tf, bf):
            if only_re and not only_re.search(r["symbol"]):
                continue
            r["unit"] = unit
            rows.append(r)
    rows.sort(key=lambda r: (r["match"], r["unit"], r["symbol"]))
    return rows


def original_text_functions():
    """All function names in the original .so .text — the coverage denominator."""
    addr2names, _ = delinker.load_symbols(delinker.DEFAULT_SYMS)
    ta, _, tsz = delinker.find_text_section(delinker.DEFAULT_SO)
    te = ta + tsz
    return {n for a, ns in addr2names.items() if ta <= a < te for n in ns}


def find_symbol_objects(build_dir, sym):
    """Locate the (base_o, target_o) that define a given symbol, for --show."""
    target_root = os.path.join(build_dir, "target")
    for unit, base_o in find_base_objects(build_dir):
        bf = asmdiff.disassemble(base_o)
        if sym in bf:
            target_o = os.path.join(target_root, unit + ".o")
            delink(base_o, target_o)
            return unit, base_o, target_o, bf
    return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--build-dir", default=DEFAULT_BUILD)
    ap.add_argument("--only", default=None, help="regex filter on mangled symbol")
    ap.add_argument("--no-build", action="store_true",
                    help="skip the ARM compile step (reuse existing base/ objects)")
    ap.add_argument("--show", nargs="?", const="", default=None, metavar="SYMBOL",
                    help="print side-by-side disassembly diff for one symbol "
                         "(falls back to the FN environment variable)")
    ap.add_argument("--report", default=None, help="write JSON report to this path")
    args = ap.parse_args()

    if not args.no_build and not args.show:
        run(["bash", os.path.join(HERE, "build_objs.sh"), args.build_dir])

    if args.show is not None:
        sym = args.show or os.environ.get("FN", "")
        if not sym:
            print("set the symbol: --show <mangled> or FN=<mangled>")
            return 1
        found = find_symbol_objects(args.build_dir, sym)
        if not found:
            print(f"symbol not found in any base object: {sym}")
            return 1
        unit, base_o, target_o, bf = found
        tf = asmdiff.disassemble(target_o)
        print(f"# unit {unit}   symbol {sym}")
        r = asmdiff.compare(tf, bf)
        r = next((x for x in r if x["symbol"] == sym), None)
        if r:
            print(f"# match {r['match']}%   bytes_equal={r['bytes_equal']}   "
                  f"insns target={r['n_target']} base={r['n_base']}\n")
        print(asmdiff.unified(tf, bf, sym) or "(identical after normalization)")
        return 0

    rows = collect(args.build_dir, only=args.only)
    if not rows:
        print("No comparable functions found. Did the ARM build produce any .o files?")
        return 1

    perfect = sum(1 for r in rows if r["match"] >= 100)
    bytes_eq = sum(1 for r in rows if r["bytes_equal"])
    avg = sum(r["match"] for r in rows) / len(rows)

    print(f"\n{'match':>7}  {'bytes':>5}  {'unit':<34} symbol")
    print("-" * 100)
    for r in rows:
        flag = "==" if r["bytes_equal"] else "  "
        print(f"{r['match']:6.1f}%  {flag:>5}  {r['unit']:<34} {r['symbol'][:60]}")
    print("-" * 100)

    report = {"avg_match": round(avg, 2), "count": len(rows),
              "fuzzy_perfect": perfect, "byte_exact": bytes_eq}

    # Coverage vs the original: functions in the .so .text that we never compared
    # (not decompiled/compiled yet, or whose signature mangles differently — e.g.
    # Array<T> vs std::vector). Skipped when --only narrows the set.
    if not args.only:
        universe = original_text_functions()
        compared = {r["symbol"] for r in rows}
        missing = sorted(universe - compared)
        report.update({"original_functions": len(universe),
                       "compared_unique": len(compared), "missing": len(missing)})
        miss_path = os.path.join(args.build_dir, "missing.txt")
        os.makedirs(os.path.dirname(miss_path), exist_ok=True)
        with open(miss_path, "w") as f:
            f.write("\n".join(missing) + ("\n" if missing else ""))

    print(f"{len(rows)} comparisons   avg {avg:.1f}%   "
          f"100%-fuzzy {perfect}   byte-exact {bytes_eq}")
    if not args.only:
        print(f"coverage: compared {report['compared_unique']}/{report['original_functions']} "
              f"original functions   missing {report['missing']} "
              f"(-> {os.path.relpath(miss_path, REPO)})")

    report["functions"] = rows
    out = args.report or os.path.join(args.build_dir, "report.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    json.dump(report, open(out, "w"), indent=2)
    print(f"report -> {os.path.relpath(out, REPO)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
