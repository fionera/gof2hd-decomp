# ModStation ctor (`_ZN10ModStationC1Ev` / `C2Ev`) — wave 92 (w4220)

Status: **analysis pass, no code change landed.** The ctor is still 1008 B with
**39 differing bytes / 8 runs** against the original (98.6 %). A strict improvement
requires a full byte match (`byte_exact` only counts whole functions), and no source
variant tried here reached 0. This note records the exact residual, the mechanism that
produces it, and the experiments that pin it down.

Worktree `wt/w4220`, branch `wave92-w4220`, base master `b379d00d`.
Every number below was measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 and the official `tools/verify/verify.py` (`autojunk=False`).
Scratch drivers live in `/dev/shm/w4220` (`fdiff.py`, `tryvar.py`, `llpatch.py`,
`irexp.py`, `renum2.py`, `dumpfn.py`).

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4498 / 4524 | **4498 / 4524** |
| linked_exact | 4498 / 4524 | **4498 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (-8).
`cmake --build cmake-build-match --target check` prints `RATCHET PASS`.
No source change was landed, so the baseline is untouched.

## 1. The residual: 39 bytes, 8 runs

`fdiff.py _ZN10ModStationC1Ev` on the pristine tree:

```
orig 1008 0xd52d0 ours 1008 0xd52d0
diff bytes 39 runs 8
+0x169..+0x169 (1B)  orig=4b  ours=4c
+0x16b..+0x16b (1B)  orig=4c  ours=4b
+0x16e..+0x179 (12B) orig=7b449af800107c4450f82820  ours=9af800107b4450f828207c44
+0x186..+0x186 (1B)  orig=50  ours=cd
+0x188..+0x18c (5B)  orig=28300220cd  ours=b880144650
+0x18e..+0x19e (17B) orig=b88000210df58078cde9001050a9404614  ours=2830022000210df58078cde9001050a940
+0x3e4..+0x3e4 (1B)  orig=16  ours=34
+0x3e8..+0x3e8 (1B)  orig=38  ours=12
```

`C1` is an alias of `C2`; both are 1008 B at `0xd52d0`. Only the near-camera-rotation
block and the literal-pool slots differ. The far block, the camKey block, and the rest
of the 1008 bytes are identical.

## 2. Disassembly: original vs ours (pristine tree)

```
orig                                                    ours
d5436 489e  ldr r0,[pc,#632]  ; d56b0 Pitch            d5436 489e  ldr r0,[pc,#632]  ; d56b0 Pitch
d5438 4b9e  ldr r3,[pc,#632]  ; d56b4 YawIPad          d5438 4c9e  ldr r4,[pc,#632]  ; d56b4 Yaw
d543a 4c9f  ldr r4,[pc,#636]  ; d56b8 Yaw              d543a 4b9f  ldr r3,[pc,#636]  ; d56b8 YawIPad
d543c 4478  add r0,pc                                  d543c 4478  add r0,pc
d543e 447b  add r3,pc                                  d543e f89a1000 ldrb.w r1,[sl]
d5440 f89a1000 ldrb.w r1,[sl]                          d5442 447b  add r3,pc
d5444 447c  add r4,pc                                  d5444 f8502028 ldr.w r2,[r0,r8,lsl#2]
d5446 f8502028 ldr.w r2,[r0,r8,lsl#2]                  d5448 447c  add r4,pc
d544a 4618  mov r0,r3                                  d544a 4618  mov r0,r3
d544c 2900  cmp r1,#0                                  d544c 2900  cmp r1,#0
d544e 932d  str r3,[sp,#180]                           d544e 932d  str r3,[sp,#180]
d5450 942c  str r4,[sp,#176]                           d5450 942c  str r4,[sp,#176]
d5452 bf08  it eq                                      d5452 bf08  it eq
d5454 4620  moveq r0,r4                                d5454 4620  moveq r0,r4
d5456 f8503028 ldr.w r3,[r0,r8,lsl#2]                  d5456 f8cd80b8 str.w r8,[sp,#184]
d545a 2002  movs r0,#2                                 d545a 4614  mov r4,r2
d545c f8cd80b8 str.w r8,[sp,#184]                      d545c f8503028 ldr.w r3,[r0,r8,lsl#2]
d5460 2100  movs r1,#0                                 d5460 2002  movs r0,#2
d5462 f50d7880 add.w r8,sp,#256                        d5462 2100  movs r1,#0
d5466 e9cd1000 strd r1,r0,[sp]                         d5464 f50d7880 add.w r8,sp,#256
d546a a950  add r1,sp,#320                             d5468 e9cd1000 strd r1,r0,[sp]
d546c 4640  mov r0,r8                                  d546c a950  add r1,sp,#320
d546e 4614  mov r4,r2                                  d546e 4640  mov r0,r8
d5470 f78dedf6 blx MatrixSetRotation                   d5470 f78dedf6 blx MatrixSetRotation
```

