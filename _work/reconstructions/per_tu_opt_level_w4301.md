# Per-TU optimization level and language-standard sweep (w4301, wave 93)

Status: research note, 2026-11-03. Worktree `/home/debian/GalaxyOnFire2/wt/w4301`,
branch `wave93-w4301`, base `eea2e3c1`. Scratch: `/dev/shm/w4301`. **No `src/` or
build-flag change is landed**: no per-TU optimization level or language standard
strictly raises `byte_exact`, and every alternative level makes the target function
worse. This commit contains this note (plus a stale-doc correction, §8).

## 0. Bottom line

* **The original is consistent with a single `-Oz` for every TU.** All 192 app TUs
  (4,282 unique app functions) are **100% size-exact** against the original at `-Oz`;
  176 TUs are also 100% byte-exact. The only 16 TUs with any non-byte-exact function
  are exactly the 16 target TUs of this sweep. If any TU had been compiled at
  `-O1/-O2/-Os/-O3`, its function sizes would differ from the `-Oz` reference; they
  do not.
* **Per-TU `-O1/-O2/-Os/-O3` never helps.** For all 16 target TUs the target
  function's fuzzy % is highest at `-Oz`; every alternative lowers it. Global
  `byte_exact` for the 64 optimization variants ranges **1343–2965** (vs 4499 at
  `-Oz`), and `.text` grows by up to `+0x6160`. No variant raises `byte_exact` or
  fixes a target without breaking layout.
* **Per-TU `-std=c++11` and `-std=gnu++11` are exact no-ops** (byte-identical `.so`
  to the canonical build in the same build directory; `byte 4499`, `.text 0x1337c8`).
  Per-TU `-std=c++14` is also a no-op for 15 of 16 TUs, but changes `Gun::Gun`
  C1/C2 from 100.0% to 95.8% and drops `byte_exact` 4499 → 4497. The original
  therefore used **c++11 or gnu++11** (indistinguishable on this source), not c++14.
* **Instruction-selection/prologue fingerprint is uniform across TUs and matches
  `-Oz`.** At `-Oz` the first-instruction prologue matches the original for
  100% of functions in every target TU, the instruction count matches for
  ~95–100%, and the selected-mnemonic L1 distance is 0–366. At `-O2`/`-Os` the
  prologue match drops (e.g. `Layout.cpp` 50/53, `Status.cpp` 131/138) and the
  mnemonic L1 distance explodes (e.g. `LevelScript.cpp` 2,549 / 2,032).
* No change landed. Gate before = after: **byte 4499, linked 4499, size 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0**.

## 1. Baseline and method

Baseline (`cmake --build cmake-build-match --target check -j8`):

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes (ours = original):

```text
.text      0x1337c8 @ 0x69440
.rodata    0x041b67 @ 0x1c9e10
.ARM.exidx 0x006f70
.ARM.extab 0x026298
.data      0x00296c
.bss       0x0069ac
```

The 25 non-byte-exact functions are the same list as `docs/IDENTICALITY.md`, with
one correction: the fresh report gives `Hud::draw` **89.1%**, not the stale 86.2%
in the committed `report.json`/docs (see §8).

### 1.1 Harness

Every variant is one TU at a time, in its own scratch build directory, using a
temporary toolchain file that includes the real `cmake/toolchain.cmake` and a
`CMAKE_PROJECT_INCLUDE` file that appends the candidate flag to exactly one source:

```cmake
# /dev/shm/w4301/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w4301/cmake/toolchain.cmake")
```

```cmake
# /dev/shm/w4301/per_tu_flags.cmake  (included at the end of project())
if(GOF2_AB_TU AND GOF2_AB_FLAGS)
  set_source_files_properties("${CMAKE_SOURCE_DIR}/${GOF2_AB_TU}"
      PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
endif()
```

`COMPILE_FLAGS` is emitted after `CMAKE_CXX_FLAGS` and after the target's
`COMPILE_OPTIONS`, so the candidate `-O…`/`-std=…` is the last one on the command
line and wins. Verified in `compile_commands.json` (e.g. `Layout.cpp` ends
`… -ffunction-sections -funwind-tables -O2 -o …`). The canonical
`cmake-build-match` directory was not used as a scratch build.

One variant:

