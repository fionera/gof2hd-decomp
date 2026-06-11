#!/usr/bin/env python3
# dedup_externs.py — the decomp scattered identical-purpose extern decls (GL ES functions, the stack
# canary __stack_chk_guard/__stack_chk_fail, malloc/free) across many per-class headers with DIFFERENT
# signatures; the methodize cross-class includes now collide them. Declare GL + the canary ONCE in
# common.h (canonical form) and remove every per-header copy. malloc/free come from <cstdlib> -> just drop.
import glob, re, os, collections
ROOT="/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"; os.chdir(ROOT)
FOUND={"common.h","math.h","fwd.h"}
# a decl line for one of these artifact symbols
LINE=re.compile(r'^[ \t]*(?:extern\s+(?:"C"\s+)?)?[^\n;{]*?\b(gl[A-Z]\w*|__stack_chk_guard|__stack_chk_fail|malloc|free)\b[^\n;{]*;[ \t]*\n', re.M)
DROP={"malloc","free"}                       # provided by <cstdlib>
gl=collections.OrderedDict()                 # gl symbol -> canonical decl (first seen)
for h in glob.glob("include/gof2/*.h"):
    if os.path.basename(h) in FOUND: continue
    for m in LINE.finditer(open(h,errors='ignore').read()):
        s=m.group(1)
        if s.startswith('gl') and s not in gl:
            d=m.group(0).strip()
            if not d.startswith('extern'): d='extern "C" '+d
            gl[s]=d
# strip these artifact decls from every per-class header AND every .cpp (they come from common.h now)
for h in glob.glob("include/gof2/*.h")+glob.glob("src/*.cpp"):
    if os.path.basename(h) in FOUND: continue
    t=open(h,errors='ignore').read()
    t2=LINE.sub('', t)
    if t2!=t: open(h,'w').write(t2)
# add the canonical block to common.h once
c=open("include/gof2/common.h").read()
if "decomp artifact externs" not in c:
    block=('\n// ---- decomp artifact externs (GL ES + stack canary; were duplicated per header) ----\n'
           'extern "C" void *__stack_chk_guard;\n'
           'extern "C" __attribute__((noreturn)) void __stack_chk_fail(...) noexcept;\n'
           + "\n".join(gl.values()) + "\n")
    c=c.replace("#endif // GOF2_COMMON_H", block+"\n#endif // GOF2_COMMON_H")
    open("include/gof2/common.h","w").write(c)
print(f"centralized {len(gl)} GL functions + stack canary into common.h; stripped per-header copies")
