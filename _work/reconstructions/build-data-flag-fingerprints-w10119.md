# Build research: source-level data/layout flag fingerprints (wave 155, w10119)

Status: **note + harness only**, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10119`,
base `f4e0ea40b` ("docs wave 154"). Scratch `/dev/shm/w10119`. No `src/`, `tools/`,
`CMakeLists.txt`, baseline, allowlist or doc change is landed: **no flag is a strict
improvement**. This note and `build_data_flag_fingerprint_w10119.py` are the only commit.

## 0. Bottom line

* Baseline (fresh `check` in this worktree): **byte 4507 / linked 4507 / size 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS**.
  Residual raw same-address diff over the 17 non-byte-exact functions: **92,239 B**.
  Instruction-index alignment over the 15 aligned functions: **23,941** (17-row sum 25,959).
  Sections: `.text` 1,259,464 (86,347 diff), `.ARM.exidx` 28,528 (**0 diff**),
  `.ARM.extab` 156,312 (1,255 diff), `.rodata` 269,159 (**0**), `.data.rel.ro` 12,264 (**0**),
  `.data` 10,604 (**0**), `.bss` 27,052.
* **24 cells measured** (21 wrapper + 3 launcher). **No cell exceeds 4507.** Nine cells are
  *byte-identical to the control in every allocatable section* (`.text`, `.ARM.exidx`,
  `.ARM.extab`, `.rodata`, `.data.rel.ro`, `.data`, `.bss`): `-fmerge-all-constants`,
  `-fno-merge-all-constants`, `-fno-merge-constants`, `-fno-omit-frame-pointer`,
  `-fno-sized-deallocation`, `-fno-unwind-tables` (wrapper, see §2), and
  `-fno-asynchronous-unwind-tables` (wrapper **and** launcher). They are unobservable, not
  evidence that the original used them.
* **New flag-ordering trap found.** `CMakeLists.txt` appends
  `target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)` *after*
  `CMAKE_*_FLAGS`, so a wrapper `-fno-unwind-tables` is silently overridden. A launcher that
  appends the flag at the very end of the compile command makes it observable:
  **byte 4507 → 2878, raw 92,239 → 92,781, aligned 23,941 → 25,022, `.ARM.exidx`
  28,528 → 26,232 (−2,296), `.ARM.extab` 156,312 → 152,928 (−3,384)**. This is the first
  positive A/B witness that `-funwind-tables` is on for every TU (the `.ARM.exidx` size is
  the direct artifact).
* `-fno-asynchronous-unwind-tables` stays neutral even at the end of the line: ARM EHABI
  uses the *synchronous* tables, so the flag cannot change this target's output.
* `-fno-sized-deallocation` is neutral; `-fsized-deallocation` is a regression
  (1347/3876, `.text` +5,232, `.ARM.extab` +8). The original is the C++11 default
  (sized deallocation **off**).
* `-fstack-protector-strong` is uniquely correct: plain `-fstack-protector` (1377/3786,
  `.text` −22,048) and `-fstack-protector-all` (91/969, `.text` +114,592) are both wrong;
  `-fno-stack-protector` (1377/3783) is wrong too.
* The data sections are already byte-identical, so the only flags that *change* them are
  falsifications: `-fno-rtti` (`.rodata` −2,841, `.data.rel.ro` −972),
  `-fno-exceptions` (`.rodata` −16, `.ARM.extab` −125,580), `-fno-threadsafe-statics`
  (`.rodata` −432, `.text` −324, 3 symbols missing), `-fvisibility=hidden`
  (`.data` +192, `.bss` −128, 4,280 symbols hidden). Every other cell keeps
  `.rodata`/`.data.rel.ro`/`.data`/`.bss` size- and byte-exact.

## 1. Method

The harness is `_work/reconstructions/build_data_flag_fingerprint_w10119.py` (committed with
this note). It drives one scratch build directory per flag-ordering group:

* **wrapper** cells: a toolchain wrapper includes `cmake/toolchain.cmake` and appends the
  cell flag to `CMAKE_CXX_FLAGS`/`CMAKE_C_FLAGS` after the canonical `-Oz -g` line.
* **launcher** cells: `CMAKE_{C,CXX}_COMPILER_LAUNCHER` points at a 3-line shell script that
  appends `$GOF2_EXTRA_FLAGS` at the end of the compiler command, i.e. after
  `target_compile_options`. Needed only for `-fno-unwind-tables` /
  `-fno-asynchronous-unwind-tables`.
* For every cell: delete the 192 app `.obj` files, rebuild `link_so` (vendored archives are
  reused — they are compiled with their own flags), run the official
  `tools/verify/verify.py`, and compare every section against the original. `raw` is the
  same-address differing-byte sum over the 17 residual functions; `aligned` is
  `_work/reconstructions/align_diff.py` (branch targets normalised to instruction indices),
  reported both as the 17-row sum and the doc's 15-function sum (Layout C2 and ModStation C2
  aliases excluded).
* `check` was re-run at the end of the pass: still **4507/4507, RATCHET PASS**.

## 2. Fingerprint table (all cells, wave-154 tree)

`byte`/`linked` are out of 4,524. `raw` = 17-function same-address diff sum (control 92,239).
`align15` = 15-function aligned total (control 23,941). Section columns are **size**
(diff bytes in parentheses when non-zero); `—` means unchanged from control.

| cell | byte | linked | raw | align15 | `.text` size (diff) | `.ARM.exidx` size (diff) | `.ARM.extab` size (diff) | `.rodata` | `.data.rel.ro` | `.data` | `.bss` | verdict |
|---|---:|---:|---:|---:|---|---|---|---|---|---|---|---|
| control | **4507** | 4507 | 92,239 | 23,941 | 1,259,464 (86,347) | 28,528 (0) | 156,312 (1,255) | 269,159 | 12,264 | 10,604 | 27,052 | canonical |
| `-fmerge-all-constants` | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **unobservable** |
| `-fno-merge-all-constants` | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **unobservable** |
| `-fno-merge-constants` | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **unobservable** (clang 7 ignores) |
| `-fno-common` | — | — | — | — | link error | | | | | | | **link error** |
| `-fshort-enums` | 2871 | 4485 | 94,429 | 24,287 | 1,259,464 (851,504) | 28,528 (1,880) | 156,312 (1,285) | — | — | — | — | falsified |
| `-fno-exceptions` | 1344 | 3761 | 136,500 | 50,207 | 1,168,772 (1,132,239) | 28,520 (19,345) | 30,732 (26,263) | 269,143 | 12,264 (3,069) | 10,604 (224) | — | falsified |
| `-fno-rtti` | 1701 | 4507 | 95,345 | 24,444 | 1,259,464 (178,142) | 28,528 (1,272) | 156,312 (3,227) | 266,318 | 11,292 (4,529) | 10,604 (149) | — | falsified |
| `-fvisibility=hidden` | 57/244 | 243 | n/a | n/a | 1,249,080 (1,215,026) | 28,528 (9,632) | 156,288 (131,583) | — | 12,264 (7,085) | 10,796 (2,112) | 26,924 | falsified (4,280 missing) |
| `-fvisibility-inlines-hidden` | 1278/4383 | 4210 | 97,768 | 24,489 | 1,259,432 (892,986) | 28,528 (4,281) | 156,308 (7,174) | — | — | — | — | falsified (141 missing) |
| `-fstack-protector` | 1377 | 3786 | 137,927 | 35,973 | 1,237,416 (1,202,362) | 28,440 (16,579) | 155,772 (142,609) | — | 12,264 (1,903) | 10,604 (152) | — | falsified |
| `-fstack-protector-all` | 91 | 969 | 103,559 | 26,685 | 1,374,056 (1,228,251) | 29,232 (19,561) | 159,932 (140,239) | — | 12,264 (2,922) | 10,604 (224) | — | falsified |
| `-fno-stack-protector` | 1377 | 3783 | 137,940 | 35,975 | 1,237,288 (1,202,849) | 28,440 (16,583) | 155,772 (142,608) | — | 12,264 (1,903) | 10,604 (152) | — | falsified |
| `-fno-omit-frame-pointer` | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **unobservable** |
| `-mfloat-abi=hard` | — | — | — | — | link error (VFP args) | | | | | | | **link error** |
| `-fno-unwind-tables` (wrapper) | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **overridden** |
| `-fno-unwind-tables` (launcher) | **2878** | 4507 | 92,781 | 25,022 | 1,259,464 (102,275) | 26,232 (17,435) | 152,928 (140,142) | — | 12,264 (622) | 10,604 (145) | — | falsified |
| `-fno-asynchronous-unwind-tables` (wrapper) | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **unobservable** |
| `-fno-asynchronous-unwind-tables` (launcher) | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **unobservable** |
| `-fno-threadsafe-statics` | 1368/4521 | 4492 | 95,682 | 24,681 | 1,259,140 (1,100,891) | 28,496 (10,529) | 156,252 (130,629) | 268,727 | 12,264 (1,655) | 10,604 (148) | — | falsified (3 missing) |
| `-fno-use-cxa-atexit` | 1356 | 4506 | 95,342 | 24,505 | 1,259,544 (1,184,116) | 28,544 (13,568) | 156,300 (3,799) | — | 12,264 (1,212) | 10,604 (77) | — | falsified |
| `-fno-sized-deallocation` | 4507 | 4507 | 92,239 | 23,941 | — | — | — | — | — | — | — | **unobservable** |
| `-fsized-deallocation` | 1347 | 3876 | 107,917 | 25,360 | 1,264,696 (1,207,254) | 28,528 (8,867) | 156,320 (133,776) | — | 12,264 (1,975) | 10,604 (149) | — | falsified |

Notes:

* `-fno-common` fails at link: `multiple definition of 'g_android_back_button_pressed'`
  (`GOF2HD2012.c.obj` vs `BackButton.c.obj`). The original used clang-7's default `-fcommon`.
  (w3562 measured 4465/4496 after temporarily deleting one tentative definition; still −31.)
* `-mfloat-abi=hard` fails at link: `... uses VFP register arguments, output does not` — the
  crt/vendored archives are softfp. The original ELF flags are `EF_ARM_ABI_FLOAT_SOFT`.
* `-fvisibility=hidden` and `-fvisibility-inlines-hidden` hide dynsym entries, so their
  `byte`/`linked` rows are over a reduced symbol set (244 and 4,383 compared) and are not
  like-for-like gate comparisons. `-fvisibility-inlines-hidden` hiding 141 symbols is direct
  evidence the original exports its inline functions (default visibility).
* `-fno-threadsafe-statics` drops the three `__cxa_guard_*` exports (missing 3), matching
  w3562's earlier result at the new tree state.

## 3. What each flag fingerprints

| flag | original artifact that would move | measured | conclusion |
|---|---|---|---|
| `-fmerge-all-constants` / `-fno-merge-all-constants` / `-fno-merge-constants` | `.rodata` merges, literal-pool addresses | byte-identical | original consistent with default constant merging; the flag cannot be excluded but has no positive evidence |
| `-fno-common` | tentative-definition layout | link error | original used `-fcommon` |
| `-fshort-enums` | enum width, `.ARM.attributes` `Tag_ABI_enum_size` | 2871/4485, `.text` diff 851,504 | original enums are `int` (default) |
| `-fno-exceptions` | `.ARM.extab`/`.ARM.exidx`, EH cleanup | extab 156,312→30,732 | original uses exceptions |
| `-fno-rtti` | `_ZTI`/`_ZTS`, `.rodata`, `.data.rel.ro` vtables | rodata −2,841, rel.ro −972 | original uses RTTI |
| `-fvisibility=hidden` | `.dynsym` export set | 4,280 hidden | original exports everything (default visibility) |
| `-fvisibility-inlines-hidden` | inline-function exports | 141 hidden | original exports inline functions |
| `-fstack-protector` / `-all` / `-no` | `__stack_chk_fail` sites, `.text` | 788 canary sites match `-strong` only | original is `-fstack-protector-strong` |
| `-fno-omit-frame-pointer` | frame-pointer prologues | byte-identical | frame pointers are already on (default) |
| `-mfloat-abi=hard` | ELF flags, `.ARM.attributes` `Tag_ABI_VFP_args` | link error | original is softfp |
| `-fno-unwind-tables` | `.ARM.exidx`/`.ARM.extab` size | exidx −2,296, extab −3,384 | original has `-funwind-tables` on every TU |
| `-fno-asynchronous-unwind-tables` | `.ARM.exidx` async entries | byte-identical even at end of line | ARM EHABI emits only synchronous tables; flag is unobservable |
| `-fno-threadsafe-statics` | `__cxa_guard_*`, `.rodata`/`.text` | 3 symbols missing | original uses threadsafe statics |
| `-fno-use-cxa-atexit` | `__cxa_atexit`/`__dso_handle`, `.text` | 1356/4506 | original uses `__cxa_atexit` |
| `-fno-sized-deallocation` / `-fsized-deallocation` | sized `operator delete` calls | off neutral, on 1347/3876 | original is C++11 default (sized deallocation off) |

## 4. Proven / not settled

**Proven at `f4e0ea40b`:**

1. Baseline is byte 4507 / linked 4507 / size 4524/4524, data 843/843, raw 92,239,
   aligned 23,941; `.text` 86,347 diff, `.ARM.extab` 1,255 diff, `.ARM.exidx` 0.
2. No tested flag raises `byte_exact` above 4507. The neutral cells are byte-identical to
   the control in every allocatable section, so they cannot be distinguished from not
   passing the flag.
3. `-funwind-tables` is load-bearing and is overridden by `target_compile_options`; a
   launcher is required to observe `-fno-unwind-tables` (4507 → 2878).
4. `-fno-asynchronous-unwind-tables` is unobservable on ARM EHABI even at the end of the
   command line.
5. `-fno-sized-deallocation` is neutral and `-fsized-deallocation` is a regression: the
   original is the C++11 default.
6. `-fstack-protector-strong` is the unique stack-protector spelling; plain and `-all` are
   both regressions.
7. `-fvisibility-inlines-hidden` hides 141 symbols: the original exports inline functions.

**Not settled / unobservable:**

1. The neutral flags (`-fmerge-all-constants`, `-fno-merge-all-constants`,
   `-fno-merge-constants`, `-fno-omit-frame-pointer`, `-fno-sized-deallocation`) cannot be
   excluded as *used but neutral*; there is no positive artifact for them.
2. `-fno-common` is a source-cleanup question (the duplicate tentative
   `g_android_back_button_pressed`), not a flag win.
3. The residual 17 functions are source shape, not build configuration: every flag that
   moves codegen lowers the gate.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10119
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # 4507 / 4507

python3 _work/reconstructions/build_data_flag_fingerprint_w10119.py \
  --root "$PWD" --scratch /dev/shm/w10119 --jobs 24
# -> /dev/shm/w10119/results.json + per-cell logs in /dev/shm/w10119/logs/
```

Aligned totals were measured with `_work/reconstructions/align_diff.py` (the shared tool),
pointed at each cell's `.so` via its `OURS` environment variable.
