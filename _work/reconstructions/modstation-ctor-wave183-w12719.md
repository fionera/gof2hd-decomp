# ModStation constructor nearKey rotation — wave 183 (w12719)

Worker `w12719`, branch `wave183-w12719`, base master `9555474d8`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12719`.

## Result

| symbol | VMA | size | before raw/aligned | after raw/aligned |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | 27 / 15 | **11 / 5** |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | 27 / 15 | **11 / 5** |

Size-exact 1008/1008, instruction count 374/374, `.rel.plt` identical,
`.ARM.extab` content diff unchanged at 646. Whole-`.text` differing bytes
**75,015 -> 74,999** (-16). Gate: `RATCHET PASS` — byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.

## Landed change

In `ModStation::ModStation()` (src/game/menu/ModStation.cpp), the nearKey
camera-key setup drops the `__atomic_signal_fence` and swaps the two float
arguments at the nearKey `MatrixSetRotation` call:

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &pitchBits;
    float pitch = *(const float *) &yawBits;
    MatrixSetRotation(nearKey, yaw, pitch, 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
```

The farKey call is untouched (`MatrixSetRotation(farKey, pitch, ...)`), as is
the wave-182 `yaw = pitchBits` / `pitch = yawBits` bit swap.  The nearKey
argument swap makes the call pass `(pitchBits, yawBits)` — the original's
`r2`/`r3` — instead of the wave-182 swapped pair.

## Residual (11 B, 4 runs)

Function-relative, original vs ours:

```
+0x186  f850 3028   ldr.w r3,[r0,r8,lsl#2]   |  f8cd 80b8  str.w r8,[sp,#184]
+0x18a              (same)                    |  f850 4028  ldr.w r4,[r0,r8,lsl#2]
+0x18c  f8cd 80b8   str.w r8,[sp,#184]        |  (same)
+0x19c  4614        mov r4,r2                 |  4623       mov r3,r4
```

The original loads `yawBits` straight into the call's `r3` and copies
`pitchBits` (`r2`) to the callee-saved `r4` for the farKey call.  Ours keeps
the farKey value (`pitch` = yawBits) in `r4`, stores the `race` spill first,
and inserts `mov r3,r4` for the call.  This is a greedy-RA/scheduler tie:
every source spelling that makes the farKey value `pitchBits` (the
semantically faithful form) re-schedules the whole block to raw 43.

## Sweep (all 1008 B unless noted)

| shape | raw |
|---|---:|
| wave-182 base (fence + call `(pitch,yaw)`) | 27 |
| landed (no fence + call `(yaw,pitch)`) | **11** |
| no fence, call `(pitch,yaw)` | 43 |
| fence + call `(yaw,pitch)` | 25 |
| fence moved after table decls / after bits | 42 |
| `const float` locals | 11 |
| reversed float declaration order | 11 |
| one-line float declarations | 11 |
| `yawTable` declared before `pitchTable` | 28 |
| `yawBits` declared before `pitchBits` | 700 (size 1000) |
| farKey uses `yaw` / `*(float*)&pitchBits` | 43 |
| farKey uses `*(float*)&yawBits` | 11 |
| `idx`/`unsigned` copy of `race` | 11 |

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008
cmake --build cmake-build-match --target check -j2
```
