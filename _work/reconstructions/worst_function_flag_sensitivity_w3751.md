# Worst-function flag sensitivity (w3751)

**Status:** research note, 2026-11-03. Worktree `wt/w3751`, branch `wave87-w3751`,
base `12b42664`. No source or build-flag change landed: no flag is a strict
improvement at this tree state. This commit contains only this note.

## TL;DR

* Baseline: **byte_exact 4498/4524, linked_exact 4498/4524, size-exact
  4524/4524, missing 0**, `.text 0x1337c8`, `.rodata 0x41b67`, lint `CLEAN ✓`,
  drift 0.
* **81-variant global matrix** (the task's list plus the base): **39
  gate-neutral** (including the base), **31 worse**, **11 build/link
  failures**. Two extra variants (`-mnocrc`, the correct clang 7 spelling of
  `-mno-crc`, and `-fno-inline-functions-called-once`) are also gate-neutral.
  No variant beats 4498/4498/4524, and **no variant raises the fuzzy of any of
  the 26 targets** (max Δ = +0.0).
* **277 per-TU cells** measured: no target improves; the best per-TU Δ is 0.0;
  every flag that changes a target lowers it.
* **All 26 remaining functions are size-exact** and have the same byte length
  in both binaries. Their diffs are local register-allocation,
  instruction-scheduling, 16/32-bit encoding and basic-block-order differences.
  They are **source-shape-driven, not flag-driven**.
* **No missing original flag can be identified.** The current global flag set
  already reproduces the original's codegen; every non-neutral alternative is
  worse. The remaining gap needs source/TU-shape work.

## 1. Baseline

Commands:

```sh
cmake --build cmake-build-match --target check -j8
```

Output:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section sizes (ours vs original):

```text
.text   0x1337c8 @ 0x69440     (identical)
.rodata 0x41b67  @ 0x1c9e10    (identical)
.data   0x296c   @ 0x214000    (identical)
.bss    0x69ac   @ 0x216970    (identical)
```

`size-exact` from `report.json` is **4524/4524**. The 26 non-exact functions are
listed in §4.

## 2. Method

### 2.1 Global A/B

Scratch wrapper `/dev/shm/w3751/toolchain_extra.cmake` includes the repo
toolchain and appends `GOF2_EXTRA_FLAGS` to both `CMAKE_CXX_FLAGS` and
`CMAKE_C_FLAGS`:

```cmake
include("/home/debian/GalaxyOnFire2/wt/w3751/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM C compile flags" FORCE)
endif()
```

Each variant:

```sh
cmake -S . -B /dev/shm/w3751/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w3751/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flag>'
cmake --build /dev/shm/w3751/build --target link_so -j64
python3 /dev/shm/w3751/verify_screen.py \
  --build-dir /dev/shm/w3751/verify/<id> \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w3751/build/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

`verify_screen.py` is a copy of `tools/verify/verify.py` that uses
`autojunk=True` for sequences >2000 instructions as a screening shortcut. It
does not change `byte_exact`, `linked_exact`, `size-exact`, `missing`, `extra`
or `wrong_type`. The per-target fuzzy values in this note come from
`score_targets.py`, which calls the official `verify.score()`
(`autojunk=False`) and is therefore directly comparable to the official
verifier's fuzzy column.

### 2.2 Per-TU A/B

The first per-TU wrapper used `COMPILE_OPTIONS`:

```cmake
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/${GOF2_AB_TU}"
    PROPERTIES COMPILE_OPTIONS "${GOF2_AB_FLAGS}")
```

This is **wrong for `Radar.cpp`, `SpaceLounge.cpp` and `StarMap.cpp`**:
`CMakeLists.txt` later sets `COMPILE_OPTIONS` on those three files for their
scoped `-mllvm` knobs, replacing the test flag. The first matrix therefore
reported those 24 cells as neutral. The corrected wrapper uses `COMPILE_FLAGS`,
which is independent of `COMPILE_OPTIONS` and is appended after it:

```cmake
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/${GOF2_AB_TU}"
    PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
```

Each per-TU variant:

```sh
cmake -S . -B /dev/shm/w3751/build_pt2 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w3751/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS= \
  -DCMAKE_PROJECT_INCLUDE=/dev/shm/w3751/per_tu_flags.cmake \
  -DGOF2_AB_TU='<tu>' -DGOF2_AB_FLAGS='<flag>'
cmake --build /dev/shm/w3751/build_pt2 --target link_so -j64
python3 /dev/shm/w3751/score_targets.py \
  /dev/shm/w3751/build_pt2/libgof2hdaa.so /tmp/score.json
