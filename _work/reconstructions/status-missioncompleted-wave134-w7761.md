# `Status::missionCompleted` — wave 134 (w7761): the exact `!prof` weights are
# 4:3:2:1 and the layout is razor-sharp around them; the fold cannot synthesise
# that ratio

Status: **note-only** (no source change landed; gate stays byte 4506 /
linked 4506, `.text` diff 85,143).  Worktree `wt/w7761`, base master
`ec0acea1e` (wave-133 snapshot).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `ec0acea1e`) |
|---|---:|
| function score | **71.0 %** (523 vs 525 normalised insns) |
| function size | 1432 B |
| differing bytes in the function | 722 / 1432 |
| gate byte_exact / linked_exact | 4506 / 4506 |
| `.text` differing bytes | 85,143 |

## 0. Summary (new results this pass)

1. **The exact `!prof` weight vector is confirmed and quantified.**  Injecting
   `default=1, A=3, B=2, low=4` (A includes 167/174) into the current optimised
   IR gives **92.9 %** (523/523 insns, 1432 B) and cuts the function's differing
   bytes **722 -> 558**.  w7738 only reported "1432 B exact-order"; this is the
   first measurement of how much the score gains (71.0 -> 92.9) and of the
   byte-diff drop.
2. **The layout is extremely sensitive to the weight *ratio*, not the
   magnitude.**  Scaling all weights by k (k = 1,10,100,250,500,1000,2000,4000,
   8000,16000) keeps 92.9 %.  But any change to the ratio breaks it:
   * `(low,A,B) = (4,2,8)` / `(3,1,9)` / `(2,1,4)` / `(8,4,16)` -> 1444 B, 70.0 %;
   * `(4,1,16)` -> 1424 B, 43.9 %;
   * `(8,2,16)` -> 1384 B, 44.5 %;
   * `(100,99,98)` -> 1432 B, **78.3 %** (the best *other* 1432 B layout);
   * `(5,4,3)` / `(4,2,1)` / `(10,9,8)` -> 1368 B, 45.9 %;
   * `(2,1,3)` -> 1380 B, 47.0 %;
   * `(1,1,1)` / `(3,3,3)` / `(4,4,4)` -> 1432 B, 71.0 % (value order).
   In particular **the low-group weight must be exactly 4** for the A/B order:
   `low=1,2,3` -> 1368/1372 B ~46 %, `low=5` -> 1444 B 70.0 %, `low>=10`
   -> 1416 B or smaller.
3. **No plain-C++ fold ratio can produce 4:3:2.**  `-mllvm
   -print-machineinstrs`-level experiments on a 5-case if-chain show the
   `FoldValueComparisonIntoPredecessors` fold emits *geometric* weights with
   consecutive ratio `L+1` for `-mllvm -likely-branch-weight=L`
   (`L=2 -> 162,54,18,6,2`; `L=3 -> 768,192,48,12,3`; `L=4 -> 2500,500,100,20,4`).
   A single `__builtin_expect` gives a two-level step (e.g. `1,8004,8004,8000,2,1`),
   and multiple expects overflow the `i32` weights (`FitWeights` halves them).
   None of these is 4:3:2, and every geometric pattern tested on the real TU
   (`(8000,2,8004)`, `(4,2,8)`, `(8,4,16)`, ...) lands at 1372-1444 B, 44-70 %.
   This is consistent with the shipped `.so` being **PGO-built** (waves
   124/125/126 reached the same conclusion); the weights are real profile
   counts, not a `__builtin_expect` artefact.
4. **The if-chain body order is not reproducible with the obvious
   transformation.**  A mechanical `switch -> if/else-if` rewrite (temp
   `int mtype = m->getType();`, groups in source order, trailing `break;`
   removed) folds back to a switch but with case order `174,167,8,11,0,...`
   (the fold pops predecessors in reverse) and a **different body order**:
   the TBH bodies come out `167/174,150,151,153,154,155,156,160,162,164,165,
   166,168,169,170,171,172,152,157,158,163,189,...` — not the original
   `167/174,150,151,152,153,154,155,158,162,166,170,184,189,156,...`.  Sizes
   1336-1428 B, scores 10-22 %.  The w7738 "exact body order" chain is a
   specific shape this pass did not reproduce; the scratch generator is in
   `/dev/shm/w7761/mkif.py` / `mkif2.py`.
5. **A single-expect companion does not size-neutralise.**  Adding
   `__builtin_expect` to the loop condition (`i < missions->size()`), the
   `m->hasWon()` check, or sweeping `-mllvm -likely-branch-weight=1..16` on the
   if-chain changes the size by -80..-4 B but never restores the 1432 B
   tail/latch placement (the shared `cmp; blt latch; b setWon` tail still sits
   right after the 167/174 body).

6. **Case order *does* move the layout without weights, but no permutation
   beats 71.0.**  Reordering the source `switch` groups changes the emitted
   size/score: value order 1432 B/70.6 %, reverse 1464 B/42.8 %, low-group-last
   1380 B/53.8 %.  A 40-permutation random search found only 70.6 % at 1432 B
   (best other sizes 1356-1468 B, 48.9-58.7 %).  The current body-order source
   is already the best unweighted order.  Scratch: `mkswitch_order.py`,
   `search_order.py`.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506

# exact-flag .ll of the TU
/dev/shm/w7761/cc.sh src/game/mission/Status.cpp /dev/shm/w7761/status.ll -S -emit-llvm

# inject branch_weights on the missionCompleted switch (case order is the
# .ll order: 167,174,8,0,11,13,14,150..189,156..172)
python3 /dev/shm/w7761/inject.py /dev/shm/w7761/status.ll /dev/shm/w7761/inj.ll \
  "1,3,3,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2"
# compile inj.ll with the same flags, copy over
# cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj,
# then run /dev/shm/w7761/relink.sh (extracted gold+strip command)
python3 /dev/shm/w7761/score.py _ZN6Status16missionCompletedEbbx   # 92.9, 1432 B
```

Scratch (not committed): `/dev/shm/w7761/` — `cc.sh`, `relink.sh`, `score.py`,
`inject.py`, `sweep_inj.py`, `grid*.py`, `mkif.py`, `mkif2.py`,
`status.ll`, `status.show`.

## 2. Open question for the next wave

The only remaining lever is a source-level producer of `!prof` weights with
the exact 4:3:2:1 ratio.  Clang 7.0.2 has no
`__builtin_expect_with_probability`; the fold is geometric; `handleSwitchExpect`
is uniform.  Options not yet tried:

* a per-file `-mllvm -likely-branch-weight=4 -mllvm -unlikely-branch-weight=1`
  on a **two-switch** shape whose two `handleSwitchExpect` weight vectors
  combine to 4:3:2 (needs a source split of the A/B groups into two switches,
  which wave 125 measured at 1508 B for the plain form);
* a `#pragma`/`__builtin` in clang 7 that emits `!prof` on a switch directly
  (none found in `_work/reconstructions/mllvm_help_hidden_clang702_r18b.txt`);
* accepting PGO: a checked-in `.profdata` + `-fprofile-use` for this TU only.
