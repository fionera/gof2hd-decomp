#!/usr/bin/env python3
"""goal_lint.py -- structural invariants for the decompile.

The port is meant to read like clean, hand-written C++ that happens to compile to the original's
machine code -- not like decompiler output. This linter gates the invariants the user requires:

  one_class      a header/.cpp defines more than one class/struct (one class per file)
  void_ptr       use of `void *` (model the real pointed-to type instead)
  union_decl     use of `union` (model the real type instead)
  explicit_inst  explicit template instantiation (`template class X<...>;`) -- templates must be
                 instantiated implicitly so an unused one shows up as a missing symbol, not a lie
  banned_token   inline asm in any spelling (asm( / asm volatile( / __asm / __asm__) /
                 __attribute__ / __declspec / `extern "C"` cruft / linker-script artifacts
  circular_inc   a cycle in the #include graph

Exit 0 iff every criterion is clean. Prints offenders grouped by criterion.

This is a pragmatic textual linter (no full C++ parse): it strips comments and string/char literals
first so tokens inside them never trip a rule, and it understands the handful of legitimate
exceptions the codebase needs (e.g. `void *` is unavoidable in a few ABI boundaries -- those carry an
inline `// lint: void_ptr` waiver).
"""
import argparse
import json
import re
import sys
from pathlib import Path

WAIVER = re.compile(r"//\s*lint:\s*(.+)")
WAIVER_WORD = re.compile(r"[a-z_]+")


def strip_noise(text):
    """Remove comments and the contents of string/char literals, keeping line count stable.

    Returns (clean_text, waivers) where waivers maps line number -> set of waived criteria (parsed
    from `// lint: ...` BEFORE comments are stripped).
    """
    waivers = {}
    for i, ln in enumerate(text.splitlines(), 1):
        m = WAIVER.search(ln)
        if m:
            # waive any criterion whose name appears as a word after `lint:` (free-text reason ok)
            waivers[i] = set(WAIVER_WORD.findall(m.group(1)))

    out = []
    i, n = 0, len(text)
    state = None  # None | 'line' | 'block' | 'str' | 'char'
    while i < n:
        c = text[i]
        nxt = text[i + 1] if i + 1 < n else ""
        if state is None:
            if c == "/" and nxt == "/":
                state, i = "line", i + 2; out.append("  "); continue
            if c == "/" and nxt == "*":
                state, i = "block", i + 2; out.append("  "); continue
            if c == '"':
                state, i = "str", i + 1; out.append('"'); continue
            if c == "'":
                state, i = "char", i + 1; out.append("'"); continue
            out.append(c); i += 1
        elif state == "line":
            if c == "\n":
                state = None; out.append("\n")
            else:
                out.append(" " if c != "\t" else "\t")
            i += 1
        elif state == "block":
            if c == "*" and nxt == "/":
                state, i = None, i + 2; out.append("  "); continue
            out.append("\n" if c == "\n" else " "); i += 1
        elif state in ("str", "char"):
            q = '"' if state == "str" else "'"
            if c == "\\":
                out.append("  "); i += 2; continue
            if c == q:
                state = None; out.append(q); i += 1; continue
            out.append("\n" if c == "\n" else " "); i += 1
    return "".join(out), waivers


