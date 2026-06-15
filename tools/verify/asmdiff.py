#!/usr/bin/env python3
"""Disassemble two ARM objects with arm-linux-gnueabihf-objdump and compare them
function-by-function. Used by verify.py.

objdiff cannot read this binary (its ARM disassembler caps at ARMv6K; the game is
ARMv7-A Thumb-2 + VFP/NEON), so we disassemble with GNU objdump — which fully
supports ARMv7 — and do our own normalize + fuzzy match.

For each symbol present in BOTH objects we report:
  * bytes_equal  : raw instruction bytes identical (the gold byte-match signal)
  * match        : fuzzy instruction match 0..100 (difflib ratio over normalized
                   mnemonics+operands; tolerant of post-link absolute branch/pool
                   addresses that have no relocation in the delinked target)
"""
import difflib
import os
import re
import subprocess

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_OBJDUMP = os.path.join(HERE, "orbobjdump")

_BRANCH = re.compile(r"^(b|bl|blx|bx|cb|cbz|cbnz|beq|bne|bcs|bhs|bcc|blo|bmi|bpl|"
                     r"bvs|bvc|bhi|bls|bge|blt|bgt|ble|bal)(\.[wn])?$")
_HEXNUM = re.compile(r"0x[0-9a-fA-F]+")
_ANGLE = re.compile(r"\s*<[^>]*>")          # objdump symbol annotation
_PCREL = re.compile(r"\[pc,\s*#-?\d+\]")    # literal-pool load offset


def normalize(mnem, ops):
    """Reduce one instruction to a comparable token, neutralizing addresses that
    differ only because the target is post-link (no relocations)."""
    ops = ops.split(";")[0].split("@")[0].strip()      # drop comments
    ops = _ANGLE.sub("", ops)                          # drop <sym+0x..> notes
    ops = re.sub(r"\s+", " ", ops).strip()
    base = mnem.split(".")[0]
    if _BRANCH.match(mnem) or base in ("b", "bl", "blx", "bx"):
        return mnem.split(".")[0] + " <t>"             # branch/call target -> placeholder
    # pointer materialization & pool loads carry absolute values post-link
    if base in ("movw", "movt"):
        ops = re.sub(r"#\S+", "#i", ops)
    ops = _PCREL.sub("[pc]", ops)
    ops = _HEXNUM.sub("#x", ops)
    return f"{mnem} {ops}".strip()


def _symbol_sizes(lines):
    """Parse `objdump -t` symbol table -> {name: size_bytes}."""
    sizes, in_tab = {}, False
    for line in lines:
        if line.startswith("SYMBOL TABLE:"):
            in_tab = True
            continue
        if in_tab:
            if not line.strip() or line.startswith("Disassembly"):
                in_tab = False
                continue
            # "0000000a g     F .text\t0000001c _ZN..."
            if "\t" not in line:
                continue
            right = line.split("\t", 1)[1]
            parts = right.split(None, 1)
            if len(parts) == 2:
                try:
                    sizes[parts[1].strip()] = int(parts[0], 16)
                except ValueError:
                    pass
    return sizes


def disassemble(obj, objdump=DEFAULT_OBJDUMP):
    """{mangled symbol -> {'insns':[normalized...], 'bytes':'hex'}}.

    Each function is truncated to its real symbol size so trailing alignment
    padding (attributed by objdump to the preceding symbol) never pollutes the
    comparison."""
    out = subprocess.check_output([objdump, "-d", "-t", obj], text=True)
    lines = out.splitlines()
    sizes = _symbol_sizes(lines)
    funcs, cur, start, limit = {}, None, 0, None
    for line in lines:
        m = re.match(r"^([0-9a-f]+) <(.+)>:$", line)
        if m:
            start = int(m.group(1), 16)
            sz = sizes.get(m.group(2))
            limit = start + sz if sz else None
            cur = {"insns": [], "bytes": ""}
            funcs[m.group(2)] = cur
            continue
        if cur is None:
            continue
        # "   a:\t68 0a       \tldr\tr2, [r1, #0]"   (objdump -d, raw bytes shown)
        m = re.match(r"^\s*([0-9a-f]+):\t([0-9a-f ]+)\t(\S+)\s*(.*)$", line)
        if not m:
            continue
        addr = int(m.group(1), 16)
        if limit is not None and addr >= limit:
            continue
        raw, mnem, ops = m.group(2), m.group(3), m.group(4)
        if mnem in (".word", ".short", ".byte"):
            cur["insns"].append(mnem + " " + _HEXNUM.sub("#x", ops).strip())
        else:
            cur["insns"].append(normalize(mnem, ops))
        cur["bytes"] += raw.replace(" ", "")
    return funcs


def compare(target_funcs, base_funcs):
    """List of per-symbol dicts for symbols defined in both, sorted worst-first."""
    rows = []
    for sym, b in base_funcs.items():
        t = target_funcs.get(sym)
        if t is None:
            continue
        ratio = difflib.SequenceMatcher(None, t["insns"], b["insns"]).ratio()
        rows.append({
            "symbol": sym,
            "match": round(ratio * 100, 1),
            "bytes_equal": t["bytes"] == b["bytes"],
            "n_target": len(t["insns"]),
            "n_base": len(b["insns"]),
        })
    rows.sort(key=lambda r: (r["match"], r["symbol"]))
    return rows


def unified(target_funcs, base_funcs, sym):
    """Human-readable side-by-side diff of one symbol's normalized disassembly."""
    t = target_funcs.get(sym, {}).get("insns", [])
    b = base_funcs.get(sym, {}).get("insns", [])
    diff = difflib.unified_diff(t, b, fromfile="ORIGINAL (.so)", tofile="OURS (build)",
                                lineterm="")
    return "\n".join(diff)
