# Radar::draw wave 175 (w11908) — whole-function expect + block-scope alias sweep

**Status:** landed, 2026-11-11. Worktree `wt/w11908`, branch `wave175-w11908`,
base `master` `5f599a137`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave174-w11909.md` (frame-path `blip` locals + planet-lock
expect + `int missionBlocksPlanets`) and `radar-draw-wave174-w11908.md`
(lock-image friend-first + far-plain `210`), and `radar-draw-wave173-w11809.md`
(planet-lock OR swap + frame-test expect).

## 0. Baseline (rebuilt on this base)

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | 9,908 |
| raw same-address diff | 6,817 | **6,809** |
| instruction-index alignment | 1,602 | 1,619 |
| instruction count | 3,536 ours / 3,532 orig | **3,534 / 3,532** |
| `.text` differing bytes | 80,439 | **80,431** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 750 | 750 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.bss` | 0 / 0 / 0 | 0 / 0 / 0 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact **4,507/4,524**,
size-exact **4,524/4,524**, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact **843/843**, imports **207**, lint **CLEAN**, drift **0**,
parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (one commit, four sites)

```diff
-                if (enemy->field_0x42 && enemy->player->isAlwaysEnemy()) {
+                if (__builtin_expect((enemy->field_0x42 && enemy->player->isAlwaysEnemy()), 1)) {  // lint: arm_builtin
...
                     } else if (enemy == locked) {
-                        if (enemy->player->enemyFlagsLo) {
+                        Player *target = enemy->player;
+                        if (target->enemyFlagsLo) {
                             image = this->image_0x7c;
-                        } else if (enemy->player->carriesFriendCargoFlag) {
+                        } else if (target->carriesFriendCargoFlag) {
                             image = this->image_0x80;
...
         if ((uint32_t) (current - 136) > 15 || ((1 << (current - 136)) & 0xe071) == 0 ||
-            (current != 151 && alwaysEnemySeen)) {
+            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
...
-                if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens()) {
+                if (__builtin_expect((Globals::status->inAlienOrbit()), 0) || Globals::status->getStation()->isAttackedByAliens()) {  // lint: arm_builtin
```

All four sites are size-neutral as a set: the function stays 9,908 B and
`.rel.plt` stays byte-identical. The whole-`.so` `.text` differing bytes drop
80,439 -> **80,431** (-8); `.ARM.extab` stays 750, `.ARM.exidx` stays 0.

### 1.1 Why each site is the right shape

* **`isAlwaysEnemy` guard (line 702, `whole66`).** The original keeps the
  `enemy->field_0x42` test as a `cbz` and the `isAlwaysEnemy()` call as a
  fall-through body (`12ede0 cbz r2, 12edf0` / `12ede2 blx isAlwaysEnemy`).
  Wrapping the whole `&&` condition in `__builtin_expect(..., 1)` keeps that
  hot fall-through and re-places the `alwaysEnemy` store; the remaining
  `ldr r1,[sp,#96]; orrs r1,r0` in ours is part of the stack-slot cascade.
  The site is neutral on its own but -2 raw bytes in combination with the
  two tail expects below.
* **Far-locked `enemy->player` alias (line 771, `alias97`).** The original
  loads the player pointer once (`12ef0a ldr.w r0,[r8,#4]`) and reuses that
  register for both flag tests (`12ef0e ldrb [r0,#0x5c]`,
  `12f106 ldrb [r0,#0x5d]`); the reconstruction was reloading it. A
  block-scope `Player *target = enemy->player;` makes the two reads one SSA
  value. `enemy->player` is a plain pointer field and there is no call
  between the two reads, so the alias is semantics-preserving. This is the
  same idiom already used in the frame/bar chain
  (`Player *target = enemy->player;`).
* **`current != 151 && alwaysEnemySeen` (line 1263, `sub1263`).** The
  original's third top-level operand of the music-event guard is the cold
  arm; `__builtin_expect(..., 0)` on that operand re-places the cold block.
  The resulting `subs/it/movne/tst` sequence is still not instruction-identical
  to the original's `cmp #151` / `beq` / `eors #1` / `bne` shape, but it
  moves 1 raw byte into agreement.
