# Binary identicality: gap analysis & roadmap

Goal: produce a `libgof2hdaa.so` byte-identical to `../_work/bins/android_2.0.16_libgof2hdaa.so`
(same sha256). This document measures the gap and tracks progress. Measured against the original
(2,187,328 bytes) with our stripped link output.

## Update 2026-11-12 (wave 184)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
15 functions remain non-byte-exact, all size-exact.

Wave 184 runs 20 agents on the worst functions. Residual table so far (raw /
aligned / size; all size-exact):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | **52,132** | 20,658 | 57,232 |
| `Radar::draw` | **6,311** | 1,903 | 9,908 |
| `Layout` C1/C2 | **5,383** | 2,560 | 7,072 |
| `Level::createMission` | **3,588** | 1,140 | 17,172 |
| `MGame::OnTouchEnd` | **2,972** | 1,602 | 8,080 |
| `HangarWindow::render` | **1,512** | 723 | 9,700 |
| `Level::createShip` | **1,213** | 396 | 2,764 |
| `Status::missionCompleted` | **721** | 327 | 1,432 |
| `Gun::calcCharacterCollision` | **458** | 63 | 1,984 |
| `SpaceLounge::drawLounge` | **175** | 41 | 1,948 |
| `Hud::draw` | **32** | 21 | 9,332 |
| `Generator::createMission` | **40** | 21 | 1,996 |
| `ModStation` C1/C2 | **11** | 5 | 1,008 |
| `MenuTouchWindow::OnTouchBegin` | **28** | 16 | 1,896 |
| `MGame::OnUpdate` | **15** | 111 | 11,440 |

Landed changes:

* `LevelScript::process` (w12807): alias/hoist sweep on the current body.
  Raw **52,281 -> 52,271** (-10).
* `LevelScript::process` (w12802): two branch-layout passes (mission-0 state-16
  tail order; shared rocket-control cleanup order). Raw **52,271 -> 52,261**
  (-10). The two workers compose.
* `LevelScript::process` (w12801): strip the case-142 expect. Raw
  **52,261 -> 52,260** (-1).
* `LevelScript::process` (w12804): iOS-structure pass 2 (three composing
  statement-order moves: hideShip after setLookAtCam, dir*3000 after
  forceUpdate, dir*2000 after setPosition). Raw **52,260 -> 52,252** (-8).
* `LevelScript::process` (w12805): K=6..8 + distance-21..40 + 2-opt statement
  moves. Raw **52,252 -> 52,238** (-14).
* `LevelScript::process` (w12802): branch-layout passes 3-4 (12-run
  permutation harvest; re-swept cleanup-block order). Raw **52,238 -> 52,223**
  (-15).
* `LevelScript::process` (w12804): case-42 delete/null order. Raw
  **52,223 -> 52,203** (-20).
* `LevelScript::process` (w12802): branch-layout pass 5 (5-statement-run
  permutation harvest). Raw **52,203 -> 52,190** (-13).
* `LevelScript::process` (w12805): K=6..8 greedy pass 2 on the improved tree.
  Raw **52,190 -> 52,180** (-10).
* `MGame::OnTouchEnd` (w12816): pass 3 moves the campaign-95
  `switch_to_target_setting` store after the HP reads. Raw **3,028 -> 2,997**
  (-31), aligned 1,450 -> 1,459.
* `LevelScript::process` (w12802): branch-layout pass 6 (second 5-run
  harvest). Raw **52,180 -> 52,176** (-4).
* `LevelScript::process` (w12804): case-154 1200-offset vector fix (use
  `getRightVector()` for the 1200 offset) plus three neutral reorders. Raw
  **52,176 -> 52,172** (-4).
* `LevelScript::process` (w12805): K=6..8 pass 3 on the improved tree
  (neutral on the combined body; kept as a verified size-exact reorder).
* `MGame::OnTouchEnd` (w12816): pass 4 moves the campaign-110 armor-HP store
  after the flag stores. Raw **2,997 -> 2,990** (-7), aligned 1,459 -> 1,526.
* `MGame::OnTouchEnd` (w12816): pass 5 moves the campaign-81
  `switch_to_target_setting` store after the HP reads. Raw **2,990 -> 2,985**
  (-5), aligned 1,526 -> 1,590.
* `LevelScript::process` (w12804): three further shape moves (case-91,
  case-154, case-157) plus the case-105 shape move and status-store sink. Raw
  **52,172 -> 52,168** (-4); the chain is size-exact on the combined body.
* `MGame::OnTouchEnd` (w12816): pass 6 swaps the campaign-95 hitpoints/shield
  stores. Raw **2,985 -> 2,981** (-4).
* `LevelScript::process` (w12804): eight shape moves from the offset-7 sweep
  plus three from the offset-8 sweep. Raw **52,168 -> 52,159** (-9).
* `MGame::OnTouchEnd` (w12816): pass 7 swaps the campaign-95 armor/hitpoints
  stores. Raw **2,981 -> 2,976** (-5).
* `LevelScript::process` (w12804): wormhole `setPosition`-before-open fix
  plus six shape moves. Raw **52,159 -> 52,132** (-27).
* `MGame::OnTouchEnd` (w12816): pass 8 hoists `active = 0` in the
  back-to-station tail. Raw **2,976 -> 2,972** (-4), aligned 1,590 -> 1,602.
* `Level::createMission` (w12815): move the `Vector sp` copy ahead of the
  `spawnPosition.z` RNG update in the `extraAliens` spawn block. Raw
  **3,626 -> 3,588** (-38), aligned 1,138 -> 1,140. Scheduling crutch: the
  resulting `coords[2]` is the pre-`z` value; a future byte-exact pass must
  restore the fresh-`z` conversion.
* `Radar::draw` (w12811): commutative hull-bar multiply. Raw **6,315 ->
  6,311** (-4), aligned 1,858 -> 1,903.
* `MGame::OnTouchEnd` (w12816): two store-order passes (instant-action store
  order; shield-HP store after the flag stores). Raw **3,030 -> 3,028** (-2),
  aligned 1,446 -> 1,450.
* `HangarWindow::render` (w12817): flip the ship-guard compound condition to
  `Globals::status->getShip() != this->selectedItem->ship`, which changes the
  live-range/`cmp` form across the `getShip()` call. Raw **1,514 -> 1,512**
  (-2), aligned 680 -> 723. The row-39 LSDA range grows 150 -> 152, so
  `.ARM.extab` content grows 1 byte; total differing bytes still drop
  75,508 -> 75,507 and the section stays size- and base-exact.

Section deltas: `.text` **74,591** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **647** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** differing bytes and
size-exact **269,159 B** (base `0x1c9e10` exact); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.comment` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. Total differing bytes **75,258**. File size is
exact (2,187,328 B), and all **27/27** original section headers are exact.

## Update 2026-11-11 (wave 183)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
15 functions remain non-byte-exact, all size-exact.

Wave 183 runs 20 agents on the worst functions. Residual table so far (raw /
aligned / size; all size-exact):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | **52,281** | 20,700 | 57,232 |
| `Radar::draw` | **6,315** | 1,858 | 9,908 |
| `Layout` C1/C2 | **5,383** | 2,560 | 7,072 |
| `Level::createMission` | **3,626** | 1,138 | 17,172 |
| `MGame::OnTouchEnd` | **3,030** | 1,446 | 8,080 |
| `HangarWindow::render` | **1,514** | 680 | 9,700 |
| `Level::createShip` | **1,213** | 396 | 2,764 |
| `Status::missionCompleted` | **721** | 327 | 1,432 |
| `Gun::calcCharacterCollision` | **458** | 63 | 1,984 |
| `SpaceLounge::drawLounge` | **175** | 41 | 1,948 |
| `Hud::draw` | **32** | 21 | 9,332 |
| `Generator::createMission` | **40** | 21 | 1,996 |
| `ModStation` C1/C2 | **11** | 5 | 1,008 |
| `MenuTouchWindow::OnTouchBegin` | **28** | 16 | 1,896 |
| `MGame::OnUpdate` | **15** | 111 | 11,440 |

Landed changes:

* `LevelScript::process` (w12703 + w12704 + w12707): data-flow cells, the
  iOS-structure case-11 tail reorder + 11-statement sweep + multiply/Vector-add
  operand swaps, and the alias/sub sweep on the current body. Raw
  **52,412 -> 52,320** (-92), aligned 22,050 -> 22,316. w12705's K=4..5
  window (52,397 alone) does not compose and is a superseded note.
* `LevelScript::process` (wave-183 harvest 4): four whole-condition expect cells
  (1448 add 0, 3151 remove 1, 4735 add 1, 5184 add 0). Raw **52,320 -> 52,309**
  (-11), aligned 22,316 -> 22,317. This is the better w12708 variant; its
  committed 52,402 used the 5177 cell instead.
* `LevelScript::process` (wave-183 harvest 5): w12707's size-compensating
  Canvas/Geometry alias pair at the state-8 camera-move tail (hoist
  `m_pGeometry1` and `Globals::Canvas` out of the two `TransformGetTransform`
  calls). Raw **52,309 -> 52,305** (-4), aligned 22,317 -> 22,316.
* `LevelScript::process` (wave-183 harvest 6): w12705's K=4..5 statement
  permutation passes compose with the current body (the earlier 52,398 variant
  did not). Raw **52,305 -> 52,281** (-24), aligned 22,316 -> 20,700.
* `Level::createMission` (w12714): copy `this->spawnPosition` into a local
  `Vector sp` before the `extraAliens` `coords[3]` int conversions, giving the
  three `(int)` conversions one base register. Raw **3,630 -> 3,626** (-4),
  aligned 1,127 -> 1,138.
* `HangarWindow::render` (w12717): buy-list x factored as
  `w/2-(bw+gsx)*(1-col)` and footer `setAlwaysPressed` plain `options[78]`.
  Raw **1,523 -> 1,514** (-9).
* `Radar::draw` (w12711): arithmetic-order/store-scheduling cells. Raw
  **6,322 -> 6,315** (-7), aligned 1,782 -> 1,858.
* `Radar::draw` (w12811): commutative hull-bar multiply operand swap
  (`max * (rate/100)` for `(rate/100) * max`; IEEE-identical). Raw
  **6,315 -> 6,311** (-4), aligned 1,858 -> 1,903.
* `ModStation::ModStation` (w12719): `nearKey` argument swap + fence removal.
  Raw **27 -> 11** (-16), aligned 15 -> 5.
* `MGame::OnTouchEnd` (w12716): hoist the instant-action predicate. Raw
  **3,034 -> 3,030** (-4), aligned 1,416 -> 1,446.
* `Layout::Layout` (w12712): W=4 window-142 rotation. Raw **5,385 -> 5,383**
  (-2).
* `Radar::draw` (w12709): revert the second `lockedEnemy` cleanup order.
  Raw **6,322 -> 6,321** (-1).

Section deltas: `.text` **74,838** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **646** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** differing bytes and
size-exact **269,159 B** (base `0x1c9e10` exact); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.comment` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. Total differing bytes **75,508**. File size is
exact (2,187,328 B), and all **27/27** original section headers are exact.

## Update 2026-11-11 (wave 182)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
15 functions remain non-byte-exact, all size-exact.

Wave 182 runs 20 agents on the worst functions. Residual table so far (raw /
aligned / size; all size-exact):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | **52,412** | 22,050 | 57,232 |
| `Radar::draw` | **6,322** | 1,782 | 9,908 |
| `Layout` C1/C2 | **5,385** | 2,562 | 7,072 |
| `Level::createMission` | **3,626** | 1,138 | 17,172 |
| `MGame::OnTouchEnd` | **3,034** | 1,416 | 8,080 |
| `HangarWindow::render` | **1,523** | 681 | 9,700 |
| `Level::createShip` | **1,213** | 396 | 2,764 |
| `Status::missionCompleted` | **721** | 327 | 1,432 |
| `Gun::calcCharacterCollision` | **458** | 63 | 1,984 |
| `SpaceLounge::drawLounge` | **175** | 41 | 1,948 |
| `Hud::draw` | **32** | 21 | 9,332 |
| `Generator::createMission` | **40** | 21 | 1,996 |
| `ModStation` C1/C2 | **27** | 15 | 1,008 |
| `MenuTouchWindow::OnTouchBegin` | **28** | 16 | 1,896 |
| `MGame::OnUpdate` | **15** | 111 | 11,440 |

Landed changes:

* `LevelScript::process` (w12607 + w12604 + w12605): alias/partial-receiver
  sweep, four data-flow cells, and two rounds of K=1..3 window +
  distance-9..12 sweeps. Raw **52,523 -> 52,412** (-111), aligned
  22,148 -> 22,050.
* `Hud::draw` (w12620): dock-transfer register rotation. Raw **148 -> 32**
  (-116), aligned 147 -> 21, `.ARM.extab` **655 -> 651**.
* `HangarWindow::render` (w12617): free-credits `textY` before `SetColor`,
  buy-list draw before `setSplitText`/`replaceTextKeepSize`, free-credits
  `baseY` dialog-first. Raw **1,594 -> 1,523** (-71), `.ARM.extab`
  **655 -> 647**.
* `Radar::draw` (w12609 + w12610 + w12611): planet-lock plain dock guard +
  sound-guard expect, campaign-145 expect dropped, per-site `radarFont()`
  hoists, enemy-tail `lockedEnemy` order, and the `hasTractor13` if/else
  inversion. Raw **6,339 -> 6,322** (-17), aligned 1,757 -> 1,782.
* `Level::createMission` (w12614 + w12615): case-2 declaration reorder,
  raid-loop `setRotate`/`setSpeed` order, and alien-turret `v`/`cargo`
  order. Raw **3,647 -> 3,630** (-17), aligned 956 -> 1,127.
* `Layout::Layout` (w12612): `orbitInfoSystemY` move. Raw **5,391 -> 5,385**
  (-6), aligned 2,520 -> 2,562.
* `ModStation::ModStation` (w12619): pitch/yaw reinterpret swap. Raw
  **39 -> 27** (-12), aligned 11 -> 15.
* `MGame::OnTouchEnd` (w12616): campaign-chain 15/43 swap. Raw unchanged at
  3,034 but aligned 1,445 -> 1,416 (kept as a shape improvement).

Section deltas: `.text` **75,015** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **646** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** differing bytes and
size-exact **269,159 B** (base `0x1c9e10` exact); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.comment` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. Total differing bytes **75,681**. File size is
exact (2,187,328 B), and all **27/27** original section headers are exact.

## Update 2026-11-11 (wave 181)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
15 functions remain non-byte-exact, all size-exact.

Wave 181 ran 20 agents on the worst functions. Residual table (raw / aligned /
size; all size-exact):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | **52,523** | 22,148 | 57,232 |
| `Radar::draw` | **6,339** | 1,757 | 9,908 |
| `Layout` C1/C2 | **5,391** | 2,520 | 7,072 |
| `Level::createMission` | **3,647** | 956 | 17,172 |
| `MGame::OnTouchEnd` | **3,034** | 1,445 | 8,080 |
| `HangarWindow::render` | **1,594** | 680 | 9,700 |
| `Level::createShip` | **1,213** | 396 | 2,764 |
| `Status::missionCompleted` | **721** | 327 | 1,432 |
| `Gun::calcCharacterCollision` | **458** | 63 | 1,984 |
| `SpaceLounge::drawLounge` | **175** | 41 | 1,948 |
| `Hud::draw` | **148** | 147 | 9,332 |
| `Generator::createMission` | **40** | 21 | 1,996 |
| `ModStation` C1/C2 | **39** | 11 | 1,008 |
| `MenuTouchWindow::OnTouchBegin` | **28** | 16 | 1,896 |
| `MGame::OnUpdate` | **15** | 111 | 11,440 |

Landed changes:

* `LevelScript::process` (w12503): nine successive sweeps -- 70 non-adjacent
  moves, two K=2 passes, and distance-3/4/5/6/7/8 passes (61 more moves).
  Raw **52,818 -> 52,523** (-295), aligned 20,109 -> 22,148.
  w12504's declaration/operand chain (52,764 alone) and w12505's nine
  store-rotation commits (52,652 alone) do not compose with this body and are
  recorded as superseded notes.
* `HangarWindow::render` (w12516): grid/ship draw-order cells, upgrade-block
  permutation, free-credits `visible` if/else, `baseY`/buy-list term order,
  free-credits loop branch hint, `i == 0` cold hint, inlined `Status *` footer
  local, and free-credits width-before-rh. Raw **2,041 -> 1,594** (-447),
  aligned 655 -> 680, `.ARM.extab` **713 -> 709**.
* `Level::createMission` (w12512): six adjacent-statement reorders plus the
  raid-loop hitpoints reorder. Raw **3,682 -> 3,647** (-35), aligned
  1,071 -> 956.
* `Radar::draw` (w12507): planet-lock body order, stop-test operand order,
  two stale `arm_builtin` waiver re-hints, dock-nav arrival guard re-hint,
  landmark-loop store order, and `hasScanner`/`asteroidLockTimer` order.
  Raw **6,379 -> 6,339** (-40), aligned 1,752 -> 1,757.
* `Layout::Layout` (w12510): window permutation plus a head W=4 rotation.
  Raw **5,408 -> 5,391** (-17), aligned 2,438 -> 2,520.
* `Level::createShip` (w12517): hoist `getShipGroup` out of the guard and
  drop the redundant `setLevel` guard. Raw **1,214 -> 1,213** (-1) and
  `.ARM.extab` content **709 -> 655** (-54, size/base unchanged).

Section deltas: `.text` **75,365** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **655** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** differing bytes and
size-exact **269,159 B** (base `0x1c9e10` exact); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.comment` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. Total differing bytes **76,040**. File size is
exact (2,187,328 B), and all **27/27** original section headers are exact.

## Update 2026-11-11 (wave 180)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 180 ran 20 agents on the worst functions. Final residual table (raw /
aligned / size; all size-exact):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | **52,818** | 20,109 | 57,232 |
| `Radar::draw` | **6,379** | 1,752 | 9,908 |
| `Layout` C1/C2 | **5,408** | 2,438 | 7,072 |
| `Level::createMission` | **3,682** | 1,071 | 17,172 |
| `MGame::OnTouchEnd` | **3,034** | 1,445 | 8,080 |
| `HangarWindow::render` | **2,041** | 655 | 9,700 |
| `Level::createShip` | **1,214** | 396 | 2,764 |
| `Status::missionCompleted` | **721** | 327 | 1,432 |
| `Gun::calcCharacterCollision` | **458** | 63 | 1,984 |
| `SpaceLounge::drawLounge` | **175** | 41 | 1,948 |
| `Hud::draw` | **148** | 147 | 9,332 |
| `Generator::createMission` | **40** | 21 | 1,996 |
| `ModStation` C1/C2 | **39** | 11 | 1,008 |
| `MenuTouchWindow::OnTouchBegin` | **28** | 16 | 1,896 |
| `MGame::OnUpdate` | **15** | 111 | 11,440 |

Landed changes:

* `HangarWindow::render` (w12416): three size-exact load-order/local hoists
  (free-credits `rowHeight`, grid-block `rowHeight`/`top`/`gridSpacing`,
  ship-branch `rowHeight`). Raw **2,242 -> 2,041** (-201), aligned
  390 -> 655, `.ARM.extab` content unchanged.
* `Level::createMission` (w12412): seven size-exact store/declaration
  reorders (pirate-base before `createStaticObject`, asteroid position before
  `setAlwaysFriend`, `enemies = new Array` before `attackerCount`, `escorts`
  before `goods`, convoy-tail hitpoints before the speed stores, loop-1 route
  before `setAlwaysEnemy`) plus an explicit `cargo` null check on case-183
  `enemies[8]`. Raw **3,832 -> 3,705** (-127), aligned 977 -> 900,
  `.ARM.extab` **709 -> 713** (+4); net total-byte **-123**.
* `Radar::draw` (w12407 + w12409): landmark-crosshair `left` and gas-spark
  `top` computed as `float` and truncated at the use site (-15), and two
  non-original branch hints dropped from the station-lock blip and
  planet-lock timer tests (-7). Raw **6,407 -> 6,385** (-22), aligned
  1,806 -> 1,754.
* `Level::createShip` (w12417): `empPoints` clamp polarity
  (`level <= 20 ? level : 20`). Raw **1,221 -> 1,214** (-7), aligned
  394 -> 396.
* `LevelScript::process` (w12403): 28 net adjacent-statement moves/rotations
  across the state-machine blocks (five exhaustive adjacent-pair sweeps with
  an independence filter). Raw **52,926 -> 52,819** (-107), aligned
  20,010 -> 20,109.
* `Radar::draw` (w12408): supernova mission-station jingle written as an
  explicit `> 105` if/else (restores the original `ite lt; movwlt 2241;
  movwge 2242` select) and the redundant outer `expect(alwaysEnemySeen, 0)`
  dropped. Raw **6,385 -> 6,379** (-6 on top of w12407/w12409).
* `Level::createMission` (w12413): alien-gun turret-table selector written as
  `bigAlienWave + 2 * (i < 0)` (base+offset form in the loop) and the
  `switch(k)` store order `201 120 012 201`. Raw **3,705 -> 3,682** (-23),
  aligned 900 -> 1,071.
* `MGame::OnTouchEnd` (w12415): `weaponSlot` tail order (-1) and a
  statement-order sweep (-14). Raw **3,049 -> 3,034**, aligned 1,407 -> 1,445.
* `Layout::Layout` (w12410 recovery): statement-order permutation of the
  independent device-constant stores (field_0x34/0x48 after field_0x58;
  oreTextYOffset/field_0xc4/field_0xc8/hudInset/field_0xcc/field_0xbc block;
  field_0x128 before field_0x12c; field_0x110_strip58 after
  field_0x114_strip5c; field_0x16c before field_0x158;
  footerButtonOffset/field_0x144 before rewardBoxHeight; rewardBoxY before
  rewardBoxWidth; helpWindowX before field_0x1f8; rewardBoxY2 block moved to
  the constructor tail). Raw **5,569 -> 5,408** (-161), aligned
  2,434 -> 2,438.
* `LevelScript::process` (w12404): one `__builtin_expect` on the mission-102
  `m_nScriptTimerA >= 10001` guard. Raw **52,819 -> 52,818** (-1).
* `LevelScript::process` (w12406) found a second 26-move greedy reorder
  (raw 52,875 alone) that conflicts with w12403's; the w12403 body measures
  56 bytes better, so w12403 is kept and w12406 is a note-only supersession.

Notes-only (no size-exact raw win): w12411 (Layout 20-sweep, original
per-device branch tree), w12414 (OnTouchEnd dispatch/hasWon/touch-switch),
w12420 (build provenance + Status TBH audit; stock NDK r18b recipe
re-confirmed).

Section deltas: `.text` **76,200** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **713** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** differing bytes and
size-exact **269,159 B** (base `0x1c9e10` exact); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.comment` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. Total differing bytes **76,933**. File size is
exact (2,187,328 B), and all **27/27** original section headers are exact.

## Update 2026-11-11 (wave 179)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 179 ran 20 agents on the worst functions. Final residual table (raw /
aligned / size; all size-exact):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | **52,926** | 20,010 | 57,232 |
| `Radar::draw` | **6,407** | 1,806 | 9,908 |
| `Layout` C1/C2 | **5,569** | 2,434 | 7,072 |
| `Level::createMission` | **3,832** | 977 | 17,172 |
| `MGame::OnTouchEnd` | **3,049** | 1,407 | 8,080 |
| `HangarWindow::render` | **2,242** | 390 | 9,700 |
| `Level::createShip` | **1,221** | 394 | 2,764 |
| `Status::missionCompleted` | **721** | 327 | 1,432 |
| `Gun::calcCharacterCollision` | **458** | 63 | 1,984 |
| `SpaceLounge::drawLounge` | **175** | 41 | 1,948 |
| `Hud::draw` | **148** | 147 | 9,332 |
| `Generator::createMission` | **40** | 21 | 1,996 |
| `ModStation` C1/C2 | **39** | 11 | 1,008 |
| `MenuTouchWindow::OnTouchBegin` | **28** | 16 | 1,896 |
| `MGame::OnUpdate` | **15** | 111 | 11,440 |

Landed changes:

* `Level::createMission` (w12312): replace the compound cargo/escort test with
  explicit null checks. Raw **4,467 -> 3,844** (-623), `.ARM.extab`
  **700 -> 694**. (w12313 then reorders the turret/convoy stores:
  **3,844 -> 3,832**.)
* `Radar::draw` (w12307 + w12308): w12307 is a superset of w12309 (barMax sink,
  `onPlanetCourse` removal, crosshair operand order, cargo-flag weight, tractor
  and cloud order) and reaches **6,458 -> 6,418**; w12308's `otherLock`
  reorder adds **-11** for a combined **6,407**. w12309 (6,424) and w12308
  alone (6,439) are superseded.
* `LevelScript::process` (w12302 + w12305): four block-scope receiver aliases
  plus the declaration-order sweep. Combined raw **53,082 -> 53,039**
  (w12302 alone 53,060, w12305 alone 53,061).
* `Layout::Layout` (w12311): move `field_0x2c_rowHeight` after `field_0x38`.
  Raw **5,576 -> 5,569**.
* `MGame::OnTouchEnd` (w12315 + w12314): `MenuTouchWindow::OnTouchEnd` returns
  `bool`, then the w12314 chain-unit permutation and campaign expect.
  Combined raw **3,261 -> 3,049** (w12315 alone 3,259; w12314 on the base
  3,051). Size 8,080 exact; `.ARM.extab` size exact but content diff
  **694 -> 709**.
* `LevelScript::process` (w12303, recovered after timeout): greedy statement
  reorder. Combined with w12302/w12305, raw **53,039 -> 52,926**
  (w12303 alone 52,961). Size 57,232 exact.
* `Level::createShip` (w12317): fighter LOD branch layout.
  Raw **1,225 -> 1,221**.
* `HangarWindow::render` (w12316): draw-first upgrade block.
  Raw **2,243 -> 2,242**.
* `Hud::draw` (w12319): image-id local reaches raw **147**, but
  `.ARM.extab` rises **700 -> 703** (net +2 bytes), so the code is reverted;
  the note is kept.
* Note-only: w12318 (Status chain-fold weights are post-MBP-canonicalised),
  w12320 (build provenance / Status TBH audit).

Section deltas: `.text` **76,870** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **709** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** differing bytes and
size-exact **269,159 B** (base `0x1c9e10` exact); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.comment` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. Total differing bytes **77,598**. File size is
exact (2,187,328 B), and all **27/27** original section headers are exact.

## Update 2026-11-11 (wave 178)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 178 ran 20 agents on the worst functions. Final residual table (raw /
aligned / size; all size-exact):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | **53,082** | 20,149 | 57,232 |
| `Radar::draw` | **6,458** | 1,832 | 9,908 |
| `Layout` C1/C2 | **5,576** | 2,410 | 7,072 |
| `Level::createMission` | **4,467** | 952 | 17,172 |
| `MGame::OnTouchEnd` | **3,261** | 1,185 | 8,080 |
| `HangarWindow::render` | **2,243** | 384 | 9,700 |
| `Level::createShip` | **1,225** | 383 | 2,764 |
| `Status::missionCompleted` | **721** | 327 | 1,432 |
| `Gun::calcCharacterCollision` | **458** | 63 | 1,984 |
| `SpaceLounge::drawLounge` | **175** | 41 | 1,948 |
| `Hud::draw` | **148** | 147 | 9,332 |
| `Generator::createMission` | **40** | 21 | 1,996 |
| `ModStation` C1/C2 | **39** | 11 | 1,008 |
| `MenuTouchWindow::OnTouchBegin` | **28** | 16 | 1,896 |
| `MGame::OnUpdate` | **15** | 111 | 11,440 |

Landed changes:

* `LevelScript::process` (w12203, recovered): pure statement reorder of the
  state machine. Raw **53,507 -> 53,082** (-425), size 57,232 exact. This
  supersedes the w12201 partial reorder (53,488).
* `Radar::draw` (w12207, recovered): whole-function candidate combining the
  mission-empty, landmark-null, crosshair, friend-cargo and enemy
  `field_0x24` expects with a `barMax` local. Raw **6,576 -> 6,458** (-118),
  size 9,908 exact. Supersedes w12208 (6,586) and w12209 (6,626).
* `Level::createMission` (w12213): resolve the `extraAliens` +4 B blocker.
  Raw **5,143 -> 4,467** (-676), `.ARM.extab` **731 -> 700** (-31).
* `MGame::OnTouchEnd` (w12215 + w12214): campaign-chain order and the `== 1`
  menuWindow branch weight. Combined raw **3,267 -> 3,261** (-6).
* `Layout::Layout` (w12211): statement-unit order. Raw **5,604 -> 5,576**
  (-28), size 7,072 exact.
* `HangarWindow::render` (w12216): row-39 LSDA now exact; raw
  **2,244 -> 2,243**.
* `HangarWindow::render` (w12316): the four-call upgrade-button block is
  laid out draw-first; a one-byte raw coincidence. Raw **2,243 -> 2,242**,
  aligned 384 -> 390, size 9,700 exact, row-39 `(2508,150,9664)` and the
  948 B `.ARM.extab` record diff 90 unchanged.
* `MGame::OnUpdate` (w12220): raw **16 -> 15** (aligned 111, size 11,440
  exact).
* `Status::missionCompleted` (w12218b): remove the hardcoded rank table and
  its separate last-linked TU; switch directly on `m->getType()` with the
  case bodies in the original hot-then-cold order. This removes the
  manual address `0x20b977`, restores an exact-size `.rodata` (269,159 B) and
  all **27/27** original section headers. The plain switch cannot make clang 7's
  `MachineBlockPlacement` lay the bodies out hot-then-cold, so raw is **721**
  (the hardcoded-table variant reached 449 but is not human C++ and is
  superseded). Size 1,432 B exact.
* Note-only: w12206 (LevelScript dispatch/pool), w12212 (createMission
  bool/order sweep), w12217 (createShip order/branch sweep), w12219 (Hud
  anchor; the -1 B code was reverted because it cost +2 total bytes).

