# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13415

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13415`, base master `96f53fadf` (raw **3,003**,
aligned **1,116**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Lens for this pass: case-2 body, raid block, alien-turret loop, literal-pool
ordering (the other workers own the case-1/2 rotations / tails).

## 1. Result — landed

```text
                                     size            raw    aligned  insn
base 96f53fadf                       17172/17172     3003   1116     6210/6197
landed                               17172/17172     3001   1112     6210/6197
```

Raw **3,003 -> 3,001** (-2), aligned **1,116 -> 1,112** (-4), size exact
17,172 B. `.text` differing bytes 72,944 -> 72,942 (-2). `check` ->
**RATCHET PASS** (byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370).

## 2. The change: `Vector asteroidPos` declaration moved after `ArraySetLength`

`case 2` declared

```cpp
Vector asteroidPos = {0.0f, 0.0f, 0.0f};
int convoyEnd = escorts + 2;
this->enemies = new Array<KIPlayer *>();
int total = convoyEnd + goods;
ArraySetLength(total, *this->enemies);
```

The original zero-initialises `asteroidPos` at `0xb16d4` (the very first
instruction of the case-2 body: `movs r4,#0; strd r4,r4,[sp,#272];
str r4,[sp,#280]`), i.e. the *declaration order* is not the lever here — the
spill-slot placement is. Moving the declaration **after** `ArraySetLength`
lets clang place the `sp#272` zero-init without the extra hoisted `movs`
and removes two bytes of the `0x1700` block (235 -> 233). It is a pure
declaration move: `asteroidPos` is only read in the second (escort) loop.

## 3. Angles measured and rejected (all size 17,172 unless noted)

- case-183 switch source-order matrix (8 orders): base order `0,1,default,2`
  and `0,default,1,2` both 3003; `0,1,2,default` 3004; the rest 5267+.
  The block layout is always `case0, case2, case1, default` (the compare
  tree is `cmp #2` first), so the literal pool is always
  `-3000,20000,-50000,25000,-36000,-7000,10000,-30000,...` vs the original
  `-3000,20000,-50000,-7000,10000,-30000,25000,-36000,...`.
  `switch (i)` / `case 3` explicit (tbb) do not fix it (`case 3` gives the
  original pool order but shifts the pool +4 and costs 5276).
- case-2 `convoyEnd` variants (`const`, `2+escorts`, `total` before
  `new Array`, `goods + convoyEnd`): all 3003.
- case-2 first-loop statement order `setAlwaysEnemy, setRoute, reachWaypoint`
  (the actual original emission order: `0xb1848` setAlwaysEnemy,
  `0xb1862` setRoute, `0xb1876` reachWaypoint): 3032 — the w13399 note's
  `P W R E` claim is wrong, but the register rotation dominates.
- alien-turret loop: loading `gun` **after** the `createStaticObject` store
  (the original form, `0xb32ae` store then `0xb32bc` load) is semantically
  the original but 3004 (+1); the `(i<0)+(i<0)` size lever is load-bearing.
- fortress loop: same `gun`-after-store form is 3010 (+7).
- `bigShipsCount`/`jumperCount` pointers replaced by direct `field_168`/
  `field_164` access: size-fatal 17,156.
