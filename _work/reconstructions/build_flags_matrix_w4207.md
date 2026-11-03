# Build-flag matrix A: optimization + language semantics (w4207, wave 92)

Status: research note (2026-11-03). Worktree `wt/w4207`, branch `wave92-w4207`,
base `b379d00d` ("ModStation ctor pass 3").  Every number below is from the official
`tools/verify/verify.py` run against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` on a fresh
link of this tree, not from a proxy.  No source or build-flag change is landed:
**no candidate flag strictly raises `byte_exact`**, and all four per-file
`-mllvm -likely-branch-weight` overrides are individually load-bearing.  This
commit contains this note only.

## 0. Bottom line

* No matrix-A candidate improves on the current global flag set.  `-Oz` is far ahead of
  `-Os/-O1/-O2/-O3`; every tested variant either leaves the gate unchanged
  or makes it worse.
* The four current per-file `-mllvm -likely-branch-weight` overrides
  (`Radar.cpp` 80:1, `SpaceLounge.cpp` 20:12, `StarMap.cpp` 7:1,
  `MenuTouchWindow.cpp` 5:1) **cannot be replaced by a single global flag**.
  A global 80:1/20:12/7:1/5:1 applied to every TU drops `byte_exact` from 4498
  to 2404/3191/3633/3154 respectively.
* Each per-file override is load-bearing for at least one function.  The
  `StarMap.cpp` 7:1 override is the critical one: removing it changes
  `StarMap::drawOnScreenInfo` from 3488 to 3500 bytes, shifts `.text` by
  `+0x20`, and cascades into **2094** `byte_exact` losses (4498 -> 2404) even
  though `linked_exact` only drops 1.  This is a layout-shift effect, not 2094
  independent codegen failures.
* A flag-order trap invalidates the naive appended test of `-fno-unwind-tables`:
  `target_compile_options(gof2 PRIVATE -funwind-tables)` appears after
  `CMAKE_CXX_FLAGS`, so appending `-fno-unwind-tables` is neutral.  A proper
  filtered removal drops `byte_exact` 4498 -> 2878.  `-fno-asynchronous-unwind-tables`
  behaves identically.

## 1. Baseline and gate

Fresh `check` before and after the matrix (the canonical `cmake-build-match`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json`: `compared 4524`, `size_exact 4524/4524`.
`.text` = `0x1337c8` at `0x69440`; `.rodata` = `0x41b67`; `.data` = `0x296c`;
`.bss` = `0x69ac`; `.ARM.exidx` = `0x6f70`; `.ARM.extab` = `0x26290`
(original `0x26298`, the known -8 residual).

| artifact | sha256 |
|---|---|
| tree `cmake-build-match/libgof2hdaa.so` | `2283c01c057d49f02a582701167fdbfe40010a753d530de13842fe5e0dfe6527` |
| ground truth | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` |

## 2. Toolchain identity (re-checked)

`readelf` on the ground truth confirms the toolchain the tree already uses:

| observable | value |
|---|---|
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123 (prerelease)` |
| `.note.android.ident` | API `16` (`0x10`), `r18b`, build `5063045` |
| `.note.gnu.gold-version` | `gold 1.12` |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, int enums, 8-byte align, unaligned v6 |
| ELF | `ELF32 DYN ARM`, `Version5 EABI, soft-float ABI` |

The matrix below therefore varies only the flags under test; the compiler,
target triple, ABI and linker are held fixed.

## 3. Method

Each variant gets its own build directory under `/dev/shm/w4207/build-<tag>` and
its own verify directory under `/dev/shm/w4207/verify-<tag>`.  A temporary
toolchain file includes the real `cmake/toolchain.cmake` and appends exactly one
candidate flag to `CMAKE_CXX_FLAGS`:

```cmake
include("/home/debian/GalaxyOnFire2/wt/w4207/cmake/toolchain.cmake")
if(GOF2_EXTRA_CXX_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_CXX_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Optimization levels are selected with `-DGOF2_MATCH_OPT=<level>` (the same knob
the canonical preset uses), not by appending a second `-O` flag.  The build is
`link_so`; the measurement is the official verifier:

```sh
cmake -S . -B /dev/shm/w4207/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4207/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_CXX_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4207/build-<tag> --target link_so -j16
python3 tools/verify/verify.py --build-dir /dev/shm/w4207/verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4207/build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

`size-exact` is `sum(osize == usize)` over the verifier rows.  `.text` /
`.rodata` are `readelf -SW` sizes of the linked `.so`.

Worktree note: the default `GOF2_ORIGINAL_SO` is
`${CMAKE_SOURCE_DIR}/../_work/bins/...`, which is correct in the main repo but
resolves to `wt/_work/...` in a worktree.  Both the ground-truth path and
`GOF2_FMOD_LIBDIR` are therefore passed explicitly above.  This is a
reproducibility detail, not a build-flag result.

