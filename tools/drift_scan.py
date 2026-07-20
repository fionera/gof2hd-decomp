#!/usr/bin/env python3
"""drift_scan.py -- find struct fields whose name encodes an offset (field_0xNN, byte_0xNN, ...)
that does NOT match the field's actual compiled offset.

Such "drift" means the struct has missing/mis-sized members upstream, so accesses land at the wrong
byte vs the original binary. It is invisible to symbol-parity (function-level), so we check
`__builtin_offsetof` for every hex-named member against the offset in its name, under the REAL
ARM32 target: the project's NDK clang++ (armv7 AAPCS), resolved from cmake-build-match/. The host
compiler is never used -- host `clang++ -m32` is i386, which aligns long long/double/uint64_t to 4
where AAPCS aligns them to 8, so its offsets are wrong for any class holding an 8-byte scalar.

Mechanics: for each hex-named member we emit `DRIFT_OFFSET<__builtin_offsetof(Q, m)> probe_i;`
against an undeclared template -- the guaranteed error `undefined template 'DRIFT_OFFSET<N>'`
carries the ACTUAL offset (NDK clang 7 static_assert failures do not print evaluated expressions).
This also force-completes every probed class; a probe that produces no diagnostic at all means the
compiler never reached it, which is reported instead of silently skipped.

Reports each (Class, member, named-offset, actual-offset) mismatch.
"""
import json
import re
import shlex
import subprocess
import sys
import tempfile
import os
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
ROOT = REPO / "src"
BUILD = REPO / "cmake-build-match"


def resolve_toolchain():
    """Return (clang++ path, target/include flags) of the project's NDK ARM toolchain.

    Preferred: steal the exact compiler + flags of a real TU from compile_commands.json.
    Fallback: GOF2_NDK_CLANGXX + CMAKE_CXX_FLAGS from CMakeCache.txt.
    Fatal if neither resolves -- never silently fall back to the host compiler (i386 -m32 is
    not AAPCS; its layouts are wrong for any class holding an 8-byte scalar).
    """
    db = BUILD / "compile_commands.json"
    if db.is_file():
        try:
            entries = json.loads(db.read_text())
        except Exception as e:
            print(f"warning: unreadable {db}: {e}", file=sys.stderr)
            entries = []
        for ent in entries:
            args = ent.get("arguments") or shlex.split(ent.get("command", ""))
            if not args or "clang++" not in Path(args[0]).name:
                continue
            cxx, flags, it = args[0], [], iter(args[1:])
            for a in it:
                if a == "-o":
                    next(it, None)                      # drop output file
                elif a == "-c" or a == ent.get("file") or a.endswith((".cpp", ".cc", ".cxx")):
                    pass                                # drop compile step + source file
                else:
                    flags.append(a)
            if Path(cxx).is_file():
                return cxx, flags
    cache = BUILD / "CMakeCache.txt"
    if cache.is_file():
        cxx = flags = None
        for ln in cache.read_text(errors="replace").splitlines():
            if ln.startswith("GOF2_NDK_CLANGXX:"):
                cxx = ln.split("=", 1)[1].strip()
            elif ln.startswith("CMAKE_CXX_FLAGS:"):
                flags = shlex.split(ln.split("=", 1)[1])
        if cxx and flags and Path(cxx).is_file():
            return cxx, flags
    sys.exit("drift_scan.py: FATAL: cannot resolve the NDK ARM toolchain.\n"
             "  Need cmake-build-match/compile_commands.json or CMakeCache.txt with "
             "GOF2_NDK_CLANGXX\n  (run `cmake --preset default` first).\n"
             "  Refusing to fall back to the host compiler: host `clang++ -m32` is i386, whose "
             "layouts\n  differ from ARM AAPCS for any class containing long long/double/uint64_t.")


CXX, TARGET_FLAGS = resolve_toolchain()
# No -DGOF2_MATCH: the real build doesn't define it (it'd activate a known-failing Transform assert).
# -include cstdint/cstddef so headers that forgot them still compile standalone.
PROBE_FLAGS = ["-fsyntax-only", "-ferror-limit=0", "-Wno-everything", "-Winvalid-offsetof",
               "-Qunused-arguments", "-include", "cstdint", "-include", "cstddef"]

# a data-member line: <type ...> <name>[ [..] ];   (no '(' => not a method; no '=', static, typedef)
MEMBER = re.compile(r'^\s*([A-Za-z_][\w:<>,*& ]*?[ *&])([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*;\s*$')
HEXNAME = re.compile(r'_0x([0-9a-fA-F]+)')          # the offset encoded in a member name
CLASS_OPEN = re.compile(r'\b(?:class|struct)\s+([A-Za-z_]\w*)\s*(?:final\s*)?(?::[^{]*)?\{')
NS_OPEN = re.compile(r'\bnamespace\s+([A-Za-z_]\w*)\s*\{')
# the actual offset, carried by the undefined-template diagnostic on the probe's line:
#   probe.cpp:7:49: error: implicit instantiation of undefined template 'DRIFT_OFFSET<420>'
PROBE_ERR = re.compile(r':(\d+):\d+: error: implicit instantiation of undefined template '
                       r"'DRIFT_OFFSET<(\d+)")
