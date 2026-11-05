# Level::createShip — wave 119 (w6781): faithful waypoint/spawn mapping (+0.3 %)

**Status:** source change landed, 2026-11-05. Worktree `wt/w6781`, branch
`wave119-w6781`, base `aca284d5c`. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`).

## Result

```
                                   before (HEAD)        after (landed)
createShip fuzzy (raw)             67.100000            67.400000
createShip size                    2764 B               2764 B
createShip LSDA record             336 B / 57 rows      336 B / 57 rows  (exact)
Generator::createMission LSDA      168 B                168 B            (-4)
.ARM.extab size / addr             0x26294              0x26294          (unchanged)
.ARM.exidx                         0x6f70  (exact)      0x6f70  (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
imports / data / lint / drift      207 / 843 / CLEAN / 0
```

`check` (final, after the landed change): **RATCHET PASS** —
`verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`,
`data: exact 843 of 843`, `sodiff: 0/0 imports 207`, lint CLEAN, drift 0.
No section size, base, import, data or symbol-parity movement; the only
section bytes that change are `.text` (and the `.text` literal pool word at
`+0xa8c`). `.ARM.extab` stays `0x26294`: createShip's record is exact, the
only remaining record delta is `Generator::createMission` -4 (different TU,
line 95 of `tools/source_order.txt`).

## Landed diff (8 insertions / 8 deletions, ordinary C++)

```diff
-    int wx, wz, wy;
+    int wy, wz, wx;
     if (waypoint != nullptr) {
-        wz = waypoint->x;
+        wx = waypoint->x;
         wy = waypoint->y;
-        wx = waypoint->z;
+        wz = waypoint->z;
     } else {
-        wz = 0;
-        wy = 0;
         wx = 0;
+        wy = 0;
+        wz = 0;
     }
 ...
-    int x = jitterZ + jitterX - 20000;
-    float y = (float) (wy + jitterY - 20000);
-    float z = (float) (wz + wx - 20000);
+    int x = wx + jitterY - 20000;
+    float y = (float) (wy + jitterZ - 20000);
+    float z = (float) (wz + jitterX - 20000);
```

The spawn mapping is the one the original actually computes (Thumb
`0xc03c8`-`0xc03e4`): `x = wpX + jitterCall1`, `y = wpY + jitterCall2`,
`z = wpZ + jitterCall3`, all `- 20000`; the names `jitterY/Z/X` in the
source are just the call order 1/2/3. The declaration order `wy, wz, wx`
is not semantic — it only steers the greedy allocator's spill choice for
the three waypoint words and is worth +0.2 % on its own
(`wx, wy, wz` = 67.2, `wy, wz, wx` = 67.4, `wz, ...` = 66.8/67.0).

## Measurement table (official scorer, full gold link, 2764 B)

| variant | raw % | note |
|---|---:|---|
| **HEAD: tuned spawn, `int wx, wz, wy`** | **67.100000** | before |
| **landed: faithful spawn, `int wy, wz, wx`** | **67.400000** | best |
| faithful spawn, `int wx, wy, wz` | 67.200000 | decl order |
| faithful spawn, `int wx, wz, wy` | 67.200000 | decl order |
| faithful spawn, `int wy, wx, wz` | 67.200000 | decl order |
| faithful spawn, `int wz, wy, wx` | 67.000000 | decl order |
| faithful spawn, `int wz, wx, wy` | 66.800000 | decl order |
| spawn after `setEmpData` (source order) | 67.400000 | tie, compiler sinks either way |
| spawn before `new Player` | 62.200000 | 2788 B |
| spawn decl order y,z,x / y,x,z | 67.400000 | tie |
| spawn decl order z,y,x / z,x,y | 67.200000 | |
| `int x, yi, zi; float y=(float)yi, z=(float)zi` (`iif`) | 67.400000 | tie |
| all-int spawn (`iii`) | 63.100000 | 2792 B, +7 pool words |
| `Status *status = Globals::status;` local | 61.700000 | 2768 B |
| `AbyssEngine::AERandom *rnd = Globals::rnd;` local | 63.800000 | 2768 B |
| `armor` before the `switch (type)` | 63.100000 | 2784 B |
| `int sc = shipClass;` copy | 67.400000 | tie, coalesced |
| `bool isCapital = shipClass == 1;` | 67.400000 | tie, coalesced |
| `int empTime = 15000; if (...) empTime = 45000;` | 67.400000 | tie |
| `int hitpoints;` declared before `level` | 67.400000 | tie |
| LOD `if (geo == nullptr) geo = geometry;` if/else | 65.100000 | 2768 B |
| LOD if/else, geometry default then override | 67.300000 | |
| cargo `type == 0x31 \|\| type == 0x2c` | 67.400000 | tie |
| cargo as `switch (type)` | 67.400000 | tie |
| waypoint `wx = wy = wz = 0;` | 67.400000 | tie |
| waypoint `int wy = 0, wz = 0, wx = 0;` | 67.400000 | tie |
| waypoint inverted `if (waypoint == nullptr)` | 67.400000 | tie |
| `Waypoint *wp = waypoint;` local | 67.400000 | tie |

All rows 2764 B unless noted. No variant exceeded 67.4 %; the 67.4 plateau
is broad (declaration-order / alias / tie-break changes are all coalesced
away by the allocator). 67.4 % is the best measured variant.

## Residual structural deltas vs the original

The first divergence is instruction 4 of the prologue:

```
orig  c0220 str r1,[sp,#60]        ; spill race only
      c0226 mov fp,r3              ; type -> fp
      c0228 mov r5,r2              ; shipClass -> r5
ours  c0220 strd r1,r2,[sp,#56]    ; spill race AND shipClass
      c0228 mov sl,r3              ; type -> sl
```

Downstream consequences (all pure greedy-allocator state, normalising the
register names alone would score ~83.8):

* `Globals::status` is `r4` in the original, `r6` in ours; `hitpoints` is
  `r5` in the original, `r4` in ours.
* The original keeps `shipClass` in a register (`r5` -> `sl` -> `r6`, with
  the `mov sl,r5` copy at `0xc0238`); ours spills it to `[sp,#60]` and
  reloads at `0xc0328`.
* `this` is spilled at the `empPoints` merge (`0xc0332`) in both, but the
  original reloads it per branch (`r6` at `0xc071e`/`0xc0b5e`), ours once
  into `sl` at `0xc03fc`.
* The original keeps the `PlayerFixedObject`/`PlayerFighter` `nullptr`
  geometry argument as a rematerialised `movs r0,#0` / `str r0,[sp]`
  (and reuses the `ship = nullptr` value `r9` for the fighter branch);
  ours hoists it into `r8` (`str.w r8,[sp]`).
* The original computes the spawn coordinates *before* `setEmpData`
  (`0xc03c8`-`0xc03e4`), ours sinks them after (`0xc03d4`-`0xc0402`).
  Moving the source statement does not change this (the scheduler sinks
  it either way); making the coordinates `int` restores the original's
  conversion-at-call-site shape but costs 28 B of literal pool and drops
  the score to 63.1.
* Literal pool word at `+0xa8c`: ours `0x0014f6cc`, original `0x0014f6d0`
  (a branch-offset word, not a data constant).

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

Scratch helpers (not committed): `/dev/shm/w6781/{mk,s1..s7,side,gaps,gapsord}.py`,
`measure2.py`, `build_variant.sh`, `orig.asm`, `ours.asm`, `iii.asm`, `v/*.so`.
