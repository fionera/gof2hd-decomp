#!/usr/bin/env python3
"""Per-entry `.ARM.exidx` / `.ARM.extab` table for the GoF2 ARM EHABI binaries.

Usage:
  python3 exidx_table.py <so>                 # full table: fn, extab offset, model, kind
  python3 exidx_table.py --diff <orig> <ours> # per-function kind-sequence diff + extab lengths

The parser reads the raw prel31 words (the `readelf --unwind` output does not expose
the LSDA/call-site data).  Entry format (ARM EHABI):
  word0 = prel31(function)
  word1 = 1                      -> EXIDX_CANTUNWIND
        = 0x8xxxxxxx             -> inline compact model (bits 27-24 = model)
        = prel31(.ARM.extab)     -> personality + LSDA record

The LSDA record is parsed with the real libc++abi layout
(`sources/cxx-stl/llvm-libc++abi/src/cxa_personality.cpp`):
  lpStartEncoding (1 B) + lpStart (encoded)
  ttypeEncoding (1 B) + classInfoOffset (encoded with ttypeEncoding)
  callSiteEncoding (1 B) + callSiteTableLength (ULEB128)
  call-site table: (start, length, landingPad) encoded with callSiteEncoding, action ULEB128
"""
import struct
import sys


def _sections(data):
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


def _prel31(data, off, place):
    w, = struct.unpack_from("<I", data, off)
    v = w & 0x7fffffff
    if v & 0x40000000:
        v -= 0x80000000
    return place + v, w


