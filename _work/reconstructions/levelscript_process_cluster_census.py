#!/usr/bin/env python3
"""LevelScript::process same-address diff-run census.

Lists every maximal run of byte positions where the original and our linked
libgof2hdaa.so differ at the same function-relative address, sorted by run
length, and maps each run to:
  * our source file:line (DWARF line table from the per-TU object file), and
  * the source `case N:` block that contains that line, and
  * the original's first instructions at the run start (semantic hint).

Usage: python3 levelscript_process_cluster_census.py [--top N]
"""
import re, subprocess, bisect, sys, os
from collections import OrderedDict

ROOT = os.environ.get('GOF2_ROOT', '/home/debian/GalaxyOnFire2/wt/w9205')
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = os.path.join(ROOT, 'cmake-build-match/libgof2hdaa.so')
OBJ = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump')
READELF = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf')
OBJFILE = os.path.join(ROOT, 'cmake-build-match/CMakeFiles/gof2.dir/src/game/world/LevelScript.cpp.obj')
SRC = os.path.join(ROOT, 'src/game/world/LevelScript.cpp')
SEC = '.text._ZN11LevelScript7processEi'
NAME = '_ZN11LevelScript7processEi'

def sym(path, name):
    out = subprocess.check_output([OBJ, '-T', path], text=True)
    for l in out.splitlines():
        p = l.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    raise SystemExit('missing ' + name)

def textsec(path):
    out = subprocess.check_output([OBJ, '-h', path], text=True)
    for l in out.splitlines():
        p = l.split()
        if len(p) > 3 and p[1] == '.text':
            return int(p[3], 16), int(p[4], 16), int(p[5], 16)
    raise SystemExit('no .text')

def data(path, addr, size):
    f = open(path, 'rb').read()
    sz, vma, off = textsec(path)
    return f[off + (addr - vma):off + (addr - vma) + size]

def parse_dis(path, lo, hi):
    out = subprocess.check_output([OBJ, '-d', '--start-address=0x%x' % lo,
                                   '--stop-address=0x%x' % hi, path], text=True)
    ins = []
    for l in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s*((?:[0-9a-f]{2,8}\s)+)\s*(.*)', l)
        if m:
            ins.append((int(m.group(1), 16), bytes.fromhex(m.group(2).replace(' ', '')),
                        m.group(3).strip()))
    return ins

def _readelf_line_map():
    out=subprocess.check_output([READELF,'--debug-dump=decodedline',OBJFILE],text=True)
    lm={}
    for l in out.splitlines():
        p=l.split()
        if len(p)==3 and p[0].endswith('.cpp'):
            try:
                line=int(p[1]); addr=int(p[2],16)
            except ValueError: continue
            lm[addr]=line
    return lm

def line_map():
    lm = _readelf_line_map()
    # drop line-0 rows (compiler-generated), inherit the previous real line
    out = {}
    last = None
    for a in sorted(lm):
        if lm[a]:
            last = lm[a]
        if last is not None:
            out[a] = last
    return out

def case_table():
    PATH = SRC
    src=open(PATH).read().splitlines()
    depth=0; sw=[]; line_case={}; pending_switch=False
    for ln,l in enumerate(src,1):
        line_case[ln]=next((x[1] for x in reversed(sw) if x[1] is not None), None)
        i=0
        while i<len(l):
            if l.startswith('//',i): break
            if l.startswith('/*',i):
                j=l.find('*/',i+2); i=(j+2) if j>=0 else len(l); continue
            c=l[i]
            if c=='"':
                j=i+1
                while j<len(l):
                    if l[j]=='\\': j+=2; continue
                    if l[j]=='"': break
                    j+=1
                i=j+1; continue
            if c=="'":
                j=i+1
                while j<len(l):
                    if l[j]=='\\': j+=2; continue
                    if l[j]=="'": break
                    j+=1
                i=j+1; continue
            if c=='{':
                depth+=1
                if pending_switch: sw.append([depth,None]); pending_switch=False
            elif c=='}':
                if sw and depth==sw[-1][0]: sw.pop()
                depth-=1
            elif l.startswith('switch',i): pending_switch=True
            elif l.startswith('case ',i):
                m=re.match(r'case\s+(\w+)\s*:',l[i:])
                if m and sw:
                    sw[-1][1]=m.group(1); i+=m.end(); continue
            i+=1
    return line_case


def main():
    top = 40
    if '--top' in sys.argv:
        top = int(sys.argv[sys.argv.index('--top') + 1])
    ADDR, SIZE = sym(ORIG, NAME)
    oaddr, osize = sym(OURS, NAME)
    assert osize == SIZE, (osize, SIZE)
    x = data(ORIG, ADDR, SIZE)
    y = data(OURS, oaddr, osize)
    diff = [i for i in range(SIZE) if x[i] != y[i]]
    runs = []
    if diff:
        s = p = diff[0]
        for d in diff[1:]:
            if d == p + 1:
                p = d
            else:
                runs.append((s, p)); s = p = d
        runs.append((s, p))
    runs.sort(key=lambda r: -(r[1] - r[0] + 1))
    lm = line_map(); litems = sorted(lm)
    cases = case_table()
    def srcline(off):
        i = bisect.bisect_right(litems, off) - 1
        return lm[litems[i]] if i >= 0 else None
    def scase(line):
        cn = cases.get(line)
        return (0, cn) if cn is not None else None
    # original instructions, indexed by address
    oins = parse_dis(ORIG, ADDR, ADDR + SIZE)
    oaddrs = [a for a, b, t in oins]
    def orig_hint(addr):
        i = bisect.bisect_right(oaddrs, addr) - 1
        if i < 0:
            return ''
        return ' ; '.join('%x:%s' % (oins[j][0], oins[j][2]) for j in range(i, min(i + 3, len(oins))))
    print('orig %06x size %d  ours %06x size %d  diff bytes %d  runs %d'
          % (ADDR, SIZE, oaddr, osize, len(diff), len(runs)))
    print()
    print('%5s %-19s %6s  %-6s %-5s  %s' % ('#', 'addr range', 'len', 'line', 'case', 'orig hint'))
    for n, (s, e) in enumerate(runs[:top]):
        line = srcline(s)
        c = scase(line) if line else None
        print('%5d %08x..%08x %6d  L%-5s %-5s  %s'
              % (n + 1, ADDR + s, ADDR + e, e - s + 1, line,
                 ('c' + c[1]) if c else '-', orig_hint(ADDR + s)))
    # summary by source case
    print()
    agg = OrderedDict()
    for s, e in runs:
        line = srcline(s)
        c = scase(line) if line else None
        key = ('case ' + c[1]) if c else '?'
        a = agg.setdefault(key, [0, 0])
        a[0] += e - s + 1
        a[1] += 1
    print('%-12s %10s %6s' % ('source case', 'diffbytes', 'runs'))
    for k, v in sorted(agg.items(), key=lambda kv: -kv[1][0]):
        print('%-12s %10d %6d' % (k, v[0], v[1]))

if __name__ == '__main__':
    main()
