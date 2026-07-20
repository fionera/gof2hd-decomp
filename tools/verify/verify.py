#!/usr/bin/env python3
"""verify.py -- per-function ASM match of our build vs the original libgof2hdaa.so.

Both binaries are fully linked, so we compare function-for-function directly: for every function
symbol defined in BOTH .so files, pull its bytes (via .dynsym st_value/st_size), disassemble each
side with the NDK objdump in the symbol's ARM/Thumb mode, and score the match:

  byte_exact     raw bytes identical (leaf code with no address-dependent operands)
  linked_exact   normalized disassembly identical AND same size -- same instructions, where
                 address-dependent operands are compared *structurally*:
                   - branch/call targets INSIDE the function are function-relative and
                     link-independent -> compared literally (a different intra-function branch
                     is a real difference, never masked)
                   - branch/call targets OUTSIDE the function are resolved to the containing
                     .dynsym symbol and compared BY NAME (a call to the wrong function fails
                     even though every absolute address differs between the two links)
                   - unresolvable targets (local/static code) and remaining absolute-address
                     operands are masked, since two independent links legitimately differ there
  fuzzy %        difflib ratio over the normalized instruction stream (0..100), autojunk OFF
                 (autojunk floors the ratio for repetitive streams >=200 elements -- i.e. all
                 large ASM functions -- steering the worst-first list with garbage numbers)

Zero-size symbols are classified, not dropped:
  stub_zero_size   defined in both, original has a body, ours is size 0 -- a UB-collapsed body
                   (fall-off-the-end of a non-void fn, call through a never-set static fn ptr).
                   These alias a neighbouring function's address: broken code, not "missing".
  orig_zero_size   original exports the symbol with size 0 (e.g. getAppVersion) -- nothing to
                   compare against; tracked, excluded from scoring.

Also reports the symbol-set deltas (so verify alone tells the whole story):
  missing        function in the original, not in our build at all
  wrong_type     same demangled Class::method, different signature
  extra          function in our build, not in the original

Writes report.json + a worst-first table. `--show <mangled>` prints a side-by-side disassembly
(markers: '|' structural difference, '~' raw-only difference such as a shifted literal pool).
Exit non-zero if any function is missing or (with --fail-on-wrong-type) any wrong_type exists;
stub_zero_size does NOT fail the run -- the ratchet (tools/ratchet.py) pins its count instead.
"""
import argparse
import bisect
import difflib
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from elf import Elf32  # noqa: E402

HEX = re.compile(r"\b0x[0-9a-fA-F]+\b")
ADDR_COL = re.compile(r"^\s*([0-9a-fA-F]+):\s*(?:[0-9a-fA-F]{2,8}\s+)+\t(.*)$")
# objdump annotates absolute targets as "<symbol+0x..>" / "; 0x..." -- noise for a structural match.
ANNOT = re.compile(r"\s*(?:<[^>]*>|;.*)$")
# PC-relative control flow with an immediate target: b/bl/blx (+condition, +.n/.w) and cbz/cbnz.
# Anchored on the full mnemonic so bic/bfi/bkpt/bx-to-register never match.
BRANCH = re.compile(
    r"^(?:cbn?z|b(?:l|lx)?(?:eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?(?:\.[nw])?)$")
# PC-relative loads mark their target words as literal-pool DATA: those bytes must not be compared
# as (garbage-decoded) instructions -- a pool word can decode as a fake branch whose "target"
# resolves into whatever function happens to be nearby, differing between the two layouts even
# when the bytes are identical.
PCREL_LOAD = re.compile(
    r"^(ldr|ldrd|ldrsb|ldrsh|ldrb|ldrh|vldr)(?:\.[nw])?\s+(\w+)(?:\s*,\s*(?!\[)\w+)?"
    r"\s*,\s*\[pc(?:,\s*#(-?\d+))?\]")


class Resolver:
    """Map a virtual address to an exported function-symbol name.

    Only names defined in BOTH binaries (`common`) are ever returned -- anything else masks to #X
    like local code, because the other side can never produce the same name (e.g. our shim-import
    call via PLT stands for a local, stripped function in the original; naming it on one side only
    would fail every such call pair). Where several common names share one address (C1/C2 ctor
    aliases), the lexicographically first is used so both sides pick the same string. PLT entries
    resolve to the symbol they trampoline to, so a PLT-routed call and a directly-bound call to
    the same callee compare equal.
    """

    def __init__(self, functions, common, plt=None, addr_range=(0, 0)):
        by_addr = {}
        for name, f in functions.items():
            by_addr.setdefault(f["value"], []).append((name, f["size"]))
        self.starts = sorted(by_addr)
        self.by_addr = by_addr
        self.common = common
        self.plt = plt or {}
        self.addr_range = addr_range  # [lo, hi): pool words in this range look like addresses

    def resolve(self, addr):
        addr &= ~1
        plt_name = self.plt.get(addr)
        if plt_name is not None:
            return plt_name if plt_name in self.common else None
        i = bisect.bisect_right(self.starts, addr) - 1
        if i < 0:
            return None
        start = self.starts[i]
        names = [n for n, size in self.by_addr[start]
                 if addr == start or addr < start + size]
        preferred = sorted(n for n in names if n in self.common)
        return preferred[0] if preferred else None

    def resolve_entry(self, addr):
        """Like resolve(), but only an ENTRY-exact hit counts (no mid-function spans, no PLT).

        For literal-pool words: a real function-pointer datum holds an exact entry address
        (with the Thumb bit), while link-dependent constants that merely look like addresses
        (GOT-slot deltas, rodata pointers) land mid-span in whatever function is nearby --
        naming those would produce random cross-link mismatches.
        """
        names = self.by_addr.get(addr & ~1, ())
        preferred = sorted(n for n, _size in names if n in self.common)
        return preferred[0] if preferred else None


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


