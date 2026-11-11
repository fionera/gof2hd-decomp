# Radar::draw wave 180 (w12407) — crosshair `left`/`top` float cells

**Status:** landed, 2026-11-11. Worktree `wt/w12407`, branch `wave180-w12407`,
base `master` `99e341e9b` (wave-179 merge). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave179-w12307.md` (the landed 6,407 shape this pass starts
from) and `radar-draw-wave174-w11907.md` (the `campaign`/`sl` allocation
residual, still open). This note is self-contained: every number below was
measured on `99e341e9b` in this worktree.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,407 | **6,392** (-15) |
| instruction-index alignment | 1,806 | 1,837 |
| instruction count | 3,542 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 76,870 | **76,855** (-15) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 709 (size/base exact) | 709 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` | 0 / 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 / 0 |
| total differing bytes | 77,598 | **77,584** |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

Aligned rises (1,806 -> 1,837) while raw falls; raw is the landing metric and
the gate is unchanged. The function is still not byte-exact.

## 1. Landed change (two cells, net size 0)

Both cells change a crosshair-bounds local from `int` (converted at the
declaration) to `float` (converted at the use site). The original computes the
difference in an FP register and truncates once; the `int` local forced the
conversion before the branch, which changed the scheduling of the whole
bounds block. No condition semantics change: `(int) (x - (float) size)` and
`float left = x - (float) size; (int) left` are the same truncation.

```diff
@@ landmark crosshair bounds (line ~438)
                     int size = this->field_0x124;
-                    int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
+                    float left = PlayerEgo::crosshairPos.x - (float) size;
                     int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
                     bool topOk = top < this->screenY;
-                    if (this->screenY < top + size + size && topOk && this->screenX < left + size + size &&
-                        left < this->screenX) {
+                    if (this->screenY < top + size + size && topOk && this->screenX < (int) left + size + size &&
+                        (int) left < this->screenX) {

@@ gas-spark inSight bounds (line ~962)
                         int size = this->turretScopeHalfWidth;
                         int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
-                        int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
-                        inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
-                                  this->screenY < top + size * 2;
+                        float top = PlayerEgo::crosshairPos.y - (float) size;
+                        inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > (int) top &&
+                                  this->screenY < (int) top + size * 2;
```

## 2. Attribution (all size 9,908)

| variant | raw | aligned |
|---|---:|---:|
| base (w12307 landed) | 6,407 | 1,806 |
| `L` landmark `left` int -> float | 6,404 | 1,816 |
| `S` spark `top` int -> float | 6,395 | 1,827 |
| **`L+S` (landed)** | **6,392** | **1,837** |

The two cells are nearly additive (-3, -12, combined -15). `L` alone is the
same raw as removing the `left` alias (6,404) and `S` alone the same as
removing the `top` alias (6,395); the float form is the cleaner source shape
and reaches the same allocation. Removing the aliases entirely gives the same
raw but repeats the cast expression twice.

## 3. Sweeps performed (post-wave-179 base)

Every variant is a full `link_so` + `fn_diff` under
`ulimit -v 12000000`, `-j2`.

### 3.1 Single `__builtin_expect` re-sweep (66 variants)

All 33 sites were removed and flipped. Best is neutral at 6,407:
`rm4` (line 433), `rm14` (inner line 653), `rm17` (line 785), `rm29`
(line 1283), `fl14`, `fl17`, `fl19`. No single edit wins.

### 3.2 Crosshair `size`/`left`/`top` alias sweep

Every `int size`/`int left`/`int top` local in the five crosshair-bounds
blocks (landmark, planet, enemy tractor, gas-spark, gas, asteroid) was
removed (inlined) and/or changed to `float`, individually and in all
combinations. Only the two landed cells win:

| block | `left` float | `top` float | remove `left` | remove `top` |
|---|---:|---:|---:|---:|
| landmark | **6,404** | 6,407 | **6,404** | 6,407 |
| planet | 8,816 | 8,816 | 8,657 | 9,591 |
| enemy tractor | 8,663 | 8,663 | 6,415 | 6,449 |
| gas-spark | 6,406 | **6,395** | 6,406 | **6,395** |
| gas | 8,663 | 8,663 | 6,407 | 6,433 |
| asteroid | 6,456 | 6,456 | 6,415 | 6,456 |

