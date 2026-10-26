# Layout-anchor audit (wave39a-w917)

Analysis-only report. No `src/`, build, or gate changes were made for this pass.

## 0. Provenance and scope

- Worktree: `/home/debian/GalaxyOnFire2/wt/w917`
- Branch: `wave39a-w917`
- HEAD at analysis time: `969f4608ea4712912611ec0d1890e39950d5ec5f`
  (`campaign: launch wave 39 (20 parallel non-linked TU finishers)`)
- Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so`
  - sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`
  - stripped; only `.dynsym` is available, so local anchor addresses were recovered by
    disassembly and the approved section-ordering file.
- Linked reconstruction artifact used for local-anchor disassembly:
  `/home/debian/GalaxyOnFire2/gof2-decomp/cmake-build-match/libgof2hdaa.so`
  - sha256 `94c6f914f2d79adefcd7c7367dd4eb86136ba6af0236f64b90434f00242a49a9`
  - that sibling build is at `3985ec62bb15ad94e5966fb74946225832f6dfef`, not the worktree
    HEAD. Its `verify/report.json` is byte-identical to this worktree's
    `cmake-build-match/verify/report.json` (both sha256
    `9a94fe354e3881e49ac5fb07dbcbd23ed1601cb128d6ccdf751a295f2434f599`).
  - all anchor TUs checked with `cmp` are byte-identical between the two trees except
    `src/game/mission/Generator.cpp`, whose only difference is the `kaamoSpecialItems`
    initializer at line 49; the Generator anchor and code shape are unchanged.
- Verify baseline quoted below: `cmake-build-match/verify/report.json`
  (`byte_exact 1352`, `linked_exact 4468`, `avg 99.78`, `compared 4523`,
  `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`).

The report distinguishes **observations** (ELF sizes/addresses, disassembly, source lines,
verify rows) from **inferences** (why a hack exists, removal arithmetic, ranking).

## 1. Commands used

Representative commands; all paths are relative to the worktree unless absolute:

```sh
readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -SW /home/debian/GalaxyOnFire2/gof2-decomp/cmake-build-match/libgof2hdaa.so
readelf -lW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -lW /home/debian/GalaxyOnFire2/gof2-decomp/cmake-build-match/libgof2hdaa.so
readelf -sW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -sW /home/debian/GalaxyOnFire2/gof2-decomp/cmake-build-match/libgof2hdaa.so

NDK=/home/debian/GalaxyOnFire2/gof2-decomp/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin
$NDK/arm-linux-androideabi-nm -nS <object>
$NDK/arm-linux-androideabi-objdump -dr <object>
$NDK/arm-linux-androideabi-objdump -d --start-address=... --stop-address=... <so>

python3 - <<'PY'
import json
rows = json.load(open('cmake-build-match/verify/report.json'))['rows']
# size-mismatch and byte-exact queries
PY

rg -n 'anchor|layoutPad|\[\[gnu::used\]\]|#pragma clang section' src
```

The NDK binutils path is the one used by the project cache. `readelf -sW` reports Thumb
function `st_value`s with bit 0 set; disassembly addresses below are the even code addresses.

## 2. Current section/address facts (observed)

Both binaries start `.text` at `0x69440`. The delta is entirely in `.text` length, not its
start address.

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | `0x1337c8` = 1,259,464 | `0x1343c0` = 1,262,528 | **`+0xbf8` = +3,064** |
| `.ARM.exidx` | `0x6f70` | `0x5e48` | `-0x1128` = -4,392 |
| `.ARM.extab` | `0x26298` | `0x250d4` | `-0x11c4` = -4,548 |
| `.rodata` | `0x41b67` | `0x416e7` | `-0x480` = -1,152 |
| `.data` | `0x296c` | `0x2978` | `+0xc` = +12 |
| `.bss` | `0x69ac` | `0x6a90` | `+0xe4` = +228 |
| `.got` | `0x430c` | `0x430c` | `0` |
| `.aezdata` | absent | `0x58` at `0x210978` | `+0x58` = +88 |

Key addresses:

| item | original | ours |
|---|---:|---:|
| `.text` start | `0x69440` | `0x69440` |
| `.text` end | `0x19cc08` | `0x19d800` |
| `.rodata` end | `0x20b977` | `0x209e07` |
| `.fini_array` | `0x20cba0` | `0x20aec0` |
| `.got` | `0x20fcf4` | `0x2109d0` |
| `.data` | `0x214000` | `0x20e000` |
| `.bss` | `0x216970` | `0x214ce0` |
| RW LOAD file offset | `0x20bba0` | `0x209ec0` |
| section count | 27 | 28 (extra `.aezdata`) |

