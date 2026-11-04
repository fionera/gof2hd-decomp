# Level::createShip — wave 103 (wf305): oracle spawn order + declaration order

**Status:** source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). The wave-102 extab-exact body
(plain `new`, if-chain, no `try`) is kept: `.ARM.extab` remains
record-length-exact at **336 B / 57 call-site rows**, and every gate
number is unchanged. The per-function fuzzy score improves
**66.0 -> 66.4**.

Gate before and after (full `check`):

```text
before: byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
        .ARM.extab 0x26298  lint CLEAN  drift 0  RATCHET PASS
after:  byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
        .ARM.extab 0x26298  lint CLEAN  drift 0  RATCHET PASS
```

## 1. The landed diff

```diff
-    int wx, wz, wy;
+    int wz, wy, wx;
...
     if (type == 0x33) {
         hitpoints = (int) ((float) hitpoints * 1.7f);
-    } else if (type == 0x31) {
-        hitpoints = (int) ((float) hitpoints * 17.0f);
     } else if (type == 0x2c) {
         hitpoints = (int) ((float) hitpoints * 2.25f);
+    } else if (type == 0x31) {
+        hitpoints = (int) ((float) hitpoints * 17.0f);
     }
...
     int x = wz + jitterY - 20000;
-    float z = (float) (wx + jitterX - 20000);
     float y = (float) (wy + jitterZ - 20000);
+    float z = (float) (wx + jitterX - 20000);
```

The changes are source-shape only:

1. **Waypoint locals declared in field order** `wz, wy, wx` (they are
   assigned `waypoint->x`, `->y`, `->z`).  The old `wx, wz, wy`
   declaration order scored 66.0; every other declaration order scores
   66.4.
2. **Spawn written in the oracle/binary order** `x, y, z`
   (oracle `game/Level.c` createShip computes `x`, `y`, `z` in that
   order after `Player::Player`).  The old body wrote `x, z, y`.
   `x` stays `int` and `y`/`z` stay `float`; the all-`int` form is
   semantically exact but scores 62.8 (see §3).
3. **Type if-chain written `0x33, 0x2c, 0x31`.**  Clang 7 reorders
   this chain to the original binary's compare order
   `cmp #0x33; cmp #0x31; cmp #0x2c`; the old `0x33, 0x31, 0x2c`
   source order compiled to `0x33, 0x2c, 0x31` (wrong compare order).
   Both score 66.4, so the landed order is the one that matches the
   original instruction stream.

## 2. Oracle evidence

`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Level.c`
(`Level::createShip @0x000cf83c`) gives the real body:

```c
iVar1 = Status::getCurrentCampaignMission(Globals::status);
/* waypoint -> local_58/local_54/iVar2 */
iVar3 = AERandom::nextInt(Globals::rnd, 40000);   // 1st
iVar4 = AERandom::nextInt(Globals::rnd, 40000);   // 2nd
iVar5 = AERandom::nextInt(Globals::rnd, 40000);   // 3rd
...
this_00 = operator_new(0x114);
Player::Player(this_00, iVar15, iVar6, 1, 1, 0);
iVar3 = local_58 + -20000 + iVar3;   // x = wp->x + 1st
iVar4 = local_54 + -20000 + iVar4;   // y = wp->y + 2nd
fVar20 = (float)(iVar2 + -20000 + iVar5); // z = wp->z + 3rd
Player::setEmpData(this_00, iVar7, iVar14);
this_01 = (PlayerFixedObject *)0x0;
if (param_2 == 0) { ... PlayerFighter ... }
if (param_2 != 1) goto done;         // else-if chain
... PlayerFixedObject + BoundingAAB switch(param_1) ...
(**(code **)(*(int *)this_01 + 0x14))(this_01, this); // setLevel
return this_01;
```

This confirms plain `new`, the `shipClass` if/else-if chain, the
`setEmpData` position, the `x,y,z` spawn order and the cleanup scopes.
The original LSDA (decoded from `.ARM.extab` at `0x1ab09c`) has
**57 rows, all `action = 0`, 315-byte call-site table, 336-byte
record**; ours is byte-for-byte structurally identical (same row
sequence, same landing-pad order, same 57 rows / 315-byte table /
336-byte record).  The entry lengths differ only by the code-size
delta of each region (e.g. original spawn+setEmpData row `0x5e`,
ours `0x64`), which is the remaining RA/scheduling residual.

