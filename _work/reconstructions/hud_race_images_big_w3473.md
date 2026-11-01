# Hud `_RACE_IMAGES_BIG`: the original is a 10-int table (w3473)

Status: research note only (2026-11-01). Branch `wave87-w3473`, based on master
`8efda908`. Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).
All numbers below were measured in this worktree. Scratch files are in `/tmp`.

## Result in one line

The original `Hud::_RACE_IMAGES_BIG` is a **10-int plain `.rodata` array**
`{1190,1187,1189,1188,-1,-1,-1,-1,-1,-1}` at **0x203758**, and `Hud::init`
loads exactly that address. Our tree has a 4-int `alignas(16)` table at
**0x203770**. Changing it to 10 ints is the correct source shape, but it is
**not a strict gate improvement** in the current tree: `.rodata` grows by
0x10 and `byte_exact` drops 4414 -> 4394, because the whole Hud region is
still +0x10 from an upstream `.rodata` shift. No source change is landed with
this note.

## Baseline (task brief's 4182/4495 is stale)

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4414  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Every ELF section is size- and base-exact:

```
.text        0x1337c8 @ 0x69440
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

`Hud::init` is `pct == 100.0`, `linked == True`, `byte == False`
(`osize == usize == 3464`). Its only difference is the race-table literal.

## Original-binary evidence

### The bytes

```sh
od -Ax -tx4 -v -j 0x203758 -N 0x28 ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```
203758 000004a6 000004a3 000004a5 000004a4
203768 ffffffff ffffffff ffffffff ffffffff
203778 ffffffff ffffffff
```

That is exactly ten 32-bit words: `{1190, 1187, 1189, 1188, -1, -1, -1, -1,
-1, -1}`. The following `_SECURITY_COLORS_RGB` starts at 0x203780
(`ff 00 00 00 2a 00 00 00 ...`), so the Hud plain `.rodata` block is
`0x203758..0x2037b0` = 0x58 bytes: 0x28 table + 0x30 colour table.

### The consumer

`Hud::init` (`_ZN3Hud4initEv`, 0x1604e4, size 0xd88) at 0x16119a:

```
16119a: 4930        ldr  r1, [pc, #192]   ; literal at 0x16125c
16119e: 4479        add  r1, pc
1611a0: f831 1020   ldrh.w r1, [r1, r0, lsl #2]   ; r0 = SolarSystem::getRace()
```

Resolving the PIC literal (`target = add_insn_addr + 4 + pool_word`):

```sh
python3 - <<'PY'
import struct
for p in ['../_work/bins/android_2.0.16_libgof2hdaa.so',
          'cmake-build-match/libgof2hdaa.so']:
    b = open(p, 'rb').read()
    word = struct.unpack_from('<I', b, 0x16125c)[0]
    print(p, hex(word), 'target', hex(0x1611a2 + word))
PY
```

```
../_work/bins/android_2.0.16_libgof2hdaa.so 0xa25b6 target 0x203758
cmake-build-match/libgof2hdaa.so            0xa25ce target 0x203770
```

The original code reads 0x203758; our baseline code reads 0x203770.

### The source-dump witness

`_work/original_layout/original_layout.json`, `tu_symbols.gof2hd_1.1.19_ios`,
`Classes/Game/Hud.cpp` lists `_RACE_IMAGES_BIG` immediately before
`_SECURITY_COLORS_RGB`:

```
"_RACE_IMAGES_BIG",
"_SECURITY_COLORS_RGB",
"_Hud::RADAR_WIDTH",
"_Hud::RADAR_HEIGHT"
```

The dump does not carry a size, but it confirms the symbol belongs to the Hud
TU and is the only race-image symbol there. The bytes above are the only
10-int `{1190,1187,1189,1188,-1...}` block in the Hud region. The name "BIG"
and the three identical 10-int tables elsewhere in the binary (StarMap and
ModStation) are consistent with a 10-entry table.

## Current source and placement

`src/game/ui/Hud.cpp:43`:

```cpp
alignas(16) static const int g_Hud_raceBadge[4] = {1190, 1187, 1189, 1188};
```

The `alignas(16)` is a placement hack, not the original source shape. It puts
the 4-int table in its own 16-byte `.rodata.cst16` block at 0x203770, leaving
`g_Hud_securityColors` at 0x203780 (the original address) and keeping the
`.rodata` size exact. It does **not** put the race table at 0x203758.

Baseline bytes around the region:

```
203750 65 63 74 47 75 6e 00 31 31 41 62 73 74 72 61 63  "ectGun\0" + "11Abstrac"
203760 74 47 75 6e 00 00 00 00 00 00 00 00 00 00 00 00  "tGun\0" + padding
203770 a6 04 00 00 a3 04 00 00 a5 04 00 00 a4 04 00 00  g_Hud_raceBadge[4]
203780 ff 00 00 00 2a 00 00 00 00 00 00 00 ff 00 00 00  g_Hud_securityColors
```

