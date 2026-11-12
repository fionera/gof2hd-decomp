# Radar::draw wave 190 (w13440) — enemy-lock blip call order (raw 6288 -> 6284,
# size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13440`, branch `wave190-w13440`, base
`master` `915b26b82`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13433.md` (planet-lock store order, the current base),
`radar-draw-wave190-w13426.md` (adjacent-swap sweep) and
`radar-draw-campaign-sl-wave167-w11206.md` (campaign-in-`sl` residual).

## 0. The change

In the enemy-lock blip block (inside the enemy loop, the
`if (__builtin_expect(this->dockNavPtr != current, 1))` body) the three sprite calls
were emitted as `blip->setFrame(...)` -> `this->blipSprite->setRefPixelPosition(...)`
-> `this->blipSprite->draw(...)`.  `Sprite::setFrame` and
`Sprite::setRefPixelPosition` write independent sprite properties, so their order is
free; reordering them (and spelling `setFrame` through the already-live
`this->blipSprite` pointer) changes the global register allocation just enough that
clang schedules the *station* blip block 4 B tighter:

```cpp
-                        blip->setFrame((int) (elapsed / total * frames));
                         this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                               (int) PlayerEgo::crosshairPos.y);
+                        this->blipSprite->setFrame((int) (elapsed / total * frames));
                         this->blipSprite->draw(1.0f, 1.0f);
```

`blip` is still used for `blip->getRawFrameCount()` two lines above, and
`blip == this->blipSprite` at this point (the local was loaded from that member and
nothing writes it in between), so the rewrite is semantically identical.

The raw win is not in the edited block itself: the whole-function diff profile shows
the only changed 0x100 window is `12ff50` (251 -> 247).  The station blip block at
`12ff5e` switches its VFP temps from `s0/s2/s4` to `s16/s18/s0` and emits
`setRefPixelPosition` before `setFrame` (4 B shorter, both orders independent), which
is what removes the four same-address bytes.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,288 | **6,284** |
| instruction-index alignment | 1,913 | 1,937 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,543 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,878 | **72,874** |
| `.ARM.extab` differing bytes | 601 | 601 (no growth) |
| `.ARM.exidx` | 0 | 0 |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,498 | **73,494** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.  The function is already
non-byte-exact, so `byte_exact` is unchanged; the gain is four raw `.text` bytes.

## 1. How it was found

The prior adjacent-statement sweep (`w13426`) only considered single-line statements.
The blip blocks contain multi-line `setRefPixelPosition(...)` calls, so their
permutations were not covered.  A permutation sweep over every
`setFrame`/`setRefPixelPosition`/`draw` triple in `Radar::draw` (station `516`,
planet `661`, enemy `936`, gas `1060`, asteroid `1241`/`1247`) found the enemy block
as the only raw-improving site:

| variant (enemy block) | raw | aligned | size |
|---|---:|---:|---:|
| base (`sf, rp, dr`, `blip->setFrame`) | 6,288 | 1,913 | 9,908 |
| `rp, sf, dr` via `this->blipSprite` (**landed**) | **6,284** | 1,937 | 9,908 |
| `rp` via `blip`, `sf` via `this->blipSprite` | 6,284 | 1,954 | 9,908 |
| `rp, sf, dr` with `sf` via `blip` | 6,285 | 1,935 | 9,908 |
| `dr, rp, sf` (semantically wrong) | 6,284 | 1,933 | 9,908 |
| `rp, dr, sf` | 6,285 | 1,937 | 9,908 |
| `sf, dr, rp` | 6,288 | 1,921 | 9,908 |
| all three via `blip` | 6,873 | 1,916 | 9,904 |

The station/planet/asteroid blocks' permutations were all size-fatal or worse
(station `sf,dr,rp` 6,286; planet `sf,dr,rp` 6,290; asteroid variants 6,842-6,877).
The gas block's two-statement swap is neutral.

## 2. Negative results (all reverted)

Full `link_so -j2` + `tools/fn_diff.py` measurements, scratch
`/home/debian/GalaxyOnFire2/scratch/w13440/`:

* All 465 pairs of `__builtin_expect` weight flips: best neutral 6,288
  (`p14_16` flips the enemy-loop `enemyFlagsLo`/`carriesFriendCargoFlag` hints and
  improves *aligned* 1,913 -> 1,847 but not raw).  No pair beats 6,288.
* All 31 single expect flips: best neutral 6,288 (`flip14`, `flip16`); the rest
  6,290-9,576 or size-fatal.  (Re-confirms w13433 on the post-store-swap base.)
* Removing the landmark-if `__builtin_expect` (w13435's w13435-base win) is
  **worse on this base**: alone 8,540 / size 9,904; together with dropping the
  `onScreen` expect 6,430.  w13435's 6,272 is tied to the *pre*-w13433
  `planetDockIndex`-first store order (its parent `be282d6e9`); the w13433
  `field_0x18`-first order inverts the interaction.
* Reordering the six `this->X = this->level->getY()` assignments (adjacent swaps
  6,292-6,303, full reverse 6,355), the camera `m[3]/m[7]/m[11]` reads (neutral),
  the four background `DrawImage2D` calls (6,301-6,391), or moving the camera block
  before the level block (6,398): no win.
* `otherLock` direct forms, `landmarkHit`/`notTurretMode`/`notOtherLock`
  declaration order, `alwaysEnemySeen`/`alwaysEnemy` order/type,
  `missionBlocksPlanets` assigned in an explicit `else` or after the `if`: all
  6,288 or worse.
* Distance-3 adjacent-statement swaps (160 cells): only the landed enemy-blip cell
  wins (6,284); every other cell 6,288.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6284 aligned= 1937 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13440/`: `Radar.base.cpp` (6,288 base),
