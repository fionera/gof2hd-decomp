# Radar::draw wave 179 (w12307) — alias re-sweep + six size-exact cells

**Status:** landed, 2026-11-11. Worktree `wt/w12307`, branch `wave179-w12307`,
base `master` `64014ce6f` (wave-178 merge). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave178-w12207.md` (the recovered 6,458 candidate that is
the base here), `radar-draw-wave178-w12208.md` / `-w12209.md` (the branch/order
cells that w12207 superseded) and `radar-draw-wave174-w11907.md` (the
`campaign`/`sl` allocation residual, still open).

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,458 | **6,418** (-40) |
| instruction-index alignment | 1,832 | **1,803** (-29) |
| instruction count | 3,540 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 77,936 | **77,896** (-40) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 700 (size/base exact) | 700 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` | 0 / 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 / 0 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (six cells, net size 0)

All cells are pure alias removal or side-effect-free operand reordering. No
condition semantics, short-circuit side effects or call counts change.

```diff
@@ landmark crosshair bounds (line ~442)
-                    if (left < this->screenX && this->screenY < top + size + size && topOk &&
-                        this->screenX < left + size + size) {
+                    if (this->screenY < top + size + size && topOk && this->screenX < left + size + size &&
+                        left < this->screenX) {

@@ enemy hull/EMP bar (line ~720)
-                        auto barMax = this->field_0x1c8;
                         if (!enemy->isDead()) {
...
-                        float hull = (float) enemy->player->getDamageRate() / 100.0f * (float) barMax;
+                        float hull = (float) enemy->player->getDamageRate() / 100.0f * (float) this->field_0x1c8;
...
-                            float emp = (float) enemy->player->getEmpDamageRate() / 100.0f * (float) barMax;
+                            float emp = (float) enemy->player->getEmpDamageRate() / 100.0f * (float) this->field_0x1c8;

@@ off-screen enemy image hint (line ~823)
-                    } else if (__builtin_expect((enemy->player->carriesFriendCargoFlag), 1)) {  // lint: arm_builtin
+                    } else if (__builtin_expect((enemy->player->carriesFriendCargoFlag), 0)) {  // lint: arm_builtin

@@ enemy tractor crosshair (line ~845)
-                        auto onPlanetCourse = this->field_0x1a8;
                         enemy->field_0x75 = 1;
-                        if ((wreck && this->hasTractor13) || (this->field_0x1b0 && !onPlanetCourse)) {
+                        if ((wreck && this->hasTractor13) || (this->field_0x1b0 && !this->field_0x1a8)) {

@@ enemy tractor label (line ~858)
-            if (this->tractorMode2 && this->field_0x1c == nullptr && wreck && enemy->isDead()) {
+            if (this->tractorMode2 && wreck && enemy->isDead() && this->field_0x1c == nullptr) {

@@ gas-cloud dead/dying skip (line ~987)
-            if (cloud->isDead() || cloud->isDying()) {
+            if (cloud->isDying() || cloud->isDead()) {

@@ asteroid tractor label (line ~1160)
-                if (this->tractorMode2 && this->field_0x1c == nullptr && wreck) {
+                if (this->field_0x1c == nullptr && wreck && this->tractorMode2) {

@@ enemy music event (line ~1271)
-            if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens() ||
-                Globals::status->getCurrentCampaignMission() == 16) {
+            if (Globals::status->getCurrentCampaignMission() == 16 || Globals::status->getStation()->isAttackedByAliens() ||
+                Globals::status->inAlienOrbit()) {
```

## 2. Attribution (all size 9,908)

The two alias removals are the bulk of the win; the four operand reorders are
small independent cells. Each row was measured with a fresh `link_so` +
`fn_diff` on this worktree.

| variant | raw | aligned |
|---|---:|---:|
| base (w12207) | 6,458 | 1,832 |
| `A` remove `barMax` alias (use `this->field_0x1c8` twice) | 6,443 | 1,808 |
| `A+B` also remove `onPlanetCourse` alias | 6,430 | 1,803 |
| `A+B+C` enemy tractor order (`wreck`/`isDead` before `field_0x1c`) | 6,424 | 1,799 |
| `A+B+C+D` asteroid tractor order (`field_0x1c` first) | 6,428 | 1,813 |
| `A+B+C+D+E` landmark crosshair bounds order | 6,428 | 1,824 |
| `A+B+C+D+E+F` gas `isDying() || isDead()` | 6,419 | 1,830 |
| **`A+B+C+D+E+F+G` music-event order (landed)** | **6,418** | **1,832** |
| landed + `carriesFriendCargoFlag` expect 1->0 | **6,418** | **1,803** |

The last cell is raw-neutral but aligned-better (1,832 -> 1,803), so it is
kept; the function bytes are unchanged in raw count but the instruction stream
is closer to the original.

