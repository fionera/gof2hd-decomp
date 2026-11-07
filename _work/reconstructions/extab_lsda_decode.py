#!/usr/bin/env python3
"""Side-by-side decoder for differing `.ARM.extab` LSDA records.

Usage:
  python3 extab_lsda_decode.py <original.so> <ours.so>

Generic-personality record layout (ARM EHABI):
  [4-byte prel31 personality]
  [unwind opcodes: (SIZE+1) 32-bit words]
  [LSDA: LPStart enc, TType enc, ULEB class-info offset,
         call-site enc, ULEB call-site length, call-site rows]

The unwind opcode block is emitted MSB-to-LSB per 32-bit word, so the SIZE
byte is at record offset 7.  The LSDA therefore starts at
`record + 4 + (SIZE+1)*4` (offset 12 for the usual SIZE==1 records).
"""
import collections
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
    return place + v


def uleb(data, off):
    r = s = 0
    while True:
        b = data[off]
        off += 1
        r |= (b & 0x7f) << s
        if not (b & 0x80):
            return r, off
        s += 7


def sleb(data, off):
    r = s = 0
    b = 0
    while True:
        b = data[off]
        off += 1
        r |= (b & 0x7f) << s
        s += 7
        if not (b & 0x80):
            break
    if b & 0x40:
        r -= 1 << s
    return r, off


def enc_val(data, off, enc):
    if enc == 0xff:
        return 0, off
    fmt = enc & 0x0f
    if fmt == 0x00:
        v, = struct.unpack_from("<I", data, off)
        return v, off + 4
    if fmt == 0x01:
        return uleb(data, off)
    if fmt == 0x02:
        v, = struct.unpack_from("<H", data, off)
        return v, off + 2
    if fmt == 0x03:
        v, = struct.unpack_from("<I", data, off)
        return v, off + 4
    if fmt == 0x04:
        v, = struct.unpack_from("<Q", data, off)
        return v, off + 8
    if fmt == 0x09:
        return sleb(data, off)
    if fmt == 0x0a:
        v, = struct.unpack_from("<h", data, off)
        return v, off + 2
    if fmt == 0x0b:
        v, = struct.unpack_from("<i", data, off)
        return v, off + 4
    if fmt == 0x0c:
        v, = struct.unpack_from("<q", data, off)
        return v, off + 8
    raise ValueError(hex(enc))


def decode_lsda(data, secs, recaddr, reclen):
    et = secs[".ARM.extab"]
    off = et["off"] + (recaddr - et["addr"])
    w0, = struct.unpack_from("<I", data, off)
    if w0 & 0x80000000:
        return dict(kind="compact", model=(w0 >> 24) & 0xf, rows=[])
    size = data[off + 7]
    lsda = off + 4 + (size + 1) * 4
    i = lsda
    lps_enc = data[i]
    i += 1
    _, i = enc_val(data, i, lps_enc)
    tt_enc = data[i]
    i += 1
    tt_off = 0
    if tt_enc != 0xff:
        tt_off, i = uleb(data, i)
    cs_enc = data[i]
    i += 1
    n, i = uleb(data, i)
    end = i + n
    rows = []
    while i < end:
        start, i = enc_val(data, i, cs_enc)
        length, i = enc_val(data, i, cs_enc)
        lp, i = enc_val(data, i, cs_enc)
        action, i = uleb(data, i)
        rows.append((start, length, lp, action))
    return dict(kind="generic", personality=prel31(data, off, recaddr),
                unwind_words=size + 1, lpstart_enc=lps_enc,
                ttype_enc=tt_enc, ttype_off=tt_off, cs_enc=cs_enc,
                cst_len=n, rows=rows, reclen=reclen)


def parse(path):
    data = open(path, "rb").read()
    secs = sections(data)
    ex, et = secs[".ARM.exidx"], secs[".ARM.extab"]
    entries = []
    for i in range(ex["size"] // 8):
        eoff = ex["off"] + i * 8
        eaddr = ex["addr"] + i * 8
        fn = prel31(data, eoff, eaddr)
        w1, = struct.unpack_from("<I", data, eoff + 4)
        rec = dict(i=i, fn=fn, w1=w1)
        if w1 == 1:
            rec["kind"] = "CANT"
        elif w1 & 0x80000000:
            rec["kind"] = "INL"
        else:
            rec["kind"] = "EXT"
            rec["extab"] = prel31(data, eoff + 4, eaddr + 4)
        entries.append(rec)
    ts = sorted(set(r["extab"] for r in entries if r["kind"] == "EXT"))
    end0 = et["addr"] + et["size"]
    lens = {t: (ts[i + 1] if i + 1 < len(ts) else end0) - t
            for i, t in enumerate(ts)}
    for r in entries:
        if r["kind"] == "EXT":
            r["reclen"] = lens[r["extab"]]
    return data, secs, entries


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    da, sa, ea = parse(argv[1])
    db, sb, eb = parse(argv[2])
    ga, gb = collections.defaultdict(list), collections.defaultdict(list)
    for r in ea:
        if r["kind"] == "EXT":
            ga[r["fn"]].append(r)
    for r in eb:
        if r["kind"] == "EXT":
            gb[r["fn"]].append(r)
    for fn in sorted(ga):
        la, lb = ga[fn], gb.get(fn, [])
        for k in range(min(len(la), len(lb))):
            ra, rb = la[k], lb[k]
            oa = da[sa[".ARM.extab"]["off"] +
                    (ra["extab"] - sa[".ARM.extab"]["addr"]):][:ra["reclen"]]
            ob = db[sb[".ARM.extab"]["off"] +
                    (rb["extab"] - sb[".ARM.extab"]["addr"]):][:rb["reclen"]]
            if oa == ob:
                continue
            ia = decode_lsda(da, sa, ra["extab"], ra["reclen"])
            ib = decode_lsda(db, sb, rb["extab"], rb["reclen"])
            nd = sum(1 for p, q in zip(oa, ob) if p != q)
            print(f"=== fn {fn:#x}  reclen {ra['reclen']} diffbytes {nd}")
            if ia["kind"] != "generic" or ib["kind"] != "generic":
                print(f"  orig {ia['kind']} ours {ib['kind']}")
                continue
            print(f"  orig pers {ia['personality']:#x} uw {ia['unwind_words']}w "
                  f"lp {ia['lpstart_enc']:#x} tt {ia['ttype_enc']:#x}/"
                  f"{ia['ttype_off']} cst {ia['cs_enc']:#x} len {ia['cst_len']} "
                  f"rows {len(ia['rows'])}")
            print(f"  ours pers {ib['personality']:#x} uw {ib['unwind_words']}w "
                  f"lp {ib['lpstart_enc']:#x} tt {ib['ttype_enc']:#x}/"
                  f"{ib['ttype_off']} cst {ib['cs_enc']:#x} len {ib['cst_len']} "
                  f"rows {len(ib['rows'])}")
            for i in range(max(len(ia["rows"]), len(ib["rows"]))):
                a = ia["rows"][i] if i < len(ia["rows"]) else None
                b = ib["rows"][i] if i < len(ib["rows"]) else None
                mark = "  " if a == b else "**"
                print(f"  {mark} {i:3d} orig {a}  ours {b}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
