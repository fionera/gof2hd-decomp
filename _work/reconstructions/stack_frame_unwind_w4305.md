# Stack protector, frame pointer and unwind-table provenance (w4305, wave 93)

Status: research note, 2026-11-03. Worktree `wt/w4305`, branch `wave93-w4305`, base
`eea2e3c1`. Scratch is `/dev/shm/w4305`. **No `src/` or build-flag change is landed**:
no tested variant strictly improves the gate. Baseline and final gate are identical
(§1). The note is the deliverable; the full 756-function canary table is committed
alongside as `stack_frame_unwind_w4305_canary.csv`.

## 0. Bottom line

The original's stack-protector / frame-pointer / unwind-table flags are **uniform
across every TU**, and the tree's current settings are the only ones that reproduce
the binary's fingerprints:

* **`-fstack-protector-strong` — proven.** The original has a canary in **756** of
  the 4524 compared functions. Our `-fstack-protector-strong` build has a canary in
  exactly the same 756 functions (0 presence mismatches), and the canary value
  register is identical for all 756 (r0 605, r1 78, r2 36, r4 14, r3 12, r5 4,
  r6 2, 5 undecoded in both). `-fstack-protector` (basic) protects only 25
  functions (21 vendored-lib + 4 app), `-fstack-protector-all` protects 3835, and
  `-fno-stack-protector` protects 21 (all vendored-lib). Each is 731 / 3079 / 735
  canary-presence mismatches and drops `byte_exact` to 1377 / 91 / 1377.
* **Frame pointers are kept — proven; the default is already correct.**
  The original has a frame pointer in **2712** of 4524 functions; our build has
  exactly the same 2712 (0 mismatches). `-fno-omit-frame-pointer`,
  `-mno-omit-leaf-frame-pointer` and `-Xclang -mdisable-fp-elim` all produce
  **byte-identical `.text`** to the baseline (sha `07cc0465…`), i.e. the tree's
  default is already the original's setting. `-fomit-frame-pointer` leaves only
  131 frame pointers (2581 mismatches, `byte_exact` 1298); `-momit-leaf-frame-pointer`
  leaves 2625 (87 mismatches, `byte_exact` 1386).
* **`-funwind-tables` — proven.** The original `.ARM.exidx` has **3566 entries**
  (272 `CANTUNWIND`, 3294 extab) and the entry-target and entry-kind sequences are
  **identical** to our build (0 target diffs, 0 kind diffs). Removing
  `-funwind-tables` (by filtering it out of `target_compile_options`, not by
  appending `-fno-unwind-tables`) leaves 3279 entries (698 CANT, 2581 extab),
  3275 target diffs, and drops `byte_exact` 4499 -> 2878.
  `-fno-unwind-tables` / `-fno-asynchronous-unwind-tables` appended to
  `CMAKE_CXX_FLAGS` are **neutral because of the flag-order trap** (§4.2);
  `-fasynchronous-unwind-tables` is an alias of `-funwind-tables` in clang 7.0.2.
* **No per-TU divergence.** Mapping every compared function to its object file
  gives 192 TUs (157 with at least one canary); the original and our build have
  **0 canary, 0 frame-pointer and 0 exidx-kind mismatches in every TU**. A per-TU
  A/B spot check (§6) reproduces the global per-target delta exactly.
* **clang 7.0.2 has no `-fstack-protector-explicit`.** It is rejected by the
  driver (`unknown argument`), and the cc1 `-stack-protector` levels are
  0=none, 1=basic, 2=strong, 3=all — there is no explicit level. `-mllvm
  -frame-pointer` and `-mllvm -enable-frame-pointer` are also rejected by clang
  7.0.2 (`Unknown command line argument`). None of the three can be an original
  flag.