```sh
cmake -S . -B /dev/shm/w4301/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4301/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DCMAKE_PROJECT_INCLUDE=/dev/shm/w4301/per_tu_flags.cmake \
  -DGOF2_AB_TU=src/game/ui/Layout.cpp -DGOF2_AB_FLAGS=-O2
cmake --build /dev/shm/w4301/build --target link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w4301/verify/Layout__O2 \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4301/build/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

All 112 variants (16 TUs × 7 flags) were measured with the official
`tools/verify/verify.py`; the raw summaries are `/dev/shm/w4301/verify_results.json`
and the per-variant `report.json` files. The per-TU `-Oz` control was run for all
16 TUs and produced a **byte-identical `.so`** (same sha256
`0aa43556c5a8ace4…` in `/dev/shm/w4301/build`) for every TU, proving the harness
applies the flag and that the differences below are real.

Build-id caveat: gold's build-id is a hash of the *unstripped* link output, which
contains `DW_AT_comp_dir`; therefore `.so` sha256 equality is only meaningful
within one build directory. All sha256 comparisons in this note are within
`/dev/shm/w4301/build`. The stripped section bytes are identical across build
directories except the 20-byte `.note.gnu.build-id`.

## 2. Per-TU optimization level

### 2.1 Target-function fuzzy %

`-Oz` is the canonical/global level. Every cell is the official verifier's `pct`
for that function.

| TU | target | -Oz | -O1 | -O2 | -Os | -O3 |
|---|---|---:|---:|---:|---:|---:|
| `Layout.cpp` | `_ZN6LayoutC1Ev` | 47.7 | 1.9 | 43.0 | 46.3 | 23.1 |
| `Layout.cpp` | `_ZN6LayoutC2Ev` | 47.7 | 1.9 | 43.0 | 46.3 | 23.1 |
| `LevelScript.cpp` | `_ZN11LevelScript7processEi` | 52.0 | 11.6 | 24.1 | 27.1 | 13.5 |
| `Status.cpp` | `_ZN6Status16missionCompletedEbbx` | 57.7 | 20.7 | 21.7 | 28.9 | 13.2 |
| `Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 73.9 | 20.7 | 24.5 | 27.8 | 31.0 |
| `Level.cpp` | `_ZN5Level10createShipEiiiP8Waypointbb` | 74.1 | 19.5 | 34.6 | 37.0 | 34.6 |
| `Level.cpp` | `_ZN5Level11createSceneEv` | 88.3 | 49.0 | 49.5 | 53.9 | 38.5 |
| `Level.cpp` | `_ZN5Level13createMissionEv` | 90.7 | 22.5 | 13.3 | 40.1 | 14.6 |
| `Generator.cpp` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 74.3 | 28.5 | 31.4 | 38.9 | 24.6 |
| `MGame.cpp` | `_ZN5MGame10OnTouchEndEiiPv` | 78.4 | 42.2 | 24.0 | 59.2 | 23.4 |
| `MGame.cpp` | `_ZN5MGame8OnUpdateEv` | 99.9 | 32.1 | 29.1 | 63.5 | 28.8 |
| `HangarWindow.cpp` | `_ZN12HangarWindow6renderEv` | 78.8 | 6.6 | 57.8 | 58.7 | 38.2 |
| `Globals.cpp` | `_ZN7GlobalsC1Ev` | 80.5 | 40.0 | 71.1 | 80.5 | 71.1 |
| `Globals.cpp` | `_ZN7GlobalsC2Ev` | 80.5 | 40.0 | 71.1 | 80.5 | 71.1 |
| `RocketGun.cpp` | `_ZN9RocketGun6updateEi` | 86.9 | 42.3 | 56.6 | 68.9 | 51.5 |
| `PlayerEgo.cpp` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 87.3 | 44.6 | 59.1 | 67.0 | 61.6 |
| `Gun.cpp` | `_ZN3Gun22calcCharacterCollisionEv` | 90.0 | 37.6 | 63.2 | 63.6 | 62.9 |
| `Hud.cpp` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 89.1 | 56.7 | 51.6 | 59.4 | 49.4 |
| `MenuTouchWindow.cpp` | `_ZN15MenuTouchWindowC1Ei` | 94.2 | 54.1 | 53.7 | 54.0 | 44.3 |
| `MenuTouchWindow.cpp` | `_ZN15MenuTouchWindowC2Ei` | 94.2 | 54.1 | 53.7 | 54.0 | 44.3 |
| `MenuTouchWindow.cpp` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 97.7 | 36.0 | 44.3 | 44.3 | 44.1 |
| `MenuTouchWindow.cpp` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | 97.9 | 10.7 | 31.1 | 39.7 | 31.0 |
| `SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | 95.5 | 59.3 | 78.0 | 77.9 | 77.4 |
| `ModStation.cpp` | `_ZN10ModStationC1Ev` | 98.4 | 44.2 | 90.8 | 93.1 | 87.2 |
| `ModStation.cpp` | `_ZN10ModStationC2Ev` | 98.4 | 44.2 | 90.8 | 93.1 | 87.2 |

**No cell improves on `-Oz`.** The only target-neutral cell is `Globals.cpp` with
`-Os` (80.5%, same 612-byte body); it still breaks the rest of the TU (next table)
and shifts `.text`, so it is not a "fix without breaking layout".

### 2.2 TU-local byte-exact count

Number of functions in that TU that are byte-exact against the original. `-Oz`
is 100% or near-100% everywhere; every alternative collapses.

| TU | funcs | -Oz | -O1 | -O2 | -Os | -O3 |
|---|---:|---:|---:|---:|---:|---:|
| `Layout.cpp` | 53 | 51 | 5 | 5 | 5 | 5 |
| `LevelScript.cpp` | 22 | 21 | 6 | 6 | 6 | 6 |
| `Status.cpp` | 147 | 146 | 62 | 67 | 68 | 67 |
| `Radar.cpp` | 26 | 25 | 8 | 8 | 9 | 8 |
| `Level.cpp` | 131 | 128 | 30 | 31 | 33 | 31 |
| `Generator.cpp` | 27 | 26 | 4 | 9 | 9 | 9 |
| `MGame.cpp` | 48 | 46 | 9 | 9 | 11 | 9 |
| `HangarWindow.cpp` | 40 | 39 | 3 | 3 | 7 | 3 |
| `Globals.cpp` | 55 | 53 | 2 | 2 | 3 | 2 |
| `RocketGun.cpp` | 21 | 20 | 6 | 6 | 11 | 6 |
| `PlayerEgo.cpp` | 177 | 176 | 55 | 60 | 73 | 60 |
| `Gun.cpp` | 38 | 37 | 13 | 13 | 13 | 13 |
| `Hud.cpp` | 60 | 59 | 18 | 18 | 19 | 18 |
| `MenuTouchWindow.cpp` | 50 | 46 | 5 | 5 | 7 | 5 |
| `SpaceLounge.cpp` | 46 | 45 | 9 | 9 | 9 | 9 |
| `ModStation.cpp` | 45 | 43 | 7 | 7 | 7 | 7 |

### 2.3 Non-`-Oz` levels also lose symbols

At non-`-Oz` levels clang inlines and can stop emitting some weak template
instantiations that the original exports. This would fail the `missing` gate even
if layout were ignored. `missing` = `4524 − compared` (wrong-type is 0 in these
variants; `extra` does not affect `compared`):

| TU | -O1 | -O2 | -Os | -O3 |
|---|---:|---:|---:|---:|
| `Layout.cpp` | 0 | 0 | 0 | 0 |
| `LevelScript.cpp` | 0 | 0 | 0 | 0 |
| `Status.cpp` | 0 | 9 | 4 | 9 |
| `Radar.cpp` | 0 | 0 | 0 | 0 |
| `Level.cpp` | 0 | 8 | 5 | 8 |
| `Generator.cpp` | 0 | 0 | 0 | 0 |
| `MGame.cpp` | 0 | 0 | 0 | 0 |
| `HangarWindow.cpp` | 0 | 0 | 0 | 0 |
| `Globals.cpp` | 0 | 1 | 1 | 1 |
| `RocketGun.cpp` | 0 | 0 | 0 | 0 |
| `PlayerEgo.cpp` | 0 | 4 | 3 | 4 |
| `Gun.cpp` | 0 | 0 | 0 | 0 |
| `Hud.cpp` | 0 | 2 | 1 | 2 |
| `MenuTouchWindow.cpp` | 0 | 5 | 5 | 5 |
| `SpaceLounge.cpp` | 0 | 3 | 2 | 3 |
| `ModStation.cpp` | 0 | 2 | 1 | 2 |

### 2.4 Global gate

Across the 64 optimization variants:

| flag | byte_exact range | linked_exact range | size-exact range | `.text` |
|---|---|---|---|---|
| `-O1` | 1346–1485 | 4408–4494 | 4461–4516 | 0x133978–0x135828 |
| `-O2` | 1343–1668 | 4424–4493 | 4456–4516 | 0x1339c8–0x136a68 |
| `-Os` | 1353–2965 | 4448–4493 | 4474–4519 | 0x133888–0x134858 |
| `-O3` | 1343–1791 | 4422–4493 | 4454–4516 | 0x133ac8–0x139928 |

`.rodata` stays `0x41b67` in every optimization variant (the per-TU flag does not
change data emission). The full 112-row table is in Appendix A.

## 3. Per-TU language standard

`-std=c++11` and `-std=gnu++11` are exact no-ops: every per-TU variant produces a
`.so` byte-identical (same sha256 in `/dev/shm/w4301/build`) to the canonical
build, with `byte 4499`, `linked 4499`, `size 4524/4524`, `.text 0x1337c8`,
`.rodata 0x41b67`. `-std=c++14` is also a no-op for 15 of 16 TUs; the exception is
`Gun.cpp`.

| TU | target | -Oz | c++11 | gnu++11 | c++14 | global byte (c++11 / gnu++11 / c++14) |
|---|---|---:|---:|---:|---:|---|
| `Layout.cpp` | `Layout::Layout` C1/C2 | 47.7 | 47.7 | 47.7 | 47.7 | 4499 / 4499 / 4499 |
| `LevelScript.cpp` | `LevelScript::process` | 52.0 | 52.0 | 52.0 | 52.0 | 4499 / 4499 / 4499 |
| `Status.cpp` | `Status::missionCompleted` | 57.7 | 57.7 | 57.7 | 57.7 | 4499 / 4499 / 4499 |
| `Radar.cpp` | `Radar::draw` | 73.9 | 73.9 | 73.9 | 73.9 | 4499 / 4499 / 4499 |
| `Level.cpp` | `createShip` / `createScene` / `createMission` | 74.1 / 88.3 / 90.7 | same | same | same | 4499 / 4499 / 4499 |
| `Generator.cpp` | `Generator::createMission` | 74.3 | 74.3 | 74.3 | 74.3 | 4499 / 4499 / 4499 |
| `MGame.cpp` | `OnTouchEnd` / `OnUpdate` | 78.4 / 99.9 | same | same | same | 4499 / 4499 / 4499 |
| `HangarWindow.cpp` | `HangarWindow::render` | 78.8 | 78.8 | 78.8 | 78.8 | 4499 / 4499 / 4499 |
| `Globals.cpp` | `Globals::Globals` C1/C2 | 80.5 | 80.5 | 80.5 | 80.5 | 4499 / 4499 / 4499 |
| `RocketGun.cpp` | `RocketGun::update` | 86.9 | 86.9 | 86.9 | 86.9 | 4499 / 4499 / 4499 |
| `PlayerEgo.cpp` | `PlayerEgo::calcCollision` | 87.3 | 87.3 | 87.3 | 87.3 | 4499 / 4499 / 4499 |
| `Gun.cpp` | `Gun::calcCharacterCollision` | 90.0 | 90.0 | 90.0 | 90.0 | 4499 / 4499 / **4497** |
| `Hud.cpp` | `Hud::draw` | 89.1 | 89.1 | 89.1 | 89.1 | 4499 / 4499 / 4499 |
| `MenuTouchWindow.cpp` | ctor / OnTouchBegin / OnTouchEnd | 94.2 / 97.7 / 97.9 | same | same | same | 4499 / 4499 / 4499 |
| `SpaceLounge.cpp` | `SpaceLounge::drawLounge` | 95.5 | 95.5 | 95.5 | 95.5 | 4499 / 4499 / 4499 |
| `ModStation.cpp` | `ModStation::ModStation` C1/C2 | 98.4 | 98.4 | 98.4 | 98.4 | 4499 / 4499 / 4499 |

`Gun.cpp` + `-std=c++14` changes exactly two functions:

```text
_ZN3GunC1EiiiiiifN11AbyssEngine6AEMath6VectorES2_  100.0 -> 95.8  (532 B, size same)
_ZN3GunC2EiiiiiifN11AbyssEngine6AEMath6VectorES2_  100.0 -> 95.8  (532 B, size same)
```

This is enough to rule out c++14 for `Gun.cpp`; it is neutral (unobservable) in the
other 15 TUs. The global `-std=c++14` build is already known to be catastrophic
(wave-92 matrix: `byte_exact 1512`, `linked_exact 4485`), so the original's
standard is **c++11 or gnu++11**, which are codegen-identical on this source.

## 4. Instruction-selection / prologue fingerprint

For each function in a target TU, the original and the variant are disassembled
with the verifier's own Thumb decoder. The fingerprint is:

* **prologue**: exact first instruction text (e.g. `push {r4, r5, r6, r7, lr}`,
  `push {r4, lr}`, `sub sp, #…`);
