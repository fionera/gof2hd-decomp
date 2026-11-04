# Optimization level per TU — independent re-verification (w5215, wave 104)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5215`,
branch `wave104-w5215`, base commit `8c5fd3646`. Scratch: `/dev/shm/w5215`
(`TMPDIR=/dev/shm/w5215/tmp`). **No `src/`, build-flag, tool or baseline change is landed:**
this commit contains only this note. The canonical tree is unchanged and the gate is green.

## 0. Bottom line

* **The original is `-Oz`, globally and per TU.** A global sweep over all 192 app TUs gives
  **4500/4524 byte-exact, 4500 linked, 4524/4524 size-exact** at `-Oz`; the best non-`-Oz`
  level is `-Os` at **1263** byte-exact. The original's `.text` size (`0x1337c8`), `.ARM.exidx`,
  `.ARM.extab`, `.rodata`, `.data`, `.bss` and all other section sizes match **only** the `-Oz`
  build (13/13 sections; see §3.1).
* **`-O0`, `-O1`, `-O2`, `-O3` and `-Os` are disproven as the global level** by 3.5x–74x
  fewer byte-exact functions, hundreds of missing/extra symbols at `-O2/-O3/-Os`, and a
  `.text` that is `+0x185e0` (`-Os`) to `+0x133910` (`-O0`) larger.
* **Per-TU sweeps on all 16 target TUs plus 15 representative exact TUs (one per source
  directory) find no better level.** Every target function's fuzzy % is highest at `-Oz`
  (or tied, for `Globals::Globals` C1/C2 with `-Os`); every alternative collapses the TU-local
  exact count. All 31 per-TU `-Oz` controls are byte-identical to the global `-Oz` build
  (same sha256), proving the harness applies the flag.
* **`-mllvm` optimization knobs do not help.** `-unroll-threshold=0` and
  `-enable-machine-outliner=false` are **byte-identical no-ops** at `-Oz`; `-inline-threshold=0`,
  `-inline-threshold=100`, `-enable-shrink-wrap=false` and `-opt-bisect-limit=0/1` are worse.
  clang 7.0.2 has no `-mllvm -opt-level=` knob (rejected at option parsing).
* **No strict improvement exists.** No variant in this study exceeds `byte_exact = 4500`,
  and no function is better at a non-`-Oz` level. The two equal-but-below-100 cells
  (`Globals::Globals` C1/C2 at `-Os`) and the two byte-identical `-mllvm` variants are
  **optimization-level-insensitive**, not evidence for another flag (§8).

## 1. Baseline and canonical gate

The preset's default original path is relative to the repo and resolves to the wrong
`wt/_work` directory in a nested worktree, so the cache was configured explicitly:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
python3 tools/goal_lint.py src
```

Raw output:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`cmake-build-match/verify/report.json` (and the auto-synced root `report.json`) contains
4524 compared functions, **4524/4524 size-exact**, and exactly **24 non-byte-exact**
functions (all size-exact), listed in §3.3.

Hashes and sections:

```text
bbbe86f3abe01712f3a831e65bd6f1c2a0a613cac3dad22124facc799326c6c5  cmake-build-match/libgof2hdaa.so
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  _work/bins/android_2.0.16_libgof2hdaa.so
62462d2f51d55870b76f876a03d54d3ce8073065dad0f11cca534dc8054a6758  /dev/shm/w5215/build/libgof2hdaa.so  (scratch, -Oz)
```

The canonical and scratch `-Oz` `.so` files differ only because gold's build-id hashes the
unstripped link output, which embeds `DW_AT_comp_dir`; sha256 equality is therefore only
meaningful **within one build directory**. All A/B sha comparisons below are within
`/dev/shm/w5215/build`.

The original's `.comment` records the compiler but not the optimization level, and the
binary is `--strip-all` (no DWARF), so the level is inferred from codegen, not read directly:

```text
Android (4751641 based on r328903) clang version 7.0.2 (...) (based on LLVM 7.0.2svn)
GCC: (GNU) 4.9.x 20150123 (prerelease)
```

## 2. Method

### 2.1 Harness

A wrapper toolchain includes the real one, and a `CMAKE_PROJECT_INCLUDE` file appends the
candidate flag to exactly one source. `COMPILE_FLAGS` is emitted **after** the target's
`COMPILE_OPTIONS`, so the candidate is the last `-O…` on the command line and wins:

```cmake
# /dev/shm/w5215/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w5215/cmake/toolchain.cmake")

