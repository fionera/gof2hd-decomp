#!/usr/bin/env python3
"""GOF2HD validation CLI: status, diff, check, layout, and lint.

Stdlib-only Python 3. Zero hardcoded machine paths.
"""

from __future__ import annotations

import argparse
import difflib
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any, Dict, List, Optional, Sequence, Tuple

SCRIPT_DIR = Path(__file__).resolve().parent
REPO_ROOT = SCRIPT_DIR.parent
if str(SCRIPT_DIR) not in sys.path:
    sys.path.insert(0, str(SCRIPT_DIR))

from elf32 import Elf32, Symbol  # noqa: E402

DEFAULT_BASELINE = SCRIPT_DIR / "baseline.json"


# ---------------------------------------------------------------------------
# Environment & toolchain discovery (zero hardcoded user paths)
# ---------------------------------------------------------------------------

def parse_cmake_cache(build_dir: Path) -> Dict[str, str]:
    cache = build_dir / "CMakeCache.txt"
    result: Dict[str, str] = {}
    if not cache.is_file():
        return result
    for line in cache.read_text(errors="replace").splitlines():
        if not line or line.startswith(("#", "//")) or "=" not in line:
            continue
        key_type, val = line.split("=", 1)
        key = key_type.split(":", 1)[0].strip()
        result[key] = val.strip()
    return result


def resolve_build_dir(explicit: Optional[str], repo_root: Path = REPO_ROOT) -> Path:
    if explicit:
        return Path(explicit).expanduser().resolve()
    env_dir = os.environ.get("GOF2_BUILD_DIR")
    if env_dir:
        return Path(env_dir).expanduser().resolve()
    for name in ("cmake-build-match", "build"):
        cand = repo_root / name
        if cand.is_dir():
            return cand
    return repo_root / "cmake-build-match"


def _find_in_ndk_cache(names: Sequence[str], repo_root: Path = REPO_ROOT) -> Optional[str]:
    ndk_root = repo_root / ".cache" / "ndk"
    if not ndk_root.is_dir():
        return None
    for name in names:
        for hit in ndk_root.rglob(name):
            if hit.is_file() and os.access(hit, os.X_OK):
                return str(hit)
    return None


def resolve_paths(args: argparse.Namespace, repo_root: Path = REPO_ROOT) -> Dict[str, Any]:
    build_dir = resolve_build_dir(getattr(args, "build_dir", None), repo_root)
    cache = parse_cmake_cache(build_dir)

    original = (
        getattr(args, "original", None)
        or os.environ.get("GOF2_ORIGINAL_SO")
        or cache.get("GOF2_ORIGINAL_SO")
    )
    if not original:
        default_orig = repo_root.parent / "_work" / "bins" / "android_2.0.16_libgof2hdaa.so"
        if default_orig.is_file():
            original = str(default_orig)

    ours = getattr(args, "ours", None) or str(build_dir / "libgof2hdaa.so")

    objdump = (
        getattr(args, "objdump", None)
        or os.environ.get("GOF2_NDK_OBJDUMP")
        or cache.get("GOF2_NDK_OBJDUMP")
        or _find_in_ndk_cache(("arm-linux-androideabi-objdump", "llvm-objdump"), repo_root)
        or shutil.which("arm-linux-androideabi-objdump")
        or shutil.which("llvm-objdump")
    )

    fmod_libdir = (
        os.environ.get("GOF2_FMOD_LIBDIR")
        or cache.get("GOF2_FMOD_LIBDIR")
    )

    return {
        "repo_root": repo_root,
        "build_dir": build_dir,
        "cache": cache,
        "original": Path(original).expanduser() if original else None,
        "ours": Path(ours).expanduser(),
        "objdump": objdump,
        "fmod_libdir": fmod_libdir,
    }


def demangle_batch(names: Sequence[str]) -> Dict[str, str]:
    if not names:
        return {}
    tool = shutil.which("llvm-cxxfilt") or shutil.which("c++filt")
    if not tool:
        return {n: n for n in names}
    try:
        proc = subprocess.run(
            [tool],
            input="\n".join(names) + "\n",
            capture_output=True,
            text=True,
            timeout=10,
        )
        if proc.returncode == 0:
            lines = proc.stdout.splitlines()
            if len(lines) == len(names):
                return dict(zip(names, lines))
    except Exception:
        pass
    return {n: n for n in names}


# ---------------------------------------------------------------------------
# Disassembly & instruction normalization (only for non-byte-exact functions)
# ---------------------------------------------------------------------------

_OBJDUMP_INSN = re.compile(
    r"^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]{4}(?:\s+[0-9a-fA-F]{4})?|[0-9a-fA-F]{8})\s+(.*)$"
)
_BRANCH_TARGET = re.compile(r"\b([0-9a-fA-F]{4,8})\s+<([^>+]+)(\+0x[0-9a-fA-F]+)?>")
_PC_COMMENT = re.compile(r";\s*(?:0x)?[0-9a-fA-F]+.*$")
_HEX_ADDR = re.compile(r"\b0x[0-9a-fA-F]{4,8}\b")


def disassemble_symbol(
    so_path: Path, sym: Symbol, objdump: str
) -> List[Tuple[int, str, str]]:
    """Disassemble a single symbol range. Returns [(rel_off, raw_hex, insn_text)]."""
    if sym.size == 0:
        return []
    start = sym.addr
    stop = sym.addr + sym.size
    cmd = [
        objdump,
        "-d",
        "--no-show-raw-insn" if False else "-z",
        f"--start-address=0x{start:x}",
        f"--stop-address=0x{stop:x}",
        str(so_path),
    ]
    # Remove the no-op "-z" for llvm-objdump compatibility
    if "llvm-objdump" in Path(objdump).name:
        cmd = [
            objdump,
            "-d",
            f"--start-address=0x{start:x}",
            f"--stop-address=0x{stop:x}",
            str(so_path),
        ]
    try:
        proc = subprocess.run(cmd, capture_output=True, text=True, timeout=15)
    except Exception:
        return []
    if proc.returncode != 0:
        return []
    insns: List[Tuple[int, str, str]] = []
    for line in proc.stdout.splitlines():
        m = _OBJDUMP_INSN.match(line)
        if not m:
            continue
        addr = int(m.group(1), 16)
        if addr < start or addr >= stop:
            continue
        raw_hex = m.group(2).strip()
        text = " ".join(m.group(3).strip().split())
        insns.append((addr - start, raw_hex, text))
    return insns