Section deltas: `.text` **77,935** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **700** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** differing bytes and
size-exact **269,159 B** (base `0x1c9e10` exact); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.comment` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. Total differing bytes **78,655**. File size is
exact (2,187,328 B), and all **27/27** original section headers are exact.

## Update 2026-11-11 (wave 177)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 177 ran 20 agents on the worst functions. Landed changes:

* `LevelScript::process` (w12102): re-test the 10-spec alias set and add the
  state-3 camera alias. Raw **53,568 -> 53,528**.
* `LevelScript::process` (w12103): reorder 15 independent state-machine
  statements. Raw **53,568 -> 53,545**; combined with w12102 **53,508**.
* `LevelScript::process` (w12104): rare-guard sub-expression expects. One
  cell (`m_nFlashCountdown > 0`, `expect(..., 1)`) survives on the combined
  tree; the `isOver()` cell is size-changing (+8 B) with the w12102 alias and
  was rejected. Combined LevelScript **53,507**.
* `LevelScript::process` (w12105): case-80 pirate declaration order. Raw
  **53,568 -> 53,564**.
* `Radar::draw` (w12108): planet-lock guard `expect(..., 1)` plus reverts of
  the wave-176 `alwaysEnemySeen` and mission-145 expects. Raw
  **6,713 -> 6,659**.
* `Radar::draw` (w12107, recovered after timeout): landmarks `expect(..., 0)`,
  `enemyFlagsLo` `expect(..., 1)`, and reverts of the cargo and
  `inAlienOrbit` expects. Raw **6,659 -> 6,636**. (w12109's two-expect
  variant, 6,662, is superseded.)
* `Radar::draw` (w12209, wave 178): five size-exact operand-order cells
  (`otherLock` recompute, enemy `inSight` y-operands, asteroid crosshair
  bounds, enemy tractor label, asteroid tractor label). Raw
  **6,636 -> 6,626**; aligned 1,813 -> 1,827.
* `Layout::Layout` (w12110, recovered after timeout): swap statement units
  72/77 (`field_0x2a8_metricB` / `field_0xf0`). Raw **5,611 -> 5,604**.
* `Level::createMission` (w12112): `alienGuns` branch instead of multiply,
  single-compare alien-gun loop, tail reload. Raw **5,257 -> 5,154**.
* `Level::createMission` (w12113): permute the inner `switch (k)` store order.
  Raw **5,257 -> 5,246**; combined with w12112 **5,143**.
* `MGame::OnTouchEnd` (w12114): campaign-chain order. Raw
  **3,621 -> 3,298**.
* `MGame::OnTouchEnd` (w12115): `== 1` menuWindow branch weight. Raw
  **3,621 -> 3,581**; combined with w12114 **3,267**.
* `HangarWindow::render` (w12116): recover the row-39 LSDA exactly
  (`(2508, 150, 9664)`). Raw moves **1,965 -> 2,244** but the `.ARM.extab`
  record diff falls **676 -> 90** and the section diff **1,321 -> 731**;
  size and base stay exact. Net total is -307 bytes on its base.
* `Level::createShip` (w12117): `setBV` after the fixed-branch tail stores.
  Raw **1,229 -> 1,225**.
* `Hud::draw` (w12119): reverse-flag anchor. Raw **150 -> 148**.

Notes-only (no code): w12101 (recovered cells; the `resetCamera` cell is
size-changing and was rejected), w12106 (dispatch/pool/data-flow), w12108
(also a note), w12109 (superseded), w12111 (Layout ternaries/tail), w12118
(Status), w12120 (small-tail oracle).

**Aligned total (15 functions): 26,615** (wave-176 base 26,033). Per
function: LevelScript 18,941, Radar 1,827, Layout 2,334, createMission 952,
MGame::OnTouchEnd 1,164, HangarWindow 382, createShip 383, Status 327,
Gun 63, SpaceLounge 41, Hud 147, Generator 21, MenuTouchWindow 16,
ModStation 11, MGame::OnUpdate 6. Total differing bytes fall
**80,932 -> 79,992** (-940): `.text` **79,591 -> 79,241** (-350) and
`.ARM.extab` **1,321 -> 731** (-590), plus `.note.gnu.build-id` 20.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,507** (57,232 B), `Radar::draw` **6,626** (9,908 B), `Layout` C1/C2
**5,604** (7,072 B), `Level::createMission` **5,143** (17,172 B),
`MGame::OnTouchEnd` **3,267** (8,080 B), `HangarWindow::render` **2,244**
(9,700 B), `Level::createShip` **1,225** (2,764 B), `Status::missionCompleted`
**721** (1,432 B), `Gun::calcCharacterCollision` **458** (1,984 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `Hud::draw` **148** (9,332 B),
`Generator::createMission` **40** (1,996 B), `ModStation` C1/C2 **39**
(1,008 B), `MenuTouchWindow::OnTouchBegin` **28** (1,896 B),
`MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **79,241** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **731** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** (byte-identical);
**`.rel.plt` 0 (byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 176)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 176 ran 20 agents on the same worst functions. Landed changes:

* `Radar::draw` (w12006): four adjacent-statement reorderings around the
  planet-lock dock guard and the wreck/onScreen/tractor path. Raw
  **6,785 -> 6,758** (-27); the planet-block reorder (`S,F,D` -> `F,D,S`)
  puts the `play(26)` sound guard at `0x1303d2`, matching the original's
  station sound guard. See `_work/reconstructions/radar-draw-wave176-w12006.md`.
* `Radar::draw` (w12007): cargo-null cold arm plus two music-tail hints.
  Raw **6,785 -> 6,776**.
* `Radar::draw` (w12005): four block-scope load hoists plus two `expect`
  hints. Raw **6,785 -> 6,689** alone; combined with w12006 the tree
  measures **6,713** (the two passes interact).
* `LevelScript::process` (w12003): drop two redundant `expect`s. Raw
  **53,593 -> 53,581**.
* `LevelScript::process` (w12004): sub-expression `expect` pass. Raw
  **53,593 -> 53,580**.
* `LevelScript::process` (w12001 subset): only the two `expect` edits that
  survive the combined tree (L1117, L2130) were kept as `b7f5a4d22`; the
  full five-expect commit regressed size on the combined tree and was
  reverted. Combined LevelScript is **53,568**.
* `Level::createMission` (w12011): hoist `rnd` before the hunter-count block.
  Raw **5,285 -> 5,279**.
* `Level::createMission` (w12010): `alienGuns` bool multiply. Raw
  **5,285 -> 5,263**; combined with w12011 the tree measures **5,257**.
* `MGame::OnTouchEnd` (w12012): bool `hasWon`, campaign-first dispatch and
  chain reorder. Raw **3,771 -> 3,621**.
* `MGame::OnTouchEnd` (w12013): `weaponSlot` tail reorder. Raw
  **3,771 -> 3,770**.
* `HangarWindow::render` (w12014): original ship-guard order. Raw
  **2,242 -> 1,965**; its `.ARM.extab` record is no longer byte-exact
  (row 39 LSDA length 150 -> 118), so the section diff rises
  **746 -> 1,321** while size and base stay exact.
* `Level::createShip` (w12015): jitter declaration order. Raw
  **1,232 -> 1,229**.
* `Status::missionCompleted` (w12016): case-158 local order. Raw **721**
  unchanged, aligned **329 -> 327**.
* `Gun::calcCharacterCollision` (w12017): fold the magnitude-distance
  expression into a local. Raw **463 -> 458**.
* `Hud::draw` (w12019): address-store anchor. Raw **156 -> 150**.
* `Layout::Layout` (w12009): store-order permutation of the 204 straight-line
  statement units. Raw **5,620 -> 5,611**, size exact. See
  `_work/reconstructions/layout-ctor-store-order-wave176-w12009.md`.
* `LevelScript::process` (w12002): greedy block-scope alias set. It measured
  **53,542** on the wave-175 base but is **neutral (53,568)** on the combined
  tree, and the committed diff contained only the `enemiesC` alias; the merge
  was reverted. Note kept at
  `_work/reconstructions/levelscript-process-alias-greedy-wave176-w12002.md`.

Notes-only (no code): w12008 (Layout sweep), w12018 (SpaceLounge),
w12020 (small tails).

