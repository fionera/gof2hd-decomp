# Wave 136 (w8117): `Status::missionCompleted` — no source fold produces
# 4:3:2:1; `Level::createShip` — all-int + barrier forms re-measured, current
# body is a local optimum

Status: **note-only** (no `src/` change landed).  Gate on the unchanged tree:
`byte_exact 4506/4524`, `linked_exact 4506/4524`, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8117`, base master
`2e51d765d` (wave-135 snapshot).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**, baseline
  **71.0 %** (523/525 insns).
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline **70.6 %** (1007/1005 insns), exact 336 B / 57-row / `ttype 0xff`
  LSDA.

## 0. Summary of new results

1. **`LowerExpectIntrinsic::handleSwitchExpect` is one-hot** (verified against
   the LLVM 7.0.1 source): it sets *one* weight to `-likely-branch-weight` and
   every other case **and the default** to `-unlikely-branch-weight`.  So a
   direct `switch (__builtin_expect(t, K))` can never give 4:3:2:1.
2. **The `FoldValueComparisonIntoPredecessors` fold is multiplicative.**  With
   the real group sizes, a 2-level nested switch (outer = low+A, inner = B,
   both expects on default) gives `default=L, outer cases=11U, inner cases=L`;
   a 3-level nest (low -> A -> B) gives `default=L, low=165U, A=44U, B=L`.
   These are the geometric/arithmetic families the earlier waves saw.
3. **Exhaustive fold searches found no 4:3:2:1.**  Swept:
   * 2- and 3-level nested `switch` trees, all 6 group orderings, expect target
     ∈ {none, default, first case of the group, second case}, and
     (L,U) ∈ 17 pairs (2,1)..(16,1);
   * the full if-chain (source order 167/174, low, A, B) with
     `__builtin_expect` on every subset of 11 candidate cases (up to 5 at a
     time), 7 (L,U) pairs;
   * the `||`-chain form (`if (__builtin_expect(t == a || t == b || ..., 1))`)
     with the A group split into A1={167,174} / A2={150..189}, all 24 group
     orderings, all 16 expect subsets, 20 (L,U) pairs.
   None yields group weights proportional to low:A:B:default = 4:3:2:1.
   Scratch: `/dev/shm/w8117/search_nest2.py`, `search_chain.py`,
   `search_or.py`, `dump_or.py`, `sweep_fold.py`, `sweep3.py`.
4. **The `||` form is the only fold that separates the groups at all.**  With
   the natural order (A1, low, A2, B) and all four expects on, L=4/U=1 it
   gives `default=1, A1+low=660, A2∈{44,55}, B=1` — the A1/low merge and the
   non-uniform A2 are fatal.  Reordering to (A1, A2, B, low) gives
   `default=1, A1=A2=300, B∈{24,30}, low=1`.
5. **Conclusion for Status:** with clang 7.0.2 the only source-level producers
   of switch `!prof` weights are `__builtin_expect` (one-hot) and the
   if/switch fold (multiplicative).  Neither can express the *arithmetic*
   4:3:2:1 ratio.  The remaining source-level route is a checked-in
   `.profdata` whose **non-switch** counters are tuned to reproduce
   `BranchProbabilityInfo`'s heuristics (the w7781 note showed the switch
   weights alone reproduce the layout at 92.9 % / 1432 B, but an untuned
   profile re-weights every other branch and collapses to 15–18 %).
6. **createShip: the current body is a local optimum.**  Re-measured the
   all-`int` coordinate forms (the faithful original types, conversions at the
   ctor sites) with several scheduling barriers between the coordinate block
   and `setEmpData`; every one loses on size and/or score:

   | variant | score | usize |
   |---|---:|---:|
   | **master** (`float z`, `float y`, `int x`; no barrier) | **70.6** | **2764** |
   | all-`int` x,y,z (no barrier) | 61.5 | 2772 |
   | all-`int` + `volatile int vb = x;` | 59.6 | 2800 |
   | all-`int` + `Globals::rnd->nextInt(1);` call barrier | 60.3 | 2800 |
   | all-`int` + `(void) Globals::status->getLevel();` call barrier | 61.4 | 2780 |
   | all-`int` + `try { setEmpData } catch (...) {}` | 63.0 | 2784 |
   | all-`int` + volatile store to `Globals::status` | 61.3 | 2776 |
   | `float z,y` computed before `new Player` | 62.9 | 2768 |
   | `int x`, `float y,z` + `try { setEmpData } catch (...) { __builtin_trap(); }` | 67.1 | 2752 |
   | `try { new Player } catch { return nullptr; }` **+** `try { setEmpData }` | 41.1 | 2780 |
   | `int x,y,z` declaration order (vs `z,y,x`) | 70.6 | 2764 (identical) |

   The two `try` forms also move `.ARM.extab`: `try{setEmpData}` alone is
   `0x262a8` (156,328, +16 B), `try{new Player}` + `try{setEmpData}` is
   `0x262b0` (156,336, +24 B), so neither can keep the exact 336 B record.
   Scratch: `/dev/shm/w8117/cs.py`, `cs2.py`, `cs3.py`.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8117/score.py    # both targets, one-function official scorer

# fold searches (compile small C++ shapes with the NDK clang and read the
# resulting switch `!prof` branch_weights):
python3 /dev/shm/w8117/search_nest2.py
python3 /dev/shm/w8117/search_chain.py
python3 /dev/shm/w8117/search_or.py
```