Observed section-order difference: the original places `.got` **before** `.data`
(`.fini_array .data.rel.ro .init_array .dynamic .got .data .bss`). Ours places
`.data` and `.aezdata` **before** `.got`
(`.fini_array .data.rel.ro .init_array .dynamic .data .aezdata .got .bss`).
This is the structural reason data anchors can move `.got` independently of `.text`.

`docs/IDENTICALITY.md:35` already records the `+3,064` `.text` delta and attributes it to
the remaining non-linked functions plus the MenuTouchWindow anchor. `docs/IDENTICALITY.md:40`
records `.data +12` and says the Transform pre-`.got` anchor now lives in `.aezdata`.
`docs/VALIDATION.md:8-10` records the compiler/toolchain and `docs/VALIDATION.md:37-44`
records the approved section-ordering-file deviation.

## 3. The ten libstdc++ exception ctors (observed invariant)

`verify/report.json` marks all ten C1/C2 constructors `byte: true`. C1 and C2 are aliases
at the same address, so there are five unique bodies:

| class | original code addr | ours code addr | our/orig delta | `.got - ctor` original | `.got - ctor` ours |
|---|---:|---:|---:|---:|---:|
| `_ZNSt9bad_allocC1/C2Ev` | `0x193c34` | `0x194910` | `+0xcdc` | `0x7c0c0` | `0x7c0c0` |
| `_ZNSt20bad_array_new_lengthC1/C2Ev` | `0x193c54` | `0x194930` | `+0xcdc` | `0x7c0a0` | `0x7c0a0` |
| `_ZNSt16bad_array_lengthC1/C2Ev` | `0x193c74` | `0x194950` | `+0xcdc` | `0x7c080` | `0x7c080` |
| `_ZNSt8bad_castC1/C2Ev` | `0x193e04` | `0x194ae0` | `+0xcdc` | `0x7bef0` | `0x7bef0` |
| `_ZNSt10bad_typeidC1/C2Ev` | `0x193e34` | `0x194b10` | `+0xcdc` | `0x7bec0` | `0x7bec0` |

Observed `_ZNSt9bad_allocC1Ev` disassembly:

```text
; original
193c34: 4902       ldr r1, [pc, #8]      ; literal at 193c40
193c36: 4479       add r1, pc
193c38: 6809       ldr r1, [r1, #0]
193c3a: 3108       adds r1, #8
193c3c: 6001       str r1, [r0, #0]
193c3e: 4770       bx lr
193c40: ca32 0007  .word 0x0007ca32

; ours
194910: 4902       ldr r1, [pc, #8]      ; literal at 19491c
194912: 4479       add r1, pc
194914: 6809       ldr r1, [r1, #0]
194916: 3108       adds r1, #8
194918: 6001       str r1, [r0, #0]
19491a: 4770       bx lr
19491c: ca32 0007  .word 0x0007ca32
```

The literal is exactly:

```text
literal = .got + 0x976 - (ctor + 4)
        = 0x20fcf4 + 0x976 - 0x193c38
        = 0x7ca32
```

Equivalently, the invariant is `.got - ctor == 0x7c0c0`. The current `.text` delta is
`+0xbf8`, while both `.got` and the ctors are `+0xcdc`; the extra `0xe4` is consistent
with the different `.data`/`.aezdata`/`.got` ordering, not with a `.text`-only shift.

### 3.1 Why the 0x1000 page point matters (observed + inferred)

Observed current `.rodata` end is `0x209e07`; the RW LOAD file offset is `0x209ec0`, only
`0xb9` above it. The source comment at `src/game/ui/MenuTouchWindow.cpp:3900-3906` states
that `.fini_array` jumps one `0x1000` page when `.rodata` crosses `0x208ec0`, and that the
only reachable byte-exact `.text` sizes are `0x1333c0 + k*0x1000`.

The arithmetic for the two page points is:

