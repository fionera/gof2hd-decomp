# `Level::createShip` — wave 122 (w7207): capital-ship position vector

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base master `29cd275f6`. Worktree left clean.

**Status: landed.** `createShip` 73.8 → **73.9 %** (official scorer),
body stays 2764 B, insns 1007 / 1008, LSDA stays 328 B (original
336 B). The gate is unchanged: byte_exact / linked_exact **4506 / 4506**.

```
                                    before (HEAD)        after
createShip fuzzy (official)         73.8                 73.9
createShip size                     2764 B               2764 B
createShip insns (orig/ours)        1007 / 1008          1007 / 1008
createShip LSDA record              328 B                328 B
byte_exact / linked_exact           4506 / 4506          4506 / 4506
drift / lint                        0 / CLEAN            0 / CLEAN
imports                             207                  207
```

`check`: **RATCHET PASS** — `verify: byte 4506 linked 4506 stubs 0
missing 0 extra 0 avg 99.93`, `data: exact 843 of 843`, `sodiff:
imports 207`, lint CLEAN, `drift` 0 fields.

## 1. What changed

`src/game/world/Level.cpp`, `createShip()` capital branch only.  The
original converts the integer ship position to floats immediately
after the `PlayerFixedObject` allocation and keeps it in
`s20 / s18 / s16` for the constructor and every `BoundingAAB` call.
The old body passed the mixed `int x` / `float y,z` expressions
directly to each call, so clang re-materialised the conversions at
each use.  The new body materialises the float position once:

```diff
     } else if (shipClass == 1) {
         // capital ships: hull collision boxes per race
+        Vector pos = {(float) x, (float) y, (float) z};
         PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
-        new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        new (fixed) PlayerFixedObject(type, race, player, nullptr, pos.x, pos.y, pos.z);
         Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
...
-                    (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, ...);
+                    (*volumes)[0] = new BoundingAAB(pos.x, pos.y, pos.z, 0.0f, ...);
```

All 26 `BoundingAAB` calls in the capital branch use `pos.x / pos.y /
pos.z` now.  This is semantically identical (x is already `int`, y/z
are already `float`) and keeps the existing `Vector` include.

## 2. Why it scores better

The prologue is untouched (first 94 instructions still match the
original exactly).  The gain is inside the capital branch: the
`PlayerFixedObject` constructor now sees the position as a single
float triple, so clang schedules the `vcvt.f32.s32` pair before the
`operator new` call instead of after it.  The emitted capital-ctor
sequence becomes

```
vmov  s0, r4
mov.w r0, #444
vcvt.f32.s32 s20, s0
blx   operator new
...
vstr  s20, [sp, #4]
vstr  s18, [sp, #8]
vstr  s16, [sp, #12]
blx   PlayerFixedObject::PlayerFixedObject(...)
```

instead of the previous `new`-then-`vcvt` order.  That one-block
scheduling change aligns one more instruction with the original's
LCS (73.8 → 73.9) without changing size, LSDA, or any other function.

## 3. Remaining root cause (unchanged from wave 121)

The residual is the register-allocation cascade documented in
`level-createship-wave121-w7107.md` and `level-createship-ra-wave121-w7108.md`:

1. **Capital-ship `fixed` / `volumes` swap.**  Original:
   `ship = r9`, `fixed = r9`, `volumes = fp`.  Ours:
   `ship = fp`, `fixed = fp`, `volumes = r9`.  The cause is the mixed
   body's `int x`: the compiler sinks `x = wx + jx - 20000` past
   `setEmpData` (it is pure), so `fp` is free when `ship = nullptr`
   is materialised; `ship` takes `fp`, and the geometry null-constant
   is commoned into the same register.  The original computes
   `x,y,z` as **ints before** `setEmpData`, so `fp` is occupied by
   `x` at the branch, `ship` takes `r9`, and `fixed` overwrites it.
2. **Waypoint/jitter stack slots.**  Original `wx,wy,wz = 44,48,40`
   and `jx,jy,jz = 36,32,28`; ours `wx,wy,wz = 32,48,44` and
   `jx,jy,jz = 28,40,36`.  All six declaration/computation orders
   give the same slots; the difference follows from the live ranges
   of the all-int coordinate block.
3. **`BoundingAAB` argument setup.**  Original `sl/r8/r4` for
   `x/y/z`; ours `r6/r8/r3(sl)`.  This is a consequence of (1).
4. **One early spill.**  Original spills `this` after
   `add.w sl,r0,#40`; ours spills it one instruction earlier.
5. **EH shape.**  Original is real-`new` cleanup-only: 336 B LSDA,
   57 rows, `ttype 0xff`.  Ours is placement-`new` + `try/catch`:
   328 B, 54 rows, `ttype 0x00`.

### Variants measured this pass

All rows use the official one-function scorer and a full `link_so`.

| variant | % | size | insns | prologue |
|---|---:|---:|---:|---|
| HEAD (mixed, placement+try) | 73.8 | 2764 | 1007/1008 | matches |
| **capital `Vector pos` (this pass)** | **73.9** | 2764 | 1007/1008 | matches |
| capital `float bx,by,bz` | 73.9 | 2764 | 1007/1008 | matches |
| capital `Vector` after ctor / after Array | 73.8 | 2764 | 1007/1008 | matches |
| all-int coords after ctor, placement+try | 61.6 | 2752 | 1007/1004 | type `r9`, `this` spilled |
| all-int coords before ctor, real new | 64.2 | 2796 | 1007/1008 | type spilled |
| all-int coords before ctor, placement+try | 61.6 | 2752 | 1007/1004 | type `r9`, `this` spilled |
| mixed + real new | 64.3 | 2768 | 1007/996 | `strd r1,r2` |
| all-float x,y,z | 65.0 | 2748 | 1007/991 | type `sl` |
| `struct { int x,y,z; }` | 61.6 | 2752 | 1007/1004 | type `r9` |
| fighter branch float locals | 61.4 | 2764 | 1007/1008 | matches |
| floats before ship branch | 43.6 | 2752 | 1007/1003 | prologue changes |

The all-int form is the faithful shape (it is what the original
disassembly does) but it changes Greedy RA enough to spill `type` in
the prologue, which costs 10–12 points.  Real `new` reproduces the
336 B cleanup-only LSDA but also changes the prologue and grows
`.ARM.extab` by 8 B (0x26294 → 0x2629c), which would drop
`byte_exact` unless paired with the `Generator::createMission`
172 B LSDA fix from `5be8ac28b`.  The current placement+`try/catch`
body remains the only shape that keeps the prologue and the gate
green.

## 4. Reproduction

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
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```
