#!/usr/bin/env python3
"""Print every differing byte-hunk of one function, with both disassemblies.

Usage: python3 scratch/wave192/hunks.py <mangled> [--ours PATH] [--ctx 48]

Why: the residual functions are size-exact, so the difference is always a
handful of local instruction shapes. Agents kept re-deriving this by hand.
This prints the hunks directly, so the first thing you look at is the actual
difference.
"""
import argparse, os, re, subprocess, sys

ROOT = '/home/debian/GalaxyOnFire2/gof2-decomp'
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OBJDUMP = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/'
                       'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
                       'arm-linux-androideabi-objdump')


def sections(path):
    out = subprocess.run(['readelf', '-SW', path], capture_output=True, text=True).stdout
    d = {}
    for l in out.splitlines():
        m = re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)', l)
        if m:
            d[m.group(1)] = (int(m.group(2), 16), int(m.group(4), 16), int(m.group(3), 16))
    return d


def sym(path, name):
    out = subprocess.run(['readelf', '-sW', path], capture_output=True, text=True).stdout
    for l in out.splitlines():
        p = l.split()
        if len(p) >= 8 and p[7] == name:
            return int(p[1], 16) & ~1, int(p[2])
    return None


def body(path, addr, size):
    sec = sections(path)
    taddr, tsize, toff = sec['.text']
    with open(path, 'rb') as f:
        f.seek(toff + (addr - taddr))
        return f.read(size)


def dis(path, lo, n):
    hi = lo + n
    out = subprocess.run([OBJDUMP, '-d', '--start-address', hex(lo),
                          '--stop-address', hex(hi), path],
                         capture_output=True, text=True).stdout
    return '\n'.join(l for l in out.splitlines() if re.match(r'\s*[0-9a-f]+:', l))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('symbol')
    ap.add_argument('--ours', default=os.path.join(ROOT, 'cmake-build-match/libgof2hdaa.so'))
    ap.add_argument('--ctx', type=int, default=48)
    a = ap.parse_args()

    so = sym(ORIG, a.symbol)
    su = sym(a.ours, a.symbol)
    if not so or not su:
        sys.exit(f'symbol not found: {a.symbol} (orig={so} ours={su})')
    oaddr, osize = so
    uaddr, usize = su
    print(f'{a.symbol}\n  orig {oaddr:#x} size {osize}\n  ours {uaddr:#x} size {usize}')
    if osize != usize:
        print(f'  SIZE DIFF {usize - osize:+d}')
    n = min(osize, usize)
    ob = body(ORIG, oaddr, n)
    ub = body(a.ours, uaddr, n)
    diffs = [i for i in range(n) if ob[i] != ub[i]]
    print(f'  differing bytes: {len(diffs)}')
    if not diffs:
        print('  byte-identical')
        return
    groups = []
    for i in diffs:
        if groups and i - groups[-1][-1] <= 8:
            groups[-1].append(i)
        else:
            groups.append([i])
    print(f'  hunks: {len(groups)}')
    for k, g in enumerate(groups):
        s, e = g[0], g[-1]
        lo = max(0, (s & ~3) - a.ctx)
        hi = min(n, (e | 3) + a.ctx)
        print(f'\n=== hunk {k+1}/{len(groups)}  +{s:#x}..+{e:#x} '
              f'({e - s + 1} bytes, {len(g)} differ)')
        print(f'  orig bytes: {ob[s:e+1].hex()}')
        print(f'  ours bytes: {ub[s:e+1].hex()}')
        print('--- orig ---')
        print(dis(ORIG, oaddr + lo, hi - lo))
        print('--- ours ---')
        print(dis(a.ours, uaddr + lo, hi - lo))


if __name__ == '__main__':
    main()
