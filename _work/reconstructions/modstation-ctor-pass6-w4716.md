# ModStation ctor (`_ZN10ModStationC1Ev` / `C2Ev`) — wave 97 pass 6 (w4716)

Status: **raw-residual improvement landed (43 → 39 differing bytes); no
`byte_exact` change** (the function still does not match, so the gate count is
unchanged). The pass pins the residual to two independent backend choices:
(1) the SelectionDAG emission order of the two `LDRLIT` pseudos for
`gHangarCamYawIPad` / `gHangarCamYaw`, and (2) the post-RA schedule of the yaw
load, the `race` spill and the `pitchBits` copy. It also explains *why* the
LDRLIT order is the reverse of the `.rodata` order: InstCombine's select
canonicalisation.

Worktree `wt/w4716`, branch `wave97-w4716`, base master `9b58a39d5`.
All numbers measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the
tree's NDK r18b clang 7.0.2. Scratch drivers in `/dev/shm/w4716`
(`fdiff.py`, `cmp.py`, `cc.sh`, `cc_ll.sh`, `cc_mir.sh`, `tryfull2.py`,
`probe_order.py`, `selir.py`, `batchfull*.py`).

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
`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70` unchanged.

## 1. What was landed

One line added before the two table declarations in the ctor:

```cpp
    MatrixSetTranslation(nearKey, this->camKeyX, this->camKeyY, this->camKeyZ);
    __atomic_signal_fence(__ATOMIC_SEQ_CST);          // <-- added
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
```

This is a pure compiler barrier (no instruction is emitted). It reduces the
ctor's raw residual from **43 differing bytes / 8 runs** (pristine HEAD) to
**39 / 8**, and is gate-neutral (`byte_exact` stays 4500, all other counts
identical). The pass-5 note's claim that the pristine tree is already 39/8 is
incorrect: HEAD without the fence is 43/8; the 39/8 figure in pass 5 was
measured with this fence already present.

## 2. Residual on the pristine tree vs with the fence

Pristine (HEAD, no fence) — 43 bytes / 8 runs:

```
+0x169..+0x169 (1B)  orig=4b  ours=4c
+0x16b..+0x16b (1B)  orig=4c  ours=4b
+0x16e..+0x17d (16B) orig=7b449af800107c4450f8282018460029
                     ours=9af800107c4450f828207b4400291846
+0x186..+0x186 (1B)  orig=50  ours=cd
+0x188..+0x18c (5B)  orig=28300220cd  ours=b880144650
+0x18e..+0x19e (17B) orig=b88000210df58078cde9001050a9404614
                     ours=2830022000210df58078cde9001050a940
+0x3e4..+0x3e4 (1B)  orig=16  ours=3a
+0x3e8..+0x3e8 (1B)  orig=38  ours=0c
```

With the fence — 39 bytes / 8 runs (only the `+0x16e` run shrinks, 16 → 12 B;
the two pool bytes change from `3a/0c` to `34/12`):

```
+0x169..+0x169 (1B)  orig=4b  ours=4c
+0x16b..+0x16b (1B)  orig=4c  ours=4b
+0x16e..+0x179 (12B) orig=7b449af800107c4450f82820
                     ours=9af800107b4450f828207c44
+0x186..+0x186 (1B)  orig=50  ours=cd
+0x188..+0x18c (5B)  orig=28300220cd  ours=b880144650
+0x18e..+0x19e (17B) orig=b88000210df58078cde9001050a9404614
                     ours=2830022000210df58078cde9001050a940
+0x3e4..+0x3e4 (1B)  orig=16  ours=34
+0x3e8..+0x3e8 (1B)  orig=38  ours=12
```

`C1` and `C2` are aliases (same 1008 B at `0xd52d0`). Only the near-camera
rotation block and the two literal-pool slots differ; the far block, the
camKey block and the rest of the function are byte-identical.

## 3. Disassembly (fence variant)

