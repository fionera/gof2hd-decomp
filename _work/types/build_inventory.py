#!/usr/bin/env python3
"""Build the per-class cleanup inventory for the readable-cleanup sweep.

Outputs _work/types/inventory.json: one entry per .cpp translation unit that
still carries decompiler artifacts, with artifact site counts, the paired
header, subsystem, and matched DeepOpen Java reference (if any).
"""
import json, os, re, subprocess, sys
from collections import OrderedDict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # gof2-decomp
SRC = os.path.join(ROOT, "src")
DEEPOPEN = os.path.abspath(os.path.join(ROOT, "..", "DeepOpen", "code", "GoF2_jsr", "src"))

# Artifact patterns (line-counted per file).
PATTERNS = OrderedDict([
    ("field_0x",   re.compile(r"field_0x[0-9a-f]+")),
    ("op_new_del", re.compile(r"::operator\s+(new|delete)")),
    ("helpers",    re.compile(r"\b(i32|u32|f32|u8|u16|i16|pp|field_i32|field_f32|field_ptr|field_u8|FL|UC|US|BL)\s*\(")),
    ("raw_cast",   re.compile(r"\*\([A-Za-z0-9_ ]+\*\)\(\(char ?\*\)")),
    ("int_member", re.compile(r"\bint\s+field_0x[0-9a-f]+\s*;")),
])

# Build DeepOpen basename -> path map.
deepopen = {}
for dp, _, fns in os.walk(DEEPOPEN):
    for fn in fns:
        if fn.endswith(".java"):
            deepopen.setdefault(fn[:-5], os.path.join(dp, fn))

def count_artifacts(path):
    counts = {k: 0 for k in PATTERNS}
    total = 0
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            txt = f.read()
    except OSError:
        return counts, 0
    for k, pat in PATTERNS.items():
        n = len(pat.findall(txt))
        counts[k] = n
        total += n
    return counts, total

entries = []
for dp, _, fns in os.walk(SRC):
    for fn in sorted(fns):
        if not fn.endswith(".cpp"):
            continue
        cpp = os.path.join(dp, fn)
        rel = os.path.relpath(cpp, ROOT)
        stem = fn[:-4]
        hdr = os.path.join(dp, stem + ".h")
        hdr_rel = os.path.relpath(hdr, ROOT) if os.path.exists(hdr) else None
        cpp_counts, cpp_total = count_artifacts(cpp)
        hdr_counts, hdr_total = (count_artifacts(hdr) if hdr_rel else ({}, 0))
        # subsystem = first two path components under src/
        parts = os.path.relpath(cpp, SRC).split(os.sep)
        subsystem = "/".join(parts[:-1]) if len(parts) > 1 else "(root)"
        total = cpp_total + hdr_total
        if total == 0:
            continue
        entries.append({
            "class": stem,
            "cpp": rel,
            "header": hdr_rel,
            "subsystem": subsystem,
            "artifacts_total": total,
            "cpp_counts": cpp_counts,
            "header_counts": hdr_counts,
            "deepopen": os.path.relpath(deepopen[stem], ROOT) if stem in deepopen else None,
        })

entries.sort(key=lambda e: -e["artifacts_total"])
out = {
    "src_root": SRC,
    "deepopen_root": DEEPOPEN,
    "total_dirty_tus": len(entries),
    "entries": entries,
}
outpath = os.path.join(ROOT, "_work", "types", "inventory.json")
with open(outpath, "w") as f:
    json.dump(out, f, indent=1)

# Subsystem rollup
roll = OrderedDict()
for e in entries:
    roll.setdefault(e["subsystem"], [0, 0])
    roll[e["subsystem"]][0] += 1
    roll[e["subsystem"]][1] += e["artifacts_total"]
print("dirty TUs:", len(entries), "  with DeepOpen ref:", sum(1 for e in entries if e["deepopen"]))
print("\nsubsystem                         files  artifacts")
for ss, (nf, na) in sorted(roll.items(), key=lambda x: -x[1][1]):
    print(f"  {ss:30s} {nf:5d} {na:9d}")
print("\nwrote", os.path.relpath(outpath, ROOT))
