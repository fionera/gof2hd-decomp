# Radar::draw wave 171 (w11708) — condition-order / cbz / tail sweeps, no win

**Status:** no source change landed, 2026-11-11. Worktree `wt/w11708`, branch
`wave169-w11708`, base `master` `41ef4c8b6`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave171-w11608.md` and
`build-provenance-round11-extab-flags.md`.

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,061 |
| instruction-index alignment | 1,561 |
| instructions | 3,540 ours / 3,532 orig |
| `.text` differing bytes | 81,661 |
| `.ARM.extab` differing bytes | 711 |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN) |

## 1. Where the residual actually is

`rawhist.py` (256-byte windows) shows the function is nearly byte-identical at
`fn+0x000..0x2ff` (11/1/1 differing bytes) and again at
`fn+0x1800..0x1bff` (3/3/7/6 bytes). Everything between is a cumulative layout
shift. `bytediff.py` on the first 0x300 bytes shows the only content diffs are:

* `missionBlocksPlanets` in `fp` (orig) vs `sl` (ours);
* `campaign` in `sl` (orig, `12e0f8 mov sl,r0`, one use at `12e38a
  cmp.w sl,#2`) vs spilled at `sp+144` (ours);
* the resulting 4-byte stack-slot cascade (`mode` `sp+100` vs `sp+104`,
  `turretMode` `sp+148` vs `sp+152`, `otherLock` `sp+144` vs `sp+148`,
  `planetHit` `sp+152` vs `sp+144`, `stations` `sp+116` vs `sp+132`);
* branch-target displacements (layout).

The planet loop is the concrete cost: orig keeps `stations` in `sl` and the
loop index in `fp`; ours keeps the index in `sl` and reloads `stations` from
`sp+132` each iteration (`12e3fc ldr r0,[sp,#132]` vs orig `12e3fa ldr.w
r0,[sl,#4]`), which is the source of the 8 extra instructions. The route block
(`12e120..12e1da`) is already byte-identical apart from one branch target.

## 2. Sweeps tried (all size 9,908 unless noted; raw is ground truth)

### Landmark guard `!landmarkHit && i != 3 && notTurretMode && notOtherLock`

All 24 permutations. Base order is the best raw; the "faithful" reorder
`!landmarkHit && notTurretMode && i != 3 && notOtherLock` (which matches the
original's two-group `bics/ands; cmp; bne; ands; beq` shape) is **+15 raw**
(7,076) but -4 aligned (1,557). Rejected on raw.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base `!landmarkHit && i != 3 && notTurretMode && notOtherLock` | **7,061** | 1,561 | 9,908 |
| `!landmarkHit && notTurretMode && i != 3 && notOtherLock` | 7,076 | 1,557 | 9,908 |
| `i != 3` first | 8,538 | 1,577 | 9,904 |
| `notTurretMode` first | 9,142 | 1,721 | 9,900 |

Also tried: split into nested `if`s (size 9,872), `bool` `notTurretMode` /
`notOtherLock` (9,029 raw, 9,928 B), separate `continue`s for the loop guard
(neutral).

### Other condition permutations (all neutral or worse)

* planet guard `!otherLock && !planetHit && !turretMode`: 3 of 6 permutations
  are neutral (7,061); the others 9,236 raw / 9,932 B.
* enemy guard `!(isAutoPilot() || otherLock || enemyHit)`: base best (7,061);
  next 7,136.
* planet-block `campaign >= 2 && planetTargets && !alienOrbit &&
  !isDockingToPlanet`: all 24 permutations; base best.
* landmark `onScreen`/`screenX` expects: base best; dropping the `onScreen`
  expect is 9,550 raw / 9,872 B.
* `i & 0x7fffffff` vs plain `i == 3` / `i == 1`: neutral.
* `!turretMode && !enemyHit` swaps, `isDockingToAsteroid` /
  `isDockingToDockingPoint` swap: neutral.

### Music-tail expects

* outer `expect(field_0x1bc >= 1, 1)`: base best (7,061). Removing it gives
  aligned 1,434 but raw 7,116; `expect(...,0)` is 8,551.
* inner `< 3` / `< 5` probabilities: base `(0,0)` best; `(1,0)` 7,096,
  `(0,1)` 7,106, `(1,1)` 7,115.
* `alwaysEnemySeen` / `bossSeen` expects: base is in the best set.

### Campaign / `locked` allocation re-tries

* `campaign` as `int`/`const`/`unsigned`/`short`/`volatile`/`static`,
  `campaignStarted = campaign >= 2` bool (folded back, neutral), inlining the
  getter at the planet block (9,267 raw / 9,900 B), using `campaign` in the
  148/152 checks (8,966 raw, 9,916 B).
* `missionBlocksPlanets` `int` (7,080 raw / 9,904 B), `planetHit` `int`
  (9,130 raw / 9,936 B), `landmarkHit` `int` (8,682 raw / 9,928 B),
  `otherLockFlag` `int` (9,179 raw / 9,876 B), `otherLock` `bool` (9,228 raw /
  9,844 B).
* deleting the `KIPlayer *locked` local and using `this->lockedEnemy` at the
  comparison sites: 9,094 raw / 9,908 B, but `.text` 83,694 and
  `.ARM.extab` 710 (-1). Rejected (raw regression).
* `-mllvm -disable-postra-machine-sink` added to the Radar TU: 7,182 raw.

### Landing-pad experiment

The extra landing pad is the `sp+220 -> sp+232` entry in the cleanup region
(`1306d6` ours, absent in orig; orig has `sp+220 -> sp+208 -> sp+232`). Naming
the landmark concatenation temporaries (`String separator(": ")` or
`String tecLevel(...)`) changes the whole-TU data layout and is much worse
(8,671 / 7,589 raw; `.text` 439,881 / 391,277). Reverted.

## 3. Conclusion

No measured raw win. The residual remains the `campaign`-in-`sl` vs
`sp+144` allocation, which cascades the 4-byte stack-slot shift and the
`stations` reload. Every source spelling tried leaves the allocator's choice
intact; the `+cheap-predicable-cpsr` flag and `-disable-postra-machine-licm`
already in `CMakeLists.txt` are still the best known settings.

## 4. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /home/debian/GalaxyOnFire2/scratch/w11708/{adiff.py,rawhist.py,bytediff.py,secdiff.py,sweep_*.py}
```
