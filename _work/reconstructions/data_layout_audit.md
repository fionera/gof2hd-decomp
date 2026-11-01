# Data-section layout audit (`.rodata`, `.data.rel.ro`, `.data`, `.bss`)

Task: decompose the data-section deltas of `libgof2hdaa.so` against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (Android 2.0.16, armeabi-v7a,
NDK r18b clang 7.0.2 `-Oz`), on branch `research-w3305`, base `81db17d1`.

**Result in one line:** `.rodata` is **-1,056 B** (ours smaller). Of that,
**-1,777 B is one missing duplicate shader-source copy**, **+804 B is extra
content in our tree** (264 B `Item` pad + 540 B duplicate `Achievements`
table), and the remaining **-83 B is alignment/padding and small table
differences**. The 30,044 B of shader-source literals are not missing: they are
*ordered differently* (our merged string pool vs the original's per-TU
`.rodata` arrays). `.data.rel.ro` is **+32 B** (one 44 B `MenuTouchWindow`
anchor object, minus 4 B and 8 B of padding). `.bss` is **+44 B** (local-static
size/order difference; the exported objects are all the same size). `.data` is
**exact**.

All numbers below were measured on the clean tree at `81db17d1` with the
standard gate build. No `src/` or `CMakeLists.txt` change is part of this
note; the worktree is clean apart from this file.

---

## 1. Baseline and section table

```
cmake --preset default
cmake --build cmake-build-match --target check -j32
```

Gate output on the measured build:

```
verify: byte 1415  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes (`readelf -SW`):

| section | original addr / size | ours addr / size | delta (ours-orig) |
|---|---|---|---|
| `.text` | `0x0069440` / `0x1337c8` = 1,259,464 | `0x0069440` / `0x1343d0` = 1,262,544 | **+3,080** |
| `.rodata` | `0x01c9e10` / `0x41b67` = 269,159 | `0x01ca180` / `0x41747` = 268,103 | **-1,056** |
| `.data.rel.ro` | `0x020cbb0` / `0x2fe8` = 12,264 | `0x020ceb0` / `0x3008` = 12,296 | **+32** |
| `.data` | `0x0214000` / `0x296c` = 10,604 | `0x0210000` / `0x296c` = 10,604 | **0** |
| `.bss` | `0x0216970` / `0x69ac` = 27,052 | `0x0216c80` / `0x69d8` = 27,096 | **+44** |

Exported `OBJECT` anchors (`.dynsym`):

| section | anchors | offset-exact | first divergence |
|---|---|---|---|
| `.rodata` | 182 | 0 | `0xa20` (byte 2593, 1-based) |
| `.data.rel.ro` | 277 | 111 | `0xf7c` (relocation type), `0xf88` (first offset shift) |
| `.data` | 82 | **82** | none |
| `.bss` | 302 | 0 | `0x0` (`g_pActivity`: orig `0x0`, ours `0x20e4`) |

The `.rodata` and `.bss` anchors are all shifted because the original and our
tree order the local data differently; the *exported object bytes/sizes* are
still exact (the data gate is 843/843).

---

## 2. Method (reproducible)

### 2.1 Extract the sections

```sh
python3 - <<'PY'
import subprocess, re
def sections(path):
    out = subprocess.check_output(['readelf','-SW',path], text=True)
    d = {}
    for line in out.splitlines():
        m = re.match(r'\s*\[\s*(\d+)\]\s+(\S*)\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+(\S+)\s+(\d+)\s+(\d+)\s+(\d+)', line)
        if m:
            d[m.group(2)] = dict(addr=int(m.group(4),16), off=int(m.group(5),16), size=int(m.group(6),16))
    return d
for tag, path in [('orig','../_work/bins/android_2.0.16_libgof2hdaa.so'),
                  ('ours','cmake-build-match/libgof2hdaa.so')]:
    s = sections(path)
    for n in ('.rodata','.data.rel.ro','.data'):
        with open(path,'rb') as f:
            f.seek(s[n]['off']); data = f.read(s[n]['size'])
        open(f'/tmp/{tag}{n}','wb').write(data)