```

The corrected cells for the three scoped TUs are in §5; they match the global
effect exactly, confirming the flag is now applied.

## 3. Global matrix

**81 variants: 39 gate-neutral (including the base), 31 worse, 11 build/link
failures. Two extra variants (`-mnocrc`, `-fno-inline-functions-called-once`)
are also gate-neutral. No variant beats 4498/4498/4524. No variant raises any
of the 26 target fuzzy values (max Δ = +0.0).**

### 3.1 Gate-neutral flags

These leave `byte_exact`, `linked_exact`, `size-exact`, `missing` and the 26
target fuzzy values exactly at baseline. They are useful negatives: the
original's codegen is consistent with clang 7.0.2's default `-Oz` pipeline.

```text
(base)
-faligned-allocation
-ffp-contract=fast
-ffp-contract=off
-fmerge-all-constants
-fno-access-control
-fno-aligned-allocation
-fno-associative-math
-fno-assume-sane-operator-new
-fno-data-sections
-fno-delayed-template-parsing
-fno-delete-null-pointer-checks
-fno-finite-math-only
-fno-honor-infinities
-fno-merge-constants
-fno-omit-frame-pointer
-fno-operator-names
-fno-plt
-fno-rounding-math
-fno-rtti-data
-fno-signed-char
-fno-sized-deallocation
-fno-stack-size-section
-fno-trapping-math
-fno-unsafe-math-optimizations
-fno-vectorize
-fshort-wchar
-fstack-size-section
-mno-dsp
-mno-execute-only
-mno-madd4
-mno-movt
-mno-neg-immediates
-mno-restrict-it
-mno-stack-arg-probe
-mrelax-all
-mstack-alignment=16
-mstack-alignment=4
-mstack-alignment=8
```

Two extra variants tested outside the 81-row table are also gate-neutral:
`-mnocrc` (the correct clang 7 spelling of the task's `-mno-crc`) and
`-fno-inline-functions-called-once`. Both give 4498/4498/4524/4524 with no
target moved.

### 3.2 Worse flags

`targets moved` is the number of the 26 targets whose fuzzy changed; `min Δ`
and `max Δ` are over those targets. A positive `max Δ` would be an improvement;
there is none.

| flag | byte | linked | size | missing | targets moved | min Δ | max Δ |
|---|---:|---:|---:|---:|---:|---:|---:|
| `-fvisibility=hidden` | 57 | 243 | 244 | 4280 | 0 | +0.0 | +0.0 |
| `-fstack-protector-all` | 91 | 962 | 981 | 0 | 7 | -39.5 | +0.0 |
| `-fno-inline` | 1219 | 3511 | 3571 | 0 | 21 | -57.3 | +0.0 |
| `-fomit-frame-pointer` | 1298 | 1921 | 2163 | 0 | 26 | -58.9 | -6.7 |
| `-fno-optimize-sibling-calls` | 1316 | 3587 | 3686 | 0 | 3 | -17.0 | +0.0 |
| `-ffast-math` | 1319 | 4225 | 4369 | 0 | 15 | -19.3 | +0.0 |
| `-mlong-calls` | 1345 | 1664 | 1673 | 0 | 22 | -56.6 | +0.0 |
| `-fno-builtin` | 1350 | 3986 | 4121 | 0 | 7 | -34.2 | +0.0 |
| `-fno-math-errno` | 1356 | 4487 | 4514 | 0 | 0 | +0.0 | +0.0 |
| `-fno-use-cxa-atexit` | 1356 | 4497 | 4523 | 0 | 0 | +0.0 | +0.0 |
| `-mno-implicit-float` | 1357 | 4263 | 4286 | 0 | 15 | -38.6 | +0.0 |
| `-fno-jump-tables` | 1363 | 4410 | 4437 | 0 | 10 | -67.7 | +0.0 |
| `-fno-threadsafe-statics` | 1368 | 4494 | 4520 | 3 | 0 | +0.0 | +0.0 |
| `-fno-stack-protector` | 1369 | 3781 | 3789 | 0 | 19 | -37.8 | +0.0 |
| `-fstack-protector` | 1369 | 3785 | 3793 | 0 | 19 | -37.8 | +0.0 |
| `-fno-strict-aliasing` | 1371 | 4349 | 4411 | 0 | 12 | -32.5 | +0.0 |
| `-fwritable-strings` | 1373 | 4489 | 4515 | 0 | 1 | -2.6 | +0.0 |
| `-mno-global-merge` | 1397 | 4488 | 4517 | 0 | 0 | +0.0 | +0.0 |
| `-fno-strict-overflow` | 1398 | 4462 | 4500 | 0 | 3 | -8.3 | +0.0 |
| `-fwrapv` | 1398 | 4462 | 4500 | 0 | 3 | -8.3 | +0.0 |
| `-mno-unaligned-access` | 1404 | 4443 | 4471 | 0 | 6 | -23.3 | +0.0 |
| `-fno-honor-nans` | 1429 | 4373 | 4480 | 0 | 8 | -10.8 | +0.0 |
| `-fno-slp-vectorize` | 1484 | 4384 | 4408 | 0 | 11 | -38.6 | +0.0 |
| `-fno-zero-initialized-in-bss` | 1522 | 4488 | 4516 | 0 | 0 | +0.0 | +0.0 |
| `-freciprocal-math` | 1539 | 4402 | 4483 | 0 | 9 | -5.8 | +0.0 |
| `-fno-rtti` | 1725 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 |
| `-mthread-model single` | 2085 | 4495 | 4523 | 0 | 2 | -0.2 | +0.0 |
| `-fno-signed-zeros` | 2355 | 4489 | 4521 | 0 | 1 | -8.5 | +0.0 |
| `-fshort-enums` | 2869 | 4476 | 4511 | 0 | 0 | +0.0 | +0.0 |
| `-fno-unroll-loops` | 2892 | 4491 | 4520 | 0 | 1 | -14.4 | +0.0 |
| `-fsigned-char` | 4440 | 4494 | 4522 | 0 | 0 | +0.0 | +0.0 |

`-fvisibility=hidden` is not comparable for the 26 because the target symbols
become hidden; it is still unambiguously worse (57/243, 4280 missing).

### 3.3 Build/link failures

| flag | failure |
|---|---|
| `-fno-common` | duplicate `g_android_back_button_pressed` (`o005_GOF2HD2012.c.obj` vs `o000_BackButton.c.obj`) |
| `-fno-exceptions` | `Level.cpp:5217: cannot use 'try' with exceptions disabled` |
| `-fno-semantic-interposition` | clang 7.0.2: unknown argument |
| `-mhard-float` | VFP register arguments vs softfp prebuilt libs |
| `-msoft-float` | link/ABI failure |
| `-mno-crc` | clang 7.0.2: unknown argument (correct spelling `-mnocrc` is neutral) |
| `-mno-fancy-math-387`, `-mno-fp-ret-in-387` | x86-only, unknown on ARM |
| `-mthumb-interwork`, `-mno-thumb-interwork`, `-mno-neon-for-64bits` | clang 7.0.2: unknown argument |

None of these is a plausible original flag: the original is armeabi-v7a
**softfp** (`readelf -h`: `soft-float ABI`), uses exceptions and RTTI, and
clang 7.0.2 defaults to `-fcommon`. The `-fno-common` failure is a source-shape
issue (a duplicate tentative definition), not evidence that the original used
`-fno-common`.

## 4. Per-target sensitivity

The 26 targets are all size-exact and all have identical byte length in both
binaries. The table below lists the flags that move each target (leading `-`
omitted). Every Δ is zero or negative; the `max Δ` column is +0.0 for every
target.

| target | bytes | base % | flags that move it | min Δ | max Δ |
|---|---:|---:|---|---:|---:|
| `_ZN6LayoutC1Ev` | 7072 | 47.7 | `fno-slp-vectorize`, `mno-implicit-float`, `fstack-protector-all`, `fomit-frame-pointer` | -31.6 | +0.0 |
| `_ZN6LayoutC2Ev` | 7072 | 47.7 | `fno-slp-vectorize`, `mno-implicit-float`, `fstack-protector-all`, `fomit-frame-pointer` | -31.6 | +0.0 |
| `_ZN11LevelScript7processEi` | 57232 | 55.0 | `mlong-calls`, `fno-inline`, `fomit-frame-pointer`, `fno-jump-tables`, `fno-stack-protector`, `fstack-protector`, `fno-strict-aliasing`, `ffast-math`, `mno-implicit-float`, `fno-honor-nans`, `freciprocal-math`, `fno-slp-vectorize` | -34.1 | +0.0 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 73.6 | `mlong-calls`, `fno-inline`, `fno-strict-aliasing`, `fomit-frame-pointer`, `fno-stack-protector`, `fstack-protector`, `fno-jump-tables`, `ffast-math`, `freciprocal-math` | -39.0 | +0.0 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 73.9 | `mlong-calls`, `fomit-frame-pointer`, `fno-inline`, `fno-stack-protector`, `fstack-protector`, `fno-strict-aliasing`, `fwrapv`, `fno-strict-overflow`, `ffast-math`, `freciprocal-math` | -48.2 | +0.0 |
| `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 74.0 | `fno-inline`, `fomit-frame-pointer`, `mlong-calls`, `fno-stack-protector`, `fstack-protector`, `mno-implicit-float`, `ffast-math`, `fno-honor-nans` | -25.2 | +0.0 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 74.1 | `mlong-calls`, `fno-jump-tables`, `fomit-frame-pointer`, `fno-inline`, `fstack-protector-all` | -40.5 | +0.0 |
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 78.1 | `mlong-calls`, `fomit-frame-pointer`, `fno-inline`, `fno-stack-protector`, `fstack-protector`, `fno-strict-aliasing`, `fno-strict-overflow`, `fwrapv`, `fno-optimize-sibling-calls`, `fno-builtin` | -51.3 | +0.0 |
| `_ZN7GlobalsC1Ev` | 612 | 80.5 | `fomit-frame-pointer`, `fstack-protector-all`, `fno-slp-vectorize`, `mno-implicit-float`, `fno-strict-aliasing`, `mno-unaligned-access`, `ffast-math`, `fno-honor-nans` | -41.0 | +0.0 |
| `_ZN7GlobalsC2Ev` | 612 | 80.5 | `fomit-frame-pointer`, `fstack-protector-all`, `fno-slp-vectorize`, `mno-implicit-float`, `fno-strict-aliasing`, `mno-unaligned-access`, `ffast-math`, `fno-honor-nans` | -41.0 | +0.0 |
| `_ZN12HangarWindow6renderEv` | 9700 | 80.6 | `fno-inline`, `mlong-calls`, `fomit-frame-pointer`, `fno-stack-protector`, `fstack-protector`, `fno-jump-tables`, `fwritable-strings` | -33.2 | +0.0 |
| `_ZN6Status16missionCompletedEbbx` | 1432 | 86.7 | `fno-inline`, `mlong-calls`, `fomit-frame-pointer`, `fno-jump-tables`, `fstack-protector-all` | -57.3 | +0.0 |
| `_ZN9RocketGun6updateEi` | 1908 | 86.9 | `fno-inline`, `mlong-calls`, `fomit-frame-pointer`, `fno-strict-aliasing`, `fno-stack-protector`, `fstack-protector`, `ffast-math`, `freciprocal-math`, `mno-implicit-float` | -50.7 | +0.0 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 87.3 | `mlong-calls`, `fomit-frame-pointer`, `fno-stack-protector`, `fstack-protector`, `fno-inline`, `mno-implicit-float` | -44.0 | +0.0 |
| `_ZN5Level11createSceneEv` | 2252 | 88.3 | `mlong-calls`, `fno-inline`, `fomit-frame-pointer`, `fno-jump-tables`, `fno-stack-protector`, `fstack-protector`, `fno-optimize-sibling-calls`, `fno-builtin`, `fno-unroll-loops`, `ffast-math`, `freciprocal-math` | -55.3 | +0.0 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 88.8 | `mlong-calls`, `fomit-frame-pointer`, `fno-stack-protector`, `fstack-protector`, `fno-optimize-sibling-calls`, `ffast-math`, `fno-honor-nans`, `freciprocal-math`, `mno-unaligned-access`, `fno-jump-tables` | -44.3 | +0.0 |
| `_ZN5Level13createMissionEv` | 17172 | 90.7 | `fno-inline`, `mlong-calls`, `fomit-frame-pointer`, `fno-jump-tables`, `fno-strict-aliasing`, `mno-implicit-float`, `fno-stack-protector`, `fstack-protector`, `ffast-math`, `fno-builtin`, `freciprocal-math`, `fno-signed-zeros`, `fno-slp-vectorize`, `mno-unaligned-access` | -50.7 | +0.0 |
| `_ZN15MenuTouchWindowC1Ei` | 9600 | 93.9 | `mlong-calls`, `fomit-frame-pointer`, `fno-inline`, `fno-stack-protector`, `fstack-protector`, `fno-strict-aliasing`, `fno-slp-vectorize`, `mno-implicit-float`, `mno-unaligned-access`, `fno-builtin`, `ffast-math` | -51.6 | +0.0 |
| `_ZN15MenuTouchWindowC2Ei` | 9600 | 93.9 | `mlong-calls`, `fomit-frame-pointer`, `fno-inline`, `fno-stack-protector`, `fstack-protector`, `fno-strict-aliasing`, `fno-slp-vectorize`, `mno-implicit-float`, `mno-unaligned-access`, `fno-builtin`, `ffast-math` | -51.6 | +0.0 |
| `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | 8132 | 95.2 | `fno-jump-tables`, `fomit-frame-pointer`, `mlong-calls`, `fno-inline`, `fno-stack-protector`, `fstack-protector`, `fno-strict-overflow`, `fwrapv`, `ffast-math`, `fno-honor-nans` | -67.7 | +0.0 |
| `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 95.6 | `mlong-calls`, `fno-inline`, `fomit-frame-pointer`, `fno-stack-protector`, `fstack-protector` | -45.2 | +0.0 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 97.7 | `fno-inline`, `fomit-frame-pointer`, `mlong-calls`, `fno-jump-tables`, `fstack-protector-all`, `fno-strict-aliasing` | -49.2 | +0.0 |
| `_ZN10ModStationC1Ev` | 1008 | 98.6 | `fno-inline`, `mlong-calls`, `mno-implicit-float`, `fno-builtin`, `fno-slp-vectorize`, `fno-stack-protector`, `fstack-protector`, `fomit-frame-pointer`, `mthread-model single` | -53.2 | +0.0 |
| `_ZN10ModStationC2Ev` | 1008 | 98.6 | `fno-inline`, `mlong-calls`, `mno-implicit-float`, `fno-builtin`, `fno-slp-vectorize`, `fno-stack-protector`, `fstack-protector`, `fomit-frame-pointer`, `mthread-model single` | -53.2 | +0.0 |
| `_ZN7StarMap4drawEv` | 2644 | 99.1 | `mlong-calls`, `fno-inline`, `mno-implicit-float`, `fomit-frame-pointer`, `fno-stack-protector`, `fstack-protector`, `ffast-math`, `fno-honor-nans`, `freciprocal-math` | -53.0 | +0.0 |
| `_ZN5MGame8OnUpdateEv` | 11440 | 99.9 | `mlong-calls`, `fomit-frame-pointer`, `fno-inline`, `fno-stack-protector`, `fstack-protector`, `ffast-math`, `fno-honor-nans`, `fno-slp-vectorize`, `fno-strict-aliasing`, `mno-implicit-float`, `freciprocal-math` | -55.3 | +0.0 |

