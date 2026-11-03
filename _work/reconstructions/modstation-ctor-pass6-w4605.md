# ModStation ctor (`_ZN10ModStationC1Ev` / `C2Ev`) — wave 96 pass 6 (w4605)

Status: **code change landed.** The ctor is still 1008 B and still not byte-exact,
but the raw residual drops **43 differing bytes / 8 runs -> 39 differing bytes /
8 runs** by restoring the compiler barrier that the anchor-audit commit
`b67ea4710` had removed. `byte_exact` is unchanged (4500/4524) because the
function is still not exact; the change is a strict raw-diff improvement and
gate-neutral.

Worktree `wt/w4605`, branch `wave96-w4605`, base master `3bcaeb3a`.
All numbers measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the
tree's NDK r18b clang 7.0.2 and the official verifier.

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4500 / 4524 | **4500 / 4524** |
| linked_exact | 4500 / 4524 | **4500 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`cmake --build cmake-build-match --target check` prints `RATCHET PASS`.
`.text`, `.rodata`, `.ARM.exidx`, `.ARM.extab` sizes unchanged.

## 1. The change

`src/game/menu/ModStation.cpp`, `ModStation::ModStation()`:

```cpp
    MatrixSetTranslation(nearKey, this->camKeyX, this->camKeyY, this->camKeyZ);
    __atomic_signal_fence(__ATOMIC_SEQ_CST);          // <- restored (pass 3 shape)
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
```