def _dynsyms(data, secs):
    s, st = secs['.dynsym'], secs['.dynstr']
    out = []
    for i in range(s['size'] // 16):
        name, value, size, info, _other, shndx = struct.unpack_from(
            "<IIIBBH", data, s['off'] + i * 16)
        end = data.index(b"\0", st['off'] + name)
        out.append((value, size, data[st['off'] + name:end].decode(), info & 0xf))
    return out


def _uleb(data, i):
    v = shift = 0
    while True:
        b = data[i]
        i += 1
        v |= (b & 0x7f) << shift
        if not (b & 0x80):
            return v, i
        shift += 7


def _read_enc(data, i, enc):
    if enc == 0xff:
        return None, i
    size = enc & 0x0f
    if size == 0x00:
        return data[i:i + 4], i + 4
    if size == 0x01:
        return _uleb(data, i)
    if size == 0x02:
        return data[i:i + 2], i + 2
    if size == 0x03:
        return data[i:i + 4], i + 4
    if size == 0x04:
        return data[i:i + 8], i + 8
    if size == 0x09:
        return _uleb(data, i)
    if size == 0x0a:
        return data[i:i + 2], i + 2
    if size == 0x0b:
        return data[i:i + 4], i + 4
    if size == 0x0c:
        return data[i:i + 8], i + 8
    return data[i:i + 4], i + 4


def parse_lsda(data, off):
    i = off
    lps_enc = data[i]
    i += 1
    _lps, i = _read_enc(data, i, lps_enc)
    ttype_enc = data[i]
    i += 1
    # libc++abi reads classInfoOffset as a ULEB128 offset from the byte after
    # it (cxa_personality.cpp), NOT with ttypeEncoding.
    if ttype_enc != 0xff:
        _cioff, i = _uleb(data, i)
    cs_enc = data[i]
    i += 1
    n, i = _uleb(data, i)
    table = []
    end = i + n
    while i < end:
        start, i = _uleb(data, i)
        length, i = _uleb(data, i)
        lp, i = _uleb(data, i)
        action, i = _uleb(data, i)
        table.append((start, length, lp, action))
    return dict(lps_enc=lps_enc, ttype_enc=ttype_enc, cs_enc=cs_enc, table=table)


def parse(path):
    data = open(path, 'rb').read()
    secs = _sections(data)
    ex, et = secs['.ARM.exidx'], secs['.ARM.extab']
    funcs = sorted((v, sz, nm) for v, sz, nm, t in _dynsyms(data, secs)
                   if t == 2 and v)
    entries = []
    for i in range(ex['size'] // 8):
        eoff = ex['off'] + i * 8
        eaddr = ex['addr'] + i * 8
        fn, _ = _prel31(data, eoff, eaddr)
        w1, = struct.unpack_from("<I", data, eoff + 4)
        rec = dict(i=i, eaddr=eaddr, fn=fn, kind=None, model=None, extab=None)
        if w1 == 1:
            rec['kind'] = 'CANT'
        elif w1 & 0x80000000:
            rec['kind'] = 'INL'
            rec['model'] = (w1 >> 24) & 0xf
        else:
            rec['kind'] = 'EXT'
            tgt, _ = _prel31(data, eoff + 4, eaddr + 4)
            rec['extab'] = tgt
            if tgt:
                w, = struct.unpack_from("<I", data, et['off'] + (tgt - et['addr']))
                if w & 0x80000000:
                    rec['model'] = (w >> 24) & 0xf
                else:
                    rec['model'] = 'prel31'
        best = None
        for v, sz, nm in funcs:
            # .dynsym FUNC values are Thumb-tagged (odd); the exidx prel31
            # decodes to the even function address.
            if (v & ~1) <= fn:
                best = (v, sz, nm)
            else:
                break
        rec['name'] = best[2] if best else None
        rec['foff'] = fn - (best[0] & ~1) if best else None
        entries.append(rec)
    return dict(data=data, secs=secs, entries=entries)


def record_lengths(p):
    """Length of each EXT record = distance to the next extab target (packed in address order)."""
    ts = sorted(set(r['extab'] for r in p['entries'] if r['kind'] == 'EXT'))
    end0 = p['secs']['.ARM.extab']['addr'] + p['secs']['.ARM.extab']['size']
    return {t: (ts[i + 1] if i + 1 < len(ts) else end0) - t for i, t in enumerate(ts)}


def main(argv):
    if len(argv) == 4 and argv[1] == '--diff':
        a, b = parse(argv[2]), parse(argv[3])
        la, lb = record_lengths(a), record_lengths(b)
        print(f"entries: orig {len(a['entries'])} ours {len(b['entries'])}")
        for kind in ('CANT', 'INL', 'EXT'):
            ca = sum(1 for r in a['entries'] if r['kind'] == kind)
            cb = sum(1 for r in b['entries'] if r['kind'] == kind)
            print(f"  {kind:4s} orig {ca} ours {cb}")
        def seq(p):
            out = {}
            for r in p['entries']:
                nm = r['name'] or f"?{r['fn']:x}"
                out.setdefault(nm, []).append(
                    (r['kind'], r['model'], r['foff']))
            return out
        sa, sb = seq(a), seq(b)
        print("\nfunctions with differing kind sequence:")
        for nm in sorted(set(sa) | set(sb)):
            if sa.get(nm) != sb.get(nm):
                print(f"  {nm}: orig {sa.get(nm)} ours {sb.get(nm)}")
        def lens(p, l):
            out = {}
            for r in p['entries']:
                if r['kind'] == 'EXT':
                    nm = r['name'] or f"?{r['fn']:x}"
                    out.setdefault(nm, []).append(l[r['extab']])
            return out
        ea, eb = lens(a, la), lens(b, lb)
        print("\nfunctions with differing extab record length:")
        total = 0
        for nm in sorted(set(ea) | set(eb)):
            x, y = sum(ea.get(nm, [])), sum(eb.get(nm, []))
            if x != y:
                print(f"  {nm}: orig {x} ours {y} delta {y - x:+d}")
                total += y - x
        print(f"  total delta {total:+d} ({total:#x})")
        return 0
    p = parse(argv[1])
    print("#    eaddr      fn         function+off kind model extab")
    for r in p['entries']:
        print(f"{r['i']:4d} {r['eaddr']:08x} {r['fn']:08x} {r['name']}+{r['foff']} "
              f"{r['kind']:4s} {str(r['model']):6s} "
              f"{r['extab'] and format(r['extab'], '#x') or '-'}")
    return 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
