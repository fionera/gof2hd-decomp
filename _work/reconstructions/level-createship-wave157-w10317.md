# `Level::createShip` — wave 157 (w10317): the type/shipClass/race
# register permutation is a PGO spill-weight outcome; 40 new source shapes
# all coalesce to 1564 / 452 at 2,764 B (no source win)

Worktree `wt/w10317`, base `master 08575efff` (wave 156).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

| metric | base (landed) |
|---|---:|
| raw same-address diff | **1,564** |
| instruction-index alignment | **452** |
| size | 2,764 |

**No source change landed** (base is already the best on both metrics).
This note records the register map and the 40 measured variants so the next
wave does not re-run them.

## 1. Register map (the residual)

Entry, original vs ours (same 2,764 B function):

```
orig                              ours
c0220 str  r1, [sp,#60]   race   0x0c strd r1, r2, [sp,#56]  race+shipClass
c0222 mov  r9, r0         this   0x10 mov  r9, r0             this
c0224 ldr  r0, [pc]              0x12 ldr  r0, [pc]
c0226 mov  fp, r3         type   0x14 mov  sl, r3             type
c0228 mov  r5, r2         shipCl  (-- no mov, shipClass spilled --)
c022a add  r0, pc                0x16 add  r0, pc
c022c ldr  r4, [r0]       status 0x18 ldr  r4, [r0]           status
c022e ldr  r0, [r4]              0x1a ldr  r0, [r4]
c0230 blx  getCurrentCampaignMission
c0234 mov  r8, r0         mission 0x20 mov r8, r0             mission
c0236 ldr  r0, [r7,#8]    waypoint 0x22 ldr r0, [r7,#8]       waypoint
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

The original keeps **shipClass** in a callee-saved register (sl, later moved
to r6 when sl is needed for `empPoints`) and spills **race** and all three
jitters.  Ours keeps **type** in sl and **jitterX** in fp and spills both
race and shipClass (paired `strd`).  Because `type` takes sl, fp is free and
jitterX lands there; the whole downstream register assignment (r4/r6/r8/fp/
sl rotation in the BoundingAAB loops) follows from that one choice.

The original's `mov r6, sl` at c02b4 is the allocator freeing sl for
`empPoints`; it does not exist in ours because shipClass is already spilled.
This single copy is the visible tip of the permutation.

## 2. Why the mission test switch does not help the raw metric

The wave-156 `switch (mission) { case 49..52, 56: }` form emits the original
bitmask **byte-identically**:

```
orig c02fc: f1a8 0031  sub.w r0,r8,#49     ours 0x0e4: f1a8 0031
     c0300: 2807       cmp   r0,#7               0x0e8: 2807
     c0302: d807       bhi.n                     0x0ea: d807
     c0304: b2c0       uxtb  r0,r0               0x0ec: b2c0
     c0306: 218f       movs  r1,#143             0x0ee: 218f
     c0308: fa21 f000  lsr.w r0,r1,r0            0x0f0: fa21 f000
     c030c: 07c0       lsls  r0,r0,#31           0x0f4: 07c0
     c030e: bf18       it    ne                  0x0f6: bf18
     c0310: f44f 7587  movne.w r5,#270           0x0f8: f44f 7587
