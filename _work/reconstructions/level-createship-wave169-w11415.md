# `Level::createShip` — wave 169 (w11415): declaration/use-order sweep for the
# original RA map at 2,764 B; no source win

Worktree `wt/w11415`, branch `wave169-w11415`, base `master ec3ceb07c` (wave 168).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`).  Evidence: objdump of the original
and of our linked `.so`, plus clang `-mllvm -print-machineinstrs`.

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0).

**No source change landed.**  The committed body remains the raw optimum
(1,560 / 455 / 2,764 B).  This note records the declaration/use-order sweep
and the measured reason the original map is not reachable from source order.

## 1. Entry map (original vs committed)

| value | original | committed |
|---|---|---|
| `race` | `[sp,#60]` (`str r1`) | `[sp,#56]` (`strd`) |
| `shipClass` | `r5 -> sl -> r6` | `[sp,#60]` (`strd`) |
| `type` | `fp` (r11) | `sl` (r10) |
| `this` | `r9` (spill `[56]`) | `r9` (spill `[52]`) |
| `jitterX/Y/Z` | `[36]/[32]/[28]` | `fp,[28],[24]` |

The original's stack-slot order (ascending address) is
`jz, jy, jx, wz, wx, wy, type, this, race`; the committed one is
`jz, jy, jx, wy, wz, wx, type, this, race, shipClass`.  The slot order
therefore witnesses the original's declaration order `jitterX, jitterY,
jitterZ` and `wy, wx, wz` — but reproducing those declarations does not
move the register map (see §2).

## 2. Sweeps (full link, size checked)

Every row is a full `link_so` + `fn_diff.py`; all variants are 2,764 B
unless noted.  Scratch `/dev/shm/w11415/`: `gen.py` (body generator),
`try.sh` (splice + build + fn_diff), `sweep.py`/`sweep2.py`, `sbs.py`
(instruction side-by-side), `mi.txt` (machineinstrs dump).

### 2.1 jitter order x coord type x mission position x null check (48)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **committed** (`YXZ`, float, mission first, null check) | **1,560** | **455** | **2,764** |
| faithful `XYZ`, float, mission first, null check | 1,560 | 460 | 2,764 |
| `ZYX`/`XZY`, float, mission first, null check | 1,560 | 460 | 2,764 |
| `YZX`/`ZXY`, float, mission first, null check | 2,384-2,564 | 895-1,111 | 2,772-2,796 |
| `XYZ`, float, mission after jitters | 1,605 | 570 | 2,764 |
| `XYZ`, float, no null check | 2,052 | 505 | 2,764 |
| `XYZ`, int coords, null check | 2,460 | 478 | **2,788** |
| `XYZ`, int coords, no null check | 2,423 | 475 | **2,792** |
| `XYZ`, int coords, mission after, null check | 2,463 | 945 | 2,784 |

The committed shape is the only one that reaches 1,560 at 2,764 B.  The
faithful `XYZ` jitter order is raw-equal but aligned 5 worse.

### 2.2 `wy/wx/wz` declaration order x assignment order (36)

Declaration order *does* move raw, assignment order does not:

| declaration order | raw | aligned |
|---|---:|---:|
| `wy, wx, wz` | **1,560** | 460 |
| `wy, wz, wx` / `wx, wy, wz` / `wx, wz, wy` | 1,561 | 462-463 |
| `wz, wy, wx` | 1,617 | 466 |
| `wz, wx, wy` | 1,619 | 464 |

None changes the entry map (`strd r1,r2; mov sl,r3`).

### 2.3 other declaration/use-order knobs (all no raw win)

* coord declaration order (`z,y,x` and all 6 permutations): 1,560/460, no map change.
* `int x,y,z` declared at the top / after the jitters / assigned late: 2,788, map unchanged.
* `int armor` / `int empTime` / `mission,level,hitpoints,empPoints,empTime,armor`
  declared at the top and assigned later: 1,560/460, no change (mem2reg
  erases declaration order for register values).
* `switch(type)` case order 0x2c,0x33,0x31 (the original's body order) and
  the `if`-chain equivalents: 1,563/444 (best aligned seen) but raw worse.
  `if`-chain 0x31,0x2c,0x33: 1,560/455.
* `__builtin_expect` on waypoint/`mission==0x9a`/`mission==56`/`shipClass==0/1`/
  `type` guards (19 placements): no raw < 1,560 at 2,764; several change size
  (2,712-2,772).  `expect(mission==0x9a,1)` is raw-neutral (1,560/460).
* inverted/ternary/truthy waypoint null check: 1,560-1,617, no map change.
* `if (!shipClass)`, `1 == shipClass`, `mission` bitmask/`(unsigned)(m-49)<=3`,
  `!(missionPtr==1||missionPtr==23)`: no raw win.
* null check retargeted to `player`/`rnd`/`waypoint`/`shipClass`: 2,052-2,564.

## 3. Why the map is not reachable from source order

`-mllvm -print-machineinstrs` (After Machine Instruction Scheduler) shows the
entry copies in reverse parameter order:

```
%130:gprnopc = COPY $r3      ; type
%129:gprnopc = COPY $r2      ; shipClass
%128:rgpr     = COPY $r1     ; race
%127:gpr      = COPY $r0     ; this
```

The Greedy allocator then keeps `type` and `this`, and spills `race` and
`shipClass` at entry (`t2STRi12 $r2, %stack.3; t2STRi12 $r1, %stack.2`).
The live ranges are `type 1568..26192` (16 uses), `shipClass 2944..6992`
(3 uses), `race 3760..7808` (7 uses), `this` throughout.

The decisive experiment is removing the second `else if (shipClass == 1)`
check (the branch guard at `c03fc`, `cmp r6,#1`).  With `else` instead:

* float coords: size drops to **2,756** and `shipClass` moves to `fp`
  (`mov r5,r0; mov sl,r3; mov fp,r2; mov r9,r1`), raw 2,430;
* int coords: 2,784 (with null check) / **2,764** (without), raw 2,182-2,389.

The `else` shape's own map is `this->r5, type->sl, shipClass->fp, race->r9`
(float, 2,756) or `this->[28], type->r9, shipClass->fp, race->sl`
(int, no null check, 2,764) — still not the original map.  Combining `else`
with the mission-after-jitters shape (`mov r6,r3; mov fp,r2; mov r9,r1`)
gives 2,740-2,768 B and again no `type->fp`.

So the second `shipClass == 1` use is exactly what forces `shipClass` out
of `fp` and into the stack in our build.  The original keeps it in
`r5 -> sl -> r6` *with* that use present; the extra register there is
`fp` because the original's `type` is in `fp`, and the original's
`empPoints` can then take `sl` after `shipClass` vacates it at `c02b4`.
Reproducing that hop needs the original's block frequencies (the
`shipClass` interval must be cheap enough to split to `r6` rather than
spill), not a source spelling: every declaration/use-order form measured
here either leaves the map identical or changes the size.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11415
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```
