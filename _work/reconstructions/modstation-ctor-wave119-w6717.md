# ModStation::ModStation ctor — wave 119 (w6717): dead-end sweep, no source win

Status: **dead-end sweep, no landed source change.** Worktree `wt/w6717`,
branch `wave119-w6717`, base `47a1396bd`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained: it
records the baseline residual, the mechanism behind it, and every variant
measured this wave (including the ones that only shift the score sideways).
It supersedes and extends `modstation-mgame-wave118-w6619.md` §1 for the ctor;
the MGame half of that note is unchanged.

Target: `_ZN10ModStationC1Ev` / `C2Ev`, 1008 B, `src/game/menu/ModStation.cpp`
(ctor at line 273). Baseline **11 diff bytes / 4 byte runs, 99.2 %**
(wave-112 shape, unchanged). Nothing beat it; every `src/` variant was
reverted. Gate after the sweep: **RATCHET PASS**, byte_exact 4503 /
linked_exact 4503, size-exact 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, stubs 0, missing 0, extra 0.

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

Baseline diff (function-relative; function starts at `0xd52d0`):

```text
+0x186..+0x186 (1B)  orig=50 ours=cd
+0x188..+0x18f (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19c..+0x19c (1B)  orig=40 ours=23
+0x19e..+0x19e (1B)  orig=14 ours=40
```

The original's far call is `(pitch, freshYaw)`, i.e. the faithful source is
`far = pitch`; the baseline's `far = yaw` is a real semantic bug that happens
to score better in bytes. Ground truth (original Thumb, function-relative):

```text
166: ldr   r0, [pc,#632]        ; gHangarCamPitch
168: ldr   r3, [pc,#632]        ; gHangarCamYawIPad
16a: ldr   r4, [pc,#636]        ; gHangarCamYaw
16c: add   r0, pc
16e: add   r3, pc
170: ldrb.w r1, [sl]            ; Globals::iPad
174: add   r4, pc
176: ldr.w r2, [r0, r8, lsl#2]  ; pitch  (volatile int load)
17a: mov   r0, r3
17c: cmp   r1, #0
17e: str   r3, [sp,#180]
180: str   r4, [sp,#176]
182: it    eq
184: moveq r0, r4               ; yawTable = iPad ? YawIPad : Yaw
186: ldr.w r3, [r0, r8, lsl#2]  ; near yaw  -> r3, dead after the call
188: movs  r0, #2
18a: str.w r8, [sp,#184]        ; spill race
18e: movs  r1, #0
190: add.w r8, sp, #256
194: strd  r1, r0, [sp]
198: add   r1, sp, #320
19c: mov   r0, r8
19e: mov   r4, r2               ; preserve pitch across the call
1a0: blx   MatrixSetRotation
...
210: ldrb.w r0, [sl]
214: cmp   r0, #0
216: ldrd  r0, r1, [sp,#176]
21a: it    eq
21c: moveq r1, r0
21e: ldr   r0, [sp,#184]
220: mov   r2, r4               ; far x = pitch
222: ldr.w r3, [r1, r0, lsl#2]  ; far y = fresh table load
```

## 2. Mechanism: DAG order vs liveness

With `far = pitch` the yaw value dies after the near call; with `far = yaw` it
is live across the call. That single IR difference (`float %107` vs `float
%108` in the far `invoke`) flips the SelectionDAG's topological order of the
two yaw-table constants and hence the whole block's schedule:

```text
baseline (far = yaw)                 far = pitch
%114 LDRLIT Globals::iPad            %114 LDRLIT Globals::iPad
%115 t2LDRi12 %114                   %115 LDRLIT gHangarCamPitch
%116 LDRLIT gHangarCamPitch          %116 t2LDRi12 %114
%117 LDRLIT gHangarCamYawIPad        %117 LDRLIT gHangarCamYaw
%118 LDRLIT gHangarCamYaw            %118 LDRLIT gHangarCamYawIPad
```

`AssignTopologicalOrder` numbers zero-operand nodes in `AllNodes` order, and
the yaw load's live-out status changes the DAG's root set, which changes that
order. The baseline's literal pool is `{Pitch, YawIPad, Yaw}` (matching the
original); `far = pitch` gives `{Pitch, Yaw, YawIPad}`. The `t2MOVCCr` operand
order is `(YawIPad, Yaw)` in both, so the CMOV itself is identical.

`far = pitch` (43 B / 8 runs) is the same instruction *set* as the original
with only the schedule permuted:

```text
166: ldr r0, pitch
168: ldr r4, Yaw            <- swapped vs original
16a: ldr r3, YawIPad
16c: add r0, pc
16e: ldrb.w r1, [sl]
172: add r4, pc
174: ldr.w r2, [r0, r8, lsl#2]
178: add r3, pc
17a: cmp r1, #0
17c: mov r0, r3
17e: str r3, [sp,#180]
180: str r4, [sp,#176]
182: it eq
184: moveq r0, r4
186: str.w r8, [sp,#184]    <- yaw load sunk below the spill
18a: mov r4, r2             <- pitch copy hoisted
18c: ldr.w r3, [r0, r8, lsl#2]
190: movs r0, #2
...
```

