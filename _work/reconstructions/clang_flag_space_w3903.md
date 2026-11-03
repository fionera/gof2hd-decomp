# Complete clang-7 flag space beyond w3751 (w3903)

**Status:** research note, 2026-11-03. Worktree `wt/w3903`, branch `wave89-w3903`,
base `dfc9d7a1`. No source or build-flag change landed: no tested flag is a strict
improvement at this tree state. This commit contains only this note.

## 0. TL;DR

* Baseline: **byte_exact 4498/4524, linked_exact 4498/4524, size-exact
  4524/4524, missing 0, extra 0, wrong_type 0**, data_exact 843/843, imports
  207, `.text 0x1337c8`, `.rodata 0x41b67`, lint `CLEAN ✓`, drift 0.
* **215 table rows (214 distinct flag spellings plus the `-fvisibility=default`
  retry)** were tested with a full rebuild + gold link + the official
  `tools/verify/verify.py` (96 gate-neutral, 56 gate-changing, 63
  build/link/verify failures). This includes every candidate in the task list
  plus a second sweep of every other plausible codegen/ABI knob in
  `clang -cc1 --help`.
* **No variant beats 4498/4498/4524 and no variant raises the fuzzy of any of
  the 26 targets** (max Δ = +0.0). No flag is landed.
* **262 per-TU cells** were measured for the 15 newly-moving flags. No cell
  improves a target; 3 cells are exactly equal, all the rest are negative.
* New negative findings not in w3751: `-fexperimental-new-pass-manager`,
  `-mstackrealign`, `-momit-leaf-frame-pointer`, `-fsized-deallocation`,
  `-ftrapv`, `-ffreestanding`, `-fno-elide-constructors`,
  `-fsjlj-exceptions`, `-fstrict-vtable-pointers`, `-fvisibility=protected`,
  `-fno-pic`/`-fno-PIC`/`-fno-pie`/`-fno-PIE`, `-marm`/`-mno-thumb`,
  `-mexecute-only`, `-mrestrict-it`, and `-g0`/`-g1`/`-gline-tables-only`/
  `-gmlt` all lower the gate and/or move targets downward.
* `-fno-unwind-tables` is order-sensitive: appended it is neutral (the
  positive `-funwind-tables` comes later); applied after the positive flag it
  drops byte_exact 4498 -> 2878 while linked_exact stays 4498 and no target
  moves. `-fno-asynchronous-unwind-tables` is a true no-op on this target.
* `-fno-common` fails on the duplicate tentative definition
  `g_android_back_button_pressed`; after changing the unused copy in
  `GOF2HD2012.c` to `extern`, it links but drops byte_exact 4498 -> 4467 with
  linked_exact still 4498 and no target moved. The original used clang 7's
  default `-fcommon`.
* **The flag space is closed for clang 7.0.2 at `-Oz`.** The remaining 26
  functions are source-shape-driven, not flag-driven.

> **Baseline note.** The current tree's `Status::missionCompleted` is **57.7 %**
> (1432 B), not the 86.7 % quoted in the `dfc9d7a1` commit message. The
> authoritative fresh build report (`cmake-build-match/verify/report.json`) is
> used throughout this note. The gate counts are unaffected.

## 1. Baseline

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w3903
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Output:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

The 26 non-exact targets (all size-exact) and their baseline fuzzy scores:

| # | target | orig B | ours B | fuzzy % | byte | linked |
|---:|---|---:|---:|---:|---|---|
| 1 | `_ZN6LayoutC1Ev` | 7072 | 7072 | 47.7 | False | False |
| 2 | `_ZN6LayoutC2Ev` | 7072 | 7072 | 47.7 | False | False |
| 3 | `_ZN11LevelScript7processEi` | 57232 | 57232 | 55.0 | False | False |
| 4 | `_ZN6Status16missionCompletedEbbx` | 1432 | 1432 | 57.7 | False | False |
| 5 | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 1996 | 73.6 | False | False |
| 6 | `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 9908 | 73.9 | False | False |
| 7 | `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 2764 | 74.1 | False | False |
| 8 | `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 8080 | 78.4 | False | False |
| 9 | `_ZN7GlobalsC1Ev` | 612 | 612 | 80.5 | False | False |
| 10 | `_ZN7GlobalsC2Ev` | 612 | 612 | 80.5 | False | False |
| 11 | `_ZN12HangarWindow6renderEv` | 9700 | 9700 | 80.6 | False | False |
| 12 | `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 1984 | 82.7 | False | False |
| 13 | `_ZN9RocketGun6updateEi` | 1908 | 1908 | 86.9 | False | False |
| 14 | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 968 | 87.3 | False | False |
| 15 | `_ZN5Level11createSceneEv` | 2252 | 2252 | 88.3 | False | False |
| 16 | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 9332 | 88.8 | False | False |
| 17 | `_ZN5Level13createMissionEv` | 17172 | 17172 | 90.7 | False | False |
| 18 | `_ZN15MenuTouchWindowC1Ei` | 9600 | 9600 | 93.9 | False | False |
| 19 | `_ZN15MenuTouchWindowC2Ei` | 9600 | 9600 | 93.9 | False | False |
| 20 | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | 8132 | 8132 | 95.2 | False | False |
| 21 | `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 1948 | 95.6 | False | False |
| 22 | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 1896 | 97.7 | False | False |
| 23 | `_ZN10ModStationC1Ev` | 1008 | 1008 | 98.6 | False | False |
| 24 | `_ZN10ModStationC2Ev` | 1008 | 1008 | 98.6 | False | False |
| 25 | `_ZN7StarMap4drawEv` | 2644 | 2644 | 99.1 | False | False |
| 26 | `_ZN5MGame8OnUpdateEv` | 11440 | 11440 | 99.9 | False | False |

Section sizes (ours vs original): `.text 0x1337c8`, `.rodata 0x41b67`,
`.data 0x296c`, `.bss 0x69ac` — all identical.

## 2. Method

### 2.1 Harness

All scratch lives under `/dev/shm/w3903` (never `/tmp`). A wrapper toolchain
includes the repo toolchain and appends `GOF2_EXTRA_FLAGS` to both
`CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
# /dev/shm/w3903/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w3903/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM C compile flags" FORCE)
endif()
```

Each global variant is a full rebuild + gold link + official verify:

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w3903
S=/dev/shm/w3903
B=$S/builds/<id>
TMPDIR=$S/tmp cmake -S "$ROOT" -B "$B" -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$S/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=$S/tmp cmake --build "$B" --target link_so -j16
python3 "$ROOT/tools/verify/verify.py" \
  --build-dir "$B/verify" \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours "$B/libgof2hdaa.so" \
  --objdump "$ROOT/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump" \
  --fail-on-wrong-type --top 0
```

`verify.py` is the official verifier; `score()` uses `autojunk=False`, so the
`pct` column is directly comparable to the gate's fuzzy column. Eight variants
were built concurrently with `-j16` each.

### 2.2 Per-TU A/B

The per-TU wrapper appends the flag to the owning TU's `COMPILE_FLAGS`, which
CMake places after the target's `COMPILE_OPTIONS` (important for the three TUs
with scoped `-mllvm` knobs):

```cmake
# /dev/shm/w3903/per_tu_flags.cmake
if(GOF2_AB_TU)
  set_source_files_properties("${CMAKE_SOURCE_DIR}/src/${GOF2_AB_TU}"
      PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
endif()
```

```sh
TMPDIR=$S/tmp cmake -S "$ROOT" -B "$B" -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$S/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS= \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DCMAKE_PROJECT_INCLUDE=$S/per_tu_flags.cmake \
  -DGOF2_AB_TU='<tu>' -DGOF2_AB_FLAGS='<flag>'
```

### 2.3 Order-sensitive flags

Flags appended to `CMAKE_CXX_FLAGS` come **before** the target's
`-ffunction-sections -funwind-tables`. For the negative unwind/function-section
flags this is a flag-order trap, so a second wrapper appends the flag to every
source's `COMPILE_FLAGS` (after the positive target option):

```cmake
# /dev/shm/w3903/global_compile_flags.cmake
if(GOF2_GLOBAL_COMPILE_FLAGS)
  file(GLOB_RECURSE _gof2_all_srcs "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.c")
  set_source_files_properties(${_gof2_all_srcs} PROPERTIES COMPILE_FLAGS "${GOF2_GLOBAL_COMPILE_FLAGS}")
endif()
```

```sh
cmake ... -DCMAKE_PROJECT_INCLUDE=$S/global_compile_flags.cmake \
  -DGOF2_GLOBAL_COMPILE_FLAGS='-fno-unwind-tables'
```

### 2.4 Vendor-archive race

The vendored zlib/libzip/libcrypto archives are shared under
`third_party/*/*_gof2.a`. Concurrent builds occasionally re-ran the shared
`cmake -P build_match_lib.cmake` and one `visdefault` build hit a truncated
OpenSSL `.S` file (`unterminated #if`). The affected variants were rerun
sequentially (`visdefault` -> `visdefault_retry`, one `g0` per-TU cell); both
reproduced the expected result. A post-matrix `check` still reports
`RATCHET PASS`, and a repeated neutral variant is byte-identical to its first
run, so the archive contents are consistent.