* **nins**: number of code instructions (pool/`.word`/`.short` excluded);
* **mnem L1**: L1 distance of the counts of 37 selected mnemonics
  (`mov movw movt ldr str add sub cmp bl blx b cbz cbnz it push pop vldr vstr
  vpush vpop tbb tbh ldrd strd mul mla sdiv udiv uxtb sxtb uxth sxth orr and lsl
  lsr asr`) between original and variant, summed over the TU.

| TU | funcs | -Oz prologue | -O2 prologue | -Os prologue | -Oz nins | -O2 nins | -Os nins | -Oz mnem L1 | -O2 mnem L1 | -Os mnem L1 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `Layout.cpp` | 53 | 53/53 | 50/53 | 50/53 | 51/53 | 27/53 | 33/53 | 170 | 436 | 330 |
| `LevelScript.cpp` | 22 | 22/22 | 21/22 | 21/22 | 21/22 | 14/22 | 15/22 | 366 | 2549 | 2032 |
| `Status.cpp` | 147 | 147/147 | 131/138 | 138/143 | 146/147 | 101/138 | 104/143 | 5 | 2191 | 404 |
| `Radar.cpp` | 26 | 26/26 | 23/26 | 23/26 | 25/26 | 16/26 | 18/26 | 29 | 274 | 187 |
| `Level.cpp` | 131 | 131/131 | 118/123 | 123/126 | 129/131 | 71/123 | 82/126 | 84 | 4181 | 3134 |
| `Generator.cpp` | 27 | 27/27 | 27/27 | 27/27 | 26/27 | 19/27 | 19/27 | 13 | 304 | 236 |
| `MGame.cpp` | 48 | 48/48 | 47/48 | 48/48 | 46/48 | 17/48 | 28/48 | 9 | 1284 | 458 |
| `HangarWindow.cpp` | 40 | 40/40 | 38/40 | 38/40 | 39/40 | 20/40 | 25/40 | 48 | 693 | 278 |
| `Globals.cpp` | 55 | 55/55 | 50/54 | 50/54 | 53/55 | 28/54 | 33/54 | 4 | 424 | 252 |
| `RocketGun.cpp` | 21 | 21/21 | 18/21 | 18/21 | 20/21 | 14/21 | 14/21 | 12 | 196 | 64 |
| `PlayerEgo.cpp` | 177 | 177/177 | 161/173 | 163/174 | 176/177 | 110/173 | 130/174 | 7 | 878 | 531 |
| `Gun.cpp` | 38 | 38/38 | 34/38 | 36/38 | 38/38 | 26/38 | 25/38 | 4 | 391 | 220 |
| `Hud.cpp` | 60 | 60/60 | 55/58 | 57/59 | 59/60 | 32/58 | 33/59 | 31 | 989 | 382 |
| `MenuTouchWindow.cpp` | 50 | 50/50 | 41/45 | 41/45 | 49/50 | 23/45 | 25/45 | 22 | 1172 | 807 |
| `SpaceLounge.cpp` | 46 | 46/46 | 43/43 | 44/44 | 46/46 | 26/43 | 29/44 | 3 | 615 | 417 |
| `ModStation.cpp` | 45 | 45/45 | 41/43 | 43/44 | 45/45 | 21/43 | 30/44 | 0 | 876 | 253 |

