# ModStation::ModStation ctor — wave 119 (w6723): literal-order / .rodata coupling sweep

Status: **dead-end sweep, no landed source change.** Worktree `wt/w6723`,
branch `wave119-w6723`, base `432167221`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained: it
records the baseline residual, the mechanism behind it, and every variant
measured this wave. It extends `modstation-ctor-wave119-w6717.md` (same
target, same wave) with a new angle: the coupling between the module-level
global order, the `.rodata` table order, and the ARM literal-pool order.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B,
`src/game/menu/ModStation.cpp` (ctor at line 273). Baseline **11 diff bytes /
4 runs, 99.2 %** (wave-112 shape). Nothing beat it; every `src/` variant was
reverted. Gate after the sweep: **RATCHET PASS**, byte_exact 4503 /
linked_exact 4503, size-exact 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, stubs 0, missing 0, extra 0. 27 sections exact, file size
2,187,328 B, `.ARM.extab` 0x26290 (unchanged, `createShip` -8 only).

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

Baseline diff (function-relative; `fdiff.py` offsets are relative to the
dynsym value `0xd52d1`, i.e. function base + 1):

```text
+0x185 (1B)  orig=50 ours=cd
+0x187..+0x18e (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19b (1B)  orig=40 ours=23
+0x19d (1B)  orig=14 ours=40
```

Original Thumb (function-relative, `d52d0` base):

```text
d5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; near yaw -> r3 (dead after call)
d545a: 2002       movs  r0,#2
d545c: f8cd 80b8  str.w r8,[sp,#184]       ; spill race
d5460: 2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320
d546c: 4640       mov   r0,r8
d546e: 4614       mov   r4,r2             ; pitch -> r4 (live across call)
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

The whole prefix up to `moveq r0,r4` (d5454) is byte-identical. The residual
is exactly: the near yaw load lands in the callee-saved `r4` (because the
baseline keeps `yaw` live across the call) and is copied to the argument
register `r3`, whereas the original loads it straight into `r3` and instead
copies `pitch` (r2) to `r4`. The `str.w r8` spill also moves one slot.

The original's far call is `(pitch, freshYaw)`; the baseline's `far = yaw` is
a semantic bug that happens to preserve the original's DAG/literal order.

## 2. Mechanism (confirmed with `-print-after-all` MIR dumps)

`SelectionDAG::AssignTopologicalOrder` (LLVM 7) numbers zero-operand nodes in
DAG creation order, and `DoInstructionSelection` walks the sorted node list
**backwards** (root first), recursively selecting operands. The three
`tLDRLIT_ga_pcrel` pseudos are therefore emitted in the ISel DFS order, and
that order *is* the ARM constant-pool order.

Near-block ISel output (first `# Machine code for function` dump):

```text
baseline (far = yaw)                 far = pitch
%114 LDRLIT Globals::iPad            %114 LDRLIT Globals::iPad
%115 t2LDRi12 (GOT)                  %115 LDRLIT gHangarCamPitch
%116 LDRLIT gHangarCamPitch          %116 t2LDRi12 (GOT)
%117 LDRLIT gHangarCamYawIPad        %117 LDRLIT gHangarCamYaw
%118 LDRLIT gHangarCamYaw            %118 LDRLIT gHangarCamYawIPad
```

The only IR difference between the two is the far `invoke`'s first float
operand (`%107` = yaw vs `%108` = pitch), i.e. which value is live-out of the
near block. That changes the DAG root set, hence the Kahn/ISel order of the
two yaw-table `GlobalAddress` nodes, hence the literal pool
`{Pitch, YawIPad, Yaw}` (original/baseline) vs `{Pitch, Yaw, YawIPad}`
(far = pitch). The `t2MOVCCr` operand order is `(YawIPad, Yaw)` in both.

Pre-post-RA MIR of the near block:

```text
baseline                             far = pitch
$r0 = Pitch ; $r0 = PICADD           $r0 = Pitch ; $r0 = PICADD
$r3 = YawIPad ; $r3 = PICADD         $r4 = Yaw ; $r4 = PICADD
$r4 = Yaw ; $r4 = PICADD             $r3 = YawIPad ; $r3 = PICADD
$r1 = iPad load                      $r1 = iPad load
$r2 = pitch load                     $r2 = pitch load
...                                  ...
$r4 = t2LDRs (yaw)                   $r3 = t2LDRs (yaw)
$r3 = tMOVr $r4                      $r4 = tMOVr $r2
```

