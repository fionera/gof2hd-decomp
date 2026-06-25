#!/usr/bin/env python3
"""Export struct layouts, function signatures and global types from the -g DWARF build
into tools/ghidra/source_types.json, keyed by mangled symbol name.

This is the source-of-truth export consumed by apply_source.py (the Ghidra applier).
The source (`src/`) is the verified, symbol-identical model of the original, so its
DWARF carries every recovered layout/type. NOTE: reads ONLY the local -g objects under
build-dwarf/ -- never anything from _work/.

Prereq:  cmake -B build-dwarf -DGOF2_DEBUG_INFO=ON && cmake --build build-dwarf --target gof2
Usage:   python3 tools/ghidra/export_source_types.py
"""
import glob
import json
import os

from elftools.elf.elffile import ELFFile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OBJDIR = os.path.join(ROOT, "build-dwarf", "CMakeFiles", "gof2.dir")
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "source_types.json")


def _val(die, name, default=None):
    a = die.attributes.get(name)
    return a.value if a is not None else default


def _name(die):
    n = _val(die, "DW_AT_name")
    if n is None:
        return None
    return n.decode("utf-8", "replace") if isinstance(n, bytes) else n


def _target(die):
    if "DW_AT_type" not in die.attributes:
        return None
    return die.get_DIE_from_attribute("DW_AT_type")


def type_str(die, depth=0):
    """Resolve a type DIE to a readable type string (Ghidra normalizes further)."""
    if die is None or depth > 12:
        return "void"
    tag = die.tag
    if tag == "DW_TAG_base_type":
        return _name(die) or "int"
    if tag == "DW_TAG_pointer_type":
        return type_str(_target(die), depth + 1) + " *"
    if tag == "DW_TAG_reference_type":
        return type_str(_target(die), depth + 1) + " *"  # ref -> ptr for Ghidra
    if tag in ("DW_TAG_const_type", "DW_TAG_volatile_type", "DW_TAG_restrict_type"):
        return type_str(_target(die), depth + 1)  # drop cv-qualifiers
    if tag in ("DW_TAG_structure_type", "DW_TAG_class_type",
               "DW_TAG_union_type", "DW_TAG_enumeration_type"):
        return _name(die) or "anon"
    if tag == "DW_TAG_typedef":
        return _name(die) or type_str(_target(die), depth + 1)
    if tag == "DW_TAG_array_type":
        count = ""
        for c in die.iter_children():
            if c.tag == "DW_TAG_subrange_type":
                ub = _val(c, "DW_AT_upper_bound")
                cnt = _val(c, "DW_AT_count")
                if cnt is not None:
                    count = str(cnt)
                elif ub is not None:
                    count = str(ub + 1)
        return type_str(_target(die), depth + 1) + "[" + count + "]"
    name = _name(die)
    if name:
        return name
    if "DW_AT_type" in die.attributes:
        return type_str(_target(die), depth + 1)
    return "void"


def type_of(die, depth=0):
    return type_str(_target(die), depth) if "DW_AT_type" in die.attributes else "void"


structs, functions, globals_ = {}, {}, {}


def member_offset(c):
    off = _val(c, "DW_AT_data_member_location")
    if isinstance(off, list):  # exprloc (rare): give up on this member
        return None
    return off if off is not None else 0


def collect_members(die, base, out):
    """Append data members, flattening anonymous aggregates (e.g. Matrix's
    `union { float m[15]; struct {...} }` -> the array arm at the union's offset)."""
    for c in die.iter_children():
        if c.tag != "DW_TAG_member" or "DW_AT_external" in c.attributes:
            continue
        off = member_offset(c)
        if off is None:
            continue
        name = _name(c)
        tdie = _target(c)
        anon_aggr = (name is None and tdie is not None and _name(tdie) is None
                     and tdie.tag in ("DW_TAG_structure_type", "DW_TAG_class_type",
                                      "DW_TAG_union_type"))
        if anon_aggr and tdie.tag == "DW_TAG_union_type":
            first = next((m for m in tdie.iter_children()
                          if m.tag == "DW_TAG_member"), None)
            if first is not None:
                fo = member_offset(first) or 0
                out.append({"offset": base + off + fo, "type": type_of(first),
                            "name": _name(first) or ("field_0x%x" % (base + off + fo))})
        elif anon_aggr:
            collect_members(tdie, base + off, out)
        else:
            out.append({"offset": base + off, "type": type_of(c),
                        "name": name or ("field_0x%x" % (base + off))})


for obj in sorted(glob.glob(os.path.join(OBJDIR, "**", "*.obj"), recursive=True)):
    with open(obj, "rb") as f:
        elf = ELFFile(f)
        if not elf.has_dwarf_info():
            continue
        dw = elf.get_dwarf_info()
        for cu in dw.iter_CUs():
            for die in cu.iter_DIEs():
                tag = die.tag
                if tag in ("DW_TAG_structure_type", "DW_TAG_class_type"):
                    nm = _name(die)
                    sz = _val(die, "DW_AT_byte_size")
                    if not nm or sz is None or _val(die, "DW_AT_declaration"):
                        continue
                    if nm in structs:
                        continue
                    members = []
                    collect_members(die, 0, members)
                    structs[nm] = {"size": sz, "members": members}
                elif tag == "DW_TAG_subprogram":
                    ln = _val(die, "DW_AT_linkage_name")
                    if ln is None:
                        continue
                    mangled = ln.decode() if isinstance(ln, bytes) else ln
                    if mangled in functions:
                        continue
                    params = []
                    for c in die.iter_children():
                        if c.tag == "DW_TAG_formal_parameter":
                            params.append({"type": type_of(c), "name": _name(c) or ""})
                        elif c.tag == "DW_TAG_unspecified_parameters":
                            params.append({"type": "...", "name": ""})
                    functions[mangled] = {"ret": type_of(die), "params": params}
                elif tag == "DW_TAG_variable":
                    ln = _val(die, "DW_AT_linkage_name")
                    if ln is None:
                        continue
                    mangled = ln.decode() if isinstance(ln, bytes) else ln
                    if mangled in globals_:
                        continue
                    t = type_of(die)
                    if t == "void" and "DW_AT_specification" in die.attributes:
                        # static data member: type lives on the in-class declaration
                        t = type_of(die.get_DIE_from_attribute("DW_AT_specification"))
                    globals_[mangled] = t

with open(OUT, "w") as f:
    json.dump({"structs": structs, "functions": functions, "globals": globals_}, f, indent=1)

m = structs.get("Matrix")
print("structs=%d functions=%d globals=%d -> %s" % (
    len(structs), len(functions), len(globals_), OUT))
print("Matrix size: %s (expect 60 == 0x3c)" % (m["size"] if m else "MISSING"))