## 3. `.profdata` route — reproduced and partially characterised (new)

The w7781 text-profile recipe was reproduced end-to-end in this worktree:

```
python3 /dev/shm/w8117/mkprof.py            # one counter per line (the text
                                            # reader requires line-per-counter)
llvm-profdata merge p.txt -o p.profdata
# compile Status.cpp with the exact TU command + -fprofile-instr-use=p.profdata,
# then run the extracted gold/strip/reorder link command directly
bash /dev/shm/w8117/relink.sh               # 17.9 % / 1416 B
```

The profile's switch metadata is exactly `i32 1, 3, 3, 4, 4, 4, 4, 4, 3x12,
2x10`, confirming the w7781 counter map.  The remaining problem is the
non-switch branch weights.

**Baseline heuristic branch probabilities** were read from
`-mllvm -print-machineinstrs` on the real TU (last MIR dump, `# Machine code
for function _ZN6Status16missionCompletedEbbx`):

| branch | heuristic |
|---|---|
| loop `i < size()` | **124 : 4** (96.88 / 3.12) |
| `m->hasWon()` | **124 : 4** |
| `m == 0` | **124 : 4** |
| `!isCampaign && getAgent()==0 && ...` | **3 : 5** (37.5 / 62.5) |
| remaining case-body tests | 1 : 1 |

So the earlier "all counters 1" profile is 50/50 on the three 124:4 branches,
which is why it collapses the layout.  Setting the loop-body counter (index 4)
to 123 and all others to 4 gives **19.5 % / 1372 B** — better than 17.9 but
still far from 71.0.  The profile's branch weights are *normalised to the loop
iteration count* (e.g. the loop branch becomes `124 : 1`, `hasWon` `120 : 5`,
`m == 0` `5 : 116`), so matching the heuristics needs the counter system
solved, not just per-branch ratios:

* `counter[4]` = loop iterations N (the loop branch true weight is N+1);
* `counter[5]` = hasWon-true count H (the false weight is H+1);
* the uncounted successor blocks are derived as N-H, so the 124:4 target
  needs N = 128, H = 4 (and the loop-exit count 4 must come from whichever
  counter `PGOUseFunc` uses for the uncounted exit block — it is *not*
  counter 0).

Scratch: `/dev/shm/w8117/mkprof.py`, `mkprof2.py`, `relink.sh`,
`link_cmd.sh`, `status_gen.ll`, `sp3.ll`, `mir_full.txt`.

## 2. Open items for the next wave

1. **Status:** build a `.profdata` whose non-switch counters reproduce the
   baseline `BranchProbabilityInfo` weights.  Recipe: compile the baseline TU
   with `-mllvm -print-machineinstrs`, read each MBB's successor percentages,
   and set the corresponding IR counters to those ratios (the loop back-edge
   is 96.88/3.12 in the small test; the real function's branches must be read
   from the full MIR).  Keep the switch counters at low=3, A=2, B=1,
   default=0 (the w7781 map).  If the non-switch `!prof` weights equal the
   heuristics, the profile should be behaviourally identical to no-profile
   except on the switch, and should hit 92.9 % / 1432 B.
2. **createShip:** the faithful all-`int` coordinate block is 8 B *larger*
   than master's mixed `float`/`int` form, so the original's 2764 B cannot be
   the all-`int` form plus a status guard.  The remaining 16 B master spends
   on the (unfaithful) `Globals::status == nullptr` guard is still
   unidentified; a faithful extra local that costs exactly 16 B and keeps the
   LSDA exact is the open lever.