## 1. Baseline and method

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w4305
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Before and after this note:

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` 4524/4524; lint `CLEAN ✓`; drift 0. Section sizes (ours == original):
`.text 0x1337c8 @ 0x69440`, `.rodata 0x41b67 @ 0x1c9e10`,
`.ARM.exidx 0x6f70 @ 0x19cc08`, `.ARM.extab 0x26298 @ 0x1a3b78`,
`.data 0x296c`, `.bss 0x69ac`. Baseline `.so` sha256
`d4a8902128daf3818442455b1779f63f2ab4e5669f85bed4928a1bc11b72ac34`;
ground truth `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.
(The build-id is a hash of the unstripped link output, which embeds the build
directory in DWARF, so sha equality is only meaningful within one build dir.)

Each variant gets its own build dir `/dev/shm/w4305/build-<tag>` and verify dir
`/dev/shm/w4305/verify-<tag>`. A scratch toolchain includes the real
`cmake/toolchain.cmake` and appends the candidate flag to both C and C++ flags:

```cmake
# /dev/shm/w4305/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w4305/cmake/toolchain.cmake")
if(GOF2_EXTRA_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_CXX_FLAGS}" CACHE STRING "" FORCE)
endif()
if(GOF2_EXTRA_C_FLAGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GOF2_EXTRA_C_FLAGS}" CACHE STRING "" FORCE)
endif()
```

For the "filtered" unwind variants a compiler wrapper removes exact `-funwind-tables`
tokens before invoking the real clang (the flag is added by
`target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)`, so a
naive append cannot cancel it):

```python
# /dev/shm/w4305/clangxx_filter.py
REAL = ".../android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++"
FILTER = set(os.environ.get("GOF2_FILTER_FLAGS", "").split())
args = [a for a in sys.argv[1:] if a not in FILTER]
os.execv(REAL, [REAL] + args)
```

The measurement is always the official verifier:

```sh
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir /dev/shm/w4305/verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4305/build-<tag>/libgof2hdaa.so \
  --objdump "$OBJ" --fail-on-wrong-type --top 0
```

## 2. Canary classification (positive evidence for `-fstack-protector-strong`)

### 2.1 Method

For every function in the verifier's 4524-row report, read its `st_value`/`st_size`
from `.dynsym` (clearing the ARM Thumb bit), disassemble exactly
`[value, value+size)` with the NDK objdump, and classify:

* **canary**: the function contains a call/branch to `__stack_chk_fail@plt`.
* **canary value register**: the register `rC` in the prologue idiom

  ```text
  ldr  rA, [pc, #imm]      ; GOT-relative literal
  add  rA, pc              ; rA = &GOT[__stack_chk_guard]
  ldr  rB, [rA, #0]        ; rB = &__stack_chk_guard
  ldr  rC, [rB, #0]        ; rC = __stack_chk_guard
  str  rC, [sp, #off]      ; canary slot
  ```

  followed by the epilogue `ldr rC,[sp,#off]` + compare + `blx __stack_chk_fail`.
* **guard pointer register**: `rB` above.
* **canary stack offset**: `off` above.
* **frame pointer**: an `add r7, sp, #imm` / `mov r7, sp` in the first 12
  instructions.
* **exidx kind**: the `.ARM.exidx` entry for the function's address is
  `CANTUNWIND` (word1 == 1) or `extab`.

The classifier is `/dev/shm/w4305/classify_full.py`; it also parses the
`.ARM.exidx` section directly (8-byte entries, prel31-decoded) and compares the
entry-target and entry-kind sequences. The full per-function canary table is
committed as `stack_frame_unwind_w4305_canary.csv`.

### 2.2 Result: the canary sets are identical

| metric | original | ours (`-fstack-protector-strong`) |
|---|---:|---:|
| compared functions | 4524 | 4524 |
| functions with a canary | **756** | **756** |
| canary presence mismatches | — | **0** |
| canary value register distribution | r0 605, r1 78, r2 36, r4 14, r3 12, r5 4, r6 2, undecoded 5 | **identical** |
| guard pointer register distribution | r5 165, r6 111, r8 99, r0 92, r9 86, sl 69, r4 60, fp 36, r1 26, r2 5, lr 1, r3 1, undecoded 5 | r5 165, r6 111, r8 99, r0 92, r9 86, sl 69, r4 60, fp 36, r1 25, r2 6, lr 1, r3 1, undecoded 5 (one source-shape difference, below) |
| canary stack-offset mismatches | — | **2** (both `MenuTouchWindow` C1/C2, 60 vs 64) |

