# `Status::missionCompleted` — wave 135 (w7781): a text `.profdata` DOES
# reproduce the exact 4:3:2:1 `!prof` weights (counter map extracted), but
# `-fprofile-instr-use` also re-weights every other branch and re-rolls the
# layout; the two-switch fold cannot synthesise 4:3:2:1 either

Status: **note-only** (no source change landed; gate stays byte 4506 /
linked 4506, `.text` diff 85,022).  Worktree `wt/w7781`, base master
`bbffef482` (wave-134 snapshot).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline |
|---|---:|
| function score | **71.0 %** (523 vs 525 normalised insns) |
| function size | 1432 B |
| differing bytes in the function | 722 / 1432 |

## 0. Summary (new results this pass)

1. **A checked-in text profile is a working source-level producer of the
   exact weights.**  clang 7.0.2's `-fprofile-instr-use` accepts a *text*
   profile if it is first run through `llvm-profdata merge`:
   ```
   :fe
   _ZN6Status16missionCompletedEbbx
   2003737435339834133                 ; the FuncHash from @__profd__
   116                                 ; counter count
   <c0> <c1> ... <c115>
   ```
   (`:fe` = front-end instrumentation, which is what plain
   `-fprofile-instr-generate` emits here: 0 `instrprof.increment` calls.)
   The `FuncHash` must match exactly (`IndexedInstrProfReader::
   getInstrProfRecord` rejects a mismatch); it is the *second* i64 in
   `@__profd__ZN6Status16missionCompletedEbbx`, `2003737435339834133`.
   The text reader needs the hash line (name, hash, count, counters) — the
   hash is easy to omit and then `llvm-profdata merge` says "Malformed".

2. **The counter → switch-case map is extracted** (one hot counter at a time,
   116 compiles).  `weight = counter + 1` for a case, default weight is 1:
   | case | counter | | case | counter | | case | counter |
   |---:|---:|---|---:|---:|---|---:|---:|
   | 167 | 13 | | 152 | 43 | | 160 | 92 |
   | 174 | 14 | | 153 | 47 | | 163 | 96 |
   | 8 | 16 | | 154 | 49 | | 164 | 100 |
   | 0 | 28 | | 155 | 51 | | 165 | 102 |
   | 11 | 29 | | 158 | 53 | | 168 | 105 |
   | 13 | 32 | | 162 | 61 | | 169 | 107 |
   | 14 | 35 | | 166 | 67 | | 171 | 109 |
   | 150 | 39 | | 170 | 72 | | 172 | 112 |
   | 151 | 41 | | 184 | 74 | | | |
   | | | | 189 | 79 | | | |
   | | | | 156 | 84 | | | |
   | | | | 157 | 88 | | | |

   Setting low = {8,0,11,13,14} counters to 3, A = {167,174,150,151,152,
   153,154,155,158,162,166,170,184,189} to 2, B = {156,157,160,163,164,165,
   168,169,171,172} to 1, everything else 1, produces **exactly**
   `i32 1, 3, 3, 4, 4, 4, 4, 4, 3x12, 2x10` on the switch — the w7761
   injection vector, bit for bit.

3. **But `-fprofile-instr-use` re-weights every other branch too**, and that
   alone re-rolls the layout.  Measured on the real TU (full link, official
   scorer):
   | profile | score | size |
   |---|---:|---:|
   | no profile (master) | **71.0** | 1432 |
   | all counters 1 | 15.8 | 1424 |
   | all counters 1000 | 14.2 | 1428 |
   | all counters 0 | — | (cold) |
   | target 4:3:2:1 switch + all other counters 1 | **17.9** | 1416 |
   | target 4:3:2:1 switch + all other counters 0 | 17.8 | 1476 |
   | target 4:3:2:1 switch + all other counters 1000 | 16.2 | 1416 |
   The switch weights in the IR are correct in all of these; what changes is
   the loop back-edge, the `hasWon` test, the `m == 0` test and every case
   body's internal branch — each now carries explicit `!prof`
   `branch_weights` where the original (and master) use
   `BranchProbabilityInfo` heuristics.  So the `.profdata` route needs the
   *original's* full per-branch profile, not just the switch's.