## 4. Matrix A results

All variants are one flag at a time on top of the canonical `-Oz` build.
`size-exact` is `exact/compared` (the compared count drops for variants whose
symbol set changes, e.g. `-fvisibility=hidden`).  `avg` is the verifier's
`avg_match`.

| variant | flag | byte_exact | linked_exact | size-exact | avg | .text | .rodata |
|---|---|---:|---:|---:|---:|---:|---:|
| `base` | `-Oz` (baseline) | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `Os` | `-Os` | 1263 | 2650 | 2882/4356 | 84.54 | `0x14bee8` | `0x043057` |
| `O1` | `-O1` | 1033 | 1783 | 2676/4524 | 76.76 | `0x143f68` | `0x041bc7` |
| `O2rerun` | `-O2` | 1168 | 2159 | 2411/4296 | 77.32 | `0x1632a8` | `0x042d77` |
| `O3` | `-O3` | 1167 | 2119 | 2247/4295 | 76.52 | `0x1717b8` | `0x042c97` |
| `fnostrictalias` | `-fno-strict-aliasing` | 1371 | 4348 | 4411/4524 | 98.98 | `0x133d08` | `0x041b67` |
| `fwrapv` | `-fwrapv` | 1398 | 4462 | 4500/4524 | 99.8 | `0x1338c8` | `0x041b67` |
| `fnodeletenull` | `-fno-delete-null-pointer-checks` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnomatherrno` | `-fno-math-errno` | 1356 | 4487 | 4514/4524 | 99.73 | `0x133638` | `0x041b67` |
| `ffpcontractoff` | `-ffp-contract=off` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `ffpcontractfast` | `-ffp-contract=fast` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnohonornans` | `-fno-honor-nans` | 1429 | 4373 | 4480/4524 | 99.65 | `0x1334c8` | `0x041b67` |
| `fnohonorinf` | `-fno-honor-infinities` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `ffinite` | `-ffinite-math-only` | 1429 | 4373 | 4480/4524 | 99.65 | `0x1334c8` | `0x041b67` |
| `fnothreadsafestatics` | `-fno-threadsafe-statics` | 1368 | 4494 | 4520/4521 | 99.89 | `0x133684` | `0x0419b7` |
| `fnocxaatexit` | `-fno-use-cxa-atexit` | 1356 | 4497 | 4523/4524 | 99.89 | `0x133818` | `0x041b67` |
| `fnosizeddealloc` | `-fno-sized-deallocation` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `dndebug` | `-DNDEBUG` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnojumptables` | `-fno-jump-tables` | 1363 | 4412 | 4438/4524 | 99.14 | `0x1348e8` | `0x041947` |
| `visinlineshidden` | `-fvisibility-inlines-hidden` | 1278 | 4203 | 4381/4383 | 99.84 | `0x1337a8` | `0x041b67` |
| `vishidden` | `-fvisibility=hidden` | 59 | 296 | 300/300 | 99.91 | `0x130f98` | `0x041b67` |
| `fnovectorize` | `-fno-vectorize` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnoslp` | `-fno-slp-vectorize` | 1484 | 4384 | 4408/4524 | 99.26 | `0x132ea8` | `0x041b67` |
| `fmergeall` | `-fmerge-all-constants` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnomerge` | `-fno-merge-constants` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnounwind` | `-fno-unwind-tables` (appended) | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnounwind_filtered` | `-fno-unwind-tables` (filtered `-funwind-tables`) | 2878 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnoasyncunwind` | `-fno-asynchronous-unwind-tables` (appended) | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `fnoasyncunwind_filtered` | `-fno-asynchronous-unwind-tables` (filtered) | 2878 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `stdcxx11` | `-std=c++11` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `stdgnu11` | `-std=gnu++11` | 4498 | 4498 | 4524/4524 | 99.9 | `0x1337c8` | `0x041b67` |
| `stdcxx14` | `-std=c++14` | 1512 | 4485 | 4514/4524 | 99.86 | `0x133788` | `0x041b67` |

### 4.1 Reading the table

* **Optimization.** `-Oz` is uniquely correct.  `-Os` (1263), `-O2` (1168),
  `-O3` (1167) and `-O1` (1033) are all far below 4498; `.text` also grows by
  0x107a0 (`-O1`) to 0x3dff0 (`-O3`) over the baseline.  No combination is
  warranted because no optimization level other than `-Oz` wins.
* **Aliasing / overflow.** `-fno-strict-aliasing` (1371) and `-fwrapv` (1398)
  are both much worse, so the original build used strict aliasing and did not
  use `-fwrapv`.  `-fno-delete-null-pointer-checks` is neutral.
