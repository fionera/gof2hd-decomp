# `.rodata` content round 5 (w3477): byte-identical `.rodata`

Status: research note + landed source changes. Branch `wave87-w3477`, based on
master `6d70c224` (`docs: campaign notes for Level rodata block`).
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

## TL;DR

`.rodata` is now **byte-identical to the original**: **0 of 269,159 bytes differ**
(was 8,826 / 4,141 runs at `0x1fb7f8..0x201edc`).  Every one of the baseline
differing bytes was *movement*, not a wrong value: the word multiset over the
whole differing region was identical (6,586 / 6,586 words common, 0 unique).
The fix is a pure emission-order reconstruction — no data was added, removed or
edited, and no section size/base changed.

Gate: **byte_exact 4441 -> 4463 (+22), linked_exact 4496 unchanged**, stubs 0,
missing 0, extra 0, avg 99.89.  `lint` `CLEAN ✓`, `drift` 0 fields, symbol
parity 5370/5370, all ELF sections size- and base-exact.

No padding anchors, no linker flags, no inline asm, no `__attribute__`, no
manually linked data.

## 1. Baseline (independently re-measured)

`cmake-build-match` was absent, so:

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

At the base commit `6d70c224` (changes stashed, `link_so` + `verify` rebuilt):

```
verify: byte 4441  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`.rodata` baseline (file offset == VMA, `0x1c9e10`, size `0x41b67`):

```
differing bytes: 8826 of 269159
runs:            4141
first:           0x1fb7f8
last:            0x201edc
word multiset over 0x1fb7f8..0x201ee0:
  words 6586  common 6586  only-orig 0  only-ours 0
  align: equal 5858, delete 480, insert 650, replace 29 (unequal 29), moved 728
```

**Conclusion: the baseline `.rodata` content was already correct; only the
emission order of whole tables was wrong.**  A byte-diff alone cannot tell
"wrong value" from "moved block"; the multiset proves the latter.

## 2. Controlled A/B (each step rebuilt + `check`)

Each row is one incremental source change on top of the previous row, with a
full `cmake --build cmake-build-match --target check -j8`.  `.rodata` diff is
the raw byte count over `0x1c9e10..0x1c9e10+0x41b67`.

| # | change | byte | linked | `.rodata` diff bytes | first diff |
|---:|---|---:|---:|---:|---:|
| 0 | baseline | 4441 | 4496 | 8826 | `0x1fb7f8` |
| 1 | `PlayerCreature`: 16-byte table to plain `.rodata` | 4456 | 4496 | 4607 | `0x1fb7f8` |
| 2 | Level: `createSpace` before `createAsteroids` | 4458 | 4496 | 4575 | `0x1fc54c` |
| 3 | Level: `assignGuns` before `createMission` | 4459 | 4496 | 3709 | `0x1fc8f0` |
| 4 | Level: `createGun` before `createMission` | 4460 | 4496 | 1876 | `0x1fcc98` |
| 5 | Level: `(void)` station/blueprint refs in `createGun` | 4461 | 4496 | 1118 | `0x1fd578` |
| 6 | Level: `createScene` block before `createRadioMessage` | 4461 | 4496 | 645 | `0x1fd740` |
| 7 | Level: `kHangarSpots*` into `createScene` | 4461 | 4496 | 157 | `0x1fdd14` |
| 8 | StarMap `planetTextures` + MenuTouch `languageIds` refs | **4463** | **4496** | **0** | none |

Rows 0 and 8 were independently re-measured in this pass; rows 1-7 were measured
during the same pass with the same `check` command.  Every step is a strict
improvement (`byte_exact` monotone up, `linked_exact` constant) and every step
leaves all section sizes/bases unchanged.

## 3. What changed and why

### 3.1 `PlayerCreature.cpp` — input-section choice

The original keeps the 16-byte `kCreatureTypeFactor[4]` table in plain
`.rodata`; clang had put it in the mergeable `.rodata.str4.4` group.  The
`#pragma clang section` pair is the sanctioned way to select the input section
(no `__attribute__`):

```cpp
// The original TU keeps this 16-byte table in plain .rodata (not the mergeable
// .rodata.str4.4 group); the pragma reproduces that input-section choice.
#pragma clang section rodata=".rodata"
static const int kCreatureTypeFactor[4] = {1, 1, 1, 0};
#pragma clang section rodata=""
```