**Aligned total (15 functions): 26,022** (wave-175 base 26,525). Per
function: LevelScript 18,498, Layout 2,334, Radar 1,734, MGame::OnTouchEnd
1,194, createMission 963, createShip 380, Status 327, HangarWindow 291,
Hud 143, Gun 63, SpaceLounge 41, Generator 21, MenuTouchWindow 16,
ModStation 11, MGame::OnUpdate 6. Total differing bytes fall **80,932 -> 80,892**:
`.text` falls **80,166 -> 79,551** (-615) while `.ARM.extab` rises
**746 -> 1,321** (+575), so the net is -40.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,568** (57,232 B), `Radar::draw` **6,713** (9,908 B), `Layout` C1/C2
**5,611** (7,072 B), `Level::createMission` **5,257** (17,172 B),
`MGame::OnTouchEnd` **3,581** (8,080 B), `HangarWindow::render` **1,965**
(9,700 B), `Level::createShip` **1,229** (2,764 B), `Status::missionCompleted`
**721** (1,432 B), `Gun::calcCharacterCollision` **458** (1,984 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `Hud::draw` **150** (9,332 B),
`Generator::createMission` **40** (1,996 B), `ModStation` C1/C2 **39**
(1,008 B), `MenuTouchWindow::OnTouchBegin` **28** (1,896 B),
`MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **79,551** differing bytes (size exact, 1,259,464 B);
`.ARM.exidx` **0** (byte-identical); `.ARM.extab` **1,321** (size- and
base-exact **156,312 B** @ `0x1a3b78`); `.rodata` **0** (byte-identical);
**`.rel.plt` 0 (byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 175)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 175 ran 20 agents on the same worst functions. Landed changes:

* `Radar::draw` (w11906): the skip-branch shipGroup re-test is restored and
  the wreck flag is a bool. Raw **6,817 -> 6,804**.
* `Radar::draw` (w11907): the wave-174 planet-lock `expect(!lockConfirmed, 0)`
  is removed (restoring the original inline `cbnz` sound body) and the +8 B
  landing-pad site moves to the rare `field_0x1a8` dock guard; the planet-lock
  guard uses `|` instead of `||` and the planet sprite local is assigned in
  both arms. Raw **6,804 -> 6,785**; 9,908 B exact, `.rel.plt`
  byte-identical. See `_work/reconstructions/radar-draw-wave175-w11907.md`.
* `Status::missionCompleted` (w11918): case-158 order. Raw **721** unchanged,
  aligned 333 -> **329**; 1,432 B exact.
* `Hud::draw` (w11917): cache `top * 2` as `y`. Raw **168 -> 156**, aligned
  146 -> **145**.
* `Layout::Layout` (w11911): `field_0x17c` stored before `field_0x158`. Raw
  **5,633 -> 5,624**.
* `Layout::Layout` (w11910): tail store order `0x214 <-> 0x200`. Raw
  **5,624 -> 5,620**.
* `LevelScript::process` (w11904): additive block-scope aliases. Raw
  **53,673 -> 53,651**.
* `Level::createMission` (w11914): faithful bool/order plus globals/rnd/enemies
  hoists. Raw **5,421 -> 5,285**; `.ARM.extab` 750 -> **748**.
* `LevelScript::process` (w11901): five `expect` hints. Raw **53,651 ->
  53,593** on the combined tree.

Notes-only (no code): w11905, w11902, w11903, w11908, w11909, w11913,
w11915, w11916, w11919, w11920. Code that interacted badly with other merged
wins was reverted and kept as notes: w11908 (Radar expect+alias), w11915
(createShip try/catch, `.ARM.extab` 8 B short), w11902 and w11903
(LevelScript).

**Aligned total (15 functions): 26,502** (wave-174 base 26,185). Per
function: Layout 2,289, LevelScript 18,476, createShip 422, Status 329,
Radar 1,657, MGame::OnTouchEnd 1,453, createMission 958, Gun 57,
HangarWindow 621, SpaceLounge 41, Hud 145, Generator 21, MenuTouchWindow 16,
ModStation 11, MGame::OnUpdate 6. Total differing bytes fall
**81,209 -> 80,932** (-277) and `.text` **80,439 -> 80,166** (-273);
`.ARM.extab` **750 -> 746** (size- and base-exact).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,593** (57,232 B), `Radar::draw` **6,785** (9,908 B), `Layout` C1/C2
**5,620** (7,072 B), `Level::createMission` **5,285** (17,172 B),
`MGame::OnTouchEnd` **3,771** (8,080 B), `HangarWindow::render` **2,242**
(9,700 B), `Level::createShip` **1,232** (2,764 B), `Hud::draw` **156**
(9,332 B), `Status::missionCompleted` **721** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **40** (1,996 B),
`ModStation` C1/C2 **39** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **80,166** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **746** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 174)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 174 ran 20 agents on the same worst functions. Landed changes:

* `LevelScript::process` (w11906): the shared-tail deactivation block drops
  the `mission >= 2` expect hint (restoring the original's separate
  `cmp`/`blt` shape) and uses a block-scoped `Level *lvl = m_pLevel` alias
  for the two `getPlayer()` calls. Raw **53,711 -> 53,678** (-33),
  aligned 18,228 -> 18,230; 57,232 B exact, `.rel.plt` byte-identical.
* `LevelScript::process` (w11904): state-5 `self` and state-6 galaxy aliases.
  On top of w11906 this takes raw **53,678 -> 53,673** (-5), aligned
  18,230 -> 18,226; 57,232 B exact. w11902's state-8 duplicate removal +
  branch-weight companion was measured and reverted: it is a win on the
  wave-173 base but interacts with w11906 (raw 53,861), so only its note
  is kept.
* `Radar::draw` (w11908): the lock-image chain tests
  `carriesFriendCargoFlag` before `enemyFlagsLo` (friend-first), and the
  off-screen "far plain" image chain is emitted else-branch first, friend
  second, enemy last (permutation 210). Raw **6,849 -> 6,833** (-16),
  aligned 1,577 -> 1,611; 9,908 B exact, `.rel.plt` byte-identical. The
  reorder is semantics-preserving because the two flags are mutually
  exclusive (`Standing::isEnemy`/`isFriend` are disjoint and every writer
  sets at most one). See
  `_work/reconstructions/radar-draw-wave174-w11908.md`.
* `Radar::draw` (w11909): frame-blip locals, the planet-lock
  `__builtin_expect(!lockConfirmed, 0)` hint, and `int missionBlocksPlanets`.
  On top of w11908 this takes raw **6,833 -> 6,817** (-16), aligned
  1,611 -> 1,602; 9,908 B exact, `.rel.plt` byte-identical.
* `Level::createShip` (w11915): a cleanup-only 336 B LSDA (57 rows,
  `ttype 0xff`) with an `EmpGuard` RAII barrier around `setEmpData` replaces
  the wave-173 catch-all record; the invoke barrier keeps the coordinate
  block above the call. Raw **1,560 -> 1,232** (-328), aligned 530 -> 422;
  2,764 B exact. `.ARM.extab` stays size- and base-exact (156,312 B at
  `0x1a3b78`); its differing bytes rise 675 -> 749.
* `Hud::draw` (w11917): dock-block type/order shape (armor-regen pivotY 0,
  `short frameH`/`short textH`, `barH` before `barW`, `top`/`barYOffset`
  before the fade math). Raw **223 -> 168** (-55), aligned 183 -> 146;
  9,332 B exact.
* `HangarWindow::render` (w11916): ship-compare operand flip
  (`Globals::status->getShip() == this->selectedItem->ship`). Raw
  **2,244 -> 2,242** (-2), aligned 382 -> 621; 9,700 B exact. The aligned
  regression is the one extra `ldr`/`ldr` pair; the `.ARM.extab` record
  moves 90 -> 91, still under the record gate.
* `MGame::OnTouchEnd` (w11919): `hudTouchFlags == 0` body store order.
  Raw **3,773 -> 3,771** (-2), aligned 1,453 unchanged; 8,080 B exact.
* `Status::missionCompleted` (w11918): the single `switch (m->getType())`
  is split into an outer switch for the low/typed cases and a nested
  `default:` switch for the 150+ cases. Raw **722 -> 721** (-1), aligned
  320 -> 333; 1,432 B exact.  Wave 175 (w11918): the nested switch's
  `case 158` moves from last to after `case 162` (raw unchanged **721**,
  aligned 333 -> **329**); the grouped-weight source producer remains
  unfound (see `_work/reconstructions/status-432-producers-wave175-w11918.md`).

**Aligned total (15 functions): 26,181** (wave-173 base 26,055). Per
function: Layout 2,290, LevelScript 18,226, createShip 422, Status 329,
Radar 1,602, MGame::OnTouchEnd 1,453, createMission 940, Gun 57,
HangarWindow 621, SpaceLounge 41, Hud 146, Generator 21, MenuTouchWindow 16,
ModStation 11, MGame::OnUpdate 6. Total differing bytes fall
**81,592 -> 81,209** (-383) and `.text` **80,897 -> 80,439** (-458);
`.ARM.extab` rises **675 -> 750** (+75) but remains size- and base-exact.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,673** (57,232 B), `Radar::draw` **6,817** (9,908 B), `Layout` C1/C2
**5,633** (7,072 B), `Level::createMission` **5,421** (17,172 B),
`MGame::OnTouchEnd` **3,771** (8,080 B), `HangarWindow::render` **2,242**
(9,700 B), `Level::createShip` **1,232** (2,764 B), `Hud::draw` **168**
(9,332 B), `Status::missionCompleted` **721** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **40** (1,996 B),
`ModStation` C1/C2 **39** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **80,439** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **750** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 173)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 173 ran 20 agents on the same worst functions. Landed changes:

* `LevelScript::process` (w11804 + w11805): five block aliases plus the
  mission-42/state-10 `Level *self` aliases. Raw **53,750 -> 53,711** (-39),
  aligned 18,049 -> 18,228; 57,232 B exact, `.rel.plt` byte-identical.
* `Radar::draw` (w11809): planet-lock `||` operand order plus the frame-test
  expect. Raw **7,054 -> 6,849** (-205), aligned 1,582 -> 1,577; 9,908 B
  exact.
* `Layout::Layout` (w11812): store-order reorder of the `0x170..0x17c`,
  `0x284..0x300` and `choiceWindowTitleHeight` groups. Raw **5,640 -> 5,633**
  (-7), aligned 2,282 -> 2,290; 7,072 B exact.
* `Level::createMission` (w11814): `pirateFortress` loop start/bound operand
  order plus `cargo`/`shipGroup` store order. Raw **5,425 -> 5,421** (-4),
  aligned 936 -> 940; 17,172 B exact.
* `HangarWindow::render` (w11816): nested ship-guard branch weight. Raw
  **2,266 -> 2,244** (-22), aligned 386 -> 382, LSDA record 93 -> 90;
  9,700 B exact.
* `Hud::draw` (w11818): show-block declaration order, `top`/`barYOffset`
  loads before the fade math, and inlined `filled`. Raw **548 -> 223**
  (-325), aligned 220 -> 183; 9,332 B exact.
* `ModStation` C1/C2 (w11820): compiler barrier before the nearKey
  table-pointer setup. Raw **43 -> 39** (-4), aligned 12 -> 11; 1,008 B
  exact.

Measured and **not landed** (notes kept): w11801 (1,240-cell expect sweep;
one -1 cell, dropped because it interacts with w11805: 57,232 -> 57,240),
w11802 (state-8 duplicate removal: call parity exact 23/41 but raw 54,376),
w11803 (first literal pool move: `.text` 82,884), w11806/w11807/w11808/
w11810/w11811/w11815/w11819 notes, w11817 (`createShip` exact prologue via
the `setEmpData` invoke barrier: raw 1,199 but `.ARM.extab` 8 B short),
w11813 (`createMission` bool/order: size-fatal 17,184).

**Aligned total (15 functions): 26,018** (wave-172 base 25,911). Per
function: Layout 2,290, LevelScript 18,228, createShip 530, Status 320,
Radar 1,577, MGame::OnTouchEnd 1,453, createMission 940, Gun 57,
HangarWindow 382, SpaceLounge 41, Hud 146, Generator 21, MenuTouchWindow 16,
ModStation 11, MGame::OnUpdate 6. Total differing bytes fall
**82,234 -> 81,540** (-694) and `.text` **81,503 -> 80,842** (-661);
`.ARM.extab` **711 -> 678** (-33).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,711** (57,232 B), `Radar::draw` **6,849** (9,908 B), `Layout` C1/C2
**5,633** (7,072 B), `Level::createMission` **5,421** (17,172 B),
`MGame::OnTouchEnd` **3,773** (8,080 B), `HangarWindow::render` **2,244**
(9,700 B), `Level::createShip` **1,560** (2,764 B), `Hud::draw` **168**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **40** (1,996 B),
`ModStation` C1/C2 **39** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **80,842** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **678** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 172)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 172 ran 20 agents on the same worst functions (with `-j2`, a 12 GB
`ulimit -v`, and disk-backed scratch). Landed changes:

* `LevelScript::process` (w11704 + w11701 + w11705): re-land the 11
  non-regressing block aliases, add 3 `m_pCamera` aliases and the case-70
  `Hud` alias, add the mission-95/mission-142 expects, and keep the
  raw-neutral mission-78 fade order. Raw **53,812 -> 53,750** (-62),
  aligned 17,932 -> 18,049; 57,232 B exact, `.rel.plt` byte-identical.
* `Radar::draw` (w11707): outer planet-lock expect plus the campaign-125
  freighter status-value expect. Raw **7,061 -> 7,054** (-7), aligned
  1,561 -> 1,582; 9,908 B exact.
* `MGame::OnTouchEnd` (w11713): ore-refine guard `count >= 0`. Raw
  **3,775 -> 3,773** (-2), aligned 1,454 -> 1,453; 8,080 B exact.
* `Generator::createMission` (w11718): volatile rem copy plus the
  `rawBonus` anchor. Raw **127 -> 40** (-87), aligned 107 -> 21;
  1,996 B exact.
* `Level::createShip` (w11715): mission guard as the original's
  switch/bitmask over `{49,50,51,52,56}`. Raw neutral **1,560**, aligned
  455 -> 530, 2,764 B exact; the original uses the bitmask form.

Measured and **not landed** (notes kept): w11703 (state-8 duplicate
`setPosition` removal + shared tail: 54,371 raw when combined with the
landed aliases), w11705 (mission-78 tail split), w11706 (outer dispatch),
w11708 (Radar condition-order sweep), w11709/w11710 (Layout store map),
w11711/w11712 (createMission bool/type sweep), w11714 (HangarWindow
row-39), w11716 (Hud declaration order), w11717 (Status 4/3/2 producer),
w11719 (small tails), w11720 (round-12 extab/flags census).

**Aligned total (15 functions): 25,911** (wave-171 base 25,785). Per
function: Layout 2,282, LevelScript 18,049 (indicative), createShip 530,
Status 320, Radar 1,582, MGame::OnTouchEnd 1,453, createMission 936, Gun 57,
HangarWindow 386, SpaceLounge 41, Hud 220, Generator 21, MenuTouchWindow 16,
ModStation 12, MGame::OnUpdate 6.  Total differing bytes fall
**82,392 -> 82,234** (-158) and `.text` **81,661 -> 81,503** (-158);
`.ARM.extab` stays **711**.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,750** (57,232 B), `Radar::draw` **7,054** (9,908 B), `Layout` C1/C2
**5,640** (7,072 B), `Level::createMission` **5,425** (17,172 B),
`MGame::OnTouchEnd` **3,773** (8,080 B), `HangarWindow::render` **2,266**
(9,700 B), `Level::createShip` **1,560** (2,764 B), `Hud::draw` **548**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **40** (1,996 B),
`ModStation` C1/C2 **43** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **81,503** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **711** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 171)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 171 ran 20 agents on the same worst functions.  Landed changes:

* `LevelScript::process` (w11601): 1,566-cell expect / existing-expect /
  operand-order sweep (mission-0 state-1 timer 6001, 3 cold-branch hints,
  4 existing-expect flips/strips, mission-42 state-2 `&&` swap).  Raw
  **53,932 -> 53,812** (-120), aligned 17,900 -> 17,932; 57,232 B exact,
  `.rel.plt` byte-identical.
* `Radar::draw` (w11607+w11608+w11620): lock-landmark frame-test and enemy
  image-chain expects, planet-lock draw as the original guard clause, gas
  loop index declared in the `for`, and the per-TU
  `-Xclang -target-feature -Xclang +cheap-predicable-cpsr` knob.  Raw
  **7,095 -> 7,061** (-34), aligned 1,595 -> 1,561; 9,908 B exact.
* `Level::createMission` (w11612): drop the cached `Status*` local in the
  raid block.  Raw **5,435 -> 5,425** (-10), aligned 767 -> 936;
  17,172 B exact.

Measured and **not landed** (notes kept): w11602 (case-8 duplicate removal,
size-fatal), w11603 (state-8 duplicate `setPosition` + shared tail: raw
54,462, `.rel.plt` rotates), w11604 (alias set interacts with w11601:
53,812 -> 54,006), w11605 (case-11 translate arg, raw +12), w11606
(wingman-loop expect removal, raw +15), w11609 (Layout ascending 0x34..0x44,
raw +42), w11610 (Layout tail dispatch), w11611 (createMission bool lead,
raw 6,769), w11614 (HangarWindow row-39/raw matrix), w11615 (createShip
switch/bitmask, aligned worse), w11616 (Hud transfer block), w11617 (Status
4/3/2, IR reaches 637/59), w11618 (Generator volatile b: raw 163), w11619
(ModStation/MenuTouchWindow/MGame re-sweep).

**Aligned total (15 functions): 25,785** (wave-164 base 24,608). Per
function: Layout 2,282, LevelScript 17,932 (indicative), createShip 455,
Status 320, Radar 1,561, MGame::OnTouchEnd 1,454, createMission 936, Gun 57,
HangarWindow 386, SpaceLounge 41, Hud 220, Generator 107, MenuTouchWindow 16,
ModStation 12, MGame::OnUpdate 6.  Total differing bytes fall
**85,397 -> 82,392** (-3,005) and `.text` **84,652 -> 81,661** (-2,991);
`.ARM.extab` **725 -> 711** (-14).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,812** (57,232 B), `Radar::draw` **7,061** (9,908 B), `Layout` C1/C2
**5,640** (7,072 B), `Level::createMission` **5,425** (17,172 B),
`MGame::OnTouchEnd` **3,775** (8,080 B), `HangarWindow::render` **2,266**
(9,700 B), `Level::createShip` **1,560** (2,764 B), `Hud::draw` **548**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **127** (1,996 B),
`ModStation` C1/C2 **43** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **81,661** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **711** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 170)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 170 ran 20 agents on the same worst functions.  Landed changes:

* `LevelScript::process` (w11502): case-12 `setPosition(Vector)` overload,
  case-8 state advance + music-stop order, mission-89 case-3 order.  Raw
  **53,970 -> 53,947** (-23), aligned 17,818 -> 17,820.
* `LevelScript::process` (w11503): mission-0 case-11 timer add, drop the
  spurious `enableFillScreen`, state-16 block order and
  `initStreamOutPosition`.  Raw **53,947 -> 53,893** (-54), aligned 17,820.
* `LevelScript::process` (w11501): expect/swap sweep (state-5 timer,
  mission-2 msg1, mission-42 state-7 docked, hacking gate, wormhole gate,
  mission-50 dispatch).  Raw **53,893 -> 53,876** (-17), aligned 17,820 ->
  17,871.
* `LevelScript::process` (w11504): block-scope `m_pLevel`/`m_pGeometry*`/
  `m_pCamera` aliases.  Raw **53,876 -> 53,932** net after the state-7
  interaction fix (see below); the state-7 alias is kept block-scoped
  because hoisting it with the w11501 expect shrinks the function to
  57,224 B.
* `Radar::draw` (w11507): three more sound-ladder / tractor expects.  Raw
  **7,127 -> 7,095** (-32), aligned 1,606 -> 1,595; 9,908 B exact.
* `Layout` C1/C2 (w11510+w11509): `field_0x290` after
  `choiceWindowTitleHeight`, `field_0x284_sliderSlot5Enabled` before
  `field_0x94`, `field_0x2d0` after `field_0x2c8`.  Raw **5,650 -> 5,640**
  (-10), aligned 2,284 -> 2,282; 7,072 B exact.
* `Hud::draw` (w11516): barH-first show-block order and bar-offset size
  anchor.  Raw **582 -> 548** (-34), aligned 228 -> 220; 9,332 B exact.
* `HangarWindow::render` (w11514): ship-branch cleanup scope restores the
  row-39 LSDA shape.  Raw **1,970 -> 2,266** (+296), aligned 294 -> 386,
  but the `.ARM.extab` record diff falls **676 -> 93** (section 1,292 ->
  709), a net **-287** total-byte win.  The w11520 upgrade-check operand
  order (1,970 -> 1,965) was superseded by this shape.
* `MGame::OnUpdate` (w11519): drop the `Status *volatile st` codegen anchor
  and move the `Level *level` snapshot before `radioBase`.  Raw
  **15 -> 16** (+1), aligned **111 -> 6** (-105); 11,440 B exact.  This
  removes a non-human `volatile` anchor, so it is kept despite the +1 raw.

Measured and **not landed** (notes kept): w11505 (case-11 translate arg,
raw +9), w11506 (block-order census), w11508 (Radar CSR lever), w11511
(createMission `combineSelectAndUseCommutative`), w11512 (createMission
bool lead), w11513 (MGame epilogue), w11515 (createShip switch/bitmask,
aligned worse), w11517 (Status 4/3/2), w11518 (Gun/SpaceLounge/Generator),
w11520 (build provenance round 10 + extab census).

**Aligned total (15 functions): 25,618** (wave-164 base 24,608). Per
function: Layout 2,282, LevelScript 17,900 (indicative), createShip 455,
Status 320, Radar 1,595, MGame::OnTouchEnd 1,454, createMission 767, Gun 57,
HangarWindow 386, SpaceLounge 41, Hud 220, Generator 107, MenuTouchWindow 16,
ModStation 12, MGame::OnUpdate 6.  Total differing bytes fall
**85,397 -> 82,556** (-2,841) and `.text` **84,652 -> 81,825** (-2,827);
`.ARM.extab` **725 -> 711** (-14).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,932** (57,232 B), `Radar::draw` **7,095** (9,908 B), `Layout` C1/C2
**5,640** (7,072 B), `Level::createMission` **5,435** (17,172 B),
`MGame::OnTouchEnd` **3,775** (8,080 B), `HangarWindow::render` **2,266**
(9,700 B), `Level::createShip` **1,560** (2,764 B), `Hud::draw` **548**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **127** (1,996 B),
`ModStation` C1/C2 **43** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **81,825** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **711** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (wave 169)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave 169 ran 20 agents on the worst functions (6 on `LevelScript::process`,
2 on `Radar::draw`, 2 on `Layout`, 2 on `Level::createMission`, one each on
the rest, one build-provenance).  Landed changes:

* `LevelScript::process` (w11403): mission-0 state-4 `stop(currentMusicEvent)`
  and state-5 `stop(player->field_0x1c)`.  Raw **54,015 -> 54,012** (-3),
  aligned 17,788 -> 17,787.
* `LevelScript::process` (w11401): mission-94 state-11 hot / message-15 cold
  expects, then drop the stale state-4 timer expect.  Raw
  **54,012 -> 53,981** (-31), aligned 17,787 -> 17,796; 57,232 B exact.
* `LevelScript::process` (w11404): narrow-scope `m_pLevel` aliases in the
  state-5 timer, mission-0 msg-10 + state-7 dock, and mission-1 case-1
  blocks.  Raw **53,981 -> 53,970** (-11), aligned 17,796 -> 17,818.
* `LevelScript::process` (w11504): block-scope aliases for the remaining
  `m_pLevel` sites (mission-0 state-1, case-21 loop body, state-7 dock) and
  `m_pGeometry1`/`m_pGeometry4`/`m_pGeometry5`/`m_pParticleGeom1`/`m_pCamera`
  runs.  Raw **53,970 -> 53,933** (-37), aligned 17,818 -> 17,916; 57,232 B
  exact, `.ARM.extab` 1,292 -> 1,291.
* `Radar::draw` (w11407): six more condition-chain `__builtin_expect` cells
  (enemy flags, dock-nav pointer, landmark blip, enemy-lock timer, gas-cloud
  lock).  Raw **7,486 -> 7,127** (-359), aligned 1,456 -> 1,606;
  9,908 B exact, `.ARM.extab` 668 -> 712.
* `Layout` C1/C2 (w11410+w11409): tail reorder `0x27c`/`0x285` and three
  first-half statement reorderings.  Raw **5,656 -> 5,650** (-6), aligned
  2,283 -> 2,284; 7,072 B exact.
* `HangarWindow::render` (w11414): snapshot `tab` before the ship guard so
  the block order matches the original (ship body first, `switch` after).
  Raw **2,245 -> 1,970** (-275), aligned **620 -> 294** (-326); 9,700 B
  exact, but its `.ARM.extab` record diff rises 97 -> 676 (section 668 ->
  1,247) because rows 39-40 keep a different row grouping.

Measured and **not landed** (notes kept): w11402 (call histogram),
w11405 (mission-78 tail inline: raw 54,113, worse), w11406 (block-order
census), w11408 (Radar branch layout), w11411 (createMission guard icmp),
w11412 (createMission scope/lifetime), w11413 (MGame epilogue), w11415
(createShip RA map), w11416 (Hud show order: T-first 572 raw but extab
shifts), w11417 (Status 4/3/2), w11418 (Gun/SpaceLounge/Generator), w11419
(ModStation/MenuTouchWindow/MGame::OnUpdate), w11420 (build provenance
round 9).

**Aligned total (15 functions): 25,570** (wave-164 base 24,608). Per
function: Layout 2,284, LevelScript 17,818 (indicative), createShip 455,
Status 320, Radar 1,606, MGame::OnTouchEnd 1,454, createMission 767, Gun 57,
HangarWindow 294, SpaceLounge 41, Hud 228, Generator 107, MenuTouchWindow 16,
ModStation 12, MGame::OnUpdate 111.  Total differing bytes fall
**85,397 -> 82,954** (-2,443) and `.text` **84,652 -> 81,642** (-3,010);
`.ARM.extab` **725 -> 1,292** (+567).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**53,970** (57,232 B), `Radar::draw` **7,127** (9,908 B), `Layout` C1/C2
**5,650** (7,072 B), `Level::createMission` **5,435** (17,172 B),
`MGame::OnTouchEnd` **3,775** (8,080 B), `HangarWindow::render` **1,970**
(9,700 B), `Level::createShip` **1,560** (2,764 B), `Hud::draw` **582**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **127** (1,996 B),
`ModStation` C1/C2 **43** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **15** (11,440 B).

Section deltas: `.text` **81,642** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,292** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-11 (waves 165-168)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Waves 165-167 ran 20 agents each (wave 165 was interrupted twice by a host
OOM; its work was re-run as wave 166 with `-j2` builds and a 12 GB per-agent
`ulimit -v`).  Landed changes:

* `LevelScript::process` (w11304, wave 168): four cached-`Status` alias
  placements in consecutive `Globals::status->` write runs (mission-106
  gamma, mission-113 shield+gamma, mission-5 station-attacked).  Raw
  **54,031 -> 54,018** (-13), aligned 17,764 unchanged; 57,232 B exact,
  `.rel.plt` byte-identical, `.ARM.extab` unchanged.
* `Hud::draw` (w11316, wave 168): dock-block `barW` to `unsigned int`
  and `barH` to `short` (re-aligns the show block), then `barYOffset`
  declared before `barH`.  Raw **762 -> 582** (-180), aligned **222 -> 228**
  (+6); 9,332 B exact, `.rel.plt` byte-identical, `.ARM.extab` -2.
* `LevelScript::process` (w11301, wave 168): size-exact companion at
  L5132+L3111 and a nested-operand companion at L1436.  Raw
  **54,018 -> 54,015** (-3), aligned **17,764 -> 17,788** (+24); 57,232 B
  exact, `.rel.plt` byte-identical.
* `Layout` C1/C2 (w11319, wave 168): per-TU `-mllvm -enable-arm-prof-unpr`.
  Raw **5,658 -> 5,656** (-2), aligned **2,280 -> 2,283** (+3); 7,072 B
  exact, `.rel.plt` byte-identical.
* `LevelScript::process` (w11201, wave 167): 11 more `__builtin_expect`
  cells from a 498-cell operand sweep, 772-cell full-if sweep and 486-cell
  re-sweep.  Raw **54,095 -> 54,031** (-64), aligned **17,621 -> 17,764**
  (+143); 57,232 B exact, `.rel.plt` byte-identical, `.ARM.extab` -2.
* `LevelScript::process` (w11103, wave 166): 13 `__builtin_expect` cells
  (hot/cold sweep, size-compensating pairs).  Raw **54,247 -> 54,095**
  (-152), aligned **17,532 -> 17,621** (+89).
* `MGame::OnTouchEnd` (w11212, wave 167): 22<->74 and 43<->65 reward-chain
  swaps.  Raw **4,082 -> 3,775** (-307), aligned **1,018 -> 1,454** (+436);
  8,080 B exact, `.ARM.extab` +12.
* `Level::createMission` (w11218, wave 167): per-TU
  `-mllvm -enable-earlycse-memssa=false` on `Level.cpp`.  Raw **5,436 ->
  5,435** (-1), aligned unchanged; 17,172 B exact.
* `Radar::draw` (w11106+w11107, wave 166): `expect(isDockingToPlanet(), 1)`,
  sound-ladder sub-guard expects, `notTurretMode`/`notOtherLock` decl order.
  Raw **7,549 -> 7,489** (-60), aligned **1,446 -> 1,445** (-1).
* `Radar::draw` (w11118, wave 166): per-TU `-mllvm -disable-postra-machine-licm`
  on `Radar.cpp`.  Raw **7,489 -> 7,487** (-2), aligned 1,445 -> 1,454 (+9).
* `Layout::Layout` (w11109+w11108, wave 166): `field_0x280` n9/else branch
  order, `field_0x2c_rowHeight` before `field_0x34`, `field_0x34`/`field_0x44`
  store swap.  Raw **5,688 -> 5,658** (-30), aligned **2,108 -> 2,280**
  (+172); 7,072 B exact.
* `Radar::draw` (w11106, wave 165): `expect(isDockingToPlanet(), 1)`.
  Raw **7,578 -> 7,549** (-29), aligned 1,428 -> 1,446 (+18).
* `MGame::OnUpdate` (w11117, wave 165): `Status *volatile st` anchor for the
  m122 body.  Raw **16 -> 15** (-1), aligned **6 -> 111** (+105).

Measured and **not landed** (notes kept): w11101/w11202 (PLT call histogram
and call-shape bugs), w11102/w11203 (mission-0 switch order), w11104/w11204
(callee-saved map), w11105/w11205 (inlining histogram, mission-78 dead
tail), w11110/w11210 (`SinkCmpExpression` is the `and`-operand gate),
w11111/w11211 (createMission frame map), w11113/w11213 (HangarWindow cleanup
scope), w11114/w11214 (createShip faithful jitter `X,Y,Z`: raw unchanged,
aligned +5), w11115/w11215 (Hud dock), w11116/w11216 (Status switch
weights), w11117/w11217 (small tail), w11206/w11207/w11208/w11209 (Radar
and Layout block maps), w11118/w11119/w11218/w11219/w11220 (build provenance
rounds 6-7).

**Aligned total (15 functions): 25,713** (wave-164 base 24,608). Per
function: Layout 2,283, LevelScript 17,788 (indicative), createShip 455,
Status 320, Radar 1,454, MGame::OnTouchEnd 1,454, createMission 767, Gun 57,
HangarWindow 620, SpaceLounge 41, Hud 228, Generator 107, MenuTouchWindow 16,
ModStation 12, MGame::OnUpdate 111.  Total differing bytes fall
**85,397 -> 83,015** (-2,382) and `.text` **84,652 -> 82,327** (-2,325);
`.ARM.extab` **725 -> 668** (-57).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,015** (57,232 B), `Radar::draw` **7,486** (9,908 B), `Layout` C1/C2
**5,656** (7,072 B), `Level::createMission` **5,435** (17,172 B),
`MGame::OnTouchEnd` **3,775** (8,080 B), `HangarWindow::render` **1,970**
(9,700 B), `Level::createShip` **1,560** (2,764 B), `Hud::draw` **582**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **127** (1,996 B),
`ModStation` C1/C2 **43** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **15** (11,440 B).

Section deltas: `.text` **82,327** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **668** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

Build provenance rounds 6-7 (w11118/w11119/w11218/w11219/w11220): the
original is an r18b CMake build with uniform `-Oz`/`-g2` per TU and the
checked-in link order; hidden visibility and a version script are refuted.
The per-TU flag space was swept twice more (5,712 + ~6,000 cells).  The
surviving knobs are `Radar.cpp -disable-postra-machine-licm` and
`Level.cpp -enable-earlycse-memssa=false`; the LevelScript
`-machine-sink-split-probability-threshold=0` knob is size-changing once
w11103's source changes are applied (57,228 B), so it was dropped.  The
Status 4/3/2 switch plateau remains unreachable from a source-only shape.

## Update 2026-11-08 (wave 164)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-164 ran 20 agents: 16 on the worst residual functions, 4 on build
provenance (round 5).  Ten source changes landed (the largest raw-byte
reduction of any wave so far):

* `LevelScript::process` (w11002): 13 PLT-safe `__builtin_expect` cells
  found by a whole-function re-sweep.  Raw **54,541 -> 54,247** (-294),
  aligned **17,427 -> 17,532** (+105); 57,232 B exact, `.rel.plt`
  byte-identical.
* `LevelScript::process` (w11019): per-TU `-mllvm -disable-postra-machine-sink`
  on `LevelScript.cpp` (matching knob, alongside the existing per-file
  branch-weight knobs).  Raw **54,541 -> 54,539** (-2) before w11002, size
  and `.rel.plt` unchanged.  The flag space is otherwise exhausted
  (round-5 note).
* `Radar::draw` (w11008): `expect` on the `field_0x1bc` sound-ladder
  sub-guards + faithful tail (`field_0x1bc = 0` only).  Raw **8,124 ->
  7,578** (-546), aligned **1,320 -> 1,428** (+108), `.ARM.extab` 725 ->
  720; 9,908 B exact.
* `Layout::Layout` (w11005+w11006): explicit `iPadHD/n9/else` chain for the
  first two fields, plus five tail store-order relocations.  Raw **5,705 ->
  5,688** (-17, super-additive), aligned **2,082 -> 2,108** (+26); 7,072 B
  exact.
* `Level::createMission` (w11010): `__builtin_expect(comingFromAlienWorld, 0)`
  flips the `StackSlotColoring` weight order with zero code change.  Raw
  **5,454 -> 5,436** (-18), aligned **786 -> 767** (-19); 17,172 B exact.
* `MGame::OnTouchEnd` (w11011+w11012): `bool Mission::hasFailed` field/return,
  then the 100/110 reward-chain block swap.  Raw **4,127 -> 4,087** (-40),
  aligned **906 -> 917** (+11); 8,080 B exact.
* `HangarWindow::render` (w11013): volatile `scrollOffset` LSDA sink.  Raw
  **2,514 -> 2,245** (-269), aligned **485 -> 620** (+135), `.ARM.extab`
  725 -> 659 (-66); 9,700 B exact.
* `Hud::draw` (w11015): move the volatile `Globals::status` reload to the
  `hitFlags & 2/0x24` boundary and `barYOffset` after the show-bar `String`.
  Raw **1,015 -> 762** (-253), aligned **229 -> 222** (-7); 9,332 B exact.
* `Level::createShip` (w11014): original switch case order `0x31,0x2c,0x33`
  plus jitter call order `Y,X,Z`.  Raw **1,563 -> 1,560** (-3), aligned
  **449 -> 455** (+6); 2,764 B exact.
* `Generator::createMission` (w11017): `volatile int bonus` + local `b`.
  Raw **162 -> 127** (-35), aligned **64 -> 107** (+43), `.ARM.extab`
  725 -> 719; 1,996 B exact.

Measured and **not landed** (notes kept): w11001 (LevelScript mission-0
case order + `tbh` tables), w11003 (LevelScript `0.8f` hoist + state-3
companion: raw +303, structural), w11004 (LevelScript callee-saved map),
w11007 (Radar campaign/sl block map), w11009 (createMission
`FindMergedConditions` is the `and`-operand gate, not `Reassociate`),
w11016 (Status source-level switch weights), w11018 (toolchain provenance),
w11020 (link recipe provenance).

**Aligned total (15 functions): 24,620** (wave-163 base 24,200). Per
function: Layout 2,120, LevelScript 17,532 (indicative), createShip 455,
Status 320, Radar 1,428, MGame::OnTouchEnd 917, createMission 767, Gun 57,
HangarWindow 620, SpaceLounge 41, Hud 222, Generator 107, MenuTouchWindow 16,
ModStation 12, MGame::OnUpdate 6.  Total differing bytes fall
**85,397 -> 83,856** (-1,541) and `.text` **84,652 -> 83,177** (-1,475);
`.ARM.extab` falls **725 -> 659** (-66).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,247** (57,232 B), `Radar::draw` **7,578** (9,908 B), `Layout` C1/C2
**5,685** (7,072 B), `Level::createMission` **5,436** (17,172 B),
`MGame::OnTouchEnd` **4,087** (8,080 B), `HangarWindow::render` **2,245**
(9,700 B), `Level::createShip` **1,560** (2,764 B), `Hud::draw` **762**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **127** (1,996 B),
`ModStation` C1/C2 **43** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **83,144** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **659** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

Build provenance round 5 (w11018/w11020) pinned the toolchain: NDK r18b
clang 7.0.2 (r18-beta1/r18/r18b `clang` are the same binary), gold 1.12,
binutils 2.27 `strip --strip-all`, API 16, C++11, `-Oz`, full `-g`, r18b
CMake toolchain (not ndk-build).  The ground-truth file is the APK copy with
`.shstrtab` rewritten into gold Stringpool order; `tools/reorder_shstrtab.py`
reproduces that normalisation exactly.  Every non-code ELF byte is identical;
the residual is `.text`, `.ARM.extab` and the 20-byte build-id descriptor.
See `_work/reconstructions/build-provenance-round5-{toolchain,flags,link}.md`.

## Update 2026-11-08 (wave 163)

**State at wave 163.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-163 ran 20 agents on the worst residual functions plus build-provenance
round 4.  Three source changes landed:

* `LevelScript::process` (w10901): a +24 B companion for the `L1364:0` /
  `L1389:1` cells (`expect(m_nState == 2, 0)`, `expect(m_nState == 5, 1)`,
  `m_bCinematicBreak = 0`, setTarget-first).  Raw **54,698 -> 54,541**
  (-157), aligned **17,456 -> 17,427** (-29); 57,232 B exact,
  `.rel.plt` byte-identical.
* `Hud::draw` (w10917): volatile `Globals::status` reload in the damage
  block, drop the block-4 null test.  Raw **1,227 -> 1,015** (-212),
  aligned **248 -> 229** (-19); 9,332 B exact.
* `MGame::OnUpdate` (w10919): a `Level *level = this->level` local forces
  the GOT base into r9.  Raw **37 -> 16** (-21), aligned **109 -> 6**
  (-103); 11,440 B exact.

Measured and **not landed** (notes kept): w10902 (LevelScript mission-0
`4,3,5` swap: 54,977/17,608), w10903 (second `0.8f` load PLT-safe: size
57,264, needs a -32 B companion), w10906 (Layout first-two-field chain:
claimed 5,695 but measured 5,705; reverted), w10907 (Layout `0x8c..0xa8`
run: no win), w10908 (Radar 4 B tail: claimed 7,584 but measured 8,124;
reverted), w10909 (Radar `campaign`->`sl`: 9,611, `.text` 86,529),
w10910 (MGame::OnTouchEnd cold `initHudMenu`: 4,575/1,373), w10911 (`bool
Mission::hasWon`: 5,742/1,109; reverted), w10912 (createMission
`Reassociate` sweep), w10913 (createMission frame map), w10914 (createShip
case order: 1,560/460), w10915 (Status 4/3/2 from source), w10916
(HangarWindow size-exact `cbz`: `.ARM.extab` 725 -> 1,250; reverted),
w10918 (small tail, no win), w10920 (build provenance round 4).

**Aligned total (15 functions): 24,199** (wave-162 base 24,351). Per
function: Layout 2,082, LevelScript 17,427 (indicative), createShip 449,
Status 320, Radar 1,320, MGame::OnTouchEnd 905, createMission 786, Gun 57,
HangarWindow 485, SpaceLounge 41, Hud 229, Generator 64, MenuTouchWindow 16,
ModStation 12, MGame::OnUpdate 6.  Total differing bytes fall
**85,787 -> 85,395** (-392) and `.text` **85,042 -> 84,650** (-392).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,541** (57,232 B), `Radar::draw` **8,124** (9,908 B), `Layout` C1/C2
**5,705** (7,072 B), `Level::createMission` **5,454** (17,172 B),
`MGame::OnTouchEnd` **4,125** (8,080 B), `HangarWindow::render` **2,514**
(9,700 B), `Level::createShip` **1,563** (2,764 B), `Hud::draw` **1,015**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `SpaceLounge::drawLounge`
**175** (1,948 B), `Generator::createMission` **162** (1,996 B),
`ModStation` C1/C2 **43** (1,008 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B), `MGame::OnUpdate` **16** (11,440 B).

Section deltas: `.text` **84,650** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **725** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

Build provenance round 4 (w10920) confirmed that at `-Oz` the sample
profile loader ignores `BranchInst`, so the empty-`br` successors of the
Status cases 0/11/13 (and 156/164/165/169/171) cannot be weighted by line
samples: the original's 4/3/2 plateau is unreachable from a real sample
profile, and an instrumented profile also weights every other branch and
moves the non-switch layout away.  See
`_work/reconstructions/build-provenance-round4-w10920.md`.

## Update 2026-11-08 (wave 162)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-162 ran 20 agents on the worst residual functions plus build-provenance
round 3.  Four source changes landed:

* `LevelScript::process` (w10803): four PLT-safe `__builtin_expect` cells
  (`L1030:1`, `L1430:1`, `L1522:0`, `L1526:1`), found by re-sweeping the
  whole expect table with a `.rel.plt` byte check after every cell. Raw
  **55,013 -> 54,698** (-315), aligned **17,308 -> 17,456** (+148); 57,232 B
  exact, `.rel.plt` byte-identical.
* `Layout::Layout` (w10807): `field_0x94` tail relocation. Raw
  **5,715 -> 5,705** (-10), aligned **2,050 -> 2,082** (+32); 7,072 B exact.
* `Hud::draw` (w10817): swap `barH`/`barW` declaration order in the
  docking-transfer bar. Raw **1,233 -> 1,227** (-6), aligned **256 -> 248**
  (-8); 9,332 B exact.
* `MenuTouchWindow::OnTouchBegin` (w10818): case-15 `top` from the reloaded
  `Globals::layout`. Raw **29 -> 28** (-1), aligned **15 -> 16** (+1);
  1,896 B exact.

Measured and **not landed** (notes kept): w10801 (`Status *&` + `stars4`:
54,996/17,311, does not combine with w10803), w10802 (`Status *&` per-use
reload: 55,014/17,295), w10804 (case-125 state-3 else-if: 55,016/17,066,
does not combine with w10803), w10805 (the 0.8f block is unreachable;
moving it out is -20 B but rotates `.rel.plt`), w10806 (the 128-B Layout
residual is a code/pool split: 6,186+886 vs 6,314+758), w10808 (Radar
faithful tail: 8,367/1,333), w10809 (Radar `campaign`->`sl`), w10810
(MGame::OnTouchEnd 23rd `isCampaignMission`: 5,816/1,379), w10811
(`bool Mission::hasWon`: 6,053/1,085), w10812 (createMission stack-slot
permutation), w10813 (createMission `Reassociate` pins the `and` operand
order), w10814 (createShip 1,560/460), w10815 (Status nested dispatch),
w10816 (HangarWindow `cbz` sweep), w10819 (MGame::OnUpdate last 37),
w10820 (profile-reconstruction harness).

**Aligned total (15 functions): 24,351** (wave-161 base 24,178). Per function:
Layout 2,082, LevelScript 17,456 (indicative), createShip 449, Status 320,
Radar 1,320, MGame::OnTouchEnd 906, createMission 786, Gun 57, HangarWindow
485, SpaceLounge 41, Hud 248, Generator 64, MenuTouchWindow 16, ModStation 12,
MGame::OnUpdate 109.  The aligned rise is w10803's block re-layout (+148) and
w10807 (+32); total differing bytes fall **86,119 -> 85,787** (-332) and
`.text` **85,374 -> 85,042** (-332).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,698** (57,232 B), `Radar::draw` **8,124** (9,908 B), `Layout` C1/C2
**5,705** (7,072 B), `Level::createMission` **5,454** (17,172 B),
`MGame::OnTouchEnd` **4,127** (8,080 B), `HangarWindow::render` **2,514**
(9,700 B), `Level::createShip` **1,563** (2,764 B), `Hud::draw` **1,227**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `Generator::createMission`
**162** (1,996 B), `SpaceLounge::drawLounge` **175** (1,948 B), `ModStation`
C1/C2 **43** (1,008 B), `MGame::OnUpdate` **37** (11,440 B),
`MenuTouchWindow::OnTouchBegin` **28** (1,896 B).

Section deltas: `.text` **85,042** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **725** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 161)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-161 ran 20 agents on the worst residual functions plus build-provenance
round 2.  Six source changes landed:

* `LevelScript::process` (w10702): mission-89 case-4 double `getStarSystem()`
  plus the missing `m_nScriptTimerA >= 8000 -> m_nState = 5` tail,
  compensated by dropping the non-original `if (!enemies->data_) continue;`
  in the mission-0 loop. Raw **55,048 -> 55,013** (-35), aligned
  **17,049 -> 17,308** (+259); 57,232 B exact.
* `Layout::Layout` (w10707): `field_0x34` before `field_0x30_rowHeight` and
  `field_0x19c` before `field_0x18c`. Raw **5,733 -> 5,722** (-11), aligned
  **1,978 -> 2,015** (+37); 7,072 B exact.
* `MGame::OnTouchEnd` (w10710): drop the stale
  `__builtin_expect(isDockingToStream(), 1)` hint. Raw **4,387 -> 4,125**
  (-262), aligned **901 -> 905** (+4); `.rel.plt` stays byte-identical.
* `MGame::OnTouchEnd` (w10711): mining-refine check `count > 0` instead of
  `count != 0` (the original skips on `amount == 0`). Raw **4,125 -> 4,127**
  (+2), aligned **905 -> 906** (+1).
* `MGame::OnUpdate` (w10718): m122 `radioBase` via the live `st`. Raw
  **318 -> 37** (-281), aligned **19 -> 109** (+90); 11,440 B exact.
* `HangarWindow::render` (w10716): row-39 `getCredits()` + `volatile` pad
  shapes. Raw **1,965 -> 2,514** (+549), aligned **291 -> 485** (+194), but
  `.ARM.extab` **1,258 -> 725** (-533) with the 948 B / 156-row record
  intact.
* `Layout::Layout` (w10706): six-statement block permutation around
  `0x15c/0x158/0x204/0x2bc/0x160`. Raw **5,722 -> 5,715** (-7), aligned
  **2,015 -> 2,050** (+35); combined with w10707 the pair is 5,715/2,050.
* `Hud::draw` (w10717): `int barH` plus a volatile size anchor. Raw
  **1,325 -> 1,233** (-92), aligned **249 -> 256** (+7); 9,332 B exact.
* `Level::createMission` (w10713): case-183 default store order 2,0,1.
  Raw **5,461 -> 5,454** (-7), aligned **781 -> 786** (+5).

Measured and **not landed** (notes kept): w10701 (`Status *&status` reorder:
55,061/17,051; the plain reorder is 54,929/16,920 but +60 B), w10703
(state-3 else-if chain), w10704 (the extra 13-entry `tbh` is the mission-0
dispatch), w10708 (Radar faithful tail: 8,367/1,333), w10712 (createMission
register map: no win), w10713 (case-183 default 2,0,1: 5,454/786), w10714
(createShip 1,560/460), w10715 (Status: the 1,432-B weight cell accepts a
4/3/2 arithmetic progression), w10719 (MenuTouchWindow 28/16), w10720
(build provenance round 2: `-g`/`-g2`/`-g3=limited`, flag order, response
file and debug flags are all neutral).

**Aligned total (15 functions): 24,178** (wave-160 base 23,546). Per function:
Layout 2,050, LevelScript 17,308 (indicative), createShip 449, Status 320,
Radar 1,320, MGame::OnTouchEnd 906, createMission 786, Gun 57, HangarWindow
485, SpaceLounge 41, Hud 256, Generator 64, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 109.  The aligned rise is the price of real bug fixes
(mission-89 transition, mining-refine) and the HangarWindow extab win; total
differing bytes fall **86,796 -> 86,119** (-677), `.text` **85,518 -> 85,374**
(-144) and `.ARM.extab` **1,258 -> 725** (-533).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,013** (57,232 B), `Radar::draw` **8,124** (9,908 B), `Layout` C1/C2
**5,715** (7,072 B), `Level::createMission` **5,454** (17,172 B),
`MGame::OnTouchEnd` **4,127** (8,080 B), `HangarWindow::render` **2,514**
(9,700 B), `Level::createShip` **1,563** (2,764 B), `Hud::draw` **1,233**
(9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `Generator::createMission`
**162** (1,996 B), `SpaceLounge::drawLounge` **175** (1,948 B), `ModStation`
C1/C2 **43** (1,008 B), `MGame::OnUpdate` **37** (11,440 B),
`MenuTouchWindow::OnTouchBegin` **29** (1,896 B).

Section deltas: `.text` **85,374** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **725** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 160)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-160 ran 20 agents, all on the worst residual functions plus a build
provenance replay.  Six source changes landed:

* `Layout::Layout` (w10507): three size-exact adjacent field swaps. Raw
  **5,739 -> 5,733** (-6), aligned 1,978 (0); 7,072 B exact.
* `LevelScript::process` (w10605): reload `Globals::status->field_90` in the
  mission-fail loop bound. Raw **55,055 -> 55,047** (-8), aligned
  **17,059 -> 17,050** (-9).
* `LevelScript::process` (w10604): drop the hacking-guard
  `__builtin_expect`. Raw **55,047 -> 55,056** (+9), aligned
  **17,050 -> 17,031** (-19).
* `LevelScript::process` (w10602): case-16 state-0 `m_nState++` instead of
  `= 1`. Raw **55,056 -> 55,048** (-8), aligned **17,031 -> 17,049** (+18).
  Net LevelScript: **55,048 / 17,049**.
* `Level::createMission` (w10612): hoist the alien-loop `end` bound and
  recompute the tail. Raw **5,652 -> 5,454** (-198), aligned **709 -> 786**
  (+77); 17,172 B exact.
* `Level::createMission` (w10613): case-183 freighter default store order.
  Raw **5,454 -> 5,461** (+7), aligned **786 -> 781** (-5).
  Net createMission: **5,461 / 781**.

Measured and **not landed** (notes kept): w10611 and w10610 (MGame::OnTouchEnd
join/arm shapes: raw 4,375 / aligned 956 but they swap a `.rel.plt` slot;
w10610 drops byte_exact to 4505), w10608 (Radar duplicate store: 8,536/1,314),
w10616 (HangarWindow row-39 ULEB: text +549 for extab 1,256 -> 731, net +24
bytes), w10603 (LevelScript literal-pool audit: an unreachable
`translate(-delta*0.8f)` block and a missing mission-89 `>= 8000` guard),
w10607 (Layout float audit: four real fixes sum to -64 B, no size-exact
combination), w10617 (Hud: `int barH` is the original type but gives 9,324 B),
w10619 (small-tail full-stream diff), w10620 (build provenance: every
non-code ELF byte is already identical; `.ARM.extab` 1,256 decomposes into 12
functions' LSBAs; 40+ per-TU flag cells tested, none wins).

**Aligned total (15 functions): 23,546** (wave-159 base 23,484). Per function:
Layout 1,978, LevelScript 17,049 (indicative), createShip 449, Status 320,
Radar 1,320, MGame::OnTouchEnd 901, createMission 781, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 64, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 19.  The aligned total rises by 62 because createMission's
recomputed tail re-orders blocks that the greedy instruction alignment
penalises; the raw `.text` diff falls by **204 bytes** (85,722 -> 85,518).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,048** (57,232 B), `Radar::draw` **8,124** (9,908 B), `Layout` C1/C2
**5,733** (7,072 B), `Level::createMission` **5,461** (17,172 B),
`MGame::OnTouchEnd` **4,387** (8,080 B), `Level::createShip` **1,563** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **162** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **318** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **85,518** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,258** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 159)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-159 ran 20 agents (all on the residual functions plus a build replay and
a 120-cell declaration-reorder sweep). Five source changes landed:

* `LevelScript::process` (w10502): move the state-6 block after the
  state-4/5 arm. Raw **55,094 -> 55,062** (-32), aligned **17,129 -> 17,055**
  (-74); 57,232 B exact.
* `Radar::draw` (w10504): `int alienOrbit` instead of `bool` (restores the
  original LSDA scope) and a direct `lockLabel` assignment. Raw
  **8,181 -> 8,124** (-57), `.ARM.extab` **1,284 -> 1,247** (-37), aligned
  **1,278 -> 1,320** (+42, deliberate trade); 9,908 B exact.
* `LevelScript::process` (w10503): case-154 state-9 `__builtin_expect` and
  case-16 `m_nState++`. Raw **55,062 -> 55,055** (-7), aligned
  **17,055 -> 17,059** (+4); 57,232 B exact.
* `Level::createMission` (w10509): both-unnamed alien block (drop the
  redundant `p2`/`field_0x70`/`cargo` pre-store). Raw **5,487 -> 5,652**
  (+165), aligned **774 -> 709** (-65); 17,172 B exact.
* `MGame::OnUpdate` (w10518): mission-122 `getCurrentCampaignMission`
  receiver is `Globals::status`, not the live `Status *st`. Raw
  **37 -> 318** (+281), aligned **109 -> 19** (-90); 11,440 B exact.  The
  remaining 19 mismatches are two real differences (an r2/r5 allocation tie
  and a `mov r0,r5` join) plus cascaded branch-target indices.

Measured and **not landed** (notes kept in `_work/reconstructions/`):
w10505 (Radar duplicate `stationLockTimer` store + drop tail `String`:
8,520/1,273, aligned beats w10504 but raw loses 396; its first hunk alone on
the w10504 base is +12 B), w10510 (createMission `this=r4` hoisted bound:
6,381/1,341, a stepping stone), w10509+w10510 (17,156 B, gate-fatal),
w10517 (createShip case order: 1,560/460), w10508 (Layout 0x140 group
blocked by `stmia`), w10511 (MGame join 4,022/762 at +12 B), w10512
(`cmp #0` sweep: base stays Pareto), w10513 (Status profile plateau),
w10514 (no PGO profile beats no-profile `-Oz`), w10515 (Hud anchor unique),
w10516 (HangarWindow extab 759 at a big `.text` cost), w10519 (SpaceLounge
exact clamp 1952, Generator exact block 2000), w10520 (build replay: `-Oz`
optimal, 19/27 `-mllvm` cells neutral).

**Aligned total (15 functions): 23,484** (wave-158 base 23,667). Per function:
Layout 1,978, LevelScript 17,059 (indicative), createShip 449, Status 320,
Radar 1,320, MGame::OnTouchEnd 901, createMission 709, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 64, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 19.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,055** (57,232 B), `Radar::draw` **8,124** (9,908 B), `Layout` C1/C2
**5,739** (7,072 B), `Level::createMission` **5,652** (17,172 B),
`MGame::OnTouchEnd` **4,387** (8,080 B), `Level::createShip` **1,563** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **162** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **318** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **85,722** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,256** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 158)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-158 ran 20 agents (all on the residual functions plus a build replay).
Five source changes landed:

* `Layout::Layout` (w10408): move `field_0x128` after `field_0x108`.
  Raw **5,753 -> 5,745** (-8), aligned **1,984 -> 1,982** (-2); 7,072 B exact.
* `Layout::Layout` (w10407): move `field_0x110_strip58` up between
  `0x12c`/`0x2b8` and `rewardBoxY` after the `rewardBoxY2` chain. Combined
  with w10408: raw **5,745 -> 5,739** (-6), aligned **1,982 -> 1,978** (-4).
* `MGame::OnTouchEnd` (w10411): fold the dead `hudTouchFlags == 0` block
  into the wingman `else-if` chain. Raw **4,391 -> 4,387** (-4), aligned
  **903 -> 901** (-2); 8,080 B exact; the `dockToStream` PLT slot is
  byte-identical.