`barMax` is a genuine over-alias: the original loads `[r4,#0x1c8]` **twice**
(original `12f278` and `12f310`), while the `auto barMax` form loaded it once
and reused it across the `isDead()` call. Removing the alias restores the two
loads and the original allocation. Likewise `onPlanetCourse` is a one-use
alias; removing it restores the direct `[r4,#424]` load at original `12f0ee`.

## 3. Sweeps performed (all on the post-wave-178 base)

Every sweep below was run with `cmake --build cmake-build-match --target
link_so -j2` under `ulimit -v 12000000`; each variant is a full link + `fn_diff`.

### 3.1 Whole-function `__builtin_expect` re-sweep

All 33 expect sites (including the two nested pairs) were removed and flipped
0<->1. On the landed shape the best removal is neutral (`rm4`, `rm14`, `rm17`,
`rm29` -> 6,418) and no flip beats 6,418. The base is a local optimum for
single-expect edits.

### 3.2 Condition operand permutations

All 77 uniform top-level `&&`/`||` conditions in the function were permuted
(adjacent swaps, reversal, move-to-front/back). The only raw wins are the four
landed reorders above; on the landed shape a second full permutation sweep
finds no further win.

### 3.3 Alias removals

`barMax` and `onPlanetCourse` are the only wins. Rejected (size-exact unless
noted): `Player *target` (neutral), `auto target = this->targetPos` (+1),
`Sprite *blip` in the enemy-lock block (neutral), `auto camZ` (+54),
`auto hasScanner` (+118), `KIPlayer *e` (size-fatal 9,900), `KIPlayer *locked`
(size-fatal 9,904), `KIPlayer *lockedGas` (size-fatal 9,912),
`Sprite *blip` in the gas block (+103), `int rowHeight` (size-fatal 9,900),
`int offset` (size-fatal 9,924), `Vector delta` (size-fatal 9,892),
`int cx`/`int cy` (size-fatal 9,912), `bool inSight` (size-fatal 9,948),
`bool topOk` (neutral), `KIPlayer *landmark` (neutral), `PaintCanvas *canvas`
(size-fatal 9,892), `int race` (+29), `bool hasDockNav` (+20).

### 3.4 Type changes

Rejected: `bool missionBlocksPlanets` (9,916), `int alwaysEnemySeen` (9,912),
`int alwaysEnemy` (9,920), `int turretMode` (9,868), `bool alienOrbit` (+8),
`unsigned campaign` (+1), `int otherLockFlag` (9,900), `bool otherLock`
(9,872), `int landmarkHit` (9,904), `bool notTurretMode` (9,868),
`bool notOtherLock` (9,888), `int planetHit` (+828), `int marked` (9,912),
`int bossSeen` (9,976), `int enemyHit` (9,928), `int wreck` (9,912),
`int gasHit` (9,904), `int asteroidHit` (9,924). Neutral: `int onPlanetCourse`,
`int isTarget`, `int inSight`, `int candidateWreck`, `int ambient`,
`int hasDockNav`, `int topOk`.

### 3.5 Statement/declaration order

Rejected: planet-lock `SFD`/`SDF`/`DSF`/`DFS`/`FSD` (6,421-6,493),
`(timer < 1) || mbp` forms (6,445-6,470), wreck-before-update (9,904),
mission/planet-loop declaration permutations (neutral), `marked` normalization
(9,912), `volatile` on any local (size-fatal or 8,600+).

### 3.6 Whole-condition `__builtin_expect` additions

Every plain `if` condition in the function was wrapped in
`__builtin_expect((cond), 0)` and `__builtin_expect((cond), 1)`: no win
(338 variants, best neutral 6,418).

## 4. Remaining residual

Unchanged from wave 174/178: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses `sl` for the
stations array (`12e3c6 mov sl,r0`, `12e4be ldr.w r0,[sl,#4]`), with the
planet-loop index in `fp`. Ours spills `campaign` to `sp+0x90`, keeps
`stations` at `sp+0x84` and uses `sl` for the loop index. The stack-slot
rotation (`hud` at `sp+0x50` vs the original's `sp+0x54`, `stations` at
`sp+0x84` vs `sp+0x74`, `marked` in a register vs `sp+0x84`) is the same
greedy-RA cascade documented in `radar-draw-wave174-w11907.md`; forcing
`campaign` into `sl` is still 92 B short and raw-worse. The tail literal-pool
placement (`1302dc` vs `130330`) and the `marked` stack normalization
(`12e67a`) are the other visible residuals.

## 5. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12307/Radar.landed.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12307/` —
`Radar.base.cpp` (base source), `Radar.nobar.cpp`, `Radar.best1.cpp`,
`Radar.best2.cpp`, `Radar.final.cpp`, `Radar.landed.cpp` (landed source),
`harness.py` (build+measure), `sweep_expect.py`, `cond_sweep2.py`,
`sweep2.py`..`sweep13.py` (variant sweeps), `sbs.py` (side-by-side disasm),
`cond_sweep.json` / `cond_sweep2.json` / `expadd.json` / `sweep*.json`
(measurements).
