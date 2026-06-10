#!/usr/bin/env python3
# merge_one.py <Class> [<Class> ...] — consolidate a class's per-method .cpp files into ONE
# merged/src/<Class>.cpp + copy its header to merged/include/<Class>.h. Pure file consolidation
# (no struct/offset changes): hoist+dedupe system #includes and `extern "C" ...;` prototypes,
# strip per-file local-header includes, concatenate bodies.
import json, os, re, sys
ROOT="/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"; os.chdir(ROOT)
os.makedirs("merged/src",exist_ok=True); os.makedirs("merged/include",exist_ok=True)
CM=json.load(open("native/classmap.json"))

def merge(cls):
    cpps=CM.get(cls,[])
    if not cpps: return f"{cls}: NO FILES"
    # header: the class.h/quaternion.h beside the .cpp files
    hdr=None
    for cp in cpps:
        d=os.path.dirname(cp)
        for hn in ("class.h","quaternion.h"):
            p=os.path.join(d,hn)
            if os.path.exists(p): hdr=p; break
        if hdr: break
    if hdr:
        open(f"merged/include/{cls}.h","w").write(open(hdr,errors='ignore').read())
        hinc=f'#include "{cls}.h"'
    else:
        hinc=f'// (no header found for {cls})'
    sysinc=set(); externs=[]; ext_seen=set(); bodies=[]
    for cp in cpps:
        out=[]
        for ln in open(cp,errors='ignore').read().splitlines():
            s=ln.strip()
            if re.match(r'#\s*include\s+"[^"]+\.h"', s): continue          # strip local headers
            if re.match(r'#\s*include\s+<[^>]+>', s): sysinc.add(s); continue
            if re.match(r'extern\s+"C"\s+[^={]*;\s*$', s):                  # hoist+dedupe prototypes
                if s not in ext_seen: ext_seen.add(s); externs.append(s)
                continue
            out.append(ln)
        bodies.append(f"// ---- {os.path.basename(cp)} ----\n"+"\n".join(out).strip())
    txt=hinc+"\n"+"\n".join(sorted(sysinc))+"\n\n"+"\n".join(externs)+"\n\n"+"\n\n".join(bodies)+"\n"
    open(f"merged/src/{cls}.cpp","w").write(txt)
    return f"{cls}: {len(cpps)} files -> merged/src/{cls}.cpp"

if __name__=="__main__":
    for c in sys.argv[1:]: print(merge(c))