def normalize_insn(insn: str, sym_start: int, sym_size: int) -> str:
    """Normalize address-dependent operands for linked-exact comparison."""
    s = _PC_COMMENT.sub("", insn).strip()

    def _repl_target(m: re.Match[str]) -> str:
        addr = int(m.group(1), 16)
        target_name = m.group(2)
        if sym_start <= addr < sym_start + sym_size:
            return f"+0x{addr - sym_start:x}"
        return f"<{target_name}>"

    s = _BRANCH_TARGET.sub(_repl_target, s)
    s = _HEX_ADDR.sub("#ADDR", s)
    return s


def compute_disasm_match(
    orig_path: Path,
    ours_path: Path,
    orig_sym: Symbol,
    ours_sym: Symbol,
    objdump: Optional[str],
) -> Tuple[bool, Optional[float]]:
    """Return (linked_exact, fuzzy_pct) for a non-byte-exact function."""
    if not objdump or orig_sym.size == 0 or ours_sym.size == 0:
        return False, None
    o_insns = disassemble_symbol(orig_path, orig_sym, objdump)
    u_insns = disassemble_symbol(ours_path, ours_sym, objdump)
    if not o_insns or not u_insns:
        return False, None
    o_norm = [normalize_insn(t, orig_sym.addr, orig_sym.size) for _, _, t in o_insns]
    u_norm = [normalize_insn(t, ours_sym.addr, ours_sym.size) for _, _, t in u_insns]
    if o_norm == u_norm:
        return True, 100.0
    ratio = difflib.SequenceMatcher(None, o_norm, u_norm, autojunk=False).ratio()
    return False, round(ratio * 100.0, 2)


# ---------------------------------------------------------------------------
# Core ELF comparison (byte-first, zero objdump for byte-exact functions)
# ---------------------------------------------------------------------------

def count_diff_bytes(a: bytes, b: bytes) -> int:
    common = min(len(a), len(b))
    diffs = sum(1 for i in range(common) if a[i] != b[i])
    return diffs + abs(len(a) - len(b))


def compare_binaries(
    orig_path: Path,
    ours_path: Path,
    objdump: Optional[str] = None,
    use_disasm: bool = True,
) -> Dict[str, Any]:
    orig = Elf32(orig_path)
    ours = Elf32(ours_path)

    # 1. Functions (STT_FUNC)
    orig_fn_names = set(orig.functions)
    ours_fn_names = set(ours.functions)

    common_fns = sorted(orig_fn_names & ours_fn_names)
    missing_fns = sorted(
        n for n in (orig_fn_names - ours_fn_names) if n not in ours.defined
    )
    wrong_type_fns = sorted(
        n for n in (orig_fn_names - ours_fn_names) if n in ours.defined
    )
    extra_defined = sorted(set(ours.defined) - set(orig.defined))
    extra_fns = sorted(
        n for n in (ours_fn_names - orig_fn_names) if n not in orig.defined
    )

    byte_exact = 0
    linked_exact = 0
    stub_zero_size = 0
    non_exact: List[Dict[str, Any]] = []
    match_pct_sum = 0.0

    for name in common_fns:
        osym = orig.functions[name]
        usym = ours.functions[name]
        obytes = orig.read_symbol_bytes(osym)
        ubytes = ours.read_symbol_bytes(usym)

        if usym.size == 0 and osym.size > 0:
            stub_zero_size += 1

        # FAST PATH: raw byte comparison in pure Python first!
        if osym.size == usym.size and obytes == ubytes:
            byte_exact += 1
            linked_exact += 1
            match_pct_sum += 100.0
            continue

        diff_b = count_diff_bytes(obytes, ubytes)
        max_len = max(len(obytes), len(ubytes), 1)
        raw_pct = round(100.0 * (max_len - diff_b) / max_len, 2)

        is_linked = False
        fuzzy_pct: Optional[float] = None
        if use_disasm and objdump:
            is_linked, fuzzy_pct = compute_disasm_match(
                orig_path, ours_path, osym, usym, objdump
            )
            if is_linked:
                linked_exact += 1

        effective_pct = fuzzy_pct if fuzzy_pct is not None else raw_pct
        match_pct_sum += effective_pct

        non_exact.append(
            {
                "name": name,
                "orig_addr": osym.addr,
                "ours_addr": usym.addr,
                "orig_size": osym.size,
                "ours_size": usym.size,
                "size_delta": usym.size - osym.size,
                "diff_bytes": diff_b,
                "raw_pct": raw_pct,
                "fuzzy_pct": fuzzy_pct,
                "linked_exact": is_linked,
                "is_thumb": osym.is_thumb,
            }
        )

    non_exact.sort(key=lambda r: (-r["diff_bytes"], -r["orig_size"], r["name"]))
    demangled = demangle_batch([r["name"] for r in non_exact])
    for r in non_exact:
        r["demangled"] = demangled.get(r["name"], r["name"])

    compared = len(common_fns)
    avg_match = round(match_pct_sum / compared, 2) if compared else 0.0

    # 2. Data objects (STT_OBJECT)
    common_objs = sorted(set(orig.objects) & set(ours.objects))
    data_exact = 0
    data_diffs: List[Dict[str, Any]] = []
    for name in common_objs:
        osym = orig.objects[name]
        usym = ours.objects[name]
        obytes = orig.read_symbol_bytes(osym)
        ubytes = ours.read_symbol_bytes(usym)
        if osym.size == usym.size and obytes == ubytes:
            data_exact += 1
        else:
            data_diffs.append(
                {
                    "name": name,
                    "orig_size": osym.size,
                    "ours_size": usym.size,
                    "diff_bytes": count_diff_bytes(obytes, ubytes),
                }
            )

    # 3. Undefined imports (SHN_UNDEF)
    orig_imports = sorted(orig.imports)
    ours_imports = sorted(ours.imports)
    added_imports = sorted(ours.imports - orig.imports)
    removed_imports = sorted(orig.imports - ours.imports)

    # 4. ELF sections
    sec_names: List[str] = []
    seen_secs: set[str] = set()
    for s in orig.sections + ours.sections:
        if s.name and s.name not in seen_secs:
            seen_secs.add(s.name)
            sec_names.append(s.name)

    sections_report: List[Dict[str, Any]] = []
    for sname in sec_names:
        osec = orig.sections_by_name.get(sname)
        usec = ours.sections_by_name.get(sname)
        if osec is None:
            sections_report.append(
                {
                    "name": sname,
                    "status": "extra",
                    "orig_size": 0,
                    "ours_size": usec.size if usec else 0,
                    "size_delta": usec.size if usec else 0,
                    "diff_bytes": usec.size if (usec and not usec.is_nobits) else 0,
                    "exact": False,
                }
            )
            continue
        if usec is None:
            sections_report.append(
                {
                    "name": sname,
                    "status": "missing",
                    "orig_size": osec.size,
                    "ours_size": 0,
                    "size_delta": -osec.size,
                    "diff_bytes": osec.size if not osec.is_nobits else 0,
                    "exact": False,
                }
            )
            continue

        if osec.is_nobits and usec.is_nobits:
            exact = osec.size == usec.size and osec.addr == usec.addr
            diff_b = 0
        else:
            exact = osec.size == usec.size and osec.data == usec.data
            diff_b = 0 if exact else count_diff_bytes(osec.data, usec.data)

        sections_report.append(
            {
                "name": sname,
                "status": "exact" if exact else "diff",
                "orig_addr": osec.addr,
                "ours_addr": usec.addr,
                "orig_size": osec.size,
                "ours_size": usec.size,
                "size_delta": usec.size - osec.size,
                "diff_bytes": diff_b,
                "orig_sha256": osec.sha256,
                "ours_sha256": usec.sha256,
                "exact": exact,
            }
        )

    return {
        "original": str(orig_path),
        "ours": str(ours_path),
        "functions": {
            "compared": compared,
            "byte_exact": byte_exact,
            "linked_exact": linked_exact,
            "non_exact_count": len(non_exact),
            "stub_zero_size": stub_zero_size,
            "missing": len(missing_fns),
            "wrong_type": len(wrong_type_fns),
            "extra": len(extra_fns),
            "avg_match": avg_match,
            "missing_names": missing_fns,
            "wrong_type_names": wrong_type_fns,
            "extra_names": extra_fns,
            "extra_defined_names": extra_defined,
        },
        "data": {
            "exact": data_exact,
            "common": len(common_objs),
            "diffs": data_diffs,
        },
        "imports": {
            "orig_count": len(orig_imports),
            "ours_count": len(ours_imports),
            "added": added_imports,
            "removed": removed_imports,
            "ours": ours_imports,
        },
        "sections": sections_report,
        "non_exact": non_exact,
    }


