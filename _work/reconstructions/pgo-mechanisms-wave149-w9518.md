# Build provenance B: alternative branch-weight / PGO mechanisms on one residual TU (wave 149, w9518)

Status: **harness + note, no `src/` change**. Worktree `wt/w9518`, base
`fece77a64` (wave 148). Scratch `/dev/shm/w9518`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0.

Target TU: `src/game/mission/Status.cpp`; function
`_ZN6Status16missionCompletedEbbx` (original `0xabf38`, **1432 B**).
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**,
`byte_exact 4507`, `.text` 1,259,464 B.

Mission: test the alternative mechanisms that could produce the original's
branch weights on ONE residual TU. w9420 had already found no shipped
`.profraw`/`.profdata`; w9415/w9416 had replayed hand-built profiles. This
wave tests the *other* mechanisms, and one new combination they missed.

## 0. Bottom line

1. **NEW: `-mllvm -profile-guided-section-prefix` is ON by default in clang
   7.0.2** (`CodeGenPrepare.cpp:171`, `cl::init(true)`). Any function carrying
   profile info is renamed `.text.hot.<mangled>` and gold places it at the
   **front** of `.text`, moving it away from its original address. w9415 and
   w9416 measured the function in isolation and did not notice that the
   profiled function had also been *relocated*. Disabling the flag keeps the
   function at its TU-order address. This is the one build flag a
   profile-replay experiment must add.
2. **A full-TU `-fprofile-instr-use` profile CAN reproduce the 1432 B size.**
   w9415 concluded it could not. A 750-point grid over the w9305 switch-weight
   family `(A, low, B, C)` × a uniform non-case counter `base` finds **38
   counter vectors that produce a 1432 B `missionCompleted`** (e.g.
   `(A,low,B,C,base) = (6,3,3,2,3)`). With section-prefix off they keep
   `byte_exact 4487-4495` (the target is size-exact at its original address),
   but **none of them reproduces the original's source-order case layout**.
