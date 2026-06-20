#!/usr/bin/env python3
"""wave_prepare.py — turn worklist.json into a wave of jobs (Workflow `args`).

A wave takes the FIRST subbatch of each selected component (components are file-disjoint, so the
selected jobs never share a file -> safe to run in parallel). Selection can be filtered by dominant
kind and capped, so waves can be composed (drain the safe `absent` bucket first, etc).

  wave_prepare.py [--kinds absent,extra,...] [--limit N] [--exclude-kinds template,...]
                  [--out path]

Writes the wave JSON to --out (default cmake-build-match/verify/wave.json) and prints a summary.
"""
import argparse
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
VDIR = os.path.join(REPO, "cmake-build-match", "verify")

KEEP = ("symbol", "demangled", "ghidra_addr", "kind", "files", "ours",
        "paired_extra", "paired_absent")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--kinds", default=None, help="comma list of dominant kinds to include")
    ap.add_argument("--exclude-kinds", default=None)
    ap.add_argument("--limit", type=int, default=None)
    ap.add_argument("--max-entries", type=int, default=None,
                    help="skip components whose first subbatch exceeds this size")
    ap.add_argument("--out", default=os.path.join(VDIR, "wave.json"))
    args = ap.parse_args()

    wl = json.load(open(os.path.join(VDIR, "worklist.json")))
    entries = wl["entries"]
    include = set(args.kinds.split(",")) if args.kinds else None
    exclude = set(args.exclude_kinds.split(",")) if args.exclude_kinds else set()

    jobs = []
    for ci, c in enumerate(wl["components"]):
        if not c["subbatches"]:
            continue
        dk = c["dominant_kind"]
        if include is not None and dk not in include:
            continue
        if dk in exclude:
            continue
        sub = c["subbatches"][0]
        if args.max_entries and len(sub) > args.max_entries:
            continue
        je = [{k: entries[i].get(k) for k in KEEP if entries[i].get(k) is not None} for i in sub]
        # a short, file-derived label
        base = (c["files"][0].split("/")[-1].rsplit(".", 1)[0]
                if c["files"] else (c.get("target_file") or dk).split("/")[-1].rsplit(".", 1)[0])
        jobs.append({"label": f"{dk}:{base}:{ci}", "kind": dk, "files": c["files"],
                     "target_file": c.get("target_file"), "entries": je})
        if args.limit and len(jobs) >= args.limit:
            break

    json.dump({"jobs": jobs}, open(args.out, "w"), indent=1)
    n_entries = sum(len(j["entries"]) for j in jobs)
    from collections import Counter
    bk = Counter(j["kind"] for j in jobs)
    print(f"wave: {len(jobs)} components, {n_entries} entries  ({dict(bk)})  -> {os.path.relpath(args.out, REPO)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
