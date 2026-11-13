# ModStation constructor — wave 191b (w13595): pool-order / pitch-live decoupling sweep

Worker `w13595`, branch `wave191b-w13595`, base master `555ac3dc2`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13595`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`modstation-ctor-wave191b-w13575.md`, `modstation-ctor-wave191b-w13586.md`.

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no raw win found.**  Tree clean,
`check` green at the entry counts.

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
`RATCHET PASS`, byte_exact **4508/4524**, linked_exact **4508/4524**,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical; `.ARM.extab` unchanged at 577.

## What the original actually does (re-derived this pass)

Original near block `0xd5436..0xd5470`:

```
ldr r0 Pitch ; ldr r3 YawIPad ; ldr r4 Yaw
add r0,pc ; add r3,pc ; ldrb r1,[sl] ; add r4,pc
ldr.w r2,[r0,r8,lsl#2]      ; pitchBits -> r2
mov r0,r3 ; cmp r1,#0 ; str r3,[sp,#180] ; str r4,[sp,#176]
it eq ; moveq r0,r4
ldr.w r3,[r0,r8,lsl#2]      ; yawBits -> r3
movs r0,#2 ; str.w r8,[sp,#184] ; movs r1,#0 ; add.w r8,sp,#256
strd r1,r0,[sp] ; add r1,sp,#320 ; mov r0,r8
mov r4,r2                   ; save pitchBits for the far call
bl MatrixSetRotation
```

So the source semantics are unambiguous: near call `(pitchBits, yawBits)`,
far call `(pitchBits, yawTable[race])` — i.e. the **faithful** form
(`pitch = *(float*)&pitchBits`, `yaw = *(float*)&yawBits`,
`far(pitch, ...)`).  `mov r4,r2` exists only because `pitchBits` (in the
caller-saved `r2`) is live across the near call.

The landed HEAD form is a source-shape device: `yaw = pitchBits`,
`pitch = yawBits`, `far(pitch, ...)` — the far arg is `yawBits`, so the value
live across the call is `yawBits` (in `r4`), and the near arg2 is copied with
`mov r3,r4`.  That buys the correct literal-pool order at the cost of the
`mov r3,r4`/`mov r4,r2` substitution plus one schedule slot for
`str.w r8,[sp,#184]`.

The 11-byte residual therefore decomposes into three coupled facts:

1. `str.w r8,[sp,#184]` is issued one slot later in the original
   (after `movs r0,#2`, not before the yaw load),
2. the yaw value is loaded straight into `r3` (its arg register) in the
   original, not `r4`,
3. the live-out copy is `mov r4,r2` (pitchBits) in the original, not
   `mov r3,r4` (yawBits).

(2)+(3) are only reachable when the far call's first float operand is
`pitchBits`; (1) is a separate post-RA schedule slot.

## New evidence: the pool order is an ISel/DAG tie, and it tracks the far operand

`-mllvm -print-machineinstrs` (108 MB dumps, scratch `landed_mir.log`,
`faithful_mir.log`), `# After Instruction Selection:` for
`_ZN10ModStationC2Ev` bb.12:

```
landed  : %114 iPad, %115 [iPad GOT load], %116 Pitch, %117 YawIPad, %118 Yaw
faithful: %114 iPad, %115 Pitch, %116 [iPad GOT load], %117 Yaw, %118 YawIPad
```

The `t2MOVCCr` operand order is identical in both
(`MOVCCr <YawIPad>, <Yaw>, eq`), so the register roles are fixed; only the
*emission order of the two `tLDRLIT`s* differs, and that order is what the
ARM constant-pool uses (first use wins).  The only IR difference between the
two forms is the far call's first float operand, so one operand simultaneously
decides (a) whether `pitchBits` is live-out and (b) the `LDRLIT` order.

A plausible mechanism (from LLVM 7 `ScheduleDAGRRList.cpp`): both `tLDRLIT`s
are leaf nodes (`getNodePriority` returns 0 for `NumPreds==0`), so they tie in
`BURRSort` down to `NodeQueueId` (push order).  They are released by the near
`CMOV` in operand order `[YawIPad, Yaw]`, i.e. `Yaw` is pushed second and wins
the heap tie-break — which is exactly the *faithful* order.  The landed order
requires the two nodes to go through the `PendingQueue` instead
(`hybrid_ls_rr_sort::isReady` returns false when `height > CurCycle+3`), which
preserves release order.  So the flip is a pending-vs-available queue
difference driven by the far operand's effect on heights / `CurCycle`.

## New source shapes measured this pass

`measure` = `link_so -j2` under `ulimit -v 12000000` + `tools/fn_diff.py`
(1 s per candidate, so every candidate below is a *full link* measurement,
not a screen).  `screen2.py` (scratch) additionally reports the resolved
`LDRLIT` symbol order.

| shape | raw / aligned | size | pool order | note |
|---|---:|---:|---|---|
| HEAD landed form | **11 / 5** | 1008 | Pitch,YawIPad,Yaw | floor |
| faithful `far(pitch)` (baseline) | 43 / 12 | 1008 | Pitch,Yaw,YawIPad | `mov r4,r2` present |
| faithful, no float locals (all inline bitcasts) | 43 / 12 | 1008 | flipped | same IR |
| faithful, `const float` / `float&` / `const float&` locals | 43 / 12 | 1008 | flipped | same IR |
| faithful, `union{int;float;}` / `*(int*)&f=` store | 43 / 12 | 1008 | flipped | same IR |
| faithful, `float[1]` local | 43 / 12 | 1008 | flipped | same IR |
| faithful, `__builtin_expect(iPad)` near / far | 43 / 12 | 1008 | flipped | same IR |
| faithful, far select `!iPad?Yaw:YawIPad`, `iPad==0?`, `&arr[0]` | 43 / 12 | 1008 | flipped | canonicalised |
| faithful, far yaw via local `const float *` (2 uses) | 604 / 71 | 984 | — | size-fatal |
| far arg1 = fresh `*(volatile int*)&pitchTable[race]` reload | 43 / 12 | 1008 | flipped | load CSE'd away |
| far arg1 = fresh non-volatile reload | 43 / 12 | 1008 | flipped | CSE'd |
| **pitch load non-volatile**, yaw non-volatile | 667 / 59 | 1008 | **correct** | `mov r4,r1` present, but pitch folds to `vldr s0`+`vmov r1,s0` |
| **pitch volatile, yaw non-volatile** | 44 / 18 | 1008 | **correct** | `mov r4,r2` present, but `Yaw` LDRLIT scheduled late and `mov r4,r2` early |
| pitch non-volatile, yaw volatile | 666-669 / 60 | 1008 | correct | pitch still folds |
| faithful + `__atomic_signal_fence` (18 positions) | best **42 / 12** | 1008 | flipped | fence before pitch load / after loads / after yaw load |
| both volatile + fence between the loads | 44 / 12 | 1008 | flipped | yaw load before `mov r4,r2` |
| near args swapped `(yaw,pitch)`, far `pitch` | 44 / 14 | 1008 | flipped | roles swap |
| near args swapped `(yaw,pitch)`, far `yaw` | 16 / 10 | 1008 | correct | best non-faithful family, still > 11 |
| faithful, pitch load first / yaw load first / select first | 43 / 12 | 1008 | flipped | load order yaw-first is size-fatal (702/1000) |

Systematic sweep (`search1.py`): 192 full-link candidates = volatile mask
{(V,V),(V,N)} x table-decl order x load order x near-arg order x far-arg
mode {local, inline} x extra statement {none, unused pointer, `int ipad`,
`(void)yawTable`}.  Every far=`pitchBits` candidate is **43**; every
far=`yawBits` candidate is 11/16/44.  No candidate < 11.

## Why the two facts cannot be decoupled from source

* The pool order is fixed at ISel from the `tLDRLIT` emission order, which is
  a scheduler tie-break over the whole block DAG.
* The only IR lever that flips it is the far call's first float operand —
  the same lever that decides whether `pitchBits` or `yawBits` is live-out.
* Making the pitch load non-volatile flips the pool order back to correct
  *and* gives `mov r4,r2`, but then clang folds `bitcast(load i32)` into a
  float load (`vldr`+`vmov`), which is a 667-byte regression.  There is no
  free second integer use of the loaded value that survives to the DAG
  (all candidate "free uses" are folded by InstCombine), so the integer
  `ldr.w` requires `volatile`, and `volatile` is exactly what makes the pool
  order depend on the far operand.
* `-pre-RA-sched` sweeps (w13586) and `-enable-post-misched=false` (w13550)
  do not help; any non-default scheduler regresses the rest of the function.

## Conclusion

The 11-byte residual is the measured floor for clang 7.0.2 on this TU with
source-only changes.  The landed form trades the far-key pitch semantics
(passes `yawBits`) for the correct literal-pool order.  No source change
landed; the tree is clean and `check` is green at the entry counts.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
cmake --build cmake-build-match --target check -j2
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