The two canary-slot differences and the one guard-register difference are all in
still-non-exact functions and are source-shape, not flag, differences:

| function | pct | original canary/guard/off | ours canary/guard/off |
|---|---:|---|---|
| `_ZN12HangarWindow6renderEv` | 78.8 | r2 / r1 / 140 | r2 / r2 / 140 |
| `_ZN15MenuTouchWindowC1Ei` | 94.2 | r1 / r1 / 60 | r1 / r1 / 64 |
| `_ZN15MenuTouchWindowC2Ei` | 94.2 | r1 / r1 / 60 | r1 / r1 / 64 |

Every other canary function has the same value register, guard register and stack
offset in both binaries. The 5 "undecoded" functions are the same 5 on both sides
(`ArrayAdd<Matrix>`, `EaseInOutMatrix::SetRange`, `FModSound::init`,
`__cxa_demangle`, `_zip_filerange_crc`); their canary store uses a sequence the
simple pattern does not match, but both sides agree.

The canary is the **global** guard, not a TLS guard: both binaries import
`__stack_chk_guard@LIBC` as `UND OBJECT` and `__stack_chk_fail@LIBC` as `UND FUNC`
(`readelf -sW`), and the prologue loads the guard through the GOT
(`ldr rA,[pc]` / `add rA,pc` / `ldr rB,[rA]` / `ldr rC,[rB]`). The default
`-mstack-protector-guard=global` is therefore correct.

### 2.3 Variant fingerprints

| variant | canary functions | presence mismatches | `byte_exact` | `.text` |
|---|---:|---:|---:|---:|
| `-fstack-protector-strong` (baseline) | **756** | **0** | **4499** | `0x1337c8` |
| `-fstack-protector` (basic) | 25 (21 vendored + 4 app) | 731 | 1377 | `0x12e168` |
| `-fstack-protector-all` | 3835 | 3079 | 91 | `0x14f728` |
| `-fno-stack-protector` | 21 (all vendored) | 735 | 1377 | `0x12e0e8` |

The 21 vendored-lib canaries are `_ZnwjSt11align_val_t`, `__cxa_demangle`,
`__dynamic_cast`, `__gxx_personality_v0`, `_zip_dirent_read`,
`_zip_dirent_torrent_normalize`, `_zip_dirent_write`, `_zip_error_strerror`,
`_zip_file_get_offset`, `_zip_filerange_crc`, `_zip_mkstemp`, `compress2`,
`crc32_combine`, `inflate`, `inflateBack`, `inflateSync`, `inflate_table`,
`uncompress`, `zip_close`, `zip_open`, `zip_source_zip`. They come from the
separate vendored-lib build (`third_party/build_match_lib.cmake` already compiles
zlib/libzip/libc++abi with `-fstack-protector-strong`), so they are unaffected by
the app flag. The 4 app functions protected by basic are
`RecordHandler::addHashToOptions`, `RecordHandler::addHash`,
`RecordHandler::checkHash` and `FModSound::init`. Strong protects 735 app
functions; basic protects 4; all protects 3835. Only strong matches the original's
756.

## 3. Frame pointer (positive evidence for the default "keep" setting)

| variant | frame-pointer functions | fp mismatches | `byte_exact` | `.text` | `.text` sha256 (first 16) |
|---|---:|---:|---:|---:|---|
| baseline (default) | **2712** | **0** | **4499** | `0x1337c8` | `07cc0465c98f1aad` |
| `-fno-omit-frame-pointer` | 2712 | 0 | 4499 | `0x1337c8` | `07cc0465c98f1aad` |
| `-mno-omit-leaf-frame-pointer` | 2712 | 0 | 4499 | `0x1337c8` | `07cc0465c98f1aad` |
| `-Xclang -mdisable-fp-elim` | 2712 | 0 | 4499 | `0x1337c8` | `07cc0465c98f1aad` |
| `-momit-leaf-frame-pointer` | 2625 | 87 | 1386 | `0x133648` | — |
| `-fomit-frame-pointer` | 131 | 2581 | 1298 | `0x12dfc8` | — |

