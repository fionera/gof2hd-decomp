# Flag matrix 2 — ELF forensics + a second compiler-flag A/B

Status: research note (2026-11-01). Worktree `wt/w3302`, branch `research-w3302`, base
`81db17d1`. Nothing in `src/` or `CMakeLists.txt` was changed in the committed tree; the
`cmake/toolchain.cmake` edit used for the A/B was reverted before this note was committed.

## 0. Baseline and reproduction

```
cmake --preset default
cmake --build cmake-build-match --target check -j32
```

At the start and end of this work the gate printed

```
verify: byte 1415  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` is not printed by `ratchet.py`; it is `sum(osize == usize)` over
`cmake-build-match/verify/report.json` = **4522 / 4523**. `.text` = **0x1343d0**
(original: **0x1337c8**).

`GOF2_EXTRA_FLAGS` is **not** honoured by master's `cmake/toolchain.cmake`. For every
Part-B variant the following two-line patch was applied (and reverted):

```diff
-    "-std=c++11 -fPIC -frtti -fstack-protector-strong ${GOF2_MATCH_OPT} -g \
+    "-std=c++11 -fPIC -frtti -fstack-protector-strong ${GOF2_MATCH_OPT} -g ${GOF2_EXTRA_FLAGS} \
...
-    "-fPIC -fstack-protector-strong ${GOF2_MATCH_OPT} -g \
+    "-fPIC -fstack-protector-strong ${GOF2_MATCH_OPT} -g ${GOF2_EXTRA_FLAGS} \
```

i.e. append `${GOF2_EXTRA_FLAGS}` immediately after `-g` in **both** `CMAKE_CXX_FLAGS`
and `CMAKE_C_FLAGS`. Then per variant:

```
cmake --preset default -DGOF2_EXTRA_FLAGS="<flag>"
cmake --build cmake-build-match --target link_so -j32
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
readelf -SW cmake-build-match/libgof2hdaa.so | grep '\.text'
readelf -A  cmake-build-match/libgof2hdaa.so
```

Caveat: `ratchet.py` only checks source mtimes, so after changing a flag you must build
`link_so` explicitly before `check`; otherwise the stale `.so` is re-verified.

---

## Part A — observable-to-flag map

`O` = `../_work/bins/android_2.0.16_libgof2hdaa.so`, `M` = `cmake-build-match/libgof2hdaa.so`.

| observable | original (O) | ours (M) | flag(s) that control it | match? |
|---|---|---|---|---|
| `.ARM.attributes` (all tags) | see below | byte-identical to O | `-march/-mthumb/-mfpu/-mfloat-abi/-fPIC/-fshort-wchar/-ffast-math/-fno-trapping-math/-mfp16-format` | **YES, byte-identical** |
| `.comment` | clang 7.0.2 + GCC 4.9.x | byte-identical | compiler/NDK identity | **YES** |
| `.note.android.ident` | `r18b`, build `5063045` | byte-identical | NDK r18b clang driver | **YES** |
| `.note.gnu.gold-version` | gold 1.12 | byte-identical | gold linker version | **YES** |
| `.note.gnu.build-id` | `c011810d…d310` | `2599f222…d72f` | content hash (`--build-id`) | expected diff |
| `.init_array` | 3 entries: `0x69441 0x694c1 0x694dd` | **same 3, same bytes** | `-fuse-init-array` (default) | **YES** |
| `.fini_array` | 2 entries: `0x69548 0x69534` | **same 2, same bytes** | crtbegin/`-fuse-init-array` | **YES** |
| stack-protector canaries | 757 / 4524 functions | 757 / 4524, **set identical** | `-fstack-protector-strong` | **YES** |
| `__aeabi_*` symbol set | 21 names | identical | `-mfloat-abi`, libgcc | **YES** |
| `__cxa_*` symbol set | identical (see below) | identical | `-fexceptions`, libc++abi | **YES** |
| `_Unwind_*` symbol set | 7 names | identical | `-funwind-tables`/`-fexceptions` | **YES** |
| `movw` / `movt` | 9014 / 15 | 9204 / 20 | `-mno-movt` (no effect at `-Oz`), literal pools | counts differ with source |
| `ldrd` / `strd` | 2056 / 4612 | 2047 / 4574 | `-mno-ldrd` (not accepted by clang 7) | counts differ with source |
| `vld1` / `vst1` | 309 / 867 | 307 / 861 | `-mfpu=neon`, vectorizer | counts differ with source |
| `vld2` / `vst2` | 28 / 28 | 32 / 32 | `-mfpu=neon` | counts differ with source |
| `tbb` / `tbh` | 68 / 84 | 69 / 84 | `-fno-jump-tables` | ~match |
| `udiv` / `sdiv` | 0 / 0 | 0 / 0 | `-march=armv7-a` (no idiv) → `__aeabi_uidiv` | **YES** |
| `.plt` | 44108 B (3674 entries + PLT0) | 44108 B | `-fPIC`, `--exclude-libs` | **YES** |
| `.got` | 17164 B | 17164 B | `-fPIC` | **YES** |
| `.rel.plt` / `.rel.dyn` | 29392 / 28744 B | 29392 / 28744 B | `-fPIC`, link order | **YES** |
| relocation list (type,symbol) | 1817 ABS32 / 614 GLOB_DAT / 3674 JUMP_SLOT / 1162 RELATIVE | **identical sorted list** | `-fPIC`, link order | **YES** |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | **absent** | link `-z now` | **NO → add `-z now`** |
| `.aezdata` | absent | 8-byte section `.aez\0\0\0\0\0` | source `#pragma clang section data=".aezdata"` (`src/engine/file/AEFile.cpp:7`) | not a flag |

