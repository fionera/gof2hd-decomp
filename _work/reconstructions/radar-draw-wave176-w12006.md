# Radar::draw wave 176 (w12006) — statement reordering around the dock guard and wreck path

**Status:** landed, 2026-11-11. Worktree `wt/w12006`, branch `wave176-w12006`,
base `master` `b21088985` (wave-175 merge). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave175-w11907.md` (the landed planet-lock
`expect(field_0x1a8, 0)` + `(timer < 1) | (mbp != 0)` shape),
`radar-draw-wave175-w11906.md` (skip-branch `shipGroup` re-test + `bool
wreck`) and `radar-draw-wave174-w11907.md` (campaign/`sl` allocation is the
dominant residual and is size-fatal to fix).

## 0. Baseline (rebuilt on this worktree)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 6,785 |
| instruction-index alignment | 1,657 |
| instructions | 3,540 ours / 3,532 orig |
| whole-`.so` `.text` differing bytes | 80,166 |
| `.ARM.exidx` / `.ARM.extab` differing | 0 / 746 (sizes/base exact) |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370) |

Build cost is ~1 s per variant (`link_so -j2` recompiles only `Radar.cpp` and
relinks), so every cell below is an authoritative clang build + `fn_diff`.

## 1. Landed change (four adjacent-statement reorderings, net size 0)

```diff
                     if (missionBlocksPlanets) {
                         hud->hudEvent(21, this->level->getPlayer(), 0);
                         goto enemies;
                     }
-                    if (!this->lockConfirmed) {
-                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
-                    }
                     this->field_0x14 = this->field_0x18;
                     if (__builtin_expect(this->field_0x1a8, 0)) {  // lint: arm_builtin
                         this->level->getPlayer()->dockToPlanet();
                     }
+                    if (!this->lockConfirmed) {
+                        Globals::sound->play(26, nullptr, nullptr, 0.0f);
+                    }
...
-            this->update(enemy);
             bool wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
             enemy->field_0x75 = 0;