The original has 2712 frame-pointer functions and 1812 without. Our default
reproduces that set exactly. The three "keep" spellings produce byte-identical
`.text` to the baseline, so the tree's default is already the original's setting;
adding `-fno-omit-frame-pointer` would be a no-op (and is therefore not landed).
`-fomit-frame-pointer` and `-momit-leaf-frame-pointer` are disproven: they remove
frame pointers from 2581 / 87 functions respectively and drop the gate.

`-mllvm -frame-pointer` and `-mllvm -enable-frame-pointer` are **not clang 7.0.2
options** (`Unknown command line argument '-frame-pointer'` /
`'-enable-frame-pointer'`); `-mframe-pointer=all|none|non-leaf` is also rejected by
the driver. The only clang-7 frame-pointer controls are the driver flags above and
the cc1 `-mdisable-fp-elim`.

## 4. Unwind tables (positive evidence for `-funwind-tables`)

### 4.1 Entry-level comparison

Parsing `.ARM.exidx` directly (8-byte entries, prel31 targets):

| metric | original | ours (baseline) |
|---|---:|---:|
| `.ARM.exidx` size | `0x6f70` | `0x6f70` |
| entries | **3566** | **3566** |
| `CANTUNWIND` entries | **272** | **272** |
| extab entries | **3294** | **3294** |
| unique target addresses | 3372 | 3372 |
| duplicate-address pairs | 194 | 194 |
| entry-target sequence diffs | — | **0** |
| entry-kind sequence diffs | — | **0** |

The 194 duplicate-address pairs are the documented 193 empty-`.text` CANTUNWIND
sentinels from `-ffunction-sections` plus the one zero-size
`AbyssEngine::getAppVersion`. The extab *offsets* differ (the LSDA records have
different sizes in the three still-non-exact bodies), but the function-to-entry
mapping and the CANT/extab classification are identical.

### 4.2 The flag-order trap

`CMakeLists.txt` has:

```cmake
target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)
```

`target_compile_options` is emitted **after** `CMAKE_CXX_FLAGS`. Appending
`-fno-unwind-tables` to `CMAKE_CXX_FLAGS` therefore leaves `-funwind-tables` as
the last flag and is neutral. In clang 7.0.2:

* `-funwind-tables` and `-fasynchronous-unwind-tables` both emit cc1
  `-munwind-tables` (aliases).
* `-fno-unwind-tables` cancels `-munwind-tables`; last occurrence wins.
* `-fno-asynchronous-unwind-tables` does **not** cancel `-munwind-tables` (it is
  the default spelling), so appending it is neutral regardless of order.

To test the real removal one must filter `-funwind-tables` out of the compiler
command (the wrapper in §1). The filtered build is 2878 `byte_exact`, `.ARM.exidx`
`0x6678` (3279 entries, 698 CANT, 2581 extab), 3275 target diffs.

| variant | exidx size | entries | CANT | extab | target diffs | kind diffs | `byte_exact` |
|---|---:|---:|---:|---:|---:|---:|---:|
| baseline `-funwind-tables` | `0x6f70` | 3566 | 272 | 3294 | 0 | 0 | **4499** |
| `-fno-unwind-tables` appended | `0x6f70` | 3566 | 272 | 3294 | 0 | 0 | 4499 |
| `-fno-asynchronous-unwind-tables` appended | `0x6f70` | 3566 | 272 | 3294 | 0 | 0 | 4499 |
| `-fasynchronous-unwind-tables` appended | `0x6f70` | 3566 | 272 | 3294 | 0 | 0 | 4499 |
| `-funwind-tables` filtered out | `0x6678` | 3279 | 698 | 2581 | 3275 | 869 | 2878 |
| filtered + `-fno-asynchronous-unwind-tables` | `0x6678` | 3279 | 698 | 2581 | 3275 | 869 | 2878 |
| filtered + `-funwind-tables` re-appended (also filtered) | `0x6678` | 3279 | 698 | 2581 | 3275 | 869 | 2878 |

