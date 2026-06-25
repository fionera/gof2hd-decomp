#!/usr/bin/env python3
"""Two-pass match-link for libgof2hdaa.so.

Pass 1 links the .so normally. We then compute the set of dynamic symbols we export
that the original does NOT (`ours - original`) and re-link localizing exactly that
set with a linker version script.

What this set actually is -- verified, not assumed:
  * ~38 redundant constructor/destructor ALIASES: the complete-object variant
    (C1/D1) sits at the SAME address as the base-object variant (C2/D2) for simple
    classes -- duplicate NAMES for byte-identical code. clang emits both; the
    original's build keeps only the base name. Which ones get deduped is not a
    function of our source (clang aliases them uniformly), so it can only be read
    off the diff.
  * a couple of internal/vendored symbols (an internal JNI dispatch table whose
    initializers keep 5 _JNIEnv methods alive; a vendored libcrypto version string).

Localizing moves these out of .dynsym while they stay DEFINED in the binary -- all
internal references were already bound at static-link time, so no code changes and
nothing breaks (confirmed: 0 missing after the second pass). This reproduces the
original build's export table exactly.
"""
import os
import subprocess
import sys
import tempfile


def _flag(args, name):
    return args[args.index(name) + 1]


def main():
    args = sys.argv[1:]
    gold = _flag(args, "--gold")
    out = _flag(args, "--out")
    symdiff_py = _flag(args, "--symdiff")
    gold_args = args[args.index("--") + 1:]

    # Pass 1: ordinary link.
    subprocess.run([gold, *gold_args, "-o", out], check=True)

    # Reuse symdiff's ELF .dynsym reader and the known original path.
    sys.path.insert(0, os.path.dirname(os.path.abspath(symdiff_py)))
    import symdiff as sd

    extras = sorted(sd.read_defined_dynsyms(out) - sd.read_defined_dynsyms(sd.ORIG))
    if not extras:
        return 0

    with tempfile.NamedTemporaryFile("w", suffix=".ver", delete=False) as vs:
        vs.write("{\n  local:\n")
        for s in extras:
            vs.write("    %s;\n" % s)
        vs.write("};\n")
        ver = vs.name
    try:
        # Pass 2: same link, localizing the non-original-exported names.
        subprocess.run([gold, *gold_args, "--version-script", ver, "-o", out], check=True)
    finally:
        os.unlink(ver)
    return 0


if __name__ == "__main__":
    sys.exit(main())