## 3. Global matrix

**215 table rows** (214 distinct flag spellings plus the `-fvisibility=default`
retry; the one `g0` per-TU retry is in §4). `moved` is the number of the
26 targets whose fuzzy changed; `min Δ`/`max Δ` are over those targets. A
positive `max Δ` would be an improvement; there is none. `size` is the number of
functions with `osize == usize` (4524 is exact).

### 3.1 Gate-neutral variants (96)

These leave `byte_exact 4498`, `linked_exact 4498`, `size 4524`, `missing 0`
and all 26 target fuzzy values exactly at baseline. They are useful negatives:
the original's codegen is consistent with clang 7.0.2's default `-Oz` pipeline.

| # | flag | byte | linked | size | missing | extra | wrong | moved | min Δ | max Δ |
|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 1 | `-DANDROID` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 2 | `-DNDEBUG` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 3 | `-DNDEBUG -D_FORTIFY_SOURCE=2` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 4 | `-D_FORTIFY_SOURCE=0` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 5 | `-D_FORTIFY_SOURCE=2` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 6 | `-D__STDC_CONSTANT_MACROS` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 7 | `-D__STDC_LIMIT_MACROS` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 8 | `-Qn` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 9 | `-U_FORTIFY_SOURCE` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 10 | `-fclang-abi-compat=4.0` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 11 | `-fclang-abi-compat=5.0` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 12 | `-fclang-abi-compat=6.0` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 13 | `-fclang-abi-compat=7.0` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 14 | `-fcommon` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 15 | `-fcxx-exceptions` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 16 | `-fdebug-info-for-profiling` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 17 | `-fdebug-prefix-map=/home/debian/GalaxyOnFire2/wt/w3903=/X` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 18 | `-fdebug-types-section` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 19 | `-fexceptions` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 20 | `-ffine-grained-bitfield-accesses` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 21 | `-fgnu-keywords` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 22 | `-fmax-type-align=16` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 23 | `-fmax-type-align=8` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 24 | `-fmerge-all-constants` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 25 | `-fmessage-length=0` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 26 | `-fnew-alignment=8` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 27 | `-fno-access-control` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 28 | `-fno-aligned-allocation` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 29 | `-fno-assume-sane-operator-new` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 30 | `-fno-asynchronous-unwind-tables` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 31 | `-fno-asynchronous-unwind-tables (COMPILE_FLAGS)` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 32 | `-fno-caret-diagnostics` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 33 | `-fno-color-diagnostics` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 34 | `-fno-data-sections` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 35 | `-fno-debug-info-for-profiling` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 36 | `-fno-delayed-template-parsing` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 37 | `-fno-delete-null-pointer-checks` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 38 | `-fno-diagnostics-show-option` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 39 | `-fno-dollars-in-identifiers` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 40 | `-fno-elide-type` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 41 | `-fno-experimental-new-pass-manager` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 42 | `-fno-fine-grained-bitfield-accesses` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 43 | `-fno-function-sections` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 44 | `-fno-gnu-keywords` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 45 | `-fno-ident` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 46 | `-fno-inline-functions-called-once` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 47 | `-fno-lax-vector-conversions` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 48 | `-fno-lto` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 49 | `-fno-merge-constants` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 50 | `-fno-omit-frame-pointer` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 51 | `-fno-operator-names` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 52 | `-fno-plt` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 53 | `-fno-preserve-as-comments` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 54 | `-fno-rtti-data` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 55 | `-fno-show-column` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 56 | `-fno-signed-char` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 57 | `-fno-sized-deallocation` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 58 | `-fno-stack-size-section` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 59 | `-fno-standalone-debug` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 60 | `-fno-strict-vtable-pointers` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 61 | `-fno-trigraphs` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 62 | `-fno-unwind-tables` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 63 | `-fno-use-init-array` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 64 | `-fno-vectorize` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 65 | `-fno-verbose-asm` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 66 | `-fno-whole-program-vtables` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 67 | `-fpack-struct=8` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 68 | `-fshort-wchar` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 69 | `-fstack-size-section` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 70 | `-fstandalone-debug` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 71 | `-fstrict-return` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 72 | `-funique-section-names` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 73 | `-fuse-init-array` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 74 | `-fvisibility=default (retry)` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 75 | `-g` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 76 | `-g2` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 77 | `-g3` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 78 | `-mfloat-abi=softfp` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 79 | `-mglobal-merge` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 80 | `-mincremental-linker-compatible` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 81 | `-mno-dsp` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 82 | `-mno-execute-only` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 83 | `-mno-madd4` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 84 | `-mno-movt` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 85 | `-mno-neg-immediates` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 86 | `-mno-restrict-it` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 87 | `-mnocrc` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 88 | `-mprefer-vector-width=128` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 89 | `-mprefer-vector-width=64` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 90 | `-mrelax-all` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 91 | `-mstack-alignment=16` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 92 | `-mstack-alignment=4` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 93 | `-mstack-alignment=8` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 94 | `-mstack-probe-size=4096` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 95 | `-mthumb` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |
| 96 | `-mtp=soft` | 4498 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 |

`-fno-unwind-tables` in this table is the **appended** form (neutral only
because `-funwind-tables` follows it). The order-correct form is in §3.2.

### 3.2 Gate-changing variants (56)

All are worse: either a gate count drops, or a target moves downward, or a
data/section-only change drops `byte_exact` without moving a target. The
`verdict` column distinguishes the two.