So the post-RA scheduler is not the culprit: the register assignment and the
literal order are already decided before it runs. `far = pitch` gives the
original's *register shape* but the wrong literal order; `far = yaw` gives the
original's literal order but keeps yaw live (extra `mov r3,r4`).

## 3. New angle this wave: module global order ↔ `.rodata` ↔ literal pool

Two source spellings that produce **byte-identical IR instructions** can
still produce different code, because the module-level order of the internal
`gHangarCamYawIPad` / `gHangarCamYaw` globals differs. Measured with
`-emit-llvm -S` (normalised metadata) and by reading `.rodata` at
`0x1fe058` / `0x1fe080` (index 8 = 2.35 for YawIPad, 2.5 for Yaw):

| near ternary spelling | module global order | `.rodata` order | literal pool order |
|---|---|---|---|
| `Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw` | YawIPad, Yaw | **Pitch, YawIPad, Yaw** (correct) | Pitch, Yaw, YawIPad (far=pitch) |
| `!Globals::iPad ? gHangarCamYaw : gHangarCamYawIPad` | Yaw, YawIPad | Pitch, Yaw, YawIPad (**data_exact regression**) | Pitch, YawIPad, Yaw |

So the `.rodata` order follows the **first operand referenced by the source
ternary**, and the literal-pool order is the reverse of the module order for
the two yaw tables. The swapped-ternary variant (43 B / 8 runs) has the
correct literal order but swaps the two 40-byte tables in `.rodata`, which
would break `data_exact 843/843` — it is not landable. Swapping the
declaration order of the two arrays does **not** compensate (measured: the
`.rodata` order still follows the ternary's first operand).

## 4. Variants measured this wave

All 1008 B unless noted. `farp` = `MatrixSetRotation(farKey, pitch, freshYaw,
0.0f, 2)`. `freshYaw` = `(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`.
"lit" is the resolved literal-pool target order
(`Pitch`/`YawIPad`/`Yaw`); "pct" is `tools/verify/verify.py`'s difflib fuzzy
score. Baseline is 11 B / 4 runs / **99.2 %**.

| variant | diff B | runs | lit | pct |
|---|---:|---:|---|---:|
| **baseline (`far = yaw`, volatile-int bit loads)** | **11** | **4** | **Pitch, YawIPad, Yaw** | **99.2** |
| farp, volatile-int yaw | 43 | 8 | Pitch, Yaw, YawIPad | 98.4 |
| farp + `__atomic_signal_fence` before table decls | 39 | 8 | Pitch, Yaw, YawIPad | 98.4 |
| farp + fence after table ptrs | 43 | 7 | Pitch, YawIPad, Yaw | 97.5 |
| farp, near yaw **plain float** (`float yaw = yawTable[race]`) | 44 | 7 | Pitch, YawIPad, Yaw | 97.8 |
| farp, near yaw plain float + fence after ptrs | 43 | 7 | Pitch, YawIPad, Yaw | 97.5 |
| farp, near yaw plain float, far y volatile-int | 44 | 7 | Pitch, YawIPad, Yaw | 97.8 |
| farp, near yaw plain float, far y volatile-int + fence | 43 | 7 | Pitch, YawIPad, Yaw | 97.5 |
| farp, near yaw load first | 44 | 7 | Pitch, YawIPad, Yaw | — |
| farp, near ternary swapped (`!iPad ? Yaw : YawIPad`) | 43 | 8 | Pitch, YawIPad, Yaw (`.rodata` swapped) | — |
| farp, far ternary swapped | 43 | 8 | Pitch, Yaw, YawIPad | 98.4 |
| farp, both ternaries swapped | 43 | 8 | Pitch, YawIPad, Yaw (`.rodata` swapped) | — |
| farp, near pitch plain float | 666 | 87 | Pitch, YawIPad, Yaw | — |
| farp, both loads plain float | 667 | 86 | Pitch, YawIPad, Yaw | — |
| farp, pitch plain int + yaw plain float | 667 | 86 | — | — |
| farp, pitch plain int + yaw volatile int | 666 | 87 | — | — |
| farp, pitch volatile int + yaw plain int | 44 | 7 | Pitch, YawIPad, Yaw | — |
| farp, near yaw volatile-int via local ptr | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, near yaw inline ternary (`(iPad ? ... : ...)[race]`) | 3631 | 201 | — | — |
| farp, near yawTable after pitch load | 3631 | 201 | — | — |
| farp, far y volatile-int inline | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, far y plain-int bitcast | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, far x `*(const float *)&pitchBits` | 44 | 7 | Pitch, YawIPad, Yaw | — |
| farp, `const float pitch` | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, `(bool) Globals::iPad` ternary | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, `Globals::iPad == 1` ternary | 3630 | 202 | — | — |
| farp, `float yaw` declared before the table pointers | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, extra blank statement after near call | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, `float farPitch = pitch` after near call | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, `const float *volatile` tables | 671 | 94 | — | — |
| farp, `int farYawBits` volatile local | 3630 | 202 | — | — |
| farp, near yaw plain float, far x = yaw | 672 | 85 | — | — |
| farp, near yaw plain float, far y = yaw | 2109 | 99 | — | — |
| farp, near yaw volatile, far x = yaw, far y = yaw | 1995 | 83 | — | — |
| farp, `volatile int yawKeep = yawBits` after near call | 558 | 53 | — | — |
| baseline with `reinterpret_cast` bit loads | 11 | 4 | Pitch, YawIPad, Yaw | 99.2 |
| baseline with `const float` locals | 11 | 4 | Pitch, YawIPad, Yaw | 99.2 |
| baseline with far y via a local ternary pointer | 11 | 4 | Pitch, YawIPad, Yaw | 99.2 |
| baseline with far y volatile-int inline | 11 | 4 | Pitch, YawIPad, Yaw | 99.2 |
| baseline with declare-then-assign float locals | 11 | 4 | Pitch, YawIPad, Yaw | 99.2 |
| farp, declare-then-assign float locals | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, `static_cast<RotationOrder>(2)` | 43 | 8 | Pitch, Yaw, YawIPad | — |
| farp, `const float *const` table pointers | 43 | 8 | Pitch, Yaw, YawIPad | — |

Prior-wave variants (fences, `__builtin_assume`, `Vector` overload, 4-arg
overload, `memcpy`/`union` bitcasts, `posTable`/`camPos` locals, named x/y/z
locals, NRVO, `int` tables, `volatile` references, `race` type changes,
`Matrix` reorder, etc.) are tabulated in
`modstation-ctor-wave119-w6717.md` §3 and `modstation-ctor-wave112-w6018.md`;
none of them beats 11 B / 4 runs either.

## 5. Conclusion

The ctor is at a hard local optimum for this compiler. The 11-byte baseline
uses `far = yaw` to preserve the original's DAG order, literal pool and whole
prefix, at the cost of one extra `mov r3,r4` and a shifted `str.w r8`. The
faithful `far = pitch` shape has the original's register shape but a
permuted literal pool (43 B / 98.4 %); the best far=pitch variant is 39 B /
98.4 % (fence before the table declarations). Making the near yaw load a
plain float restores the correct literal order but the pre-RA scheduler then
sinks the `Yaw` LDRLIT below the volatile pitch load (44 B / 97.8 %).
Swapping the ternary spelling flips the literal order back but also swaps the
two 40-byte yaw tables in `.rodata`, which would regress `data_exact`.

The only remaining lever is the post-RA placement of `str.w r8` and the yaw
load, which is not reachable from source without changing build flags (and
`-mllvm -misched-postra=false` / `-disable-post-ra` were already measured in
the prior wave).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6723
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4503 linked 4503
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```

`.rodata` table check (index 8 is 2.35 for `gHangarCamYawIPad`, 2.5 for
`gHangarCamYaw`):

```python
import struct
b = open('cmake-build-match/libgof2hdaa.so','rb').read()
struct.unpack_from('<10f', b, 0x1fe058)[8]   # 2.35 expected
struct.unpack_from('<10f', b, 0x1fe080)[8]   # 2.5  expected
```
