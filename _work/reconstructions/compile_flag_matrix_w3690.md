# Current-state global compile-flag matrix (w3690)

Status: research note, 2026-11-02. Worktree `wt/w3690`, branch `wave87-w3690`,
HEAD `88aaf59f` (based on current `master`). Every number below is from a command run
against `../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled A/B rebuild
of this tree. No `src/` change and no build-flag change is landed: this commit contains
only this note (plus a stale-number correction in `docs/IDENTICALITY.md`, see §7).

## 0. Bottom line

At the current baseline (**byte 4496 / linked 4496 / size 4524/4524 / data 843/843 /
lint CLEAN / drift 0**, `.text` `0x1337c8`):

* **74 global compile-flag variants measured. 0 are strict improvements.**
  65 built and linked; 9 failed to build or link.
* **19 variants produce a byte-identical `.so`** to the scratch baseline (same sha256),
  i.e. they are indistinguishable from the canonical flags at the shipped-artifact level.
  Two more are codegen-neutral but differ only in the 20-byte build-id (`-fshort-wchar`,
  `-std=gnu++11` C++-only); `-fno-trapping-math` is `.so`-identical but changes the
  object-level FP ABI attribute.
* `-Oz` is uniquely best among `-Oz/-Os/-O1/-O2/-O3`; every other optimization level loses
  at least 3,200 byte-exact functions.
* `-fdata-sections` is still a regression at the current tree: **4496 -> 4333 (-163)**.
* `-fno-rtti`, `-fno-exceptions`, `-fno-common`, `-fno-builtin`, `-fvisibility=hidden`,
  `-marm`, `-mcpu=cortex-a8`, `-fomit-frame-pointer`, `-fno-stack-protector` etc. are all
  wrong or fail to build.
* The Radar-scoped `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` is
  **load-bearing**: removing it drops byte_exact 4496 -> 3632 (-864). Replacing it with a
  global `80:1` is gate-neutral (4496/4496/4524) but produces a different `.so`
  (760 differing bytes), so it is not equivalent.
* **Conclusion: the remaining gap is source/TU/PLT/literal-pool/data-order, not a missing
  global compile flag.** The original is consistent with LLVM 7.0.2 defaults plus the
  canonical flag set already in `cmake/toolchain.cmake` and `CMakeLists.txt`.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`report.json`: 4524/4524 size-exact. `.text` = `0x1337c8` at `0x69440` (exact).

```text
fe19fa9c45c400b43f8162bab578165d50b2f17cdab6936e550b75c45bf83280  cmake-build-match/libgof2hdaa.so
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  ../_work/bins/android_2.0.16_libgof2hdaa.so
```

The scratch A/B baseline (`/tmp/w3690/so-baseline.so`) has sha256
`5a324fa86e10282af85af375572849a62d96795e8c1352cf5480d916ac863c3d`; it differs from the
official `cmake-build-match` `.so` in **exactly the 20 build-id bytes** (offset 477-496)
and nothing else. All A/B comparisons below are against that scratch baseline, so a
"byte-identical" verdict means identical to the canonical build modulo the build-id.

### Canonical flag set (for reference)

From `cmake/toolchain.cmake` and `CMakeLists.txt`:

```text
-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
-std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g -stdlib=libc++
-isystem <libc++/libc++abi/support> --sysroot=<ndk> -D__ANDROID_API__=16
-Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
-ffunction-sections -funwind-tables
```

plus the per-file Radar override:

```cmake
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/weapons/Radar.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=80;-mllvm;-unlikely-branch-weight=1")
```

## 2. Method

A scratch toolchain wrapper appends `GOF2_EXTRA_FLAGS` to both `CMAKE_CXX_FLAGS` and
`CMAKE_C_FLAGS`:

```cmake
# /tmp/w3690/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w3690/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Per variant:

```sh
cmake -S . -B /tmp/w3690/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3690/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flags>'
cmake --build /tmp/w3690/build --target link_so -j16
cp /tmp/w3690/build/libgof2hdaa.so /tmp/w3690/so-<tag>.so
python3 /tmp/w3690/verify_fast.py \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3690/so-<tag>.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --build-dir /tmp/w3690/build/verify --fail-on-wrong-type
python3 tools/ratchet.py --build-dir /tmp/w3690/build --no-rebuild
```

The fast verifier was validated against the official `tools/verify/verify.py` on three
completed variants (the `-O2` official run was killed after 4m50 and is shown for
completeness only):

