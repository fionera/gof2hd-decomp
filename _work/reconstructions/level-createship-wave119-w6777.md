# Level::createShip — wave 119 (w6777): faithful spawn mapping, real-new shape

Status: **source change landed** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch `wave119-w6777`,
base `49f2ada5e`.

## Result

```
                                   before (HEAD)        after (landed)
createShip fuzzy (raw)             67.100000            67.400000
createShip size                    2764 B               2764 B
createShip LSDA record             336 B (exact)        336 B (exact)
createShip ttype                   0xff                 0xff
.ARM.extab size / addr             0x26294             0x26294
.ARM.exidx                         0x6f70 (exact)       0x6f70 (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
imports / data / lint / drift      207 / 843 / CLEAN / 0  207 / 843 / CLEAN / 0
```

`check` (final): **RATCHET PASS** — `verify: byte 4505 linked 4505
stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: 0/0 imports 207`, lint CLEAN, drift 0.  No section size, base,
import, data or symbol-parity movement.  `.ARM.extab` is still 4 B short
of the original (`0x26294` vs `0x26298`) and the only differing record is
`Generator::createMission` (172 vs 168); createShip's record is exact.

## Landed diff (8 insertions / 8 deletions, ordinary C++)

The previous body carried a *tuned but semantically wrong* spawn mapping:
`wz` held `waypoint->x`, `wy` held `waypoint->y`, `wx` held
`waypoint->z`, and the coordinate expressions were
`x = jitterZ + jitterX`, `y = wy + jitterY`, `z = wz + wx` — i.e. x/z
mixed two different sources.  The original (Thumb, confirmed against
Ghidra and the literal pool) is plainly:

```
x = waypoint->x + jitterX - 20000   (int)
y = waypoint->y + jitterY - 20000   (float)
z = waypoint->z + jitterZ - 20000   (float)
```

with the three `rnd->nextInt(40000)` calls in X, Y, Z order.  The landed
diff restores that mapping and the natural waypoint names
(`int wy, wz, wx;` declaration order, assignments `wx←x, wy←y, wz←z`)
and the X, Y, Z jitter call order.  It is a correctness fix *and* a
measured +0.3 fuzzy improvement.

## Why the score is still 67.4 (not the 74.4 placement-new shape)

The original really uses real `new` for all three ship objects, so the
LSDA record has 57 cleanup rows (336 B, exact).  With real `new` the
Greedy register allocator picks a different assignment than the
placement-new + `try` tuning hack:

```
original   str r1,[sp,#60] ; mov r9,r0 ; ldr r0,[pc] ; mov fp,r3 ; mov r5,r2
ours       str r1,[sp,#56] ; mov r9,r0 ; ldr r0,[pc] ; mov sl,r3 ; mov r4,r2
```

`this→r9` already matches.  The residual is `type→sl` (orig `fp`),
`shipClass→r4→fp` (orig `r5→sl`), `status→r6` (orig `r4`) and
`race→sp#56` (orig `sp#60`).  The cascade reaches the 24 `BoundingAAB`
calls and the capital-branch register permutation, which is where most
of the missing fuzzy points are.  Adding any `try`/`catch` restores the
placement-new RA but grows the LSDA record to 352 B (real new + try
= 63.7 %, 2768 B), so it cannot be used.

## Measurement table (official scorer, full link; all rows 2764 B unless noted)

`faith` = x/y/z use the matching waypoint field and jitter; `tuned` =
the old mixed mapping.  `wp` is the declaration order of the three
waypoint locals; `j` is the jitter call order.

| variant | raw % | note |
|---|---:|---|
| **HEAD: tuned mapping, wp `wx,wz,wy`, j `Y,Z,X`** | **67.100000** | base |
| **landed: faithful, wp `wy,wz,wx`, j `X,Y,Z`** | **67.400000** | exact LSDA |
| faithful, wp `wy,wz,wx`, j `X,Z,Y` | 67.400000 | same score |
| faithful, wp `wx,wy,wz`, j `X,Y,Z` / `X,Z,Y` | 67.200000 | |
| faithful, wp `wy,wx,wz`, j `X,Y,Z` / `X,Z,Y` | 67.200000 | |
| tuned, wp `wy,wz,wx`, j `X,Z,Y` | 67.400000 | |
| tuned, wp `wy,wz,wx`, j `X,Y,Z` | 67.300000 | |
| tuned, wp `wx,wy,wz`, j `X,Z,Y` | 67.300000 | |
| tuned, wp `wx,wy,wz`, j `X,Y,Z` | 56.900000 | bad RA, 2784 B |
| faithful, `int x, int y, float z` | 63.5–64.2 | 2780 B |
| faithful, all three `int` | 63.0–63.4 | 2788–2792 B |
| faithful, all three `float` | 58.5 | 2784 B |
| `switch (shipClass)` for the allocation | 63.4–63.8 | 2768 B, different RA |
| `PlayerFixedObject *ship` + cast | 47.6 | 2752–2756 B |
| real new + `try` around `setEmpData` | 63.7 | LSDA 352 B |
| real new + `try` around `new Player` | 62.9 | LSDA 352 B |
| `empTime = shipClass == 1 ? 45000 : 15000` | 64.7 | 2760 B |
| `empTime = 15000` + `if` | 67.1 | |
| hitpoints switch order `0x33,0x31,0x2c` | 67.3 | |
| hitpoints if-chain `0x33,0x31,0x2c` | 67.0 | |
| level/hitpoints/switch moved before jitter | 37.9 | call order broken |
| `register int` hints for type/shipClass | 67.1 | ignored |
| `Status *status = Globals::status` local | 61.3 | 2768 B |
| `AERandom *rnd = Globals::rnd` local | 66.8 | |

## Remaining `.ARM.extab` delta

Only `Generator::createMission` (172 vs 168) — different TU, out of
scope.  With it exact the section would be `0x26298` (original).

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6777/score.py` (single-function
fast scorer), `variants.py`, `sweep2.py`, `probe.py`, `eh.py`.