Register assignment is identical (`r3` = YawIPad, `r4` = Yaw, `r8` = race, `sl` =
`&Globals::iPad`). The differences are:

1. **literal-slot order** — the original materialises YawIPad first (`d5438`) and Yaw
   second (`d543a`); ours materialises Yaw first and YawIPad second;
2. the `add rX,pc` / `ldrb` interleave follows from (1);
3. a **post-RA schedule** difference in the tail: the original issues the yaw load at
   `d5456` and defers `str.w r8,[sp,#184]` to `d545c` and `mov r4,r2` to `d546e`;
   ours issues `str.w r8` / `mov r4,r2` immediately after the select and the yaw load
   at `d545c`.

## 3. Literal-pool evidence

Pool bytes (read straight from the ELF; `add rX,pc` at `d543c/d543e/d5444`):

| slot | original | target | ours | target |
|---|---|---|---|---|
| `0xd56b0` | `0x00128bf0` | `0x1fe030` `gHangarCamPitch` | `0x00128bf0` | `0x1fe030` `gHangarCamPitch` |
| `0xd56b4` | `0x00128c16` | `0x1fe058` `gHangarCamYawIPad` | `0x00128c34` | `0x1fe080` `gHangarCamYaw` |
| `0xd56b8` | `0x00128c38` | `0x1fe080` `gHangarCamYaw` | `0x00128c12` | `0x1fe058` `gHangarCamYawIPad` |

The `.rodata` arrays themselves are at identical addresses in both binaries
(`gHangarCamPitch @ 0x1fe030`, `gHangarCamYawIPad @ 0x1fe058`, `gHangarCamYaw @
0x1fe080`; verified by reading the float vectors). So this is purely **which slot the
two addresses land in**, i.e. the order in which the two `LDRLIT` pseudos are emitted.

## 4. Mechanism: the LDRLIT order is fixed at SelectionDAG/ISel time

`-Xclang -disable-llvm-passes` on the final optimised `.ll` reproduces the base exactly
(39/8, pool Pitch/Yaw/YawIPad), so the residual is decided in the backend, not by any
later IR pass.

`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN10ModStationC2Ev` shows the
ISel output for the near block (`bb.12`):

```
base (ours):                       yaw-load-non-volatile variant:
%114 = LDRLIT @iPad                %114 = LDRLIT @iPad
%115 = LDRLIT @Pitch               %115 = LDRLIT @Pitch
%116 = LDR [%114]                  %116 = LDR [%114]
%117 = LDRLIT @Yaw                 %117 = LDRLIT @YawIPad
%118 = LDRLIT @YawIPad             ...
CompilerBarrier 7                  CompilerBarrier 7
%119 = LDRB [%116]                 %118 = LDRB [%116]
%120 = LDR [%115,%11,2]  ; pitch   %119 = LDR [%115,%11,2]  ; pitch
CMP %119,0                         %120 = LDRLIT @Yaw
%121 = MOVCC %118,%117             CMP %118,0
%122 = LDR [%121,%11,2]  ; yaw     %121 = MOVCC %117,%120
                                   %122 = LDR [%121,%11,2]  ; yaw
```