# ---------------------------------------------------------------------------
# Subcommand: status
# ---------------------------------------------------------------------------

def print_status(report: Dict[str, Any], limit: Optional[int] = None) -> None:
    fn = report["functions"]
    data = report["data"]
    imp = report["imports"]
    secs = report["sections"]

    print("=== GOF2HD Validation Status ===")
    print(
        f"Functions : {fn['byte_exact']}/{fn['compared']} byte-exact | "
        f"{fn['linked_exact']}/{fn['compared']} linked-exact | "
        f"avg {fn['avg_match']:.2f}% | "
        f"stubs={fn['stub_zero_size']} missing={fn['missing']} "
        f"wrong_type={fn['wrong_type']} extra={fn['extra']}"
    )
    print(f"Data      : {data['exact']}/{data['common']} exact")
    print(
        f"Imports   : {imp['ours_count']} (orig {imp['orig_count']}, "
        f"+{len(imp['added'])} / -{len(imp['removed'])})"
    )

    diff_secs = [s for s in secs if not s["exact"]]
    print(f"Sections  : {len(secs) - len(diff_secs)}/{len(secs)} exact")
    if diff_secs:
        for s in diff_secs:
            delta_str = f"{s['size_delta']:+d}B" if s["size_delta"] else "size-exact"
            print(
                f"  section {s['name']:<20} {s['status']:<7} "
                f"orig=0x{s['orig_size']:x} ours=0x{s['ours_size']:x} "
                f"({delta_str}, diff={s['diff_bytes']}B)"
            )

    rows = report["non_exact"]
    if not rows:
        print("\nAll compared functions are byte-exact.")
        return

    shown = rows if limit is None else rows[:limit]
    print(f"\nRemaining non-exact functions ({len(rows)} total):")
    print(f"  {'DIFF_B':>7}  {'SIZE':>7}  {'DELTA':>6}  {'LNK':>3}  {'RAW%':>6}  SYMBOL")
    for r in shown:
        lnk = "yes" if r["linked_exact"] else "no"
        label = r["demangled"] if r["demangled"] != r["name"] else r["name"]
        if r["demangled"] != r["name"]:
            label = f"{r['demangled']}  [{r['name']}]"
        print(
            f"  {r['diff_bytes']:>7}  {r['orig_size']:>7}  {r['size_delta']:>+6}  "
            f"{lnk:>3}  {r['raw_pct']:>5.1f}%  {label}"
        )


def cmd_status(args: argparse.Namespace) -> int:
    paths = resolve_paths(args)
    if not paths["original"] or not paths["original"].is_file():
        print(
            f"error: original .so not found (set --original or GOF2_ORIGINAL_SO): {paths['original']}",
            file=sys.stderr,
        )
        return 2
    if not paths["ours"].is_file():
        print(
            f"error: built .so not found (build link_so or pass --ours): {paths['ours']}",
            file=sys.stderr,
        )
        return 2

    report = compare_binaries(
        paths["original"],
        paths["ours"],
        objdump=paths["objdump"],
        use_disasm=not args.no_disasm,
    )
    print_status(report, limit=args.limit)

    if not args.no_json:
        out_path = Path(args.json) if args.json else (paths["build_dir"] / "report.json")
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(json.dumps(report, indent=2) + "\n")
        print(f"\nReport written to: {out_path}")
    return 0


# ---------------------------------------------------------------------------
# Subcommand: diff
# ---------------------------------------------------------------------------

def byte_hunks(a: bytes, b: bytes, max_hunks: int = 24) -> List[Tuple[int, bytes, bytes]]:
    hunks: List[Tuple[int, bytes, bytes]] = []
    n = max(len(a), len(b))
    i = 0
    while i < n:
        ba = a[i] if i < len(a) else None
        bb = b[i] if i < len(b) else None
        if ba != bb:
            start = i
            while i < n:
                ca = a[i] if i < len(a) else None
                cb = b[i] if i < len(b) else None
                if ca == cb:
                    break
                i += 1
            hunks.append((start, a[start:i], b[start:i]))
            if len(hunks) >= max_hunks:
                break
        else:
            i += 1
    return hunks


def format_hex_words(data: bytes, thumb: bool) -> List[str]:
    step = 2 if thumb else 4
    lines: List[str] = []
    for off in range(0, len(data), step):
        chunk = data[off : off + step]
        lines.append(f"+0x{off:04x}: {chunk.hex()}")
    return lines