The original has `_ZTS11AbstractGun` at 0x203747 and the race table at
0x203758; ours has `_ZTS11AbstractGun` at 0x203757 (+0x10) and the table at
0x203770 (+0x18, because the 4-int table is 0x18 smaller than the original
10-int table). This is why the `alignas(16)` hack keeps the colour table exact
while leaving `Hud::init` wrong.

## Controlled A/B experiments

Each row is a full `cmake --build cmake-build-match --target check -j8` after
editing only the declaration at `src/game/ui/Hud.cpp:43`. No
`--update-baseline` was run. `Hud::init target` is the resolved 0x16125c pool
word.

| # | declaration | byte | linked | `.rodata` | Hud::init target | verdict |
|---|---|---:|---:|---:|---:|---|
| 0 | `alignas(16) int[4]` (baseline) | **4414** | 4496 | `0x41b67` | `0x203770` | PASS |
| 1 | `int[4]` (drop `alignas`) | 4397 | 4496 | `0x41b67` | `0x1fb8c8` | worse (-17) |
| 2 | `int[10]` (plain) | 4394 | 4496 | `0x41b77` | `0x203768` | worse (-20) |
| 3 | `alignas(8) int[10]` | 4394 | 4496 | `0x41b77` | `0x203768` | worse (-20) |
| 4 | `alignas(16) int[10]` | 4394 | 4496 | `0x41b87` | `0x203770` | worse (-20) |
| 5 | int[10] + `alignas(16)` on all three LensFlare arrays | 4394 | 4496 | `0x41ba7` | `0x203798` | worse (-20) |

`int[10]` alone gains **0** functions and loses **20**. The lost set is the
same for variants 2-4:

```
_ZN11SpaceLounge10OnTouchEndEii
_ZN11SpaceLoungeC1Ev
_ZN11SpaceLoungeC2Ev
_ZN11TouchButton4initERKN11AbyssEngine6StringEjiiiiiihh
_ZN14DialogueWindow18hasSuccessDialogueEi
_ZN14DialogueWindow19hasBriefingDialogueEi
_ZN14DialogueWindow4initEv
_ZN14DialogueWindow51pickGermanGenericTextBecauseWeSaved100EurosWithThatEiP5Agent
_ZN14DialogueWindow6lengthEv
_ZN3Hud20drawOrbitInformationEv
_ZN4Ship11adjustPriceEv
_ZN5MGame12OnTouchBeginEiiPv
_ZN7BeamGunC1EiP3GuniP5Level
_ZN7BeamGunC2EiP3GuniP5Level
_tr_flush_block
crc32
get_crc_table
inflate
inflateBack
inflate_table
```

They are all literal-pool-only (`pct == 100`, `linked == True`) and are lost
because the 10-int table pushes the following `g_Hud_securityColors` from
0x203780 to 0x203790. `Hud::init` itself is still not byte-exact because its
table target is 0x203768, not 0x203758.

Variant 1 (`int[4]` without `alignas`) is the control for the hack: the table
falls into the shared `.rodata.cst16` merge block at 0x1fb8c8, the colour
table shifts, and 17 functions are lost. The `alignas(16)` baseline is
therefore the best current placement, even though it is not the original
source form.

## The upstream +0x10 shift (the blocker)

The 10-int table cannot land at 0x203758 from Hud source alone. In the
`int[10]` variant the whole Hud TU is +0x10:

```
int[10] build:
  .rodata 0x203768 0x58 o170_Hud.cpp.obj
  Hud::init target 0x203768
  g_Hud_securityColors 0x203790

original:
  Hud plain .rodata 0x203758..0x2037b0 (0x58)
  Hud::init target 0x203758
  g_Hud_securityColors 0x203780
```

The same +0x10 appears in the TUs immediately before Hud. With the baseline
link map (`gold -Map`) and the original bytes:

| item | original | baseline/int[10] | delta |
|---|---:|---:|---:|
| `_ZTS11AbstractGun` | `0x203747` | `0x203757` | +0x10 |
| `o168_ObjectGun.cpp.obj` `.rodata` | `0x203398` (inferred) | `0x2033a8` | +0x10 |
| `o155_Achievements.cpp.obj` `.rodata` | `0x203134` (inferred) | `0x203144` | +0x10 |
| `o128_BoundingVolume.cpp.obj` `.rodata` | `0x201e60` (inferred) | `0x201e70` | +0x10 |
| `o170_Hud.cpp.obj` `.rodata` (int[10]) | `0x203758` | `0x203768` | +0x10 |

