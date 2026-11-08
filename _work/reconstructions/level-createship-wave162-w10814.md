# `Level::createShip` — wave 162 (w10814): register-map sweep, the
# int-coordinate shape, and the landed case-order raw win

Worktree `wt/w10814`, base `master 29248c1d0` (wave 161).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Ghidra
provider not used; all evidence is objdump of the original, which is
authoritative.

Gate on entry and exit (full `check`): `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,563** | **1,560** |
| instruction-index alignment | **449** | **460** |
| size | 2,764 | 2,764 |

## 1. Landed change: `switch (type)` case order `0x31, 0x2c, 0x33`

This is the w10714 measured variant (wave 161 left it unlanded).  The
original's case-body order is `0x2c, 0x33, 0x31` at c02cc/c02d2/c02e4 and
its compare order is `0x33, 0x31, 0x2c`; the base source has the
case-body order `0x2c, 0x33, 0x31` (same as the original).  Reordering
the *source* cases to `0x31, 0x2c, 0x33` moves the three `vmov`/`vldr`
constant-pool loads into the original's order and is a raw win of 3
bytes at a cost of 11 aligned instructions.  The aligned metric is not
ratcheted; the raw metric defines `byte_exact`.

## 2. The 4-byte register-allocation shift (not fixed)

Original entry (c0220-c023a):

```
str   r1, [sp, #60]     ; race spilled
mov   r9, r0            ; this
ldr   r0, [pc, #572]
mov   fp, r3            ; type -> fp
mov   r5, r2            ; shipClass -> r5
add   r0, pc
ldr   r4, [r0, #0]
ldr   r0, [r4, #0]
blx   getCurrentCampaignMission
mov   r8, r0            ; mission
ldr   r0, [r7, #8]      ; waypoint
mov   sl, r5            ; shipClass -> sl
cbz   r0, ...
```

Ours (base):

```
strd  r1, r2, [sp, #56] ; race AND shipClass spilled
mov   r9, r0
ldr   r0, [pc, #576]
mov   sl, r3            ; type -> sl
...
cbz   r0, ...
```

Register map (original vs base):

| value | original | base |
|---|---|---|
| `race` | `[sp,#60]` | `[sp,#56]` (strd) |
| `shipClass` | r5 -> sl -> r6 | `[sp,#60]` (strd) |
| `type` | fp (r11) | sl (r10) |
| `this` | r9, spilled at c0332 | r9, spilled at c0332 |
| `jitterX` | `[sp,#36]` (spilled at c026e) | fp (r11), spilled at c0318 |
| `jitterY` | `[sp,#32]` | `[sp,#28]` |
| `jitterZ` | `[sp,#28]` | `[sp,#24]` |
| `x` | `[sp,#44]` | `[sp,#44]` |
| `y` | `[sp,#48]` | `[sp,#36]` |
| `z` | `[sp,#40]` | `[sp,#40]` |

The allocator gives the second long-lived callee-saved slot to `jitterX`
in ours and to `shipClass` in the original.  Pre-RA live intervals
(`-mllvm -print-after-all`, `bb.0`):

| vreg | value | live range (slot units) | uses |
|---|---|---:|---:|
| `%130` | type | 16 .. 26192 | 14 |
| `%129` | shipClass | 32 .. 6992 | 3 |
| `%128` | race | 48 .. 26032 | 8 |
| `%127` | this | 64 .. 26528 | 4 |
| `%138` | mission | 192 .. 3584 | 5 |
| `%147` | jitterX | 720 .. 4688 | 1 |
| `%149` | jitterY | 832 .. 4528 | 1 |
| `%151` | jitterZ | 944 .. 4576 | 1 |

The greedy allocator's queue is ordered by spill weight (~uses / live
range length).  `type` (14/26176) beats `shipClass` (3/6960), so `type`
takes `sl` and `shipClass` is spilled.  In the original `shipClass`
outranks `type`; that can only come from the original's PGO branch
weights (the task context's "original was built with branch weights")
discounting `type`'s uses in the cold capital-ship branch, or from a
different IR shape.  No source-level spelling tested so far flips it at
2,764 B.

## 3. New hypotheses measured this wave (all full link)

Every variant below is size-exact at 2,764 B unless noted.  "entry map"
is the first 10 instructions.

| variant | raw | aligned | entry map |
|---|---:|---:|---|
| base | 1,563 | 449 | `strd r1,r2[56]; mov r9,r0; mov sl,r3` |
| **case order `0x31,0x2c,0x33` (landed)** | **1,560** | **460** | same as base |
| case order + bitmask mission | 1,560 | 535 | same as base |
| bitmask mission only | 1,563 | 524 | same as base |
| `int`/`unsigned` locals for shipClass/race (`sc`, `rc`, `sc+rc`) | 1,563 | 449 | coalesced, identical |
| `Waypoint &wp = *waypoint` / `Waypoint *wp = waypoint` | 1,563 | 449 | coalesced, identical |
| `if (waypoint)` instead of `!= nullptr` | 1,563 | 449 | identical |
| waypoint block in its own `{}` scope | 1,563 | 449 | identical |
| `int wy=0,wx=0,wz=0;` one line | 1,563 | 449 | identical |
| `int jitterX, jitterY, jitterZ;` then assign | 1,563 | 449 | identical |
| `int jitter[3]` array | 1,563 | 449 | identical |
| jitter declarations reversed | 1,563 | 449 | identical |
| `mission` in its own `{}` scope | 1,563 | 449 | identical |
| jitter block in its own `{}` scope | 1,563 | 449 | identical |
| `int capital = shipClass == 1;` early | 1,563 | 449 | identical |
| `+ shipClass * 0` in hitpoints | 1,563 | 449 | identical |
| `expect(status==nullptr, 0/1)` | 1,563 | 449 | identical |
| `int empTime = 15000;` at top, no else | 1,563 | 449 | identical |
| `if (shipClass < 1)` for the `==0` branch | 1,568 | 450 | same as base |
| waypoint ternary init | 1,619 | 455 | same as base |
| `int empTime = shipClass==1?45000:15000` at top | 2,488 | 519 | shipClass spilled, this->sl |
| `empTime` ternary (single init + if) | 2,258 | 491 | shipClass spilled, this->sl |
| `if (shipClass != 1)` for the `==0` branch | 2,399 | 1,117 | **race spilled, shipClass->fp, type->sl, this->r5** (size 2,712) |
| `expect(shipClass==0, 0)` | 2,429 | 1,410 | **this spilled, shipClass->fp, race->sl, type->r9** (size 2,712) |
| `expect(shipClass==0, 1)` | 2,475 | 1,299 | base map (size 2,744) |
| `expect(shipClass==1, 0)` | 2,297 | 652 | shipClass spilled, race->[24], type->sl (size 2,756) |
| swap branch order (`==1` first) | 2,377 | 698 | **type->fp, shipClass spilled** (size 2,796) |
| coords computed after the jitters | 2,490 | 543 | (size 2,776) |
| coords computed after armor | 2,456 | 503 | (size 2,780) |

## 4. The int-coordinate shape (key new finding)

The original computes `x`, `y`, `z` as **ints** and converts to float at
each call site (c03c8-c03e4 compute `fp/r4/r8`; c040c-c0422 convert for
`PlayerFixedObject`; c06c0-c06d8 convert again for `PlayerFighter`).
The base source declares `float z, y; int x`, so it converts `y`/`z`
early (c03e2/c03f0) and only `x` at the call sites.

Switching to `int x, y, z` **does** move `shipClass` into a callee-saved
register at entry:

```
str   r3, [sp, #60]     ; type spilled
mov   r9, r0
ldr   r0, [pc]
mov   sl, r2            ; shipClass -> sl
mov   r4, r1            ; race -> r4
...
str   r4, [sp, #56]     ; race spilled
```

but it grows the function to 2,788 B (`int` coords, two-check mission,
status check), 2,792 B (`int` coords + bitmask + no status check) or
2,800 B (`int` coords + bitmask + status check).  The extra bytes are
the duplicated `vmov`/`vcvt` conversions in both branches plus a
different spill set.  The original is 2,764 B with int coords, so the
original's register allocation must save ~24 B relative to ours — i.e.
the int-coord shape is correct but it only reaches 2,764 B once the
callee-saved map (shipClass in a register, jitterX spilled) is also
reproduced.  This is the strongest lead for the next wave: get the
allocator to spill `jitterX` at c026e as the original does, and the
int-coord shape should collapse back to 2,764 B.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10814
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10814/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb       # 1560
python3 /dev/shm/w10814/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb  # 460
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10814/`: `run.py` (splice body + header sig + build +
both metrics), `build.sh` (compile Level.cpp + link + measure),
`body.base.cpp` / `Level.base.cpp` / `Level.base.h` (pristine wave-161
source), `entrymap.sh` (first 20 entry instructions), `sbs.py`
(instruction side-by-side), `bytes.py` (byte-aware diff), `batch1.py`
.. `batch10.py` (variant generators), `orig.txt` / `ours.txt`.