def print_side_by_side(
    orig_lines: Sequence[str],
    ours_lines: Sequence[str],
    orig_norm: Optional[Sequence[str]] = None,
    ours_norm: Optional[Sequence[str]] = None,
) -> None:
    o_key = list(orig_norm) if orig_norm is not None else list(orig_lines)
    u_key = list(ours_norm) if ours_norm is not None else list(ours_lines)
    sm = difflib.SequenceMatcher(None, o_key, u_key, autojunk=False)
    col = 48
    print(f"  {'ORIGINAL':<{col}} | {'OURS'}")
    print(f"  {'-' * col}-+-{'-' * col}")
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            for oi, ui in zip(range(i1, i2), range(j1, j2)):
                mark = " " if orig_lines[oi] == ours_lines[ui] else "~"
                print(f"{mark} {orig_lines[oi]:<{col}} | {ours_lines[ui]}")
        else:
            span = max(i2 - i1, j2 - j1)
            for k in range(span):
                ol = orig_lines[i1 + k] if i1 + k < i2 else ""
                ul = ours_lines[j1 + k] if j1 + k < j2 else ""
                print(f"| {ol:<{col}} | {ul}")


def cmd_diff(args: argparse.Namespace) -> int:
    paths = resolve_paths(args)
    if not paths["original"] or not paths["original"].is_file():
        print(f"error: original .so not found: {paths['original']}", file=sys.stderr)
        return 2
    if not paths["ours"].is_file():
        print(f"error: built .so not found: {paths['ours']}", file=sys.stderr)
        return 2

    orig = Elf32(paths["original"])
    ours = Elf32(paths["ours"])
    all_names = sorted(set(orig.functions) | set(ours.functions))
    demangled_cache: Optional[Dict[str, str]] = None

    matched: List[str] = []
    for query in args.symbols:
        if query in orig.functions or query in ours.functions:
            matched.append(query)
            continue
        hits = [n for n in all_names if query in n]
        if not hits:
            if demangled_cache is None:
                demangled_cache = demangle_batch(all_names)
            hits = [n for n, d in demangled_cache.items() if query in d]
        if not hits:
            print(f"error: no symbol matching '{query}'", file=sys.stderr)
            return 1
        matched.extend(hits[: args.max_matches])

    # Deduplicate preserving order
    seen: set[str] = set()
    targets = [n for n in matched if not (n in seen or seen.add(n))]
    dem = demangle_batch(targets)

    for sym_name in targets:
        osym = orig.functions.get(sym_name)
        usym = ours.functions.get(sym_name)
        print(f"\n=== {dem.get(sym_name, sym_name)} ===")
        print(f"mangled : {sym_name}")
        if osym is None:
            print(f"status  : EXTRA in ours (addr=0x{usym.addr:x}, size={usym.size})")
            continue
        if usym is None:
            print(f"status  : MISSING in ours (orig addr=0x{osym.addr:x}, size={osym.size})")
            continue

        obytes = orig.read_symbol_bytes(osym)
        ubytes = ours.read_symbol_bytes(usym)
        diff_b = count_diff_bytes(obytes, ubytes)
        print(
            f"orig    : 0x{osym.addr:x} ({osym.size} B, {'Thumb' if osym.is_thumb else 'ARM'})"
        )
        print(
            f"ours    : 0x{usym.addr:x} ({usym.size} B, {'Thumb' if usym.is_thumb else 'ARM'})"
        )
        print(f"diff    : {diff_b} byte(s) differing (size delta {usym.size - osym.size:+d} B)")

        if diff_b == 0:
            print("result  : BYTE-EXACT")
            continue

        hunks = byte_hunks(obytes, ubytes)
        print(f"hunks   : showing {len(hunks)} differing byte range(s):")
        for off, oh, uh in hunks:
            print(f"  +0x{off:04x}: orig={oh.hex() or '<eof>'}  ours={uh.hex() or '<eof>'}")

        if paths["objdump"] and not args.hex_only:
            o_insns = disassemble_symbol(paths["original"], osym, paths["objdump"])
            u_insns = disassemble_symbol(paths["ours"], usym, paths["objdump"])
            if o_insns and u_insns:
                o_lines = [f"+0x{off:04x} {txt}" for off, _, txt in o_insns]
                u_lines = [f"+0x{off:04x} {txt}" for off, _, txt in u_insns]
                o_norm = [normalize_insn(t, osym.addr, osym.size) for _, _, t in o_insns]
                u_norm = [normalize_insn(t, usym.addr, usym.size) for _, _, t in u_insns]
                print()
                print_side_by_side(o_lines, u_lines, o_norm, u_norm)
                continue

        print("\n(objdump unavailable or --hex-only; showing word diff)")
        print_side_by_side(
            format_hex_words(obytes, osym.is_thumb),
            format_hex_words(ubytes, usym.is_thumb),
        )
    return 0


# ---------------------------------------------------------------------------
# Subcommand: lint (source authenticity + technical-debt audit)
# ---------------------------------------------------------------------------

_WAIVER_RE = re.compile(r"//\s*lint:\s*(.+)")
_WAIVER_WORD = re.compile(r"[a-z_]+")

_INLINE_ASM_RE = re.compile(
    r"(?<!\w)__asm(?:__)?\b|\basm\b\s*(?:(?:volatile|inline|goto|const)\b\s*)*\("
)
_COMPUTED_GOTO_RE = re.compile(
    r"\bgoto\s*\*|(?:[=,{(\[]\s*|\breturn\s+)&&\s*[A-Za-z_]\w*\s*(?=[,;\])}\n])"
)
_ATTR_HACK_RE = re.compile(r"\b__attribute__\b|\b__declspec\b")
_UNION_RE = re.compile(r"\bunion\b")
_EXTERN_C_RE = re.compile(r'extern\s*"\s*"')
_EXPLICIT_INST_RE = re.compile(r"^\s*template\s+(?:class|struct)\s+\w", re.M)
_VOID_PTR_ARITH_RE = re.compile(
    r"\(\s*(?:const\s+)?(?:char|unsigned\s+char|signed\s+char|uint8_t|int8_t)\s*\*\s*\)\s*"
    r"\(\s*(?:const\s+)?void\s*\*\s*\)"
    r"|\(\s*(?:const\s+)?void\s*\*\s*\)\s*[A-Za-z_]\w*\s*[+-]\s*(?:0x[0-9a-fA-F]+|\d+)"
)
_VOID_PTR_RE = re.compile(r"\bvoid\s*\*|\btypedef\s+void\s+\w+\s*;|\busing\s+\w+\s*=\s*void\s*;")
_BUILTIN_RE = re.compile(r"\b__aeabi_\w+|\b__builtin_(?!offsetof\b)\w+")
_OP_CALL_RE = re.compile(r"(?:::\s*)?operator\s+(?:new|delete)\b|::\s*(?:new|delete)\b")


