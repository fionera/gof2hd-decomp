# `.ARM.exidx` duplicate-address entries — root cause (w3565)

Status: research note (2026-11-01). Worktree `wt/w3565`, branch `wave87-w3565`,
based on master `113737ee`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2,
`-Oz`, gold 1.12, image base 0x10000). Every number below is from a command run in
this tree or from the original binary. Scratch files are under `/tmp`; the worktree
is clean apart from this note.

## TL;DR

The original `.ARM.exidx` has **194 duplicate-address groups** (3566 entries,
3372 distinct function addresses). They are not duplicate function definitions or
symbol aliases. There are exactly two mechanisms, and both are already reproduced
by this tree:

* **193 groups: `CANTUNWIND` + next-function.** Every `-ffunction-sections` object
  still carries an empty plain `.text` section. Gold's `fix_exidx_coverage` emits
  an `EXIDX_CANTUNWIND` for that empty section, and the CANT's address is **the
  end of the last text section that had an `.ARM.exidx` companion** — not the
  empty section's own address (gold 1.12 `Arm_exidx_cantunwind::do_fixed_endian_write`:
  `output_address = section_start + section_size`). At a TU boundary with no
  alignment padding, that end address equals the next TU's first function address,
  so the CANT and the next function's record share an address.
* **1 group: zero-size function + next function.** `AbyssEngine::getAppVersion` is
  a zero-size function section (the original's body was empty; our source uses
  `[[gnu::noreturn]]` to reproduce it). Unlike the empty plain `.text`, clang emits
  an `.ARM.exidx` companion for it, so its record is an inline (`INL`) entry at the
  section address, which equals the next function's address (`Engine::InitGL`).

The 193 CANTs are **load-bearing**: gold merges adjacent identical inline entries,
and a CANT between two identical INLs prevents that merge. Removing a marker
therefore removes *two* entries, not one (measured below).

## Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Measured section sizes (original vs ours):

| section | original | ours |
|---|---:|---:|
| `.text` | `0x1337c8` @ `0x69440` | `0x1337c8` @ `0x69440` |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | `0x6f70` @ `0x19cc08` |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | `0x26290` @ `0x1a3b78` |
| `.rodata` | `0x41b67` @ `0x1c9e10` | `0x41b67` @ `0x1c9e10` |

The `.ARM.extab` `-8` is a separate, pre-existing record-length delta (the
`exidx_table.py --diff` record deltas now sum to `-8`, not the `0` stated in
`docs/IDENTICALITY.md`); it is not caused by, and does not affect, the duplicate
addresses. The `.ARM.exidx` entry count and kind sequence are exact.

## 1. Duplicate census (original vs ours)

Tool: a small parser over the raw `.ARM.exidx` words (same prel31/kind logic as
`_work/reconstructions/exidx_table.py`). It reads word0 as prel31(function) and
classifies word1 as `CANT` (`==1`), `INL` (bit31 set), or `EXT` (prel31 to
`.ARM.extab`).

```sh
python3 /tmp/w3565_exidx_summary.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

```
../_work/bins/android_2.0.16_libgof2hdaa.so: exidx=0x6f70 entries=3566 CANT=272 INL=856 EXT=2438 dup_groups=194 dup_patterns={('CANT','INL'):47, ('CANT','EXT'):146, ('INL','EXT'):1}
cmake-build-match/libgof2hdaa.so:            exidx=0x6f70 entries=3566 CANT=272 INL=856 EXT=2438 dup_groups=194 dup_patterns={('CANT','INL'):47, ('CANT','EXT'):146, ('INL','EXT'):1}
```

A direct sequence comparison confirms the structure is already exact:

```sh
python3 /tmp/cmp_exidx.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

```
entries 3566 3566
same fn addr 3566 same kind 3566 same w1 1236
all fn+kind sequences identical
```

The 194 duplicate addresses and their kind patterns are the same set in both
files (`set(orig_dups) == set(ours_dups)` and the kind tuples are equal). The
only differing words are the `EXT` prel31 extab pointers and the `INL` model
words, i.e. address content, not table structure.

The one non-CANT duplicate is entry 90/91:

```
i=90 fn=0x06db20 INL w1=0x80b0b0b0
i=91 fn=0x06db20 EXT w1=0x00006ff0
```

## 2. Map correlation: what is at each duplicate address

Relink the current objects with a gold map file (only `-o` and `--Map` change;
the `strip` step is dropped):

