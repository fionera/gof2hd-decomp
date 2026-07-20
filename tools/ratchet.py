#!/usr/bin/env python3
"""ratchet.py -- the machine-checked no-degradation gate.

Compares the current build against the committed baseline (tools/verify_baseline.json) and the
documented irreducible symbol delta (tools/sodiff_allow.txt), enforcing the gate policy that pass
commits previously only promised in prose:

  byte_exact       must never decrease
  linked_exact     may dip 1 below baseline on a single link (section-address transient), no more
  stub_zero_size   must never increase (UB-collapsed bodies)
  missing / wrong_type must stay 0
  extra (verify, FUNC)  must not exceed baseline
  sodiff           defined-export extras beyond the allowlist: none; missing: none
  imports          our UNDEF dynsym set must be a subset of the baseline's (a typo'd callee shows
                   up as a brand-new import -- previously invisible to every tool)
  lint             goal_lint must pass

Freshness is enforced, not assumed: if the .so is older than the newest source file, or the verify
report older than the .so, the stale step is re-run (link via cmake, verify via verify.py directly).

  python3 tools/ratchet.py                     # gate against baseline (build dir cmake-build-match)
  python3 tools/ratchet.py --update-baseline   # lock the current state as the new baseline
  python3 tools/ratchet.py --no-rebuild        # fail instead of rebuilding when stale

Improvements (byte_exact up, imports shrunk, stubs fixed) are reported and require an explicit
--update-baseline in the same commit, keeping the committed baseline the single source of truth.
"""
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
sys.path.insert(0, str(ROOT / "tools" / "verify"))
import sodiff  # noqa: E402
from elf import Elf32  # noqa: E402

BASELINE = ROOT / "tools" / "verify_baseline.json"
ALLOW = ROOT / "tools" / "sodiff_allow.txt"


def cache_var(build_dir, name):
    m = re.search(rf"^{name}:\w+=(.*)$", (build_dir / "CMakeCache.txt").read_text(), re.M)
    return m.group(1).strip() if m else None


def newest_source_mtime():
    return max(p.stat().st_mtime for p in (ROOT / "src").rglob("*") if p.suffix in (".cpp", ".h"))


def run(cmd, **kw):
    print(f"  $ {' '.join(str(c) for c in cmd)}")
    return subprocess.run([str(c) for c in cmd], **kw)