def strip_cpp_noise(text: str) -> Tuple[str, Dict[int, set[str]]]:
    """Strip comments and string/char literal bodies while preserving line numbers."""
    waivers: Dict[int, set[str]] = {}
    for idx, line in enumerate(text.splitlines(), 1):
        m = _WAIVER_RE.search(line)
        if m:
            waivers[idx] = set(_WAIVER_WORD.findall(m.group(1)))

    out: List[str] = []
    i = 0
    n = len(text)
    state: Optional[str] = None
    while i < n:
        c = text[i]
        nxt = text[i + 1] if i + 1 < n else ""
        if state is None:
            if c == "/" and nxt == "/":
                state = "line"
                out.append("  ")
                i += 2
            elif c == "/" and nxt == "*":
                state = "block"
                out.append("  ")
                i += 2
            elif c == '"':
                state = "str"
                out.append('"')
                i += 1
            elif c == "'":
                state = "char"
                out.append("'")
                i += 1
            else:
                out.append(c)
                i += 1
        elif state == "line":
            if c == "\n":
                state = None
                out.append("\n")
            else:
                out.append("\t" if c == "\t" else " ")
            i += 1
        elif state == "block":
            if c == "*" and nxt == "/":
                state = None
                out.append("  ")
                i += 2
            else:
                out.append("\n" if c == "\n" else " ")
                i += 1
        else:
            q = '"' if state == "str" else "'"
            if c == "\\":
                out.append("  ")
                i += 2
            elif c == q:
                state = None
                out.append(q)
                i += 1
            else:
                out.append("\n" if c == "\n" else " ")
                i += 1
    return "".join(out), waivers


def _line_of(text: str, pos: int) -> int:
    return text.count("\n", 0, pos) + 1


def run_lint(
    src_root: Path, cmake_path: Optional[Path] = None
) -> Tuple[List[Dict[str, Any]], Dict[str, List[Dict[str, Any]]]]:
    """Run source-authenticity lint. Returns (hard_errors, audit_warnings)."""
    hard_errors: List[Dict[str, Any]] = []
    audit: Dict[str, List[Dict[str, Any]]] = {
        "cmake_per_file_flags": [],
        "builtin_expect": [],
        "void_ptr": [],
        "operator_call": [],
        "lint_waiver": [],
    }

    exts = {".c", ".cpp", ".h", ".hpp"}
    files = sorted(p for p in src_root.rglob("*") if p.is_file() and p.suffix in exts)

    for path in files:
        rel = path.relative_to(src_root).as_posix()
        raw = path.read_text(errors="replace")
        raw_lines = raw.splitlines()
        clean, waivers = strip_cpp_noise(raw)

        for lineno in sorted(waivers):
            audit["lint_waiver"].append(
                {"file": rel, "line": lineno, "text": raw_lines[lineno - 1].strip()}
            )

        def record_hard(rule: str, pos: int, waivable: bool = False) -> None:
            ln = _line_of(clean, pos)
            if waivable and rule in waivers.get(ln, ()):
                return
            hard_errors.append(
                {
                    "rule": rule,
                    "file": rel,
                    "line": ln,
                    "text": raw_lines[ln - 1].strip(),
                }
            )

        for m in _INLINE_ASM_RE.finditer(clean):
            record_hard("inline_asm", m.start(), waivable=False)
        for m in _COMPUTED_GOTO_RE.finditer(clean):
            record_hard("computed_goto", m.start(), waivable=False)
        for m in _ATTR_HACK_RE.finditer(clean):
            record_hard("attribute_hack", m.start(), waivable=False)
        for m in _UNION_RE.finditer(clean):
            record_hard("union_decl", m.start(), waivable=False)
        for m in _VOID_PTR_ARITH_RE.finditer(clean):
            record_hard("void_ptr_arith", m.start(), waivable=False)
        for m in _EXTERN_C_RE.finditer(clean):
            record_hard("extern_c", m.start(), waivable=True)
        for m in _EXPLICIT_INST_RE.finditer(clean):
            record_hard("explicit_inst", m.start(), waivable=True)

        # Audit / technical-debt warnings
        for m in _BUILTIN_RE.finditer(clean):
            ln = _line_of(clean, m.start())
            audit["builtin_expect"].append(
                {"file": rel, "line": ln, "text": raw_lines[ln - 1].strip()}
            )
        for m in _VOID_PTR_RE.finditer(clean):
            ln = _line_of(clean, m.start())
            if "void_ptr" not in waivers.get(ln, ()):
                audit["void_ptr"].append(
                    {"file": rel, "line": ln, "text": raw_lines[ln - 1].strip()}
                )
        for m in _OP_CALL_RE.finditer(clean):
            ln = _line_of(clean, m.start())
            if "operator_call" not in waivers.get(ln, ()):
                audit["operator_call"].append(
                    {"file": rel, "line": ln, "text": raw_lines[ln - 1].strip()}
                )

    if cmake_path and cmake_path.is_file():
        for idx, line in enumerate(cmake_path.read_text(errors="replace").splitlines(), 1):
            stripped = line.strip()
            if stripped.startswith("#"):
                continue
            if "-mllvm" in stripped or "-target-feature" in stripped:
                audit["cmake_per_file_flags"].append(
                    {"file": cmake_path.name, "line": idx, "text": stripped}
                )

    return hard_errors, audit


def cmd_lint(args: argparse.Namespace) -> int:
    src_root = Path(args.src).resolve()
    cmake_path = Path(args.cmake).resolve() if args.cmake else (src_root.parent / "CMakeLists.txt")
    if not src_root.is_dir():
        print(f"error: source directory not found: {src_root}", file=sys.stderr)
        return 2

    hard_errors, audit = run_lint(src_root, cmake_path)

    if hard_errors:
        print(f"LINT FAILED: {len(hard_errors)} hard violation(s):")
        for e in hard_errors:
            print(f"  [{e['rule']}] {e['file']}:{e['line']}: {e['text']}")
    else:
        print("LINT: 0 hard violations (CLEAN)")

    total_audit = sum(len(v) for v in audit.values())
    if total_audit:
        print("Audit summary (technical debt, non-blocking unless --strict):")
        for key, items in audit.items():
            if items:
                print(f"  {key:<22}: {len(items)}")
                if args.verbose:
                    for it in items[:25]:
                        print(f"    {it['file']}:{it['line']}: {it['text']}")

    if hard_errors or (args.strict and total_audit > 0):
        return 1
    return 0


# ---------------------------------------------------------------------------
# Subcommand: layout (NDK clang++ -fdump-record-layouts field_0xNN checker)
# ---------------------------------------------------------------------------

_HEXNAME_RE = re.compile(r"_0x([0-9a-fA-F]+)")
_FIELD_RE = re.compile(r"^\s*(\d+) \|(\s+)([^\n]*?)\b([A-Za-z_]\w*)\s*$")
_HEAD_RE = re.compile(r"^\s*0 \| (?:class|struct|union) ([A-Za-z_][\w:]*)")
_CLASS_OPEN_RE = re.compile(r"\b(?:class|struct|union)\s+([A-Za-z_]\w*)\s*(?:final\s*)?(?::[^{]*)?\{")
_NS_OPEN_RE = re.compile(r"\bnamespace\s+([A-Za-z_]\w*)\s*\{")
_TEMPLATE_RE = re.compile(r"\btemplate\s*<")