The ISel order is the reverse topological order of the SelectionDAG
(`SelectionDAG::AssignTopologicalOrder`, then `SelectionDAGISel::DoInstructionSelection`
walks the sorted list from the root backwards). The `LDRLIT`s are emitted in exactly
that order, so the pool order is a direct readout of the DAG topological order.

## 5. What does **not** control the order: the select predicate / arm order

The front end emits `icmp ne i8 %x, 0` + `select ne, YawIPad, Yaw` for
`Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw` (verified with
`-Xclang -disable-llvm-passes`). InstCombine's `visitSelectInst`
(`llvm/lib/Transforms/InstCombine/InstCombineSelect.cpp:1535-1550`, LLVM 7) then
canonicalises a **one-use** integer compare whose predicate is non-canonical
(`isCanonicalPredicate`, `InstCombineInternal.h:96-111`: `ICMP_NE` is non-canonical):

```cpp
if (match(CondVal, m_OneUse(m_ICmp(Pred, m_Value(), m_Value()))) &&
    !isCanonicalPredicate(Pred)) {
  Cond->setPredicate(CmpInst::getInversePredicate(Pred));
  SI.setOperand(1, FalseVal);
  SI.setOperand(2, TrueVal);
  ...
}
```

so the final IR is `icmp eq i8 %x, 0` + `select eq, Yaw, YawIPad`. `print-after-all`
confirms the flip happens in the first `Combine redundant instructions` pass
(`base_all.txt`: front-end `%143 = select i1 %142, YawIPad, Yaw` at line 909 becomes
`%119 = select i1 %118, Yaw, YawIPad` at line 6911).

**However, the select form does not decide the pool order.** Hand-written IR variants
compiled with the project flags (`irexp.py`, `llpatch.py`), pool order for the near
block:

| near-select IR | pool order |
|---|---|
| `icmp eq` + `select eq, Yaw, YawIPad` (base) | Pitch, Yaw, YawIPad |
| `icmp ne` + `select ne, YawIPad, Yaw` (semantically correct, non-canonical) | Pitch, Yaw, YawIPad |
| `icmp eq` + `select eq, YawIPad, Yaw` (semantically wrong) | **Pitch, YawIPad, Yaw** |
| `icmp ne` + `select ne, Yaw, YawIPad` (semantically wrong) | Pitch, Yaw, YawIPad |

So `select`-operand order is *not* the lever; the DAG context is.

## 6. What does flip the order (new in this pass)

Two IR-level changes flip the near-block pool order to the original's
`Pitch, YawIPad, Yaw`:

1. **Move the pitch load before the iPad load / select** (i.e. before the fence's
   consumers). `irexp.py pitchload_first`. Result: correct pool order but the function
   shrinks to **1000 B** (`_ZN10ModStationD2Ev` at `0x3e8` in the `.o`), because the
   register allocation changes (race moves from `r8` to `r4`) and the far block picks
   up the difference. Source equivalent `P_PL_Y_YL` measures **1000 B / 690 diff /
   94 runs**.
2. **Make the near yaw load non-volatile** (`int yawBits = *(const int *)&yawTable[race]`
   instead of `*(const volatile int *)`). `irexp.py yaw_nv`. Result: correct pool order,
   **1008 B**, **44 diff / 7 runs** (vs base 39/8). The far block is byte-identical to
   the original; only the near block schedule differs.

`yaw_nv` near block:

```
d5436 ldr r0 Pitch
d5438 ldr r3 YawIPad           <- correct slot order
d543a add r0,pc
d543c ldrb r1,[sl]
d5440 add r3,pc
d5442 ldr r2,[r0,r8,lsl#2]     ; pitch
d5446 cmp r1,#0                <- cmp before mov (orig: after mov)
d5448 ldr r4 Yaw               <- Yaw LDRLIT not hoisted (orig: d543a)
d544a mov r0,r3
d544c str r3,[sp,#180]
d544e add r4,pc
d5450 str r4,[sp,#176]
d5452 it eq
d5454 moveq r0,r4
d5456 mov r4,r2                <- orig: d546e
d5458 ldr r3,[r0,r8,lsl#2]     ; yaw
d545c str.w r8,[sp,#184]
d5460 movs r0,#2
...
```

