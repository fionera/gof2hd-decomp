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
import concurrent.futures
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
    # stdout/stderr suppressed so delink diagnostics don't interleave into the report table; a
    # delink failure surfaces as a concise per-unit skip line after the table.
    run([sys.executable, os.path.join(HERE, "delink.py"),
         "--base", base_o, "--out", target_o],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=asmdiff.DISASM_TIMEOUT)


def find_base_objects(build_dir):
    base_root = os.path.join(build_dir, "base")
    objs = []
    for dirpath, _, files in os.walk(base_root):
        for f in files:
            if f.endswith(".o"):
                p = os.path.join(dirpath, f)
                objs.append((os.path.relpath(p, base_root)[:-2], p))  # (unit, path)
    return sorted(objs)


def _diff_unit(unit, base_o, target_root, only_re):
    """Delink + diff one base object. Returns (rows, skip) where skip is None or a short
    reason string. Per-unit work is independent (each writes only its own target_o), so it
    is safe to run concurrently. Failures are returned, not printed, so they can be
    summarised after the table instead of interleaving with it."""
    target_o = os.path.join(target_root, unit + ".o")
    os.makedirs(os.path.dirname(target_o), exist_ok=True)
    # OrbStack calls wedge transiently; a timed-out unit is retried once (the retry almost always
    # clears) before it is skipped, so a transient wedge doesn't silently drop the unit's functions
    # from the report and make the totals vary run-to-run.
    for attempt in (1, 2):
        try:
            delink(base_o, target_o)
            tf = asmdiff.disassemble(target_o)
            bf = asmdiff.disassemble(base_o)
            break
        except subprocess.CalledProcessError:
            return [], "no delinkable target"
        except subprocess.TimeoutExpired:
            if attempt == 1:
                continue
            return [], "objdump hung twice"
    out = []
    for r in asmdiff.compare(tf, bf):
        if only_re and not only_re.search(r["symbol"]):
            continue
        r["unit"] = unit
        out.append(r)
    return out, None


def collect(build_dir, only=None):
    """Returns (rows, skips). skips is a list of (unit, reason) for units that couldn't be
    compared. The per-unit delink+disasm work is all OrbStack subprocesses (GIL released), so
    we fan it out across a thread pool — same GOF2_VERIFY_JOBS knob build_objs.sh uses."""
    target_root = os.path.join(build_dir, "target")
    only_re = re.compile(only) if only else None
    units = find_base_objects(build_dir)
    jobs = max(1, int(os.environ.get("GOF2_VERIFY_JOBS", "8")))
    rows, skips = [], []
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        futures = {ex.submit(_diff_unit, unit, base_o, target_root, only_re): unit
                   for unit, base_o in units}
        for fut, unit in futures.items():
            unit_rows, skip = fut.result()
            rows.extend(unit_rows)
            if skip:
                skips.append((unit, skip))
    rows.sort(key=lambda r: (r["match"], r["unit"], r["symbol"]))
    skips.sort()
    return rows, skips


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
            print(f"# match {r['match']}%   linked_equal={r['linked_equal']}   "
                  f"bytes_equal={r['bytes_equal']}   "
                  f"insns target={r['n_target']} base={r['n_base']}\n")
        print(asmdiff.unified(tf, bf, sym) or "(identical after normalization)")
        return 0

    rows, skips = collect(args.build_dir, only=args.only)
    if not rows:
        print("No comparable functions found. Did the ARM build produce any .o files?")
        return 1

    perfect = sum(1 for r in rows if r["match"] >= 100)
    bytes_eq = sum(1 for r in rows if r["bytes_equal"])
    linked_eq = sum(1 for r in rows if r["linked_equal"])
    avg = sum(r["match"] for r in rows) / len(rows)

    # 'L' = byte-identical after linking (matches modulo relocation fields); '==' = raw
    # bytes identical (the rarer subset with no external refs at all). The unit column is sized to
    # the widest unit so the symbol column stays aligned, and symbols are printed in full.
    uw = max(len("unit"), max(len(r["unit"]) for r in rows))
    header = f"{'match':>7}  {'link':>4}  {'unit':<{uw}}  symbol"
    print()
    print(header)
    print("-" * len(header))
    for r in rows:
        flag = "==" if r["bytes_equal"] else ("L" if r["linked_equal"] else "")
        print(f"{r['match']:6.1f}%  {flag:>4}  {r['unit']:<{uw}}  {r['symbol']}")
    print("-" * len(header))

    report = {"avg_match": round(avg, 2), "count": len(rows),
              "fuzzy_perfect": perfect, "byte_exact": bytes_eq,
              "linked_exact": linked_eq, "skipped": len(skips)}

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
          f"100%-fuzzy {perfect}   linked-exact {linked_eq}   byte-exact {bytes_eq}")
    if not args.only:
        print(f"coverage: compared {report['compared_unique']}/{report['original_functions']} "
              f"original functions   missing {report['missing']} "
              f"(-> {os.path.relpath(miss_path, REPO)})")
    if skips:
        units = ", ".join(u for u, _ in skips)
        print(f"skipped {len(skips)} units (couldn't delink/diff): {units}")

    report["functions"] = rows
    out = args.report or os.path.join(args.build_dir, "report.json")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    json.dump(report, open(out, "w"), indent=2)
    print(f"report -> {os.path.relpath(out, REPO)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