PY
```

### 2.2 Link map (per-object `.rodata`/`.data.rel.ro`/`.bss` attribution)

Re-run the same gold link with `-Map` and without `--strip-all`. The exact
command is the `COMMAND =` line of `build libgof2hdaa.so` in
`cmake-build-match/build.ninja`, with `-Map=/tmp/link.map -o /tmp/libgof2_map.so`
added and the final `strip` removed. The relevant part:

```sh
cd cmake-build-match
GOLD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
$GOLD -shared -soname libgof2hdaa.so --build-id --unresolved-symbols=ignore-all \
  --exclude-libs=libgcc.a --section-ordering-file=section_order.txt \
  -Map=/tmp/link.map -o /tmp/libgof2_map.so \
  crtbegin_so_renamed.o -L .../platforms/android-16/arch-arm/usr/lib \
  @link_order.rsp .../libcrypto_gof2.a --whole-archive .../libz_gof2.a .../libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android .../libfmodex.so .../libfmodevent.so \
  .../libc++abi.a -l m .../libandroid_support.a .../libgcc.a -l dl -l c .../crtend_so.o
```

The map gives, for every input section, `(object, section, address, size)`.
Gold also emits `** merge strings` / `** merge constants` blocks at the start
of `.rodata`; those are the global merged pools.

### 2.3 Anchor alignment

For each `.dynsym` `OBJECT` symbol, compute `offset = st_value - section.addr`
in both binaries and match by name. This gives exact anchor pairs. The
`delta = orig_off - ours_off` and its change between consecutive anchors
(`d_delta`) localise size differences. A positive `d_delta` means the original
has more bytes between those two anchors; negative means ours has more.

### 2.4 Content multiset

Split each section into maximal printable blobs (`[\\x09\\x0a\\x0d\\x20-\\x7e]{4,}`)
and compare the multisets. This separates *content* differences from
*ordering* differences:

```python
import re, collections
pat = re.compile(rb'[\x09\x0a\x0d\x20-\x7e]{4,}')
def blobs(fn, lo, hi):
    d = open(fn,'rb').read()
    return [(m.start(), m.group()) for m in pat.finditer(d) if lo <= m.start() < hi]
