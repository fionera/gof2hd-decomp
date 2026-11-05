# Level::createShip — wave 119 (w6813): one-call level/empPoints clamps + status null check reach 70.2 %

**Status:** source change landed (2026-11-05). Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns). Branch
`wave119-w6813`, base `3981b32c5`.

## Result

```
                                   before (HEAD)        after (landed)
createShip fuzzy (raw)             67.400000            70.200000
createShip size                    2764 B               2764 B
createShip insns (orig/ours)       1007 / 994           1007 / 1006
createShip LSDA record             336 B (exact)        336 B (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
.ARM.exidx                         0x19cc08 / 0x6f70    0x19cc08 / 0x6f70
.ARM.extab                         0x26294              0x26294
.rodata                            0x41c27              0x41c27
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
```

`check` (final): **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0
missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: imports
207`, lint CLEAN, drift 0.  No section size/base/import/data/parity
movement; the only remaining `.ARM.extab` delta is still
`Generator::createMission` (different TU), and `.rodata` is unchanged.

## Landed source changes (all ordinary C++)

```cpp
-    int wy, wz, wx;
+    int wx, wz, wy;
...
-    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
+    int level = Globals::status->getLevel();
+    if (level > 20) level = 20;
...
-    int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
+    int empPoints = Globals::status->getLevel();
+    if (empPoints > 20) empPoints = 20;
+    empPoints = empPoints * 5 + 40;
...
+    if (Globals::status == nullptr) return nullptr;
     KIPlayer *ship = nullptr;
```

## Why this works

The original's two clamps are the two-call ternary form
(`getLevel() > 20 ? 20 : getLevel()`), i.e. **four** `getLevel` calls
(`0xc0286`, `0xc0296`, `0xc0316`, `0xc0324`).  The one-call if-forms cut
that to **two** calls and shorten the live ranges at the prologue.  That
alone frees the `r4` slot for `&Globals::status` (the original's first
free register) and changes the spill cascade; the two clamps together
shrink the body by 16 B (2764 -> 2748).  The late
`if (Globals::status == nullptr) return nullptr;` (the wave-119 w6799
`nc` shape) adds the extra late `Globals::status` read that completes the
prologue shift and costs exactly +16 B, so the net size is again 2764 B
and `.text` does not move.  The `wx, wz, wy` declaration order is a
stack-slot/RA tie-break that adds a further +0.1.

The new prologue now matches the original on `status -> r4`; the
remaining differences are `shipClass` (orig `r5 -> sl`, ours spilled) and
`type` (orig `fp`, ours `sl`), the same cascade documented in
`level-createship-wave119-w6785.md` / `w6799.md`.

## Measurements (official scorer, full link; all rows 2764 B unless noted)

| variant | raw % | size |
|---|---:|---:|
| **HEAD (faithful two-call ternary, no null check)** | **67.4** | 2764 |
| `nc` alone (w6799 shape) | 68.3 | 2780 |
| one-call `level` clamp alone | 64.0 | 2756 |
| one-call `empPoints` clamp alone | 64.5 | 2756 |
| both one-call clamps, no `nc` | 64.0 | 2748 |
| both one-call clamps + `nc` | 70.1 | 2764 |
| **both one-call clamps + `nc` + `int wx, wz, wy;`** | **70.2** | **2764** |
| `nc` + one-call `level` + two-call `empPoints` | 68.0 | 2772 |
| `nc` + two-call `level` + one-call `empPoints` | 68.3 | 2772 |
| `nc` + both two-call (faithful) | 68.3 | 2780 |
| reuse the clamped `level` for `empPoints` (one `getLevel` total) | 67.0 | 2764 |
| semantic jitter-assignment reorder `XZY` / `ZXY` (not landed) | 70.3 | 2764 |

The `XZY`/`ZXY` rows reorder the three `nextInt` assignments, which
changes which random value goes to which coordinate; the original call
order is `X,Y,Z`, so the faithful `XYZ` order was kept.  Every other
tested form (Yoda/`>=`/ternary/braces clamp forms, `unsigned` locals,
`armor` if-form, `empTime` init form, `player` decl-then-assign,
`zero` local, waypoint ternary/init forms, `mission` switch/bitmask
forms, switch-case reorders, fighter/capital block reorders, `fixed`/
`volumes` decl-then-assign, `ship` placement, cargo/LOD forms, scale
locals, `setEmpData` locals/casts) ties 70.2 or loses.  A 399-pair
search over the tying forms found no further improvement.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6813/{fastscore,score}.py`,
`variants*.py`, `tweaks*.py`, `combo*.py`, `pairsearch.py`.
