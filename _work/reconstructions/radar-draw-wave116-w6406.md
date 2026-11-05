# Radar::draw pass (wave116, w6406) — block-placement / stack-slot sweep

**Status:** research note, 2026-11-05. Worktree `wt/w6406`, branch
`wave116-w6406`, based on current `master`. **No source change landed**; this
commit contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale). Original 9908 B.

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 raw 74.029509 %
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals). Baseline instruction count: **3492 original / 3489 ours**;
function hash `b34ff2168f3919c6`.

Structural state re-measured this wave and unchanged: **27 sections** on both
sides, exact file size **2,187,328 B**, `.text` **0x1337c8** on both sides.
`.ARM.extab` is **0x26294 (ours) vs 0x26298 (original)** — the known 4 B
shortfall. `.rodata` is 0x41b67 (orig) vs 0x41c27 (ours); the extra 0xC0 B is
the tail of the checked-in mission-case table, and every original `.rodata`
byte before `0x20b977` is exact.

## 1. Baseline region match map (equal-insn fraction, 250-insn buckets)

```
   0- 250: 225/250     250- 500: 178/250     500- 750: 155/250
 750-1000: 144/250    1000-1250: 167/250    1250-1500: 148/250
1500-1750: 185/250    1750-2000: 176/250    2000-2250: 210/250
2250-2500: 202/250    2500-2750: 179/250    2750-3000: 200/250
3000-3250: 221/250    3250-3492: 194/242    total 2584 matched
```

The gap is spread almost uniformly; no single hot spot dominates. The largest
single hunks are the landmark generic-else `DrawString` tail merge
(orig[864:896] 32 vs 7), the enemy in-sight block (orig[1383:1411] 28 vs 8),
the post-loop recompute (orig[1077] 0 vs 33), the planet tail missing at its
expected slot (orig[637:654] 17 vs 0) and the first `otherLock` chain
(orig[235:258] 23 vs 4).

## 2. New structural evidence this wave

### 2a. The original's planet-tail / landmark-loop order

Original block order (function-relative):

```
2ac..374  first otherLock chain + landmark station guard
376..6ec  planet block guard + planet-target loop
6ee..746  cold autopilot block (getAutoPilotTarget/getPlanetTargets/
          getSystem/getStations/getStationEnumIndex)
748..798  planet tail  (!turretMode && !landmarkHit)
79c..7d2  landmark setup
7d4..b40  landmark loop
b42..bc0  landmark post-loop (lockedStation / stationLockTimer)
bc2..c22  cold blocks (otherLock cold, second otherLock cold,
          missionBlocksPlanets, planetLockTimer==0)
c24..     enemies
```

Ours (baseline) block order:

```
2ac..338  first otherLock chain + landmark guard
33a..6d2  planet block guard + planet-target loop
6d2..72a  cold autopilot block
72c..780  post-loop onPlanetCourse recompute (calls)
780..7ae  landmark setup
7b0..b10  landmark loop
b10..b8a  landmark post-loop
b8c..ba8  cold blocks
bd4..c08  recompute tail
bec..c36  planet tail
c36..     enemies
```

So the baseline's extra recompute occupies the original's planet-tail slot
and keeps the landmark loop at roughly the right address, while the planet
tail is displaced to after the landmark block. The recompute is
**load-bearing for the fuzzy score** even though it is not in the original.

### 2b. Removing the recompute (V_nodup) puts the planet tail right but sinks the loop

`V_nodup` (recompute deleted) gives the **exact original call multiset** and
places the planet tail at `0x708` (original `0x6f8`), but clang sinks the
landmark loop body to `0x1474` (after the enemies section) — a 411-instruction
alignment loss. Score **62.314026 % / 9756 B**.

A 2^5 sweep of the five on-screen `__builtin_expect` terms on top of
`V_nodup` never recovers the loop: the loop lands at `0x6c4`–`0x6cc`
(hint on `onScreen` only / no hints) or `0x1474` (all other combinations).
No `V_nodup` variant beats the baseline.