### 4.1 Complete per-target delta matrix

Delta in percentage points vs base; `+0.0` means the flag leaves that target
exactly at its base fuzzy. The matrix covers the 22 flags that move at least one
of the 26 targets. All other measured flags leave all 26 at base.

Column legend: `fastmath` = `-ffast-math`, `nobuiltin` = `-fno-builtin`,
`nohonornans` = `-fno-honor-nans`, `noinline` = `-fno-inline`,
`nojumptab` = `-fno-jump-tables`, `nosibcall` =
`-fno-optimize-sibling-calls`, `nosignedzero` = `-fno-signed-zeros`,
`noslpvec` = `-fno-slp-vectorize`, `nostackprot` = `-fno-stack-protector`,
`nostrictalias` = `-fno-strict-aliasing`, `nostrictover` =
`-fno-strict-overflow`, `nounroll` = `-fno-unroll-loops`, `omitfp` =
`-fomit-frame-pointer`, `recipmath` = `-freciprocal-math`, `stackprotall` =
`-fstack-protector-all`, `stackprot` = `-fstack-protector`, `wrapv` =
`-fwrapv`, `writablestr` = `-fwritable-strings`, `longcalls` =
`-mlong-calls`, `noimplfloat` = `-mno-implicit-float`, `nounalign` =
`-mno-unaligned-access`, `threadsingle` = `-mthread-model single`.

