# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 185 / w12914

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave185-w12914`, base master `f390b0e68` (wave-184
harvest 10: raw 3588). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: continue the bool/order sweep on the wave-184 body (`bool` vs `int`,
multiply vs branch, declaration order, `Globals::*` hoists).  Land only
size-exact, `.rel.plt`-identical raw wins.

## 1. Result

**Two size-exact code wins landed: the `alienGuns` turret-table anchor
`2*(i<0)` -> `(i<0)+(i<0)`, and moving the `extraAliens` `Vector sp` copy
from after the `y` update to after the `x` update.**

```text
                                     size            raw    aligned  insn
base f390b0e68                       17172/17172     3588   1140     6206/6197
after anchor (commit 1)              17172/17172     3584   1140     6206/6197
after sp move (final)                17172/17172     3538   1140     6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,591 -> 74,541  (-50)
.ARM.extab   size   156,312 /   156,312   differing bytes    647 ->    647  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20 (unchanged)
```

Total differing bytes **75,258 -> 75,208** (-50).  `check` green:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, `size_exact 4524/4524`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
symbol parity **5370/5370**, `RATCHET PASS`.  No baseline update (the
function is still non-byte-exact).

## 2. Landed edits

### 2a. `alienGuns` turret anchor

In the `if (alienGuns != 0)` turret loop (`~line 1968`):

```cpp
-            const float (*t)[6] = (bigAlienWave + 2 * (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
+            const float (*t)[6] = (bigAlienWave + (i < 0) + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
```

`i` is always non-negative, so `(i < 0) + (i < 0)` is 0 and the selector is
semantically `bigAlienWave ? station : outpost`.  The anchor is required to
keep clang from hoisting the select out of the loop (plain selector: size
17,160).  Wave-180 (w12413) measured `add2` as 4 raw better but 195 aligned
worse on that wave's body; on the current wave-184 body the aligned metric is
**unchanged at 1140**, so this is a clean raw-only win.

### 2b. `extraAliens` `Vector sp` copy position

In the `extraAliens` spawn block (`~line 2066`):

```cpp
         this->spawnPosition.x += ...;
+        Vector sp = this->spawnPosition;
         this->spawnPosition.y += ...;
-        Vector sp = this->spawnPosition;
         this->spawnPosition.z += ...;
         int coords[3] = {(int) sp.x, (int) sp.y, (int) sp.z};
```

This extends w12815's scheduling crutch by one statement: `sp` now captures
the position after the `x` update only, so `coords` uses the pre-`y`/pre-`z`
values.  clang keeps `sp.x`/`sp.y`/`sp.z` in `s16`/`s18`/`s20` and the
`(int)` conversion sequence lines up with the original's `vldr`/`vcvt`/`vstr`
chain much better than the post-`y` copy.  Raw **3584 -> 3538** (-46),
size 17,172 exact, aligned 1140 unchanged.  A future byte-exact pass must
restore the fresh-`y`/fresh-`z` conversions (the original converts the fully
updated `spawnPosition`).

## 3. Sweep results that did NOT land (all real `link_so` + `fdiff`)

`bool` -> `int` on each flag (current body raw 3588 / size 17172):

```text
resetStation, alienRace, hard   raw 3588  size 17172   (neutral, no win)
raid                            raw 5153  size 17168
introSystem                     raw 14827 size 17180
hasWanted                       raw 12503 size 17148
nameLeader                      raw 13548 size 17180
siegedStation                   raw 16334 size 17308
bigAlienWave                    raw 12426 size 17156
alienWave                       raw 12392 size 17148
pirateFortress                  raw 12322 size 17160
```

Declaration-order permutations of the three wave flags
(`bigAlienWave, alienWave, pirateFortress`): only the current `012` order is
raw 3588; `021` 6426, `102` 8796, `120` 10283 (size 17164), `201` 12082
(17180), `210` 11931 (17180).

Anchor spellings on the current body: `2*` (base) 3588; `4*` 3588; `<<1`
3588; `|` 3588; `+0` 3588; `add2` **3584** (landed); `3*` 3916.

### 3a. `char` instead of `bool`

Same sweep with `char`: `resetStation`/`hard` neutral (3584/17172); `raid`
3811; `bigAlienWave` 12425 (17156); `alienWave` 12392 (17148);
`pirateFortress` 12375 (17160); `hasWanted` 12503 (17148); `nameLeader`
13548 (17180).  No win.

### 3b. `extraAliens` `Vector sp` position sweep (on top of the landed anchor)

```text
sp copy position                         raw    size
before x update (after getPosition)      4444   17176
after x update (landed)                  3538   17172
after y update (wave-184 base)           3584   17172
after z update                           3622   17172
component-wise x/y/z after each update   4442   17164
component-wise x then y,z after z        4439   17168
sp.x after x, sp.y/sp.z after z          4439   17168
sp copy after x + sp.z refreshed         4449   17168
sp copy after x + sp.y,z refreshed       4439   17168
sp copy after x + sp.x,z refreshed       3631   17172
direct `(int) this->spawnPosition.{x,y,z}` 3626 17172
```

### 3c. Other measured non-wins (all size-exact unless noted)

```text
bigAlienWave == 1 -> bigAlienWave / != 0       3584
alienPercent shift -> multiply                 3584
extraAliens int -> unsigned                    3585
fortressGuns / stationPirates/blueprint type   3584
raid block: guns hoist / hard first / status hoist  size 17168 / 17160 / 17160
raid loot x/y/z order: zyx 3539; xzy 4470 (17168); yzx 4469 (17168)
raid loot setRotate/setSpeed/hp order: hp first 3546, speed first 3539
raid condition: level-first / hard*20 / hard?20:0 3584
alienGuns block decl order: k first 3584, base-before-v 3612
pirateFortress/stationPirates Globals::rnd hoist   size 17188
stationPirates piratesBase scalar hoists           3553
case2 asteroidPos declaration moved before coords   3607 (size 17172)
case2 asteroidPos before Route / before goods       3600 (17172)
case2 coords zero-init then assign                  10730 (17180)
switch(k) all 16 offset[2]-last orders              10898..11306 (17164..17188)
nameLeader: drop else, add unconditional return     11034 (17164)
```

The `switch (k)` convoy-offset block and the `stationPirates` escort loop
remain the two structural residuals (the original shares the third
`offset[2]` store across the four cases and hoists `pirateBase` into
`s16`/`s18`/`s24`); every source spelling tried either canonicalises back to
the current shape or is size-fatal.
