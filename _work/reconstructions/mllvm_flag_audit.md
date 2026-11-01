# `-mllvm` flag audit (w3426)

Status: research note (2026-11-01). Worktree `wt/w3426`, branch `wave87-w3426`,
base `d00e0030`. No `src/` or build-flag change landed: every tested global
`-mllvm` option was neutral or worse, so this commit contains only this note.

## TL;DR

* Baseline (`cmake --build cmake-build-match --target check -j8`):
  **byte 4182, linked 4495, size 4523/4524, lint CLEAN, drift 0**,
  `.text` `0x1337c8` (exactly the original's `.text` size).
* **123 global `-mllvm` variants measured; 0 strict improvements.**
  67 are neutral (byte 4182 / linked 4495), 56 are worse, 1 build failure
  (`-mllvm -enable-machine-outliner=true` crashes clang 7.0.2).
* The original app TUs are consistent with LLVM 7.0.2's **default** codegen.
  No `-mllvm` flag is needed, and none is supported by the gate.
* The four historical scoped compensations are not original global flags:
  `-sink-insts-to-avoid-spills` is worse globally (1450/4344),
  `-condsider-local-interval-cost` is worse (1720/4473),
  `-likely-branch-weight=80 -unlikely-branch-weight=1` is neutral (4182/4495),
  and `20:12` is worse (2885/4490).
* The remaining 29 non-linked functions are source/TU shape, not a missing
  compiler flag.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`size-exact` is `sum(osize == usize)` over
`cmake-build-match/verify/report.json` = **4523 / 4524**.  The linked `.so`
has `.text` `0x1337c8`, `.rodata` `0x41b67`, `.data` `0x296c`, `.bss`
`0x69ac` — all exactly the original's sizes.

## 2. Method

The A/B is a **global app-TU compile-flag sweep**.  A wrapper toolchain file
includes the repo toolchain and appends the variant flags to both
`CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
include(/home/debian/GalaxyOnFire2/wt/w3426/cmake/toolchain.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} <flags>" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   <flags>" CACHE STRING "" FORCE)
```

Each variant is a fresh build directory:

```sh
cmake -S . -B /dev/shm/mllvm/b_<id> -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/dev/shm/mllvm/tc_<id>.cmake
flock /dev/shm/mllvm/lib.lock \
      cmake --build /dev/shm/mllvm/b_<id> --target gof2_libs -j1
cmake --build /dev/shm/mllvm/b_<id> --target link_so -j12
```

The `flock` serialises the vendored zlib/libzip/libcrypto rebuild that a
fresh build directory always performs once; otherwise parallel variants race
on `third_party/*/lib*_gof2.a`.  The flags apply to the app TUs only.  The
vendored archives are built by `third_party/build_match_lib.cmake` with their
own fixed flags, exactly as the original linked them; they are already exact
and are not part of the app-TU flag question.

### Verifier used for the sweep

`tools/verify/verify.py` is dominated by `difflib.SequenceMatcher` on the
~62k-instruction `BuildResourceList` function.  The sweep used a copy,
`/dev/shm/mllvm/verify_fast3.py`, with two changes that do not affect
`byte_exact`, `linked_exact` or `size-exact`:

1. byte-exact functions short-circuit (no disassembly/reconcile);
2. `reconcile` uses positional alignment for equal-length sequences longer
   than 2000 instructions instead of the O(n²) SequenceMatcher.

Validated against the official verifier on four variants:

| variant | official byte/linked/size | fast byte/linked/size |
|---|---|---|
| baseline | 4182 / 4495 / 4523 | 4182 / 4495 / 4523 |
| `-sink-insts-to-avoid-spills` | 1450 / 4344 / 4394 | 1450 / 4344 / 4394 |
| `-likely-branch-weight=80:1` | 4182 / 4495 / 4523 | 4182 / 4495 / 4523 |
| `-condsider-local-interval-cost` | 1720 / 4473 / 4504 | 1720 / 4473 / 4504 |

Only `fuzzy_perfect` can differ (the fast copy sets it equal to
`linked_exact`); it is not used below.

## 3. Results

124 variants: 123 measured, 1 build failure.  **No variant beats
byte 4182 / linked 4495.**  `size` is size-exact functions.  `.text` is the
linked section size (original: `0x1337c8`).

### Baseline

| # | flag | byte | linked | size | `.text` | verdict |
|---:|---|---:|---:|---:|---:|---|
| 1 | *(none)* | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |

### Scheduling / register allocation

| # | flag | byte | linked | size | `.text` | verdict |
|---:|---|---:|---:|---:|---:|---|
| 1 | `-mllvm -enable-deferred-spilling` | 1702 | 4315 | 4449/4524 | `1337c8` | worse |
| 2 | `-mllvm -split-spill-mode=size` | 1389 | 4413 | 4444/4524 | `1337c8` | worse |
| 3 | `-mllvm -split-spill-mode=speed` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 4 | `-mllvm -enable-local-reassign` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 5 | `-mllvm -disable-postra-machine-sink` | 1549 | 4390 | 4474/4524 | `1337c8` | worse |
| 6 | `-mllvm -disable-machine-sink` | 1222 | 3734 | 4021/4524 | `134708` | worse |
| 7 | `-mllvm -disable-machine-licm` | 1333 | 4094 | 4183/4524 | `1328c8` | worse |
| 8 | `-mllvm -sink-insts-to-avoid-spills` | 1450 | 4344 | 4394/4524 | `1335c8` | worse |
| 9 | `-mllvm -machine-sink-split` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 10 | `-mllvm -disable-post-ra` | 1176 | 2331 | 4308/4524 | `133888` | worse |
| 11 | `-mllvm -enable-misched=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 12 | `-mllvm -enable-post-misched` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 13 | `-mllvm -disable-2addr-hack` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 14 | `-mllvm -enable-arm-3-addr-conv` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 15 | `-mllvm -disable-spill-fusing` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 16 | `-mllvm -disable-spill-hoist` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 17 | `-mllvm -disable-ssc` | 1344 | 4296 | 4484/4524 | `133ce8` | worse |
| 18 | `-mllvm -enable-shrink-wrap` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 19 | `-mllvm -regalloc-csr-first-time-cost=0` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 20 | `-mllvm -regalloc-csr-first-time-cost=2` | 1981 | 4483 | 4512/4524 | `1337e8` | worse |
| 21 | `-mllvm -reverse-csr-restore-seq` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 22 | `-mllvm -stress-regalloc=12` | 1338 | 3935 | 4050/4524 | `135e68` | worse |
| 23 | `-mllvm -regalloc=fast` | 1075 | 1594 | 1654/4524 | `1c0aac` | worse |
| 24 | `-mllvm -pre-RA-sched=source` | 1171 | 2575 | 3988/4524 | `132888` | worse |
| 25 | `-mllvm -pre-RA-sched=list-burr` | 1234 | 2744 | 4089/4524 | `133008` | worse |
| 26 | `-mllvm -pre-RA-sched=list-hybrid` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 27 | `-mllvm -pre-RA-sched=list-ilp` | 1247 | 2825 | 3937/4524 | `133868` | worse |

### Block placement / CFG

| # | flag | byte | linked | size | `.text` | verdict |
|---:|---|---:|---:|---:|---:|---|
| 1 | `-mllvm -disable-block-placement` | 1232 | 3116 | 4122/4524 | `1359a8` | worse |
| 2 | `-mllvm -branch-fold-placement` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 3 | `-mllvm -tail-dup-placement` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 4 | `-mllvm -disable-early-taildup` | 2640 | 4482 | 4516/4524 | `1337e8` | worse |
| 5 | `-mllvm -disable-tail-duplicate` | 2215 | 4438 | 4500/4524 | `1337c8` | worse |
| 6 | `-mllvm -enable-tail-merge` | 2597 | 4494 | 4522/4524 | `1338a8` | worse |
| 7 | `-mllvm -switch-to-lookup` | 2035 | 4491 | 4519/4524 | `1337a8` | worse |
| 8 | `-mllvm -jump-table-density=0` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 9 | `-mllvm -jump-table-density=20` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 10 | `-mllvm -arm-adjust-jump-tables` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 11 | `-mllvm -disable-branch-fold` | 1349 | 4349 | 4425/4524 | `134628` | worse |
| 12 | `-mllvm -disable-early-ifcvt` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 13 | `-mllvm -disable-early-ifcvt=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 14 | `-mllvm -enable-if-conversion` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 15 | `-mllvm -tail-dup-size=0` | 1943 | 4425 | 4493/4524 | `1337e8` | worse |
| 16 | `-mllvm -tail-dup-placement-threshold=0` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 17 | `-mllvm -tail-merge-size=0` | 1329 | 4401 | 4483/4524 | `134148` | worse |
| 18 | `-mllvm -sink-freq-percent-threshold=0` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 19 | `-mllvm -block-placement-exit-block-bias=0` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 20 | `-mllvm -block-placement-exit-block-bias=100` | 1673 | 4433 | 4482/4524 | `133748` | worse |

### IR optimisation

| # | flag | byte | linked | size | `.text` | verdict |
|---:|---|---:|---:|---:|---:|---|
| 1 | `-mllvm -enable-gvn-hoist` | 1384 | 4402 | 4427/4524 | `133408` | worse |
| 2 | `-mllvm -enable-gvn-sink` | 1340 | 4330 | 4382/4524 | `133998` | worse |
| 3 | `-mllvm -enable-newgvn` | 1353 | 4103 | 4261/4524 | `133008` | worse |
| 4 | `-mllvm -disable-constant-hoisting` | 1500 | 4471 | 4504/4524 | `1336c8` | worse |
| 5 | `-mllvm -disable-lsr` | 1328 | 4150 | 4286/4524 | `1332c8` | worse |
| 6 | `-mllvm -disable-licm-promotion` | 1478 | 4489 | 4519/4524 | `13332c` | worse |
| 7 | `-mllvm -disable-cgp` | 1241 | 2799 | 3112/4524 | `13acc8` | worse |
| 8 | `-mllvm -disable-cgp-branch-opts` | 4181 | 4494 | 4523/4524 | `1337c8` | worse |
| 9 | `-mllvm -disable-cgp-select2branch` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 10 | `-mllvm -enable-partial-inlining` | 1432 | 4494 | 4523/4524 | `133848` | worse |
| 11 | `-mllvm -enable-loop-distribute` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 12 | `-mllvm -enable-loopinterchange` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 13 | `-mllvm -enable-pipeliner` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 14 | `-mllvm -enable-simple-loop-unswitch` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 15 | `-mllvm -enable-nontrivial-unswitch` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 16 | `-mllvm -enable-mssa-loop-dependency` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 17 | `-mllvm -disable-mergeicmps` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 18 | `-mllvm -enable-dse-partial-store-merging` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 19 | `-mllvm -enable-store-refinement` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 20 | `-mllvm -enable-load-pre` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 21 | `-mllvm -enable-pre` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 22 | `-mllvm -enable-nonnull-arg-prop` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 23 | `-mllvm -disable-basicaa` | 1318 | 3838 | 4126/4524 | `1359c8` | worse |
| 24 | `-mllvm -enable-earlycse-memssa=false` | 1890 | 4463 | 4505/4524 | `133b28` | worse |

### ARM backend / misc

| # | flag | byte | linked | size | `.text` | verdict |
|---:|---|---:|---:|---:|---:|---|
| 1 | `-mllvm -arm-load-store-opt=false` | 1201 | 3311 | 3907/4524 | `137688` | worse |
| 2 | `-mllvm -arm-promote-constant=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 3 | `-mllvm -arm-global-merge=true` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 4 | `-mllvm -arm-implicit-it=thumb` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 5 | `-mllvm -arm-implicit-it=always` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 6 | `-mllvm -arm-synthesize-thumb-1-tbb` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 7 | `-mllvm -align-neon-spills` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 8 | `-mllvm -arm-assume-misaligned-load-store` | 1202 | 3403 | 3974/4524 | `136b48` | worse |
| 9 | `-mllvm -enable-machine-outliner=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 10 | `-mllvm -enable-machine-outliner=true` | — | — | — | `—` | BUILD_FAIL |
| 11 | `-mllvm -enable-linkonceodr-outlining` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 12 | `-mllvm -remat-pic-stub-load` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 13 | `-mllvm -enable-ipra` | 1357 | 4185 | 4248/4524 | `133e08` | worse |
| 14 | `-mllvm -enable-aa-sched-mi` | 1387 | 4018 | 4496/4524 | `1337e8` | worse |
| 15 | `-mllvm -enable-cond-stores-vec` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 16 | `-mllvm -disable-peephole` | 1312 | 3559 | 4205/4524 | `1340a8` | worse |
| 17 | `-mllvm -disable-machine-cse` | 1321 | 3355 | 3460/4524 | `141d74` | worse |
| 18 | `-mllvm -disable-machine-dce` | 1369 | 4365 | 4460/4524 | `134268` | worse |
| 19 | `-mllvm -disable-copyprop` | 1314 | 3055 | 4017/4524 | `135b08` | worse |
| 20 | `-mllvm -disable-adv-copy-opt` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 21 | `-mllvm -disable-non-allocatable-phys-copy-opt` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 22 | `-mllvm -disable-lftr` | 1388 | 4380 | 4494/4524 | `1337e8` | worse |

### Extra controls (old scoped flags and default toggles)

| # | flag | byte | linked | size | `.text` | verdict |
|---:|---|---:|---:|---:|---:|---|
| 1 | `-mllvm -condsider-local-interval-cost` | 1720 | 4473 | 4504/4524 | `133748` | worse |
| 2 | `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 3 | `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | 2885 | 4490 | 4519/4524 | `1337c8` | worse |
| 4 | `-mllvm -unroll-threshold=0` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 5 | `-mllvm -inline-threshold=100` | 1341 | 3520 | 3553/4319 | `14e1a8` | worse |
| 6 | `-mllvm -inline-threshold=300` | 1341 | 3368 | 3394/4299 | `158138` | worse |
| 7 | `-mllvm -unroll-runtime` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 8 | `-mllvm -vectorize-slp=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 9 | `-mllvm -vectorize-loops=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 10 | `-mllvm -enable-tbaa=false` | 1357 | 4347 | 4410/4524 | `133cc8` | worse |
| 11 | `-mllvm -enable-scoped-noalias=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 12 | `-mllvm -disable-icp` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 13 | `-mllvm -disable-partial-inlining` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 14 | `-mllvm -disable-ifcvt-diamond` | 1388 | 4397 | 4460/4524 | `1338a8` | worse |
| 15 | `-mllvm -precise-rotation-cost` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 16 | `-mllvm -disable-libcalls-shrinkwrap` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 17 | `-mllvm -enable-loop-versioning-licm` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 18 | `-mllvm -vectorizer-maximize-bandwidth` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 19 | `-mllvm -enable-shrink-wrap=false` | 1349 | 4458 | 4499/4524 | `133768` | worse |
| 20 | `-mllvm -enable-tail-merge=false` | 1301 | 3853 | 3875/4524 | `14b788` | worse |
| 21 | `-mllvm -branch-fold-placement=false` | 1340 | 4308 | 4346/4524 | `135828` | worse |
| 22 | `-mllvm -tail-dup-placement=false` | 1350 | 4391 | 4423/4524 | `1351a8` | worse |
| 23 | `-mllvm -enable-post-misched=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 24 | `-mllvm -enable-load-pre=false` | 1360 | 4188 | 4328/4524 | `133128` | worse |
| 25 | `-mllvm -enable-pre=false` | 1359 | 4162 | 4306/4524 | `132e28` | worse |
| 26 | `-mllvm -enable-cond-stores-vec=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 27 | `-mllvm -arm-adjust-jump-tables=false` | 1555 | 4489 | 4515/4524 | `1339a8` | worse |
| 28 | `-mllvm -arm-assume-misaligned-load-store=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 29 | `-mllvm -arm-synthesize-thumb-1-tbb=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |
| 30 | `-mllvm -align-neon-spills=false` | 4182 | 4495 | 4523/4524 | `1337c8` | neutral |

## 4. What the neutral flags show

67 variants leave the gate exactly at 4182/4495.  They are a mix of:

* flags that are already the LLVM 7.0.2 `-Oz` default and are therefore
  no-ops when passed again (`-enable-shrink-wrap`, `-branch-fold-placement`,
  `-tail-dup-placement`, `-arm-adjust-jump-tables`, `-enable-load-pre`,
  `-enable-pre`, `-enable-cond-stores-vec`, `-enable-post-misched`, ...);
* flags whose pass has no opportunity in this tree at `-Oz`
  (`-enable-loop-distribute`, `-enable-loopinterchange`, `-enable-pipeliner`,
  `-enable-simple-loop-unswitch`, `-enable-nontrivial-unswitch`,
  `-enable-mssa-loop-dependency`, `-enable-loop-versioning-licm`,
  `-disable-icp`, `-disable-partial-inlining`, `-precise-rotation-cost`,
  `-disable-libcalls-shrinkwrap`, `-vectorizer-maximize-bandwidth`,
  `-enable-scoped-noalias=false`, `-arm-implicit-it=thumb/always`,
  `-arm-synthesize-thumb-1-tbb`, `-align-neon-spills`, `-remat-pic-stub-load`);
  `-enable-gvn-hoist`/`-enable-gvn-sink` are *not* neutral (see below);
* flags that are simply not the effective control at `-Oz`
  (`-vectorize-slp=false` and `-vectorize-loops=false` are neutral even
  though the old `-fno-vectorize -fno-slp-vectorize` A/B was worse; the
  clang driver's `-vectorize-slp` is the effective switch here).

The neutral rows are useful negatives: they show that adding the flag cannot
change the gate, so it cannot be the missing original build flag.

## 5. What the worse flags show

The default pipeline is not optional: every stage that can be disabled
individually loses functions.  The largest losses:

| flag | byte | linked | what it proves |
|---|---:|---:|---|
| `-regalloc=fast` | 1075 | 1594 | greedy is the `-Oz` allocator; `.text` balloons to `0x1c0aac` |
| `-disable-post-ra` | 1176 | 2331 | the post-RA scheduler is on and important |
| `-pre-RA-sched=source` | 1171 | 2575 | default pre-RA scheduler is not source order |
| `-pre-RA-sched=list-burr` | 1234 | 2744 | default is not list-burr |
| `-pre-RA-sched=list-ilp` | 1247 | 2825 | default is not list-ilp |
| `-disable-machine-sink` | 1222 | 3734 | machine sinking is on |
| `-disable-block-placement` | 1232 | 3116 | block placement is on |
| `-disable-cgp` | 1241 | 2799 | CodeGenPrepare is on |
| `-arm-load-store-opt=false` | 1201 | 3311 | ARM load/store opt is on |
| `-disable-machine-cse` | 1321 | 3355 | machine CSE is on |
| `-disable-copyprop` | 1314 | 3055 | copy propagation is on |
| `-disable-peephole` | 1312 | 3559 | peephole is on |
| `-disable-basicaa` | 1318 | 3838 | basic alias analysis is on |
| `-enable-tbaa=false` | 1357 | 4347 | TBAA is on |
| `-enable-machine-outliner=true` | — | — | **clang 7.0.2 crashes** (`Aborted`) on `AERandom.cpp`, `AbyssEngine.cpp`, `IApplicationModule.cpp`, `GameTextInit.cpp` |

Other informative negatives:

* `-enable-shrink-wrap=false` 1349/4458, `-enable-tail-merge=false`
  1301/3853, `-branch-fold-placement=false` 1340/4308,
  `-tail-dup-placement=false` 1350/4391, `-arm-adjust-jump-tables=false`
  1555/4489: these are on by default and matter.
* `-disable-lsr` 1328/4150, `-disable-licm-promotion` 1478/4489,
  `-disable-constant-hoisting` 1500/4471, `-enable-newgvn` 1353/4103,
  `-enable-gvn-hoist` 1384/4402, `-enable-gvn-sink` 1340/4330:
  the IR optimisation pipeline is on and important.
* `-enable-earlycse-memssa=false` 1890/4463, `-disable-ifcvt-diamond`
  1388/4397, `-disable-lftr` 1388/4380: early CSE, if-conversion and LFTR
  are on.
* `-enable-deferred-spilling` 1702/4315, `-split-spill-mode=size`
  1389/4413, `-split-spill-mode=speed` neutral: deferred spilling is off by
  default and worse when enabled; the tree does not benefit from a different
  spill-split mode.
* `-enable-ipra` 1357/4185 and `-enable-aa-sched-mi` 1387/4018: enabling
  either is worse.
* `-partial-inline-on` 1432/4494, `-disable-partial-inlining` neutral:
  partial inlining is off by default and worse when enabled.
* `-switch-to-lookup` 2035/4491, `-tail-dup-size=0` 1943/4425,
  `-tail-dup-off` 2215/4438, `-tail-merge-size=0` 1329/4401:
  the default tail-duplication/switch lowering is correct.
* `-inline-threshold=100` 1341/3520 (`missing 205`) and `=300`
  1341/3368 (`missing 216`): the default inline threshold is correct.
* `-disable-cgp-branch-opts` is 4181/4494 — only one function worse, but
  still a regression, so CodeGenPrepare's branch optimisation is on and
  matters at the margin.

### Register-allocator caveat

`-mllvm -regalloc=basic` and `-mllvm -regalloc=pbqp` are rejected by clang
7.0.2 at `-Oz`:

```
fatal error: error in backend: Must use fast (default) register allocator for unoptimized regalloc.
```

So `greedy` is the only usable allocator at `-Oz`; `-regalloc=fast` is the
catastrophic row above.

### Machine-outliner caveat

`-mllvm -enable-machine-outliner=false` is neutral, and
`-mllvm -enable-machine-outliner=true` crashes the compiler.  The outliner is
therefore off in the original (or has no opportunity); it cannot have been an
original build flag.

## 6. The old scoped flags, re-tested globally

`_work/reconstructions/build_flags_research.md` records four scoped
compensations that were later removed from `CMakeLists.txt`.  Re-tested
globally at the current baseline:

| old scoped flag | global byte | global linked | verdict |
|---|---:|---:|---|
| `-mllvm -sink-insts-to-avoid-spills` | 1450 | 4344 | worse |
| `-mllvm -condsider-local-interval-cost` | 1720 | 4473 | worse |
| `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` | 4182 | 4495 | neutral |
| `-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12` | 2885 | 4490 | worse |

This confirms they were **local source-shape compensations**, not original
build flags.  The 80:1 row being neutral is consistent with the current source no
longer needing that compensation.

## 7. Proven vs hypothesis

**Proven by measurement**

* Baseline is byte 4182 / linked 4495 / size 4523/4524 / lint CLEAN / drift 0,
  `.text` `0x1337c8` (exact).
* 123 global `-mllvm` variants were built and verified; **none** improves
  `byte_exact` or `linked_exact`.  67 are neutral, 56 are worse.
* The specific neutral/worse rows in the table are reproducible from the
  commands in §2.
* `-regalloc=basic`/`pbqp` are rejected at `-Oz`; `-regalloc=fast` is
  catastrophic; the machine outliner crashes clang 7.0.2.
* The old scoped flags are worse or neutral when applied globally.

**Hypothesis / limits**

* The original was built with ndk-build and therefore almost certainly used
  no `-mllvm` flags.  The A/B is consistent with that but cannot prove the
  absence of a flag that is codegen-neutral on this tree.
* The sweep covers the app TUs only.  The vendored zlib/libzip/libcrypto are
  built by `third_party/build_match_lib.cmake` with their own flags and are
  already exact; they were not part of this A/B.
* `-mllvm` options are not recorded in the ELF, so there is no direct
  binary witness for or against a specific flag; the A/B is the only
  evidence.
* The residual 29 non-linked functions (Layout::Layout, LevelScript::process,
  Radar::draw, Level::createShip, Generator::createMission, ...) are the
  known source/TU-shape queue.  No global flag in this matrix moves them.

## 8. Reproduction

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# one variant
cat > /dev/shm/mllvm/tc_variant.cmake <<'EOF'
include(/home/debian/GalaxyOnFire2/wt/w3426/cmake/toolchain.cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mllvm -enable-deferred-spilling" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -mllvm -enable-deferred-spilling" CACHE STRING "" FORCE)
EOF
cmake -S . -B /dev/shm/mllvm/b_variant -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/dev/shm/mllvm/tc_variant.cmake
flock /dev/shm/mllvm/lib.lock \
      cmake --build /dev/shm/mllvm/b_variant --target gof2_libs -j1
cmake --build /dev/shm/mllvm/b_variant --target link_so -j12

# official verifier (fast copy is /dev/shm/mllvm/verify_fast3.py)
PYTHONPATH=tools/verify python3 tools/verify/verify.py \
  --build-dir /dev/shm/mllvm/verify_variant \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/mllvm/b_variant/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

The full matrix is `/dev/shm/mllvm/matrix.tsv` (id<TAB>flags); the per-variant
result files are `/dev/shm/mllvm/res_<id>.txt`.
