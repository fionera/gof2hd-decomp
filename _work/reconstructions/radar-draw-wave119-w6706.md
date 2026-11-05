# Radar::draw pass (wave119, w6706) — source-shape audit vs. the fuzzy optimum

**Status:** research note, 2026-11-05. Worktree `wt/w6706`, branch
`wave119-w6706`, based on current `master`. **No source change landed**; this
commit contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B, 3492 instructions; ours 9908 B, 3489 instructions.

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908
official verify score 74.0  (exact difflib ratio 74.029509 %)
verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

The official `verify.py` score is rounded to one decimal, so a variant must
beat **74.05 %** to show up as an improvement. Every score in this note is
the unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
so ties can be distinguished.

## 1. What is actually different from the original

Call-count audit of the whole body (normalized `blx sym:` counts) shows the
baseline has exactly ten extra calls and is missing three:

| call | original | baseline | delta |
|---|---:|---:|---:|
| `PaintCanvas::DrawString` | 9 | 8 | **-1** |
| `KIPlayer::isDead` | 17 | 16 | **-1** |
| `KIPlayer::isDying` | 13 | 12 | **-1** |
| `Level::getPlayer` | 29 | 32 | +3 |
| `Status::getSystem` | 5 | 7 | +2 |
| `SolarSystem::getStations` | 2 | 3 | +1 |
| `SolarSystem::getStationEnumIndex` | 1 | 2 | +1 |
| `PlayerEgo::goingToPlanet` | 1 | 2 | +1 |
| `PlayerEgo::isDockingToPlanet` | 4 | 5 | +1 |
| `PlayerEgo::getAutoPilotTarget` | 1 | 2 | +1 |

The seven positive deltas are all the **invented recompute** block after the
planet loop (source lines 612-623):

```cpp
if (!alienOrbit) {
    int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
    ...
    onPlanetCourse = goingToPlanet() && !isDockingToPlanet() && ...;
    this->field_0x1a8 = onPlanetCourse;
}
```

The original has **no such block**. Its `onPlanetCourse` is computed once in
the prologue (`0x12e186..0x12e1bc`) and the `beq.w 0x12e6ee` at `0x12e1b6`
jumps to a cold tail that only does the `autoPilotTarget == targets[idx]`
comparison and branches back to the `strb field_0x1a8` at `0x12e1bc`.
The baseline's prologue is already byte-identical to the original through
`0x12e1ba` (same `goingToPlanet`/`isDockingToPlanet` merge, same branch), and
`field_0x1a8` is stored only once in the original (`0x12e1d0`) but twice in
ours (`0x12e1d0` and the recompute's store). Deleting the recompute makes the
call counts match exactly except for the three missing calls below, but the
body shrinks to 9756 B and the score falls to ~62 %: the recompute is
compensating for a global register-allocation difference (see §4).

The three missing calls are real source differences:

1. **Landmark `i <= 1` else-branch `DrawString`** (original `0x12e9e0`).
   The original has *three* DrawString calls in the `i <= 1` block: the
   landmark label (`0x12e95a`), the TecLevel string (`0x12eaae`) and the
   `i == 0 && !alienOrbit` lock label (`0x12eb36`), plus a separate lock-label
   call for the `else` arm at `0x12e9e0`. Our source has the same
   if/else but clang tail-merges the else arm's `blx DrawString` into the
   `i == 0` arm's call (both arms compute `this->lockLabel` and differ only in
   x/y). The merged call is the single worst-matching region of the body:
   instruction window 800-900 matches only **36 %**.
2. **Asteroid dead-check** (original `0x12faa6..0x12fad0`). The original
   computes `isDead`/`isDying` twice:
   ```
   if (hasCargo && (isDead || isDying)) { /* fall through */ }
   else if (isDead || isDying) continue;
   ```
   Our `bool dead = isDead() || isDying(); if (dead && !hasCargo) continue;`
   computes them once. (The later `bool wreck = hasCargo && (isDead ||
   isDying)` is already the same shape in both.)