`drive.py`, `harness.sh`, `swaps.py` (distance-3 generator), `genexp.py`,
`v1.py`..`v11.py`, `pairs.log` (465 expect pairs), `d3.log` (160 distance-3 swaps),
`rawprof.py`, `plateau.py`, `zerodelta.py`, `pools.py`, `secdiff.py`.

## 4. Follow-up win: station-blip call order (raw 6284 -> 6282)

On the post-win base (6,284), the multi-line adjacent-statement sweep (174 cells,
`stmts.py`) found one more size-exact raw win in the station blip block: moving
`this->blipSprite->draw(1.0f, 1.0f)` ahead of
`this->blipSprite->setRefPixelPosition(...)` (the `setFrame` call stays first):

```cpp
                     blip->setFrame(frame);
+                    this->blipSprite->draw(1.0f, 1.0f);
                     this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                                           (int) PlayerEgo::crosshairPos.y);
-                    this->blipSprite->draw(1.0f, 1.0f);
```

This is a codegen lever rather than a behavioural change: the compiler already
reorders the independent `setFrame`/`setRefPixelPosition` pair, and the reordered
form makes the enemy-lock blip block at `1304a6` two bytes tighter (whole-function
raw profile: `130450` 245 -> 243).  The only other 6,282 cell in the sweep is the
same station-block permutation; every neutral station spelling is 6,284 or worse
(`rp,sf,dr` 6,387; `rp,sf,dr` via `blip` 6,286; all-`blip` 6,378).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,284 | **6,282** |
| instruction-index alignment | 1,937 | 1,964 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes (whole `.so`) | 72,874 | **72,872** |
| `.ARM.extab` differing bytes | 601 | 601 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,494 | **73,492** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 5. Follow-up win: gas-cloud blip draw hoist (raw 6282 -> 6235)

On the post-win base (6,282), a long-distance *rotation* sweep (move one single-line
statement to another position in the same block, distance 6-12; 744 variants) found a
size-exact raw win in the gas-cloud block of `Radar::draw`:

```cpp
             if (this->onScreen) {
                 KIPlayer *locked = this->lockedGasCloud;
                 cloud->field_0x76 = 1;
                 this->egoPos = player->getPosition();
+                Globals::Canvas->DrawImage2D(reinterpret_cast<unsigned int &>(this->reservationDirty),
+                                             this->screenX, this->screenY, 17, 68);
                 this->targetPos = cloud->player->getPosition();
                 if (this->targetPos.x - this->egoPos.x > 24000.0f || ...) {
                     cloud->field_0x73 = 0;
                 }
-                Globals::Canvas->DrawImage2D(reinterpret_cast<unsigned int &>(this->reservationDirty),
-                                             this->screenX, this->screenY, 17, 68);
                 cloud->field_0x73 = 0;
```

`DrawImage2D` only reads `this->screenX/screenY` (already written by the preceding
`this->update(cloud)`) and `this->reservationDirty`; `targetPos` is a pure assignment
from `cloud->player->getPosition()`.  The two are independent, so hoisting the draw
above the `targetPos` store is semantically identical.  The reorder changes the
whole-function register allocation / block layout enough to remove 47 same-address
bytes, almost all in the `12f650`-`12f850` and `12fd50`-`12fe50` windows.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,282 | **6,235** |
| instruction-index alignment | 1,964 | 1,979 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,872 | **72,825** |
| `.ARM.extab` differing bytes | 601 | 601 (no growth) |
| `.ARM.exidx` | 0 | 0 |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` size | 269,351 | 269,351 |
| total (text+extab+build-id) | 73,492 | **73,446** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

### Other rotation results (all >= 6,282 or size-fatal, reverted)

`rota_995_1003` 6,269 / aligned 1,884; `rota_995_1005` 6,270 / 1,887;
`rotb_1199_1210` 6,245 / 1,966 (moves the asteroid `lockedAsteroid = nullptr` store
across the `candidateWreck` block; semantically valid but weaker than the landed
cell); every other rotation 6,381-10,148 or size-fatal.

A *swap* of the asteroid `this->lockedAsteroid = nullptr;` (line 1201) with
`this->asteroidLockTimer += mode;` (line 1213) measures 6,256 but is **not** landed:
it moves the timer increment inside the `if (lockedAsteroid != nullptr)` block and is
therefore semantically wrong (the original at `12fde0`-`12fde4` stores `nullptr`
there; the timer increment lives in the `else` branch at `12fe34`).

## 6. Follow-up win: gas-cloud `field_0x73` store hoist (raw 6235 -> 6194)

Re-running the rotation sweep (distance 6-12) on the new 6,235 base found a second
size-exact win in the same gas-cloud block: the unconditional
`cloud->field_0x73 = 0;` that closes the block is hoisted to just before
`cloud->field_0x76 = 1;`:

```cpp
             if (this->onScreen) {
                 KIPlayer *locked = this->lockedGasCloud;
+                cloud->field_0x73 = 0;
                 cloud->field_0x76 = 1;
                 this->egoPos = player->getPosition();
                 Globals::Canvas->DrawImage2D(...);
                 this->targetPos = cloud->player->getPosition();
                 if (this->targetPos.x - this->egoPos.x > 24000.0f || ...) {
                     cloud->field_0x73 = 0;
                 }
-                cloud->field_0x73 = 0;
                 if (__builtin_expect(cloud == locked, 1)) { ... }
```

`field_0x73` is write-only between the two points (the `DrawImage2D`, the `targetPos`
store and the far-distance test never read it), so the final value is still 0 and the
reorder is semantically identical.  The neighbouring cells `rotb_994_1005`,
`rotb_996_1005` measure the same 6,194; `rotb_997_1005`/`rotb_999_1005` 6,197.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,235 | **6,194** |
| instruction-index alignment | 1,979 | 1,979 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| `.text` differing bytes (whole `.so`) | 72,825 | **72,784** |
| `.ARM.extab` differing bytes | 601 | 601 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,446 | **73,405** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

Other distance 6-12 rotations on the 6,235 base: `rotb_1199_1210` 6,198 (asteroid
`lockedAsteroid = nullptr` store moved across the `candidateWreck` block),
`rotb_731_744` 6,242, `rotb_701_711` 6,245, `rotb_648_661` 6,252; everything else
>= 6,235 or size-fatal.  Distance 2-5 rotations on the 6,235 base produced no
strictly-better cell (minimum 6,235, several no-op-equivalent).
