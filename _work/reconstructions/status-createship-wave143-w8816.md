# Wave 143 (w8816): `Status::missionCompleted` — `SimplifyCFG` fold weights are
# measured from source (they are `{likely, N+1}` products, never 4:3:2:1 at the
# default likely weight); `Level::createShip` — the missing 16 B is identified:
# the two extra `Globals::status->getLevel()` calls of a `?:` min-clamp.

Status: **note-only** (no `src/` change landed).  Gate on the unchanged tree:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.  Worktree `wt/w8816`, base
master `bf083ab41`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
  baseline raw same-address diff **722 B**.
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline raw diff **1411 B**.

Raw diff bytes (not the PYTHONHASHSEED-dependent fuzzy %) are the metric.

## 0. New results

### 0.1 The switch weights are *not* a `!prof` injection artefact (re-confirmed)

Injecting `branch_weights 1, 3,3, 4,4,4,4,4, 3x12, 2x10` into the current
`Status.cpp` IR switch still gives **558 / 1432** and reproduces the original's
full MBB order.  Sweep of the 4 group weights on the real TU:

| vector (default, A(167/174), low(8,0,11,13,14), B(12), C(10)) | raw diff | size |
|---|---:|---:|
| `1 3 4 3 2` (target) | **558** | 1432 |
| `1 4 4 3 2` | **558** | 1432 |
| `2 3 4 3 2` … `10 3 4 3 2` | **558** | 1432 |
| `1 3 3 3 2` (A=3) | 1254 | 1372 |
| `1 2 4 3 2` (A=2) | 1254 | 1372 |
| `1 3 5 3 2` (low=5) | 1055 | 1444 |
| `1 3 4 4 2` (B=4) | 1252 | 1372 |
| `1 3 4 3 3` (C=3) | 722 | 1432 |
| `100 3 4 3 2` | 1291 | 1416 |

So the load-bearing constraints are `low=4 > A=3 > C=2`, `default <= 10`,
`167/174` may be 3 or 4.  `handleSwitchExpect` alone (one case 2000, rest 1)
gives 1245–1336 for every one of the 29 possible expected cases — all worse
than 722.

### 0.2 The fold weights, measured from real source (new)

`SimplifyCFG::FoldValueComparisonIntoPredecessors` (`SimplifyCFG.cpp:1005`,
LLVM 7.0.2) folds a switch that is the *default* successor of an `if`/switch
on the same value.  For `PredDefault == BB` it computes

```
new default  = Weights[0] * SuccWeights[0]
new succ case= Weights[0] * SuccWeights[i+1]
pred case    = Weights[1] * ValidTotalSuccWeight   (sum of SuccWeights)
```

`Weights[0]` is the *false* weight for an `icmp eq` branch (GetBranchWeights
swaps it to the front).  Measured with clang 7.0.2 at `-Oz` on minimal
functions (`/dev/shm/w8816/s3..s10.cpp`):

| source shape | emitted `!prof` |
|---|---|
| `if (__builtin_expect(x==1,0)) … else switch(x){2,3}` | `4, 3, 4, 4` @ likely=4; `2000, 3, 2000, 2000` @ likely=2000 |
| `if (__builtin_expect(x==1,1)) … else switch(x){2,3}` | `1, 12, 1, 1` @ likely=4; `1, 6000, 1, 1` @ likely=2000 |
| 3-deep `switch` nest, innermost `__builtin_expect(x,2)` | `1, 4002, 4000, 1` |
| 3-deep `switch` nest, outer `__builtin_expect(x,1)` | `1, 8000, 2, 1` |

i.e. a fold gives one *small* weight (`ValidTotalSuccWeight`, the successor
count of the inner switch) and the rest get the likely weight.  To hit the
target vector the "rest" would have to be 4/3/2, but the only small numbers a
fold produces are successor counts (2, 3, 4, …) *and* they are attached to the
folded case, not to the bulk.  With the default `likely=2000` the bulk is
always 2000 or 6000/8000, never 4.  A 5-level if-chain of
`__builtin_expect(x==Ci,0)` produces `{2000^k}` products
(`[250000000, -1290465046, …, 1501500, 1001000250, …]`) and overflows 32 bits
before `FitWeights` can bring it down to small integers.

**Conclusion:** the 4:3:2:1 vector cannot be produced by a
`FoldValueComparisonIntoPredecessors` fold at clang's default
`-likely-branch-weight=2000`.  It needs either a small likely weight
(`-mllvm -likely-branch-weight=4`, a build-flag change the fixed flags do not
make) or real profile counts (`count+1` gives exactly 4:3:2:1).  The profile
route is still ruled out by the consecutive-case IR shape: `-fprofile-instr-use`
sets `hasProfileClangInstr()`, which makes `EmitCaseStmt` give each of
`case 167: case 174:` and `case 0: case 11:` their own block
(`CodeGenFunction::EmitCaseStmt`, `CGStmt.cpp:1291`), while the original's
`tbh` points 167/174 at the *same* `0xac036` and 0/11 at the same `0xac0c4`.
A profile build of the real TU is 1524 B (all-zero counters) / 1412–1476 B
(tuned counters), never 1432.