### 2c. Moving the recompute after the planet tail (Y_after)

`Y_after` (recompute moved to the end of the planet block, after the tail)
places the planet tail at `0x6fa` (original `0x6f8`) and the landmark loop at
`0x7d4` — i.e. the recompute then sits between the tail and the loop and
pushes the loop 0x50 B late. Score **70.293486 % / 9948 B**.

Shrinking the recompute:

| recompute form | usize | loop addr | raw % |
|---|---:|---:|---:|
| full 10-call recompute (Y_after) | 9948 | 0x7d4 | 70.293486 |
| `goingToPlanet && !isDockingToPlanet` (4 calls, m1) | 9880 | 0x7f0 | 69.472777 |
| `goingToPlanet` only (2 calls, m4) | 9868 | 0x1498 | 63.168488 |
| `!isDockingToPlanet && goingToPlanet` (m5) | 9876 | 0x14a0 | 63.198735 |
| `field_0x1a8 = onPlanetCourse` only (m2/m3) | 9764 | 0x147c | 61.729464 |

The 4-call form is the smallest anchor that keeps the loop out of the cold
tail; it still leaves the loop 0x1c B late and loses 4.5 points overall
because the changed register pressure re-colours the whole planet loop.

### 2d. Stack-slot swap: `planetHit` vs `turretMode`

The only systematic spill-slot difference in the whole function is a **swap
of two stack slots**:

| variable | original slot | ours slot |
|---|---:|---:|
| `planetHit` | `[sp,#152]` | `[sp,#148]` |
| `turretMode` | `[sp,#148]` | `[sp,#152]` |
| `landmarkHit` | `[sp,#108]` | `[sp,#108]` |
| `otherLock` | `[sp,#144]` | `[sp,#144]` |
| `status` | `[sp,#156]` | `[sp,#156]` |
| `mode` param | `[sp,#100]` | `[sp,#112]` |

17 mismatched instruction pairs differ only in this swap. Moving the
`planetHit` declaration to the top of the function is a no-op (same hash);
moving `turretMode` down (or making either `int`/`uint8_t`) loses 2–8 points.

### 2e. Enemy in-sight block: hit-box term order is genuinely different

Original (12ef6c..12efc0):

```
dist = VectorLength(egoPos - targetPos)          ; crosshair.x/y live in s26/s24
if (dist >= 60000) r6 = 0
else r6 = screenX > (int)cx - size && screenX < (int)cx + size
        && screenY > (int)cy - size && screenY < (int)cy + size
enemy->field_0x73 = r6
```

Ours tests the hit box first in the order
`screenX > cx-size && screenY > cy-size && screenX < cx+size && screenY < cy+size`
and the distance last, and converts the crosshair to int before the hit box.
The original's order is `X-lower, X-upper, Y-lower, Y-upper` and the distance
is first. Reordering the terms is a real source difference (the landmark
hit-box order already matches exactly: `X-lower, Y-upper, Y-lower, X-upper`).

### 2f. `missionBlocksPlanets` already matches

The handoff's item 11 ("3-state int") is already produced by the baseline
`bool` source: both sides emit `subs.w fp, r0, #172; it ne; movne.w fp, #1`
at the cold block. No change needed.

## 3. Variants measured this wave (all dead ends)

All are single-edit changes on the pristine baseline unless noted. `usize` is
the emitted function size; `raw %` is the official fuzzy score.

### 3a. First `otherLock` chain