| # | flag | byte | linked | size | missing | extra | wrong | moved | min Δ | max Δ | verdict |
|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| 1 | `-marm` | 57 | 237 | 342 | 0 | 0 | 0 | 26 | -91.5 | -42.3 | worse |
| 2 | `-mno-thumb` | 57 | 237 | 342 | 0 | 0 | 0 | 26 | -91.5 | -42.3 | worse |
| 3 | `-mstackrealign` | 80 | 245 | 245 | 0 | 0 | 0 | 26 | -14.4 | -0.1 | worse |
| 4 | `-fstack-protector-all` | 91 | 962 | 981 | 0 | 0 | 0 | 7 | -39.5 | -8.4 | worse |
| 5 | `-fno-inline` | 1219 | 3511 | 3572 | 0 | 276 | 0 | 21 | -53.2 | -15.7 | worse |
| 6 | `-fno-inline-functions` | 1219 | 3511 | 3572 | 0 | 276 | 0 | 21 | -53.2 | -15.7 | worse |
| 7 | `-mrestrict-it` | 1270 | 3325 | 4192 | 0 | 0 | 0 | 24 | -18.7 | -0.1 | worse |
| 8 | `-ftrapv` | 1276 | 3864 | 3898 | 0 | 0 | 0 | 23 | -65.4 | -1.0 | worse |
| 9 | `-fomit-frame-pointer` | 1298 | 1921 | 2163 | 0 | 0 | 0 | 26 | -58.9 | -6.7 | worse |
| 10 | `-mexecute-only` | 1299 | 3996 | 4081 | 0 | 0 | 0 | 26 | -40.6 | -2.5 | worse |
| 11 | `-fexperimental-new-pass-manager` | 1300 | 3781 | 3913 | 0 | 0 | 0 | 22 | -59.5 | -1.8 | worse |
| 12 | `-fno-optimize-sibling-calls` | 1316 | 3588 | 3686 | 0 | 0 | 0 | 3 | -17.0 | -7.7 | worse |
| 13 | `-ffast-math` | 1319 | 4225 | 4369 | 0 | 0 | 0 | 15 | -19.3 | -0.1 | worse |
| 14 | `-fno-function-sections (COMPILE_FLAGS)` | 1345 | 4355 | 4399 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 15 | `-fsjlj-exceptions` | 1345 | 3764 | 3773 | 0 | 0 | 0 | 17 | -72.4 | -31.7 | worse |
| 16 | `-fvisibility=protected` | 1345 | 2386 | 4173 | 0 | 0 | 0 | 24 | -61.4 | -4.8 | worse |
| 17 | `-mlong-calls` | 1345 | 1664 | 1673 | 0 | 0 | 0 | 22 | -55.3 | -23.2 | worse |
| 18 | `-fsized-deallocation` | 1347 | 3871 | 3942 | 0 | 0 | 0 | 12 | -10.3 | -1.1 | worse |
| 19 | `-ffreestanding` | 1350 | 3986 | 4121 | 0 | 1 | 0 | 7 | -34.2 | -1.8 | worse |
| 20 | `-fno-builtin` | 1350 | 3986 | 4121 | 0 | 1 | 0 | 7 | -34.2 | -1.8 | worse |
| 21 | `-fno-math-errno` | 1356 | 4487 | 4514 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 22 | `-fno-use-cxa-atexit` | 1356 | 4497 | 4523 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 23 | `-mno-implicit-float` | 1357 | 4263 | 4286 | 0 | 0 | 0 | 15 | -38.6 | -1.0 | worse |
| 24 | `-fno-jump-tables` | 1363 | 4410 | 4436 | 0 | 0 | 0 | 10 | -67.7 | -2.7 | worse |
| 25 | `-fdwarf-exceptions` | 1369 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 26 | `-fno-elide-constructors` | 1370 | 4213 | 4234 | 0 | 0 | 0 | 16 | -61.2 | -7.6 | worse |
| 27 | `-fno-strict-aliasing` | 1371 | 4349 | 4411 | 0 | 0 | 0 | 12 | -32.5 | -3.9 | worse |
| 28 | `-fno-PIC` | 1377 | 2956 | 3078 | 0 | 0 | 0 | 26 | -67.3 | -1.8 | worse |
| 29 | `-fno-PIE` | 1377 | 2956 | 3078 | 0 | 0 | 0 | 26 | -67.3 | -1.8 | worse |
| 30 | `-fno-pic` | 1377 | 2956 | 3078 | 0 | 0 | 0 | 26 | -67.3 | -1.8 | worse |
| 31 | `-fno-pie` | 1377 | 2956 | 3078 | 0 | 0 | 0 | 26 | -67.3 | -1.8 | worse |
| 32 | `-fno-stack-protector` | 1377 | 3781 | 3789 | 0 | 0 | 0 | 19 | -37.8 | -7.2 | worse |
| 33 | `-fstack-protector` | 1377 | 3784 | 3793 | 0 | 0 | 0 | 19 | -37.8 | -7.2 | worse |
| 34 | `-momit-leaf-frame-pointer` | 1386 | 4414 | 4449 | 0 | 0 | 0 | 4 | -41.0 | -6.7 | worse |
| 35 | `-mno-global-merge` | 1397 | 4488 | 4517 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 36 | `-fno-strict-overflow` | 1398 | 4462 | 4500 | 0 | 0 | 0 | 3 | -8.3 | -1.8 | worse |
| 37 | `-fwrapv` | 1398 | 4462 | 4500 | 0 | 0 | 0 | 3 | -8.3 | -1.8 | worse |
| 38 | `-mno-unaligned-access` | 1404 | 4443 | 4471 | 0 | 0 | 0 | 6 | -23.3 | -2.1 | worse |
| 39 | `-fstrict-vtable-pointers` | 1417 | 4441 | 4500 | 0 | 18 | 0 | 4 | -31.8 | -0.1 | worse |
| 40 | `-fno-slp-vectorize` | 1484 | 4384 | 4408 | 0 | 0 | 0 | 11 | -38.6 | -0.2 | worse |
| 41 | `-fno-zero-initialized-in-bss` | 1522 | 4488 | 4516 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 42 | `-fno-rtti` | 1725 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 43 | `-fstrict-enums` | 1763 | 4492 | 4518 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 44 | `-g0` | 2069 | 4246 | 4485 | 0 | 0 | 0 | 5 | -14.9 | -0.1 | worse |
| 45 | `-g1` | 2069 | 4246 | 4485 | 0 | 0 | 0 | 5 | -14.9 | -0.1 | worse |
| 46 | `-gline-tables-only` | 2069 | 4246 | 4485 | 0 | 0 | 0 | 5 | -14.9 | -0.1 | worse |
| 47 | `-gmlt` | 2069 | 4246 | 4485 | 0 | 0 | 0 | 5 | -14.9 | -0.1 | worse |
| 48 | `-mthread-model single` | 2085 | 4495 | 4523 | 0 | 0 | 0 | 2 | -0.2 | -0.2 | worse |
| 49 | `-fshort-enums` | 2869 | 4476 | 4511 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 50 | `-fno-unwind-tables (COMPILE_FLAGS after -funwind-tables)` | 2878 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 51 | `-fno-unroll-loops` | 2892 | 4491 | 4520 | 0 | 0 | 0 | 1 | -14.4 | -14.4 | worse |
| 52 | `-femit-all-decls` | 3396 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 53 | `-fdata-sections` | 4334 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 54 | `-fsigned-char` | 4440 | 4494 | 4522 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 55 | `-fno-common + extern fix in GOF2HD2012.c` | 4467 | 4498 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |
| 56 | `-fnew-alignment=16` | 4496 | 4496 | 4524 | 0 | 0 | 0 | 0 | +0.0 | +0.0 | worse |

Key rows:

* `-fno-unwind-tables` after `-funwind-tables`: byte **2878**, linked 4498,
  size 4524, no target moved. (The appended form above is neutral.)
* `-fno-asynchronous-unwind-tables` after `-funwind-tables`: byte **4498**,
  linked 4498 — a true no-op on ARM.
* `-fno-function-sections` after `-ffunction-sections`: byte **1345**,
  linked 4355, no target moved.
* `-fno-common` + `extern` fix: byte **4467**, linked 4498, no target moved.
* `-fvisibility=default` (retry): byte **4498**, linked 4498, neutral.
* `-femit-all-decls`: byte 3396, linked 4498, no target moved.
* `-fnew-alignment=16`: byte 4496, linked 4496, no target moved.
* `-fno-rtti`: byte 1725, linked 4498, no target moved.
* `-fshort-enums`: byte 2869, linked 4476, no target moved.
* `-fsigned-char`: byte 4440, linked 4494, no target moved.
* `-fno-math-errno`: byte 1356, linked 4487, no target moved.
* `-fno-use-cxa-atexit`: byte 1356, linked 4497, no target moved.
* `-fno-zero-initialized-in-bss`: byte 1522, linked 4488, no target moved.
* `-mno-global-merge`: byte 1397, linked 4488, no target moved.
* `-fdata-sections`: byte 4334, linked 4498, no target moved.
* `-fdwarf-exceptions`: byte 1369, linked 4498, no target moved.
* `-fstrict-enums`: byte 1763, linked 4492, no target moved.

### 3.3 Build/link/verify failures (63)

Where a failure is a real clang 7.0.2 limitation it is marked in the reason;
where it is a source/ABI issue the reason is the first compiler/linker error.