| target | base % | bytes | fastmath | nobuiltin | nohonornans | noinline | nojumptab | nosibcall | nosignedzero | noslpvec | nostackprot | nostrictalias | nostrictover | nounroll | omitfp | recipmath | stackprotall | stackprot | wrapv | writablestr | longcalls | noimplfloat | nounalign | threadsingle |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `_ZN6LayoutC1Ev` | 47.7 | 7072 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | -31.6 | +0.0 | +0.0 | +0.0 | +0.0 | -6.7 | +0.0 | -10.6 | +0.0 | +0.0 | +0.0 | +0.0 | -31.6 | +0.0 | +0.0 |
| `_ZN6LayoutC2Ev` | 47.7 | 7072 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | -31.6 | +0.0 | +0.0 | +0.0 | +0.0 | -6.7 | +0.0 | -10.6 | +0.0 | +0.0 | +0.0 | +0.0 | -31.6 | +0.0 | +0.0 |
| `_ZN11LevelScript7processEi` | 55.0 | 57232 | -1.3 | +0.0 | -0.4 | -29.4 | -23.0 | +0.0 | +0.0 | -0.2 | -12.7 | -4.0 | +0.0 | +0.0 | -26.9 | -0.6 | +0.0 | -12.7 | +0.0 | +0.0 | -34.1 | -1.0 | +0.0 | +0.0 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 73.6 | 1996 | -7.4 | +0.0 | +0.0 | -38.5 | -12.7 | +0.0 | +0.0 | +0.0 | -13.5 | -23.1 | +0.0 | +0.0 | -15.8 | -5.0 | +0.0 | -13.5 | +0.0 | +0.0 | -39.0 | +0.0 | +0.0 | +0.0 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 73.9 | 9908 | -0.1 | +0.0 | +0.0 | -15.7 | +0.0 | +0.0 | +0.0 | +0.0 | -7.2 | -5.1 | -1.8 | +0.0 | -33.5 | -0.1 | +0.0 | -7.2 | -1.8 | +0.0 | -48.2 | +0.0 | +0.0 | +0.0 |
| `_ZN3Gun22calcCharacterCollisionEv` | 74.0 | 1984 | -2.2 | +0.0 | -0.6 | -25.2 | +0.0 | +0.0 | +0.0 | +0.0 | -4.9 | +0.0 | +0.0 | +0.0 | -20.7 | +0.0 | +0.0 | -4.9 | +0.0 | +0.0 | -20.2 | -3.3 | +0.0 | +0.0 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 74.1 | 2764 | +0.0 | +0.0 | +0.0 | -16.0 | -34.9 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | -21.2 | +0.0 | -12.1 | +0.0 | +0.0 | +0.0 | -40.5 | +0.0 | +0.0 | +0.0 |
| `_ZN5MGame10OnTouchEndEiiPv` | 78.1 | 8080 | +0.0 | -1.7 | +0.0 | -25.3 | +0.0 | -7.7 | +0.0 | +0.0 | -14.2 | -7.9 | -6.9 | +0.0 | -41.6 | +0.0 | +0.0 | -14.2 | -6.9 | +0.0 | -51.3 | +0.0 | +0.0 | +0.0 |
| `_ZN7GlobalsC1Ev` | 80.5 | 612 | -1.3 | +0.0 | -1.3 | +0.0 | +0.0 | +0.0 | +0.0 | -38.6 | +0.0 | -32.5 | +0.0 | +0.0 | -41.0 | +0.0 | -39.5 | +0.0 | +0.0 | +0.0 | +0.0 | -38.6 | -23.3 | +0.0 |
| `_ZN7GlobalsC2Ev` | 80.5 | 612 | -1.3 | +0.0 | -1.3 | +0.0 | +0.0 | +0.0 | +0.0 | -38.6 | +0.0 | -32.5 | +0.0 | +0.0 | -41.0 | +0.0 | -39.5 | +0.0 | +0.0 | +0.0 | +0.0 | -38.6 | -23.3 | +0.0 |
| `_ZN12HangarWindow6renderEv` | 80.6 | 9700 | +0.0 | +0.0 | +0.0 | -33.2 | -2.7 | +0.0 | +0.0 | +0.0 | -12.7 | +0.0 | +0.0 | +0.0 | -24.5 | +0.0 | +0.0 | -12.7 | +0.0 | -2.6 | -33.2 | +0.0 | +0.0 | +0.0 |
| `_ZN6Status16missionCompletedEbbx` | 86.7 | 1432 | +0.0 | +0.0 | +0.0 | -57.3 | -43.6 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | +0.0 | -51.1 | +0.0 | -24.5 | +0.0 | +0.0 | +0.0 | -56.6 | +0.0 | +0.0 | +0.0 |
| `_ZN9RocketGun6updateEi` | 86.9 | 1908 | -7.2 | +0.0 | +0.0 | -50.7 | +0.0 | +0.0 | +0.0 | +0.0 | -11.5 | -21.8 | +0.0 | +0.0 | -30.8 | -5.8 | +0.0 | -11.5 | +0.0 | +0.0 | -42.9 | -10.0 | +0.0 | +0.0 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 87.3 | 968 | +0.0 | +0.0 | +0.0 | -18.9 | +0.0 | +0.0 | +0.0 | +0.0 | -21.2 | +0.0 | +0.0 | +0.0 | -28.7 | +0.0 | +0.0 | -21.2 | +0.0 | +0.0 | -44.0 | -3.5 | +0.0 | +0.0 |
| `_ZN5Level11createSceneEv` | 88.3 | 2252 | -0.2 | -5.4 | +0.0 | -25.1 | -19.2 | -9.2 | +0.0 | +0.0 | -18.3 | +0.0 | +0.0 | -14.4 | -23.7 | -0.2 | +0.0 | -18.3 | +0.0 | +0.0 | -55.3 | +0.0 | +0.0 | +0.0 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 88.8 | 9332 | -5.4 | +0.0 | -1.9 | +0.0 | -3.1 | -17.0 | +0.0 | +0.0 | -20.2 | +0.0 | +0.0 | +0.0 | -43.9 | -4.4 | +0.0 | -20.2 | +0.0 | +0.0 | -44.3 | +0.0 | -4.8 | +0.0 |
| `_ZN5Level13createMissionEv` | 90.7 | 17172 | -5.4 | -3.8 | +0.0 | -50.7 | -26.6 | +0.0 | -8.5 | -2.1 | -20.3 | -21.7 | +0.0 | +0.0 | -33.8 | -3.9 | +0.0 | -20.3 | +0.0 | +0.0 | -45.8 | -21.0 | -2.1 | +0.0 |
| `_ZN15MenuTouchWindowC1Ei` | 93.9 | 9600 | -0.6 | -2.4 | +0.0 | -20.0 | +0.0 | +0.0 | +0.0 | -11.7 | -14.0 | -12.1 | +0.0 | +0.0 | -36.4 | +0.0 | +0.0 | -14.0 | +0.0 | +0.0 | -51.6 | -12.0 | -3.6 | +0.0 |
| `_ZN15MenuTouchWindowC2Ei` | 93.9 | 9600 | -0.6 | -2.4 | +0.0 | -20.0 | +0.0 | +0.0 | +0.0 | -11.7 | -14.0 | -12.1 | +0.0 | +0.0 | -36.4 | +0.0 | +0.0 | -14.0 | +0.0 | +0.0 | -51.6 | -12.0 | -3.6 | +0.0 |
| `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | 95.2 | 8132 | -0.8 | +0.0 | -0.5 | -48.8 | -67.7 | +0.0 | +0.0 | +0.0 | -37.8 | +0.0 | -8.3 | +0.0 | -58.9 | +0.0 | +0.0 | -37.8 | -8.3 | +0.0 | -51.8 | +0.0 | +0.0 | +0.0 |
| `_ZN11SpaceLounge10drawLoungeEv` | 95.6 | 1948 | +0.0 | +0.0 | +0.0 | -30.0 | +0.0 | +0.0 | +0.0 | +0.0 | -20.0 | +0.0 | +0.0 | +0.0 | -22.8 | +0.0 | +0.0 | -20.0 | +0.0 | +0.0 | -45.2 | +0.0 | +0.0 | +0.0 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 97.7 | 1896 | +0.0 | +0.0 | +0.0 | -49.2 | -27.5 | +0.0 | +0.0 | +0.0 | +0.0 | -7.1 | +0.0 | +0.0 | -48.0 | +0.0 | -22.6 | +0.0 | +0.0 | +0.0 | -47.7 | +0.0 | +0.0 | +0.0 |
| `_ZN10ModStationC1Ev` | 98.6 | 1008 | +0.0 | -34.2 | +0.0 | -53.2 | +0.0 | +0.0 | +0.0 | -24.8 | -23.0 | +0.0 | +0.0 | +0.0 | -22.9 | +0.0 | +0.0 | -23.0 | +0.0 | +0.0 | -42.9 | -38.4 | +0.0 | -0.2 |
| `_ZN10ModStationC2Ev` | 98.6 | 1008 | +0.0 | -34.2 | +0.0 | -53.2 | +0.0 | +0.0 | +0.0 | -24.8 | -23.0 | +0.0 | +0.0 | +0.0 | -22.9 | +0.0 | +0.0 | -23.0 | +0.0 | +0.0 | -42.9 | -38.4 | +0.0 | -0.2 |
| `_ZN7StarMap4drawEv` | 99.1 | 2644 | -9.9 | +0.0 | -0.6 | -45.6 | +0.0 | +0.0 | +0.0 | +0.0 | -12.5 | +0.0 | +0.0 | +0.0 | -24.7 | -0.4 | +0.0 | -12.5 | +0.0 | +0.0 | -53.0 | -34.0 | +0.0 | +0.0 |
| `_ZN5MGame8OnUpdateEv` | 99.9 | 11440 | -19.3 | +0.0 | -10.8 | -22.9 | +0.0 | +0.0 | +0.0 | -12.3 | -19.5 | -11.0 | +0.0 | +0.0 | -41.8 | -1.1 | +0.0 | -19.5 | +0.0 | +0.0 | -55.3 | -12.3 | +0.0 | +0.0 |

## 5. Per-TU matrix

The per-TU B/A tested every flag that moves a target globally on that target's
own TU. There are **277 cells** (26 targets, the flags that move each target).
**No cell improves a target; the best per-TU Δ is 0.0.** The driver skipped
flags containing spaces; the one such flag, `-mthread-model single`, was run
separately on `ModStation.cpp` (98.4 vs base 98.6, Δ -0.2), matching the global
result. The corrected 24 cells for the three TUs with scoped `-mllvm` flags are
below; they match the global effect exactly.

| TU | target | flag | base % | per-TU % | global % | per-TU Δ | size (B) |
|---|---|---|---:|---:|---:|---:|---:|
| `game_menu_SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | `-fno-inline` | 95.6 | 65.6 | 65.6 | -30.0 | 2032 |
| `game_menu_SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | `-fno-stack-protector` | 95.6 | 75.6 | 75.6 | -20.0 | 1912 |
| `game_menu_SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | `-fomit-frame-pointer` | 95.6 | 72.8 | 72.8 | -22.8 | 1924 |
| `game_menu_SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | `-fstack-protector` | 95.6 | 75.6 | 75.6 | -20.0 | 1912 |
| `game_menu_SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | `-mlong-calls` | 95.6 | 50.4 | 50.4 | -45.2 | 2628 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-ffast-math` | 73.9 | 73.8 | 73.8 | -0.1 | 9908 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-fno-inline` | 73.9 | 58.2 | 58.2 | -15.7 | 10068 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-fno-stack-protector` | 73.9 | 66.7 | 66.7 | -7.2 | 9820 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-fno-strict-aliasing` | 73.9 | 68.8 | 68.8 | -5.1 | 9944 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-fno-strict-overflow` | 73.9 | 72.1 | 72.1 | -1.8 | 9916 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-fomit-frame-pointer` | 73.9 | 40.4 | 40.4 | -33.5 | 9924 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-freciprocal-math` | 73.9 | 73.8 | 73.8 | -0.1 | 9908 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-fstack-protector` | 73.9 | 66.7 | 66.7 | -7.2 | 9820 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-fwrapv` | 73.9 | 72.1 | 72.1 | -1.8 | 9916 |
| `game_weapons_Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `-mlong-calls` | 73.9 | 25.7 | 25.7 | -48.2 | 12348 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-ffast-math` | 99.1 | 89.2 | 89.2 | -9.9 | 2652 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-fno-honor-nans` | 99.1 | 98.5 | 98.5 | -0.6 | 2644 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-fno-inline` | 99.1 | 53.5 | 53.5 | -45.6 | 2764 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-fno-stack-protector` | 99.1 | 86.6 | 86.6 | -12.5 | 2612 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-fomit-frame-pointer` | 99.1 | 74.4 | 74.4 | -24.7 | 2604 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-freciprocal-math` | 99.1 | 98.7 | 98.7 | -0.4 | 2644 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-fstack-protector` | 99.1 | 86.6 | 86.6 | -12.5 | 2612 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-mlong-calls` | 99.1 | 46.1 | 46.1 | -53.0 | 3256 |
| `game_world_StarMap.cpp` | `_ZN7StarMap4drawEv` | `-mno-implicit-float` | 99.1 | 65.1 | 65.1 | -34.0 | 2612 |