* **Floating point.** `-fno-math-errno` is worse (1356), so math errno is on.
  `-fno-honor-nans` and `-ffinite-math-only` are worse (both 1429) and produce
  the same `.so` sha, i.e. the effect is entirely NaN handling;
  `-fno-honor-infinities` alone is neutral.  `-ffp-contract=off` and
  `-ffp-contract=fast` are both neutral, so this source does not distinguish
  them at `-Oz`.
* **Statics / EH / deallocation.** `-fno-threadsafe-statics` (1368) and
  `-fno-use-cxa-atexit` (1356) are worse: the original uses thread-safe statics
  and `__cxa_atexit`.  `-fno-sized-deallocation` is neutral.
* **Jump tables / visibility.** `-fno-jump-tables` is worse (1363), so the
  original uses jump tables.  `-fvisibility=hidden` is catastrophic (59
  byte-exact, 4224 missing) and `-fvisibility-inlines-hidden` is worse (1278,
  141 missing): the original exports the default-visibility symbol set.
* **Vectorization.** `-fno-vectorize` is neutral (the loop vectorizer is
  effectively idle at `-Oz` here); `-fno-slp-vectorize` is worse (1484), so SLP
  vectorization is on and load-bearing.
* **Constants.** `-fmerge-all-constants` and `-fno-merge-constants` are both
  neutral; constant merging does not affect this tree's codegen.
* **Unwind tables.** See §5; the naive appended tests are neutral, the proper
  filtered removals are much worse (2878).
* **Language standard.** `-std=c++11` and `-std=gnu++11` are neutral;
  `-std=c++14` is worse (1512).  The current `-std=c++11` is correct.
* **`-DNDEBUG`.** Neutral.  The tree does not depend on `assert` codegen.

No flag strictly raises `byte_exact` above 4498, so no combination pass is
required by the protocol.  The current flags are confirmed rather than merely
assumed.

## 5. The `-funwind-tables` flag-order trap

`CMakeLists.txt` has:

```cmake
target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)
```

`target_compile_options` is emitted **after** `CMAKE_CXX_FLAGS` on the compile
line.  Appending `-fno-unwind-tables` to `CMAKE_CXX_FLAGS` therefore leaves
`-funwind-tables` as the last flag and is neutral:

| variant | byte | linked | size-exact | `.ARM.exidx` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| baseline | 4498 | 4498 | 4524/4524 | `0x6f70` | `0x26290` |
| `-fno-unwind-tables` appended | 4498 | 4498 | 4524/4524 | `0x6f70` | `0x26290` |
| `-funwind-tables` filtered out, nothing added | 2878 | 4498 | 4524/4524 | `0x6678` | `0x25564` |
| `-fno-unwind-tables`, `-funwind-tables` filtered out | 2878 | 4498 | 4524/4524 | `0x6678` | `0x25564` |
| `-fno-asynchronous-unwind-tables` appended | 4498 | 4498 | 4524/4524 | `0x6f70` | `0x26290` |
| `-fno-asynchronous-unwind-tables`, filtered | 2878 | 4498 | 4524/4524 | `0x6678` | `0x25564` |

The three filtered variants have the same sha
(`552539c7da3d04b8ae3265189b8088d919c05b5a1c1f88060a5eb38818505ec7`), confirming
that clang 7's `-funwind-tables` is the asynchronous-unwind-tables switch and
that it is not the ARM default.  `-funwind-tables` is load-bearing: it must stay.

## 6. Per-file `-mllvm -likely-branch-weight` overrides

The current base has **four** per-file overrides (the task text says "three";
`MenuTouchWindow.cpp` is the fourth).  They are not original build flags: the
original almost certainly used LLVM 7.0.2 defaults.  They are local
compensations for source/TU shape.  I tested each one in isolation, all four
removed, and each weight applied globally with all four per-file overrides
removed.

### 6.1 Removing one override at a time

| variant | override removed | byte | linked | size-exact | `.text` | affected function (fuzzy) |
|---|---|---:|---:|---:|---:|---|
| baseline | none | 4498 | 4498 | 4524/4524 | `0x1337c8` | — |
| `noradar` | `Radar.cpp` 80:1 | 4498 | 4498 | 4524/4524 | `0x1337c8` | `Radar::draw` 73.9 -> 72.5 |
| `nospace` | `SpaceLounge.cpp` 20:12 | 4498 | 4498 | 4524/4524 | `0x1337c8` | `SpaceLounge::drawLounge` 95.6 -> 95.5 |
| `nostarmap` | `StarMap.cpp` 7:1 | **2404** | 4497 | 4523/4524 | `0x1337e8` | `StarMap::drawOnScreenInfo` 100.0 -> 87.5, size 3488 -> 3500 |
| `nomenu` | `MenuTouchWindow.cpp` 5:1 | 4498 | 4498 | 4524/4524 | `0x1337c8` | `MenuTouchWindow::OnTouchEnd` 98.0 -> 97.9 |
| `nofileflags` | all four | **2404** | 4497 | 4523/4524 | `0x1337e8` | all of the above |