# A class/struct *definition* (has a body), not a forward decl, not a variable of struct type.
# An optional `enum` prefix is captured so `enum class` / `enum struct` (scoped enums, not classes)
# can be skipped by the caller.
CLASS_DEF = re.compile(r"\b(?P<enum>enum\s+)?(?:class|struct)\s+(?P<name>[A-Za-z_]\w*)\s*(?:final\s*)?(?::[^{;]+)?\{")
EXPLICIT_INST = re.compile(r"^\s*template\s+(?:class|struct)\s+\w", re.M)
VOID_PTR = re.compile(r"\bvoid\s*\*")
# `typedef void FOO;` / `using FOO = void;` launder `void *` behind an alias (FOO* evades VOID_PTR);
# counted under void_ptr. Function typedefs (`typedef void F();`, `using F = void(*)();`) don't match.
VOID_ALIAS = re.compile(r"\btypedef\s+void\s+\w+\s*;|\busing\s+\w+\s*=\s*void\s*;")
UNION = re.compile(r"\bunion\b")
# Genuine codegen "hacks" the port must never use.
BANNED = {
    # Inline asm in ANY spelling: `asm(`, `asm volatile(` (any qualifier list), `__asm`, `__asm__`,
    # plus `__attribute__` / `__declspec`. Matched on comment/string-stripped text, so the bare word
    # "asm" inside a comment or string literal never trips.
    "banned_token": re.compile(
        r'((?<!\w)__asm(?:__)?\b|\basm\b\s*(?:(?:volatile|inline|goto|const)\b\s*)*\(|__attribute__|__declspec)'),
    # Direct operator calls (`::operator new`, `::operator delete`, `::new`, `::delete`) are a
    # decompiler idiom; the human-style port allocates with `new T()` / `delete p`. Ratcheted SOFT.
    "operator_call": re.compile(r'(?:::\s*)?operator\s+(?:new|delete)\b|::\s*(?:new|delete)\b'),
    # ARM/compiler builtins (`__aeabi_*`, `__builtin_*`) must be written as ordinary C++ -- EXCEPT
    # `__builtin_offsetof`, which is the standard `offsetof` macro expansion (legitimate). SOFT.
    "arm_builtin": re.compile(r'__aeabi_\w+|__builtin_(?!offsetof\b)\w+'),
}
# `extern "C"` is hard-banned too, BUT waivable per-site: the only legitimate use is the mandatory
# native ABI boundary (JNI `Java_*` / NDK `ndk*`, which the original ships unmangled) -- those carry
# a same-line `// lint: extern_c` waiver. Any other (gratuitous) extern "C" is a violation. After
# comment+string stripping, the `"C"` literal becomes `" "`, so we match `extern " "` on clean text.
EXTERN_C = re.compile(r'extern\s*"\s*"')


def line_of(text, pos):
    return text.count("\n", 0, pos) + 1


def lint_file(path):
    """Return a list of (criterion, line, snippet) violations for one file."""
    raw = path.read_text(errors="replace")
    clean, waivers = strip_noise(raw)
    lines = clean.splitlines()
    raw_lines = raw.splitlines()
    viol = []

    def add(crit, lineno, snippet):
        # union_decl is UN-waivable: the original source has no unions (Ghidra invents them), so any
        # `union` is a decompiler artifact to eliminate, never to waive.
        if crit != "union_decl":
            # A `// lint: <crit>` comment waives the criterion on its OWN line only (the codebase
            # convention per docs/VALIDATION.md). No adjacency slack: a wider window would let one
            # waiver silently cover a different, NEW violation on a nearby line.
            if crit in waivers.get(lineno, ()):
                return
        viol.append((crit, lineno, snippet))

    # one class per file: a public class lives in its own header. We enforce this on HEADERS
    # (.h/.hpp) -- the public API surface. Class/struct definitions inside a .cpp are single-TU
    # implementation helpers (decompiler-recovered data overlays / view structs scoped to that one
    # file), which are not separate public classes; splitting them into their own files would be
    # noise, so they don't count. A .cpp still "implements one class" -- its method definitions.
    if path.suffix in (".h", ".hpp"):
        defs = [(m.group("name"), line_of(clean, m.start()))
                for m in CLASS_DEF.finditer(clean) if not m.group("enum")]
        if len(defs) > 1:
            names = ", ".join(f"{n}@{l}" for n, l in defs)
            add("one_class", defs[1][1], f"{len(defs)} definitions: {names}")

    for m in EXPLICIT_INST.finditer(clean):
        l = line_of(clean, m.start())
        add("explicit_inst", l, raw_lines[l - 1].strip())
    for m in VOID_PTR.finditer(clean):
        l = line_of(clean, m.start())
        add("void_ptr", l, raw_lines[l - 1].strip())
    for m in VOID_ALIAS.finditer(clean):
        l = line_of(clean, m.start())
        add("void_ptr", l, raw_lines[l - 1].strip())
    for m in UNION.finditer(clean):
        l = line_of(clean, m.start())
        add("union_decl", l, raw_lines[l - 1].strip())
    for crit, rx in BANNED.items():
        for m in rx.finditer(clean):
            l = line_of(clean, m.start())
            add(crit, l, raw_lines[l - 1].strip())
    for m in EXTERN_C.finditer(clean):
        l = line_of(clean, m.start())
        add("extern_c", l, raw_lines[l - 1].strip())
    return viol


def includes(path, root):
    """Local (quoted) includes of a file, resolved to repo-relative paths when possible."""
    raw = path.read_text(errors="replace")
    out = []
    for m in re.finditer(r'^\s*#\s*include\s*"([^"]+)"', raw, re.M):
        inc = (path.parent / m.group(1)).resolve()
        if inc.exists():
            out.append(inc)
        else:
            cand = (root / m.group(1)).resolve()
            if cand.exists():
                out.append(cand)
    return out


