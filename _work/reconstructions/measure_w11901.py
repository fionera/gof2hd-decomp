#!/usr/bin/env python3
"""Fast ELF32 ARM measurement of one function vs the original .so.

Usage: measure.py <candidate.so> [symbol]
Prints: raw=<n> size=<ours>/<orig> text=<n> relplt=<md5>
raw = same-address differing bytes over the original function body (plus size delta).
"""
import sys, os, struct, hashlib

ORIG = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..', '..', '_work', 'bins', 'android_2.0.16_libgof2hdaa.so'))
SYM = '_ZN11LevelScript7processEi'
FUNC_ADDR = 0x137d18
FUNC_SIZE = 0xdf90

def sections(path):
    data = open(path, 'rb').read()
    e_shoff = struct.unpack_from('<I', data, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', data, 0x2e)[0]
    e_shnum = struct.unpack_from('<H', data, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', data, 0x32)[0]
    shstr_off = struct.unpack_from('<I', data, e_shoff + e_shstrndx*e_shentsize + 0x10)[0]
    shstr_size = struct.unpack_from('<I', data, e_shoff + e_shstrndx*e_shentsize + 0x14)[0]
    shstr = data[shstr_off:shstr_off+shstr_size]
    out = {}
    for i in range(e_shnum):
        base = e_shoff + i*e_shentsize
        nameoff = struct.unpack_from('<I', data, base)[0]
        end = shstr.find(b'\x00', nameoff)
        name = shstr[nameoff:end].decode()
        addr, off, size, link, entsize = struct.unpack_from('<IIIII', data, base+0x0c)
        out[name] = dict(addr=addr, off=off, size=size, link=link, entsize=entsize)
    return data, out

def dynsym(data, secs, sym):
    ds = secs['.dynsym']; dstr = secs['.dynstr']
    strtab = data[dstr['off']:dstr['off']+dstr['size']]
    n = ds['size']//16
    for i in range(n):
        base = ds['off'] + i*16
        st_name, st_value, st_size = struct.unpack_from('<III', data, base)
        if st_name == 0:
            continue
        end = strtab.find(b'\x00', st_name)
        name = strtab[st_name:end].decode()
        if name == sym:
            return st_value, st_size
    return None, None

def main():
    cand = sys.argv[1]
    sym = sys.argv[2] if len(sys.argv) > 2 else SYM
    odata, osecs = sections(ORIG)
    cdata, csecs = sections(cand)
    ot = osecs['.text']; ct = csecs['.text']
    oaddr, osz = dynsym(odata, osecs, sym)
    caddr, csz = dynsym(cdata, csecs, sym)
    if oaddr is None or caddr is None:
        print('missing symbol'); return 1
    # function body bytes (use symbol addr)
    ooff = ot['off'] + (oaddr - ot['addr'])
    coff = ct['off'] + (caddr - ct['addr'])
    ob = odata[ooff:ooff+osz]
    cb = cdata[coff:coff+csz]
    raw = sum(1 for a, b in zip(ob, cb) if a != b) + abs(len(ob)-len(cb))
    # whole .text diff
    otext = odata[ot['off']:ot['off']+ot['size']]
    ctext = cdata[ct['off']:ct['off']+ct['size']]
    textdiff = sum(1 for a, b in zip(otext, ctext) if a != b) + abs(len(otext)-len(ctext))
    rp = hashlib.md5(cdata[csecs['.rel.plt']['off']:csecs['.rel.plt']['off']+csecs['.rel.plt']['size']]).hexdigest()
    print('raw=%d size=%d/%d text=%d relplt=%s' % (raw, csz, osz, textdiff, rp))
    return 0

if __name__ == '__main__':
    sys.exit(main())
