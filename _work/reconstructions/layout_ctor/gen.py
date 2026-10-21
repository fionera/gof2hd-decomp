#!/usr/bin/env python3
"""gen.py -- emit the Layout::Layout() body from the emulated store table of the original.

Inputs: lay_orig.json (table.py output: per flag combination, the bytes the original ctor
stores into `this`), the field-name dump of Layout.h (offset name lines) and the store order
of the default (all flags 0) path, which follows source order.  Every 4-byte field becomes one
assignment whose right-hand side is the shortest if/else-if chain over the five device flags
that reproduces the original value for all 32 flag combinations (chain.py)."""
import json, sys, struct, itertools, os
d = json.load(open(sys.argv[1]))
names = {}
for l in open(sys.argv[2]):
    p = l.split()
    if len(p) >= 3 and p[0].startswith('0x'):
        names[int(p[0], 16)] = (p[1], p[-1])
order = [int(x, 16) for x in open(sys.argv[3]).read().split()]
style = sys.argv[4] if len(sys.argv) > 4 else 'chain'
F = ['iPadHD', 'n9', 'iPad', 'iPadLarge', 'retinaDisplay']
combos = sorted(d)
FLOATS = {0x90, 0xdc, 0xe0, 0xe4, 0xe8, 0x1e0, 0x2f8}
BYTES = {0x284, 0x285, 0x286}
BLK = {0x238: 'field_0x238_blk16.a', 0x23c: 'field_0x238_blk16.b', 0x240: 'field_0x238_blk16.c', 0x244: 'field_0x238_blk16.d'}

def val(c, w, n):
    st = d[c]
    if not all(str(w + j) in st for j in range(n)):
        return None
    return sum(st[str(w + j)] << (8 * j) for j in range(n))

def lit(w, v):
    if v is None:
        return None
    if w in FLOATS:
        f = struct.unpack('<f', struct.pack('<I', v))[0]
        s = repr(f)
        if 'e' not in s and '.' not in s: s += '.0'
        return s + 'f'
    if w in BYTES:
        return str(v)
    return str(struct.unpack('<i', struct.pack('<I', v))[0])

def chain(w, n):
    best = None
    for perm in itertools.permutations([0, 1, 3, 4, 2]):
        rem = list(combos); out = []; ok = True
        for vi in perm:
            if len({val(c, w, n) for c in rem}) <= 1: break
            sel = [c for c in rem if c[vi] == '1']
            vs = {val(c, w, n) for c in sel}
            if len(sel) == 0: continue
            if len(vs) == 1:
                out.append((F[vi], vs.pop())); rem = [c for c in rem if c[vi] == '0']
            else:
                ok = False; break
        if not ok: continue
        vs = {val(c, w, n) for c in rem}
        if len(vs) != 1: continue
        if best is None or len(out) < len(best[0]): best = (out, vs.pop())
    return best

def tree(combos_, w, n, vars_):
    vals = {val(c, w, n) for c in combos_}
    if len(vals) == 1: return lit(w, vals.pop())
    v = vars_[0]; i = F.index(v)
    a = tree([c for c in combos_ if c[i] == '1'], w, n, vars_[1:])
    b = tree([c for c in combos_ if c[i] == '0'], w, n, vars_[1:])
    if a == b: return a
    return '(Globals::%s ? %s : %s)' % (v, a, b)

def fname(w):
    if w in BLK: return BLK[w]
    return names[w][1]

def stmt(w, flags_order=None, drop=()):
    n = 1 if w in BYTES else 4
    sub = [c for c in combos if all(c[F.index(v)] == '0' for v in drop)]
    target = 'this->' + fname(w)
    c = chain_sub(w, n, sub)
    if c is None:
        return '%s = %s;' % (target, tree(sub, w, n, [v for v in ['iPadHD', 'n9', 'iPadLarge', 'iPad', 'retinaDisplay'] if v not in drop]))
    conds, last = c
    if last is None:
        if not conds:
            return None
        cond = ' || '.join('Globals::%s' % f for f, v in conds)
        vals = {v for f, v in conds}
        if len(vals) == 1:
            return 'if (%s)\n        %s = %s;' % (cond, target, lit(w, vals.pop()))
        s = ''
        for k, (f, v) in enumerate(conds):
            s += ('if' if k == 0 else ' else if') + ' (Globals::%s)\n        %s = %s;' % (f, target, lit(w, v))
        return s
    expr = lit(w, last)
    first = True
    for f, v in reversed(conds):
        expr = ('Globals::%s ? %s : %s' if first else 'Globals::%s ? %s : (%s)') % (f, lit(w, v), expr)
        first = False
    return '%s = %s;' % (target, expr)

def chain_sub(w, n, sub):
    best = None
    for perm in itertools.permutations([0, 1, 3, 4, 2]):
        rem = list(sub); out = []; ok = True
        for vi in perm:
            if len({val(c, w, n) for c in rem}) <= 1: break
            sel = [c for c in rem if c[vi] == '1']
            vs = {val(c, w, n) for c in sel}
            if len(sel) == 0: continue
            if len(vs) == 1:
                out.append((F[vi], vs.pop())); rem = [c for c in rem if c[vi] == '0']
            else:
                ok = False; break
        if not ok: continue
        vs = {val(c, w, n) for c in rem}
        if len(vs) != 1: continue
        if best is None or len(out) < len(best[0]): best = (out, vs.pop())
    return best

def ind(s, k):
    return '\n'.join(('    ' * k + l if l.strip() else l) for l in s.split('\n'))

out = []
if style == 'chain':
    for w in order:
        s = stmt(w)
        if s: out.append(ind(s, 1))
elif style.startswith('hdblock'):
    hdset = set(int(x, 16) for x in open(sys.argv[5]).read().split())
    hdc = [c for c in combos if c[0] == '1']
    out.append('    if (Globals::iPadHD) {')
    for w in (sorted(order) if 'sorted' in style else order):
        if w in hdset:
            n = 1 if w in BYTES else 4
            vs = {val(c, w, n) for c in hdc}
            if len(vs) == 1:
                out.append('        this->%s = %s;' % (fname(w), lit(w, vs.pop())))
            else:
                s = stmt(w)
                out.append(ind(s, 2))
    out.append('    } else {')
    order2 = [int(x, 16) for x in open(sys.argv[6]).read().split()] if len(sys.argv) > 6 else order
    n9set = set()
    if 'n9g1' in style:
        n9set = set(int(x, 16) for x in open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'n9_region.txt')).read().split())
        n9c = [c for c in combos if c[0] == '0' and c[1] == '1']
        out.append('        if (Globals::n9) {')
        for w in order2:
            if w in hdset and w in n9set:
                n = 1 if w in BYTES else 4
                vs = {val(c, w, n) for c in n9c}
                if len(vs) == 1:
                    out.append('            this->%s = %s;' % (fname(w), lit(w, vs.pop())))
                else:
                    out.append(ind(stmt(w, drop=('iPadHD',)), 3))
        out.append('        } else {')
        for w in order2:
            if w in hdset and w in n9set:
                s = stmt(w, drop=('iPadHD', 'n9'))
                if s: out.append(ind(s, 3))
        out.append('        }')
    for w in (sorted(order2) if 'sortall' in style else order2):
        if w in hdset and w not in n9set:
            s = stmt(w, drop=('iPadHD',))
            if s: out.append(ind(s, 2))
    out.append('    }')
    for w in order2:
        if w not in hdset:
            s = stmt(w)
            if s: out.append(ind(s, 1))
print('Layout::Layout() {')
print('\n'.join(out))
print('}')
