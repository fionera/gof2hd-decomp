# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 182 / w12614

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave182-w12614`, base master `57e085059` (wave-181 harvest 5).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

This pass continues the wave-181 bool/order sweep. Nothing from wave 181 is
re-landed; the edit below is on top of the wave-181 tree (raw 3647).

## 1. Result

**One size-exact code win landed: a declaration reorder in `case 2`.**

```text
                                     size            raw    aligned  insn
base 57e085059                       17172/17172     3647   956      6206/6197
after (final)                        17172/17172     3632   948      6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 75,365 -> 75,350  (-15)
.ARM.extab   size   156,312 /   156,312   differing bytes    655 ->    657  (+2)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 19 (unchanged count)
```

Net total-byte win **-13** (76,039 -> 76,026: `.text` -15, `.ARM.extab` +2,
build-id 19 both sides).  `.text` and `.ARM.extab` stay size- and base-exact
(`.ARM.extab` @ `0x1a3b78`, 156,312 B).  The function is still one of the 17
non-byte-exact bodies, so the ratcheted `byte_exact` count is unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact.
`RATCHET PASS`; no baseline update needed.

## 2. Landed edit

`case 2` (`~line 2275`), the declaration block before the two convoy loops.
The original emits the `Vector asteroidPos` zero-init stores **after**
`Route`/`getDifficulty`/`getProductionGoodAmount`, and computes
`getProductionGoodAmount()` before `getDifficulty()`.  Reordering the pure
declarations reproduces that schedule:

```cpp
                 case 2: {
                     int coords[6] = {(int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0,
                                      (int) (float) (-20000 - Globals::rnd->nextInt(20000)), 0, 0, 0};
-                    Vector asteroidPos = {0.0f, 0.0f, 0.0f};
                     this->enemyRoute = new Route(coords, 6);
-                    int escorts = (int) ((float) mission->getDifficulty() / 10.0f * 4.0f);
                     int goods = mission->getProductionGoodAmount();
+                    int escorts = (int) ((float) mission->getDifficulty() / 10.0f * 4.0f);
+                    Vector asteroidPos = {0.0f, 0.0f, 0.0f};
                     int convoyEnd = escorts + 2;
```

All three statements are pure: `Route(coords, 6)` reads only `coords` and does
not touch `asteroidPos`; `getProductionGoodAmount()`/`getDifficulty()` are
getters with no RNG or side effects; the `Vector` default ctor only zeroes its
stack slot.  RNG order is unchanged.

Measured: raw 3647 -> 3632, `.text` 75,365 -> 75,350, `.ARM.extab` 655 -> 657,
size 17172/17172, `.rel.plt` diff 0.

## 3. Sweep methodology and measured variants

* **Triple rotations** over the whole `createMission` body (570 simple
  statements, 2 rotations each, ~1,136 `link_so` builds): no size-exact raw win.
* **Triple rotations** over the switch half (lines 2148-2577, ~300 builds): none.
* **Quad rotations** (`L1/L2/L3`) over the switch half (~620 builds): the only
  hit was `q94_L1` (the `Vector asteroidPos` move above), raw 3643 alone.
* On top of `q94_L1`, swapping `goods`/`escorts` gives raw 3632 (the landed
  form).  `goods`/`escorts` alone on the wave-181 base is raw 3648 (worse), so
  the two edits are additive only together.
* A full **position search** over the nine `case 2` declarations (72 moves)
  found no further improvement: 3632 is a local optimum for that block.
* `bool` -> `int` conversions (`introSystem`, `hasWanted`, `nameLeader`,
  `resetStation`, `siegedStation`, `bigAlienWave`, `alienWave`,
  `pirateFortress`, `raid`, `alienRace`, `hard`) are all size-fatal or neutral
  on this body; `alienRace`/`resetStation`/`hard` are codegen-neutral.
* `Globals::*` hoists (`status`, `rnd`, `globals`, `gameText` locals) are all
  much worse (raw 16,000+).
* `multiply vs branch` variants tried and rejected (worse or neutral):
  `1 - 2 * nextInt(2)` for `nextInt(2) == 0 ? 1 : -1` (all 10 sites, worse);
  `hard ? 40 : 20` -> `20 + hard * 20` (neutral); `<< hardCoreMode()` ->
  `* (1 + hardCoreMode())` (worse); `alienWave * (bigAlienWave ? 8 : 7)`
  (worse); `pirateFortress * 5` (neutral).
* `case 183` `switch (k)` per-case `offset[]` store order: all 20 single-case
  permutations measured; none beats 3647 (`case1=102` ties).
* `timeLimit`/`objectivesA` revert in `case 183`: raw 3664 (worse).
* `case 2` second-loop reorders (`setAlwaysFriend` before `asteroidPos`,
  `crateCaptured`/`cargo` swap, `Player *p` after `setRotationSpeed`): all
  raw 3648-3658 (worse).

## 4. Remaining residuals (unchanged from wave 181)

```text
vma        len  source
0xb3b74    124  pirate-escort `name = getText(441)` / `setToSleep` block;
                ours hoists `getWaypoint()->getPosition()` before
                `createStaticObject`, the original emits createStaticObject
                first (reverting measures worse: raw 3684).
0xb328a     94  alien-turret loop: `(bigAlienWave + 2 * (i < 0))` select is a
                branchless `it/movne` in ours, a `cbz` branch in the original.
0xb3a3e     90  pirate raider `setPosition` float schedule.
0xb397e     80  raid-loop `createShip`/`getWaypoint` schedule.
0xb1688    105  case-2 first convoy loop: ours keeps `offset` in `r9` and
                spills `convoyEnd`; the original spills `offset` to `[sp,#120]`
                and keeps `convoyEnd` in `fp`.
```

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12614
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
cmake --build cmake-build-match --target diff -j2
```

Expected: `raw=3632 aligned=948 size=17172/17172`, `.text` diff 75,350,
`.ARM.extab` diff 657, `.rel.plt` diff 0, `RATCHET PASS`, `DRIFT: 0`,
`SYMBOL PARITY ✓`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12614/`
(`sweep.py`, `rotfull.py`, `rot2.py`, `rot4.py`, `possearch.py`, `sec.py`,
`diffruns.py`, `align_ops.py`, `cand*.py`, `*.log`).