| # | flag | failure |
|---:|---|---|
| 1 | `-fPIE` | /home/debian/GalaxyOnFire2/wt/w3903/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold: er |
| 2 | `-fconst-strings` | clang: error: unknown argument: '-fconst-strings' |
| 3 | `-fdebug-compilation-dir=/dev/shm/w3903` | clang: error: unknown argument: '-fdebug-compilation-dir=/dev/shm/w3903' |
| 4 | `-ffile-prefix-map=/home/debian/GalaxyOnFire2/wt/w3903=/X` | clang: error: unknown argument: '-ffile-prefix-map=/home/debian/GalaxyOnFire2/wt/w3903=/X' |
| 5 | `-fgnu89-inline` | error: invalid argument '-fgnu89-inline' not allowed with 'C++' |
| 6 | `-flto` | BUILD_FAIL flto                                        ~~~~~~~~~~~~              ^~~~~~~~~~ 24 warnings generated. [195/ |
| 7 | `-fmacro-prefix-map=/home/debian/GalaxyOnFire2/wt/w3903=/X` | clang: error: unknown argument: '-fmacro-prefix-map=/home/debian/GalaxyOnFire2/wt/w3903=/X' |
| 8 | `-fmerge-functions` | clang: error: unknown argument: '-fmerge-functions' |
| 9 | `-fno-bitfield-type-align` | clang++: error: unknown argument: '-fno-bitfield-type-align' |
| 10 | `-fno-c++-static-destructors` | clang++: error: unknown argument: '-fno-c++-static-destructors' |
| 11 | `-fno-common` | /home/debian/GalaxyOnFire2/wt/w3903/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold: er |
| 12 | `-fno-const-strings` | clang: error: unknown argument: '-fno-const-strings' |
| 13 | `-fno-cxx-exceptions` | BUILD_FAIL nocxxexceptions          ^ /home/debian/GalaxyOnFire2/wt/w3903/src/game/ship/KIPlayer.h:170:18: note: overrid |
| 14 | `-fno-exceptions` | BUILD_FAIL noexceptions          ^ /home/debian/GalaxyOnFire2/wt/w3903/src/game/ship/KIPlayer.h:170:18: note: overridden |
| 15 | `-fno-integrated-as` | clang++: error: assembler command failed with exit code 1 (use -v to see invocation) |
| 16 | `-fno-integrated-cc1` | clang: error: unknown argument: '-fno-integrated-cc1' |
| 17 | `-fno-math-builtin` | clang++: error: unknown argument: '-fno-math-builtin' |
| 18 | `-fno-merge-functions` | clang++: error: unknown argument: '-fno-merge-functions' |
| 19 | `-fno-semantic-interposition` | clang++: error: unknown argument: '-fno-semantic-interposition' |
| 20 | `-fno-signed-wchar` | clang++: error: unknown argument '-fno-signed-wchar', did you mean '-fno-signed-char'? |
| 21 | `-fno-threadsafe-statics` | VERIFY_FAIL nothreadsafestatics compared 4521  avg 99.89%  byte_exact 1368  linked_exact 4494  fuzzy_perfect 4494 stub_z |
| 22 | `-fno-unique-section-names` | BUILD_FAIL no_unique_section_names /home/debian/GalaxyOnFire2/wt/w3903/src/game/world/Level.cpp:5510:66: warning: implic |
| 23 | `-fno-wchar` | clang++: error: unknown argument: '-fno-wchar' |
| 24 | `-fpack-struct=4` | /home/debian/GalaxyOnFire2/wt/w3903/src/game/mission/Status.h:430:1: error: static_assert failed "Status must be 496 bytes" |
| 25 | `-fpcc-struct-return` | clang++: error: unsupported option '-fpcc-struct-return' for target 'armv7-none-linux-android16' |
| 26 | `-fpie` | /home/debian/GalaxyOnFire2/wt/w3903/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold: er |
| 27 | `-freg-struct-return` | clang++: error: unsupported option '-freg-struct-return' for target 'armv7-none-linux-android16' |
| 28 | `-fsigned-wchar` | clang++: error: unknown argument '-fsigned-wchar', did you mean '-fsigned-char'? |
| 29 | `-ftype-visibility=default` | clang++: error: unknown argument: '-ftype-visibility=default' |
| 30 | `-fuse-register-sized-bitfield-access` | clang++: error: unknown argument: '-fuse-register-sized-bitfield-access' |
| 31 | `-fvisibility-inlines-hidden` | VERIFY_FAIL visinlineshidden compared 4383  avg 99.84%  byte_exact 1278  linked_exact 4203  fuzzy_perfect 4209 stub_zero |
| 32 | `-fvisibility=default` | BUILD_FAIL visdefault In file included from /home/debian/GalaxyOnFire2/wt/w3903/src/game/world/Level.cpp:58: /home/debia |
| 33 | `-fvisibility=hidden` | VERIFY_FAIL vishidden compared 244  avg 99.92%  byte_exact 57  linked_exact 243  fuzzy_perfect 243 stub_zero_size 0  ori |
| 34 | `-fwchar-type=int` | clang++: error: unknown argument: '-fwchar-type=int' |
| 35 | `-fwchar-type=short` | clang++: error: unknown argument: '-fwchar-type=short' |
| 36 | `-fwhole-program-vtables` | clang: error: invalid argument '-fwhole-program-vtables' only allowed with '-flto' |
| 37 | `-mconstructor-aliases` | clang++: error: unknown argument: '-mconstructor-aliases' |
| 38 | `-mdisable-fp-elim` | clang++: error: unknown argument: '-mdisable-fp-elim' |
| 39 | `-mdisable-tail-calls` | clang++: error: unknown argument: '-mdisable-tail-calls' |
| 40 | `-menable-no-infs` | clang++: error: unknown argument: '-menable-no-infs' |
| 41 | `-menable-no-nans` | clang++: error: unknown argument: '-menable-no-nans' |
| 42 | `-menable-unsafe-fp-math` | clang++: error: unknown argument: '-menable-unsafe-fp-math' |
| 43 | `-mfloat-abi=hard` | /home/debian/GalaxyOnFire2/wt/w3903/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold: er |
| 44 | `-mfpu=vfpv3-d16` | /home/debian/GalaxyOnFire2/wt/w3903/.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/7.0.2/include/arm_neon.h:62:24: error: 'neon_v |
| 45 | `-mlimit-float-precision` | clang++: error: unknown argument: '-mlimit-float-precision' |
| 46 | `-mno-constructor-aliases` | clang++: error: unknown argument: '-mno-constructor-aliases' |
| 47 | `-mno-crc` | clang++: error: unknown argument '-mno-crc', did you mean '-mnocrc'? |
| 48 | `-mno-fancy-math-387` | clang++: error: unknown argument: '-mno-fancy-math-387' |
| 49 | `-mno-fp-ret-in-387` | clang++: error: unknown argument: '-mno-fp-ret-in-387' |
| 50 | `-mno-fp16` | clang: error: unknown argument: '-mno-fp16' |
| 51 | `-mno-ldrd` | clang++: error: unknown argument: '-mno-ldrd' |
| 52 | `-mno-movw` | clang++: error: unknown argument '-mno-movw', did you mean '-mno-movt'? |
| 53 | `-mno-neon` | clang++: error: unknown argument: '-mno-neon' |
| 54 | `-mno-neon-for-64bits` | clang++: error: unknown argument: '-mno-neon-for-64bits' |
| 55 | `-mno-strd` | clang: error: unknown argument: '-mno-strd' |
| 56 | `-mno-thumb-interwork` | clang++: error: unknown argument: '-mno-thumb-interwork' |
| 57 | `-mno-zero-initialized-in-bss` | clang++: error: unknown argument '-mno-zero-initialized-in-bss', did you mean '-fno-zero-initialized-in-bss'? |
| 58 | `-mreassociate` | clang++: error: unknown argument: '-mreassociate' |
| 59 | `-msoft-float` | /home/debian/GalaxyOnFire2/wt/w3903/.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/7.0.2/include/arm_neon.h:62:24: error: 'neon_v |
| 60 | `-mstructure-size-boundary=4` | clang++: error: unknown argument: '-mstructure-size-boundary=4' |
| 61 | `-mstructure-size-boundary=8` | clang++: error: unknown argument: '-mstructure-size-boundary=8' |
| 62 | `-mthumb-interwork` | clang++: error: unknown argument: '-mthumb-interwork' |
| 63 | `-mtp=hard` | clang++: error: invalid thread pointer reading mode '-mtp=hard' |

The `-fvisibility=default` row in §3.1 is the retry; the first run failed only
because of the vendor-archive race. The `-fno-common` row in §3.3 is the
unmodified tree; the source-fixed form is in §3.2.

## 4. Per-TU matrix

For every flag that moved a target globally, the flag was applied to the
target's owning TU only. **262 cells, no improvement.** The 15 flags and their
cell summaries:

| flag | cells | min Δ | max Δ | improved | equal | worst cell |
|---|---:|---:|---:|---:|---:|---|
| `-fexperimental-new-pass-manager` | 22 | -59.5 | -1.8 | 0 | 0 | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` 95.2->35.7 (-59.5) |
| `-ffreestanding` | 7 | -34.2 | -1.8 | 0 | 0 | `_ZN10ModStationC1Ev` 98.6->64.4 (-34.2) |
| `-fno-elide-constructors` | 16 | -61.2 | -7.6 | 0 | 0 | `_ZN5MGame8OnUpdateEv` 99.9->38.7 (-61.2) |
| `-fno-pic` | 26 | -67.3 | -1.8 | 0 | 0 | `_ZN7GlobalsC1Ev` 80.5->13.2 (-67.3) |
| `-fsized-deallocation` | 12 | -10.3 | -1.1 | 0 | 0 | `_ZN10ModStationC1Ev` 98.6->88.3 (-10.3) |
| `-fsjlj-exceptions` | 17 | -72.4 | -31.7 | 0 | 0 | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` 95.2->22.8 (-72.4) |
| `-fstrict-vtable-pointers` | 4 | -31.8 | -0.1 | 0 | 0 | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` 87.3->55.5 (-31.8) |
| `-ftrapv` | 23 | -65.4 | -1.0 | 0 | 0 | `_ZN5Level13createMissionEv` 90.7->25.3 (-65.4) |
| `-fvisibility=protected` | 24 | -61.4 | +0.0 | 0 | 3 | `_ZN7GlobalsC1Ev` 80.5->19.1 (-61.4) |
| `-g0` | 5 | -14.9 | -0.1 | 0 | 0 | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` 97.7->82.8 (-14.9) |
| `-marm` | 26 | -91.5 | -42.3 | 0 | 0 | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` 95.2->3.7 (-91.5) |
| `-mexecute-only` | 26 | -40.6 | -2.5 | 0 | 0 | `_ZN5MGame8OnUpdateEv` 99.9->59.3 (-40.6) |
| `-momit-leaf-frame-pointer` | 4 | -41.0 | -6.7 | 0 | 0 | `_ZN7GlobalsC1Ev` 80.5->39.5 (-41.0) |
| `-mrestrict-it` | 24 | -18.7 | -0.1 | 0 | 0 | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` 95.2->76.5 (-18.7) |
| `-mstackrealign` | 26 | -14.4 | -0.1 | 0 | 0 | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` 97.7->83.3 (-14.4) |

The three equal cells are `-fvisibility=protected` on `Status::missionCompleted`,
`SpaceLounge::drawLounge`, and `MenuTouchWindow::OnTouchBegin`; every other
cell is negative. The complete cell list is in Appendix A.

## 5. Additional clang-7 knobs found in `clang -cc1 --help`

The task's candidate list was extended with every remaining plausible
codegen/ABI knob. The second sweep (57 variants) found:

* **Worse:** `-fexperimental-new-pass-manager` (1300/3781, 22 targets moved),
  `-mstackrealign` (80/245, 26), `-momit-leaf-frame-pointer` (1386/4414, 4),
  `-fsized-deallocation` (1347/3871, 12), `-ftrapv` (1276/3864, 23),
  `-ffreestanding` (1350/3986, 7).
* **Neutral:** `-fclang-abi-compat=4.0/5.0/6.0/7.0`, `-fmax-type-align=8/16`,
  `-fnew-alignment=8`, `-fpack-struct=8`, `-ffine-grained-bitfield-accesses`,
  `-fno-fine-grained-bitfield-accesses`, `-fno-lax-vector-conversions`,
  `-fno-preserve-as-comments`, `-fno-trigraphs`, `-fno-experimental-new-pass-manager`,
  `-mincremental-linker-compatible`, `-mtp=soft`, `-mprefer-vector-width=64/128`,
  `-mrelax-all`, `-mstack-probe-size=4096`, `-funique-section-names`,
  `-fdebug-types-section`.
* **Gate-drop without target movement:** `-femit-all-decls` (3396/4498),
  `-fnew-alignment=16` (4496/4496).
* **Unsupported in clang 7.0.2:** `-mconstructor-aliases`,
  `-mno-constructor-aliases`, `-fconst-strings`, `-fno-const-strings`,
  `-fno-math-builtin`, `-mdisable-fp-elim`, `-mdisable-tail-calls`,
  `-mlimit-float-precision`, `-menable-no-nans`, `-menable-no-infs`,
  `-menable-unsafe-fp-math`, `-mreassociate`, `-mtp=hard`,
  `-fno-bitfield-type-align`, `-fuse-register-sized-bitfield-access`,
  `-fsigned-wchar`, `-fno-signed-wchar`, `-fno-wchar`, `-fwchar-type=short/int`,
  `-ftype-visibility=default`, `-mno-zero-initialized-in-bss` (use
  `-fno-zero-initialized-in-bss`).
* **Build failure:** `-fno-unique-section-names` (section-name collision makes
  the NDK `objcopy` segfault in `gen_exidx_markers.py`); `-fpack-struct=4`
  (`static_assert "Status must be 496 bytes"`); `-fpcc-struct-return` /
  `-freg-struct-return` (unsupported for `armv7-none-linux-android16`).

## 6. Excluded flags

### 6.1 Unknown/unsupported in clang 7.0.2 (cannot be the original flag)

```text
-fno-semantic-interposition          -fno-c++-static-destructors
-mno-neon                            -mno-fp16
-mno-thumb-interwork                 -mthumb-interwork
-mno-neon-for-64bits                 -mstructure-size-boundary=4/8
-mno-ldrd                            -mno-strd
-mno-movw                            -mno-crc        (correct: -mnocrc)
-mno-fancy-math-387                  -mno-fp-ret-in-387
-fdebug-compilation-dir=...          -fmacro-prefix-map=...
-ffile-prefix-map=...                -fno-integrated-cc1
-fmerge-functions                    -fno-merge-functions
-mconstructor-aliases                -mno-constructor-aliases
-fconst-strings                      -fno-const-strings
-fno-math-builtin                    -mdisable-fp-elim
-mdisable-tail-calls                 -mlimit-float-precision
-menable-no-nans                     -menable-no-infs
-menable-unsafe-fp-math              -mreassociate
-mtp=hard                            -fno-bitfield-type-align
-fuse-register-sized-bitfield-access -fsigned-wchar
-fno-signed-wchar                    -fno-wchar
-fwchar-type=short/int               -ftype-visibility=default
-mno-zero-initialized-in-bss
```

### 6.2 Build/link/verify failures (not usable as-is)

```text
-flto                         gold/plugin build failure
-fwhole-program-vtables       requires -flto
-fPIE / -fpie                 R_ARM_REL32 link failure
-mfpu=vfpv3-d16               arm_neon.h: NEON not enabled
-msoft-float                  arm_neon.h: NEON not enabled
-mfloat-abi=hard              VFP register arguments vs softfp ABI
-fno-integrated-as            external as does not accept -mfpu=neon
-fgnu89-inline                invalid with C++
-fno-exceptions               Level.cpp:5217 uses try
-fno-cxx-exceptions           Level.cpp:5217 uses try
-fno-threadsafe-statics       verify: 3 missing guard functions
-fvisibility=hidden           hides 4280 functions
-fvisibility-inlines-hidden   hides 141 functions
-fno-common                   duplicate tentative g_android_back_button_pressed
-fno-unique-section-names     objcopy SIGSEGV in gen_exidx_markers.py
-fpack-struct=4               Status static_assert (496 B)
-fpcc-struct-return           unsupported on ARM
-freg-struct-return           unsupported on ARM
```

### 6.3 Neutral negatives (do not re-test)

These are gate-neutral at this tree state and are not expected to change with
source-shape work (they are listed in §3.1):

```text
-DANDROID -DNDEBUG -D_FORTIFY_SOURCE=0/2 -D__STDC_CONSTANT_MACROS
-D__STDC_LIMIT_MACROS -U_FORTIFY_SOURCE -Qn -fcommon
-fdebug-prefix-map=... -fdebug-info-for-profiling -fcxx-exceptions
-fexceptions -fgnu-keywords -fno-gnu-keywords -g -g2 -g3
-fmerge-all-constants -fmessage-length=0 -mglobal-merge -mnocrc -mno-dsp
-mno-execute-only -mno-madd4 -mno-movt -mno-neg-immediates -mno-restrict-it
-mfloat-abi=softfp -mstack-alignment=4/8/16 -mthumb
-fno-access-control -fno-aligned-allocation -fno-assume-sane-operator-new
-fno-asynchronous-unwind-tables -fno-caret-diagnostics -fno-color-diagnostics
-fno-data-sections -fno-debug-info-for-profiling -fno-delayed-template-parsing
-fno-delete-null-pointer-checks -fno-diagnostics-show-option
-fno-dollars-in-identifiers -fno-elide-type -fno-function-sections
-fno-ident -fno-inline-functions-called-once -fno-lto -fno-merge-constants
-fno-omit-frame-pointer -fno-operator-names -fno-plt -fno-rtti-data
-fno-show-column -fno-signed-char -fno-sized-deallocation
-fno-stack-size-section -fno-standalone-debug -fno-strict-vtable-pointers
-fno-unwind-tables (appended) -fno-use-init-array -fno-vectorize
-fno-verbose-asm -fno-whole-program-vtables -fshort-wchar -fstack-size-section
-fstandalone-debug -fstrict-return -fuse-init-array
```

### 6.4 Worse (do not re-test)

```text
-fdata-sections -fdwarf-exceptions -ffast-math -g0 -g1 -gline-tables-only -gmlt
-marm -mexecute-only -mlong-calls -mno-global-merge -mno-implicit-float
-mno-thumb -mno-unaligned-access -mrestrict-it -mthread-model single
-fno-PIC -fno-PIE -fno-builtin -fno-elide-constructors -fno-inline
-fno-inline-functions -fno-jump-tables -fno-math-errno -fno-pic -fno-pie
-fno-rtti -fno-optimize-sibling-calls -fno-slp-vectorize -fno-stack-protector
-fno-strict-aliasing -fno-strict-overflow -fno-unroll-loops
-fno-use-cxa-atexit -fno-zero-initialized-in-bss -fomit-frame-pointer
-fshort-enums -fsigned-char -fsjlj-exceptions -fstack-protector
-fstack-protector-all -fstrict-enums -fstrict-vtable-pointers
-fvisibility=protected -fwrapv -femit-all-decls -ffreestanding
-momit-leaf-frame-pointer -mstackrealign -fnew-alignment=16
-fexperimental-new-pass-manager -fsized-deallocation -ftrapv
-fno-unwind-tables (after -funwind-tables) -fno-function-sections (after
-ffunction-sections) -fno-common (+extern fix)
```

## 7. Interpretation

A flag-driven gap would show at least one of:

1. a flag that raises `byte_exact` or `linked_exact` without dropping the gate;
2. a target whose fuzzy rises under some flag;
3. a systematic fingerprint mismatch (frame pointer, stack canary, NEON,
   jump table, `movw`/`movt`, sized new/delete, guard variable) across a TU.

None is present:

* No variant beats 4498/4498/4524; no target's fuzzy rises (max Δ = +0.0).
* The per-TU matrix finds no local win either: 262 cells, 0 improved, 3 equal.
* The frame-pointer and stack-canary fingerprints are already exact (w3751
  §4/§7); `-fomit-frame-pointer`, `-fno-stack-protector`, `-fstack-protector`,
  and `-fstack-protector-all` are all worse.
* The 26 functions are all size-exact; their diffs are local register
  allocation, scheduling, encoding and block order.

Therefore **all 26 remaining functions are source-shape-driven**. The current
global flag set (including `-Oz`, `-g`, `-fPIC`, `-frtti`,
`-fstack-protector-strong`, strict aliasing, frame pointers, vectorization,
inlining, threadsafe statics and `-fcommon` default) already reproduces the
original's codegen; every non-neutral alternative is worse. The remaining gap
is the known source/TU-shape queue: expression order, local lifetimes,
statement order, TU membership and the scoped `-mllvm` compensations.

## 8. Reproduction commands

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w3903
S=/dev/shm/w3903
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
OBJDUMP=$ROOT/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump

# baseline
cd $ROOT
cmake --build cmake-build-match --target check -j8

# one global variant (repeat for each row of §3)
cmake -S $ROOT -B $S/builds/<id> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$S/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=$O \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build $S/builds/<id> --target link_so -j16
python3 $ROOT/tools/verify/verify.py --build-dir $S/builds/<id>/verify \
  --original $O --ours $S/builds/<id>/libgof2hdaa.so \
  --objdump $OBJDUMP --fail-on-wrong-type --top 0

# order-correct negative unwind/function-section variants
cmake -S $ROOT -B $S/builds/<id> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$S/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS= \
  -DGOF2_ORIGINAL_SO=$O \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DCMAKE_PROJECT_INCLUDE=$S/global_compile_flags.cmake \
  -DGOF2_GLOBAL_COMPILE_FLAGS='-fno-unwind-tables'
cmake --build $S/builds/<id> --target link_so -j16
python3 $ROOT/tools/verify/verify.py --build-dir $S/builds/<id>/verify \
  --original $O --ours $S/builds/<id>/libgof2hdaa.so \
  --objdump $OBJDUMP --fail-on-wrong-type --top 0

# one per-TU cell
cmake -S $ROOT -B $S/builds/<id> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$S/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS= \
  -DGOF2_ORIGINAL_SO=$O \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DCMAKE_PROJECT_INCLUDE=$S/per_tu_flags.cmake \
  -DGOF2_AB_TU='game/ui/Layout.cpp' -DGOF2_AB_FLAGS='-g0'
cmake --build $S/builds/<id> --target link_so -j16
python3 $ROOT/tools/verify/verify.py --build-dir $S/builds/<id>/verify \
  --original $O --ours $S/builds/<id>/libgof2hdaa.so \
  --objdump $OBJDUMP --fail-on-wrong-type --top 0
```

## Appendix A: complete per-TU cell list

`base` is the unmodified fuzzy %, `per-TU` is the owning TU with the flag
applied. Every Δ is ≤ 0.

| flag | target | TU | base % | per-TU % | Δ | size (orig/ours) |
|---|---|---|---:|---:|---:|---|
| `-fexperimental-new-pass-manager` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 35.7 | -59.5 | 8132/8220 |
| `-fexperimental-new-pass-manager` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 32.8 | -41.1 | 9908/10176 |
| `-fexperimental-new-pass-manager` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 53.9 | -36.8 | 17172/17572 |
| `-fexperimental-new-pass-manager` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 63.5 | -35.6 | 2644/2716 |
| `-fexperimental-new-pass-manager` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 42.2 | -31.4 | 1996/2016 |
| `-fexperimental-new-pass-manager` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 69.4 | -30.5 | 11440/11368 |
| `-fexperimental-new-pass-manager` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 22.0 | -25.7 | 7072/7096 |
| `-fexperimental-new-pass-manager` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 22.0 | -25.7 | 7072/7096 |
| `-fexperimental-new-pass-manager` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 54.9 | -25.7 | 9700/9724 |
| `-fexperimental-new-pass-manager` | `_ZN6Status16missionCompletedEbbx` | `game/mission/Status.cpp` | 57.7 | 35.7 | -22.0 | 1432/1460 |
| `-fexperimental-new-pass-manager` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 76.5 | -21.2 | 1896/1988 |
| `-fexperimental-new-pass-manager` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 61.9 | -20.8 | 1984/2000 |
| `-fexperimental-new-pass-manager` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 59.4 | -19.0 | 8080/8188 |
| `-fexperimental-new-pass-manager` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 71.2 | -17.1 | 2252/2268 |
| `-fexperimental-new-pass-manager` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 82.4 | -13.2 | 1948/1960 |
| `-fexperimental-new-pass-manager` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 43.5 | -11.5 | 57232/57440 |
| `-fexperimental-new-pass-manager` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 76.9 | -10.4 | 968/984 |
| `-fexperimental-new-pass-manager` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 65.8 | -8.3 | 2764/2772 |
| `-fexperimental-new-pass-manager` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 87.7 | -6.2 | 9600/9604 |
| `-fexperimental-new-pass-manager` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 87.7 | -6.2 | 9600/9604 |
| `-fexperimental-new-pass-manager` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 82.5 | -4.4 | 1908/1920 |
| `-fexperimental-new-pass-manager` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 87.0 | -1.8 | 9332/9328 |
| `-ffreestanding` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 64.4 | -34.2 | 1008/1040 |
| `-ffreestanding` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 64.4 | -34.2 | 1008/1040 |
| `-ffreestanding` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 82.9 | -5.4 | 2252/2260 |
| `-ffreestanding` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 86.9 | -3.8 | 17172/17300 |
| `-ffreestanding` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 91.5 | -2.4 | 9600/9600 |
| `-ffreestanding` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 91.5 | -2.4 | 9600/9600 |
| `-ffreestanding` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 76.6 | -1.8 | 8080/8088 |
| `-fno-elide-constructors` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 38.7 | -61.2 | 11440/11916 |
| `-fno-elide-constructors` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 47.1 | -48.5 | 1948/2752 |
| `-fno-elide-constructors` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 54.3 | -32.6 | 1908/1876 |
| `-fno-elide-constructors` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 62.7 | -28.0 | 17172/17244 |
| `-fno-elide-constructors` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 62.4 | -24.9 | 968/1032 |
| `-fno-elide-constructors` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 70.8 | -24.4 | 8132/8260 |
| `-fno-elide-constructors` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 52.6 | -21.3 | 9908/10224 |
| `-fno-elide-constructors` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 60.4 | -20.2 | 9700/10012 |
| `-fno-elide-constructors` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 64.5 | -18.2 | 1984/1996 |
| `-fno-elide-constructors` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 77.1 | -16.8 | 9600/9680 |
| `-fno-elide-constructors` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 77.1 | -16.8 | 9600/9680 |
| `-fno-elide-constructors` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 82.7 | -16.4 | 2644/2692 |
| `-fno-elide-constructors` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 72.5 | -16.3 | 9332/9492 |
| `-fno-elide-constructors` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 64.6 | -13.8 | 8080/8204 |
| `-fno-elide-constructors` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 42.3 | -12.7 | 57232/57740 |
| `-fno-elide-constructors` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 66.0 | -7.6 | 1996/2052 |
| `-fno-pic` | `_ZN7GlobalsC1Ev` | `game/core/Globals.cpp` | 80.5 | 13.2 | -67.3 | 612/316 |
| `-fno-pic` | `_ZN7GlobalsC2Ev` | `game/core/Globals.cpp` | 80.5 | 13.2 | -67.3 | 612/316 |
| `-fno-pic` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 44.1 | -51.1 | 8132/7640 |
| `-fno-pic` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 51.9 | -48.0 | 11440/11036 |
| `-fno-pic` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 57.0 | -41.6 | 1008/952 |
| `-fno-pic` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 57.0 | -41.6 | 1008/952 |
| `-fno-pic` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 44.1 | -36.5 | 9700/9404 |
| `-fno-pic` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 61.3 | -34.3 | 1948/1824 |
| `-fno-pic` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 56.2 | -32.6 | 9332/9128 |
| `-fno-pic` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 68.9 | -30.2 | 2644/2568 |
| `-fno-pic` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 60.7 | -30.0 | 17172/17116 |
| `-fno-pic` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 69.3 | -28.4 | 1896/1796 |
| `-fno-pic` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 61.3 | -27.0 | 2252/2152 |
| `-fno-pic` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 67.8 | -26.1 | 9600/9508 |
| `-fno-pic` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 67.8 | -26.1 | 9600/9508 |
| `-fno-pic` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 22.7 | -25.0 | 7072/6952 |
| `-fno-pic` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 22.7 | -25.0 | 7072/6952 |
| `-fno-pic` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 66.1 | -21.2 | 968/936 |
| `-fno-pic` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 36.5 | -18.5 | 57232/55864 |
| `-fno-pic` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 60.9 | -17.5 | 8080/7620 |
| `-fno-pic` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 56.9 | -16.7 | 1996/1976 |
| `-fno-pic` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 59.1 | -14.8 | 9908/9816 |
| `-fno-pic` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 59.8 | -14.3 | 2764/2748 |
| `-fno-pic` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 73.5 | -13.4 | 1908/1892 |
| `-fno-pic` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 72.0 | -10.7 | 1984/1956 |
| `-fno-pic` | `_ZN6Status16missionCompletedEbbx` | `game/mission/Status.cpp` | 57.7 | 55.9 | -1.8 | 1432/1408 |
| `-fsized-deallocation` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 88.3 | -10.3 | 1008/1016 |
| `-fsized-deallocation` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 88.3 | -10.3 | 1008/1016 |
| `-fsized-deallocation` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 85.1 | -10.1 | 8132/8144 |
| `-fsized-deallocation` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 90.4 | -9.5 | 11440/11472 |
| `-fsized-deallocation` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 65.6 | -8.5 | 2764/2776 |
| `-fsized-deallocation` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 81.8 | -6.5 | 2252/2276 |
| `-fsized-deallocation` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 71.9 | -6.5 | 8080/8116 |
| `-fsized-deallocation` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 91.2 | -2.7 | 9600/9628 |
| `-fsized-deallocation` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 91.2 | -2.7 | 9600/9628 |
| `-fsized-deallocation` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 52.9 | -2.1 | 57232/57240 |
| `-fsized-deallocation` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 71.6 | -2.0 | 1996/2000 |
| `-fsized-deallocation` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 89.6 | -1.1 | 17172/17232 |
| `-fsjlj-exceptions` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 22.8 | -72.4 | 8132/9948 |
| `-fsjlj-exceptions` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 29.7 | -65.9 | 1948/2504 |
| `-fsjlj-exceptions` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 28.3 | -65.6 | 9600/11904 |
| `-fsjlj-exceptions` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 28.3 | -65.6 | 9600/11904 |
| `-fsjlj-exceptions` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 33.6 | -65.0 | 1008/1244 |
| `-fsjlj-exceptions` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 33.6 | -65.0 | 1008/1244 |
| `-fsjlj-exceptions` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 30.3 | -60.4 | 17172/21404 |
| `-fsjlj-exceptions` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 29.4 | -58.9 | 2252/2900 |
| `-fsjlj-exceptions` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 41.0 | -58.1 | 2644/3148 |
| `-fsjlj-exceptions` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 32.9 | -55.9 | 9332/11608 |
| `-fsjlj-exceptions` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 45.3 | -54.6 | 11440/14452 |
| `-fsjlj-exceptions` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 23.4 | -50.7 | 2764/3524 |
| `-fsjlj-exceptions` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 33.2 | -47.4 | 9700/11908 |
| `-fsjlj-exceptions` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 30.8 | -43.1 | 9908/11632 |
| `-fsjlj-exceptions` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 36.7 | -41.7 | 8080/9904 |
| `-fsjlj-exceptions` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 36.7 | -36.9 | 1996/2480 |
| `-fsjlj-exceptions` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 23.3 | -31.7 | 57232/67356 |
| `-fstrict-vtable-pointers` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 55.5 | -31.8 | 968/948 |
| `-fstrict-vtable-pointers` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 92.7 | -2.5 | 8132/8128 |
| `-fstrict-vtable-pointers` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 54.8 | -0.2 | 57232/57260 |
| `-fstrict-vtable-pointers` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 80.5 | -0.1 | 9700/9700 |
| `-ftrapv` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 25.3 | -65.4 | 17172/18664 |
| `-ftrapv` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 31.9 | -63.7 | 1948/2104 |
| `-ftrapv` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 27.1 | -53.5 | 9700/10836 |
| `-ftrapv` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 22.8 | -51.1 | 9908/10884 |
| `-ftrapv` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 37.8 | -51.0 | 9332/9752 |
| `-ftrapv` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 43.1 | -50.8 | 9600/11024 |
| `-ftrapv` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 43.1 | -50.8 | 9600/11024 |
| `-ftrapv` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 50.9 | -46.8 | 1896/2280 |
| `-ftrapv` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 50.4 | -44.8 | 8132/8216 |
| `-ftrapv` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 59.5 | -40.4 | 11440/12076 |
| `-ftrapv` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 44.5 | -33.9 | 8080/8084 |
| `-ftrapv` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 42.0 | -31.6 | 1996/2136 |
| `-ftrapv` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 67.2 | -31.4 | 1008/1024 |
| `-ftrapv` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 67.2 | -31.4 | 1008/1024 |
| `-ftrapv` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 60.0 | -28.3 | 2252/2332 |
| `-ftrapv` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 71.4 | -27.7 | 2644/2720 |
| `-ftrapv` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 37.3 | -17.7 | 57232/64220 |
| `-ftrapv` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 71.6 | -15.3 | 1908/1932 |
| `-ftrapv` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 72.4 | -10.3 | 1984/2032 |
| `-ftrapv` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 65.5 | -8.6 | 2764/2856 |
| `-ftrapv` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 79.3 | -8.0 | 968/980 |
| `-ftrapv` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 46.7 | -1.0 | 7072/7104 |
| `-ftrapv` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 46.7 | -1.0 | 7072/7104 |
| `-fvisibility=protected` | `_ZN7GlobalsC1Ev` | `game/core/Globals.cpp` | 80.5 | 19.1 | -61.4 | 612/300 |
| `-fvisibility=protected` | `_ZN7GlobalsC2Ev` | `game/core/Globals.cpp` | 80.5 | 19.1 | -61.4 | 612/300 |
| `-fvisibility=protected` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 79.4 | -11.3 | 17172/17156 |
| `-fvisibility=protected` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 90.1 | -8.5 | 1008/1000 |
| `-fvisibility=protected` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 90.1 | -8.5 | 1008/1000 |
| `-fvisibility=protected` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 86.4 | -1.9 | 2252/2252 |
| `-fvisibility=protected` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 92.3 | -1.6 | 9600/9600 |
| `-fvisibility=protected` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 92.3 | -1.6 | 9600/9600 |
| `-fvisibility=protected` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 98.5 | -1.4 | 11440/11440 |
| `-fvisibility=protected` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 77.6 | -0.8 | 8080/8080 |
| `-fvisibility=protected` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 98.3 | -0.8 | 2644/2644 |
| `-fvisibility=protected` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 73.4 | -0.7 | 2764/2764 |
| `-fvisibility=protected` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 80.0 | -0.6 | 9700/9700 |
| `-fvisibility=protected` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 86.7 | -0.6 | 968/968 |
| `-fvisibility=protected` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 94.7 | -0.5 | 8132/8132 |
| `-fvisibility=protected` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 88.3 | -0.5 | 9332/9332 |
| `-fvisibility=protected` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 73.4 | -0.5 | 9908/9908 |
| `-fvisibility=protected` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 73.2 | -0.4 | 1996/1996 |
| `-fvisibility=protected` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 54.8 | -0.2 | 57232/57232 |
| `-fvisibility=protected` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 86.7 | -0.2 | 1908/1908 |
| `-fvisibility=protected` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 82.6 | -0.1 | 1984/1984 |
| `-fvisibility=protected` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 95.6 | +0.0 | 1948/1948 |
| `-fvisibility=protected` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 97.7 | +0.0 | 1896/1896 |
| `-fvisibility=protected` | `_ZN6Status16missionCompletedEbbx` | `game/mission/Status.cpp` | 57.7 | 57.7 | +0.0 | 1432/1432 |
| `-g0` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 82.8 | -14.9 | 1896/1892 |
| `-g0` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 98.7 | -0.4 | 2644/2644 |
| `-g0` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 87.0 | -0.3 | 968/968 |
| `-g0` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 78.2 | -0.2 | 8080/8080 |
| `-g0` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 73.5 | -0.1 | 1996/1996 |
| `-marm` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 3.7 | -91.5 | 8132/11696 |
| `-marm` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 12.8 | -87.1 | 11440/15600 |
| `-marm` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 11.0 | -86.7 | 1896/2860 |
| `-marm` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 10.9 | -83.0 | 9600/13984 |
| `-marm` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 10.9 | -83.0 | 9600/13984 |
| `-marm` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 6.6 | -81.7 | 2252/3192 |
| `-marm` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 10.2 | -80.5 | 17172/24256 |
| `-marm` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 9.0 | -79.8 | 9332/14348 |
| `-marm` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 23.4 | -75.2 | 1008/1444 |
| `-marm` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 23.4 | -75.2 | 1008/1444 |
| `-marm` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 20.8 | -74.8 | 1948/3056 |
| `-marm` | `_ZN7GlobalsC1Ev` | `game/core/Globals.cpp` | 80.5 | 6.9 | -73.6 | 612/732 |
| `-marm` | `_ZN7GlobalsC2Ev` | `game/core/Globals.cpp` | 80.5 | 6.9 | -73.6 | 612/732 |
| `-marm` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 4.9 | -73.5 | 8080/10952 |
| `-marm` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 26.4 | -72.7 | 2644/3640 |
| `-marm` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 9.2 | -71.4 | 9700/15116 |
| `-marm` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 17.4 | -69.9 | 968/1416 |
| `-marm` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 19.7 | -67.2 | 1908/2484 |
| `-marm` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 6.9 | -67.2 | 2764/3980 |
| `-marm` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 12.1 | -61.8 | 9908/14352 |
| `-marm` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 14.1 | -59.5 | 1996/2920 |
| `-marm` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 26.2 | -56.5 | 1984/2600 |
| `-marm` | `_ZN6Status16missionCompletedEbbx` | `game/mission/Status.cpp` | 57.7 | 3.5 | -54.2 | 1432/2048 |
| `-marm` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 10.0 | -45.0 | 57232/79360 |
| `-marm` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 5.4 | -42.3 | 7072/8144 |
| `-marm` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 5.4 | -42.3 | 7072/8144 |
| `-mexecute-only` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 59.3 | -40.6 | 11440/11436 |
| `-mexecute-only` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 39.5 | -34.6 | 2764/2980 |
| `-mexecute-only` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 62.7 | -28.0 | 17172/17548 |
| `-mexecute-only` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 20.6 | -27.1 | 7072/6248 |
| `-mexecute-only` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 20.6 | -27.1 | 7072/6248 |
| `-mexecute-only` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 56.9 | -23.7 | 9700/9744 |
| `-mexecute-only` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 77.9 | -20.7 | 1008/992 |
| `-mexecute-only` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 77.9 | -20.7 | 1008/992 |
| `-mexecute-only` | `_ZN7GlobalsC1Ev` | `game/core/Globals.cpp` | 80.5 | 61.9 | -18.6 | 612/604 |
| `-mexecute-only` | `_ZN7GlobalsC2Ev` | `game/core/Globals.cpp` | 80.5 | 61.9 | -18.6 | 612/604 |
| `-mexecute-only` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 78.6 | -17.0 | 1948/1976 |
| `-mexecute-only` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 82.2 | -15.5 | 1896/1936 |
| `-mexecute-only` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 81.0 | -14.2 | 8132/8368 |
| `-mexecute-only` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 86.8 | -12.3 | 2644/2656 |
| `-mexecute-only` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 77.7 | -9.6 | 968/972 |
| `-mexecute-only` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 64.7 | -9.2 | 9908/9924 |
| `-mexecute-only` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 78.0 | -8.9 | 1908/1940 |
| `-mexecute-only` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 75.9 | -6.8 | 1984/1984 |
| `-mexecute-only` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 71.8 | -6.6 | 8080/8092 |
| `-mexecute-only` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 82.6 | -6.2 | 9332/9392 |
| `-mexecute-only` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 87.8 | -6.1 | 9600/9648 |
| `-mexecute-only` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 87.8 | -6.1 | 9600/9648 |
| `-mexecute-only` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 67.9 | -5.7 | 1996/2084 |
| `-mexecute-only` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 50.0 | -5.0 | 57232/58820 |
| `-mexecute-only` | `_ZN6Status16missionCompletedEbbx` | `game/mission/Status.cpp` | 57.7 | 54.5 | -3.2 | 1432/1540 |
| `-mexecute-only` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 85.8 | -2.5 | 2252/2256 |
| `-momit-leaf-frame-pointer` | `_ZN7GlobalsC1Ev` | `game/core/Globals.cpp` | 80.5 | 39.5 | -41.0 | 612/596 |
| `-momit-leaf-frame-pointer` | `_ZN7GlobalsC2Ev` | `game/core/Globals.cpp` | 80.5 | 39.5 | -41.0 | 612/596 |
| `-momit-leaf-frame-pointer` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 41.0 | -6.7 | 7072/7016 |
| `-momit-leaf-frame-pointer` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 41.0 | -6.7 | 7072/7016 |
| `-mrestrict-it` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 76.5 | -18.7 | 8132/8108 |
| `-mrestrict-it` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 82.4 | -15.3 | 1896/1888 |
| `-mrestrict-it` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 77.5 | -11.3 | 9332/9368 |
| `-mrestrict-it` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 88.7 | -11.2 | 11440/11412 |
| `-mrestrict-it` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 78.3 | -10.0 | 2252/2244 |
| `-mrestrict-it` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 89.8 | -8.8 | 1008/1000 |
| `-mrestrict-it` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 89.8 | -8.8 | 1008/1000 |
| `-mrestrict-it` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 70.3 | -8.1 | 8080/8076 |
| `-mrestrict-it` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 78.9 | -8.0 | 1908/1916 |
| `-mrestrict-it` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 76.1 | -6.6 | 1984/1972 |
| `-mrestrict-it` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 92.6 | -6.5 | 2644/2648 |
| `-mrestrict-it` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 68.8 | -5.1 | 9908/9944 |
| `-mrestrict-it` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 89.2 | -4.7 | 9600/9580 |
| `-mrestrict-it` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 89.2 | -4.7 | 9600/9580 |
| `-mrestrict-it` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 86.3 | -4.4 | 17172/17160 |
| `-mrestrict-it` | `_ZN6Status16missionCompletedEbbx` | `game/mission/Status.cpp` | 57.7 | 53.4 | -4.3 | 1432/1416 |
| `-mrestrict-it` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 76.3 | -4.3 | 9700/9704 |
| `-mrestrict-it` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 69.7 | -3.9 | 1996/1996 |
| `-mrestrict-it` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 83.8 | -3.5 | 968/968 |
| `-mrestrict-it` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 71.4 | -2.7 | 2764/2760 |
| `-mrestrict-it` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 93.8 | -1.8 | 1948/1948 |
| `-mrestrict-it` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 53.3 | -1.7 | 57232/57244 |
| `-mrestrict-it` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 47.6 | -0.1 | 7072/7032 |
| `-mrestrict-it` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 47.6 | -0.1 | 7072/7032 |
| `-mstackrealign` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `game/ui/MenuTouchWindow.cpp` | 97.7 | 83.3 | -14.4 | 1896/1908 |
| `-mstackrealign` | `_ZN5MGame8OnUpdateEv` | `game/menu/MGame.cpp` | 99.9 | 87.5 | -12.4 | 11440/11452 |
| `-mstackrealign` | `_ZN7StarMap4drawEv` | `game/world/StarMap.cpp` | 99.1 | 88.6 | -10.5 | 2644/2660 |
| `-mstackrealign` | `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | `game/ui/MenuTouchWindow.cpp` | 95.2 | 86.0 | -9.2 | 8132/8144 |
| `-mstackrealign` | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | `game/ship/PlayerEgo.cpp` | 87.3 | 78.2 | -9.1 | 968/980 |
| `-mstackrealign` | `_ZN11SpaceLounge10drawLoungeEv` | `game/menu/SpaceLounge.cpp` | 95.6 | 86.6 | -9.0 | 1948/1960 |
| `-mstackrealign` | `_ZN7GlobalsC1Ev` | `game/core/Globals.cpp` | 80.5 | 71.6 | -8.9 | 612/628 |
| `-mstackrealign` | `_ZN7GlobalsC2Ev` | `game/core/Globals.cpp` | 80.5 | 71.6 | -8.9 | 612/628 |
| `-mstackrealign` | `_ZN5Level10createShipEiiiP8Waypointbb` | `game/world/Level.cpp` | 74.1 | 65.4 | -8.7 | 2764/2776 |
| `-mstackrealign` | `_ZN5Level11createSceneEv` | `game/world/Level.cpp` | 88.3 | 80.1 | -8.2 | 2252/2272 |
| `-mstackrealign` | `_ZN9RocketGun6updateEi` | `game/weapons/RocketGun.cpp` | 86.9 | 80.2 | -6.7 | 1908/1920 |
| `-mstackrealign` | `_ZN3Gun22calcCharacterCollisionEv` | `game/weapons/Gun.cpp` | 82.7 | 76.2 | -6.5 | 1984/1996 |
| `-mstackrealign` | `_ZN5MGame10OnTouchEndEiiPv` | `game/menu/MGame.cpp` | 78.4 | 72.0 | -6.4 | 8080/8132 |
| `-mstackrealign` | `_ZN10ModStationC1Ev` | `game/menu/ModStation.cpp` | 98.6 | 93.4 | -5.2 | 1008/1016 |
| `-mstackrealign` | `_ZN10ModStationC2Ev` | `game/menu/ModStation.cpp` | 98.6 | 93.4 | -5.2 | 1008/1016 |
| `-mstackrealign` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `game/ui/Hud.cpp` | 88.8 | 83.8 | -5.0 | 9332/9348 |
| `-mstackrealign` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `game/mission/Generator.cpp` | 73.6 | 69.6 | -4.0 | 1996/2012 |
| `-mstackrealign` | `_ZN15MenuTouchWindowC1Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 90.1 | -3.8 | 9600/9616 |
| `-mstackrealign` | `_ZN15MenuTouchWindowC2Ei` | `game/ui/MenuTouchWindow.cpp` | 93.9 | 90.1 | -3.8 | 9600/9616 |
| `-mstackrealign` | `_ZN5Level13createMissionEv` | `game/world/Level.cpp` | 90.7 | 87.3 | -3.4 | 17172/17188 |
| `-mstackrealign` | `_ZN5Radar4drawEP6PlayerP3Hudi` | `game/weapons/Radar.cpp` | 73.9 | 71.5 | -2.4 | 9908/9920 |
| `-mstackrealign` | `_ZN12HangarWindow6renderEv` | `game/ui/HangarWindow.cpp` | 80.6 | 78.5 | -2.1 | 9700/9712 |
| `-mstackrealign` | `_ZN6Status16missionCompletedEbbx` | `game/mission/Status.cpp` | 57.7 | 56.3 | -1.4 | 1432/1444 |
| `-mstackrealign` | `_ZN6LayoutC1Ev` | `game/ui/Layout.cpp` | 47.7 | 47.4 | -0.3 | 7072/7088 |
| `-mstackrealign` | `_ZN6LayoutC2Ev` | `game/ui/Layout.cpp` | 47.7 | 47.4 | -0.3 | 7072/7088 |
| `-mstackrealign` | `_ZN11LevelScript7processEi` | `game/world/LevelScript.cpp` | 55.0 | 54.9 | -0.1 | 57232/57244 |
