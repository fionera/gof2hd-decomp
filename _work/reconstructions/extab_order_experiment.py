#!/usr/bin/env python3
"""Generate a scratch gold --section-ordering file that reproduces the original
`.ARM.extab` record order.

Usage:
  python3 extab_order_experiment.py <original.so> <ours.map> <base-order.txt> <out.txt>

The original was linked without a section-ordering file, so its `.ARM.extab`
input sections are in link-input order.  Our link reorders `.text` with
`--section-ordering-file` but not `.ARM.extab`, so our extab order follows the
link-input order instead.  This script:

  1. parses the original `.ARM.exidx`/`.ARM.extab`, sorts the EXT entries by
     their extab target address (the original's record order), and maps each
     function address to its `.text.*` section via a gold --Map file;
  2. emits `.ARM.extab<text-section>` lines in that order;
  3. gives the three same-named `.ARM.extab.text.startup` sections unique names
     (`.ARM.extab.text.startup.oNNN`) so gold does not group them, and lists the
     plain `.ARM.extab` (libgcc) at its original position;
  4. appends `.ARM.extab*` for any remaining archive sections.

The caller must rename the three startup extab sections in the marked object
copies with objcopy before relinking (see the note's reproduction section).
"""
import collections
import os
import re
import struct
import sys


def sections(data):
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    hdrs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize)
            for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    out = {}
    for h in hdrs:
        name, typ, flags, addr, off, size, link, info, align, entsize = h
        end = data.index(b"\0", stroff + name)
        out[data[stroff + name:end].decode()] = dict(
            addr=addr, off=off, size=size)
    return out


def prel31(data, off, place):
    w, = struct.unpack_from("<I", data, off)
    v = w & 0x7fffffff
    if v & 0x40000000:
        v -= 0x80000000
    return place + v


def ext_entries(path):
    data = open(path, "rb").read()
    secs = sections(data)
    ex = secs['.ARM.exidx']
    out = []
    for i in range(ex['size'] // 8):
        eoff = ex['off'] + i * 8
        eaddr = ex['addr'] + i * 8
        fn = prel31(data, eoff, eaddr)
        w1, = struct.unpack_from("<I", data, eoff + 4)
        if w1 == 1 or (w1 & 0x80000000):
            continue
        out.append((fn, prel31(data, eoff + 4, eaddr + 4)))
    return out


def parse_map_text(path):
    lines = open(path).read().splitlines()
    addrpat = re.compile(r'^\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)')
    inlinepat = re.compile(r'^ (\S+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)')
    i = next(k for k, l in enumerate(lines) if re.match(r'^\.text\s+0x', l))
    secs, cur = [], None
    for l in lines[i + 1:]:
        m = inlinepat.match(l)
        if m:
            secs.append((m.group(1), int(m.group(2), 16), int(m.group(3), 16),
                         m.group(4)))
            cur = None
            continue
        m = re.match(r'^ (\S+)$', l)
        if m:
            cur = m.group(1)
            continue
        m = addrpat.match(l)
        if m and cur:
            secs.append((cur, int(m.group(1), 16), int(m.group(2), 16), m.group(3)))
            cur = None
            continue
        if l and not l.startswith(' '):
            break
    return secs


def main(argv):
    if len(argv) != 5:
        print(__doc__)
        return 2
    orig, mapfile, base, outfile = argv[1:]
    tsecs = parse_map_text(mapfile)

    def lookup(fn):
        best = None
        for n, a, s, p in tsecs:
            if s > 0 and a <= fn < a + s:
                if best is None or a > best[1]:
                    best = (n, a, s, p)
        return best

    entries = sorted(ext_entries(orig), key=lambda x: x[1])
    order = []
    for fn, _extab in entries:
        hit = lookup(fn)
        if not hit:
            order.append('.ARM.extab')
            continue
        n, _a, _s, p = hit
        if n == '.text.startup':
            tag = os.path.basename(p).split('_')[0]
            order.append('.ARM.extab.text.startup.' + tag)
        elif n == '.text':
            order.append('.ARM.extab')
        else:
            order.append('.ARM.extab' + n)
    seen = set()
    uniq = []
    for x in order:
        if x not in seen:
            seen.add(x)
            uniq.append(x)
    base_lines = [l for l in open(base).read().splitlines()
                  if not l.startswith('.ARM.extab')]
    with open(outfile, 'w') as f:
        f.write('\n'.join(base_lines + uniq + ['.ARM.extab*']) + '\n')
    print(f"wrote {outfile}: {len(entries)} extab entries, "
          f"{len(uniq)} unique section names")
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