```sh
# extract the COMMAND from cmake-build-match/build.ninja, replace
#   -o .../libgof2hdaa.so  with  --Map=/tmp/w3565_ours.map -o /tmp/w3565_ours_map.so
# and drop the trailing " && ...strip --strip-all ..."
bash /tmp/w3565_link_map.sh
```

The map gives the address/size of every `.text.*` input section. Correlating it
with the original's 272 CANT addresses (the original's fn+kind sequence is
identical to ours, so our map is a valid witness for it) gives:

| trigger section at/after the CANT | duplicate? | gap to trigger | count |
|---|---|---|---:|
| `.text.tuend.*` (empty plain `.text` marker) | **yes** | 0 | **193** |
| `.text.tuend.*` | no | >0 (alignment padding) | 71 |
| `.text.tuend.*` | no | 0 (next section has no exidx, or starts +4) | 4 |
| cold/plain `.text` (`crtbegin`, `__clang_call_terminate`) | no | 0 | 2 |
| plain `.text` (libgcc/crtend) | no | 0 or >0 | 2 |
| **total CANTs** | | | **272** |

For all 193 duplicate CANTs the CANT address is exactly the end of the nearest
preceding non-zero `.text` section, and the `.text.tuend.*` marker is at the same
address as the next function. The marker belongs to a different object than the
next function in **193/193** cases — they are all inter-TU boundaries:

```
0x69588 o000_BackButton.c.obj        -> o001_NFC.cpp.obj
0x69e5c o001_NFC.cpp.obj             -> o002_VirtualInput.cpp.obj
0x6b2a0 o002_VirtualInput.cpp.obj    -> o003_BlowfishSubkeyFile.c.obj
0x6b550 o003_BlowfishSubkeyFile.c.obj-> o004_main.c.obj
...
```

The 71 "solo" markers prove the CANT address is *not* the marker's address: e.g.
the `EaseInOut.cpp` marker is at `0x719e0` (4-byte aligned) but the CANT is at
`0x719de`, the end of `EaseInOut::GetMaxValue` (`0x719cc + 0x12`). The next
function `AERandom::AERandom` starts at `0x719e0`.

The single non-CANT duplicate is intra-object:

```
0x6db20 sections:
  .text._ZN11AbyssEngine13getAppVersionEv   0x0    o007_GLES1Compat.cpp.obj
  .text._ZN11AbyssEngine6Engine6InitGLEbii  0x198  o007_GLES1Compat.cpp.obj
```

The object file confirms that the zero-size function has its own exidx companion:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -h cmake-build-match/exidx_markers/objects/o007_GLES1Compat.cpp.obj | grep -E 'getAppVersion|InitGL'
 56 .text._ZN11AbyssEngine13getAppVersionEv 00000000 ... 2**1
 57 .ARM.exidx.text._ZN11AbyssEngine13getAppVersionEv 00000008 ... 2**2
 58 .text._ZN11AbyssEngine6Engine6InitGLEbii 00000198 ... 2**2
 59 .ARM.extab.text._ZN11AbyssEngine6Engine6InitGLEbii 00000030 ...
 60 .ARM.exidx.text._ZN11AbyssEngine6Engine6InitGLEbii 00000008 ...
$OD -s -j .ARM.exidx.text._ZN11AbyssEngine13getAppVersionEv \
  cmake-build-match/exidx_markers/objects/o007_GLES1Compat.cpp.obj
 0000 00000000 b0b0b080
```

`00000000 b0b0b080` is prel31 offset 0 (function address = section address) plus
inline model 0 (`0x80b0b0b0`), i.e. the `INL` entry in the duplicate pair.

## 3. Gold's algorithm (why the CANT address is the previous function's end)

The original used gold 1.12 (`.note.gnu.gold-version`). The relevant code is
unchanged in the binutils 2.27 branch (gold 1.12):

```sh
curl -sL 'https://sourceware.org/git/?p=binutils-gdb.git;a=blob_plain;f=gold/arm.cc;hb=binutils-2_27-branch' \
  -o /tmp/w3565_arm227.cc