The three filtered variants have identical `.text` (sha `1d97e91ab69b1425`),
confirming that clang 7's `-funwind-tables` is the asynchronous-unwind-tables
switch and that the 2878 drop is the real effect of removing it.

## 5. Full variant matrix (official verifier)

All rows are one flag on top of the canonical `-Oz` build. `size` is
`size-exact/compared`; `avg` is the verifier's `avg_match`. `.rodata` is
`0x41b67` in every row.

| tag | flag(s) | byte | linked | size | avg | `.text` | `.ARM.exidx` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| base | `-fstack-protector-strong` (canonical) | **4499** | **4499** | 4524/4524 | 99.90 | `0x1337c8` | `0x6f70` | `0x26298` |
| sp1 | `-fstack-protector` | 1377 | 3785 | 3793/4524 | 93.50 | `0x12e168` | `0x6f18` | `0x2607c` |
| spall | `-fstack-protector-all` | 91 | 963 | 981/4524 | 45.97 | `0x14f728` | `0x7230` | `0x270bc` |
| nosp | `-fno-stack-protector` | 1377 | 3781 | 3789/4524 | 93.47 | `0x12e0e8` | `0x6f18` | `0x2607c` |
| omitfp | `-fomit-frame-pointer` | 1298 | 1922 | 2163/4524 | 84.09 | `0x12dfc8` | `0x6868` | `0x214f4` |
| noomitfp | `-fno-omit-frame-pointer` | 4499 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6f70` | `0x26298` |
| omitleaffp | `-momit-leaf-frame-pointer` | 1386 | 4415 | 4449/4524 | 99.49 | `0x133648` | `0x6f60` | `0x26034` |
| mnoomitleaffp | `-mno-omit-leaf-frame-pointer` | 4499 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6f70` | `0x26298` |
| disablefp | `-Xclang -mdisable-fp-elim` | 4499 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6f70` | `0x26298` |
| nounwind_append | `-fno-unwind-tables` appended | 4499 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6f70` | `0x26298` |
| noasync_append | `-fno-asynchronous-unwind-tables` appended | 4499 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6f70` | `0x26298` |
| async_append | `-fasynchronous-unwind-tables` appended | 4499 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6f70` | `0x26298` |
| nounwind_filter | `-funwind-tables` filtered out | 2878 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6678` | `0x2556c` |
| noasync_filter | filtered + `-fno-asynchronous-unwind-tables` | 2878 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6678` | `0x2556c` |
| unwind_filter_keep | filtered + `-funwind-tables` re-appended (also filtered) | 2878 | 4499 | 4524/4524 | 99.90 | `0x1337c8` | `0x6678` | `0x2556c` |

No variant beats 4499 / 4499 / 4524. The neutral variants are byte-identical in
`.text` to the baseline; they are not improvements, just confirmations.

## 6. Per-TU evidence and spot checks

Mapping each of the 4524 compared functions to its object file
(`nm --defined-only` over the 192 `CMakeFiles/gof2.dir/src/**/*.obj` objects)
gives:

* 192 TUs mapped (the remaining report rows are vendored-lib symbols).
* 157 TUs contain at least one canary function.
* **0 TUs** have a canary-presence, frame-pointer or exidx-kind mismatch between
  the original and our build.

The largest canary TUs are `PaintCanvas.cpp` (49), `PlayerEgo.cpp` (25),
`RecordHandler.cpp` (25), `AEFile.cpp` (23), `Level.cpp` (20), `String.cpp` (18),
`FileRead.cpp` (17), `Engine.cpp` (17), `FModSound.cpp` (17), `Layout.cpp` (15).
The per-TU counts are identical on both sides.

Per-TU A/B spot checks (one TU compiled with the flag, official verifier, full
gate):