def resolve_ndk_clangxx(
    build_dir: Path,
    explicit: Optional[str] = None,
    repo_root: Path = REPO_ROOT,
) -> Optional[Tuple[str, List[str]]]:
    """Resolve NDK ARM clang++ and its target/include flags. Never uses host clang++."""
    db = build_dir / "compile_commands.json"
    if not explicit and db.is_file():
        try:
            entries = json.loads(db.read_text())
        except Exception:
            entries = []
        for ent in entries:
            cmd_args = ent.get("arguments") or shlex.split(ent.get("command", ""))
            if not cmd_args or "clang++" not in Path(cmd_args[0]).name:
                continue
            cxx = cmd_args[0]
            if not Path(cxx).is_file():
                continue
            flags: List[str] = []
            it = iter(cmd_args[1:])
            for a in it:
                if a == "-o":
                    next(it, None)
                elif a == "-c" or a == ent.get("file") or a.endswith((".cpp", ".cc", ".cxx")):
                    continue
                else:
                    flags.append(a)
            return cxx, flags

    cache = parse_cmake_cache(build_dir)
    cxx = explicit or os.environ.get("GOF2_NDK_CLANGXX") or cache.get("GOF2_NDK_CLANGXX")
    if not cxx:
        cxx = _find_in_ndk_cache(("clang++",), repo_root)
    if not cxx or not Path(cxx).is_file():
        return None

    raw_flags = cache.get("CMAKE_CXX_FLAGS", "")
    flags = shlex.split(raw_flags) if raw_flags else [
        "-target", "armv7-none-linux-androideabi16",
        "-march=armv7-a", "-mthumb", "-mfpu=neon", "-mfloat-abi=softfp",
        "-std=c++11", "-I", str(repo_root / "src"),
    ]
    return cxx, flags


def _probe_classes(path: Path) -> List[str]:
    text = path.read_text(errors="replace")
    text = re.sub(r"//.*", "", text)
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    names: List[str] = []
    stack: List[Tuple[str, str, int, bool]] = []
    depth = 0
    pending_template = False
    for ln in text.splitlines():
        mns = _NS_OPEN_RE.search(ln)
        mcl = _CLASS_OPEN_RE.search(ln)
        if mcl and ln[: mcl.start()].rstrip().endswith("enum"):
            mcl = None
        if mns:
            stack.append(("ns", mns.group(1), depth, False))
        if mcl:
            templated = (
                pending_template
                or bool(_TEMPLATE_RE.search(ln[: mcl.start()]))
                or any(t for _, _, _, t in stack)
            )
            stack.append(("cls", mcl.group(1), depth, templated))
            if not templated:
                names.append("::".join(n for _, n, _, _ in stack))
        if _TEMPLATE_RE.search(ln) and not mcl and ";" not in ln and "{" not in ln:
            pending_template = True
        elif mcl or ";" in ln or "{" in ln:
            pending_template = False
        depth += ln.count("{") - ln.count("}")
        while stack and stack[-1][2] >= depth:
            stack.pop()
    return names


def _dump_layouts(
    cxx: str, target_flags: Sequence[str], header: str, probes: Sequence[str]
) -> Tuple[Dict[str, Dict[str, int]], List[str]]:
    probe_flags = [
        "-fsyntax-only",
        "-ferror-limit=0",
        "-Wno-everything",
        "-Qunused-arguments",
        "-include",
        "cstdint",
        "-include",
        "cstddef",
        "-Xclang",
        "-fdump-record-layouts",
    ]
    lines = [f'#include "{header}"'] + [
        f'static_assert(sizeof({q}) > 0, "PROBE {q}");' for q in probes
    ]
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as tf:
        tf.write("\n".join(lines) + "\n")
        tmp = tf.name
    try:
        proc = subprocess.run(
            [cxx, *target_flags, *probe_flags, tmp],
            capture_output=True,
            text=True,
        )
    finally:
        try:
            os.unlink(tmp)
        except OSError:
            pass

    classes: Dict[str, Dict[str, int]] = {}
    cur: Optional[str] = None
    allowed: set[int] = set()
    for ln in proc.stdout.splitlines():
        if "Dumping AST Record Layout" in ln:
            cur = None
            continue
        h = _HEAD_RE.match(ln)
        if h:
            if "(anonymous" in ln:
                m_anon = re.match(r"^\s*0 \| (?:class|struct|union) (.+?)\s*$", ln)
                cur = m_anon.group(1) if m_anon else h.group(1)
            else:
                cur = h.group(1)
            classes.setdefault(cur, {})
            allowed = {3}
            continue
        if cur is None:
            continue
        mi = re.match(r"^\s*\d+ \|(\s+)\S", ln)
        if not mi or len(mi.group(1)) not in allowed:
            continue
        if "(anonymous" in ln:
            allowed.add(len(mi.group(1)) + 2)
            continue
        if "vtable pointer" in ln:
            continue
        fm = _FIELD_RE.match(ln)
        if fm:
            classes[cur].setdefault(fm.group(4), int(fm.group(1)))

    dumped = set(classes)
    unprobed = [
        q for q in probes if q not in dumped and not any(d.endswith("::" + q) for d in dumped)
    ]
    return classes, unprobed


