#!/usr/bin/env python3
"""Sub-expression / existing-expect __builtin_expect sweep for LevelScript::process.

Wave 176 / w12004.  Complements levelscript-process-expect-sweep-w11901.py
(which wraps whole conditions): this harness wraps operands of `&&`/`||`
(including operands nested inside an existing __builtin_expect call and inside
`!(...)`), and flips/strips every existing __builtin_expect site.

Usage:
  python3 levelscript-process-subs-sweep-w12004.py [--mode subs|flips] [--limit N]

Writes cells.tsv in the current directory.  Each cell is built with the real
gold link (`ninja -C cmake-build-match libgof2hdaa.so -j2`) and measured from
the candidate ELF: raw same-address diff, function size, whole-.text diff and
the .rel.plt md5.  The source is restored on exit.
"""
import os, re, sys, json, time, struct, hashlib, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(ROOT, 'src/game/world/LevelScript.cpp')
BUILD = os.path.join(ROOT, 'cmake-build-match')
SO = os.path.join(BUILD, 'libgof2hdaa.so')
ORIG = os.path.normpath(os.path.join(ROOT, '..', '..', '_work', 'bins',
                                    'android_2.0.16_libgof2hdaa.so'))
SYM = '_ZN11LevelScript7processEi'
BASE_REL = '289b2a069f8504d1e8a3919e43915e04'

def sections(path):
    data = open(path, 'rb').read()
    shoff = struct.unpack_from('<I', data, 0x20)[0]
    ents = struct.unpack_from('<H', data, 0x2e)[0]
    n = struct.unpack_from('<H', data, 0x30)[0]
    si = struct.unpack_from('<H', data, 0x32)[0]
    so = struct.unpack_from('<I', data, shoff + si * ents + 0x10)[0]
    ss = struct.unpack_from('<I', data, shoff + si * ents + 0x14)[0]
    shstr = data[so:so + ss]
    out = {}
    for i in range(n):
        b = shoff + i * ents
        no = struct.unpack_from('<I', data, b)[0]
        e = shstr.find(b'\0', no)
        name = shstr[no:e].decode()
        addr, off, size = struct.unpack_from('<III', data, b + 0x0c)
        out[name] = dict(addr=addr, off=off, size=size)
    return data, out

