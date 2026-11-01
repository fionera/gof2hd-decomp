#!/usr/bin/env python3
"""Order fingerprint: section bytes, .dynsym defined-symbol alignment, .bss address
order and .plt symbol order, original vs ours.

Usage: fp.py [OURS.so] [ORIGINAL.so]
Defaults are the repo's cmake-build-match/libgof2hdaa.so and ../_work/bins/... .
Run from the repo root (or pass absolute paths).
"""
import difflib
import os
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
ORIG = sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, '..', '_work', 'bins', 'android_2.0.16_libgof2hdaa.so')
OURS = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, 'cmake-build-match', 'libgof2hdaa.so')
OD = os.path.join(ROOT, '.cache', 'ndk', 'android-ndk-r18b', 'toolchains',
                  'arm-linux-androideabi-4.9', 'prebuilt', 'linux-x86_64', 'bin',
                  'arm-linux-androideabi-objdump')


def sections(path):
    data = open(path, 'rb').read()
    shoff = struct.unpack_from('<I', data, 0x20)[0]
    shentsize = struct.unpack_from('<H', data, 0x2e)[0]
    shnum = struct.unpack_from('<H', data, 0x30)[0]
    shstrndx = struct.unpack_from('<H', data, 0x32)[0]
    out = []
    for i in range(shnum):
        off = shoff + i * shentsize
        v = struct.unpack_from('<IIIIIIIIII', data, off)
        out.append(dict(name=v[0], type=v[1], flags=v[2], addr=v[3], offset=v[4],
                        size=v[5], link=v[6], info=v[7], align=v[8], entsize=v[9]))
    st = out[shstrndx]
    for s in out:
        s['sname'] = data[st['offset'] + s['name']:data.index(b'\0', st['offset'] + s['name'])].decode()
    return data, out


def dynsym(path):
    data, secs = sections(path)
    ds = [s for s in secs if s['type'] == 11][0]
    st = secs[ds['link']]
    out = []
    for i in range(ds['size'] // 16):
        off = ds['offset'] + i * 16
        name, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', data, off)
        nm = data[st['offset'] + name:data.index(b'\0', st['offset'] + name)].decode()
        out.append(dict(idx=i, name=nm, value=value, size=size, shndx=shndx,
                        sec=secs[shndx]['sname'] if shndx < len(secs) else ''))
    return out


def plt(path):
    text = subprocess.run([OD, '-d', '-j', '.plt', path], capture_output=True, text=True).stdout
    return [line.split('<')[1].split('@')[0] for line in text.splitlines() if '@plt>:' in line]


def main():
    do, so = sections(ORIG)
    du, su = sections(OURS)
    mo = {s['sname']: s for s in so}
    mu = {s['sname']: s for s in su}
    print('== sections ==')
    for n in mo:
        a, b = mo[n], mu.get(n)
        if b is None:
            print('MISSING', n)
            continue
        da = do[a['offset']:a['offset'] + a['size']]
        db = du[b['offset']:b['offset'] + b['size']]
        nd = sum(1 for x, y in zip(da, db) if x != y)
        flag = '' if (a['size'] == b['size'] and a['addr'] == b['addr']) else \
            f" SIZE/ADDR {a['addr']:#x}/{a['size']:#x} vs {b['addr']:#x}/{b['size']:#x}"
        if nd or flag:
            print(f"{n:22} diffbytes {nd:7}{flag}")

    print('== dynsym defined alignment ==')
    o, u = dynsym(ORIG), dynsym(OURS)
    od = [s['name'] for s in o if s['shndx'] != 0]
    ud = [s['name'] for s in u if s['shndx'] != 0]
    sm = difflib.SequenceMatcher(None, od, ud, autojunk=False)
    blocks = [b for b in sm.get_matching_blocks() if b.size]
    print(f"defined {len(od)}/{len(ud)} ratio {sm.ratio():.6f} "
          f"matched {sum(b.size for b in blocks)} gaps {len(blocks) - 1}")
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            continue
        print(f"  {tag} orig[{i1}:{i2}] ours[{j1}:{j2}]")
        for k in range(i1, i2):
            print('    O', k, od[k])
        for k in range(j1, j2):
            print('    U', k, ud[k])

    print('== .bss exported address order ==')
    ob = sorted([s for s in o if s['sec'] == '.bss' and s['size'] > 0], key=lambda s: s['value'])
    ub = sorted([s for s in u if s['sec'] == '.bss' and s['size'] > 0], key=lambda s: s['value'])
    on, un = [s['name'] for s in ob], [s['name'] for s in ub]
    sm2 = difflib.SequenceMatcher(None, on, un, autojunk=False)
    print(f"bss syms {len(on)}/{len(un)} same-order "
          f"{sum(1 for a, b in zip(on, un) if a == b)} ratio {sm2.ratio():.6f}")
    for tag, i1, i2, j1, j2 in sm2.get_opcodes():
        if tag == 'equal':
            continue
        print(f"  {tag} orig[{i1}:{i2}] ours[{j1}:{j2}]")
        for k in range(i1, i2):
            print(f"    O {ob[k]['value']:#x} {on[k]}")
        for k in range(j1, j2):
            print(f"    U {ub[k]['value']:#x} {un[k]}")

    print('== plt symbol order ==')
    po, pu = plt(ORIG), plt(OURS)
    sm3 = difflib.SequenceMatcher(None, po, pu, autojunk=False)
    print(f"plt {len(po)}/{len(pu)} same-index "
          f"{sum(1 for a, b in zip(po, pu) if a == b)} ratio {sm3.ratio():.6f}")
    for tag, i1, i2, j1, j2 in sm3.get_opcodes():
        if tag == 'equal':
            continue
        print(f"  {tag} orig[{i1}:{i2}] ours[{j1}:{j2}]")
        for k in range(i1, i2):
            print('    O', k, po[k])
        for k in range(j1, j2):
            print('    U', k, pu[k])


if __name__ == '__main__':
    main()
