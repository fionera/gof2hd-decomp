# `Level::createShip` — wave 158 (w10417): waypoint null-check as
# `if/else` with `wy, wx, wz` declaration order is a small raw+aligned win
# (1,564/452 -> 1,563/449); the type/shipClass/race register permutation is
# still not reachable from 100+ source shapes

Worktree `wt/w10417`, base `master 9e4795206` (wave 157).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Ghidra provider was unavailable (`rea decompile` -> `provider_unavailable`);
all evidence below is objdump of the original, which is authoritative.

Gate on entry and exit (full `check`, `RATCHET PASS`): `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, avg 99.93 % (unchanged; the baseline's 99.94 is
stale and is not ratcheted).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,564** | **1,563** |
| instruction-index alignment | **452** | **449** |
| size | 2,764 | 2,764 |

## 1. Landed change: the waypoint null check is an `if/else`, not a ternary

Before (wave-157 body):

```cpp
    int wy = waypoint != nullptr ? waypoint->y : 0;
    int wz = waypoint != nullptr ? waypoint->z : 0;
    int wx = waypoint != nullptr ? waypoint->x : 0;
```

After (landed):

```cpp
    int wy = 0;
    int wx = 0;
    int wz = 0;
    if (waypoint != nullptr) {
        wy = waypoint->y;
        wx = waypoint->x;
        wz = waypoint->z;
    }