The remaining 44 bytes are the `ldr r4` hoist, the `cmp`/`mov` order, and the
`mov r4,r2` placement — all post-RA scheduler decisions that follow from where the
`LDRLIT @Yaw` sits in the pre-RA MIR (after the pitch load in `yaw_nv`, at the top in
the original).

## 7. Counter-example: `OnInitialize` is byte-exact with the same pattern

`ModStation::OnInitialize` (100.0 %, 6396 B) contains the *same*
`Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw` expression twice (source lines
792 and 797) and its near block has exactly the original's pool order:

```
d63e4 ldr r0 ...          ; (Globals::Canvas)
d63e6 ldr r1 Pitch
d63ea ldr r4 YawIPad      ; 0x1fe058
d63ee ldr r5 Yaw          ; 0x1fe080
...
d6402 ldrb r0,[sl]
d640a cmp r0,#0
d640c mov r0,r4           ; YawIPad
d6410 moveq r0,r5         ; Yaw
```

Its IR has the pitch load **before** the select and both loads non-volatile
(`%1499 = gep Pitch; %1500 = load float; ... %1503 = select eq, Yaw, YawIPad;
%1505 = load float`). That is consistent with finding 6.1/6.2: the ctor's near block
differs from `OnInitialize`'s by putting the pitch load after the select and marking
both loads volatile.

## 8. Variant table (all measured with the official verifier / `fdiff.py`)

Base is 1008 B / 39 diff / 8 runs. All variants are 1008 B unless noted.

| variant | raw diff | runs |
|---|---:|---:|
| **base (landed)** | **39** | **8** |
| `pitch_vol_only` (yaw non-volatile) | 44 | 7 |
| `yaw_nv` / `yaw_float` / `yaw_float2` / `yaw_int_nv` (near yaw non-volatile) | 44 | 7 |
| `nv_float_local` | 44 | 7 |
| `pvo_f_after_tables` | 43 | 7 |
| `nv_P_Y_F_PL_YL` | 43 | 7 |
| `bool_two_selects` (bool ipad + yawTable + int* yawInt) | 44 | 7 |
| `fence_after_tables` (P Y F PL YL) | 42 | 7 |
| `fence_after_yawload` | 42 | 7 |
| `P_Y_F_PL_YL` | 42 | 7 |
| `P_Y_PL_YL_F` | 42 | 7 |
| `P_Y_PL_F_YL` | 44 | 7 |
| `P_F_Y_PL_YL` | 39 | 8 |
| `decls_yaw_first` / `yaw_then_pitch_decl` | 42 | 9 |
| `yaw_before_pitch` | 42 | 9 |
| `bool_before_fence` | 42 | 7 |
| `no_fence` | 43 | 8 |
| `nofence_yaw_nv` | 44 | 7 |
| `f_before_call` | 103 | 28 |
| `near_int_far_float` | 44 | 7 |
| `near_float_far_int` | 575 | 31 |
| `near_int_far_int` | 976 B | 705 | 44 |
| `direct_yaw` / `direct_both` | 1000 B | 690 | 94 |
| `yaw_ref` | 984 B | 706 | 67 |
| `cast_ptr` / `cast_addr` / `float_then_bits` / `bits_then_float` | 631 | 81 |
| `yaw_vol_only` | 630 | 82 |
| `loads_yaw_first` / `both_yaw_first` | 1000 B | 744 | 40 |
| `P_PL_Y_YL` (pitch load before select) | 1000 B | 690 | 94 |
| `nofence_plf` / `nofence_plf_yaw_nv` | 1000 B | 689-690 | 92-94 |
| `nv_direct` | 1000 B | 689 | 92 |
| `Globals::iPad == 1` | 1000 B | 691 | 91 |
| `Globals::iPad & 1` | 570 | 22 |
| `shared_bool` / `far_reuse` / `yref` / `twosel` / `arrsel` / `share2` | 984 B | 580 | 72 |
| `ne_live_store` | 1016 B | 700 | — |
| `bool iPad` in Globals | 1008 @0xd52e0 | 73 | 41 |
| `__ATOMIC_RELAXED` | 43 | — |
| `__atomic_thread_fence` | 516 | — |
| `two_vol` (bool ipad + int* yawInt, yawBits volatile) | 1016 B | 621 | 40 |
| `two_vol_fix` | 575 | 31 |
| `i32sel` / `i64` / `char` | 984 B | 650 | 47 |
| `near_shared` | 562 | 98 |
| `shared_all` | 888 B | 648 | 53 |
| `sgt` / `sgt_ge` | 1000 B | 691 | 91 |
| `and1` (`Globals::iPad & 1`) | 570 | 22 |
| `st_local` | 70 | 14 |
| `plf_again` / `pitchload_first` | 1000 B | 690 | 94 |
| `st_plf` | 1000 B | 719 | 98 |
| `f_vol` | 992 B | 701 | 40 |
| `f_int` / `f_byte` | 984 B | 580 | 72 |
| `f_plus` | 996 B | 693 | 44 |
| `staticptr` | 1060 B | 737 | 36 |
| `swap_eq` (`Globals::iPad == 0 ? YawIPad : Yaw`) | 1000 B | 691 | 91 |
| `float_nv` / `int_nv` (plain non-volatile loads) | 631 | 81 |
| `float_pvol` (volatile float pitch, plain yaw) | 205 | 19 |
| `float_vol` / `float_yvol` | 1016 B | 744/749 | 41/40 |
| `const_yaw` / `addr0` / `plus0` / `ipad_local` / `comma` / `if_else_ptr` / `if_else_ptr2` | 39 | 8 (neutral) |
| `pitch_after_yawload` | 1000 B | 744 | 40 |
| `nv_both_pitchfirst` (both loads non-volatile, pitch first) | 1008 | wrong pool order |