def run_layout_check(
    build_dir: Path,
    src_root: Path = REPO_ROOT / "src",
    class_filter: Sequence[str] = (),
    explicit_clangxx: Optional[str] = None,
) -> int:
    tc = resolve_ndk_clangxx(build_dir, explicit_clangxx, src_root.parent)
    if tc is None:
        print(
            "layout: NDK ARM clang++ not found (need compile_commands.json, CMakeCache.txt, "
            "GOF2_NDK_CLANGXX, or .cache/ndk). Refusing host compiler.",
            file=sys.stderr,
        )
        return 2
    cxx, target_flags = tc

    hdrs = [
        h
        for h in sorted(src_root.rglob("*.h"))
        if class_filter or re.search(r"\b\w+_0x[0-9a-fA-F]+\b", h.read_text(errors="replace"))
    ]
    all_classes: Dict[str, Tuple[str, Dict[str, int]]] = {}
    unchecked: Dict[str, str] = {}
    for h in hdrs:
        rel = h.relative_to(src_root).as_posix()
        layouts, unprobed = _dump_layouts(cxx, target_flags, rel, _probe_classes(h))
        for q in unprobed:
            unchecked.setdefault(q, rel)
        for cls, fields in layouts.items():
            if fields and cls not in all_classes:
                all_classes[cls] = (rel, fields)

    dumped_short = {c.split("::")[-1] for c in all_classes}
    unchecked = {
        q: rel
        for q, rel in unchecked.items()
        if q not in all_classes and q.split("::")[-1] not in dumped_short
    }

    def suppressed(cls: str, name: str) -> bool:
        if "::(anonymous" not in cls:
            return False
        parent = cls.split("::(anonymous")[0]
        return name in all_classes.get(parent, ("", {}))[1]

    if class_filter:
        for want in class_filter:
            for cls, (rel, fields) in all_classes.items():
                if cls.split("::")[-1] == want or (
                    "(anonymous" in cls and cls.split("::(anonymous")[0].split("::")[-1] == want
                ):
                    print(f"\n=== {cls} ({rel}) ===")
                    for name, off in sorted(fields.items(), key=lambda kv: kv[1]):
                        hx = _HEXNAME_RE.search(name)
                        mark = ""
                        if hx and int(hx.group(1), 16) != off:
                            mark = f"  <-- DRIFT name=0x{int(hx.group(1), 16):x} actual=0x{off:x}"
                        print(f"  0x{off:<5x} {name}{mark}")
        return 0

    drift: List[Tuple[str, str, int, int, str]] = []
    for cls, (rel, fields) in all_classes.items():
        for name, off in fields.items():
            hx = _HEXNAME_RE.search(name)
            if hx and not suppressed(cls, name):
                named = int(hx.group(1), 16)
                if named != off:
                    drift.append((cls, name, named, off, rel))

    print(f"Layout drift: {len(drift)} field(s), {len(unchecked)} unchecked class(es)")
    for cls, name, named, off, rel in drift:
        print(f"  DRIFT {cls} ({rel}): {name} name=0x{named:x} actual=0x{off:x}")
    for q, rel in sorted(unchecked.items()):
        print(f"  UNCHECKED {q} ({rel})")
    return 1 if (drift or unchecked) else 0


def cmd_layout(args: argparse.Namespace) -> int:
    build_dir = resolve_build_dir(args.build_dir)
    return run_layout_check(
        build_dir=build_dir,
        src_root=Path(args.src).resolve(),
        class_filter=args.classes,
        explicit_clangxx=args.clangxx,
    )


# ---------------------------------------------------------------------------
# Subcommand: check (full regression gate + atomic baseline ratchet)
# ---------------------------------------------------------------------------

def check_build_freshness(so_path: Path, repo_root: Path) -> List[Path]:
    """Return list of tracked source/build inputs newer than so_path."""
    so_mtime = so_path.stat().st_mtime
    stale: List[Path] = []
    src_exts = {".c", ".cpp", ".h", ".hpp"}

    def _scan_dir(root: Path, exts: set[str], skip_dirs: set[str]) -> None:
        if not root.is_dir():
            return
        for dirpath, dirnames, filenames in os.walk(root):
            dirnames[:] = [d for d in dirnames if d not in skip_dirs]
            for fn in filenames:
                p = Path(dirpath) / fn
                if p.suffix in exts and p.stat().st_mtime > so_mtime:
                    stale.append(p)

    _scan_dir(repo_root / "src", src_exts, {"__pycache__"})
    _scan_dir(
        repo_root / "third_party",
        src_exts | {".cmake"},
        {"_obj_gof2", "__pycache__"},
    )
    for extra in (repo_root / "CMakeLists.txt", repo_root / "tools" / "source_order.txt"):
        if extra.is_file() and extra.stat().st_mtime > so_mtime:
            stale.append(extra)
    return stale


