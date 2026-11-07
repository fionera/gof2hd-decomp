# `Level::createShip` — wave 128 (w7626): machine sinking / int-coordinate RA study (no source win)

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `4bbe88776` (wave-127 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7626`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.**  The checked-in body stays
the best at **70.5415 %** (official rounded 70.5), 2764 B, 1007/1006 insns,
exact 336 B / 57-row / `ttype 0xff` cleanup-only LSDA.  New result: the
original's `x`/`y`/`z` are **ints** kept in `fp`/`r4`/`r8` and converted to
floats only at each ctor call; the int spelling is *worse* in our build
(61.5 %, 2772 B) because the scheduler **sinks** the coordinate block below
`setEmpData` instead of leaving it above it like the original.  The RA-time
block order (not the source spelling) is what decides the exception-pointer
register.

## Result

```
                                   original        HEAD (4bbe88776)
createShip fuzzy (official)        —               70.5
createShip fuzzy (raw ratio)       —               70.5415
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA landing pads       28              28
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. Ground truth: the original keeps x/y/z as ints

Original (`0xc03c8`..`0xc03ee`), after `new Player` and before `setEmpData`:

```
c03c8  ldr   r2,[sp,#48]   ; wy
c03ca  ldr   r3,[sp,#32]   ; jitterY
c03cc  ldr   r0,[sp,#44]   ; wx
c03ce  ldr   r1,[sp,#36]   ; jitterX
c03d0  add   r2,r3         ; wy+jitterY
c03d2  ldr   r3,[sp,#40]   ; wz
c03d4  ldr   r4,[sp,#28]   ; jitterZ
c03d6  add   r0,r1         ; wx+jitterX
c03d8  movw  r1,#20000
c03dc  sub.w fp,r0,r1      ; x (int) -> fp
c03e0  add   r3,r4         ; wz+jitterZ
c03e2  subs  r4,r2,r1      ; y (int) -> r4
c03e4  sub.w r8,r3,r1      ; z (int) -> r8
c03e8  mov   r0,r5         ; player
c03ea  mov   r1,sl         ; empPoints
c03ec  mov   r2,r9         ; empTime
c03ee  blx   setEmpData
```

The ints are converted to floats later, once per consumer, in **x, y, z**
order:

```
; PlayerFixedObject ctor (c040c..c0422)
c040c  vmov  s0,fp   ; x
c0412  vcvt  s20,s0
c0416  vmov  s0,r4   ; y
c041a  vcvt  s18,s0
c041e  vmov  s0,r8   ; z
c0422  vcvt  s16,s0
; PlayerFighter ctor (c06c0..c06d8)
c06c0  vmov  s0,fp   ; x
c06c6  vmov  s2,r8   ; z
c06cc  vmov  s4,r4   ; y
c06d0  vcvt  s0,s0   ; x
c06d4  vcvt  s2,s2   ; z
c06d8  vcvt  s4,s4   ; y
```

The stack-slot map is `sp28=jitterZ, sp32=jitterY, sp36=jitterX, sp40=wz,
sp44=wx, sp48=wy, sp52=type, sp56=this, sp60=race`.

## 2. What was tried this pass (raw ratio, full link)

| variant | raw % | size | insns | note |
|---|---:|---:|---:|---|
| **HEAD: `float z, float y, int x` after `new Player`** | **70.5415** | 2764 | 1007/1006 | best |
| `int x, int y, int z` after `new Player` | 61.5461 | 2772 | 1007/998 | coords sink below setEmpData |
| `int x, int y, int z` before `new Player` | 61.5461 | 2772 | 1007/998 | still sinks below setEmpData |
| `int x,y,z` + `if (x==x && y==y && z==z) setEmpData` | 61.5461 | 2772 | 1007/998 | fold removes the guard, still sinks |
| `float z, float y, int x` before `new Player` | 62.8429 | 2768 | 1007/998 | worse |

The int spelling is the faithful form but the pre-RA scheduler moves the
coordinate block **below** `setEmpData` (to `0xc03cc`..`0xc03e8` in our build,
vs `0xc03c8`..`0xc03e4` in the original), so the whole downstream register
permutation differs.

## 3. The RA root cause is the RA-time block order, not the source

`-mllvm -print-machineinstrs` on the HEAD body shows the pre-RA (NoPHIs)
block order has the landing pads **interleaved** with the main body:

```
bb.0 .. bb.17 (main)  bb.19 (lp: new Player)  bb.20 .. bb.26
bb.27 (lp: new PlayerFighter)  bb.28 .. bb.40
bb.41 (lp: new PlayerFixedObject)  bb.42 (lp: new Array)
bb.43 .. bb.55  bb.56..bb.66 (24 BoundingAAB lps)  bb.67 .. bb.99
bb.100 (resume PHI)
```

The exception-pointer PHI at `bb.100` has 28 operands, one per landing pad
(`%31` from `bb.19`, `%45` from `bb.41`, ...).  Because `bb.19` is early, the
coalesced PHI value is live across almost the whole function, so Greedy RA
gives it a callee-saved register that is free for that whole span.  It spills
`type` to free **r6** and uses r6 for the exception pointer:

```
c0bbe  mov r6,r0        ; landing pad (volumes)
c0bc6  mov r6,r0        ; landing pad (fixed)
c0bcc  mov r6,r0        ; landing pad (player)
c0bd4  mov r0,r6
c0bd6  blx _Unwind_Resume
```

The original uses **r8** for the exception pointer (r8 is free there because
the int z died at the end of the body):

```
c0bba  mov r8,r0
c0bc0  mov r8,r0
c0bc6  mov r8,r0
c0bcc  mov r8,r0
c0bd4  mov r0,r8
c0bd6  blx _Unwind_Resume
```

The **final** code order is identical in both (landing pads at the end,
`bb.131`..`bb.160`), but that reorder happens *after* RA.  The original's RA
must have seen the landing pads at the end.  The difference is therefore in
the IR/CFG shape that feeds `MachineBlockPlacement`, not in the coordinate
spelling.

## 4. LSDA call-site table is structurally identical

Both records are 336 B, `lpStart=0xff`, `ttype=0xff`, `callSite=0x01`,
57 rows, 28 rows with a landing pad.  The landing-pad offsets are the same
range (original 2422..2488, ours 2426..2488); only the call offsets differ by
the 4-byte prologue shift.  So the EH structure is not the lever.

## 5. Remaining lever

A source shape that makes clang emit the cleanup landing pads at the **end of
the IR block list** (so the RA sees them after the BoundingAAB loop) while
keeping the cleanup-only `ttype 0xff` LSDA.  The int-coordinate spelling is
correct but insufficient: the scheduler sinks it below `setEmpData`.  A plain
`try`/`catch` moves the pads to the end but changes the LSDA to `ttype 0x00`.

## 6. Reproduction

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

MIR (RA-time block order and PHI live range):

```
<clang++> ... -c src/game/world/Level.cpp \
  -mllvm -print-machineinstrs -mllvm -filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w7626_*` (exact-ratio scorer,
LSDA call-site parser, disassembly dumps).
