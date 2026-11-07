# ModStation::ModStation ctor — wave 125 (w7660): far=pitch live-out vs. BURR pool order (no source win)

Status: **no landed source change.** Worktree `wt/w7660`, base `fee528cbe` (wave-124 note).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained and extends
`modstation-ctor-wave124-w7639.md`, `modstation-ctor-wave124-w7616.md`,
`modstation-ctor-wave123-w7317.md`, `modstation-ctor-wave122-w7220.md`.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B, `src/game/menu/ModStation.cpp`
(ctor at line 273). Baseline **11 diff bytes / 4 runs, 99.2 %** for both C1 and C2. Gate after the
sweep: **RATCHET PASS**, byte_exact 4506 / linked_exact 4506, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

## 1. The two near blocks, byte-for-byte (function base `0xd52d0`)

The residual is entirely in the near `MatrixSetRotation` argument tail. The original
(`d5436..d5470`) is:

```text
d5436  ldr   r0, [pc, #632]      ; Pitch   (pool d56b0 = 0x128bf0)
d5438  ldr   r3, [pc, #632]      ; YawIPad (pool d56b4 = 0x128c16)
d543a  ldr   r4, [pc, #636]      ; Yaw     (pool d56b8 = 0x128c38)
d543c  add   r0, pc
d543e  add   r3, pc
d5440  ldrb.w r1, [sl]           ; Globals::iPad
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

Baseline source (`far = yaw`, committed) gives the same pool order and the same instructions up to
`moveq r0,r4`, then:

```text
d5456  str.w r8, [sp, #184]
d545a  ldr.w r4, [r0, r8, lsl #2]  ; yaw -> r4 (live)
d545e  movs  r0, #2
d5460  movs  r1, #0
d5462  add.w r8, sp, #256
d5466  strd  r1, r0, [sp]
d546a  add   r1, sp, #320
d546c  mov   r3, r4                ; extra copy
d546e  mov   r0, r8
d5470  blx
```

Faithful source (`far = pitch`) gives the correct register roles (`yaw -> r3` dead, pitch -> r4
via `mov r4,r2`) but the wrong pool order and schedule:

```text
d5436  ldr   r0, [pc, #632]      ; Pitch
d5438  ldr   r4, [pc, #632]      ; Yaw      <- pool order swapped
d543a  ldr   r3, [pc, #636]      ; YawIPad
d543c  add   r0, pc
d543e  ldrb.w r1, [sl]
d5442  add   r4, pc
d5444  ldr.w r2, [r0, r8, lsl #2]
d5448  add   r3, pc
d544a  cmp   r1, #0
d544c  mov   r0, r3
d544e  str   r3, [sp, #180]
d5450  str   r4, [sp, #176]
d5452  it    eq
d5454  moveq r0, r4
d5456  str.w r8, [sp, #184]        ; spill early
d545a  mov   r4, r2                ; pitch copy early
d545c  ldr.w r3, [r0, r8, lsl #2]
d5460  movs  r0, #2
d5462  movs  r1, #0
d5464  add.w r8, sp, #256
d5468  strd  r1, r0, [sp]
d546c  add   r1, sp, #320
d546e  mov   r0, r8
d5470  blx
```

So the target is exactly the faithful `far = pitch` shape with the **baseline's pool order and
schedule**. The task is to make the pre-RA BURR scheduler pick `Yaw` before `YawIPad` while the
live-out is `pitch`.

## 2. BURR stall data (standalone llc 7.0.0, `-g0 -emit-llvm` IR)

Command:

```sh
/home/debian/llvm7build/build/bin/llc -march=arm -mtriple=thumbv7-none-linux-android16 \
  -mcpu=cortex-a8 -float-abi=soft -debug-only=pre-RA-sched -o /dev/null ModStation.ll
```

Heights at the LDRLIT pick (SU(11)=Yaw, SU(12)=YawIPad, SU(8)=Pitch):

| shape | Yaw | YawIPad | Pitch | pick order (cycle) | emitted order |
|---|---:|---:|---:|---|---|
| baseline `far = yaw` | 16 | 18 | 18 | Yaw@16, YawIPad@18 | **Pitch, YawIPad, Yaw** (target) |
| `far = pitch` | 13 | 15 | 16 | YawIPad@15, Yaw@16, Pitch@17 | Pitch, Yaw, YawIPad (wrong) |

`BUHasStall(SU, Height) = CurCycle < Height`. At cycle 15 with the `far = pitch` heights,
`YawIPad` (15) does **not** stall (15 < 15 false), so the taller `YawIPad` is picked first. At
cycle 16 with the `far = yaw` heights, `YawIPad` (18) stalls (16 < 18 true) while `Yaw` (16) does
not (16 < 16 false), so `Yaw` is picked first. The 3-cycle height difference is exactly the
live-out: the near `yaw` load feeds the far call when `far = yaw`, which lengthens the whole
pointer/select/load chain by 3.

To flip the pick in the `far = pitch` shape we must make `YawIPad`'s height >= 16 (or drop
CurCycle to 14). A source-level `yaw = yaw + 0.0f` does exactly that (wave-124) but costs an
`fadd` (1020 B). No zero-cost source spelling found.

## 3. New variants measured this wave (all reverted; 1008 B unless noted)

This wave concentrated on the exact `MatrixSetRotation`/`MatrixSetTranslation` sequence, the
volatile/non-volatile local placement, the far-key expression order, and the EH/cleanup shape.
`BIT` = the committed volatile-int bit-load block. `fresh` =
`(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`.

| variant | diff bytes / runs | notes |
|---|---:|---|
| **baseline (`far = yaw`, volatile-int)** | **11 / 4** | committed |
| `far = pitch` (volatile-int) | 43 / 8 | correct roles, wrong pool+schedule |
| `far = pitch`, pitch non-volatile int | 666 / 87 | load hoisted/CSE'd |
| `far = pitch`, yaw non-volatile int | 44 / 7 | same as pitch-vol/yaw-plain |
| `far = pitch`, both non-volatile int | 667 / 86 | |
| `far = pitch`, `farYaw` local after near call | 43 / 8 | |
| `far = pitch`, `farYawTable` pointer after near call | 43 / 8 | |
| `far = pitch`, far y via `yawTable[race]` (pointer var) | 984 B | far block loses the fresh re-select |
| `far = pitch`, far x via `pitchTable[race]` | 984 B | |
| `far = pitch`, `*(const float*)&pitchBits` inline | 43 / 8 | |
| `far = pitch`, near y inline volatile load | 43 / 8 | |
| `far = pitch`, `reinterpret_cast` loads | 43 / 8 | |
| `far = pitch`, `const float` locals | 43 / 8 | |
| `far = pitch`, `int farRace = race;` after loads | 43 / 8 | |
| `far = pitch`, `volatile int farRace` after loads | 1000 B | extra stack traffic |
| `far = pitch`, `!iPad ? Yaw : YawIPad` (flipped ternary) | 43 / 8 | pool still wrong |
| `far = yaw`, flipped ternary | 13 / 6 | correct pool, but 2 extra pool-delta bytes |
| `far = pitch`, `iPad == 0 ? Yaw : YawIPad` | 43 / 8 | |
| `far = pitch`, explicit `if/else` pointer select | 43 / 8 | |
| `far = pitch`, row-pointer select (`&Yaw[race]`) | 1000 B | |
| `far = pitch`, `ptrvars` (YawIPad/Yaw pointers first) | 43 / 8 | |
| `far = yaw`, `ptrvars` | 11 / 4 | identical to baseline |
| `far = pitch`, `Matrix tmp = MatrixSetRotation(...)` | 992 B | sret temp becomes a named local |
| `far = pitch`, `nearKey = MatrixSetRotation(...)` | 1000 B | |
| `far = pitch`, `Vector{pitch, yaw, 0.0f}` overload | 556 / 31 | |
| `far = pitch`, nested scope around near call | 43 / 8 | |
| `far = pitch`, `try { ... } catch (...) { throw; }` | 1024 B | EH table changes |
| `far = yaw`, `try { ... } catch (...) { throw; }` | 1016 B | |
| `far = pitch`, `try { ... } catch (...) { }` | 613 / 37 | |
| `far = pitch`, near call x = fresh volatile pitch | 43 / 8 | |
| `far = pitch`, `volatile float yaw` + far y = yaw | 992 B | |
| `far = pitch`, `unsigned race` | 43 / 8 | |
| `far = pitch`, `const int pitchBits/yawBits` | 43 / 8 | |
| `far = pitch`, `const float &yaw/pitch` | 43 / 8 | |
| `far = pitch`, `RotationOrder order` local | 43 / 8 | |
| `far = yaw`, `farRace` local / far translation via members | 11 / 4 | byte-identical to baseline |
| `far = pitch`, `float pitch = *(float*)&*(volatile int*)&pitchTable[race]` | 666 / 87 | |
| `far = pitch`, yaw bitcast temp (`*(float*)&*(volatile int*)&yawTable[race]`) | 44 / 7 | correct pool + roles, wrong schedule |

The `44 / 7` shape is the closest "correct pool + correct roles" shape: the `Yaw` LDRLIT is sunk
after `cmp` and `mov r4,r2` is hoisted, so it is still worse than the 11-byte baseline.

## 4. Other evidence checked this wave

* `ModStation::resetIdleCamForHangar` is byte-exact and calls
  `MatrixSetRotation(*reinterpret_cast<Matrix*>(...), gHangarCamPitch[race], gHangarCamYaw[race],
  0.0f, 2)` with **inline plain-float indexing**; clang emits `ldr.w r2` / `ldr.w r3` there (no
  `vldr`). The ctor cannot use plain floats because `pitch` is live across the near call, so
  clang keeps it in a VFP register (`vldr s0` + `vmov`), which is why the volatile-int bit-load
  proxy is still required.
* The far block is byte-identical in every 1008 B variant: `ldrb iPad; cmp; ldrd [sp,#176];
  it eq; moveq; ldr race; mov r2,r4; ldr.w r3,[r1,race*4]`. It unambiguously reads `far x = pitch`
  (r4 = the near pitch load) and a **fresh** `far y` re-select. Any source that reuses the near
  `yaw` for the far call shrinks the function to 984 B, so the fresh far load is mandatory.
* `.rodata` order (`Pitch 0x1fe030`, `YawIPad 0x1fe058`, `Yaw 0x1fe080`) is already exact; the
  residual pool is the `.text` literal pool at `d56b0..d56b8`, whose word order follows the
  LDRLIT emission order.
* The EH call-site entry #3 start (`0x186` original vs `0x18e` baseline) is a consequence of the
  same schedule: the original's `EH_LABEL` sits before the near yaw load, ours after it. Wrapping
  the call in `try`/`catch` changes the EH table and does not help.

## 5. Conclusion

The ctor is at the same hard local optimum, now with the mechanism pinned to the BURR stall
decision and the exact instruction bytes:

* faithful `far = pitch` gives the right live-out/register roles but flips the LDRLIT pick
  (`YawIPad` height 15 at CurCycle 15 -> no stall -> `YawIPad` first), which swaps the `.text`
  literal pool and moves the race spill / pitch copy early: **43 B / 8 runs**;
* the committed `far = yaw` keeps the original pool and schedule but keeps `yaw` live in `r4`:
  **11 B / 4 runs**.

The target needs `YawIPad` height >= 16 at CurCycle 15 (or CurCycle 14) with a `pitch` live-out.
The only zero-cost lever would be a source spelling that adds a chain node to the near yaw path
without adding an instruction; `yaw = yaw + 0.0f` proves the mechanism but costs 12 bytes (1020 B).
No source spelling of the call sequence, local volatility, far-key expression order or EH scope
found this wave produces both properties.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7660
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

Raw-byte diff helper used this wave (`/dev/shm/w7660/score.py`): load both ELFs with
`tools/verify/elf.py`, `bytes_at(fn.value, fn.size)`, count differing byte offsets. Baseline is
`0x186 0x188..0x18f 0x19c 0x19e` (11 bytes, 4 runs).