For the other 23 targets the per-TU result is the same as the global result for
that target's TU: every non-neutral flag lowers the target. Examples:
`Layout.cpp` + `-fno-slp-vectorize` = 16.1 (base 47.7), `Globals.cpp` +
`-fno-strict-aliasing` = 48.0 (base 80.5), `Level.cpp` + `-fno-jump-tables` =
39.2/69.1 (base 74.1/88.3 for `createShip`/`createScene`), `Hud.cpp` +
`-fno-optimize-sibling-calls` = 71.8 (base 88.8), `MenuTouchWindow.cpp` +
`-fno-jump-tables` = 27.5 (base 95.2 for `OnTouchEnd`).

## 6. Official verifier confirmations

The official `tools/verify/verify.py` was run on the four variants that were
close enough to matter. The gate counts agree with the screening verifier:

| variant | byte | linked | size-exact | missing | verdict |
|---|---:|---:|---:|---:|---|
| base | 4498 | 4498 | 4524 | 0 | — |
| `-fno-data-sections` | 4498 | 4498 | 4524 | 0 | gate-neutral |
| `-fno-use-cxa-atexit` | 1356 | 4497 | 4523 | 0 | worse |
| `-fno-threadsafe-statics` | 1368 | 4494 | 4520 | 3 | worse |
| `-fomit-frame-pointer` | 1298 | 1921 | 2163 | 0 | worse |

