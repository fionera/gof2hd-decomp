# Radar::draw wave 176 (w12005) — post-wave-175 expect/alias re-sweep

**Status:** landed, 2026-11-11. Worktree `wt/w12005`, branch `wave176-w12005`,
base `master` `b21088985` (wave-175 merge). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave175-w11905.md` (campaign-in-`sl` re-search),
`radar-draw-wave175-w11906.md` (skip-branch shipGroup re-test + `bool wreck`),
`radar-draw-wave175-w11907.md` (planet-lock expect moved to the dock guard) and
`radar-draw-wave175-w11908.md` (the reverted whole-function expect + alias set).

## 0. Baseline (rebuilt on `b21088985`)

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | 9,908 |
| raw same-address diff | 6,785 | **6,689** |
| instruction-index alignment | 1,657 | 1,689 |
| instruction count | 3,540 ours / 3,532 orig | 3,540 / 3,532 |
| whole-`.so` `.text` differing bytes | 80,166 | **80,070** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 746 | 746 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.bss` | 0 / 0 / 0 | 0 / 0 / 0 |
| file size | 2,187,328 | 2,187,328 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (one commit, six sites)

```diff
@@ enemy-locked label block
                     if (enemy == locked) {
-                        this->lockLabel = this->calcDistance(this->targetPos.x, this->targetPos.y, this->targetPos.z,
+                        auto target = this->targetPos;
+                        this->lockLabel = this->calcDistance(target.x, target.y, target.z,
                                                              this->cameraX, this->cameraY, this->cameraZ);
@@ enemy on-screen block
                         this->screenY < top + size * 2) {
+                        auto onPlanetCourse = this->field_0x1a8;
                         enemy->field_0x75 = 1;
-                        if ((wreck && this->hasTractor13) || (this->field_0x1b0 && !this->field_0x1a8)) {
+                        if ((wreck && this->hasTractor13) || (this->field_0x1b0 && !onPlanetCourse)) {
@@ enemy cargo tail
                                 Array<int> *cargo = this->lockedEnemy->cargo;
-                                if (cargo == nullptr) {
+                                if (__builtin_expect((cargo == nullptr), 0)) {  // lint: arm_builtin
@@ gas-cloud loop
         for (uint32_t i = 0; i < this->gasClouds->size(); ++i) {
+            auto camZ = this->cameraZ;
             PlayerGasCloud *cloud = (PlayerGasCloud *) this->gasClouds->data()[i];
@@ gas-cloud lock label
                     this->lockLabel = this->calcDistance(this->targetPos.x, this->targetPos.y, this->targetPos.z,
-                                                         this->cameraX, this->cameraY, this->cameraZ);
+                                                         this->cameraX, this->cameraY, camZ);
@@ asteroid lock else block
                 } else {
+                    auto hasScanner = this->hasScanner19;
                     this->asteroidLockTimer += mode;
                     if (this->asteroidLockTimer > this->field_0x1b8 - 200) {
                         if (!candidateWreck) {
-                            if (!this->hasScanner19) {
+                            if (!hasScanner) {
@@ music tail
         this->field_0x54 = 0;
-        if (Globals::status->getCurrentCampaignMission() != 145) {
+        if (__builtin_expect((Globals::status->getCurrentCampaignMission() != 145), 1)) {  // lint: arm_builtin
```

All six sites are semantics-preserving:

* **`target` / `onPlanetCourse` / `camZ` / `hasScanner`** are block-scope
  value copies of plain member loads. None of the members is written inside
  the block, and none of the intervening calls can write it:
  `targetPos`/`cameraZ` are written only in the `draw` prologue,
  `field_0x1a8` is written only before the enemy loop, and `hasScanner19` is
  written only in the constructor. The copies therefore remove repeated
  loads / extend live ranges without changing any value.
* **`expect(cargo == nullptr, 0)`** is a pure branch-weight hint. The
  original has the null-cargo arm out of line (`12f080 cmp r0,#0;
  12f082 beq.w 12f1e8`), so the cold hint is the faithful shape.
* **`expect(getCurrentCampaignMission() != 145, 1)`** is a pure branch-weight
  hint on the music block's outer guard.

## 2. Re-test of the reverted w11908 set

The four w11908 sites were re-measured on the post-wave-175 base. The
interaction that forced the revert is still present:

| w11908 site | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,785 | 1,657 | 9,908 |
| `W66` whole `isAlwaysEnemy` expect | 8,495 | 1,676 | **9,904** |
| `A97` far-locked `enemy->player` alias | 6,785 | 1,657 | 9,908 |
| `S1263` `(current != 151 && alwaysEnemySeen)` expect 0 | 6,784 | 1,679 | 9,908 |
| `S1312` `inAlienOrbit()` expect 0 | 6,783 | 1,657 | 9,908 |
| all four (w11908 set) | 8,541 | 1,684 | **9,904** |

`W66` is now size-fatal on its own (-4 B), so the four-site set cannot be
landed as-is. `A97` is codegen-neutral on this base. The two tail expects are
size-exact but individually only -1/-2 raw; they are superseded by the alias
set below and are not in the landed change.

## 3. Whole-function expect sweep (post-wave-175 base)

Every `if`/`while` condition in `Radar::draw` (197 conditions, 394 cells) and
every top-level `&&`/`||` operand (660 cells) was wrapped in
`__builtin_expect((cond), 0/1)`. Only two size-exact raw wins exist:

| site | expect | raw | aligned | size |
|---|---|---:|---:|---:|
| line 895 `cargo == nullptr` | 0 | 6,778 | 1,682 | 9,908 |
| line 1299 `getCurrentCampaignMission() != 145` | 1 | 6,783 | 1,657 | 9,908 |

Both are landed. Removing or flipping each of the 24 existing
`__builtin_expect` sites was also measured (48 cells); none is a size-exact
raw win.

## 4. Block-scope alias sweep (post-wave-175 base)

Two sweeps were run over every brace block of `draw`:

1. every `this->FIELD`, `Globals::NAME`, `EXPR->FIELD`, `EXPR->data()[i]`,
   `EXPR->player` read repeated in the block (1,051 candidates), and
2. every single-occurrence `this->FIELD` / `Globals::NAME` read (677
   candidates).

Size-exact raw winners (top cells):

| block (open-brace line) | alias | raw | aligned |
|---|---|---:|---:|
| 1207 asteroid lock else | `this->hasScanner19` | 6,711 | 1,664 |
| 1207 asteroid lock else | `this->lockConfirmed` | 6,711 | 1,664 |
| 872 `if (enemyHit)` | `this->blipSprite` | 6,725 | 1,667 |
| 872 `if (enemyHit)` | `this->field_0x1a9` | 6,761 | 1,662 |
| 952 gas-cloud loop | `this->cameraZ` | 6,774 | 1,652 |
| 952 gas-cloud loop | `this->cameraX` / `this->cameraY` | 6,775 | 1,656 / 1,654 |
| 912 enemy-lock tail | `this->blipSprite` | 6,775 | 1,659 |
| 912 enemy-lock tail | `this->field_0x1b4` | 6,778 | 1,660 |
| 799 enemy-locked label | `this->targetPos` | 6,783 | 1,659 |
| 844 / 839 enemy on-screen | `this->field_0x1a8` | 6,783 | 1,662 |

Combination search over the winners:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,785 | 1,657 | 9,908 |
| `hasScanner19` | 6,711 | 1,664 | 9,908 |
| `+ cameraZ` | 6,701 | 1,656 | 9,908 |
| `+ targetPos` | 6,699 | 1,658 | 9,908 |
| `+ field_0x1a8` | 6,697 | 1,663 | 9,908 |
| `+ cargo expect` | 6,690 | 1,688 | 9,908 |
| `+ campaign-145 expect` (landed) | **6,689** | 1,689 | **9,908** |

The four aliases are the load-hoist shape the original allocation needs in the
asteroid/gas/enemy tails; the two expects are the only size-exact whole-function
hints. The landed set is the only measured combination below 6,690.

## 5. Remaining residual

The dominant residual is unchanged: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`) and `missionBlocksPlanets` in `fp`, while ours spills
`campaign` to `sp+144` and uses `sl` for the shared zero (see
`radar-draw-wave175-w11905.md` §1-2). The three-way rotation
`otherLock -> r11`, zero/mbp -> `r10`, `campaign` -> spill still needs a
source shape that raises `campaign`'s spill weight without changing the block
layout; every size-exact campaign-in-`sl` shape measured in wave 175 is
raw-worse than its base.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12005/` —
`Radar.base.cpp`, `variants.py`, `multivar.py`, `sweep.py` (whole-condition
and top-level-operand sweeps), `sweep_existing.py`, `sweep_alias.py`
(repeated-read aliases), `sweep_alias1.py` (single-read aliases),
`secdiff.py` (section differing-byte counter), `make_final.py` (landed source
generator), `whole.log`, `operand.log`, `existing.log`, `alias.log`,
`alias1.log`.