```

`gold/arm.cc` (binutils-2_27-branch):

* lines 5530-5546, `Arm_exidx_fixup::add_exidx_cantunwind_as_needed()`:
  "The new EXIDX_CANTUNWIND entry points to the end of the last seen EXIDX
  section."
* lines 5336-5373, `Arm_exidx_cantunwind<big_endian>::do_fixed_endian_write()`:
  `section_start = os->address() + output_offset;`
  `section_size = exidx_input_section->text_size();`
  `output_address = section_start + section_size;`
* lines 5555-5580, `Arm_exidx_fixup::process_exidx_entry()`: consecutive
  `EXIDX_CANTUNWIND` entries are merged unconditionally; consecutive *identical*
  inline entries are merged when `--merge-exidx-entries` (the default) is on.

So the sequence is:

1. clang emits one `.text.<mangled>` + one `.ARM.exidx.text.<mangled>` per
   function, plus an empty plain `.text` in every object (`-ffunction-sections`).
2. The empty plain `.text` has no exidx companion. Gold walks the sorted text
   sections; when it reaches that empty section it calls
   `add_exidx_cantunwind_as_needed()`.
3. That emits a CANT at the end of the **last text section that did have an
   exidx companion** (the previous function), not at the empty section's address.
4. If the previous function ends exactly where the next TU's first function
   begins, the CANT and the next function's exidx record have the same address:
   the duplicate.

Our build reproduces this by renaming each empty plain `.text` to a unique
`.text.tuend.*` marker (`tools/gen_exidx_markers.py`) and placing it at the TU end
(`tools/gen_section_order.py`). The marker is what makes the empty section sort
at the right point; its own address is irrelevant to the CANT address (the 71
padding cases prove this).

## 4. Controlled A/B rebuilds

All variants change only `tools/gen_exidx_markers.py`'s `DROP_EMPTY_TEXT` set,
rebuild, and are reverted. No baseline update.

| variant | `.ARM.exidx` | entries | CANT | INL | EXT | dup groups | dup patterns | byte | linked | `.text` | `.rodata` |
|---|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|---:|
| **baseline** | **`0x6f70`** | **3566** | **272** | **856** | **2438** | **194** | 47/146/1 | **4496** | **4496** | `0x1337c8` | `0x1c9e10` |
| drop `BackButton.c.obj` marker | `0x6f60` | 3564 | 271 | 855 | 2438 | 193 | 46/146/1 | 4040 | 4496 | `0x1337c8` | `0x1c9e00` |
| keep `Vector.cpp.obj` marker | `0x6f78` | 3567 | 273 | 856 | 2438 | 195 | 47/147/1 | 4496 | 4496 | `0x1337c8` | `0x1c9e10` |

### 4a. Drop one marker (`BackButton.c.obj`)

`BackButton.c.obj` owns the duplicate at `0x69588` (CANT + `NFC::NFC` INL).
Adding it to `DROP_EMPTY_TEXT` removes its empty `.text` entirely:

```
verify: byte 4040  linked 4496 ...   FAIL byte_exact 4496 -> 4040
.ARM.exidx 0x6f60, 3564 entries, CANT 271, INL 855, EXT 2438, dup 193
```

The CANT at `0x69588` is gone. The following `INL` at `0x69588` is also gone:
without the CANT between them, gold merges it into the previous identical
`INL` at `0x69578` (`0x80b0b0b0`). That is why the entry count drops by 2 and the
duplicate count by 1. This is direct proof that the CANT is produced by the
empty-section marker and that it protects the following INL from merging.

### 4b. Keep one marker that the tree currently drops (`Vector.cpp.obj`)

Removing `Vector.cpp.obj` from `DROP_EMPTY_TEXT` adds one empty `.text` marker at
`0x74000`, the end of `AEMath::BSphere::Merge` (`0x73f30 + 0xd0`) and the start
of `BSphere.cpp`'s first function:

```
verify: byte 4496  linked 4496 ...   RATCHET PASS
.ARM.exidx 0x6f78, 3567 entries, CANT 273, INL 856, EXT 2438, dup 195
```

The new exidx sequence around the marker is:

```
orig:   289 0x73f30 EXT   290 0x74000 EXT
variant:289 0x73f30 EXT   290 0x74000 CANT   291 0x74000 EXT
```

One empty `.text` marker produces exactly one CANT, and because the previous
function ends at the next function's start, it produces exactly one new
duplicate-address group (`CANT` + `EXT`). Note that `byte_exact` stays 4496 and
the ratchet passes even though `.ARM.exidx` is now `0x6f78` (3567 entries): the
gate does not check exidx structure. The correct tree is the baseline with the
marker dropped.

## 5. Proven vs hypothesis

Proven by measurement in this tree:

* The original has 194 duplicate-address groups: 47 `CANT`+`INL`, 146
  `CANT`+`EXT`, 1 `INL`+`EXT` (3566 entries, 272 CANT, 856 INL, 2438 EXT).
* Our tree reproduces the original's complete fn-address + kind sequence
  (3566/3566 identical) and the same 194 duplicate address set.
* All 193 `CANT` duplicates are `.text.tuend.*` empty-`.text` markers at
  inter-object (TU) boundaries; the CANT address is the end of the previous
  function and equals the next function's address.
* The 1 `INL`+`EXT` duplicate is the zero-size `AbyssEngine::getAppVersion`
  function section, which has its own `.ARM.exidx` companion (`INL` model 0) and
  shares its address with `Engine::InitGL`.
* Gold 1.12 emits the CANT at `section_start + section_size` of the last
  exidx-covered section, not at the empty section's address (71 padding cases
  where the marker and CANT addresses differ).
* Dropping one marker removes one duplicate and two entries (the CANT and the
  following INL, which then merges); keeping one marker adds one CANT and one
  duplicate.

Hypothesis / still open:

* The original's TU boundaries are exactly the 193 duplicate addresses. Our
  build's TU boundaries reproduce the original's exidx fn+kind sequence exactly,
  which is strong evidence, but the original's input sections are not directly
  observable; the claim rests on the exact sequence match plus the map/A-B
  mechanism.
* The `.ARM.extab` `-8` (ours `0x26290` vs original `0x26298`) is an independent
  record-length delta (`ArrayAdd<KIPlayer> -8`, `LevelScript::~LevelScript +4`,
  `HangarWindow::render3D -20`, `RecordHandler::readWanted -16`,
  `Hud::drawChallengeModeScore +16`, `StarMap::drawOnScreenInfo +12`,
  `Generator::createAgent +4`; total `-8`). It does not affect the duplicate
  addresses, but it means `docs/IDENTICALITY.md`'s `0x26298`/net-zero statement is
  currently stale.

## 6. Reproduction commands

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# duplicate census / kind sequence
python3 /tmp/w3565_exidx_summary.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
python3 /tmp/cmp_exidx.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so

# map correlation (relink with --Map, drop strip)
bash /tmp/w3565_link_map.sh
python3 /tmp/w3565_exidx_cause2.py
python3 /tmp/w3565_dup_objects.py

# A/B: edit DROP_EMPTY_TEXT in tools/gen_exidx_markers.py, then
cmake --build cmake-build-match --target check -j8
```

