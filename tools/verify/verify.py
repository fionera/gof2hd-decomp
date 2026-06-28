#!/usr/bin/env python3
"""verify.py -- per-function ASM match of our build vs the original libgof2hdaa.so.

Both binaries are fully linked, so we compare function-for-function directly: for every function
symbol defined in BOTH .so files, pull its bytes (via .dynsym st_value/st_size), disassemble each
side with the NDK objdump in the symbol's ARM/Thumb mode, and score the match:

  byte_exact     raw bytes identical (leaf code with no address-dependent operands)
  linked_exact   normalized disassembly identical -- same instructions, operands may point at
                 different absolute addresses (the realistic target: load-address differences and
                 distinct-but-equivalent literal pools are expected between two independent links)
  fuzzy %        difflib ratio over the normalized instruction stream (0..100)

Also reports the symbol-set deltas (so verify alone tells the whole story):
  missing        function in the original, not in our build
  wrong_type     same demangled Class::method, different signature
  extra          function in our build, not in the original

Writes report.json + a worst-first table. `--show <mangled>` prints a side-by-side disassembly.
Exit non-zero if any function is missing or (with --fail-on-wrong-type) any wrong_type exists.
"""
import argparse
import difflib
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from elf import Elf32  # noqa: E402

HEX = re.compile(r"\b0x[0-9a-fA-F]+\b")
ADDR_COL = re.compile(r"^\s*[0-9a-fA-F]+:\s*(?:[0-9a-fA-F]{2,8}\s+)+\t(.*)$")
# objdump annotates absolute targets as "<symbol+0x..>" / "; 0x..." -- noise for a structural match.
ANNOT = re.compile(r"\s*(?:<[^>]*>|;.*)$")


def demangle_map(names):
    for tool in ("c++filt", "llvm-cxxfilt"):
        try:
            p = subprocess.run([tool], input="\n".join(names), capture_output=True,
                               text=True, check=True)
            return dict(zip(names, p.stdout.splitlines()))
        except (FileNotFoundError, subprocess.CalledProcessError):
            continue
    return {n: n for n in names}


def qualified(demangled):
    s, depth = demangled, 0
    for i, c in enumerate(demangled):
        if c == "(" and depth == 0:
            s = demangled[:i]; break
        depth += {"<": 1, ">": -1}.get(c, 0)
    s, depth, cut = s.strip(), 0, -1
    for i, c in enumerate(s):
        depth += {"<": 1, ">": -1, "(": 1, ")": -1}.get(c, 0)
        if c == " " and depth == 0:
            cut = i
    return s[cut + 1:] if cut >= 0 else s


def disassemble(objdump, blob, thumb):
    """Return (raw_lines, normalized_lines) for a code blob disassembled in the right mode."""
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
        f.write(blob); tmp = f.name
    try:
        cmd = [objdump, "-D", "-b", "binary", "-m", "arm",
               "-M", "force-thumb" if thumb else "reg-names-std", tmp]
        out = subprocess.run(cmd, capture_output=True, text=True).stdout
    finally:
        os.unlink(tmp)
    raw, norm = [], []
    for ln in out.splitlines():
        m = ADDR_COL.match(ln)
        if not m:
            continue
        insn = ANNOT.sub("", m.group(1)).strip()
        raw.append(insn)
        # normalize: collapse absolute hex operands so independent links still match structurally
        norm.append(HEX.sub("#A", insn))
    return raw, norm


def score(a_norm, b_norm):
    if a_norm == b_norm:
        return 100.0
    if not a_norm and not b_norm:
        return 100.0
    return round(difflib.SequenceMatcher(None, a_norm, b_norm).ratio() * 100, 1)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--original", required=True)
    ap.add_argument("--ours", required=True)
    ap.add_argument("--objdump", default=os.environ.get("GOF2_NDK_OBJDUMP", "objdump"))
    ap.add_argument("--build-dir", default=".")
    ap.add_argument("--fail-on-wrong-type", action="store_true")
    ap.add_argument("--show", metavar="MANGLED", help="side-by-side disasm for one symbol")
    ap.add_argument("--top", type=int, default=40, help="worst-N to print")
    args = ap.parse_args()

    orig = Elf32(args.original).functions()
    ours_elf = Elf32(args.ours)
    ours = ours_elf.functions()
    of = Elf32(args.original)

    if args.show:
        name = args.show
        if name not in orig or name not in ours:
            print(f"{name}: present orig={name in orig} ours={name in ours}"); return 2
        o, u = orig[name], ours[name]
        oraw, _ = disassemble(args.objdump, of.bytes_at(o["value"], o["size"]), o["thumb"])
        uraw, _ = disassemble(args.objdump, ours_elf.bytes_at(u["value"], u["size"]), u["thumb"])
        print(demangle_map([name])[name])
        for ol, ul in zip(oraw, uraw):
            mark = " " if ol.split("\t")[0] == ul.split("\t")[0] else "|"
            print(f"  {ol:38} {mark} {ul}")
        if len(oraw) != len(uraw):
            print(f"  (length differs: orig {len(oraw)} vs ours {len(uraw)} insns)")
        return 0

    common = sorted(set(orig) & set(ours))
    missing = set(orig) - set(ours)
    extra = set(ours) - set(orig)

    dm = demangle_map(missing | extra)
    extra_q = {qualified(dm[n]) for n in extra}
    wrong_type = {n for n in missing if qualified(dm[n]) in extra_q}
    absent = missing - wrong_type

    rows, byte_exact, linked_exact, fuzzy_perfect = [], 0, 0, 0
    for name in common:
        o, u = orig[name], ours[name]
        ob = of.bytes_at(o["value"], o["size"])
        ub = ours_elf.bytes_at(u["value"], u["size"])
        be = ob == ub
        _, on = disassemble(args.objdump, ob, o["thumb"])
        _, un = disassemble(args.objdump, ub, u["thumb"])
        le = on == un
        pct = 100.0 if be else score(on, un)
        byte_exact += be
        linked_exact += le
        fuzzy_perfect += pct == 100.0
        rows.append(dict(name=name, pct=pct, byte=be, linked=le,
                         osize=o["size"], usize=u["size"]))

    rows.sort(key=lambda r: (r["pct"], r["name"]))
    avg = round(sum(r["pct"] for r in rows) / len(rows), 2) if rows else 0.0
    summary = dict(original_functions=len(orig), ours_functions=len(ours),
                   compared=len(common), avg_match=avg, byte_exact=byte_exact,
                   linked_exact=linked_exact, fuzzy_perfect=fuzzy_perfect,
                   missing=len(missing), wrong_type=len(wrong_type), absent=len(absent),
                   extra=len(extra))

    bd = Path(args.build_dir); bd.mkdir(parents=True, exist_ok=True)
    (bd / "report.json").write_text(json.dumps(
        dict(summary=summary, rows=rows,
             missing=sorted(missing), extra=sorted(extra), wrong_type=sorted(wrong_type)), indent=1))

    print(f"compared {len(common)}  avg {avg}%  byte_exact {byte_exact}  "
          f"linked_exact {linked_exact}  fuzzy_perfect {fuzzy_perfect}")
    print(f"missing {len(missing)} (wrong_type {len(wrong_type)}  absent {len(absent)})  "
          f"extra {len(extra)}")
    print(f"\nworst {min(args.top, len(rows))} matched functions:")
    for r in rows[:args.top]:
        flag = "B" if r["byte"] else ("L" if r["linked"] else " ")
        print(f"  {r['pct']:6.1f}% {flag} {dm.get(r['name'], r['name'])[:90]}")

    ok = not missing and not (args.fail_on_wrong_type and wrong_type)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
