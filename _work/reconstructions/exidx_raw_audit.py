#!/usr/bin/env python3
"""Raw `.ARM.exidx` / `.ARM.extab` record audit for the GoF2 ARM EHABI binaries.

Usage:
  python3 exidx_raw_audit.py <original.so> <ours.so> [<ours.map>]

Prints:
  * entry / kind counts and the raw word-by-word comparison of the two
    `.ARM.exidx` tables (word0 = prel31(function), word1 = CANT / inline
    compact model / prel31(.ARM.extab));
  * per-EXT-record byte comparison (records are aligned by the function that
    owns them, not by address, so record-order and length deltas do not
    masquerade as content deltas);
  * with a gold `--Map` file, the position of every referenced extab record in
    the original's record order vs ours (a record-order metric).

The parser is the same one used by `exidx_table.py`; the raw-word and
record-content comparisons are the additions from the w3625 pass.
"""
import collections
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
            addr=addr, off=off, size=size, entsize=entsize)
    return out


def prel31(data, off, place):
    w, = struct.unpack_from("<I", data, off)
    v = w & 0x7fffffff
    if v & 0x40000000:
        v -= 0x80000000
    return place + v, w


def dynsyms(data, secs):
    s, st = secs['.dynsym'], secs['.dynstr']
    out = []
    for i in range(s['size'] // 16):
        name, value, size, info, _other, shndx = struct.unpack_from(
            "<IIIBBH", data, s['off'] + i * 16)
        end = data.index(b"\0", st['off'] + name)
        out.append((value, size, data[st['off'] + name:end].decode(), info & 0xf))
    return out


def parse(path):
    data = open(path, 'rb').read()
    secs = sections(data)
    ex, et = secs['.ARM.exidx'], secs['.ARM.extab']
    funcs = sorted((v, sz, nm) for v, sz, nm, t in dynsyms(data, secs)
                   if t == 2 and v)
    entries = []
    for i in range(ex['size'] // 8):
        eoff = ex['off'] + i * 8
        eaddr = ex['addr'] + i * 8
        fn, w0 = prel31(data, eoff, eaddr)
        w1, = struct.unpack_from("<I", data, eoff + 4)
        rec = dict(i=i, eaddr=eaddr, fn=fn, w0=w0, w1=w1)
        if w1 == 1:
            rec['kind'] = 'CANT'
        elif w1 & 0x80000000:
            rec['kind'] = 'INL'
            rec['model'] = (w1 >> 24) & 0xf
        else:
            tgt, _ = prel31(data, eoff + 4, eaddr + 4)
            rec['kind'] = 'EXT'
            rec['extab'] = tgt
        best = None
        for v, sz, nm in funcs:
            if v <= fn:
                best = (v, sz, nm)
            else:
                break
        rec['name'] = best[2] if best else None
        rec['foff'] = fn - best[0] if best else None
        entries.append(rec)
    ts = sorted(set(r['extab'] for r in entries if r['kind'] == 'EXT'))
    end0 = et['addr'] + et['size']
    lens = {t: (ts[i + 1] if i + 1 < len(ts) else end0) - t
            for i, t in enumerate(ts)}
    for r in entries:
        if r['kind'] == 'EXT':
            r['reclen'] = lens[r['extab']]
            off = et['off'] + (r['extab'] - et['addr'])
            r['recbytes'] = data[off:off + r['reclen']]
    return data, secs, entries


def parse_map_text(path):
    """(name, addr, size, object) for every input section in the .text output section."""
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


def audit(orig, ours, mapfile=None):
    da, sa, ea = parse(orig)
    db, sb, eb = parse(ours)
    print(f"orig: exidx {sa['.ARM.exidx']['size']:#x} extab {sa['.ARM.extab']['size']:#x}")
    print(f"ours: exidx {sb['.ARM.exidx']['size']:#x} extab {sb['.ARM.extab']['size']:#x}")
    print(f"entries {len(ea)} {len(eb)}")
    for k in ('CANT', 'INL', 'EXT'):
        ca = sum(1 for r in ea if r['kind'] == k)
        cb = sum(1 for r in eb if r['kind'] == k)
        print(f"  {k:4s} {ca} {cb}")
    print("same w0", sum(1 for x, y in zip(ea, eb) if x['w0'] == y['w0']),
          "same fn", sum(1 for x, y in zip(ea, eb) if x['fn'] == y['fn']),
          "same w1", sum(1 for x, y in zip(ea, eb) if x['w1'] == y['w1']),
          "same kind", sum(1 for x, y in zip(ea, eb) if x['kind'] == y['kind']))
    for k in ('CANT', 'INL', 'EXT'):
        pairs = [(x, y) for x, y in zip(ea, eb)
                 if x['kind'] == k and y['kind'] == k]
        print(f"  {k:4s} pairs {len(pairs)} w1 differ "
              f"{sum(1 for x, y in pairs if x['w1'] != y['w1'])}")
    oa, ob = sa['.ARM.exidx']['off'], sb['.ARM.exidx']['off']
    n = min(sa['.ARM.exidx']['size'], sb['.ARM.exidx']['size'])
    print("exidx differing bytes",
          sum(1 for p, q in zip(da[oa:oa + n], db[ob:ob + n]) if p != q), "of", n)
    oa, ob = sa['.ARM.extab']['off'], sb['.ARM.extab']['off']
    n = min(sa['.ARM.extab']['size'], sb['.ARM.extab']['size'])
    print("extab differing bytes",
          sum(1 for p, q in zip(da[oa:oa + n], db[ob:ob + n]) if p != q), "of", n)
    ga, gb = collections.defaultdict(list), collections.defaultdict(list)
    for r in ea:
        if r['kind'] == 'EXT':
            ga[(r['name'], r['foff'])].append(r)
    for r in eb:
        if r['kind'] == 'EXT':
            gb[(r['name'], r['foff'])].append(r)
    nd = eq = td = ld = 0
    firsts = collections.Counter()
    for k in ga:
        for x, y in zip(ga[k], gb.get(k, [])):
            if x['recbytes'] != y['recbytes']:
                nd += 1
                if len(x['recbytes']) == len(y['recbytes']):
                    eq += 1
                    td += sum(1 for p, q in zip(x['recbytes'], y['recbytes'])
                              if p != q)
                    j = next(i for i in range(len(x['recbytes']))
                             if x['recbytes'][i] != y['recbytes'][i])
                    firsts[j] += 1
                else:
                    ld += 1
    print(f"extab records differ {nd} equal-len {eq} diff bytes {td} "
          f"len-diff {ld} first-diff {dict(sorted(firsts.items()))}")
    print("extab record length deltas:")
    total = 0
    for k in ga:
        for x, y in zip(ga[k], gb.get(k, [])):
            if x['reclen'] != y['reclen']:
                print(f"  {k[0]}+{k[1]}: {x['reclen']} -> {y['reclen']} "
                      f"({y['reclen'] - x['reclen']:+d})")
                total += y['reclen'] - x['reclen']
    print(f"  total {total:+d}")
    if mapfile:
        tsecs = parse_map_text(mapfile)

        def lookup(fn):
            best = None
            for n, a, s, p in tsecs:
                if s > 0 and a <= fn < a + s:
                    if best is None or a > best[1]:
                        best = (n, a, s, p)
            return best

        def name_of(fn):
            h = lookup(fn)
            if not h:
                return '.ARM.extab'
            n, a, s, p = h
            if n == '.text.startup':
                return '.ARM.extab.text.startup.' + p.rsplit('_', 1)[0].rsplit('/', 1)[-1]
            if n == '.text':
                return '.ARM.extab'
            return '.ARM.extab' + n

        xa = sorted((r for r in ea if r['kind'] == 'EXT'),
                    key=lambda r: r['extab'])
        xb = sorted((r for r in eb if r['kind'] == 'EXT'),
                    key=lambda r: r['extab'])
        oa = [name_of(r['fn']) for r in xa]
        ob = [name_of(r['fn']) for r in xb]
        print("extab order positions equal",
              sum(1 for a, b in zip(oa, ob) if a == b), "/", len(oa),
              "sequence equal", oa == ob)


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    audit(argv[1], argv[2], argv[3] if len(argv) > 3 else None)
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