## 2. Enemy `inSight`: evaluation order and the `vpush` mismatch

Baseline prologue: `vpush {d8-d11}` (4 float regs). Original:
`vpush {d8-d13}` (6). The two extra float registers are `s24`/`s26`, used
only in the enemy loop's `inSight` block (`0x12ef5a..0x12efa4`): the original
converts `crosshairPos.x/y` to `int` **before** the `VectorLength` call and
keeps them live in `s26`/`s24` across it.

The original's condition order is

```
VectorLength(egoPos - targetPos) < 60000.0f
  && screenX > cx - size && screenX < cx + size
  && screenY > cy - size && screenY < cy + size
```

Our source has the four screen comparisons first (in `x>, y>, x<, y<` order)
and the `VectorLength` last. Reordering to the original's exact shape does
reproduce `vpush {d8-d13}` and matches the `inSight` block instruction for
instruction, but it re-colours the whole function and the global score drops
to **67.5 %** (the asteroid section around `0x1de0..0x1f24` falls to 10-18 %
because the stack-slot layout shifts). This is the clearest example of the
fuzzy metric preferring a tuned-but-unfaithful shape.

## 3. Other confirmed-but-unhelpful source differences

* **`marked = marked != 0`** — the original normalises the campaign `marked`
  flag to 0/1 at `0x12e67a..0x12e684` (5 instructions, right before the
  `campaign == 125` test). Our `int marked` never normalises. Writing
  `bool marked` moves the normalisation to the end of the chain
  (`0x12e6e0`) instead and costs 20 B.
* **Stack slot for `mode`** — original `str r3,[sp,#100]`, baseline
  `[sp,#112]`. `hud` (`[sp,#136]`) and the stack canary (`[sp,#292]`) match,
  so the 296-byte frame is right; only the `mode` slot assignment differs.
  The `inSight` reorder moves it to `[sp,#104]` (closer) but does not fix it.
* **`otherLock` register split** — original keeps `otherLock` in `r5` and
  re-materialises `status` into `r6` at `0x12ca`; baseline keeps `status` in
  `r5` and puts `otherLock` in `r9`. The single-OR-expression
  `otherLock = a || b || c || d` reproduces the original assignment but makes
  `status` live in `r6` from the top (70.5 %).
* **`field_0x1a0` / planet-tail store order** — the wave-118 finding stands:
  duplicating the `planetLockTimer = 0` store (`!= 0` hot arm) reproduces the
  original's `[cold autopilot][planet tail][landmark]` block order but loses
  score on its own.

## 4. Variants measured this wave