def atomic_write_json(path: Path, payload: Dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, tmp_name = tempfile.mkstemp(prefix=".baseline_", suffix=".tmp", dir=str(path.parent))
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as fh:
            json.dump(payload, fh, indent=2)
            fh.write("\n")
        os.replace(tmp_name, path)
    except Exception:
        try:
            os.unlink(tmp_name)
        except OSError:
            pass
        raise


def cmd_check(args: argparse.Namespace) -> int:
    paths = resolve_paths(args)
    repo_root = Path(args.repo_root).resolve() if args.repo_root else REPO_ROOT
    baseline_path = Path(args.baseline).resolve() if args.baseline else (repo_root / "tools" / "baseline.json")

    if not baseline_path.is_file():
        print(f"error: baseline file not found: {baseline_path}", file=sys.stderr)
        return 2
    if not paths["original"] or not paths["original"].is_file():
        print(f"error: original .so not found: {paths['original']}", file=sys.stderr)
        return 2
    if not paths["ours"].is_file():
        print(f"error: built .so not found: {paths['ours']}", file=sys.stderr)
        return 2

    failures: List[str] = []

    # 1. Build freshness
    if not args.no_freshness:
        stale = check_build_freshness(paths["ours"], repo_root)
        if stale:
            sample = ", ".join(str(p.relative_to(repo_root)) for p in stale[:5])
            failures.append(
                f"build is stale: {len(stale)} file(s) newer than {paths['ours'].name} ({sample})"
            )

    # 2. Binary comparison vs baseline
    baseline = json.loads(baseline_path.read_text())
    report = compare_binaries(
        paths["original"],
        paths["ours"],
        objdump=paths["objdump"],
        use_disasm=not args.no_disasm,
    )
    print_status(report, limit=20)

    bf = baseline["functions"]
    rf = report["functions"]
    bd = baseline["data"]
    rd = report["data"]
    allowed_extra = set(baseline.get("allowed_extra", []))
    base_imports = set(baseline.get("imports", []))
    ours_imports = set(report["imports"]["ours"])

    if rf["byte_exact"] < bf["byte_exact"]:
        failures.append(f"byte_exact regressed: {rf['byte_exact']} < {bf['byte_exact']}")
    if rf["linked_exact"] < bf["linked_exact"]:
        failures.append(f"linked_exact regressed: {rf['linked_exact']} < {bf['linked_exact']}")
    if rf["stub_zero_size"] > bf["stub_zero_size"]:
        failures.append(f"stub_zero_size increased: {rf['stub_zero_size']} > {bf['stub_zero_size']}")
    if rf["missing"] > bf["missing"]:
        failures.append(f"missing functions increased: {rf['missing']} > {bf['missing']}")
    if rf["wrong_type"] > bf["wrong_type"]:
        failures.append(f"wrong_type increased: {rf['wrong_type']} > {bf['wrong_type']}")
    if rf["extra"] > bf["extra"]:
        failures.append(f"extra functions increased: {rf['extra']} > {bf['extra']}")

    unallowed_extra = sorted(set(rf["extra_defined_names"]) - allowed_extra)
    if unallowed_extra:
        failures.append(
            f"undocumented extra exports ({len(unallowed_extra)}): {', '.join(unallowed_extra[:5])}"
        )

    if rd["exact"] < bd["exact"]:
        failures.append(f"data_exact regressed: {rd['exact']} < {bd['exact']}")

    new_imports = sorted(ours_imports - base_imports)
    if new_imports:
        failures.append(
            f"unexpected new undefined imports ({len(new_imports)}): {', '.join(new_imports[:5])}"
        )

    # 3. Source authenticity lint
    if not args.skip_lint:
        src_root = repo_root / "src"
        if src_root.is_dir():
            hard_errors, audit = run_lint(src_root, repo_root / "CMakeLists.txt")
            if hard_errors:
                failures.append(f"lint failed with {len(hard_errors)} hard violation(s)")
                for e in hard_errors[:10]:
                    print(f"  LINT [{e['rule']}] {e['file']}:{e['line']}: {e['text']}")
            else:
                debt = {k: len(v) for k, v in audit.items() if v}
                print(f"\nLint      : CLEAN (audit debt: {debt or 'none'})")

    # 4. Struct layout check (when NDK clang++ is available)
    if not args.skip_layout:
        tc = resolve_ndk_clangxx(paths["build_dir"], None, repo_root)
        if tc is not None:
            rc_layout = run_layout_check(paths["build_dir"], repo_root / "src")
            if rc_layout != 0:
                failures.append("layout check failed (drifted or unchecked fields)")
        else:
            print("Layout    : skipped (NDK clang++ not found; pass --skip-layout to silence)")

    # Write report to build dir (never repo root)
    if not args.no_json:
        out_path = Path(args.json) if args.json else (paths["build_dir"] / "report.json")
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text(json.dumps(report, indent=2) + "\n")

    if failures:
        print("\nCHECK FAILED:")
        for f in failures:
            print(f"  - {f}")
        return 1

    if args.update_baseline:
        updated = {
            "functions": {
                "compared": rf["compared"],
                "byte_exact": max(rf["byte_exact"], bf["byte_exact"]),
                "linked_exact": max(rf["linked_exact"], bf["linked_exact"]),
                "stub_zero_size": min(rf["stub_zero_size"], bf["stub_zero_size"]),
                "missing": min(rf["missing"], bf["missing"]),
                "wrong_type": min(rf["wrong_type"], bf["wrong_type"]),
                "extra": min(rf["extra"], bf["extra"]),
            },
            "data": {
                "exact": max(rd["exact"], bd["exact"]),
                "common": rd["common"],
            },
            "allowed_extra": sorted(allowed_extra),
            "imports": sorted(ours_imports),
        }
        atomic_write_json(baseline_path, updated)
        print(f"\nBaseline updated atomically: {baseline_path}")

    print("\nCHECK PASSED")
    return 0


# ---------------------------------------------------------------------------
# CLI entrypoint
# ---------------------------------------------------------------------------

def build_parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(
        prog="validate.py",
        description="Fast validation, diff, layout, lint, and regression gate for GOF2HD.",
    )
    sub = p.add_subparsers(dest="cmd", required=True)

    def add_common_binary_args(sp: argparse.ArgumentParser) -> None:
        sp.add_argument("--original", help="Path to original libgof2hdaa.so")
        sp.add_argument("--ours", help="Path to built libgof2hdaa.so")
        sp.add_argument("--build-dir", help="CMake build directory (default: cmake-build-match)")
        sp.add_argument("--objdump", help="Path to NDK objdump or llvm-objdump")

    sp_status = sub.add_parser("status", help="Compare original vs built .so and rank remaining diffs")
    add_common_binary_args(sp_status)
    sp_status.add_argument("--json", help="Write JSON report to path (default: <build-dir>/report.json)")
    sp_status.add_argument("--no-json", action="store_true", help="Do not write JSON report")
    sp_status.add_argument("--no-disasm", action="store_true", help="Skip objdump disassembly for non-exact functions")
    sp_status.add_argument("--limit", type=int, default=None, help="Limit displayed non-exact functions")
    sp_status.set_defaults(func=cmd_status)

    sp_diff = sub.add_parser("diff", help="Side-by-side disassembly and byte hunks for symbol(s)")
    add_common_binary_args(sp_diff)
    sp_diff.add_argument("symbols", nargs="+", help="Exact mangled symbol name(s) or substring(s)")
    sp_diff.add_argument("--hex-only", action="store_true", help="Force raw word/hex diff without objdump")
    sp_diff.add_argument("--max-matches", type=int, default=5, help="Max symbols per substring query")
    sp_diff.set_defaults(func=cmd_diff)

    sp_check = sub.add_parser("check", help="Full regression gate vs tools/baseline.json")
    add_common_binary_args(sp_check)
    sp_check.add_argument("--baseline", help="Path to baseline JSON (default: tools/baseline.json)")
    sp_check.add_argument("--repo-root", help="Repository root (default: auto)")
    sp_check.add_argument("--json", help="Write JSON report to path (default: <build-dir>/report.json)")
    sp_check.add_argument("--no-json", action="store_true", help="Do not write JSON report")
    sp_check.add_argument("--no-disasm", action="store_true", help="Skip objdump disassembly for non-exact functions")
    sp_check.add_argument("--no-freshness", action="store_true", help="Skip mtime freshness check")
    sp_check.add_argument("--skip-lint", action="store_true", help="Skip source lint")
    sp_check.add_argument("--skip-layout", action="store_true", help="Skip NDK clang++ struct layout check")
    sp_check.add_argument("--update-baseline", action="store_true", help="Atomically ratchet baseline upward if clean")
    sp_check.set_defaults(func=cmd_check)

    sp_layout = sub.add_parser("layout", help="Verify field_0xNN struct offsets via NDK clang++")
    sp_layout.add_argument("classes", nargs="*", help="Optional class names to dump")
    sp_layout.add_argument("--build-dir", help="CMake build directory")
    sp_layout.add_argument("--src", default=str(REPO_ROOT / "src"), help="Source root")
    sp_layout.add_argument("--clangxx", help="Explicit path to NDK clang++")
    sp_layout.set_defaults(func=cmd_layout)

    sp_lint = sub.add_parser("lint", help="Source-authenticity linter over src/")
    sp_lint.add_argument("--src", default=str(REPO_ROOT / "src"), help="Source root")
    sp_lint.add_argument("--cmake", help="Path to CMakeLists.txt for per-file flag audit")
    sp_lint.add_argument("--strict", action="store_true", help="Also fail on audit/technical-debt warnings")
    sp_lint.add_argument("-v", "--verbose", action="store_true", help="Print individual audit items")
    sp_lint.set_defaults(func=cmd_lint)

    return p


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    return int(args.func(args))


if __name__ == "__main__":
    sys.exit(main())