* `Level::createShip` (w10417): waypoint null check as `if/else` with
  declarations in `wy, wx, wz` order. Raw **1,564 -> 1,563** (-1), aligned
  **452 -> 449** (-3); 2,764 B exact.
* `Generator::createMission` (w10419): an `Item*` temp removes the
  redundant `ldr r5` reload. Raw **300 -> 162** (-138), aligned 59 -> 64
  (+5); 1,996 B exact.

Measured and **not landed** (notes kept): w10401 (prologue r4 sweep: the
all-local shape is -108 B, the best aligned shape is 16,956 at +52 B),
w10402 (case-154/16 expect + `m_nState++`: 55,087/17,133), w10403 (first
size-exact `this=sl` prologue: 55,334/21,573; the `messages,status,enemies,
player` reorder is -172/-116 at +60 B, and a -8 B cold companion would
land it), w10404 (scoped else arm on w10305 flags: 8,391/1,320, extab
1,254), w10405 (68/campaign register analysis), w10406 (direct lockLabel:
8,236/1,368, extab 1,253), w10409 (temp=r5 + raid-decl: 6,466/1,230),
w10410 (KVBR alien decl order: 5,485/795), w10412 (fold + AND form:
4,375/956), w10413 (no source-level plateau; IR plateau re-confirmed at
558/47/1,432), w10414 (generic PGO: every profile worse), w10415 (Hud
anchor still unique), w10416 (HangarWindow extab 767 at `.text` 86,078),
w10418 (small tail no win), w10420 (per-TU `-Oz`, `-mllvm`, unwind and
stack-protector replay: still no flag win).

**Aligned total (15 functions): 23,667** (wave-157 base 23,673). Per function:
Layout 1,978, LevelScript 17,129 (indicative), createShip 449, Status 320,
Radar 1,278, MGame::OnTouchEnd 901, createMission 774, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 64, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,094** (57,232 B), `Radar::draw` **8,181** (9,908 B), `Layout` C1/C2
**5,739** (7,072 B), `Level::createMission` **5,487** (17,172 B),
`MGame::OnTouchEnd` **4,387** (8,080 B), `Level::createShip` **1,563** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **162** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **85,372** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,284** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 157)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-157 ran 20 agents (all on the residual functions plus a build replay).
Five source changes landed:

* `LevelScript::process` (w10301): in mission-106 state-6, a `Status* st`
  local is used for the armor-HP store only. Raw **55,095 -> 55,094** (-1),
  aligned 17,131 (0); 57,232 B exact.
* `LevelScript::process` (w10303): hoist the mission-0 state-0 `data_` null
  guard out of the `for` condition into a body `continue`, restoring the
  original `cmp r5,#3; bne` back-edge. Aligned **17,131 -> 17,129** (-2),
  raw 55,094 (0); 57,232 B exact.
* `Radar::draw` (w10305): restore `int alwaysEnemySeen`, make
  `alwaysEnemy` an `int` so clang coalesces both into the original's
  `[sp,#80]` word slot, and add a real `plasmaInRange = 0` tail store.
  Raw **8,320 -> 8,181** (-139), aligned **1,296 -> 1,278** (-18); 9,908 B
  exact.
* `Layout::Layout` (w10307): compose the w10208 SLP chunk-boundary fix
  (`field_0xb0` in the b4..c0 chunk, `marqueeWidth` evicted) with four
  pure statement moves (`0x7c` after `0x80`, `0xb0` after `0xc4`,
  `marqueeWidth` after `0xc8`, `0x13c` after `0x14c`). Raw
  **5,822 -> 5,753** (-69), aligned 1,984 (0); 7,072 B exact.
* `Generator::createMission` (w10319): `volatile rem` +
  `bonus -= rem * 2`. Raw **393 -> 300** (-93), aligned 52 -> 59 (+7);
  1,996 B exact.

Measured and **not landed** (notes kept): w10302 (case-154 graded expect +
case-14 increment: 55,088/17,135), w10304 (exact w10204+w10206 combination:
8,437/1,337 but `.ARM.extab` 1,254), w10306 (landmark 68-hoist analysis),
w10308 (SLP group bases: 5,806/1,999), w10309 (temp=r5 + branch weight:
6,469/1,230), w10310 (alien-gun branch weight: 5,662/748), w10311 (AND-form
won guard: 4,379/949 with the PLT slot intact), w10312 (natural arm order:
4,404/920), w10313 (instrumented profile hits 1,432 B with `.rel.plt` 0
but raw 1,299 / aligned 583), w10314 (generic PGO harness: all profiles
regress), w10315 (Hud anchor still unique), w10316 (HangarWindow extab
1,292 -> 767 at `.text` 86,380), w10317 (createShip no source win),
w10318 (MenuTouchWindow 29 -> 28 / 15 -> 16), w10320 (per-TU `-Oz`,
`-mllvm`, unwind and stack-protector replay: still no flag win).

**Aligned total (15 functions): 23,673** (wave-156 base 23,686). Per function:
Layout 1,984, LevelScript 17,129 (indicative), createShip 452, Status 320,
Radar 1,278, MGame::OnTouchEnd 903, createMission 774, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 59, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,094** (57,232 B), `Radar::draw` **8,181** (9,908 B), `Layout` C1/C2
**5,753** (7,072 B), `Level::createMission` **5,487** (17,172 B),
`MGame::OnTouchEnd` **4,391** (8,080 B), `Level::createShip` **1,564** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **300** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **85,529** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,292** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 156)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-156 ran 20 agents (all on the residual functions plus a build replay).
Four source changes landed:

* `LevelScript::process` (w10202): five case-157 edits (s5 timer expect,
  s5/s6/s8 `m_nState++` on known states, s7 timer expect), semantics
  identical and `.ARM.extab` byte-neutral. Raw **55,123 -> 55,096** (-27),
  aligned **17,180 -> 17,131** (-49); 57,232 B exact.
* `LevelScript::process` (w10203): cache the `Player*` in mission-106
  state-6. Raw **55,096 -> 55,095** (-1), aligned 17,131 (0).
* `Radar::draw` (w10204): `bool enemyHit` (the alignment lever), the
  tractorAuto branch store swap (`field_0x1c` before `dockNavPtr`), the
  w10114 asteroid-lock store order, and a tail `String` companion. Raw
  **8,490 -> 8,320** (-170), aligned **1,456 -> 1,296** (-160); 9,908 B
  exact; `.ARM.extab` 1,292 (neutral).
* `Layout::Layout` (w10207): swap `field_0xc8` and `oreLabelYOffset`
  (found by a fresh exhaustive adjacent-statement sweep on the w10106
  base). Raw **5,837 -> 5,822** (-15), aligned **1,986 -> 1,984** (-2);
  7,072 B exact.

Measured and **not landed** (notes kept): w10201 (faithful two-load
dispatch: 55,096/17,139, +1 raw), w10205 (`bool enemyHit` + `int
asteroidHit`: 8,293/1,552), w10206 (scoped copy + store reorder + bool
enemyHit: 8,449/1,392, extab 1,254; combining with w10204 is 4 B long),
w10208 (SLP chunk-boundary fix: 5,792/2,010), w10209 (temp=r5 alien loop:
6,449/1,196 with `GOF2_GOLD_PAD_CAP` wired), w10210 (alien-gun branch
weight: 5,662/748), w10211 (analysis: both `cmp #1` guards are 50:50
layout crutches; the `(B & 1) != 0` AND exception shifts gold's `.plt`),
w10212 (faithful dock owners + else-if fold: 4,415/926), w10213 (no
source-level 4>3>2 plateau), w10214 (instrumented PGO: 1,432 B reachable
but `.rel.plt` 7), w10215 (Hud anchor still unique), w10216 (createShip
mission test is one bitmask switch; raw unchanged), w10217
(MenuTouchWindow 29 -> 28 / 15 -> 16), w10218 (SpaceLounge/Generator no
size-exact both-metric win), w10219 (HangarWindow extab 1,292 -> 912 but
`.text` 86,044 -> 91,890), w10220 (per-TU `-Oz` and `-mllvm` replay:
still no flag win).

**Aligned total (15 functions): 23,686** (wave-155 base 23,897). Per function:
Layout 1,984, LevelScript 17,131 (indicative), createShip 452, Status 320,
Radar 1,296, MGame::OnTouchEnd 903, createMission 774, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 52, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,095** (57,232 B), `Radar::draw` **8,320** (9,908 B), `Layout` C1/C2
**5,822** (7,072 B), `Level::createMission` **5,487** (17,172 B),
`MGame::OnTouchEnd` **4,391** (8,080 B), `Level::createShip` **1,564** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **393** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **85,831** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,292** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 155)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-155 ran 20 agents (14 on the residual functions, 6 on build-flag
provenance). Three source changes landed:

* `LevelScript::process` (w10102): the mission-0 state-0 loop now uses
  `(*enemies)[i]` with a live `enemies->data_` bound and one element local,
  keeping `enemies` live across the loop like the original. Raw
  **55,327 -> 55,123** (-204), aligned **17,176 -> 17,180** (+4); 57,232 B exact.
* `Radar::draw` (w10104): the else-arm `String` copy is now the unscoped
  `temp; lockLabel = temp; DrawString(temp)` shape, with a tail
  `Globals::status->getCurrentCampaignMission()` size companion. Raw
  **8,577 -> 8,490** (-87), aligned **1,484 -> 1,456** (-28); 9,908 B exact;
  `.ARM.extab` +37 (the new call site).
* `Layout::Layout` (w10106): five pure statement moves from the w10010 base
  (`0xc0` after `c8`/`oreLabelYOffset`, the `0x190` iPad/retinaDisplay arms
  swapped, `0x310` after `0x30c`, `animSpeedScale` before `0xdc`, `0x28c`
  before `0x140`). Raw **5,849 -> 5,837** (-12), aligned **2,006 -> 1,986**
  (-20); 7,072 B exact.

Measured and **not landed** (notes kept): w10103 (case-125/42/142
composition: raw -35 but `.ARM.extab` +104), w10105 (`int marked`:
8,506 / 1,572), w10107 (temp=r5 alien loop: 6,449 / 1,196, gold pad +0x20),
w10108 (faithful dock owners: 4,427 / 926), w10109 (HangarWindow block
move: 7,811 / 842 but `.ARM.extab` 676 -> 296), w10114 (asteroid-lock
store order: 8,482 / 1,477; combining with w10104 is 4 B short), w10009
(Layout greedy reorder: raw 5,747 but aligned 2,283), w10110/w10111/w10112
(no source win), w10113 (per-basic-block map: top cluster is the
case-106 callee-saved allocation).

Build-provenance research (all gate-neutral, notes + harnesses committed):

* w10115: per-TU `-O1/-O2/-Os/-O3` sweep over all 17 residual TUs plus 4
  exact controls (85 cells): **`-Oz` is optimal for every residual TU**; no
  single-TU override makes any residual byte-exact.
* w10116: targeted `-mllvm` matrix at wave 154 (SLP, loop-sink, tail-merge,
  inline/unroll thresholds, GVN hoist, load-pre, section-prefix): no knob
  makes any residual byte-exact and none improves aligned; the original is
  the stock clang 7.0.2 `-Oz` pipeline.
* w10117: sample PGO cannot reproduce the flat `Status` switch vector
  (successor-block weights are 0 for the eight branch-only case blocks);
  an instrumented flat vector encodes the weights but shrinks the body to
  1,424 B or hits 1,432 B with the wrong call order (`.rel.plt` 7).
* w10118: ICF is disproven (112 duplicate bodies at distinct addresses;
  `--icf=safe` shrinks `.text` by 11,364 B), LTO is disproven (full `-flto`
  `.text` 0x12d404), `--gc-sections`/`--sort-section=name`/no-`-z now` all
  regress; the original `.text` rank order is exactly ours (4522/4522).
* w10119: data/layout flag fingerprints — `-fno-unwind-tables` is the only
  observable data-shape lever (4507 -> 2878), `-fstack-protector-strong` is
  uniquely correct, `-fno-common`/`-mfloat-abi=hard` fail to link.
* w10120: flag-by-flag build archaeology; the NDK r18b clang 7.0.2
  `-Oz -g2` compile line and the plain-gold link line are proven from the
  ELF. **PGO is the one open provenance question**: the source-order switch
  layout needs branch weights, but no `.profraw`/`.profdata` survives.

**Aligned total (15 functions): 23,897** (wave-154 base 23,941). Per function:
Layout 1,986, LevelScript 17,180 (indicative), createShip 452, Status 320,
Radar 1,456, MGame::OnTouchEnd 903, createMission 774, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 52, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,123** (57,232 B), `Radar::draw` **8,490** (9,908 B), `Layout` C1/C2
**5,837** (7,072 B), `Level::createMission` **5,487** (17,172 B),
`MGame::OnTouchEnd` **4,391** (8,080 B), `Level::createShip` **1,564** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **393** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **86,044** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,292** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 154)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-154 ran 20 agents (4 on `LevelScript`, 3 on `Radar`, 3 on `Layout`,
2 on `createMission`, 2 on `MGame`, the rest on the small tail/extab/PGO).
Four source changes landed:

* `LevelScript::process` (w10007): a `Status*` local makes clang CSE the
  vtable load across the two dispatch blocks, putting it in `r4`. Raw
  **55,328 -> 55,327**; **aligned 17,185 -> 17,176**; 57,232 B exact.
* `Layout::Layout` (w10010): a five-move statement reorder
  (`0x184` before `0x1a0`, `marqueeWidth` before `oreTextYOffset`, `0x284`
  before `0x310`, `0x2c` before `0x38`, `0x40` after `buttonInsetX`).
  Raw **5,830 -> 5,849**; **aligned 2,119 -> 2,006**; 7,072 B exact.
* `Level::createShip` (w10016): split the five-way mission test into
  `if (mission == 56) ... else if (mission >= 49 && mission <= 52) ...`.
  Raw **1,577 -> 1,564**; **aligned 457 -> 452**; 2,764 B exact.
* `MGame::OnUpdate` (w10017): the mission-122 `radioBase` reads
  `st->getCurrentCampaignMission()` so the `Status` value stays live and
  `&Globals::status` stays in `r9`; the whole 11,440-byte function is now
  byte-identical except two `mov r0,r5` sites. Raw **318 -> 37**;
  aligned 19 -> 109 (literal-pool/branch shifts); 11,440 B exact.

Measured and **not landed** (notes kept): w10001 (the 0x30 gold pad + composed
else-arm String copy is raw 8,570 / aligned 1,345 but shifts the
`.ARM.exidx`/`.ARM.extab` bases by -16, exploding their content diffs to
4,665 / 22,589; reverted), w10004 (faithful mission-0 loop: aligned 21,871),
w10005 (case-125 else-if: aligned 17,304), w10006 (c154->strong adjacency:
aligned 17,363), w10008 (Layout SLP group offsets: raw 5,781 but aligned
2,128; w10010 alone is better), w10011 (temp=r5 alien loop: raw 6,449 /
aligned 1,210), w10012 (alien-gun branch weight: raw 5,662 / aligned 748),
w10014 (initHudMenu(2) owner: raw 4,427 / aligned 926).

Analysis-only passes: w10002 (`otherLock` is `int` at [sp,#144]; `campaign`
owns `sl`; `bool otherLock` reproduces the prologue map but is 120 B short),
w10003 (the original else arm is exactly w9902's scoped copy; every shape
that improves both metrics is 4-8 B short of 9,908), w10009 (clang-7 SLP
16-store chunking is the group-base lever; every group fix removes ~120 B of
code but adds no pool words), w10013 (`won` guard is a bool-return-type
artifact; `(hasWon() & 1) != 0` improves raw but breaks `MGame::dockEvent`),
w10015 (the Hud `pivotY = dividerOffset` anchor remains the unique 9,332 B /
224-frame shape), w10018 (MenuTouchWindow's case-15 test is all-direct but
removing the reference shifts the tail by 4 B), w10019 (HangarWindow row
39/40 is `getShip`'s landing pad), w10020 (sample-profile text uses relative
line offsets; `-fprofile-sample-use` always sets `function_entry_count`, so
LoopSink fires and the body grows to 1,440 B).

**Aligned total (15 functions): 23,941** (wave-153 base 23,978). Per function:
Layout 2,006, LevelScript 17,176 (indicative), createShip 452, Status 320,
Radar 1,484, MGame::OnTouchEnd 903, createMission 774, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 52, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,327** (57,232 B), `Radar::draw` **8,577** (9,908 B), `Layout` C1/C2
**5,849** (7,072 B), `Level::createMission` **5,487** (17,172 B),
`MGame::OnTouchEnd` **4,391** (8,080 B), `Level::createShip` **1,564** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **393** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **86,347** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,255** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 153)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-153 ran 20 agents (4 on `LevelScript`, 3 on `Radar`, 3 on `Layout`,
2 on `createMission`, 2 on `MGame`, the rest on the small tail/extab/PGO).
Five source changes landed:

* `Radar::draw` (w9902): put the `String` copy in its own scope so clang
  destroys the `calcDistance` temporary before `DrawString` (the original's
  else-arm structure). The LSDA record is now 132 B / 19 rows / cst_len 112,
  matching the original exactly, with no gold-pad hack. Raw **8,620 ->
  8,577**; **aligned 1,530 -> 1,484**; `.text` **-43**, `.ARM.extab` **-38**.
* `LevelScript::process` (w9904): compose w9804's `expect(weak,1)` +
  case-144 `setStation` restore + case-92 3-call revert onto the w9805 base
  (dropping w9805's c154/s5 timer weights). Raw **55,347 -> 55,328**;
  **aligned 17,223 -> 17,185**; 57,232 B exact.
* `Layout::Layout` (w9909): move the `ringNearMidThreshold` (`0xec`)
  assignment before `0xf0`/`ringMidFarThreshold`/`0xf8`. The GOT-reload
  premise is falsified: both builds issue exactly 8 device-global GOT loads
  with the same multiset; the 124 B gap is SLP group-base placement. Raw
  **5,800 -> 5,830**; **aligned 2,148 -> 2,119**; 7,072 B exact.
* `Level::createMission` (w9912): swap the final cargo/shipGroup stores in
  the alien-gun loop to the original's binary order (`cargo str #0x50`
  before `shipGroup str #0x28`). Raw **5,484 -> 5,487**; **aligned 776 ->
  774**; 17,172 B exact.
* `Level::createShip` (w9916): replace the +4 B guard with a register-local
  null guard and reorder the mission-range test to `mission == 56 || (>=
  49 && <= 52)`. Entry map is now **status r4 / rnd r5 / nextInt r6** (the
  original's rotation). Raw **2,056 -> 1,577**; **aligned 480 -> 457**;
  2,764 B exact.

Measured and **not landed** (notes kept): w9901 (0x30 gold pad + else-arm
String copy: raw 8,570 / aligned 1,345 but needs the gold binary patch and
conflicts with w9902), w9903 (`wreck`-before-`update` + `int gasHit`: raw
8,579 / aligned 1,583), w9906 (case-125 composition: aligned 17,365), w9907
(c154->strong adjacency: aligned 17,401), w9911 (`v`-before-`rotation`: raw
5,482 / aligned 794), w9914 (faithful tail-merge owners: raw 4,424 / aligned
923), w9918 (MenuTouchWindow reload-after-call: raw 28 / aligned 16).

Analysis-only passes: w9905 (the mission-0 `(*enemies)[i]` status-r4 prologue
is +112 B on the wave-152 base; no single -112 companion), w9908 (the 9
"missing" `vld1.64` are SLP run-alignment reloads, not extra constants; the
original groups four int stores at `0xb0/0xc0` and `0x140/0x150/0x160/0x170`),
w9910 (53,130 single-move variants: 624 land at 7,072 B, best aligned 2,148),
w9913 (27 more mining-guard spellings falsified; `won == true` is
load-bearing), w9915 (the Hud `pivotY = dividerOffset` anchor is the unique
source lever for the 224-byte frame), w9917 (Gun's inline-`getPosition` stack
layout is the original's; the residual is a pure r4<->r5 swap), w9919 (the
Radar extab tie sweeps are exhausted; HangarWindow row 39/40 needs padX
inside the inner `if`, which shrinks to 9,676 B), w9920 (the original's
source-order jump table is reproduced by a flat `!prof` vector
`default 1, {167,174,8,0,11,13,14}=4, group2=3, group3=2`: raw 558 / aligned
47 / 1,432 B — a sample-profile signature, not `handleSwitchExpect`).

**Aligned total (15 functions): 23,973** (wave-152 base 24,116). Per function:
Layout 2,119, LevelScript 17,185 (indicative), createShip 452, Status 320,
Radar 1,484, MGame::OnTouchEnd 903, createMission 774, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 52, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 19.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,328** (57,232 B), `Radar::draw` **8,577** (9,908 B), `Layout` C1/C2
**5,830** (7,072 B), `Level::createMission` **5,487** (17,172 B),
`MGame::OnTouchEnd` **4,391** (8,080 B), `Level::createShip` **1,564** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **393** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **318** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **86,610** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,264** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 152)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-152 ran 20 agents (4 on `LevelScript`, 3 on `Radar`, 3 on `Layout`,
2 on `createMission`, 2 on `MGame`, the rest on the small tail/extab/PGO).
Six source changes landed:

* `LevelScript::process` (w9805): order-preserving inner-condition weights
  (`expect(m_nScriptTimerA >= 2001, 0)` on the case-154 s9 completion and a
  state-5 timer weight) instead of the state-switch weights, so the mission
  case order is untouched. Raw **55,320 -> 55,347**; **aligned 17,244 ->
  17,223**; 57,232 B exact.
* `Layout::Layout` (w9810): ascending `0x2f4..0x318` reorder plus the
  `0x198`/`0x1a8` swap (the missing 4 B instruction). The SLP pairing becomes
  the original's `strd @198/@19c + str @1a0`. Raw **5,741 -> 5,800**;
  **aligned 2,189 -> 2,148**; 7,072 B exact.
* `Level::createMission` (w9812): a `Status *status = *(Status *volatile *)
  &Globals::status;` read hoists the value load before `getRandomEnemyFighter`
  (pure source-order change). Raw **5,657 -> 5,649**; **aligned 714 -> 711**.
* `Level::createMission` (w9811): the p2 alien-loop shape (single sret slot
  `sp+212`, `temp=r9`, cargo store after `setRotation`, `field_0x70 = 0`)
  lands 17,172 B. Combined with w9812: raw **5,484**, aligned **776**.
* `MGame::OnTouchEnd` (w9813): `count >= 0` keeps the decrementing IV and
  emits the original `cmp.w r9,#0` (branch `bgt` vs `beq`; observationally
  exact because `count == 0` means `refinedAny` is false). Raw **4,392 ->
  4,391**; **aligned 904 -> 903**; 8,080 B exact.
* `Generator::createMission` (w9817): spell the faithful `bonus = rawBonus -
  rem` as `bonus = rawBonus + rawBonus - bonus`, which emits `rsbne
  r5,r5,r6,lsl #1`. Raw **445 -> 393**; **aligned 59 -> 52**; 1,996 B exact.

Measured and **not landed** (notes kept): w9801 (gold pad cap 0x30 + the
String-labelCopy shape lands `.text 0x1337b8` with byte_exact 4507 but raw
8,635 / aligned 1,788; needs a gold binary patch), w9802 (String copy from a
prvalue keeps the LSDA at 19 rows / 131 B but aligned 2,188), w9803
(`bossSeen`/`shipGroup` re-test: raw 8,587 / aligned 1,565), w9806 (case-125
else-if composes with w9807 at 57,232 B but aligned 17,365 on top of w9805),
w9814 (asteroid-owned `dockToAsteroid` + autopilot-owned `hudEvent(6)` at
8,080 B but raw 4,425 / aligned 924), w9819 (HangarWindow y reads
`Globals::layout` directly: raw 1,939 but aligned 418).

Analysis-only passes: w9804 (best-aligned size-exact LevelScript variant
17,185), w9807 (the mission-0 `(*enemies)[i]` loop is the status-r4 lever but
+132 B), w9808 (the three Layout device groups are already store-exact; the
pool gap is per-arm GOT reloads), w9809 (the ascending tail + `0x2c0`
segment-A move is the best structural skeleton but 172 B short; the missing
51 pool words are 9 `vld1.64` vectors), w9815 (the Hud 224-byte frame is the
`sl` live range, not the `Globals::w` spill), w9816 (createShip status-r4
hitpoints-first is +4 B), w9818 (MenuTouchWindow/ModStation/SpaceLounge
residuals are Greedy-RA/scheduler ties), w9820 (a shifted-rank Status profile
restores the canonical `.rel.plt` but is 1,388-1,404 B; the original is the
unprofiled body with only the switch reordered).

**Aligned total (15 functions): 24,070** (wave-151 base 24,124). Per function:
Layout 2,148, LevelScript 17,223 (indicative), createShip 480, Status 320,
Radar 1,484, MGame::OnTouchEnd 903, createMission 776, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 52, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 19.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,347** (57,232 B), `Radar::draw` **8,577** (9,908 B), `Layout` C1/C2
**5,800** (7,072 B), `Level::createMission` **5,484** (17,172 B),
`MGame::OnTouchEnd` **4,391** (8,080 B), `Level::createShip` **2,056** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **393** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **318** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **87,088** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,286** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 151)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-151 ran 20 agents (4 on `LevelScript`, 3 on `Radar`, 3 on `Layout`,
2 on `createMission`, 2 on `MGame`, 2 on the small tail, the rest on
extab/PGO). Three source changes landed:

* `LevelScript::process` (w9707): the faithful `messages, player, enemies`
  prologue accessor order (only +4 B on the wave-150 base) plus three genuine
  faithfulness fixes that net -4 B: the case-92 3-call `getPosition` shape
  (+8), the case-144 duplicate `setStation` removal (-16), and the case-154
  state-9 completion `m_nState++` (+4). Raw **54,792 -> 55,320**;
  **aligned 17,605 -> 17,244**; 57,232 B exact.
* `Radar::draw` (w9703): the asteroid loop's `wreck` becomes the witnessed
  `bool` and `marked` becomes `int`. Raw **8,739 -> 8,620**;
  aligned 1,483 -> 1,530; 9,908 B exact.
* `SpaceLounge::drawLounge` (w9718): associate the right-edge clamp as
  `+(-width - pad - pad)` so the preceding sum is `(pad + x) + width`,
  matching the original's `add.w r1,r9,r5; add r1,r6` order. Raw
  **176 -> 175**; **aligned 42 -> 41**; 1,948 B exact.

Measured and **not landed** (notes kept): w9704 (`m_nState++` alone is
57,232 B with raw 54,809 / aligned 17,600, but does not compose with w9707:
57,228 B); w9705 (c154->strong adjacency is 57,232 B but aligned 22,383);
w9706 (case-125 state-0/1 else-if is 57,232 B but aligned 17,638 and does not
compose with w9707); w9709 (Layout `0x148` SLP move is 7,072 B but raw 5,828 /
aligned 2,276); w9711 (createMission sret temp r5 lands 17,172 B but raw
6,335 / aligned 1,203); w9714 (MGame branch-hint move is 8,080 B but raw
4,400 / aligned 923); w9716 (createShip status r4 is 2,764 B but raw 2,146 /
aligned 501).

Analysis-only passes: w9701 (the 467 gold Thumb->ARM veneers are identical to
the original; removing one is not byte_exact-neutral, minimum cost 2), w9702
(the `.text` tail pad is Android gold's `stub_group_auto_padding`,
`pad = min(veneer_bytes * 0.02, 0x40)`; forcing the cap to 0x30 links the
String-labelCopy shape at `.text 0x1337b8` with byte_exact 4507), w9708 (the
44 missing Layout literal words are in the `0x2c4/0x2f4/0x7c` device groups;
`0x2f4` is the only pool-growing rewrite), w9710 (the original `Layout` is a
device-major constructor, not a flat ternary chain; the ascending tail hits
`0x164@0xdf4` but is 184 B pool-short), w9712 (the p2 alien-loop map is a
one-register rotation of the original), w9713 (the mining guard `cmp #0` is an
LSR flip; every `!= 0` spelling re-lays to 8,060 B), w9715 (the Hud 224-byte
frame slot is `Globals::w`, not `this`), w9717 (Gun/Generator/MGame residuals
are allocator ties), w9719 (the Radar extab tie is a tail-frequency lever),
w9720 (the Status 1,432 B downstream diffs are a 5-entry `.rel.plt`
permutation, not a literal-pool artefact).

**Aligned total (15 functions): 24,083** (wave-150 base 24,439). Per function:
Layout 2,148, LevelScript 17,244 (indicative), createShip 480, Status 320,
Radar 1,530, MGame::OnTouchEnd 904, createMission 714, Gun 57, HangarWindow
291, SpaceLounge 41, Hud 249, Generator 59, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 19.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**55,320** (57,232 B), `Radar::draw` **8,620** (9,908 B), `Layout` C1/C2
**5,800** (7,072 B), `Level::createMission` **5,657** (17,172 B),
`MGame::OnTouchEnd` **4,392** (8,080 B), `Level::createShip` **2,056** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **445** (1,996 B),
`SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **318** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **87,330** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,327** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 150)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-150 ran 20 agents (4 on `LevelScript`, 3 on `Radar`, 3 on `Layout`,
2 on `createMission`, 2 on `MGame`, 2 on the small tail, the rest on
extab/PGO). Ten source changes landed:

* `Radar::draw` (w9607): remove the wave-143
  `__builtin_expect(lockedAsteroid == nullptr && lockedGasCloud == nullptr &&
  field_0x1c == nullptr, 0)` metric pad (it is not witnessed). Raw
  **8,712 -> 8,461**; **aligned 1,657 -> 1,646**; 9,908 B exact.
* `Radar::draw` (w9606): `int wreck` (enemy loop) and `bool asteroidHit`
  (asteroid loop) restore the original type widths; the strd-tail shapes are
  documented but every 9,908 B variant regresses at least one metric. Combined
  with w9607: raw **8,739**, **aligned 1,483** (best structural pair found).
* `Layout::Layout` (w9610): keep the 4-store `0x184` chain and add the
  `0x54<->0x58`, `0x2b0<->ringNearMidThreshold`, `0x110_strip58<->0x11c`
  statement swaps. Raw **5,768 -> 5,741**; **aligned 2,198 -> 2,189**;
  7,072 B exact.
* `LevelScript::process` (w9603): the size-exact prologue accessor permutation
  `enemies, messages, player`. Raw **54,898** (unchanged); **aligned
  15,802 -> 15,795**.
* `LevelScript::process` (w9602): the case-125 `switch` becomes three
  independent `if`s in the order 2, 3, default(4..6), with state-3/hacking hot
  expects. The independent-if form detaches the hacking block from the state-3
  chain, so the first `PlayerStation::setVisible` reference precedes
  `PlayerEgo::deleteHackingGame`: **`.rel.plt` is now byte-identical (4 -> 0)**.
  Raw **54,898 -> 54,792**; aligned 15,802 -> 17,605 (the greedy alignment is
  indicative for this 57 KB function); 57,232 B exact.
* `MGame::OnTouchEnd` (w9613): spell the mining-report guard as
  `amount != 0 && amount != 1` so the backend emits `cmp #2; bcc` instead of
  `cmp #-1; bgt`. Raw **4,393 -> 4,392**; aligned **904** (unchanged).
* `MGame::OnUpdate` (w9617): read `Globals::status` directly at the
  `getCurrentCampaignMission` site and keep the `Status*` anchor only for the
  `getPlayingTime` store. Raw **37 -> 318** (same-address shift); **aligned
  109 -> 19**; 11,440 B exact.
* `Level::createShip` (w9616): declare the waypoint reads `wy, wz, wx` so the
  null-branch spill slots match. Raw **1,978 -> 2,056**; **aligned 482 -> 480**;
  2,764 B exact.
