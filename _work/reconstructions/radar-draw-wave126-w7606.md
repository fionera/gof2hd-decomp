# Radar::draw pass (wave 126, w7606) — duplicate-recompute accounting, no source win

**Status:** research note, 2026-11-07. Worktree `wt/w7606`, based on current
`master` (`bf365a964`, wave-125 final). **No source change landed**; this
commit contains this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.268817 %** (official display 75.3). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, data 843/843, imports 207,
lint CLEAN, drift 0.

Read with `radar-draw-wave123-w7307.md`, `radar-draw-wave124-w7413.md` and
`radar-draw-wave125-w7511.md`. This wave re-derived the *whole-function*
call/instruction accounting rather than re-searching the same local
spellings, and confirms the baseline is a strong local optimum.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7606
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8       # RATCHET PASS
```

Per-variant loop (≈0.9 s each): edit `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j8`, score the single symbol with a
scratch harness mirroring `tools/verify/verify.py`'s
`disassemble`/`reconcile` (reproduces 75.268817 % exactly) and read the
first `ldr [pc,#…]` island address from the linked `.so`.

## 2. Baseline block map (re-measured)

| block | original | ours |
|---|---:|---:|
| prologue / onPlanetCourse | `12e050`–`12e1b6` | `12e050`–`12e1b6` |
| 4 images + route | `12e1bc`–`12e2fc` | `12e1bc`–`12e2fc` |
| first `otherLock` + landmark guard | `12e2fc`–`12e376` | `12e2fc`–`12e376` |
| second `otherLock` + planet-loop setup | `12e376`–`12e3d4` | `12e376`–`12e3d4` |
| **planet loop body** | `12e3d4`–`12e6ee` | `12e3d4`–`12e6d2` |
| prologue-else recompute (cold) | `12e6ee`–`12e746` | `12e6d2`–`12e72a` |
| **duplicate after-loop recompute** | **absent** | `12e72c`–`12e774` |
| planet-lock tail | `12e748`–`12e79a` | `12e776`–`12e7c2` |
| first literal pool | `12e340` | `12e310` |
| landmark loop | `12e7a8`–`12eb42` | `12e7c4`–`12eb48` |
| landmark tail | `12eb42`–`12ebc2` | `12eb48`–`12ebd0` |
| cold `otherLock` | `12ebc2` | `1305fa` |
| cold second `otherLock` | `12ebce` | `12ebd4` |
| enemies | `12ec24` | `12ec28` |

The planet loop body is **296 instructions / 792 B** in the original and
**288 instructions / 764 B** in ours (28 B short). The loop is **top-tested**
in the original (`12e7d4` condition, `12ea18` increment, `b.n 12e7d4`) and
**rotated bottom-tested** in ours (`12eb44` increment, `12eb48` condition,
`bcc.w 12e800`).

## 3. The duplicate after-loop recompute is a size-padding hack

The original has exactly **one** `getAutoPilotTarget` (`12e6f4`) and **one**
`getStationEnumIndex` (`12e72a`), both in the cold prologue-else block
reached from `12e1b6`. Ours has **two** of each. Whole-function call-count
diff (original vs ours):

```
+2  Status::getSystem
+2  SolarSystem::getStations
+1  SolarSystem::getStationEnumIndex
+1  PlayerEgo::getAutoPilotTarget
+1  Level::getPlayer
-1  AbyssEngine::String::operator=
```

All other 100+ call sites match exactly (including isDead 17 / isDying 13).
The extra calls are precisely the invented `if (!alienOrbit) { … }`
recompute after the planet loop (`Radar.cpp` lines ~614–623). The original
source does **not** have it: the loop-exit branch at `12e3dc` goes straight
to the planet-lock tail at `12e748`, and the prologue-else recompute is the
only one.

Removing the duplicate (`rec_none`) gives **9856 B / 70.762590 %**, pool
`0x12e318` — i.e. the duplicate contributes 52 B and is load-bearing only
for size, exactly as wave-124 found.

## 4. Instruction-count deficit is elsewhere

Without the duplicate, ours is 3465 instructions vs the original's 3492
(27 short); the loop body accounts for 8 of those. The remaining ~19
instructions are missing real logic, not just encoding. Candidate
observations from the source/ASM diff:

* **Planet loop body register roll**: the original keeps the stations array
  in `sl` and the campaign mission in `r6`; ours keeps stations in `r6` and
  copies to `sl`. This costs/gains `mov` and `ldr`/`ldr.w` encodings at
  `12e3f8`, `12e474`, `12e4be`, `12e4d8`, `12e56e`, `12e6b8`, `12e6e4`,
  `12e6ea` (≈8 instructions).
* **`marked` normalization**: the original emits
  `ldr r1,[sp,#132]; cmp r1,#0; it ne; movne r1,#1; str r1,[sp,#132]` at
  `12e67a` before the `==125` test. Ours does not normalize `marked`.
  Changing `int marked` to `bool marked` grows ours to 9916 B but drops the
  score to 72.664658 % (whole-body re-roll).
* **`navWreck`**: original computes it inline at `12f3de` (dockNavPtr →
  hasCargo → isDead/isDying); ours outlines it to `12f8ae` and splits the
  `isDockingToAsteroid` / `isDockingToDockingPoint` condition across cold
  blocks `12f888`/`12f8a6`.

## 5. Variants measured this wave (all single edits on the pristine baseline)

| variant | size | raw % | pool |
|---|---:|---:|---:|
| **baseline** | 9908 | **75.268817** | `12e310` |
| `rec_none` (drop duplicate recompute) | 9856 | 70.762590 | `12e318` |
| first `otherLock` no inner `expect` | 9908 | 75.240143 | `12e310` |
| first `otherLock` plain (no expects) | 9908 | 74.093190 | `12e310` |
| first `otherLock` outer `expect(…,1)` | 9872 | 73.393704 | `12e2e8` |
| move `notOtherLock`/`notTurretMode` inside the landmark `if` | 9900 | 73.465290 | `12e310` |
| `otherLock`/`notOtherLock`/`notTurretMode` as `bool` (8 combos) | 9800–9920 | 69.08–73.45 | varies |
| distance-first enemy `inSight` (4 spellings) | 9888–9936 | 62.3–68.5 | varies |
| `bool marked` | 9916 | 72.664658 | `12e310` |
| `int` for `enemyHit`/`wreck`/`bossSeen`/`alwaysEnemy`/`planetHit`/`landmarkHit`/`gasHit`/`asteroidHit`/`candidateWreck`/asteroid `dead` | 9884–9924 | 70.24–75.24 | `12e310` |
| `navWreck` declared after the `lockedEnemy` check | 9908 | 74.695341 | `12e310` |
| `navWreck` split into two statements | 9908 | 75.268817 (tie) | `12e310` |
| `__builtin_expect` on the enemy-loop `isDockingToAsteroid` guard (4 spellings) | 9872–9892 | 62.7–69.4 | `12e310` |

Every non-tie variant is a strict regression. `navWreck` split and
`isTarget`/`textOffset` type flips are codegen-identical to the baseline.

## 6. Remaining root cause

The function is a tuned local optimum. The residual is dominated by coupled
compiler decisions that source-level reshaping could not steer independently:

1. **First island** (`12e310` vs `12e340`) — pure block-layout fixed point,
   as wave-125 established.
2. **Duplicate recompute** — the original has one, ours has two; removing it
   exposes a ~19-instruction deficit elsewhere and re-rolls the layout.
3. **Planet loop body** — 8 instructions shorter, caused by the
   `sl`↔`r6` stations/campaign register swap and the missing `marked`
   normalization.
4. **Loop rotation** — original top-tested, ours bottom-tested.
5. **`navWreck`/enemy-tail outlining** — original inline, ours cold.

A real win needs the ~19 missing instructions found (probably in the planet
and enemy loops) *and* the duplicate recompute removed, so the planet-lock
tail lands at `12e748` and the landmark loop at `12e7a8`. That is a
multi-wave reconstruction, not a local spelling search.

## 7. Artifacts

Scratch (not committed, `/dev/shm/w7606`): `Radar.base.cpp` (pristine
source), `score.py`, `diff.py`, `dump.py`, `align.py`, `try.py`,
`try2.py`–`try4.py`, `sweep.py`–`sweep7.py`, `orig.asm`, `ours.asm`,
`orig.calls`, `ours.calls`. The analysis is self-contained in this note.
