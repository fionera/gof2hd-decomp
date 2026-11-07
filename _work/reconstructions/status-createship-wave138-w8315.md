# Wave 138 (w8315): `Status::missionCompleted` — the original's block order
# is the *source* order (A, low, B, C) while master's is sorted; the
# two-switch fold emits `{1,2010,2000}` / `{1,2022,4044000}` families but
# never 4:3:2:1.  `Level::createShip` — `switch (shipClass)` is a no-op,
# the status guard is load-bearing for the current RA

Status: **note-only** (no `src/` change landed).  Gate on the unchanged
tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8315`, base
master `96b069792`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
  baseline **71.0 %** (523/525 insns).
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline **70.6 %** (1007/1005 insns), exact 336 B / 57-row `ttype 0xff`
  LSDA.

## 0. Summary of new results

1. **The original's switch block order is exactly the source case order.**
   Decoding both `tbh` tables (base `0xabfe6`, 40 entries for type-150;
   base `0xac052`, 7 entries for type-8) gives, in *address* order:

   ```
   original: 167/174, 8, 0/11, 13, 14, 150,151,152,153,154,155,158,162,166,
             170,184,189, 156,157,160,163,164,165,168,169,171,172, default
   master:   167/174, 8, 11, 13, 14, 150,151,152,153,154,155,156,157,158,
             160,162,163,164,165,166,168,169,170,171,172,184,189, default
   ```

   The original is the *source* order (group A `{167,174}`, group low
   `{8,0,11,13,14}`, group B `{150..189}`, group C `{156..172}`); master's
   is **sorted by case value within B∪C** (156/157/160 interleave B).  So
   the whole difference is that the backend sorts the single jump-table
   cluster in master but not in the original — i.e. the original's block
   placement is weight-driven, exactly as the w8219 injection showed.

2. **The IR switch case order is the source order** (confirmed by
   `-emit-llvm` on the real TU: `switch i32 %52` lists 167,174,8,0,11,13,14,
   150,...189,156,...172).  So clang does *not* sort; the sorting happens in
   `MachineBlockPlacement` because a jump-table switch with equal weights
   lays the case blocks out in jump-table (sorted) order.

3. **A real two-switch source shape cannot produce 4:3:2:1.**  Measured with
   the real clang 7.0.2 (`-emit-llvm`, reading the emitted `!prof`):

   | shape | emitted `branch_weights` |
   |---|---|
   | `switch (__builtin_expect(t, 8))` outer + `switch (__builtin_expect(t, 150))` inner (default) | `1, 2022, 2022, 4044000, 2022x4, 2000, 1x21` |
   | same, no outer expect, inner `__builtin_expect(t, 156)` | `1, 2002 x8, 2000, 1` |
   | two-switch fold, **no** expects on either | **no `!prof` at all** |
   | `select`/PHI of two expects feeding the switch | no `!prof` |
   | if-chain with `__builtin_expect` (converted to a switch) | no `!prof` |

   The fold is multiplicative: `outer_case_i *= sum(inner_weights)` and
   `inner_case_j = outer_default * inner_weight_j` (verified numerically:
   outer one-hot 2000/1 x inner one-hot 2000/1 -> `1, 2022, 2022, 4044000,
   ...`).  With the real group sizes a no-expect outer + no-expect inner
   would give `A=low=B=11, C=1, default=1`, but the fold emits **no
   metadata** in that case, so the backend still sorts.

4. **In the real TU the two-switch shape loses.**  Moving group C into a
   nested `switch` in the outer default (with `missionType` cached and
   `__builtin_expect(missionType, 156)` on the inner) folds to one switch
   with weights `1, 2010x19, 2000, 1x9`, but the function shrinks to
   **1368 B / 43.6 %** — the fold also removes the separate case-0 handling
   and re-rolls the layout (13/14 move to the end).  A plain two-switch
   split (A+low | B+C) gives **1456 B / 51.8 %**.  Neither is a win.

5. **Conclusion for Status:** clang 7.0.2 has no source-level producer of a
   four-value switch weight vector.  The only producers are
   `handleSwitchExpect` (one-hot) and `FoldValueComparisonIntoPredecessors`
   (multiplicative `{1, 2010, 2000}` family).  The 4:3:2:1 vector is a
   `-fprofile-instr-use` artifact (w7781: counter+1 = 4:3:2:1); the profile
   route needs the non-switch counters solved to the `BranchProbabilityInfo`
   heuristics, which w8219 measured at 22-24 % at best.

6. **createShip: `switch (shipClass)` is a no-op.**  Rewriting the
   `if (shipClass == 1) { ... } else { ... }` hitpoint/emp block as a
   `switch (shipClass) { case 1: ... default: ... }` compiles to the
   **identical** 2764 B / 70.6 % body (clang folds it back to if/else).

7. **createShip: the `Globals::status == nullptr` guard is load-bearing for
   the current RA.**  Removing it (the faithful original has no such check —
   it goes straight to `mov.w r9,#0; cmp r6,#0; beq` after `setEmpData`)
   gives **2748 B / 63.6 %**: the RA re-rolls completely (`Globals::status`
   moves r4->r6, `fp`/`sl` swap, the coordinate block moves after
   `setEmpData`).  So the 16 B the original spends is *not* simply "the
   guard removed"; the faithful body needs a different source shape that
   also restores the original RA (`this -> r9`, `race -> [sp,#60]`,
   `type -> fp`, `shipClass -> r5/sl`).

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8315/score.py _ZN6Status16missionCompletedEbbx \
                                 _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch: `/dev/shm/w8315/` — `score.py` (single-function scorer built on
`tools/verify/verify.py`), `cmp.py` (instruction-sequence diff),
`t/h.py` + `t/gen.py` + `t/exotic.py` (clang 7 `!prof` shape harness),
`status_orig.txt` / `status_ours.txt`, `createship_orig.txt` /
`createship_ours.txt`, `createship_noguard.txt`.

## 2. Open items for the next wave

1. **Status:** land the `.profdata` route with the non-switch counters tuned
   to the `BranchProbabilityInfo` heuristics (w8117 gives the counter map:
   `counter[4]` = loop N, `counter[5]` = hasWon H, target 124:4 needs
   N=128, H=4).  If the switch metadata alone is 92.9 %, the remaining 7 %
   is the §1 instruction differences.  Alternatively accept that the
   original was built with a different profile and reconstruct the 7 %
   source differences (the `ldr r6,[sp,#16]` placement at the end of the
   case-158 loop, and the `mov r9,r0` vs `mov r5,r0` at `0x32e..0x344`).
2. **createShip:** find a faithful source shape that (a) drops the bogus
   `Globals::status == nullptr` guard, (b) keeps the original RA
   (`this -> r9`, `race -> [sp,#60]`, `type -> fp`, `shipClass -> r5/sl`),
   and (c) keeps the exact 336 B / 57-row LSDA.  The current body is a
   local optimum at 70.6 % / 2764 B.