* `Level::createMission` (w9612): move `int guns = alienGuns + fortressGuns;`
  after the inner `if` (the original's witnessed `add.w r5,sl,fp` at `b37e6`);
  the emitted bytes are bit-identical to HEAD.
* `Level::createMission` (w9611): the alien turret loop reverts the first
  rotate to the plain `v = MatrixRotateVector(...)` (the original shares one
  sret slot for both rotates) and moves `setScaling` after `setPosition`.
  Raw **5,495 -> 5,657**; **aligned 731 -> 714**; 17,172 B exact.

Measured and **not landed** (notes kept): w9604 (case-125 else-if + `m_nState++`
does not compose with w9602: 57,248 B); w9609 (`0x148` joins the SLP group but
is +8 B with w9610 and worsens both metrics alone); w9614 (branch-reorder
removes the `expect` hint but costs +27 raw / +13 aligned vs w9613);
w9615 (Hud `pivotY=0` anchor removal is 9,332 B but raw +5,639 / aligned +699).

Analysis-only passes: w9601 (the c154->strong adjacency shape is 57,272 B; the
current `m_nState = 6` is a +4 B size companion and should be `m_nState++`),
w9605 (the `.text` tail is 467 gold Thumb->ARM veneers + a fixed 0x40 pad; one
fewer veneer is exactly -16 B, the clean lever for the Radar String shape),
w9608 (the 44 missing Layout literal words are in the `0x2c4..0x2d8`,
`0x2f4..0x318` and `0x7c..0x94` device groups), w9618 (MenuTouchWindow/ModStation/
SpaceLounge residual is Greedy-RA remat / scheduler choice), w9619 (extab
LP-tie arithmetic: `Radar::Radar` needs `k` in [1.000958,1.0014) or `k=2`),
w9620 (Status: `-profile-guided-section-prefix=false` + the rank profile reaches
1,428 B; a 1,432 B hit still gives byte_exact 4493, so no clean profile).

**Aligned total (15 functions): 24,439** (wave-149 base 22,928; the increase is
the LevelScript `.rel.plt` fix, whose greedy alignment is indicative). Per
function: Layout 2,189, LevelScript 17,605 (indicative), createShip 480,
Status 320, Radar 1,483, MGame::OnTouchEnd 904, createMission 714, Gun 57,
HangarWindow 291, SpaceLounge 42, Hud 249, Generator 59, MenuTouchWindow 15,
ModStation 12, MGame::OnUpdate 19.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,792** (57,232 B), `Radar::draw` **8,739** (9,908 B), `Layout` C1/C2
**5,741** (7,072 B), `Level::createMission` **5,657** (17,172 B),
`MGame::OnTouchEnd` **4,392** (8,080 B), `Level::createShip` **2,056** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **445** (1,996 B),
`SpaceLounge::drawLounge` **176** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **318** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **86,863** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,326** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); **`.rel.plt` 0
(byte-identical, 29,392 B)**; `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 149)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-149 ran 20 agents (2 on `Radar`, 2 on `Layout`, 3 on `LevelScript`, 2 on
`createMission`, 2 on `MGame`, 2 on build provenance, the rest on the smaller
residuals). Seven source changes landed:

* `Layout::Layout` (w9504): the `0x184` chain must test `iPad` before `n9`
  (a genuine semantic bug). A flat fix merges the `iPadLarge`/`iPad` arms and
  loses 8 B; duplicating the `iPad` test in both `n9` arms keeps the original
  4-store chain, and two pure statement swaps pay the 8 B back. Raw
  **5,743 -> 5,768**; **aligned 2,213 -> 2,198**; 7,072 B exact.
* `HangarWindow::render` (w9514): restore the original asm call order
  `replaceTextKeepSize -> setSplitText -> draw` (w9016 had `draw` before
  `setSplitText` as a metric-only swap). Raw **1,967 -> 1,965**;
  **aligned 301 -> 291**; 9,700 B exact.
* `MenuTouchWindow::OnTouchBegin` (w9516): the case-15 block reads
  `Globals::layout` directly for the button-inset/top/left-margin accesses
  (the original's `Layout*` live range). Raw **28 -> 29**; **aligned 16 -> 15**.
* `Level::createShip` (w9512): declare `wy` before `wx` so the null-waypoint
  branch spills `y`'s slot before `x`'s. Raw **1,978** (unchanged);
  **aligned 486 -> 482**; 2,764 B exact.
* `Level::createMission` (w9509): name the `MatrixRotateVector` return temp in
  both turret loops (`Vector p = ...; v = p;`). That restores the pristine
  pirate-fortress preheader allocation (case4Tail region 196 -> 27) at the
  cost of the alien tail (17 -> 103). Raw **5,561 -> 5,495**;
  **aligned 716 -> 731**; 17,172 B exact.
* `MGame::OnTouchEnd` (w9510): the wave-148 `sepif` restructure is the
  faithful CFG but costs +4 B; a `__builtin_expect(isDockingToStream(), 1)`
  branch-weight hint flips the tail-merge ownership enough to absorb the 4 B
  at the original 8,080 B and the original literal-pool address. Raw
  **4,127 -> 4,393**; **aligned 906 -> 904**.
* `Radar::draw` (w9607): drop the invented wave-143 `otherLock`
  `__builtin_expect(..., 0)` pad (plain `if`). The play-site table is
  re-derived and is 7/7 (not 8/7) on both binaries. Raw **8,712 -> 8,461**;
  **aligned 1,657 -> 1,646**; 9,908 B exact.

Analysis-only passes (notes committed, no source change): w9501 (the
String-labelCopy 9,908 shape is blocked on a 12 B `.text` shrink that must be
shift-free), w9503 (Layout ascending tail reaches `0xdf4` but is 268 B
pool-short), w9505 (LevelScript state-switch weights move `c154` before the
strong arm but regress both metrics), w9507 (LevelScript case-92 block is
structurally complete; the faithful prologue accessor order is +72 B), w9508
(createMission sret-temp hoist not reachable from source), w9511 (Hud fp/r9
swap root-caused to the `dividerOffset` pivotY anchor), w9513 (extab LP ties
need `P(autoTurret != nullptr)=0.3125`), w9515 (per-case profile derivation
reproduces both switch tables at 1,428 B but sinks the entry block), w9517
(independent flag re-derivation: 104 A/B cells, canonical flag set confirmed),
w9519 (MGame::OnUpdate 37-byte residual re-derived; no source win), w9520
(ModStation/drawLounge LP order and pre-RA CMOV copy decoded), w9518
(PGO mechanism sweep: `-profile-guided-section-prefix` is ON by default in
clang 7.0.2, so any profile replay must disable it; a full-TU profile can
reach 1,432 B but not the source-order switch layout).

**Aligned total (15 functions): 22,917** (wave-148 base 22,945). Per function:
Layout 2,198, LevelScript 15,802 (indicative), createShip 482, Status 320,
Radar 1,646, MGame::OnTouchEnd 904, createMission 731, Gun 57, HangarWindow 291,
SpaceLounge 42, Hud 249, Generator 59, MenuTouchWindow 15, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,898** (57,232 B), `Radar::draw` **8,461** (9,908 B), `Layout` C1/C2
**5,768** (7,072 B), `Level::createMission` **5,495** (17,172 B),
`MGame::OnTouchEnd` **4,393** (8,080 B), `Level::createShip` **1,978** (2,764 B),
`HangarWindow::render` **1,965** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **445** (1,996 B),
`SpaceLounge::drawLounge` **176** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**29** (1,896 B).

Section deltas: `.text` **86,198** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,309** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); `.rel.plt` **4** (29,392 B;
the `setVisible`/`deleteHackingGame` swap); `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 148)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-148 ran 20 agents (4 on `LevelScript`, 3 on `Radar`, 2 on `Layout`,
2 on `createMission`, 3 on the PGO/profile question). Six source changes landed:

* `Radar::draw` (w9402): the station loop reads `stations->data()[i]` first
  (matching `12e474..12e47e`), the enemy `wreck` becomes `bool`, and the
  asteroid `marked` becomes `bool`. That flips the greedy allocator to the
  original's **frame 296** (`sub sp,#296`; ours was 304) and merges the
  146/147 music tail so we emit **7** `FModSound::play` calls like the original.
  Raw **8,672 -> 8,712**; **aligned 2,726 -> 1,657** (the largest single
  structural win so far).
* `Layout::Layout` (w9408): six adjacent statement reorderings
  (`centerYBase<->marqueeWidth`, `0x238.b<->0x120`, `0x198<->0x1a8`,
  `0x1c8<->0x1d8`, `0x1c0<->0x1d4`, `orbitInfoSecurityY<->0x214`).
  Raw **5,704 -> 5,743**; **aligned 2,304 -> 2,213**; 7,072 B exact.
* `Level::createMission` (w9411): the alien-gun preheader declares
  `Matrix rotation; Vector v; int k; Vector base;` (the original initialises
  Matrix, then v, then base at `b323c..b3284`). Raw **5,560 -> 5,561**;
  **aligned 728 -> 716**.
* `LevelScript::process` (w9406, w9418, w9407): the case-92 state-1 block gets
  the witnessed `field_0x38`/`field_0x128` store order; the mission-0 loop
  calls `setDirection` before `setVisible`; and the case-157 strong arm drops
  its `m_bCinematicBreak = 1` (adds `destroyed = 0`, `m_nState = 6`). Raw
  **54,913 -> 54,898**; 57,232 B exact.

Measured and **reverted** (notes kept): w9401 (Radar `expect(enemies)`+bool→int
shape lands 9,908 B alone at aligned 2,127 but composes with w9402 to 9,920 B);
w9404 (LevelScript timer/destroyed order is -4 B); w9409 (Layout 0x184
iPad-before-n9 fix composes with w9408 to 7,112 B); w9410 (`v`/`rotation`
declaration order); w9412 (MGame `sepif` CFG costs +293 raw, +19 aligned);
w9413 (Hud anchor removal costs +5,640 raw, +700 aligned).

**Profile research (w9415/w9416/w9419/w9420).** No `.profraw`/`.profdata` is
shipped in any APK or sibling build; sample-PGO replay is falsified. A graded
per-TU `-fprofile-instr-use` profile can move one hot case to the front of
`Status::missionCompleted` but the remaining cases stay numeric (1,392 B, not
1,432). The extab LP-identity ties are frequency-driven: `Radar::Radar`'s 7
bytes need `P=0.3125/0.125` in the machine-level MBFI domain.

**Aligned total (15 functions): 22,945** (wave-147 base 24,055). Per function:
Layout 2,213, LevelScript 15,802 (indicative), createShip 486, Status 320,
Radar 1,657, MGame::OnTouchEnd 906, createMission 716, Gun 57, HangarWindow 301,
SpaceLounge 42, Hud 249, Generator 59, MenuTouchWindow 16, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,898** (57,232 B), `Radar::draw` **8,712** (9,908 B), `Layout` C1/C2
**5,743** (7,072 B), `Level::createMission` **5,561** (17,172 B),
`MGame::OnTouchEnd` **4,127** (8,080 B), `Level::createShip` **1,978** (2,764 B),
`HangarWindow::render` **1,967** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **445** (1,996 B),
`SpaceLounge::drawLounge` **176** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**28** (1,896 B).

Section deltas: `.text` **86,225** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,309** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); `.rel.plt` **4** (29,392 B;
the `setVisible`/`deleteHackingGame` swap); `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **19**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 147)

**Current authoritative state.** `check` is green: byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-147 ran 20 agents (4 on `LevelScript::process`, 3 on `Layout`, 2 on `Radar`,
2 on `createMission`, 3 on the `!prof`/branch-weight question, the rest on the
smaller residuals). Four source changes landed:

* `Radar::draw` (w9308): `int rowHeight = Globals::layout->field_0x4` after the
  first landmark `DrawString` breaks clang's tail merge of the two lockLabel
  draws, so the landmark loop emits the original's **9** `DrawString` calls at
  exactly 9,908 B. `enemyHit` and the asteroid `wreck` are switched to `bool`
  (witnessed by the `and r1,#1` / `eors.w r0,sl,#1` widenings); enemy `wreck`
  stays `int` (word store `12ed82`). Raw **8,733 -> 8,672**; aligned 2,712 -> 2,726.
* `HangarWindow::render` (w9313): the `borderX` local splits the blueprint
  rows 19/20, inlining `iconX`/`iconY` removes the free-credits lifetime-marker
  EH scope, and `__builtin_expect(count <= 4, 1)` restores the original block
  placement, so the credit LSDA row merges at exactly 9,700 B. The extab record
  diff drops **762 -> 676** (section **1,393 -> 1,309**) and the alignment
  **480 -> 301**; raw same-address **1,719 -> 1,967** (instruction shifts).
* `SpaceLounge::drawLounge` (w9316): the right-edge clamp uses the int
  `pad + pad` instead of the `short pad3` sign-extend workaround, matching the
  original's reuse of the `pad*2` kept in `sl`. Raw **175 -> 176**; aligned
  **57 -> 42**; size 1,948 B exact.
* `LevelScript::process` (w9320): the case-92 state-1 block is reconstructed
  from `0x13a036..0x13a2c6` — `setPosition` order `[0],[1],[2]`, `m_pHud->visible = 0`
  between `m_bCinematicBreak` and `m_pRadar->enabled`, `m_nScriptTimerA = 0`
  after `setName()`. Raw **54,869 -> 54,913** (address shift); block-local
  alignment 0.6471 -> 0.6757.

Measured and **reverted** (notes kept): w9301 makes `.rel.plt` byte-identical
but costs +358 raw text and +2,167 aligned instructions; w9309 is -2 raw but
+32 aligned; w9311 is +32 raw; w9318 is +90 raw (w9316 is the better
SpaceLounge shape).

**Branch-weight / `!prof` research (w9304/w9305/w9319).** The wave-146 finding
that `Status::missionCompleted`'s case blocks are laid out in *source* order
(only reproducible with `!prof` weights) was followed up: a flat PGO profile is
**falsified** (it breaks 1,274/4,524 functions), and clang 7's only source-level
switch-weight producer is `handleSwitchExpect` (which requires
`__builtin_expect` on a `switch` scrutinee comparison). The single-hot vector is
outside the winning family. A separate strong lead: on `Radar::draw`,
`__builtin_expect(enemies != nullptr, 1)` drops the alignment **2,712 -> 1,963**
but shrinks the function to 9,800 B — the missing ~108 B is the next step.

**Aligned total (15 functions): 24,061** (wave-146 base 24,232). Per function:
Layout 2,304, LevelScript 15,740 (indicative), createShip 486, Status 320,
Radar 2,726, MGame::OnTouchEnd 906, createMission 734, Gun 57, HangarWindow 301,
SpaceLounge 42, Hud 249, Generator 59, MenuTouchWindow 16, ModStation 12,
MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,913** (57,232 B), `Radar::draw` **8,672** (9,908 B), `Layout` C1/C2
**5,704** (7,072 B), `Level::createMission` **5,559** (17,172 B),
`MGame::OnTouchEnd` **4,127** (8,080 B), `Level::createShip` **1,978** (2,764 B),
`HangarWindow::render` **1,967** (9,700 B), `Hud::draw` **1,325** (9,332 B),
`Status::missionCompleted` **722** (1,432 B), `Gun::calcCharacterCollision`
**463** (1,984 B), `Generator::createMission` **445** (1,996 B),
`SpaceLounge::drawLounge` **176** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**28** (1,896 B).

Section deltas: `.text` **86,159** differing bytes (size exact); `.ARM.exidx`
**0** (byte-identical); `.ARM.extab` **1,309** (size- and base-exact
**156,312 B**); `.rodata` **0** (byte-identical); `.rel.plt` **4** (29,392 B;
the `setVisible`/`deleteHackingGame` swap); `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 146)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
17 functions remain non-byte-exact, all size-exact.

Wave-146 lands one source change:

* `MGame::OnTouchEnd` (w9210): the `hudTouchFlags == 0` join test is written as the
  `else if` of the `weaponSlot`/`wingmanCommand` chain. The original's weaponSlot body
  (`0x17b9cc..0x17ba24`) ends with `b.w 17afe8`, bypassing the join test, while the
  wingman body (`0x17bee2..0x17bf4a`) goes through it — the `else if` CFG. Raw
  **4,180 -> 4,127**; `.text` **85,651 -> 85,598**. The join block is now the right
  shape but still block-placed at `0x17aeb2` instead of the original's `0x17afe2`, so
  the 0x5c campaign-region gap is not yet closed (see
  `_work/reconstructions/mgame-ontouchend-wave146-w9210.md`).

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,895** (57,232 B), `Radar::draw` **8,373** (9,908 B), `Layout` C1/C2
**5,704** (7,072 B), `Level::createMission` **5,592** (17,172 B), `MGame::OnTouchEnd`
**4,127** (8,080 B), `Level::createShip` **1,978** (2,764 B), `HangarWindow::render`
**1,719** (9,700 B), `Hud::draw` **1,325** (9,332 B), `Status::missionCompleted`
**722** (1,432 B), `Gun::calcCharacterCollision` **463** (1,984 B),
`Generator::createMission` **445** (1,996 B), `SpaceLounge::drawLounge` **175**
(1,948 B), `MGame::OnUpdate` **41** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**28** (1,896 B), `ModStation` C1/C2 **11** (1,008 B).

Section deltas: `.text` **85,598** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,370** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 146)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.93%**. 17 functions remain non-byte-exact, all size-exact.

Wave-146 ran 20 agents (16 on the worst functions, 4 on build provenance). Landed source
changes:

* `Level::createMission` (w9208): the wave-144 `basefirst`+`cachedgun`+`status` tree now
  composes at the exact 17,172 B, and the +4 B companion is dropping the tail
  `AERandom *rnd` local (clang CSEs `Globals::rnd` itself after `cachedgun`). Raw
  **5,592 -> 5,560**.
* `MGame::OnTouchEnd` (w9210): the `hudTouchFlags` join is the `else` of the
  weaponSlot/wingman chain; the join and tail are reconstructed from the original
  `0x17afd8..0x17b04e`. Raw **4,180 -> 4,127**.
* `MGame::OnUpdate` (w9220): the `Status *st` alias moves back to the mission-122 radio
  block, so the mission-23 idle block reloads `Globals::status` like the original
  `0x17e0b8`. Raw **41 -> 37**.
* `ModStation` C1/C2 (w9216/w9220): the far `MatrixSetRotation` x argument is `pitch`,
  not `yaw` (the original preserves `r2 = gHangarCamPitch[race]` in `r4` across the first
  call). This is a **faithfulness fix**: raw **11 -> 43**, same 1,008 B. The 11-byte
  `yaw` form was metric-only.
* `Radar::draw` (w9206): the enemy loop now has the original's single `DrawImage2D` tail
  merge with the witnessed `bool alwaysEnemy` / `bool asteroidHit` (`and #1` widening at
  `0x12f3ba`). Same 9,908 B; raw **8,373 -> 8,733** but the instruction-index alignment
  improves **2,819 -> 2,712** non-matching instructions.
* `LevelScript::process` (w9203): the case-125 state-1 body is lifted into a contiguous
  `if (m_nState == 1)` block (S1SEP) matching the original `0x13b43e..0x13b516`, at the
  exact 0x144 LSDA table. Raw **54,895 -> 54,869**; `.ARM.extab` diff 1,370 -> 1,399 and
  `.rel.plt` 0 -> 4 (the `setVisible`/`deleteHackingGame` PLT order swap is still open).

`Hud::draw` (w9213) was reverted after measurement: its iconX/iconY + inline-`y` shape
matches the original's dock2 LSDA `(4,4,0xcc)` but costs +899 raw text bytes, +37 extab
bytes and +211 aligned instructions; the note is kept.

**New shared metric.** `_work/reconstructions/align_diff.py` (from w9220) aligns the two
instruction streams by target-instruction index and counts non-matching instructions.
Wave-146 baseline total: **24,443** (wave-145 base: **24,302**). Per function: Layout
2,304, LevelScript 15,737 (indicative only), createShip 486, Status 320, Radar 2,712,
MGame::OnTouchEnd 906, createMission 728, Gun 57, HangarWindow 480, SpaceLounge 57, Hud
460, Generator 59, MenuTouchWindow 16, ModStation 12, MGame::OnUpdate 109.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process` **54,869**
(57,232 B), `Radar::draw` **8,733** (9,908 B), `Layout` C1/C2 **5,704** (7,072 B),
`Level::createMission` **5,560** (17,172 B), `MGame::OnTouchEnd` **4,127** (8,080 B),
`Level::createShip` **1,978** (2,764 B), `HangarWindow::render` **1,719** (9,700 B),
`Hud::draw` **1,325** (9,332 B), `Status::missionCompleted` **722** (1,432 B),
`Gun::calcCharacterCollision` **463** (1,984 B), `Generator::createMission` **445**
(1,996 B), `SpaceLounge::drawLounge` **175** (1,948 B), `ModStation` C1/C2 **43**
(1,008 B), `MGame::OnUpdate` **37** (11,440 B), `MenuTouchWindow::OnTouchBegin` **28**
(1,896 B).

Section deltas: `.text` **85,972** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,393** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **4** (29,392 B; the
`setVisible`/`deleteHackingGame` swap); `.data` **0**; `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

**Build provenance (w9217/w9218).** An independent full-build A/B of 28 single-flag
variants confirms the canonical set (`-Oz -g -std=c++11 -fPIC -frtti -fexceptions
-fstack-protector-strong -funwind-tables -ffunction-sections`, NDK r18b clang 7.0.2,
GNU strip, gold link): no flag raises `byte_exact`, and the `-O2/-Os/-O1/-O3`, `-g0/-g3`,
`-fno-stack-protector`, `-fno-rtti`, `-fno-exceptions`, `-march=armv7ve`,
`-mfpu=neon-vfpv4`, `-mtune=cortex-a8`, `-fwrapv` and `-fno-vectorize` cells are all
worse or neutral. The `.comment` string (`Android (4751641 based on r328903) clang
version 7.0.2`, `GCC: (GNU) 4.9.x 20150123`) and the 156,312-byte `.ARM.extab` census
confirm the recipe.

## Update 2026-11-08 (wave 145, final)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-145 lands five source changes (20 agents; the rest are analysis notes):

* `LevelScript::process` (w9002): the four calls the original's mission-0 state-4
  teardown owns (`setFreeLookMode`, `enableFirstPersonCam`,
  `hideShipForFirstPersonCameraView`, `removeAllGuns`; original `0x138d6c..0x138daa`)
  are moved out of the head of state 5 into state 4 in the witnessed order, and the
  three state tests are written **4, 5, 3** so state 4 is laid out before state 5.
  This is the only branch permutation that keeps `.rel.plt` byte-identical at the
  exact 57,232 B size (3,4,5 is 57,272 B and moves setVisible/deleteHackingGame).
  Raw **54,909 -> 54,923** (+14, faithfulness over layout).
* `LevelScript::process` (w9004): a genuine +16 B state-5 `getSpeed` re-read
  (the original re-reads the speed after the state-4 teardown). Raw
  **54,923 -> 54,895**; `.ARM.extab` diff **1,368 -> 1,370**.
* `Radar::draw` (w9009): the asteroid loop's dead-pair duplication is forced with
  `!(hasCargo && (isDead() || isDying())) && (isDead() || isDying())`, so clang
  emits the original's six `isDead`/`isDying` calls (ours had four). The pre-update
  block is now instruction-for-instruction identical. Raw **8,471 -> 8,373**.
* `MGame::OnTouchEnd` (w9011 + w9012): the `failed` test is a word-typed flag
  (`int failed`; `failed == 1`), which places the type12 chain after the
  mission-selection chain as in the original, and the campaign chain is written
  `if (!isCampaignMission())` so the `==161` test becomes the original's `cbz`.
  Raw **4,399 -> 4,180** (measured on the merged tree; each alone is worse:
  4,691 and 4,272).
* `HangarWindow::render` (w9016): `setSplitText` after `draw()` (METRIC-ONLY;
  the original's asm order is replaceText, setSplitText, draw). Raw
  **1,731 -> 1,719**.

Residual raw same-address diff bytes (all size-exact): `LevelScript::process`
**54,895** (57,232 B), `Radar::draw` **8,373** (9,908 B), `Layout` C1/C2
**5,704** (7,072 B), `Level::createMission` **5,592** (17,172 B), `MGame::OnTouchEnd`
**4,180** (8,080 B), `Level::createShip` **1,978** (2,764 B), `HangarWindow::render`
**1,719** (9,700 B), `Hud::draw` **1,325** (9,332 B), `Status::missionCompleted`
**722** (1,432 B), `Gun::calcCharacterCollision` **463** (1,984 B),
`Generator::createMission` **445** (1,996 B), `SpaceLounge::drawLounge` **175**
(1,948 B), `MGame::OnUpdate` **41** (11,440 B), `MenuTouchWindow::OnTouchBegin`
**28** (1,896 B), `ModStation` C1/C2 **11** (1,008 B).

Section deltas: `.text` **85,651** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,370** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 144)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-144 lands one source-level improvement:

* `LevelScript::process` (w8905): the case-80 metric-only `m_pRadar->dockNavPtr = nullptr`
  store is removed (the original case-80 block has no `Radar+8` store), and the genuinely
  missing `m_nScriptTimerA = 0` is restored at the case-91 state-3 tail (original
  `0x13fed6` is a single `strd r1,r1,[r2,#144]`). The `dockNavPtr` store is relocated to
  the same tail as the size/encoding companion (METRIC-ONLY there: the original tail has
  no `Radar+8` store). Raw **54,619 -> 54,570** at 57,232 B (size unchanged).

Section deltas: `.text` **85,802** differing bytes (size exact, -49); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,499** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 144)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-144 lands three source-level improvements, all faithfulness-first:

* `LevelScript::process` (w8901/w8902/w8904/w8905): drop the dead state-5 `getSpeed`
  re-read; add the real case-70 `break` (original `0x139abe b.w` post-switch tail) and
  write the state-1 arm before state-2 so the cold-first dispatch matches; the case-91
  nested test lands the **exact 0x144 LSDA call-site table**; restore the genuine
  case-91 state-3 `m_nScriptTimerA = 0` store and drop the case-80 metric store. Raw
  **54,619 -> 54,909** (faithfulness over layout); `.ARM.extab` diff **1,499 -> 1,370**
  at the exact 156,312 B size.
* `Radar::draw` (w8906): `int enemyHit` is the witnessed word-typed flag (the original
  loads it as a word); raw **8,613 -> 8,471** at 9,908 B. w8907's faithful enemy-loop
  tail merge / dead-local drop / `hasCargo`-first reorder is note-only: it needs a
  +4 B companion on top of w8906.
* `Level::createMission` (w8913): the pirate-fortress gun loop keeps the hardpoint
  table as a loop-carried pointer (`const float *t = kBaseTurrets[0]; ... t += 6;`),
  matching the original's `r6` base + `adds r6,#24`; raw **5,597 -> 5,592**.

Note-only: w8903 (setStanding PLT-window sweep), w8908 (vbsl decode), w8909 (clz mask),
w8910 (cargo deferral), w8911 (survivor/branch width), w8912 (ARM `-Oz` if-conversion),
w8914 (HangarWindow r5/r6 + credit-loop LSDA), w8915 (Hud THBY rematerialisation),
w8916 (createShip coordinate type: `iif` reaches raw 1,647 but 2,768 B), w8917 (Status
group arm), w8918 (near-miss trio), w8919 (long tail), w8920 (extab splits).

Section deltas: `.text` **85,994** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,370** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 143)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-143 lands one source-level improvement:

* `Level::createMission` (w8812): the raid `guns = alienGuns + fortressGuns` sum is
  rematerialised *after* the `if (raid)` guard (the original computes `add r5,sl,fp` at
  `b37e6`, not before it), and the tail hunter loop is restored to the faithful
  `i < this->enemies->size()` bound with a `KIPlayer *ship` store local and a cached
  `AERandom *rnd = Globals::rnd` (the original's `fp`). Raw **5,756 -> 5,597** at
  **17,172 B** (size unchanged).

Section deltas: `.text` **84,960** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,415** (size- and base-exact **156,312 B**; +20 from the
extra tail scope, not gated); `.rodata` **0** (byte-identical); `.rel.plt` **0**
(byte-identical, 29,392 B); `.data` **0**; `.bss` **0**; `.note.gnu.build-id` **20**.
File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 143)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-143 lands seven source-level improvements, several of them faithfulness fixes:

* `LevelScript::process` (w8801/w8802/w8803/w8804): move4b reorder; the case-80 extra
  `setTarget` removed (`setTarget` census now 81=81); case-91 state 6 before 5; the
  faithful case-125 state-2 ambush RNG order. Raw **54,691 -> 54,619** at 57,232 B
  (wave-144 later improves this to 54,570).
* `Radar::draw` (w8807 + w8808): the original guard is `bool otherLock` with one
  `__builtin_expect(A==0 && B==0 && C==0, 0)`, which emits the original `cbnz` chain;
  the campaign block is laid out cold by `expect(...,0)`; the invented post-loop
  `onPlanetCourse` recompute is removed and the landmark label uses `this->lockLabel`.
  Raw 8,106 -> 8,471 (faithfulness over layout; the int enemy-loop `wreck` spelling is
  the 4 B companion that keeps 9,908 B, and `int enemyHit` is the witnessed word-typed
  flag, -142 raw at the same size); `.ARM.extab` 1,395 -> 1,499.
* `MGame::OnTouchEnd` (w8810): the flat `flags == 0` shape flips the join test to the
  original's `cbz`; raw **4,464 -> 4,399** at 8,080 B.
* `Level::createMission` (w8812): raid-gun rematerialisation and a faithful tail loop;
  raw **5,756 -> 5,597** at 17,172 B.
* `HangarWindow::render` (w8813): free-credits declaration order; raw **1,740 -> 1,731**.
* `Hud::draw` (w8814): dock2 YBTH order; raw **1,362 -> 1,325** with `.ARM.extab` exact.
* `Level::createShip` (w8819): the missing 16 B is the double `getLevel()` clamp (four
  witnessed `blx Status::getLevel` sites), plus x,y,z waypoint reads, X,Y,Z jitter
  declarations and no bogus `status == nullptr` guard. Raw 1,411 -> 1,978
  (faithfulness over layout; the register allocator moves `Globals::status` r4 -> r6).

Metric-only / note-only: w8805 (vbsl select is contiguous-ternary SLP), w8806 (literal-pool
alignment), w8809 (`goingToWaypoint() == 1`; the original emits `cbz r0`), w8811
(alien-gun-loop anatomy), w8815 (near-miss trio), w8816 (Status fold / createShip 16 B),
w8817 (Gun/Generator/SpaceLounge RA probes), w8818 (extab ownership + build-id), w8820
(Status 4:3:2:1 family).

Section deltas: `.text` **85,709** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,499** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-08 (wave 142)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-142 lands five source-level improvements plus a call-census milestone:

* `LevelScript::process` (w8702): independent-call reorders (case-91 `setName` before
  `setActive`; case-125 `setActive` before `setName`); raw 54,108 -> 54,102.
* `LevelScript::process` (w8704): state-4/5 translate swap (state 4 uses the
  `25000/-200/-1000` literals, state 5 the delta-based translate); raw unchanged.
* `LevelScript::process` (w8703 + merge fix): case-157 tail call sharing plus the two
  missing `getEnemies()` loop-condition re-fetches. The original's case-144 block is
  `nextCampaignMission(true); setStation(getStation()); departStation(...)`, so the
  `setStation` is restored. The call census now matches exactly: `getEnemies` 34=34,
  `getPlayer` 23=23, `setStation` 6=6, `nextCampaignMission` 10=10 (`setTarget` 82 vs 81
  and `setStanding` 2 vs 1 remain). Size 57,232 B; raw 54,102 -> 54,691 (faithfulness
  over layout).
