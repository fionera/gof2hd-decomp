#!/usr/bin/env python3
"""rename_locs.py <file.cpp>... -- rename shader location fields after their GLSL names.

For every `this->FIELD = glGet{Attrib,Uniform}Location(prog, "glsl_name")` (scalar fields only) the
field is renamed in the .cpp and its .h to a name derived from the GLSL identifier:
a_position -> aPositionLoc, u_lightdirmodel[1] -> uLightdirmodel1Loc, s_texture_base ->
sTextureBaseLoc, glColor -> uGlColorLoc.  Fields assigned from more than one name, and derived names
that would collide with another member, are left alone (reported)."""
import re, sys
from pathlib import Path

ASSIGN = re.compile(r'(?:this->)?\b([A-Za-z_]\w*) = glGet(Attrib|Uniform)Location\([^,]+, "([^"]+)"\)')


def derive(glsl, kind):
    m = re.match(r'([aus])_(.*)', glsl)
    if m:
        pre, rest = m.group(1), m.group(2)
    else:
        pre, rest = ('a' if kind == 'Attrib' else 'u'), glsl
    rest = rest.replace('[', '_').replace(']', '')
    parts = [p for p in rest.split('_') if p]
    return pre + ''.join(p[0].upper() + p[1:] for p in parts) + 'Loc'


for cpp in sys.argv[1:]:
    cpp = Path(cpp)
    h = cpp.with_suffix('.h')
    src = cpp.read_text()
    hdr = h.read_text()
    seen = {}
    for f, kind, glsl in ASSIGN.findall(src):
        seen.setdefault(f, set()).add(derive(glsl, kind))
    ren = {}
    members = set(re.findall(r'\b(\w+);', hdr))
    for f, names in seen.items():
        if len(names) != 1:
            print(f'{cpp.name}: skip {f} -> {names}')
            continue
        n = names.pop()
        if n == f:
            continue
        if n in members and n not in seen:
            print(f'{cpp.name}: collide {f} -> {n}')
            continue
        ren[f] = n
    if len(set(ren.values())) != len(ren):
        print(f'{cpp.name}: duplicate targets, skipped'); continue
    for path, text in ((cpp, src), (h, hdr)):
        # split into string literals / code; rename identifiers in code chunks only
        chunks = re.split(r'("(?:[^"\\\n]|\\.)*")', text)
        for k in range(0, len(chunks), 2):
            c = chunks[k]
            for i, a in enumerate(ren):
                c = re.sub(r'\b%s\b' % re.escape(a), f'@@{i}@@', c)
            for i, b in enumerate(ren.values()):
                c = c.replace(f'@@{i}@@', b)
            chunks[k] = c
        path.write_text(''.join(chunks))
    print(f'{cpp.name}: renamed {len(ren)}')