## 3. Measured table (official verifier)

All rows are `_ZN5Level10createShipEiiiP8Waypointbb`; `pct` is
`verify.py`'s fuzzy score, `usize` the body size, `extab` the function's
`.ARM.extab` record length.

| source shape | pct | usize | extab | note |
|---|---:|---:|---:|---|
| wave-102 baseline (`wx,wz,wy`; spawn `x,z,y`; if-chain `33,31,2c`) | 66.0 | 2764 | 336 | committed before this pass |
| **landed** (`wz,wy,wx`; spawn `x,y,z`; if-chain `33,2c,31`) | **66.4** | **2764** | **336** | full gate green |
| same, if-chain `33,31,2c` (oracle order) | 66.4 | 2764 | 336 | compare order becomes `33,2c,31` |
| faithful all-`int` spawn `x,y,z` | 62.8 | 2788 | 336 | correct slot map, wrong prologue/RA |
| all-`int`, spawn before `new Player` | 63.9 | 2796 | 336 | spawn stays before `setEmpData` |
| all-`int`, `switch(type)` | 63.1 | 2788 | 336 | original compare order, worse RA |
| mixed, `switch(type)` | 64.3 | 2768 | 336 | |
| mixed, plain `new`, `try`/`catch` | 62.5 | 2768 | 352 | adds catch handler |
| pass-10 placement `new` + `try` + `switch` (wrong spawn) | 74.4 | 2764 | 328 | not extab-exact |

Every `int`/`float` x `x,y,z` permutation was measured (48 variants):
the best is `int x`, `float y`, `float z` in `x,y,z` or `x,z,y`
order; all score 66.4.  All-`float` is 38-39%.  The `armor`,
`empPoints`, `empTime`, `ship` declaration and BoundingAAB case-order
sweeps found no shape above 66.4 that keeps the 336-byte LSDA.

## 4. What is proven vs still hypothesis

**Proven by the binary / LSDA:**

- plain `new` (no `try`/catch): 57 cleanup-only rows, 336-byte record;
- `shipClass == 0` / `shipClass == 1` else-if chain;
- spawn is all-`int` in the original (`add`/`subs` on r-registers,
  `vcvt.f32.s32` emitted after each branch's `operator new`);
- original spawn order is `x = wp->x + 1st`, `y = wp->y + 2nd`,
  `z = wp->z + 3rd`;
- original type compare order is `0x33, 0x31, 0x2c`.

**Still hypothesis / residual:**

- The original slot map is `wz@44, wy@48, wx@40`; the landed mixed
  form keeps `wz@48, wy@40, wx@44`.  The faithful all-`int` form gets
  the original slot map and a near-identical literal-pool order but
  loses the prologue (`type` is spilled instead of kept in `fp`, and
  the first jitter occupies `fp`).
- Original downstream colouring is `volumes=fp, fixed=r9, z=r4,
  x=sl, y=r8, type=r4, race=r6`; ours is `volumes=r9, fixed=fp,
  z=sl, x=r6, y=r8, type=r6, race=r4`.  This is the same
  Greedy-RA compact-region decision documented in
  `level-createship-pass10-w5006.md`.
- The spawn is still scheduled *after* `setEmpData` in our build,
  while the original computes it before.  Moving it before
  `new Player` forces the right position but changes the prologue
  (63.9%).  A source shape that keeps the mixed-type prologue and
  places the spawn before `setEmpData` without a scheduling barrier
  was not found.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf305
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch harnesses (this pass, under `/dev/shm/wf305`): `score.py`
(single-function official score), `pools.py` (literal-pool sequence
diff), `lsda.py` (`.ARM.extab` LSDA decoder), `sweep_spawn2.py`,
`sweep_wp_slots.py`, `sweep_armor.py`, `sweep_emp.py`,
`sweep_ifchain2.py`, `sweep_cases.py`.
