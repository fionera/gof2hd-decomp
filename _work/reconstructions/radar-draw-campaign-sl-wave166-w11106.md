# Radar::draw wave 166 (w11106) — campaign / space-lounge block

**Status:** in progress, 2026-11-10. Worktree `wt/w11106`, branch
`wave166-w11106`, base master `85c1f88df`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-campaign-sl-wave164-w11007.md`,
`radar-draw-campaign-sl-wave163-w10909.md`,
`radar-draw-campaign-sl-wave165-w11106.md`,
`radar-draw-wave164-w11008.md`.

## 0. Baseline

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,549 |
| instruction-index alignment | 1,446 |
| `.text` differing bytes | 83,148 |
| `.ARM.extab` differing bytes | 659 |
| `.rel.plt` | byte-identical |
| gate | byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5370 |

## 1. Landed changes

* `expect(isDockingToPlanet(), 1)` on the planet-lock blip's docking skip
  (`if (isDockingToPlanet()) goto enemies;`).  raw 7,549 -> **7,546**,
  aligned 1,446 -> 1,460, size 9,908 exact.  Pure branch-probability hint.

## 2. Block map re-verified

The wave-164 w11007 map is correct.  The original campaign block is
`12e390`–`12e7a8`; `campaign` lives in `sl`, the zero/missionBlocksPlanets
vreg in `fp`, `marked` in `sp+132`, `planetHit` in `sp+152`,
`turretMode` in `sp+148`.  Our base has `campaign` spilled to `sp+144`,
zero/mbp in `sl`, `marked` in `fp`, `planetHit` in `sp+144`,
`turretMode` in `sp+152`.

The original's body1 is inline (guard hot, `12e474`); ours is outlined
(`12e5ca`) because of the load-bearing `expect(guard, 0)`.  Removing it
gives the original's inline body1 but the mission chain then becomes
inline too, pushing the else far away: size 9,976 (+68), raw 8,838.

`expect(stationId != currentStation && isTarget, 0)` on body1 moves the
chain out of line to `12e620` (original address) and gives the original
block order, but size 9,924 (+16) and raw 8,548.  The +16 is in the
landmark block / tail; all companion cells tried (exit, freelance, 125,
landmarks, route) either miss 9,908 or worsen raw.

## 3. Sweeps (no win at 9,908 B)

* Type/order sweeps: `campaign` unsigned/long, `missionBlocksPlanets`
  int/char/short/long/unsigned, `turretMode` int/char, `alienOrbit`
  bool/char, `otherLock` bool, `notOtherLock`/`notTurretMode` bool,
  `planetHit` int/char, `marked` int/char, `isTarget` int, `i` int,
  `stationId`/`currentStation`/`textOffset`/`warpGate` unsigned,
  campaign-before-alien/turret — all neutral or worse.
* `expect` cells on every campaign-block condition (guard sub-conditions,
  campaign>=2, planetTargets, alienOrbit, docking, data null, onScreen,
  warp gate, body1, cur==120, chain 116/120/125, freelance, hit-test,
  orbit index, field_0x18, station draw, exit chain, field_0x14,
  planetHit, timer, missionBlocksPlanets, lockConfirmed, field_0x1a8,
  blip, frame) — only `isDockingToPlanet()` in the blip block wins
  (-3 raw); `!lockConfirmed` expected 0 is 7,522 raw but 9,916 B (+8).

## 4. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11106/{sweep.py,sweep_expect.py,sweep_b1cold.py,combo*.py}
```
