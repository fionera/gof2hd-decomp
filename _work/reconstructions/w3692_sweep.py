#!/usr/bin/env python3
"""w3692 per-TU flag sweep driver.

For every non-linked function in cmake-build-match/verify/report.json, find its TU,
compile that TU once per flag set with the w3692 object-level oracle, and report any
function that becomes instruction-exact. Run from the repo root after a baseline build:

  python3 _work/reconstructions/w3692_sweep.py

The flag list is the 125 unique `-mllvm` options in mllvm_flag_audit.md plus the extra
combinations and driver flags used in per_tu_mllvm_and_assume_audit_w3692.md.
"""
import json
import multiprocessing as mp
import os
import re
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
import w3692_oracle as o  # noqa: E402

NM = o.NM
REPORT = json.load(open(ROOT / "cmake-build-match/verify/report.json"))
BAD = [r for r in REPORT["rows"] if not r["linked"]]
NAMES = {r["name"] for r in BAD}

# symbol -> source TU
OBJS = subprocess.check_output(
    ["find", str(ROOT / "cmake-build-match/CMakeFiles/gof2.dir/src"), "-name", "*.obj"]
).decode().split()
SYM2SRC = {}
for obj in OBJS:
    out = subprocess.check_output([NM, "-S", "--defined-only", obj], text=True, errors="replace")
    for ln in out.splitlines():
        p = ln.split()
        if len(p) >= 4 and p[3] in NAMES:
            SYM2SRC[p[3]] = str(ROOT / "src" / obj.split("/src/", 1)[1][:-4])
GROUPS = {}
for r in BAD:
    GROUPS.setdefault(SYM2SRC[r["name"]], []).append(r)


def flag_sets():
    sets = []
    note = (ROOT / "_work/reconstructions/mllvm_flag_audit.md").read_text()
    for line in note.splitlines():
        if "`-mllvm" in line:
            for m in re.finditer(r"`(-mllvm[^`]*)`", line):
                v = m.group(1).split()
                if v and v != ["-mllvm"]:
                    sets.append(v)
    sets += [
        ["-mllvm", "-likely-branch-weight=80", "-mllvm", "-unlikely-branch-weight=1"],
        ["-mllvm", "-likely-branch-weight=20", "-mllvm", "-unlikely-branch-weight=12"],
        ["-mllvm", "-likely-branch-weight=2000", "-mllvm", "-unlikely-branch-weight=1"],
        ["-mllvm", "-likely-branch-weight=1", "-mllvm", "-unlikely-branch-weight=1"],
        ["-mllvm", "-stress-regalloc=4"], ["-mllvm", "-stress-regalloc=8"],
        ["-mllvm", "-stress-regalloc=16"],
        ["-mllvm", "-regalloc-csr-first-time-cost=1"],
        ["-mllvm", "-regalloc-csr-first-time-cost=3"],
        ["-mllvm", "-tail-dup-size=8"], ["-mllvm", "-tail-dup-size=16"],
        ["-mllvm", "-tail-dup-size=32"],
        ["-mllvm", "-tail-merge-size=8"], ["-mllvm", "-tail-merge-size=16"],
        ["-mllvm", "-jump-table-density=5"], ["-mllvm", "-jump-table-density=10"],
        ["-mllvm", "-block-placement-exit-block-bias=10"],
        ["-mllvm", "-block-placement-exit-block-bias=50"],
        ["-mllvm", "-inline-threshold=50"], ["-mllvm", "-inline-threshold=200"],
        ["-mllvm", "-unroll-threshold=50"], ["-mllvm", "-unroll-threshold=100"],
        ["-mllvm", "-sink-freq-percent-threshold=10"],
        ["-mllvm", "-sink-freq-percent-threshold=50"],
        ["-mllvm", "-split-spill-mode=default"],
        ["-mllvm", "-enable-deferred-spilling=false"],
        ["-mllvm", "-enable-local-reassign=false"],
        ["-mllvm", "-enable-shrink-wrap=true"],
        ["-mllvm", "-enable-tail-merge=true"],
        ["-mllvm", "-tail-dup-placement=true"],
        ["-mllvm", "-branch-fold-placement=true"],
        ["-mllvm", "-arm-adjust-jump-tables=true"],
        ["-mllvm", "-enable-load-pre=true"], ["-mllvm", "-enable-pre=true"],
        ["-mllvm", "-enable-post-misched=true"],
        ["-mllvm", "-enable-cond-stores-vec=true"],
        ["-mllvm", "-arm-assume-misaligned-load-store=true"],
        ["-mllvm", "-arm-synthesize-thumb-1-tbb=true"],
        ["-mllvm", "-align-neon-spills=true"],
    ]
    for f in [
        "-O2", "-O3", "-Os", "-O1", "-O0", "-fno-inline-functions-called-once",
        "-fno-delete-null-pointer-checks", "-fno-threadsafe-statics",
        "-fomit-frame-pointer", "-fno-stack-protector", "-fno-unroll-loops",
        "-fno-vectorize", "-fno-slp-vectorize", "-fno-jump-tables", "-mno-movt",
        "-fno-sized-deallocation", "-fno-rtti", "-fno-strict-aliasing", "-fwrapv",
        "-fno-strict-overflow", "-mno-unaligned-access", "-fno-omit-frame-pointer",
        "-fno-common", "-fno-builtin", "-mcpu=cortex-a8", "-mcpu=cortex-a9",
        "-mcpu=cortex-a15", "-mthumb", "-fno-inline", "-finline-functions",
        "-fno-inline-functions", "-fmerge-all-constants", "-fno-merge-all-constants",
        "-fno-optimize-sibling-calls", "-foptimize-sibling-calls", "-fno-ipa-cp",
        "-fno-ipa-sra", "-fno-exceptions", "-fno-unwind-tables",
        "-fno-asynchronous-unwind-tables", "-fno-use-cxa-atexit",
        "-fno-lax-vector-conversions",
    ]:
        sets.append([f])
    seen, out = set(), []
    for s in sets:
        k = tuple(s)
        if k not in seen:
            seen.add(k)
            out.append(s)
    return out