```

For `.rodata` the result is unusually clean: the *only* blob content
difference is one extra copy of a 1,777-byte shader source in the original.
Everything else is present in both, only at different offsets.

### 2.5 Sequential per-TU alignment

Walk our per-object `.rodata` input sections in output order (from the map),
and search each section's bytes in the original `.rodata` at/after the end of
the previous match. This recovers the original offset of each TU block and
exposes the original-only gaps (the per-TU shader arrays). The first 40 TUs
align exactly; the game TUs after `ModStation` have internal table/order
differences and are analysed with the anchors instead.

---

## 3. `.rodata` — ordered map and decomposition

### 3.1 The two-region structure

Gold puts the merged string/constant pools first, then per-object `.rodata`.
The first per-object `.rodata` section is `FileInterfaceAndroid.cpp`'s
typeinfo name, which is the first `.rodata` `OBJECT` anchor.

| region | original | ours | delta |
|---|---|---|---|
| merged string/constant pool (`0 .. first typeinfo`) | `0x00000 .. 0x1d760` = 120,672 | `0x00000 .. 0x25190` = 151,952 | **+31,280** |
| per-TU `.rodata` (`first typeinfo .. end`) | `0x1d760 .. 0x41b67` = 148,455 | `0x25190 .. 0x41747` = 116,151 | **-32,304** |
| total | 269,159 | 268,103 | **-1,056** |

The +31,280 / -32,304 swap is the **shader-source ordering shift**: our shader
source string literals are mergeable and land in the global string pool at the
front; the original keeps them as non-mergeable `const char[]` arrays in each
shader TU's `.rodata`. It is almost size-neutral. The exact split is:

| component | merged pool | per-TU `.rodata` | net |
|---|---:|---:|---:|
| 27 shader/non-shader blobs (30,108 B unique) | +30,108 | -30,108 | 0 |
| duplicate `CubeMapping`/`SpecCubeMapping` copy | 0 | -1,777 | **-1,777** |
| non-printable (NULs, merged constants, padding, tables) | +1,172 | -419 | +753 |
| boundary at first anchor | -32 | 0 | -32 |
| **total** | **+31,280** | **-32,304** | **-1,056** |

The `+753` non-printable net is the same `+721` measured over the whole
section plus the 32-byte boundary effect; the `-32` row is the first-anchor
boundary (the first `.rodata` `OBJECT` is not exactly the first per-TU byte).

### 3.2 First divergence

`cmp -l orig.rodata ours.rodata`:

```
2593 137 164
```

Byte 2593 (1-based) = offset **`0xa20`**. The original has `a_tangent`
(attribute-name string, `_` = 0x5f); ours has the first byte of the
`BumpShaderParticle` vertex source (`attribute highp vec4 ...`, `t` = 0x74).
This is the first shader-source literal that our mergeable pool pulls to the
front. The preceding 2,592 bytes (JNI strings, IAP names, language names, ...)
are identical.

### 3.3 Blob content accounting

| metric | original | ours | delta |
|---|---|---|---|
| printable blobs (len >= 4) | 2,779 | 2,778 | -1 |
| printable blob bytes | 200,846 | 199,069 | **-1,777** |
| non-printable bytes (NULs, tables, padding) | 68,313 | 69,034 | **+721** |
| total | 269,159 | 268,103 | **-1,056** |

The `-1,777` is exactly one duplicate blob: the 1,777-byte
`CubeMapping`/`SpecCubeMapping` vertex source. The original has it twice
(`0x2ccd0` and `0x30ae0`), ours once (merged at `0x7c49`). The two original
copies are byte-identical including the trailing NUL.

The `+721` non-printable excess in ours is dominated by:

| extra non-printable content in ours | bytes |
|---|---|
| `src/game/mission/Item.cpp` `canBeInstalledMultipleTimesPad[264]` | **264** |
| `src/game/mission/Achievements.cpp` duplicate 135-int threshold table | **540** |
| residual alignment/small-table difference | **-83** |
| net | **+721** |

### 3.4 The shader-source ordering shift (the big one)

The 27 blobs that are in our merged pool but in the original's per-TU
`.rodata` total **30,108 B** (30,044 B of shader source + 64 B of non-shader
string literals). The original's per-TU copies total **31,885 B** because of
the one extra 1,777-byte duplicate.

The 23 shader-source blobs, by owning TU (our source tree):

| TU | VS bytes | FS bytes | total |
|---|---:|---:|---:|
| `game/core/BumpShaderParticle.cpp` | 1,223 | 1,181 | 2,404 |
| `game/core/BumpRimCubeShader_new.cpp` | 2,195 | 2,809 | 5,004 |
| `engine/render/shaders/BlurShader.cpp` | 311 | 773 | 1,084 |
| `game/core/BumpShaderCloak.cpp` | 1,547 | 2,323 | 3,870 |
| `game/core/BumpShaderV4.cpp` | 1,470 | 1,098 | 2,568 |
| `game/core/BumpShaderV3.cpp` | 1,346 | 2,000 | 3,346 |
| `game/core/TextureConference.cpp` | 505 | 449 | 954 |
| `game/core/BumpShaderV2.cpp` | 1,071 | 886 | 1,957 |
| `game/core/BumpShaderRefract.cpp` | 568 | 930 | 1,498 |
| `engine/render/shaders/BumpRimCubeShader.cpp` | 2,145 | 2,811 | 4,956 |
| `engine/render/shaders/SpecCubeMapping.cpp` | 1,777 | — | 1,777 |
| `engine/render/shaders/PulseShader.cpp` | 343 | 283 | 626 |
| **total (23 blobs)** | | | **30,044** |

The remaining 4 blobs (64 B) are `FModSound.cpp` `kSuffixA`/`kSuffixB`
literals (13+24 B) and `RecordHandler.cpp` salt literals (7+20 B).

Note the asymmetry: `CubeMapping.cpp` uses
`static const char (&kCubeMappingVertexSource)[1778] = "..."` (a reference to
a string literal, so mergeable), while `SpecCubeMapping.cpp` uses
`static const char *const ... = "..."` (also mergeable). The original's two
copies are non-mergeable, consistent with `const char[]` arrays for both, so
both survive. `CubeNormalMapping.cpp` and `SpecCubeAlphaMapping.cpp` already use
`const char[]` in our tree and therefore already land in per-TU `.rodata`.

### 3.5 Other ordered differences

| location (original offsets) | content | bytes | kind |
|---|---|---:|---|
| `0x3d8ac` / `0x4018a` (libz) | `crc32.o` 8,192-byte CRC table is before `deflate.o` in ours, after `inflate.o` in original | 8,192 moved (5,980 offset swing, net 0) | ordering |
| `0x34130` (ModStation) | typeinfo name and 28-byte table are swapped inside the TU block | 28 shift, net 0 | ordering |
| `0x32514` (Level) | original-only 552-byte jump/index table | 552 | table/codegen |
| `0x33b50` (LevelCreateScene) | original-only 2,000-byte jump/index table | 2,000 | table/codegen |
| `0x319ec` (MTitle) | original-only 252-byte jump/index table | 252 | table/codegen |
| `0x316c3` (FModSound) | original-only 109 B of strings/table (strings also exist in our merged pool) | 109 | ordering |

The Level/LevelCreateScene/MTitle tables are non-printable and are the main
reason the per-TU non-printable accounting is not zero. They are codegen
(jump-table shape) differences, not missing source.

---

## 4. `.rodata` — top 20 individual divergences (ranked by bytes)

Ordering shifts are marked `(order)`; size-changing content is marked
`(size)`. The byte count is the amount of content moved or added/removed.

| # | divergence | bytes | kind |
|---:|---|---:|---|
| 1 | 23 shader-source literals: our merged string pool -> original per-TU `.rodata` | 30,044 | order |
| 2 | libz `crc32.o` 8,192-byte CRC table order vs `deflate.o`/`inflate.o` | 8,192 | order |
| 3 | `BumpRimCubeShader_new` VS+FS moved | 5,004 | order |
| 4 | `BumpRimCubeShader` VS+FS moved | 4,956 | order |
| 5 | `BumpShaderCloak` VS+FS moved | 3,870 | order |
| 6 | `BumpShaderV3` VS+FS moved | 3,346 | order |
| 7 | `BumpShaderV4` VS+FS moved | 2,568 | order |
| 8 | `BumpShaderParticle` VS+FS moved | 2,404 | order |
| 9 | `LevelCreateScene` original-only jump table | 2,000 | table |
| 10 | `BumpShaderV2` VS+FS moved | 1,957 | order |
| 11 | `SpecCubeMapping` VS moved | 1,777 | order |
| 12 | **duplicate** `CubeMapping`/`SpecCubeMapping` VS (original has 2, ours 1) | **1,777** | **size (missing)** |
| 13 | `BumpShaderRefract` VS+FS moved | 1,498 | order |
| 14 | `BlurShader` VS+FS moved | 1,084 | order |
| 15 | `TextureConference` VS+FS moved | 954 | order |
| 16 | `PulseShader` VS+FS moved | 626 | order |
| 17 | `Level` original-only jump table | 552 | table |
| 18 | **`Achievements.cpp` duplicate 135-int table** | **540** | **size (extra)** |
| 19 | `Item.cpp` `canBeInstalledMultipleTimesPad[264]` | **264** | **size (extra)** |
| 20 | `MTitle` original-only jump table | 252 | table |

Two smaller ordering differences are not in the top 20: `FModSound` 109 B and
the `ModStation` 28-byte typeinfo/table swap (see §3.5).

The three `(size)` rows are the only *identified* size-changing content:
`-1,777 + 540 + 264 = -973`; the remaining `-83 B` is alignment/padding and
the net effect of the jump-table shape differences (the `table` rows are
ordering/shape differences, not missing source).

---

## 5. `.data.rel.ro` — +32 B

### 5.1 First divergence

The first 111 `OBJECT` anchors are at identical offsets. The first relocation
difference is at **`0xf7c`**:

```
orig: 0xf7c  R_ARM_RELATIVE
ours: 0xf7c  R_ARM_ABS32  ... _ZTVN10__cxxabiv120__si_class_type_infoE
```

This is the `ModStation` typeinfo's vtable pointer. The first *offset* shift is
at **`0xf88`** (`PlayerFighter` vtable): ours is 4 bytes later.

### 5.2 Exact budget

| component | bytes | effect |
|---|---:|---|
| `MenuTouchWindow.cpp` extra `.data.rel.ro` object (`MenuTouchWindow_languageIds[11]` anchor: 10 ints + function pointer) | **44** | ours +44 |
| `ModStation` vtable -> typeinfo padding (orig `0xfc`, ours `0xf8`) | **-4** | ours -4 |
| `cxa_demangle` 8-byte zero gap before first switch table (orig has it, ours does not) | **-8** | ours -8 |
| **net** | **+32** | |

Anchor shift distribution: 111 anchors at `+0`, 15 at `-4`, 151 at `+40`.
The `+40` group is everything after the `MenuTouchWindow` block; the `-4`
group is the `ModStation`..`PlayerStation` range.

The `MenuTouchWindow` object is a deliberate layout anchor in our tree:

```cpp
// src/game/ui/MenuTouchWindow.cpp:277-281
static void mtwTextLayoutAnchor();
static const int MenuTouchWindow_languageIds[11] = {27, 28, 30, 29, 31, 32, 33, 35, 38, 39,
                                                    reinterpret_cast<int>(mtwTextLayoutAnchor)};
