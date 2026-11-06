# `Level::createShip` — wave 119 (w6973): RA dead-end sweep, no source win

Target: `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave119-w6973`, base = master `ffb4027ca`.  Source file
`src/game/world/Level.cpp` (function starts at the `PlayerFixedObject *Level::createShip(` line).

## Result

No source variant beat the landed body.  The function stays at **74.4 %**,
**2764 B**, 1007/1007 instructions.  `git diff` of `src/` is empty; only this
note is committed.

```
                                    value
createShip fuzzy (official)         74.4
createShip size                     2764 B
createShip insns (orig/ours)        1007 / 1007
createShip LSDA record              336 B / 328 B (-8)
HangarWindow::render LSDA           948 / 952 (+4)
Generator::createMission LSDA       172 / 176 (+4)
.ARM.extab total delta              0 (section stays 0x26298)
```

`check` baseline (unchanged): **RATCHET PASS** — byte 4505 / linked 4505,
data 843/843, imports 207, lint CLEAN, drift 0.

## Why the remaining 25 % is register allocation, not source shape

The landed body is the w6960 placement-new + `try`/`__builtin_trap()` shape.
The first 98 instructions are byte-identical; the diff is almost entirely
register-name and branch-offset differences caused by one RA decision.

The original's fixed branch uses:

```
fixed   = r9        volumes = fp
x = sl   y = r6     z = r8      (BoundingAAB loop)
type = r4  race = r6            (reloaded from [sp,#52]/[sp,#60])
this = r6                       (reloaded at c03fe, after cmp r6,#1)
```

Ours uses:

```
fixed   = fp (after `mov fp,r6`)   volumes = r9
x = r6   y = r8   z = sl
race = r4  type = reloaded from [sp,#52] on each use
this = r10 (sl)                    (reloaded at c03fc, before the beq)
```

Root cause (MIR): the `fixed` bitcast `%44 = COPY %259` is scheduled into the
block *after* the `Array` ctor (bb.42).  `%259` (operator-new result) gets r6
and `%44` gets fp, so the copy survives as `mov fp,r6`; the extra 2 bytes push
the jump table from `c0458` to `c045a` and add a `nop` at `c0462`.  Because
`this` was reloaded into r10 (sl) instead of r6, r6 was free when `fixed` was
allocated, so the RA picked r6; the loop then needs r6 for `x` and the value
must move to fp.  In the original, `this` occupies r6 at that point (reloaded
after `cmp r6,#1`), so `fixed` gets r9 and `volumes` gets fp.

The `this` reload position is the other half: ours is hoisted to the common
dominator (end of bb.26, before `beq`), the original rematerialises it inside
each branch.  Every source spelling that would move it (cache `this`, cache
`lodManager`, cache `missionPtr`, `if`/`else`/`switch` forms, `bool` locals,
`__builtin_expect` on the conditions) either leaves the code identical
(74.4) or collapses the RA.

## Measurements (official single-function scorer, full link)

All rows are 2764 B unless noted.  "faithful" = `wx=waypoint->x`,
`wy=waypoint->y`, `wz=waypoint->z`, jitter order X,Y,Z,
`x=wx+jitterX`, `y=wy+jitterY`, `z=wz+jitterZ`, and
`setShipGroup(..., type, hostile)`.