def normalize(insn, blob_size, base, resolver):
    """Structural form of one instruction (see module docstring for the rules)."""
    parts = insn.split(None, 1)
    if len(parts) == 2 and BRANCH.match(parts[0]):
        mnem, ops = parts
        m = None
        for m in HEX.finditer(ops):
            pass  # keep the LAST hex token (cbz has a register first)
        if m is not None:
            t = int(m.group(0), 16)
            if t < blob_size:
                sub = f"loc_{t:x}"          # function-relative: compare literally
            else:
                name = resolver.resolve((base + t) & 0xFFFFFFFF) if resolver else None
                sub = f"sym:{name}" if name else "#X"
            insn = f"{mnem}\t{ops[:m.start()]}{sub}{ops[m.end():]}"
    # remaining absolute hex operands legitimately differ between two independent links
    return HEX.sub("#A", insn)


def pool_norm(blob, off, resolver):
    """Structural form of a literal-pool word: constants compare by exact value, words that look
    like addresses compare by the symbol they reference (or a neutral mask -- two independent
    links legitimately place the same datum at different addresses)."""
    word_off = off & ~3
    if word_off + 4 > len(blob):
        return "pool:short"
    val = struct.unpack_from("<I", blob, word_off)[0]
    lo, hi = resolver.addr_range if resolver else (0, 0)
    if lo <= val < hi:
        name = resolver.resolve_entry(val) if resolver else None
        return f"pool:sym:{name}" if name else "pool:addr"
    return f"pool:0x{val:x}"


def disassemble(objdump, blob, thumb, base=0, resolver=None):
    """Return (raw_lines, normalized_lines) for a code blob disassembled in the right mode."""
    with tempfile.NamedTemporaryFile(suffix=".bin", delete=False) as f:
        f.write(blob); tmp = f.name
    try:
        cmd = [objdump, "-D", "-b", "binary", "-m", "arm",
               "-M", "force-thumb" if thumb else "reg-names-std", tmp]
        out = subprocess.run(cmd, capture_output=True, text=True).stdout
    finally:
        os.unlink(tmp)
    lines = []
    for ln in out.splitlines():
        m = ADDR_COL.match(ln)
        if not m:
            continue
        lines.append((int(m.group(1), 16), ANNOT.sub("", m.group(2)).strip()))
    # pass 1: literal-pool bytes = targets of pc-relative loads (ARM pc bias +8, Thumb +4 word-aligned)
    pool = set()
    for addr, insn in lines:
        pm = PCREL_LOAD.match(insn)
        if pm:
            mnem, reg, imm = pm.group(1), pm.group(2), int(pm.group(3) or 0)
            tgt = ((addr + (4 if thumb else 8)) & ~3) + imm
            width = 8 if (mnem == "ldrd" or (mnem == "vldr" and reg.startswith("d"))) else 4
            pool.update(range(tgt, tgt + width))
    raw, norm = [], []
    for addr, insn in lines:
        raw.append(insn)
        if addr in pool:
            norm.append(pool_norm(blob, addr, resolver))
        else:
            norm.append(normalize(insn, len(blob), base, resolver))
    return raw, norm