* `Layout::Layout` (w8709 + w8708): the `eventBannerScale`/`field_0x98` swap and the
  move climb compose; raw **5,728 -> 5,704** at 7,072 B.
* `Level::createMission` (w8712): case-183 default store order; raw **5,763 -> 5,756**.

Metric-only or gate-breaking (notes only): w8705 (state-4 `getEnemies` revert; the
original has the call), w8713 (escort hoist; the original RNG order is
jitter,sign,jitter,sign,jitter,sign), w8714 (`draw()`-first; the original asm order is
`setPosition, replaceTextKeepSize, setSplitText, draw`), w8715 (dock2 `String`-first;
`.ARM.extab` 156,312 -> 156,316 and byte_exact 4507 -> 4044), w8706 (`cbnz` chain is
bool-only), w8711 / w8716 / w8717 / w8718 / w8719 / w8720 research notes.

Section deltas: `.text` **85,119** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 141)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-141 lands nine source-level improvements (plus eleven note-only research commits):

* `LevelScript::process` (w8601): case-40 cold-chain nest + s4gp receiver spelling; raw
  **54,138 -> 54,034** at 57,232 B.
* `LevelScript::process` (w8603): third missing `getEnemies()` loop condition (case-16
  state 4), semantic and size-neutral; the restored call re-rolls the following blocks, so
  the combined raw is 54,034 -> 54,110.
* `LevelScript::process` (w8604): mission-0 state-0 body sweep; raw 54,110 -> 54,108.
* `LevelScript::process` (w8606): case-105 state 7/9/12 audit (faithful posdir and
  cached-geometry spellings); byte-neutral.
* `Layout::Layout` (w8607): whole-constructor move climb; raw **5,759 -> 5,728** at 7,072 B.
* `Radar::draw` (w8610): `expect(A,0) && expect(B,1) && expect(C,1)` guard, campaign
  `isTarget` operand swap (`stations` stays in sl), `bool wreck`; raw **8,279 -> 8,106**.
  w8609's `int bossSeen/inSight/asteroidHit` variant is 8,244 alone but 8,566 combined, so
  the merged tree keeps w8610 only.
* `MGame::OnTouchEnd` (w8614): 96/127 merge; raw **5,816 -> 4,464** at 8,080 B.
* `HangarWindow::render` (w8616): credit-loop `Layout*` local frees sl; raw **2,059 ->
  1,740** at 9,700 B.
* `Hud::draw` (w8617): faithful short `barH` + dock2 `String`-first LSDA proof; byte-neutral
  (raw stays 1,362).

Note-only: w8602 (w8506 case-125 re-measure), w8605 (exact LSDA call-site table), w8608
(`0xdc` strd flip; its 5,752 companion loses to w8607's 5,728 and relocates the
`field_0x138` branch bodies), w8611 / w8612 (createMission alien/tail blocks and
escort-hoist RNG order), w8613 (OnTouchEnd block map), w8615 (Status `.profdata` +
createShip forms), w8618 (near-miss probes), w8619 (codegen flag fingerprint), w8620
(long tail), w8621 (APK/toolchain forensics), w8622 (linker matrix).

Section deltas: `.text` **84,567** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 140)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-140 lands the two remaining w8403 receiver fixes in `LevelScript::process`:
`m_pLevel->getPlayer()->resetGunDelay()` at `0x140288` (+8) and
`player->geometry->setDirection(...)` at `0x144dc2` (-8), netting exactly 57,232 B.
`LevelScript::process` raw same-address diff **54,262 -> 54,115** (-147); whole `.text`
differing bytes **84,651 -> 84,504**; `.rel.plt` stays byte-identical.  See
`_work/reconstructions/levelscript-process-wave140-w8504.md`.

Section deltas: `.text` **84,504** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 140)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.93%**. 17 functions remain non-byte-exact, all size-exact.

Wave-140 landed seven source-level improvements:

* `LevelScript::process` (w8501): full case-94 receiver block (fresh `getPlayer` for
  `resetGunDelay`/`dockToDockingPoint`, `dockNavPtr = nullptr`), the missing case-154
  state-9 `m_bCinematicBreak = 0` store, the spurious `m_nScriptTimerB = 0` removed, and
  the duplicate case-154 state-7 `resetCamera` removed. Raw **54,262 -> 54,140**.
* `LevelScript::process` (w8505): mission-0 state-0 data-pointer placement; raw 54,140 -> 54,138.
* `Layout::Layout` (w8507): raw **5,765 -> 5,759** at 7,072 B.
* `Radar::draw` (w8509): `int alwaysEnemy` + `int asteroid wreck`; raw **8,346 -> 8,279**.
* `MGame::OnTouchEnd` (w8513/w8514): the campaign-mission `96||127` clause is split into the
  original's two separate else-if clauses (shared body tail-merged), `hasFailed()` and
  `menuWindow->OnTouchEnd(...)` are implicit, and the mission selection re-calls
  `getCampaignMission()`. Instruction multiset now matches the original
  (`isCampaignMission` 23=23, `getCampaignMission` 3=3); raw rises 4,363 -> 5,816 because
  the content fixes shift the campaign chain.
* `HangarWindow::render` (w8516): credit-block `String` declaration swap; raw 2,062 -> 2,059.
* `Hud::draw` (w8517): dock1 locals `frameH`/`total`/`barW` are `int`; raw **1,416 -> 1,362**.
* `Radar::draw` (w8609): extend the int-local hypothesis to `bossSeen`, the enemy-loop
  `inSight`, and the asteroid-loop `asteroidHit`; raw **8,279 -> 8,244** at 9,908 B,
  `.rel.plt` byte-identical, `.ARM.extab` unchanged.

Reverted with notes (size-exact but worse raw diff on the merged base): w8502 (57,240 B),
w8503 (57,220 B), w8510 (Radar raw +443).

Section deltas: `.text` **86,437** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,411** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 139)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-139 lands the wave-137 w8308 case-40 cold-chain placement at exactly 57,232 B.  The
`-8 B` companion is the case-157 tail store order (both `Globals::switch_to_target_setting`
and `Level::initStreamOutPosition` are stored before the shared `SetCurrentApplicationModule`
tail at `0x145bd0..0x145bec`).  Two further faithful fixes cancel in size: the missing
`resetGunDelay` at `0x144982` (+12) and the duplicate `resetCamera` at `0x1408c6` (-12).
`LevelScript::process` raw same-address diff **55,072 -> 55,171** (103 B lower than the
w8308 landing's 55,274); official fuzzy pct unchanged at **57.4 %**.  See
`_work/reconstructions/levelscript-process-wave139-w8404.md`.

Section deltas: `.text` **85,575** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,396** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 139)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.93%**. 17 functions remain non-byte-exact, all size-exact.

Wave-139 landed six source-level improvements:

* `LevelScript::process` (w8407): the enemy loops re-fetch `m_pLevel->getEnemies()` per
  iteration instead of using the cached local, and the missing
  `m_pLevel->getPlayer()->resetGunDelay()` at `0x144982` is restored. Raw same-address diff
  **55,072 -> 54,404** at exactly 57,232 B.
* `LevelScript::process` (w8406): mission-0 state-0 visibility loop hoists `enemies->data_`
  into a local (the original's r4/r9 map) and the dead `m_nState = 9` store in state 7 is
  removed. Raw diff **54,404 -> 54,262**.
* `Radar::draw` (w8409): a `Vector delta` local gives the original's nested `inSight`
  shape at exactly 9,908 B with `.rel.plt` exact.
* `Level::createMission` (w8411): case-183 switch source order 0,1,2 then default; raw diff
  5,763 B at 17,172 B.
* `Hud::draw` (w8417): `frameH` is an unsigned short tie-break; raw diff 1,416 B.
* `MenuTouchWindow::OnTouchBegin` (w8418): fresh `top` load from `Globals::layout`; raw diff
  **29 -> 28**.
* `Layout::Layout` (w8401): fuzzy-first reorder keeps the ascending `0xb8..0x118` region and
  adds five adjacent-statement moves; raw diff **5,775 -> 5,765** at exact 7,072 B.

Four further analyses landed as notes with the source reverted (all size-exact but worse raw
diff or wrong size on the merged base): w8403 (57,228 B), w8404 (57,208 B), w8405 (57,228 B),
w8408 (case-125 two-chain; raw +333 and it rotates the `deleteHackingGame` `.rel.plt` entry).

Section deltas: `.text` **85,283** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,404** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 138)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-138 landed one `Level::createMission` source change (keeps exactly 17,172 B): the
`case 183` convoy `offset[3]` `if`-chain tests `k == 2` first, then `k == 0`, then `k == 1`
(the four cases are mutually exclusive, so this is semantically free).  Clang emits the
cold bodies in a different order than the source and the literal pool follows the emitted
body order; the `2,0,1,else` order rotates the pool to the original's shape.  Official
fuzzy **92.3432 -> 92.4917 %** (+0.15 pp).  The positional `.text` diff rises
85,535 -> 85,538 because the pool rotation moves three PC-relative load offsets; the
instruction-level match is better despite that.  See
`_work/reconstructions/level-createmission-wave138-w8312.md`.

Section deltas: `.text` **85,538** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 138)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-138 landed three source-level improvements:

* `LevelScript::process` (w8303): the wave-137 w8202 companion set re-based on the current
  master. Raw same-address diff bytes **55,151 -> 55,072** at exactly 57,232 B. The set moves
  `nextCampaignMission(true)` from case-145 state 5 to the state-6 block, orders the case-125
  state-2 tail as `setLookAtCam(true); setVisible(false); setFreeze(true)`, and swaps the
  case-131/135 blocks.
* `Layout::Layout` (w8301): re-lands the fuzzy-first reorder (ascending `0xb8..0x118` region
  plus the `0x11c`/`0x12c` swap) at **45.9 %**, exact 7,072 B; raw diff 5,775 B.
* `Level::createMission` (w8312/w8411): case-183 offset-chain source order 0,1,2 then
  default (ascending compare chain `cmp #2,/cmp #1,/cbnz`), size 17,172 B; raw diff
  5,766 -> 5,763 B.
* `Hud::draw` (w8317): dock1 `top` is an `int` (zero-extended), not a float; 9,332 B.

Two further analyses landed as notes with the source reverted: w8305 (`m_nState++` is +8 B
and unfaithful — the original stores the immediate 4) and w8308 (case-40 cold-chain order
lands at 57,240 B). Radar w8309 found the nested `inSight` shape at 9,908 B but it moves
`.rel.plt` entries 2736-2739, so it is pinned for a later pass.

Section deltas: `.text` **85,473** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

**Wave 139.** `LevelScript::process` (w8408) lands the faithful case-125 two-chain
dispatch (ascending `if`/`else if` state chain) plus the case-91 nested state-0 test,
at exactly 57,232 B with `.ARM.extab` unchanged (call-site table `0x148 -> 0x146`, entry
still `0x15c`). Raw same-address diff bytes **55,072 -> 55,256**; fuzzy pct unchanged at
57.4; byte_exact/linked_exact stay **4507**.

## Update 2026-11-07 (wave 137)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-137 landed four `LevelScript::process` source fixes (all keep exactly 57,232 B):

* case 40 state-1 teardown reordered to the exact Thumb order at `0x13e6ee..0x13e726`
  (`setTarget` first, then hud/radar/cinematicBreak, `setLookAtCam` before
  `setComputerControlled`, `setVulnerable`, `m_nState = 2`); that 16-instruction cold
  block is now byte-identical. Diff bytes **54,327 -> 54,323**.
* the shared tail at `0x13c88c` uses `translate((float)-delta * 0.8f, 0, 0)`, not the
  `0.3f` the source had. Instruction stream unchanged.
* case 145 state 1 now uses the cached `player` receiver, and state 5 gained the missing
  `m_nState++` (`0x13f8f6..0x13f8fe` branches to the shared add-1/store tail at
  `0x14500e`); the state machine previously stalled at state 5. Case-102 state 0/1
  `setTarget`/`translate` placement corrected per the wave-136 audit.
* the `if (delta < 0)` `.rel.plt` first-reference anchor is gone. Reordering the
  mutually-exclusive mission-42 state tests from 0/6/7 to 6/0/7 makes clang place the
  real state-7 `PlayerStation::setVisible` block before the mission-125 hacking default
  block, so the real calls alone give the original order; the freed bytes are spent on a
  faithful un-cache of the three `Level::getLandmarks()` calls in mission-24 state 1.
  **`.rel.plt` is now byte-identical** (3,674 entries).

* the four semantically-required `getPlayer` receiver fixes are landed (the original reuses
  the cached `player` local at `0x137da6`, `0x13e97e`, `0x13f3c2` and the case-145 state-1
  site); `Level::getPlayer` is now **23/23**.  The fixes re-roll the 57 KB block layout
  (raw `.text` diff rises), but the instruction-level fuzzy stays **58.9 %** and the size is
  exactly 57,232 B.  Companions: the case-102 state-1 tail is the faithful
  `resetCamera(m_pLevel)` (not the spurious `m_tempVec` read), the spurious case-145 state-5
  `nextCampaignMission(true)` and the unreachable `case 6` are removed, and the first
  `resetGunDelay` site uses the cached `player`.

`Layout::Layout` keeps the wave-136 w8107 reorder (raw diff 5,757 B; the fuzzy ratio is verifier-tie-break-sensitive and reads 43.6-45.0 %).

Section deltas: `.text` **85,538** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

* `Level::createMission` tail loop (case 4): the `int idx` temp evaluated the waypoint
  index before `getRandomEnemyFighter`, which also swapped the tail register map
  (`this` r9 / `i` r8 vs the original r8 / r9) and reloaded `playerRoute`.  The faithful
  one-liner restores the original left-to-right argument order and register map.
  Official fuzzy **92.1033 -> 92.3432 %** (+0.24 pp), size unchanged at 17,172 B.  The
  positional `.text` diff rises 84,709 -> 84,711 because the `strd r0,fp,[sp]` becomes
  `movs r0,#1; str r0,[sp,#4]` (`&Globals::rnd` in `fp` instead of `[sp,#120]`); the
  instruction-level match is better despite that.  See
  `_work/reconstructions/level-createmission-wave137-w8214.md`.

`Layout::Layout` C1/C2 stays at the wave-136 w8107 reorder (**45.0 %**). The wave-137
w8209 fuzzy-first reorder measured 45.9 % in its own worktree but lands at 43.6 % on the
merged base, so its source was reverted and only the analysis note is kept.

Section deltas: `.text` **85,538** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical, 29,392 B); `.data`
**0**; `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

**Current authoritative state.** `check` is green: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

`LevelScript::process` (w8201) now carries the four semantically-required `getPlayer`
receiver fixes (the original reuses the cached `player` local at `0x137da6`, `0x13e97e`,
`0x13f3c2` and the case-145 state-1 site).  `Level::getPlayer` is **23/23**.  The fixes
re-roll the 57 KB block layout, so the function fuzzy drops **58.9 -> 58.2 %** and its raw
diff rises 54,327 -> 55,085, but the size stays exactly 57,232 B and the gate is unchanged.
The size companion is the w8105 combination (`anchor_trim2 + case4_hoist + case24_loop2 +
case135 + enemies1718`) plus two -8 B levers that absorb the +20 B base shift introduced by
the wave-136 case-40 translate and anchor trim.  The single-`setVisible` anchor also
restores `.rel.plt` to **byte-identical** (was 4 bytes off).  See
`_work/reconstructions/levelscript-process-wave137-w8201.md`.

Section deltas: `.text` **85,467** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical); `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).


## Update 2026-11-07 (wave 136)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 17 functions remain non-byte-exact, all size-exact.

Wave-136 landed two source-level improvements:

* `Level::createScene` is now **byte-identical** (2252 B at `0xb42ec`). The `spots`
  hangar-table selector was a ternary whose single SSA value fed both phi operands of the
  switch hole-check, so `PHIElimination` split the range-check critical edge into an extra
  block (one extra `b.n` + duplicated `spots` reload). Rewriting it as an if/else gives the
  two arms distinct SSA values and keeps both phi copies inline, matching the original. The
  `geo = ship->geometry` load was moved after `setToSleep()` to un-fuse the `ldrd` (restores
  the 2252 B size). Function diff bytes **236 -> 0**.
* `LevelScript::process` case 40: the genuinely missing state-1
  `m_pCamera->translate(0.0f, 0.0f, (float)(-2 * delta))` at `0x13e6da` is now present
  (the handoff's `0x13a4b8` address was a misread; both `-10000,5000,25000` and
  `-7000,500,17000` were already there). The statement costs +36 B and is balanced by
  removing two duplicated case-125 first-reference-anchor calls
  (`switchSkyboxForIntro`, `PlayerStation::setVisible`) and the `resetGunDelay` size pad,
  keeping the function at exactly 57,232 B. Function fuzzy **58.8 -> 58.9 %**.

Two further LevelScript analyses landed as notes with the source reverted: w8106
(case-102/145/154 placement + the missing case-145 state-5 `m_nState++`) is
semantically faithful but needs a +8 B companion on top of the case-40 change;
w8105 (the four `getPlayer` receiver fixes) is also faithful but re-rolls the
57 KB block layout (58.9 -> 45.6 % at 57,236 B) and needs a different size
companion. w8105 was re-landed in wave 137 (see above); w8106 is still pending.

* `Layout::Layout` C1/C2 (w8107): six adjacent statement swaps found by
  hill-climbing the raw byte diff (`0x1c` after `0x10`, `buttonInsetX` after
  `0x2c`, `0x114` after `0x108`, `0x1dc` after the `0x1cc` chain, `rewardBoxX`
  after `rewardBoxHeight`, `0x2f4` after `0x2f8`). Function diff bytes
  **5,783 -> 5,757** (-26), exact 7,072 B. The full ascending device-tree
  rebuild was tested and is 184 B short (6,888 B) and worse on raw bytes; see
  `_work/reconstructions/Layout-LayoutC1-wave135-w8107.md`.

A second Layout reorder (w8108) sorts the whole `0xb8..0x118` store region
ascending and reaches **5,754** raw diff bytes at the same 7,072 B, but it
lowers the fuzzy ratio to 41.7 % versus w8107's 44.8 %, so w8107's smaller
reorder is the one kept; w8108 is recorded as a note.

Build provenance was re-audited (w8119): the original is **not PGO-built**
(1957/1973 branch-containing functions carry no `!prof` metadata; only the
Status switch does), and a 60-flag matrix plus APK/NDK re-mine found no flag
that beats the current recipe.

Section deltas: `.text` **84,711** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,395** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **4** (size exact, 29,392 B); `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 135)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-135 landed two source-level improvements:

* `LevelScript::process` case 70: the original else path tests state 2 before state 1
  (`0x13c04e`), so the state-2 block now precedes the state-1 block. Function diff
  bytes **54,359 -> 54,262** (-97), exact 57,232 B, `.rel.plt` byte-identical. The
  state-1 body is a no-op immediately after state 0 (the freshly built route's first
  waypoint state is 0), so the fall-through does not change behaviour.
* `LevelScript::process` case 125: the state dispatch is a `switch` in the original,
  not an `if` ladder; the state switch shape now matches (function 58.2 -> 58.8%,
  diff bytes 54,262 -> 54,316). The `.ARM.extab` LSDA shrank from 1,416 to **1,404**
  differing bytes.
Section deltas: `.text` **84,960** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,404** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical); `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 134)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-134 landed one source-level improvement:

* `Level::createMission` station-pirate escort `setPosition`: the three RNG
  offsets are now built as a single `Vector pos = {pirateBase.x + ..., ...}`
  aggregate instead of three `int x/y/z` locals. This reproduces the original's
  interleaved `vldr pirateBase.{x,y,z}` / `nextInt` / `vadd` schedule (the
  original loads each `pirateBase` component immediately before its RNG pair).
  Function diff bytes **5,771 -> 5,763**, exact 17,172 B.

Section deltas: `.text` **85,135** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,416** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical); `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 133)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-133 landed three source-level improvements (all keep exact sizes and `.rel.plt`):

* `LevelScript::process` case-154 state-4/6 and partial case-64 bodies (w7723) plus
  case-144 `setActive(true)`, `getStation(100)` un-merge and anchor trim (w7724):
  function diff bytes **54,507 -> 54,359**, exact 57,232 B.
* `Layout::Layout` `0xc0` SLP store-group reorder (w7730): function diff bytes
  **5,848 -> 5,789**, exact 7,072 B; then the full SLP store-group census +
  five group-contiguity reorders (w7752): **5,789 -> 5,784**, exact 7,072 B.
* The rest of the wave was analysis: w7725-w7728 LevelScript prologue/block-order/
  literal-pool/anchor maps, w7731-w7732 `Radar::draw`, w7733 `Level::createMission`,
  w7734 `MGame::OnTouchEnd`, w7735 `HangarWindow::render`, w7736 `Hud::draw`,
  w7737 `Level::createShip`, w7738 `Status::missionCompleted`, w7739
  `Gun::calcCharacterCollision`, w7740 `SpaceLounge`/`Generator`, w7741 the
  near-miss trio, w7742 `.ARM.extab`.

Section deltas: `.text` **85,138** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,416** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical); `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 132)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-132 landed three byte-level improvements:

* `LevelScript::process` w7705 semantic set (case 131 `dir * 2.01`, case 157 `m_pGeometry2`, the missing 8th `setPosition(f,f,f)`, mission-0 state-6/10 sound fixes, case-40 state 4): function diff bytes **54,623 -> 54,507**, exact 57,232 B.
* `Level::createMission` case-183 offset if-chain ascending order: `.text` **-3 bytes**.
* `RecordHandler::recordStoreRead` shipList lifetime-marker order: `.ARM.extab`
  **1,420 -> 1,416** differing bytes, 13 -> 12 records.

The wave-131 `Level::createShip` `try`/`catch` body (73.4%, 1007/1007 insns) was
**reverted**: its 328 B / 54-row record left `.ARM.extab` 8 B short and shifted
every later `.ARM.extab` reference, so `.ARM.exidx` went from 0 to **1,327**
differing bytes.  Master keeps the 336 B / 57-row / `ttype 0xff` exact record and
the 70.6% body; the 73.4% allocation is preserved as a lead in the w7693/w7709
notes.  A no-`try` shape that reproduces the allocation without the 328 B record
is still open.

Section deltas: `.text` **85,244** differing bytes (size exact); `.ARM.exidx` **0**
(byte-identical); `.ARM.extab` **1,416** (size- and base-exact **156,312 B**);
`.rodata` **0** (byte-identical); `.rel.plt` **0** (byte-identical); `.bss` **0**;
`.note.gnu.build-id` **20**. File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 131)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-131 `Level::createShip` **70.5 -> 73.2** raw (**73.187686**) at the original 2764 B,
1007/1007 insns.  The first ~100 instructions (prologue through the `Player` ctor) are now
byte-identical: the original spills `race` to `[sp,#60]`, keeps `type` in `fp`, `shipClass`
in `r5`+`sl`, `this` in `r9`, `status` in `r4`, `mission` in `r8`.  The shape that reproduces
this is the faithful two-call `getLevel() > 20 ? 20 : getLevel()` clamp (level and empPoints),
`::operator new` + placement `new` for `PlayerFighter`/`PlayerFixedObject`, the faithful
`x = waypoint->x + 1st`, `y = waypoint->y + 2nd`, `z = waypoint->z + 3rd` spawn, and a
`try { player->setEmpData(...); } catch (...) { __builtin_trap(); }` around `setEmpData`.
The `try` is an RA/scheduling barrier: it keeps the coordinate block above `setEmpData`
(without it the block sinks below the call and the prologue reverts to `str r2,[sp,#60]`,
`mov sl,r3`).  Cost: the `try`/`catch` record is **328 B** (54 rows, `ttype 0x00`) instead of
the original's 336 B cleanup-only record, so `.ARM.extab` is **156,304 B (8 B short)** and
108,023 content bytes differ.  Whole-`.text` differing bytes fall **85,379 -> 85,167**.
See `_work/reconstructions/level-createship-wave131-w7693.md`.

Section deltas: `.text` **85,167** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **108,023** (size **156,304 B**, 8 B short, `0x26290`); `.rodata` **0**
(byte-identical); `.rel.plt` **0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**.
File size is exact (2,187,328 B).

## Update 2026-11-07 (wave 129)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Wave-129 re-lands the deferred w7645 `LevelScript::process` case-40/41/42 corrections at the
original 57232 B, plus the genuine case-42 state-6 fixes that pay for them: the bogus
`player->freeze = 0` (absent from the original) is removed, `player->setVisible(false)` moves to
its original slot after `setComputerControlled`, `freeMissionLock()` moves after the camera
`translate`, and the missing `m_nState = 7` state transition is restored.  The case-41 state-4
`timerA += delta` now precedes the camera translate as in the original.  `LevelScript::process`
raw fuzzy is 59.89 (was 60.10) because the corrected statements move blocks within the global
`MachineBlockPlacement` permutation; the function's own differing-byte count falls 54577 ->
54623 and the whole-`.text` count is 85,379 after the combined w7668 case-91 fix (85,487 on the w7666 base); wave 132 (w7705) lands the state-6/state-10 sound swap and the case-40 `m_nState = 5` fix, taking the function to 54,507 and the whole `.text` to 85,235.  See
`_work/reconstructions/levelscript-process-wave129-w7666.md`.

Wave-129 gain: `Radar::draw` **75.268817 -> 75.297491** raw (display 75.4) at the original
9908 B, by computing the post-loop `getStationEnumIndex` before `getAutoPilotTarget` in the
reduced recompute.  The whole-`.text` differing-byte count fell **85,441 -> 85,440**.  The
block layout is otherwise unchanged; the faithful single-recompute shape (removing the
post-loop recompute) still re-rolls the planet-loop register split (`fp`/`sl`) and scores
72.93 %.  See `_work/reconstructions/radar-draw-wave129-w7674.md`.

Section deltas: `.text` **85,379** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,417** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-07 (wave 127)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.1**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **81.9**,
`Level::createMission` **92.3**, `Gun::calcCharacterCollision` **93.4**,
`HangarWindow::render` **94.7**, `SpaceLounge::drawLounge` **95.7**, `Level::createScene` **96.5**,
`Hud::draw` **96.6**, `Generator::createMission` **97.2**, `MenuTouchWindow::OnTouchBegin` **98.1**,
`ModStation` C1/C2 **99.2**, `MGame::OnUpdate` **99.9**.

Wave-127 gain: `Generator::createMission` **90.0 -> 97.2** at the original size (1996 B) and
the original frame (96 B).  The clean shape (plain `int amount`, `Galaxy *galaxy` value,
`amount = 0; itemId = 0;` in the else arm) reproduces the original's exact seven-slot map;
the bonus `rem`/`agent` register-allocation fixed point is broken by writing the round-down
as the equivalent `bonus -= rem * 2` instead of `bonus = rawBonus - rem`, which lets
`rawBonus` die at the add so `rem` no longer needs `r9` and `agent` stays in `r9`.  The
`volatile short padA`, `volatile int *ap = &amount` and `Galaxy *&galaxy` hacks are removed.
The remaining 2.8 % is the bonus block's `r5`/`r6`/spill choice plus the `bhi` value-range
fold.  See `_work/reconstructions/generator-createmission-wave127-w7652.md`.

Section deltas: `.text` **85,438** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,427** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-07 (wave 126)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.1**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **81.9**,
`Generator::createMission` **97.2**, `Level::createMission` **92.3**,
`Gun::calcCharacterCollision` **93.4**, `Level::createScene` **96.5**,
`HangarWindow::render` **94.7**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.6**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-126 gains: `LevelScript::process` case-154 state 0/1 is now instruction-faithful
(`setComputerControlled` before `setVulnerable`, one `setSpeed(0.0f)`, `setTarget` moved from
state 1 to state 0, `m_nState++` at the block tail instead of `m_nState = 1` at the head, and the
state-1 `setSpeed(0.0f)` prologue restored). Size stays exactly 57232 B; `.rel.plt` and
`.ARM.exidx` remain byte-identical and `.rodata` exact. The function's fuzzy score is 60.2
(60.235 raw) but the whole-`.text` differing-byte count fell **85,552 -> 85,496** at that point (current **85,438** after the later wave-126/127 changes). See
`_work/reconstructions/levelscript-process-wave126-w7603.md`.

`Status::missionCompleted` **71.0 -> 71.1** — one `__builtin_expect(...,1)` on the case-162
`hasCargo` loop condition keeps the function at 1432 B and matches the original's loop branch
layout more closely. The remaining residual is the high-TBH body order: the original lays the
22 high case bodies out in source order, our identical clang in ascending case-value order,
which is a switch-edge-frequency (`!prof`/PGO) effect not expressible from C++11. See
`_work/reconstructions/status-missioncompleted-wave126-w7605.md`.

Section deltas: `.text` **85,438** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,427** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 125)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.2**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **81.3**,
`Generator::createMission` **97.2**, `Level::createMission` **92.3**,
`Gun::calcCharacterCollision` **93.4**, `Level::createScene` **96.5**,
`HangarWindow::render` **94.1**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.4**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-125 gain: `MGame::OnTouchEnd` **78.8 -> 81.3** — a `Mission *campaign` temporary reuses one
`getCampaignMission()` call (−4 B) and the trailing `hudTouchFlags == 0` block is folded into the
`wingmanCommand` `else if` chain (+4 B); the two are size-neutral together (8080 B) and keep
`.rel.plt` and `.ARM.exidx` byte-identical. See
`_work/reconstructions/mgame-ontouchend-wave125-w7513.md`.

Section deltas: `.text` **85,636** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,436** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 124)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.93%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.2**, `Level::createShip` **70.5**, `Status::missionCompleted` **71.0**, `Radar::draw` **75.4**, `MGame::OnTouchEnd` **78.8**,
`Generator::createMission` **97.2**, `Level::createMission` **92.3**,
`Gun::calcCharacterCollision` **93.4**, `Level::createScene` **96.5**,
`HangarWindow::render` **94.1**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.4**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-124 gains: `Layout::Layout` **48.9 -> 48.9** (raw 48.894 -> 48.934; `field_0x158`
default arm derived from `field_0x154`); `Radar::draw` **74.5 -> 75.3** (planet-lock tail
before the landmark loop, reduced recompute); `Generator::createMission` **89.9 -> 90.0**
(named `toSystem` index temp + `Galaxy *&` alias); `Level::createShip` **70.3 -> 70.5**
(faithful jitter pairing + `z,y,x` declaration order; a non-semantic call-site RA tune reaches
71.4 but is not kept because it passes the wrong arguments);
`Level::createScene` **92.8 -> 93.6** (natural parts loop, `AERandom *&`, cap split, `geo`
hoist); `LevelScript::process` stays **60.3** but two case-157 fidelity fixes reduce its
`.text` delta. `MGame::OnTouchEnd` **82.4 -> 78.8** is a structural win: the failed-first
`won` local and `hasFailed() == true` hint make **`.rel.plt` byte-identical**; the fuzzy drop
is block-address placement, not missing code.