```

but it is at **0x0e4**, whereas the original's is at **0x0e8** (the original
has `vmov r5,s0` at 0x0e4; ours has it at 0x0e0).  The 4-byte cumulative
offset shift from the entry register permutation means the byte-identical
block does not land on the same addresses, so the raw diff is unchanged:
**every region of the function has exactly the same raw diff count for the
`switch` form and the `if/else-if` form** (0x000-0x120: 189 both,
0x120-0x200: 217 both, 0x200-0x540: 782 both, 0x540-0x880: 79 both,
0x880-0x9c0: 288 both, 0x9c0-0xac0: 8 both).  The switch form only moves
the aligned count 452 -> 527 (trailing literal-pool decode), so it is worse
on the stated metric.

## 3. Variants measured this wave (all full link, clang 7.0.2 `-Oz`)

Every size-2,764 variant below reproduces the **identical** 1564 / 452 body
(byte-for-byte identical `.text` for the function), i.e. the change
GVN/coalesces away or does not perturb the allocator:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| **base (if/else-if mission)** | **2,764** | **1,564** | **452** |
| `int sc = shipClass;` (top / before empTime / `(void)shipClass`) | 2,764 | 1,564 | 452 |
| `const int` params | 2,764 | 1,564 | 452 |
| `int r2 = race;` used everywhere | 2,764 | 1,564 | 452 |
| `int ty = type;` (switch only / switch+type==14) | 2,764 | 1,564 | 452 |
| `int ty2 = type;` used only in the ship branches | 2,764 | 1,564 | 452 |
| `switch (shipClass) { case 1: ... default: }` for empTime | 2,764 | 1,564 | 452 |
| `empPoints` as one expression | 2,764 | 1,564 | 452 |
| `1 == shipClass` (yoda) | 2,764 | 1,564 | 452 |
| `do { entry } while (0)` / `do { whole body } while (0)` / `while(1){...break;}` | 2,764 | 1,564 | 452 |
| `x = Globals::status != nullptr ? x : 0;` | 2,764 | 1,564 | 452 |
| `Status *st = Globals::status; if (st==nullptr) x=0;` | 2,764 | 1,564 | 452 |
| `__builtin_expect(type == 0x33, 0)` on the 0x33 case | 2,764 | 1,564 | 452 |
| `__builtin_expect(type == 0x2c \|\| type == 0x31, 0)` (cargo) | 2,764 | 1,564 | 452 |
| `__builtin_expect(missionPtr != 1 && missionPtr != 23, 1)` | 2,764 | 1,564 | 452 |
| reorder `switch(type)` cases to 0x33,0x31,0x2c | 2,764 | 1,564 | **454** |

Size-changing variants (all regress size, so gate-illegal):

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `switch (shipClass)` ship creation (with/without default) | 2,780 | 2,491 | 488 |
| `expect(shipClass == 1, 0)` (empTime) | 2,768 | 2,417 | 514 |
| `expect(shipClass == 1, 1)` (empTime, hot) | 2,768 | 2,114 | 503 |
| `expect(shipClass == 0, 1)` (fighter, hot) | 2,744 | 2,476 | 1,302 |
| `expect(shipClass == 0, 0)` (fighter, cold) | 2,712 | 2,430 | 1,413 |
| both shipClass expects | 2,748 | 2,520 | 1,316 |
| `expect(mission == 0x9a, 0)` | 2,772 | 2,423 | 544 |
| `expect(mission == 56, 0)` | 2,768 | 2,144 | 500 |
| `switch(mission)` + `expect(shipClass==0,1)` | 2,744 | 2,476 | 1,291 |
| `int armor` before `switch(type)` | 2,796 | 2,499 | 528 |
| ternary empTime | 2,768 | 2,347 | 545 |
| `switch(mission)` (bitmask) | 2,764 | 1,564 | 527 |
| `switch(mission)` + both shipClass expects | 2,748 | 2,520 | 1,305 |
| two separate `if (shipClass==0)` / `if (shipClass==1)` | 2,704 | 2,504 | 1,394 |
| `switch(type)` -> if-chain 0x33/0x31/0x2c | 2,764 | 2,395 | 1,370 |
| `volatile int sc = shipClass;` | 2,828 | 2,519 | 1,005 |
| `switch(mission)` + guard before `setEmpData` | 2,776 | 2,417 | 492 |
| guard before `setEmpData` | 2,776 | 2,417 | 503 |

## 4. New structural finding: the coordinate computation

The original keeps `x`, `y`, `z` as **ints** in fp/r4/r8 and converts them
with `vcvt.f32.s32` *inside each ship-construction branch* (capital at
c040c-c0422, fighter at c06c0-c06d8).  Our source uses
`float z = (float)(wz + jitterZ - 20000); float y = ...; int x = ...;`,
which converts z/y early (0x157) and keeps only x as an int.  Making all
three ints (`int x,y,z` in any order, with or without the status guard)
gives **2,780-2,800 B** (+16..+36), so it is size-illegal on its own.  The
original evidently has some other -16..-36 B shape that our source lacks.

The `if (Globals::status == nullptr) { x = 0; }` guard added in wave 153 is
a **size filler**, not faithfulness: the original computes
`x = wx + jitterX - 20000` unconditionally at c03e4.  Removing the guard
keeps 2,764 B but worsens raw to **2,056** / aligned **494**, so the guard
is kept.

## 5. Conclusion

The type/shipClass/race permutation is the PGO spill-weight choice: the
original's hot-block frequencies make `shipClass` outrank `type` for the
low callee-saved register, so shipClass keeps sl/r6, type takes fp, and
jitterX spills.  Our static (non-PGO) build ranks `type` first, takes sl,
and the free fp goes to jitterX.  No source spelling tried (locals, const,
switch, do/while, expect, declaration order, coordinate types, guard
placement) changes that ranking at 2,764 B; every one either coalesces to
the identical body or changes the size.  The landed body remains the best
on both metrics.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10317
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10317/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb   # 1564
python3 /dev/shm/w10317/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb  # 452
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10317/`: `run.py` (body splice + build + both metrics),
`gen.py`..`gen8.py` (variant generators, `v_*.cpp` bodies), `sbs.py`
(aligned side-by-side), `rawmap.py`/`regiondiff.py`/`hexdump.py` (raw diff
by region), `Level.orig.cpp` (pristine source).