At `-Oz` the prologue is exact for every function in every target TU (the only
non-100% cells are due to the 25 known body mismatches, not prologue shape). At
`-O2`/`-Os` the prologue, instruction count and mnemonic mix all diverge. The
pattern is uniform across TUs: no TU looks like it was built at a different level.

## 5. Single-level consistency across all TUs

The 4,282 unique app functions (192 TUs) were classified against the fresh
baseline report:

```text
app TUs:                     192
100% size-exact at -Oz:      192 / 192
100% byte-exact at -Oz:      176 / 192
TUs with any non-byte-exact:  16 / 192  (exactly the 16 target TUs)
non-byte-exact app functions: 25 / 4,282
```

The 16 TUs with any mismatch are exactly the 16 target TUs of this sweep. The
other 176 TUs are 100% byte-exact at `-Oz`, which is already the strongest
possible evidence that they are `-Oz`: a different optimization level would have
to reproduce every byte of every function in the TU by coincidence.

To check that this is not just a property of the target TUs, four large non-target
TUs were also swept at `-O2`/`-Os`:

| TU | funcs | -Oz byte | -O2 TU byte | -Os TU byte | -O2 global byte | -Os global byte |
|---|---:|---:|---:|---:|---:|---:|
| `PaintCanvas.cpp` | 231 | 231/231 | 22/231 | 29/231 | 1345 | 1352 |
| `Engine.cpp` | 80 | 80/80 | 15/80 | 17/80 | 1353 | 1355 |
| `Player.cpp` | 116 | 116/116 | 49/116 | 53/116 | 1349 | 1380 |
| `Ship.cpp` | 100 | 100/100 | 41/100 | 43/100 | 1347 | 1358 |

