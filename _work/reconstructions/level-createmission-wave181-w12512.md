# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 181 / w12512

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave181-w12512`, base master `1e270aa0e` (wave-180 harvest 4).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

This pass continues the wave-180 bool/order sweep.  Nothing from wave 180 is
re-landed; all edits below are on top of the wave-180 tree (raw 3682).

## 1. Result

**Two size-exact code wins landed: six adjacent-statement swaps, then a
raid-loop four-statement rotation.**

```text
                                     size            raw    aligned  insn
base 1e270aa0e                       17172/17172     3682   1071     6206/6197
after 6 adjacent swaps               17172/17172     3654    941     6206/6197
after raid-loop rotation (final)     17172/17172     3647    956     6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 76,200 -> 76,165  (-35)
.ARM.extab   size   156,312 /   156,312   differing bytes    713 ->    711  (-2)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (cmp clean)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20
```

Net total-byte win **-37** (76,933 -> 76,896: `.text` + `.ARM.extab` + 20 for the
build-id, matching the task's gate convention).  `.text` and `.ARM.extab` stay
size- and base-exact (`.ARM.extab` @ `0x1a3b78`, 156,312 B).  The function is
still one of the 17 non-byte-exact bodies, so the ratcheted `byte_exact` count
is unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact.
`RATCHET PASS`; no baseline update needed.

## 2. Landed edits

All six are independent non-volatile stores / calls on distinct receivers (or
on the same receiver but disjoint members), so the source order is free and the
emitted schedule is what changes.  Found by an exhaustive adjacent-statement
swap sweep over the whole function body (420 candidates, each a real
`link_so` + `tools/fn_diff.py` measurement).

### 2a. Pirate-fortress turret loop (`~line 1972`)

```cpp
             v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
-            gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
-            ((PlayerTurret *) gun)->setScaling(6.0f);
             gun->cargo = nullptr;
+            gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
+            ((PlayerTurret *) gun)->setScaling(6.0f);
```

Emission order becomes `cargo`, `setPosition`, `setScaling` (of the six
permutations of these three statements, this one measures best: 3654; the
others are 3656/3657/3658/3659).  The store order is free: `cargo` is a plain
field store and the two calls touch unrelated state.

### 2b. `case4Tail` hunter count (`~line 2124`)

```cpp
     float hunters = ...;
-    int hunterCount = (int) (hunters + (*(float *) &Globals::options[44] - 0.5f) * hunters);
     this->enemies = new Array<KIPlayer *>();
+    int hunterCount = (int) (hunters + (*(float *) &Globals::options[44] - 0.5f) * hunters);
```

`hunterCount` is pure arithmetic; the allocation is hoisted ahead of it.

### 2c. `case 183` tail (`~line 2224`)

```cpp
                     this->player->setPosition(-30000.0f, 5000.0f, -100000.0f);
                     this->stationPosition = this->player->getPosition();
-                    this->timeLimit = 181000;
                     this->objectivesA = new Objective(3, 181000, this);
+                    this->timeLimit = 181000;
```

`Objective`'s ctor (`src/game/mission/Objective.cpp`) does not read
`Level::timeLimit`, so the two stores commute.

### 2d. `case 2` convoy loop (`~line 2301`)

```cpp
                         asteroidPos = ...;
-                        (*this->enemies)[convoyEnd + j]->player->setAlwaysFriend(true);
                         (*this->enemies)[convoyEnd + j]->setPosition(asteroidPos.x, asteroidPos.y + 2000.0f, asteroidPos.z);
+                        (*this->enemies)[convoyEnd + j]->player->setAlwaysFriend(true);
```

### 2e. `case 2` objectives (`~line 2312`)

```cpp
-                    this->objectivesA = new Objective(18, 0, convoyEnd, this);
                     this->objectivesB = new Objective(18, convoyEnd, total, this);
+                    this->objectivesA = new Objective(18, 0, convoyEnd, this);
```

Two independent allocations; the `Objective` ctors do not touch each other.

### 2f. `case 9` attacker positions (`~line 2387`)

```cpp
                     (*this->enemies)[attackerCount]->setPosition(-2500.0f, -300.0f, 27000.0f);
                     (*this->enemies)[attackerCount + 1]->setPosition(6500.0f, 3000.0f, 24000.0f);
-                    (*this->enemies)[attackerCount + 2]->setPosition(-4000.0f, -2000.0f, 19000.0f);
                     (*this->enemies)[attackerCount + 3]->setPosition(9000.0f, -6000.0f, 17000.0f);
+                    (*this->enemies)[attackerCount + 2]->setPosition(-4000.0f, -2000.0f, 19000.0f);
                     (*this->enemies)[attackerCount + 4]->setPosition(3000.0f, 7000.0f, 15000.0f);