| variant | official verifier | fast verifier |
|---|---|---|
| baseline | 4496 / 4496 / 4524 | 4496 / 4496 / 4524 |
| `-fdata-sections` | 4333 / 4496 / 4524 | 4333 / 4496 / 4524 |
| `-fno-rtti` | 1725 / 4496 / 4524 | 1725 / 4496 / 4524 |
| `-O2` | killed after 4m50 (not completed) | 1168 / 2159 / 2411, missing 224 |

The fast verifier reports `avg` as 99.38 for neutral variants because it assigns 100 to
linked-exact and 0 otherwise; **do not quote its `avg` as the official fuzzy average**
(the official baseline `avg` is 99.9). Byte/linked/size/missing/wrong-type are validated.

## 3. Results

### 3.1 Optimization level

| variant | byte | linked | size | missing | `.text` |
|---|---:|---:|---:|---:|---:|
| `-Oz` | **4496** | **4496** | **4524** | 0 | `0x1337c8` |
| `-Os` | 1263 | 2650 | 2882 | 168 | `0x14be08` |
| `-O1` | 1033 | 1784 | 2676 | 0 | `0x143ef8` |
| `-O2` | 1168 | 2159 | 2411 | 224 | `0x162f08` |
| `-O3` | 1167 | 2119 | 2247 | 225 | `0x171278` |

`-Oz` is the only level that reproduces the original `.text` size; the next-best
(`-O1`) loses 3,463 byte-exact functions. This is hard evidence that the original was
built with `-Oz` (already in `cmake/toolchain.cmake`).

### 3.2 Neutral flags (byte-identical `.so`)

These 19 variants reproduce the scratch baseline sha256 exactly:

```text
-fno-omit-frame-pointer
-fno-unwind-tables
-fno-asynchronous-unwind-tables
-fno-sized-deallocation
-fno-delete-null-pointer-checks
-fno-data-sections
-fmerge-all-constants
-fno-short-wchar
-DNDEBUG
-fno-plt
-fno-use-init-array
-mno-restrict-it
-fno-inline-functions-called-once
-fno-trapping-math
-fno-finite-math-only
-faligned-allocation
-fno-aligned-allocation
-mno-movt
-mllvm -unroll-threshold=0
```

Caveats:

* `-fno-trapping-math` is `.so`-identical but changes each object's
  `Tag_ABI_FP_exceptions` from `Needed` to `Unused`; the linker merges the final
  `.ARM.attributes` back to `Needed`, so the shipped artifact is unchanged. It is a
  codegen-neutral flag, not evidence about the original command line.
* `-fshort-wchar` changes codegen only in the 20-byte build-id and sets the object
  attribute `Tag_ABI_PCS_wchar_t: 2` (canonical is `4`). The linked `.so` still reports 4,
  but the object-level ABI change makes it a weaker candidate; it is not needed.
* `-std=gnu++11` applied to **all** TUs fails (`invalid argument '-std=gnu++11' not
  allowed with 'C'`). C++-only it differs from `-std=c++11` only in the 20-byte build-id
  (4496 / 4496 / 4524). `-std=gnu++14` C++-only is much worse: 1512 / 4483 / 4514,
  `.text 0x133788`.

### 3.3 Worse flags

Selected measured regressions (full table in §3.6):