These TUs are 100% byte-exact at `-Oz` and collapse at `-O2`/`-Os`, exactly like
the target TUs. The original's per-TU codegen is therefore consistent with one
global `-Oz` for the whole app.

## 6. Verdict per hypothesis

| hypothesis | verdict | evidence |
|---|---|---|
| Not every TU was compiled at `-Oz`. | **Disproven** for all 16 target TUs; **likely false** for the other 176. | Every target TU's target function is best at `-Oz`; TU-local byte-exact counts collapse at every other level; all 192 TUs are 100% size-exact at `-Oz`; four large non-target TUs are 100% byte-exact at `-Oz` and collapse at `-O2`/`-Os`. |
| Not every TU used the same language standard. | **Disproven for c++14 in `Gun.cpp`; unobservable for c++11 vs gnu++11.** | Per-TU c++11/gnu++11 are byte-identical no-ops; per-TU c++14 is a no-op in 15/16 TUs but changes `Gun::Gun` (100.0 → 95.8, byte 4499 → 4497). Global c++14 is catastrophic (1512). |
| A per-TU `-O1/-O2/-Os/-O3` can fix a target without breaking layout. | **Disproven.** | Every non-`-Oz` cell lowers the target fuzzy %; `Globals.cpp -Os` is target-neutral (80.5, 612 B) but breaks the rest of the TU (3/55 vs 53/55) and shifts `.text`. |
| A per-TU level strictly raises `byte_exact`. | **Disproven.** | Max global `byte_exact` among the 64 optimization variants is 2965 (`RocketGun.cpp -Os`), far below 4499. |
| The original's per-TU codegen is consistent with a single level. | **Proven.** | 192/192 TUs size-exact at `-Oz`; 176/192 100% byte-exact; prologue/instruction-count/mnemonic fingerprint matches `-Oz` and diverges at `-O2`/`-Os` uniformly across TUs. |