| tag | TU | flag | target pct (base -> per-TU) | byte | linked | size |
|---|---|---|---:|---:|---:|---:|
| ptu_layout_omitleaf | `src/game/ui/Layout.cpp` | `-momit-leaf-frame-pointer` | `Layout::Layout` 47.7 -> 41.0 | 2454 | 4499 | 4522/4524 |
| ptu_globals_omitleaf | `src/game/core/Globals.cpp` | `-momit-leaf-frame-pointer` | `Globals::Globals` 80.5 -> 39.5 | 2538 | 4497 | 4520/4524 |
| ptu_menu_omitleaf | `src/game/ui/MenuTouchWindow.cpp` | `-momit-leaf-frame-pointer` | all targets unchanged (97.7 etc.) | 4499 | 4499 | 4524/4524 |
| ptu_level_sp | `src/game/world/Level.cpp` | `-fstack-protector` | `Level::createScene` 88.3 -> 70.0, `createMission` 90.7 -> 70.4 | 2469 | 4481 | 4504/4524 |
| ptu_radar_omitfp | `src/game/weapons/Radar.cpp` | `-fomit-frame-pointer` | `Radar::draw` 73.9 -> 40.4 | 2771 | 4489 | 4515/4524 |

The per-TU delta equals the global delta for the target's own TU (e.g.
`Layout.cpp` + `-momit-leaf-frame-pointer` gives 41.0 both per-TU and globally;
`Radar.cpp` + `-fomit-frame-pointer` gives 40.4 both ways). Since the target
function's codegen is TU-local (no LTO), a per-TU flag cannot improve a target
that the global flag worsens. This agrees with the 277-cell per-TU matrix in
`_work/reconstructions/worst_function_flag_sensitivity_w3751.md`, which found no
per-TU improvement for the stack-protector or frame-pointer flags.

## 7. Per-target table for the 25 non-exact functions

`can`/`creg`/`greg`/`off` are the original's canary classification (only
meaningful when `can` is true); `fp` is the original's frame pointer; `ex` is the
original's exidx kind. The last columns are the official verifier pct under the
most informative variants.

