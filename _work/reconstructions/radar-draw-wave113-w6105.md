# Radar::draw pass (wave113, w6105) — block-layout archaeology: the planet tail / landmark-loop rotation

**Status:** research note, 2026-11-05. Worktree `wt/w6105`, branch
`wave113-w6105`. No source change landed; this commit contains this note only.
The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale).

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 raw 74.029509 %
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals). Everything in 73.95..74.04 still rounds to the reported 74.0.
Baseline instruction count: **3447 original / 3444 ours** (non-pool entries).

## 1. New result: the original's planet-block / landmark-loop *rotation*

The original's function-relative layout is

```
0x2b4  first otherLock chain
0x2c4  landmarks/station guard
0x326  second otherLock chain  <--+
0x388  planet block (campaign check, planet loop 0x3d4..0x6f6, planet tail 0x6f8..0x748)
0x758  landmark loop preheader / body 0x7d4..0xaf2
0xbd4  enemies section
```

The landmark loop's exit at `0xaf2` and its turret block at `0xbbe` both do
`b.w 0x326`, i.e. they jump **back** to the second `otherLock` chain / planet
block. The planet tail's last instruction is `b.n 0xbd4` (enemies) — it does
*not* fall through, because the landmark loop is laid out between the planet
tail and the enemies.

So the source shape is the one we have (`if (guard) { landmark loop }
otherLock = ...; if (campaign>=2 ...) { planet block }`), but
`MachineBlockPlacement` has rotated the layout so that the common code
(second chain + planet block) is emitted first and the conditional landmark
loop second.