The shift is already present at BoundingVolume and persists through
Achievements, ObjectGun and Hud. It is not caused by the Hud table size; it
is an upstream `.rodata` layout difference. The known candidates are the
ImageFactory/LensFlare/PlayerCreature region immediately before
BoundingVolume: the original's LensFlare colour arrays are each 0x20 apart
(0x201e00, 0x201e20, 0x201e40), while ours are contiguous at 0x201e34; the
original PlayerCreature typeinfo is at 0x201de0, ours at 0x201e20. This is
the same chain described in `rodata_dialoguewindow_region.md` and
`level_rodata_shipTypeMap_64_w3469.md`.

The map was produced with a scratch relink of the current link command plus
`-Map` and without `--strip-all`:

```sh
/tmp/w3473_relink_map.sh /tmp/w3473_baseline_unstripped.so /tmp/w3473_baseline.map
rg -n "\.rodata.*o170_Hud|\.rodata.*o168_ObjectGun|\.rodata.*o155_Achievements" \
    /tmp/w3473_baseline.map
```

The script is the exact `ld.gold` line from `cmake-build-match/build.ninja`
with `-Map="$2"` and no final `strip`; its body is in the appendix.

## Conclusion

* The original Hud `_RACE_IMAGES_BIG` is a **10-int plain `.rodata` array**,
  not a 4-int table. The bytes, the `Hud::init` literal, and the iOS symbol
  dump all agree.
* The current `alignas(16) int[4]` is a gate hack: it keeps
  `g_Hud_securityColors` at the original address and keeps `.rodata`
  size-exact, but it leaves the race table at 0x203770 and `Hud::init`
  non-byte-exact.
* Changing only the declaration to `int[10]` is **not** a strict improvement:
  `byte_exact` 4414 -> 4394, `linked_exact` unchanged, `.rodata`
  0x41b67 -> 0x41b77, 0 gains, 20 losses. The table still lands at 0x203768
  because the Hud region is +0x10 upstream.
* The next pass should fix the upstream `.rodata` shift (ImageFactory /
  LensFlare / PlayerCreature / BoundingVolume chain) first. Once Hud starts
  at 0x203758, the 10-int declaration should be re-tested; it should then
  put the table at the original address and make `Hud::init` byte-exact.

## Proven vs hypothesis

**Proven (command-backed):**

* Original bytes at 0x203758 are ten ints
  `{1190,1187,1189,1188,-1,-1,-1,-1,-1,-1}`; `Hud::init` resolves the
  literal to 0x203758.
* Baseline source is `alignas(16) int[4]`; its table is at 0x203770 and
  `Hud::init` resolves to 0x203770.
* `int[10]` alone gives byte 4394 / linked 4496 / `.rodata` 0x41b77, with
  0 gains and 20 losses; the table lands at 0x203768.
* The +0x10 shift is present at BoundingVolume, Achievements, ObjectGun and
  Hud, so it is upstream of the Hud table.
* `alignas(16) int[4]` is the best current placement among the tested forms
  (4414 vs 4397 for plain int[4] and 4394 for the int[10] forms).

**Hypothesis (not yet proven by a landed rebuild):**

* Fixing the ImageFactory/LensFlare/PlayerCreature `.rodata` chain will
  remove the +0x10 shift and make the 10-int Hud table land at 0x203758.
* At that point the 10-int declaration should be a strict gain (at least
  `Hud::init` plus the 20 colour-table functions), but this needs a separate
  upstream pass and a full gate run.
* The six `-1` ints are part of the same `_RACE_IMAGES_BIG` array rather than
  a separate anonymous array. The bytes and the single `_RACE_IMAGES_BIG`
  symbol favour the 10-int reading, but the stripped binary cannot distinguish
  the two source spellings.


## Appendix: scratch relink script

`/tmp/w3473_relink_map.sh` (the exact build link, plus `-Map`, minus `strip`):

```sh
#!/bin/bash
set -e
B=/home/debian/GalaxyOnFire2/wt/w3473/cmake-build-match
NDK=/home/debian/GalaxyOnFire2/wt/w3473/.cache/ndk/android-ndk-r18b
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
"$GOLD" -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  --section-ordering-file=$B/section_order.txt -Map="$2" -o "$1" \
  $B/crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @$B/exidx_markers/link_order.rsp \
  --whole-archive $B/exidx_markers/archives/libz_gof2.a \
                 $B/exidx_markers/archives/libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  ../_work/bins/apk_libs/libfmodex.so ../_work/bins/apk_libs/libfmodevent.so \
  $B/exidx_markers/archives/libcrypto_gof2.a -l m \
  $B/exidx_markers/archives/libc++abi.a \
  $B/exidx_markers/archives/libandroid_support.a \
  $B/exidx_markers/archives/libgcc.a -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

Usage:

```sh
/tmp/w3473_relink_map.sh /tmp/w3473_baseline_unstripped.so /tmp/w3473_baseline.map
```
