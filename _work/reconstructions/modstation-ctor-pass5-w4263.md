# ModStation ctor (`_ZN10ModStationC1Ev` / `C2Ev`) — wave 92 pass 5 (w4263)

Status: **analysis pass, no code change landed.** The ctor is still 1008 B with
**39 differing bytes / 8 runs** against the original (98.4 %). `byte_exact` only
counts whole functions, so no strict improvement is possible until the residual
reaches 0. This note records the pass-5 evidence: the residual is the *emission
order of two `LDRLIT` pseudos* in the near-camera block; the register assignment
and `.rodata` layout are already correct; and a hand-crafted IR shape that fixes
the order (11 diff / 4 runs) exists but cannot be produced by any source shape
tried here.

Worktree `wt/w4263`, branch `wave92-w4263`, base master `b379d00d`.
All numbers measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the
tree's NDK r18b clang 7.0.2 and the official `tools/verify/verify.py`
(`autojunk=False`). Scratch drivers live in `/dev/shm/w4263`
(`fastsrc.py`, `trysrc.py`, `srcir.py`, `fastbuild.py`, `showdis.py`,
`dis2.py`, `fdiff.py`, `getmir.sh`).

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4499 / 4524 | **4499 / 4524** |
| linked_exact | 4499 / 4524 | **4499 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70` exact,
`.ARM.extab 0x26290` (-8). `cmake --build cmake-build-match --target check`
prints `RATCHET PASS`. No source change landed, baseline untouched.

## 1. Residual on the pristine tree

`python3 fdiff.py _ZN10ModStationC1Ev`:

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

`C1` and `C2` are aliases (same 1008 B at `0xd52d0`). Only the near-camera
rotation block and the two literal-pool slots differ; the far block, the camKey
block, and the rest of the function are byte-identical.

## 2. Disassembly, original vs ours (pristine)

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

The register assignment is **identical**: `r3` = `gHangarCamYawIPad`,
`r4` = `gHangarCamYaw`, `r8` = `race`, `sl` = `&Globals::iPad`. The only
structural difference is the **emission order of the two literal loads**:
the original materialises `YawIPad` (`r3`) at `d5438` and `Yaw` (`r4`) at
`d543a`; ours materialises `Yaw` (`r4`) first and `YawIPad` (`r3`) second.
The `add rX,pc` / `ldrb` interleave and the pool slots follow from that order.
A secondary post-RA schedule difference (`str.w r8` and `mov r4,r2` placement,
yaw load position) is a consequence of the same flip.

## 3. Literal-pool evidence (PC-relative)

`ldr rX,[pc,#imm]` loads a **PC-relative** 32-bit offset; `add rX,pc` then
forms the address. Reading the pool straight from the ELF:

| slot | original | target | ours | target |
|---|---|---|---|---|
| `0xd56b0` | `0x00128bf0` | `0x1fe030` `gHangarCamPitch` | `0x00128bf0` | `0x1fe030` `gHangarCamPitch` |
| `0xd56b4` | `0x00128c16` | `0x1fe058` `gHangarCamYawIPad` | `0x00128c34` | `0x1fe080` `gHangarCamYaw` |
| `0xd56b8` | `0x00128c38` | `0x1fe080` `gHangarCamYaw` | `0x00128c12` | `0x1fe058` `gHangarCamYawIPad` |

The `.rodata` arrays are at **identical addresses** in both binaries
(`gHangarCamPitch @ 0x1fe030`, `gHangarCamYawIPad @ 0x1fe058`,
`gHangarCamYaw @ 0x1fe080`, verified by reading the float vectors; the
`iPad` array has `2.35f` at index 8, the phone array `2.5f`). So the pool
values differ only because the two `LDRLIT` pseudos are emitted in the
opposite order; the data layout is already correct.

## 4. Ghidra decompile (near block, `ctor.c` lines 150-215)

```
local_104 = DAT_000e56b4 + 0xe5442;      // gHangarCamYawIPad
local_108 = DAT_000e56b8 + 0xe5448;      // gHangarCamYaw
uVar19 = *(undefined4 *)(DAT_000e56b0 + 0xe5440 + iVar5 * 4);   // Pitch[race]
iVar6 = local_104;
if (*pcVar14 == '\0') {                  // !Globals::iPad
  iVar6 = local_108;
}
local_100 = iVar5;
AbyssEngine::AEMath::MatrixSetRotation(
    (Matrix *)&local_b8, &local_78, uVar19,
    *(undefined4 *)(iVar6 + iVar5 * 4), 0, 2);
```

`pcVar14` is `&Globals::iPad`; `iVar5` is `race`. The decompile confirms the
pitch value is loaded once and reused by the far block, and that the yaw table
select is a two-arm pointer select.

## 5. Mechanism: ISel order is the lever, and it is DAG-wide

`-Xclang -disable-llvm-passes` on the final optimised `.ll` reproduces the base
exactly (39/8, pool Pitch/Yaw/YawIPad), so the residual is decided in the
backend, not by any later IR pass.

Pre-RA MIR (`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN10ModStationC2Ev`)
for the near block (`bb.12`), base:

```
%114 = tLDRLIT @_ZN7Globals4iPadE
%115 = tLDRLIT @_ZL15gHangarCamPitch
%116 = tLDRLIT @_ZL13gHangarCamYaw        <- Yaw first
%117 = tLDRLIT @_ZL17gHangarCamYawIPad
%118 = t2LDRi12 %114                       ; load iPad
CompilerBarrier 7
CompilerBarrier 7
%119 = t2LDRBi12 %118                      ; ldrb
%120 = t2LDRs %115, %11, 2                 ; volatile pitch load
t2CMPri %119, 0
%121 = t2MOVCCr %116, %117                 ; select
%122 = t2LDRs %121, %11, 2                 ; volatile yaw load
...
$r2 = COPY %120
$r3 = COPY %122
```

`SelectionDAGISel::DoInstructionSelection` emits in reverse topological order
of the SelectionDAG (`SelectionDAG::AssignTopologicalOrder`, LLVM 7
`SelectionDAG.cpp:7936`): nodes with 0 operands are moved to the front in
`allnodes` order, then a use-scan orders the rest. The `LDRLIT` pseudos are
0-operand leaves, so their `allnodes` creation order (which follows the IR
operand order of the `select`) fixes the ISel order; the pre-RA scheduler
(`ScheduleDAGRRList::ListScheduleBottomUp` + `hybrid_ls_rr_sort` / `BURRSort`)
then preserves it. `-mllvm -pre-RA-sched=source|list-ilp|list-burr` all
degrade badly (877/59, 836/91, 856/87); `list-hybrid` is the default and
yields 39/8.

## 6. New pass-5 evidence

### 6.1 Front-end IR order alone is not the lever

A `pitchPtr` source shape

```cpp
const float *pitchTable = gHangarCamPitch;
const float *pitchPtr = &pitchTable[race];
const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
int pitchBits = *(const volatile int *) pitchPtr;
int yawBits = *(const volatile int *) &yawTable[race];
```

emits front-end IR with the pitch `getelementptr` **before** the iPad
load/`icmp`/`select`:

```
fence
%98 = getelementptr @gHangarCamPitch, 0, %77
call llvm.dbg.value(%98)
%99 = load i8 @Globals::iPad
%100 = icmp eq i8 %99, 0
%101 = select %100, @gHangarCamYaw, @gHangarCamYawIPad
%102 = bitcast float* %98 to i32*
%103 = load volatile i32 %102
%104 = getelementptr %101, 0, %77
...
```

yet it compiles to **exactly the base 39/8** with the base pool order. The
scheduler normalises the DAG, so arbitrary IR statement reordering is not
enough.

### 6.2 A hand-crafted IR shape does fix the order (11 diff / 4 runs)

Starting from the final optimised `base.ll` and hand-editing the near block to
`fence; fence; gep Pitch; load iPad; icmp; select; bitcast Pitch; load volatile
pitch; gep Yaw; bitcast; load volatile yaw; bitcast yaw->float; dbg...;
bitcast pitch->float` (and keeping the semantically correct call argument
order `pitch, yaw`) gives:

```
best16d   size 1008/1008 diff 11 runs 4
pool ['0x128bf0', '0x128c16', '0x128c38']      <- exactly the original pool
+0x186..+0x186 orig=50 ours=cd
+0x188..+0x18f orig=28300220cdf8b880 ours=b88050f828400220
+0x19c..+0x19c orig=40 ours=23
+0x19e..+0x19e orig=14 ours=40
```

Its pre-RA MIR has `%116 = @YawIPad`, `%117 = @Yaw` (opposite of base), so the
two `LDRLIT`s swap and the pool becomes the original's. The remaining 11 bytes
are register allocation / post-RA schedule only:

```
orig:   d5456 ldr.w r3,[r0,r8,lsl#2]   ; yaw load into r3
        d545a movs r0,#2
        d545c str.w r8,[sp,#184]
        ...
        d546e mov r4,r2
best16d:d5456 str.w r8,[sp,#184]
        d545a ldr.w r4,[r0,r8,lsl#2]   ; yaw load into r4
        ...
        d546c mov r3,r4                 ; extra copy
        d546e mov r0,r8
```

The extra `mov r3,r4` is because the yaw load's result is assigned to `r4`
(the just-killed `Yaw` pointer register) rather than directly to the argument
register `r3`, and `str.w r8` is scheduled before the load instead of after
`movs r0,#2` — both post-RA choices.

The second fence is load-bearing at the IR level: with one fence the same
shape gives **25 diff / 10 runs** and the pool reverts to Pitch/Yaw/YawIPad.
Two *source-level* `__atomic_signal_fence` calls, however, are merged by the
optimiser into one and still give 39/8, so the two-fence IR shape is not
directly expressible from C++.

### 6.3 The data-layout trap

The `.rodata` emission order of the two yaw arrays follows the order the
source *references* them. The base ternary references `YawIPad` first, so
`.rodata` is `Pitch, YawIPad, Yaw` (matching the original). Writing the
equivalent `!Globals::iPad ? gHangarCamYaw : gHangarCamYawIPad`, or an
`if (iPad) yawTable = YawIPad;` form, references `Yaw` first and emits
`.rodata` as `Pitch, Yaw, YawIPad`. The code bytes are otherwise identical
(the register assignment swaps with the layout), so these variants are
semantically correct but their pool values at `+0x3e4/+0x3e8` are the *wrong*
addresses (`0x1fe058` = Yaw, `0x1fe080` = YawIPad). They still measure 39/8.
The base ternary is therefore the only shape that keeps the original data
layout.

## 7. Source-variant table (this pass; all 1008 B unless noted)

| variant | raw diff | runs | pool (d56b0,d56b4,d56b8) |
|---|---:|---:|---|
| **base (pristine)** | **39** | **8** | `0x128bf0, 0x128c34, 0x128c12` |
| `pitchptr_first` / `pitchptr_first2` | 39 | 8 | same as base |
| `addr0` / `ptr_ternary` | 39 | 8 | same as base |
| `ifelse` (`Yaw; if(iPad) YawIPad`) | 39 | 8 | `0x128bf0, 0x128c0c, 0x128c3a` (layout swapped) |
| `notipad` / `ifnotipad` / `ifipad` | 39 | 8 | layout swapped or base |
| `Globals::iPad != 0` / `!= false` / `0 != iPad` | 39 | 8 | same as base |
| two source fences (`ff_base`, `ff_ptr`) | 39 | 8 | same as base |
| `nofence` | 43 | 8 | `0x128bf0, 0x128c3a, 0x128c0c` |
| `fence_after_decl` | 42 | 7 | `0x128bf2, 0x128c12, 0x128c34` |
| `pvol_yfloat` (pitch volatile, yaw plain float) | 44 | 7 | `0x128bf2, 0x128c14, 0x128c2e` |
| `pitchload_first` (pitch load before yaw decl) | 690 | 94 | 1000 B, code shifted |
| `direct_vol` / `inline` / `inline_nopitch` | 690 | 94 | 1000 B |
| `inline_yaw2` (inline yaw in the call) | 689 | 92 | 1000 B |
| `pvol_yfloat_inline` | 689 | 92 | 1000 B |
| `pitchload_first` + two fences | 690 | 94 | 1000 B |
| `Globals::iPad == 1` | 691 | 91 | 1000 B |
| `ternary_rev` (yaw load before pitch load) | 744 | 40 | 1000 B |
| `novol` / `novol_plf` / `direct_novol` | 631 | 81 | 1008 B |
| `pint_yvol` (pitch plain, yaw volatile) | 630 | 82 | 1008 B |
| `volfloat` (volatile float loads) | 745 | 62 | 1032 B |
| `volfloat2` | 744 | 41 | 1016 B |

Neutral (39/8): `ifelse`, `notipad`, `ifnotipad`, `ifipad`, `pitchptr_first`,
`pitchptr_first2`, `addr0`, `ptr_ternary`, `!= 0`, `!= false`, `0 != iPad`,
`ff_base`, `ff_ptr`.

### IR-level variants (hand-edited `base.ll`, `-Xclang -disable-llvm-passes`)

| IR shape | diff | runs | pool |
|---|---:|---:|---|
| base | 39 | 8 | Pitch, Yaw, YawIPad |
| `fence, pgep, ipad, icmp, sel, pbc, pl, ygep, ybc, yl, ylf, plf` (best16d) | **11** | **4** | Pitch, YawIPad, Yaw |
| same with one fence (best16e) | 25 | 10 | Pitch, Yaw, YawIPad |
| best16b (same but yaw/pitch call args swapped — semantically wrong) | 16 | 8 | Pitch, YawIPad, Yaw |
| best16c (one fence, args swapped) | 27 | 11 | — |

## 8. Cross-reference: `resetIdleCamForHangar` and `OnInitialize`

`ModStation::resetIdleCamForHangar` (0xd8cf0, 360 B) is byte-exact and uses the
simpler `gHangarCamPitch[race], gHangarCamYaw[race]` pair (no iPad select).
`ModStation::OnInitialize` (100 %, 6396 B) contains the same
`Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw` expression twice and its
near block already has the original's pool order: its IR has the pitch load
*before* the select and both loads non-volatile. The ctor differs by putting
the pitch load after the select and marking both loads volatile (the yaw load
must be volatile so it is not CSE'd with the far block's yaw load).

## 9. Conclusion

The residual is not missing code, a wrong constant, or a wrong callee. It is
the SelectionDAG `allnodes` order of the two `LDRLIT` leaves for
`gHangarCamYawIPad` / `gHangarCamYaw` in the near block, plus the post-RA
schedule that follows. The register assignment, the `.rodata` layout, and the
far block are already correct. A hand-edited IR shape reaches 11 diff / 4 runs
with the exact original pool, proving the order is achievable in the backend,
but no C++ source shape tried here reproduces that IR (front-end reordering,
two fences, `!iPad`, `if/else`, inline expressions, pointer hoisting, volatile
permutations, `== 1`, and pitch-load-first all either stay at 39/8 or change
the function size / data layout).

## 10. Next steps

1. Find a source shape that makes the optimiser emit two non-merged
   `fence seq_cst` instructions (or any equivalent pair of barriers) in the
   near block while keeping the pitch GEP before the iPad load and the yaw
   load after the select. That is the only IR shape found so far that flips
   the `LDRLIT` order without changing the function size.
2. Alternatively, start from `best16d`'s IR and find the post-RA tweak that
   makes the yaw load target `r3` directly (removing the `mov r3,r4`) and
   schedules `str.w r8` after `movs r0,#2` — then search for a source shape
   that produces that IR.
3. Do not land any of the 39/8 variants: they keep `byte_exact` at 4499 and
   do not change the raw diff. The base ternary is the best source-level
   shape found so far.