### `.ARM.attributes` (every tag)

Both files decode to exactly:

```
Tag_CPU_name: "ARM v7"          Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes            Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3              Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_GOT_use: GOT-indirect
Tag_ABI_PCS_wchar_t: 4          Tag_ABI_FP_denormal: Needed
Tag_ABI_FP_exceptions: Needed   Tag_ABI_FP_number_model: IEEE 754
Tag_ABI_align_needed: 8-byte    Tag_ABI_enum_size: int
Tag_ABI_HardFP_use: Deprecated  Tag_CPU_unaligned_access: v6
Tag_ABI_FP_16bit_format: IEEE 754
```

`readelf -x .ARM.attributes` is **byte-for-byte identical** between O and M.
Controlling flags: `-march=armv7-a` (arch/profile), `-mthumb` (Thumb-2),
`-mfpu=neon` (VFPv3/NEONv1), `-mfloat-abi=softfp` (HardFP_use Deprecated),
`-fPIC` (GOT-indirect), `-fshort-wchar` (wchar_t 2 vs 4), `-ffast-math`
(denormal/exceptions/number-model), `-fno-trapping-math` (FP_exceptions Unused),
`-mfp16-format` (FP_16bit_format), default `-fno-short-enums` (enum int),
default `-munaligned-access` (unaligned v6).

**Important measurement caveat:** the final `.so` attribute is the *merge* over every
linked object, and the vendored zlib/libzip/openssl + crt/libgcc/libc++abi archives are
built without `GOF2_EXTRA_FLAGS`. A flag that changes only app TUs is therefore masked in
the final `.so`. Demonstrated: compiling `Matrix.cpp` with `-fshort-wchar` gives the object
`Tag_ABI_PCS_wchar_t: 2`, but the linked `.so` still reports `4`. Likewise `-ffast-math`
gives an object with `denormal: Sign only / exceptions: Unused / number_model: Finite`,
while the linked `.so` still reports `Needed / Needed / IEEE 754`. So for Part B the
attribute check must be done on an app object (or by comparing the final `.so` only when
the flag is applied to every input). All Part-B variants produced the *same final*
`.ARM.attributes` hash `4dcf7448b3fc`; the disqualifying attribute evidence is the
object-level one noted in the table.

### `.ARM.exidx` / `.ARM.extab` coverage

| metric | original | ours |
|---|---:|---:|
| `.ARM.exidx` bytes | 28528 (0x6f70) | 26312 (0x66c8) |
| exidx entries | 3566 | 3289 |
| unique entry addresses | 3372 | 3288 |
| duplicate-address entries | **194** | 1 |
| compact (inline) entries | 856 | 842 |
| extab-pointer entries | 2710 | 2447 |
| `.ARM.extab` bytes | 156312 | 156324 |
| exported funcs with an exidx entry | 3310 / 4524 | 3286 / 4524 |
| exidx entries not at an exported func start | 406 | 333 |
| per-function coverage vs O (common 4524) | — | 3285 both, 25 O-only, 1 M-only |

Flag: **`-funwind-tables` is ON in the original.** Removing it from
`target_compile_options(gof2 ...)` (temporarily) gives:

| build | byte | linked | size | .text | exidx entries | compact | extab bytes |
|---|---:|---:|---:|---:|---:|---:|---:|
| with `-funwind-tables` (baseline) | 1415 | 4493 | 4522 | 0x1343d0 | 3289 | 842 | 156324 |
| without `-funwind-tables` | **1414** | 4493 | 4522 | 0x1343d0 | 3152 | **417** | 153024 |
| original | — | — | — | 0x1337c8 | 3566 | 856 | 156312 |

So the flag is confirmed both by the compact-entry count (842 vs 856; without it 417) and
by a 1-function byte-exact loss.

Per-function coverage deltas (common exported functions):

```
ORIG-ONLY (25):  Java_..._GetLeaderboardScore, _Z7glErrorv,
  _ZN10HangarListC1Ev, _ZN10HangarListC2Ev,
  _ZN11AbyssEngine10QuaternionC1Ev, _ZN11AbyssEngine10QuaternionC2Ev,
  _ZN11AbyssEngine13DrawFBOShaderC1Ev, _ZN11AbyssEngine13DrawFBOShaderC2Ev,
  _ZN11AbyssEngine18IApplicationModule21SetApplicationManagerEPNS_18ApplicationManagerE,
  _ZN11AbyssEngine6AEMath6MatrixcvPfEv,
  _ZN11ModMainMenuC1Ev, _ZN11ModMainMenuC2Ev, _ZN3NFCC1Ev, _ZN3NFCC2Ev,
  _ZN9GeneratorC1Ev, _ZN9GeneratorC2Ev, _ZN9ObjectiveC1EiiP5Level,
  _ZN9ObjectiveC2EiiP5Level, _ZNSt9type_infoD1Ev, _ZNSt9type_infoD2Ev,
  _ZSt14set_unexpectedPFvvE, inflateReset, zip_get_archive_comment,
  zip_set_archive_flag, zip_strerror
OURS-ONLY (1):   __cxa_end_cleanup
```

17 of the 25 ORIG-ONLY names are byte-exact functions (the rest differ in source shape),
and most are C1/C2 constructor pairs — exactly the alias pattern that produces the
duplicate-address entries.

The residual is **not** a flag: the original carries 194 duplicate-address entries, and
193 of the 194 pairs are `(cantunwind, real)` (146 `(cant, extab)`, 47 `(cant, compact)`,
1 `(compact, extab)`). That is the signature of two input sections contributing an unwind entry for one
address — i.e. a duplicate/aliased definition where one copy was compiled without unwind
tables (the original source has known ODR duplicates; AGENTS.md records a Vector/Matrix
operator triplicate). Our tree emits one exidx entry per function. Link-level attempts to
reproduce it did **not** match: `--no-merge-exidx-entries` → 4536 entries / 4535 unique;
`--icf=all` → 2801; `--icf=safe` → 2873; `--icf=all --no-merge-exidx-entries` → 3599.
None produces the 194 `(cant, real)` same-address pairs. This is a source/TU/ODR artifact,
not a compiler flag.

### Stack-protector canaries

Disassembling every function and looking for `__stack_chk_fail` / `__stack_chk_guard`:

```
orig protected: 757 of 4524
ours protected: 757 of 4524
common: 4524  both: 757  orig-only: 0  ours-only: 0
```

`-fstack-protector-strong` reproduces the original's heuristic **exactly**, function for
function. `-fstack-protector` (1344/3784) and `-fno-stack-protector` (1344/3780) are both
far worse.

### `__cxa_*` / `__aeabi_*` / `_Unwind_*`

