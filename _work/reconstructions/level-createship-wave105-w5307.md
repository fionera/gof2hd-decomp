# Level::createShip — wave 105 (w5307): waypoint declaration order + slot-map study

**Status:** source change landed (one line). Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). Fuzzy **66.4 -> 66.6**; every gate
count unchanged, `.ARM.extab` stays `0x26298` (record-length exact).

Gate before and after (full `check`):

```text
before: byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x1337c8  .ARM.exidx 0x6f70  .ARM.extab 0x26298
        lint CLEAN  drift 0  RATCHET PASS
after:  byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x1337c8  .ARM.exidx 0x6f70  .ARM.extab 0x26298
        lint CLEAN  drift 0  RATCHET PASS
```

## 1. The landed diff

```diff
-    int wz, wy, wx;
+    int wy, wx, wz;
```

The three locals are still assigned `wz = waypoint->x; wy = waypoint->y;
wx = waypoint->z;` in that order.  The declaration order only changes the
order of the three zero stores in the `waypoint == nullptr` arm:

```text
orig  else arm:  str r0,[sp,#48] ; str r0,[sp,#44] ; str r0,[sp,#40]
wave103 (wz,wy,wx): str r0,[sp,#44] ; str r0,[sp,#40] ; str r0,[sp,#48]
landed  (wy,wx,wz): str r0,[sp,#48] ; str r0,[sp,#44] ; str r0,[sp,#40]
```

The landed order matches the original's else-arm store order exactly.  The
then-arm mapping is still permuted (`x@48,y@40,z@44` vs the original
`x@44,y@48,z@40`), which is the remaining StackSlotColoring residual.

## 2. Slot map (decoded from the original at `0xc0214`)

```text
waypoint block:  wp->x (+292) -> [sp,#44]
                 wp->y (+296) -> [sp,#48]
                 wp->z (+300) -> [sp,#40]
jitters:         1st rnd -> [sp,#36]   2nd -> [sp,#32]   3rd -> [sp,#28]
spawn:           x = wp->x + 1st - 20000   (-> fp)
                 y = wp->y + 2nd - 20000   (-> r4)
                 z = wp->z + 3rd - 20000   (-> r8)
```

All three spawn values are `int` in the original; the three
`vcvt.f32.s32` are emitted after each branch's `operator new`
(`0xc040c..0xc0422`, `0xc06c6..0xc06d8`).  The Ghidra/REA decompilation
(`rea decompile ... --procedure _ZN5Level10createShipEiiiP8Waypointbb`)
confirms `iVar3 = local_54 + iVar3 + -20000` etc. with `local_54 =
*(wp+0x124)`.

## 3. The `::operator new` shape (measured, not landed)

The pass-10 shape (`switch (type)` + `::operator new(sizeof(T))` +
placement `new` + `try { setEmpData } catch (...) { __builtin_trap(); }`)
reproduces the **original prologue and slot map exactly** and scores:

| spawn | score | usize | extab |
|---|---:|---:|---:|
| pass-10 hack (`x=j2+j3, y=wy+j1, z=wz+wx`) | 74.4 | 2764 | 0x26290 |
| semantically correct (`x=wz+j1, y=wy+j2, z=wx+j3`) | 73.8 | 2764 | 0x26290 |

It is **not landed** because the assignment bans direct `operator new`
spelling, and adding two `::operator new` sites would raise
`goal_lint`'s `operator_call` count for `Level.cpp` from 1 to 3 (baseline
1, and the task forbids editing `tools/goal_lint_baseline.json`).  It also
changes `.ARM.extab` from `0x26298` to `0x26290`.

The reason this shape scores higher is the **slot map**: the hack's
`x = jitterZ + jitterX` / `y = wy + jitterY` / `z = wz + wx` expression
structure makes StackSlotColoring produce the original's exact
`wp->x@44, wp->y@48, wp->z@40, j1@36, j2@32, j3@28` map, while every
semantically-correct expression (`x=wz+j1`, `y=wy+j2`, `z=wx+j3`) produces
`wp->x@32, wp->y@48, wp->z@44, j1@28, j2@40, j3@36`.  A sweep of all 6
waypoint declaration orders x 6 assignment orders x 6 spawn statement
orders with correct expressions never produced the original map.

## 4. Plain-`new` sweep (the landed constraint)

With plain `new` (no `::operator new`, no `try`), the measured optimum is
66.6.  Sweeps this pass (official verifier, `score.py`):

- waypoint decl x assignment order (36 variants): best
  `int wy, wx, wz;` + `wz,wy,wx` = **66.6** (landed);
- spawn statement order x int/float type (48 variants on the landed decl):
  best `xyz` + `int x, float y, float z` = **66.6**;
- `switch (type)` 64.9 (usize 2768); `switch (shipClass)` 66.6;
  `int armor` inline 62.4; `empLevel` temp 66.6; `empPoints` if-form 63.6;
  `getLevel() <= 20` 66.5;
- plain `new` + `try` around `setEmpData` 62.9 (usize 2768, extab 0x262a8);
  try around `new Player` 62.0 (usize 2780); around `new Player` +
  `setEmpData` 62.2 (usize 2776);
- all-`int` correct spawn 62.8/62.9 (usize 2792); spawn before `new Player`
  58.9-63.7.

## 5. Remaining residual / next hypothesis

1. Prologue: original `str r1,[sp,#60]` (race), `mov fp,r3` (type),
   `mov r5,r2` (shipClass); ours `strd r1,r2,[sp,#56]` (race+shipClass),
   `mov sl,r3` (type).  The original keeps `shipClass` in a register and
   spills only `race`.
2. Then-arm slot mapping `x@48,y@40,z@44` vs `x@44,y@48,z@40`.
3. Spawn is scheduled after `setEmpData` in ours, before it in the original.

The next lever is a plain-`new` source shape that makes StackSlotColoring
choose the original map without the `::operator new`/`try` hack.  The
slot map is not reachable from the correct spawn expressions alone
(§3); it likely needs a different waypoint-block or jitter-declaration
shape that changes the live ranges without changing semantics.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5307
cmake --preset default
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch harnesses (this pass, `/dev/shm/w5307`): `score.py` (official
single-function score), `variant.py` (splice body + relink + score),
`sweep1..16.py`, `slotmap3.py` (waypoint/jitter slot-map decoder),
`lsda2.py` (LSDA decoder), `rea-createship.json` (REA decompilation).