`-fno-data-sections` is the only flag that keeps the gate exactly at baseline;
it does not improve any of the 26 targets, so it is not landed.

## 7. Function-level evidence

All 26 functions are size-exact. The table below gives the byte size, the
normalized instruction count, the index of the first structural difference, the
number of positional differences in the normalized stream, whether the first
eight and last eight instructions are identical, and whether the frame pointer
(`add r7, sp, #...`) is present. The positional-difference count is inflated by
alignment shifts after a register-allocation difference; the first-difference
index is the more informative column.

| target | bytes | insns orig/ours | first diff | diff positions | prologue same | epilogue same | frame ptr |
|---|---:|---:|---:|---:|---|---|---|
| `_ZN6LayoutC1Ev` | 7072 | 2529/2503 | 2 | 2388 | no | no | yes |
| `_ZN6LayoutC2Ev` | 7072 | 2529/2503 | 2 | 2388 | no | no | yes |
| `_ZN11LevelScript7processEi` | 57232 | 20169/19968 | 6 | 19847 | no | yes | yes |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 727/727 | 5 | 665 | no | no | yes |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 3492/3488 | 3 | 3194 | no | yes | yes |
| `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 650/650 | 6 | 576 | no | no | yes |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 1007/1008 | 28 | 826 | yes | yes | yes |
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 2775/2775 | 134 | 1438 | yes | yes | yes |
| `_ZN7GlobalsC1Ev` | 612 | 225/227 | 34 | 179 | yes | no | yes |
| `_ZN7GlobalsC2Ev` | 612 | 225/227 | 34 | 179 | yes | no | yes |
| `_ZN12HangarWindow6renderEv` | 9700 | 3626/3637 | 6 | 3441 | no | yes | yes |
| `_ZN6Status16missionCompletedEbbx` | 1432 | 523/522 | 56 | 459 | yes | yes | yes |
| `_ZN9RocketGun6updateEi` | 1908 | 635/636 | 7 | 428 | no | no | yes |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 347/345 | 26 | 299 | yes | yes | yes |
| `_ZN5Level11createSceneEv` | 2252 | 783/791 | 460 | 268 | yes | yes | yes |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 3149/3153 | 22 | 2319 | yes | yes | yes |
| `_ZN5Level13createMissionEv` | 17172 | 6055/6070 | 28 | 4194 | yes | no | yes |
| `_ZN15MenuTouchWindowC1Ei` | 9600 | 3320/3323 | 13 | 2871 | yes | yes | yes |
| `_ZN15MenuTouchWindowC2Ei` | 9600 | 3320/3323 | 13 | 2871 | yes | yes | yes |
| `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | 8132 | 2903/2902 | 40 | 1778 | yes | no | yes |
| `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 729/729 | 142 | 80 | yes | yes | yes |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 734/734 | 402 | 26 | yes | yes | yes |
| `_ZN10ModStationC1Ev` | 1008 | 364/364 | 119 | 15 | yes | yes | yes |
| `_ZN10ModStationC2Ev` | 1008 | 364/364 | 119 | 15 | yes | yes | yes |
| `_ZN7StarMap4drawEv` | 2644 | 917/916 | 197 | 590 | yes | yes | yes |
| `_ZN5MGame8OnUpdateEv` | 11440 | 3862/3863 | 1992 | 1823 | yes | yes | yes |

Key observations:

* **Frame pointer is present in all 26.** `-fomit-frame-pointer` is not the
  missing flag; the original's `push {r4, r5, r6, r7, lr}` / `add r7, sp, #N`
  prologue is reproduced.
