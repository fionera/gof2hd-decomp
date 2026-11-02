#!/usr/bin/env python3
"""Order the link's object files the way the original link received them.

The original was linked without a section-ordering file, so its .text is plain input order: each
object's functions in turn. Each of our objects is therefore placed by the lowest original address
of a STRONG (T) function it defines -- weak/inline (W) functions are excluded because the copy the
linker keeps is decided by this very order (first object wins). Objects with no strong function
fall back to the earliest original .dynsym index of anything they define; objects with neither
keep their relative order at the end. Writes a gold response file (one path per line).

Usage: gen_link_order.py <original.so> <nm-tool> <out.rsp> <object>...
"""
import os
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "verify"))
from elf import Elf32, SHT_DYNSYM  # noqa: E402


def main() -> int:
    orig, nm, out, objs = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4:]
    e = Elf32(orig)
    index, addr = {}, {}
    for i, s in enumerate(e._symbols(SHT_DYNSYM)):
        if s["name"] and s["shndx"] != 0:
            index.setdefault(s["name"], i)
    for name, f in e.functions().items():
        addr[name] = f["value"]
    keyed = []
    for pos, o in enumerate(objs):
        res = subprocess.run([nm, "--defined-only", "-g", o], capture_output=True, text=True)
        syms = [p for p in (ln.split() for ln in res.stdout.splitlines()) if len(p) == 3]
        strong = [addr[p[2]] for p in syms if p[1] == "T" and p[2] in addr]
        if strong:
            keyed.append((0, min(strong), pos, o))
            continue
        best = min((index[p[2]] for p in syms if p[2] in index), default=None)
        keyed.append((1 if best is not None else 2, best or 0, pos, o))
    keyed.sort()
    # The `.text` address heuristic above is not always the original's link order: the
    # original's .rodata.str1.1 chunks are the witness for the *link* order, and they can
    # disagree when the original reordered `.text` separately.  The w3624 GameResource
    # merge removed the last such case: BuildResourceList, loadPortraits and
    # loadLowTexturesAndMaterials are one TU again (Classes/Game/GameResource.cpp), whose
    # lowest strong address is BuildResourceList; its internal .rodata order is Build,
    # loadLow, loadPortraits, exactly the original's.
    # GameText.cpp is data-only (its sole symbol is GameText::currentLang).  The original
    # Android link order put it between PaintCanvas.cpp and Engine.cpp: currentLang sits at
    # .data+0x10c4, between PaintCanvas's i_fov (+0x10c0) and Engine's lodBiasDiffuse
    # (+0x10c8).  The .text-address key cannot place a TU with no strong function, so move
    # it next to Engine.cpp.  No TU between PaintCanvas and Engine owns a .data section, so
    # "immediately before Engine" is exactly "immediately after PaintCanvas" for .data.
    for first, second in (
        ("GameText.cpp.obj", "Engine.cpp.obj"),
    ):
        names = [k[3] for k in keyed]
        fi = next((i for i, o in enumerate(names) if o.endswith(first)), None)
        si = next((i for i, o in enumerate(names) if o.endswith(second)), None)
        if fi is not None and si is not None and fi > si:
            keyed.insert(si, keyed.pop(fi))
    names = [k[3] for k in keyed]
    hi = next((i for i, o in enumerate(names) if o.endswith("NdkHooks.cpp.obj")), None)
    ai = next((i for i, o in enumerate(names) if o.endswith("NdkAutosave.cpp.obj")), None)
    if hi is not None and ai is not None and ai != hi + 1:
        keyed.insert(hi + 1, keyed.pop(ai))
    with open(out, "w") as f:
        f.write("\n".join(o for *_, o in keyed) + "\n")
    print(f"wrote link order for {len(objs)} objects to {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
