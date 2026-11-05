# Level::createShip — wave 119 (w6790): register-allocation sweep, no source win

**Status:** no source change landed; note only (2026-11-05). Worktree `wt/w6790`,
branch `wave119-w6790`, base `32e535a5e`. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).

## Result

```
                                   before (HEAD)        after
createShip fuzzy (raw)             67.400000            67.400000
createShip size                    2764 B               2764 B
createShip match / len             674 / 1007           674 / 1007
createShip LSDA record             336 B / 57 rows      336 B / 57 rows  (exact)
.ARM.extab size / addr             0x26294              0x26294          (unchanged)
.ARM.exidx                         0x6f70  (exact)      0x6f70  (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
imports / data / lint / drift      207 / 843 / CLEAN / 0
```

`check` (final): **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0
missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: 0/0
imports 207`, lint CLEAN, drift 0.  The only remaining `.ARM.extab`
delta is `Generator::createMission` −4 (172 vs 168, different TU); the
createShip record is exact.  No section size/base/import/data/parity
movement.

## What was swept this session (all ordinary C++)

Every variant below was spliced into `src/game/world/Level.cpp`, fully
gold-linked and scored with the official fuzzy scorer (`tools/verify`
normalisation, NDK objdump).  Unless noted all rows are 2764 B and keep
the exact 336 B LSDA.

| variant | % | note |
|---|---:|---|
| **HEAD (int x, float y,z; spawn after `new Player`)** | **67.4** | base |
| `float x` instead of `int x` (before or after setEmpData) | 56.9 | 2784 B |
| `int ix,iy,iz` then `float x,y,z` (4 placements) | 56.9–57.6 | 2776–2784 B |
| all-int spawn (`iii`), spawn after `new Player` | 63.1 | 2792 B |
| all-int spawn, spawn before `new Player` | 62.3 | 2792 B |
| all-int spawn, spawn after `armor`, before `new Player` | 62.3 | 2792 B |
| int x + float y,z, spawn before `new Player` | 62.2 | 2788 B |
| int x + float y,z, spawn right after jitters | 59.5 | 2772 B |
| v1-style `x=wx; x += nextInt(40000)-20000;` | 62.2 | 2792 B |
| `int x` before `new Player`, y,z after | 67.4 | tie |
| y,z before `new Player`, x after | 58.6 | 2772 B |
| `float zero = 0.0f;` local (4 placements) used for BoundingAAB `ex` | 67.4 | tie |
| `const float zero` | 67.4 | tie |
| C89-style all locals declared at top | 67.4 | tie |
| `LODManager *lod = lodManager;` before branch | 36.4 | 2756 B |
| `Level *self = this;` | 67.4 | tie |
| `this->lodManager->addObject(...)` | 67.4 | tie |
| `ship` declared before `new Player` | 67.4 | tie |
| explicit `else { ship = nullptr; }` | 67.4 | tie |
| `AEGeometry *geometry = nullptr;` local for both ctors | 67.4 | tie |
| drop `kiFighter` local / drop `!= nullptr` in LOD test | 67.4 | tie |
| `switch (shipClass)` for empTime (2 forms) | 67.4 | tie |
| `int empTime = 15000; if (...) empTime = 45000;` | 67.4 | tie |
| `if (shipClass != 0 && shipClass == 1)` | 67.4 | tie |
| `if (shipClass == 1) ...; if (shipClass != 1) ...` | 66.9 | |
| empTime computed at top (ternary / if-else) | 62.2 | 2776 B |
| empTime computed after jitters | 64.1 | 2772 B |
| Yoda conditions (`1 == shipClass`, `0 == shipClass`) | 67.4 | tie |
| `if (waypoint)` instead of `!= nullptr` | 67.4 | tie |
| waypoint else-assignment order (3 permutations) | 67.4 | tie |
| `mission`/waypoint declaration order (3 forms) | 67.4 | tie |
| `jitterX + wx - 20000` / `-20000 + wx + jitterX` | 67.4 | tie |
| `static_cast<float>` instead of C cast | 67.4 | tie |
| `hitpoints *= 2.25f;` etc. (implicit conversion) | 67.4 | tie |
| `armor = 1000; if (hardCoreMode()) armor = 650;` | 67.4 | tie |
| `level = getLevel(); if (level > 20) level = 20;` | 64.0 | 2756 B |

The 67.4 plateau is universal: every semantically equivalent rewrite is
coalesced by the allocator to the same 674/1007 instruction match.  No
variant exceeded 67.4 %.

## Why 67.4 is the ceiling for this shape

The residual is almost entirely Greedy register allocation, not source
semantics.  The structural (register-name-normalised) match is 850/1007
(84.4 %); the best *global* register permutation only reaches 71.2 %
(swap r9↔fp), because the original re-uses registers per block.

Fixed-branch register map:

```
value        original              ours (HEAD)
this         r9 (entry)            r9
race         [sp,#60]              [sp,#56]
shipClass    r5 -> sl -> r6        [sp,#60]   (spilled at entry)
type         fp                    sl
status       r4                    r6
mission      r8                    r8
rnd          r5                    r5
nextInt fn   r6                    r4
jitter1      [sp,#36]              fp
jitter2      [sp,#32]              [sp,#32]
jitter3      [sp,#28]              [sp,#28]
empPoints    sl                    fp
x            fp -> sl              r6
y            r4 -> r8              r8
z            r8 -> r4              r4
volumes      fp                    r9
0.0f (ex)    r6  (movs r6,#0)      sl (mov.w sl,#0)
```

The chain is: the allocator spills `shipClass` at entry (ours) instead of
keeping it in `r5` (orig).  That frees `r5` for `rnd` and pushes `status`
to `r6`, `type` to `sl`, leaves `fp` free for `jitter1`/`empPoints`, so
`volumes` takes `r9`, `x` takes `r6`, and the BoundingAAB `0.0f` `ex`
argument can only be rematerialised in `sl` (`mov.w sl,#0` / `str.w sl,[sp]`,
8 B) instead of `r6` (`movs r6,#0` / `str r6,[sp]`, 6 B).  That is the
+4 B per BoundingAAB call-site LSDA range (orig +30, ours +34) and the
source of most of the missing match.

The second divergence is spawn placement: the original computes the three
spawn ints *before* `setEmpData` and converts them at each ctor call
(`int x, y, z`), whereas HEAD computes `x` after `setEmpData` (MachineSink /
ISel DAG scheduling) and converts `y`,`z` before it.  Making the coords
all-`int` restores the conversion-at-call-site shape but adds 28 B of
conversions (2792 B) and still gets the wrong allocation (63.1 %).
Putting the spawn before `new Player` restores the pre-`setEmpData`
placement (verified in the disassembly) but also perturbs the allocator
(62.3 %).

## LSDA call-site comparison (orig vs HEAD)

Both tables have 57 rows, TType `0xff`, all actions 0.  Ours' ranges are
shifted +2..+4 from the first `Player` invoke onward; each BoundingAAB
call site is +34 vs the original's +30 (the `mov.w sl,#0` rematerialisation
above).  The fixed branch is correspondingly shorter elsewhere, so the
function size stays 2764 B.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/gameresource_gen/lsda.py cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6790/{try.sh,splice.py,score.py,
score2.py,shape.py,mnem.py,perm.py,search3.py}` and the `body_*.cpp`
variants listed above.