```

The declaration order is the lever, not the `if` itself.  All six
declaration/assignment permutations were measured (full link):

| form (decl order) | raw | aligned | size |
|---|---:|---:|---:|
| **`if` wy, wx, wz (landed)** | **1,563** | **449** | 2,764 |
| `if` wy, wz, wx | 1,564 | 451 | 2,764 |
| `if` wx, wy, wz | 1,564 | 452 | 2,764 |
| `if` wx, wz, wy | 1,564 | 452 | 2,764 |
| `if` wz, wx, wy | 1,621 | 453 | 2,764 |
| `if` wz, wy, wx | 1,619 | 455 | 2,764 |
| ternary wy, wz, wx (base) | 1,564 | 452 | 2,764 |
| ternary wy, wx, wz | 1,619 | 455 | 2,764 |
| ternary wx, wy, wz | 1,621 | 453 | 2,764 |

`if` wy, wx, wz is the only shape that improves **both** metrics.  It is also
closer to the original's null path, which zeroes `wy, wx, wz` in that order:

```
orig c024e movs r0,#0
     c0250 str  r0,[sp,#48]   ; wy
     c0252 movs r0,#0
     c0254 str  r0,[sp,#44]   ; wx
     c0256 movs r0,#0
     c0258 str  r0,[sp,#40]   ; wz
```

Ours now zeroes `wz, wx, wy` (reverse), but the non-null store order and the
coordinate reload order move one byte/three instructions closer.  The
register map is **unchanged** by this shape.

## 2. Register map (unchanged; the residual)

Entry, original vs ours (both 2,764 B):

```
orig                              ours
c0220 str  r1, [sp,#60]   race   0x0c strd r1, r2, [sp,#56]  race+shipClass
c0222 mov  r9, r0         this   0x10 mov  r9, r0             this
c0224 ldr  r0, [pc]              0x12 ldr  r0, [pc]
c0226 mov  fp, r3         type   0x14 mov  sl, r3             type
c0228 mov  r5, r2         shipCl  (-- shipClass spilled --)
c022a add  r0, pc                0x16 add  r0, pc
c022c ldr  r4, [r0]       status 0x18 ldr  r4, [r0]           status
c022e ldr  r0, [r4]              0x1a ldr  r0, [r4]
c0230 blx  getCurrentCampaignMission
c0234 mov  r8, r0         mission 0x20 mov r8, r0             mission
c0236 ldr  r0, [r7,#8]    waypoint 0x22 ldr r0, [r7,#8]      waypoint
c0238 mov  sl, r5         shipCl  (-- no mov --)
c023a cbz  r0                    0x24 cbz  r0
```

| value | original | ours |
|---|---|---|
| `this` | r9 | r9 |
| `race` | `[sp,#60]` | `[sp,#56]` |
| `shipClass` | r5 -> sl -> r6 | `[sp,#60]` |
| `type` | fp (r11) | sl (r10) |
| `Status *` | r4 | r4 |
| `mission` | r8 | r8 |
| `&AERandom` | r5 (after jitter block) | r5 |
| `&nextInt` | r6 | r6 |
| `jitterX` | `[sp,#36]` (spilled) | fp (r11) |
| `jitterY` | `[sp,#32]` | `[sp,#28]` |
| `jitterZ` | `[sp,#28]` | `[sp,#24]` |

The original keeps **both** `shipClass` (r5->sl->r6) and `type` (fp) in
callee-saved registers and spills all three jitters.  Ours keeps `type` in sl
and `jitterX` in fp, and spills `race`+`shipClass` as a `strd` pair.  The
competition is therefore **shipClass vs jitterX** for the fifth callee-saved
slot (this/status/mission/type take four), not type vs shipClass: `type` is
the first value materialised in both builds.  The original's PGO branch
weights make `shipClass`'s uses outrank `jitterX`'s single use; our static
heuristic ranks the entry-block `jitterX` use higher.

The original's `mov r6, sl` at c02b4 (frees sl for `empPoints`) is the visible
tip; ours has no such move because shipClass is already spilled.

## 3. New shapes measured this wave (all full link, clang 7.0.2 `-Oz`)

### 3.1 `__builtin_expect` matrix (3^4 = 81 combinations)

Knobs: `waypoint != nullptr`, `shipClass == 1` (empTime), `shipClass == 0`
(creation), `mission == 0x9a`, each `{none, 0, 1}`.  **No combination
produces the original's `mov fp, r3` (type->fp) / `mov r5, r2`
(shipClass->r5).**  The base map (`type->sl`, shipClass spilled) is stable
for 60+ of the 81.  The only map-changing combination is
`expect(shipClass == 0, 0)`, which re-rolls the whole entry to
`race->sl, shipClass->fp, type->r9, this spilled` (2,712 B, raw 2,430,
aligned 1,413).  Selected sizes/metrics:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base | 2,764 | 1,564 | 452 |
| `expect(shipClass==1,1)` | 2,768 | 2,114 | 503 |
| `expect(shipClass==1,0)` | 2,768 | 2,417 | 514 |
| `expect(shipClass==0,1)` | 2,744 | 2,476 | 1,302 |
| `expect(shipClass==0,0)` | 2,712 | 2,430 | 1,413 |
| both `==1` | 2,748 | 2,510 | 1,306 |
| both `==0` | 2,724 | 2,565 | 1,431 |
| `==1` + `==0` | 2,716 | 2,414 | 1,415 |
| `==0` + `==1` | 2,748 | 2,520 | 1,316 |

### 3.2 Structural shapes (all coalesce or change size)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `bool isCapital = shipClass == 1;` used in both sites | 2,712 | 2,399 | 1,116 |
| `switch (shipClass)` creation, case 0 first | 2,780 | 2,491 | 488 |
| `switch (shipClass)` creation, case 1 first | 2,776 | 2,537 | 1,346 |
| `Status *st = Globals::status;` all uses | 2,800 | 2,455 | 951 |
| `int sc = shipClass;` used everywhere | 2,764 | 1,564 | 452 (coalesced) |
| `level += shipClass & 0;` (extra use) | 2,764 | 1,564 | 452 (coalesced) |
| `int lvl0 = getLevel(); level = lvl0 > 20 ? 20 : lvl0;` | 2,756 | 2,298 | 541 |
| `do { entry } while (0)` | 2,764 | 1,564 | 452 (coalesced) |
| jitters declared-then-assigned | 2,764 | 1,564 | 452 (coalesced) |
| jitters computed Z,Y,X | 2,764 | 1,564 | 452 (coalesced) |
| `switch (mission)` bitmask | 2,764 | 1,564 | 527 |
| `mission == 56 \|\| (unsigned)(mission-49) <= 3u` | 2,764 | 1,577 | 456 |
| `expect(mission==0x9a,1)` | 2,764 | 1,564 | 452 (coalesced) |

### 3.3 Guard-body variants (the wave-153 `status == nullptr` filler)

Adding a second use inside the guard changes the size in every case:

| guard body | size | raw | aligned |
|---|---:|---:|---:|
| `x = 0;` (base) | 2,764 | 1,564 | 452 |
| `x = 0; mission = 0;` | 2,764 | 1,564 | 452 (coalesced) |
| `x = 0; empPoints = 0;` | 2,764 | 1,564 | 452 (coalesced) |
| `x = 0; waypoint = nullptr;` | 2,764 | 1,564 | 452 (coalesced) |
| `x = 0; jitterX = 0;` | 2,764 | 1,564 | 452 (coalesced) |
| `x = 0; shipClass = 0;` | 2,784 | 2,488 | 489 |
| `x = 0; type = 0;` | 2,796 | 2,469 | 520 |
| `x = 0; race = 0;` | 2,772 | 2,430 | 499 |
| `x = 0; hostile = false;` | 2,768 | 2,425 | 540 |
| `x = 0; group = false;` | 2,788 | 2,426 | 475 |
| `status == nullptr && shipClass == 0` | 2,768 | 2,410 | 540 |
| guard moved before `setEmpData` | 2,776 | 2,417 | 503 |
| guard after `ship->setLevel` | 2,764 | 1,564 | 452 (dead, coalesced) |

### 3.4 Coordinate-type permutations (current body)

The wave-152 `iif`/`expect(mission==0x9a,1)` win does **not** transfer to the
wave-157 body: every non-`iff` shape is +20..+36 B.

| x/y/z | size | raw | aligned |
|---|---:|---:|---:|
| `int x, float y, float z` (base) | 2,764 | 1,564 | 452 |
| `int x, int y, float z` | 2,784 | 2,488 | 474 |
| `int x, float y, int z` | 2,784 | 2,488 | 485 |
| `float x, int y, int z` | 2,796 | 2,523 | 472 |
| `float x, int y, float z` | 2,776 | 2,439 | 516 |
| `float x, float y, int z` | 2,784 | 2,374 | 1,087 |
| `int x, int y, int z` | 2,788 | 2,481 | 903 |
| `float x, float y, float z` | 2,800 | 2,466 | 564 |

Declaration order of the coordinate locals is irrelevant (clang reorders);
all four orders of `iff` give 1,563/449 on the landed null-check shape.

## 4. Conclusion

The landed `if/else` null check with `wy, wx, wz` declaration order is a
small but real improvement on **both** metrics (raw 1,564 -> 1,563, aligned
452 -> 449) at exact size 2,764 B, with the gate green and the fuzzy
unchanged (99.93 %).  The type/shipClass/race permutation itself remains a
PGO spill-weight outcome: across 81 expect combinations plus ~30 structural
shapes, no source spelling reaches `type->fp` + `shipClass->r5`; the only
map-changing lever (`expect(shipClass==0,0)`) re-rolls the whole entry and
regresses size.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10417
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10417/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb   # 1563
python3 /dev/shm/w10417/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb  # 449
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10417/`: `fdiff.py`, `align_diff.py` (worktree-pointed),
`sbs.py` (aligned side-by-side), `measure.py` (body splice + build + both
metrics), `entry_o.py`/`compile_o.sh` (compile-only entry-map probe),
`search.py` (81-combo expect matrix), `gen*.py` and `v_*.cpp` (all variants),
`Level.base.cpp` (pristine wave-157 source).