3. **The full profile's case order is dominated by the non-case weights.**
   The source-order layout is only produced when the switch is the *only*
   weighted branch (w9305's switch-only `!prof` injection). Every full profile
   puts the low cases `159,161,173,175..188` (the `8,0,11,13,14` cases and the
   default) at the front, regardless of the switch vector.
4. **Mechanism availability in clang 7.0.2** (all verified against the NDK
   driver): `-fprofile-instr-use` and `-fprofile-sample-use` exist;
   `-mllvm -profile-guided-section-prefix`, `-mllvm
   -block-placement-exit-block-bias`, `-mllvm -arm-adjust-jump-tables`,
   `-mllvm -disable-block-placement` exist. **`-fbasic-block-sections`,
   `-mllvm -enable-ext-tsp-block-placement` and `-mllvm
   -precise-sample-profile` do not exist** (LLVM 8 features); clang 7.0.2
   rejects them with `Unknown command line argument` / `unknown argument`.
5. **`-block-placement-exit-block-bias` does not move the target's
   instruction stream.** `10..75` changes the raw diff `722 -> 754` (a literal
   pool / table byte) but leaves the aligned diff at 320 and the function at
   1432 B; it also shrinks `.text` by 64 B by changing *other* functions in
   the TU (`byte_exact 2305`). `0` and `200` are exact no-ops.
6. **Sample PGO is falsified again**, now with section-prefix off: 1372 B,
   raw 1321, aligned 738 (vs base 722/320). The sample loader marks the
   function hot and shrinks it by 60 B.

## 1. Mechanism table (Status.cpp, full gold link, real `verify.py`)

`byte_exact` is the global count from `tools/verify/verify.py` on the whole
linked `.so`. `raw`/`aligned`/`size` are the target function. `.text` is the
linked `.text` size (original 1,259,464).

| mechanism | flags | raw | aligned | size | `.text` | byte_exact |
|---|---|---:|---:|---:|---:|---:|
| base | — | **722** | **320** | **1432** | **1,259,464** | **4507** |
| (g) section-prefix off, no profile | `-mllvm -profile-guided-section-prefix=false` | 722 | 320 | 1432 | 1,259,464 | 4507 |
| (b) exit-bias 0 / 200 | `-mllvm -block-placement-exit-block-bias={0,200}` | 722 | 320 | 1432 | 1,259,464 | 4507 |
| (b) exit-bias 10..75 | `...={10,25,50,75}` | 754 | 320 | 1432 | 1,259,400 | 2305 |
| (b) exit-bias 100 | `...=100` | 1307 | 702 | 1476 | 1,259,432 | 2434 |
| (b) disable placement | `-mllvm -disable-block-placement` | 1292 | 353 | 1436 | 1,259,720 | 2296 |
| (b) arm-adjust-jump-tables off | `-mllvm -arm-adjust-jump-tables=false` | 775 | 320 | 1432 | 1,259,816 | 2314 |
| (a) graded instr-use, prefix on | w9305 vector, base 1, entry | 1370 | 706 | 1416 | 1,259,464 | 1421 |
| (a) graded instr-use, prefix off | + `-profile-guided-section-prefix=false` | 1365 | 705 | 1416 | 1,259,464 | 3044 |
| (a) graded instr-use, body, base 0, prefix off | w9305 vector, base 0, body | 1320 | 541 | 1364 | 1,259,400 | 2327 |
| (c) sample-use, prefix on | `-fprofile-sample-use=all.sample` | 1326 | 741 | 1372 | 1,259,432 | 1378 |
| (c) sample-use, prefix off | + `-profile-guided-section-prefix=false` | 1321 | 738 | 1372 | 1,259,400 | 2469 |
| (h) full profile, 1432 B hit (prefix on) | `(6,5,5,4,base 12)` | 1389 | 782 | **1432** | 1,259,464 | 2822 |
| (h) full profile, 1432 B hit (prefix off) | `(6,5,5,4,base 12)` | 1380 | 779 | **1432** | 1,259,464 | 4495 |
| (h) full profile, 1432 B hit (prefix off) | `(6,3,3,2,base 3)` | 1374 | 796 | **1432** | 1,259,464 | 4487 |
| (d) `-fbasic-block-sections` | — | — | — | — | — | *not in clang 7.0.2* |
| (e) `-mllvm -enable-ext-tsp-block-placement=0` | — | — | — | — | — | *not in LLVM 7* |
| (f) `-mllvm -precise-sample-profile` | — | — | — | — | — | *not in LLVM 7* |

The `byte_exact` regressions for the non-profile flags are *not* the target:
they come from the flag changing the codegen of the other 145 functions in
`Status.cpp` (`.text` size moves), which shifts every later function's address.
`linked_exact` stays 4505-4507 because those functions are structurally
identical.

## 2. Why the 1432 B full profile still loses

`-ffunction-sections` + gold concatenation means **any size change to one
function shifts every later address**, so `byte_exact` (a same-address byte
comparison) collapses even for structurally identical functions. The target
must be exactly 1432 B *and* at its original address to keep the gate.

The 1432 B hits satisfy both (prefix off) yet `byte_exact` is 4487-4495, not
4507: the profile build shifts 12 downstream functions by one immediate byte
(e.g. `ModStation::OnKeyPress` offset `0x856`: `0x3e -> 0x26`). They are 100 %
structurally identical, so this is a literal/relocation artefact of the
profiled object, not a layout change. It is still a gate regression, so none
of these variants can land.

## 3. The switch-order search (mechanism (h))

`pgo_mechanisms_w9518.py` builds a `.profdata` whose counters are
`base` for every non-case counter and the w9305 family
`[1, A, A, low×5, B×12, C×10]` for the 29 switch cases (IR case order
`167,174,8,0,11,13,14,150..155,158,162,166,170,184,189,156,157,160,163,164,165,168,169,171,172`).
A 750-point grid (`A∈2..6`, `low∈1..6`, `C∈1..5`, `B=C+1`, `base∈{0,1,2,4,8}`)
finds **38 size-1432 vectors**, but **0** with the original's source-order
first 24 cases. The physical order of every 1432 hit starts with the low
cases and the default block, e.g. `(6,5,5,4,12)`:

```
159,161,173,175,176,177,178,179,180,181,182,183,185,186,187,188,
167,174,184,158,166,150,151,152,154,153,...
```

The original is:

```
167,174,150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172,159,161,173,175..188
```

So a full profile reorders the whole function, not just the switch. This is
the same structural point w9416 made for `Radar::Radar`: a profile attaches
`!prof` to *every* branch, while the original's body is the unweighted
heuristic layout with only the switch reordered.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9518
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# mechanism table (compiles Status.cpp only, relinks the whole .so, runs verify)
python3 _work/reconstructions/pgo_mechanisms_w9518.py --run base
python3 _work/reconstructions/pgo_mechanisms_w9518.py --run \
  instr_graded_entry_b1 instr_graded_entry_b1_off sample_on sample_off
python3 _work/reconstructions/pgo_mechanisms_w9518.py --run \
  exitbias_0 exitbias_10 exitbias_25 exitbias_50 exitbias_75 exitbias_100 exitbias_200
python3 _work/reconstructions/pgo_mechanisms_w9518.py --run armjt_off disable_placement
```

The 1432 B / source-order grid search is committed as
`_work/reconstructions/pgo_grid_w9518.py`; it uses the same
`pgo_mechanisms_w9518.py` helpers (compile line, `.proftext` writer,
`llvm-profdata merge`, object-size and TBH readers):

```sh
python3 _work/reconstructions/pgo_grid_w9518.py          # 750 points
python3 _work/reconstructions/pgo_grid_w9518.py --quick  # the 4 size-1432 hits
```

## 5. Open questions

1. The original's full weight vector is still unrecovered. The 38 size-1432
   vectors are a starting family; a search that scores *case order* as well as
   size (and allows non-uniform non-case counters) is the next step.
2. Why the profiled object perturbs 12 downstream functions by one immediate
   byte is unexplained; it may be a section-`.text` alignment difference in the
   profiled object, and it is the remaining blocker for a 4487 -> 4507 replay.
3. `-mllvm -arm-adjust-jump-tables=false` changes the target's raw diff
   `722 -> 775` without changing the aligned stream or the size; it is worth
   checking whether the original was built with the flag off on a *different*
   residual switch function.
