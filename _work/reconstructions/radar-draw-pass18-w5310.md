# Radar::draw pass 18 (w5310) — source is faithful; the residual is block placement.
# Refreshing the stale report moves the reported score 73.9 -> 74.0 (raw unchanged).

**Status:** research note + generated-report refresh, 2026-11-04. Worktree `wt/w5310`,
branch `wave105-w5310`, base `835748c18`.

## 0. Baseline and result

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Before (source and build at base `835748c18`):

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  raw 73.972210 %  norm 3492/3489  report 73.9 (stale report.json)
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

After (`verify` target re-syncs the generated `report.json`):

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  raw 73.972210 %  norm 3492/3489  report 74.0
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

The source is byte-identical to the base. The raw verifier ratio is unchanged; the
checked-in `report.json` was stale (it still said 73.9 and also carried an older
`Level::createShip` 66.0 / `Generator::createMission` row set). Re-running the
`verify` target rewrites it to the current 74.0 / 66.4 report.

## 1. REA evidence: the source logic is faithful

`rea decompile` of `_ZN5Radar4drawEP6PlayerP3Hudi` (Ghidra 12.1.2, full 1812-line
pseudocode saved under `/dev/shm/w5310/ghidra-draw.c`) was compared block by block
against `src/game/weapons/Radar.cpp`. Every control-flow decision matches:

* mission-type filter (`11/0/13/171`, else `!=172`);
* `onPlanetCourse` (single `goingToPlanet()` call, CSE'd by clang exactly as the
  original; the two source spellings in the branches compile to the same 0x136..0x16a
  sequence);
* the four radar-background `DrawImage2D` calls and the route/waypoint block;
* both `otherLock` chains, the landmark loop (`i!=2`, `isVisible`, hit box, `i!=3`
  image, `i<=1` label block), the station-lock timer tail;
* the planet loop (campaign>=2, warp-gate image, campaign/freelance markers, planet
  hit box, planet-lock timer tail);
* the enemy loop (`isActive`, wreck predicate, `stealFlagByte`/`countsAsEnemyExcludeFlag`,
  on-screen 24000-unit bounds, hull/EMP bars, lock image, tractor block, enemy-lock
  timer, cargo catch);
* the gas-cloud loop (sparks, equipment 35/33/34, in-sight box, gas-lock timer);
* the asteroid loop (5 slots, nearest-slot selection, asteroid lock timer);
* the music tail (bitmask `0x23c1c8f` for 127..152, `0xe071` for 136..151,
  `2238..2242`, race table `{134,139,138,137}`).

The instruction streams agree instruction-for-instruction in the hot prologue
(0x0..0x1c6) and in the whole music tail (0x2224..0x22d6), apart from register
allocation and the two swapped `current != 151` / `alwaysEnemySeen` tests at
0x2248 (semantically identical).

## 2. Exact remaining diff (the residual is codegen, not statements)

The normalized stream is 3492 (orig) / 3489 (ours); only 3 instructions differ in
count. The mismatch is block order and register/stack colouring:

1. **First `otherLock` cold block.** Original puts `otherLock = field_0x14 != 0`
   at `0xb72` (`ldr r5,[r4,#20]`, `r5` = otherLock), right after the landmark
   tail and before the second cold block at `0xb7e`. Ours puts it at `0x25aa`
   (`ldr.w r9,[r4,#20]`, `r9` = otherLock), at the very end of the function.
   This is the single largest diff region: 23 instructions move.
2. **Second `otherLock` cold block.** Original `0xb7e`, ours `0xb3c` (same
   5-instruction body, only the address and the return branch offset differ).
3. **Landmark loop block order.** Original's `i==1||i==3` off-screen blip block
   is at `0x99a`, after the on-screen block; ours is at `0x85a`, before it.
   The `i==0 && !alienOrbit` special label block is at `0x9da` in the original
   and `0x996` in ours. This produces the 0x44 shift that moves the whole
   landmark tail (and therefore the second cold block) 0x42 earlier.
4. **Planet tail.** Original `if (!turretMode && !landmarkHit)` tail is inline
   at `0x6f8`; ours is in the cold region at `0xba0`.
5. **vpush width.** Original `vpush {d8-d13}` (crosshair floats live across
   `operator-`/`VectorLength` in the enemy in-sight block); ours
   `vpush {d8-d11}` (bounds-first order).
6. **Stack-slot rotation.** Original `mode` sp#100, `marked` sp#132,
   `textOffset` sp#128, `stations` sp#116; ours `mode` sp#112, `textOffset`
   sp#116, `stations` sp#132, `marked` in r10.
7. **Duplicate autopilot block.** Ours calls `getAutoPilotTarget` twice
   (0x688 and 0x6e2); the original has one call (0x6a4). Removing the
   post-planet-loop copy shrinks the body to 9756 B (norm 3431), so it is
   load-bearing for the hard 9908 B size.
8. **Asteroid dead predicate.** Original evaluates
   `(hasCargo && dead) || !dead` (6 `isDead`/`isDying` calls); ours computes
   `dead` once and reuses it (4 calls). Faithful 6-call form is 9928 B / 71.30 %
   (or 9908 B / 73.74 % with a `bool dead` local), both below the current score.