| flag | byte | linked | size | notes |
|---|---:|---:|---:|---|
| `-fno-strict-aliasing` | 1371 | 4347 | 4410 | strict aliasing is on |
| `-fomit-frame-pointer` | 1298 | 1922 | 2163 | frame pointers are kept |
| `-fstack-protector` | 1369 | 3785 | 3793 | `-strong` is correct |
| `-fstack-protector-all` | 83 | 960 | 981 | catastrophic |
| `-fno-stack-protector` | 1369 | 3781 | 3789 | worse |
| `-fno-rtti` | 1725 | 4496 | 4524 | data 593/673, missing 170 |
| `-fno-builtin` | 1350 | 3984 | 4121 | extra `_ZN11AbyssEngine4MeshC2Ev`, import `fabsf` |
| `-fno-use-cxa-atexit` | 1356 | 4495 | 4523 | slightly worse |
| `-fwrapv` | 1451 | 4461 | 4502 | worse |
| `-fno-strict-overflow` | 1451 | 4461 | 4502 | worse |
| `-mno-unaligned-access` | 1376 | 4441 | 4472 | unaligned access is on |
| `-fno-vectorize -fno-slp-vectorize` | 1484 | 4382 | 4408 | vectorization is on |
| `-fno-inline-functions` | 1219 | 3511 | 3571 | extra 281 |
| `-fno-threadsafe-statics` | 1368 | 4492 | 4520 | missing 3 |
| `-fvisibility=hidden` | 57 | 243 | 244 | missing 4280 |
| `-fvisibility-inlines-hidden` | 1278 | 4203 | 4381 | missing 141 |
| `-mcpu=cortex-a8` | 1330 | 4225 | 4366 | plain `armv7-a` is correct |
| `-marm` | 57 | 237 | 342 | `.text 0x1a3af8`; Thumb-2 is correct |
| `-fdata-sections` | 4333 | 4496 | 4524 | **-163**; confirms w3562 |
| `-fno-zero-initialized-in-bss` | 1522 | 4486 | 4516 | data 557/843 |
| `-mno-global-merge` | 1397 | 4486 | 4517 | worse |
| `-fwritable-strings` | 1425 | 4487 | 4515 | worse |
| `-fno-jump-tables` | 1363 | 4408 | 4436 | worse |
| `-fno-optimize-sibling-calls` | 1316 | 3586 | 3686 | worse |
| `-fno-unroll-loops` | 2883 | 4489 | 4520 | worse |
| `-ffast-math` | 1319 | 4225 | 4369 | object FP attrs `Sign only/Unused/Finite` |
| `-fno-math-errno` | 1356 | 4485 | 4514 | worse |
| `-fno-signed-zeros` | 2355 | 4487 | 4521 | worse |
| `-freciprocal-math` | 1539 | 4400 | 4483 | worse |
| `-mllvm -sink-insts-to-avoid-spills` | 1480 | 4344 | 4393 | worse globally |
| `-mllvm -condsider-local-interval-cost` | 1773 | 4474 | 4507 | worse globally |
| `-mllvm -disable-postra-machine-sink` | 1573 | 4391 | 4475 | worse |
| `-mllvm -regalloc=fast` | 1099 | 1594 | 1654 | `.text 0x1c0ae8` |
| `-mllvm -inline-threshold=100` | 1350 | 3520 | 3554 | missing 205 |
| `-mllvm -inline-threshold=300` | 1350 | 3370 | 3397 | missing 225 |

The `-mllvm -sink-insts-to-avoid-spills` and `-condsider-local-interval-cost` results
confirm the earlier `mllvm_flag_audit.md` conclusion: the historical scoped
compensations are not original global flags.

### 3.4 Build/link failures

| variant | failure |
|---|---|
| `-fno-exceptions` | `Level.cpp:5215: cannot use 'try' with exceptions disabled` |
| `-fno-common` | duplicate `g_android_back_button_pressed` in `BackButton.c` and `GOF2HD2012.c` |
| `-std=gnu++11` (global) | applied to C TUs: `invalid argument '-std=gnu++11' not allowed with 'C'` |
| `-mfloat-abi=hard` | gold: objects use VFP register arguments, output does not |
| `-mfpu=vfpv3-d16` | `arm_neon.h: NEON support not enabled` |
| `-fno-integrated-as` | `as: unrecognized option '-mfpu=neon'` |
| `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` (global, with the Radar scoped pair still present) | option may only occur once |
| `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` (same) | option may only occur once |
| `-mllvm -enable-machine-outliner=true` | clang 7.0.2 crash (`unable to execute command: Aborted`) |

Unsupported clang 7.0.2 flags confirmed: `-mno-ldrd`, `-fno-semantic-interposition`,
`-mfp16-format=ieee`, `-mthumb-interwork`, `-mno-thumb-interwork`,
`-fno-c++-static-destructors`, `-fno-const-strings`. `-fmerge-constants` /
`-fno-merge-constants` are accepted but warn "not supported".

### 3.5 Scoped Radar flag experiment

The Radar `-mllvm` pair is per-file, not global. To measure its contribution I
temporarily commented out the `set_source_files_properties(...)` line in `CMakeLists.txt`
and rebuilt, then restored it (`git diff` clean):