### 0.3 `createShip`: the missing 16 B is the `?:` min-clamp (new)

The original's `level` and `empPoints` computations each call
`Globals::status->getLevel()` **twice**:

```
c0284: ldr r0,[r4,#0] ; c0286: blx getLevel ; c028a: cmp r0,#20
c028c: str r5,[sp,#28] ; c028e: ble c0294 ; c0290: movs r5,#20 ; c0292: b c029c
c0294: ldr r0,[r4,#0] ; c0296: blx getLevel ; c029a: mov r5,r0
```

and identically for `empPoints` at `c0314`/`c0322`.  The current source
(`int level = Globals::status->getLevel(); if (level > 20) level = 20;`) emits
a single call + `it ge; movge` (4 B).  The two-call shape is exactly what
`int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();`
produces (verified by compiling the minimal form and the real TU).  Two extra
calls = **16 B**, and `faithful (2748) + 16 = 2764`, the original size.  This
is the "statement that keeps `Globals` live [longer] yet is consumed before
`setEmpData`" — it extends `&Globals::status` (r4) to `c0324`, still before the
`setEmpData` at `c03ee`.

Measured on the real TU (raw diff / size):

| shape | raw diff | size |
|---|---:|---:|
| master (bogus `Globals::status == nullptr` guard, 1 call each) | **1411** | 2764 |
| faithful (guard removed, 1 call each) | 2354 | 2748 |
| faithful + `?:` min-clamp (2 calls each) | 2055 | **2764** |
| guard + `?:` min-clamp | 2443 | 2780 |
| `int x,y,z` + guard | 2424 | 2772 |
| `int x,y,z` + `?:` + no guard | 2388 | 2792 |

So the 16 B is now *identified* (`?:` min-clamp), but the guard build still
wins on the raw same-address metric (1411 vs 2055): the guard's 16 B happen to
restore the original's code addresses even though its code is wrong.

**The master guard is METRIC-ONLY.**  The original has no
`Globals::status == nullptr` check: after `setEmpData` it goes straight to
`mov.w r9,#0; cmp r6,#0; beq …` (`0xc03f2`).  The guard is a 16-byte
placeholder that happens to restore the size/addresses; the faithful 16 bytes
are the `?:` min-clamp's two extra `getLevel()` calls.  The faithful form
(`?:`, no guard) is 2764 B — size-exact — but raw 2055.  Additional faithful
variants measured: `x,y,z` declaration order x→y→z 2056, moving the
`race==9` check before the shipClass branch 2055.  The `?:` form's empPoints
region now matches the original exactly through
`str.w r9,[sp,#52]; cmp r1,#1` (this spilled at the same point); the remaining
swap is `type` (ours: kept in `sl`; original: spilled to `[sp,#52]`) vs
`shipClass` (ours: `[sp,#60]`; original: `r5`).  The
remaining faithful-form gap is register allocation: the original keeps
`race=[sp,#60]`, `this=r9`, `type=fp`, `shipClass=r5` (one spill); both the
faithful and the `?:` forms spill *two* of `{race, shipClass}`
(`strd r1,r2,[sp,#56]` / `str r2,[sp,#60]; str r1,[sp,#52]`).

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8816/fdiff.py _ZN6Status16missionCompletedEbbx
python3 /dev/shm/w8816/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch (`/dev/shm/w8816/`): `fdiff.py` / `score.py` (raw-diff scorers),
`relink.py` (checked-in gold link + strip + shstrtab with one object
replaced), `cc.py` / `tryst.sh` / `trylv.sh` (exact-TU compile + link + score),
`sweep.py` / `sweep2.py` (IR `!prof` injection sweeps), `inject.py`,
`decode.py` (tbh decoder), `sdiff.py` / `idiff.py` (sequence/address diffs),
`s3.cpp`…`s10.cpp` (fold-weight minimal cases), `lv_*.cpp` (createShip forms).

## 2. Open items

1. **Status:** the only remaining producer of the 4:3:2:1 ratio is a small
   `-likely-branch-weight` (a build-flag change) or a real profile (blocked by
   `hasProfileClangInstr`'s consecutive-case splitting).  If the coordinator is
   willing to add `-mllvm -likely-branch-weight=4` globally, the fold form
   `if (__builtin_expect(type == C, 0)) … else switch(type)` emits `{4,3,4,4}`
   and is worth a full-TU gate run; the 9 files that already use
   `__builtin_expect` would all change, so it must be measured as a whole.
2. **createShip:** with the 16 B identified, the next lever is the parameter
   register allocation (`race` spilled vs `shipClass` spilled).  The original
   has exactly one parameter spill; every faithful form tried spills two.  A
   source form that lowers the live-range pressure by one register (e.g. a
   different `mission`/`waypoint`/`jitter` declaration order) is the open
   question.