## 7. What remains

* The 25 non-byte-exact functions are **source-shape**, not flag, differences.
  The remaining gap needs source/TU work, not a per-TU optimization flag.
* `c++11` vs `gnu++11` is unobservable on this source (identical codegen). The
  canonical `-std=c++11` is a valid representative; the CMake r18b toolchain's
  effective `-std=gnu++11` is equally consistent.
* A per-TU `c++14` in a TU where it is a no-op cannot be excluded from the binary
  alone, but there is no positive evidence for it and `Gun.cpp` rules it out
  there. The parsimonious reading is one standard for the whole build.
* The four per-file `-mllvm -likely-branch-weight` overrides are a separate
  source-shape compensation (see `build_flags_matrix_w4207.md`); they are not
  original build flags and are unaffected by this sweep.
* Process observation: the tracked root `report.json` is stale (`Hud::draw`
  86.2) while `cmake-build-match/verify/report.json` is fresh (89.1). The
  `check` target runs `ratchet.py` and does not copy `report.json` to the root;
  only the `verify` target does. §8 corrects the two stale doc numbers.

## 8. Doc correction

`docs/IDENTICALITY.md` states `Hud::draw` 86.2% in two places (the current-state
paragraph and the priority list). The fresh official verifier report gives
**89.1%** for the current tree. This commit updates those two numbers. No gate
count changes.

## 9. Reproduction

Scratch artifacts (not tracked):

```text
/dev/shm/w4301/toolchain_extra.cmake       includes the real toolchain
/dev/shm/w4301/per_tu_flags.cmake          per-source COMPILE_FLAGS
/dev/shm/w4301/run_sweep.py                112-variant build driver
/dev/shm/w4301/verify_all.py               official-verifier driver (4 workers)
/dev/shm/w4301/verify_results.json         global gate per variant
/dev/shm/w4301/verify/<tag>/report.json    per-variant official report
/dev/shm/w4301/sos/<tag>.so                per-variant linked .so
/dev/shm/w4301/targets.json                TU -> target mangled names
/dev/shm/w4301/tu_map.json                 function -> TU (nm on the build objects)
/dev/shm/w4301/fingerprint.py/.txt         prologue/instruction fingerprint
/dev/shm/w4301/run_extra.py, verify_extra.py  large non-target TU check
```