FLAGS = flag_sets()


def eval_tu(task):
    src, extra, tag = task
    out = f"/tmp/w3692_sweep_{os.getpid()}_{tag}.obj"
    try:
        o.compile_tu(src, extra, out)
    except Exception as e:
        return src, tag, None, str(e)[-300:]
    res = {}
    try:
        nm = subprocess.check_output([NM, "-S", out], text=True, errors="replace")
        hdr = subprocess.check_output([o.OD, "-h", out], text=True, errors="replace")
        actual = {ln.split()[1] for ln in hdr.splitlines()
                  if ln.split() and ln.split()[0].isdigit()}
        secs = {}
        for ln in nm.splitlines():
            p = ln.split()
            if len(p) >= 4 and (".text." + p[3]) in actual:
                secs[p[3]] = ".text." + p[3]
        for r in GROUPS[src]:
            name = r["name"]
            sec = secs.get(name)
            if not sec and "C1" in name:
                alt = name.replace("C1", "C2")
                sec = secs.get(alt)
            if not sec:
                res[name] = dict(error="no symbol")
                continue
            ob = o.section_bytes(out, sec)
            _sl, relocs = o.objdump_section(out, sec)
            fd, path = tempfile.mkstemp(suffix=".bin")
            os.write(fd, ob)
            os.close(fd)
            try:
                lines = o.verify._objdump_lines(o.OD, path, True)
            finally:
                os.unlink(path)
            f, blob, onorm = o.norm_orig(name)
            unorm = o.norm_obj(ob, lines, relocs, len(ob))
            exact = len(ob) == len(blob) and onorm == unorm
            pct = 100.0 if exact else round(
                __import__("difflib").SequenceMatcher(None, onorm, unorm,
                                                      autojunk=False).ratio() * 100, 1)
            res[name] = dict(pct=pct, exact=exact, osize=len(blob), usize=len(ob))
    finally:
        try:
            os.unlink(out)
        except OSError:
            pass
    return src, tag, res, None


def main():
    tasks = [(src, fl, tag.replace(" ", "_")[:80])
             for src in GROUPS for tag, fl in [((" ".join(f)), f) for f in FLAGS]]
    print(f"tasks {len(tasks)} flags {len(FLAGS)} TUs {len(GROUPS)}", flush=True)
    t0 = time.time()
    results = {}
    with mp.Pool(24) as pool:
        for i, (src, tag, res, err) in enumerate(pool.imap_unordered(eval_tu, tasks)):
            results.setdefault(src, {})[tag] = dict(res=res, err=err)
            if (i + 1) % 200 == 0:
                print(f"done {i+1}/{len(tasks)} {time.time()-t0:.1f}s", flush=True)
    json.dump(results, open("/tmp/w3692_sweep_results.json", "w"))
    print(f"elapsed {time.time()-t0:.1f}s")
    for src, vs in results.items():
        for r in GROUPS[src]:
            best = (0.0, "BASE", r["usize"])
            for tag, v in vs.items():
                x = (v["res"] or {}).get(r["name"])
                if x and x.get("pct", 0) > best[0]:
                    best = (x["pct"], tag, x["usize"])
            if best[1] != "BASE":
                print(f"{r['name']} {r['pct']} -> {best[0]} {best[1]} size {best[2]}")


if __name__ == "__main__":
    main()
