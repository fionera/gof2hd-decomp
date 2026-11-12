# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13427

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13427`, base master `32b826e31` (raw **3,001**,
aligned **1,112**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

## 1. Result — landed

```text
                                     size            raw    aligned  insn
base 32b826e31                       17172/17172     3001   1112     6210/6197
landed                               17172/17172     2984   1119     6210/6197
```

Raw **3,001 -> 2,984** (-17), aligned **1,112 -> 1,119** (+7), size exact
17,172 B. `.text` differing bytes **72,922 -> 72,905** (-17);
`.ARM.exidx` 0, `.ARM.extab` **603 -> 602** (-1), `.note.gnu.build-id` 20
unchanged. Total (text+extab+build-id) **73,545 -> 73,527** (-18).
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

Gate: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
`RATCHET PASS`. No baseline update.

## 2. The change: `case 2` goods read before the route allocation

In the `case 2` body the base built the enemy route first and then read the
production-good amount:

```cpp
this->enemyRoute = new Route(coords, 6);
int goods = mission->getProductionGoodAmount();
```

The original reads the good amount **before** allocating the route
(`0xb16d4` body: the `getProductionGoodAmount` call precedes the `Route`
constructor and the `Route` store). Swapping the two statements is the whole
patch:

```cpp
int goods = mission->getProductionGoodAmount();
this->enemyRoute = new Route(coords, 6);
```

Both statements are independent (`goods` is only consumed by the later
`ArraySetLength` and the `convoyEnd + goods` bound), so the permutation is
behaviour-preserving. It removes 17 raw bytes and one `.ARM.extab` byte; the
aligned metric rises by 7 because the following case-2 block is re-laid-out.

## 3. How it was found

A 390-candidate adjacent-simple-statement swap sweep over the whole
`createMission` body on current master (`scratch/w13427/sweep_adj.py`). It was
the only candidate with `raw < 3001` at `size == 17172`; every other candidate
was neutral or worse (many size-fatal). This cell was not covered by the
w12512/w13359/w13399 sweeps, which ran on older baselines and/or other
statement classes.

## 4. Other measured non-wins on this baseline (all size 17,172 unless noted)

* `case 183` convoy `switch (k)` arm order: all 24 permutations measured;
  current `case0, case1, default, case2` best at 3,001; `case0, case1,
  case2, default` is 3,002; any arm before `case 0` is ~5,285.
* `case 2` escort `crateCaptured`/`setSpeed`/`cargo`/`setRotationSpeed`:
  all 24 permutations; current `C S A R` best at 3,001 (`C S R A` 3,005,
  `A C S R` size-fatal 17,180).
* raid-loop `setRotate`/`setSpeed`/`setHitpoints`: all 6 permutations;
  current `R S H` best at 3,001 (`S R H` 3,002, faithful `H S R` 3,009).
* alien-turret / fortress `gun` read after `createStaticObject` (the
  original's order): alien-only 3,002, fortress-only 3,007, both 3,008.
* `case 9` ascending `attackerCount+2`/`+3` `setPosition` order (faithful):
  3,007 raw but aligned 1,085; the literal-pool shift eats the win.
* `volatile int enemyRace` and a volatile in-loop copy: size-fatal
  (17,256) and raw ~16,500.
* moving `int guns = alienGuns + fortressGuns` after the raid `if`:
  size-fatal 17,164, raw 4,555.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13427
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=2984 aligned=1119 size=17172/17172`, `.text` diff 72,905,
`.ARM.extab` diff 602, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13427/`
(`dump.py`, `bytediff.py`, `structdiff.py`, `secdiff.py`, `sw_arms.py`,
`sw_c2.py`, `sw_raid.py`, `sweep_adj.py`).
