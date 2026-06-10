#!/usr/bin/env python3
# methodize_apply.py [--only Class1,Class2] — convert `RetType Class_method(Class *self, ...)` free
# functions into C++ instance methods on the struct. Global, single consistent pass:
#   * definition:  [extern "C"] R Class_method(Class *self, rest){body}  ->  R Class::method(rest){ Class *self=this; body }
#   * prototype:   [extern "C"] R Class_method(...);                      ->  (removed)
#   * call site:   Class_method(arg0, rest)  ->  (arg0)->method(rest)     [everywhere]
#   * header:      add `R method(rest);` inside struct Class
#   * includes:    add #include "gof2/Other.h" to files that now call Other's methods
# &Class_method (address-of, function-pointer use) is LEFT in place and reported (member-fn-ptr type
# differs; handled by follow-up compile-fix).
import glob, re, os, json, sys, collections
ROOT="/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"; os.chdir(ROOT)
REG=json.load(open("tools/method_registry.json"))
only=None
if "--only" in sys.argv: only=set(sys.argv[sys.argv.index("--only")+1].split(","))
SYMS={s:v for s,v in REG.items() if (only is None or v["class"] in only)}
BY_CLASS=collections.defaultdict(list)
for s,v in SYMS.items(): BY_CLASS[v["class"]].append(v)

SIG=re.compile(r'(?P<full>(?P<ext>extern\s+"C"\s+)?'
               r'(?P<head>[A-Za-z_][\w:\s]*?[\w>]\s*[\*&]*\s*)'
               r'(?P<sym>[A-Za-z_]\w*?_[A-Za-z_]\w*)\s*'
               r'\((?P<params>[^;{]*?)\)\s*)(?P<term>[{;])')

def convert_defs_decls(txt, report):
    out=[]; i=0
    for m in SIG.finditer(txt):
        sym=m.group('sym')
        if sym not in SYMS: continue
        v=SYMS[sym]; term=m.group('term')
        out.append(txt[i:m.start()])
        if term=='{':
            params=", ".join(v["params_after_self"])
            out.append(f'{v["ret"]} {v["class"]}::{v["method"]}({params}) {{\n    {v["class"]} *{v["self_name"]} = this;')
            report["defs"]+=1
            i=m.end()        # consumed through the '{'
        else:                # prototype -> drop entirely
            report["decls"]+=1
            i=m.end()
            while i<len(txt) and txt[i] in ' \t': i+=1
            if i<len(txt) and txt[i]=='\n': i+=1   # swallow trailing newline of the removed proto
    out.append(txt[i:])
    return "".join(out)

CALL=re.compile(r'(?P<pre>.?)\b(?P<sym>[A-Za-z_]\w*?_[A-Za-z_]\w*)\s*\(')
def rewrite_calls(txt, report, called):
    out=[]; i=0
    for m in CALL.finditer(txt):
        sym=m.group('sym')
        if sym not in SYMS: continue
        if m.group('pre')=='&':           # address-of: leave for manual fix
            report["addrof"]+=1; continue
        # find matching ')'
        j=m.end(); depth=1
        while j<len(txt) and depth:
            c=txt[j]
            if c=='(': depth+=1
            elif c==')': depth-=1
            j+=1
        inner=txt[m.end():j-1]
        # split first top-level comma
        depth=0; k=-1
        for idx,c in enumerate(inner):
            if c in '([{': depth+=1
            elif c in ')]}': depth-=1
            elif c==',' and depth==0: k=idx; break
        arg0=(inner if k<0 else inner[:k]).strip()
        rest=('' if k<0 else inner[k+1:].strip())
        v=SYMS[sym]
        out.append(txt[i:m.start()+ (1 if m.group('pre') else 0)])  # keep the pre char
        if m.group('pre') and m.group('pre')!='&':
            pass
        out.append(f'({arg0})->{v["method"]}({rest})')
        called.add(v["class"])
        report["calls"]+=1
        i=j
    out.append(txt[i:])
    return "".join(out)

def add_includes(txt, called, own):
    need=[c for c in sorted(called) if c!=own and f'"gof2/{c}.h"' not in txt]
    if not need: return txt
    blk="".join(f'#include "gof2/{c}.h"\n' for c in need)
    m=re.search(r'^#include "gof2/[^"]+\.h"\n', txt, re.M)
    if m: return txt[:m.end()]+blk+txt[m.end():]
    return blk+txt

def update_header(cls):
    hp=f"include/gof2/{cls}.h"
    if not os.path.exists(hp): return 0
    t=open(hp).read()
    m=re.search(r'struct\s+'+re.escape(cls)+r'\b[^{]*\{', t)
    if not m: return 0
    # find matching closing '};'
    depth=0; i=m.end()-1
    while i<len(t):
        if t[i]=='{': depth+=1
        elif t[i]=='}':
            depth-=1
            if depth==0: break
        i+=1
    close=i  # index of '}'
    decls=[]
    for v in sorted(BY_CLASS[cls], key=lambda x:x["method"]):
        d=f'    {v["ret"]} {v["method"]}({", ".join(v["params_after_self"])});'
        if re.search(r'\b%s\s*\('%re.escape(v["method"]), t): continue   # already declared
        decls.append(d)
    if not decls: return 0
    ins="\n    // ---- methods (converted from free functions) ----\n"+"\n".join(decls)+"\n"
    open(hp,"w").write(t[:close]+ins+t[close:])
    return len(decls)

def main():
    rep=collections.Counter(); files=glob.glob("src/*.cpp")
    for f in files:
        own=os.path.basename(f)[:-4]
        txt=open(f,errors='ignore').read()
        txt=convert_defs_decls(txt, rep)
        called=set()
        txt=rewrite_calls(txt, rep, called)
        txt=add_includes(txt, called, own)
        open(f,"w").write(txt)
    hdr=sum(update_header(c) for c in BY_CLASS)
    print(f"defs->methods: {rep['defs']}  protos removed: {rep['decls']}  calls rewritten: {rep['calls']}  "
          f"header decls added: {hdr}  &addr-of left: {rep['addrof']}")

if __name__=="__main__": main()
