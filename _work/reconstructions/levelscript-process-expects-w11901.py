#!/usr/bin/env python3
# Self-contained existing-expect flip/strip sweep for LevelScript::process.
# Usage: python3 levelscript-process-expects-w11901.py
# Writes cellsB.tsv in the current directory; restores the source on exit.
import sys, os, subprocess, time, re

WT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = WT + '/src/game/world/LevelScript.cpp'
BUILD = WT + '/cmake-build-match'
SO = BUILD + '/libgof2hdaa.so'
MEASURE = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'measure_w11901.py')
OUT = os.path.join(os.getcwd(), 'cellsB.tsv')

def measure():
    p = subprocess.run([sys.executable, MEASURE, SO], capture_output=True, text=True)
    if p.returncode != 0: return None
    m = re.search(r'raw=(\d+) size=(\d+)/(\d+) text=(\d+) relplt=([0-9a-f]+)', p.stdout)
    if not m: return None
    return dict(raw=int(m.group(1)), size=int(m.group(2)), osize=int(m.group(3)),
                text=int(m.group(4)), relplt=m.group(5))

def build():
    p = subprocess.run(['ninja', '-C', BUILD, 'libgof2hdaa.so', '-j2'],
                       capture_output=True, text=True)
    return p.returncode == 0, p.stderr[-2000:]

import sys, bisect

def find_expects(text):
    out = []
    nl = [i for i, ch in enumerate(text) if ch == '\n']
    def lineno(pos): return bisect.bisect_right(nl, pos) + 1
    key = '__builtin_expect'
    i = 0
    n = len(text)
    while True:
        i = text.find(key, i)
        if i < 0: break
        # must be followed by optional ws then (
        k = i + len(key)
        while k < n and text[k] in ' \t\r\n': k += 1
        if k >= n or text[k] != '(':
            i += len(key); continue
        depth = 0; m = k; st = 'code'
        while m < n:
            ch = text[m]
            if st == 'code':
                if ch == '(': depth += 1
                elif ch == ')':
                    depth -= 1
                    if depth == 0: break
                elif ch == '"': st = 'string'
                elif ch == "'": st = 'char'
                elif ch == '/' and m+1<n and text[m+1]=='/': st='linecomment'
                elif ch == '/' and m+1<n and text[m+1]=='*': st='blockcomment'
            elif st == 'string':
                if ch == '\\': m += 1
                elif ch == '"': st = 'code'
            elif st == 'char':
                if ch == '\\': m += 1
                elif ch == "'": st = 'code'
            elif st == 'linecomment':
                if ch == '\n': st='code'
            elif st == 'blockcomment':
                if ch == '/' and m>0 and text[m-1]=='*': st='code'
            m += 1
        if m >= n: break
        # split top-level comma
        args=[]; d=0; st='code'; last=k+1; q=k+1
        while q < m:
            ch=text[q]
            if st=='code':
                if ch=='(': d+=1
                elif ch==')': d-=1
                elif ch==',' and d==0:
                    args.append((last,q)); last=q+1
                elif ch=='"': st='string'
                elif ch=="'": st='char'
            elif st=='string':
                if ch=='\\': q+=1
                elif ch=='"': st='code'
            elif st=='char':
                if ch=='\\': q+=1
                elif ch=="'": st='code'
            q+=1
        args.append((last,m))
        if len(args)==2:
            a0s,a0e=args[0]; a1s,a1e=args[1]
            while a0s<a0e and text[a0s] in ' \t\r\n': a0s+=1
            while a0e>a0s and text[a0e-1] in ' \t\r\n': a0e-=1
            while a1s<a1e and text[a1s] in ' \t\r\n': a1s+=1
            while a1e>a1s and text[a1e-1] in ' \t\r\n': a1e-=1
            out.append(dict(start=i, end=m+1, line=lineno(i), arg0=text[a0s:a0e],
                            a0s=a0s, a0e=a0e, arg1=text[a1s:a1e], a1s=a1s, a1e=a1e))
        i = m+1
    return out

def main():
    base = open(SRC).read()
    es = [e for e in find_expects(base) if 605 <= e['line'] < 5342]
    f = open(OUT, 'w')
    f.write('line\tvariant\traw\tsize\ttext\trelplt\targ0\n'); f.flush()
    t0 = time.time(); n = 0
    try:
        for e in es:
            for variant in ('flip', 'strip'):
                if variant == 'flip':
                    new = base[:e['a1s']] + ('0' if e['arg1'] == '1' else '1') + base[e['a1e']:]
                else:
                    new = base[:e['start']] + e['arg0'] + base[e['end']:]
                open(SRC, 'w').write(new)
                ok, err = build()
                if not ok:
                    row = [str(e['line']), variant, 'FAIL', '0', '0', '-', e['arg0'].replace('\t',' ').replace('\n',' ')[:120]]
                else:
                    r = measure()
                    row = [str(e['line']), variant, str(r['raw']), str(r['size']), str(r['text']),
                           r['relplt'], e['arg0'].replace('\t',' ').replace('\n',' ')[:120]]
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
