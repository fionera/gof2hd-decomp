# ModStation ctor — wave 106 (w5419): barrier placement fixes the yaw literal order

Status: landed. Worktree `wt/w5419`, branch `wave106-w5419`, base `486f8a119`.
Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B.

## Result

```text
before: raw 98.351648 %, pct 98.4, 39 differing bytes in 8 runs
after:  raw 98.626374 %, pct 98.6, 42 differing bytes in 7 runs
size:   1008 / 1008 (unchanged)
gate:   verify byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
        data exact 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The byte-run count fell from 8 to 7 even though the raw byte count rose by 3:
the three literal-pool words moved from "different target" to "same target,
different PC-relative offset" (the pool values now differ by 2 because the
`add rX, pc` instructions sit at different addresses). The instruction stream
now has the correct literal order and a much shorter residual.

## What changed

`src/game/menu/ModStation.cpp`, `ModStation::ModStation()`:

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    // Compiler barrier only (no instruction emitted).  Placed after the table-pointer
    // setup so the two yaw-table address materialisations keep the original's order
    // (YawIPad before Yaw) and are scheduled ahead of the volatile pitch load.
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
```

Before, the fence was the first statement of the block (before both table
pointers).  Moving it *after* the `yawTable` select is the whole change.

## Evidence

Original near-camera rotation block (objdump, `0xd5436`):

```text
ldr r0, [pc,#632]   ; gHangarCamPitch
ldr r3, [pc,#632]   ; gHangarCamYawIPad
ldr r4, [pc,#636]   ; gHangarCamYaw
add r0, pc
add r3, pc
ldrb.w r1, [sl]     ; Globals::iPad
add r4, pc
ldr.w r2, [r0,r8,lsl#2]   ; pitch
mov r0, r3
cmp r1, #0
str r3, [sp,#180]
str r4, [sp,#176]
it eq
moveq r0, r4
ldr.w r3, [r0,r8,lsl#2]   ; yaw
...
```

The literal pool at `0xd56b0` is `{0x128bf0, 0x128c16, 0x128c38}` =
`{Pitch, YawIPad, Yaw}`.  The old source emitted `{Pitch, Yaw, YawIPad}`.
The pool slot order follows the LDRLIT emission order, which the pre-RA
`list-hybrid` scheduler derives from the DAG (not from the select operand
order: rewriting the IR select to the non-canonical `select ne, YawIPad, Yaw`
and recompiling the `.ll` directly does **not** change the LDRLIT order).

With the fence after the `yawTable` select, the backend emits the original
order.  The fence is a pure compiler barrier (`fence syncscope("singlethread")`
in IR) and emits no instruction.

## Measured variants (compile-only screen, then full link+verify for the best)

All permutations of `{pitchTable decl A, yawTable decl B, pitchBits load C,
yawBits load D}` with one fence in any of the five gaps were compiled to `.s`
and the LDRLIT order read back:

```text
order Pitch,Yaw,YawIPad (old, wrong):  ABCD fence@0/@1/@3, BACD fence@0/@3
order Pitch,YawIPad,Yaw (target):      ABCD fence@2/@4, BACD fence@1/@2/@4
order Yaw,... (pitch not first):       ABDC, BADC, BDAC (all fence positions)
```

The target-order variants split into two register-allocation families:
`ABCD f2/f4` and `BACD f1/f2/f4` keep the original allocation (`r8 = race`,
`sl = iPad pointer`, `r4` = pitch at the call); `ACBD` (pitch load before the
select) moves `race` to `r4` and drops the function to 88.6 % / 1000 B.
`ABCD f2` (landed) was the smallest source change.

Variants tried and rejected (same or worse):

```text
pitch load before the yaw select (ACBD)        88.6 %, 1000 B
yaw load before the pitch load (ABDC/BDAC)     different schedule
two fences (before A and after B)              ldrb scheduled first
volatile read of Globals::iPad                 same schedule
if/else instead of ternary                     same schedule (if-converted)
static inline pickYawTable() helper            same schedule (inlined)
const volatile float* + float loads            vldr codegen, worse
fence memory order ACQUIRE/RELEASE/ACQ_REL     same schedule
fence memory order RELAXED                     wrong literal order
```

## Remaining residual (hypothesis)

The 42 differing bytes are the near-block instruction schedule only:

```text
+0x16a..+0x179 16B   ldr r4 (Yaw) / add r0 / add r3 / ldrb / add r4 / ldr r2 order
+0x186..+0x19e 23B   tail: original emits the yaw load before str r8 / mov r4,r2
+0x3e0/+0x3e4/+0x3e8 3B  pool words differ only by the shifted add-PC base
```

The original groups the three LDRLITs before the adds; our build interleaves
`add r0` and `ldrb` before the third LDRLIT, and hoists `str r8` / `mov r4,r2`
above the yaw load.  This is a `list-hybrid` scheduler tie-break (height/depth/
`closestSucc`/`NodeQueueId`) that no source permutation tried so far moves
without also changing the register allocation.  The literal-pool *targets*
are now correct, so a future pass only needs to reproduce the schedule.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Scratch (`/dev/shm/w5419`): `ms2.txt` (side-by-side), `rawscore.py` (raw
difflib ratio + byte runs), `gen.py` / `brute*.py` (permutation screen),
`compile.sh` / `asmview.py` (TU-only `.s` extraction).