def score(a_norm, b_norm):
    if a_norm == b_norm:
        return 100.0
    if not a_norm and not b_norm:
        return 100.0
    return round(difflib.SequenceMatcher(None, a_norm, b_norm, autojunk=False)
                 .ratio() * 100, 1)


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

    of = Elf32(args.original)
    ours_elf = Elf32(args.ours)
    orig = of.functions()
    ours = ours_elf.functions()
    common = set(orig) & set(ours)

    def addr_range(elf):
        """[.plt start, top of mapped image): pool words in here look like addresses, not data."""
        mapped = [s for s in elf.sections if s["addr"]]
        lo = next((s["addr"] for s in mapped if s["name"] == ".plt"),
                  min(s["addr"] for s in mapped))
        return lo, max(s["addr"] + s["size"] for s in mapped)

    orig_res = Resolver(orig, common, of.plt_map(), addr_range(of))
    ours_res = Resolver(ours, common, ours_elf.plt_map(), addr_range(ours_elf))

    if args.show:
        name = args.show
        if name not in orig or name not in ours:
            print(f"{name}: present orig={name in orig} ours={name in ours}"); return 2
        o, u = orig[name], ours[name]
        oraw, onorm = disassemble(args.objdump, of.bytes_at(o["value"], o["size"]),
                                  o["thumb"], o["value"], orig_res)
        uraw, unorm = disassemble(args.objdump, ours_elf.bytes_at(u["value"], u["size"]),
                                  u["thumb"], u["value"], ours_res)
        print(demangle_map([name])[name])
        print(f"  orig {o['size']}B @{o['value']:#x}  |  ours {u['size']}B @{u['value']:#x}"
              f"   ('|' structural diff, '~' link-address diff only)")
        for i in range(max(len(oraw), len(uraw))):
            ol = oraw[i] if i < len(oraw) else ""
            ul = uraw[i] if i < len(uraw) else ""
            on = onorm[i] if i < len(onorm) else None
            un = unorm[i] if i < len(unorm) else None
            mark = " " if on == un and ol == ul else ("~" if on == un else "|")
            print(f"  {ol:38} {mark} {ul}")
        if len(oraw) != len(uraw):
            print(f"  (length differs: orig {len(oraw)} vs ours {len(uraw)} insns)")
        return 0

    missing = set(orig) - set(ours)
    extra = set(ours) - set(orig)

    dm = demangle_map(missing | extra)
    extra_q = {qualified(dm[n]) for n in extra}
    wrong_type = {n for n in missing if qualified(dm[n]) in extra_q}
    absent = missing - wrong_type

    rows, byte_exact, linked_exact, fuzzy_perfect = [], 0, 0, 0
    stub_zero, orig_zero = [], []
    for name in sorted(common):
        o, u = orig[name], ours[name]
        if o["size"] > 0 and u["size"] == 0:
            stub_zero.append(name)          # UB-collapsed body on our side: broken, not scored
            continue
        if o["size"] == 0 and u["size"] > 0:
            orig_zero.append(name)          # original exports it size-0: nothing to compare
            continue
        ob = of.bytes_at(o["value"], o["size"])
        ub = ours_elf.bytes_at(u["value"], u["size"])
        be = ob == ub
        _, on = disassemble(args.objdump, ob, o["thumb"], o["value"], orig_res)
        _, un = disassemble(args.objdump, ub, u["thumb"], u["value"], ours_res)
        le = on == un and o["size"] == u["size"]
        pct = 100.0 if be else score(on, un)
        byte_exact += be
        linked_exact += le
        fuzzy_perfect += pct == 100.0
        rows.append(dict(name=name, pct=pct, byte=be, linked=le,
                         osize=o["size"], usize=u["size"]))

    rows.sort(key=lambda r: (r["pct"], r["name"]))
    avg = round(sum(r["pct"] for r in rows) / len(rows), 2) if rows else 0.0
    summary = dict(original_functions=len(orig), ours_functions=len(ours),
                   compared=len(rows), avg_match=avg, byte_exact=byte_exact,
                   linked_exact=linked_exact, fuzzy_perfect=fuzzy_perfect,
                   stub_zero_size=len(stub_zero), orig_zero_size=len(orig_zero),
                   missing=len(missing), wrong_type=len(wrong_type), absent=len(absent),
                   extra=len(extra))

    bd = Path(args.build_dir); bd.mkdir(parents=True, exist_ok=True)
    (bd / "report.json").write_text(json.dumps(
        dict(summary=summary, rows=rows,
             stub_zero_size=sorted(stub_zero), orig_zero_size=sorted(orig_zero),
             missing=sorted(missing), extra=sorted(extra),
             wrong_type=sorted(wrong_type)), indent=1))

    print(f"compared {len(rows)}  avg {avg}%  byte_exact {byte_exact}  "
          f"linked_exact {linked_exact}  fuzzy_perfect {fuzzy_perfect}")
    print(f"stub_zero_size {len(stub_zero)}  orig_zero_size {len(orig_zero)}  "
          f"missing {len(missing)} (wrong_type {len(wrong_type)}  absent {len(absent)})  "
          f"extra {len(extra)}")
    if stub_zero:
        sdm = demangle_map(stub_zero)
        print("\nUB-collapsed stubs (defined here, compiled to 0 bytes -- broken bodies):")
        for n in sorted(stub_zero):
            print(f"  {sdm.get(n, n)}")
    print(f"\nworst {min(args.top, len(rows))} matched functions:")
    for r in rows[:args.top]:
        flag = "B" if r["byte"] else ("L" if r["linked"] else " ")
        print(f"  {r['pct']:6.1f}% {flag} {dm.get(r['name'], r['name'])[:90]}")

    ok = not missing and not (args.fail_on_wrong_type and wrong_type)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