The analysis scripts are short and self-contained:

```python
# /tmp/w3565_exidx_summary.py — entry/kind/duplicate summary
import struct, sys, collections
exec(open('/tmp/dup_exidx.py').read().split('for path in sys.argv')[0])
for path in sys.argv[1:]:
    data, secs, entries, byaddr = parse(path)
    cnt = collections.Counter(e['fn'] for e in entries)
    kinds = collections.Counter(e['kind'] for e in entries)
    groups = collections.defaultdict(list)
    for e in entries: groups[e['fn']].append(e)
    dup = [a for a in groups if len(groups[a]) > 1]
    dupcat = collections.Counter(tuple(e['kind'] for e in groups[a]) for a in dup)
    print(f"{path}: exidx={secs['.ARM.exidx']['size']:#x} entries={len(entries)} "
          f"CANT={kinds['CANT']} INL={kinds['INL']} EXT={kinds['EXT']} "
          f"dup_groups={len(dup)} dup_patterns={dict(dupcat)}")
```

```python
# /tmp/dup_exidx.py — raw .ARM.exidx parser (prel31 + kind)
import struct, sys, collections
def sections(data):
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2e)
    hdrs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i*shentsize) for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    out = {}
    for h in hdrs:
        name, typ, flags, addr, off, size, link, info, align, entsize = h
        end = data.index(b"\0", stroff + name)
        out[data[stroff+name:end].decode()] = dict(addr=addr, off=off, size=size, entsize=entsize)
    return out
def prel31(data, off, place):
    w, = struct.unpack_from("<I", data, off)
    v = w & 0x7fffffff
    if v & 0x40000000: v -= 0x80000000
    return place + v, w
def parse(path):
    data = open(path, 'rb').read(); secs = sections(data); ex = secs['.ARM.exidx']
    entries = []
    for i in range(ex['size'] // 8):
        eoff = ex['off'] + i*8; eaddr = ex['addr'] + i*8
        fn, _ = prel31(data, eoff, eaddr)
        w1, = struct.unpack_from("<I", data, eoff + 4)
        kind = 'CANT' if w1 == 1 else ('INL' if w1 & 0x80000000 else 'EXT')
        entries.append(dict(i=i, eaddr=eaddr, fn=fn, kind=kind, w1=w1))
    return data, secs, entries, None
```