* **`inAlienOrbit() || isAttackedByAliens()` (line 1312, `sub1312`).** The
  original tests `inAlienOrbit()` with `cbnz` and then `isAttackedByAliens()`
  with a 32-bit `cmp r0,#0; beq.w` (`13039a blx inAlienOrbit;
  13039e cbnz r0,...; 1303a8 blx isAttackedByAliens; 1303ac cmp r0,#0;
  1303ae beq.w`). Hinting the first operand unlikely re-places the cold arm
  so the `isAttackedByAliens()` test uses a 16-bit `cbz` and the following
  block lands on the original offsets; measured -4 raw bytes.

The `// lint: arm_builtin` waivers are same-line on the `__builtin_expect`
tokens. `check` stays green (lint CLEAN, drift 0, parity 5,370).

## 2. Sweeps run

All builds were `-j2` under `ulimit -v 12000000`; scratch is
`/home/debian/GalaxyOnFire2/scratch/w11908/`.

### 2.1 Whole-function `__builtin_expect` sweep

Every `if`/`while` condition in `Radar::draw` (197 conditions, 394 cells:
whole condition wrapped in `__builtin_expect((cond), 0/1)`). Only one direct
size-exact raw win:

| site | expect | raw | aligned | size |
|---|---|---:|---:|---:|
| line 1297 `getCurrentCampaignMission() != 145` | 1 | 6,813 | 1,624 | 9,908 |

The other 196 conditions are neutral or worse; the two next-closest
size-exact cells are 6,816 (line 772 `enemyFlagsLo`, expect 0) and 6,817
(neutral). The line-1297 expect is the -4 win that the final combination
supersedes (it conflicts with `sub1312`, so it is not in the landed set).

### 2.2 Whole-function top-level operand sweep

Every top-level `&&`/`||` operand of every condition (208 operands, 416
cells). Two direct size-exact raw wins:

| site | operand | expect | raw | aligned | size |
|---|---|---:|---:|---:|---:|
| line 1312 `inAlienOrbit()` | 0 | 0 | 6,813 | 1,624 | 9,908 |
| line 1263 `(current != 151 && alwaysEnemySeen)` | 2 | 0 | 6,816 | 1,622 | 9,908 |

### 2.3 Block-scope alias sweep

Generated 175 candidates from repeated load expressions inside every brace
block of the function (`this->FIELD`, `Globals::NAME`, `EXPR->FIELD`,
`EXPR->data()[i]`, `EXPR->player`), inserting `auto alias = EXPR;` at block
scope and replacing the repeated reads. 37 candidates were size-exact; one
was a direct raw win:

| site | alias | raw | aligned | size |
|---|---|---:|---:|---:|
| far-locked chain (line 771) | `enemy->player` | 6,816 | 1,602 | 9,908 |

The top neutral aliases (`this->landmarks`, `this->planetTargets`,
`this->field_0x124`, `enemy->player` in the frame/bar block) combine to
6,809 but do not go below it.

### 2.4 Combination search

Pairwise/triple combinations of the winning cells and the neutral
body-different cells (whole-condition and subterm) were measured. The best
size-exact set is:

```
sub1312 + sub1263 + alias97 + whole66   -> raw 6,809, aligned 1,619, size 9,908
```

Removing any one site gives 6,810-6,811; adding `music` (line 1297) gives
6,861 (it conflicts with `sub1312`); adding `sub855`/`whole159` stays 6,809
but does not improve. The final source uses the four-site set above.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w11908/`
(`Radar.cpp.base`, `final.cpp`, `sweep_expect.py`, `sweep_subterm.py`,
`sweep_alias_auto.py`, `combo_*.py`, `expect_results.json`,
`subterm_results.json`, `alias_auto_results.json`, `combo_*.json`).
