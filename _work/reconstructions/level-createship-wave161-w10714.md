# `Level::createShip` — wave 161 (w10714): switch case order re-sweep,
# the mission range is a bitmask in the original, and the 4-byte
# register-allocation shift that blocks everything else

Worktree `wt/w10714`, base `master 01c7f92d6` (wave 160).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Ghidra
provider not used; all evidence is objdump of the original, which is
authoritative.

Gate on entry and exit (full `check`): `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0),
`.ARM.exidx` 0, `.rodata`/`.data`/`.bss` 0.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,563** | **1,560** |
| instruction-index alignment | **449** | **460** |
| size | 2,764 | 2,764 |
| `.text` differing bytes | 85,518 | 85,515 |

## 1. Landed change: `switch (type)` case order `0x31, 0x2c, 0x33`

The base source already has the original's *case-body* order
(`0x2c, 0x33, 0x31`, at c02cc/c02d2/c02e4) and clang already emits the
original's *compare* order (`0x33, 0x31, 0x2c` at c02b0/c02c0/c02c6).
The switch is therefore structurally correct; the only residual is the
4-byte shift (section 3).  All six source permutations were re-measured
(full link, size exact in every case):

| case order | raw | aligned |
|---|---:|---:|
| **`0x31, 0x2c, 0x33` (landed)** | **1,560** | **460** |
| `0x31, 0x33, 0x2c` | 1,560 | 460 |
| `0x2c, 0x33, 0x31` (base) | 1,563 | 449 |
| `0x2c, 0x31, 0x33` | 1,563 | 449 |
| `0x33, 0x31, 0x2c` | 1,563 | 451 |
| `0x33, 0x2c, 0x31` | 1,563 | 451 |

The `if/else if` chain form lowers identically for every order (same
numbers).  The landed order is a raw win of 3 bytes at the cost of 11
aligned instructions; the aligned metric is not ratcheted and the raw
metric defines `byte_exact`, so the change is gate-legal.  If the
coordinator prefers the aligned number, reverting this one hunk
restores 1,563 / 449.

## 2. New finding: the original's mission range is a bitmask

Original c02fc-c0312:

```
sub.w r0, r8, #49
cmp   r0, #7
bhi.n ...
uxtb  r0, r0
movs  r1, #143          ; 0b10001111 = bits 0,1,2,3,7
lsr.w r0, r1, r0
lsls  r0, r0, #31
it    ne
movne.w r5, #270
```

`143 >> (mission - 49)` selects exactly `{49,50,51,52,56}`.  The
original source therefore combines the two arms into one condition:

```cpp
if ((mission >= 49 && mission <= 52) || mission == 56)
    hitpoints = 270;