All four are load-bearing for at least one function's fuzzy match.  The
`StarMap.cpp` 7:1 override is also load-bearing for layout: removing it changes
`StarMap::drawOnScreenInfo` from 3488 to 3500 bytes, which grows `.text` by
`+0x20` and makes every later address-dependent function fail `byte_exact`
(2094 rows change byte but only 1 row changes size).  `linked_exact` stays at
4497 because the verifier normalizes branch/literal targets.

### 6.2 Global replacement

With all four per-file overrides removed, applying one weight globally:

| global flag | byte | linked | size-exact | `.text` |
|---|---:|---:|---:|---:|
| `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 2404 | 4497 | 4523/4524 | `0x1337e8` |
| `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | 3191 | 4492 | 4519/4524 | `0x1337c8` |
| `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` | 3633 | 4496 | 4521/4524 | `0x1337c8` |
| `-mllvm -likely-branch-weight=5 -mllvm -unlikely-branch-weight=1` | 3154 | 4495 | 4520/4524 | `0x1337c8` |

Every global variant is far below 4498.  The reason is visible in the
per-function rows: a global weight changes unrelated TUs (`NewsTicker` ctors,
`PlayerEgo::update`, `Radar::draw`, `RocketGun::update`, ...), while the
per-file scoping leaves them alone.  For example, global 20:12 changes
`NewsTicker::NewsTicker` from 972 to 968 bytes and 100.0% to 68.0% fuzzy.
Global 7:1 changes the same ctor to 956 bytes / 64.9%.  The per-file scoping is
therefore essential; a single global flag is not a replacement.

### 6.3 Faithful-source replacement

The right long-term fix is still source/TU reconstruction, not flags: each
override corresponds to one function whose current source shape does not
naturally produce the original's block layout.

* `Radar::draw` (73.9% with the flag, 72.5% without) — the flag buys 1.4 points.
* `SpaceLounge::drawLounge` (95.6% vs 95.5) — 0.1 point.
* `StarMap::drawOnScreenInfo` (100.0% vs 87.5%, and 3488 vs 3500 bytes) — the
  flag is required for the current source to be byte-exact; the 12-byte size
  difference is the layout cascade.
* `MenuTouchWindow::OnTouchEnd` (98.0% vs 97.9) — 0.1 point; the comment in
  `CMakeLists.txt` already notes that the 3:1..19:1 plateau reproduces the
  original block layout and 5:1 only raises fuzzy by 0.1.

Removing any of these flags without a source change is gate-neutral for
`noradar`/`nospace`/`nomenu` but worsens the target function's fuzzy match, so
none is a free "hack removal".  The `StarMap.cpp` flag cannot be removed at all
without collapsing `byte_exact`.  A future pass that reconstructs one of these
functions can re-test the flag removal with the same harness; until then the
four overrides should stay.

## 7. What remains

* No global build-flag change is justified by this matrix.
  `cmake/toolchain.cmake` stays as is.
* The four per-file `-mllvm` overrides stay.  The `StarMap.cpp` one is the
  highest-value target for a source-level fix because it is the only one that
  currently changes `.text` size.
* The remaining gap is source shape, TU membership and link layout, not a
  missing compiler flag.  This is consistent with the earlier build-provenance
  notes (`build_flags_research.md`, `build_config_rederivation_w3745.md`,
  `build_flags_independent_audit_w3750.md`), which this matrix re-confirms on
  the wave-92 base with the official verifier.
* `docs/IDENTICALITY.md` still quotes `byte_exact 4115` in its "Current" line;
  that number is stale relative to the wave-92 gate (4498).  It is not changed
  here because this pass does not change any gate count.

## 8. Reproduction

Scratch harness (all under `/dev/shm/w4207`):

* `toolchain_extra.cmake` — includes the real toolchain, appends
  `GOF2_EXTRA_CXX_FLAGS`.
* `toolchain_filter.cmake`, `clangxx_filter.sh`, `clang_filter.sh` — remove
  exact tokens listed in `GOF2_FILTER_FLAGS` before invoking the real clang
  (used only for the `-funwind-tables` removal).
* `run_variant.sh`, `run_filter_variant.sh` — configure, `link_so`, official
  `verify.py`, and append one JSON line to `results.jsonl`.
* `results.jsonl` — the raw data behind the tables above.

The canonical build directory `cmake-build-match` was not disturbed; it was
reconfigured only to point `GOF2_ORIGINAL_SO`/`GOF2_FMOD_LIBDIR` at the
ground-truth files for this worktree.