ANY_ERR = re.compile(r':(\d+):\d+: (?:fatal )?error: (.*)')
NONSTD = set()


def members_of(path):
    """Yield (qualified_class, member, named_offset) for hex-named data members in a header."""
    text = path.read_text(errors="replace")
    text = re.sub(r'//.*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    stack = []          # list of (kind, name, depth_when_opened)  kind: 'ns' | 'cls'
    depth = 0
    for ln in text.splitlines():
        # member detection: only when the innermost scope is a class/struct
        if stack and stack[-1][0] == 'cls':
            mm = MEMBER.match(ln)
            if mm and not any(t in ln for t in ('static', 'typedef', 'using', 'return')) \
                    and '=' not in ln and 'union' not in mm.group(1) and 'struct' not in mm.group(1):
                hx = HEXNAME.search(mm.group(2))
                if hx:
                    qual = "::".join(n for _k, n, _d in stack)
                    yield qual, mm.group(2), int(hx.group(1), 16)
        # scope changes on this line: push ns/class at the current depth, then apply braces
        mns = NS_OPEN.search(ln)
        mcl = CLASS_OPEN.search(ln)
        if mns:
            stack.append(('ns', mns.group(1), depth))
        if mcl:
            stack.append(('cls', mcl.group(1), depth))
        depth += ln.count('{') - ln.count('}')
        while stack and stack[-1][2] >= depth:
            stack.pop()


def main():
    checks = []   # (header, qualclass, member, named)
    headers = sorted(ROOT.rglob("*.h"))
    for h in headers:
        try:
            for qual, member, named in members_of(h):
                checks.append((h, qual, member, named))
        except Exception as e:
            print(f"parse-skip {h}: {e}", file=sys.stderr)

    # group by header; emit one TU per header so class names resolve in their own context
    rows = []     # (rel, qualclass, member, named, actual_or_None)
    by_header = {}
    for h, q, m, n in checks:
        by_header.setdefault(h, []).append((q, m, n))
    for h, items in by_header.items():
        rel = h.relative_to(ROOT).as_posix()
        body = [f'#include "{rel}"', "template<unsigned long N> struct DRIFT_OFFSET;"]
        lineof = {}   # TU line -> (qual, member, named); every probe line MUST diagnose
        for i, (q, m, n) in enumerate(items):
            lineof[len(body) + 1] = (q, m, n)
            body.append(f'DRIFT_OFFSET<__builtin_offsetof({q}, {m})> probe_{i};')
        with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
            f.write("\n".join(body) + "\n"); tmp = f.name
        out = subprocess.run([CXX] + TARGET_FLAGS + PROBE_FLAGS + [tmp],
                             capture_output=True, text=True)
        os.unlink(tmp)
        # classes flagged non-standard-layout: offsetof there is compiler-defined -> layout.py
        # (record-layout dump) is the authority; we skip them from this report
        for w in re.findall(r"non-standard-layout type '([\w:]+)'", out.stderr):
            NONSTD.add(w.split("::")[-1])
        seen = {}     # TU line -> actual offset
        failed = {}   # TU line -> first non-probe error text
        for ln in out.stderr.splitlines():
            pe = PROBE_ERR.search(ln)
            if pe and int(pe.group(1)) in lineof:
                seen.setdefault(int(pe.group(1)), int(pe.group(2)))
                continue
            ae = ANY_ERR.search(ln)
            if ae and int(ae.group(1)) in lineof:
                failed.setdefault(int(ae.group(1)), ae.group(2).strip())
        for line, (q, m, n) in sorted(lineof.items()):
            if line in seen:
                if seen[line] != n:
                    rows.append((rel, q, m, n, seen[line]))
            else:
                # no diagnostic at the probe line: member unprobed (bad member, or the header
                # died before the probes) -- report, never skip silently
                why = failed.get(line, "no diagnostic; header failed to compile?")
                print(f"probe-fail {rel} {q}::{m}: {why[:90]}", file=sys.stderr)

    rows2, skipped = [], set()
    for rel, cls, mem, named, actual in sorted(set(rows)):
        short = cls.split("::")[-1]
        if short in NONSTD:                         # offsetof unreliable here -> layout.py decides
            skipped.add(short); continue
        rows2.append((rel, cls, mem, named, actual))
    print(f"\n=== DRIFT (standard-layout, reliable): {len(rows2)} field(s) ===")
    for rel, cls, mem, named, actual in rows2:
        print(f"  {cls}::{mem}  name=0x{named:x}  actual=0x{actual:x}  ({rel})")
    print(f"\n=== {len(NONSTD)} non-standard-layout classes "
          f"(offsetof unreliable; use tools/layout.py): ===")
    print("  " + ", ".join(sorted(NONSTD)))
    return 1 if rows2 else 0


if __name__ == "__main__":
    sys.exit(main())
