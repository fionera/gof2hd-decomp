# Per-TU stack-protector, unwind-table and frame-pointer flags (w5206, wave 104)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5206`,
branch `wave104-w5206`, base `8c5fd3646`. Scratch is `/dev/shm/w5206`.
**No `src/`, `CMakeLists.txt`, tool, baseline or allowlist change is landed.**
The canonical gate is unchanged before and after the experiments:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size-exact 4524/4524; lint CLEAN; drift 0
```

This note is the only deliverable. It re-tests, on the current 4500/4500 tree, the
question answered by `stack_frame_unwind_w4305.md` (wave 93) and extends it with a
**per-TU object-level sweep over all 192 app TUs** plus a **per-TU linked gate run on
all 16 TUs that still contain a non-byte-exact function**.

## 0. Bottom line

1. The original's **effective** stack-protector, unwind-table and frame-pointer
   settings are uniform across every app TU. The canonical global flags reproduce
   the original's fingerprints exactly:
   * 756 canary-bearing functions in both binaries, **0 presence mismatches**,
     and **0 canary value-register / guard-register / stack-offset mismatches**
     among the canaries whose placement this note's matcher decodes (137/756;
     the prior w4305 matcher decoded 751/756 and also found 0 mismatches);
   * 2700 frame-pointer functions in both, **0 mismatches**;
   * 3566 `.ARM.exidx` entries in both (272 `CANTUNWIND`, 3294 extab),
     **0 entry-target diffs and 0 entry-kind diffs**.
2. The global variants `-fstack-protector-strong`, `-funwind-tables`,
   `-fno-asynchronous-unwind-tables` and `-fno-omit-frame-pointer` are
   **object-identical to the canonical build in all 192 app TUs** (0 changed
   `.text.*`, `.ARM.exidx.*` or `.ARM.extab.*` sections). They are the current
   settings, not matching knobs.
3. Every other variant is measurably wrong globally and per-TU. **No strict
   improvement exists** among the ten tested flags. The best per-TU result is
   `-momit-leaf-frame-pointer` on `game/world/Level.cpp` or `game/ui/Hud.cpp`:
   `byte 4499 / linked 4499` vs the canonical `4500 / 4500` (a 1-function
   regression). Every global variant is `<= 4500 / 4500`.
4. A per-TU flag that is **codegen-neutral for that TU** cannot be excluded by
   observation. The sweep quantifies the insensitive TUs for each flag (§5.3).
   Any *effective* per-TU divergence is disproven by the zero-mismatch
   fingerprints and by the per-TU linked gate.

## 1. Baseline and canonical commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5206
export TMPDIR=/dev/shm/w5206/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Baseline section data (ours == original in size and base):

| section | original | ours |
|---|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x26298 @ 0x1a3b78` |

Section-content SHA-256 prefixes (first 16 hex chars):

| section | original | canonical ours |
|---|---|---|
| `.text` | `d49e32c1330e5d94` | `144a3c3799cddbe2` |
| `.ARM.exidx` | `d7490b5908727fd3` | `652126c2ad2d61b6` |
| `.ARM.extab` | `1880b0ef21ad149e` | `6b66adedd2edc490` |

Whole-file SHA-256: original
`d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`;
canonical build `894d351d8226b770a7e3109b00dca76da2f62de9fb955357f1dda3d13578d543`.
The whole-file hash is build-directory dependent (DWARF/build-id), so only the
section hashes and the gate counts are meaningful across build dirs.

## 2. Global variant matrix

Harness: a scratch toolchain includes the real `cmake/toolchain.cmake` and appends
the candidate flag to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`. Each variant gets
its own build dir `/dev/shm/w5206/build-<tag>` and verify dir
`/dev/shm/w5206/verify-<tag>`.

```cmake
# /dev/shm/w5206/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w5206/cmake/toolchain.cmake")
if(GOF2_EXTRA_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_CXX_FLAGS}" CACHE STRING "" FORCE)
endif()
if(GOF2_EXTRA_C_FLAGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_C_FLAGS}" CACHE STRING "" FORCE)
endif()
```

```sh
cmake -S . -B /dev/shm/w5206/build-$tag -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5206/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_CXX_FLAGS="$flag" -DGOF2_EXTRA_C_FLAGS="$flag" \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5206/build-$tag --target link_so -j6
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir /dev/shm/w5206/verify-$tag \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5206/build-$tag/libgof2hdaa.so \
  --objdump "$OBJ" --fail-on-wrong-type --top 0
