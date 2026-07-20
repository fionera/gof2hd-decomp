#!/usr/bin/env python3
"""Parse GoF2 original-source structure dumps into original_layout.json."""
import json, os, re, sys

DUMP_DIR = "/Users/fionera/Downloads/GalaxyOnFire2/source file list dump"
OUT_DIR = "/private/tmp/claude-502/-Users-fionera-Downloads-GalaxyOnFire2-gof2-decomp/9d0fd300-c199-4045-914b-4133edd83934/scratchpad"

SRC_EXT = re.compile(r'\.(cpp|cxx|cc|c|mm|m|h|hpp|hh|inl)$', re.I)

# ---------------------------------------------------------------- file lists
SECTIONS = [
    "windows:", "mac fhd:", "gof2 hd IOS 1.1.19:", "gof2 sd 1.0.1 ios:",
    "gof3d 1.1.5:", "gof3d 1.1.4:", "gof3d zeebo:",
    "Asserts gof2 sd MeeGo 1.2.7:", "Asserts mac fhd:",
]

def parse_file_lists(path):
    file_lists = {}
    raw_sections = {}
    cur = None
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n").rstrip("\r")
            stripped = line.strip()
            if stripped in SECTIONS:
                cur = stripped[:-1]
                file_lists[cur] = []
                raw_sections[cur] = []
                continue
            if cur is None or not stripped:
                continue
            raw_sections[cur].append(stripped)
            # strip trailing comments like "# missing in 1.1.4"
            entry = stripped
            comment = None
            if "#" in entry:
                entry, comment = entry.split("#", 1)
                entry = entry.strip()
                comment = comment.strip()
            if not entry:
                continue
            is_pathlike = (("/" in entry or "\\" in entry) and
                           (SRC_EXT.search(entry) or entry.endswith("/")))
            # bare filenames like "main.m" don't appear in this file; asserts
            # sections mix in assert strings and signatures -> keep only paths
            if cur.startswith("Asserts"):
                if is_pathlike and not entry.endswith("/"):
                    item = {"path": entry}
                    if comment: item["comment"] = comment
                    file_lists[cur].append(item if comment else entry)
                continue
            if entry.endswith("/") or SRC_EXT.search(entry):
                item = entry if not comment else {"path": entry, "comment": comment}
                file_lists[cur].append(item)
            else:
                # e.g. "...as above" marker in gof3d 1.1.4
                file_lists[cur].append({"note": entry})
    return file_lists, raw_sections

# ---------------------------------------------------------------- symbol dumps
def is_object_line(line):
    return ((("/Objects-normal/" in line) and line.endswith(".o")) or
            ((".a(" in line) and line.endswith(".o)")))

def is_dir_header(line):
    return line.startswith("/") and line.endswith("/")

def is_contributor(line):
    if line.startswith(("_", "-[", "+[")):
        return False
    if "/" not in line:
        return False
    return bool(SRC_EXT.search(line))

PROJECT_ROOTS = [
    "/Users/n.tran/projects/gof2/",
    "/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/",
    "/Users/marchehmeyer/Documents/Dev/GalaxyOnFire/",   # gof3d 1.1.4
    "/Users/marchehmeyer/Documents/Dev/Svn_iPhone_AE2/", # SD engine lib sources
    "/Users/lberger/Documents/Dev/GOF/",
]

def relativize(p):
    for root in PROJECT_ROOTS:
        if p.startswith(root):
            rel = p[len(root):]
            # keep engine-lib provenance visible
            if root.endswith("Svn_iPhone_AE2/"):
                return "Svn_iPhone_AE2/" + rel
            return rel
    return p

def parse_symbol_dump(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        lines = [l.rstrip("\n").rstrip("\r") for l in f]
    obj_idxs = [i for i, l in enumerate(lines) if is_object_line(l)]
    tus = {}
    order = []
    carried_dir = None
    for k, oi in enumerate(obj_idxs):
        tu_name_idx = oi - 1
        tu_name = lines[tu_name_idx]
        # directory header immediately before the TU name line?
        dir_ctx = lines[tu_name_idx - 1] if tu_name_idx >= 1 and is_dir_header(lines[tu_name_idx - 1]) else None
        if dir_ctx is not None:
            carried_dir = dir_ctx
        end = obj_idxs[k + 1] - 1 if k + 1 < len(obj_idxs) else len(lines)
        # the next TU's name line and possible dir header must be excluded
        block_end = end
        if k + 1 < len(obj_idxs):
            block_end = obj_idxs[k + 1] - 1  # exclude next TU name line
        block = lines[oi + 1: block_end]
        symbols, contributors = [], []
        for l in block:
            if not l.strip():
                continue
            if is_dir_header(l):
                continue
            if is_contributor(l):
                contributors.append(l)
            else:
                symbols.append(l)
        # resolve TU path
        base = os.path.basename(tu_name)
        tu_path = None
        own = None
        for c in contributors:
            if os.path.basename(c) == base:
                own = c
                break
        if tu_name.startswith("/"):
            tu_path = relativize(tu_name)
        elif own is not None:
            tu_path = relativize(own)
        elif carried_dir is not None:
            tu_path = relativize(carried_dir) + tu_name
        else:
            tu_path = tu_name
        headers_seen = []
        for c in contributors:
            if c == own:
                continue
            rc = relativize(c)
            if rc == tu_path:
                continue
            if rc not in headers_seen:
                headers_seen.append(rc)
        obj_line = lines[oi]
        if tu_path in tus:  # shouldn't happen, but be safe
            tu_path = tu_path + f"#dup{k}"
        tus[tu_path] = {
            "object": os.path.basename(obj_line),
            "symbols": symbols,
            "headers_seen": headers_seen,
        }
        order.append(tu_path)
    return tus

def main():
    fl_path = os.path.join(DUMP_DIR, "source file lists dump.txt")
    file_lists, raw_sections = parse_file_lists(fl_path)

    tu_symbols = {}
    for dump in ("gof2hd_1.1.19_ios", "gof2sd_1.0.1_ios", "gof3d_1.1.4_ios"):
        tu_symbols[dump] = parse_symbol_dump(os.path.join(DUMP_DIR, dump + ".cpp"))

    out = {
        "file_lists": file_lists,
        "tu_symbols": tu_symbols,
        "asserts_raw": {k: v for k, v in raw_sections.items() if k.startswith("Asserts")},
    }
    out_path = os.path.join(OUT_DIR, "original_layout.json")
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(out, f, indent=1, ensure_ascii=False)

    # stats
    for dump, tus in tu_symbols.items():
        nsym = sum(len(t["symbols"]) for t in tus.values())
        nhdr = sum(len(t["headers_seen"]) for t in tus.values())
        print(f"{dump}: {len(tus)} TUs, {nsym} symbols, {nhdr} header/contributor refs")
    for sec, entries in file_lists.items():
        print(f"file_list[{sec}]: {len(entries)} entries")
    print("wrote", out_path)

if __name__ == "__main__":
    main()