So the two constraints are mutually exclusive at source level:

* `far = pitch` gives the original's register shape (yaw -> r3 dead, pitch ->
  r4 live, `mov r4,r2`) but flips the DAG order and the schedule (43 B).
* `far = yaw` gives the original's DAG order and prefix byte-for-byte but
  keeps yaw live into the far call (r4 + `mov r3,r4`, 11 B).

`-mllvm -misched-postra=false` (scratch only) reproduces the *register*
shape of the original near block (`ldr r3` yaw, `mov r4,r2`) but still leaves
the literal order `{Pitch, Yaw, YawIPad}` and the `str r8`/`mov r4,r2`
schedule permuted, so it is not a path to 0 either.

## 3. Variants measured this wave (wave 119)

All 1008 B unless a size is given. `far` is `pitch` unless stated. `fresh` =
`(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`.

| variant | diff B | runs |
|---|---:|---:|
| **baseline (`far = yaw`, volatile-int bit loads)** | **11** | **4** |
| `far = pitch` (`fp`) | 43 | 8 |
| `far = pitch`, `__atomic_signal_fence` before table decls | 39 | 8 |
| `far = pitch`, fence after yawBits / after float yaw / between loads / after near call | 42 | 7 |
| `far = pitch`, fence after pitchBits | 44 | 7 |
| `far = pitch`, fence at each of 8 inter-line positions | 39/39/42/44/42/42/42/43 | 8/8/7/7/7/7/7/8 |
| `far = pitch`, `__builtin_assume(yaw == yaw)` after far call | 42 | 7 |
| `far = pitch`, `__builtin_assume(yaw == yaw)` before far translation | 42 | 7 |
| `far = pitch`, `__builtin_assume(__builtin_isfinite(yaw))` | 42 | 7 |
| `far = pitch`, far y = `*(float*)&*(volatile int*)&fresh` | 43 | 8 |
| `far = pitch`, far y = `*(volatile float*)&fresh` | 337 | 25 |
| `far = pitch`, far y = `yaw` / `yawTable[race]` / `*(float*)&yawBits` | 580 | 72 (984 B) |
| `far = pitch`, far y = `yaw` (reuse) + volatile-int near loads | 580 | 72 (984 B) |
| `far = pitch`, plain float loads, far y = fresh | 667 | 86 |
| `far = pitch`, plain float loads, far y = `yaw` | 701 | 79 (984 B) |
| `far = pitch`, plain int bit loads, far y = fresh | 667 | 86 |
| `far = pitch`, plain int bit loads, far y = `yaw` | 701 | 79 (984 B) |
| `far = pitch`, volatile-int pitch + plain-float yaw, far y = fresh | 44 | 7 |
| `far = pitch`, plain-float pitch + volatile-int yaw, far y = fresh | 43 | 8 |
| `far = pitch`, volatile-int pitch + volatile-float yaw, far y = fresh | 528 | 90 (1016 B) |
| `far = pitch`, `const float *volatile` pitchTable | 531 | 88 |
| `far = pitch`, `const float *volatile` yawTable | 278 | 99 |
| `far = pitch`, `const volatile float &yaw` | 750 | 39 (1016 B) |
| `far = pitch`, `int &yaw` bit reference | 724 | 33 |
| `far = pitch`, `float &yaw = *(float*)&yawTable[race]` | 667 | 86 |
| `far = pitch`, near y = `yawTable[race]`, far y = fresh | 44 | 7 |
| `far = pitch`, near y = `yawTable[race]`, far y = `yawTable[race]` | 580 | 72 (984 B) |
| `far = pitch`, both calls `yawTable[race]` | 580 | 72 (984 B) |
| `far = pitch`, near y = fresh, far y = `yaw` | 580 | 72 (984 B) |
| `far = pitch`, near y = `yaw`, far y = `yaw` (`far = (pitch, yaw)`) | 580 | 72 (984 B) |
| `far = pitch`, `bool pad` / `char pad` local | 43 | 8 |
| `far = pitch`, helper `ModStation_yawTable()` inline | 43 | 8 |
| `far = pitch`, `int race = 0` | 43 | 8 |
| `far = pitch`, `volatile int race` | 867 | 83 (1016 B) |
| `far = pitch`, `posTable` local for far translation | 856 | 49 |
| `far = pitch`, `farPos` local for far translation | 43 | 8 |
| `far = pitch`, `camPos` local for near camKey | 856 | 49 |
| `far = pitch`, `Matrix farKey = MatrixSetRotation(...)` NRVO | 895 | 31 (944 B) |
| `far = pitch`, `Matrix nearKey` moved later | 795 | 92 (1000 B) |
| `far = pitch`, `pitch + 0.0f` near x | 682 | 92 (1012 B) |
| `far = pitch`, `Matrix &nearRef` | 43 | 8 |
| `far = pitch`, `volatile int sink = pitchBits` after block | 646 | 48 |
| `far = pitch`, `float farPitch = pitch` before far call | 43 | 8 |
| `far = pitch`, enum `ROTATION_ORDER_YXZ` | 43 | 8 |
| `far = pitch`, `static_cast<RotationOrder>(2)` / `int 0` / `float zero` / `order` var | 43 | 8 |
| `far = pitch`, `__builtin_memcpy` / `union` bitcast for yaw | 676 | 69 (976 B) |
| `far = pitch`, `int` tables + plain loads + bitcast fixes | 667 | 86 |
| `far = pitch`, `int` tables + `pitchBits[race]` plain load + far y bitcast | 667 | 86 |
| `far = pitch`, `int` tables + `pitchBits & 1` / `-pitchBits` / `pitchBits^x` sink | 667 | 86 |
| `far = pitch`, `int` tables + `pitchBits^x` sink + far y = `yaw` | 676 | 69 (976 B) |
| `far = pitch`, `int` tables + both volatile int loads | 43 | 8 |
| `far = pitch`, `int` tables + pitch volatile / yaw plain | 44 | 7 |
| `far = pitch`, `int` tables + pitch plain / yaw volatile | 666 | 87 |
| `far = pitch`, `int` locals (`*(float*)&pitch`) + far y fresh | 667 | 86 |
| `far = pitch`, `int` locals + far y = `yaw` | 701 | 79 (984 B) |
| `far = pitch`, far z = `yaw - yaw` | 746 | 81 (1024 B) |
| `far = pitch`, far z = `yaw * 0.0f` / `0.0f * yaw` | 755 | 74 (1028 B) |
| `far = pitch`, 4-arg `MatrixSetRotation(m, x, y, z)` overload | 526 | 88 (1000 B) |
| `far = pitch`, `farKey` declared before `nearKey` | 925 | 31 (968 B) |