Alone this moved the first diff from `0x1fb7f8` (the table was blocking the
whole Level block) and gained 15 byte_exact functions.

### 3.2 `Level.cpp` — function and table emission order

clang 7 emits local statics in function-emission order, and a namespace-scope
`static const` table is emitted at the point of first *odr-use* inside the TU.
The original's `.rodata` order (recovered with
`_work/reconstructions/rodata_content_map_round3.py`) is:

```
0x1fc54c  g_cg_meshTable
0x1fc8f0  g_cg_soundTable
0x1fcc94  kStationTurrets
0x1fcd74  kAlienStationTurrets
0x1fce34  kAlienOutpostTurrets
0x1fcedc  kBaseTurrets
0x1fcf54  kPirateStationCoords
0x1fcf84  kBlueprintRaces
0x1fcf98  kBlueprintPositions
0x1fcfd4  kBlueprintSpacePoints
0x1fcfe8  createMission local coords
0x1fd200  kCarrierTurrets
0x1fd578  kShipHangarHeights
0x1fd678  kHangarMeshes
0x1fd718  kHangarCapacity
0x1fd740  kHangarSpotsTerran
0x1fd758  kHangarSpotsVossk
0x1fd7c4  kHangarSpotsNivelian
0x1fd7e8  kHangarSpotsVoid
0x1fd7f4  kHangarSpotsKhador
0x1fd818  kHangarSpotsDefault
0x1fd830  kBarSeats
0x1fd980  kBarMeshes
0x1fd9f8  stageEntries
...
0x1fdc54  jumpgateMeshes
0x1fdc74  kAgentMeshes
0x1fdc94  stageCounts
0x1fdcb4  wantedEntries
```

The reconstruction is three function moves plus explicit odr-use references:

1. `Level::createSpace` moved before `Level::createAsteroids` (the original
   emits `jumpgateMeshes`/`asteroidMeshes` in that order).
