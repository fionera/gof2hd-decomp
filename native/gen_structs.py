#!/usr/bin/env python3
# gen_structs.py — build the CANONICAL, cross-class-consistent struct contract for the native build.
# Phase-1 prep for the 20-agent cleanup: scan every merged/src/*.cpp, attribute each byte-offset
# access to a class (self/this -> owning class; typed var name -> that class), vote a type per
# (class, offset), and emit:
#   * native/fieldmap.json            {Class: {"0xNN": "type", ...}}  (the shared contract)
#   * native/include/gof2/<Class>.h   real struct (named fields) + #include "gof2/common.h"
# Classes that already have a proper named struct (the byte-match recovery) keep it (shared-type
# redefs stripped). Opaque classes get `struct C { <type> field_0xNN; ... };` from the field map.
import os, re, json, glob, collections
ROOT="/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"; os.chdir(ROOT)
CM=json.load(open("native/classmap.json"))
CLASSES=sorted(CM.keys())
SHARED={"Vector","Matrix","Quaternion","String","String12","Array"}
name2class={c.lower():c for c in CLASSES}          # engine->Engine, layout->Layout, mesh->Mesh ...
OUT_H="native/include/gof2"; os.makedirs(OUT_H,exist_ok=True)

# ---- access patterns -------------------------------------------------------------------------
RAW = re.compile(r'\*\(\s*([A-Za-z_][\w\s:]*?\**)\s*\)\s*\(\s*\(\s*char\s*\*\s*\)\s*'
                 r'([A-Za-z_]\w*)\s*(?:\+\s*(0x[0-9a-fA-F]+|\d+))?\s*\)')
HLP = re.compile(r'\b([FG])\s*<\s*([^>]+?)\s*>\s*\(\s*([A-Za-z_]\w*)\s*,\s*(0x[0-9a-fA-F]+|\d+)\s*\)')

def norm_type(t):
    t=re.sub(r'\s+',' ',t.strip())
    t=t.replace(' *','*').replace('* ','*')
    return t or 'int'

def base_class(var, owner):
    # only self/this are reliably the owning class; class-named vars (engine->Engine) map by name.
    # ambiguous locals (t, pPVar12, ...) are left to the Phase-2 agents to type from context.
    if var in ('self','this'): return owner
    return name2class.get(var.lower())

# ---- collect (class, offset) -> type votes ---------------------------------------------------
votes=collections.defaultdict(collections.Counter)   # (cls, off) -> Counter(type)
for cls in CLASSES:
    cp=f"merged/src/{cls}.cpp"
    if not os.path.exists(cp): continue
    txt=open(cp,errors='ignore').read()
    for m in RAW.finditer(txt):
        # cast is `(FIELDTYPE *)` — strip exactly one trailing '*' (the cast pointer) to get the field type
        raw=norm_type(m.group(1))
        typ=norm_type(raw[:-1]) if raw.endswith('*') else raw
        var,off=m.group(2),(m.group(3) or '0')
        tc=base_class(var,cls)
        if tc: votes[(tc,int(off,16) if off.startswith('0x') else int(off))][typ]+=1
    for m in HLP.finditer(txt):
        typ,var,off=norm_type(m.group(2)),m.group(3),m.group(4)
        tc=base_class(var,cls)
        if tc: votes[(tc,int(off,16) if off.startswith('0x') else int(off))][typ]+=1

fieldmap=collections.defaultdict(dict)
for (cls,off),ctr in votes.items():
    fieldmap[cls][off]=ctr.most_common(1)[0][0]
json.dump({c:{hex(o):t for o,t in sorted(d.items())} for c,d in fieldmap.items()},
          open("native/fieldmap.json","w"), indent=0)

# ---- header generation -----------------------------------------------------------------------
def find_blocks(text):
    for m in re.finditer(r'(template\s*<[^>]*>\s*)?\b(struct|class|union|enum(?:\s+class)?)\s+([A-Za-z_]\w*)\s*(?=[:{])', text):
        b=text.find('{',m.end())
        if b<0: continue
        depth=0;i=b
        while i<len(text):
            if text[i]=='{':depth+=1
            elif text[i]=='}':
                depth-=1
                if depth==0:
                    e=text.find(';',i); yield(m.group(3),m.start(),(e+1 if e>=0 else i+1)); break
            i+=1

def has_named_struct(cls,htext):
    mblk=[b for n,s,e in find_blocks(htext) if n==cls for b in [htext[s:e]]]
    if not mblk: return None
    blk=mblk[0]
    # "named" = has >=2 real data members that aren't F<>/G<> helpers
    members=re.findall(r'\b(?:int|float|bool|char|short|double|long|unsigned|void|[A-Z]\w+)[\s\*]+([a-z]\w*)\s*;', blk)
    return blk if len(members)>=2 else None

def emit_struct_from_fieldmap(cls):
    fields=fieldmap.get(cls,{})
    if not fields:
        return f"struct {cls} {{ void* _opaque; }};  // no offset accesses observed"
    lines=[f"struct {cls} {{"]
    for off in sorted(fields):
        t=fieldmap[cls][off]
        lines.append(f"    {t} field_{hex(off)};".ljust(40)+f"// +{hex(off)}")
    lines.append("};")
    return "\n".join(lines)

for cls in CLASSES:
    hp=f"merged/include/{cls}.h"
    htext=open(hp,errors='ignore').read() if os.path.exists(hp) else ""
    guard=f"GOF2_{re.sub(r'%W','_',cls).upper()}_H"
    kept=has_named_struct(cls,htext)
    if kept:
        body=kept  # keep the recovered/real struct verbatim
        note="// real struct kept from byte-match recovery"
    else:
        body=emit_struct_from_fieldmap(cls)
        note="// struct derived from offset-access field map (deterministic field_0xNN naming)"
    open(f"{OUT_H}/{cls}.h","w").write(
        f"#ifndef {guard}\n#define {guard}\n#include \"gof2/common.h\"\n{note}\n{body}\n#endif\n")

print(f"fieldmap: {len(fieldmap)} classes, {sum(len(d) for d in fieldmap.values())} fields")
print(f"headers : {len(CLASSES)} written to native/include/gof2/")
print(f"  kept real struct: {sum(1 for c in CLASSES if has_named_struct(c, open(f'merged/include/{c}.h',errors='ignore').read() if os.path.exists(f'merged/include/{c}.h') else ''))}")