| variant | byte | linked | size | `.so` vs baseline |
|---|---:|---:|---:|---|
| canonical (scoped pair present) | **4496** | **4496** | **4524** | baseline |
| scoped pair removed | 3632 | 4496 | 4523 | 375,197 differing bytes |
| scoped pair removed + global `80:1` | 4496 | 4496 | 4524 | **760 differing bytes** |
| scoped pair removed + global `20:12` | 3263 | 4491 | 4519 | 443,937 differing bytes |

So the scoped pair is worth **+864 byte-exact functions** and +1 size match. A global
`80:1` recovers the gate counts but is **not** the same code (760 differing bytes); it
must not be substituted for the scoped flag.

### 3.6 Full measured table

All 74 variants. "size" is size-exact / 4524. "identical" means the `.so` sha256 equals
the scratch baseline (build-id included).

| # | variant | byte | linked | size | missing | extra | `.so` vs scratch baseline |
|---:|---|---:|---:|---:|---:|---:|---|
| 1 | `-Oz` (canonical baseline) | 4496 | 4496 | 4524/4524 | 0 | 0 | baseline |
| 2 | `-Os` | 1263 | 2650 | 2882/4524 | 168 | 0 | differs |
| 3 | `-O1` | 1033 | 1784 | 2676/4524 | 0 | 276 | differs |
| 4 | `-O2` | 1168 | 2159 | 2411/4524 | 224 | 0 | differs |
| 5 | `-O3` | 1167 | 2119 | 2247/4524 | 225 | 0 | differs |
| 6 | `-Oz -fno-strict-aliasing` | 1371 | 4347 | 4410/4524 | 0 | 0 | differs |
| 7 | `-Oz -fomit-frame-pointer` | 1298 | 1922 | 2163/4524 | 0 | 0 | differs |
| 8 | `-Oz -fno-omit-frame-pointer` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 9 | `-Oz -fstack-protector` | 1369 | 3785 | 3793/4524 | 0 | 0 | differs |
| 10 | `-Oz -fstack-protector-all` | 83 | 960 | 981/4524 | 0 | 0 | differs |
| 11 | `-Oz -fno-stack-protector` | 1369 | 3781 | 3789/4524 | 0 | 0 | differs |
| 12 | `-Oz -fno-rtti` | 1725 | 4496 | 4524/4524 | 0 | 0 | differs |
| 13 | `-Oz -fno-exceptions` | *build fail* | — | — | — | — | — |
| 14 | `-Oz -fno-unwind-tables` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 15 | `-Oz -fno-asynchronous-unwind-tables` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 16 | `-Oz -fno-common` | *build fail* | — | — | — | — | — |
| 17 | `-Oz -fno-builtin` | 1350 | 3984 | 4121/4524 | 0 | 1 | differs |
| 18 | `-Oz -fno-use-cxa-atexit` | 1356 | 4495 | 4523/4524 | 0 | 0 | differs |
| 19 | `-Oz -fno-sized-deallocation` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 20 | `-Oz -fno-delete-null-pointer-checks` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 21 | `-Oz -fwrapv` | 1451 | 4461 | 4502/4524 | 0 | 0 | differs |
| 22 | `-Oz -fno-strict-overflow` | 1451 | 4461 | 4502/4524 | 0 | 0 | differs |
| 23 | `-Oz -mno-unaligned-access` | 1376 | 4441 | 4472/4524 | 0 | 0 | differs |
| 24 | `-Oz -fno-vectorize -fno-slp-vectorize` | 1484 | 4382 | 4408/4524 | 0 | 0 | differs |
| 25 | `-Oz -fno-inline-functions` | 1219 | 3511 | 3571/4524 | 0 | 276 | differs |
| 26 | `-Oz -fno-threadsafe-statics` | 1368 | 4492 | 4520/4524 | 3 | 0 | differs |
| 27 | `-Oz -fvisibility=hidden` | 57 | 243 | 244/4524 | 4280 | 0 | differs |
| 28 | `-Oz -fvisibility-inlines-hidden` | 1278 | 4203 | 4381/4524 | 141 | 0 | differs |
| 29 | `-Oz -mcpu=cortex-a8` | 1330 | 4225 | 4366/4524 | 0 | 0 | differs |
| 30 | `-Oz -marm` | 57 | 237 | 342/4524 | 0 | 0 | differs |
| 31 | `-Oz -std=gnu++11` | *build fail* | — | — | — | — | — |
| 32 | `-Oz -mfloat-abi=hard` | *build fail* | — | — | — | — | — |
| 33 | `-Oz -mfpu=vfpv3-d16` | *build fail* | — | — | — | — | — |
| 34 | `-Oz -fno-integrated-as` | *build fail* | — | — | — | — | — |
| 35 | `-Oz -fdata-sections` | 4333 | 4496 | 4524/4524 | 0 | 0 | differs |
| 36 | `-Oz -fno-data-sections` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 37 | `-Oz -fmerge-all-constants` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 38 | `-Oz -fno-zero-initialized-in-bss` | 1522 | 4486 | 4516/4524 | 0 | 0 | differs |
| 39 | `-Oz -mno-global-merge` | 1397 | 4486 | 4517/4524 | 0 | 0 | differs |
| 40 | `-Oz -fshort-wchar` | 4496 | 4496 | 4524/4524 | 0 | 0 | differs |
| 41 | `-Oz -fno-short-wchar` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 42 | `-Oz -fwritable-strings` | 1425 | 4487 | 4515/4524 | 0 | 0 | differs |
| 43 | `-Oz -DNDEBUG` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 44 | `-Oz -fno-plt` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 45 | `-Oz -fno-use-init-array` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 46 | `-Oz -mno-restrict-it` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 47 | `-Oz -fno-jump-tables` | 1363 | 4408 | 4436/4524 | 0 | 0 | differs |
| 48 | `-Oz -fno-optimize-sibling-calls` | 1316 | 3586 | 3686/4524 | 0 | 0 | differs |
| 49 | `-Oz -fno-inline-functions-called-once` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 50 | `-Oz -fno-unroll-loops` | 2883 | 4489 | 4520/4524 | 0 | 0 | differs |
| 51 | `-Oz -ffast-math` | 1319 | 4225 | 4369/4524 | 0 | 0 | differs |
| 52 | `-Oz -fno-math-errno` | 1356 | 4485 | 4514/4524 | 0 | 0 | differs |
| 53 | `-Oz -fno-trapping-math` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 54 | `-Oz -fno-signed-zeros` | 2355 | 4487 | 4521/4524 | 0 | 0 | differs |
| 55 | `-Oz -freciprocal-math` | 1539 | 4400 | 4483/4524 | 0 | 0 | differs |
| 56 | `-Oz -fno-finite-math-only` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 57 | `-Oz -faligned-allocation` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 58 | `-Oz -fno-aligned-allocation` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 59 | `-Oz -mno-movt` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 60 | `-Oz -mllvm -sink-insts-to-avoid-spills` | 1480 | 4344 | 4393/4524 | 0 | 0 | differs |
| 61 | `-Oz -mllvm -condsider-local-interval-cost` | 1773 | 4474 | 4507/4524 | 0 | 0 | differs |
| 62 | `-Oz -mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | *build fail* | — | — | — | — | — |
| 63 | `-Oz -mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | *build fail* | — | — | — | — | — |
| 64 | `-Oz -mllvm -disable-postra-machine-sink` | 1573 | 4391 | 4475/4524 | 0 | 0 | differs |
| 65 | `-Oz -mllvm -regalloc=fast` | 1099 | 1594 | 1654/4524 | 0 | 0 | differs |
| 66 | `-Oz -mllvm -inline-threshold=100` | 1350 | 3520 | 3554/4524 | 205 | 0 | differs |
| 67 | `-Oz -mllvm -inline-threshold=300` | 1350 | 3370 | 3397/4524 | 225 | 0 | differs |
| 68 | `-Oz -mllvm -unroll-threshold=0` | 4496 | 4496 | 4524/4524 | 0 | 0 | **identical** |
| 69 | `-Oz -mllvm -enable-machine-outliner=true` | *build fail* | — | — | — | — | — |
| 70 | `-Oz` (Radar scoped `-mllvm` removed) | 3632 | 4496 | 4523/4524 | 0 | 0 | differs |
| 71 | `-Oz -mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` (Radar scoped removed) | 4496 | 4496 | 4524/4524 | 0 | 0 | differs |
| 72 | `-Oz -mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` (Radar scoped removed) | 3263 | 4491 | 4519/4524 | 0 | 0 | differs |
| 73 | `-Oz -std=gnu++11` (C++ TUs only) | 4496 | 4496 | 4524/4524 | 0 | 0 | differs |
| 74 | `-Oz -std=gnu++14` (C++ TUs only) | 1512 | 4483 | 4514/4524 | 0 | 0 | differs |