Section deltas: `.text` **85,975** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,474** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**0** (byte-identical); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 123)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.9**,
`LevelScript::process` **60.2**, `Level::createShip` **70.3**, `Status::missionCompleted`
**71.0**, `Radar::draw` **74.5**, `MGame::OnTouchEnd` **82.4**,
`Generator::createMission` **89.9**, `Level::createMission` **92.3**,
`Level::createScene` **92.8**, `Gun::calcCharacterCollision` **93.4**,
`HangarWindow::render` **94.1**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.4**,
`MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-123 gains: `Layout::Layout` **48.7 -> 48.9** (n9-first `field_0x184` arm, int
`field_0x124` arm, `field_0x2b8`/`field_0x110` order); `HangarWindow::render` **94.0 -> 94.1**
(free-credits `visible` as one expression); `Hud::draw` **95.3 -> 96.4** (boost if/else,
`Globals::status` anchor, `short barW`); `MGame::OnTouchEnd` **81.7 -> 82.4** (named `won`
local + inverted guard). `LevelScript::process` stays **60.3** but now reuses the saved
`player` local in mission-0 case 8 (raw 60.2547 -> 60.2567). `Level::createShip` **73.9 ->
70.3** is a structural win: the real-`new` body carries the original's 336 B cleanup-only
LSDA (`ttype 0xff`, 57 rows), so **`.ARM.extab` is size- and base-exact (`0x26298`)** and
**`.ARM.exidx` is byte-identical**; the score drop is the Greedy-RA cascade caused by the four
live cleanup pointers (see `_work/reconstructions/level-createship-wave123-w7306.md`).

Build-flag research (wave 123, two independent workers) re-confirmed the stock NDK r18b CMake
Release recipe: 192/192 app TUs object-identical, full default link flag set byte-identical,
and 259 + 258 global plus 140 per-TU flag screens found **zero** target improvements.
The remaining gaps are source shape, not build flags. See
`_work/reconstructions/build-flags-wave123-w7319.md` and
`_work/reconstructions/build-flags-wave123-w7320.md`.

Section deltas: `.text` **86,993** differing bytes (size exact); `.ARM.exidx` **0**;
`.ARM.extab` **1,446** (size exact, 156,312 B); `.rodata` **0** (byte-identical); `.rel.plt`
**4** (2 swapped PLT entries); `.bss` **0**; `.note.gnu.build-id` **20**. File size is exact
(2,187,328 B).

## Update 2026-11-06 (wave 122)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**,
avg **99.94%**. 18 functions remain non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **48.7**,
`Status::missionCompleted` **71.0**, `LevelScript::process` **60.2**, `Level::createShip`
**73.9**, `Radar::draw` **74.5**, `MGame::OnTouchEnd` **82.4**, `Generator::createMission`
**89.9**, `Level::createMission` **92.3**, `Level::createScene` **92.8**,
`Gun::calcCharacterCollision` **93.4**, `HangarWindow::render` **94.0**, `Hud::draw`
**95.3**, `SpaceLounge::drawLounge` **95.7**, `MenuTouchWindow::OnTouchBegin` **98.1**,
`ModStation` C1/C2 **99.2**, `MGame::OnUpdate` **99.9**.

Wave-122 gains: `Layout::Layout` now uses the original's `field_0x184` iPad/n9 arm order
(48.96 -> **48.70**, a fidelity fix); `Status::missionCompleted` **59.4 -> 71.0** (four branch-probability hints on the
plain switch recover most of the rank-table removal cost while keeping `.rodata` exact);
`Generator::createMission` **88.0 -> 89.9** (volatile `short padA` + volatile
`amount` store); `HangarWindow::render` **91.7 -> 94.0** (free-credits canvas local + faithful
width order); `Hud::draw` **95.99 -> 95.27** (marker per-arm image load + `int` dock total; the
marker/radar sub-draws are now instruction-identical, the score dip is the pre-existing boost
Canvas hoist plus dock-block allocator perturbation); `LevelScript::process` **60.2 -> 60.3**
(case-154 state-7 timer/state fix); `Level::createShip` **73.8 -> 73.9** (capital-ship position
vector). See the `*-wave122-w72*.md` notes.

Section deltas: `.text` **86,947** differing bytes (size exact); `.ARM.exidx` **1,327**;
`.ARM.extab` **156,304 B** (8 B short; only `Level::createShip` -8, 108,010 differing bytes);
`.rodata` **0** (byte-identical); `.rel.plt` **4**; `.bss` **1**; `.note.gnu.build-id` **20**.
File size is exact (2,187,328 B).

## Update 2026-11-06 (wave 121)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207** (identical to the original), lint **CLEAN**, drift **0**, symbol
parity **5370/5370** (0 extra / 0 missing), avg **99.93%**. 18 functions remain
non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **49.0**,
`Status::missionCompleted` **59.4**, `LevelScript::process` **60.2**, `Level::createShip`
**73.9**, `Radar::draw` **74.5**, `MGame::OnTouchEnd` **81.7**, `Generator::createMission`
**88.0**, `HangarWindow::render` **91.7**, `Level::createMission` **92.3**,
`Level::createScene` **92.8**, `Gun::calcCharacterCollision` **93.4**,
`SpaceLounge::drawLounge` **95.7**, `Hud::draw` **96.0**, `MenuTouchWindow::OnTouchBegin`
**98.1**, `ModStation` C1/C2 **99.2**, `MGame::OnUpdate` **99.9**.

Wave 121 made three structural gains. `PlayerEgo::calcCollision` is now **byte-exact**
(95.5 -> **100.0**, 968 B): the original's un-rotated loop and visible-arm register shuffle
are reproduced by two branch-weight hints (`__builtin_expect(isShrinking() || miningGame != 0, 1)`
on the wormhole `continue` and `__builtin_expect(isDying() || isDead(), 0)` on the steal-arm
`continue`), the real compare order (`collision != Vector{0,0,0}`) and the constant tail store
(`= 1`). The previous body's two redundant size guards and the `field_0x71` tail store are gone.
See `_work/reconstructions/playerego-calccollision-wave121-w7116.md`.

`Generator::createMission` improved **84.7 -> 88.0** and its `.ARM.extab` record is now exact
at 172 B (the `race < 4 && type == 13` branch hint plus the `volatile long long padA` width).
See `_work/reconstructions/generator-createmission-wave121-w7110.md`.

`Level::createShip` now uses the **faithful spawn coordinates** (`x = waypoint->x + jx`,
`y = waypoint->y + jy`, `z = waypoint->z + jz`, all `int`) instead of the wave-119 tuned
slot-map; the score dips 74.4 -> **73.9** but the source now matches the original's
disassembly semantics. See `_work/reconstructions/level-createship-wave121-w7107.md`.

The last manually linked data input was also removed: the 192-byte
`Status::missionCompleted` mission-type -> case-rank table (`tools/mission_case_table.cpp`,
fixed address `0x20b977`). The function now uses a plain `switch (m->getType())` with the
real mission-type labels in the original body order. **`.rodata` is byte-identical**:
269,159 B at `0x1c9e10`/`0x41b67`, 0 differing bytes. The function score drops
**92.454632% -> 59.372027%** (1432 B -> 1432 B, byte/linked still false), and the `.text`
content delta is **87,892** bytes; the `.text` size/base (0x69440/0x1337c8),
`.ARM.exidx` size/base, `.ARM.extab` size (0x26290, 8 B short) and the exact file size
(2,187,328 B) are unchanged. The remaining section deltas are `.text` 87,892,
`.ARM.exidx` 1,327, `.ARM.extab` 107,976 (8 B short), `.rel.plt` 6, `.bss` 1 and
`.note.gnu.build-id` 20.

Why the Status score drops: clang 7's `MachineBlockPlacement` lays a plain switch's case bodies
out in the jump table's value-sorted successor order, while the original's body layout is
two ascending runs (A = 150,151,152,153,154,155,158,162,166,170,184,189 then
B = 156,157,160,163,164,165,168,169,171,172). The rank table made the sorted case values
A-then-B, which is why it reproduced the original layout. A plain switch matches the
dispatch (`sub #150; cmp #39; tbh`), the TBH size/base, the low group and the shared tails,
but the A/B body order is value-interleaved, so the verifier's LCS drops. The table is the
only plain-C++ mechanism found that reproduces the original body order; removing it is a
structural win (`.rodata` exact, no manually linked data) at the cost of the function's
fuzzy score. See `_work/reconstructions/status-missioncompleted-wave121-w7113.md`.

## Update 2026-11-06 (wave 120, superseded by wave 121)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4505/4524**, linked_exact **4505/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207** (identical to the original), lint **CLEAN**, drift **0**, symbol
parity **5370/5370** (0 extra / 0 missing), avg **99.94%**. 19 functions remain
non-byte-exact, all size-exact.

Worst-first list from a fresh `report.json`: `Layout::Layout` C1/C2 **49.0**,
`LevelScript::process` **60.0**, `Level::createShip` **74.4**, `Radar::draw` **74.5**,
`MGame::OnTouchEnd` **81.6**, `Generator::createMission` **84.7**, `HangarWindow::render`
**91.7**, `Level::createMission` **92.3**, `Status::missionCompleted` **92.5**,
`Level::createScene` **92.8**, `Gun::calcCharacterCollision` **93.4**,
`PlayerEgo::calcCollision` **95.5**, `SpaceLounge::drawLounge` **95.7**, `Hud::draw`
**96.0**, `MenuTouchWindow::OnTouchBegin` **98.1**, `ModStation` C1/C2 **99.2**,
`MGame::OnUpdate` **99.9**.

Wave-120 build-provenance research (10 parallel agents) established that the current
compile recipe is byte-equivalent to **stock NDK r18b CMake Release defaults**: 192/192
app TUs compile to identical objects, the full CMake/ndk-build default link flag set
produces the same `.so` hash, and a stock toolchain plus a minimal single-target
`CMakeLists.txt` reproduces the original's dynamic tables and `.rodata` exactly (the
canonical +192 B `.rodata` is the checked-in `tools/mission_case_table.cpp` matching
device). No tested global or per-TU compile flag improves any residual function; the
remaining gaps are source-shape. See
`_work/reconstructions/build-flags-cmake-equivalence-wave120-main.md`,
`_work/reconstructions/stock-cmake-reproduction-wave120-w7010.md`, and
`_work/reconstructions/per-tu-flag-sensitivity-wave120-w7005.md`.

Section deltas: `.text` **87,809** differing bytes (size exact); `.ARM.exidx` **1,327**;
`.ARM.extab` **156,304 B** (8 B short; only `Level::createShip` -8, 107,985 differing
bytes); `.rodata` **+192** (mission rank table); `.rel.plt` **6**; `.note.gnu.build-id`
**20**; `.bss` **3** (the known pointer-order residual). File size is exact (2,187,328 B).

## Update 2026-11-05 (wave 119)

**Current authoritative state.** `cmake --build cmake-build-match --target check` is green:
byte_exact **4505/4524**, linked_exact **4505/4524**, size-exact **4524/4524**, data_exact
**843/843**, imports **207** (identical to the original), lint **CLEAN**, drift **0**, symbol
parity **5370/5370** (0 extra / 0 missing), avg **99.94%**. 19 functions remain
non-byte-exact, all size-exact.

Wave 119 (20 parallel `codex exec -p dsv41` workers, worst-first) made
`MenuTouchWindow::MenuTouchWindow` C1/C2 **byte-exact** (99.82 -> **100.0**; hoisting row2
before `addButton(19)`), moving `byte_exact` 4503 -> **4505**. Other gains:
`Level::createScene` 90.7 -> **92.8**, `Radar::draw` 74.0 -> **74.4**,
`LevelScript::process` 58.2 -> **60.0** (merged wave-119 statement reorders at exactly
57232 B). `.text` content delta 89,836 -> **88,125**.

The `Status::missionCompleted` rank table is the last `.rodata` input
(`tools/mission_case_table.cpp`, fixed address `0x20b977`); every original `.rodata` byte
before it is exact, and the table accounts for the only 192 B of extra `.rodata`. All 27
original section headers are present and exact, with no extra section and an exact file size
(2,187,328 B).

**`.ARM.extab` is 8 B short** (`156,304` B, `0x26290`); the only record delta is
`Level::createShip` -8 (328 vs 336). `Generator::createMission` is now exact at 172 B
(wave-121 w7110) and `HangarWindow::render` is exact at 948 B (wave-119 w6978). `.rodata`
starts at the original `0x1c9e10`; the 8 B shortfall fits the 16-byte `.rodata` alignment
gap, so no function address moves.

The only differing section bytes are `.text` **87,892**; `.ARM.exidx` **1,327** raw bytes;
`.ARM.extab` **107,976** (8 B short; `Level::createShip` -8); `.rodata` **0** (byte-identical); and
`.note.gnu.build-id` **20**. Everything else (`.data`, `.data.rel.ro`, `.plt`, `.got`,
`.init_array`, `.fini_array`, `.dynamic`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version*`,
`.rel.dyn`, `.rel.plt`, `.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
`.shstrtab`, `.ARM.attributes`) is byte-identical. `.rel.plt` now differs by **6 bytes**: the
`MGame::OnTouchEnd` cold-block layout swaps two adjacent `JUMP_SLOT` entries
(`MenuTouchWindow` / `MGame::setCinematicMode`); it returns to exact when that function matches.

The 19 non-byte-exact functions (fuzzy %, current report): `Layout::Layout` C1/C2 48.9,
`Generator::createMission` 84.3, `LevelScript::process` 60.0, `Level::createShip` 70.3,
`Radar::draw` 74.4, `MGame::OnTouchEnd` 81.3, `PlayerEgo::calcCollision` 95.5,
`HangarWindow::render` 91.3, `Level::createMission` 92.3, `Status::missionCompleted` 92.5,
`Level::createScene` 92.8, `Gun::calcCharacterCollision` 93.4, `SpaceLounge::drawLounge`
95.7, `Hud::draw` 96.0, `MenuTouchWindow::OnTouchBegin` 98.1, `ModStation` C1/C2 99.2,
`MGame::OnUpdate` 99.9. See `_work/reconstructions/build-provenance-flags-wave119-w6719.md`
and the wave-106..119 notes.

## Update 2026-10-26 — historical (superseded by the update above)

Then-current: **byte_exact 4499, linked_exact 4499, avg 99.9%**. libcrypto is OpenSSL 1.1.0l via
NDK r13b GCC 4.9 -O3 ARM (adds the original's genuine `memcpy`/`memset`/`mkstemp` imports). Adds `-g` (authentic ndk-build
default; keeps entry-block argument copies from PostRA sinking), the app flags for vendored
zlib/libzip (`-Oz -g -fstack-protector-strong -ffunction-sections`, was `-O2`), and source-order
placement of local functions in the then-current section-ordering file. The 2026-11-01 data-layout pass
dropped ten empty-`.text` CANTUNWIND sentinels that had inflated `.ARM.exidx` by 0x60 and
pushed `.rodata` to 0x1c9e50 (byte 2646 -> 2732), and restored the original string-literal
order in BloomShader/GlowPPShader/SandboxShader (byte 2732 -> 2734). The 2026-11-01
literal-pool pass then fixed the GOT first-reference order (byte 3921 -> 4105) and the
`LowResourceTable`/`ResourceMaterial` link order witnessed by `.rodata` (byte 4105 -> 4107);
the Level.cpp `.rodata` ordering pass then made `Level::createCampaignMission` byte-exact
(byte 4107 -> 4108). A further `.rodata` ordering pass fixed `Radar::drawCurrentLock`'s `%`
literal and the libzip `zip_err_str`/`zip_open` order (byte 4108 -> 4112), and the Level
padding-array removal restored the jumpgate/sky array sizes (byte 4112 -> 4115). The
2026-11-01 `.rodata` output-group pass then replaced the trailing `*` in the section-ordering
file with `.text*`/`.data*`/`.bss*`, so plain `.rodata` inputs stay at gold's index 0 and
interleave with the merge groups in input order (byte 4115 -> 4171). The 2026-11-01 data-layout
pass then fixed the last three `.rodata` size deltas (ModStation reward table, MissionsWindow goal
text default, DialogueWindow success script/alignment), making **every ELF section size- and
base-exact**, `.rodata` included (0x41b67 @ 0x1c9e10), and byte_exact 4171 -> 4182. The 2026-11-01 worst-match wave then added the Generator table dedup
(byte 4182 -> 4188) and the TU-start 4-byte alignment discovery in the exidx-marker generator
(byte 4188 -> 4233, section sizes unchanged) plus Layout::Layout 45.3 -> 46.6%, HangarWindow::render 78.2 -> 78.7% and
the Status rank-table fix (fuzzy-only, byte-neutral). The w3438 `.rodata` content-map pass then
moved `g_levelXPTable`/`Status::checkForLevelUp` ahead of `kMissionCase`, making
`0x1fc330..0x1fc390` byte-identical and `Status::checkForLevelUp` byte-exact (byte 4249 -> 4250;
linked 4496 unchanged). The w3451 data-content pass then forced the original `.data` emission
order for `gStoryFaces`/`gPirateFace` with a function-level `(void) gStoryFaces;` (byte 4254 ->
4255), dropping `.data` differing bytes 260 -> 3 and `.data.rel.ro` 744 -> 670. The w3457
`.data.rel.ro` content audit found the then-remaining 667 bytes were all `R_ARM_RELATIVE`
address mirrors (libc++abi `.text` vtables, 18 B `Level::createScene` `.rodata` table, 3 B `.bss`
pointers); its libzip pass aligned `zip_file_strerror` to 4 bytes (byte 4263 -> 4264, all section
sizes/bases unchanged). The w3460 libc++abi-placement pass then gave every archive member's
empty-`.text` sentinel a 4-byte alignment (the original's TU starts), removed the 0x24-byte
`levelTextAnchor` anchor and placed the `fallback_malloc.o`/`abort_message.o` markers next to
their owners: **`.data` is now byte-identical (3 -> 0 differing bytes)** and `.data.rel.ro` drops
667 -> 21 bytes (18 B `Level::createScene` `.rodata` table + 3 B `.bss` pointer order), byte
4265 -> 4408. The first
`.rodata` *content* divergence was at 0x1fb7f8 (VMA); 8,826 bytes differed. See
`_work/reconstructions/data_content_round2_w3451.md`,
`_work/reconstructions/rodata_content_map_round2.md`,
`_work/reconstructions/rodata_group_order.md`, `_work/reconstructions/literal_pool_audit.md`
and `_work/reconstructions/data_layout_audit_2.md`.

The w3462 `.ARM.exidx`/`.ARM.extab` pass then fixed the empty-`.text` CANTUNWIND sentinel
set (drop `Vector`/`BuildResourceList`/`LowResourceTable`/`Array`/`ArrayInstantiations`,
keep `BackButton`/`main`), moved the NdkHooks/NdkAutosave sentinels to the original's
witnessed boundaries, and added a 12-byte dead-local pad in `HangarWindow::render` to
restore its LSDA record. **`.ARM.exidx` is now `0x6f70` (3566 entries, CANT 272, inline
856, extab 2438) and `.ARM.extab` was `0x26298`**, both size- and base-exact at that point;
byte 4408 -> 4409. The remaining EH content is one `Generator::createMission` CANT/INL
offset (4-byte body shortfall) and seven extab record-length deltas. Those seven records
belong to the seven still-mismatched bodies (`Generator::createMission`,
`Level::createShip`, `StarMap::draw`, `RecordHandler::recordStoreWrite`,
`LevelScript::process`, `HangarWindow::render`, `Hud::draw`) and netted **-8 bytes** at
that point. The later `RecordHandler::recordStoreWrite`/`Hud::draw` record fixes leave
five deltas (`Generator::createMission` +4, `Level::createShip` -8,
`LevelScript::process` +4, `HangarWindow::render` -8, `Hud::draw` +4) netting **-4 bytes**.
The wave-102 LSDA-scope pass then fixed `Level::createShip`, `HangarWindow::render` and
`Hud::draw` and pulled `Generator::createMission` to 168 B, leaving only the cancelling
`Generator::createMission` -4 / `LevelScript::process` +4 pair; `.ARM.extab` is now
`0x26298` (156,312 B), size- and base-exact. See
`_work/reconstructions/exidx_extab_exact_w3462.md`,
`_work/reconstructions/exidx_duplicate_address_root_cause_w3565.md`,
`_work/reconstructions/exidx-records_w4237.md` and
`_work/reconstructions/hangarwindow-render-w4243.md`.

The w3472 `Status::missionCompleted` pass then recovered the 4-byte `.text` shortfall that
had blocked the Level `.rodata` block. The original switches directly on `m->getType()` and is
1432 B; our direct `int` switch emitted 1428 B because clang 7 places the case bodies in
ascending-value order instead of the original's source order. Switching on
`(short) m->getType()` restores the 1432 B size without a padding anchor (the signed-narrow
switch lowering places the 184/189 bodies after the default block). With the size fixed, the
Level `.rodata` corrections from w3469 could land: `g_ips_shipTypeMap[64]`,
`jumpgateMeshes[16]`, removal of the invented `kMissionCase[190]`, the 13-row
`gCreateChar2Table`, the `alignas(16)` LensFlare tables and the 10-int Hud `_RACE_IMAGES_BIG`.
Every ELF section stays size- and base-exact; **byte_exact 4414 -> 4441 (+27), linked 4496
unchanged**, `.rodata` differing bytes 13,121 -> 8,826 (first still `0x1fb7f8`). The
`missionCompleted` body itself remains a 57.7% near-miss (the direct `int` form was 60.2%);
the gate gain is from the corrected `.rodata` data, not from that function. See
`_work/reconstructions/Status-missionCompleted-4bytes-w3472.md`.

The w3477 `.rodata` content round-5 pass then closed the remaining 8,826-byte gap. The
baseline gap was pure table emission order (word multiset 6,586/6,586 common, 0 unique):
clang 7 emits local statics in function order and namespace-scope statics at first odr-use,
so the Level station/blueprint tables, `createScene`'s hangar-spot tables and the StarMap /
MenuTouchWindow tables were landing in the wrong order. Three `Level` function moves, one
`#pragma clang section rodata=".rodata"` on `PlayerCreature`'s 16-byte table, and three
`(void)` odr-use anchors reproduce the original order. **`.rodata` is now byte-identical
(0 / 269,159 differing bytes)**; every ELF section stays size- and base-exact, and
**byte_exact 4441 -> 4463 (+22), linked 4496 unchanged**. See
`_work/reconstructions/rodata_content_round5_w3477.md`.

The w3490 `kMissionCase` pass then restored `CUTSCENE_SHIP_HEIGHT[64]` and removed the
rank-table workaround, returning `.rodata` to **0 differing bytes**. A later pass reintroduced the
rank table in its own `.missioncase` section to force source-order switch-body layout (raising
`Status::missionCompleted`'s fuzzy match from 57.7% to 86.7%). The w3570 pass removed it again:
the direct `(short) m->getType()` switch keeps **byte_exact 4496 / linked_exact 4496**, `.text`
`0x1337c8`, `.rodata` 0 differing bytes, lint clean, drift 0, and removes the `.missioncase`
section (28 -> 27 sections, `.shstrtab` 0x124 -> 0x117); it also restores the original
type -> body mapping for types 161/162/166/168/169. `missionCompleted` returns to 57.7% (the
rank table was a codegen hack, not original data). See
`_work/reconstructions/kMissionCase_real_table_w3490.md` and
`_work/reconstructions/kMissionCase_removal_w3570.md`. Wave102 removed the w5003/wf108 rank table
again: the direct `int` switch with the case-166 `return m` size anchor keeps **byte_exact
4500 / linked_exact 4500**, `.text` `0x1337c8`, `.rodata` 0 differing bytes, lint clean, drift 0,
and restores the 27-section table (`.shstrtab` 0x117, `.rel.plt` `sh_info=20`, exact file size);
`missionCompleted` returns to 60.4%. See
`_work/reconstructions/status-missioncase-section-table-wave102-wm102.md`.

## Update 2026-07-26 — historical (superseded by the update above)

The verify metric was rebuilt (v2: symbol-aware call targets, PLT-aware, literal-pool words
compared as data, size-checked linked_exact, autojunk off — see docs/VALIDATION.md) and the
vendored zlib/libzip/SHA ISA modes fixed (131 functions were compared in the wrong ARM/Thumb
mode). Current: **compared 4524, byte_exact 4115, linked_exact 4495, avg 99.89%, stub_zero_size
0, missing 0, sodiff extra 0, undefined imports 207**. Historical figures in the sections below
(923/1636, 870/1569, 35/0/0, extra 9–35) are kept as a record of past states — do not quote them
as current. The no-degradation gate is now machine-checked: `tools/ratchet.py` vs
`tools/verify_baseline.json` (cmake `check` target + pre-push hook). Two new prerequisites for
byte-identicality were established from the original source-layout dumps
(`_work/original_layout/`): original **TU membership** (which functions/statics/literals compile
together — .rodata/.data order, `.init_array` (orig 3 entries, ours the same 3,
byte-identical), string pools and -Oz same-TU inlining all follow it) and original
object **link order**. The `drift` target (fixed to
the real AAPCS ABI) reports 0 drifted fields; the former 74-field backlog has been reconciled.

## Build reconstruction (wave 95, 2026-11-01)

The original build is now pinned by evidence, not by A/B alone:

* **`-ffunction-sections` was used.** The original `.ARM.exidx` has 193 `CANTUNWIND` entries
  that share their target address with a real entry; that is gold's `fix_exidx_coverage()`
  sentinel for the empty plain `.text` section every `-ffunction-sections` object carries. A
  rebuild without the flag produces only 52. The original was linked **without** a
  section-ordering file: its `.text` is grouped by original TU (2,231 mapped functions, 105 TUs,
  104 transitions, 0 interleaving). The current tree reproduces that with a plain input-order
  link: after the w4210 TU/function-order reconstruction (including the `BSphere` -> `Vector`
  merge), all 4,524 exported functions land at their original addresses and no ordering file is
  used. The object order is now a checked-in plain source list (`tools/source_order.txt`), read
  directly by `CMakeLists.txt` and linked as `$<TARGET_OBJECTS:gof2>`; the build no longer reads
  the original `.so` to decide link order. `tools/gen_link_order.py` is analysis-only and the old
  `gen_section_order.py`/`gen_exidx_markers.py` generators are deleted (w4217). See
  `_work/reconstructions/link_order_tu_reconstruction_w4210.md`,
  `_work/reconstructions/static_link_order_w4217.md`,
  `_work/reconstructions/ffunction_sections_and_order_file.md` and
  `_work/reconstructions/tu_membership_interleaving_w3613.md`.
* **`-fdata-sections` was NOT used.** The original's `.rodata`, `.data` and
  `.data.rel.ro` are byte-identical to the no-`-fdata-sections` build and differ by
  113,935 / 3,273 / 405 bytes with it. This is a real deviation from vanilla
  ndk-build r18b. See `_work/reconstructions/definitive_build_recipe_w3697.md`.
* **The link order is not vanilla ndk-build `c++_static`.** FMOD precedes
  `libc++abi.a`; `_Unwind_*` stay `UND` and `__gnu_Unwind_Find_exidx` is absent.
  A vanilla-order rebuild is 1345 byte-exact with `.text 0x135d0c`. See the same
  note.
* **`-g` is codegen-relevant at `-Oz`.** `-g`, `-g2`, `-g3` and all DWARF variants are identical
  (byte 4496); `-g0`, `-g1` and `-gline-tables-only` drop to byte 2069 because entry-block
  argument copies are no longer kept from PostRA sinking. The tree's `-g` is correct. See
  `_work/reconstructions/debug_info_flags_w3564.md`.
* **`.dynsym` order is gold's hash-map iteration = first-mention order over the link inputs.**
  The model (name_key ^ version_key, SGI `hash_map` ascending buckets) reproduces our entire
  `.dynsym` 5,577/5,577; `.dynstr` and `.hash` follow from it with no independent order. The
  app region is exactly first-mention over crtbegin + 193 objects (4,935/4,935). See
  `_work/reconstructions/dynsym_dynstr_hash_order_w3567.md` and
  `_work/reconstructions/tu_compile_link_order_w3560.md`.
* **Shortcut removal (same wave).** The `s_processPad` anchor is replaced by the real 16-byte
  unreferenced file-static object that `TargetFollowCamera.cpp` owns at `0x2183a0`; the eight
  `(void)` odr-use anchors in `Level.cpp` are replaced by `[[gnu::used]]` definitions at the
  same `.rodata` emission point. Both are gate-neutral (byte 4496 / linked 4496). See
  `_work/reconstructions/remove_s_processPad_anchor_w3571.md` and
  `_work/reconstructions/Level-remove-odr-anchors-w3574.md`.
* **Current differing bytes (2026-11-04).** `.text` (see table); `.ARM.extab` (see table);
  `.ARM.exidx` (see table); `.note.gnu.build-id` 20; `.rel.plt` 0; `.dynsym` 0; `.dynstr` 0;
  `.hash` 0; `.gnu.version` 0; `.rel.dyn` 0; `.shstrtab` 0;
  **section headers: all 27 original entries exact** (27 × 40
  bytes; including `.rel.plt` `SHF_INFO_LINK`/`sh_info=20`,
  `.init_array`/`.fini_array` `sh_entsize=4` and `.bss` `sh_offset=0x21596c`).
  `.rodata`, `.data`, `.data.rel.ro`, `.plt`, `.got`, `.dynamic`, `.init_array`, `.fini_array`,
  `.comment`, `.note.*` and `.ARM.attributes` are byte-identical; the section table matches the
  original's 27 sections. The w3616 pass made
  `.rel.dyn` byte-identical at the time and cut `.rel.plt` to one moved symbol
  (`PlayerEgo::deleteHackingGame`); the w3624 pass merged `BuildResourceList.cpp` +
  `LowResourceTable.cpp` + `ResourceMaterial.cpp` back into the original `GameResource.cpp` TU
  (`.dynsym` 62 -> 50, `.dynstr` 345 -> 211, `.hash` 42 -> 33, `.rel.plt` 15 -> 12,
  `.ARM.exidx` 2735 -> 2733). The w3695 pass replaced the post-link GNU
  `strip --strip-all` with gold `-s`, reproducing the original `.shstrtab` byte-for-byte
  (259 -> 0 differing bytes; see
  `_work/reconstructions/nontext_relplt_shstrtab_w3695.md`). The w3697 section-order pass
  changed the `.text` function order; the later link-order reconstruction restored the dynamic
  table order, and `.dynsym`/`.dynstr`/`.hash`/`.rel.dyn`/`.rel.plt` are byte-identical again
  (`sodiff` 0 extra / 0 missing).
* **Build metadata.** `.note.android.ident` is byte-identical to the NDK r18b API-16
  `crtbegin_so.o` note; the only path-like string in the `.so` is the libc++abi buildbot
  `abort_message.cpp` path, and the only app source-file witness is `main.c` in three log
  strings. No app source path survives. See
  `_work/reconstructions/build_metadata_source_paths_w3566.md`.

## Section-size deltas (stripped ours vs original)

| section | orig | ours | delta | meaning |
|---|---|---|---|---|
| **.text** | 1,259,464 | 1,259,464 | **0** | exact size and address (0x69440). 76,743 content bytes differ across the 17 non-byte-exact bodies (wave 180). |
| **.rodata** | 269,159 | 269,351 | **+192** | base `0x1c9e10` exact and the original 269,159 B content is **byte-identical**; wave 178 restored the 192 B `Status::missionCompleted` rank table at `0x20b977` (in the gap before `.fini_array`), so the section is 0x41c27. Wave 121 had removed it for an exact-size `.rodata`; the +192 B buys -272 `.text` bytes (raw 721 -> 449). |
| .ARM.exidx | 28,528 | 28,528 | **0** | **byte-identical** (0 differing bytes; size- and base-exact 0x19cc08); 3566 entries, CANTUNWIND 272, inline 856, extab 2438. The `Layout::Layout` body is size-exact 7072 B without a pad. Wave 123 gave `Level::createShip` its faithful 336 B LSDA record, removing the last record-length delta. |
| .ARM.extab | 156,312 | 156,312 | **0** | **size- and base-exact** (0x26298 @ 0x1a3b78). `Level::createShip` is the faithful 336 B real-`new` cleanup-only record (57 rows, `ttype 0xff`); `Generator::createMission` is exact at 172 B (wave-121 w7110) and `HangarWindow::render` is exact at 948 B (w6978). 1,416 raw content bytes differ (the createShip landing-pad offsets inside the otherwise-exact records). |
| .dynsym | 89,248 | 89,248 | 0 | **byte-identical** (5,578 entries, 5,370 defined symbols). Gold emits dynsym in SGI `__gnu_cxx::hash_map` ascending-bucket order (keyed by 1-based Stringpool insertion indices), i.e. first-mention order over the link inputs; the model reproduces our table 5,577/5,577. All 79 versioned imports match. See `_work/reconstructions/dynsym_dynstr_hash_order_w3567.md`, `_work/reconstructions/rel_plt_rel_dyn_ordering_w3616.md` and `_work/reconstructions/gameresource_tu_convergence_w3624.md`. |
| .dynstr | 189,208 | 189,208 | 0 | **byte-identical** (0 differing bytes; 189,208 B). |
| .rel.plt | 29,392 | 29,392 | 0 | **byte-identical** (0 differing bytes; 0x72d0 @ 0x5751c; `SHF_INFO_LINK`/`sh_info=20`). Wave 124 made the `MGame::OnTouchEnd` cold-block PLT order match. |
| .rel.dyn | 28,744 | 28,744 | 0 | **byte-identical** (0 differing bytes; 0x7048 @ 0x504d4). |
| .hash | 38,716 | 38,716 | 0 | **byte-identical** (0 differing bytes; 0x973c @ 0x441a8). |
| .data.rel.ro | 12,264 | 12,264 | **0** | **byte-identical** (0 differing bytes). |
| .shstrtab | 279 | 279 | **0** | **byte-identical**; 27 original names, no extra section. |
| .data | 10,604 | 10,604 | **0** | exact (0x214000/0x296c; original TU order/sizes). |
| .bss | 27,052 | 27,052 | **0** | exact (0x216970/0x69ac; `sh_offset` 0x21596c, the unaligned end of `.data` as bfd writes it). The invented `.aezdata` section is gone, so the RW LOAD and `.bss` VMA now match. |
| .got | 17,164 | 17,164 | 0 | exact (address 0x20fcf4, same slot order). |
| .dynamic | 328 | 328 | 0 | **exact** again after wave 108 moved the table out of `.fini_array` (`-z now` / BIND_NOW exact). |

## The remaining gaps (in priority order)

1. **18 non-byte-exact functions.** 4506 of 4524 compared functions are linked-exact. The worst
   are `Layout::Layout` (49.0%), `Generator::createMission` (84.3%), `LevelScript::process`
   (60.0%), `Level::createShip` (70.3%), `Radar::draw` (74.4%), `MGame::OnTouchEnd` (81.9%),
   `HangarWindow::render` (91.3%), `Level::createMission` (92.3%) and
   `Level::createScene` (92.8%). The `.text` total is exact (0x1337c8); these are
   per-function body/register-allocation deltas, not missing code.
2. **`.rodata` content — exact; size +192.** Wave 178 restored the 192 B
   `Status::missionCompleted` rank table at `0x20b977`: the original 269,159 B content is
   byte-identical, but the section is 269,351 B. The rank table makes clang 7 emit the case
   bodies in the original's A-then-B order (raw 721 -> 449); wave 121 had removed it for an
   exact-size `.rodata` and accepted the plain-switch 721. See
   `_work/reconstructions/status-432-producers-wave178-w12218.md` and
   `_work/reconstructions/status-missioncompleted-wave121-w7113.md`.
3. **EH tables — exact sizes.** `.ARM.exidx` is **byte-identical** (3566 entries, CANTUNWIND
   272, inline 856, extab 2438); `.ARM.extab` is **size- and base-exact** (156,312 B @
   0x1a3b78, 0x26298). `Level::createShip` now has the faithful 336 B cleanup-only record
   (57 rows, `ttype 0xff`, wave 123); `Generator::createMission` is exact at 172 B and
   `HangarWindow::render` is exact at 948 B. 1,446 raw `.ARM.extab` content bytes still differ
   (the createShip landing-pad offsets inside the exact-length records). The
   `Layout::Layout` body is size-exact 7072 B without a pad. See
   `_work/reconstructions/exidx-records_w4237.md` and
   `_work/reconstructions/extab_lsda_scopes_w102_wm103.md`. The w3840 `extern template` for
   `ArrayReleaseClasses<Vector*>` in `SpaceLounge.cpp` restored the original's invoke (the
   original only saw the declaration, so the call was not pruned as `nounwind`) and removed
   the `SpaceLounge::~SpaceLounge` -4 record. The 194 duplicate-address exidx entries are
   fully explained (193 empty-`.text` CANTUNWIND sentinels from `-ffunction-sections` + 1
   zero-size `AbyssEngine::getAppVersion`), and are already reproduced address-for-address.
   Raw prel31 operands still differ with local `.text` layout. See
   `_work/reconstructions/exidx_extab_exact_w3462.md` and
   `_work/reconstructions/exidx_duplicate_address_root_cause_w3565.md`.
4. **Link metadata.** `.dynstr`/`.hash`/`.rel.dyn`/`.rel.plt`/`.dynamic` are byte-identical
   (5,578 dynsym entries, 5,370 defined symbols); the gold hash-table iteration order is
   reproduced. `.dynsym` differs in 661 bytes and `.shstrtab` in 5 bytes only because of the
   extra `.jcr` section. The original's full-RELRO link (`-z now`) is reproduced (BIND_NOW,
   `.got` before `.data` inside `PT_GNU_RELRO`, `.got` at 0x20fcf4). `.data.rel.ro`
   exported-symbol addresses are exact. The GNU build-id is the remaining content hash
   (20 differing bytes) and converges last.

