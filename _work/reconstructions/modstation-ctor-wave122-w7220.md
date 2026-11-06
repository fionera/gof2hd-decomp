# ModStation::ModStation ctor — wave 122 (w7220): scheduler-order dead end, no source win

Status: **no landed source change.** Worktree `wt/w7220`, base `29cd275f6`. Original
binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, sha256
`d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`). This note is
self-contained and extends `modstation-ctor-wave121-w7120.md` /
`modstation-ctor-wave119-w6931.md`.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B,
`src/game/menu/ModStation.cpp` (ctor at line ~273). Baseline **11 diff bytes / 4 runs,
99.2 %** for both C1 and C2. Gate after the sweep: **RATCHET PASS**, byte_exact
4506 / linked_exact 4506, avg 99.93, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, stubs 0, missing 0, extra 0.

## 1. Baseline and residual

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &yawBits;
    float pitch = *(const float *) &pitchBits;
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f, (RotationOrder) 2);
    ...
    MatrixSetRotation(farKey, yaw, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                      (RotationOrder) 2);
```

`.rodata` is already **byte-identical** (`0x1fe040..0x1fe0b0`): `Pitch`, `YawIPad`,
`Yaw`, and the whole `.rodata` section matches the original (0 diff bytes). The
wave-121 note's `.rodata` premise does not apply to this base.

The residual is confined to the near `MatrixSetRotation` call tail
(function-relative, base `0xd52d0`):

```text
+0x186 (1B)          orig=50 ours=cd
+0x188..+0x18f (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19c (1B)          orig=40 ours=23
+0x19e (1B)          orig=14 ours=40
```

Original near tail:

```text
d5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; yaw -> r3 (dead after the call)
d545a: 2002       movs  r0,#2
d545c: f8cd 80b8  str.w r8,[sp,#184]
d5460: 2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320
d546c: 4640       mov   r0,r8
d546e: 4614       mov   r4,r2             ; pitch preserved in r4
d5470: f78d edf6  blx   MatrixSetRotation
```

Ours (baseline, `far = yaw`):

```text
d5456: f8cd 80b8  str.w r8,[sp,#184]
d545a: f850 4028  ldr.w r4,[r0,r8,lsl#2]   ; yaw -> r4 (live across the call)
d545e: 2002       movs  r0,#2
d5460: 2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320
d546c: 4623       mov   r3,r4             ; extra copy
d546e: 4640       mov   r0,r8
d5470: f78d edf6  blx   MatrixSetRotation
```

The **far block is byte-identical** in the baseline, in `far = pitch`, and in the
original (`0xd54e0..0xd5502`): `ldrb iPad; cmp; ldrd [sp,#176]; it eq; moveq;
ldr race; mov r2,r4; ldr.w r3,[r1,race*4]`. So the far call is
`MatrixSetRotation(farKey, pitch, freshYaw, 0.0f, 2)` in all three and does not need
touching.

## 2. Root cause: BURR pre-RA scheduler reorders the two yaw-table LDRLITs

`llc` (LLVM 7.0.0 debug build at `/home/debian/llvm7build/build/bin/llc`) with
`-debug-only=isel` / `-debug-only=pre-RA-sched` on a `-g0` IR dump of the same
frontend shows the near block's SelectionDAG ISel order is **identical** for
`far = yaw` and `far = pitch`:

```text
ISEL: Starting selection on root node: t74: TargetConstantPool<@_ZL13gHangarCamYaw ...>
ISEL: Starting selection on root node: t71: TargetConstantPool<@_ZL17gHangarCamYawIPad ...>
ISEL: Starting selection on root node: t68: TargetConstantPool<@_ZL15gHangarCamPitch ...>
```

i.e. bottom-up selection order `Yaw, YawIPad, Pitch`, whose reverse is the original
instruction/pool order `Pitch, YawIPad, Yaw`. The BURR scheduler then reorders it in
the `far = pitch` shape and keeps it in the `far = yaw` shape.

The picker is `ScheduleDAGRRList::PickNodeToScheduleBottomUp` with `bu_ls_rr_sort`
(`lib/CodeGen/SelectionDAG/ScheduleDAGRRList.cpp`). Both yaw LDRLITs have
`getNodePriority() == 0` (no preds, has succs), so the decision falls through
`closestSucc` (equal: both feed the `t2MOVCCr`), `calcMaxScratches` (equal: 0), to
`BUCompareLatency`, which is dominated by:

```cpp
bool BUHasStall(SU, Height) { return CurCycle < Height; }
if (LStall) { if (!RStall) return 1; ... } else if (RStall) return -1;
if (!hazardRec->isEnabled()) { if (LHeight != RHeight) return LHeight > RHeight ? 1 : -1; }
```

Measured heights and the current cycle at the pick (`-debug-only=pre-RA-sched`):

| shape | CurCycle at pick | Yaw height | YawIPad height | stall? | picked first |
|---|---:|---:|---:|---|---|
| `far = pitch` | 15 | 13 | 15 | neither (15<15 false) | **YawIPad** (higher height) |
| `far = yaw` | 16 | 16 | 18 | YawIPad stalls (16<18) | **Yaw** |

`ReleasePredecessors` bumps each LDRLIT to `MOVCC.height + edge latency`
(`Yaw = MOVCC+1`, `YawIPad = MOVCC+3`, the `+3` is the `mov r0,r3` copy of the
non-MOVCC operand). The absolute heights differ between the two shapes only because
the live-out value changes (`%15:spr = VMOVSR pitch` vs `= VMOVSR yaw`), which
lengthens the `MOVCC` successor chain by 3 in the `far = yaw` shape.

So the original needs the `far = yaw`-like **heights** (so YawIPad stalls) together
with the `far = pitch` **live-out** (so yaw dies in `r3` and pitch is preserved in
`r4`). Those are produced by different live-out values, and the scheduler's decision
is a pure function of them.

## 3. Variants measured this wave (all reverted)

`fresh` = `(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`. All 1008 B
unless stated. `bytes`/`runs` = raw byte diff vs the original at `0xd52d0`.

| variant | bytes | runs |
|---|---:|---:|
| **baseline (`far = yaw`, volatile-int bit loads)** | **11** | **4** |
| `far = pitch` (faithful far x), volatile-int loads, far y = `fresh` | 43 | 8 |
| `far = pitch`, near y = `fresh`, far y = `fresh` | 43 | 8 |
| `far = pitch`, far y = `*(float*)&*(volatile int*)&fresh` | 43 | 8 |
| `far = pitch`, far y = `yawTable[race]` | 604 | 73 |
| `far = pitch`, `float pitch` declared before `float yaw` | 43 | 8 |
| `far = pitch`, `bool pad = Globals::iPad` + `pad ? ... : ...` twice | 604 | 73 |
| `far = pitch`, `bool pad` (near only) + far y = `Globals::iPad ? ... : ...` | 43 | 8 |
| `far = pitch`, far x = `*(float*)&pitchBits` | 43 | 8 |
| `far = pitch`, `const float zero`, `RotationOrder order` locals | 43 | 8 |
| `far = pitch`, `nearPitch`/`nearYaw` copies | 43 | 8 |
| `far = pitch`, plain `float pitch/yaw` loads (no volatile-int) | 667 | 86 |
| `far = pitch`, plain loads + `far y = fresh` | 667 | 86 |
| `far = pitch`, `volatile float` yaw local | 551 | 37 |
| `far = pitch`, `volatile float` pitch local | 676 | 77 |
| `far = pitch`, `__builtin_memcpy` int bit loads | 667 | 86 |
| `far = pitch`, `union { int i; float f; }` yaw bit load | 707 | 71 |
| `far = pitch`, `reinterpret_cast<const int*>` yaw load | 707 | 71 |
| `far = pitch`, `reinterpret_cast<const int*>` yaw load, far y = `fresh` | 44 | 7 |
| `far = yaw`, `far y = yawTable[race]` | 604 | 73 |
| `far = yaw`, `const float &yawRef` / `const float *yawPtr` | 604 | 73 |
| `far = yaw`, `volatile float &yawRef` | 621 | 88 |
| `far = yaw`, `*(float*)&*(volatile int*)&yawBits` (volatile stack reload) | 604 | 73 |
| `far = yaw`, `yaw + 0.0f` | 702 | 47 |
| `far = yaw`, pitch declared before yaw | 604 | 73 |
| `far = yaw`, yaw bit load before pitch bit load | 601 | 75 |

`-pre-RA-sched=list-hybrid` on the faithful `far = pitch` shape does emit the pool in
the original `Pitch, YawIPad, Yaw` order, but assigns the pointer registers as
`r0/r4/r3` instead of `r0/r3/r4`, so it is not byte-exact either (the build flags are
fixed at `list-burr` anyway).

## 4. Conclusion

The ctor is at the same hard local optimum wave 121 found, now with the mechanism
pinned down: the two yaw-table LDRLITs are selected in the original order by ISel in
both shapes, and the BURR bottom-up scheduler swaps them only when the near block's
live-out is `pitch` (the faithful shape). The original binary needs the faithful
`far = pitch` live-out **and** the `far = yaw`-like scheduler heights; with clang
7.0.2 `-Oz` those are mutually exclusive at source level. The remaining 11 bytes are
the post-RA schedule of `str.w r8` plus `ldr.w r4` / `mov r3,r4` instead of
`ldr.w r3` / `mov r4,r2`.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7220
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506 linked 4506
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```

Scheduler evidence: compile the TU with `-g0 -S -emit-llvm`, then

```sh
/home/debian/llvm7build/build/bin/llc -march=arm -mtriple=thumbv7-none-linux-android16 \
  -mcpu=cortex-a8 -float-abi=soft -debug-only=pre-RA-sched -o /dev/null ModStation.ll
```

and read the `*** Scheduling [cycle]: SU(N)` order for the block containing
`@_ZL13gHangarCamYaw` / `@_ZL17gHangarCamYawIPad` / `@_ZL15gHangarCamPitch`.

## 6. Wave-122 direct check against the real NDK clang

The scheduler numbers in section 2 come from the standalone LLVM-7 `llc` on a
`-g0` IR dump (that build is not bit-identical to the NDK r18b clang 7.0.2 used
by the project). To remove any doubt, the two shapes were also compiled with the
*actual* build compiler (`clang++ -Oz -g`, same flags as `build.ninja`) and the
near-block tails compared instruction by instruction:

```text
original            ldr.w r3,[r0,r8,lsl#2] ; movs r0,#2 ; str.w r8,[sp,#184] ; ...
                    mov r0,r8 ; mov r4,r2 ; blx
baseline (far=yaw)  str.w r8,[sp,#184] ; ldr.w r4,[r0,r8,lsl#2] ; ...
                    mov r3,r4 ; mov r0,r8 ; blx
far=pitch           str.w r8,[sp,#184] ; mov r4,r2 ; ldr.w r3,[r0,r8,lsl#2] ; ...
                    mov r0,r8 ; blx
```

`far = pitch` restores the original register roles (`yaw` dies in `r3`, `pitch`
survives in `r4`) but not the instruction placement (`str.w r8` / `mov r4,r2`
are scheduled before the `ldr.w r3` instead of after it), and it also flips the
literal-pool order to `{Pitch, Yaw, YawIPad}` (43 B / 8 runs). The baseline keeps
the pool order and every byte up to `moveq r0,r4` but keeps `yaw` live (11 B /
4 runs). The direct check therefore confirms the section 4 conclusion
independently of the `llc` model: no source spelling tried in waves 121-122
gives the original tail and the original pool order together.