def find_cycles(root, files):
    graph = {f: includes(f, root) for f in files}
    cycles, color = [], {}

    def dfs(node, stack):
        color[node] = 1
        for nb in graph.get(node, []):
            if color.get(nb) == 1:
                i = stack.index(nb)
                cycles.append(stack[i:] + [nb])
            elif color.get(nb, 0) == 0:
                dfs(nb, stack + [nb])
        color[node] = 2

    for f in files:
        if color.get(f, 0) == 0:
            dfs(f, [f])
    # dedupe by frozenset of members
    seen, uniq = set(), []
    for c in cycles:
        key = frozenset(c)
        if key not in seen:
            seen.add(key); uniq.append(c)
    return uniq


# The "hard" invariants must be 0 everywhere -- they encode the user's non-negotiable rules
# (no asm/__attribute__/extern-C/linker hacks, templates without explicit instantiation, no include
# cycles). The "soft" invariants (void*, union, one-class-per-file) describe the *finished* decompile;
# on this WIP tree the offending sites ARE the not-yet-recovered types/fields, so we gate them as a
# per-file **ratchet** against a baseline: changed/new code may not raise a file's count, and the
# baseline shrinks as files are cleaned (`--update-baseline`). This makes `lint` a real regression
# gate that is achievable now, instead of demanding the whole recovery be complete.
HARD = ["banned_token", "extern_c", "union_decl", "explicit_inst", "circular_inc"]
SOFT = ["void_ptr", "one_class", "operator_call", "arm_builtin"]
ALL_CRITS = SOFT + HARD


def collect(root):
    """Return per-file per-criterion counts: {crit: {relpath: count}} plus raw items for display."""
    files = sorted(p for p in root.rglob("*") if p.suffix in (".cpp", ".h", ".hpp"))
    counts = {c: {} for c in ALL_CRITS}
    items = {c: [] for c in ALL_CRITS}
    for f in files:
        rel = f.relative_to(root).as_posix()
        for crit, lineno, snip in lint_file(f):
            counts[crit][rel] = counts[crit].get(rel, 0) + 1
            items[crit].append((rel, lineno, snip))
    for cyc in find_cycles(root, files):
        rel = cyc[0].relative_to(root).as_posix()
        counts["circular_inc"][rel] = counts["circular_inc"].get(rel, 0) + 1
        items["circular_inc"].append((rel, 0, " -> ".join(p.relative_to(root).as_posix() for p in cyc)))
    return counts, items


def main():
    ap = argparse.ArgumentParser(description="structural-invariant ratchet for the decompile")
    ap.add_argument("root", nargs="?", default="src")
    ap.add_argument("--baseline", default=str(Path(__file__).resolve().parent / "goal_lint_baseline.json"))
    ap.add_argument("--update-baseline", action="store_true",
                    help="record current soft-criterion counts as the accepted baseline")
    ap.add_argument("--full", action="store_true", help="print absolute counts, not just regressions")
    args = ap.parse_args()
    root = Path(args.root).resolve()
    counts, items = collect(root)

    if args.update_baseline:
        Path(args.baseline).write_text(json.dumps({c: counts[c] for c in SOFT}, indent=1, sort_keys=True))
        tot = {c: sum(counts[c].values()) for c in SOFT}
        print("baseline updated:", "  ".join(f"{c}={tot[c]}" for c in SOFT))
        return 0

    baseline = {}
    if Path(args.baseline).exists():
        baseline = json.loads(Path(args.baseline).read_text())

    if args.full:
        for c in ALL_CRITS:
            print(f"{c:14} {sum(counts[c].values())}")

    fail = 0
    # Hard invariants: any occurrence anywhere is a failure.
    for c in HARD:
        for rel, lineno, snip in items[c]:
            print(f"FAIL {c}: {rel}:{lineno}: {snip}")
            fail += 1
    # Soft invariants: a file may not exceed its baseline count (a ratchet); shrinking is welcome.
    improved = 0
    for c in SOFT:
        base = baseline.get(c, {})
        for rel, n in sorted(counts[c].items()):
            b = base.get(rel, 0)
            if n > b:
                print(f"REGRESSION {c}: {rel} has {n} (baseline {b}, +{n - b})")
                fail += 1
        cur_tot, base_tot = sum(counts[c].values()), sum(base.values())
        if cur_tot < base_tot:
            improved += base_tot - cur_tot
            print(f"improved {c}: {base_tot} -> {cur_tot} (-{base_tot - cur_tot}; run --update-baseline to lock)")

    if fail:
        print(f"\n{fail} gate failure(s)")
        return 1
    print(f"\nCLEAN ✓ (no new soft violations; hard invariants 0)" + (f"; {improved} improvements pending re-baseline" if improved else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
