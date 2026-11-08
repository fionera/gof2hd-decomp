# Radar::draw wave 141 (w8707) — cold-block reconstruction after removing the post-loop recompute (no source win)

**Status:** research note, 2026-11-08. Worktree `wt/w8707`, based on `master`
(`1d10d0694`). **No source change landed**; this commit contains this note only.
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8106** (the w8610
shape). Removing the invented post-loop `if (!alienOrbit)` `onPlanetCourse`
recompute gives **9860 B** (48 short; w8609 measured 9856 on the w8509 base) and
raw **9186** — the removal alone is a 1080-byte raw regression because the whole
cold-block layout shifts.

Read with `radar-draw-wave141-w8609.md`, `radar-draw-wave140-w8510.md`,
`radar-draw-wave137-w8212.md`, `radar-draw-wave141-w8610.md`.

## 1. What the 48-byte gap actually is (Thumb evidence)

The original has exactly one `getAutoPilotTarget` (0x12e6f4) and one
`getStationEnumIndex` (0x12e72a), so the post-loop recompute is genuinely
invented. The 48 B is **not** one missing cold block; it is a sum of local
block-size differences, all confirmed against the original's disassembly:

| site | original | ours (norecompute) | delta |
|---|---:|---:|---:|
| top-level `onPlanetCourse` else (0x12e6ee) | 88 B | 80 B | **+8** |
| sound `current != 151 && alwaysEnemySeen` (0x130298) | `cmp/beq/ldr/eors/bne` = 12 B | `ldr/cbz/cmp/beq` = 8 B | **+4** |
| `if (!missionBlocksPlanets && planetLockTimer >= 1)` (0x130486) | `ldr/movs/cmp/it/movlt/orrs/bne` = 20 B | `cmp/bne/b` + `cmp/blt` = 14 B | **+6** |
| cargo ±24000 bound order | `< -24000` first, then `> 24000` | `> 24000` first, then `< -24000` | 0 (same size) |

The top-level `onPlanetCourse` +8 is the spill the mission named: the original
does `str r0,[sp,#152]` for `autoPilotTarget` and saves `r9/fp/r8/sl` around the
block; ours keeps `autoPilotTarget` in `r9` and saves only `fp/sl`. This is a
`GreedyRA` fixed point (more live values in the original), not a source edit.

The sound-condition +4 is a **real type difference**: the original's
`eors.w r2,r2,#1` proves `alwaysEnemySeen` is a `bool` (0/1). Our source has
`int alwaysEnemySeen`, which clang folds to `cbz`. Changing it to `bool` on the
norecompute base reproduces the original's `cmp r1,#151; beq; ldr; eors; bne`
and grows the function by **12 B** (9860 -> 9872), of which 4 B is this block
and 8 B is register-allocation fallout.

The `missionBlocksPlanets` +6 is a real source-shape difference: ours merges the
second `!missionBlocksPlanets` test into the preceding `if (missionBlocksPlanets)
goto enemies` (`cmp r5,#0; bne enemies; b planetLockTimerCheck`, 8 B) plus a
bare `cmp r0,#1; blt` (6 B); the original keeps the combined
`missionBlocksPlanets || planetLockTimer < 1` test (20 B). Re-spelling it as
`!(missionBlocksPlanets || planetLockTimer < 1)`, `planetLockTimer >= 1 &&
!missionBlocksPlanets`, or `missionBlocksPlanets || planetLockTimer <= 0`
changes nothing (clang re-folds it).

## 2. Variant matrix (all on the norecompute base, 9860 B)

Every variant below is a source type/spelling edit added to the recompute-free
base. `A` = `bool alwaysEnemySeen` (the faithful type).

| variant | usize | raw |
|---|---:|---:|
| norecompute base | 9860 | 9186 |
| `A` | 9872 | 9118 |
| `A + int enemyHit` | 9896 | 9145 |
| `A + int enemyHit + int wreck` (enemy) | **9908** | 9113 |
| `A + int isTarget + int enemyHit + int wreck` (asteroid) | **9908** | 8688 |
| `A + bool marked` | 9928 | 9121 |
| `A + int topOk` | 9880 | 9089 |
| `A + int wreck` (enemy) | 9880 | 9089 |
| `int marked -> bool` alone | 9920 | 9089 |
| `int enemyHit` alone | 9884 | 9190 |
| `int asteroidHit` alone | 9880 | 9202 |

The two 9908-B norecompute variants are raw **8688** and **9113**, both far
worse than the with-recompute baseline **8106**. So the "remove the recompute
and re-absorb the 48 B in cold blocks" hypothesis does **not** produce a raw win
with any type/spelling edit tried; the recompute-free layout never re-aligns
with the original.

## 3. Ruled out

* `bool missionBlocksPlanets`, `bool alienOrbit`, `bool landmarkHit`,
  `bool hasDockNav`, `bool candidateWreck`, `bool ambient`, `int inSight`,
  `int dead` (enemy/asteroid), `int navWreck`: 0 size change on the norecompute
  base.
* `bool turretMode`: 9800 (-60).
* `int bossSeen`: 9852 (-8).
* `int alwaysEnemy`: 9868 (+8).
* `int planetHit`: 9868 (+8); `int gasHit`: 9864 (+4); `int isTarget` (campaign):
  9864 (+4); `int wreck` (asteroid): 9864 (+4).
* `__builtin_expect(!alienOrbit, 0)` on the recompute: identical 9908 / 8106
  (the block is already cold).
* Reordering the cargo ±24000 bounds: 0 size change, raw unchanged.

## 4. Open questions

The 48 B is real but distributed: +8 top-level spill (register pressure, not
source), +4 `alwaysEnemySeen` bool (faithful, but costs 8 B of layout
elsewhere), +6 `missionBlocksPlanets` merge (clang re-folds every spelling).
The remaining ~30 B were not located as a single cold block; the original's
cold blocks (cargo 254 vs 252, hudEvent 32 vs 32, blip frames 192/64/78 vs
192/64/78, sound blocks 70/48/48 vs 72/50/48, station timer 132 vs 132) differ
only by ±2-byte branch encodings. The next wave should try forcing
`missionBlocksPlanets` into `fp` (the original's register) or a source shape
that keeps `autoPilotTarget` live across the `getSystem` chain.

## 5. Gate

Baseline source restored unchanged; gate unchanged from master:
`RATCHET PASS`, byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
Radar::draw raw 8106, 9908 B; `.text` 84,567 differing bytes.