## 3. What was swept this pass (all measured with the official verifier API)

All variants keep `osize 9908` unless noted. Baseline raw = **73.972210 %**.

### 3.1 `otherLock` first chain

| variant | usize | raw % |
|---|---:|---:|
| landed negative-AND + `expect(whole,0)` + inner `expect(b,1)` | 9908 | **73.972210** |
| positive OR, no expect | 9908 | 72.740295 |
| positive OR + `expect(whole,1)` | 9908 | 73.943561 |
| positive OR + `expect(whole,0)` | 9912 | 70.858455 |
| positive OR + `expect(a,1)` / `expect(b,1)` | 9912 | 71.899 / 71.928 |
| positive OR + `expect(c,1)` | 9908 | 73.943561 |
| positive OR + all three `expect(...,1)` | 9924 | 60.790 |
| `int otherLock = (a||b||c) ? 1 : (field_0x14 != 0)` | 9908 | 72.740295 |
| `otherLock = (a||b||c); if (!otherLock) ...` | 9904 | 70.492 |
| `goto` out-of-line cold block, no expect | 9908 | 72.740295 |
| `goto` out-of-line cold block + `expect(whole,0)` | 9908 | 73.972210 (cold still at 0x25aa) |
| `goto` + `expect(whole,1)` | 9904 | 70.725 |

### 3.2 Landmark `onScreen` hint mask

All 32 subsets of the five-conjunct `&&` chain were measured at polarity 1 and
the promising subset at polarity 0. The landed mask (hints on `onScreen` and the
first bounds test) is the unique 9908 B / 73.972210 % shape; every other 9908 B
mask is <= 73.943561, and polarity 0 is <= 63.32.

### 3.3 Other source shapes

| variant | usize | raw % |
|---|---:|---:|
| landed | 9908 | **73.972210** |
| `i==1||i==3` plain (no expect/mask) | 9908 | 73.972210 |
| `i==1||i==3` mask/expect variants | 9908/9912 | 73.972210 / 72.186 |
| `otherLock` block moved before route/planetTargets/cameraZ/inverse/field1a8/draw4 | 9812..10476 | 47.4..70.0 |
| `notOtherLock`/`notTurretMode` locals removed or reordered | 9900..9928 | 70.4..72.1 |
| distance-first in-sight (x-pair, interleaved, float cx/cy, `Vector diff`, direct bool) | 9896..9952 | 62.4..67.5 |
| `float cx/cy` bounds-first | 9908 | 73.972210 |
| `int inSight`, `int cx/cy`, `bool/int` declaration reorders | 9908 | 73.972210 |
| `planetHit` int / `landmarkHit` int / `enemyHit` int / `bossSeen` int | 9896..9924 | 70.0..72.1 |
| `turretMode` int | 9888 | 66.08 |
| `missionBlocksPlanets` int | 9904 | 72.76 |
| `alwaysEnemySeen` bool | 9908 | 71.87 |
| `alienOrbit` int | 9908 | 73.962 |
| `campaign` bool | 8784 | 24.61 |
| music-tail `current != 151 && alwaysEnemySeen` reordered / split | 9908 | 73.811..73.915 |
| `size + size` -> `size * 2` in landmark hit box | 9908 | 73.858 |
| `size * 2` -> `size + size` elsewhere | 9908 | 73.972210 |
| landmark loop `continue` -> nested `if` | 9908 | 73.972210 |
| off-screen blip `else if` -> `else { if }` / `continue` / bool | 9908 | 73.972210 |
| planet-tail `!turretMode && !landmarkHit` reordered/nested | 9908 | 73.972210 |
| remove duplicate autopilot block | 9756 | 62.31 |
| faithful asteroid 6-call predicate | 9928 | 71.30 |
| faithful asteroid + `bool dead` local | 9908 | 73.743 |

## 4. Proven vs hypothesis

**Proven (measured):**
* The source logic matches the original block for block; the residual is ARM
  backend block placement and register/stack colouring, not a missing statement.
* The first `otherLock` cold block is at `0x25aa` in every size-9908 source
  spelling tried; the original's `0xb72` placement is not reachable by source
  hints alone.
* Removing the duplicate autopilot block shrinks the body to 9756 B; the 9908 B
  size is a hard constraint that keeps that duplicate block load-bearing.
* Refreshing the stale `report.json` moves the reported score 73.9 -> 74.0 with
  the raw ratio unchanged at 73.972210.

**Next hypothesis:**
* The 0x44 landmark-loop shift and the 0x25aa cold-block placement are decided by
  `MachineBlockPlacement`'s cold-block ordering. Matching them likely requires the
  original's exact TU/source shape (or a different `-mllvm` block-placement
  configuration), not a local statement spelling. The most promising remaining
  lever is to remove the duplicate autopilot block and find ~124 B of faithful
  code (asteroid predicate + distance-first in-sight + tractor CSE) that keeps
  the body at 9908 B.

## 5. Evidence / scratch

All scratch under `/dev/shm/w5310` (not committed):
`rea-radar-draw.json`, `ghidra-draw.c`, `orig2.dis`, `ours2.dis`,
`measure.py`, `diffregions.py`, `summary.txt`, `regions.txt`, `variants*.py`,
`Radar.base.cpp`.