## 4. Section / differing-byte table

Measured against `../_work/bins/android_2.0.16_libgof2hdaa.so` with `readelf -SW` +
a byte-by-byte section comparison (scratch baseline; the official build differs only in
the 20-byte build-id):

| section | size orig / ours | differing bytes |
|---|---:|---:|
| `.text` | `0x1337c8` / `0x1337c8` | **114,696** |
| `.ARM.exidx` | `0x6f70` / `0x6f70` | 2,707 |
| `.ARM.extab` | `0x26298` / **`0x26290`** | 121,226 |
| `.rodata` | `0x41b67` / `0x41b67` | **0** |
| `.data.rel.ro` | `0x2fe8` / `0x2fe8` | **0** |
| `.data` | `0x296c` / `0x296c` | **0** |
| `.bss` | `0x69ac` / `0x69ac` | n/a (no file bytes) |
| `.dynsym` | `0x15ca0` / `0x15ca0` | **0** |
| `.dynstr` | `0x2e318` / `0x2e318` | **0** |
| `.hash` | `0x973c` / `0x973c` | **0** |
| `.gnu.version` | `0x2b94` / `0x2b94` | **0** |
| `.rel.dyn` | `0x7048` / `0x7048` | **0** |
| `.rel.plt` | `0x72d0` / `0x72d0` | 12 |
| `.plt` | `0xac4c` / `0xac4c` | **0** |
| `.got` | `0x430c` / `0x430c` | **0** |
| `.dynamic` | `0x148` / `0x148` | **0** |
| `.init_array` | `0xc` / `0xc` | **0** |
| `.fini_array` | `0x8` / `0x8` | **0** |

