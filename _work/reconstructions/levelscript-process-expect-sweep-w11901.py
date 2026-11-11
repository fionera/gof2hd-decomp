#!/usr/bin/env python3
# Self-contained whole-condition __builtin_expect sweep for LevelScript::process.
# Usage: python3 levelscript-process-expect-sweep-w11901.py [limit] [line]
# Writes cells.tsv in the current directory; restores the source on exit.
import sys, os, subprocess, time, re

WT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = WT + '/src/game/world/LevelScript.cpp'
BUILD = WT + '/cmake-build-match'
SO = BUILD + '/libgof2hdaa.so'
MEASURE = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'measure_w11901.py')
OUT = os.path.join(os.getcwd(), 'cells.tsv')

def measure():
    p = subprocess.run([sys.executable, MEASURE, SO], capture_output=True, text=True)
    if p.returncode != 0:
        return None
    m = re.search(r'raw=(\d+) size=(\d+)/(\d+) text=(\d+) relplt=([0-9a-f]+)', p.stdout)
    if not m:
        return None
    return dict(raw=int(m.group(1)), size=int(m.group(2)), osize=int(m.group(3)),
                text=int(m.group(4)), relplt=m.group(5))

def build():
    p = subprocess.run(['ninja', '-C', BUILD, 'libgof2hdaa.so', '-j2'],
                       capture_output=True, text=True)
    return p.returncode == 0, p.stderr[-2000:]

def apply_edit(text, c, e):
    return text[:c['start']] + '__builtin_expect((' + text[c['start']:c['end']] + '), ' + str(e) + ')' + text[c['end']:]

import sys
import bisect

def _ident(c):
    return c.isalnum() or c == '_'

def _pp_mask(text):
    n = len(text)
    pp = [False]*n
    j = 0
    while j < n:
        k = j
        while k < n and text[k] in ' \t':
            k += 1
        if k < n and text[k] == '#':
            e = k
            while e < n:
                if text[e] == '\n':
                    b = e-1
                    if b >= 0 and text[b] == '\r':
                        b -= 1
                    if b >= 0 and text[b] == '\\':
                        e += 1
                        continue
                    break
                e += 1
            for x in range(j, min(e+1, n)):
                pp[x] = True
            j = e+1
        else:
            e = text.find('\n', j)
            if e < 0:
                break
            j = e+1
    return pp

def find_conditions(text):
    n = len(text)
    pp = _pp_mask(text)
    nl = [i for i, ch in enumerate(text) if ch == '\n']
    def lineno(pos):
        return bisect.bisect_right(nl, pos) + 1
    out = []
    i = 0
    state = 'code'
    line = 1
    while i < n:
        c = text[i]
        if state == 'code':
            if c == '\n':
                line += 1; i += 1; continue
            if pp[i]:
                i += 1; continue
            if c == '/' and i+1 < n and text[i+1] == '/':
                state = 'linecomment'; i += 2; continue
            if c == '/' and i+1 < n and text[i+1] == '*':
                state = 'blockcomment'; i += 2; continue
            if c == '"':
                state = 'string'; i += 1; continue
            if c == "'":
                state = 'char'; i += 1; continue
            if _ident(c):
                s = i
                while i < n and _ident(text[i]):
                    i += 1
                word = text[s:i]
                if word in ('if', 'while', 'for'):
                    p = s-1
                    while p >= 0 and text[p] in ' \t':
                        p -= 1
                    member = p >= 0 and (text[p] == '.' or (text[p] == '>' and p > 0 and text[p-1] == '-'))
                    if not member:
                        k = i
                        while k < n and text[k] in ' \t\r\n':
                            k += 1
                        if k < n and text[k] == '(':
                            depth = 0; m = k; st = 'code'
                            while m < n:
                                ch = text[m]
                                if st == 'code':
                                    if ch == '(':
                                        depth += 1
                                    elif ch == ')':
                                        depth -= 1
                                        if depth == 0:
                                            break
                                    elif ch == '"':
                                        st = 'string'
                                    elif ch == "'":
                                        st = 'char'
                                    elif ch == '/' and m+1 < n and text[m+1] == '/':
                                        st = 'linecomment'
                                    elif ch == '/' and m+1 < n and text[m+1] == '*':
                                        st = 'blockcomment'
                                elif st == 'string':
                                    if ch == '\\': m += 1
                                    elif ch == '"': st = 'code'
                                elif st == 'char':
                                    if ch == '\\': m += 1
                                    elif ch == "'": st = 'code'
                                elif st == 'linecomment':
                                    if ch == '\n': st = 'code'
                                elif st == 'blockcomment':
                                    if ch == '/' and m > 0 and text[m-1] == '*':
                                        st = 'code'
                                m += 1
                            if m < n:
                                if word == 'for':
                                    parts = []; d2 = 0; st2 = 'code'; last = k+1; q = k+1
                                    while q < m:
                                        ch = text[q]
                                        if st2 == 'code':
                                            if ch == '(': d2 += 1
                                            elif ch == ')': d2 -= 1
                                            elif ch == ';' and d2 == 0:
                                                parts.append((last, q)); last = q+1
                                            elif ch == '"': st2 = 'string'
                                            elif ch == "'": st2 = 'char'
                                        elif st2 == 'string':
                                            if ch == '\\': q += 1
                                            elif ch == '"': st2 = 'code'
                                        elif st2 == 'char':
                                            if ch == '\\': q += 1
                                            elif ch == "'": st2 = 'code'
                                        q += 1
                                    parts.append((last, m))
                                    if len(parts) == 3:
                                        cs, ce = parts[1]
                                        while cs < ce and text[cs] in ' \t\r\n': cs += 1
                                        while ce > cs and text[ce-1] in ' \t\r\n': ce -= 1
                                        out.append(dict(kw=word, line=lineno(s), start=cs, end=ce,
                                                        cond=text[cs:ce]))
                                else:
                                    cs, ce = k+1, m
                                    while cs < ce and text[cs] in ' \t\r\n': cs += 1
                                    while ce > cs and text[ce-1] in ' \t\r\n': ce -= 1
                                    out.append(dict(kw=word, line=lineno(s), start=cs, end=ce,
                                                    cond=text[cs:ce]))
                                i = m+1
                                continue
                continue
            i += 1
        elif state == 'linecomment':
            if c == '\n':
                state = 'code'; line += 1
            i += 1
        elif state == 'blockcomment':
            if c == '\n':
                line += 1
            elif c == '*' and i+1 < n and text[i+1] == '/':
                state = 'code'; i += 2; continue
            i += 1
        elif state == 'string':
            if c == '\\':
                i += 2; continue
            if c == '\n':
                line += 1
            elif c == '"':
                state = 'code'
            i += 1
        elif state == 'char':
            if c == '\\':
                i += 2; continue
            if c == '\n':
                line += 1
            elif c == "'":
                state = 'code'
            i += 1
    return out

