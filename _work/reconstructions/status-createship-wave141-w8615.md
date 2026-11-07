# Wave 141 (w8615): `Status::missionCompleted` — the checked-in `.profdata`
# route is measured end-to-end and **loses** (1355–1388 raw diff vs 722),
# because the profile record changes the IR shape even before any branch
# weight is emitted.  `Level::createShip` — two more faithful non-EH forms,
# both worse than the guard build.

Status: **note-only** (no `src/` change landed).  Gate on the unchanged
tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8615`,
base master `e18e0c2a4`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
  baseline raw same-address diff **722 B**.
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline raw diff **1411 B**, exact 336 B / 57-row `ttype 0xff` LSDA.

Raw diff bytes (not the PYTHONHASHSEED-dependent fuzzy %) are the metric.

## 0. New results

### 0.1 The `.profdata` route is now measured end-to-end (and loses)

The wave-140 open item was "the `.profdata` route with the non-switch
counters tuned to the `BranchProbabilityInfo` heuristics".  This wave built
the real text profile (`:fe` / name / `2003737435339834133` / 116 counters,
`llvm-profdata merge`), compiled the real `Status.cpp` TU with
`-fprofile-instr-use=...`, ran the real gold link, and scored the real
function.  The switch case counters were set to the exact wave-135 values
(`low {8,0,11,13,14}` = 3, `A {167,174,150,151,152,153,154,155,158,162,
166,170,184,189}` = 2, `B {156,157,160,163,164,165,168,169,171,172}` = 1,
i.e. `!prof` weights `1,3,3,4,4,4,4,4,3x12,2x10`), and every other counter
was swept over `{0,1,100,1000}`:

| non-case counters | raw diff | size |
|---|---:|---:|
| 0   | 1388 | 1476 |
| 1   | 1355 | 1416 |
| 100 | 1369 | 1412 |
| 1000| 1369 | 1412 |

All are **worse than the no-profile baseline (722 / 1432)**.  The wave-134
IR-injection ceiling (558 / 1432) is *not* reachable this way.

**Why: the profile record changes the IR shape before any weight is used.**
With a profile record for `missionCompleted` but *all counters zero* (so
`createProfileWeights` returns `nullptr` for every branch and the switch),
the emitted IR is already different from the no-profile IR: the entry block
keeps `%5 = getelementptr [115]` and branches to `%6`, whereas the no-profile
entry block hoists the `shortMission`/`longMission` `xor/icmp/and` chain and
branches to `%22` (block count is 101 in both, control-flow shape identical,
instruction placement different).  A profile *without* a
`missionCompleted` record leaves the IR byte-identical to baseline.  So the
difference is caused by the record itself (the `prof` function attribute /
`!prof` entry-count metadata), not by the branch weights.  The original's
`prof`-attributed IR therefore cannot be the wave-134 injection IR; the
558 B ceiling is an artefact of injecting weights into the *no-profile* IR.

**The counter→weight map is not solvable as one linear system at the
all-equal base.**  A finite-difference matrix (116 compiles, one counter
`+1` at a time from an all-1000 base) predicts the real weights only for 15
of 75 branches.  `ComputeRegionCounts` (`CodeGenPGO.cpp`) derives many
counts as `ParentCount - ThenCount` and `max(CondCount, LoopCount) -
LoopCount`; at the all-equal base these underflow (e.g. the `!isCampaign`
`&&`-chain branch emits `i32 -1, i32 1` = `2^32:1`), so the finite
differences are not local derivatives.  Fitting the 75 heuristic ratio
constraints (`31:1`, `3:5`, `5:3`, `1:1`) gives rank 64/75 and is
inconsistent.  A sane (non-degenerate) base would require a real execution
profile, which is not available.

Scratch (`/dev/shm/w8615/`): `fdiff.py` (raw-diff scorer), `cc.sh` (exact-TU
compile), `mkprof.py` / `build_status.py` (text profile + exact compile +
gold link + score), `matrix.py` (116-compile finite-difference matrix),
`solve.py` (rank/consistency), `inject_switch2.py` (IR `!prof` injection,
reproduces 558), `linkcmd.sh` (checked-in gold link command).

### 0.2 `createShip`: two more faithful (guard-free) forms

Both drop the bogus `Globals::status == nullptr` guard (the original has no
such check) and keep the exact 2764 B / 57-row LSDA structure intact:

| shape | raw diff | size |
|---|---:|---:|
| master (guard after `setEmpData`) | **1411** | 2764 |
| faithful, `x/y/z` computed before `new Player` | 2435 | 2752 |
| faithful, `Vector pos` before `setEmpData`, used by both branches | 2306 | 2788 |

The first form's prologue spills `this` (`str r0,[sp,#60]`), puts
`shipClass` in `r9` and `type` in `r5` — i.e. it re-rolls the RA even
further from the original (`race->[sp,#60]`, `this->r9`, `type->fp`,
`shipClass->r5/sl`).  The second form is 24 B too large.  Neither beats the
guard build, so the guard remains the best local optimum at 1411.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8615/fdiff.py _ZN6Status16missionCompletedEbbx
python3 /dev/shm/w8615/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
```

## 2. Open items

1. **Status:** the `.profdata` route is closed unless the `prof`-attribute
   IR change can be suppressed (no clang 7.0.2 flag found; the `prof`
   inline attribute is not even re-parseable by this clang — `llvm-as`
   rejects `define ... align 2 prof {`).  The only remaining producers of
   the 4:3:2:1 vector are a real profile on a clang that does not add the
   attribute, or a different backend.
2. **createShip:** the missing statement still has to (a) keep `Globals`
   alive across `setEmpData` yet (b) be consumed before it, and (c) pin
   `x/y/z` before the call.  The original's `[sp,#56]` slot (loaded just
   before the `shipClass == 1` branch) is still unidentified.