```

Distinct ships, independent virtual calls.

## 3. Sweep methodology and measured variants

The sweep enumerated every pair of adjacent, same-indentation, semicolon-
terminated, non-control-flow statements in `createMission` (420 pairs), swapped
each pair textually, rebuilt with `-j2` under `ulimit -v 12000000`, and measured
`raw`/`aligned`/`size`/`.text`/`.ARM.extab`.  Results (raw, `.text`, `.extab`):

```text
base                                        3682  76,200  713
2a cargo-before-setPosition alone           3678  76,196  713
2a rotation cargo,setPosition,setScaling    3679  76,197  713
2b hunterCount after enemies alone          3680  76,198  713
2c timeLimit/objectivesA alone              3671  76,189  711
2d setPosition/setAlwaysFriend alone        3680  76,198  713
2e objectivesB before objectivesA alone     3681  76,199  713
2f attacker+3 before +2 alone               3676  76,194  713
all six combined (landed, 2a=CAB order)     3654  76,172  711
all six, 2a=BCA order                       3657  76,175  711
all six, 2a=ACB order                       3656  76,174  711
all six, 2a=BAC order                       3659  76,177  711
all six, 2a=CBA order                       3658  76,176  711
```

The combination is additive (3654), and `.ARM.extab` drops by 2 as a bonus.

### Rejected (semantics change)

* `float ox`/`float oy` declaration swap (raw 3680): swaps the two
  `nextInt(20000), nextInt(2)` RNG pairs.  The original's escort loop consumes
  the RNG as x, then y, then z (`0xb3be8`-`0xb3c36`), so the natural `ox, oy,
  oz` source order is required (same rejection as wave 180).
* `spawnPosition.y +=` / `spawnPosition.z +=` swap (raw 3681): same RNG-order
  problem.
* `player->setPosition(...)` / `stationPosition = player->getPosition()` swap
  (raw 3680): `getPosition()` reads the position just written.
* `spawnPosition.z += ...` / `enemies[i]->setPosition(spawnPosition)` swap
  (raw 3681): `setPosition` reads the updated `spawnPosition`.
* `KIPlayer *gun = (*enemies)[i];` moved before the `createStaticObject` that
  writes `(*enemies)[i]` (raw 3676): reads the stale slot.  The equivalent
  declare-then-assign form is codegen-neutral (3682).

## 4. Remaining residuals (unchanged from wave 180)

```text
vma        len  source
0xb328a    125  alien-turret loop: `(bigAlienWave + 2 * (i < 0))` select is a
                branchless `it/movne` in ours, a `cbz` branch in the original;
                every if/else / `!=0` / pointer-arith spelling canonicalises to
                the same branchless form.  The `(i<0)` term also supplies the
                extra 16 B stack slot that keeps the frame size exact.
0xb3b74    124  pirate-escort `name = getText(441)` / `setToSleep` block;
                wave-180's PIR reorder (pirateBase before createStaticObject)
                still measures best for raw even though the original emits
                createStaticObject first (reverting it is raw 3684).
0xb1688    105  case-2 first convoy loop: ours keeps `offset` in `r9` and
                spills `convoyEnd`; the original spills `offset` to `[sp,#120]`
                and keeps `convoyEnd` in `fp`.
0xb3a3e     90  pirate raider `setPosition` float schedule.
0xb397e     80  raid-loop `createShip`/`getWaypoint` schedule.
```

The case-2 `switch (k)` store order is already optimal for raw: a full
per-case permutation sweep (20 single-case variants) found no improvement over
the landed `case0=201, case1=120, case2=012, default=201` source order; the
original's emitted per-case order is `102/102/012/102` but source permutations
that produce it measure equal-or-worse raw.

`bool` -> `int` conversions are all size-fatal or neutral: `resetStation`,
`alienRace`, `hard`, `stationPirates`, `blueprint` are codegen-neutral; the
remaining bool locals change the function size by 4-160 B when widened.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12512
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
cmake --build cmake-build-match --target diff -j2
```

Expected: `raw=3647 aligned=956 size=17172/17172`, `.text` diff 76,165,
`.ARM.extab` diff 711, `.rel.plt` diff 0, `RATCHET PASS`, `DRIFT: 0`,
`SYMBOL PARITY ✓`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12512/`
(`sweep.py`, `sweep_lines.py`, `measure.py`, `diffmap.py`, `sameaddr.py`,
`adj.txt`, `adj_results.txt`, `v1.json`..`v8.json`).

## 6. Second win: raid-loop hitpoints reorder

The 4-line rotation sweep over the hot regions (`1955-2070`, `2280-2320`,
`2380-2400`, three rotations per consecutive simple-statement quad) found one
more safe win in the `if (raid)` / `inPirateLootOrbit` body (`~line 2012`):

```cpp
-                   Player *p = (*this->enemies)[i]->player;
-                   p->setHitpoints(p->getMaxHitpoints() * 2);
                    (*this->enemies)[i]->setSpeed(3.5f);
                    ((PlayerFighter *) (*this->enemies)[i])->setRotate(3);
+                   Player *p = (*this->enemies)[i]->player;
+                   p->setHitpoints(p->getMaxHitpoints() * 2);
```

`setSpeed`/`setRotate` do not touch the `player` pointer, so reading it after
them is equivalent.  Measured alone on the six-swap base: raw 3654 -> 3647,
`.text` 76,172 -> 76,165, `.extab` 711 unchanged, size 17172/17172.
(The other rotations of the same four lines are 3654 (`q2012_3`) or fail to
build; the `[setRotate, p, hitpoints, setSpeed]` spelling ties at 3647.)

The remaining safe-looking 4-rotation win, `Q2041_2` (raw 3648), rotates
`[pirateBase, createStaticObject, setToSleep, name]` so that `setToSleep`/`name`
run before `createStaticObject` writes the array slot; that reads the stale
slot, so it was not landed.