* **Stack canary is present where the original has one.** The per-TU
  fingerprint audit (`per_tu_flags_research.md`) found the stack-canary set
  identical across all 195 app TUs (652 functions in both), so
  `-fno-stack-protector` / `-fstack-protector` are not missing. The same audit
  found the other instruction-selection fingerprints exact across all 195 app
  TUs: `movw` 407/407, `strd` 775/775, NEON 640/640, `tbb`/`tbh` 84/84, sized
  new/delete 573/573, division helpers 54/54. Flags that disable those
  (`-mno-movt`, `-mno-movw`, `-mno-ldrd`, `-mno-strd`, `-fno-vectorize`,
  `-fno-jump-tables`, `-fno-sized-deallocation`) are therefore not missing.
* **The diffs are local.** The first difference is often in the callee-saved
  register set (e.g. `Layout::Layout`: original `stmdb sp!, {r6,r7,r8,r9,sl,fp}`
  vs ours `{r5,r6,r7,r8,r9,sl,fp}`; `StarMap::draw`: the same stack/table base
  in `r8` on one side and `fp` on the other), in instruction scheduling (e.g.
  `ModStation::ModStation` reorders a PC-relative table load around `add r0,
  pc`), or in basic-block order (e.g. `MGame::OnUpdate` around instruction
  1992). These are exactly the things a source-shape change moves, not a global
  flag.
* **The scoped `-mllvm` flags are compensations, not original flags.** The tree
  has `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` on
  `Radar.cpp`, `20/12` on `SpaceLounge.cpp`, and `7/1` on `StarMap.cpp`
  (CMakeLists lines 49-59). `mllvm_flag_audit.md` shows the older two are
  neutral/worse when applied globally; the `7/1` StarMap hint is a later
  compensation for the same allocator threshold. The per-TU matrix above
  preserves them and still finds no global flag that improves the target.

## 8. Interpretation: flag-driven vs source-shape-driven

A flag-driven gap would show at least one of:

1. a flag that raises `byte_exact` or `linked_exact` without dropping the gate;
2. a target whose fuzzy rises under some flag;
3. a systematic fingerprint mismatch (frame pointer, stack canary, NEON,
   jump table, `movw`/`movt`, sized new/delete, guard variable) across a TU.

None of these is present:

* No variant beats 4498/4498/4524; no target's fuzzy rises (max Δ = +0.0).
* The frame-pointer and stack-canary fingerprints are already exact.
* The 26 functions are all size-exact; their diffs are local register
  allocation, scheduling, encoding and block order.
* The per-TU matrix finds no local flag win either.

Therefore **all 26 remaining functions are source-shape-driven**. The current
global flag set (including `-Oz`, `-g`, `-fPIC`, `-frtti`,
`-fstack-protector-strong`, strict aliasing, frame pointers, vectorization,
inlining, threadsafe statics and `-fcommon` default) already reproduces the
original's codegen; every non-neutral alternative is worse. The remaining gap
is the known source/TU-shape queue: expression order, local lifetimes,
statement order, TU membership and the scoped `-mllvm` compensations.

## 9. Bottom line

* **No flag is a strict improvement.** No flag is landed; this commit contains
  only this note.
* **No remaining function is flag-driven.** All 26 are source-shape-driven.
* **No missing original flag can be identified** from the tested matrix. The
  next work on these functions should be source-shape/TU-membership work, not
  another global-flag sweep.

## Appendix A: full global matrix

`targets moved` = number of the 26 targets whose fuzzy changed; `min Δ`/`max Δ`
over those targets. `size` is size-exact functions. `-fvisibility=hidden` hides
the target symbols, so its `targets moved` is 0 by construction.

