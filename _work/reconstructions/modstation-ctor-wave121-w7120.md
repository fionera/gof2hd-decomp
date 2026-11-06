# ModStation::ModStation ctor — wave 121 (w7120): dead-end sweep, no source win

Status: **dead-end sweep, no landed source change.** Worktree `wt/w7120`, base
`bd061a489`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained and
extends `modstation-ctor-wave119-w6931.md` / `-w6909.md` / `-w6723.md` /
`-w6717.md` / `-pass6-w4605.md`. It records the current baseline residual, the
mechanism, and every variant measured this wave; all `src/` variants were
reverted.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B,
`src/game/menu/ModStation.cpp` (ctor at line ~273). Baseline **11 diff bytes /
4 runs, 99.2 %** for both C1 and C2. Gate after the sweep: **RATCHET PASS**,
byte_exact 4505 / linked_exact 4505, avg 99.94, size-exact 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, stubs 0, missing 0, extra 0.

## 1. Baseline source and residual

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &yawBits;
    float pitch = *(const float *) &pitchBits;
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
    ...
    MatrixSetRotation(farKey, yaw, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
```

Baseline diff (function-relative, function base `0xd52d0`):

```text
+0x186 (1B)          orig=50 ours=cd
+0x188..+0x18f (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19c (1B)          orig=40 ours=23
+0x19e (1B)          orig=14 ours=40
```

Original Thumb near-block tail:

```text
d5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; near yaw -> r3 (dead after call)
d545a: 2002       movs  r0,#2
d545c: f8cd 80b8  str.w r8,[sp,#184]       ; spill race
d5460: 2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320
d546c: 4640       mov   r0,r8
d546e: 4614       mov   r4,r2             ; preserve pitch across the call
d5470: f78d edf6  blx   MatrixSetRotation
```

Ours (baseline):

```text
d5456: f8cd 80b8  str.w r8,[sp,#184]
d545a: f850 4028  ldr.w r4,[r0,r8,lsl#2]   ; near yaw -> r4 (live across call)
d545e: 2002       movs  r0,#2
d5460: 2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320
d546c: 4623       mov   r3,r4             ; extra copy
d546e: 4640       mov   r0,r8
d5470: f78d edf6  blx   MatrixSetRotation
```

The original far call is `(pitch, freshYaw)`: the far block re-reads
`Globals::iPad`, re-selects the yaw table from the two spilled pointers, and
emits `mov r2,r4` with `r4 = pitch` (`d54f0`). The baseline's `far = yaw` is a
semantic bug that keeps `yaw` live in `r4` and produces `mov r3,r4` instead;
it is the only shape that reproduces the original literal pool
(`{Pitch, YawIPad, Yaw}`) and every byte up to `moveq r0,r4`.

## 2. The two mutually-exclusive constraints

* `far = pitch` (faithful) gives the original register shape (near yaw -> `r3`
  dead, pitch -> `r4` via `mov r4,r2`) but flips the SelectionDAG topological
  order of the two yaw-table `tLDRLIT`s, so the literal pool becomes
  `{Pitch, Yaw, YawIPad}` and the whole block schedule is permuted: **43 B /
  8 runs**.
* `far = yaw` (baseline) keeps the original DAG order / literal pool / prefix
  but keeps yaw live into the far call: **11 B / 4 runs**.

The residual is three instructions: the placement of `str.w r8`, `ldr r4`
instead of `ldr r3`, and `mov r3,r4` instead of `mov r4,r2`. It is a post-RA
scheduling artifact, not missing code, a wrong constant, or a wrong callee.

## 3. Variants measured this wave (all reverted)

All 1008 B unless a size is given. `fresh` =
`(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`.

| variant | diff B | runs |
|---|---:|---:|
| **baseline (`far = yaw`, volatile-int bit loads)** | **11** | **4** |
| `far = pitch`, volatile-int pitch/yaw, far y = fresh | 43 | 8 |
| `far = pitch`, volatile-int pitch / plain-float yaw | 44 | 7 |
| `far = pitch`, plain-float pitch / volatile-int yaw | 666 | 87 |
| `far = pitch`, plain-float pitch/yaw | 667 | 86 |
| `far = pitch`, near y = `fresh` | 43 | 8 |
| `far = pitch`, near y = `yawTable[race]`, far y = fresh | 43 | 8 |
| `far = pitch`, near y = `yawTable[race]`, far y = `yawTable[race]` | 580 | 72 (984 B) |
| `far = pitch`, far y = `*(float *)&*(const volatile int *)&fresh` | 43 | 8 |
| `far = pitch`, `bool pad` two-use compare | 580 | 72 (984 B) |
| `far = pitch`, `signed char pad` two-use compare | 580 | 72 (984 B) |
| `far = pitch`, explicit `if/else` table select | 43 | 8 |
| `far = pitch`, `iPad == 0 ? Yaw : YawIPad` spelling | 43 | 8 |
| `far = pitch`, volatile read of `Globals::iPad` | 43 | 8 |
| `far = pitch`, `float nearPitch = pitch` near copy | 43 | 8 |
| `far = pitch`, `float farPitch = pitch` far copy | 43 | 8 |
| `far = pitch`, `volatile float yaw` local | 618 | 65 (992 B) |
| `far = pitch`, `volatile float yaw` local + far y = fresh | 551 | 37 |
| `far = pitch`, far y = `yaw + 0.0f` | 690 | 46 (996 B) |
| `far = pitch`, far z = `yaw` | 728 | 89 (1016 B) |
| `far = (pitch, yaw)` + fence between calls | 580 | 72 (984 B) |
| `far = pitch`, `volatile float sink = yaw` after far call | 582 | 56 (1016 B) |
| `far = pitch`, `volatile float sink = yaw` before far call | 594 | 65 (1016 B) |
| `far = pitch`, `this->camKeyX = yaw` after far call | 542 | 47 |
| `far = yaw`, `volatile float sink = pitch` after far call | 607 | 51 (1016 B) |
| `Matrix nearKey, farKey;` both declared at top | 542 | 47 |
| `Matrix nearKey;` moved after the race computation | 11 | 4 |

Fence sweep on the faithful `far = pitch` shape (`__atomic_signal_fence`
inserted at each of the 7 inter-statement positions 0..6):

| fence position | diff B | runs |
|---|---:|---:|
| 0 (before `pitchTable`) | 39 | 8 |
| 1 (before `yawTable`) | 39 | 8 |
| 2 (before `pitchBits`) | 42 | 7 |
| 3 (before `yawBits`) | 44 | 7 |
| 4 (before `float yaw`) | 42 | 7 |
| 5 (before `float pitch`) | 42 | 7 |
| 6 (before the near call) | 42 | 7 |

`(void)` reference sweep: `(void) gHangarCamPitch;`, `(void) gHangarCamYaw;`,
`(void) gHangarCamYawIPad;` inserted at each of the 7 positions of the
faithful `far = pitch` block — every one measures 43 B / 8 runs (the
`(void) gHangarCamYaw;@0` case is 46 B / 9 runs). The `(void)` reference does
not move the `far = pitch` literal pool, confirming the pool order is a DAG
ISel property, not a source-reference-order property for this shape.

## 4. Conclusion

The ctor is at a hard local optimum for clang 7.0.2 `-Oz`. The original needs
three things at once:

1. `.rodata` = `Pitch, YawIPad, Yaw` (fixed by the source first-reference
   order);
2. literal pool = `Pitch, YawIPad, Yaw` — only produced when `yaw` is **live**
   across the near call (`far = yaw`), i.e. the semantically-wrong baseline;
3. near block: yaw -> `r3` (dead), pitch -> `r4` (`mov r4,r2`) — only produced
   when `yaw` is **dead** (`far = pitch`) or when the RA rematerialises the
   yaw load.

(2) and (3) are mutually exclusive at source level with this compiler. The
remaining 11 bytes are the post-RA placement of `str.w r8` plus the
`ldr.w r4` / `mov r3,r4` shape produced by the live-yaw choice. No source
spelling of the ternary, the loads, the locals, the fences, the `(void)`
anchors, the matrix declarations, or the far-call arguments moved the score
below 11 B / 4 runs.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7120
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505 linked 4505
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```
