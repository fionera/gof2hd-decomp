# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13399

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13399`, base master `ca19fd252` (raw **3,030**,
aligned **1,135**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave190-w13391.md` (case-1/2/183 rotations
landed) and `level-createmission-wave189-w13359.md`. Scope this pass: the
cells w13391 did not exhaust — case-3/5/9 bodies, the raid block, the
alien-turret loop, and the `0x3200-0x3e00` callee-saved rotation.

## 1. Result — landed

```text
                                     size            raw    aligned  insn
base ca19fd252                       17172/17172     3030   1135     6210/6197
landed                               17172/17172     3009   1133     6210/6197
```

Raw **3,030 -> 3,009** (-21), aligned **1,135 -> 1,133** (-2), size exact
17,172 B. `.text` differing bytes **73,003 -> 72,982** (-21);
`.ARM.exidx` 0, `.ARM.extab` 603 unchanged, `.note.gnu.build-id` 20
unchanged. Total (text+extab+build-id) **73,626 -> 73,605** (-21).
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

Gate: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
`RATCHET PASS`. No baseline update.

## 2. The change: `case 2` first convoy loop statement order

In the first convoy loop of `case 2` the base emitted

```cpp
ship->setPosition(...);
(*this->enemies)[i]->setRoute(this->enemyRoute->clone());
(*this->enemies)[i]->getRoute()->reachWaypoint(0);
(*this->enemies)[i]->player->setAlwaysEnemy(true);
```

The original emits `reachWaypoint(0)` **before** `setRoute(clone())`
(permutation `P W R E`: `setPosition`, `reachWaypoint`, `setRoute`,
`setAlwaysEnemy`). Swapping the two statements is the whole patch:

```cpp
ship->setPosition(...);
(*this->enemies)[i]->getRoute()->reachWaypoint(0);
(*this->enemies)[i]->setRoute(this->enemyRoute->clone());
(*this->enemies)[i]->player->setAlwaysEnemy(true);
```

The change is a scheduling / register-allocation win of the same class as the
earlier accepted rotations. Semantic caveat: `reachWaypoint(0)` now runs on
the ship's old route before the new route is installed; the original does
exactly this, so the permutation is faithful to the binary rather than a
behaviour-preserving rewrite.

## 3. Angles tried and rejected (all measured, base 3,030)

- Station pirate loop: 24 permutations; base `C D B A` best. Moving
  `createStaticObject` first makes size **17,176** (fatal).
- Alien turret: `(bigAlienWave + (i < 0) + (i < 0))` is load-bearing;
  removing terms gives size **17,168**. Gun statement permutations neutral or
  worse. Removing `Vector p` in the fortress loop is size-fatal **17,316**.
- Raid block: hoisting/moving `int guns` worse or size-fatal; inlining
  `alienGuns + fortressGuns` is size-fatal **17,164**.
- Case 3/5: swapping `objectivesA`/`objectivesB` -> raw 3,033; local
  `objA`/`objB` -> raw 3,055; `int last` cache size-fatal **17,148**.
- Case 2 second loop: loot permutations base best; `spawnPosition`
  reorder base best (`SafterZ` raw 3,031); `offset` moves all worse.
- Case 1 attacker loop variants (`unsigned`, `!=`, `++i`, race copy)
  neutral raw 3,030. `int guns` variants neutral.

## Current-master harvest (wave 190)

Applied on master `dcb6b809c`: raw **3,030 -> 3,009** (-21), aligned
1,135 -> 1,133, size exact 17,172 B. `.text` **72,999 -> 72,976** (-23,
combined with the w13403 createShip win); `.ARM.extab` 603 and build-id 20
unchanged. `check` -> RATCHET PASS.