Neutral (39 diff / 8 runs, no change): `const_bits`, `scope_block`, `helper_inline`,
`pitch_ptr_const`, `const_ptr2`, `init_order1/2`, `init_none`, `arr_unsized`,
`arr_yaw_first`, `order_var`, `zero_var`, `pitch_var`, `race_before_nearkey`,
`race_init0`, `far_const_cast`, `far_eq0`, `far_second_load`, `far_pitch_first`,
`fence_mid`, `expect_ne`, `expect_cond`, `ipad_bool_near_only`, `yawtable_use_ref`,
`bool_ne`, `bool_implicit`, `bool_const`, `vol_local`, `vol_local_bool`,
`vol_local_int`, `vol_ptr`, `yaw_vol_ptr`, `yaw_vol_int_ptr`, `yaw_vol_both_ptr`,
`pitch_vol_both_ptr`, `y_0..y_4`, `f_between_pitch_decl_load`, `f_after_pitchdecl`,
`boolcast`, `boolcast2`, `constptr`, `lambda`, `comma`, `ternary2`, `ifelse`,
`ifelse2`, `addr0`, `plus0`, `ref`, `emptyif`, `emptyif2`, `gepfirst`, `gepfirst2`,
`bvar`, `Globals::iPad > 0`, `!= 0`, `>= 1`, `!!`, `(bool)`, `> 0u`, `>= 1u`,
`!= 0u`, `(int) > 0`, `!= 1`, `p_vol_y_vol`, `p_nv_y_nv`, `p_nv_y_vol`,
`ifelse_true_default`, `ifelse_false_default`, `yawipad_first_ref`, `sel_then_pl`,
`ipad_local`.

Flag sweep (per-TU `COMPILE_OPTIONS` in `CMakeLists.txt`, then restored):