| `.text` size | `.rodata` end | `.fini_array` vaddr | `.got` | ctor | `.got - ctor` | status |
|---|---:|---:|---:|---:|---:|---|
| `0x1333c0` | `0x208e07` (inferred) | `0x209ec0` (inferred) | `0x20f9d0` (inferred) | `0x193910` (inferred) | `0x7c0c0` | ctor-safe |
| `0x133588` (no anchor) | `0x208fcf` (inferred) | `0x20aec0` (inferred) | `0x2109d0` (inferred) | `0x193ad8` (inferred) | `0x7cef8` | **not** ctor-safe |
| `0x1343c0` (current) | `0x209e07` (observed) | `0x20aec0` (observed) | `0x2109d0` (observed) | `0x194910` (observed) | `0x7c0c0` | ctor-safe |
| original `0x1337c8` | `0x20b977` (observed) | `0x20cba0` (observed) | `0x20fcf4` (observed) | `0x193c34` (observed) | `0x7c0c0` | original layout |

The key inference: at `0x1333c0` the `.rodata` end is `0xb9` below the `0x208ec0`
threshold, so `.fini_array`/`.got` sit one page lower; at `0x133588` and `0x1343c0` they
are above the threshold and `.got` stays on the upper page. The natural no-anchor size
`0x133588` is just above the threshold but not a page point, so the ctors move down
`0xe38` while `.got` does not: predicted delta `0x7cef8`, not `0x7c0c0`.

This is why the anchor is not a round `0x1000`: the MenuTouchWindow body is `0xe38`; the
other anchors, hook/call sites, and alignment make up the rest of the page. The whole
cluster is a page shim, not a single 3,640-byte size fix.

## 4. Anchor inventory

### 4.1 `.text` anchors

Sizes are object-symbol sizes from `nm -S`; linked addresses are from disassembly of the
linked artifact. `section_order.txt` line numbers are from the sibling build's generated
ordering file.

| # | location | mechanism | object size | current related function delta | evidence |
|---|---|---|---:|---:|---|
| 1 | `src/game/ui/MenuTouchWindow.cpp:326,328-334,3897-3911` | `_ZL19mtwTextLayoutAnchorv`; 686 `int` + 2 `char` volatile stores; ctor hook `if (menuType == -12345) { volatile fn ptr; fn(); }` | `0xe38` = 3,640 B | ctor `+40`, `draw +116`, `OnTouchEnd +64` | object `.text._ZL19mtwTextLayoutAnchorv` size `0xe38`; linked `0x127798`..`0x1285d0`; `section_order.txt:3041`; ctor hook at `0x125208`..`0x12521e` |
| 2 | `src/game/ui/HangarWindow.cpp:898-908` | six `description = description;` self-assignments | 48 B (6 × 8 B calls) | `render +32` | object calls at `0x187a,0x1882,0x188a,0x1892,0x189a,0x18a2`; linked `0x14df42`..`0x14df6a`; original `0x14a94a`..`0x14a95c` has no `String::operator=` calls |
| 3 | `src/game/world/StarSystem.cpp:130-135` | `_ZL22starSystemLayoutAnchori`; `value*3+1+(value&1)` | `0xe` = 14 B | ctor exact (2900 B) | object symbol `0xe`; linked `0x137194`; ctor `0x1371a8`; original ctor `0x133b68`; 14 B body + 6 B alignment padding |
| 4 | `src/game/mission/Generator.cpp:51-55` | `_ZL29generatorShipListLayoutAnchori`; `value+1+(value>>3)` | `0x8` = 8 B | `createMission -4` | object symbol `0x8`; linked `0x97d34`; `section_order.txt:1476` |
| 5 | `src/game/weapons/ObjectGun.cpp:375-383` | `_ZN12_GLOBAL__N_123ObjectGunProjectileMeshEi`; unused clamp + `MUZZLE_MESHES[item]` | `0x14` = 20 B | all 12 ObjectGun functions size-exact | object symbol `0x14`; linked near `0x1638f8`; `section_order.txt:3697` |
| 6 | `src/engine/core/GameText.cpp:51-55` | `_ZN12_GLOBAL__N_113arabicFormRowEj`; unused Arabic row offset | `0x10` = 16 B | all 14 GameText functions size-exact | object symbol `0x10`; linked `0x817dc`; `section_order.txt:756` |
| 7 | `src/game/ui/StatusWindow.cpp:16-23` | `_ZL17statusWindowClampiii`; unused clamp | `0x10` = 16 B | all 13 StatusWindow functions size-exact | object symbol `0x10`; `section_order.txt:3508` |
| 8 | `src/game/mission/RecordHandler.cpp:1415-1425` | `_ZN12_GLOBAL__N_118mixRecordStoreSlotEi`; unused hash | `0x1e` = 30 B | all 32 RecordHandler functions size-exact | object symbol `0x1e`; `section_order.txt:2428` |
| 9 | `src/game/world/StarMap.cpp:55-57` | `_ZL25StarMap_GetScreenPosition...`; unused wrapper around `GetScreenPosition` | `0x10` = 16 B | `draw -8` | object symbol `0x10`; `section_order.txt:2383` |
| 10 | `src/game/menu/ModStation.cpp:50,863,1102-1109` | `_ZL21modStationTouchEndPadi`; recursive Fibonacci-like helper; called in `OnUpdate` and stored into `field_0x5c` | `0x3e` = 62 B | `OnInitialize -12`; `OnUpdate` size-exact | object symbol `0x3e`; call site `0x828`..`0x82c`; `section_order.txt:2538` |
| 11 | `src/game/world/Level.cpp:1601,2598-2599,7247-7256` | `_ZL15levelTextAnchorv`; 12 volatile double stores; unconditional call at end of `createMission` | `0x84` = 132 B | `createMission +16` | object symbol `0x84`; linked `0x84040`; call site `0x40e0`..`0x40e8`; `section_order.txt:887` |
| 12 | `src/game/world/LevelScript.cpp:4805-4812` | `volatile char layoutPad = 0` store inside `process` | 4 B store | `process -80` | linked store `0x1491e0` (`strb.w r8,[r2]`) |
| 13 | `src/game/menu/HackingGame.cpp:236-238` | `volatile int *anchor = &offset[0][0]; *anchor = *anchor + 1;` | 6 B store | `render2D -18` | linked `0x152fe8` (`ldr/adds/str` on `[sp,#52]`) |
| 14 | `src/game/mission/Status.cpp:2250-2253` | unreachable `if (i == 0xffff) break;` in `missionCompleted` case 171 | 8 B | `missionCompleted` size-exact | linked `0xaed54`: `movw r0,#0xffff; cmp fp,r0; beq` |
| 15 | `src/game/mission/Status.cpp:2330-2333` | unreachable `if ((intptr_t)this == 1) return;` in `moveWanted` | 6 B | `moveWanted` size-exact | linked `0xac030`: `cmp r0,#1; beq.w` |

