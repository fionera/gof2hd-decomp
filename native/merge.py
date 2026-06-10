#!/usr/bin/env python3
# merge.py — consolidate the per-method decomp into one header + one .cpp per class for the native
# (macOS) build: pull shared types from gof2/common.h, keep each class's own struct(s), merge all
# its method .cpp into native/src/<Class>.cpp with named-field access (no re-defined shared types).
import os, re, sys, glob, json

ROOT = "/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"
os.chdir(ROOT)
SHARED = {"Vector","Matrix","Quaternion","String","String12","Array"}  # provided by common.h
OUT_H = "native/include/gof2"; OUT_C = "native/src"
os.makedirs(OUT_H, exist_ok=True); os.makedirs(OUT_C, exist_ok=True)

def find_blocks(text):
    """Yield (kind, name, start, end) for top-level struct/class/enum/union/namespace defs."""
    for m in re.finditer(r'(template\s*<[^>]*>\s*)?\b(struct|class|union|enum(?:\s+class)?)\s+([A-Za-z_]\w*)', text):
        name = m.group(3)
        b = text.find('{', m.end())
        if b < 0: continue
        depth = 0; i = b
        while i < len(text):
            if text[i] == '{': depth += 1
            elif text[i] == '}':
                depth -= 1
                if depth == 0:
                    e = text.find(';', i)
                    yield (m.group(2), name, m.start(), (e+1 if e>=0 else i+1)); break
            i += 1

def class_header(cls, hdr_paths):
    """Build native/include/gof2/<cls>.h: include common.h, keep non-shared struct defs."""
    seen_names=set(); kept=[]; ns=False
    for hp in hdr_paths:
        if not os.path.exists(hp): continue
        t=open(hp,errors='ignore').read()
        if 'namespace AbyssEngine' in t: ns=True
        for kind,name,s,e in find_blocks(t):
            if name in SHARED or name in seen_names: continue
            blk=t[s:e]
            # skip if it's just a shared type re-decl or a forward decl we don't need
            seen_names.add(name); kept.append(blk)
    guard="GOF2_%s_H"%re.sub(r'\W','_',cls).upper()
    body="\n\n".join(kept)
    wrap = ("namespace AbyssEngine {\n%s\n}\n"%body) if ns and 'AbyssEngine::' not in body else body
    return f"""#ifndef {guard}
#define {guard}
#include "gof2/common.h"
{wrap}
#endif
"""

def merge_cpp(cls, cpp_paths, hdr_name):
    extern_lines=set(); bodies=[]
    for cp in cpp_paths:
        t=open(cp,errors='ignore').read()
        out=[]
        for ln in t.splitlines():
            s=ln.strip()
            if re.match(r'#\s*include\s+"(class\.h|quaternion\.h|[A-Za-z_]+\.h)"', s): continue
            if s.startswith('#include'):  # keep system includes once
                extern_lines.add(s); continue
            # dedupe top-level extern "C" single-line declarations (prototypes)
            if re.match(r'extern\s+"C"\s+[^{]*;\s*$', s):
                extern_lines.add(s); continue
            out.append(ln)
        bodies.append("// ---- %s ----\n%s"%(os.path.basename(cp), "\n".join(out).strip()))
    incs="\n".join(sorted(x for x in extern_lines if x.startswith('#include')))
    externs="\n".join(sorted(x for x in extern_lines if x.startswith('extern')))
    return f'#include "gof2/{hdr_name}"\n{incs}\n\n{externs}\n\n' + "\n\n".join(bodies) + "\n"

def main():
    classmap=json.load(open("native/classmap.json"))
    targets = sys.argv[1:] or sorted(classmap.keys())
    done=0
    for cls in targets:
        cpps = classmap.get(cls, [])
        if not cpps: continue
        hdrs=[]
        for cp in cpps:
            d=os.path.dirname(cp)
            for hn in ("class.h","quaternion.h"):
                if os.path.exists(os.path.join(d,hn)): hdrs.append(os.path.join(d,hn))
        hdrs=list(dict.fromkeys(hdrs))
        open(f"{OUT_H}/{cls}.h","w").write(class_header(cls,hdrs))
        open(f"{OUT_C}/{cls}.cpp","w").write(merge_cpp(cls,cpps,f"{cls}.h"))
        done+=1
    print("merged %d classes -> native/{include/gof2,src}/"%done)

if __name__=="__main__": main()