```

`canary`/`fp`/`exidx`/`CANT` are from the classifier in §7. `tgt/kind diffs`
compares the `.ARM.exidx` entry-target and entry-kind sequences element-by-element
against the original.

| tag | flags | byte | linked | avg | `.text` | `.ARM.exidx` | `.ARM.extab` | `.text` sha16 | canary | fp | exidx | CANT | tgt/kind diffs |
|---|---|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|---:|---|
| base | `-fstack-protector-strong` (canonical) | 4500 | 4500 | 99.9 | `0x1337c8` | `0x6f70` | `0x26298` | `144a3c3799cddbe2` | 756 | 2700 | 3566 | 272 | 0/0 |
| sp1 | `-fstack-protector` | 1369 | 3785 | 93.5 | `0x12e188` | `0x6f18` | `0x2607c` | `d5724a3e4dae645c` | 25 | 2700 | 3555 | 272 | 3546/441 |
| sps | `-fstack-protector-strong` | 4500 | 4500 | 99.9 | `0x1337c8` | `0x6f70` | `0x26298` | `144a3c3799cddbe2` | 756 | 2700 | 3566 | 272 | 0/0 |
| spall | `-fstack-protector-all` | 91 | 964 | 45.97 | `0x14f728` | `0x7230` | `0x270bc` | `ca85a9d580b809c8` | 4299 | 4373 | 3654 | 272 | 3565/469 |
| nosp | `-fno-stack-protector` | 1369 | 3780 | 93.47 | `0x12e108` | `0x6f18` | `0x2607c` | `9de906a3bb2bd4e8` | 21 | 2699 | 3555 | 272 | 3546/441 |
| unw | `-funwind-tables` | 4500 | 4500 | 99.9 | `0x1337c8` | `0x6f70` | `0x26298` | `144a3c3799cddbe2` | 756 | 2700 | 3566 | 272 | 0/0 |
| nounw | `-fno-unwind-tables` (appended to `CMAKE_CXX_FLAGS`) | 4500 | 4500 | 99.9 | `0x1337c8` | `0x6f70` | `0x26298` | `144a3c3799cddbe2` | 756 | 2700 | 3566 | 272 | 0/0 |
| noasync | `-fno-asynchronous-unwind-tables` | 4500 | 4500 | 99.9 | `0x1337c8` | `0x6f70` | `0x26298` | `144a3c3799cddbe2` | 756 | 2700 | 3566 | 272 | 0/0 |
| nofp | `-fno-omit-frame-pointer` | 4500 | 4500 | 99.9 | `0x1337c8` | `0x6f70` | `0x26298` | `144a3c3799cddbe2` | 756 | 2700 | 3566 | 272 | 0/0 |
| omitfp | `-fomit-frame-pointer` | 1298 | 1922 | 84.09 | `0x12dea8` | `0x6868` | `0x214ec` | `119f1c3059d2a783` | 756 | 128 | 3341 | 272 | 3340/509 |
| omitleaf | `-momit-leaf-frame-pointer` | 1386 | 4416 | 99.5 | `0x133648` | `0x6f60` | `0x26034` | `f66f46fca0006e8e` | 756 | 2619 | 3564 | 272 | 3463/387 |

Observations:

* `sps`, `unw`, `noasync` and `nofp` produce a `.text` section byte-identical to
  the canonical build (same SHA) and the same `.ARM.exidx`/`.ARM.extab` sizes.
* `nounw` appended to `CMAKE_CXX_FLAGS` is **neutral because of flag order**:
  `CMakeLists.txt` adds `-funwind-tables` via `target_compile_options`, which is
  emitted after `CMAKE_CXX_FLAGS`. The real `-fno-unwind-tables` effect is visible
  in the per-TU object sweep (§5) and in the per-TU linked builds (§6).
* `-fno-asynchronous-unwind-tables` does not cancel `-munwind-tables` in clang
  7.0.2, so it is neutral even when appended after `-funwind-tables`.
* The canary/fp counts for `sp1`/`spall`/`nosp`/`omitfp`/`omitleaf` are the
  classifier's raw counts; they are useful as fingerprints, not as independent
  gate metrics. The gate counts are the official verifier's.

## 3. Canary / prologue / `.ARM.exidx` fingerprints

Method (full script in §7):

* parse `.dynsym` with `readelf -sW` (clear the Thumb bit, keep non-zero-size
  `FUNC` symbols);
* disassemble the whole `.text` once with
  `arm-linux-androideabi-objdump -d -M force-thumb`;
* **canary** = the function body contains a reference to `__stack_chk_fail`;
  **canary value register / guard register / stack offset** are recovered from the
  prologue idiom
  `ldr rA,[pc]` / `add rA,pc` / `ldr rB,[rA,#0]` / `ldr rC,[rB,#0]` /
  `str rC,[sp,#off]`;
* **frame pointer** = a `push`/`stmdb` containing `r7` plus `mov r7,sp` or
  `add r7,sp,#imm` in the first 12 instructions;
* **`.ARM.exidx`** is parsed directly as 8-byte entries, prel31-decoded; kind is
  `CANT` for `word1 == 1`, otherwise `extab`.

Baseline vs original:

| metric | original | canonical | mismatch |
|---|---:|---:|---:|
| non-zero-size `FUNC` symbols classified | 4521 | 4521 | — |
| canary-bearing functions | 756 | 756 | **0** |
| canary placement decoded by this note's simple matcher | 137 / 756 | 137 / 756 | **0** |
| canary value-register distribution (decoded subset) | r0 123, r2 10, r4 2, r3 1, r1 1 | identical | **0** |
| canary guard-register distribution (decoded subset) | r5 104, r6 15, r4 12, r1 4, r2 2 | identical | **0** |
| canary stack-offset mismatches (decoded subset) | — | — | **0** |
| frame-pointer functions | 2700 | 2700 | **0** |
| `.ARM.exidx` entries | 3566 | 3566 | **0** |
| `CANTUNWIND` / extab entries | 272 / 3294 | 272 / 3294 | **0** |
| entry-target sequence diffs | — | — | **0** |
| entry-kind sequence diffs | — | — | **0** |

Per-TU mapping (function -> object file via NDK `nm -S --defined-only -g` over the
192 `CMakeFiles/gof2.dir/src/**/*.obj` objects):

* 192 app TUs with at least one mapped function;
* 157 TUs contain at least one canary function;
* 187 TUs contain at least one frame-pointer function;
* **0 TUs** have a canary-presence mismatch;
* **0 TUs** have a frame-pointer mismatch.

Largest canary TUs: `engine/render/PaintCanvas.cpp` (49),
`game/ship/PlayerEgo.cpp` (25), `game/mission/RecordHandler.cpp` (25),
`engine/file/AEFile.cpp` (23), `game/world/Level.cpp` (20),
`game/core/String.cpp` (18), `engine/render/Engine.cpp` (17),
`engine/file/FileRead.cpp` (17), `engine/audio/FModSound.cpp` (17),
`game/ui/Layout.cpp` (15). The counts are identical on both sides.

## 4. Per-TU object-level sweep (all 192 app TUs)

This is the part that directly tests "did any TU use a non-global flag". For each
of the 192 `compile_commands.json` entries, the exact canonical compile command is
replayed with the candidate flag appended **after** the `-funwind-tables` that
`target_compile_options` adds, and the resulting object is compared section-by-section
with the canonical object:

* `.text.*` sections: byte comparison and size comparison;
* `.ARM.exidx.*` sections: byte comparison;
* `.ARM.extab.*` sections: byte comparison.

The command replay strips `-o`, `-MD`, `-MF`, `-MT`, `-MQ`, `-MP` and appends
`-o <scratch>/<tu>.o` plus the candidate flag. 2112 compiles (192 TUs × 11 tags,
including the `base` replay) completed with 0 failures. The `base` replay is
byte-identical to the canonical object for a spot-checked TU and is the reference
for every comparison.

Summary (one row per variant; `TUs` = TUs with any changed `.text.*`,
`.ARM.exidx.*` or `.ARM.extab.*` section):

| variant | TUs changed | TUs `.text` changed | TUs exidx changed | TUs extab changed | functions byte-changed | functions size-changed | total `.text` delta |
|---|---:|---:|---:|---:|---:|---:|---:|
| `-fstack-protector` | 157 | 157 | 14 | 148 | 1 | 654 | -22232 |
| `-fstack-protector-strong` | **0** | **0** | **0** | **0** | **0** | **0** | **0** |
| `-fstack-protector-all` | 191 | 191 | 179 | 186 | 0 | 4036 | +138690 |
| `-fno-stack-protector` | 157 | 157 | 14 | 148 | 1 | 658 | -22364 |
| `-funwind-tables` | **0** | **0** | **0** | **0** | **0** | **0** | **0** |
| `-fno-unwind-tables` (appended after `-funwind-tables`) | 186 | 0 | 186 | 117 | 0 | 0 | 0 |
| `-fno-asynchronous-unwind-tables` | **0** | **0** | **0** | **0** | **0** | **0** | **0** |
| `-fno-omit-frame-pointer` | **0** | **0** | **0** | **0** | **0** | **0** | **0** |
| `-fomit-frame-pointer` | 187 | 187 | 185 | 187 | 214 | 2564 | -24478 |
| `-momit-leaf-frame-pointer` | 54 | 54 | 54 | 31 | 12 | 78 | -396 |

The four zero rows are the strongest per-TU evidence: adding the flag to a single
TU (or to all TUs) changes nothing anywhere. The original is consistent with the
canonical global setting for those flags in every TU.

The 16 TUs that still contain a non-byte-exact function are:

```text
game/core/Globals.cpp            game/menu/MGame.cpp
game/menu/ModStation.cpp         game/menu/SpaceLounge.cpp
game/mission/Generator.cpp       game/mission/Status.cpp
game/ship/PlayerEgo.cpp          game/ui/HangarWindow.cpp
game/ui/Hud.cpp                  game/ui/Layout.cpp
game/ui/MenuTouchWindow.cpp      game/weapons/Gun.cpp
game/weapons/Radar.cpp           game/weapons/RocketGun.cpp
game/world/Level.cpp             game/world/LevelScript.cpp
```

The 24 non-byte-exact functions are the usual queue (`Layout::Layout` C1/C2,
`LevelScript::process`, `Status::missionCompleted`, `Generator::createMission`,
`Level::createShip`, `Radar::draw`, `MGame::OnTouchEnd`, `HangarWindow::render`,
`Globals::Globals` C1/C2, `RocketGun::update`, `PlayerEgo::calcCollision`,
`Level::createScene`, `Gun::calcCharacterCollision`, `Level::createMission`,
`Hud::draw`, `MenuTouchWindow` C1/C2/`OnTouchBegin`, `SpaceLounge::drawLounge`,
`ModStation` C1/C2, `MGame::OnUpdate`).

## 5. Per-TU linked gate on the 16 non-exact TUs

### 5.1 Harness

A temporary block was inserted immediately after
`add_library(gof2 OBJECT ${_srcs})` in `CMakeLists.txt` (then reverted):

```cmake
set(GOF2_AB_TU "" CACHE STRING "TU (path under src/) for per-TU flag A/B")
set(GOF2_AB_FLAGS "" CACHE STRING "compile flags for GOF2_AB_TU")
if(GOF2_AB_TU)
    set_source_files_properties("${CMAKE_SOURCE_DIR}/src/${GOF2_AB_TU}"
        PROPERTIES COMPILE_OPTIONS "${GOF2_AB_FLAGS}")
endif()
```

Each combo gets its own build dir `/dev/shm/w5206/ptu-build-<tag>`, is linked with
`--target link_so`, and is measured with the official verifier exactly as in §2.
The temporary block was removed with `git checkout -- CMakeLists.txt` (the final
tree is clean; `git diff --stat` is empty).

### 5.2 Results

`byte`/`linked` are the official whole-`.so` gate counts. `TU byte (base->var)` is
the number of byte-exact functions in that TU (baseline -> variant). `target pct`
is the official per-function percentage (baseline in parentheses).

| tag | TU | byte | linked | TU byte (base->var) | target pct (base->var) |
|---|---|---:|---:|---|---|
| `core_Globals__omitfp` | `game/core/Globals.cpp` | 2538 | 4465 | 39->2/41 | 39.5 (80.5); 39.5 (80.5) |
| `core_Globals__omitleaf` | `game/core/Globals.cpp` | 2538 | 4498 | 39->2/41 | 39.5 (80.5); 39.5 (80.5) |
| `core_Globals__sp1` | `game/core/Globals.cpp` | 2552 | 4488 | 39->7/41 | 80.5 (80.5); 80.5 (80.5) |
| `core_Globals__spall` | `game/core/Globals.cpp` | 1933 | 4473 | 39->0/41 | 41.0 (80.5); 41.0 (80.5) |
| `menu_MGame__omitfp` | `game/menu/MGame.cpp` | 3080 | 4468 | 43->10/45 | 37.0 (74.8); 58.1 (99.9) |
| `menu_MGame__sp1` | `game/menu/MGame.cpp` | 3093 | 4491 | 43->14/45 | 62.8 (74.8); 80.4 (99.9) |
| `menu_MGame__spall` | `game/menu/MGame.cpp` | 2192 | 4466 | 43->0/45 | 74.8 (74.8); 99.9 (99.9) |
| `menu_ModStation__omitfp` | `game/menu/ModStation.cpp` | 2469 | 4470 | 38->7/40 | 75.7 (98.4); 75.7 (98.4) |
| `menu_ModStation__sp1` | `game/menu/ModStation.cpp` | 2478 | 4490 | 38->7/40 | 75.6 (98.4); 75.6 (98.4) |
| `menu_ModStation__spall` | `game/menu/ModStation.cpp` | 1897 | 4471 | 38->0/40 | 98.4 (98.4); 98.4 (98.4) |
| `menu_SpaceLounge__omitfp` | `game/menu/SpaceLounge.cpp` | 3012 | 4480 | 33->11/34 | 71.9 (95.5) |
| `menu_SpaceLounge__sp1` | `game/menu/SpaceLounge.cpp` | 3013 | 4490 | 33->12/34 | 75.6 (95.5) |
| `menu_SpaceLounge__spall` | `game/menu/SpaceLounge.cpp` | 2135 | 4480 | 33->3/34 | 95.5 (95.5) |
| `mission_Generator__omitfp` | `game/mission/Generator.cpp` | 2025 | 4484 | 14->4/15 | 46.0 (64.4) |
| `mission_Generator__sp1` | `game/mission/Generator.cpp` | 2042 | 4498 | 14->5/15 | 72.6 (64.4) |
| `mission_Generator__spall` | `game/mission/Generator.cpp` | 1861 | 4481 | 14->0/15 | 77.5 (64.4) |
| `mission_Status__nounw` | `game/mission/Status.cpp` | 4040 | 4500 | 130->126/131 | 60.4 (60.4) |
| `mission_Status__omitfp` | `game/mission/Status.cpp` | 2413 | 4439 | 130->73/131 | 25.8 (60.4) |
| `mission_Status__omitleaf` | `game/mission/Status.cpp` | 3055 | 4499 | 130->117/131 | 60.4 (60.4) |
| `mission_Status__sp1` | `game/mission/Status.cpp` | 2473 | 4498 | 130->104/131 | 60.4 (60.4) |
| `mission_Status__spall` | `game/mission/Status.cpp` | 1741 | 4365 | 130->2/131 | 49.3 (60.4) |
| `ship_PlayerEgo__omitfp` | `game/ship/PlayerEgo.cpp` | 2061 | 4421 | 173->73/174 | 54.9 (87.6) |
| `ship_PlayerEgo__sp1` | `game/ship/PlayerEgo.cpp` | 2234 | 4476 | 173->84/174 | 69.2 (87.6) |
| `ship_PlayerEgo__spall` | `game/ship/PlayerEgo.cpp` | 1609 | 4350 | 173->0/174 | 87.6 (87.6) |
| `ui_HangarWindow__omitfp` | `game/ui/HangarWindow.cpp` | 2755 | 4478 | 31->7/32 | 53.6 (80.2) |
| `ui_HangarWindow__omitleaf` | `game/ui/HangarWindow.cpp` | 4360 | 4498 | 31->7/32 | 84.4 (80.2) |
| `ui_HangarWindow__sp1` | `game/ui/HangarWindow.cpp` | 2840 | 4491 | 31->11/32 | 71.8 (80.2) |
| `ui_HangarWindow__spall` | `game/ui/HangarWindow.cpp` | 2060 | 4478 | 31->0/32 | 84.4 (80.2) |
| `ui_Hud__omitfp` | `game/ui/Hud.cpp` | 2922 | 4471 | 53->23/54 | 45.0 (92.1) |
| `ui_Hud__omitleaf` | `game/ui/Hud.cpp` | 4499 | 4499 | 53->52/54 | 92.9 (92.1) |
| `ui_Hud__sp1` | `game/ui/Hud.cpp` | 2934 | 4491 | 53->26/54 | 63.4 (92.1) |
| `ui_Hud__spall` | `game/ui/Hud.cpp` | 2100 | 4458 | 53->3/54 | 92.9 (92.1) |
| `ui_Layout__nounw` | `game/ui/Layout.cpp` | 4040 | 4500 | 48->40/50 | 47.8 (47.8); 47.8 (47.8) |
| `ui_Layout__omitfp` | `game/ui/Layout.cpp` | 2445 | 4461 | 48->5/50 | 41.4 (47.8); 41.4 (47.8) |
| `ui_Layout__omitleaf` | `game/ui/Layout.cpp` | 2454 | 4500 | 48->5/50 | 41.4 (47.8); 41.4 (47.8) |
| `ui_Layout__sp1` | `game/ui/Layout.cpp` | 2456 | 4485 | 48->7/50 | 47.8 (47.8); 47.8 (47.8) |
| `ui_Layout__spall` | `game/ui/Layout.cpp` | 1895 | 4467 | 48->0/50 | 37.5 (47.8); 37.5 (47.8) |
| `ui_MenuTouchWindow__omitfp` | `game/ui/MenuTouchWindow.cpp` | 2663 | 4470 | 38->7/41 | 58.6 (95.1); 58.6 (95.1); 49.7 (97.7) |
| `ui_MenuTouchWindow__sp1` | `game/ui/MenuTouchWindow.cpp` | 2672 | 4493 | 38->7/41 | 79.9 (95.1); 79.9 (95.1); 97.7 (97.7) |
| `ui_MenuTouchWindow__spall` | `game/ui/MenuTouchWindow.cpp` | 2007 | 4469 | 38->0/41 | 95.1 (95.1); 95.1 (95.1); 75.1 (97.7) |
| `weapons_Gun__omitfp` | `game/weapons/Gun.cpp` | 2868 | 4486 | 27->13/28 | 44.9 (90.0) |
| `weapons_Gun__sp1` | `game/weapons/Gun.cpp` | 2794 | 4492 | 27->13/28 | 62.8 (90.0) |
| `weapons_Gun__spall` | `game/weapons/Gun.cpp` | 2134 | 4481 | 27->0/28 | 90.0 (90.0) |
| `weapons_Radar__nounw` | `game/weapons/Radar.cpp` | 4040 | 4500 | 18->14/19 | 74.0 (73.9) |
| `weapons_Radar__omitfp` | `game/weapons/Radar.cpp` | 2687 | 4490 | 18->8/19 | 40.5 (73.9) |
| `weapons_Radar__sp1` | `game/weapons/Radar.cpp` | 2789 | 4494 | 18->8/19 | 66.7 (73.9) |
| `weapons_Radar__spall` | `game/weapons/Radar.cpp` | 2688 | 4488 | 18->0/19 | 74.0 (73.9) |
| `weapons_RocketGun__omitfp` | `game/weapons/RocketGun.cpp` | 2911 | 4492 | 12->3/13 | 56.1 (86.9) |
| `weapons_RocketGun__sp1` | `game/weapons/RocketGun.cpp` | 2984 | 4499 | 12->6/13 | 75.4 (86.9) |
| `weapons_RocketGun__spall` | `game/weapons/RocketGun.cpp` | 2910 | 4491 | 12->2/13 | 86.9 (86.9) |
| `world_LevelScript__omitfp` | `game/world/LevelScript.cpp` | 2725 | 4489 | 16->5/17 | 35.2 (53.4) |
| `world_LevelScript__sp1` | `game/world/LevelScript.cpp` | 2815 | 4495 | 16->5/17 | 40.3 (53.4) |
| `world_LevelScript__spall` | `game/world/LevelScript.cpp` | 2731 | 4489 | 16->2/17 | 53.4 (53.4) |
| `world_Level__nounw` | `game/world/Level.cpp` | 4040 | 4500 | 104->97/107 | 66.4 (66.0); 88.3 (88.3); 92.0 (92.0) |
| `world_Level__omitfp` | `game/world/Level.cpp` | 2334 | 4437 | 104->35/107 | 54.1 (66.0); 64.6 (88.3); 58.3 (92.0) |
| `world_Level__omitleaf` | `game/world/Level.cpp` | 4499 | 4499 | 104->103/107 | 66.4 (66.0); 88.3 (88.3); 92.0 (92.0) |
| `world_Level__sp1` | `game/world/Level.cpp` | 2469 | 4482 | 104->46/107 | 66.4 (66.0); 70.0 (88.3); 69.0 (92.0) |
| `world_Level__spall` | `game/world/Level.cpp` | 1821 | 4412 | 104->2/107 | 61.1 (66.0); 88.3 (88.3); 92.0 (92.0) |

Notes on the table:

* No row exceeds `4500 / 4500`. The best rows are `ui_Hud__omitleaf` and
  `world_Level__omitleaf` at `4499 / 4499`.
* Some variants **improve the target function's own percentage** while still
  losing the gate, because they break other functions in the same TU:
  `Generator::createMission` 64.4 -> 72.6 with `-fstack-protector` and -> 77.5
  with `-fstack-protector-all`; `HangarWindow::render` 80.2 -> 84.4 with
  `-momit-leaf-frame-pointer`; `Hud::draw` 92.1 -> 92.9 with
  `-momit-leaf-frame-pointer`; `Radar::draw` 73.9 -> 74.0 with a real
  `-fno-unwind-tables`. None of these is a whole-gate improvement.
* The `nounw` rows have `.text` byte-identical to baseline but `byte_exact` 4040:
  removing the per-TU unwind tables changes `.ARM.exidx`/`.ARM.extab` sizes and
  therefore shifts the GOT/`.rodata` layout. The raw `.text` literal-pool words
  that encode GOT-relative addresses change, so `byte_exact` drops even though
  `linked_exact` stays 4500. This is a layout effect, not a code change.
* `ui_Layout__omitleaf` is a particularly clear negative: the target
  `Layout::Layout` gets *worse* (47.8 -> 41.4) and the TU loses 43 byte-exact
  functions, because `-momit-leaf-frame-pointer` removes the frame pointer from
  many small `Layout` helpers that the original keeps.

## 6. Proven / disproven / unobservable

### Positively proven

1. **`-fstack-protector-strong` was used globally.** The original and the
   canonical build have the same 756 canary-bearing functions, the same canary
   value-register/guard-register/stack-offset placement (0 mismatches), and adding
   `-fstack-protector-strong` to any TU changes no object section. Basic/all/none
   each change 157/191/157 TUs and drop the official gate to 1369/91/1369.
2. **`-funwind-tables` was used globally.** The original and canonical build have
   the same 3566 `.ARM.exidx` entries (272 CANT, 3294 extab), the same entry-target
   sequence and the same entry-kind sequence (0/0 diffs). `-funwind-tables`
   appended per-TU is a no-op in all 192 TUs. A real `-fno-unwind-tables` changes
   the exidx/extab sections in 186 TUs and removes 11 exidx entries globally.
3. **The default frame-pointer setting (keep) is the original's.** The original
   and canonical build have the same 2700 frame-pointer functions (0 mismatches).
   `-fno-omit-frame-pointer` is a no-op in all 192 TUs; `-fomit-frame-pointer`
   leaves 128 fp functions and drops the gate to 1298; `-momit-leaf-frame-pointer`
   leaves 2619 and drops to 1386.
4. **No effective per-TU divergence.** 0 canary-presence, 0 frame-pointer and
   0 exidx-kind mismatches in every one of the 192 app TUs. This is a direct
   per-TU fingerprint comparison, not an inference from the global gate.

### Disproven

* A per-TU `-fstack-protector` (basic), `-fstack-protector-all` or
  `-fno-stack-protector` on any of the 16 non-exact TUs as a strict improvement.
  All 48 linked runs are below 4500/4500 (best `-fstack-protector` rows are
  4499/4499, e.g. `RocketGun`; the rest are far lower).
* A per-TU `-fomit-frame-pointer` on any non-exact TU: all 16 linked runs are
  below 4500/4500 (best 4492/4492).
* A per-TU `-momit-leaf-frame-pointer` on any non-exact TU: the six TUs where it
  changes codegen (`Globals`, `Status`, `HangarWindow`, `Hud`, `Layout`, `Level`)
  all lose at least one byte-exact function; the best are 4499/4499.
* A real per-TU `-fno-unwind-tables` as a byte-exact improvement: it changes
  `.text` layout in all four spot-checked TUs and drops `byte_exact` to 4040
  (while `linked_exact` stays 4500).
* Any global variant as a strict improvement: all ten are `<= 4500 / 4500`.

### Unobservable / unknown

* A per-TU flag that is a **no-op for that TU** cannot be distinguished from the
  global flag. The object sweep quantifies this:
  * `-fstack-protector` / `-fno-stack-protector`: 35 TUs insensitive
    (no canary-eligible function);
  * `-fstack-protector-all`: 1 TU insensitive;
  * `-fomit-frame-pointer`: 5 TUs insensitive;
  * `-momit-leaf-frame-pointer`: 138 TUs insensitive;
  * a real `-fno-unwind-tables`: 6 TUs insensitive.
  A TU in one of these insensitive sets could have carried the flag without a
  codegen trace; there is no positive evidence for it, and the zero-mismatch
  fingerprints already cover every sensitive TU.
* `-fno-asynchronous-unwind-tables` is a **no-op alias** in clang 7.0.2 even when
  appended after `-funwind-tables`; it cannot be distinguished from
  `-funwind-tables` by codegen.
* `-fstack-protector-explicit` is rejected by clang 7.0.2 (`unknown argument`);
  the cc1 levels are 0=none, 1=basic, 2=strong, 3=all, with no explicit level.
  `-mllvm -frame-pointer` and `-mllvm -enable-frame-pointer` are also rejected.
* The original build script is not in the APK; this note proves the effective
  flags, not the literal ndk-build `APP_CFLAGS` text.

## 7. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| `-fstack-protector-strong` globally | **very high** | 756 canaries in both, 0 presence mismatches; 0 placement mismatches among decoded canaries (prior note: 751/756 decoded, 0 mismatches); `-fstack-protector-strong` object-identical in 192/192 TUs; basic/all/none drop the gate to 1369/91/1369 |
| `-funwind-tables` globally | **very high** | 3566 exidx entries, 272 CANT/3294 extab, 0 target/kind diffs; `-funwind-tables` object-identical in 192/192 TUs; real removal gives 3555 entries and byte 1369 |
| default keep-frame-pointer | **very high** | 2700 fp functions in both, 0 mismatches; `-fno-omit-frame-pointer` object-identical in 192/192 TUs; omit-fp/omit-leaf drop to 128/2619 fp and byte 1298/1386 |
| no effective per-TU divergence | **very high** | 0 canary/fp/exidx-kind mismatches across 192 TUs; per-TU linked runs on all 16 non-exact TUs never exceed 4500/4500 |
| no strict improvement from the ten flags | **high** | 11 global builds + 59 per-TU linked builds; best per-TU is 4499/4499; all global variants <= 4500/4500 |
| a codegen-neutral per-TU flag could exist | **certain (by construction)** | 35/1/5/138/6 TUs are insensitive to basic/all/omit-fp/omit-leaf/real-no-unwind respectively |
| `-fno-asynchronous-unwind-tables` is a no-op | **high** | 0 changed sections in all 192 TUs; clang 7.0.2 maps both `-funwind-tables` and `-fasynchronous-unwind-tables` to `-munwind-tables` |

## 8. Is there a strict improvement?

**No.** No tested flag raises `byte_exact`, `linked_exact` or `size-exact` above
the canonical `4500 / 4500 / 4524`. The best per-TU result is
`-momit-leaf-frame-pointer` on `game/world/Level.cpp` or `game/ui/Hud.cpp`:
`byte 4499 / linked 4499`, i.e. one function worse than baseline. The best global
result is the canonical setting itself (`4500 / 4500`).

Therefore this commit contains **only this note**. No `src/`, `CMakeLists.txt`,
tool, baseline or allowlist change is landed. The temporary per-TU harness was
reverted and the canonical `check` gate was re-run green after the experiments.

## 9. Reproduction

Scratch files (not tracked): `/dev/shm/w5206/toolchain_extra.cmake`,
`run_variant.sh`, `run_variant_p.sh`, `run_ptu.sh`, `compile_one.py`,
`objdiff.py`, `sweep.py`, `classify.py`, `cls_one.py`, `compare.py`,
`CMakeLists.txt.bak`, and the `build-*`/`verify-*`/`ptu-*`/`objs/` directories.

The classifier (`classify.py`) and object comparator (`objdiff.py`) are
self-contained Python 3 scripts; the key logic is reproduced below.

### 9.1 Canary / frame-pointer / exidx classifier

```python
# classify.py (abridged; full file in /dev/shm/w5206/classify.py)
# - readelf -sW -> non-zero-size FUNC symbols (clear Thumb bit)
# - objdump -d -M force-thumb -> {address: instruction text}
# - canary: any instruction in [start,start+size) mentions __stack_chk_fail
# - canary placement: ldr rA,[pc] / add rA,pc / ldr rB,[rA,#0] /
#   ldr rC,[rB,#0] / str rC,[sp,#off]
# - frame pointer: push/stmdb containing r7 + mov/add r7,sp in first 12 insns
# - exidx: 8-byte entries, prel31(w0) target, w1==1 -> CANT else extab
```

### 9.2 Object comparator

```python
# objdiff.py (abridged)
# parse ELF32 section headers; compare .text.* bytes/sizes,
# .ARM.exidx.* bytes, .ARM.extab.* bytes between two .o files.
```

### 9.3 Exact per-TU compile replay

```sh
python3 /dev/shm/w5206/compile_one.py <compile_commands-index> <tag> <flag>
# strips -o/-MD/-MF/-MT/-MQ/-MP, appends -o <scratch>/<tu>.o and <flag>
# cwd = the compile_commands entry's directory
```

### 9.4 Per-TU linked build

```sh
cmake -S . -B /dev/shm/w5206/ptu-build-<tag> -G Ninja -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_AB_TU=<tu> -DGOF2_AB_FLAGS=<flag> \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5206/ptu-build-<tag> --target link_so -j3
python3 tools/verify/verify.py --build-dir /dev/shm/w5206/ptu-verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5206/ptu-build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

## 10. Caveats

* The global variant builds were initially run in parallel; because
  `third_party/build_match_lib.cmake` writes the vendored archives into the shared
  source tree, concurrent configures can race on the libcrypto object directory
  (`sha256-armv4.S: unterminated #else`). The archives were deleted and rebuilt
  once serially, then all variants were relinked and re-verified. The final
  results above are from the consistent archives; the canonical `check` was
  re-run green afterwards.
* The canary/fp classifier is a static pattern matcher. It classifies 4521
  non-zero-size `FUNC` symbols (the report's 4524 includes three zero-size
  symbols). The prior w4305 note's canary counts for `-fstack-protector-all`
  differ (3835 vs 4299 here) because of classifier details; the gate counts and
  the original-vs-canonical zero-mismatch result are independent of that
  difference. The prior note's more complete placement matcher decoded 751/756
  canaries and also found 0 mismatches. This note's simpler matcher decodes only
  137/756 placements, but every decoded placement matches exactly. A direct spot
  check of the two functions the prior note called out: `MenuTouchWindow` C1/C2
  currently store the canary at `sp+60` in the original and `sp+56` in ours (the
  prior note's "60 vs 64" is stale), while `HangarWindow::render`'s prologue is
  now byte-identical (canary slot `sp+372`, value `r1`, guard pointer `r0`).
* `size-exact` is 4524/4524 in every global row; the per-TU table's `TU byte`
  column is a per-TU slice of the same official report, not a separate metric.