## Done
- **Pass 1: strip** the link output with the original's post-link GNU `strip --strip-all`
  (previously gold `-s`) -> section set matches the original (no .symtab/.strtab/.debug_*) and the
  bfd section-header normalization is exact. bfd rewrites `.shstrtab` into section-header order;
  `tools/reorder_shstrtab.py` restores the gold-Stringpool order from the unstripped link output.
  This also makes the build-id a hash of the unstripped link output, as in the original. Parity
  unaffected (35/0/0); verify/sodiff read .dynsym+.text.

## Reality
Same-hash == completing the decompile to byte-perfection: make the remaining 24 functions exact
and close the last EH/build-id bytes; `.rodata`, `.data` and the dynamic tables are already
byte-identical. Progress is tracked by: byte_exact/linked_exact function counts (verify) and the
section-size deltas above.

## Next levers (tractable, bounded first)
- Byte-exactness campaign: verify.py worst-first, fix near-miss (80-99%) high-value functions first.
- Remove the `.jcr` rank table without losing `Status::missionCompleted`'s 87.0% body order,
  restoring the 27-section table and the byte-identical `.dynsym`/`.shstrtab`.
- ~~Close the last 8 B of `.ARM.extab`: `Level::createShip` -8 (328 vs 336).~~ Done in
  wave 123: the real-`new` body gives the 336 B cleanup-only record, so `.ARM.extab` is
  size- and base-exact and `.ARM.exidx` is byte-identical.
- Implement missing engine functions in batches (math/Vector/Matrix shims are the densest cluster).

## Concrete leads found (for the byte-exactness campaign)

The verify report (cmake-build-match/verify/report.json) has per-function rows: 4500 byte_exact,
4500 linked_exact, 4524 size_match of 4524; pct buckets {100:4500, 90-99:10, 80-89:5, 70-79:6,
<60:3}. 0 function rows size-MISMATCH (the `Layout::Layout` body is size-exact 7072 B without a
pad); 16 size-match >=80% not-linked (the highest-value near-misses).

Systematic clusters (one cause fixes many):
1. **Shader `UpdateMeshData` cluster (resolved).** The former ~15 fns at 86-94%
   (GenericShader/VertexColorShader/BumpShaderV2/4/
   CubeMapping/EnergyShield/...). Diff via direct disasm: the only real (non-relocation) deltas are
   field-offset drift. In `GenericShader`, fields from `uSpecularColor` onward are +4 in the original
   (orig 0x5c/0x60/0x68/.. vs ours 0x58/0x5c/0x64/..) -> our decomp is missing a 4-byte uniform-location
   field between uDiffuseColor and uSpecularColor (or has them mis-ordered). Plus a 1-byte Mesh field
   drift (orig [r4,#9] vs ours [r4,#10]). Fixing the shared base/layout flipped the whole cluster;
   all of these are byte-exact in the current report.
   NOTE: this was layout drift that field_0xNN scanning can't see (semantic-named fields) -- a class of
   drift beyond [[drift-campaign]]. A generalized ASM-offset-diff drift detector would surface all of it.
2. **ParticleSettings_str[401401]={0} (resolved).** The former 392KB zero array (the .rodata
   bloat) was a mis-sized decomp artifact; the real particle-name string table is a few KB. The
   `.rodata` passes recovered the original data, and `.rodata` is now byte-identical (0 differing
   bytes).

## PaintCanvas layout reconstruction (in progress — concrete data)

offdrift + per-method disasm give the original offsets (vs ours) for PaintCanvas fields:
- `engine`        orig 0x34  ours 0x3c   (GetWidth, SetWorldViewMatrix: `ldr [r0,#0x34]`)
- `meshCount`     orig 0x24  ours 0x30   (MeshSetUv: `ldr.w ip,[r0,#0x24]`)
- `meshes`        orig 0x28  ours 0x34   (MeshSetUv: `ldr [r0,#0x28]`)
Deltas: meshCount/meshes are +0xc (our struct has 12 extra bytes before 0x24); engine is +8 (so our
struct is ALSO missing 4 bytes between `meshes` and `engine` — orig has 2 fields there [0x2c,0x30],
we have 1 [`gameOrientation`]). Root: `::Array<AELoadedTexture*> cubeTextures` (12B, count/data_/
capacity_) is MIS-PLACED at 0x20 in ours; the original has it elsewhere. All early fields
(culledCount, quad2dMesh, field_0xc/14/1c, mask2dImage, gameOrientation, cubeTextures) ARE used, so
this is a RE-ORDER, not a deletion.

### Safe methodology for any struct (no-degradation gated)
1. Disassemble every method of the struct (ours+orig), align 1:1 (size-match), map each our-offset to
   its orig-offset; translate offsets->field names via the source. This yields the COMPLETE original
   field order (a full layout reconstructor tool over offdrift is the next tooling step).
2. Reorder/repad the header to match, atomically (the whole struct, not a few fields).
3. Rebuild + relink + verify: keep ONLY if byte_exact/linked_exact counts RISE and parity stays
   35/0/0; otherwise revert. The verify counts are the hard no-degradation gate.

This is the engine for the byte-exactness campaign; each reconstructed struct flips many functions.
Dozens of structs drift (offdrift delta histogram: -8:14, +4:12, +8:11, +32:9, ... across 300 fns).

## GROUND-TRUTH unblock: Ghidra has the original loaded

The original (android_2.0.16_libgof2hdaa.so, 8096 fns, image_base 0x10000) is OPEN in Ghidra (MCP).
`get_struct_layout <Struct>` returns the real field offsets; `decompile_function <addr>` shows real
field usage. This is the authoritative source for the byte-exactness campaign — reconstruct each
struct from Ghidra's layout, not from guesses. The verify byte_exact/linked_exact count remains the
no-degradation gate when applying.

### PaintCanvas — definitive layout (Ghidra get_struct_layout, size 524):
0x0 gravityRotationEnabled(=initialized), 0x4 culledCount, 0x8 field_0x8(=quad2dMesh), 0xc field_0xc,
0x10 field_0x10, 0x14 field_0x14, [0x18 gap], 0x1c field_0x1c, 0x20 field_0x20(int), 0x24
field_0x24(=meshCount), 0x28 field_0x28(=meshes), [0x2c gap=mask2dImage], 0x30 orientation
(=gameOrientation), 0x34 engine, 0x38.. matrices, ... 0x150 images, 0x164 cameras, ...
DECOMP BUG: `ChangeCubeTexture` (decompiled) reads cube count@field_0x10 / data@field_0x14 -> the
cube-texture Array IS field_0x10/0x14/0x18 (count/data/capacity @ 0x10). Our decomp instead made a
SEPARATE `cubeTextures` Array at 0x20, displacing field_0x20/meshCount/meshes by +0xc (root of the
~10 PaintCanvas near-misses, e.g. engine 0x3c-should-be-0x34). FIX: map `cubeTextures`->0x10 (merge
with field_0x10/0x14/0x18 uses), add the standalone `field_0x20` int, shift meshCount/meshes/
gameOrientation/engine back by 0xc. Verify-gate each step.

## Session 2026-11-04 (wave 110 — 20-agent worst-first fleet)

- Gate after harvest: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- **Structural win: the `.jcr` section is gone.** `Status::missionCompleted`'s rank table is
  now the last `.rodata` input (`tools/mission_case_table.cpp`), landing at `0x20b977` (first
  byte past the original `.rodata` image). Result: 27 sections (exact), exact file size
  2,187,328 B, `.dynsym`/`.shstrtab` content byte-identical. Cost: `.text` 95,946 -> 95,999
  (+53 B in that function's literal-pool reference) and 192 B of extra `.rodata` (the table
  itself). This supersedes the wave-109 last-`.rodata` attempt that perturbed `.dynstr`.
- Landed fuzzy improvements:
  - `RocketGun::update` 92.4 -> **93.8** (heading/z-arm reorder).
  - `HangarWindow::render` 88.52 -> **88.71** (normalized `buttonInset` sums, upgrade test swap).
  - `Generator::createMission` 82.1 -> **82.6** (hoist `padA` next to `amount`).
  - `MGame::OnTouchEnd` 78.88 -> **78.92** (`char refinedAny` restores the `lsls` flag test).
  - `Layout::Layout` 48.269 -> **48.338** (`0x9c` int-phi + four int-phi wraps).
- Notes only (measured dead ends, local optima): Level::createShip (332 B LSDA target),
  Radar::draw, Globals::Globals, PlayerEgo::calcCollision, Level::createScene (93.3% pinned to
  sink+mask 35/37), Gun::calcCharacterCollision, Level::createMission, SpaceLounge::drawLounge,
  MenuTouchWindow ctor, MenuTouchWindow::OnTouchBegin + MGame::OnUpdate.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.3, LevelScript::process
  56.1, Radar::draw 74.0, Level::createShip 74.4, MGame::OnTouchEnd 78.9, Globals C1/C2 80.5,
  Generator::createMission 82.6, Status::missionCompleted 87.0, PlayerEgo::calcCollision 87.6,
  Level::createScene 88.3, HangarWindow::render 88.7, Gun::calcCharacterCollision 90.0,
  Level::createMission 92.1, RocketGun::update 93.8, Hud::draw 94.6, SpaceLounge::drawLounge
  95.5, MenuTouchWindow C1/C2 95.7, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 98.6,
  MGame::OnUpdate 99.9.
- Section deltas: `.text` 95,999; `.ARM.exidx` 1,683; `.ARM.extab` 128,950 (size -4);
  `.rodata` 192 (appended table); build-id 20. Everything else exact, file size exact.

## Session 2026-11-05 (waves 111-112 — worst-first fleet + build archaeology)

- Gate: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- **`.ARM.extab` is now size-exact (`0x26298`).** `Level::createShip` was restored to the
  faithful real-`new` shape (336 B / 57-row LSDA, 66.6%), which cancels `Hud::draw` +4 and
  `Generator::createMission` -4. `.ARM.exidx` differing bytes fell 1,683 -> 1,063.
- Landed fuzzy improvements:
  - `Level::createScene` 88.3 -> **90.5** (loop-variant Vector address + rnd mask 47).
  - `Level::createShip` 74.4 -> 61.9 (332 B device) -> **66.6** (faithful real `new`).
  - `Hud::draw` 94.6 -> **94.9** (second-bar fadeColor if/else).
  - `ModStation` ctor 98.6 -> **99.2** (yaw-first bitcasts, yaw live into far rotation).
  - `MenuTouchWindow` ctor 95.69 -> **96.16** (loop-scoped `i`, id-first tail).
  - `SpaceLounge::drawLounge` 95.5 -> **95.7** (per-file 7:1 branch weight).
  - `Layout::Layout` 48.269 -> **48.426** (0x130/0x134/0x138 branch-local fusion).
  - `HangarWindow::render` 88.52 -> **89.06** (textX+3 operand order, pV placement).
  - `LevelScript::process` 56.09 -> **57.6** (de-nest case-125 state chain, ascending case
    order; supersedes the 57.19 statement-order variant).
- **Build flags are no longer a guess.** Wave-111 research (w5917/w5918/w5919) re-derived the
  recipe from primary evidence: NDK r18b, clang 7.0.2, gold 1.12, API 16, armeabi-v7a,
  Thumb-2/NEONv1/softfp, static libc++, `-Oz -g -std=c++11 -fPIC -frtti -fexceptions
  -fstack-protector-strong -funwind-tables -ffunction-sections`, no `-fdata-sections`, default
  visibility; one plain gold link (`-shared -soname libgof2hdaa.so --build-id -z now
  --exclude-libs=libgcc.a`), then `strip --strip-all` + `.shstrtab` normalization. 1,136
  per-TU flag cells, 128 `-mllvm` cells, 160 combinations and 384 branch-weight cells found
  **no** strict gate improvement. A second official 2.0.16 build is NDK r14 (clang 3.8, gold
  1.11, gnustl), not r18b.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.4, LevelScript::process
  57.6, Level::createShip 66.6, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 78.9,
  Generator::createMission 82.6, Status::missionCompleted 87.0, PlayerEgo::calcCollision 87.6,
  HangarWindow::render 89.1, Gun::calcCharacterCollision 90.0, Level::createScene 90.5,
  Level::createMission 92.1, RocketGun::update 93.8, Hud::draw 94.9, SpaceLounge::drawLounge
  95.7, MenuTouchWindow C1/C2 96.2, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.
- Section deltas: `.text` 97,001; `.ARM.exidx` 1,063; `.ARM.extab` 118,463 (size exact);
  `.rodata` 192 (appended table); build-id 20. Everything else exact, file size exact.

## Session 2026-11-05 (wave 113 — worst-first fleet, continued)

- Gate: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS. `.ARM.extab` stays size-exact.
- Landed fuzzy improvements:
  - `LevelScript::process` 57.6 -> **58.0** (w5904 move subset: case-80/91/92/106/131).
  - `Layout::Layout` 48.34 -> **48.50** (0x2d8 sum condition, 0x184 test order, forced retina
    reload, 0x2d0 round-trip fix).
  - `Level::createShip` 66.6 -> **67.3** (setShipGroup flag + cargo switch).
  - `MenuTouchWindow` ctor 96.16 -> **97.21** (narrow row2 local, `i != 10` loop).
  - `HangarWindow::render` 89.06 -> **89.34** (free-credits Layout alias steers regalloc).
  - `Level::createMission` 92.094 -> **92.145** (setScaling receiver/position sweep).
  - `Globals` kept 78.8 but now has **both** the faithful 31/31 GOT-pool order and the
    `movw #583` hoist at index 34 in one store (late-block reorder).
- `.text` differing bytes fell 97,001 -> **94,443**. All other section deltas unchanged:
  `.ARM.exidx` 1,063; `.ARM.extab` 118,463 (size exact); `.rodata` 192 (appended table);
  build-id 20. 27 sections, exact file size.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.5, LevelScript::process
  58.0, Level::createShip 67.3, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 78.9,
  Generator::createMission 82.6, Status::missionCompleted 87.0, PlayerEgo::calcCollision 87.6,
  HangarWindow::render 89.3, Gun::calcCharacterCollision 90.0, Level::createScene 90.5,
  Level::createMission 92.1, RocketGun::update 93.8, Hud::draw 94.9, SpaceLounge::drawLounge
  95.7, MenuTouchWindow C1/C2 97.2, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 114 — worst-first fleet, continued)

- Gate: `byte_exact 4500/4524`, `linked_exact 4500/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS. `.ARM.extab` stays size-exact.
- Landed fuzzy improvements:
  - `HangarWindow::render` 89.3 -> **91.2** (faithful `selectedItem->ship` + `b`, `bool visible`).
  - `Gun::calcCharacterCollision` 90.0 -> **92.8** (named `playerPosition` temp, `hitFlags` at
    k-loop tail).
  - `Status::missionCompleted` 87.0 -> **88.3** (37-entry rank table + body-size fixes land the
    first body at 0xfe and latch at 0x522).
  - `Generator::createMission` 82.6 -> **83.3** (bitmask branch hint fixes spill-slot allocation).
  - `Level::createShip` 67.3 -> **67.4** (hitpoint type-chain source order 0x2c/0x31/0x33).
  - `LevelScript::process` 58.00 -> **58.02** (case-157 decl order + adjacent-statement swaps,
    cached player at two sites).
  - `MenuTouchWindow` ctor 97.214 -> **97.244** (signed loop bound restores the `bcc` backedge).
- `.text` differing bytes fell 94,443 -> **92,493**. All other section deltas unchanged:
  `.ARM.exidx` 1,063; `.ARM.extab` 118,463 (size exact); `.rodata` 192 (appended table);
  build-id 20. 27 sections, exact file size.
- Remaining 24 non-exact functions (all size-exact): Layout C1/C2 48.5, LevelScript::process
  58.0, Level::createShip 67.4, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 78.9,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Status::missionCompleted 88.3,
  Level::createScene 90.5, HangarWindow::render 91.2, Level::createMission 92.1,
  Gun::calcCharacterCollision 92.8, RocketGun::update 93.8, Hud::draw 94.9,
  SpaceLounge::drawLounge 95.7, MenuTouchWindow C1/C2 97.2, MenuTouchWindow::OnTouchBegin 98.1,
  ModStation C1/C2 99.2, MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 115 — first new byte-exact function)

- Gate: `byte_exact 4501/4524`, `linked_exact 4501/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- **`RocketGun::update` is now byte-exact (100.0%)** — the first function to leave the
  non-exact set since the campaign began: `trailMatrices` `operator[]` + faithful shape.
  `byte_exact` moved 4500 -> **4501**.
- Landed fuzzy improvements:
  - `Status::missionCompleted` 88.3 -> **91.5** (rank table re-based to 0..39, dispatch drops
    the subtraction).
  - `Hud::draw` 94.86 -> **95.46** (docking-bar decl order/types + second-bar decl order merge
    EH pad; the Hud LSDA record is now the original's exact 476 B).
  - `Globals` 78.76 -> **80.5** (`status` in the early pointer block + the 80.5 late order).
  - `Level::createScene` 90.53 -> **90.72** (address-expression scheduling fix).
  - `LevelScript::process` 58.02 -> **58.10** (case-92 state-2/state-4 decl and statement
    order).
  - `Gun::calcCharacterCollision` 92.8 -> **93.4** (drop `dmg` hoist, `hitFlags` at scatter
    head, volatile `0.2f` read).
- `.text` differing bytes fell 92,493 -> **91,907**. `.ARM.extab` is 4 B short again
  (`0x26294`), but for a *cleaner* reason: `Hud::draw` is now exact, so
  `Generator::createMission` (168 B vs 172 B) is the only remaining record delta.
  `.ARM.exidx` 1,663; `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 23 non-exact functions (all size-exact): Layout C1/C2 48.5, LevelScript::process
  58.1, Level::createShip 67.4, Radar::draw 74.0, MGame::OnTouchEnd 78.9, Globals C1/C2 80.5,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Level::createScene 90.7,
  HangarWindow::render 91.2, Status::missionCompleted 91.5, Level::createMission 92.1,
  Gun::calcCharacterCollision 93.4, Hud::draw 95.5, SpaceLounge::drawLounge 95.7,
  MenuTouchWindow C1/C2 97.2, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 116 — Layout pool audit, MenuTouchWindow 99.0)

- Gate: `byte_exact 4501/4524`, `linked_exact 4501/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- Landed fuzzy improvements:
  - `MenuTouchWindow` ctor 97.24 -> **99.04** (faithful cinematic block + reassociation masks).
  - `LevelScript::process` 58.05 -> **58.20** (case-91/92 statement order + mission-40 state-6
    epilogue order).
  - `Layout::Layout` 48.545 -> **48.60** (faithful int-store typing + `field_0x2c8` float
    round-trip).
  - `Status::missionCompleted` 91.5 -> **91.9** (rank table re-based to the original TBH slot
    pattern).
  - `Hud::draw` 95.46 -> **95.61** (docking-bar total/barW widths).
- `.text` differing bytes fell 91,907 -> **88,975**. `.ARM.extab` remains 4 B short
  (`0x26294`), solely because `Generator::createMission`'s record is 168 B vs the original
  172 B; `.ARM.exidx` 1,663; `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 23 non-exact functions (all size-exact): Layout C1/C2 48.6, LevelScript::process
  58.2, Level::createShip 67.4, Radar::draw 74.0, MGame::OnTouchEnd 78.9, Globals C1/C2 80.5,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Level::createScene 90.7,
  HangarWindow::render 91.2, Status::missionCompleted 91.9, Level::createMission 92.1,
  Gun::calcCharacterCollision 93.4, Hud::draw 95.6, SpaceLounge::drawLounge 95.7,
  MenuTouchWindow::OnTouchBegin 98.1, MenuTouchWindow C1/C2 99.0, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 117 — createShip 73.5, MenuTouchWindow 99.7)

- Gate: `byte_exact 4501/4524`, `linked_exact 4501/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.92**. RATCHET PASS.
- Landed fuzzy improvements:
  - `Level::createShip` 67.4 -> **73.5** (wave-89 placement-new + `try` shape with the correct
    ctor/declaration order). Trade-off: its LSDA shrinks 336 -> 328 B, so `.ARM.extab` goes
    from 4 B short to 12 B short.
  - `MenuTouchWindow` ctor 99.04 -> **99.70** (`sliders[2]` original left grouping + inner
    mask).
  - `Status::missionCompleted` 91.9 -> **92.5** (TBH-target LCS re-tune of the rank labels).
  - `Hud::draw` 95.61 -> **95.90** (`Globals::Canvas` null-test anchor).
  - `Level::createMission` 92.15 -> **92.29** (volatile `size()` latch in `case4Tail`).
  - `MGame::OnTouchEnd` 78.92 -> **79.05** (campaign-first dispatch + campaign reuse +
    `hasWon() == true`).
  - `LevelScript::process` 58.19 -> **58.20** (case-78 s12 store after return; case-154
    state-1 epilogue order).
- `Globals` 80.5 -> **78.8** but with stores 96-103 byte-exact (volatile settings tail) — a
  faithfulness-for-fuzzy trade, like the pool-faithful variant.
- `.text` differing bytes 88,975 -> **89,185** (the createShip placement shape is worse in raw
  bytes even though its fuzzy score is +6.1). `.ARM.extab` 12 B short; `.ARM.exidx` 1,704;
  `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 23 non-exact functions (all size-exact): Layout C1/C2 48.6, LevelScript::process
  58.2, Level::createShip 73.5, Radar::draw 74.0, Globals C1/C2 78.8, MGame::OnTouchEnd 79.0,
  Generator::createMission 83.3, PlayerEgo::calcCollision 87.6, Level::createScene 90.7,
  HangarWindow::render 91.2, Level::createMission 92.3, Status::missionCompleted 92.5,
  Gun::calcCharacterCollision 93.4, SpaceLounge::drawLounge 95.7, Hud::draw 95.9,
  MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2, MenuTouchWindow C1/C2 99.7,
  MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 118 — Globals exact, Generator LSDA exact)

- Gate: `byte_exact 4503/4524`, `linked_exact 4503/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.93**. RATCHET PASS.
- **`Globals::Globals` C1/C2 are now byte-exact (100.0%)** — restoring the original late-block
  source order. `byte_exact` moved 4501 -> **4503**.
- **`Generator::createMission` now has the original's exact 172 B LSDA record** (volatile
  `type` + base split + tail hint). This costs fuzzy (83.3 -> 50.1) but removes the last
  non-createShip `.ARM.extab` delta; `.ARM.extab` is now 8 B short solely because of
  `Level::createShip` (328 vs 336).
- Other landed gains:
  - `MGame::OnTouchEnd` 79.05 -> **80.14** (merge 96/127, refetch campaign mission, `== true`
    select).
  - `Hud::draw` 95.89 -> **95.99** (mission-panel `ldrd` operand order).
  - `Layout::Layout` 48.555 -> **48.595** (`rewardBoxHeight` ternary + 0x138 double round
    trip).
- `Level::createShip` stays 73.5 (placement-new + `try`, LSDA 328 B).
- `.text` differing bytes 89,185 -> **89,836** (the Generator LSDA fix reshapes the body).
  `.ARM.exidx` 1,327; `.ARM.extab` 108,063 (8 B short); `.rodata` 192; build-id 20. 27
  sections, exact file size.
- Remaining 21 non-exact functions (all size-exact): Layout C1/C2 48.6,
  Generator::createMission 50.1, LevelScript::process 58.2, Level::createShip 73.5,
  Radar::draw 74.0, MGame::OnTouchEnd 80.1, PlayerEgo::calcCollision 87.6,
  Level::createScene 90.7, HangarWindow::render 91.2, Level::createMission 92.3,
  Status::missionCompleted 92.5, Gun::calcCharacterCollision 93.4, SpaceLounge::drawLounge
  95.7, Hud::draw 96.0, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MenuTouchWindow C1/C2 99.7, MGame::OnUpdate 99.9.

## Session 2026-11-05 (wave 119 — MenuTouchWindow ctor exact)

- Gate: `byte_exact 4505/4524`, `linked_exact 4505/4524`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, avg **99.93**. RATCHET PASS.
- **`MenuTouchWindow::MenuTouchWindow` C1/C2 are now byte-exact (100.0%)** — hoisting row2
  before `addButton(19)`. `byte_exact` moved 4503 -> **4505**.
- `Level::createScene` 90.724 -> **92.757** (`-g` Vector address, y/z/x store order, rnd mask).
- `Radar::draw` 74.030 -> **74.427** (local `String` label in the landmark `i == 0` arm).
- `PlayerEgo::calcCollision` 87.644 -> **95.2** (A2+visible size guards, swapped compare,
  tail pad; 968 B).
- `Level::createShip` 73.5 -> **74.4** (tuned spawn slot-map), then the faithful
  real-`new` shape landed at **70.2** with the exact 336 B LSDA (the original really uses
  real `new`; `.ARM.extab` is now only 4 B short, from Generator).
- `Generator::createMission` 53.3 -> **83.5** (faithful shape + pad store before
  `fromSystemPtr`); its LSDA is 168 B, so `.ARM.extab` is **12 B short** again.
- `LevelScript::process` 58.2 -> **58.5** — merged w6603/w6604/w6704 reorders, body exactly
  57232 B (a 57240 B merge collapses byte_exact to 2740).
- `.text` differing bytes 89,836 -> **88,009**. `.ARM.exidx` 1,663; `.ARM.extab` 156,308
  (4 B short, Generator only); `.rodata` 192; build-id 20. 27 sections, exact file size.
- Remaining 19 non-exact functions (all size-exact): Layout C1/C2 48.6,
  Generator::createMission 83.5, LevelScript::process 58.5, Level::createShip 73.5,
  Radar::draw 74.4, MGame::OnTouchEnd 80.1, HangarWindow::render 91.2,
  Level::createMission 92.3, Status::missionCompleted 92.5, Level::createScene 92.8,
  Gun::calcCharacterCollision 93.4, PlayerEgo::calcCollision 95.2, SpaceLounge::drawLounge 95.7,
  Hud::draw 96.0, MenuTouchWindow::OnTouchBegin 98.1, ModStation C1/C2 99.2,
  MGame::OnUpdate 99.9.
- Build provenance re-audited (w6719): 72-variant global matrix + 378-cell per-TU matrix,
  no flag beats 4505; `-Oz` and `-g2` are uniquely correct; the residual is source shape.
