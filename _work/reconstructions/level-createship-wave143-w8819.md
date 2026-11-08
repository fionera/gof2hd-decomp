# `Level::createShip` — wave 143 (w8819): the missing 16 B is the **double
# `getLevel()` clamp** (both clamps are `getLevel() > 20 ? 20 : getLevel()`),
# witnessed as 4 `blx Status::getLevel` sites in the original.  Landed the
# faithful statement sequence (double clamp + `x,y,z` waypoint reads +
# `jitterX,jitterY,jitterZ` declaration order + no bogus `status == nullptr`
# guard) at the exact 2764 B / 336 B 57-row LSDA.  The raw same-address diff
# and the official fuzzy both regress (see §3) because the register allocator
# moves `Globals::status` from r4 to r6 once the guard is gone; this is a
# FAITHFULNESS change, not a metric win.

Status: **source change landed**.  Gate on the changed tree: `byte_exact
4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data 843/843, imports
207, lint CLEAN, drift 0.  Worktree `wt/w8819`, base master `bf083ab41`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903, `-Oz`, image base 0x10000).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`, exact 336 B / 57-row `ttype 0xff` LSDA.

## 0. The 16 B statement is the double `getLevel()` clamp

The original has **four** `blx Status::getLevel` call sites:

```
c0284 ldr r0,[r4,#0] / c0286 blx getLevel      ; level clamp, first eval
c028a cmp r0,#20
c028e ble c0294
c0290 movs r5,#20
c0292 b c029c
c0294 ldr r0,[r4,#0] / c0296 blx getLevel      ; level clamp, second eval
c029a mov r5,r0
c029c ldr r0,[r4,#0] / c029e blx gameWon
...
c0314 ldr r0,[r4,#0] / c0316 blx getLevel      ; empPoints clamp, first eval
c031a cmp r0,#20
c031c ble c0322
c031e movs r0,#20
c0320 b c0328
c0322 ldr r0,[r4,#0] / c0324 blx getLevel      ; empPoints clamp, second eval
c0328 add.w r0,r0,r0,lsl #2
```

The master source (`int level = getLevel(); if (level > 20) level = 20;`)
emits **two** calls (`it ge; movge`), so it is 2 × (`ldr`+`blx`) = **16 B**
short.  The exact source producer is the ternary idiom already used elsewhere
in this TU (e.g. `Level.cpp:1867`, `4813`):

```cpp
int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
...
int empPoints = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
```

`reassign`/`if-else` spellings produce the identical IR (verified: all three
give the same 2792 B body).

## 1. Statement order reconstructed from Thumb

```
mission   = Globals::status->getCurrentCampaignMission()   ; r8
wx,wy,wz  = waypoint ? waypoint->x/y/z : 0                 ; [sp,#44],[sp,#48],[sp,#40]
jx,jy,jz  = Globals::rnd->nextInt(40000) x3                ; [sp,#36],[sp,#32],[sp,#28]
level     = getLevel() > 20 ? 20 : getLevel()              ; r5
hitpoints = (gameWon()?180:mission*4) + level*14 + 20      ; r5
switch(type) 0x33(*1.7) 0x31(*17.0) 0x2c(*2.25)
if (mission in {49,50,51,52,56}) hitpoints = 270
empPoints = getLevel() > 20 ? 20 : getLevel(); empPoints = empPoints*5+40  ; sl
if (shipClass==1) { hitpoints *= type==14?25:5; empPoints*=3; empTime=45000; }
else empTime=15000
hitpoints = (int)(hitpoints + (options[0x2c]-0.5f)*hitpoints)
if (mission==0x9a) hitpoints <<= inAlienOrbit() & (race==9)
armor = hardCoreMode()?650:1000
player = new Player(armor, hitpoints, 1, 1, 0)
x = wx+jx-20000 ; y = wy+jy-20000 ; z = wz+jz-20000       ; ints, fp/r4/r8
player->setEmpData(empPoints, empTime)
ship = nullptr ; if (shipClass==0) {...} else if (shipClass==1) {...}
ship->setLevel(this) ; return ship
```

The jitter/waypoint pairing is **x+jitterX, y+jitterY, z+jitterZ** with the
waypoint reads in **x, y, z** order and the jitter declarations in
**X, Y, Z** order (confirmed at `c023c/c0242/c0246` and `c03c8..c03e4`).
The original has **no** `if (Globals::status == nullptr)` guard: the first
instruction after `setEmpData` is `mov.w r9,#0; cmp r6,#0` (`ship = nullptr;
if (shipClass == 0)`).

## 2. What landed

`src/game/world/Level.cpp`:
* waypoint reads reordered `wz,wy,wx` -> `wx,wy,wz`;
* jitter declarations reordered `X,Z,Y` -> `X,Y,Z`;
* both clamps rewritten to the ternary `getLevel() > 20 ? 20 : getLevel()`;
* the unfaithful `if (Globals::status == nullptr) return nullptr;` guard removed.

Body stays **2764 B**, `.ARM.extab` stays **156,312 B** (exact 336 B / 57-row
`ttype 0xff` record).  Full `check` is green.

## 3. Metric trade-off (FAITHFULNESS change, not a metric win)

| variant | raw diff | fuzzy | size |
|---|---:|---:|---:|
| master (single clamp, guard, `z,y,x`) | **1411** | **70.6 %** | 2764 |
| landed: double clamp, no guard, `x,y,z`+`X,Y,Z` | 1978 | 66.6 % | 2764 |
| double clamp, no guard, `z,y,x`+`X,Z,Y` | 2055 | — | 2764 |
| double clamp + guard (RA preserved) | 2443 | 68.7 % | 2780 |
| double clamp + int coords before `Player` (full faithful) | 2313 | 61.4 % | 2792 |

The regression is pure register allocation: the removed guard was the only
post-`setEmpData` use of `Globals::status`, so it pinned `status` in **r4**
(matching the original) and let `shipClass` stay in **r5/sl**.  Without it the
greedy allocator moves `status` to **r6**, `rnd` to r4 and `nextInt` to r5,
then keeps `jitter1` in `sl` (instead of `shipClass`) and spills `shipClass`.
Every downstream register (x/y/z, player, fixed, volumes) shifts with it.
The original's r4/r5/r6/sl/fp map is *not* reachable from any source shape
tried so far; the binary has no post-`setEmpData` use of `Globals::status`
(r4 dies at `c0398`), so the coordinator's "keeps Globals live across
setEmpData" premise does not hold for the original asm.

## 4. Full faithful body is 28 B too large

Making the coordinates `int` and declaring them before `new Player` (so they
are computed before `setEmpData`, as in the original) gives the exact original
pre-call layout (470 B vs 474 B) but the body grows to **2792 B**: the two
per-branch `vcvt.f32.s32` conversion sites (capital `c040c..c0422`, fighter
`c06c0..c06d8`) cost 24 B more than the single conversion the compiler emits
when the coords are floats, and the shifted RA adds the rest.  Landing it
requires another 28 B of RA savings that no source shape found this pass.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8819/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch (`/dev/shm/w8819/`): `fdiff.py`, `measure.py` (size/insn/raw-diff),
`gen*.py` variant generators, `align.py` / `cumdelta.py` (structural diff).

## 6. Open items

1. The original's r4=`status`, r5=`shipClass`, r6=`nextInt`, sl=`shipClass`
   then `empPoints`, fp=`type` then `hitpoints` then `x` map has no source
   producer yet.  The guard is what pins it in master, but the original has
   no guard.
2. The full faithful body (int coords before `Player`) is 2792 B; 28 B of RA
   savings are still needed to land it at 2764.