**The baseline reproduces this layout only by accident.** Our source still
contains the semantically-wrong post-planet-loop `onPlanetCourse` recompute
(152 B, 10 extra calls). That block occupies almost exactly the space where
the original's planet tail sits, so the landmark loop lands at `0x730`
(8 B before the original's `0x758`) and the whole function aligns at 74 %.

Removing the recompute is semantically correct and restores the original's
call multiset exactly (354 vs 354; see §2), but it flips MBSP's chain order:
the landmark loop sinks to `0x13fe`, **after** the enemies section
(`0x754..0x13fc`), and the score collapses to 62.31 %.

Measured (all on the pristine baseline unless noted):

| variant | usize | raw % | landmark loop @ | enemies @ |
|---|---:|---:|---|---|
| baseline (recompute kept) | 9908 | **74.029509** | 0x730 | 0xab0 |
| `nodup` (recompute deleted) | 9756 | 62.314026 | 0x13fe | 0x754 |
| original | 9908 | — | 0x758 | 0xbd4 |

## 2. Call multiset (re-measured this wave)

```
                       orig ours(baseline)
DrawString                9    8
isDead                   17   16
isDying                  13   12
getStations               2    3
getStationEnumIndex       1    2
getPlayer                29   32
getSystem                 5    7
goingToPlanet             1    2
isDockingToPlanet         4    5
getAutoPilotTarget        1    2
total blx               354  361
```

The 10-call surplus is exactly the recompute block. `nodup` gives
354 vs 354 for every symbol **except** `DrawString` (9 vs 8), `isDead`
(17 vs 16) and `isDying` (13 vs 12) — i.e. the remaining three are the
un-merged landmark else-branch `DrawString` and the duplicated asteroid
`dead` predicate (§5), not missing statements.

## 3. The original's planet loop has two separate `continue` blocks

Original `0x692..0x69e`:

```
692 b.n   0x45a            ; end-of-body -> loop increment
694 ldr.w sl,[sp,#116]
698 b.n   0x45c            ; continue #1 (data[i] == nullptr)
69a mov   sl,r6
69c b.n   0x45c            ; continue #2 (!onScreen)
69e ldr   r0,[r4,#0]
```

Ours has a single merged continue block (`0x680 b.n 0x44e`). The original
keeps two, because the register restore differs (`sl` from `[sp,#116]` vs
from `r6`). This is a BranchFolder/tail-merge artefact, not a missing
statement: the source already has both `continue`s
(`planetTargets->data()[i] == nullptr` and `!this->onScreen`).

The original's planet loop is therefore ~24 B larger than ours
(0x3d4..0x6f6 = 804 B vs our 0x384..0x6da = 856 B including the extra
preheader work).

## 4. The original's enemy in-sight block is distance-first

Original `0xf08..0xf70` (function-relative `0xeb8`..`0xf20`) loads
`PlayerEgo::crosshairPos.x/.y` into `s26`/`s24` **before** the
`AEMath::operator-` call, converts them to int **after** it, and only then
tests the 60000-unit distance followed by the four screen bounds. That is
why the original saves `d8-d13` (6 doubles) while the baseline saves
`d8-d11` (4).

The faithful shape is reachable from source with the *existing* `int cx/cy`
locals by moving the distance test first:

```cpp
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
    this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
    inSight = true;
}
```

It reproduces the original's `vldr s26/s24` / `vcvt` split and the
`bpl` + branchy-bounds body instruction-for-instruction, but the body is
12 B *shorter* and every later block shifts:

| in-sight shape | usize | raw % |
|---|---:|---:|
| baseline (bounds-first, `int cx/cy`) | 9908 | **74.029509** |
| distance-first `&&` (`int cx/cy`) | 9896 | 67.517202 |
| distance-first nested `if` | 9936 | 62.415625 |
| distance-first, screenX/screenY interleaved | 9948 | 63.375431 |
| `float cx/cy` distance-first `&&` (wave110 G1) | 9928 | 64.474625 |

## 5. The original's asteroid `dead` predicate is duplicated (6 calls)

Original `0x1a94..0x1ad4`:

```
1aa6 ldrb  r0,[r6,#76]     ; hasCargo
1aaa cbz   r0,0x1abc
1aac isDead
1ab2 cbnz  r0,0x1ad4       ; hasCargo && dead -> update
1ab4 isDying
1aba cbnz  r0,0x1ad4
1abc isDead
1ac2 cmp   r0,#0
1ac4 bne   0x1cd2          ; dead && !hasCargo -> continue
1ac8 isDying
1ace cmp   r0,#0
1ad0 bne   0x1cd2
1ad4 update(asteroid)
```

i.e. the `dead` predicate is evaluated twice, once under `hasCargo` and once
under `!hasCargo`. Our source evaluates it once
(`bool dead = isDead() || isDying(); if (dead && !hasCargo) continue;`).
The 4-call form is reachable from source:

```cpp
if (asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying())) {
} else if (asteroid->isDead() || asteroid->isDying()) {
    continue;
}
```

| asteroid `dead` spelling | usize | raw % | isDead/isDying |
|---|---:|---:|---|
| baseline (`bool dead`, one eval) | 9908 | **74.029509** | 16 / 12 |
| `hasCargo && dead {} else if (dead) continue` (A) | 9928 | 71.465369 | **17 / 13** |
| A with nested `if` (B) | 9924 | 71.283445 | 17 / 13 |
| `bool dead; if (hasCargo && dead) {} else if (dead) continue` (C) | 9908 | 73.847035 | 16 / 12 |
| `!hasCargo && dead` (D) | 9912 | 72.146642 | 16 / 12 |
| `if (dead) { if (!hasCargo) continue; }` | 9908 | 74.029509 | 16 / 12 |
| `if (!hasCargo && dead) continue;` | 9908 | 73.847035 | 16 / 12 |

Only variant A reproduces the original's 6-call predicate, and it costs
20 B / 2.6 points because the block grows and everything downstream shifts.

## 6. The original's asteroid blip offset block reloads `screenX`

Original `0x1c0a..0x1c5a` reloads `this->screenX` (`ldr r2,[r4,#252]`) in
*each* branch of the `field_0x11d/11e/11f/120` offset chain (4 loads); ours
CSEs it into one register. The branch structure is otherwise identical, so
this is a register-allocation artefact (the original keeps `r2` busy with
the flag bytes).

## 7. Full sweep of this wave (all dead ends)

First `otherLock` chain (positive-OR / `cbnz` shapes):

| spelling | usize | raw % |
|---|---:|---:|
| baseline nested-expect negative-AND | 9908 | **74.029509** |
| `expect(a||b||c,1)` (`pos_or_exp1`) | 9908 | 74.000859 |
| plain negative-AND (cold blocks adjacent at 0xb3c/0xb4e) | 9908 | 72.797593 |
| `expect(a!=0,0)||expect(b!=0,0)||c!=0` (`p00x`) | 9900 | 72.836676 |
| `p00x` + whole `expect(...,1)` | 9916 | 71.888873 |
| `p00x` + whole `expect(...,0)` | 9912 | 70.915877 |
| `p11x` / `p11x1` | 9916 / 9924 | 60.48 / 60.79 |

Landmark else-if (`(i & 0x7fffffff) == 1 || == 3`):

| spelling | usize | raw % | shape |
|---|---:|---:|---|
| baseline (`||` + `expect(...,0)`) | 9908 | **74.029509** | `orr sl,#2` |
| no mask (`i == 1 || i == 3`) | 9908 | 74.029509 | identical |
| whole hint `1` | 9908 | 74.029509 | identical |
| per-term hints + whole `0` | 9908 | 74.029509 | identical |
| two separate plain `else if` | 9916 | 72.318488 | **`bic`**, `cmp #3` first |
| two separate hinted `else if` | 9916 | 72.472071 | **`bic`**, `cmp #1` first, tail far |
| two separate reversed `else if` | 9916 | 71.144207 | `bic` |
| `switch` | 9916 | 72.318488 | `bic` |
| per-term hints only | 9912 | 72.242910 | |
| `mask_alt` (`& ~0x80000000`) | 9908 | 74.029509 | identical |

Hit-condition (`!landmarkHit && i != 3 && notTurretMode && notOtherLock`):

| spelling | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `!landmarkHit && notOtherLock && i != 3 && notTurretMode` | 9908 | 73.982808 |
| `notOtherLock && !landmarkHit && ...` | 9908 | 73.857614 |
| `i != 3 && notTurretMode && !landmarkHit && notOtherLock` | 9912 | 72.081364 |
| `(!landmarkHit & notOtherLock) && i != 3 && notTurretMode` | 9896 | 71.670494 |
| all-bitwise `&` form | 9908 | 70.890067 |
| `!(landmarkHit || !notOtherLock || i == 3 || !notTurretMode)` | 9904 | 72.226999 |
| `char` / `uint8_t` landmarkHit | 9908 | 70.419711 |
| `volatile bool` landmarkHit | 9808 | 57.758621 |
| `int landmarkHit = 0` | 9904 | 70.525863 |

`nodup` (recompute deleted) + layout nudges:

| variant | usize | raw % |
|---|---:|---:|
| `nodup` | 9756 | 62.314026 |
| + asteroid 6-call predicate (A) | 9776 | 61.924686 |
| + `expect(landmarks != nullptr, 1)` | 9752 | 64.818024 |
| + `expect(landmarks != nullptr, 0)` | 9740 | 54.786621 |
| + `expect(campaign >= 2, 1)` | 9852 | 60.506621 |
| + `expect(scannerAvailable, 1)` | 9668 | 52.755220 |
| + landmark loop-cond `expect(...,1)` | 9956 | 46.240115 |
| + enemies loop-cond `expect(...,0)` | 10112 | 31.420455 |
| + enemies loop-cond `expect(...,1)` | 9732 | 47.737100 |
| + enemies `if` `expect(...,0)` | 9996 | 43.844185 |
| + planet-dock `expect(...,1)` | 9792 | 59.673929 |

Recompute-block placements (recompute kept):

| placement | usize | raw % |
|---|---:|---:|
| baseline (after planet loop, before tail) | 9908 | **74.029509** |
| after the planet tail | 9948 | 70.293486 |
| before the planet loop | 9868 | 63.977028 |
| before the second chain | 9868 | 61.052026 |
| before the enemies section | 9936 | 62.067976 |
| after the enemies section | 9936 | 62.067976 |
| before the asteroids section | 9864 | 48.564044 |
| planet block moved before the landmark loop (source reorder) | 9788 | 56.036973 |

Neutral (byte-identical at 9908 / 74.029509 %, left in the source as-is):
`i == 2` -> `2 == i` / `2u`, `i != 3` -> `3 != i`, `i <= 1` -> `i < 2`,
`i == 0` -> `0 == i`, `i == 1` -> `1 == i`, `!turretMode && !landmarkHit`
swap, `notOtherLock = otherLock == 0`, `notTurretMode = turretMode == 0`,
`otherLock ^ 1` / `1 - otherLock`, `landmarkHit = 0`, `(int) landmarks->size()`,
`i == 1 || i == 3` (no mask), else-if hint `1`, `screenY + 2 * field_0x4`,
`enemy wreck` hoist, `if (dead) { if (!hasCargo) continue; }`.

Also measured (worse): `int offset` removed (9924 / 73.17), `landmark`
local hoisted into both branches (9860 / 70.62), `String dist` local
(9896 / 72.70), `landmark` pointer local (identical), `nLandmarks` local
(9912 / 70.84), `enemy wreck` reorder (10252 / 38.85).

Literal pools: **45 words on both sides, identical content and order**
(verified with `lib.load()`; every `pool:pic:got:*`, `pool:pic:str:*`,
`pool:0x...` and `pool:addr` entry matches index-for-index). The fuzzy
gap is therefore entirely instruction order / register colouring, not pool
placement.

## 8. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 %**.
The remaining gap is a single, self-consistent compiler artefact: the
original has the planet block laid out *before* the landmark loop (with the
landmark loop's back-edge jumping to `0x326`), while our semantically
faithful `nodup` source lets MBSP sink the landmark loop after the enemies.
The baseline's extra (semantically wrong) recompute block happens to occupy
the original's planet-tail slot and therefore keeps the layout aligned.

A byte-faithful reconstruction would need to (a) delete the recompute,
(b) keep the landmark loop hot at `0x758`, (c) restore the two separate
planet-loop `continue` blocks, (d) restore the duplicated asteroid `dead`
predicate, (e) un-merge the landmark else-branch `DrawString`, and
(f) keep the crosshair floats live across `AEMath::operator-`. Each of
(a)–(f) is individually reachable from source (measurements above), but
every one of them moves `MachineBlockPlacement` enough to lose more matched
instructions than it gains. The baseline is a tuned local optimum of the
fuzzy metric.

The worktree was restored to the pristine `Radar.cpp` before the final gate.
`check` is green: byte 4500, linked 4500, lint CLEAN, drift 0, data 843/843,
imports 207, `RATCHET PASS`.

## 9. Scratch

All scratch under `/dev/shm/w6105` (not committed): `Radar.base.cpp`
(pristine baseline source), `measure.sh`, `score.py`, `lib.py`, `show2.py`,
`trym.py`, `v1.py`..`v24.py` (the variant harnesses used for every table
above), `rea.json`/`rea.c` (Ghidra), `orig.dis`/`ours.dis`,
`orig_radar.dis`.