def main():
    limit = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    only = sys.argv[2] if len(sys.argv) > 2 else None
    base = open(SRC).read()
    cs = find_conditions(base)
    # process + tail (line >= 605).  Add the one `if` inside a // comment as a manual cell.
    sel = [c for c in cs if c['line'] >= 605]
    # comment if at line 1550: locate `if` inside the comment text
    m = re.search(r'// The former `if \(', base)
    if m:
        st = m.start() + m.group(0).index('if')
        # condition is `m_nState == 0x7f` inside the backticks
        cm = re.search(r'`if \((.*?)\) player', base[st:])
        if cm:
            cs0 = st + cm.start(1); ce0 = st + cm.end(1)
            sel.append(dict(kw='if', line=base[:st].count('\n')+1, start=cs0, end=ce0,
                            cond=base[cs0:ce0], comment=True))
    sel.sort(key=lambda c: c['start'])
    if only:
        sel = [c for c in sel if str(c['line']) == only]
    if limit:
        sel = sel[:limit]
    done = set()
    if os.path.exists(OUT):
        for l in open(OUT).read().splitlines()[1:]:
            p = l.split('\t')
            if len(p) >= 3:
                done.add((p[0], p[1], p[2]))
    first = not os.path.exists(OUT)
    f = open(OUT, 'a')
    if first:
        f.write('line\tkind\tvariant\traw\tsize\ttext\trelplt\tcond\n')
        f.flush()
    t0 = time.time()
    n = 0
    try:
        for c in sel:
            for e in (0, 1):
                key = (str(c['line']), c['kw'], str(e))
                if key in done:
                    continue
                text = apply_edit(base, c, e)
                open(SRC, 'w').write(text)
                ok, err = build()
                if not ok:
                    row = [str(c['line']), c['kw'], str(e), 'FAIL', '0', '0', '-', c['cond'].replace('\t',' ').replace('\n',' ').replace('\r',' ')[:160]]
                else:
                    r = measure()
                    if r is None:
                        row = [str(c['line']), c['kw'], str(e), 'MEASFAIL', '0', '0', '-', c['cond'].replace('\t',' ').replace('\n',' ').replace('\r',' ')[:160]]
                    else:
                        row = [str(c['line']), c['kw'], str(e), str(r['raw']), str(r['size']),
                               str(r['text']), r['relplt'], c['cond'].replace('\t',' ').replace('\n',' ').replace('\r',' ')[:160]]
                f.write('\t'.join(row) + '\n'); f.flush()
                n += 1
                if n % 20 == 0:
                    el = time.time()-t0
                    print('%d cells, %.1fs, %.2fs/cell' % (n, el, el/n), flush=True)
    finally:
        open(SRC, 'w').write(base)
        f.close()
    print('done', n, 'cells in %.1fs' % (time.time()-t0))

if __name__ == '__main__':
    main()