| function | base | can | creg | greg | off | fp | ex | sp1 | spall | nosp | omitfp | omitleaf | nounwind_filter |
|---|---:|---|---|---:|---:|---|---:|---:|---:|---:|---:|---:|
| `Layout::Layout` C1 | 47.7 | no | — | — | — | yes | extab | 47.7 | 37.1 | 47.7 | 41.0 | 41.0 | 47.7 |
| `Layout::Layout` C2 | 47.7 | no | — | — | — | yes | extab | 47.7 | 37.1 | 47.7 | 41.0 | 41.0 | 47.7 |
| `LevelScript::process` | 52.0 | yes | r0 | fp | 428 | yes | extab | 42.2 | 52.0 | 42.2 | 26.2 | 52.0 | 52.0 |
| `Status::missionCompleted` | 57.7 | no | — | — | — | yes | extab | 57.7 | 49.3 | 57.7 | 28.5 | 57.7 | 57.7 |
| `Radar::draw` | 73.9 | yes | r0 | r5 | 292 | yes | extab | 66.7 | 73.9 | 66.7 | 40.4 | 73.9 | 73.9 |
| `Level::createShip` | 74.1 | no | — | — | — | yes | extab | 74.1 | 61.9 | 74.1 | 52.2 | 74.1 | 74.1 |
| `Generator::createMission` | 74.3 | yes | r0 | r4 | 92 | yes | extab | 60.3 | 74.3 | 60.3 | 58.7 | 74.3 | 74.3 |
| `MGame::OnTouchEnd` | 78.4 | yes | r0 | r9 | 200 | yes | extab | 64.1 | 78.4 | 64.1 | 35.3 | 78.4 | 78.4 |
| `HangarWindow::render` | 78.8 | yes | r2 | r1 | 140 | yes | extab | 67.7 | 78.8 | 67.7 | 56.2 | 78.8 | 78.8 |
| `Globals::Globals` C1 | 80.5 | no | — | — | — | yes | extab | 80.5 | 41.0 | 80.5 | 39.5 | 39.5 | 80.5 |
| `Globals::Globals` C2 | 80.5 | no | — | — | — | yes | extab | 80.5 | 41.0 | 80.5 | 39.5 | 39.5 | 80.5 |
| `RocketGun::update` | 86.9 | yes | r0 | r0 | 4 | yes | extab | 75.4 | 86.9 | 75.4 | 56.1 | 86.9 | 86.9 |
| `PlayerEgo::calcCollision` | 87.3 | yes | r0 | r0 | 24 | yes | extab | 66.1 | 87.3 | 66.1 | 58.6 | 87.3 | 87.3 |
| `Level::createScene` | 88.3 | yes | r0 | r0 | 36 | yes | extab | 70.0 | 88.3 | 70.0 | 64.6 | 88.3 | 88.3 |
| `Hud::draw` | 89.1 | yes | r0 | r0 | 88 | yes | extab | 55.2 | 89.1 | 55.2 | 52.9 | 89.1 | 89.1 |
| `Gun::calcCharacterCollision` | 90.0 | yes | r0 | r0 | 12 | yes | extab | 62.8 | 90.0 | 62.8 | 44.9 | 90.0 | 90.0 |
| `Level::createMission` | 90.7 | yes | r1 | fp | 396 | yes | extab | 70.4 | 90.7 | 70.4 | 56.9 | 90.7 | 90.7 |
| `MenuTouchWindow::MenuTouchWindow` C1 | 94.2 | yes | r1 | r1 | 60 | yes | extab | 80.1 | 94.2 | 80.1 | 57.9 | 94.2 | 94.2 |
| `MenuTouchWindow::MenuTouchWindow` C2 | 94.2 | yes | r1 | r1 | 60 | yes | extab | 80.1 | 94.2 | 80.1 | 57.9 | 94.2 | 94.2 |
| `SpaceLounge::drawLounge` | 95.5 | yes | r0 | r5 | 116 | yes | extab | 75.6 | 95.5 | 75.6 | 71.9 | 95.5 | 95.5 |
| `MenuTouchWindow::OnTouchBegin` | 97.7 | no | — | — | — | yes | extab | 75.1 | 97.7 | 75.1 | 49.7 | 97.7 | 97.7 |
| `MenuTouchWindow::OnTouchEnd` | 97.9 | yes | r0 | r6 | 276 | yes | extab | 58.0 | 97.9 | 58.0 | 36.6 | 97.9 | 97.9 |
| `ModStation::ModStation` C1 | 98.4 | yes | r2 | r0 | 172 | yes | extab | 75.6 | 98.4 | 75.6 | 75.7 | 98.4 | 98.4 |
| `ModStation::ModStation` C2 | 98.4 | yes | r2 | r0 | 172 | yes | extab | 75.6 | 98.4 | 75.6 | 75.7 | 98.4 | 98.4 |
| `MGame::OnUpdate` | 99.9 | yes | r0 | r0 | 44 | yes | extab | 80.4 | 99.9 | 80.4 | 58.1 | 99.9 | 99.9 |

No variant raises any target's pct. Several targets are unchanged by a variant
because they already match that aspect (e.g. `-fstack-protector-all` leaves the
canary-bearing targets alone when the extra canaries land in other functions).

## 8. Unsupported spellings in clang 7.0.2

```text
$ clang++ --target=armv7-none-linux-androideabi16 -Oz -fstack-protector-explicit -c t.cpp
clang++: error: unknown argument: '-fstack-protector-explicit'

$ clang++ --target=armv7-none-linux-androideabi16 -Oz -mllvm -frame-pointer -c t.cpp
clang (LLVM option parsing): Unknown command line argument '-frame-pointer'. ...
clang (LLVM option parsing): Did you mean '-debug-counter'?

$ clang++ --target=armv7-none-linux-androideabi16 -Oz -mllvm -enable-frame-pointer -c t.cpp
clang (LLVM option parsing): Unknown command line argument '-enable-frame-pointer'. ...
clang (LLVM option parsing): Did you mean '-enable-pipeliner'?
```

The cc1 stack-protector levels in clang 7.0.2 are `0=none, 1=basic, 2=strong,
3=all` (verified with `-Xclang -stack-protector -Xclang N` on a test TU with a
stack buffer); there is no explicit level. The driver maps
`-fstack-protector` -> 1, `-fstack-protector-strong` -> 2,
`-fstack-protector-all` -> 3, `-fno-stack-protector` -> 0 (verified with
`clang -###`).

