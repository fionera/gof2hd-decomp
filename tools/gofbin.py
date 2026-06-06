#!/usr/bin/env python3
"""
gofbin.py — reader for Galaxy on Fire 2 data/bin/*.bin tables.

These are Java DataInputStream-style files (AbyssEngine's J2ME heritage): BIG-ENDIAN
integers, and strings as `u16 length + bytes` (Java writeUTF). The engine reads ints via
AEFile::ReadSwitched (byte-swapped = big-endian on the LE ARM/x86 builds).

Schemas reverse-engineered from the loaders in libgof2hdaa.so:
  names_*.bin     : u32 count, then count * (u16 len + UTF bytes)        [name pools]
  items.bin       : 233 records (FileRead::loadItemsBinary, ArraySetLength 0xe9), each:
                    arr1 = u32 n + n*i32  (blueprint ingredient item IDs)
                    arr2 = u32 n + n*i32  (blueprint ingredient counts)
                    arr3 = u32 n + n*i32  (ItemInfo: 30 ints -> ItemInfo struct fields)

Generic mode dumps any .bin as a best-effort int/string token stream.

Usage:
  gofbin.py names   <names_*.bin>
  gofbin.py items   <items.bin>   [--json]   # 233 records (FileRead::loadItemsBinary)
  gofbin.py systems <systems.bin> [--json]   # 34 records (FileRead::loadSystemsBinary)
  gofbin.py lang    <*.lang>      [--json]   # localized string table
  gofbin.py dump    <file.bin>               # generic big-endian int/UTF token stream
"""
import sys, struct, json, os

# ItemInfo field names (from KaamoClubModApi Game/structs.h, 30 ints)
ITEMINFO_FIELDS = [
    "field_0","m_nID","m_bIsThermo","m_nType","field_10","m_nSubType","field_18",
    "m_nTechLevel","m_nHighestPriceSystemId","m_nLowestPriceSystemId","field_28","field_2C",
    "field_30","field_34","field_38","field_3C","field_40","m_nPrice","m_nPropertyOne",
    "m_nPropertyOneValue","m_nPropertyTwo","m_nPropertyTwoValue","m_nPropertyThree",
    "m_nPropertyThreeValue","m_nPropertyFour","m_nPropertyFourValue","m_nPropertyFive",
    "m_nPropertyFiveValue","field_70","field_74",
]

class R:
    def __init__(self, data): self.d = data; self.p = 0
    def eof(self): return self.p >= len(self.d)
    def u8(self):  v = self.d[self.p]; self.p += 1; return v
    def u16(self): v = struct.unpack_from('>H', self.d, self.p)[0]; self.p += 2; return v
    def i32(self): v = struct.unpack_from('>i', self.d, self.p)[0]; self.p += 4; return v
    def u32(self): v = struct.unpack_from('>I', self.d, self.p)[0]; self.p += 4; return v
    def utf(self):
        n = self.u16(); s = self.d[self.p:self.p+n]; self.p += n
        return s.decode('utf-8', 'replace')
    def i32arr(self):
        n = self.i32()
        return [self.i32() for _ in range(n)] if n > 0 else []

def names(path):
    r = R(open(path, 'rb').read()); n = r.u32(); out = [r.utf() for _ in range(n)]
    print("%s: %d names" % (os.path.basename(path), n)); print(out); return out

def items(path, as_json=False):
    r = R(open(path, 'rb').read()); recs = []
    for i in range(0xe9):  # 233
        if r.eof(): print("  [EOF after %d records, offset %d/%d]" % (i, r.p, len(r.d))); break
        ing = r.i32arr(); cnt = r.i32arr(); info = r.i32arr()
        rec = {"index": i, "blueprint_ingredient_ids": ing, "blueprint_counts": cnt}
        rec["iteminfo"] = {ITEMINFO_FIELDS[j] if j < len(ITEMINFO_FIELDS) else "x%d" % j: v
                           for j, v in enumerate(info)}
        recs.append(rec)
    if as_json:
        print(json.dumps(recs, indent=2)); return recs
    print("%s: %d items (bytes consumed %d/%d)" % (os.path.basename(path), len(recs), r.p, len(r.d)))
    for rec in recs[:8]:
        ii = rec["iteminfo"]
        print("  item %3d  id=%s type=%s subtype=%s tech=%s price=%s  ingredients=%s"
              % (rec["index"], ii.get("m_nID"), ii.get("m_nType"), ii.get("m_nSubType"),
                 ii.get("m_nTechLevel"), ii.get("m_nPrice"), rec["blueprint_ingredient_ids"]))
    print("  ... (use --json for all)"); return recs

def systems(path, as_json=False):
    """systems.bin: 34 records (FileRead::loadSystemsBinary). Each = UTF name + 8 i32 +
    4 length-prefixed i32 arrays (station ids / linked systems / etc.)."""
    r = R(open(path, 'rb').read()); recs = []
    for i in range(34):
        if r.eof(): print("  [EOF after %d records, %d/%d]" % (i, r.p, len(r.d))); break
        name = r.utf()
        ints = [r.i32() for _ in range(8)]
        arrs = [r.i32arr() for _ in range(4)]
        recs.append({"index": i, "name": name, "ints": ints, "arrays": arrs})
    if as_json: print(json.dumps(recs, ensure_ascii=False, indent=1)); return recs
    print("%s: %d systems (consumed %d/%d)" % (os.path.basename(path), len(recs), r.p, len(r.d)))
    for rec in recs[:12]:
        print("  [%2d] %-16s ints=%s arr_lens=%s" % (rec["index"], rec["name"], rec["ints"],
                                                     [len(a) for a in rec["arrays"]]))
    return recs

def lang(path, as_json=False):
    """.lang = flat sequence of Java writeUTF strings (u16 len + UTF-8). Localized text table."""
    r = R(open(path, 'rb').read()); out = []
    while r.p + 2 <= len(r.d):
        try: out.append(r.utf())
        except Exception: break
    if as_json: print(json.dumps(out, ensure_ascii=False, indent=1)); return out
    print("%s: %d strings (consumed %d/%d)" % (os.path.basename(path), len(out), r.p, len(r.d)))
    for i, s in enumerate(out[:24]):
        print("  [%d] %r" % (i, s))
    print("  ..."); return out

def dump(path):
    """Best-effort: print big-endian i32 tokens; flag plausible UTF strings."""
    r = R(open(path, 'rb').read()); n = 0
    while not r.eof() and n < 200:
        save = r.p
        if r.p + 2 <= len(r.d):
            ln = struct.unpack_from('>H', r.d, r.p)[0]
            if 0 < ln < 40 and r.p + 2 + ln <= len(r.d):
                s = r.d[r.p+2:r.p+2+ln]
                if all(32 <= b < 127 for b in s):
                    print("  @%05d str(%d) %r" % (r.p, ln, s.decode())); r.p += 2 + ln; n += 1; continue
        if r.p + 4 <= len(r.d):
            print("  @%05d i32 %d" % (r.p, struct.unpack_from('>i', r.d, r.p)[0])); r.p += 4; n += 1
        else: break

if __name__ == '__main__':
    if len(sys.argv) < 3: print(__doc__); sys.exit(1)
    cmd, path = sys.argv[1], sys.argv[2]
    if cmd == 'names': names(path)
    elif cmd == 'items': items(path, '--json' in sys.argv)
    elif cmd == 'systems': systems(path, '--json' in sys.argv)
    elif cmd == 'lang': lang(path, '--json' in sys.argv)
    elif cmd == 'dump': dump(path)
    else: print(__doc__)