```

The current source (`if (mission == 56) ... else if (mission >= 49 &&
mission <= 52) ...`) emits two separate checks (24 B, at c02f8-c030e).
The single-condition form reproduces the original's bitmask
byte-for-byte (24 B, c02f8-c030e) — but because the block sits at the
4-byte shift, the raw count does **not** move (1,563) and the greedy
instruction alignment worsens to 524.  Two-separate-`if` forms give a
shorter 16-B two-check sequence (raw 1,576 / aligned 454).

Measured (full link, all size 2,764):

| mission form | raw | aligned |
|---|---:|---:|
| base `if/else if` | 1,563 | 449 |
| single condition (bitmask) | 1,563 | 524 |
| two separate `if`s | 1,576 | 454 |
| bitmask + `__builtin_expect(...,0/1)` | 1,576 | 454 |

Not landed: it is a real correctness fix but does not improve the raw
metric, and the task requires both metrics not to regress.

## 3. The blocker: a 4-byte register-allocation shift

Original entry (c0220-c023a):

```
str   r1, [sp, #60]     ; race spilled
mov   r9, r0            ; this
mov   fp, r3            ; type -> fp
mov   r5, r2            ; shipClass -> r5
...
mov   sl, r5            ; shipClass -> sl (r5 needed for &AERandom)
```

Ours (c0220-c0238):

```
strd  r1, r2, [sp, #56] ; race AND shipClass spilled
mov   r9, r0
mov   sl, r3            ; type -> sl
```

The original keeps `shipClass` in a callee-saved register
(`r5 -> sl -> r6`) and spills only `race`; ours spills both and keeps
`type` in `sl` with `jitterX` temporarily in `fp`.  The missing
`mov r5, r2` + `mov sl, r5` are exactly the 4 bytes that shift every
instruction after the waypoint arm, which is why the (already correct)
switch, mission bitmask and waypoint arm still count as raw diffs.

Register map (original vs ours):

| value | original | ours |
|---|---|---|
| `race` | `[sp,#60]` | `[sp,#56]` (strd) |
| `shipClass` | r5 -> sl -> r6 | `[sp,#60]` (strd) |
| `type` | fp (r11) | sl (r10) |
| `jitterX` | `[sp,#36]` | fp (r11) |
| `jitterY` | `[sp,#32]` | `[sp,#28]` |
| `jitterZ` | `[sp,#28]` | `[sp,#24]` |
| `x` | `[sp,#44]` | `[sp,#44]` |
| `y` | `[sp,#48]` | `[sp,#36]` |
| `z` | `[sp,#40]` | `[sp,#40]` |

The allocator gives the second long-lived callee-saved slot to
`jitterX` in ours and to `shipClass` in the original.  The only
source-level lever found in wave 159 (all-`int` coordinates computed
before `new Player`) does flip `type` into `fp`, but costs +28 B
(2,792) and still spills `shipClass`.

## 4. Waypoint arm

The if/else form with `wy, wx, wz` (wave-10417) is correct.  Original
null arm stores `y([48]), x([44]), z([40])`; ours stores
`z([40]), x([44]), y([36])` — the `y` slot differs only because
`jitterX` occupies `[36]` in the original but `fp` in ours.  All six
declaration orders were re-swept with the landed case order:

| decl order | raw | aligned |
|---|---:|---:|
| **`y,x,z` (landed/base)** | **1,560** | **460** |
| `x,y,z` / `x,z,y` / `y,z,x` | 1,561 | 462-463 |
| `z,x,y` | 1,619 | 464 |
| `z,y,x` | 1,617 | 466 |

Ternary forms (`waypoint ? waypoint->y : 0`) give 1,619/455; a pointer
test instead of `!= nullptr` and a single `int wy=0,wx=0,wz=0;` line
are code-identical to the base.

## 5. Other shapes measured this wave (all full link unless noted)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| base | 2,764 | 1,563 | 449 |
| all-`int` coords before `new Player`, no null check | 2,792 | 2,413 | 475 |
| all-`int` coords, no null check | 2,792 | 2,386 | 460 |
| all-`int` coords + `expect(shipClass==0,1)` | 2,764 | 2,457 | 1,310 |
| drop `if (Globals::status == nullptr) x = 0;` | 2,764 | 2,055 | 494 |
| `expect(shipClass==0,1)` on float base | 2,744 | 2,475 | 1,299 |
| `expect(waypoint != nullptr, 1)` | 2,764 | 2,315 | 497 |
| `expect(mission == 0x9a, 0)` | 2,772 | 2,422 | 545 |
| `int empTime = shipClass==1 ? 45000 : 15000` | 2,764 | 2,258 | 491 |
| `int cls = shipClass` used everywhere | 2,764 | 1,563 | 449 (coalesced) |
| `int r/c/t = race/shipClass/type` copies | 2,764 | 1,563 | 449 (coalesced) |
| `Player *player` line moved / coord placement sweep | 2,764 | 1,563 | 449 |

The `if (Globals::status == nullptr) x = 0;` is spurious (the original
computes `x` unconditionally) but is load-bearing for the register map:
removing it costs 492 raw bytes.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10714
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10714/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb       # 1560
python3 /dev/shm/w10714/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb  # 460
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10714/`: `m.py` (splice + compile-only size + entry
map), `screen.py` (compile-only size/`fp=type` screen), `run.py`
(splice + link + both metrics), `fdiff.py`/`align_diff.py`/
`region.py`/`sbs.py` (worktree-pointed copies), `gen2.py`..`gen11.py`
(variant generators), `Level.orig.cpp` (pristine wave-160 source),
`body.base.cpp` (pristine body), `orig.txt`/`ours.txt` (full
disassembly).