def dynsym(data, secs, sym):
    ds, dstr = secs['.dynsym'], secs['.dynstr']
    st = data[dstr['off']:dstr['off'] + dstr['size']]
    for i in range(ds['size'] // 16):
        b = ds['off'] + i * 16
        no, val, sz = struct.unpack_from('<III', data, b)
        if no and st[no:st.find(b'\0', no)].decode() == sym:
            return val, sz
    return None, None

def measure():
    od, os_ = sections(ORIG)
    cd, cs = sections(SO)
    oa, osz = dynsym(od, os_, SYM)
    ca, csz = dynsym(cd, cs, SYM)
    if oa is None or ca is None:
        return None
    ot, ct = os_['.text'], cs['.text']
    ob = od[ot['off'] + oa - ot['addr']:ot['off'] + oa - ot['addr'] + osz]
    cb = cd[ct['off'] + ca - ct['addr']:ct['off'] + ca - ct['addr'] + csz]
    raw = sum(1 for a, b in zip(ob, cb) if a != b) + abs(len(ob) - len(cb))
    otx = od[ot['off']:ot['off'] + ot['size']]
    ctx = cd[ct['off']:ct['off'] + ct['size']]
    text = sum(1 for a, b in zip(otx, ctx) if a != b) + abs(len(otx) - len(ctx))
    rp = hashlib.md5(cd[cs['.rel.plt']['off']:
                        cs['.rel.plt']['off'] + cs['.rel.plt']['size']]).hexdigest()
    return dict(raw=raw, size=csz, osize=osz, text=text, relplt=rp)

def build():
    return subprocess.run(['ninja', '-C', BUILD, 'libgof2hdaa.so', '-j2'],
                          capture_output=True, text=True).returncode == 0

def trim(t, s, e):
    while s < e and t[s] in ' \t\r\n': s += 1
    while e > s and t[e - 1] in ' \t\r\n': e -= 1
    return s, e

def split_top(t, s, e):
    ops, last, depth, st, i = [], s, 0, 'code', s
    while i < e:
        c = t[i]
        if st == 'code':
            if c == '(': depth += 1
            elif c == ')': depth -= 1
            elif c == '"': st = 'string'
            elif c == "'": st = 'char'
            elif depth == 0 and c in '&|' and i + 1 < e and t[i + 1] == c:
                ops.append((last, i)); i += 2; last = i; continue
        elif st == 'string':
            if c == '\\': i += 1
            elif c == '"': st = 'code'
        elif st == 'char':
            if c == '\\': i += 1
            elif c == "'": st = 'code'
        i += 1
    ops.append((last, e))
    return ops

def collect(t, s, e, out):
    ops = split_top(t, s, e)
    if len(ops) > 1:
        for a, b in ops:
            a, b = trim(t, a, b)
            if b > a:
                out.append((a, b))
                if t[a] == '(' and t[b - 1] == ')': collect(t, a + 1, b - 1, out)
                if t[a] == '!' and t[a + 1] == '(' and t[b - 1] == ')':
                    collect(t, a + 2, b - 1, out)

def find_process(t):
    a = t.index('bool LevelScript::process(int delta)')
    b = t.index('\nvoid LevelScript::resetCamera', a)
    return a, b

def cells_subs(t):
    p0, p1 = find_process(t)
    text = t[p0:p1]
    out = []
    # (a) operands of every &&/|| in an if/while/for condition
    for m in re.finditer(r'\b(?:if|while|for)\s*\(', text):
        op = m.end() - 1
        d, i = 0, op
        while i < len(text):
            if text[i] == '(': d += 1
            elif text[i] == ')':
                d -= 1
                if d == 0: break
            i += 1
        a, b = op + 1, i
        if m.group(0).startswith('for'):
            parts = [x for x in re.split(r';', text[a:b])]
            if len(parts) == 3:
                a = a + text[a:b].index(parts[1])
                b = a + len(parts[1])
        out.append(('andor', p0 + a, p0 + b))
    # (b) operands inside an existing __builtin_expect(EXPR, e)
    for m in re.finditer(r'__builtin_expect\s*\(', text):
        op = m.end() - 1
        d, i = 0, op
        while i < len(text):
            if text[i] == '(': d += 1
            elif text[i] == ')':
                d -= 1
                if d == 0: break
            i += 1
        d, j, comma = 0, op + 1, None
        while j < i:
            if text[j] == '(': d += 1
            elif text[j] == ')': d -= 1
            elif text[j] == ',' and d == 0: comma = j; break
            j += 1
        if comma is None: continue
        out.append(('inner-expect', p0 + op + 1, p0 + comma))
    # (c) inner expression of every !(EXPR)
    for m in re.finditer(r'!', text):
        i = m.end()
        while i < len(text) and text[i] in ' \t': i += 1
        if i >= len(text) or text[i] != '(': continue
        d, j = 0, i
        while j < len(text):
            if text[j] == '(': d += 1
            elif text[j] == ')':
                d -= 1
                if d == 0: break
            j += 1
        out.append(('bang-inner', p0 + i + 1, p0 + j))
    # expand each span into its &&/|| operands, then dedupe
    res, seen = [], set()
    for label, s, e in out:
        s, e = trim(t, s, e)
        sub = []
        collect(t, s, e, sub)
        if not sub:
            sub = [(s, e)]
        for a, b in sub:
            a, b = trim(t, a, b)
            if b <= a or (a, b) in seen: continue
            seen.add((a, b))
            res.append(dict(set=label, start=a, end=b, text=t[a:b]))
    return res

def cells_flips(t):
    p0, p1 = find_process(t)
    text = t[p0:p1]
    res = []
    for m in re.finditer(r'__builtin_expect\s*\(', text):
        op = m.end() - 1
        d, i = 0, op
        while i < len(text):
            if text[i] == '(': d += 1
            elif text[i] == ')':
                d -= 1
                if d == 0: break
            i += 1
        d, j, comma = 0, op + 1, None
        while j < i:
            if text[j] == '(': d += 1
            elif text[j] == ')': d -= 1
            elif text[j] == ',' and d == 0: comma = j; break
            j += 1
        if comma is None: continue
        val = text[comma + 1:i].strip()
        if val not in ('0', '1'): continue
        res.append(dict(set='flip', start=p0 + comma + 1, end=p0 + i,
                        repl=str(1 - int(val)), text=text[op + 1:comma].strip()))
        res.append(dict(set='strip', start=p0 + m.start(), end=p0 + i + 1,
                        repl='(' + text[op + 1:comma] + ')',
                        text=text[op + 1:comma].strip()))
    return res

def main():
    mode = 'subs'
    limit = None
    args = sys.argv[1:]
    if '--mode' in args: mode = args[args.index('--mode') + 1]
    if '--limit' in args: limit = int(args[args.index('--limit') + 1])
    base = open(SRC).read()
    cells = cells_subs(base) if mode == 'subs' else cells_flips(base)
    if limit: cells = cells[:limit]
    out = os.path.join(os.getcwd(), 'cells.tsv')
    done = set()
    if os.path.exists(out):
        for l in open(out).read().splitlines()[1:]:
            p = l.split('\t')
            if len(p) >= 4: done.add((p[0], p[1], p[2]))
    f = open(out, 'a')
    if not done:
        f.write('set\tline\tidx\tvariant\traw\tsize\ttext\trelplt\tcond\n')
        f.flush()
    t0 = time.time()
    try:
        for idx, c in enumerate(cells):
            line = base[:c['start']].count('\n') + 1
            for e in (0, 1):
                key = (str(line), str(idx), str(e))
                if key in done: continue
                if c['set'] in ('flip', 'strip'):
                    text = base[:c['start']] + c['repl'] + base[c['end']:]
                else:
                    text = (base[:c['start']] + '__builtin_expect((' +
                            base[c['start']:c['end']] + '), ' + str(e) + ')' +
                            base[c['end']:])
                open(SRC, 'w').write(text)
                if not build():
                    row = [c['set'], str(line), str(idx), str(e), 'FAIL', '0', '0', '-', c['text'][:120]]
                else:
                    m = measure()
                    row = ([c['set'], str(line), str(idx), str(e), str(m['raw']),
                            str(m['size']), str(m['text']), m['relplt'], c['text'][:120]]
                           if m else [c['set'], str(line), str(idx), str(e), 'MEASFAIL',
                                      '0', '0', '-', c['text'][:120]])
                f.write('\t'.join(x.replace('\t', ' ').replace('\n', ' ') for x in row) + '\n')
                f.flush()
            if (idx + 1) % 20 == 0:
                print('%d/%d cells %.0fs' % (idx + 1, len(cells), time.time() - t0), flush=True)
    finally:
        open(SRC, 'w').write(base)
        f.close()
    print('done', len(cells), 'cells in %.1fs' % (time.time() - t0))

if __name__ == '__main__':
    main()