```

It is not present in the original. The object's bytes are
`1b000000 1c000000 1e000000 1d000000 1f000000 20000000 21000000 23000000
26000000 27000000 <reloc mtwTextLayoutAnchor>`; the 10-int prefix is also not
found in the original `.data.rel.ro`.

---

## 6. `.bss` — +44 B

### 6.1 First divergence

The first exported `.bss` symbol `g_pActivity` is at original offset `0x0`
but ours at `0x20e4`. The original `.bss` is ordered by symbol/data-section
(the first symbols are `g_pActivity`, then `stackX` at `0x1010`, then
`ndk_APK_Path` at `0x1034`), while ours is ordered by TU (our `.bss` starts
with `VirtualInput`'s 0x1040-byte block, then `BlowfishSubkeyFile`'s 0x104c
block, then `main.c`'s 0x14 block). All 302 exported objects have the same
sizes; the +44 is entirely local statics.

### 6.2 Local-static evidence

Our per-object `.bss` sizes (from the link map) include:

| object | ours `.bss` | notes |
|---|---:|---|
| `VirtualInput.cpp` | `0x1040` | contains `g_delayedKeys[4096]` + input/mouse state |
| `BlowfishSubkeyFile.cpp` | `0x104c` | `S[4096]` + `P[72]` + `SubkeyFile` |
| `ParticleSettings.cpp` | `0x3c08` | `ParticleSettingsRef::cur/init[7680]` each |
| `Globals.cpp` | `0x748` | |
| `main.c` | `0x14` | original main.c globals are C `COMMON` symbols at the end of `.bss` |

The original `.bss` first gap `0x0..0x1010` is `g_pActivity` + 4,108 B of
local statics before `stackX`; our `VirtualInput` block is 4,160 B, a +52 B
difference. The `BlowfishSubkeyFile` S+P span is `0x1048` in the original and
`0x104c` in ours, a +4 B difference. These two nearly cancel; the net +44 B
is the residual of the local-static size/order differences across the app
TUs. The exact owner cannot be read from the stripped original because the
original's local `.bss` symbols are not in `.dynsym`; the reproducible way to
close it is to compare per-object `.bss` sizes from a `-Map` link of a rebuilt
original (or to bisect the candidate TUs under the gate).

---

## 7. `.data` — exact

All 82 exported `OBJECT` anchors are at identical offsets and sizes; the
section size is `0x296c` in both. There is no first divergence. The only
difference is relocation/addend values caused by the different `.text`/section
addresses, not layout.

---

## 8. Ranked fixes

Ordered by expected effect and risk. All are source/link changes; none are
part of this note.

1. **Make the shader sources non-mergeable `const char[]` arrays.**
   Convert the 23 shader-source string literals (especially the
   `static const char *const` / `const char (&)[N]` forms in
   `engine/render/shaders/*.cpp` and `game/core/*.cpp`) to
   `static const char name[] = "...";`. This moves ~30,044 B from the merged
   string pool back into per-TU `.rodata`, restores the duplicate
   `CubeMapping`/`SpecCubeMapping` copy (+1,777 B), and fixes the whole
   `0xa20` ordering divergence. Expected: `.rodata` +1,777 B before the
   extra-content fixes; gate-neutral if the `.text` section order is kept.
2. **Delete the duplicate `Achievements.cpp` table.** `gAchievementValues[135]`
   and `gCFN_req[45][3]` are identical; the original has one 540-byte table.
   Expected: `.rodata` **-540 B**.
3. **Remove the `Item.cpp` 264-byte `canBeInstalledMultipleTimesPad`.**
   Expected: `.rodata` **-264 B**. This is a layout anchor; re-check
   `byte_exact`/`linked_exact` because removing it may shift `.text`.
4. **Fix the `MenuTouchWindow` `.data.rel.ro` anchor.** Replace the 11th
   `MenuTouchWindow_languageIds` element (function-pointer anchor) with a
   non-`.data.rel.ro` way to keep `mtwTextLayoutAnchor` alive, or accept the
   44 B. Expected: `.data.rel.ro` **-44 B** (then -12 B vs original; the
   remaining +12 B is the `ModStation`/`cxa_demangle` padding).
5. **Restore the `ModStation` 4-byte vtable->typeinfo padding and the
   `cxa_demangle` 8-byte switch-table gap.** These are alignment/order
   differences; expected `.data.rel.ro` **+12 B** after fix 4.
6. **Match the original `.bss` symbol/data-section order.** The original's
   `main.c` globals are C `COMMON` symbols at the end of `.bss`; ours are in
   `main.c`'s `.bss` block. Reproduce the original's per-symbol `.bss`
   ordering (or the `-fdata-sections` shape) and re-check the 44 B local-static
   residual. Expected: `.bss` **-44 B**.
7. **Reorder the zlib objects** so `crc32.o` lands after `deflate.o`/`inflate.o`
   as in the original. Size-neutral, but removes the 8,192-byte `.rodata`
   ordering swing.
8. **Match the `ModStation` per-TU `.rodata` order** (typeinfo before the
   28-byte table). Size-neutral, removes the 28-byte anchor shift.
9. **Investigate the `Level`/`LevelCreateScene`/`MTitle` jump tables.**
   These are original-only non-printable tables (552 / 2,000 / 252 B). They
   are codegen-shape differences; matching them may also affect `.text`.
10. **Re-run the gate after each step** and lock improvements with
    `python3 tools/ratchet.py --update-baseline` only when the counts really
    improve.

Expected end state after fixes 1-3: `.rodata` = `268,103 + 1,777 - 540 - 264
= 269,076`, i.e. **83 B smaller** than the original. The residual 83 B is the
alignment/small-table difference quantified in §3.3.

---

## 9. Reproduction checklist

```sh
# 1. clean build + gate
cmake --preset default
cmake --build cmake-build-match --target check -j32

# 2. section sizes
readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E '\.rodata|\.data.rel.ro|\.data|\.bss'
readelf -SW cmake-build-match/libgof2hdaa.so          | grep -E '\.rodata|\.data.rel.ro|\.data|\.bss'

# 3. first .rodata divergence
python3 - <<'PY'
import subprocess
def sec(path,name):
    out=subprocess.check_output(['readelf','-SW',path],text=True)
    for l in out.splitlines():
        if name in l:
            p=l.split(); return int(p[4],16),int(p[5],16)
for tag,p in [('orig','../_work/bins/android_2.0.16_libgof2hdaa.so'),
              ('ours','cmake-build-match/libgof2hdaa.so')]:
    off,size=sec(p,'.rodata')
    open('/tmp/'+tag+'.rodata','wb').write(open(p,'rb').read()[off:off+size])
PY
cmp -l /tmp/orig.rodata /tmp/ours.rodata | head

# 4. anchor table (see §2.3)
python3 - <<'PY'
import subprocess,re
def syms(p):
    out=subprocess.check_output(['readelf','-sW',p],text=True)
    for l in out.splitlines():
        m=re.match(r'\s*\d+:\s+([0-9a-f]+)\s+(\d+)\s+OBJECT\s+\S+\s+\S+\s+(\d+)\s+(.*)',l)
        if m: yield int(m.group(1),16),int(m.group(2)),int(m.group(3)),m.group(4).strip()
for p in ['../_work/bins/android_2.0.16_libgof2hdaa.so','cmake-build-match/libgof2hdaa.so']:
    print(p, sum(1 for _ in syms(p)))
PY

# 5. blob multiset (see §2.4)
# 6. link map (see §2.2)
```

### Caveats

- The original is stripped; local `.bss`/`.rodata` symbols have no names or
  sizes in `.dynsym`. Per-TU attribution of local statics is inferred from the
  link map of *our* build plus the original's exported anchors.
- `linked_exact` compares cross-function calls by symbol name and local calls
  as `#X`; a data-layout change that also shifts `.text` can move
  `linked_exact` transiently. Re-run `check` after each step.
- The `Item` pad and `MenuTouchWindow` anchor are deliberate layout knobs in
  the current tree; removing them may trade a data-section win for a `.text`
  regression. Measure both counts before keeping the change.