# /dev/shm/w5215/per_tu_flags.cmake
if(GOF2_AB_TU AND GOF2_AB_FLAGS)
  set_source_files_properties("${CMAKE_SOURCE_DIR}/${GOF2_AB_TU}"
      PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
endif()
```

Verified in `compile_commands.json`: the canonical `Layout.cpp` command has `-Oz` at
position 12 and `-ffunction-sections -funwind-tables` at positions 37–38; with
`GOF2_AB_FLAGS=-Os` the command ends `… -ffunction-sections -funwind-tables -Os -o …`.

One global variant:

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w5215 -B /dev/shm/w5215/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5215/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=<level> \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DCMAKE_PROJECT_INCLUDE=/dev/shm/w5215/per_tu_flags.cmake \
  -DGOF2_AB_TU= -DGOF2_AB_FLAGS=
cmake --build /dev/shm/w5215/build --target link_so -j6
```

One per-TU variant is identical except `-DGOF2_MATCH_OPT=-Oz` and
`-DGOF2_AB_TU=<tu> -DGOF2_AB_FLAGS=<level>`. The canonical `cmake-build-match` directory
was never used as a scratch build.

Scoring used `/dev/shm/w5215/fast_gate.py`, which reuses `tools/verify/verify.py`'s
`disassemble`/`reconcile`/`Resolver`/`Elf32` code on the same function/PLT/data model.
It was validated two ways:

* on the canonical `-Oz` build it reproduces the official gate exactly
  (`4500 / 4500 / 4524`, `missing 0`, `extra 0`);
* on the non-baseline `Layout.cpp -Os` variant it reproduces the official
  `tools/verify/verify.py` result exactly: **byte 1506, linked 4479, size 4507/4524**
  (`official_Layout_Os/report.json`). This is the cross-check that the per-TU gate
  numbers are not an artifact of the fast harness.

`score_tu.py` scores a named function list with the same official verifier functions and
produced the per-function `pct` values.

### 2.2 Levels swept

`-Oz`, `-O0`, `-O1`, `-O2`, `-O3`, `-Os` — globally (6 builds) and per TU (31 TUs × 6 =
186 builds). The 31 TUs are:

* **16 target TUs** (the TUs that own the 24 non-byte-exact functions):
  `Layout`, `LevelScript`, `Status`, `Generator`, `Level`, `Radar`, `MGame`,
  `HangarWindow`, `Globals`, `RocketGun`, `PlayerEgo`, `Gun`, `Hud`, `MenuTouchWindow`,
  `SpaceLounge`, `ModStation`.
* **15 representative exact TUs, one per source directory**:
  `AESoundRessource` (engine/audio), `ApplicationManager` (engine/core), `AEFile`
  (engine/file), `VirtualInput` (engine/input), `Transform` (engine/math), `PaintCanvas`
  (engine/render), `BloomShader` (engine/render/shaders), `String` (game/core), `MTitle`
  (game/menu), `Item` (game/mission), `Player` (game/ship), `ListItem` (game/ui),
  `ObjectGun` (game/weapons), `Station` (game/world), `NdkHooks` (platform/android).

## 3. Global sweep

### G. Global optimization-level sweep (all 192 app TUs)

| level | byte_exact | linked_exact | size_exact | missing | extra | ours funcs | `.text` | `.so` sha256[:16] |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| `-Oz` | **4500** | 4500 | 4524/4524 | 0 | 0 | 4524 | `0x1337c8` | `62462d2f51d55870…` |
| `-O0` | **61** | 255 | 259/4524 | 0 | 313 | 4837 | `0x2670d8` | `dbc684f730f1269a…` |
| `-O1` | **1033** | 1783 | 2676/4524 | 0 | 276 | 4800 | `0x143f08` | `1ce0039c0a2755a0…` |
| `-O2` | **1168** | 2159 | 2411/4524 | 228 | 0 | 4296 | `0x1631e8` | `482aef17992445ad…` |
| `-O3` | **1167** | 2119 | 2247/4524 | 229 | 0 | 4295 | `0x171578` | `5a51b66652cf63f0…` |
| `-Os` | **1263** | 2650 | 2883/4524 | 168 | 0 | 4356 | `0x14bda8` | `1eb0508087caae77…` |

### 3.1 Section sizes (the original matches only `-Oz`)

| section | original | `-Oz` | `-O0` | `-O1` | `-O2` | `-O3` | `-Os` |
|---|---:|---:|---:|---:|---:|---:|---:|
| `.text` | `0x1337c8` | `0x1337c8` | `0x2670d8` | `0x143f08` | `0x1631e8` | `0x171578` | `0x14bda8` |
| `.ARM.exidx` | `0x6f70` | `0x6f70` | `0x71e8` | `0x7840` | `0x6880` | `0x6838` | `0x6a30` |
| `.ARM.extab` | `0x26298` | `0x26298` | `0x2a644` | `0x26284` | `0x250b4` | `0x25488` | `0x24774` |
| `.rodata` | `0x41b67` | `0x41b67` | `0x419d7` | `0x41bc7` | `0x42d77` | `0x42c97` | `0x43057` |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | `0x30d8` | `0x3128` | `0x2fd8` | `0x2fd8` | `0x2fe8` |
| `.data` | `0x296c` | `0x296c` | `0x29ec` | `0x294c` | `0x294c` | `0x296c` | `0x294c` |
| `.bss` | `0x69ac` | `0x69ac` | `0x69bc` | `0x699c` | `0x699c` | `0x69ac` | `0x699c` |
| `.rel.plt` | `0x72d0` | `0x72d0` | `0x7d20` | `0x7c60` | `0x6650` | `0x6628` | `0x6b58` |
| `.rel.dyn` | `0x7048` | `0x7048` | `0x6e18` | `0x6de0` | `0x6b20` | `0x6b20` | `0x6b40` |
| `.dynsym` | `0x15ca0` | `0x15ca0` | `0x17060` | `0x16e40` | `0x14eb0` | `0x14ea0` | `0x15230` |
| `.dynstr` | `0x2e318` | `0x2e318` | `0x30d77` | `0x3082e` | `0x2c2eb` | `0x2c2cd` | `0x2cc8d` |
| `.hash` | `0x973c` | `0x973c` | `0x9c2c` | `0x9ba4` | `0x93c0` | `0x93bc` | `0x94a0` |
| `.got` | `0x430c` | `0x430c` | `0x45dc` | `0x4564` | `0x3a48` | `0x3a34` | `0x3ccc` |

Only `-Oz` matches **all 13** listed section sizes. `.data`/`.bss` at `-O3` happen to match,
but `.text`, `.ARM.exidx`, `.ARM.extab`, `.rodata` and the dynamic tables do not.

### 3.2 `-O0` is not the clang default for this build

clang 7.0.2's default is `-O0`; the original is clearly not default codegen. `-O0` gives
61 byte-exact, 259 size-exact, 313 extra symbols and `.text = 0x2670d8` (2.0x the original).
The `-mllvm -opt-bisect-limit=0/1` builds also give exactly 61 byte-exact, confirming that
the `-Oz` result is produced by the LLVM optimization pipeline rather than by the front end.

### 3.3 The 24 non-byte-exact functions at `-Oz`

All are size-exact; only instruction bytes differ. They live in exactly the 16 target TUs
(176 of 192 TUs are 100% byte-exact; 192/192 are 100% size-exact).

| function | pct | size |
|---|---:|---:|
| `_ZN6LayoutC1Ev` | 47.8 | 7072 |
| `_ZN6LayoutC2Ev` | 47.8 | 7072 |
| `_ZN11LevelScript7processEi` | 53.4 | 57232 |
| `_ZN6Status16missionCompletedEbbx` | 60.4 | 1432 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 66.4 | 2764 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 74.0 | 9908 |
| `_ZN5MGame10OnTouchEndEiiPv` | 74.8 | 8080 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 77.5 | 1996 |
| `_ZN7GlobalsC1Ev` | 80.5 | 612 |
| `_ZN7GlobalsC2Ev` | 80.5 | 612 |
| `_ZN12HangarWindow6renderEv` | 84.4 | 9700 |
| `_ZN9RocketGun6updateEi` | 86.9 | 1908 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 87.6 | 968 |
| `_ZN5Level11createSceneEv` | 88.3 | 2252 |
| `_ZN3Gun22calcCharacterCollisionEv` | 90.0 | 1984 |
| `_ZN5Level13createMissionEv` | 92.0 | 17172 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 92.9 | 9332 |
| `_ZN15MenuTouchWindowC1Ei` | 95.1 | 9600 |
| `_ZN15MenuTouchWindowC2Ei` | 95.1 | 9600 |
| `_ZN11SpaceLounge10drawLoungeEv` | 95.5 | 1948 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 97.7 | 1896 |
| `_ZN10ModStationC1Ev` | 98.4 | 1008 |
| `_ZN10ModStationC2Ev` | 98.4 | 1008 |
| `_ZN5MGame8OnUpdateEv` | 99.9 | 11440 |

## 4. Per-TU sweep

### 4.1 Target TUs — TU-local exact counts and global gate

Each cell is `byte/linked/size` exact out of the TU function count; the last row per TU is the
global gate `byte_exact` after relinking the whole `.so` with that one TU changed.

**`Layout.cpp`** (53 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 51 | 51 | 53/53 | 4500 | `0x1337c8` |
| `-O0` | 0 | 3 | 3/53 | 1351 | `0x139068` |
| `-O1` | 5 | 14 | 29/53 | 1369 | `0x133c88` |
| `-O2` | 5 | 24 | 27/53 | 1369 | `0x133a58` |
| `-O3` | 5 | 23 | 26/53 | 1369 | `0x133b88` |
| `-Os` | 5 | 30 | 36/53 | 1506 | `0x1338a8` |

**`LevelScript.cpp`** (22 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 21 | 21 | 22/22 | 4500 | `0x1337c8` |
| `-O0` | 1 | 5 | 5/22 | 1351 | `0x147a38` |
| `-O1` | 6 | 13 | 13/22 | 1453 | `0x134a08` |
| `-O2` | 6 | 14 | 14/22 | 1369 | `0x135188` |
| `-O3` | 6 | 13 | 13/22 | 1369 | `0x135e6c` |
| `-Os` | 6 | 15 | 15/22 | 1471 | `0x133828` |

**`Status.cpp`** (147 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 146 | 146 | 147/147 | 4500 | `0x1337c8` |
| `-O0` | 0 | 9 | 9/147 | 1284 | `0x138698` |
| `-O1` | 62 | 77 | 95/147 | 1346 | `0x134128` |
| `-O2` | 67 | 99 | 101/147 | 1380 | `0x135578` |
| `-O3` | 67 | 99 | 100/147 | 1380 | `0x1363f8` |
| `-Os` | 68 | 100 | 103/147 | 1365 | `0x133a68` |

**`Generator.cpp`** (27 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 26 | 26 | 27/27 | 4500 | `0x1337c8` |
| `-O0` | 0 | 5 | 5/27 | 1554 | `0x135328` |
| `-O1` | 4 | 9 | 19/27 | 1369 | `0x133d08` |
| `-O2` | 9 | 19 | 19/27 | 1668 | `0x133c28` |
| `-O3` | 9 | 19 | 19/27 | 1659 | `0x134148` |
| `-Os` | 9 | 19 | 19/27 | 1668 | `0x133ba8` |

**`Level.cpp`** (131 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 128 | 128 | 131/131 | 4500 | `0x1337c8` |
| `-O0` | 1 | 22 | 23/131 | 1345 | `0x14e658` |
| `-O1` | 30 | 56 | 73/131 | 1363 | `0x135848` |
| `-O2` | 31 | 70 | 72/131 | 1351 | `0x136a28` |
| `-O3` | 31 | 70 | 72/131 | 1351 | `0x139948` |
| `-Os` | 33 | 77 | 81/131 | 1376 | `0x134858` |

**`Radar.cpp`** (26 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 25 | 25 | 26/26 | 4500 | `0x1337c8` |
| `-O0` | 1 | 7 | 7/26 | 1445 | `0x136708` |
| `-O1` | 8 | 16 | 18/26 | 1368 | `0x133b88` |
| `-O2` | 8 | 16 | 16/26 | 1377 | `0x133b08` |
| `-O3` | 8 | 16 | 16/26 | 1470 | `0x133d68` |
| `-Os` | 9 | 18 | 18/26 | 1387 | `0x1339c8` |

**`MGame.cpp`** (48 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 46 | 46 | 48/48 | 4500 | `0x1337c8` |
| `-O0` | 0 | 3 | 3/48 | 1348 | `0x1399c8` |
| `-O1` | 9 | 14 | 18/48 | 1355 | `0x133f28` |
| `-O2` | 9 | 16 | 19/48 | 1357 | `0x1352a8` |
| `-O3` | 9 | 15 | 18/48 | 1355 | `0x1355c8` |
| `-Os` | 11 | 24 | 31/48 | 2201 | `0x133988` |

**`HangarWindow.cpp`** (40 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 39 | 39 | 40/40 | 4500 | `0x1337c8` |
| `-O0` | 0 | 8 | 8/40 | 1374 | `0x138188` |
| `-O1` | 3 | 15 | 21/40 | 1365 | `0x133cc8` |
| `-O2` | 3 | 19 | 21/40 | 1352 | `0x133e98` |
| `-O3` | 3 | 19 | 19/40 | 1407 | `0x1346d8` |
| `-Os` | 7 | 23 | 23/40 | 1369 | `0x133908` |

**`Globals.cpp`** (55 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 53 | 53 | 55/55 | 4500 | `0x1337c8` |
| `-O0` | 0 | 14 | 14/55 | 1354 | `0x1376c8` |
| `-O1` | 2 | 21 | 31/55 | 1369 | `0x133d28` |
| `-O2` | 2 | 27 | 28/55 | 1369 | `0x133f68` |
| `-O3` | 2 | 27 | 28/55 | 1369 | `0x1342e8` |
| `-Os` | 3 | 33 | 36/55 | 1473 | `0x1339e8` |

**`RocketGun.cpp`** (21 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 20 | 20 | 21/21 | 4500 | `0x1337c8` |
| `-O0` | 5 | 10 | 10/21 | 1401 | `0x134078` |
| `-O1` | 6 | 15 | 18/21 | 1356 | `0x133978` |
| `-O2` | 6 | 14 | 16/21 | 1378 | `0x1339c8` |
| `-O3` | 6 | 14 | 14/21 | 1369 | `0x133ac8` |
| `-Os` | 11 | 14 | 16/21 | 2965 | `0x133888` |

**`PlayerEgo.cpp`** (177 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 176 | 176 | 177/177 | 4500 | `0x1337c8` |
| `-O0` | 1 | 3 | 3/177 | 1288 | `0x138c98` |
| `-O1` | 54 | 85 | 114/177 | 1337 | `0x133ca4` |
| `-O2` | 60 | 101 | 109/177 | 1343 | `0x134148` |
| `-O3` | 60 | 99 | 107/177 | 1343 | `0x134348` |
| `-Os` | 73 | 125 | 131/177 | 1371 | `0x133b38` |

**`Gun.cpp`** (38 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 37 | 37 | 38/38 | 4500 | `0x1337c8` |
| `-O0` | 0 | 10 | 10/38 | 1396 | `0x134abc` |
| `-O1` | 13 | 23 | 25/38 | 1369 | `0x133a68` |
| `-O2` | 13 | 24 | 25/38 | 1356 | `0x133a98` |
| `-O3` | 13 | 24 | 24/38 | 1416 | `0x133ad8` |
| `-Os` | 13 | 24 | 26/38 | 1365 | `0x1338f8` |

**`Hud.cpp`** (60 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 59 | 59 | 60/60 | 4500 | `0x1337c8` |
| `-O0` | 0 | 9 | 9/60 | 1335 | `0x137fb8` |
| `-O1` | 18 | 29 | 34/60 | 1375 | `0x133c28` |
| `-O2` | 18 | 29 | 33/60 | 1353 | `0x134598` |
| `-O3` | 18 | 29 | 30/60 | 1384 | `0x134d78` |
| `-Os` | 19 | 31 | 34/60 | 1373 | `0x133c38` |

**`MenuTouchWindow.cpp`** (50 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 47 | 47 | 50/50 | 4500 | `0x1337c8` |
| `-O0` | 0 | 9 | 9/50 | 1349 | `0x13bc28` |
| `-O1` | 5 | 14 | 21/50 | 1379 | `0x1341f8` |
| `-O2` | 5 | 23 | 23/50 | 1367 | `0x1343e8` |
| `-O3` | 5 | 22 | 22/50 | 1367 | `0x134c88` |
| `-Os` | 7 | 24 | 25/50 | 1369 | `0x133c08` |

**`SpaceLounge.cpp`** (46 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 45 | 45 | 46/46 | 4500 | `0x1337c8` |
| `-O0` | 1 | 15 | 15/46 | 1348 | `0x13afd8` |
| `-O1` | 9 | 24 | 26/46 | 1412 | `0x133cc8` |
| `-O2` | 9 | 26 | 26/46 | 1369 | `0x134048` |
| `-O3` | 9 | 26 | 26/46 | 1369 | `0x1345c8` |
| `-Os` | 9 | 27 | 28/46 | 1369 | `0x133ba8` |

**`ModStation.cpp`** (45 functions)

| level | TU byte | TU linked | TU size | gate byte | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | 43 | 43 | 45/45 | 4500 | `0x1337c8` |
| `-O0` | 0 | 5 | 5/45 | 1349 | `0x1384d8` |
| `-O1` | 7 | 14 | 20/45 | 1356 | `0x133c68` |
| `-O2` | 7 | 20 | 22/45 | 1356 | `0x134418` |
| `-O3` | 7 | 20 | 21/45 | 1439 | `0x134c78` |
| `-Os` | 7 | 26 | 30/45 | 1369 | `0x1339c8` |

### 4.2 Representative exact TU per directory

| TU | funcs | -Oz | -O0 | -O1 | -O2 | -O3 | -Os |
|---|---:|---:|---:|---:|---:|---:|---:|
| `AESoundRessource.cpp` | 47 | 47/47 | 0/47 | 16/47 | 16/47 | 16/47 | 16/47 |
| `ApplicationManager.cpp` | 88 | 88/88 | 0/88 | 20/88 | 21/88 | 21/88 | 23/88 |
| `AEFile.cpp` | 81 | 81/81 | 0/81 | 2/81 | 2/81 | 2/81 | 9/81 |
| `VirtualInput.cpp` | 34 | 34/34 | 0/34 | 2/34 | 2/34 | 2/34 | 4/34 |
| `Transform.cpp` | 42 | 42/42 | 1/42 | 7/42 | 7/42 | 7/42 | 8/42 |
| `PaintCanvas.cpp` | 231 | 231/231 | 1/231 | 16/231 | 22/231 | 21/231 | 29/231 |
| `BloomShader.cpp` | 13 | 13/13 | 2/13 | 2/13 | 4/13 | 2/13 | 2/13 |
| `String.cpp` | 65 | 65/65 | 0/65 | 5/65 | 6/65 | 6/65 | 7/65 |
| `MTitle.cpp` | 21 | 21/21 | 0/21 | 12/21 | 14/21 | 14/21 | 17/21 |
| `Item.cpp` | 62 | 62/62 | 0/62 | 33/62 | 34/62 | 34/62 | 41/62 |
| `Player.cpp` | 116 | 116/116 | 1/116 | 46/116 | 49/116 | 49/116 | 53/116 |
| `ListItem.cpp` | 55 | 55/55 | 0/55 | 15/55 | 17/55 | 17/55 | 18/55 |
| `ObjectGun.cpp` | 21 | 21/21 | 5/21 | 7/21 | 12/21 | 12/21 | 12/21 |
| `Station.cpp` | 48 | 48/48 | 0/48 | 10/48 | 10/48 | 10/48 | 19/48 |
| `NdkHooks.cpp` | 41 | 41/41 | 3/41 | 4/41 | 4/41 | 4/41 | 5/41 |

### 4.3 Target-function fuzzy % (official verifier `pct`)

| function | TU | -Oz | -O0 | -O1 | -O2 | -O3 | -Os | best non-Oz |
|---|---|---:|---:|---:|---:|---:|---:|---|
| `_ZN10ModStationC1Ev` | `ModStation.cpp` | 98.4 | 3.4 | 44.2 | 90.8 | 87.2 | 93.1 | 93.1 (`-Os`) |
| `_ZN10ModStationC2Ev` | `ModStation.cpp` | 98.4 | 3.4 | 44.2 | 90.8 | 87.2 | 93.1 | 93.1 (`-Os`) |
| `_ZN11LevelScript7processEi` | `LevelScript.cpp` | 53.4 | 2.6 | 7.4 | 24.6 | 20.3 | 28.5 | 28.5 (`-Os`) |
| `_ZN11SpaceLounge10drawLoungeEv` | `SpaceLounge.cpp` | 95.5 | 7.9 | 59.3 | 78.0 | 77.4 | 77.9 | 78.0 (`-O2`) |
| `_ZN12HangarWindow6renderEv` | `HangarWindow.cpp` | 84.4 | 5.2 | 11.8 | 47.5 | 45.5 | 48.0 | 48.0 (`-Os`) |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `MenuTouchWindow.cpp` | 97.7 | 4.5 | 36.0 | 44.3 | 44.1 | 44.3 | 44.3 (`-Os`) |
| `_ZN15MenuTouchWindowC1Ei` | `MenuTouchWindow.cpp` | 95.1 | 4.7 | 54.2 | 60.0 | 52.7 | 60.3 | 60.3 (`-Os`) |
| `_ZN15MenuTouchWindowC2Ei` | `MenuTouchWindow.cpp` | 95.1 | 4.7 | 54.2 | 60.0 | 52.7 | 60.3 | 60.3 (`-Os`) |
| `_ZN3Gun22calcCharacterCollisionEv` | `Gun.cpp` | 90.0 | 2.6 | 37.6 | 63.2 | 62.9 | 63.6 | 63.6 (`-Os`) |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `Hud.cpp` | 92.9 | 7.0 | 57.4 | 55.2 | 52.2 | 60.0 | 60.0 (`-Os`) |
| `_ZN5Level10createShipEiiiP8Waypointbb` | `Level.cpp` | 66.4 | 9.0 | 17.7 | 25.3 | 25.2 | 33.9 | 33.9 (`-Os`) |
| `_ZN5Level11createSceneEv` | `Level.cpp` | 88.3 | 3.1 | 49.0 | 49.5 | 38.5 | 53.9 | 53.9 (`-Os`) |
| `_ZN5Level13createMissionEv` | `Level.cpp` | 92.0 | 5.2 | 22.5 | 13.2 | 29.9 | 40.3 | 40.3 (`-Os`) |
| `_ZN5MGame10OnTouchEndEiiPv` | `MGame.cpp` | 74.8 | 5.8 | 42.8 | 27.0 | 22.9 | 53.5 | 53.5 (`-Os`) |
| `_ZN5MGame8OnUpdateEv` | `MGame.cpp` | 99.9 | 6.7 | 32.1 | 29.1 | 28.8 | 63.5 | 63.5 (`-Os`) |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | `Radar.cpp` | 74.0 | 5.4 | 20.7 | 24.6 | 30.7 | 27.8 | 30.7 (`-O3`) |
| `_ZN6LayoutC1Ev` | `Layout.cpp` | 47.8 | 0.8 | 1.9 | 43.0 | 23.2 | 46.4 | 46.4 (`-Os`) |
| `_ZN6LayoutC2Ev` | `Layout.cpp` | 47.8 | 0.8 | 1.9 | 43.0 | 23.2 | 46.4 | 46.4 (`-Os`) |
| `_ZN6Status16missionCompletedEbbx` | `Status.cpp` | 60.4 | 8.7 | 20.6 | 21.8 | 13.2 | 27.8 | 27.8 (`-Os`) |
| `_ZN7GlobalsC1Ev` | `Globals.cpp` | 80.5 | 13.0 | 40.0 | 71.1 | 71.1 | 80.5 | 80.5 (`-Os`) |
| `_ZN7GlobalsC2Ev` | `Globals.cpp` | 80.5 | 13.0 | 40.0 | 71.1 | 71.1 | 80.5 | 80.5 (`-Os`) |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `Generator.cpp` | 77.5 | 9.0 | 32.9 | 34.1 | 24.8 | 41.2 | 41.2 (`-Os`) |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `PlayerEgo.cpp` | 87.6 | 6.1 | 42.0 | 27.8 | 65.4 | 64.5 | 65.4 (`-O3`) |
| `_ZN9RocketGun6updateEi` | `RocketGun.cpp` | 86.9 | 6.2 | 42.3 | 56.6 | 51.5 | 68.9 | 68.9 (`-Os`) |

**No target function is better at a non-`-Oz` level.** The maximum non-`-Oz` value is
`ModStation::ModStation` C1/C2 at **93.1** (`-Os`) vs **98.4** at `-Oz`. The only equality is
`Globals::Globals` C1/C2 at **80.5** (`-Oz` and `-Os`, same 612-byte body).

Across **all** functions in the 31 swept TUs (not just the named targets), the count of
`pct(non-Oz) > pct(-Oz)` is **0**; the count of `pct(non-Oz) == pct(-Oz) < 100` is **2**
(the two `Globals` constructors at `-Os`).

### 4.4 Harness control

All **31** per-TU `-Oz` variants produced the **same sha256**
`62462d2f51d55870b76f876a03d54d3ce8073065dad0f11cca534dc8054a6758` and the same global
gate `4500/4500/4524` as the global `-Oz` build. The per-TU flag mechanism is therefore
sound, and every difference in §4.1–4.3 is caused by the optimization level alone.

The maximum global `byte_exact` over all 186 per-TU variants is **4500** (at `-Oz` for
every TU); no variant exceeds it.

## 5. `-mllvm` optimization-level knobs

All variants are global, on top of `-Oz`:

| variant | byte_exact | linked_exact | size_exact | missing | extra | `.text` | sha256[:16] |
|---|---:|---:|---:|---:|---:|---:|---|
| `-Oz -mllvm -unroll-threshold=0` | 4500 | 4500 | 4524/4524 | 0 | 0 | `0x1337c8` | `62462d2f51d55870…` |
| `-Oz -mllvm -inline-threshold=0` | 1356 | 4451 | 4498/4524 | 0 | 16 | `0x133688` | `a7b2f502056f48d6…` |
| `-Oz -mllvm -inline-threshold=100` | 1350 | 3519 | 3553/4524 | 206 | 0 | `0x14e1c8` | `5f783959f89816d6…` |
| `-Oz -mllvm -enable-shrink-wrap=false` | 1457 | 4463 | 4500/4524 | 0 | 0 | `0x133788` | `fb41b2da1a5489fc…` |
| `-Oz -mllvm -enable-machine-outliner=false` | 4500 | 4500 | 4524/4524 | 0 | 0 | `0x1337c8` | `62462d2f51d55870…` |
| `-Oz -mllvm -opt-bisect-limit=0` | 61 | 253 | 295/4524 | 0 | 276 | `0x1bf4d8` | `a5eacffacddc0c3b…` |
| `-Oz -mllvm -opt-bisect-limit=1` | 61 | 253 | 295/4524 | 0 | 276 | `0x1ba558` | `cd09120fbeed4755…` |

* `-mllvm -unroll-threshold=0` and `-mllvm -enable-machine-outliner=false` are
  **byte-identical to plain `-Oz`** (same sha256, same `.text`, same 4500/4500/4524).
  They do not change the emitted code at `-Oz` on this codebase, i.e. they are neutral
  (the `-Oz` pipeline already makes the same unroll/outline decisions), not evidence of a flag.
* `-inline-threshold=0` (1356), `-inline-threshold=100` (1350, 206 missing symbols),
  `-enable-shrink-wrap=false` (1457) and `-opt-bisect-limit=0/1` (61) are all worse.
* clang 7.0.2 rejects `-mllvm -opt-level=0/1/2/3/s/z`, `-mllvm -O2` and `-mllvm -Oz` at
  option parsing (`Unknown command line argument '-opt-level=2'`), and
  `clang -mllvm -help-hidden` has no `opt-level` option. There is no LLVM-side
  optimization-level knob to test in this toolchain; `-O…` is the only control.

## 6. What is proven, disproven, and unobservable

### Positively proven

1. **The app TUs as a whole match `-Oz` codegen, not `-O0/-O1/-O2/-O3/-Os`.** 4500/4524
   byte-exact and 4524/4524 size-exact at `-Oz`; the next best level is 1263 byte-exact
   (`-Os`). `.text` is byte-size-identical to the original at `-Oz` and larger at every
   other level.
2. **All 13 checked section sizes match only at `-Oz`** (§3.1).
3. **The 16 target TUs are `-Oz` TUs.** Every target function's fuzzy % is maximal at
   `-Oz`; every alternative level lowers it and collapses the TU-local exact count.
4. **The 15 sampled exact TUs are `-Oz` TUs.** Each is 100% byte-exact at `-Oz` and
   collapses at every alternative (e.g. `PaintCanvas` 231/231 → 22/231 at `-O2`,
   29/231 at `-Os`).
5. **The original is not compiled at the clang default (`-O0`).** `-O0` gives 61
   byte-exact, 259 size-exact, 313 extra symbols, `.text = 0x2670d8`.
6. **No `-mllvm` optimization knob is required.** Two representative knobs are
   byte-identical no-ops; the rest are worse; `-mllvm -opt-level` does not exist in
   clang 7.0.2.
7. **No strict improvement exists in the swept space.** No variant exceeds 4500
   byte-exact; no function is better at a non-`-Oz` level.

### Disproven

* `-O0`, `-O1`, `-O2`, `-O3`, `-Os` as the global level (each loses ≥3237 byte-exact
  functions and changes `.text`).
* Any per-TU level other than `-Oz` for the 31 swept TUs (each loses TU-local exactness
  and lowers the global gate).
* `-mllvm -inline-threshold=0/100`, `-enable-shrink-wrap=false`,
  `-opt-bisect-limit=0/1` as global flags (all worse).
* A missing `-mllvm -opt-level` knob (the option does not parse).

### Unobservable / unknown

* **The original command line is not recorded in the binary.** `.comment` has the clang
  version only; DWARF is stripped by `--strip-all`. "`-Oz`" is an inference from codegen,
  not a direct read of the build log.
* **`-Oz` + a neutral `-mllvm` knob is indistinguishable from plain `-Oz`.** Any knob that
  is inert at `-Oz` (like the two byte-identical ones above) could have been present; it
  would not change a single byte. This is unobservable by construction and does not affect
  the reconstruction.
* **The remaining 161 unswept TUs were not individually A/B-tested.** They are strongly
  constrained: all 192 TUs are size-exact at `-Oz`, and 176 are fully byte-exact; every one
  of the 31 swept TUs changes function sizes at every non-`-Oz` level, so a different level
  in an unswept TU would almost certainly show up as a size mismatch. It is not a proof for
  each individual TU, only a strong global constraint.
* **Codegen-coincident alternatives.** A tiny TU could in principle produce identical code
  at two levels. That would be indistinguishable and harmless; the 31-TU sample found only
  one function-level coincidence (`Globals` ctor at `-Os`), and even there the TU as a whole
  is not coincident.
* **The `-mllvm` space is large.** Seven representative knobs were tested; this is not an
  exhaustive enumeration. It is enough to show that no *required* knob is missing from the
  current build, because the current build already reproduces 4500/4524 bytes.

## 7. Confidence table

| Claim | Confidence | Hard evidence |
|---|---|---|
| Original app code is `-Oz` globally | **Very high** | 4500 byte / 4500 linked / 4524 size vs ≤1263 byte at any other level; `.text` exactly `0x1337c8`; 13/13 section sizes match only `-Oz` |
| Not `-O0` | **Certain** | 61 byte-exact, 259 size-exact, 313 extra, `.text 0x2670d8` |
| Not `-O1` | **Certain** | 1033 byte-exact, 276 extra, `.text 0x143f08` |
| Not `-O2` | **Certain** | 1168 byte-exact, 228 missing, `.text 0x1631e8` |
| Not `-O3` | **Certain** | 1167 byte-exact, 229 missing, `.text 0x171578` |
| Not `-Os` | **Very high** | 1263 byte-exact, 168 missing, `.text 0x14bda8` (+0x185e0); closest competitor but still 3237 functions short |
| All 16 target TUs are `-Oz` | **Very high** | target fuzzy maximal at `-Oz` for all 24 functions; TU-local exact collapses at every alternative; 31/31 `-Oz` controls byte-identical |
| 15 sampled exact TUs are `-Oz` | **High** | 100% byte-exact at `-Oz`, 0–80% at alternatives |
| No per-TU level gives a strict improvement | **High** | max global `byte_exact` over 186 variants = 4500; 0 functions better at non-`-Oz` |
| No `-mllvm` knob is required | **High** | `-unroll-threshold=0` and `-enable-machine-outliner=false` byte-identical; all others worse; no `-opt-level` knob |
| Original command line was literally `-Oz` (not `-Oz` + inert knob) | **Medium** | unobservable; any inert knob is byte-invisible |
| Every one of the 192 TUs is `-Oz` | **High (global), not per-TU exhaustive** | 192/192 size-exact, 176/192 fully byte-exact at `-Oz`; 31/192 individually swept |

## 8. Neutral cells: optimization-level-insensitive, not evidence for another flag

Two kinds of neutrality appear in the data. Neither is evidence that another level or flag
was used:

1. **`Globals::Globals` C1/C2: `-Oz` 80.5 == `-Os` 80.5.** The constructor body is short
   (612 bytes) and LLVM makes the same instruction-selection/inlining choices at `-Oz` and
   `-Os` for this one function. But the rest of `Globals.cpp` is not neutral: the TU-local
   exact count falls **53/55 → 3/55** and the global gate falls **4500 → 1473** at `-Os`.
   The function is optimization-level-insensitive; the TU is not.
2. **`-mllvm -unroll-threshold=0` and `-enable-machine-outliner=false` are byte-identical
   to plain `-Oz`.** These knobs are inert at `-Oz` for this codebase, so they neither
   improve nor degrade anything. They are neutral because they match the default `-Oz`
   behaviour, not because the original needed them.

No TU in the sweep is *globally* neutral: every per-TU non-`-Oz` variant lowers the global
gate. The only "neutral" whole-build statement is that `-Oz` plus an inert knob equals
`-Oz`, which is trivially true and unobservable.

## 9. Strict improvement?

**No strict improvement exists in the swept space.** There is no per-TU optimization-level
change and no tested `-mllvm` knob that raises `byte_exact` above **4500** or makes any
target function better than its `-Oz` value. Therefore:

* no `src/` change is landed;
* no build flag is changed;
* no baseline, tool or allowlist is touched;
* this commit contains only this note.

Gate before = after: **byte 4500, linked 4500, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0**.

## 10. Reproduction artifacts

All scratch artifacts are in `/dev/shm/w5215` (not tracked):

```text
toolchain_extra.cmake, per_tu_flags.cmake   harness
run_global.py, run_per_tu.py, run_mllvm.py  sweep drivers
fast_gate.py, score_tu.py                   verifier
results_global.json                         6 global builds
results_per_tu.json                         186 per-TU builds
results_mllvm.json                          7 -mllvm builds
verify/                                     per-variant gate/score JSON
analysis.txt                                full per-TU tables
```

The raw per-variant JSON files (`verify/global_*_gate.json`,
`verify/pt_*_gate.json`, `verify/mllvm_*_gate.json`, `verify/official_Layout_Os/report.json`)
contain the per-function rows behind every count in this note.