The canonical `cmake-build-match` was used only for the baseline and final
`check`; all variants were built in `/dev/shm/w4301/build`.

## Appendix A — full 112-variant global gate

`size-exact` is `exact/compared`; `compared` drops when a non-`-Oz` level fails
to emit weak template instantiations (see §2.3). `.rodata` is `0x41b67` for every
row and is omitted.

| TU | flag | byte_exact | linked_exact | size-exact | .text |
|---|---:|---:|---:|---:|---|
| `Layout.cpp` | `-O1` | 1484 | 4462 | 4500/4524 | `0x133c68` |
| `Layout.cpp` | `-O2` | 1369 | 4471 | 4498/4524 | `0x133a58` |
| `Layout.cpp` | `-Os` | 1506 | 4478 | 4507/4524 | `0x1338a8` |
| `Layout.cpp` | `-O3` | 1369 | 4471 | 4497/4524 | `0x133b88` |
| `Layout.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Layout.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Layout.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `LevelScript.cpp` | `-O1` | 1369 | 4491 | 4515/4524 | `0x134ae8` |
| `LevelScript.cpp` | `-O2` | 1453 | 4492 | 4516/4524 | `0x135168` |
| `LevelScript.cpp` | `-Os` | 1378 | 4493 | 4517/4524 | `0x1339a8` |
| `LevelScript.cpp` | `-O3` | 1369 | 4491 | 4515/4524 | `0x136048` |
| `LevelScript.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `LevelScript.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `LevelScript.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Status.cpp` | `-O1` | 1346 | 4430 | 4472/4524 | `0x134128` |
| `Status.cpp` | `-O2` | 1380 | 4452 | 4478/4515 | `0x135578` | `0x41b07` |
| `Status.cpp` | `-Os` | 1365 | 4453 | 4480/4520 | `0x133a68` |
| `Status.cpp` | `-O3` | 1351 | 4451 | 4477/4515 | `0x136404` | `0x41b07` |
| `Status.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Status.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Status.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Radar.cpp` | `-O1` | 1368 | 4490 | 4516/4524 | `0x133b88` |
| `Radar.cpp` | `-O2` | 1377 | 4490 | 4514/4524 | `0x133b08` |
| `Radar.cpp` | `-Os` | 1387 | 4492 | 4516/4524 | `0x1339c8` |
| `Radar.cpp` | `-O3` | 1377 | 4490 | 4514/4524 | `0x133d88` |
| `Radar.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Radar.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Radar.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Level.cpp` | `-O1` | 1485 | 4427 | 4466/4524 | `0x135828` | `0x41b77` |
| `Level.cpp` | `-O2` | 1351 | 4441 | 4465/4516 | `0x136a68` |
| `Level.cpp` | `-Os` | 1353 | 4448 | 4474/4519 | `0x134858` |
| `Level.cpp` | `-O3` | 1351 | 4441 | 4465/4516 | `0x139928` |
| `Level.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Level.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Level.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Generator.cpp` | `-O1` | 1369 | 4482 | 4516/4524 | `0x133d28` |
| `Generator.cpp` | `-O2` | 1668 | 4492 | 4516/4524 | `0x133c28` |
| `Generator.cpp` | `-Os` | 1668 | 4492 | 4516/4524 | `0x133b88` |
| `Generator.cpp` | `-O3` | 1791 | 4492 | 4516/4524 | `0x134188` | `0x41a87` |
| `Generator.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Generator.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Generator.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `MGame.cpp` | `-O1` | 1355 | 4467 | 4494/4524 | `0x133f48` |
| `MGame.cpp` | `-O2` | 1355 | 4469 | 4495/4524 | `0x1352c8` | `0x41b57` |
| `MGame.cpp` | `-Os` | 2201 | 4477 | 4507/4524 | `0x1339a8` |
| `MGame.cpp` | `-O3` | 1358 | 4468 | 4494/4524 | `0x1355e8` | `0x41b57` |
| `MGame.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `MGame.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `MGame.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `HangarWindow.cpp` | `-O1` | 1439 | 4475 | 4505/4524 | `0x133ca8` |
| `HangarWindow.cpp` | `-O2` | 1352 | 4479 | 4505/4524 | `0x133ed8` |
| `HangarWindow.cpp` | `-Os` | 1369 | 4483 | 4507/4524 | `0x133948` |
| `HangarWindow.cpp` | `-O3` | 1352 | 4479 | 4503/4524 | `0x1346f8` |
| `HangarWindow.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `HangarWindow.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `HangarWindow.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Globals.cpp` | `-O1` | 1369 | 4467 | 4500/4524 | `0x133d28` |
| `Globals.cpp` | `-O2` | 1369 | 4473 | 4497/4523 | `0x133f68` |
| `Globals.cpp` | `-Os` | 1473 | 4479 | 4505/4523 | `0x1339e8` |
| `Globals.cpp` | `-O3` | 1369 | 4473 | 4497/4523 | `0x1342e8` |
| `Globals.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Globals.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Globals.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `RocketGun.cpp` | `-O1` | 1356 | 4494 | 4521/4524 | `0x133978` |
| `RocketGun.cpp` | `-O2` | 1378 | 4493 | 4519/4524 | `0x1339c8` |
| `RocketGun.cpp` | `-Os` | 2965 | 4493 | 4519/4524 | `0x133888` |
| `RocketGun.cpp` | `-O3` | 1369 | 4493 | 4517/4524 | `0x133ac8` |
| `RocketGun.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `RocketGun.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `RocketGun.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `PlayerEgo.cpp` | `-O1` | 1347 | 4408 | 4461/4524 | `0x133c78` |
| `PlayerEgo.cpp` | `-O2` | 1343 | 4424 | 4456/4520 | `0x134148` |
| `PlayerEgo.cpp` | `-Os` | 1371 | 4448 | 4478/4521 | `0x133b38` |
| `PlayerEgo.cpp` | `-O3` | 1343 | 4422 | 4454/4520 | `0x134348` |
| `PlayerEgo.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `PlayerEgo.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `PlayerEgo.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Gun.cpp` | `-O1` | 1369 | 4485 | 4511/4524 | `0x133a68` |
| `Gun.cpp` | `-O2` | 1356 | 4486 | 4511/4524 | `0x133a98` |
| `Gun.cpp` | `-Os` | 1365 | 4486 | 4512/4524 | `0x1338f8` |
| `Gun.cpp` | `-O3` | 1416 | 4486 | 4510/4524 | `0x133ad8` |
| `Gun.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Gun.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Gun.cpp` | `-std=c++14` | 4497 | 4497 | 4524/4524 | `0x1337c8` |
| `Hud.cpp` | `-O1` | 1375 | 4469 | 4498/4524 | `0x133c48` |
| `Hud.cpp` | `-O2` | 1384 | 4469 | 4497/4522 | `0x1345b8` |
| `Hud.cpp` | `-Os` | 1354 | 4471 | 4498/4523 | `0x133c58` |
| `Hud.cpp` | `-O3` | 1367 | 4469 | 4494/4522 | `0x134d98` |
| `Hud.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Hud.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `Hud.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `MenuTouchWindow.cpp` | `-O1` | 1379 | 4467 | 4495/4524 | `0x134218` |
| `MenuTouchWindow.cpp` | `-O2` | 1367 | 4476 | 4497/4519 | `0x1343c8` |
| `MenuTouchWindow.cpp` | `-Os` | 1369 | 4477 | 4499/4519 | `0x133c08` |
| `MenuTouchWindow.cpp` | `-O3` | 1367 | 4475 | 4496/4519 | `0x134c88` | `0x41b47` |
| `MenuTouchWindow.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `MenuTouchWindow.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `MenuTouchWindow.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `SpaceLounge.cpp` | `-O1` | 1412 | 4478 | 4504/4524 | `0x133cc8` | `0x41b97` |
| `SpaceLounge.cpp` | `-O2` | 1369 | 4480 | 4504/4521 | `0x134048` |
| `SpaceLounge.cpp` | `-Os` | 1369 | 4481 | 4506/4522 | `0x133ba8` |
| `SpaceLounge.cpp` | `-O3` | 1369 | 4480 | 4504/4521 | `0x1345c8` |
| `SpaceLounge.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `SpaceLounge.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `SpaceLounge.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `ModStation.cpp` | `-O1` | 1356 | 4470 | 4499/4524 | `0x133c68` |
| `ModStation.cpp` | `-O2` | 1356 | 4476 | 4501/4522 | `0x134418` |
| `ModStation.cpp` | `-Os` | 1369 | 4482 | 4509/4523 | `0x1339c8` |
| `ModStation.cpp` | `-O3` | 1439 | 4476 | 4500/4522 | `0x134c78` |
| `ModStation.cpp` | `-std=c++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `ModStation.cpp` | `-std=gnu++11` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
| `ModStation.cpp` | `-std=c++14` | 4499 | 4499 | 4524/4524 | `0x1337c8` |
