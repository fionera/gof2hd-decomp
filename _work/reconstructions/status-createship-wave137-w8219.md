# Wave 137 (w8219): `Status::missionCompleted` — the 4:3:2:1 vector is
# confirmed to be a *layout* target (injection 92.9 %), the fold is now
# exhausted with U != 1 and case-targeted expects; `Level::createShip` —
# jitter-declaration permutations and the VLA/try forms re-measured

Status: **note-only** (no `src/` change landed).  Gate on the unchanged tree:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8219`, base master
`655e3bcc5`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**, baseline
  **71.0 %** (523/525 insns).
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline **70.6 %** (1007/1005 insns), exact 336 B / 57-row `ttype 0xff`
  LSDA (`.ARM.extab` 156,312 B, exact).

## 0. Summary of new results

1. **The 4:3:2:1 vector is a *block-placement* target, not an instruction
   target.**  Injecting `!prof !{!"branch_weights", i32 1, i32 3, i32 3,
   i32 4,4,4,4,4, i32 3x12, i32 2x10}` into the *current* IR (source order
   167,174,8,0,11,13,14,150..189,156..172) reproduces the original's **entire
   jump-table block order** and scores **92.9 % / 1432 B / 523 insns** (the
   w7761 result, re-confirmed).  The remaining 7 % is *source-level* (see §3),
   not weights.
2. **The one-hot `switch (__builtin_expect(t, K))` route is dead.**  K ∈
   {8, 167, 150, 156, 0} on the real TU scores **18.7 / 27.3 / 22.2 / 20.2 /
   26.4 %** and shrinks the body to 1384-1424 B.  Confirms
   `LowerExpectIntrinsic::handleSwitchExpect` sets exactly one weight.
3. **The fold search is now exhausted with U != 1 and case-targeted
   expects.**  New sweeps (all compiled with the real clang 7.0.2, reading
   the emitted `!prof` metadata):
   * every consecutive 1/2/3-level split of the four source-order groups
     `g1={167,174}`, `g2={8,0,11,13,14}`, `g3={150..189}`,
     `g4={156..172}`, with `__builtin_expect` targets ∈ {none, default,
     first value of every group} at every level, `L ∈ 1..32`, `U ∈ 1..16`
     (8 splits x up to 6^3 target combinations x 512 (L,U) pairs).
   * No combination produces a per-case weight vector proportional to
     `(default,g1,g2,g3,g4) = (1,3,4,3,2)`.  The families the fold can make
     are geometric/arithmetic (e.g. the 3-level all-default nest gives
     `default=L, g1=858U, g2=143LU, g3=11L, g4=L`), never 1:3:4:3:2.
   * Scratch: `/dev/shm/w8219/search2.py`, `search3.py`, `search4.py`,
     `gen3.py`.
4. **The `.profdata` route is reproduced end-to-end** (text profile ->
   `llvm-profdata merge` -> `-fprofile-instr-use` -> relink): the switch
   metadata comes out **exactly** `i32 1, 3, 3, 4, 4, 4, 4, 4, 3x12, 2x10`,
   but the score is **17.9 % / 1416 B** because every *other* branch is
   re-weighted.  Tuning the loop counter (the w8117 open item) only reaches
   **22-24 %** (`counter[4] = 127`, `= 1000`; `counter[5] = 3`), so the
   profile's non-switch counters are still unsolved.  Scratch:
   `/dev/shm/w8219/mkprof.py`, `profbuild.sh`, `profvar.py`.
5. **Why the profile can never be the whole answer:** the injection (switch
   weights only, every other branch on the `BranchProbabilityInfo`
   heuristic) already matches the original's block order.  If the original
   had been PGO-built its non-switch branches would carry profile weights;
   the fact that the heuristic-only injection is 92.9 % says the original's
   non-switch branches are heuristic.  So the 4:3:2:1 must be a
   *source-level* producer — and the fold search says clang 7.0.2 has none.
6. **createShip: the jitter-call order is not the lever.**  The original's
   three `nextInt(40000)` results land in `[sp,#36]=jitterZ`, `[sp,#32]=
   jitterY`, `[sp,#28]=jitterX` (calls cannot be reordered), i.e. the source
   order is Z,Y,X, while master declares X,Z,Y.  All six declaration
   permutations were built:

   | order | score | usize |
   |---|---:|---:|
   | **XZY (master)** | **70.6** | **2764** |
   | ZXY | 70.6 | 2764 |
   | XYZ | 70.5 | 2764 |
   | YXZ | 70.5 | 2764 |
   | ZYX | 62.6 | 2768 |
   | YZX | 62.5 | 2768 |

   XZY and ZXY are byte-identical, so the slot map is driven by live ranges,
   not declaration order.
7. **createShip: the mission's VLA / try forms re-measured** (all lose):

   | variant | score | usize |
   |---|---:|---:|
   | `volatile int vn = hostile?1:2; int vla[vn]; vla[0]=x;` before setEmpData | 52.8 | 2864 |
   | `int vla[hostile?1:2]; vla[0]=x; ... empTime + vla[0]*0` | 61.5 | 2772 |
   | all-`int` x,y,z (faithful) | 61.5 | 2772 |
   | `try { setEmpData } catch (...) { __builtin_trap(); }` | 67.1 | 2752 |

   The `try` form moves `.ARM.extab` off 156,312 B, so it cannot land.

## 1. Where the Status difference actually is

Decoding the original's `tbh` table (tbh @ `0xabfe2`, 40 entries for
`type-150`) gives the block order.  The original places the g4 bodies
(156,157) **last** (`0x3f2`, `0x41c`, after 189 at `0x3b6`); the current
baseline places them **early** (`0x274`, `0x2a8`, right after 155).  Every
other body is in the same order.  So the only thing the weights change is
where 156/157 go — exactly the 71 % -> 92.9 % jump.

With the injection the block *order* is identical; the residual 7 % is a
handful of real instruction differences in the case bodies:

* original has an extra `ldr r6,[sp,#16]` (reload `this`) at `0x298` (end of
  the case-158 loop) and at `0x440`; master reloads it later at `0x36a`.
* master has an extra `cmp r0,#0; ldr r6,[sp,#16]; beq; b` at `0x368`.
* register allocation around `0x32e..0x344` (`mov r9,r0` vs `mov r5,r0`).
* original has a `nop` at `0x4be`.
These are *source* differences in the case bodies (missing/extra null checks
or a different local), not weights.

## 2. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8219/score.py _ZN6Status16missionCompletedEbbx \
                                 _ZN5Level10createShipEiiiP8Waypointbb

# IR injection (92.9 %):
python3 /dev/shm/w8219/inject.py 1 3 3 4 4 4 4 4 3 3 3 3 3 3 3 3 3 3 3 3 2 2 2 2 2 2 2 2 2 2

# fold searches:
python3 /dev/shm/w8219/search2.py   # all-default nests, L 1..32 U 1..16
python3 /dev/shm/w8219/search4.py   # + case-targeted expects, per-case check

# .profdata route (17.9 %):
python3 /dev/shm/w8219/mkprof.py && \
  .cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata \
    merge /dev/shm/w8219/p.txt -o /dev/shm/w8219/p.profdata
bash /dev/shm/w8219/profbuild.sh /dev/shm/w8219/p.profdata
```

## 3. Open items for the next wave

1. **Status:** the only remaining source-level lever is a producer of a
   *four-value* switch weight vector.  clang 7.0.2 has exactly two
   (`handleSwitchExpect` one-hot, `FoldValueComparisonIntoPredecessors`
   multiplicative) and neither can make 1:3:4:3:2.  Options: (a) a
   checked-in `.profdata` whose non-switch counters are solved so the
   branch weights equal the `BranchProbabilityInfo` heuristics (then the
   injection's 92.9 % is the ceiling, and the remaining 7 % is §1);
   (b) accept that the original was built with a *different* clang/flag set
   and reconstruct the 7 % source differences first.
2. **createShip:** the 16 B the original spends instead of master's
   (unfaithful) `Globals::status == nullptr` guard is still unidentified.
   The prologue RA (`type -> fp`, `shipClass -> r5`, `race -> sl`) is the
   first divergence; no declaration-order or VLA/try form reproduces it
   while keeping the exact 336 B LSDA.