| flag | diff | runs |
|---|---:|---:|
| `-disable-post-ra` | 423 | 77 |
| `-enable-post-misched=false` | 39 | 8 |
| `-misched-postra` | 58 | 9 |
| `-post-RA-scheduler=false` | 423 | 77 |
| `-misched=ilpmin/ilpmax/converge` | 39 | 8 |
| `-misched-bottomup/topdown` | 39 | 8 |
| `-break-anti-dependencies=all/none` | 39 | 8 |
| `-max-sched-reorder=0/1000` | 39 | 8 |
| `-misched-cluster` | 39 | 8 |
| `-disable-2addr-hack` | 39 | 8 |
| `-regalloc=basic` | 905 | 68 |
| `-enable-greedy-fp=false` | 905 | 68 |
| `-split-spill-mode=size` | 43 | 11 |
| `-split-spill-mode=speed` | 39 | 8 |
| `-pre-RA-sched=source` | 877 | 59 |
| `-pre-RA-sched=list-ilp` | 836 | 91 |
| `-pre-RA-sched=list-hybrid` | 39 | 8 |
| `-pre-RA-sched=list-burr` | 856 | 87 |
| `-pre-RA-sched=fast` | 856 | 87 |
| `-enable-misched=false` | 39 | 8 |

## 9. Negative / cross-check evidence

* **iOS 1.1.4 HD** (`/home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD`, Mach-O
  armv7) has the same `gHangarCamPitch` constant blob at file offset `0x3eca54` but no
  `ModStation` symbol exported (`llvm-nm`, 1527 lines) — no independent instruction
  oracle.
* The original `.so` `.comment` is exactly the tree's NDK r18b clang 7.0.2
  (`r328903`, llvm `1d739ffb`); there are no `.debug_*` sections in the original.
* `-Xclang -disable-llvm-passes` on the final `.ll` reproduces the base exactly
  (39/8, pool Pitch/Yaw/YawIPad), proving the residual is backend-only.
* The pre-RA `Machine Instruction Scheduler` dump for `bb.15` already contains
  `tLDRLIT @gHangarCamPitch`, then `@gHangarCamYaw`, then `@gHangarCamYawIPad`, and the
  post-RA `top-down list latency scheduler` preserves that order — so the flip is
  upstream of the schedulers, at ISel.

## 10. Conclusion

The residual is **not** missing code, a wrong constant, or a wrong callee. It is the
SelectionDAG topological order of the two `LDRLIT` nodes for `gHangarCamYawIPad` and
`gHangarCamYaw` in the near-camera block, plus the post-RA schedule that follows from
it. The order is set at ISel and is a function of the whole basic block's DAG, not of
the `select` operand order (proved by `icmp ne` + `select ne, YawIPad, Yaw` still
yielding Pitch/Yaw/YawIPad).

The two known levers — (a) moving the pitch volatile load before the iPad load/select,
(b) making the near yaw load non-volatile — both produce the original's pool order.
(a) shrinks the function to 1000 B; (b) keeps 1008 B and leaves only a 44-byte
post-RA schedule delta. A source shape that combines (b)'s correct ISel order with the
base's `tMOVr`+`tMOVCCr` select lowering (so the `LDRLIT @Yaw` is hoisted to the top
and `cmp`/`mov r4,r2` land where the original has them) has not been found.

## 11. Next steps

1. Search for a source shape that keeps the near yaw load volatile (base schedule) but
   forces the DAG to order the `YawIPad` `LDRLIT` before the `Yaw` one. The most
   promising untried direction is a second, non-CSE-able use of the compare that
   survives InstCombine's `m_OneUse` gate without merging the two yaw selects.
2. Alternatively, start from `yaw_nv` (correct pool order, 1008 B) and find a source
   tweak that restores the base's `tMOVr`+`tMOVCCr` lowering and the hoisted
   `LDRLIT @Yaw`; the far block already matches byte-for-byte.
3. Do not land `yaw_nv` as-is: it keeps `byte_exact` at 4498 and raises the raw diff
   from 39 to 44 bytes, i.e. it is not a strict improvement.
