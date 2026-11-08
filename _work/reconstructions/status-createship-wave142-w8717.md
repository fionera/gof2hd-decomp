# Wave 142 (w8717): `Status::missionCompleted` — new switch-restructuring
# attempts (4-level and 2-level nests with `int shipType`) all lose; the
# 4:3:2:1 `!prof` vector is re-confirmed but still has no source producer.
# `Level::createShip` — the original's jitter/waypoint pairing is now pinned
# (`x+jitterX, y+jitterY, z+jitterZ`), but no source form beats 1411.

Status: **note-only** (no `src/` change landed).  Gate on the unchanged
tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8717`,
base master `1d10d0694`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
  baseline raw same-address diff **722 B**.
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline raw diff **1411 B**, exact 336 B / 57-row `ttype 0xff` LSDA.

Raw diff bytes (not the PYTHONHASHSEED-dependent fuzzy %) are the metric.

## 0. New results

### 0.1 The target `!prof` vector is re-confirmed (558 B) and is razor-sharp

Injecting, on the current optimised IR switch (case order is the `.ll`
order, i.e. source order `167,174,8,0,11,13,14,150..189,156..172`):

```
!prof = branch_weights 1, 3,3, 4,4,4,4,4, 3x12, 2x10
```

gives **raw diff 558 / 1432 B** and reproduces the original's full block
order (`167/174, 8, 11, 13, 14, 150,151,152,153,154,155,158,162,166,170,
184,189, 156,157,160,163,164,165,168,169,171,172`).  This is the same
ceiling reported by wave 134/140/141.

New sweep (injection only, real TU, same-address diff / size):

| vector (default, A, low, B, C) | raw diff | size |
|---|---:|---:|
| `1 3 3 4 3 2` (target) | **558** | 1432 |
| `1 4 4 4 3 2` | **558** | 1432 |
| `1 5 4 3 2` | **558** | 1432 |
| `1 6 8 6 4` (= 2x target) | **558** | 1432 |
| `1 9 12 9 6` (= 3x target) | **558** | 1432 |
| `1 3 3 4 3 3` (C=3) | 722 | 1432 |
| `1 3 3 4 4 2` (B=4) | 1252 | 1372 |
| `1 2 2 3 2 1` (2/3x target) | 1265 | 1380 |
| `1 4 3 2 1` (A,low swapped) | 1174 | 1444 |
| `1 10 10 10 10` (uniform cases) | 722 | 1432 |

So the load-bearing ratios are `A≈3-5, low=4, B=3, C=2, default≤4`; scaling
by 2x/3x is exact, but 2/3x is not (integer probability rounding).

### 0.2 `__builtin_expect` on the switch condition alone loses (1277 / 1392)

`switch (__builtin_expect(m->getType(), 167))` emits `[1,2000,1,...]` and
gives **1277 / 1392 B** — the hot case dominates MBP.  Injecting the same
`[1,2000,1,...]` into the plain IR gives the identical 1277 / 1392, so the
source form is not the problem; the ratio is.

### 0.3 The 4-level nest with `int shipType` folds correctly but emits
### geometric weights — all 256 expect placements lose

A nest of four switches on a single `int shipType` (A | low | B | C, body
order preserved) *does* fold into one switch (unlike the `m->getType()`
version, whose four `getType()` calls are different SSA values and do not
fold).  Sweeping the `__builtin_expect(shipType, e)` expected value of each
of the four switches over `{167, 8, 150, 156, 999}` (256 builds) gives:

* size **1360-1416 B** (never 1432), raw diff **1243-1324** — every one
  worse than the 722 baseline;
* the emitted `!prof` is always geometric (e.g. `0, -335758019, 1979604,
  1974667, 987x4, 981, 0...` for the all-first-case placement), because
  `FoldValueComparisonIntoPredecessors` multiplies each outer case by the
  inner switch's total weight (`L+1` growth).

A 2-level nest (A+low | B+C, same `int shipType`) with 16 expect placements
is the same story: size 1372-1404, diff 1243-1317.

### 0.4 `createShip`: the original's jitter/waypoint pairing is pinned

The original computes the three offsets **before** `setEmpData` and keeps
them as ints in `fp/r4/r8` (c03c8-c03e4), then converts in each branch:

```
fp = waypoint->x + jitter1 - 20000     (x)
r4 = waypoint->y + jitter2 - 20000     (y)
r8 = waypoint->z + jitter3 - 20000     (z)
```

i.e. the jitter declaration order is **X, Y, Z** and the waypoint reads are
x, y, z (offsets 292/296/300).  The current source declares the jitters
`X, Z, Y` (so `y` uses the 3rd `nextInt` and `z` the 2nd).  Measured forms
(real TU, same-address diff / size):

| shape | raw diff | size |
|---|---:|---:|
| master (jitters X,Z,Y; waypoints z,y,x) | **1411** | 2764 |
| jitters X,Y,Z only (pairing fixed) | 1411 | 2764 |
| waypoints x,y,z only | 1461 | 2764 |
| waypoints x,y,z + jitters X,Y,Z | 1461 | 2764 |
| declaration order z,x,y + jitters X,Y,Z | 1412 | 2764 |
| `int x,y,z` (no float locals) | 2419 | 2772 |
| `x,y,z` computed before `new Player` | 2334 | 2768 |
| guard removed (faithful body) | 2354 | 2748 |

The jitter declaration order is codegen-neutral (the `nextInt` call order is
fixed and the compiler schedules the adds identically); the waypoint read
order x,y,z is a 50-byte regression.  The 1411 guard build is still the
local optimum.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8717/fdiff.py _ZN6Status16missionCompletedEbbx
python3 /dev/shm/w8717/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch (`/dev/shm/w8717/`): `fdiff.py` (raw-diff scorer), `cc_ll.sh`
(exact-TU `-S -emit-llvm`), `cc_obj.sh` (exact-TU `.ll` -> `.obj`),
`linkcmd.sh` (checked-in gold link command), `mk.py` (inject
`branch_weights`), `nest2.py` / `nest3.py` (4-/2-level nest generators),
`sweep*.py`, `exp.py` / `exp2.py` (decode both `tbh` tables and print the
block order).

## 2. Open items

1. **Status:** the 4:3:2:1 vector is still the only known producer of the
   558 B layout, and no `__builtin_expect`/fold source shape emits it.  The
   remaining untried lever is a *real* profile on a clang that does not add
   the `prof` attribute (wave 141 showed the attribute changes the entry
   block), or a non-geometric fold (e.g. a switch whose predecessor is an
   `if` with a *small* `-mllvm -likely-branch-weight`, which the fixed build
   flags do not use).
2. **createShip:** the missing 16 B is still a *statement* that keeps
   `Globals` live across `setEmpData` yet is consumed before it, and that
   pins `x/y/z` in `fp/r4/r8` across the call.  The pairing is now known to
   be `x+jitterX, y+jitterY, z+jitterZ`; the RA (`type->fp`,
   `shipClass->r5/sl`, `race->[sp,#60]`) is still not reachable.