+            this->update(enemy);
...
             if (this->onScreen) {
-                enemy->field_0x76 = 1;
                 this->egoPos = player->getPosition();
+                enemy->field_0x76 = 1;
                 this->targetPos = enemy->player->getPosition();
...
         tractor:
             if (this->tractorMode2 && this->field_0x1c == nullptr && wreck && enemy->isDead()) {
-                this->field_0x1c = enemy;
                 this->dockNavPtr = enemy;
+                this->field_0x1c = enemy;
                 enemyHit = true;
             }
```

All four swaps are between side-effect-free or mutually independent
statements:

* **planet block `S,F,D` -> `F,D,S`** (sound, `field_0x14 = field_0x18`,
  dock). `lockConfirmed` is read before the block; `dockToPlanet()` acts on
  `PlayerEgo` and the sound on `Globals::sound`; neither touches `Radar`'s
  `lockConfirmed`/`field_0x14`.
* **`bool wreck` before `Radar::update(enemy)`.** `wreck` reads
  `enemy->hasCargo`/`isDead()`/`isDying()`; `update` reads the enemy
  position. Independent.
* **`egoPos = player->getPosition()` before `enemy->field_0x76 = 1`.**
  Different objects; `getPosition()` is a pure read.
* **tractor `dockNavPtr = enemy` before `field_0x1c = enemy`.** Two
  independent pointer stores.

## 2. Result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | 9,908 |
| raw same-address diff | 6,785 | **6,758** |
| instruction-index alignment | 1,657 | 1,680 |
| instructions | 3,540 / 3,532 | 3,540 / 3,532 |
| whole-`.so` `.text` differing bytes | 80,166 | **80,139** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 746 | 746 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |

`check` after the commit: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub 0, missing 0, wrong_type 0,
extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**, drift **0**,
parity **5,370/5,370**. `RATCHET PASS`.

The raw/`.text` metric is the primary local metric and falls **-27**; the
aligned metric rises 1,657 -> 1,680 (+23) because the reorderings move a few
instructions relative to the original's block boundaries. The win is the
byte-identity measure, so it is landed and the aligned rise is reported
honestly.

## 3. Why the planet-block reorder wins

The original has the *station* lock sound guard at `0x1303d2`
(`ldrb r1,[r4,#536]; cbnz; ... play(26)`) and the *planet* block at
`0x130454`. The base planet block sits at `0x1303c4` (its sound guard at
`0x1303ca`), so its bytes do not line up with anything. Moving the planet
sound guard after the store+dock puts it at `0x1303d2`, exactly where the
original's station sound guard lives; the two 12-byte `play(26)` sequences
then match at the same address (22 bytes recovered). The remaining three
reorders are smaller coincidental alignments in the wreck/onScreen/tractor
path (1 byte each).

## 4. Variant table (all size 9,908 unless noted)

Planet-block statement order (`S` = `if (!lockConfirmed) play(26)`,
`F` = `field_0x14 = field_0x18`, `D` = `if (expect(field_0x1a8,0)) dock`):

| order | raw | aligned | size |
|---|---:|---:|---:|
| `SFD` (base) | 6,785 | 1,657 | 9,908 |
| `SDF` | 6,788 | 1,662 | 9,908 |
| `FSD` | 6,841 | 1,661 | 9,908 |
| **`FDS` (landed)** | **6,762** | 1,662 | 9,908 |
| `DSF` | 6,787 | 1,660 | 9,908 |
| `DFS` | 6,837 | 1,663 | 9,908 |

Wreck-path statement order (`U` = `update(enemy)`, `W` = `bool wreck = ...`,
`Z` = `enemy->field_0x75 = 0`), on top of `FDS`:

| order | raw | aligned | size |
|---|---:|---:|---:|
| `UWZ` (base) | 6,762 | 1,662 | 9,908 |
| `WUZ` | 6,761 | 1,679 | 9,908 |
| **`WZU` (landed)** | **6,760** | 1,668 | 9,908 |
| `UZW` / `ZUW` | 8,494 | 1,672 | 9,904 |
| `ZWU` | 8,484 | 1,687 | 9,904 |

Then `egoPos` before `field_0x76 = 1` -> **6,759**; tractor
`dockNavPtr` before `field_0x1c` -> **6,758**.

## 5. Rejected variants (all measured on the base / `FDS`)

* Planet-lock OR guard: `||` bool form, independent-if, reversed order,
  `!(timer >= 1 && !mbp)` — 6,840 / 6,840 / 6,998 (size exact, raw-worse).
* `field_0x1a8` plain (`if (this->field_0x1a8)`, `!= 0`, `expect(...,1)`,
  inverted `else`, ternary) — 6,880 raw / 9,900 B (size-short); with
  `bool missionBlocksPlanets` it recovers 9,908 but is 6,827 raw.
* `bool missionBlocksPlanets` on the landed shape — 9,916 B (size-fatal).
* `expect(missionBlocksPlanets, 0)` — 6,904 raw / 9,904 B; with `bool mbp`
  9,908 but 7,467.
* `expect(!lockConfirmed, 0)` in the planet block — 6,846 raw / 9,908 B.
* `expect(field_0x18 != 0, 0/1)`, plain frame test — 6,838-6,867.
* `isDockingToPlanet` single/plain expect — identical to base (6,785).
* Skip-branch `bossSeen`: `= true` (8,060 / 9,900), nested `if` (6,799),
  `if/else` (identical), `int bossSeen` (9,098 / 9,904).
* `bool wreck` -> `int wreck` — 8,544 / 9,916; with `bossSeen = true`
  6,798.
* `expect` on `hasCargo`/`isDead||isDying`/`enemyFlagsLo`/`stealFlagByte`/
  the continue guard — 8,368-10,065.
* Station block: swap sound/`lockedStation` (6,834), swap the two
  `stationLockTimer = 0` stores (8,130 / 9,888), move `landmarkHit = true`
  inside the `if (timer > 0)` (identical).
* Enemy-lock / gas-cloud / image-chain / tractor-guard reorders and the
  `otherLock` chain reorder — all >= 6,758 (mostly 8,000-9,000).
* The 768-cell grid over {dock expect/plain} x {int/bool mbp} x
  {`|`,`||`,independent} x {frame expect/plain} x {lock expect/plain} x
  {mission guard expect/plain} x {bossSeen re-test/true} x {bool/int wreck}
  contains **no** cell below 6,785.

## 6. Remaining residual

Unchanged from wave 175: the original keeps `campaign` in `sl` and
`missionBlocksPlanets` in `fp`, while ours spills `campaign` to `sp+144` and
uses `sl` for the shared zero (see `radar-draw-wave174-w11907.md` §3-4).
Every campaign-in-`sl` source shape is size-short and raw-worse, so the
spilled-campaign allocation remains the best known shape. The other residual
is the tail exception-pad site (`sp+220` twice vs the original's
`220`/`208`), which wave 164 showed is not source-reachable.

## 7. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12006/Radar.win4.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12006/` — `Radar.cpp.base`,
`Radar.win4.cpp` (landed source), `sweep.py` (variant harness),
`measure.sh`, `textdiff.py` (whole-`.so` section differ), `v1..v21.json`
(variant grids), `grid*.out` (768/128/64-cell sweeps).
