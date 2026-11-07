# ModStation::ModStation ctor — wave 124 (w7639): Ghidra-confirmed far=pitch, scheduler stall study (no source win)

Status: **no landed source change.** Worktree `wt/w7639`, base `4bbe88776`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2, `-Oz`, sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
This note is self-contained and extends `modstation-ctor-wave123-w7317.md` /
`modstation-ctor-wave122-w7220.md` / `modstation-ctor-wave121-w7120.md`.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B, `src/game/menu/ModStation.cpp`
(ctor at line 273). Baseline **11 diff bytes / 4 runs, 99.2 %** for both C1 and C2. Gate after
the sweep: **RATCHET PASS**, byte_exact 4506 / linked_exact 4506, avg 99.94, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. Baseline and residual (unchanged)

Function-relative, base `0xd52d0`:

```text
+0x185 (1B)          orig=50 ours=cd
+0x187..+0x18e (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19b (1B)          orig=40 ours=23
+0x19d (1B)          orig=14 ours=40
```

Near `MatrixSetRotation` tail only:

```text
orig                                 ours (baseline, far = yaw)
ldr.w r3,[r0,r8,lsl#2]  ; yaw->r3    str.w r8,[sp,#184]
movs  r0,#2                          ldr.w r4,[r0,r8,lsl#2] ; yaw->r4
str.w r8,[sp,#184]                   movs  r0,#2
movs  r1,#0                          movs  r1,#0
add.w r8,sp,#256                     add.w r8,sp,#256
strd  r1,r0,[sp]                     strd  r1,r0,[sp]
add   r1,sp,#320                     add   r1,sp,#320
mov   r0,r8                          mov   r3,r4
mov   r4,r2                          mov   r0,r8
blx   MatrixSetRotation              blx   MatrixSetRotation
```

## 2. New evidence: Ghidra confirms the far call is `(pitch, freshYaw)`

`rea decompile` (Ghidra 12.1.2, `GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC`) of
`_ZN10ModStationC1Ev` gives:

```c
uVar13 = *(undefined4 *)(&UNK_0020e030 + iVar3 * 4);   /* pitch, one word load */
...
puVar4 = &DAT_0020e058;                                 /* YawIPad */
if (Globals::iPad == '\0') puVar4 = &DAT_0020e080;      /* Yaw */
MatrixSetRotation((Matrix*)&local_b8, &local_78, uVar13,
                  *(undefined4*)(puVar4 + iVar3*4), 0, 2);
...
ppuVar1 = &local_104;                                   /* YawIPad */
if (Globals::iPad == '\0') ppuVar1 = &local_108;        /* Yaw */
MatrixSetRotation(aAStack_f4, &local_b8, uVar13,
                  *(undefined4*)(*ppuVar1 + local_100*4), 0, 2);
```

The far call reuses the **same** `uVar13` (pitch) as the near call and re-selects a **fresh**
yaw load. So the faithful source is `far = (pitch, freshYaw)`, not the baseline's
`far = (yaw, freshYaw)`. This confirms waves 121-123 by an independent route.

## 3. New evidence: the pre-RA scheduler pick order (standalone llc 7.0.0)

Generated `-g0 -emit-llvm` IR for the baseline and for `far = pitch`, then ran
`/home/debian/llvm7build/build/bin/llc -march=arm -mtriple=thumbv7-none-linux-android16
-mcpu=cortex-a8 -float-abi=soft -debug-only=pre-RA-sched` and read the
`*** Scheduling [cycle]: SU(N)` sequence for the block containing the three `tLDRpci`
leaves. The BURR scheduler is bottom-up, so the final instruction order is the **reverse** of
the pick order.

| shape | pick order | final MIR order | Yaw height | YawIPad height | CurCycle at pick |
|---|---|---|---:|---:|---:|
| baseline (`far = yaw`) | Yaw @16, YawIPad @18, Pitch @19 | **Pitch, YawIPad, Yaw** | 16 | 18 | 16 |
| `far = pitch` | YawIPad @15, Yaw @16, Pitch @17 | Pitch, **Yaw, YawIPad** | 13 | 15 | 15 |

The decision is `BUHasStall(SU, Height) = CurCycle < Height` in
`ScheduleDAGRRList::BUCompareLatency`:

* baseline: at cycle 16, YawIPad (height 18) **stalls** (16 < 18), so Yaw (no stall) is picked
  first; the reversed final order is the wanted `Pitch, YawIPad, Yaw`.
* `far = pitch`: at cycle 15, YawIPad (height 15) does **not** stall (15 < 15 false), so the
  higher-height YawIPad is picked first; the reversed final order is `Pitch, Yaw, YawIPad`.