| variant | usize | raw % |
|---|---:|---:|
| baseline (nested negative AND, outer expect 0, inner expect 1) | 9908 | **74.029509** |
| `int otherLock = 1; if (expect(AND,0)) otherLock = d != 0;` | 9908 | 74.029509 (identical hash) |
| `int otherLock = 1; if (expect(OR,1)) otherLock = 1; else ...` | 9908 | 74.000859 |
| plain positive OR, no expects | 9908 | 72.797593 |
| `int otherLock = 1; if (a==0&&b==0&&c==0) ...` | 9908 | 72.797593 |
| `char` / `unsigned` otherLock (positive OR) | 9908 | 72.797593 |
| `bool otherLock` (positive OR) | 9880 | 67.824340 |
| `int otherLock = d != 0; if (OR) otherLock = 1;` | 9928 | 69.754768 |
| `int otherLock = 1;` + per-term expects on a/b/c | 9912 | 70.915877 |
| `int otherLock = 1; if (expect(OR,1)) ...` | 9908 | 74.000859 |

### 3b. Landmark guard / `status` colouring

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `if (landmarks) { Status *status = Globals::status; if (!(...)) ... }` | 9864 | 69.838086 |
| same but direct `Globals::status` (extra nesting only) | 9908 | 74.029509 (identical) |
| `Status *status = Globals::status;` before the guard | 9912 | 72.032078 |

### 3c. Landmark generic-else `DrawString` (tail merge)

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `offset + screenX`, `field_0x4 + screenY` (commuted) | 9908 | 74.029509 (identical) |
| local `int labelY` / `int labelX,labelY` | 9908 | 74.029509 (identical) |
| `if (i != 0 \|\| alienOrbit) {generic} else {i0}` (prior wave) | 9912 | 72.289847 |

### 3d. Enemy in-sight block

| variant | usize | raw % |
|---|---:|---:|
| baseline (hit box first, baseline order, int cx/cy) | 9908 | **74.029509** |
| hit box first, order X,X,Y,Y, int cx/cy | 9936 | 70.844063 |
| hit box first, order X,X,Y,Y, float cx/cy + `(int)` | 9928 | 72.784538 |
| hit box first, order X,X,Y,Y, float cx/cy + int locals | 9936 | 70.844063 |
| hit box first, baseline order, float cx/cy + int locals | 9908 | 74.029509 (identical) |
| distance first, order X,X,Y,Y, int cx/cy | 9896 | 67.517202 |
| distance first, order X,X,Y,Y, inline `(int)crosshairPos.x` | 9896 | 66.159805 |
| distance first, order X,X,Y,Y, float locals | 9904 | 66.934328 |
| distance first, order X,X,Y,Y, float locals + `(int)cx - size` | 9896 | 67.459862 |
| distance first into a local `float dist` | 9928 | 64.503217 |
| `enemy->field_0x73 = dist<60000 && hitbox` (single expr) | 9928 | 64.874911 |
| `bool inSight = dist<60000 && hitbox` | 9928 | 64.874911 |
| hit box first, `inSight = VectorLength(...) < 60000` in the body | 9936 | 70.815451 |

### 3e. Asteroid `dead` predicate

| variant | usize | raw % |
|---|---:|---:|
| baseline (`bool dead` once) | 9908 | **74.029509** |
| direct `(isDead() \|\| isDying()) && !hasCargo` | 9908 | 74.029509 (identical) |
| `dead && !hasCargo && (isDead() \|\| isDying())` (4 calls) | 9928 | 71.808815 |
| Ghidra `!(hasCargo && dead \|\| (!dead && !dying))` | 9928 | 71.465369 |

### 3f. Types / declarations / misc

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `int landmarkHit` | 9904 | 70.525863 |
| `int planetHit` | 9896 | 71.492045 |
| `int enemyHit` | 9924 | 70.072993 |
| `int bossSeen` | 9904 | 72.169676 |
| `int turretMode` / `uint8_t turretMode` | 9888 | 66.169511 |
| `int alienOrbit` | 9908 | 74.018906 |
| `planetHit` declaration moved to the top | 9908 | 74.029509 (identical) |
| `turretMode` declaration moved down | 9904 | 71.270956 |
| `if (i != 2 && data != 0 && isVisible())` (inverted skip) | 10144 | 37.630465 |
| `if (!landmarkHit && !turretMode)` (guard swap) | 9908 | 74.029509 (identical) |
| `if (!planetHit && !otherLock && !turretMode)` | 9908 | 74.029509 (identical) |
| `if (!turretMode && !otherLock && !planetHit)` | 9852 | 57.795660 |
| `if (!otherLock && !turretMode && !planetHit)` | 9852 | 57.795660 |
| `if (turretMode == 0)` for the landmark post-loop | 9908 | 74.029509 (identical) |
| landmark post-loop guard with `landmarkHit` last | 9916 | 71.232092 |

