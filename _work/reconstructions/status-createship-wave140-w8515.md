# Wave 140 (w8515): `Status::missionCompleted` — a two-switch fold can
# **never** emit the 4:3:2:1 vector (proof: the inner group's weight is
# always <= the default weight).  `Level::createShip` — three more
# non-EH forms measured, none beats 2764 B / 1411 raw diff

Status: **note-only** (no `src/` change landed).  Gate on the unchanged
tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8515`,
base master `d4857f174`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
  baseline raw same-address diff **722 B** (71.0 %).
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline raw diff **1411 B** (70.6 %), exact 336 B / 57-row `ttype 0xff`
  LSDA.

Raw diff bytes (not the PYTHONHASHSEED-dependent fuzzy %) are the metric.

## 0. New results

1. **Proof that no two-switch fold can emit the 4:3:2:1 vector.**
   `SimplifyCFG::FoldValueComparisonIntoPredecessors` (LLVM 7
   `lib/Transforms/Utils/SimplifyCFG.cpp:1005`, the `PredDefault == BB`
   branch) folds an inner switch sitting in the outer switch's default as:

   ```
   outer case i : Weights[i] * ValidTotalSuccWeight
   inner case j : Weights[0]  * SuccWeights[j+1]
   new default  : Weights[0]  * SuccWeights[0]
   ```

   `handleSwitchExpect` (`LowerExpectIntrinsic.cpp:50`) only ever writes
   `{likely=2000, unlikely=1}`: one case gets 2000, *every* other case and
   the default get 1.  Therefore, for the inner switch,

   * expect on a case  -> `wI_j = 1` for j != expected, `wI_default = 1`
   * expect on default -> `wI_j = 1` for all j, `wI_default = 2000`
   * no expect         -> `wI_j = wI_default = 1`

   In every case `inner_case_weight <= default_weight`.  The original needs
   `C = 2 > default = 1`, so **no two-switch source shape can produce it**.
   (A 3-level nest has the same property for its innermost group, so the
   w139 exhaustive search returning 0 hits is not a search artifact — it is
   structural.)  This closes the question in the mission.

2. **The fold vectors measured on the real TU lose.**  I injected the
   folded `!prof` vector straight into the real `Status.cpp` IR (the folded
   source produces exactly this single switch; w139 showed case order is
   irrelevant, so the injection is a faithful proxy).  Raw diff / size:

   | vector (default, A, low, B, C) | raw diff | size |
   |---|---:|---:|
   | `1 3 4 3 2` (required, w139 ceiling) | **558** | 1432 |
   | `1 2010 2010 2010 2000` (fold, no hot case) | 811 | 1432 |
   | `1 2010 2010 2010 2000` with 156=2000, C-rest=1 (exact fold) | 1258 | 1368 |
   | `1 3 3 3 2` (low=3) | 1254 | 1372 |
   | `1 3 4 3 1` (C=1=default) | 1265 | 1380 |
   | `1 3 4 2 2` | 884 | 1428 |

   The required vector is **scale-invariant** (`1 6 8 6 4` and
   `1 30 40 30 20` both give 558), but the *ratio* is exact: `low=4` is
   load-bearing (dropping it to 3 shrinks the body to 1372) and `C=2 >
   default=1` is load-bearing (C=1 gives 1380).  Since the fold cannot give
   `C > default`, the profile route (`counter+1`) remains the only known
   producer of the required ratio.

3. **createShip: three more non-EH forms, all worse.**  Measured with the
   real TU (compile `Level.cpp`, relink, raw diff):

   | shape | raw diff | size |
   |---|---:|---:|
   | master (guard after `setEmpData`) | **1411** | 2764 |
   | guard moved before the `x/y/z` block (after `new Player`) | 2466 | 2776 |
   | `Vector pos` hoisted before `setEmpData`, reused in both branches | 2416 | 2776 |
   | faithful body, guard removed | 2354 | 2748 |

   None recovers the original RA.  Prologues (first four instructions after
   the `vpush`):

   ```
   orig : str r1,[sp,#60]  mov r9,r0  mov fp,r3  mov r5,r2
          (race spilled, this->r9, type->fp, shipClass->r5/sl, Globals->r4)
   guard: str r2,[sp,#60]  mov r9,r0  str r1,[sp,#52]  mov sl,r3
          (shipClass spilled, race->[sp,#52], type->sl, Globals->r4)
   faith: strd r1,r2,[sp,#56]  mov r9,r0  mov sl,r3
          (race->[sp,#56], shipClass->[sp,#60], type->sl, Globals->r6)
   ```

   So the guard build is still the best local optimum; the original's
   `race->[sp,#60] / type->fp` assignment is not reachable from either the
   guarded or the faithful body with the forms tried so far.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8515/fdiff.py _ZN6Status16missionCompletedEbbx
python3 /dev/shm/w8515/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch (`/dev/shm/w8515/`): `fdiff.py` (raw-diff scorer), `inject.py`
(inserts `!prof` into the real `Status.cpp` IR switch at `%52`),
`grp.py <default> <A> <low> <B> <C>` (group-uniform weight sweep),
`score.sh <ll> <mangled>` (compile .ll -> obj, run the checked-in gold link
command, score), `cs.sh` (compile `Level.cpp`, relink, score), `decode.py`
(decodes both `tbh` tables), `linkcmd.sh` (the `ninja -t commands link_so`
gold link + strip + shstrtab command, used to bypass ninja's build-log
mtime check when injecting an object).

## 2. Open items

1. **Status:** the `.profdata` route with the non-switch counters tuned to
   the `BranchProbabilityInfo` heuristics is still the only way to land the
   558 B ceiling.  The two-switch source route is now closed by proof.
2. **createShip:** a *statement* (not a declaration permutation) that pins
   `x/y/z` live across `setEmpData` and keeps `Globals` in r4 is still
   unidentified; all three new forms re-roll the RA.
