# Radar::draw wave 190 (w13465) — gas-cloud `field_0x73` store hoist
# raw 6,218 -> 6,177 (-41), size-exact, `.rel.plt` identical

**Status:** landed, 2026-11-12. Worktree `wt/w13465`, branch `wave190-w13465`, base
`master` `41443d2e8`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13440.md` (gas-cloud blip draw hoist, the current base),
`radar-draw-wave190-w13448.md`, `radar-draw-wave190-w13398.md`,
`radar-draw-wave190-w13437.md` and the campaign/`sl` family
`radar-draw-campaign-sl-wave164..167-*.md`.

## 0. The change

In the gas-cloud on-screen block the unconditional `cloud->field_0x73 = 0;` store was
emitted after the 24,000-unit box test.  Hoisting it to the top of the block (right after
`KIPlayer *locked = this->lockedGasCloud;`) is semantically identical — the if-arm also
stores zero, and nothing between the two positions reads `field_0x73`:

```cpp
             if (this->onScreen) {
                 KIPlayer *locked = this->lockedGasCloud;
+                cloud->field_0x73 = 0;
                 cloud->field_0x76 = 1;
                 this->egoPos = player->getPosition();
                 Globals::Canvas->DrawImage2D(...);
                 this->targetPos = cloud->player->getPosition();
                 if (24,000-unit box test) {
                     cloud->field_0x73 = 0;
                 }
-                cloud->field_0x73 = 0;
```

The gas block's emitted order becomes `field_0x73=0` -> `field_0x76=1` -> draw, which
removes 41 same-address bytes (the `12f650`-`12f850` / `12fd50`-`12fe50` windows and the
asteroid tail shift back toward the original layout).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,218 | **6,177 (-41)** |
| instruction-index alignment | 2,025 | 2,025 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,173 | **72,132 (-41)** |
| `.ARM.extab` differing bytes | 577 | 577 (no growth) |
| `.ARM.exidx` | 0 | 0 |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 72,770 | **72,729** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. How it was found

An exhaustive single-statement rotation sweep over the whole `Radar::draw` body (449
statements; every same-brace-depth move at statement distance 2-10, 1,958 variants) found
this as the best semantically safe cell.  Other measured wins in the sweep (scratch
`/home/debian/GalaxyOnFire2/scratch/w13465/sweep.py`, `sweep_results.json`):

| move | raw | size | faithful? |
|---|---:|---:|---|
| gas `field_0x73=0` before `field_0x76=1` (**landed**) | **6,177** | 9,908 | yes |
| gas `field_0x76=1` after the unconditional `field_0x73=0` | 6,208 | 9,908 | yes |
| gas `field_0x73=0` before `egoPos`/draw | 6,180 | 9,908 | yes |
| gas `locked` after `egoPos`/draw | 6,217 | 9,908 | yes |
| asteroid `lockedAsteroid=nullptr` into `best` block | 6,138 | 9,908 | no (conditional on `field_0x10 != data[best]`) |
| asteroid `asteroidLockTimer=0` into `lockConfirmed` block | 6,177 | 9,908 | no (conditional on `lockedAsteroid == field_0x10`) |
| landmark `image_0x88` draw into the hit-test block | 6,214 | 9,908 | no (conditional on `!landmarkHit && ...`) |

## 2. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6177 aligned= 2025 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 3. Follow-up win: planet-loop `currentStation` hoist (raw 6177 -> 6176)

On the post-win base (6,177), hoisting the `currentStation` getter above the
`if (!this->onScreen) continue;` guard in the planet loop is size-exact and removes one
more same-address byte (aligned 2,025 -> 2,032):

```cpp
             this->update(this->planetTargets->data()[i]);
+            int currentStation = Globals::status->getStation()->getIndex();
             if (!this->onScreen) {
                 continue;
             }
             int stationId = stations->data()[i];
-            int currentStation = Globals::status->getStation()->getIndex();
```

`getStation()`/`getIndex()` are pure getters, so evaluating them before the off-screen
`continue` is semantically identical.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,177 | **6,176 (-1)** |
| instruction-index alignment | 2,025 | **2,032** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 72,132 | **72,131 (-1)** |
| `.ARM.extab` differing bytes | 577 | 577 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 72,729 | **72,728** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 4. Follow-up win: gas-cloud 24,000-box axis form (raw 6176 -> 6175)

The gas-cloud box test spelled the x axis as `targetPos - egoPos` but the y/z axes as
`egoPos - targetPos`.  Making all three axes `targetPos - egoPos` is semantically identical
(the `> +24000 || < -24000` pair is symmetric under a sign flip) and removes one more
same-address byte:

```cpp
                 if (this->targetPos.x - this->egoPos.x > 24000.0f || this->targetPos.x - this->egoPos.x < -24000.0f ||
-                    this->egoPos.y - this->targetPos.y > 24000.0f || this->egoPos.y - this->targetPos.y < -24000.0f ||
-                    this->egoPos.z - this->targetPos.z > 24000.0f || this->egoPos.z - this->targetPos.z < -24000.0f) {
+                    this->targetPos.y - this->egoPos.y > 24000.0f || this->targetPos.y - this->egoPos.y < -24000.0f ||
+                    this->targetPos.z - this->egoPos.z > 24000.0f || this->targetPos.z - this->egoPos.z < -24000.0f) {
```

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,176 | **6,175 (-1)** |
| instruction-index alignment | 2,032 | 2,032 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes | 72,131 | **72,130 (-1)** |
| `.ARM.extab` differing bytes | 577 | 577 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 72,728 | **72,727** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.