Observed total of the 15 `.text` anchor bodies: **`0xfba` = 4,026 B**. This already exceeds
the current `+3,064` `.text` delta by `0x3c2` = 962 B; the difference is real code deficits
in the 22 size-mismatched functions.

Approximate page-shim decomposition (inference): 4,026 B anchor bodies + 24 B MenuTouchWindow
ctor hook + 8 B Level call + 8 B ModStation call = 4,066 B; the remaining ~30 B is alignment
padding, giving the observed one-page (`0x1000`) shift from `0x1333c0` to `0x1343c0`.

### 4.2 Data anchors

| # | location | mechanism | size | section/effect | evidence |
|---|---|---|---:|---|---|
| 16 | `src/engine/render/shaders/CubeMapping.cpp:71-75,82` | `kCubeMappingLayoutAnchor[265] = "CubeMapping"` used as the shader name | `0x109` = 265 B | `.rodata` pad; keeps `.rodata` end below the RW page threshold | object symbol `0x109`; comment says it compensates the 190-byte Status table |
| 17 | `src/game/mission/Status.cpp:2048-2051,2072` | `static volatile unsigned char kMissionOrder[190]` zero table used in `switch` | `0xbe` = 190 B | `.bss`; original 190-byte table lives in `.bss` | object symbol `0xbe`; `.bss` section size `0xbe` |
| 18 | `src/engine/math/Transform.cpp:8-17` | `g_transform_matrix_flag[80] = {1}` in `.aezdata` | 80 B | `.aezdata`; moves `.got`/`.bss` without changing `.data` | object `.aezdata` size `0x50`; original has no `.aezdata` |
| 19 | `src/game/weapons/RocketGun.cpp:31-46` | unused runtime `kRocketTrailSets[4]` copy in `.data` | 16 B | `.data`; page-quantized GOT alignment | object `.data` size `0x10`; `kRocketTrailSetValues` constexpr is the used copy |
| 20 | `src/engine/file/AEFile.cpp:7-9` | unused `pakExtension[8] = ".aez"` in `.aezdata` | 8 B | `.aezdata`; second component of the 0x58 section | object `.aezdata` size `0x8`; actual `.aez` literal is at `AEFile.cpp:443` |

