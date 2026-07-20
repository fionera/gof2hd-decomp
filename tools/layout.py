#!/usr/bin/env python3
"""layout.py -- authoritative field-offset / drift checker using clang -fdump-record-layouts.

Unlike __builtin_offsetof (UB / returns 0 on non-standard-layout types), -fdump-record-layouts prints
the real offset of every field of every record, polymorphic or not. We parse it, then for each
hex-named member (field_0xNN / byte_0xNN / flag_0xNN / image_0xNN / pad_0xNN ...) compare the actual
offset to the offset encoded in the name. Mismatch = drift.

Layouts are probed with the PROJECT toolchain (NDK clang++, armv7 AAPCS), resolved from
cmake-build-match/. The host compiler is never used: host `clang++ -m32` targets i386, whose ABI
aligns long long / double / uint64_t to 4 bytes where AAPCS aligns them to 8, so every class
containing an 8-byte scalar gets wrong offsets (false drift on correct layouts, false clean on
broken ones).

clang computes record layout lazily: merely #including a header dumps NOTHING for classes the TU
never uses. The probe TU therefore emits `static_assert(sizeof(X) > 0, ...)` for every class
defined in the header, forcing every scanned class to complete; classes that still don't appear in
the dump are reported as UNCHECKED instead of being silently skipped.

Usage:
  tools/layout.py                 # scan all headers, list drift
  tools/layout.py Gun KIPlayer    # dump the resolved layouts of named classes (offset | type name)
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
    sys.exit("layout.py: FATAL: cannot resolve the NDK ARM toolchain.\n"
             "  Need cmake-build-match/compile_commands.json or CMakeCache.txt with "
             "GOF2_NDK_CLANGXX\n  (run `cmake --preset default` first).\n"
             "  Refusing to fall back to the host compiler: host `clang++ -m32` is i386, whose "
             "layouts\n  differ from ARM AAPCS for any class containing long long/double/uint64_t.")


CXX, TARGET_FLAGS = resolve_toolchain()
PROBE_FLAGS = ["-fsyntax-only", "-ferror-limit=0", "-Wno-everything", "-Qunused-arguments",
               "-include", "cstdint", "-include", "cstddef",
               "-Xclang", "-fdump-record-layouts"]
HEXNAME = re.compile(r'_0x([0-9a-fA-F]+)')
# a field line in the dump:  "        64 |   char field_0x3f"  (offset | indent type... name)
FIELD = re.compile(r'^\s*(\d+) \|(\s+)([^\n]*?)\b([A-Za-z_]\w*)\s*$')
HEAD = re.compile(r'^\s*0 \| (?:class|struct|union) ([A-Za-z_][\w:]*)')
CLASS_OPEN = re.compile(r'\b(?:class|struct|union)\s+([A-Za-z_]\w*)\s*(?:final\s*)?(?::[^{]*)?\{')
NS_OPEN = re.compile(r'\bnamespace\s+([A-Za-z_]\w*)\s*\{')
TEMPLATE = re.compile(r'\btemplate\s*<')


def probe_classes(path):
    """Qualified names of every named, non-template class/struct/union DEFINED in `path`.

    These are the classes the probe TU must force-complete so -fdump-record-layouts emits them.
    """
    text = path.read_text(errors="replace")
    text = re.sub(r'//.*', '', text)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    names = []
    stack = []          # (kind, name, depth_when_opened, templated)  kind: 'ns' | 'cls'
    depth = 0
    pending_template = False
    for ln in text.splitlines():
        mns = NS_OPEN.search(ln)
        mcl = CLASS_OPEN.search(ln)
        if mcl and ln[:mcl.start()].rstrip().endswith('enum'):
            mcl = None                                   # `enum class X {` is not a record
        if mns:
            stack.append(('ns', mns.group(1), depth, False))
        if mcl:
            templated = (pending_template or bool(TEMPLATE.search(ln[:mcl.start()]))
                         or any(t for _k, _n, _d, t in stack))
            stack.append(('cls', mcl.group(1), depth, templated))
            if not templated:
                names.append("::".join(n for _k, n, _d, _t in stack))
        if TEMPLATE.search(ln) and not mcl and ';' not in ln and '{' not in ln:
            pending_template = True                      # `template<...>` line, class on next line
        elif mcl or ';' in ln or '{' in ln:
            pending_template = False
        depth += ln.count('{') - ln.count('}')
        while stack and stack[-1][2] >= depth:
            stack.pop()
    return names


def dump_layouts(header, probes):
    """Return ({classname: {field: offset}}, [unprobed-class]) for records visible from `header`.

    `probes` (qualified class names defined in the header) are force-completed via sizeof so the
    lazy record-layout dump actually emits them; any probe still absent from the dump is returned
    as unchecked instead of vanishing silently.
    """
    src = [f'#include "{header}"']
    for q in probes:
        src.append(f'static_assert(sizeof({q}) > 0, "PROBE {q}");')
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
        f.write("\n".join(src) + "\n"); tmp = f.name
    out = subprocess.run([CXX] + TARGET_FLAGS + PROBE_FLAGS + [tmp],
                         capture_output=True, text=True)
    os.unlink(tmp)
    classes = {}
    cur = None
    allowed = set()     # member indent levels whose fields belong directly to `cur`
    for ln in out.stdout.splitlines():
        if "Dumping AST Record Layout" in ln:
            cur = None
            continue
        h = HEAD.match(ln)
        if h:
            if "(anonymous" in ln:
                # standalone dump of an anonymous record (e.g. `struct {...} sets[48];`): keyed
                # by its full "Parent::(anonymous at file:line)" name; offsets are RELATIVE to
                # the record. Hex names inside such records encode element-relative offsets.
                # Fields that a parent already captured absolutely (truly anonymous members are
                # dumped inline into the parent) are suppressed at report time in main().
                cur = re.match(r'^\s*0 \| (?:class|struct|union) (.+?)\s*$', ln).group(1)
            else:
                cur = h.group(1)
            classes.setdefault(cur, {})
            allowed = {3}   # direct members sit at indent 3; deeper = nested record contents
            continue
        if cur is None:
            continue
        mi = re.match(r'^\s*\d+ \|(\s+)\S', ln)
        if not mi or len(mi.group(1)) not in allowed:
            continue        # nested named-member / base-class contents: checked in their own record
        if "(anonymous" in ln:
            # anonymous struct/union member: its fields are the parent's fields (absolute offsets)
            allowed.add(len(mi.group(1)) + 2)
            continue
        if "vtable pointer" in ln:
            continue
        fm = FIELD.match(ln)
        if fm:
            # keep the FIRST occurrence, don't overwrite duplicates
            classes[cur].setdefault(fm.group(4), int(fm.group(1)))
    dumped = set(classes)
    unprobed = [q for q in probes
                if q not in dumped and not any(d.endswith("::" + q) for d in dumped)]
    return classes, unprobed


def main():
    args = sys.argv[1:]
    # build a class -> header map by scanning which header defines each class.
    # pre-filter: only headers that textually contain a hex-named member (huge speedup).
    hdrs = [h for h in sorted(ROOT.rglob("*.h"))
            if (args or re.search(r'\b\w+_0x[0-9a-fA-F]+\b', h.read_text(errors="replace")))]
    all_classes = {}
    unchecked = {}       # qualified class -> header it failed to complete in
    for h in hdrs:
        rel = h.relative_to(ROOT).as_posix()
        try:
            layouts, unprobed = dump_layouts(rel, probe_classes(h))
        except Exception as e:
            print(f"probe-fail {rel}: {e}", file=sys.stderr)
            continue
        for q in unprobed:
            unchecked.setdefault(q, rel)
        for cls, fields in layouts.items():
            # only record a class once, from a header that actually defines it (has fields)
            if fields and cls not in all_classes:
                all_classes[cls] = (rel, fields)
    # a class that failed in its own header may still have completed via another TU
    dumped_short = {c.split("::")[-1] for c in all_classes}
    unchecked = {q: rel for q, rel in unchecked.items()
                 if q not in all_classes and q.split("::")[-1] not in dumped_short}

    def suppressed(cls, name):
        """Anonymous-record field already checked absolutely through its parent's inline dump."""
        if "::(anonymous" not in cls:
            return False
        parent = cls.split("::(anonymous")[0]
        return name in all_classes.get(parent, ("", {}))[1]

    if args:  # dump mode
        for want in args:
            for cls, (rel, fields) in all_classes.items():
                if (cls.split("::")[-1] == want
                        or ("(anonymous" in cls
                            and cls.split("::(anonymous")[0].split("::")[-1] == want)):
                    print(f"\n=== {cls}  ({rel}) ===")
                    for name, off in sorted(fields.items(), key=lambda kv: kv[1]):
                        mark = ""
                        hx = HEXNAME.search(name)
                        if hx and int(hx.group(1), 16) != off:
                            mark = f"   <-- DRIFT name=0x{int(hx.group(1),16):x} actual=0x{off:x}"
                        print(f"  0x{off:<5x} {name}{mark}")
        return 0

    drift = []
    for cls, (rel, fields) in all_classes.items():
        for name, off in fields.items():
            hx = HEXNAME.search(name)
            if hx and not suppressed(cls, name):
                named = int(hx.group(1), 16)
                if named != off:
                    drift.append((cls, name, named, off, rel))
    by_cls = {}
    for cls, name, named, off, rel in drift:
        by_cls.setdefault((cls, rel), []).append((name, named, off))
    print(f"=== DRIFT: {len(drift)} field(s) across {len(by_cls)} class(es) ===")
    for (cls, rel), items in sorted(by_cls.items(), key=lambda kv: -len(kv[1])):
        print(f"\n{cls}  ({rel})  [{len(items)}]")
        for name, named, off in sorted(items, key=lambda t: t[2]):
            print(f"  {name}  name=0x{named:x}  actual=0x{off:x}")
    if unchecked:
        print(f"\n=== UNCHECKED: {len(unchecked)} class(es) failed to complete "
              f"(no layout dumped -- NOT verified): ===")
        for q, rel in sorted(unchecked.items()):
            print(f"  {q}  ({rel})")
    return 1 if (drift or unchecked) else 0


if __name__ == "__main__":
    sys.exit(main())
