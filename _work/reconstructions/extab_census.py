#!/usr/bin/env python3
"""Extab census v2: LSDA rows annotated with the calls they protect.

For every differing .ARM.extab owner, prints the call-site rows of both
binaries, the call targets inside each row's [start,start+len) range, and a
structural classification:

  LP-REMAP   same range, landing pad differs
  SHIFT      same call(s), range start/end moved by code layout
  SPLIT      one orig row  -> two+ ours rows  (same call span)
  MERGE      two+ orig rows -> one ours row
  ROW        unmatched row
"""
import collections
import struct
import subprocess
import sys

OWNERS = {
    0x1490d8: "HangarWindow::render",
    0xaffd8: "Level::createMission",
    0x137d18: "LevelScript::process",
    0xc0214: "Level::createShip",
    0x12e050: "Radar::draw",
    0x163b90: "Hud::draw",
    0x17a144: "MGame::OnTouchEnd",
    0x971b8: "Generator::createMission",
    0x170d38: "SpaceLounge::drawLounge",
    0x12d484: "Radar::Radar",
    0xd52d0: "ModStation::ModStation",
}


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
        b = data[off]; off += 1
        r |= (b & 0x7f) << s
        if not (b & 0x80):
            return r, off
        s += 7


def sleb(data, off):
    r = s = 0
    b = 0
    while True:
        b = data[off]; off += 1
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
        return struct.unpack_from("<I", data, off)[0], off + 4
    if fmt == 0x01:
        return uleb(data, off)
    if fmt == 0x02:
        return struct.unpack_from("<H", data, off)[0], off + 2
    if fmt == 0x03:
        return struct.unpack_from("<I", data, off)[0], off + 4
    if fmt == 0x04:
        return struct.unpack_from("<Q", data, off)[0], off + 8
    if fmt == 0x09:
        return sleb(data, off)
    if fmt == 0x0a:
        return struct.unpack_from("<h", data, off)[0], off + 2
    if fmt == 0x0b:
        return struct.unpack_from("<i", data, off)[0], off + 4
    if fmt == 0x0c:
        return struct.unpack_from("<q", data, off)[0], off + 8
    raise ValueError(hex(enc))


def decode_lsda(data, secs, recaddr):
    et = secs[".ARM.extab"]
    off = et["off"] + (recaddr - et["addr"])
    w0, = struct.unpack_from("<I", data, off)
    if w0 & 0x80000000:
        return dict(kind="compact", rows=[])
    size = data[off + 7]
    i = off + 4 + (size + 1) * 4
    lps_enc = data[i]; i += 1
    _, i = enc_val(data, i, lps_enc)
    tt_enc = data[i]; i += 1
    if tt_enc != 0xff:
        _, i = uleb(data, i)
    cs_enc = data[i]; i += 1
    n, i = uleb(data, i)
    end = i + n
    rows = []
    while i < end:
        start, i = enc_val(data, i, cs_enc)
        length, i = enc_val(data, i, cs_enc)
        lp, i = enc_val(data, i, cs_enc)
        action, i = uleb(data, i)
        rows.append((start, length, lp, action))
    return dict(kind="generic", rows=rows, cs_enc=cs_enc)


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
        rec = dict(fn=fn, w1=w1)
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
            off = et["off"] + (r["extab"] - et["addr"])
            r["recbytes"] = data[off:off + r["reclen"]]
    return data, secs, entries


def disasm(objdump, path, base, size):
    out = subprocess.run([objdump, "-d", "--start-address=%#x" % base,
                          "--stop-address=%#x" % (base + size), path],
                         capture_output=True, text=True).stdout
    ins = []
    for line in out.splitlines():
        line = line.strip()
        if not line or ":" not in line:
            continue
        a, _, rest = line.partition(":")
        try:
            addr = int(a, 16)
        except ValueError:
            continue
        if not (base <= addr < base + size):
            continue
        parts = rest.split("\t")
        mnem = " ".join(x.strip() for x in parts[2:] if x.strip())
        ins.append((addr - base, mnem))
    return ins


def calls_in(ins, start, length):
    out = []
    for off, mnem in ins:
        if start <= off < start + length:
            m = mnem.split()
            if m and m[0] in ("bl", "blx", "blne", "bleq", "blt", "bls"):
                tgt = m[-1]
                out.append(tgt)
    return out


