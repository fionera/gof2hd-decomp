# `Level::createShip` — wave 124 (w7411): faithful jitter pairing + waypoint/coord declaration order

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `6f8ee2320` (wave-123 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7411`.

**Status: source change landed.** The jitter pairing is now the original's
(`x = wx + 1st`, `y = wy + 2nd`, `z = wz + 3rd`); the previous body paired
`y` with the 3rd `nextInt` and `z` with the 2nd.  Reordering the waypoint
and coordinate declarations to `z, y, x` (semantically neutral) recovers
the score lost by the pairing fix and adds a little: **70.3 → 70.5 %**,
body stays **2764 B**, LSDA stays the exact **336 B / 57 rows / ttype 0xff**
cleanup-only record.  Full gate stays green.

## Result

```
                                   original        HEAD (6f8ee2320)     landed
createShip fuzzy (official)        —               70.3                 70.5
createShip size                    2764 B          2764 B               2764 B
createShip insns (orig/ours)       1007            1007 / 1005          1007 / 1006
createShip LSDA record             336 B           336 B                336 B
createShip LSDA rows               57              57                   57
createShip LSDA ttype              0xff            0xff                 0xff
.text differing bytes              0               87,014               86,964
.ARM.exidx differing bytes         0               0                    0
.ARM.extab size/base               0x26298         0x26298              0x26298
.ARM.extab differing bytes         0               1,446                1,446
.rel.plt differing bytes           0               4                    4
.note.gnu.build-id differing       0               20                   20
byte_exact / linked_exact          —               4506 / 4506          4506 / 4506
avg_match                          —               99.94                99.94
drift / lint                       —               0 / CLEAN            0 / CLEAN
imports                            —               207                  207
```

`check`: **RATCHET PASS** — `verify: byte 4506 linked 4506 stubs 0 missing 0
extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: imports 207`, lint
`CLEAN`, `drift` 0 fields.  `.rodata` / `.data` / `.bss` / `.got` /
`.rel.dyn` / `.dynsym` / `.dynstr` / `.hash` stay byte-identical; all 27
section headers stay size- and base-exact.

## 1. The bug: jitter pairing was swapped

Ground truth (Thumb disasm, `0xc023c`..`0xc028c` and `0xc03c8`..`0xc03e4`):

```
c026e  str r0,[sp,#36]   ; jitterX = 1st nextInt(40000)
c0278  str r0,[sp,#32]   ; jitterY = 2nd nextInt(40000)
c028c  str r5,[sp,#28]   ; jitterZ = 3rd nextInt(40000)
...
c03c8  ldr r2,[sp,#48]   ; wy
c03ca  ldr r3,[sp,#32]   ; jitterY (2nd)
c03cc  ldr r0,[sp,#44]   ; wx
c03ce  ldr r1,[sp,#36]   ; jitterX (1st)
c03d0  add r2,r3         ; wy + jitterY
c03d2  ldr r3,[sp,#40]   ; wz
c03d4  ldr r4,[sp,#28]   ; jitterZ (3rd)
c03d6  add r0,r1         ; wx + jitterX
c03dc  sub.w fp, r0, r1  ; x = wx + jitterX - 20000
c03e2  subs  r4, r2, r1  ; y = wy + jitterY - 20000
c03e4  sub.w r8, r3, r1  ; z = wz + jitterZ - 20000
```

The wave-123 body declared `jitterX, jitterZ, jitterY` (so `jitterZ` was the
2nd call and `jitterY` the 3rd) and then used `y = wy + jitterY`,
`z = wz + jitterZ` — i.e. `y` got the 3rd draw and `z` the 2nd.  The new
body declares `jitterX, jitterY, jitterZ` in call order and uses each with
its own axis, which is the original's semantics.

## 2. What changed (Level.cpp, `createShip` only)

```diff
     int mission = Globals::status->getCurrentCampaignMission();
-    int wx = waypoint != nullptr ? waypoint->x : 0;
-    int wy = waypoint != nullptr ? waypoint->y : 0;
     int wz = waypoint != nullptr ? waypoint->z : 0;
+    int wy = waypoint != nullptr ? waypoint->y : 0;
+    int wx = waypoint != nullptr ? waypoint->x : 0;
     int jitterX = Globals::rnd->nextInt(40000);
-    int jitterZ = Globals::rnd->nextInt(40000);
     int jitterY = Globals::rnd->nextInt(40000);
+    int jitterZ = Globals::rnd->nextInt(40000);
...
-    int x = wx + jitterX - 20000;
-    float y = (float) (wy + jitterY - 20000);
     float z = (float) (wz + jitterZ - 20000);
+    float y = (float) (wy + jitterY - 20000);
+    int x = wx + jitterX - 20000;
```

The declaration reorder is semantically neutral (the waypoint is not
modified by the reads).  It changes the waypoint/jitter spill-slot
colouring: the landed body puts `wz/wy/wx` at `sp48/sp44/sp28` and
`jitterX/jitterY/jitterZ` at `sp24/sp40/sp36`, versus the original's
`sp40/sp48/sp44` and `sp36/sp32/sp28`.  The `z,y,x` coordinate declaration
order additionally lines up the `vcvt`/`vmov` block one instruction
further into the original's sequence than the previous `x,y,z` order.

## 3. Variants measured this pass (official one-function scorer)

All rows are real-`new` (336 B LSDA, `ttype 0xff`) unless noted.

| variant | % | size | insns | struc |
|---|---:|---:|---:|---:|
| HEAD (wave 123) | 70.3 | 2764 | 1005 | 81.8 |
| **landed: wp `z,y,x` + jitter `X,Y,Z` + coord `z,y,x`** | **70.5** | 2764 | 1006 | 82.5 |
| wp `x,z,y` + faithful pairing | 70.4 | 2764 | 1005 | 81.9 |
| wp `z,x,y` + faithful pairing | 70.4 | 2764 | 1005 | 82.4 |
| wp `y,z,x` + faithful pairing | 70.5 | 2764 | 1005 | 82.4 |
| wp `x,y,z` + faithful pairing (natural) | 70.0 | 2764 | 1006 | 82.2 |
| wp `x,y,z` + old (swapped) pairing | 70.3 | 2764 | 1005 | 81.8 |
| wp `z,y,x` + old (swapped) pairing | 70.3 | 2764 | 1006 | 82.3 |
| landed + `float px,py,pz` instead of `Vector pos` | 70.5 | 2764 | 1006 | 82.5 |
| landed + `armor` if-form | 70.5 | 2764 | 1006 | 82.5 |
| landed + `switch (mission)` bitmask form | 70.5 | 2764 | 1006 | 82.5 |
| landed + `switch (type)` case order `0x33,0x31,0x2c` | 70.5 | 2764 | 1006 | 82.5 |
| landed + any combination of the four above | 70.5 | 2764 | 1006 | 82.5 |
| landed + `Vector pos` after `volumes` | 70.2 | 2764 | 1006 | 82.8 |
| landed + `volumes` allocated before `fixed` | 61.4 | 2760 | 994 | 85.7 |
| landed + `Vector pos` before `setEmpData` | 57.5 | 2776 | 1004 | 78.5 |
| landed + no `Globals::status == nullptr` guard | 63.6 | 2748 | 988 | 86.5 |
| landed + two-call `getLevel()>20?20:getLevel()` clamps | 68.6 | 2780 | 1012 | 80.6 |
| landed + both (no guard + faithful clamps) | 67.0 | 2764 | 994 | 90.0 |
| landed + all-int coordinates | 61.7 | 2772 | 998 | 86.7 |
| landed + `empTime` ternary | 61.5 | 2784 | 996 | 84.6 |
| landed + `hitpoints` if-form | 70.3 | 2764 | 1006 | 82.5 |
| landed + `switch (race)` -> if/else chain | 63.8 | 2764 | 1005 | 81.9 |

The `wp`/`coord` declaration-order grid (6×6, faithful pairing) peaks at
**70.5** for `wp=zyx` with `co ∈ {zyx, zxy, xzy}` and for `wp=yzx` with the
same three; every other pair is 70.0–70.4.  The jitter declaration order
(`X,Y,Z` vs `X,Z,Y` vs any permutation) makes no difference once the
pairing is faithful.

## 4. Remaining root cause (unchanged RA cascade)

The landed body is still the real-`new` shape that reproduces the 336 B
cleanup-only LSDA, and it still cannot reproduce the original's prologue:

```
orig : str r1,[sp,#60]   ; race spilled
       mov r9,r0         ; this
       ldr r0,[pc,#572]
       mov fp,r3         ; type -> fp
       mov r5,r2         ; shipClass -> r5
       add r0,pc
       ldr r4,[r0,#0]    ; status
...
       mov sl,r5         ; shipClass -> sl

ours : str r2,[sp,#60]   ; shipClass spilled
       mov r9,r0
       str r1,[sp,#52]   ; race spilled
       mov sl,r3         ; type -> sl
       ldr r0,[pc,#560]
       add r0,pc
       ldr r4,[r0,#0]
```

The four live cleanup pointers (`r5` Player/BoundingAAB, `r9` Fixed,
`fp` volumes, `sl` Fighter) change Greedy RA's first decision: the original
reuses the early `this`/`type`/`shipClass` registers for the later cleanup
pointers (`r9` this -> fixed, `fp` type -> x -> volumes, `sl` shipClass ->
empPoints -> fighter), whereas the landed body keeps `type` in `sl`, spills
`shipClass`, and gives `ship`/`fixed` `fp` and `volumes` `r9`.  Downstream
that swaps `fixed`/`volumes` and re-seats the `BoundingAAB` argument
registers (`orig sl/r8/r4/r6` for `x/y/z/w`; ours `r6/r8/sl/r0`).

The placement-`new` + `try/catch` body (commit `e2fa39a15`, 74.4 %) is the
only shape that matches the original prologue (first 94 instructions), but
it carries a 328 B type-table LSDA (`ttype 0x00`) and leaves `.ARM.extab`
8 B short, so it cannot be used under the "exact 336 B record" constraint
without a compensating change in another TU.

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
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```