### 3g. Extra `__builtin_expect` hints (beyond wave112's sweep)

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| planet-block guard expect 1 | 9880 | 60.258250 |
| enemy skip `(!active \|\| dying&&!cargo \|\| field_0x74)` expect 0 | 9916 | 56.275072 |
| landmark skip `(i==2 \|\| null \|\| !visible)` expect 0 | 10148 | 38.471302 |
| landmark guard expect 1 | 9912 | 63.117218 |
| planet `data[i] == nullptr` expect 0 | 9876 | 71.153846 |

## 4. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 % /
3492/3489**. This wave confirmed the mechanism behind the residual gap:

1. **The post-loop `onPlanetCourse` recompute is a tuned block-placement
   anchor.** It is semantically redundant (the original has no such block),
   but deleting it lets MachineBlockPlacement sink the landmark loop past the
   enemies section (62.31 %), and moving it after the planet tail pushes the
   landmark loop 0x50 B late (70.29 %). Its current position happens to
   reproduce the original's planet-tail slot size almost exactly.
2. **The only systematic register/stack difference is the
   `planetHit`/`turretMode` stack-slot swap** (17 instruction pairs). Every
   source spelling tried for it either changes nothing (declaration order) or
   loses 2–8 points (type changes).
3. **The enemy in-sight block genuinely differs in source shape** (distance
   first, hit-box order X,X,Y,Y, float crosshair kept across
   `AEMath::operator-`). Every faithful spelling reproduces the original's
   `vpush {d8-d13}` and block, but the global re-colouring costs more than it
   gains (best 72.78 %).
4. The landmark generic-else `DrawString` tail merge cannot be broken by
   coordinate commutation or local temporaries; the two calls only separate
   when a real `String` copy is introduced (which adds ctor/dtor traffic).

The baseline is a strong (tuned) local optimum of the fuzzy metric. A
byte-faithful reconstruction would need to satisfy all of: (a) delete the
recompute, (b) keep the landmark loop hot at `0x7d4`, (c) restore the
duplicated asteroid `dead` predicate, (d) un-merge the landmark else-branch
`DrawString`, (e) keep the crosshair floats live across `AEMath::operator-`,
and (f) swap the `planetHit`/`turretMode` slots — each individually reachable
from source, but each moves MachineBlockPlacement enough to lose more matched
instructions than it gains.

The worktree was restored to the pristine `Radar.cpp` before the final gate.
`check` is green: byte 4501, linked 4501, lint CLEAN, drift 0, data 843/843,
imports 207, `RATCHET PASS`.

## 5. Scratch

All scratch under `/dev/shm/w6406` (not committed): `Radar.base.cpp` (pristine
baseline source), `measure.py`, `try.sh`, `tryaddr.sh` (prints the landmark
loop address + score), `fnhash.py`, `diff.py`, `diff.txt`, `dump.py`,
`dump2.py`, `dump3.py`, `region.py` (250-insn match map), `regmap.py`,
`pos.py`, `blocks.py`, `rea.json`/`rea.c` (Ghidra), `orig_init.txt`,
variant generators `gen_ol.py`, `gen_ol2.py`, `gen_os.py`, `gen_lm.py`,
`gen_guard.py`, `gen_ei.py`..`gen_ei4.py`, `gen_ast.py`, `gen_misc.py`,
`gen_slot.py`, `gen_ord.py`, `gen_hint.py`, and every `V_*.cpp`, `Z_*.cpp`,
`X_*.cpp`, `Y_*.cpp` variant listed above.