| flag | byte | linked | size | missing | targets moved | min Δ | max Δ | verdict |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| `` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-faligned-allocation` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-ffast-math` | 1319 | 4225 | 4369 | 0 | 15 | -19.3 | +0.0 | worse |
| `-ffp-contract=fast` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-ffp-contract=off` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fmerge-all-constants` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-access-control` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-aligned-allocation` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-associative-math` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-assume-sane-operator-new` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-builtin` | 1350 | 3986 | 4121 | 0 | 7 | -34.2 | +0.0 | worse |
| `-fno-common` | — | — | — | — | — | — | — | build/link fail |
| `-fno-data-sections` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-delayed-template-parsing` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-delete-null-pointer-checks` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-exceptions` | — | — | — | — | — | — | — | build/link fail |
| `-fno-finite-math-only` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-honor-infinities` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-honor-nans` | 1429 | 4373 | 4480 | 0 | 8 | -10.8 | +0.0 | worse |
| `-fno-inline` | 1219 | 3511 | 3571 | 0 | 21 | -57.3 | +0.0 | worse |
| `-fno-jump-tables` | 1363 | 4410 | 4437 | 0 | 10 | -67.7 | +0.0 | worse |
| `-fno-math-errno` | 1356 | 4487 | 4514 | 0 | 0 | +0.0 | +0.0 | worse |
| `-fno-merge-constants` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-omit-frame-pointer` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-operator-names` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-optimize-sibling-calls` | 1316 | 3587 | 3686 | 0 | 3 | -17.0 | +0.0 | worse |
| `-fno-plt` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-rounding-math` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-rtti-data` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-rtti` | 1725 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | worse |
| `-fno-semantic-interposition` | — | — | — | — | — | — | — | build/link fail |
| `-fno-signed-char` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-signed-zeros` | 2355 | 4489 | 4521 | 0 | 1 | -8.5 | +0.0 | worse |
| `-fno-sized-deallocation` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-slp-vectorize` | 1484 | 4384 | 4408 | 0 | 11 | -38.6 | +0.0 | worse |
| `-fno-stack-protector` | 1369 | 3781 | 3789 | 0 | 19 | -37.8 | +0.0 | worse |
| `-fno-stack-size-section` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-strict-aliasing` | 1371 | 4349 | 4411 | 0 | 12 | -32.5 | +0.0 | worse |
| `-fno-strict-overflow` | 1398 | 4462 | 4500 | 0 | 3 | -8.3 | +0.0 | worse |
| `-fno-threadsafe-statics` | 1368 | 4494 | 4520 | 3 | 0 | +0.0 | +0.0 | worse |
| `-fno-trapping-math` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-unroll-loops` | 2892 | 4491 | 4520 | 0 | 1 | -14.4 | +0.0 | worse |
| `-fno-unsafe-math-optimizations` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-use-cxa-atexit` | 1356 | 4497 | 4523 | 0 | 0 | +0.0 | +0.0 | worse |
| `-fno-vectorize` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fno-zero-initialized-in-bss` | 1522 | 4488 | 4516 | 0 | 0 | +0.0 | +0.0 | worse |
| `-fomit-frame-pointer` | 1298 | 1921 | 2163 | 0 | 26 | -58.9 | -6.7 | worse |
| `-freciprocal-math` | 1539 | 4402 | 4483 | 0 | 9 | -5.8 | +0.0 | worse |
| `-fshort-enums` | 2869 | 4476 | 4511 | 0 | 0 | +0.0 | +0.0 | worse |
| `-fshort-wchar` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fsigned-char` | 4440 | 4494 | 4522 | 0 | 0 | +0.0 | +0.0 | worse |
| `-fstack-protector-all` | 91 | 962 | 981 | 0 | 7 | -39.5 | +0.0 | worse |
| `-fstack-protector` | 1369 | 3785 | 3793 | 0 | 19 | -37.8 | +0.0 | worse |
| `-fstack-size-section` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-fvisibility=hidden` | 57 | 243 | 244 | 4280 | — | — | — | worse |
| `-fwrapv` | 1398 | 4462 | 4500 | 0 | 3 | -8.3 | +0.0 | worse |
| `-fwritable-strings` | 1373 | 4489 | 4515 | 0 | 1 | -2.6 | +0.0 | worse |
| `-mhard-float` | — | — | — | — | — | — | — | build/link fail |
| `-mlong-calls` | 1345 | 1664 | 1673 | 0 | 22 | -56.6 | +0.0 | worse |
| `-mno-crc` | — | — | — | — | — | — | — | build/link fail |
| `-mno-dsp` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mno-execute-only` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mno-fancy-math-387` | — | — | — | — | — | — | — | build/link fail |
| `-mno-fp-ret-in-387` | — | — | — | — | — | — | — | build/link fail |
| `-mno-global-merge` | 1397 | 4488 | 4517 | 0 | 0 | +0.0 | +0.0 | worse |
| `-mno-implicit-float` | 1357 | 4263 | 4286 | 0 | 15 | -38.6 | +0.0 | worse |
| `-mno-madd4` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mno-movt` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mno-neg-immediates` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mno-neon-for-64bits` | — | — | — | — | — | — | — | build/link fail |
| `-mno-restrict-it` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mno-stack-arg-probe` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mno-thumb-interwork` | — | — | — | — | — | — | — | build/link fail |
| `-mno-unaligned-access` | 1404 | 4443 | 4471 | 0 | 6 | -23.3 | +0.0 | worse |
| `-mrelax-all` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-msoft-float` | — | — | — | — | — | — | — | build/link fail |
| `-mstack-alignment=16` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mstack-alignment=4` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mstack-alignment=8` | 4498 | 4498 | 4524 | 0 | 0 | +0.0 | +0.0 | gate-neutral |
| `-mthread-model single` | 2085 | 4495 | 4523 | 0 | 2 | -0.2 | +0.0 | worse |
| `-mthumb-interwork` | — | — | — | — | — | — | — | build/link fail |

## Appendix B: reproduction artifacts

Scratch data (not tracked):

```text
/dev/shm/w3751/toolchain_extra.cmake       global A/B wrapper
/dev/shm/w3751/per_tu_flags.cmake          corrected per-TU wrapper (COMPILE_FLAGS)
/dev/shm/w3751/run_matrix.py               81-variant global driver
/dev/shm/w3751/run_pt.py                   first per-TU driver (COMPILE_OPTIONS)
/dev/shm/w3751/run_pt2.py                  corrected per-TU driver (COMPILE_FLAGS)
/dev/shm/w3751/score_targets.py            accurate per-target scorer (autojunk=False)
/dev/shm/w3751/results/*.json              per-variant summaries
/dev/shm/w3751/scores/*.json               per-variant 26-target fuzzy
/dev/shm/w3751/pt_scores2/*.json           corrected per-TU 26-target fuzzy
/dev/shm/w3751/verify_official_*/report.json  official verifier reports
```

The repo's `check` target was re-run after the sweep: `RATCHET PASS` at
4498/4498, lint `CLEAN ✓`, drift 0. `report.json` was not committed.