```python
# /tmp/w3565_parse_map.py — parse .text.* input sections from a gold --Map file
import re
MAP = '/tmp/w3565_ours.map'
lines = open(MAP).read().splitlines()
start = next(i for i, l in enumerate(lines)
             if re.match(r'^\.text\s+0x[0-9a-f]+\s+0x[0-9a-f]+', l))
secs = []
pat_inline = re.compile(r'^ (\.text\S*)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)')
pat_sec = re.compile(r'^ (\.text\S*)$')
pat_addr = re.compile(r'^\s+0x([0-9a-f]+)\s+(?:0x([0-9a-f]+)\s+)?(\S+)?')
cur = None
for l in lines[start:]:
    m = pat_inline.match(l)
    if m:
        secs.append(dict(name=m.group(1), addr=int(m.group(2), 16),
                         size=int(m.group(3), 16), path=m.group(4)))
        cur = None; continue
    m = pat_sec.match(l)
    if m: cur = m.group(1); continue
    if cur:
        m = pat_addr.match(l)
        if m:
            addr = int(m.group(1), 16); size = m.group(2); path = m.group(3)
            if size is not None:
                secs.append(dict(name=cur, addr=addr, size=int(size, 16),
                                 path=path or ''))
                cur = None
        elif not l.startswith(' '): break
secs.sort(key=lambda s: (s['addr'], s['size']))
```

```python
# /tmp/cmp_exidx.py — compare fn-address + kind sequences
import struct, sys, collections
exec(open('/tmp/dup_exidx.py').read().split('for path in sys.argv')[0])
a = parse(sys.argv[1]); b = parse(sys.argv[2])
ea, eb = a[2], b[2]
print("entries", len(ea), len(eb))
print("same fn addr", sum(1 for x, y in zip(ea, eb) if x['fn'] == y['fn']),
      "same kind", sum(1 for x, y in zip(ea, eb) if x['kind'] == y['kind']),
      "same w1", sum(1 for x, y in zip(ea, eb) if x['w1'] == y['w1']))
for i, (x, y) in enumerate(zip(ea, eb)):
    if x['fn'] != y['fn'] or x['kind'] != y['kind']:
        print("first mismatch entry", i, hex(x['fn']), x['kind'],
              "vs", hex(y['fn']), y['kind']); break
else:
    print("all fn+kind sequences identical")
```

```python
# /tmp/w3565_exidx_cause2.py — map correlation
import re, collections
exec(open('/tmp/w3565_parse_map.py').read())      # parses /tmp/w3565_ours.map
exec(open('/tmp/dup_exidx.py').read().split('for path in sys.argv')[0])
data, esecs, entries, _ = parse('../_work/bins/android_2.0.16_libgof2hdaa.so')
cnt = collections.Counter(e['fn'] for e in entries)
def prev_end(a):
    best = None
    for s in secs:
        if s['size'] > 0 and s['addr'] + s['size'] <= a:
            if best is None or s['addr'] + s['size'] > best: best = s['addr'] + s['size']
    return best
def next_sec(a):
    cand = [s for s in secs if s['addr'] >= a]
    return min(cand, key=lambda s: s['addr']) if cand else None
cat = collections.Counter()
for e in entries:
    if e['kind'] != 'CANT': continue
    a = e['fn']; pe = prev_end(a); ns = next_sec(a); dup = cnt[a] > 1
    trig = 'tuend' if (ns and ns['name'].startswith('.text.tuend')) else 'cold/plain'
    gap = (ns['addr'] - a) if ns else None
    cat[(trig, 'dup' if dup else 'solo', 'end-ok' if pe == a else 'end-BAD',
         'gap0' if gap == 0 else 'gap+')] += 1
for k, v in sorted(cat.items()): print(k, v)
```

## 7. Files changed

```
_work/reconstructions/exidx_duplicate_address_root_cause_w3565.md   new (this note)
```

No source, tool, or flag change: the duplicate-address structure is already
exact, and both A/B variants are regressions (one in `.ARM.exidx` size/byte
count, one in `.ARM.exidx` entry count despite a passing gate).
