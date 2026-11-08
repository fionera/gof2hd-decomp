# `Level::createShip` — wave 159 (w10517): the coordinate locals are
# `int` in the original, and their *liveness* (not the type/shipClass
# order) is what moves `type` into `fp`; landed a 3-byte raw win from
# the `switch (type)` case order (1,563 -> 1,560)

Worktree `wt/w10517`, base `master cfde1f5f2` (wave 158).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Ghidra
provider unavailable; all evidence is objdump of the original, which is
authoritative.

Gate on entry and exit (full `check`): `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,563** | **1,560** |
| instruction-index alignment | **449** | **460** |
| size | 2,764 | 2,764 |

The raw win is small and costs 11 aligned instructions; the aligned
metric is not ratcheted, and the raw metric is the one that defines
`byte_exact`, so the change is gate-legal.  If the coordinator prefers
the aligned number, reverting this one hunk restores 1,563 / 449.

## 1. Landed change: `switch (type)` case order `0x31, 0x2c, 0x33`

The original's compare order is `0x33, 0x31, 0x2c` (c02b0/c02c0/c02c6)
and its case-body order is `0x2c, 0x33, 0x31` (c02cc/c02d2/c02e4), which
is what the wave-158 source already had.  All six source permutations
were measured (full link):

| case order | raw | aligned | size |
|---|---:|---:|---:|
| **`0x31, 0x2c, 0x33` (landed)** | **1,560** | **460** | 2,764 |
| `0x31, 0x33, 0x2c` | 1,560 | 460 | 2,764 |
| `0x2c, 0x33, 0x31` (base) | 1,563 | 449 | 2,764 |
| `0x2c, 0x31, 0x33` | 1,563 | 449 | 2,764 |
| `0x33, 0x31, 0x2c` | 1,563 | 451 | 2,764 |
| `0x33, 0x2c, 0x31` | 1,563 | 451 | 2,764 |

Only the two orders that put `0x31` first change the raw count; they
also re-roll the trailing branch offsets, which is the aligned +11.
The waypoint declaration order was re-swept against the new case order:
`y,x,z` (the landed wave-158 order) remains the best raw (1,560);
`x,y,z`/`x,z,y`/`y,z,x` give 1,561, `z,x,y`/`z,y,x` give 1,619/1,617.

## 2. New finding: the original's coordinate locals are `int`

The original converts the coordinates to float **inside each ship
branch**, not once up front:

```
capital (c040c-c0422):  vmov s0,fp; vcvt s20 = (float)x
                        vmov s0,r4; vcvt s18 = (float)y
                        vmov s0,r8; vcvt s16 = (float)z
fighter (c06c0-c06d8):  vmov s0,fp; vmov s2,r8; vmov s4,r4
                        vcvt s0/s2/s4
