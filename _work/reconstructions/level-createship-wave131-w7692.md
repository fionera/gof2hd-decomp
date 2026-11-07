# `Level::createShip` — wave 131 (w7692): jitter order X,Z,Y; the r6/r8 exception-pointer mechanism

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `7c6933c2d` (wave-130 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7692`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: one landed source change.**  The three independent
`Globals::rnd->nextInt(40000)` jitter initialisers are emitted in **X, Z, Y**
order instead of X, Y, Z.  The calls are independent and the RNG stream is
identical, so this is semantically invisible; it moves the verifier fuzzy score
**70.5415 -> 70.5765** (2764 B both sides, 1007/1005 insns).  No gate count
changes.

```
before: createShip 70.5415  byte 4506 linked 4506 stubs 0 missing 0 extra 0
after : createShip 70.5765  byte 4506 linked 4506 stubs 0 missing 0 extra 0
data 843/843, imports 207, .text diff 85,379, lint CLEAN, drift 0,
RATCHET PASS both sides.
```

## 1. New finding: why the exception pointer is r6, and how it becomes r8

The wave-130 note attributed the r6 exception pointer to the lpad block order.
This pass measured the actual pre-RA live-in sets and found a simpler cause:

* The first cleanup lpad is the `new Player` lpad (`bb.25` after Greedy,
  `%ir-block.121`).  Its live-ins are `$r0, $r1, $r5` — **r6 is free**, so
  Greedy RA gives the exception PHI `%129` (`[bb.25, bb.106]`) register r6.
* In the original, **shipClass is live in r6 at that point** (`mov r6,sl` at
  `0xc02b4`, `cmp r6,#0/#1` at `0xc03f6/0xc03fc`), so the exception pointer
  must take r8.  The r8 choice is therefore a consequence of the *shipClass
  allocation*, not of a late lpad block.

Confirmed by a controlled experiment: moving the coordinate declarations
before `new Player` in the source leaves the first lpad at IR block index 14
unchanged, but the allocator then puts the `Player *` in **r6**; the landing
pads switch from `mov r6,r0` to `mov r8,r0` (score 62.84, size 2768, because
the coordinate liveness cascades through the rest of the allocation).  So
"occupy r6 at the lpad" is the real lever, and either shipClass (original) or
player (that variant) does it.

## 2. New finding: jitter declaration order X,Z,Y is a strict win

The three jitter calls are independent.  A full 6x6 declaration-order sweep
(waypoint `w*` order x jitter `jitter*` order) gives:

| waypoint | best jitter order | exact % |
|---|---|---:|
| z,y,x | **X,Z,Y / Z,X,Y** | **70.5765** |
| z,y,x | X,Y,Z / Y,X,Z | 70.5415 |
| y,z,x | X,Y,Z / Y,X,Z | 70.4771 |
| x,z,y | X,Y,Z / Y,X,Z | 70.3783 |
| z,x,y | X,Y,Z / Y,X,Z | 70.3783 |
| x,y,z | X,Z,Y / Z,X,Y | 70.2783 |
| any | Y,Z,X or Z,Y,X | 62.0–62.6 |

The landed order is `jitterX, jitterZ, jitterY` (X,Z,Y).  The gain is a
colouring/scheduling artefact (the jitter stores land at `[sp,#24]/[sp,#40]/
[sp,#36]` instead of `[sp,#24]/[sp,#40]/[sp,#36]` with a slightly different
call/store interleave), but it is reproducible and does not change any gate
count.  Coordinate declaration order has no effect once the jitter order is
fixed (all six orders score 70.5765).

## 3. Faithful variants re-measured (all still worse)

| variant | exact % | size | note |
|---|---:|---:|---|
| **HEAD: mixed coords + guard + jitter X,Z,Y** | **70.5765** | 2764 | landed |
| no `Vector pos` (direct `(float)x,y,z`) | 70.2783 | 2764 | -0.30 |
| `MIN(getLevel(),20)` double call (both clamps) | 68.6819 | 2780 | +16 B |
| double call, level only | 68.6849 | 2772 | |
| double call, empPoints only | 68.3871 | 2772 | |
| all-int coords + guard | 61.4693 | 2772 | |
| all-int coords, no guard | 61.4770 | 2772 | |
| coords before `new Player`, mixed, guard | 62.8743 | 2768 | r8 exception pointer |
| coords before `new Player`, mixed, no guard | 58.3459 | 2752 | |
| early `return` in both ship branches | 59.9702 | 2748 | |
| extra `{ }` around the body | 70.5765 | 2764 | folded away |
| `bool isCapital = shipClass == 1` early | 62.0551 | 2740 | |
| `empTime` computed early | 60.3698 | 2780 | |
| `Vector pos` at function scope | 56.9860 | 2788 | |
| `float x,y,z` all-float | 57.2849 | 2776 | |

## 4. Root cause and remaining lever

The whole function-wide register map follows one early decision: **`type` is
assigned to `sl` in our body, but to `fp` in the original.**

* ours: `type=sl`, `empPoints=fp`, `shipClass=[sp,#60]`, `armor=sl`,
  `fighter=fp`, `Array=fp` (fighter and Array share one cleanup pad), zero
  rematerialised per `BoundingAAB` call.
* original: `type=fp`, `empPoints=sl`, `shipClass=r6`, `armor=fp`,
  `fighter=sl`, `Array=fp` (four separate cleanup pads), zero kept in r6
  (`strd r6,r0,[sp]`, 12 B smaller).

If `type` could be forced into `fp` (e.g. by occupying `sl` with another
callee-saved value at entry), `empPoints` would reuse `sl`, `shipClass` would
move to `r6`, the exception pointer would take `r8`, the fighter would take
`sl`, and the zero register would become available — i.e. the whole cascade
would match.  Every source spelling tried this pass (parameter copies,
`bool isCapital`, early `empTime`, `MIN`, declaration reorder) either
coalesced away or changed the allocation in the wrong direction.  The next
pass should attack the entry allocation directly, e.g. by finding a Clang 7
source construct that gives `type` a register hint of `fp` (or that makes
`shipClass` the first callee-saved value allocated).

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

MIR live-ins of the first lpad (`-mllvm -print-machineinstrs`):

```
bb.25 (%ir-block.121, landing-pad):
  liveins: $r0, $r1, $r5          ; r6 free -> exception PHI takes r6
```

Scratch helpers (not committed): `/dev/shm/w7692/{score.py,apply.py,run.sh,
sweep*.py,base.txt,v_best.txt}`.