Observed `.aezdata` total is `0x58` = 88 B = Transform 80 + AEFile 8. The original has no
`.aezdata`; ours has 28 sections instead of 27.

### 4.3 Non-anchors / stale comments

- `HangarWindow *self = this;` at `src/game/ui/HangarWindow.cpp:308,2158` is a decompiler
  alias, not a layout anchor.
- `FileInterfaceAndroid::GetDirPreFix` is currently the real 20-byte body, 100% linked-exact
  (`verify/report.json`), not the historical 16-byte anchor mentioned in `_work/campaign.md:135`.
- `Level.cpp:2774 framePad` and the `HangarWindow` volatile fields are codegen-shape
  devices, not size anchors.
- Stale comments observed: `MenuTouchWindow.cpp:3900` says “680 volatile stores” but the
  source has 686 `int` + 2 `char` = 688; `HackingGame.cpp:236` says `.text at the original
  0x1333c0` but current `.text` is `0x1343c0`; `HangarWindow.cpp:901-902` mentions old
  5828/5832-byte `OnTouchEnd` sizes while current `OnTouchEnd` is 8196/8132.

## 5. Current `.text` delta and the 22 size mismatches (observed)

`verify/report.json` has 22 rows with `osize != usize`:

| delta (ours − orig) | ours/orig | pct | symbol |
|---:|---:|---:|---|
| `-436` | 6636/7072 | 45.4 | `_ZN6LayoutC1Ev` |
| `-436` | 6636/7072 | 45.4 | `_ZN6LayoutC2Ev` |
| `-312` | 9596/9908 | 62.1 | `_ZN5Radar4drawEP6PlayerP3Hudi` |
| `-120` | 9212/9332 | 68.3 | `_ZN3Hud4drawExxP9PlayerEgobjj` |
| `-92` | 1892/1984 | 64.1 | `_ZN3Gun22calcCharacterCollisionEv` |
| `-80` | 57152/57232 | 47.4 | `_ZN11LevelScript7processEi` |
| `-32` | 2600/2632 | 70.8 | `_ZN11AbyssEngine12MeshReadDataEPNS_6EngineERKjjPPNS_4MeshEPNS_8MaterialE` |
| `-18` | 2072/2090 | 70.3 | `_ZN11HackingGame8render2DEv` |
| `-12` | 1896/1908 | 82.5 | `_ZN9RocketGun6updateEi` |
| `-12` | 6384/6396 | 86.0 | `_ZN10ModStation12OnInitializeEv` |
| `-8` | 2636/2644 | 89.7 | `_ZN7StarMap4drawEv` |
| `-4` | 1992/1996 | 65.7 | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` |
| `+4` | 11444/11440 | 84.6 | `_ZN5MGame8OnUpdateEv` |
| `+4` | 3444/3440 | 93.6 | `_ZN13PlayerStationC1EP7Station` |
| `+4` | 3444/3440 | 93.6 | `_ZN13PlayerStationC2EP7Station` |
| `+12` | 980/968 | 71.8 | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` |
| `+16` | 17188/17172 | 71.0 | `_ZN5Level13createMissionEv` |
| `+32` | 9732/9700 | 72.6 | `_ZN12HangarWindow6renderEv` |
| `+40` | 9640/9600 | 64.2 | `_ZN15MenuTouchWindowC1Ei` |
| `+40` | 9640/9600 | 64.2 | `_ZN15MenuTouchWindowC2Ei` |
| `+64` | 8196/8132 | 67.8 | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` |
| `+116` | 13168/13052 | 73.0 | `_ZN15MenuTouchWindow4drawEv` |

Deficits: 12 rows, sum `-1,562` B. Excesses: 10 rows, sum `+332` B. Net: **`-1,230` B**.
The current `.text` is nevertheless `+3,064` B because the anchors/padding over-compensate
the function-body deficits.

### 5.1 The central arithmetic

Observed:

```text
current .text                0x1343c0
MenuTouchWindow anchor body  0xe38
current .text - anchor       0x133588
original .text               0x1337c8
0x1337c8 - 0x133588          0x240 = 576 B
```

So **removing the MenuTouchWindow anchor alone would leave `.text` 576 B short of the
original**, and at `0x133588` the ctor invariant would be broken (predicted
`.got - ctor = 0x7cef8`). This is the single most important number in the audit: the
anchor is not the whole `+3,064`, and deleting it is not the fix.

The 22 size-mismatched functions net `-1,230` B. If one removed the MenuTouchWindow anchor
and made all 22 functions exactly original size, the net change would be
`-3,640 + 1,230 = -2,410`; current `+3,064` would become `+654`. Removing the remaining
anchor bodies (`386` B) and hook/call sites (~40 B) would leave roughly `+228` B of
alignment/other padding. This is an inference, not an additive guarantee; rebuild and
measure after each batch.

## 6. Observations vs inferences

**Observed**

- Section sizes/addresses in §2, including `.text +0xbf8`, `.got`/ctor `+0xcdc`,
  `.aezdata 0x58`, and the `.got`-after-`.data` order.
- All anchor object-symbol sizes in §4 (`nm -S`).
- Linked addresses and instruction sequences for the anchors where shown.
- The six HangarWindow `String::operator=` calls at `0x14df42..0x14df6a` and their absence
  from the original at `0x14a94a..0x14a95c`.
- The ten exception ctors are `byte: true`; `.got - ctor` is `0x7c0c0` for all five unique
  bodies.
- The 22 size mismatches and their net `-1,230` B.
- `docs/IDENTICALITY.md:35,40,47-48`, `docs/VALIDATION.md:8-10,37-44`, and
  `_work/campaign.md:503-506` state the current layout/anchor situation.

**Inferred**

- The anchor cluster is a one-page shim: 4,026 B bodies + ~40 B hook/calls + ~30 B
  alignment ≈ `0x1000`.
- At `0x1333c0`, `.rodata` end `0x208e07` is below the `0x208ec0` threshold, so
  `.fini_array`/`.got` shift down a page and preserve the ctor delta.
- At the natural no-anchor size `0x133588`, `.rodata` end `0x208fcf` is above the threshold,
  so `.got` stays on the upper page and the ctor delta becomes `0x7cef8`.
- The proper fix is to replace fake bytes with real code/data, not to delete bytes and
  re-add new padding.
- The ranking and expected byte effects in §7 are recommendations, not measured results.

## 7. Ranked fixes

### Rank 1 — MenuTouchWindow 0x1000 anchor (largest impact)

**Why first by impact:** 3,640 B is 90.4% of the 4,026 B of identified `.text` anchor
bodies, and it is the anchor named in `docs/IDENTICALITY.md:35`. It is also the main reason
the current `.text` is `+3,064` rather than the function-body net `-1,230`.

**Proper source-shape fix**

1. Delete the ctor hook at `MenuTouchWindow.cpp:328-334` and the
   `mtwTextLayoutAnchor` declaration/definition at `:326` and `:3897-3911`.
2. Recover the short real bodies, not new padding. Removing the anchor body alone leaves
   `.text` 576 B short; removing the ctor hook as well leaves about 600 B short
   (576 B + ~24 B hook). The short functions are:
   `Layout C1/C2` (`-436` each), `Radar::draw` (`-312`), `Hud::draw` (`-120`),
   `Gun::calcCharacterCollision` (`-92`), `LevelScript::process` (`-80`),
   `MeshReadData` (`-32`), `HackingGame::render2D` (`-18`),
   `ModStation::OnInitialize` (`-12`), `RocketGun::update` (`-12`),
   `StarMap::draw` (`-8`), `Generator::createMission` (`-4`).
   Those individual deficits total 1,562 B; the 576/600 B figure is the net after the
   332 B of excesses and the remaining anchors. Also fix MenuTouchWindow's own excess:
   `draw +116`, `OnTouchEnd +64`, ctor `+40`.
3. Land on a ctor-safe point. For the current data layout the immediate candidates are
   `0x1333c0` and `0x1343c0`, not the original `0x1337c8`. The original size becomes
   reachable only after the `.rodata`/`.data`/`.got` layout is also converged.
4. Verify with `verify-fn` / `verify.py --show _ZNSt9bad_allocC1Ev` and `readelf -SW`;
   do not add a replacement stub.

**Expected effect:** removing the anchor body + hook is about `-0xe50`; recovering about
600 B net real code brings `.text` back to the original size `0x1337c8` (the ctor-safe
point still requires the data-layout convergence described in Rank 5). This is the only
fix that can account for the bulk of the current `+3,064`.

**Risk:** high, because it touches the page boundary and the short-function cluster. Do it
as a coordinated pass, not as a one-line deletion.

### Rank 2 — HangarWindow six self-assignments (most bounded fake code)

**Why second:** 48 B, one function, and the original disassembly proves the six calls are
absent. It is the cleanest bounded assignment for one worker.

**Proper source-shape fix**

- Delete all six `description = description;` lines at `HangarWindow.cpp:903-908`.
- Recover 16 B of real code in `HangarWindow::render`: current `render` is `9732` vs
  original `9700` (`+32`); removing 48 B gives `9684`, so 16 B of the original body is
  still missing. The original block is around `0x14a94a..0x14a95c`, immediately after the
  second `String` constructor and before `TouchButton::setVisible`.
- Net effect after recovering the 16 B: `-32` B. Pair with 32 B of real-code recovery
  elsewhere (or a coordinated anchor re-tune) if the commit must keep the current ctor
  page point.

**Expected effect:** removes 48 B of fake calls and 32 B of net excess; small but
unambiguous.

**Risk:** low-to-medium; one function, but the 16 B recovery must be real code, not a new
`volatile` store.

### Rank 3 — small pure-dead-code cluster (mechanical, 120 B)

**Why third:** all seven helpers are unused `[[gnu::used]]` functions and their owning
functions are size-exact except `Generator::createMission -4` and `StarMap::draw -8`.
This is the easiest batch to audit mechanically.

**Proper source-shape fix**

- Remove the unused helpers:
  `StarSystem` 14 B, `Generator` 8 B, `ObjectGun` 20 B, `GameText` 16 B,
  `StatusWindow` 16 B, `RecordHandler` 30 B, `StarMap` 16 B.
- Recover the 4 B in `Generator::createMission` and 8 B in `StarMap::draw`; the other
  owning functions are already exact, so the original TU likely has a missing local
  helper/rodata block or alignment, not a missing exported body.
- Remove them as a batch and re-measure `.text`; a 120 B batch will move the ctor page
  point unless paired with equivalent real-code recovery.

**Expected effect:** `-120` B fake bodies, plus `+12` B real recovery in the two short
functions; net about `-108` B.

**Risk:** low mechanically, medium for the ctor invariant. Do not land the batch alone
without checking the ten ctors.

### Rank 4 — semantic inline anchors (Ghidra-dependent, ~234 B)

**Why fourth:** these are not separate dead functions; they are live stores/branches and
therefore require real-body recovery.

- `Level.cpp:2598-2599,7247-7256`: remove the unconditional `levelTextAnchor()` call and
  the 132-byte function. The call site is 8 B (`0x40e0..0x40e8`); `createMission` is `+16`,
  so 8 B of real body remains to recover. The other 132 B must be recovered elsewhere in
  Level.
- `ModStation.cpp:50,863,1102-1109`: replace `modStationTouchEndPad(this->dt)` with the
  original `field_0x5c` assignment/body. The call site is 8 B (`0x828..0x82c`); `OnUpdate`
  is size-exact, so the replacement must keep its 3652-byte size.
- `HackingGame.cpp:236-238`: remove the 6-byte `offset[0][0]++` store; `render2D` is `-18`,
  so recover 24 B of real body.
- `LevelScript.cpp:4805-4812`: remove the 4-byte `layoutPad` store; `process` is `-80`, so
  recover 84 B of real body.
- `Status.cpp:2250-2253,2330-2333`: remove the 8-byte `i == 0xffff` guard and the 6-byte
  `this == 1` guard; both functions are size-exact, so recover the equivalent real
  branch/body in `missionCompleted` and `moveWanted`.

**Expected effect:** about `-234` B of anchor bodies/stores plus real-code recovery.

**Risk:** medium-high; each is a Ghidra/disassembly reconstruction, not a deletion.

### Rank 5 — data anchors (separate section problem, 559 B)

**Why last:** these do not explain the `.text` delta; they explain `.rodata`/`.data`/`.bss`
and the `.got` address. They should be fixed after the `.text` shape is stable, or in a
coordinated layout pass.

- `CubeMapping.cpp:71-75`: restore the real 190-byte Status mission-order table and use
  `"CubeMapping"` directly; remove the 265-byte `.rodata` pad.
- `Status.cpp:2048-2051`: recover the original 190-byte table values/initialization; the
  table is a real `.bss` object, not just padding.
- `Transform.cpp:8-17`: restore the original 72-byte pre-`.got` data in `.data` (or its
  original TU) and remove the 80-byte `.aezdata` anchor.
- `RocketGun.cpp:31-46`: remove the unused runtime `kRocketTrailSets[4]` copy; keep the
  `constexpr` values. Recover 16 B of real `.data` if the original TU has it.
- `AEFile.cpp:7-9`: remove the unused `pakExtension[8]` and the `.aezdata` pragma; the real
  `.aez` literal is already at `AEFile.cpp:443`.

**Expected effect:** removes 559 B of artificial data and the extra `.aezdata` section;
moves `.got` and therefore the ctor addresses. This is the step that can make the original
`.text` size `0x1337c8` ctor-safe again.

**Risk:** high; `.got` is page-quantized and the data anchors are currently doing real
layout work. Do not remove them without re-checking `.got - ctor` and `.fini_array`.

## 8. Suggested assignment order for one worker

If the goal is a bounded, reviewable commit that preserves the ten ctors:

1. **HangarWindow** (Rank 2): remove six self-assignments, recover 16 B in `render`,
   pair with 32 B of real code elsewhere. One function, one TU, exact original disasm
   evidence.
2. **Small dead-code cluster** (Rank 3): remove the seven unused helpers, recover 4 B + 8 B
   in Generator/StarMap. Mechanical, but batch it and re-check the ctors.
3. **MenuTouchWindow anchor** (Rank 1): the big pass; remove hook + anchor and recover the
   ~600 B net shortfall (576 B anchor body + ~24 B hook) across the listed short functions.
   This is the main `.text` payoff.
4. **Semantic inline anchors** (Rank 4): Level, ModStation, HackingGame, LevelScript,
   Status. Each needs Ghidra/disassembly reconstruction.
5. **Data anchors** (Rank 5): CubeMapping/Status/Transform/RocketGun/AEFile. Do this after
   `.text` is stable, then re-check `.got`/`.fini_array` and the ten ctors.

A worker should not “fix” a size by adding a new `volatile` store, `[[gnu::used]]` stub, or
new `.aezdata` object. The point of this audit is to replace the existing fake bytes with
the original source shape.

## 9. Verification recipe for the next worker

After each bounded change:

```sh
cmake --build cmake-build-match --target link_so -j4
cmake --build cmake-build-match --target verify -j4
FN=_ZNSt9bad_allocC1Ev cmake --build cmake-build-match --target verify-fn
python3 tools/verify/verify.py --show _ZNSt9bad_allocC1Ev
readelf -SW cmake-build-match/libgof2hdaa.so | rg '\.text|\.rodata|\.fini_array|\.got|\.aezdata'
```

Check:

- `verify/report.json` still has all ten C1/C2 ctors `byte: true`.
- `.got - ctor` is still `0x7c0c0` for all five unique bodies.
- `.text` is at a ctor-safe point for the current data layout (`0x1333c0` or `0x1343c0`),
  or the data layout was moved in lockstep.
- `byte_exact` does not decrease; `linked_exact` does not persistently decrease.
- No new undefined imports; no new exported globals; lint stays clean.

## 10. Top findings

1. **The 0x1000 anchor is a 3,640-byte fake function, not a 0x1000-byte one.**
   `_ZL19mtwTextLayoutAnchorv` is `0xe38` bytes (686 `int` + 2 `char` volatile stores) plus
   a ctor hook. Current `.text` is `0x1343c0` (`+0xbf8` = `+3,064`); removing the anchor
   alone leaves `0x133588`, 576 B short of original, and breaks the ctor invariant
   (predicted `.got - ctor = 0x7cef8`). The rest of the page is made up by the other
   anchors/hook/calls/alignment.

2. **HangarWindow's six `description = description;` are real compiled code.**
   They become six `String::operator=` calls (48 B) at `0x14df42..0x14df6a`; the original
   `render` has no such calls between `0x14a94a` and `0x14a95c`. `render` is currently
   `+32` B, so removing the six lines and recovering the missing 16 B of real body is a
   bounded one-function fix.

3. **Most remaining anchors are pure padding or semantic no-ops.**
   The small dead-code cluster is 120 B (`StarSystem` 14, `Generator` 8, `ObjectGun` 20,
   `GameText` 16, `StatusWindow` 16, `RecordHandler` 30, `StarMap` 16); the semantic inline
   anchors add about 234 B (`Level` 132+8 call, `ModStation` 62+8 call, `HackingGame` 6,
   `LevelScript` 4, `Status` 14); the data anchors add 559 B (`CubeMapping` 265,
   `Status` 190, `Transform` 80, `RocketGun` 16, `AEFile` 8). The hard target is not
   simply deleting `3,064` B of `.text`; it includes the 22 size-mismatched functions
   (net `-1,230` B) plus the data-layout anchors.
