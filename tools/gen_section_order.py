#!/usr/bin/env python3
"""Generate a gold --section-ordering-file that reproduces the original .so's .text
function order.

With -ffunction-sections every function `SYM` lands in its own section `.text.SYM`.
The original .so (stripped, only .dynsym) exposes its function symbols with addresses;
sorting those by address gives the original's function layout order. Emitting the
corresponding `.text.SYM` names in that order lets gold place our functions in the
exact same sequence, so inter-function bl/blx and GOT/PLT relative offsets line up --
the prerequisite for whole-.so byte/hash identity while keeping one-class-per-file source.

Usage: gen_section_order.py <original.so> <nm-tool> <out-file>
"""
import subprocess
import sys


def main() -> int:
    orig, nm, out = sys.argv[1], sys.argv[2], sys.argv[3]
    # -n: sort by address; -D: dynamic symbols (all the stripped original exposes).
    res = subprocess.run([nm, "-nD", "--defined-only", orig],
                         capture_output=True, text=True)
    lines = []
    for ln in res.stdout.splitlines():
        parts = ln.split(maxsplit=2)
        # "<addr> <type> <name>"; text symbols are t/T/w/W.
        if len(parts) == 3 and parts[1] in ("t", "T", "w", "W"):
            lines.append(".text." + parts[2])
    with open(out, "w") as f:
        f.write("\n".join(lines) + "\n")
    print(f"wrote {len(lines)} section names to {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