```
orig                                                    ours
d5436 489e  ldr r0,[pc,#632]  ; Pitch                   d5436 489e  ldr r0,[pc,#632]  ; Pitch
d5438 4b9e  ldr r3,[pc,#632]  ; YawIPad                 d5438 4c9e  ldr r4,[pc,#632]  ; Yaw
d543a 4c9f  ldr r4,[pc,#636]  ; Yaw                     d543a 4b9f  ldr r3,[pc,#636]  ; YawIPad
d543c 4478  add r0,pc                                   d543c 4478  add r0,pc
d543e 447b  add r3,pc                                   d543e f89a1000  ldrb.w r1,[sl]
d5440 f89a1000  ldrb.w r1,[sl]                          d5442 447b  add r3,pc
d5444 447c  add r4,pc                                   d5444 f8502028  ldr.w r2,[r0,r8,lsl#2]
d5446 f8502028  ldr.w r2,[r0,r8,lsl#2]                  d5448 447c  add r4,pc
d544a 4618  mov r0,r3                                   d544a 4618  mov r0,r3
d544c 2900  cmp r1,#0                                   d544c 2900  cmp r1,#0
d544e 932d  str r3,[sp,#180]                            d544e 932d  str r3,[sp,#180]
d5450 942c  str r4,[sp,#176]                            d5450 942c  str r4,[sp,#176]
d5452 bf08  it eq                                       d5452 bf08  it eq
d5454 4620  moveq r0,r4                                 d5454 4620  moveq r0,r4
d5456 f8503028  ldr.w r3,[r0,r8,lsl#2]                  d5456 f8cd80b8  str.w r8,[sp,#184]
d545a 2002  movs r0,#2                                  d545a 4614  mov r4,r2
d545c f8cd80b8  str.w r8,[sp,#184]                      d545c f8503028  ldr.w r3,[r0,r8,lsl#2]
d5460 2100  movs r1,#0                                  d5460 2002  movs r0,#2
d5462 f50d7880  add.w r8,sp,#256                        d5462 2100  movs r1,#0
d5466 e9cd1000  strd r1,r0,[sp]                         d5464 f50d7880  add.w r8,sp,#256
d546a a950  add r1,sp,#320                              d5468 e9cd1000  strd r1,r0,[sp]
d546c 4640  mov r0,r8                                   d546c a950  add r1,sp,#320
d546e 4614  mov r4,r2                                   d546e 4640  mov r0,r8
d5470 f78dedf6  blx MatrixSetRotation                   d5470 f78dedf6  blx MatrixSetRotation
```

Register assignment is identical: `r3` = `&gHangarCamYawIPad` (0x1fe058),
`r4` = `&gHangarCamYaw` (0x1fe080), `r8` = `race`, `sl` = `&Globals::iPad`.
The two independent differences are:

1. **LDRLIT emission order.** Original: `YawIPad` (r3) then `Yaw` (r4).
   Ours: `Yaw` (r4) then `YawIPad` (r3). The pool slots follow
   (`0xd56b4`/`0xd56b8`); the `.rodata` arrays themselves are at identical
   addresses in both binaries.
2. **Post-RA schedule of the tail.** Original issues the yaw load at `d5456`
   (before `movs r0,#2`), spills `race` at `d545c`, and copies pitch at
   `d546e` (last, just before the call). Ours issues `str.w r8` / `mov r4,r2`
   immediately after the select and the yaw load at `d545c`.

## 4. Why the LDRLIT order is the reverse of the `.rodata` order

The front-end lowers the ternary to a branch + phi:

```
%157 = icmp ne i8 %156, 0          ; %156 = load Globals::iPad
br i1 %157, label %158, label %159
158: br label %160
159: br label %160
160: %161 = phi [ @YawIPad, %158 ], [ @Yaw, %159 ]
```

`SimplifyCFG` if-converts this to `select i1 %157, @YawIPad, @Yaw` (true =
YawIPad). Then `InstCombineSelect` canonicalises a one-use `icmp` with a
non-canonical predicate (`ICMP_NE` is non-canonical, `ICMP_EQ` is canonical):

```cpp
  if (match(CondVal, m_OneUse(m_ICmp(Pred, m_Value(), m_Value()))) &&
      !isCanonicalPredicate(Pred)) {
    Cond->setPredicate(CmpInst::getInversePredicate(Pred));
    SI.setOperand(1, FalseVal);
    SI.setOperand(2, TrueVal);
```

so the final optimised IR is

```
%99  = icmp eq i8 %98, 0
%100 = select i1 %99, @_ZL13gHangarCamYaw, @_ZL17gHangarCamYawIPad
```

i.e. the select's true operand is `Yaw` (the *source* false arm). The
SelectionDAG builder visits `select` operands as (cond, true, false), so the
`Yaw` `GlobalAddress` node is created before `YawIPad`, and the ISel output
order is `Pitch, Yaw, YawIPad` — the reverse of the module/`.rodata` order
(`Pitch, YawIPad, Yaw`). Confirmed in the first MIR dump
(`After Expand ISel Pseudo-instructions`):

```
%115 = tLDRLIT_ga_pcrel @_ZL15gHangarCamPitch
%117 = tLDRLIT_ga_pcrel @_ZL13gHangarCamYaw       <- Yaw first
%118 = tLDRLIT_ga_pcrel @_ZL17gHangarCamYawIPad
```

The original's ISel order is `Pitch, YawIPad, Yaw` (the `.rodata` order), i.e.
its select must have been `select %eq, @YawIPad, @Yaw` (true = YawIPad), which
is exactly what the *flipped* source `Globals::iPad ? gHangarCamYaw :
gHangarCamYawIPad` produces. But flipping the source also flips the
front-end's first-use order and therefore the `.rodata` emission order
(`Pitch, Yaw, YawIPad`), so the two effects cancel and the code bytes stay at
39/8 while `data_exact` would drop.

## 5. The flip trap

`Globals::iPad ? gHangarCamYaw : gHangarCamYawIPad` (near + far) gives the
original's LDRLIT order in the MIR:

```
%115 = tLDRLIT @Pitch
%117 = tLDRLIT @YawIPad     <- correct order
%118 = tLDRLIT @Yaw
```

but the `.rodata` arrays are emitted `Pitch, Yaw(2.5), YawIPad(2.35)`, i.e.
`gHangarCamYaw @ 0x1fe058` and `gHangarCamYawIPad @ 0x1fe080` — swapped
relative to the original. The register allocator then assigns
`YawIPad → r4`, `Yaw → r3`, so the code bytes are the same 39/8 (by address
`r4 = 0x1fe080`, `r3 = 0x1fe058` in both). Flipping is therefore
**data-neutral-to-worse**, not an improvement.

Equivalently: the LDRLIT emission order is always `(r4, r3)` in every source
shape tried, while the original is `(r3, r4)`. The register allocator assigns
the first LDRLIT to `r4` and the second to `r3`; the original assigns the
first to `r3`. That is the whole remaining code difference.

## 6. Post-RA scheduler

The residual tail is produced by the old `PostRASchedulerList` /
`LatencyPriorityQueue` top-down list scheduler. The MIR immediately before it
(`After If Converter`) already has:

```
str  r3,[sp,#180]        ; spill YawIPad
mov  r0,r3
str  r4,[sp,#176]        ; spill Yaw
movcc r0,r4
str  r8,[sp,#184]        ; spill race
mov  r4,r2               ; save pitchBits
ldr  r3,[r0,r8,lsl#2]    ; yaw load
```

and the scheduler keeps that order. The original instead has the yaw load
first, then `movs r0,#2`, then the `race` spill, and the pitch copy last.
The store-before-load order is self-reinforcing: the yaw load is marked
`killed $r8` in the input MIR, so `str r8` must precede it; in the original
the spill store is the last use of `r8`, so the load precedes the store.

`-mllvm -disable-post-ra` degrades the whole function massively (282 differing
instructions, 32 runs), so the scheduler itself is required; there is no
`-postra-sched` option in this clang (only `-pre-RA-sched`), and the old
scheduler's priority queue is not configurable from the command line.

## 7. Variants measured this pass (full `.so` link + `fdiff`)

| variant | bytes | runs | `.rodata` |
|---|---:|---:|---|
| **pristine HEAD (no fence)** | 43 | 8 | ok |
| **fence before both table decls (landed)** | **39** | **8** | **ok** |
| fence between the two decls | 42 | 7 | ok |
| fence after both decls | 42 | 7 | ok |
| fence after pitch load | 44 | 7 | ok |
| flip ternary (near + far) | 39 | 8 | **SWAP** |
| flip near only | 691 | 91 | SWAP |
| `!iPad ? Yaw : YawIPad` | 39 | 8 | SWAP |
| `iPad == 0 ? Yaw : YawIPad` | 39 | 8 | SWAP |
| `iPad != 0 ? YawIPad : Yaw` | 39 | 8 | ok (= base) |
| two-pointer locals (`yawTableIPad`/`yawTablePhone`) | 39 | 8 | ok |
| `if/else` pointer assignment | 39 | 8 | ok |
| `bool`/`int` local `ipad` (near only) | 39 | 8 | ok |
| `bool`/`int` local `ipad` + volatile second use | 733 | 39 | ok |
| `bool`/`int` local `ipad` across the camKey block | 562 | 98 | ok |
| `volatile int pitchBits/yawBits` locals | 627 | 56 | ok |
| `const volatile float *` table pointers | 631 | 81 | ok |
| pointer-to-int locals (`pitchPtr`/`yawPtr`) | 39 | 8 | ok |
| `float pitch/yaw` locals from the int bits | 39 | 8 | ok |
| `Globals::iPad` changed to `bool` (Globals.h/cpp) | 77 | 41 | ok, 1016 B |

The `bool iPad` experiment was reverted (it also changes 403 call sites and
grows the function by 8 B).

## 8. Conclusion / next steps

The residual is not missing code, a wrong constant, or a wrong callee. It is:

1. the SelectionDAG `allnodes`/ISel order of the two `LDRLIT` leaves for
   `gHangarCamYawIPad` / `gHangarCamYaw` — the original emits the *false*
   select arm first, ours emits the *true* arm first because InstCombine
   canonicalised the select to `icmp eq; select Yaw, YawIPad`; and
2. the post-RA schedule of the yaw load, the `race` spill and the
   `pitchBits` copy in the call setup.

The landed fence removes 4 of the 43 differing bytes (43 → 39) at zero gate
cost. A full match needs a source shape that makes the false arm
(`YawIPad`) be emitted first *without* flipping the `.rodata` order — i.e. a
non-canonicalised select (`select %ne, YawIPad, Yaw` with a multi-use `icmp`)
combined with a `.rodata` first-use order of `YawIPad` before `Yaw`. Every
source form tried so far either canonicalises the select or flips the
`.rodata`. The hand-crafted IR shape from pass 5 (11 diff / 4 runs at the
`.o` level) still does not survive the full pipeline (42/7 when linked).
