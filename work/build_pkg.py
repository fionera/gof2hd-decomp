#!/usr/bin/env python3
# build_pkg.py — assemble a per-class decomp package (methods.tsv) for codex workers, by joining
# the demangled Android symbol table (elf addr + name) with candidates.tsv (body_bytes).
# Work-items (.md) are then filled in by the Ghidra batch extractor. Run from repo root.
import sys, os, re

SYM = "../_work/symbols/android_2.0.16.symbols.tsv"
CAND = "work/candidates.tsv"

# elf_addr(hex) -> body_bytes  (ghidra_addr = elf + 0x10000)
size_by_elf = {}
for ln in open(CAND):
    p = ln.rstrip("\n").split("\t")
    if len(p) < 3 or not re.fullmatch(r"[0-9a-fA-F]+", p[1]):
        continue
    try:
        gh = int(p[1], 16); bb = int(p[2])
    except ValueError:
        continue
    size_by_elf[gh - 0x10000] = bb

def method_of(dem):
    base = dem.split("(", 1)[0]          # drop arg list
    base = base.rstrip()
    return base.rsplit("::", 1)[-1] if "::" in base else base

def build(prefix, clsdir):
    # `prefix` is the class name; match where the owning class (component before the method) == it.
    cls_name = prefix
    seen = set(); rows = []
    for ln in open(SYM):
        p = ln.rstrip("\n").split("\t")
        if len(p) < 3: continue
        addr, mangled, dem = p[0], p[1], p[2]
        base = dem.split("(", 1)[0]
        parts = base.split("::")
        if len(parts) < 2 or parts[-2] != cls_name: continue   # owning class must match exactly
        try: elf = int(addr, 16)
        except ValueError: continue
        if elf in seen: continue
        seen.add(elf)
        meth = method_of(dem)
        n = size_by_elf.get(elf)
        if not n: continue                # no body size -> skip (likely thunk/alias)
        key = "%s_%x" % (re.sub(r'[^A-Za-z0-9_]', '_', meth), elf)
        rows.append((key, meth, "%x" % elf, n, dem))
    rows.sort(key=lambda r: r[3])         # smallest first (accessors -> momentum)
    os.makedirs(clsdir + "/src", exist_ok=True)
    with open(clsdir + "/methods.tsv", "w") as f:
        for key, meth, vaddr, n, dem in rows:
            f.write("%s\t%s\t%s\t%s\n" % (key, meth, vaddr, n))
    with open(clsdir + "/manifest.tsv", "w") as f:
        for key, meth, vaddr, n, dem in rows:
            f.write("%s\t%s\t%s\t%s\t%s\n" % (key, meth, vaddr, n, dem))
    return len(rows)

if __name__ == "__main__":
    # argv: Class names (under AbyssEngine::). Default = the next high-value batch.
    classes = sys.argv[1:] or ["Engine", "AESoundRessource", "AESound"]
    for cls in classes:
        d = "work/classes/%s" % cls
        n = build(cls, d)
        print("  %-24s -> %3d methods  (%s/methods.tsv)" % (cls, n, d))