| variant | % | size | note |
|---|---:|---:|---|
| **HEAD (landed body)** | **74.4** | 2764 | 1007/1007 |
| `setShipGroup(..., type, hostile)` only | 73.9 | 2764 | semantic fix, score -0.5 |
| faithful waypoint map only | 72.5 | 2764 | |
| faithful map + `hostile` | 72.0 | 2764 | |
| fully faithful | 72.0 | 2764 | |
| faithful map, `int x`, `float y,z` | 72.5 | 2764 | |
| faithful map, `int x,y,z` | 61.6 | 2756 | 1006 insns |
| Java `spawnX/Y/Z` shape (ints) | 61.0 | 2752 | 1005 insns |
| Java shape, real `new` | 64.0 | 2796 | |
| Java shape, no `try` | 61.9 | 2796 | |
| base, no `try` (plain `setEmpData`) | 70.5 | 2776 | |
| real `new` fixed only | 63.6 | 2768 | |
| real `new` fighter only | 65.4 | 2784 | |
| `try` around `setLevel` | 66.1 | 2780 | |
| `try` around fixed branch | 45.8 | 2796 | |
| `try` around whole if/else | 37.6 | 2748 | |
| `try` around `Player new`+coords+`setEmpData` | 64.5 | 2788 | |
| `NoopGuard` around `setEmpData` | 66.4 | 2760 | |
| `NoopGuard` + trap anchor | 55.9 | 2800 | |
| `catch(...) { throw; }` | 49.8 | 2740 | |
| `catch(...) { }` | 65.5 | 2772 | |
| `catch(...) { return nullptr; }` | 66.4 | 2772 | |
| `catch(...) { __builtin_abort(); }` | 71.8 | 2764 | |
| `catch(...) { getLevel(); }` | 45.5 | 2756 | |
| `NoopGuard` with dtor call | 69.4 | 2776 | |
| `min`-form level/empPoints clamp | 73.8 | 2764 | |
| convert coords after `setEmpData` (faithful) | 65.1 | 2756 | |
| convert coords after `setEmpData` (tuned) | 65.3 | 2756 | |
| cache `lodManager` in a local | 36.8 | 2748 | |
| cache `this` in a local | 74.4 | 2764 | tie |
| cache `missionPtr` in a local | 67.3 | 2760 | |
| `volumes` placement-new | 66.4 | 2756 | |
| `volumes` allocated before `fixed` | 59.3 | 2760 | |
| both placement-new, `volumes` first | 64.4 | 2748 | |
| swap fighter/fixed branch order | 34.0 | 2760 | |
| `switch (shipClass)` | 39.3 | 2760 | |
| type check before `missionPtr` check | 47.1 | 2760 | |
| `switch (type)` for cargo cleanup | 74.4 | 2764 | tie |
| `field_0x40=1` before `addObject` | 74.3 | 2764 | |
| `ship=fixed` before `field_0x40=1` | 74.4 | 2764 | tie |
| `addObject` before `setShipGroup` | 74.3 | 2764 | |
| `default` last / first in race switch | 74.4 | 2764 | tie |
| `__builtin_expect(missionPtr…)` 1 / 0 | 74.4 | 2764 | tie |
| `__builtin_expect(type…)` 1 | 74.4 | 2764 | tie |
| `__builtin_expect(shipClass==0, 1)` | 52.7 | 2740 | |
| `__builtin_expect(shipClass==0, 0)` | 27.5 | 2732 | |
| `unsigned` empPoints / `long` empTime / `long` hitpoints | 74.4 | 2764 | tie |
| all 6 x/y/z declaration orders | 74.4 / 74.3 | 2764 | tie |
| `volumes` declared first / at top / `fixed` at top | 74.4 | 2764 | tie |
| `ship` declared before try / before `Player new` / at top | 74.4 | 2764 | tie |
| `auto` volumes / `auto` fighter | 74.4 | 2764 | tie |
| no `kiFighter` alias / truthy / local `geom` | 74.4 | 2764 | tie |
| `bool isFighter/isCapital` locals | 74.4 | 2764 | tie |
| `__builtin_assume` anchor | 70.5 | 2776 | |
| `__builtin_unreachable` anchor | 70.5 | 2776 | |
| `if (mission==-1) __builtin_trap()` anchor | 39.2 | 2736 | |
| `empPoints` before `hitpoints` / `level` / top | 56.9 | 2768 | |

### x/y/z type sweep (tuned map, 8 combinations)

`f`=float, `i`=int, order x/y/z:

| types | % | size |
|---|---:|---:|
| `fff` | 64.8 | 2748 |
| `ffi` | 72.2 | 2764 |
| `fif` | 72.6 | 2764 |
| `fii` | 66.8 | 2780 |
| **`iff` (HEAD)** | **74.4** | **2764** |
| `ifi` | 63.1 | 2804 |
| `iif` | 68.8 | 2780 |
| `iii` | 61.0 | 2752 |

The landed `iff` body is the best of the eight.  The original is `iii`
(conversions at the ctor calls), but `iii` collapses the RA: it is 8 B short
(2756 vs 2764) and the prologue changes (`str r2,[sp,#60]`, `mov r9,r3`,
`str r0,[sp,#56]` — `this` spilled at entry, `type` in r9).

## Suggested next step

The only lever left is the `this` reload.  If `this` can be forced into r6
(reloaded after `cmp r6,#1`, as in the original), `fixed` should fall into r9
and the whole fixed-branch register assignment (and the `mov fp,r6` + `nop`)
should follow.  Source-level attempts all normalise; a future pass should try
MIR-level experiments (e.g. a shape that makes the `%44 = COPY %259` bitcast
stay in the allocation block so the coalescer can merge it) before spending
more source variants.

## Reproduction

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

Scratch helpers (not committed): `/dev/shm/w6973/` — `apply.py` (splice),
`score.sh`, `opcodes.py` (normalised diff + score), `align.txt` (side-by-side
address/instruction alignment), `base_fn.cpp` and the `t_*`, `v*`, `s*`,
`q_*`, `r_*`, `u_*`, `w*`, `x*`, `y*`, `z*`, `j_*`, `m*`, `n*`, `o*`, `p*`
variant files.