## 9. Verdicts

| hypothesis | verdict | evidence |
|---|---|---|
| `-fstack-protector-strong` used globally | **proven** | 756 canaries in both, 0 presence mismatches, identical value-register distribution; basic/all/none each differ by 731/3079/735 functions and drop the gate |
| stack-protector level differs per TU | **disproven** | 192 TUs, 157 with canaries, 0 per-TU presence mismatches; per-TU A/B equals global delta |
| frame pointers kept (default / `-fno-omit-frame-pointer`) | **proven** | 2712 fp functions in both, 0 mismatches; keep spellings produce byte-identical `.text`; omit variants drop to 131/2625 fp and 1298/1386 byte_exact |
| frame-pointer setting differs per TU | **disproven** | 0 per-TU fp mismatches |
| `-funwind-tables` used globally | **proven** | 3566 exidx entries, 272 CANT, 3294 extab, target/kind sequences identical; filtered removal gives 3279 entries and byte 2878 |
| unwind setting differs per TU | **disproven** | 0 per-TU exidx-kind mismatches |
| `-fstack-protector` (basic) | **disproven** | 25 canary functions vs 756; byte 1377 |
| `-fstack-protector-all` | **disproven** | 3835 canary functions vs 756; byte 91 |
| `-fno-stack-protector` | **disproven** | 21 canary functions (all vendored) vs 756; byte 1377 |
| `-fstack-protector-explicit` | **not applicable** | clang 7.0.2 rejects it; no explicit level exists |
| `-fomit-frame-pointer` | **disproven** | 131 fp functions vs 2712; byte 1298 |
| `-momit-leaf-frame-pointer` | **disproven** | 2625 fp functions vs 2712; byte 1386 |
| `-mllvm -frame-pointer`, `-mllvm -enable-frame-pointer` | **not applicable** | clang 7.0.2 rejects both |
| `-fno-unwind-tables` / `-fno-asynchronous-unwind-tables` (effective) | **disproven** | filtered removal gives 3279 entries, byte 2878 |
| `-fasynchronous-unwind-tables` | **neutral alias** | byte-identical `.text`; clang 7 emits `-munwind-tables` for both |

## 10. What remains

* No build-flag change is landed: no variant strictly raises `byte_exact` /
  `linked_exact` / `size-exact`. The current `-fstack-protector-strong`,
  default frame pointer and `-funwind-tables` are confirmed by positive
  fingerprints, not merely by "the gate did not improve".
* The three canary-register/offset differences (`HangarWindow::render` guard
  register; `MenuTouchWindow` C1/C2 stack slot) are source-shape residues in
  still-non-exact functions. They are not flag-driven and should be fixed by the
  normal source/TU reconstruction campaign.
* The remaining 25 non-exact functions are unchanged by every flag tested here;
  the diffs are register allocation, scheduling, basic-block order and stack-frame
  layout, i.e. source shape.

## 11. Reproduction

```sh
# baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# one variant (example: basic stack protector)
cmake -S . -B /dev/shm/w4305/build-sp1 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4305/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_CXX_FLAGS='-fstack-protector' \
  -DGOF2_EXTRA_C_FLAGS='-fstack-protector' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4305/build-sp1 --target link_so -j8
python3 tools/verify/verify.py --build-dir /dev/shm/w4305/verify-sp1 \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4305/build-sp1/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# canary / frame / exidx classification
python3 /dev/shm/w4305/classify_full.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so \
  cmake-build-match/verify/report.json base

# exidx entry-sequence comparison
python3 /dev/shm/w4305/exidx_seq.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch harness (not tracked): `/dev/shm/w4305/toolchain_extra.cmake`,
`toolchain_filter.cmake`, `clangxx_filter.py`, `clang_filter.py`,
`run_variant.sh`, `run_ptu.sh`, `classify_full.py`, `exidx_seq.py`,
`summarize.py`, `per_tu.py`, and the per-variant `build-*`/`verify-*` dirs.
The committed data file is
`_work/reconstructions/stack_frame_unwind_w4305_canary.csv` (756 rows).