All are single-edit changes on the pristine baseline unless noted. `usize` is
the emitted function size; `raw %` is the unrounded ratio; `n` is the number
of positionally-equal normalized instructions (3492 original).

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** | 9908 | **74.029509** | 267 |
| `insight` enemy `inSight` in original order (VectorLength first, x/x/y/y) | 9896 | 67.507525 | 251 |
| `insight2` same order but `int size` hoisted | 9908 | 74.029509 | 267 |
| `insight3` `int size` + VectorLength first, x/x/y/y | 9920 | 64.5 | 257 |
| `ins_el` insight + `int elapsed = mode` | 9908 | 74.029509 | 267 |
| `el` `int elapsed = mode` alone | 9908 | 74.029509 | 267 |
| `t2` `int alienOrbit` (instead of `bool`) | 9908 | 74.018906 | **386** |
| `t2i` t2 + insight | 9876 | 67.507525 | 259 |
| `t2m` t2 + `char marked` | 9908 | 74.018906 | 386 |
| `t2a` t2 + `String label` un-merge | 9908 | 72.636103 | — |
| `t2e`/`t2s`/`t2h` t2 + do/while, size hoist, `int enemyHit` | 9908 | 74.018906 | 386 |
| `t2c1` t2 + `uint32_t campaign` | 9908 | 74.018906 | 386 |
| `t2c6` t2 + `int gasHit` | 9908 | 73.990261 | — |
| `t1` `int turretMode` | 9888 | 66.2 | 259 |
| `t3` `int turretMode` + `int alienOrbit` | 9888 | 66.1 | 407 |
| `t4` `int missionBlocksPlanets` | 9904 | 72.8 | 286 |
| `c1` `uint32_t campaign` | 9908 | 74.029509 | 267 |
| `c2` `char alwaysEnemySeen` | 9908 | 74.029509 | 267 |
| `c3` `int bossSeen` | 9908 | 72.169676 | — |
| `c4` `int landmarkHit` | 9908 | 70.525863 | — |
| `c5` `int planetHit` | 9908 | 71.492045 | — |
| `c6` `int gasHit` | 9908 | 74.000859 | 267 |
| `c7` `int asteroidHit` | 9908 | 68.862790 | — |
| `c8` `bool inCenter` | 9908 | 71.696492 | — |
| `l1` landmark loop `uint32_t i` | 9908 | 72.841804 | — |
| `l2`/`l3`/`l4` planet/enemy/asteroid loop `int i` | 9908 | 74.029509 | 267 |
| `m1` explicit `marked = marked != 0` (split else-if) | 9936 | 71.9 | 415 |
| `marked` `bool marked` | 9928 | 71.0 | 373 |
| `always` `bool alwaysEnemySeen` | 9908 | 71.9 | 361 |
| `marked_char`/`marked_char2` `char`/`unsigned char marked` | 9908 | 74.029509 | 267 |
| `enemyhit` `int enemyHit` | 9908 | 74.029509 | 267 |
| `dsA` else-branch `String label` (un-merges the DrawString) | 9896 | 72.6 | 260 |
| `dsB` `i==0` branch `String label` | 9912 | 70.5 | 265 |
| `dsC` swap if/else order | 9912 | 72.3 | 264 |
| `dsD` shared `landmark` local outside if/else | 9860 | 70.6 | 463 |
| `edo` `do { } while (0)` around else branch | 9908 | 74.029509 | 267 |
| `edirect` else branch via float temporaries | 9908 | 73.8 | 268 |
| `x1` `expect(i <= 1, 1)` | 9912 | 66.1 | 252 |
| `x2` `expect(i == 0 && !alienOrbit, 1)` | 9868 | 62.2 | 99 |
| `x3` `expect(i <= 1, 0)` | 9868 | 61.5 | 118 |
| `orchain` `otherLock = a \|\| b \|\| c \|\| d` | 9904 | 70.5 | 138 |
| `orchain_insight` | 9876 | 61.4 | 259 |
| `faithful` recompute deleted + asteroid dead shape + `String label` un-merge | 9772 | 61.9 | 233 |

`n` is only a tie-breaker diagnostic; the two variants with the highest
positional match (`dsD` 463, `t3` 407) both score far below the baseline
because the matching runs are short.

## 5. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 % /
267 positional**. The baseline is a strong local optimum: the invented
recompute is *wrong* by the call graph but it is exactly what keeps the
global register allocation (and therefore the fuzzy score) in place. The
three genuine source differences (missing landmark else-branch `DrawString`,
doubled asteroid `isDead`/`isDying`, `inSight` evaluation order) each move
the code *closer* to the original locally and *further* away globally.

The next pass should treat the `mode` stack slot (`[sp,#112]` vs the
original `[sp,#100]`) and the `status`/`otherLock` register split
(`r5`/`r9` vs `r5`/`r6`) as the primary targets: they are the two global
properties that, if fixed, would let the faithful source shape (no recompute,
original `inSight` order) score at or above the tuned baseline.

## 6. Scratch

All scratch under `/dev/shm/w6706` (not committed): `Radar.base.cpp`
(pristine baseline source), `score.py` / `pscore.py` (official and unrounded
scores), `try.sh`, `vdis.py` (per-100-instruction match map with addresses),
`callcmp.py` (call-count diff), `calladdr.py`, `sbs.py`, `ndump.py`, and the
variant scripts listed above.