4. **This is direct evidence that the original's non-switch branches are
   heuristic, not profile-weighted.**  The w7761 IR-only injection (add
   `!prof` to the switch and nothing else) reaches **92.9 %** (523/523 insns,
   558 diff bytes).  If the original had profile weights on the loop /
   `hasWon` / body branches, that injection could not be that close.  So the
   4:3:2:1 switch weights must come from a *source-level* construct after
   all, and the profile can only land if its other counters are tuned to
   reproduce the heuristics exactly.

5. **A single `switch (__builtin_expect(t, K))` is confirmed 2-level.**
   `LowerExpectIntrinsic::handleSwitchExpect` sets ONE weight to
   `-likely-branch-weight` and every other weight (incl. default) to
   `-unlikely-branch-weight`.  Swept K ∈ {8, 0, 167, 150, 0x100} ×
   (L,U) ∈ {(4,1),(4,3),(3,2),(3,1),(2,1)} on the real TU: every variant is
   1384-1424 B and 18.7-29.7 % (all far below 71.0).  The L:U ratio is
   irrelevant when L>U — the single hot case sorts first and the rest tie,
   so the codegen is identical for every (L,U) pair.

6. **The nested two-switch fold produces 4 levels but never 4:3:2:1.**
   `switch (__builtin_expect(t, 8)) { low...; default: }` followed by
   `switch (__builtin_expect(t, 150)) { ... }` is folded by
   `FoldValueComparisonIntoPredecessors` into one switch.  With L=2,U=1 the
   weights are `1, 6, 12, 6, 6, 6, 1, 1, 2, 1` (case 8 hot, low group 6,
   inner 2/1); L=3,U=1 → `1,7,21,7,7,7,1,1,3,1`; L=4,U=1 →
   `1,8,32,8,8,8,1,1,4,1`; L=5,U=1 → `1,9,45,9,9,9,1,1,5,1`.  A 120-point
   sweep (outer expected ∈ {case, default}, inner expected ∈ {case,
   default}, (L,U) ∈ 7 pairs) found **no** vector whose reduced distinct
   values are {1,2,3,4}.  The fold's `p_i * (w_0 + Σ w_j)` term keeps
   producing the `(L+1)·L` geometric family, not 4:3:2:1.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# 1. text profile (low=3, A=2, B=1, rest 1)
python3 /dev/shm/w7781/mkprof.py             # writes /dev/shm/w7781/p.txt
.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata \
  merge /dev/shm/w7781/p.txt -o /dev/shm/w7781/p.profdata
# 2. compile Status.cpp with the exact TU flags + -fprofile-instr-use=...
#    (see /dev/shm/w7781/build_prof.sh), relink, score
```

Scratch (not committed): `/dev/shm/w7781/` — `cc_status.sh`, `relink.sh`,
`score.py`, `build_status.sh`, `build_ll.sh`, `build_prof.sh`, `inject.py`,
`inj_score.sh`, `mkprof.py`, `mkprof2.py`, `scan.py` (the counter map),
`fold.py`/`fold2.py` (if-chain fold search), `sweep_sw.py`, `sweep2.py`.

## 2. Open question for the next wave

The switch weights are 4:3:2:1 and every non-switch branch is heuristic.
The remaining lever is a source construct that gives `handleSwitchExpect`'s
single-hot-case vector a *per-group* structure.  Ideas not yet tried:

* tune the `.profdata`'s non-switch counters until the branch weights equal
  the `BranchProbabilityInfo` heuristic values (then the profile is
  behaviourally identical to no-profile except on the switch).  The
  heuristic weights can be read out of the baseline MIR
  (`-mllvm -print-machineinstrs` MBB successor probabilities).
* a `switch` whose condition is a `PHI` of `__builtin_expect` values
  (`handlePhiDef` in `LowerExpectIntrinsic` writes branch weights from the
  PHI's constant operands).
* `case 0 ... 14:` GNU case ranges + `__builtin_expect` (a range cluster has
  one weight, so `findCaseValue` on the expected value would hit it).