Only `.ARM.extab` differs in size (`-8`), as documented; its differing-byte count is
over the common `0x26290`-byte prefix. The `.text` and `.ARM.extab` differing-byte counts
are now **114,696** and **121,226** (the older `docs/IDENTICALITY.md` text said 114,680 and
121,218; corrected in this commit).

## 5. Proven vs hypothesis

**Proven by measurement:**

* Baseline is 4496 / 4496 / 4524, `.text 0x1337c8`, data 843/843, lint CLEAN, drift 0.
* No tested global compile flag raises byte_exact above 4496.
* `-Oz` is uniquely best among the five optimization levels.
* `-fdata-sections` is a regression at the current tree (-163).
* The Radar-scoped `-mllvm` pair is load-bearing (+864 byte-exact).
* A global `80:1` is gate-neutral but not byte-equivalent to the scoped pair (760 bytes).
* The neutral list in §3.2 produces a byte-identical `.so` (modulo build-id where noted).
* `-fno-rtti`, `-fno-exceptions`, `-fno-common`, `-fno-builtin`, `-fvisibility=hidden`,
  `-marm`, `-mcpu=cortex-a8` etc. are wrong or fail to build.

**Hypothesis / limits:**

* The remaining gaps are source/TU/PLT/literal-pool/data-order, not a missing global
  flag. This is supported by the fact that no flag improves the count, but it is a
  negative result, not a positive reconstruction of the original command line.
* The original was built with ndk-build/LLVM 7.0.2 defaults plus the canonical set. The
  exact original command line is not recoverable from the stripped `.so`; the evidence is
  the `.comment`/`.note.android.ident` toolchain identity, the `.ARM.attributes`, the
  `.text` size, and the A/B counts.
* The fast verifier's `avg` is not comparable to the official fuzzy average.

## 6. Reproduction

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8

# one A/B variant (scratch; never --update-baseline)
cmake -S . -B /tmp/w3690/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3690/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections'
cmake --build /tmp/w3690/build --target link_so -j16
python3 tools/ratchet.py --build-dir /tmp/w3690/build --no-rebuild
```

The raw per-variant data (`matrix.jsonl`, 74 records), build/verify/ratchet logs,
per-variant `.so` files, and the `toolchain_extra.cmake` wrapper are under `/tmp/w3690/`
and are not part of the repository. The table in §3.6 is generated from `matrix.jsonl`.

## 7. Doc correction landed with this note

`docs/IDENTICALITY.md` previously stated `.text` differing bytes **114,680** and
`.ARM.extab` **121,218**; the current measurement is **114,696** and **121,226**
(the `.ARM.extab` size delta remains `-8`). The two numbers are corrected in the same
commit so the doc stays consistent with the gate.
