"""Dump the ARM EHABI LSDA call-site table of a function (to see which calls are invokes
with landing pads). usage: lsda.py <so> <mangled>"""
import sys, struct
sys.path.insert(0, __file__.rsplit('/', 1)[0])
import armdis


def sec(so, name):
    import subprocess, re
    out = subprocess.run(['readelf', '-SW', so], capture_output=True, text=True).stdout
    for line in out.splitlines():
        m = re.search(r'\]\s+(\S+)\s+\S+\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)', line)
        if m and m.group(1) == name:
            return int(m.group(2), 16), int(m.group(3), 16), int(m.group(4), 16)


def prel31(addr, w):
    v = w & 0x7fffffff
    if v & 0x40000000:
        v -= 0x80000000
    return addr + v


def uleb(d, p):
    r = s = 0
    while True:
        b = d[p]; p += 1
        r |= (b & 0x7f) << s; s += 7
        if not b & 0x80:
            return r, p


def main(so, name):
    armdis.use_binary(so)
    d = armdis._data
    fst, fsz = armdis.func_range(name)
    va, off, size = sec(so, '.ARM.exidx')
    ent = None
    for k in range(0, size, 8):
        a = va + k
        w0, w1 = struct.unpack_from('<II', d, off + k)
        f = prel31(a, w0)
        if f == fst:
            ent = (a, w1)
    a, w1 = ent
    ex = prel31(a + 4, w1)  # extab address (vaddr == file offset in first LOAD)
    p = ex
    w = struct.unpack_from('<I', d, p)[0]
    if w & 0x80000000:
        print('compact model'); return
    pers = prel31(p, w); p += 4
    w = struct.unpack_from('<I', d, p)[0]
    nwords = (w >> 24) & 0xff  # generic model: unwind words
    p += 4 + 4 * nwords
    # LSDA
    lpstart_enc = d[p]; p += 1
    assert lpstart_enc == 0xff
    ttype_enc = d[p]; p += 1
    if ttype_enc != 0xff:
        _, p = uleb(d, p)
    cs_enc = d[p]; p += 1
    cs_len, p = uleb(d, p)
    end = p + cs_len
    rows = []
    while p < end:
        if cs_enc == 0x01:  # uleb128
            s, p = uleb(d, p); l, p = uleb(d, p); lp, p = uleb(d, p); act, p = uleb(d, p)
        elif cs_enc == 0x03:
            s, l, lp = struct.unpack_from('<III', d, p); p += 12; act, p = uleb(d, p)
        else:
            raise ValueError(hex(cs_enc))
        rows.append((fst + s, l, fst + lp if lp else 0, act))
    return rows


if __name__ == '__main__':
    for r in main(sys.argv[1], sys.argv[2]):
        print('%x +%d lp=%x act=%d' % r)
