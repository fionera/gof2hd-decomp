# `Level::createShip` — wave 160 (w10614): the type-switch body order
# `0x31,0x2c,0x33` is still the only raw win (1,563 -> 1,560) and no
# source shape improves *both* raw and aligned at 2,764 B

Worktree `wt/w10614`, base `master 08af4633b` (wave 159).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Ghidra
provider unavailable; all evidence is objdump of the original.

Gate on entry and exit (full `check`, `RATCHET PASS`): `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,563** | **1,560** |
| instruction-index alignment | **449** | **460** |
| size | 2,764 | 2,764 |

The landed change is the wave-159 w10517 case order (`0x31,0x2c,0x33`);
it is the only shape that lowers the raw count, at a cost of 11 aligned
instructions.  The coordinator can revert this one hunk to restore
1,563 / 449 if the aligned number is preferred.

## 1. Landed change: `switch (type)` body order `0x31, 0x2c, 0x33`

The original's compare order is `0x33,0x31,0x2c` (c02b0/c02c0/c02c6) and
its case-body order is `0x2c,0x33,0x31` (c02cc/c02d2/c02e4).  clang emits
the compare order as a left-rotation of the source order, so the source
order `0x2c,0x33,0x31` (base) is the one that reproduces the original's
compare order, and `0x31,0x2c,0x33` is the one that reproduces the body
order starting with 0x31.  The two cannot be had together with a single
`switch` at 2,764 B.

All six orders were re-measured on the current body (full link):

| case order | raw | aligned | size |
|---|---:|---:|---:|
| **`0x31,0x2c,0x33` (landed)** | **1,560** | **460** | 2,764 |
| `0x31,0x33,0x2c` | 1,560 | 460 | 2,764 |
| `0x2c,0x33,0x31` (base) | 1,563 | 449 | 2,764 |
| `0x2c,0x31,0x33` | 1,563 | 449 | 2,764 |
| `0x33,0x31,0x2c` | 1,563 | 451 | 2,764 |
| `0x33,0x2c,0x31` | 1,563 | 451 | 2,764 |

The raw win is 3 bytes and lives entirely in the switch region
(function-relative 0x080-0x100: 62/54 diff bytes vs 63/56 for the base);
the rest of the function is byte-for-byte unchanged.  The `if/else-if`
chain coalesces to the identical body for every order (`ifchain` =
1,563/449; `ifchain` in `0x31`-first order = 1,560/460), so the chain
form is not an independent lever.

## 2. The mission test: the bitmask form is byte-identical but shifted

The original's mission test is the bitmask at c02fc-c0312:

```
sub.w r0,r8,#49 ; cmp r0,#7 ; bhi ; uxtb r0,r0 ; movs r1,#143
lsr.w r0,r1,r0 ; lsls r0,r0,#31 ; it ne ; movne.w r5,#270
```

The source form

```cpp
if ((unsigned) (mission - 49) < 4u || mission == 56) {
    hitpoints = 270;
}
```

reproduces that block **byte-identically** (measured; also
`switch (mission) { case 49..52, 56: }`).  But it lands at c02f8, not
c02fc, because the entry register map is 4 bytes shorter (see §3), so the
raw count is unchanged (1,563) and the aligned metric degrades to 524
(difflib mis-aligns the shifted block).  The base `if/else-if` form is
*not* byte-identical (it emits `cmp #56; bne; mov 270; b; sub; cmp #4;
it cc; movcc`), but it sits at the same relative offset as the rest of
the shifted body, which is why the aligned metric prefers it.

Other mission forms measured: `mission == 56 || (mission >= 49 &&
mission <= 52)` 1,576/454; two separate `if`s 1,576/454; `switch`
1,563/524; explicit `||` of the five values 1,563/524; ternary 1,563/449
(coalesces to base); reversed `else if` order 1,576/454.

## 3. Waypoint null-check arm: the original shares the `wz` store

Original (c023a-c025a, 13 instructions):

```
cbz r0, null
ldr.w r1,[r0,#292] ; str r1,[sp,#44]   ; wx
ldr.w r1,[r0,#296] ; ldr.w r0,[r0,#300]
str r1,[sp,#48]                         ; wy
b merge
null: movs r0,#0 ; str r0,[sp,#48]
      movs r0,#0 ; str r0,[sp,#44]
      movs r0,#0
merge: str r0,[sp,#40]                  ; wz (shared!)
```

Ours is 14 instructions: the `wz` store is duplicated in both paths
(`str r0,[sp,#40]` before the branch and `str r0,[sp,#40]` in the null
path), because the null path's store order is `wz,wx,wy` (ends on `wy`)
rather than the original's `wy,wx,wz` (ends on `wz`).  The null-store
order is fixed by the allocator, not by source: all of the following
coalesce to the identical 1,563/449 body:

- `int wy=0; int wx=0; int wz=0; if (wp) { wy=...; wx=...; wz=...; }`
  (base, and assignment orders `wx,wy,wz`, `wx,wy,wz` in an explicit
  `if/else`, `null`-first `if`, combined `int wy=0,wx=0,wz=0;`)
- all six ternary orders: `xzy` gives 1,563/449; `yzx`/`zyx` 1,564/452;
  `zxy` 1,564/451; `xyz`/`yxz` 1,619-1,621/453-455
- mixed forms (`wz` ternary + `wx,wy` in the `if`) 1,563/449
- `waypoint->getPosition()` (Vector) 2,585/573 at 2,864 B (wrong shape)

So the 2-byte waypoint-arm difference is a register-allocation
consequence of the same spill map as §4.

## 4. Register map (unchanged residual)

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
| `jitterX` | `[sp,#36]` | fp (r11) |
| `jitterY` | `[sp,#32]` | `[sp,#28]` |
| `jitterZ` | `[sp,#28]` | `[sp,#24]` |

The original keeps `shipClass` in a callee-saved register and spills
`jitterX`; ours does the reverse.  This is the 4-byte entry difference
(`str r1,[sp,#60]; mov fp,r3; mov r5,r2; ...; mov sl,r5` vs
`strd r1,r2,[sp,#56]; mov sl,r3`) and the source of every downstream
offset/register mismatch.  It is a PGO spill-weight outcome (wave
157/158/159 conclusion, re-confirmed here).

## 5. Race / shipClass lookup tables

The `switch (race)` capital-ship case order is already optimal.  Source
order `0,3,2,default,1` (base) gives 1,563/449; every other permutation
is worse (full link):

| race case order | raw | aligned |
|---|---:|---:|
| **`0,3,2,default,1` (base)** | **1,563** | **449** |
| `0,2,3,1,default` / `0,2,3,default,1` | 1,648 | 516 |
| `0,3,1,2,default` | 1,700 | 548 |
| `0,1,2,3,default` / `0,1,2,default,3` | 1,710 | 589 |
| `0,1,3,2,default` | 1,715 | 556 |
| `0,2,1,3,default` | 1,735 | 544 |

The `shipClass` creation `switch` remains size-illegal (2,776-2,780 B,
wave 157/158), and the cargo `switch (type)` coalesces to the base
`if (type == 0x2c || type == 0x31)` body (1,563/449).

## 6. Other new shapes measured this wave (all full link)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 1,563 | 449 | 2,764 |
| case order `0x31` first (landed) | 1,560 | 460 | 2,764 |
| mission bitmask `(unsigned)(m-49)<4 \|\| m==56` | 1,563 | 524 | 2,764 |
| mission `\|\|` of five values | 1,563 | 524 | 2,764 |
| mission two `if`s / reversed else-if | 1,576 | 454 | 2,764 |
| `hitpoints` multiply forms (implicit/compound/float-first) | 1,563 | 449 | 2,764 |
| `float mult` + `switch` (default 1.0) | 2,284 | 508 | 2,756 |
| `hitpoints = (int)(h * (options+0.5f))` | 1,564 | 452 | 2,764 |
| `hitpoints += (int)((options-0.5f)*h)` | 2,422 | 538 | 2,768 |
| `empTime` initialized to 15000 / `switch (shipClass)` | 1,563 | 449 | 2,764 |
| `empTime` ternary | 2,258 | 491 | 2,764 |
| cargo `switch (type)` / `if (0x31 \|\| 0x2c)` | 1,563 | 449 | 2,764 |
| cargo `if` reversed | 2,394 | 1,360 | 2,764 |
| `Vector pos` removed (pass `(float)x,y,z`) | 2,416 | 493 | 2,772 |
| `waypoint->getPosition()` | 2,585 | 573 | 2,864 |
| all-int coords (faithful, after `new Player`) | 2,482 | 901 | 2,788 |
| all-int coords before `new Player` | 2,433 | 852 | 2,772 |
| all-int + `expect(shipClass==0,1)` | 2,457 | 1,310 | 2,764 |
| coordinate type mixes (8) x mission x guard (32) | none 2,764 with raw < 1,563 |
| expect matrix (18) x guard | none 2,764 with raw < 1,563 |
| `switch (mission)` + level-clamp forms | 2,302-2,492 | 498-524 | 2,756-2,772 |

## 7. Conclusion

The type-switch body order is the only remaining source lever that moves
the raw count at exact size: it buys 3 raw bytes for 11 aligned
instructions.  The waypoint arm, the race switch and the shipClass
creation are already at their best source shape; the residual is the
PGO spill map (`type`<->`fp`, `shipClass`<->spilled, `jitterX`<->`fp`).

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10614
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10614/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb       # 1560
python3 /dev/shm/w10614/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb  # 460
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10614/`: `measure.py` (body splice + link + both
metrics), `batch*.py` (all variant generators), `fdiff.py`/`align_diff.py`
(worktree-pointed), `regions.py` (raw diff by 0x40 region), `sbs.py`
(aligned side-by-side), `fn.base.cpp` (pristine wave-159 body),
`Level.base.cpp` (pristine wave-159 file).
