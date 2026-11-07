# ModStation::ModStation ctor — wave 130 (w7682): BURR tie-break is NodeQueueId, not height (no source win)

Status: **no landed source change.** Worktree `wt/w7682`, base `003a9623a`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2, `-Oz`). This note is self-contained and extends
`modstation-ctor-wave125-w7660.md`, `-wave124-w7639.md`, `-wave123-w7317.md`.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B, `src/game/menu/ModStation.cpp`
(ctor at line 273). Baseline **11 diff bytes / 4 runs, raw 99.2 %** for both C1 and C2.
Gate after this wave: **RATCHET PASS**, byte_exact 4506 / linked_exact 4506, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.

## 1. Residual (unchanged)

Function-relative, base `0xd52d0`; the only differing region is the near `MatrixSetRotation`
argument tail (`+0x185..+0x19d`). Original (`d5436..d5470`):

```text
d5436  ldr   r0, [pc, #632]      ; Pitch
d5438  ldr   r3, [pc, #632]      ; YawIPad
d543a  ldr   r4, [pc, #636]      ; Yaw
d543c  add   r0, pc
d543e  add   r3, pc
d5440  ldrb.w r1, [sl]           ; iPad
d5444  add   r4, pc
d5446  ldr.w r2, [r0, r8, lsl #2]  ; pitch
d544a  mov   r0, r3
d544c  cmp   r1, #0
d544e  str   r3, [sp, #180]        ; spill YawIPad ptr
d5450  str   r4, [sp, #176]        ; spill Yaw ptr
d5452  it    eq
d5454  moveq r0, r4
d5456  ldr.w r3, [r0, r8, lsl #2]  ; yaw  (dead after the call)
d545a  movs  r0, #2
d545c  str.w r8, [sp, #184]        ; spill race
d5460  movs  r1, #0
d5462  add.w r8, sp, #256
d5466  strd  r1, r0, [sp]
d546a  add   r1, sp, #320
d546c  mov   r0, r8
d546e  mov   r4, r2                ; pitch kept live for the far call
d5470  blx   MatrixSetRotation
```

Committed baseline (`far = yaw`, volatile-int bit loads) keeps the original pool order
(`Pitch, YawIPad, Yaw`) but keeps `yaw` live in `r4` and adds a `mov r3,r4`: 5 differing
instructions. Faithful `far = pitch` gives the original register roles (`yaw -> r3` dead,
`pitch -> r4`) but emits the pool order `Pitch, Yaw, YawIPad` and schedules the race spill /
pitch copy early: 10 differing instructions.

## 2. NEW: the Yaw/YawIPad pick is a NodeQueueId tie-break, not a height/stall decision

Wave-124/125 explained the pool flip by the BURR stall rule
(`BUHasStall(SU,Height) = CurCycle < Height`). Re-running the standalone scheduler on the
faithful IR (`/home/debian/llvm7build/build/bin/llc -march=arm -mtriple=thumbv7-none-linux-android16
-mcpu=cortex-a8 -float-abi=soft -debug-only=pre-RA-sched`) shows the actual comparison:

```text
*** Scheduling [15]: SU(6): lifetime.end
GPR: 4 / 9
Examining Available:
Height 15: SU(12): gHangarCamYawIPad
Height 13: SU(11): gHangarCamYaw
Height 16: SU(8):  gHangarCamPitch
Height 17: SU(7):  Globals::iPad
*** Scheduling [15]: SU(12): gHangarCamYawIPad
```

There is **no `Comparing latency of SU(12) ... vs SU(11)` line**: `BUCompareLatency` returns 0
for the two LDRLITs (both non-stalling at CurCycle 15, equal depth/latency), so the pick falls
through to `BURRSort`'s final tie-break `left->NodeQueueId > right->NodeQueueId`. The pool order
is therefore the order the two LDRLITs become ready in the DAG, not a height difference. This is
why every height-affecting source tweak (`yaw + 0.0f`, branch weights, `__builtin_expect`) either
does nothing to the pool or costs an instruction.

## 3. Variants measured this wave (all reverted; 1008 B unless noted)

Harness: `/dev/shm/w7682/mod/` (per-variant `compile.sh` + instruction-sequence scorer).
Baseline = 5 differing instructions; faithful = 10.

| variant | diff insns | notes |
|---|---:|---|
| **baseline (`far = yaw`)** | **5** | committed |
| faithful `far = pitch` | 10 | correct roles, wrong pool/schedule |
| faithful + 720 declaration-order permutations (6 statements) | 10 (best 6 orders) / 31 (rest) | no order changes the pool |
| faithful + `Globals::iPad != 0 / > 0 / !(!iPad) / (iPad&1)` | 10 | all canonicalize to `select (iPad==0), Yaw, YawIPad` |
| faithful + `Globals::iPad == 1` | 47+ | changes `cmp #0` to `cmp #1`, rotates race to r4 |
| faithful + `select-after-load` (`iPad ? YawIPad[race] : Yaw[race]`) | 10 | canonicalizes back to the pointer select |
| faithful + `yawTable[race]` / `*(yawTable+race)` / `race[yawTable]` | 55 | far block reuses live yaw, 984 B |
| faithful + `*(const volatile int*)&yawTable[race]` | 55 | same |
| faithful + `far = (pitch, yaw)` (near yaw live-out) | 55 | 984 B, far block reuses yaw |
| faithful + `farYaw` local before/after near call | 10 / 55 | no rematerialization |
| faithful + `volatile`/`const` pitch or yaw locals | 10–174 | volatile adds stack traffic |
| faithful + branch-weight sweep {1:1,2:1,3:1,4:1,8:1,16:1,31:1,100:1,1:7,7:7,20:12,5:3,2000:1} | 10 (best) / 66 | same as 7:1 for the faithful pool |

The far block is byte-identical in every 1008 B variant, so the residual is exactly the near
LDRLIT pick + the race-spill/pitch-copy schedule.

## 4. Conclusion

The ctor is at the same hard local optimum. The new evidence is that the pool flip is a
`NodeQueueId` tie-break between two equal-height LDRLITs, not the `BUHasStall` height rule the
wave-124/125 notes assumed. That makes the pool order a pure DAG-readiness ordering fact: the
faithful `far = pitch` live-out shortens the near-yaw-load chain, the yaw LDRLIT becomes ready
after the YawIPad one, and the pool comes out `Pitch, Yaw, YawIPad`. No source spelling of the
call sequence, local volatility, far-key expression or EH scope found this wave produces both the
original pool and the original register roles. The committed baseline (`far = yaw`,
**99.2 %**, 1008 B) remains the best shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7682
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6       # RATCHET PASS, byte 4506 linked 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```

Scheduler evidence: generate `-g0 -S -emit-llvm` IR for the TU, then

```sh
/home/debian/llvm7build/build/bin/llc -march=arm -mtriple=thumbv7-none-linux-android16 \
  -mcpu=cortex-a8 -float-abi=soft -debug-only=pre-RA-sched -o /dev/null ModStation.ll 2> sched.txt
```

and read the `*** Scheduling [N]` / `Height` lines around the three `gHangarCam*` LDRLITs. The
decisive observation is the absence of a `Comparing latency` line for SU(11) vs SU(12).