def classify(rows_a, rows_b, ins_a, ins_b):
    """Greedy structural alignment on (call-target list, range span)."""
    res = []
    i = j = 0
    na, nb = len(rows_a), len(rows_b)
    while i < na and j < nb:
        ra, rb = rows_a[i], rows_b[j]
        ca = calls_in(ins_a, ra[0], ra[1])
        cb = calls_in(ins_b, rb[0], rb[1])
        if ra[0] == rb[0] and ra[1] == rb[1]:
            res.append((i, j, "LP-REMAP" if ra[2] != rb[2] else "SAME", ra, rb, ca, cb))
            i += 1; j += 1
            continue
        # try to find a match: same call-target list, near span
        if ca == cb:
            # same calls, range moved
            res.append((i, j, "SHIFT", ra, rb, ca, cb))
            i += 1; j += 1
            continue
        # try split: orig row i covers calls of ours j..j+k
        matched = False
        for k in (2, 3):
            if j + k <= nb:
                comb = []
                for t in range(k):
                    comb += calls_in(ins_b, rows_b[j + t][0], rows_b[j + t][1])
                if comb == ca:
                    res.append((i, j, "SPLIT->%d" % k, ra, rb, ca, comb))
                    i += 1; j += k
                    matched = True
                    break
        if matched:
            continue
        for k in (2, 3):
            if i + k <= na:
                comb = []
                for t in range(k):
                    comb += calls_in(ins_a, rows_a[i + t][0], rows_a[i + t][1])
                if comb == cb:
                    res.append((i, j, "MERGE<-%d" % k, ra, rb, comb, cb))
                    i += k; j += 1
                    matched = True
                    break
        if matched:
            continue
        res.append((i, j, "ROW", ra, rb, ca, cb))
        i += 1; j += 1
    while i < na:
        res.append((i, j, "EXTRA-ORIG", rows_a[i], None,
                    calls_in(ins_a, rows_a[i][0], rows_a[i][1]), None))
        i += 1
    while j < nb:
        res.append((i, j, "EXTRA-OURS", None, rows_b[j],
                    None, calls_in(ins_b, rows_b[j][0], rows_b[j][1])))
        j += 1
    return res


def main(argv):
    if len(argv) < 4:
        print(__doc__)
        return 2
    orig, ours, objdump = argv[1], argv[2], argv[3]
    da, sa, ea = parse(orig)
    db, sb, eb = parse(ours)
    ga = {r["fn"]: r for r in ea if r["kind"] == "EXT"}
    gb = {r["fn"]: r for r in eb if r["kind"] == "EXT"}
    # symbol sizes
    def sizes(path):
        out = subprocess.run([objdump, "-T", path], capture_output=True,
                             text=True).stdout
        d = {}
        for line in out.splitlines():
            p = line.split()
            if len(p) >= 6:
                try:
                    d[int(p[0], 16)] = int(p[4], 16)
                except ValueError:
                    pass
        return d
    sza, szb = sizes(orig), sizes(ours)
    for fn in sorted(OWNERS):
        ra, rb = ga.get(fn), gb.get(fn)
        if not ra or not rb:
            continue
        if ra["recbytes"] == rb["recbytes"]:
            continue
        name = OWNERS[fn]
        ia = decode_lsda(da, sa, ra["extab"])
        ib = decode_lsda(db, sb, rb["extab"])
        nd = sum(1 for p, q in zip(ra["recbytes"], rb["recbytes"]) if p != q)
        ins_a = disasm(objdump, orig, fn, sza.get(fn, 0))
        ins_b = disasm(objdump, ours, fn, szb.get(fn, 0))
        # raw same-address body diff
        size = min(sza.get(fn, 0), szb.get(fn, 0))
        da_ = open(orig, "rb").read()
        db_ = open(ours, "rb").read()
        sec_a = sections(da_); sec_b = sections(db_)
        ta = sec_a[".text"]; tb = sec_b[".text"]
        ba = da_[ta["off"] + (fn - ta["addr"]):][:size]
        bb = db_[tb["off"] + (fn - tb["addr"]):][:size]
        raw = sum(1 for p, q in zip(ba, bb) if p != q)
        # LP delta histogram for same-range rows
        deltas = collections.Counter()
        for x, y in zip(ia["rows"], ib["rows"]):
            if x[0] == y[0] and x[1] == y[1] and x[2] != y[2]:
                deltas[y[2] - x[2]] += 1
        print("=" * 100)
        print("fn %#x  %s   reclen %d  diffbytes %d  bodyraw %d  rows %d/%d  lp-deltas %s"
              % (fn, name, ra["reclen"], nd, raw, len(ia["rows"]), len(ib["rows"]),
                 dict(sorted(deltas.items()))))
        print("  %-3s %-28s %-28s %-10s %s"
              % ("#", "orig (start,len,lp)", "ours (start,len,lp)", "class", "calls"))
        res = classify(ia["rows"], ib["rows"], ins_a, ins_b)
        for (i, j, kind, ra_, rb_, ca, cb) in res:
            if kind == "SAME":
                continue
            sa_ = "(%d,%d,%d)" % ra_[:3] if ra_ else "-"
            sb_ = "(%d,%d,%d)" % rb_[:3] if rb_ else "-"
            def short(lst):
                if not lst:
                    return ""
                if len(lst) > 4:
                    return ",".join(lst[:4]) + ",+%d" % (len(lst) - 4)
                return ",".join(lst)
            calls = []
            if ca:
                calls.append("O:" + short(ca))
            if cb and cb != ca:
                calls.append("U:" + short(cb))
            print("  %-3d %-28s %-28s %-10s %s"
                  % (i, sa_, sb_, kind, " ".join(calls)))
        c = collections.Counter(r[2] for r in res if r[2] != "SAME")
        print("  summary:", dict(c))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