2. `Level::assignGuns` and `Level::createGun` moved before
   `Level::createMission` (the original emits the station/blueprint tables
   before `createMission`'s local coordinate tables).
3. `Level::createScene` (with its `kHangarSpots*` tables now *inside* the
   function, after `kHangarCapacity` and before `kBarSeats`) moved before
   `Level::createRadioMessage`.

The station/blueprint tables are file-scope statics used only inside
`createMission`, so they needed an explicit first-use anchor in `createGun`
(which is emitted before `createMission`):

```cpp
    // Emission-order references: the original Level TU emits the station/blueprint
    // tables here, before createMission's local coordinate tables.
    (void) kStationTurrets;
    (void) kAlienStationTurrets;
    (void) kAlienOutpostTurrets;
    (void) kBaseTurrets;
    (void) kPirateStationCoords;
    (void) kBlueprintRaces;
    (void) kBlueprintPositions;
    (void) kBlueprintSpacePoints;
    gun->setLevel(this);
```

`(void) table;` is a real odr-use (it forces the definition to be emitted) but
generates no code at `-Oz`; it is not a padding anchor.

### 3.3 `StarMap.cpp` / `MenuTouchWindow.cpp` — one odr-use each

The original `.rodata` order in these TUs is:

```
0x1fdd14  StarMap_planetTextures
0x1fdd60  StarMap_keyTextIds
0x1fdd78  StarMap_stationScales
0x1fdde8  StarMap_raceImages
0x1fde10  StarMap_securityColors
...
0x201e90  MenuTouchWindow_languageIds
0x201eb8  MenuTouchWindow_languageTextIds
0x201ee0  MenuTouchWindow_controlImages
```

clang had emitted the tables in the reverse order.  One `(void)` reference at
the right point fixes each TU:

```cpp
// StarMap::init, before the keyBoxWidth loop:
// Emission-order reference: the original StarMap TU emits planetTextures first.
(void) StarMap_planetTextures;
```

```cpp
// MenuTouchWindow::MenuTouchWindow, before the language loop:
// Emission-order reference: the original TU emits languageIds before languageTextIds.
(void) MenuTouchWindow_languageIds;
```

## 4. Final verification

```
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4463  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 4441 -> 4463 (lock with --update-baseline)
RATCHET PASS
```

```
cmake --build cmake-build-match --target lint  -j8   -> CLEAN ✓
cmake --build cmake-build-match --target drift -j8   -> DRIFT: 0 field(s) across 0 class(es)
cmake --build cmake-build-match --target diff  -j8   -> 5370/5370, extra 0, missing 0
```

Raw `.rodata` compare (stripped link output):

```
$ python3 - <<'PY'
from pathlib import Path
O=Path('../_work/bins/android_2.0.16_libgof2hdaa.so').read_bytes()
U=Path('cmake-build-match/libgof2hdaa.so').read_bytes()
base=0x1c9e10; size=0x41b67
print(sum(a!=b for a,b in zip(O[base:base+size], U[base:base+size])))
PY
0
```

Same result on the scratch unstripped relink (`/tmp/w3477_unstripped.so`,
built with `-Map`): **0 / 269159**.

Section sizes/bases are unchanged from the base commit (`readelf -SW`):

```
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x6f70   @ 0x19cc08
.ARM.extab   0x26298  @ 0x1a3b78
.rodata      0x41b67  @ 0x1c9e10
.data.rel.ro 0x2fe8   @ 0x20cbb0
.got         0x430c   @ 0x20fcf4
.data        0x296c   @ 0x214000
.bss         0x69ac   @ 0x216970
```

Full per-section content diff vs original after this pass (informational):

| section | size | content diff bytes |
|---|---:|---:|
| .rodata | 269,159 | **0** |
| .data | 10,604 | **0** |
| .got | 17,164 | **0** |
| .dynamic | 328 | **0** |
| .init_array | 12 | **0** |
| .fini_array | 8 | **0** |
| .data.rel.ro | 12,264 | 3 (the known `.bss` pointer-order residue) |
| .text | 1,259,464 | 118,942 (function bodies) |
| .ARM.exidx | 28,528 | 2,348 |
| .ARM.extab | 156,312 | 112,601 |
| .dynsym | 89,248 | 2,133 (symbol order) |
| .dynstr | 189,208 | 10,742 |
| .hash | 38,716 | 851 |
| .rel.dyn | 28,744 | 126 |
| .rel.plt | 29,392 | 261 |
| .note.gnu.build-id | 36 | 20 |

`.rodata` is therefore fully resolved; the remaining content gaps are `.text`
bodies, EH tables and link metadata.

## 5. Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
# .rodata content diff (fixed for the zero-diff case in this pass):
python3 _work/reconstructions/rodata_content_diff_map.py
# -> differing bytes 0 / 269159 (0.00%) ; runs 0 (byte-identical)
```

`rodata_content_diff_map.py` previously indexed `diff[0]` unconditionally and
raised `IndexError` on a byte-identical `.rodata`; it now prints
`runs 0 (byte-identical)` and returns.

Scratch relink with `-Map` (no source change; used only to get `.symtab` and a
link map for the table map above): `/tmp/w3477_relink.sh` ->
`/tmp/w3477_unstripped.so`, `/tmp/w3477.map`.

## 6. Proven vs hypothesis

**Proven (commands above):**
- Baseline `.rodata` divergence was 100% movement: 6,586/6,586 words common,
  0 words unique to either side.
- Final `.rodata` is byte-identical: 0 / 269,159 differing bytes, on both the
  stripped link output and the scratch unstripped relink.
- The gate improves strictly: byte_exact 4441 -> 4463 (+22), linked 4496
  unchanged; stubs/missing/extra 0; avg 99.89.
- All ELF sections remain size- and base-exact; `lint` `CLEAN ✓`, `drift` 0,
  symbol parity 5370/5370.
- No padding anchors, linker flags, inline asm or `__attribute__` were added.
- The `(void)` anchor is a genuine odr-use that emits no code at `-Oz`
  (confirmed by the unchanged `.text` size 0x1337c8 and unchanged function
  byte_exact/linked_exact for the functions that contain it).

**Hypothesis (not proven from source):**
- The original source likely had these tables at namespace scope in a
  particular declaration order; the `(void)` odr-use references are a
  codegen-shaping reconstruction that reproduces the binary's emission order,
  not necessarily the original source text.
- The exact clang 7 static-emission worklist is only partially characterized;
  the ordering was established empirically by the controlled A/B above, not
  from a documented compiler rule.
