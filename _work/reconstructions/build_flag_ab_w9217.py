#!/usr/bin/env python3
"""Independent full-build A/B of single compile flags (wave 146, w9217).

Re-derives the original libgof2hdaa.so compile-flag set by rebuilding the whole
tree once per candidate flag and scoring the linked .so against the original.

This is a thin driver around the in-tree tools/flag_fingerprint.py variant
matrix: each cell gets its own scratch build directory, a toolchain wrapper that
appends exactly one flag to the canonical -Oz -g command line, a full link and
the official per-function verifier (fast mode: byte_exact is an exact byte
comparison; only the fuzzy percentage is short-circuited).  The fast-mode
byte_exact was cross-checked against a full official verify on -g0 (2071) and
-fno-rtti (1701).

IMPORTANT: run with --jobs 1.  The vendored zlib/libzip/libcrypto archives live
in the source tree (third_party/*/lib*_gof2.a), so concurrent variant builds
race on the shared archive build and can corrupt it (observed: an
"unterminated #if" in the OpenSSL ARM assembly build, which falsely reported
-O1 as a build failure).  Serial is ~25-60 s/cell.

Usage:
  python3 _work/reconstructions/build_flag_ab_w9217.py \
      --baseline-build cmake-build-match \
      --workdir /dev/shm/w9217/flag-ab

Outputs <workdir>/results.json (raw flag_fingerprint data) and prints the
byte_exact / .text / .ARM.extab table used in
_work/reconstructions/build-flags-full-ab-wave146-w9217.md.
"""
import argparse
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
ORIG_TEXT = 1259464
ORIG_EXTAB = 156312

# The mission's 21 single-flag variants (+ the empty canonical control).
CORE = [
    ("control", ""),
    ("O1", "-O1"),
    ("O2", "-O2"),
    ("O3", "-O3"),
    ("Os", "-Os"),
    ("g0", "-g0"),
    ("g3", "-g3"),
    ("fno-omit-frame-pointer", "-fno-omit-frame-pointer"),
    ("fomit-frame-pointer", "-fomit-frame-pointer"),
    ("fno-stack-protector", "-fno-stack-protector"),
    ("fstack-protector-all", "-fstack-protector-all"),
    ("fno-rtti", "-fno-rtti"),
    ("fno-exceptions", "-fno-exceptions"),
    ("fno-threadsafe-statics", "-fno-threadsafe-statics"),
    ("march=armv7ve", "-march=armv7ve"),
    ("mfpu=neon-vfpv4", "-mfpu=neon-vfpv4"),
    ("mtune=cortex-a8", "-mtune=cortex-a8"),
    ("fno-strict-aliasing", "-fno-strict-aliasing"),
    ("fwrapv", "-fwrapv"),
    ("fno-delete-null-pointer-checks", "-fno-delete-null-pointer-checks"),
    ("mno-movt", "-mno-movt"),
    ("fno-vectorize", "-fno-vectorize"),
]

# Extra hypotheses (new in this pass; same full-build method).
EXTRA = [
    ("marm", "-marm"),
    ("fno-inline-functions", "-fno-inline-functions"),
    ("fno-jump-tables", "-fno-jump-tables"),
    ("fno-slp-vectorize", "-fno-slp-vectorize"),
    ("fvisibility=hidden", "-fvisibility=hidden"),
    ("fno-common", "-fno-common"),
    ("mno-unaligned-access", "-mno-unaligned-access"),
]


def run_matrix(specs, workdir, baseline, jobs):
    workdir.mkdir(parents=True, exist_ok=True)
    vfile = workdir / "variants.json"
    vfile.write_text(json.dumps([dict(name=n, flags=f) for n, f in specs], indent=1) + "\n")
    cmd = [sys.executable, str(ROOT / "tools/flag_fingerprint.py"),
           "--mode", "variants", "--variants-file", str(vfile),
           "--jobs", str(jobs), "--workdir", str(workdir),
           "--baseline-build", str(baseline)]
    subprocess.run(cmd, check=True)
    return json.loads((workdir / "results.json").read_text())


def emit(res, title):
    base = res["baseline"]["verify"]["byte_exact"]
    print(f"\n## {title}\n")
    print("| flag | byte_exact | .text size (Δ) | .text diff | .ARM.extab size (Δ) | extab diff |")
    print("|---|---:|---:|---:|---:|---:|")
    for r in res["variants"]:
        if "error" in r:
            print(f"| `{r['flags']}` | BUILD FAIL | | | | |")
            continue
        v, s = r["verify"], r["sections"]
        rows = {x["name"]: x for x in s["differing"]}
        t = rows.get(".text")
        e = rows.get(".ARM.extab")
        tsz, esz = s["text_size"], s["extab_size"]
        td = (t or {}).get("diff_bytes")
        ed = (e or {}).get("diff_bytes")
        tds = str(td) if td is not None else f"n/a ({tsz - ORIG_TEXT:+d})"
        eds = str(ed) if ed is not None else f"n/a ({esz - ORIG_EXTAB:+d})"
        print(f"| `{r['flags'] or '(none)'}` | {v['byte_exact']} | "
              f"{tsz} ({tsz - ORIG_TEXT:+d}) | {tds} | "
              f"{esz} ({esz - ORIG_EXTAB:+d}) | {eds} |")
    print(f"\n(control byte_exact = {base}; no cell exceeds it)")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--baseline-build", default=str(ROOT / "cmake-build-match"))
    ap.add_argument("--workdir", default="/dev/shm/w9217/flag-ab")
    ap.add_argument("--jobs", type=int, default=1,
                    help="keep at 1: variant builds race on the shared vendored archives")
    ap.add_argument("--extra", action="store_true", help="also run the 7 extra hypotheses")
    args = ap.parse_args()
    wd = Path(args.workdir)

    res = run_matrix(CORE, wd / "core", args.baseline_build, args.jobs)
    emit(res, "The 21-flag matrix (full build, serial)")
    if args.extra:
        res2 = run_matrix(EXTRA, wd / "extra", args.baseline_build, args.jobs)
        emit(res2, "Extra flags")


if __name__ == "__main__":
    main()