This is exactly the shape landed by `b379d00de` ("ModStation ctor pass 3:
fence before table pointers (42 -> 39 diff bytes)") and dropped by
`b67ea4710` ("Anchor audit pass 1: drop nine non-load-bearing codegen
anchors"). The audit only checked gate counts; the fence is not load-bearing
for `byte_exact` but it *is* load-bearing for the raw residual (43 -> 39).
`__atomic_signal_fence` emits no instruction (a compiler barrier only); it is
a builtin, not an `asm` spelling and not `__attribute__`, so lint stays CLEAN.

## 2. Residual: pristine (no fence) vs with fence

`fdiff.py _ZN10ModStationC1Ev` (byte-wise diff against the original, offsets
relative to the function start at `0xd52d0`):

pristine (`b67ea4710` shape), **43 bytes / 8 runs**:

```
+0x169..+0x169 (1B)  orig=4b ours=4c
+0x16b..+0x16b (1B)  orig=4c ours=4b
+0x16e..+0x17d (16B) orig=7b449af800107c4450f8282018460029 ours=9af800107c4450f828207b4400291846
+0x186..+0x186 (1B)  orig=50 ours=cd
+0x188..+0x18c (5B)  orig=28300220cd ours=b880144650
+0x18e..+0x19e (17B) orig=b88000210df58078cde9001050a9404614 ours=2830022000210df58078cde9001050a940
+0x3e4..+0x3e4 (1B)  orig=16 ours=3a
+0x3e8..+0x3e8 (1B)  orig=38 ours=0c
```

with the fence, **39 bytes / 8 runs**:

```
+0x169..+0x169 (1B)  orig=4b ours=4c
+0x16b..+0x16b (1B)  orig=4c ours=4b
+0x16e..+0x179 (12B) orig=7b449af800107c4450f82820 ours=9af800107b4450f828207c44
+0x186..+0x186 (1B)  orig=50 ours=cd
+0x188..+0x18c (5B)  orig=28300220cd ours=b880144650
+0x18e..+0x19e (17B) orig=b88000210df58078cde9001050a9404614 ours=2830022000210df58078cde9001050a940
+0x3e4..+0x3e4 (1B)  orig=16 ours=34
+0x3e8..+0x3e8 (1B)  orig=38 ours=12
```

The fence removes the 4-byte `18460029` (`mov r0,r3; cmp r1,#0`) from the
first run: without it the scheduler hoists `mov r0,r3` past `ldrb`/`ldr r2`
and the whole block shifts by 4 bytes. The remaining 39 bytes are the same
two effects described in pass 5: the `LDRLIT` order of the two yaw tables and
the post-RA schedule of the tail.

## 3. Disassembly, original vs ours (with fence)

```
orig                                                    ours (fence_base)
d5436 489e  ldr r0,[pc,#632]  ; Pitch                   d5436 489e  ldr r0,[pc,#632]  ; Pitch
d5438 4b9e  ldr r3,[pc,#632]  ; YawIPad                 d5438 4c9e  ldr r4,[pc,#632]  ; Yaw
d543a 4c9f  ldr r4,[pc,#636]  ; Yaw                     d543a 4b9f  ldr r3,[pc,#636]  ; YawIPad
d543c 4478  add r0,pc                                   d543c 4478  add r0,pc
d543e 447b  add r3,pc                                   d543e f89a1000 ldrb.w r1,[sl]
d5440 f89a1000 ldrb.w r1,[sl]                           d5442 447b  add r3,pc
d5444 447c  add r4,pc                                   d5444 f8502028 ldr.w r2,[r0,r8,lsl#2]
d5446 f8502028 ldr.w r2,[r0,r8,lsl#2]                   d5448 447c  add r4,pc
d544a 4618  mov r0,r3                                   d544a 4618  mov r0,r3
d544c 2900  cmp r1,#0                                   d544c 2900  cmp r1,#0
d544e 932d  str r3,[sp,#180]                            d544e 932d  str r3,[sp,#180]
d5450 942c  str r4,[sp,#176]                            d5450 942c  str r4,[sp,#176]
d5452 bf08  it eq                                       d5452 bf08  it eq
d5454 4620  moveq r0,r4                                 d5454 4620  moveq r0,r4
d5456 f8503028 ldr.w r3,[r0,r8,lsl#2]                   d5456 f8cd80b8 str.w r8,[sp,#184]
d545a 2002  movs r0,#2                                  d545a 4614  mov r4,r2
d545c f8cd80b8 str.w r8,[sp,#184]                       d545c f8503028 ldr.w r3,[r0,r8,lsl#2]
d5460 2100  movs r1,#0                                  d5460 2002  movs r0,#2
d5462 f50d7880 add.w r8,sp,#256                         d5462 2100  movs r1,#0
d5466 e9cd1000 strd r1,r0,[sp]                          d5464 f50d7880 add.w r8,sp,#256
d546a a950  add r1,sp,#320                              d5468 e9cd1000 strd r1,r0,[sp]
d546c 4640  mov r0,r8                                   d546c a950  add r1,sp,#320
d546e 4614  mov r4,r2                                   d546e 4640  mov r0,r8
d5470 f78dedf6 blx MatrixSetRotation                     d5470 f78dedf6 blx MatrixSetRotation
```

Register assignment is identical (`r3` = YawIPad = `0x1fe058`, `r4` = Yaw =
`0x1fe080`, `r8` = race, `sl` = `&Globals::iPad`). The stack stores
(`sp+180` = YawIPad, `sp+176` = Yaw) and the far block (which reloads them with
`ldrd r0,r1,[sp,#176]` and re-selects) are byte-identical.

## 4. Literal pool

```
slot      original    ours (fence_base)
d56b0     0x00128bf0  0x00128bf0    Pitch  (0x1fe030)
d56b4     0x00128c16  0x00128c34    YawIPad vs Yaw
d56b8     0x00128c38  0x00128c12    Yaw vs YawIPad
```

The `.rodata` arrays are at identical addresses in both binaries, so the two
slots are swapped. This is the `LDRLIT` emission order.

## 5. Mechanism (new evidence this pass)

### 5.1 Why the front-end `icmp ne` becomes `icmp eq`

The source `Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw` is emitted by
clang as `icmp ne` + `select(ne, YawIPad, Yaw)` (verified on the unoptimised
IR: `%217 = icmp ne i8 %216, 0` / phi `[YawIPad if ne], [Yaw if eq]`).
InstCombine then canonicalises it. The exact rule (LLVM 7.0.2,
`lib/Transforms/InstCombine/InstCombineSelect.cpp`, `visitSelectInst`):

```cpp
  // Canonicalize a one-use integer compare with a non-canonical predicate by
  // inverting the predicate and swapping the select operands.
  CmpInst::Predicate Pred;
  if (match(CondVal, m_OneUse(m_ICmp(Pred, m_Value(), m_Value()))) &&
      !isCanonicalPredicate(Pred)) {
    CmpInst *Cond = cast<CmpInst>(CondVal);
    Cond->setPredicate(CmpInst::getInversePredicate(Pred));
    SI.setOperand(1, FalseVal);
    SI.setOperand(2, TrueVal);
    ...
  }
```

`isCanonicalPredicate` (`InstCombineInternal.h`) returns false for
`ICMP_NE`, so a *one-use* `icmp ne` is inverted to `icmp eq` and the select
operands are swapped: `select(ne, YawIPad, Yaw)` -> `select(eq, Yaw, YawIPad)`.
The `m_OneUse` gate is the reason the front-end operand order is lost. If the
compare had two uses the `ne` form would survive, but the ARM backend then
lowers the condition as `NE` (`it ne`), not the original's `it eq`.

### 5.2 ARM lowering keeps the DAG operand order

`ARMISelLowering.cpp` `LowerSELECT_CC` ends with

```cpp
    return getCMOV(dl, VT, FalseVal, TrueVal, ARMcc, CCR, Cmp, DAG);
```

so the `t2MOVCCr` pseudo is `(FalseVal, TrueVal, cc)` and the final code is
`mov dst, FalseVal; movcc dst, TrueVal`. For our IR
(`select(eq, Yaw, YawIPad)`) that is `mov dst, YawIPad; moveq dst, Yaw` --
exactly the original's `mov r0,r3; it eq; moveq r0,r4`.

### 5.3 The LDRLIT order is a *scheduler* decision, not an IR property

This is the key new observation. The `camKey` block immediately above the
near-camera block has the *same* IR shape
(`select(eq, Pos, PosIPad)`, true = `gHangarCamPos`) and is byte-exact, yet its
post-ISel MIR emits the two `tLDRLIT`s in the **false-then-true** order:

```
%91 = tLDRLIT @_ZL17gHangarCamPosIPad     <- false
%92 = tLDRLIT @_ZL13gHangarCamPos         <- true
%94 = t2MOVCCr %91, %92, 0(EQ)
```

while the near yaw block of the same build emits **true-then-false**:

```
%117 = tLDRLIT @_ZL13gHangarCamYaw        <- true
%118 = tLDRLIT @_ZL17gHangarCamYawIPad    <- false
%121 = t2MOVCCr %118, %117, 0(EQ)
```

Both come from `SelectionDAGBuilder::visitSelect`, which visits
`getOperand(1)` (true) before `getOperand(2)` (false), so the DAG creation
order is the same. The difference is the pre-RA scheduler
(`ScheduleDAGRRList`) tie-breaking, which depends on the surrounding memory
operations. That is why no pure IR reordering of the near block reproduces the
original order together with the original tail: the schedule is sensitive to
the whole block, including the volatile loads and the debug intrinsics.

A compiler barrier (`__atomic_signal_fence`) is enough to perturb that
tie-breaking: with the fence before the table declarations the near block
reverts to the pass-3 39-byte residual.

## 6. Variant table (all 1008 B unless noted)

Raw diff/runs vs the original; `pool` is the order of the two yaw slots
(`OK` = YawIPad then Yaw, matching the original).

| variant | diff | runs | pool |
|---|---:|---:|---|
| pristine (no fence) | 43 | 8 | wrong |
| **fence before `pitchTable` (landed)** | **39** | **8** | wrong |
| fence between `pitchTable` and `yawTable` | 39 | 8 | wrong |
| two fences before the declarations | 39 | 8 | wrong |
| fence after the pitch load | 44 | 7 | wrong |
| fence after both loads / after the yaw load | 42 | 7 | **OK** |
| two fences, one after each load | 43 | 6 | **OK** |
| fence after the yaw declaration | 42 | 7 | **OK** |
| `yawTable` declared first | 42 | 9 | wrong |
| `yawBits` loaded first | 744 | 40 | wrong |
| pitch load before the yaw declaration | 688..690 | 93..94 | wrong |
| `Globals::iPad != 0` / `== false` / `0 != iPad` | 39 | 8 | wrong |
| `bool pad = Globals::iPad != 0; pad ? ...` | 39 | 8 | wrong |
| `const float *const` tables | 39 | 8 | wrong |
| `float` locals from the bit patterns | 39 | 8 | wrong |
| `const volatile int *` tables | 39 | 8 | wrong |
| `const volatile int *` tables + fence | 42 | 7 | **OK** |
| non-volatile pitch load, volatile yaw | 44 | 7 | wrong |
| both loads non-volatile | 631..667 | 81..86 | wrong |
| `const volatile float *` tables | 631 | 81 | wrong |
| `const float *volatile` tables | 671 | 94 | wrong |
| value ternary `(iPad ? YawIPad[race] : Yaw[race])` | 631 | 81 | wrong |
| `(iPad ? YawIPad : Yaw)[race]` inline in the call | 690 | 94 | wrong |
| `Globals::iPad == 1` | 691 | 91 | 1000 B |

IR-level search (`irexp3.py`, base = the fence-after-yaw-load optimised IR,
120 random topological orders of the near block): the best shape with the
correct pool order is still **42 / 7**; every 39-byte shape has the pool
swapped. So 39/8 is the raw-diff optimum and 42/7 is the pool-order optimum;
they cannot be combined with the current source vocabulary.

## 7. Scripts used (content, not paths)

* `fdiff.py <mangled>` — byte/run diff against the original; uses
  `tools/verify/elf.py` (`Elf32.functions()`, `bytes_at`).
* `try.py <variant.py>` — replaces the near block
  (`const float *pitchTable = ...` .. `Matrix farKey;`) in
  `src/game/menu/ModStation.cpp`, builds `link_so`, prints `fdiff`.
* `batch2.py <variant.py>` — runs a list of `(name, text)` variants and also
  reports the two literal-pool slots at `+0x3e4`/`+0x3e8`.
* `irexp3.py <spec>` — replaces the near-block IR in the optimised `.ll` with a
  named-SSA reordering (tokens `I C S D P B L DP Y BY YL DY F1 F2 F`), renumbers
  the rest of the function, and links the object with the official gold
  command (`llbuild.sh`). Used to prove that IR reordering alone cannot beat
  42/7 with the correct pool order.
* `gen3.py` / `gen6.py` — exhaustive fence-placement sweeps (32 positions x
  the 4 declaration/load orders).

## 8. Conclusion / next steps

The residual is not missing code, a wrong constant, or a wrong callee. It is
(1) the pre-RA scheduler's `LDRLIT` tie-break between the two yaw tables and
(2) the post-RA placement of `str.w r8`, `mov r4,r2` and the yaw load. Both are
downstream of the same block schedule. The 39/8 shape is the best raw-diff
source shape and is landed; the 42/7 shape is the best shape that also
reproduces the original literal-pool order.

Next steps that could still crack it:

1. Find a source shape that makes the near block's `icmp ne` survive
   InstCombine (two uses) *and* is lowered with `it eq`. The `m_OneUse` gate in
   `visitSelectInst` is the only lever found that controls the front-end
   operand order; a two-use compare currently lowers as `it ne`.
2. Match the `camKey` block's false-then-true `LDRLIT` order in the yaw block
   by changing the memory-operation mix around the select (the camKey block
   proves the order is achievable in this TU with the same IR shape).
3. If a future pass can move the yaw load ahead of `str.w r8`/`mov r4,r2`
   without changing the block size, the 17-byte tail run collapses.