- `__aeabi_*`, `__cxa_*`, `_Unwind_*` name sets are **identical** between O and M.
- `__cxa_guard_acquire/release/abort` are **defined** in both (from `libc++abi.a`, exported
  because libc++abi is not `--exclude-libs`'d), not imported.
- `__cxa_atexit` and `__cxa_finalize` are **undefined imports** in both.
- `__gxx_personality_v0`, `__cxa_throw`, `__cxa_begin_catch`, `__cxa_end_catch`,
  `__cxa_allocate_exception`, `__cxa_free_exception`, `__cxa_pure_virtual` are defined in
  both at their libc++abi addresses.
- `__aeabi_unwind_cpp_pr0/pr1`, `__gnu_unwind_frame` are undefined imports in both.

### Section flags / alignment / entsize

All sections have the same `sh_flags`, `sh_addralign` and `sh_entsize` in O and M. Sizes:

| section | orig | ours | note |
|---|---:|---:|---|
| `.text` | 1259464 (0x1337c8) | 1262544 (0x1343d0) | ours +3080 B |
| `.ARM.exidx` | 28528 | 26312 | see above |
| `.ARM.extab` | 156312 | 156324 | +12 |
| `.rodata` | 269159 (0x41b67) | 268103 (0x41747) | ours −1056 |
| `.data.rel.ro` | 12264 | 12296 | +32 |
| `.data` | 10604 | 10604 | = |
| `.bss` | 27052 | 27096 | +44 |
| `.dynamic` | 328 (36 entries) | 312 (34) | missing `DT_FLAGS/_1` |
| `.dynsym` | 89248 | 89248 | = |
| `.dynstr` | 189208 | 189208 | = |
| `.hash` | 38716 | 38716 | = |
| `.gnu.version` | 11156 | 11156 | = |
| `.rel.dyn` | 28744 | 28744 | = |
| `.rel.plt` | 29392 | 29392 | = |
| `.plt` | 44108 | 44108 | = |
| `.got` | 17164 | 17164 | = |
| `.init_array` / `.fini_array` | 12 / 8 | 12 / 8 | identical contents |
| `.comment` | 303 | 303 | identical |
| `.ARM.attributes` | 56 | 56 | identical |
| `.shstrtab` | 279 | 288 | ours has `.aezdata` |
| `.aezdata` | — | 8 | source pragma, not a flag |

Section flags/align: `.text` AX/32, `.ARM.exidx` AL/4, `.ARM.extab` A/4, `.rodata` A/16,
`.data.rel.ro` WA/16, `.data` WA/16, `.bss` WA/16, `.got` WA/4, `.plt` AX/4,
`.rel.dyn` A/4 entsize 8, `.rel.plt` AI/4 entsize 8, `.init_array` WA/4 entsize 4,
`.fini_array` WA/4 entsize 4, `.dynsym` A/4 entsize 16, `.comment` MS/1,
`.ARM.attributes` (none)/1. All identical.

### PLT / GOT

`.plt` is 44108 B = PLT0 (20 B) + 3674×12 B in both. `.rel.plt` has exactly 3674
`R_ARM_JUMP_SLOT`. `.rel.dyn` has 1817 `R_ARM_ABS32`, 614 `R_ARM_GLOB_DAT`,
1162 `R_ARM_RELATIVE` in both. The sorted `(relocation type, symbol name)` list is
**identical** (0 diff lines). So the entire PLT/GOT relocation set matches; only the
`DT_FLAGS`/`DT_FLAGS_1` entries are missing (see below).

### `DT_FLAGS` — a real link-flag gap

```
O: 0x0000001e (FLAGS)   BIND_NOW
   0x6ffffffb (FLAGS_1) Flags: NOW
M: (absent)
```

Adding `-z now` to the gold link (temporarily) produces exactly the original's two entries
and leaves `.text` / `.ARM.exidx` unchanged. This is a **required** link flag, not a
compiler flag. It is not part of the current `CMakeLists.txt`.

---

## Part B — measured A/B matrix

Baseline row: **1415 / 4493 / 4522 / 0x1343d0**, `.ARM.attributes` hash `4dcf7448b3fc`.
`size` is size-exact out of 4523 compared functions. `.text` is the linked `.so` `.text`
size. `attrs` is the final `.so` `.ARM.attributes` hash (all `4dcf7448b3fc` unless noted).

| # | flag | byte | linked | size | .text | attrs | verdict |
|---:|---|---:|---:|---:|---|---|---|
| 0 | *(baseline)* | 1415 | 4493 | 4522 | 0x1343d0 | 4dcf7448b3fc | — |
| 1 | `-fno-semantic-interposition` | — | — | — | — | — | **BUILD FAIL**: clang 7 unknown argument |
| 2 | `-fvisibility-inlines-hidden` | 1276 | 4201 | 4380 | 0x134390 | same | worse; `missing=140` (hides inline exports) |
| 3 | `-faligned-allocation` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 4 | `-fno-aligned-allocation` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 5 | `-fmerge-all-constants` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 6 | `-fno-merge-constants` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 7 | `-fno-use-init-array` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral; `.init_array` still 3 entries |
| 8 | `-fno-c++-static-destructors` | — | — | — | — | — | **BUILD FAIL**: clang 7 unknown argument |
| 9 | `-mno-movt` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral (movt 20 unchanged; `-Oz` uses literal pools) |
| 10 | `-mno-ldrd` | — | — | — | — | — | **BUILD FAIL**: clang 7 unknown argument |
| 11 | `-fno-jump-tables` | 1344 | 4407 | 4437 | 0x13547e | same | worse |
| 12 | `-fno-optimize-sibling-calls` | 1314 | 3583 | 3684 | 0x1321a4 | same | much worse |
| 13 | `-fno-inline-functions-called-once` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 14 | `-fno-delete-null-pointer-checks` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral (matches prior note) |
| 15 | `-mllvm -inline-threshold=100` | 1339 | 3520 | 3553 | 0x14eb60 | same | worse; `missing=205` |
| 16 | `-mllvm -inline-threshold=300` | 1339 | 3384 | 3410 | 0x1583c0 | same | worse; `missing=216` |
| 17 | `-mllvm -unroll-threshold=0` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 18 | `-fno-unroll-loops` | 1384 | 4486 | 4518 | 0x134390 | same | slightly worse |
| 19 | `-ffast-math` | 1306 | 4224 | 4368 | 0x133580 | same¹ | worse; ¹object FP tags change → disqualified |
| 20 | `-fno-math-errno` | 1343 | 4482 | 4512 | 0x134220 | same | worse |
| 21 | `-fno-trapping-math` | 1415 | 4493 | 4522 | 0x1343d0 | same¹ | neutral codegen; ¹object `FP_exceptions: Unused` → disqualified |
| 22 | `-fno-signed-zeros` | 1403 | 4484 | 4519 | 0x134390 | same | slightly worse |
| 23 | `-freciprocal-math` | 1364 | 4398 | 4483 | 0x134410 | same | worse |
| 24 | `-fno-finite-math-only` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 25 | `-mfp16-format=ieee` | — | — | — | — | — | **BUILD FAIL**: clang 7 unknown argument |
| 26 | `-fshort-wchar` | 1415 | 4493 | 4522 | 0x1343d0 | same¹ | neutral codegen; ¹object `wchar_t: 2` → disqualified |
| 27 | `-fno-short-wchar` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral |
| 28 | `-fno-sized-deallocation` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral (matches prior note) |
| 29 | `-fno-threadsafe-statics` | 1347 | 4489 | 4518 | 0x13428c | same | worse; `missing=3` |
| 30 | `-fno-rtti` | 1375 | 4493 | 4522 | 0x1343d0 | same | worse byte (matches prior note) |
| 31 | `-fno-stack-protector` | 1344 | 3780 | 3788 | 0x12ec50 | same | much worse |
| 32 | `-fstack-protector` | 1344 | 3784 | 3792 | 0x12ecd0 | same | much worse |
| 33 | `-fno-omit-frame-pointer` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral (default already keeps FPs) |
| 34 | `-mthumb-interwork` | — | — | — | — | — | **BUILD FAIL**: clang 7 unknown argument |
| 35 | `-mno-thumb-interwork` | — | — | — | — | — | **BUILD FAIL**: clang 7 unknown argument |
| 36 | `-fno-exceptions` | — | — | — | — | — | **BUILD FAIL**: `try` in `Level.cpp:2628` |
| 37 | `-fno-asynchronous-unwind-tables` | 1415 | 4493 | 4522 | 0x1343d0 | same | neutral (exidx/extab unchanged) |
| 38 | `-mllvm -enable-unsafe-fp-math` | — | — | — | — | — | **BUILD FAIL**: LLVM 7 unknown option |
| 39 | `-mllvm -enable-no-infs-fp-math` | — | — | — | — | — | **BUILD FAIL**: LLVM 7 unknown option |

Notes:
- `-fno-semantic-interposition`, `-mno-ldrd`, `-mfp16-format=ieee`, `-mthumb-interwork`,
  `-mno-thumb-interwork`, `-fno-c++-static-destructors` are **not accepted by NDK r18b
  clang 7.0.2**, so none of them can have been an original build flag.
- `-mllvm -enable-unsafe-fp-math` / `-enable-no-infs-fp-math` are LLVM 7 option names that
  do not exist (clang reports `Unknown command line argument`), so they cannot have been
  used either.
- `-fvisibility-inlines-hidden` is the only "near" variant that changes the symbol set
  (`missing=140`); it is not the original (original exports those inline functions).
- `-fno-use-init-array` is a no-op on the final `.init_array` in this tree: it still emits
  the same 3 entries at the same addresses.

### Extra link-level A/B (not compiler flags, but part of the ELF story)

Applied temporarily to the gold link command in `CMakeLists.txt`; verify counts are
unchanged unless noted (these are link-time only).

| link flag | .text | exidx entries | verdict |
|---|---:|---:|---|
| *(baseline)* | 0x1343d0 | 3289 | — |
| `-z now` | 0x1343d0 | 3289 | **matches O's `DT_FLAGS`/`DT_FLAGS_1`; required** |
| `--no-merge-exidx-entries` | 0x1343d0 | 4536 | over-produces (O 3566) |
| `--icf=all` | 0x1305a8 | 2801 | over-folds (O 0x1337c8) |
| `--icf=safe` | 0x131570 | 2873 | over-folds |
| `--icf=all --no-merge-exidx-entries` | 0x1305a8 | 3599 | entries near O, .text far off |

`-z now` is the one link flag that is clearly part of the original link and is currently
missing from `CMakeLists.txt`.

---

## What is now proven / what is still unprovable

### Proven (or strongly evidenced) by this pass

1. **Global compiler flags.** `-Oz`, `-g`, `-std=c++11`, `-fPIC`, `-frtti`,
   `-fstack-protector-strong`, `-funwind-tables`,
   `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`, default strict aliasing,
   frame pointers kept, vectorization on, default inlining/unrolling, threadsafe statics
   on, exceptions on, and no fast-math are all confirmed by neutral-or-worse A/B.
2. **`-fstack-protector-strong` is exact**: the canary set is 757/757 identical
   function-for-function. `-fstack-protector` and `-fno-stack-protector` are much worse.
3. **`-funwind-tables` is on**: removing it drops compact exidx entries 842→417 and
   byte_exact 1415→1414.
4. **`.ARM.attributes` and `.comment` are byte-identical**; `.note.android.ident` and
   `.note.gnu.gold-version` are byte-identical; only the build-id differs.
5. **`.init_array` / `.fini_array` match exactly** (3 and 2 entries, same addresses).
   This corrects the stale AGENTS.md / `build_flags_research.md` claim "original 3,
   ours 1": the current tree is 3 vs 3.
6. **The entire PLT/GOT relocation set matches** (same sizes, same 7267 relocations,
   same sorted `(type, symbol)` list).
7. **`__aeabi_*` / `__cxa_*` / `_Unwind_*` symbol sets match exactly**; `__cxa_guard_*`
   are defined (libc++abi), `__cxa_atexit`/`__cxa_finalize` are imported, in both.
8. **`DT_FLAGS = BIND_NOW` / `DT_FLAGS_1 = NOW` is a real missing link flag** (`-z now`).
   It reproduces the original's dynamic entries and does not change code.
9. **Eight candidate flags cannot have been used** because NDK r18b clang 7.0.2 rejects
   them: `-fno-semantic-interposition`, `-mno-ldrd`, `-mfp16-format=ieee`,
   `-mthumb-interwork`, `-mno-thumb-interwork`, `-fno-c++-static-destructors`,
   `-mllvm -enable-unsafe-fp-math`, `-mllvm -enable-no-infs-fp-math`.
10. **No tested compiler flag improves the gate.** Every accepted variant is neutral or
    worse; the best accepted variants are exactly neutral (1415/4493/4522/0x1343d0).

### Still unprovable from flags alone

1. **The 3080-byte `.text` gap** (0x1343d0 vs 0x1337c8) is source/TU-shape, not a flag.
   `--icf=all` over-folds by ~12.8 KB, so simple ICF is not the explanation.
2. **The 194 duplicate-address exidx entries** in the original, all `(cantunwind, real)`.
   They are consistent with ODR-duplicate/aliased definitions where one copy was compiled
   without unwind tables, but no compiler flag or tested link flag reproduces them in our
   tree. This is the most concrete open structural clue in `.ARM.exidx`.
3. **`.rodata` −1056 B, `.data.rel.ro` +32 B, `.bss` +44 B** differences: data/TU layout.
4. **Exact TU membership and link order** of the original: the section-ordering file
   reproduces `.text` order, but `.rodata`/literal pools/`__GLOBAL__sub_I_*` follow TU
   ownership, which is still the known open work item.
5. **The `.aezdata` extra section** in ours is a source artifact
   (`#pragma clang section data=".aezdata"` in `AEFile.cpp`), not a flag; the original has
   that 8-byte blob elsewhere (`"aez"` string is in the original's `.rodata`).
6. **Whether the original used `--allow-multiple-definition` / ICF** cannot be settled
   from the flag matrix; the exidx duplicate pattern is the only evidence, and our
   controlled ICF links do not reproduce it.