(`landmark`/`spark` numbers are on the base; the rest are single-cell
measurements on the base.) `int size` removal is neutral in every block.

### 3.3 Re-sweeps on the landed 6,392 shape

* Single expect remove/flip (66): best neutral 6,392 (`rm4`, `rm14`, `rm17`,
  `rm29`, `fl14`, `fl17`, `fl19`).
* Expect pairs (652 pairs with size deltas summing within +/-8): best neutral
  6,392 (`fl14+rm17`, `fl14+fl17`, `fl14+fl29`, ...). No pair wins.
* Crosshair block sweep (40 variants): no further win; the landed spark block
  is already at its best.
* Remaining alias removals: `camZ` 6,456, `hasDockNav` 6,412, `target`
  (player) neutral, `cargo` neutral, `blip` (enemy) neutral, `e` neutral,
  `current` neutral, `landmark` neutral, `topOk` neutral, `candidate` 6,404,
  `last` 7,876, `rowHeight` 8,531, `offset` 8,688, `campaignMission` 9,818,
  `freelance` 9,422, `isTarget` 8,527, `notTurretMode` 8,808. No win.
* Immutable-local removals: `total` neutral, `level` neutral, `system`
  neutral, `race` 6,421, `warpGate` 9,316, `stationId` 9,303,
  `currentStation` 9,941, `enemy` 10,122, `wreck` 10,053, `hull` 8,495,
  `cloud` 8,549, `sparks` 10,229, `distance` 9,455, `range` 9,320,
  `candidateWreck` 7,553, `mission` 9,408, `waypoint` 9,004,
  `autoPilotTarget`/`targets` 9,182, `stationIndex` 9,210, `frames` 6,946,
  `asteroid` 9,541. No win.
* Type changes: `bool missionBlocksPlanets` 6,859 (9,916), `int alwaysEnemySeen`
  9,142, `int alwaysEnemy` 8,591, `int turretMode` 9,256, `bool alienOrbit`
  6,400, `unsigned campaign` 6,393, `bool otherLock` 9,254, `int landmarkHit`
  8,701, `bool notTurretMode` 9,546, `bool notOtherLock` 8,878,
  `int planetHit` 9,261, `int marked` 8,445, `int bossSeen` 9,471,
  `int wreck` 8,234, `int gasHit` 7,990, `int asteroidHit` 7,737. Neutral:
  `int missionBlocksPlanets`, `int notTurretMode`, `int enemyHit`,
  `int onPlanetCourse`, `int candidateWreck`, `int ambient`, `int hasDockNav`,
  `int topOk`, `int campaign`.
* `float cx`/`float cy` in the two enemy crosshair blocks: 8,330 / 6,427
  (worse).
* Condition permutations (24 each) in the landmark and spark blocks on the
  landed shape: best neutral 6,392; no order beats the landed order.

## 4. Remaining residual

Unchanged from wave 174/179: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses `sl` for the
stations array (`12e3c6 mov sl,r0`, `12e4be ldr.w r0,[sl,#4]`), with the
planet-loop index in `fp`. Ours spills `campaign` to `sp+0x90`, keeps
`stations` at `sp+0x84` and uses `sl` for the loop index. Forcing
`campaign` into `sl` is still size-short and raw-worse (w11907). The tail
literal-pool placement and the `marked` stack normalization are the other
visible residuals.

The landed cells do not touch the `campaign` allocation; the aligned metric
worsens because the two bounds blocks now schedule closer to the original's
raw bytes but further from its instruction order.

## 5. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12407/Radar.final.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12407/` —
`Radar.base.cpp` (post-wave-179 base), `Radar.final.cpp` (landed source),
`harness.py` (build+measure), `sweep_expect.py` / `sweep_expect2.py`
(single-expect sweeps), `alias_sweep.py` / `alias2.py` / `alias3.py`
(alias sweeps), `block_sweep.py` / `block_sweep_win.py` (crosshair-block
sweeps), `local_sweep.py` / `perm_landmark.py` / `perm2.py` (local/condition
permutations), `pair_sweep.py` (expect pairs), `type_sweep.py`, `combo.py`,
`secdiff.py` (section diff), and the `*.json` measurement dumps.
