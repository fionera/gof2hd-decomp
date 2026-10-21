#!/usr/bin/env python3
"""emit_const.py NAME=0xADDR ... -- print `static const char *const NAME = ...;` for the NUL-terminated
string at each original-.so vaddr (exact bytes, split after each \\n)."""
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import pic_strings as ps  # noqa: E402
from elf import Elf32  # noqa: E402

e = Elf32(ps.ORIG)
for arg in sys.argv[1:]:
    n, a = arg.split("=")
    b = e.bytes_at(int(a, 16), 65536)
    b = b[:b.find(b"\0")]
    print(f"static const char *const {n} =\n" +
          "\n".join("    " + l for l in ps.c_literal(b).split("\n")) + ";\n")