def undef_imports(so_path):
    return sorted({name for name, _info, shndx in sodiff._read_elf32_dynsym(so_path)
                   if shndx == sodiff.SHN_UNDEF})


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--build-dir", default=str(ROOT / "cmake-build-match"))
    ap.add_argument("--update-baseline", action="store_true")
    ap.add_argument("--no-rebuild", action="store_true",
                    help="fail on stale artifacts instead of rebuilding")
    args = ap.parse_args()

    bd = Path(args.build_dir)
    so = bd / "libgof2hdaa.so"
    report = bd / "verify" / "report.json"
    original = cache_var(bd, "GOF2_ORIGINAL_SO")
    objdump = cache_var(bd, "GOF2_NDK_OBJDUMP")
    if not (original and Path(original).exists()):
        print(f"FAIL: original .so not found (GOF2_ORIGINAL_SO={original})"); return 1

    # -- freshness ---------------------------------------------------------------------------
    src_m = newest_source_mtime()
    if not so.exists() or so.stat().st_mtime < src_m:
        if args.no_rebuild:
            print("FAIL: libgof2hdaa.so is stale vs src/ (run the link_so target)"); return 1
        print("stale .so -> relinking")
        if run(["cmake", "--build", bd, "--target", "link_so"]).returncode != 0:
            print("FAIL: link_so failed"); return 1
    if not report.exists() or report.stat().st_mtime < so.stat().st_mtime:
        if args.no_rebuild:
            print("FAIL: verify report is stale vs the .so (run the verify target)"); return 1
        print("stale report -> running verify")
        if run([sys.executable, ROOT / "tools" / "verify" / "verify.py",
                "--build-dir", bd / "verify", "--original", original, "--ours", so,
                "--objdump", objdump, "--fail-on-wrong-type", "--top", "0"]).returncode not in (0, 1):
            print("FAIL: verify.py crashed"); return 1

    summary = json.loads(report.read_text())["summary"]

    # -- gather ------------------------------------------------------------------------------
    allow = {ln.strip() for ln in ALLOW.read_text().splitlines()
             if ln.strip() and not ln.startswith("#")} if ALLOW.exists() else set()
    so_orig = sodiff.defined_exports(str(Path(original)))
    so_ours = sodiff.defined_exports(str(so))
    sod_extra = (so_ours - so_orig) - allow
    sod_missing = so_orig - so_ours
    imports = undef_imports(str(so))

    lint = run([sys.executable, ROOT / "tools" / "goal_lint.py", str(ROOT / "src")],
               capture_output=True, text=True)

    state = dict(
        verify={k: summary[k] for k in ("byte_exact", "linked_exact", "stub_zero_size",
                                        "missing", "wrong_type", "extra", "avg_match",
                                        "compared")},
        sodiff=dict(extra_beyond_allow=sorted(sod_extra), missing=sorted(sod_missing),
                    allowed=len(allow & (so_ours - so_orig))),
        imports=imports,
    )

    if args.update_baseline:
        if sod_extra:
            print("REFUSED: exported symbols beyond tools/sodiff_allow.txt -- document them "
                  "there (and in docs/VALIDATION.md) first:")
            for n in sorted(sod_extra):
                print(f"  {n}")
            return 1
        BASELINE.write_text(json.dumps(
            dict(verify=state["verify"], imports=imports), indent=1) + "\n")
        print(f"baseline updated: {BASELINE.relative_to(ROOT)}")
        print(json.dumps(state["verify"], indent=1))
        return 0

    if not BASELINE.exists():
        print("FAIL: no baseline (run --update-baseline once on a known-good state)"); return 1
    base = json.loads(BASELINE.read_text())
    bv, cv = base["verify"], state["verify"]

    fails, warns, gains = [], [], []

    if cv["byte_exact"] < bv["byte_exact"]:
        fails.append(f"byte_exact {bv['byte_exact']} -> {cv['byte_exact']}")
    elif cv["byte_exact"] > bv["byte_exact"]:
        gains.append(f"byte_exact {bv['byte_exact']} -> {cv['byte_exact']}")
    if cv["linked_exact"] < bv["linked_exact"] - 1:
        fails.append(f"linked_exact {bv['linked_exact']} -> {cv['linked_exact']} (beyond the +-1 band)")
    elif cv["linked_exact"] == bv["linked_exact"] - 1:
        warns.append(f"linked_exact {bv['linked_exact']} -> {cv['linked_exact']} (transient band; "
                     "must recover next link, revert if persistent)")
    elif cv["linked_exact"] > bv["linked_exact"]:
        gains.append(f"linked_exact {bv['linked_exact']} -> {cv['linked_exact']}")
    for key in ("stub_zero_size", "missing", "wrong_type", "extra"):
        if cv[key] > bv[key]:
            fails.append(f"{key} {bv[key]} -> {cv[key]}")
        elif cv[key] < bv[key]:
            gains.append(f"{key} {bv[key]} -> {cv[key]}")

    if sod_missing:
        fails.append(f"sodiff missing exports: {len(sod_missing)} "
                     f"(e.g. {sorted(sod_missing)[:3]})")
    if sod_extra:
        fails.append(f"exported symbols beyond allowlist: {sorted(sod_extra)}")

    new_imports = sorted(set(imports) - set(base.get("imports", [])))
    if new_imports:
        fails.append(f"NEW undefined imports (typo'd callee / new shim -- must be added "
                     f"deliberately via --update-baseline): {new_imports[:10]}"
                     + (f" ... +{len(new_imports) - 10}" if len(new_imports) > 10 else ""))
    dropped = len(set(base.get("imports", []))) - len(set(imports) & set(base.get("imports", [])))
    if dropped:
        gains.append(f"imports shrank by {dropped}")

    if lint.returncode != 0:
        fails.append("goal_lint failed:\n" + (lint.stdout + lint.stderr).strip()[-2000:])

    # -- verdict -----------------------------------------------------------------------------
    print(f"verify: byte {cv['byte_exact']}  linked {cv['linked_exact']}  "
          f"stubs {cv['stub_zero_size']}  missing {cv['missing']}  extra {cv['extra']}  "
          f"avg {cv['avg_match']}")
    print(f"sodiff: extra-beyond-allow {len(sod_extra)}  missing {len(sod_missing)}  "
          f"allowed {state['sodiff']['allowed']}   imports: {len(imports)}")
    for g in gains:
        print(f"  GAIN  {g} (lock with --update-baseline)")
    for w in warns:
        print(f"  WARN  {w}")
    for f in fails:
        print(f"  FAIL  {f}")
    print("RATCHET " + ("PASS" if not fails else "FAIL"))
    return 0 if not fails else 1


if __name__ == "__main__":
    sys.exit(main())