```

`fp`,`r4`,`r8` hold **ints** `x`,`y`,`z` at that point.  Our source
declares `float y`, `float z` and converts them early, so it has only
three `vcvt` sites; the original has six.  This is also visible in the
data layout: the original's code ends at `c0b88` (pop) leaving **344**
bytes of jump-table/literal-pool data; the all-`int` variant ends at
`0x990` leaving the same 344 bytes, while the landed base leaves 338.
So the original source is

```cpp
int x = wx + jitterX - 20000;
int y = wy + jitterY - 20000;
int z = wz + jitterZ - 20000;
...
new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
...
Vector pos = {(float) x, (float) y, (float) z};
```

All-`int` alone is **2,792 B** (+28), so it is not gate-legal by
itself; the +28 is entirely in the code (the data matches), i.e. it is
a *consequence* of the register map, not an independent size cost.
If the map is ever matched, the size should snap back to 2,764.

## 3. The lever for `type -> fp` is coordinate liveness

Screening was done compile-only (exact section size + entry map; the
object's `.text.<fn>` size equals the linked size).  The entry-map
signature of the original is `mov fp, r3` (type) + `mov r5, r2`
(shipClass) + `str r1,[sp,#60]` (race spill) + `mov sl, r5` after the
waypoint load.

**Moving the coordinate computation *before* `new Player(...)` puts
`type` in `fp`:**

```
int z = ...; int y = ...; int x = ...;
Player *player = new Player(armor, hitpoints, 1, 1, 0);
player->setEmpData(empPoints, empTime);
```

gives `mov fp, r3` at the entry (size 2,792).  With
`__builtin_expect(shipClass == 0, 1)` it is 2,768 B, still
`fp_r3` but `shipClass` remains in the `strd` spill.  Putting the
coords *after* `new Player` (the faithful order) drops `type` back to
`sl`.  So `type`'s callee-saved slot is decided by whether the int
coords are live across the `Player` constructor: when they are, the
allocator keeps `type` in `fp` and the coords take the scratch
callee-saved regs; when they are not, `type` takes `sl` and `fp` goes
to `jitterX`.

No shape reached the full original map (`type->fp` **and**
`shipClass->r5->sl` **and** `race` spilled) at size 2,764.  The closest
are:

| shape | size | entry |
|---|---:|---|
| all-int, coords before `new Player`, no guard | 2,792 | `fp=type`, shipClass spilled |
| + `expect(shipClass==0,1)` | 2,768 | `fp=type`, shipClass spilled |
| + `expect(shipClass==0,1)` + `int level; if(level>20)level=20` | 2,760 | `fp=type`, shipClass spilled |
| `expect(shipClass==0,1)`, coords after `new Player` | 2,736 | no `fp=type` |

## 4. Register map (landed body, unchanged from wave 158)

| value | original | ours |
|---|---|---|
| `this` | r9 | r9 |
| `race` | `[sp,#60]` | `[sp,#56]` |
| `shipClass` | r5 -> sl -> r6 | `[sp,#60]` |
| `type` | fp (r11) | sl (r10) |
| `Status *` | r4 | r4 |
| `mission` | r8 | r8 |
| `&AERandom` | r5 | r5 |
| `&nextInt` | r6 | r6 |
| `jitterX` | `[sp,#36]` | fp (r11) |
| `jitterY` | `[sp,#32]` | `[sp,#28]` |
| `jitterZ` | `[sp,#28]` | `[sp,#24]` |

The original keeps **both** `shipClass` (r5->sl->r6) and `type` (fp)
and spills all three jitters; ours keeps `type` (sl) and `jitterX`
(fp) and spills `race`+`shipClass` as a `strd` pair.  The competition
is `shipClass` vs `jitterX` for the fifth long-lived callee-saved slot
once `type` is fixed in `fp`; the coordinate-liveness lever above is
the only source-level control over `type`'s slot found so far.

## 5. Other shapes measured this wave (all full link unless noted)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base (wave 158) | 2,764 | 1,563 | 449 |
| all-int coords, no guard | 2,792 | 2,388 | 477 |
| all-int coords, x,y,z order, no guard | 2,792 | 2,386 | 468 |
| all-int + guard | 2,788 | 2,482 | 903 |
| all-float coords | 2,800 | 2,466 | 564 |
| `Status *status` local (all uses) | 2,800 | 2,455 | 951 |
| `do { whole body } while (0)` | 2,800 | — | — |
| `switch (mission)` bitmask | 2,764 | 1,563 | 524 |
| `switch (shipClass)` construction | 2,780 | 2,491 | 488 |
| int coords + `expect(shipClass==0,1)` | 2,764 | 2,457 | 1,318 |
| int coords + both shipClass expects | 2,764 | 2,448 | 1,319 |
| `int sc = shipClass` (construct / empTime) | 2,764 | 1,563 | 449 (coalesced) |
| `bool fighter = shipClass == 0` | 2,764 | 1,563 | 449 (coalesced) |
| `if (!shipClass)` | 2,764 | 1,563 | 449 (coalesced) |
| `int ty = type` for switch | 2,764 | 1,563 | 449 (coalesced) |
| `int empPoints` hoisted | 2,764 | 1,563 | 449 (coalesced) |

Coordinate type permutations (int/float per axis, 16 shapes, compile
screen): sizes 2,720-2,760, none 2,764 and none with `fp=type` when
the coords are after `new Player`.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10517
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10517/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb       # 1560
python3 /dev/shm/w10517/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb  # 460
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10517/`: `m.py` (splice + compile-only section size +
entry map), `batch.py` (screen many bodies), `full.py` (splice + link +
both metrics), `fdiff.py`/`align_diff.py` (worktree-pointed copies),
`sbs.py` (aligned side-by-side), `gen1.py`..`gen6.py` and `v_*.cpp`/
`g_*.cpp`/`t_*.cpp`/`u_*.cpp`/`w_*.cpp`/`x_*.cpp`/`y_*.cpp`/`z_*.cpp`/
`p_*.cpp`/`q_*.cpp`/`r_*.cpp`/`m_*.cpp`/`n_*.cpp`/`o_*.cpp`/`k_*.cpp`
(all measured variants), `Level.orig.cpp` (pristine wave-158 source),
`orig_full.txt`/`ours_full.txt` (full disassembly), `mir.txt`
(`-mllvm -print-machineinstrs` dump).