The height difference comes only from the live-out value: the `MOVCC`/yaw-load chain is 3
shorter when the value kept across the call is `pitch` (a separate direct load) instead of
`yaw` (the load's own result). So the target needs the **baseline stall** together with the
**`far = pitch` live-out** — the two are still mutually exclusive at source level.

## 4. Also confirmed this wave

* The `.rodata` order is already byte-identical (`Pitch 0x1fe030`, `YawIPad 0x1fe058`,
  `Yaw 0x1fe080`); the residual is not a data-layout issue.
* The far block is byte-identical in the baseline and in `far = pitch` (it re-selects from the
  two spilled table pointers and does `mov r2, r4`), so the far block is already correct.
* The member-store sequence and the EH cleanup tail match byte-for-byte; the residual is
  exactly the six-instruction near-call tail above, not EH/cleanup or store order.

## 5. Variants measured this wave (all reverted, 1008 B unless noted)

`fresh` = `(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`. Diff = instruction
mismatches in the near block (baseline = 6; each mismatch is ~1-3 bytes).

| variant | near-block diffs |
|---|---:|
| **baseline (`far = yaw`, volatile-int loads)** | **6** |
| `far = pitch`, volatile-int loads | 18 |
| `far = pitch`, flipped ternary (`!iPad ? Yaw : YawIPad`) | 18 (pool fixed, `.rodata` swapped) |
| `far = pitch`, pitch volatile-int / yaw plain float | 16 |
| `far = pitch`, pitch volatile-int / yaw plain float, fence at every one of the 5 load-block positions | 16-18 |
| `far = pitch`, `const float pitch/yaw` | 18 |
| `far = pitch`, `ROTATION_ORDER_YXZ` enum / order local / zero local | 18 |
| `far = pitch`, `nearKey = MatrixSetRotation(...)` | 20 |
| `far = pitch`, `Matrix nearKey`/`int race` swapped | 18 |
| `far = pitch`, table decls moved before the camKey stores | 20 |
| `far = (pitch, yaw)` (same yaw variable) | 22, 984 B (far block reuses live yaw) |
| `far = (pitch, yaw)` + `volatile float yaw` / `volatile float pitch` | 20-23 |
| `far = (pitch, yaw)` + `(void) yaw` / `(void) pitch` | 22 |
| `far = pitch`, `float nearYaw = yaw` / `nearPitch` copies | 18 |
| `far = pitch`, plain-float loads (no volatile-int bitcast) | 26 (`vldr` + `vmov`) |
| `far = pitch`, `yaw = yaw + 0.0f` (chain +1 fadd) | **14, 1020 B** |
| `far = pitch`, `yaw = yaw * 1.0f` (folds) | 18 |
| `far = pitch`, `pitch = pitch + 0.0f` | 25 |
| `far = pitch`, `+yaw` / `(float)yaw` / double bitcast / `yawTable[race+0]` | 18 |

Also re-tested the earlier dead ends: `(void)` anchors, `__atomic_signal_fence` at every
position, `reinterpret_cast`, `memcpy`/union bit reads, swapped declaration order, swapped
ternary polarity, `yawTable[race]` for the far y, and the `Vector` overload. None reaches the
baseline's 6.

### 5.1 Chain-lengthening confirms the mechanism

`yaw = yaw + 0.0f` (which clang cannot fold because `x + 0.0f != x` for `x = -0.0`) adds one
`fadd` to the yaw chain and flips the pre-RA pick to the wanted order:

```text
ldr r0, Pitch
ldr r3, YawIPad
ldr r4, Yaw
add r0, pc
add r3, pc
ldrb.w r1, [r10]
add r4, pc
ldr.w r2, [r0, r8, lsl #2]   ; pitch
mov r0, r3
cmp r1, #0
str r3, [sp, #180]
str r4, [sp, #176]
it eq
moveq r0, r4
str.w r8, [sp, #184]
mov r4, r2
ldr.w r0, [r0, r8, lsl #2]   ; yaw
vldr s0, .LCPI0_1
vmov s2, r0
vadd.f32 s0, s2, s0
vmov r3, s0
...
```

This is the only shape found with **both** the original LDRLIT order (`Pitch, YawIPad, Yaw`)
and the original register roles (`yaw -> r3`, `pitch -> r4` via `mov r4,r2`) under the
`far = pitch` live-out, but the `fadd` costs 12 bytes (1020 B, not size-exact). It proves the
order is a pure chain-height effect and that the remaining gap is the missing +1..+3 height
that the `far = yaw` live-out supplies for free.

## 6. Conclusion

The ctor is at the same hard local optimum waves 121-123 found, now with the mechanism pinned
to the pre-RA BURR stall decision (section 3) and the far call independently confirmed as
`(pitch, freshYaw)` by Ghidra (section 2). The target needs the baseline's stall
(`YawIPad` height >= `CurCycle + 1`) together with a `pitch` live-out; the live-out is exactly
what sets the height, so no source spelling tried produces both. The remaining 11 bytes are
the post-RA placement of `str.w r8` plus `ldr.w r4`/`mov r3,r4` instead of `ldr.w r3`/
`mov r4,r2`.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7639
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6       # RATCHET PASS, byte 4506 linked 4506
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```

Scheduler evidence: compile the TU with `-g0 -S -emit-llvm`, then

```sh
/home/debian/llvm7build/build/bin/llc -march=arm -mtriple=thumbv7-none-linux-android16 \
  -mcpu=cortex-a8 -float-abi=soft -debug-only=pre-RA-sched -o /dev/null ModStation.ll 2> sched.txt
```

and read the `*** Scheduling [cycle]: SU(N)` order for the block containing
`@_ZL13gHangarCamYaw` / `@_ZL17gHangarCamYawIPad` / `@_ZL15gHangarCamPitch`.