Wave-119 additions (this session), baseline `far = yaw` unless stated:

| variant | diff B | runs |
|---|---:|---:|
| `yawTable` declared before `pitchTable` | 16 | 7 |
| yaw load before pitch load | 694 | 92 (1000 B) |
| `float pitch` declared before `float yaw` | 11 | 4 |
| `bool pad = Globals::iPad` local, both ternaries use `pad` | 891 | 29 (968 B) |
| explicit `if/else` for `yawTable` | 11 | 4 |
| named `RotationOrder order` variable | 11 | 4 |
| named `float zero = 0.0f` | 11 | 4 |
| far y = `*(float*)&*(volatile int*)&fresh` | 11 | 4 |
| inline `*(float*)&pitchBits` at near call | 11 | 4 |
| `const int pitchBits` / `const int yawBits` | 11 | 4 |
| drop `pitchTable` local, index `gHangarCamPitch` directly | 16 | 7 |
| `int farRace = race` used by far call | 11 | 4 |
| far y index `race * 1` / `race + 0` / `(unsigned) race` | 11 | 4 |
| `const float *const yawTable` | 11 | 4 |
| `__builtin_expect(Globals::iPad, 1)` in ternary | 11 | 4 |
| extra unused `Matrix spare` local | 11 | 4 |
| `int idx = race * 3` for camKey/translation | 11 | 4 |
| camKey assignments reordered Z,Y,X | 11 | 4 |
| `unsigned race` / `long race` / `intptr_t race` | 11 | 4 |

## 4. Int-table hypothesis (ruled out)

Converting `gHangarCamPitch` / `gHangarCamYawIPad` / `gHangarCamYaw` to
`static const int` bit patterns with plain (non-volatile) loads and
`*(const float *) &` bitcasts at every use site leaves `OnInitialize` and
`resetLight` byte-exact (0 diff each) but regresses the ctor to 667 B / 86
runs. The current `const float` tables already leave `OnInitialize` and
`resetLight` byte-exact, so the int-table variant is not evidence of a
different original table type; it only changes the ctor's register allocation
(pitch is kept in `s0`/`fp` instead of `r2`/`r4`). The original ctor's pitch
load is `ldr.w r2` (integer load), but every plain-int spelling we tried is
folded back to `vldr` + `vmov` by the DAG combiner because the value is
live across the call; only the `volatile int` bit load keeps it in a core
register, and that is exactly the baseline hack.

## 5. Conclusion

The ctor is at a hard local optimum for this compiler: the 11-byte baseline
uses `far = yaw` to preserve the original's DAG order and prefix, at the cost
of `mov r3,r4` instead of the original `mov r4,r2`. The faithful `far =
pitch` shape (43 B) has the original's register shape but a permuted schedule.
No source spelling of the ternary, the loads, the locals, the table types, the
`race` variable, or the surrounding camKey/translation code moved the score
below 11 B / 4 runs. The only known lever left is the post-RA scheduler's
placement of `str.w r8,[sp,#184]` and the yaw load, which is not reachable
from source without changing the build flags.
