# ModStation constructor — wave 191b (w13605): faithful-operand / pool-order re-probe

Worker `w13605`, branch `wave191b-w13605`, base master `855fc0820`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13605`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`modstation-ctor-wave191b-w13575.md`, `modstation-ctor-wave191b-w13586.md`,
`modstation-ctor-wave191b-w13595.md`; also `modstation-ctor-wave112-w6018.md`
(the float-declaration-order / pool-order lever).

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no raw win below 11 found.**
Tree clean; `check` green at the entry counts.

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
`RATCHET PASS`, byte_exact **4508/4524**, linked_exact **4508/4524**,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
Section deltas re-measured this pass: `.text` **71,199**, `.ARM.extab` **577**,
`.ARM.exidx` **0**, `.rodata` **0**, `.data` **0**, `.rel.plt` **0**
(byte-identical), `.dynsym`/`.dynstr`/`.hash`/`.rel.dyn` **0**.

## The residual, re-confirmed

Original near block `0xd5436..0xd5470` (unchanged from w13595):

```
ldr r0,Pitch ; ldr r3,YawIPad ; ldr r4,Yaw
add r0,pc ; add r3,pc ; ldrb r1,[sl] ; add r4,pc
ldr.w r2,[r0,r8,lsl#2]      ; pitchBits
mov r0,r3 ; cmp r1,#0 ; str r3,[sp,#180] ; str r4,[sp,#176]
it eq ; moveq r0,r4
ldr.w r3,[r0,r8,lsl#2]      ; yawBits
movs r0,#2 ; str.w r8,[sp,#184] ; movs r1,#0 ; add.w r8,sp,#256
strd r1,r0,[sp] ; add r1,sp,#320 ; mov r0,r8
mov r4,r2                   ; save pitchBits for the far call
blx MatrixSetRotation
```

The landed HEAD form is `yaw = pitchBits`, `pitch = yawBits`,
`near(yaw,pitch)`, `far(pitch,...)` (far operand = `yawBits`): correct pool
`Pitch,YawIPad,Yaw` but `str.w r8` one slot early, yaw loaded into `r4` not
`r3`, and `mov r3,r4` instead of `mov r4,r2`.  The semantically faithful form
(`pitch = pitchBits`, `yaw = yawBits`, `far(pitch,...)`) gives the right
instruction multiset but flips the pool to `Pitch,Yaw,YawIPad` and moves the
spill/save before the yaw load — raw **43 / 12**.

## New evidence: the integer load is only forced by `volatile`, and only for the live-out value

Re-derived this pass by compiling variants and reading the `.s`/MIR:

* A **non-volatile** `int` load whose only use is a `bitcast` to `float` is
  folded by InstCombine to a `float` load (`vldr`+`vmov`) **when the value is
  live across the near call**.  For the value *not* live across the call
  (the yaw in the faithful form) the same non-volatile `int` load stays
  `ldr.w`.  This is why the faithful pitch must be `volatile` to get `ldr.w`.
* `volatile` is exactly what couples the pool order to the far operand: with
  both loads volatile the ISel LDRLIT order is `Pitch,Yaw,YawIPad` for
  `far=pitchBits` and `Pitch,YawIPad,Yaw` for `far=yawBits`.
* A *free* integer use that keeps `pitchBits` an `int` without `volatile`
  exists: `__builtin_assume(pitchBits != 0x7f800001)` (and any non-tautology
  assume on `pitchBits`).  It keeps the pitch load as `ldr.w`, and the near
  block then schedules **exactly like the original** (yaw load → `movs r0,#2`
  → spill → save).  But the extra integer use changes greedy-RA: `race` moves
  from `r8` to `r4`, so `mov.w r8,#8`/`mov.w r8,#7` become 2-byte
  `movs r4,#8`/`movs r4,#7` and the function is **1000 B** (raw 702).  The
  size is wrong and the register roles are wrong, so it is not landable.

## Source shapes measured this pass (full link + `tools/fn_diff.py`)

| shape | raw / aligned | size | note |
|---|---:|---:|---|
| HEAD landed form | **11 / 5** | 1008 | floor |
| faithful `far(pitchBits)` (all spellings) | 43 / 12 | 1008 | pool flips |
| faithful, swap float declaration order (`yaw` first, `pitch` second) | 43 / 12 | 1008 | wave-112 lever does **not** help when far=pitch |
| faithful, far via late `float farPitch = pitchBits` | 43 / 12 | 1008 | same IR |
| faithful, near args inline bitcasts / far inline bitcast | 43 / 12 | 1008 | same IR |
| faithful, `yaw`-first names (`yaw=pitchBits`, `pitch=yawBits`, far=`yaw`) | 43 / 12 | 1008 | same IR as faithful |
| faithful, `bool`/`unsigned char ipad` local before select | 43 / 12 | 1008 | same IR |
| pitch **volatile** int, yaw **non-volatile** int | **44 / 18** | 1008 | pool **correct**, `mov r4,r2` present, `ldr.w r3` direct; Yaw LDRLIT sinks below `cmp`, save/spill still early |
| pitch volatile, yaw non-volatile float | 44 / 18 | 1008 | same |
| pitch volatile, yaw non-volatile, far via late local | 44 / 18 | 1008 | same |
| `iPad == 1 ? YawIPad : Yaw` (faithful) | 44 / 13 | 1008 | pool correct, `cmp #1`, roles swapped |
| `iPad != 1 ? Yaw : YawIPad` (faithful) | 44 / 13 | 1008 | pool correct, `cmp #1`, roles swapped |
| `iPad == 0 ? Yaw : YawIPad` / `iPad < 1` (faithful) | 43 / 12 | 1008 | canonicalised back |
| non-volatile int bits (both) | 667 / 59 | 1008 | pitch folds to `vldr`+`vmov`, RA changes |
| non-volatile int bits, HEAD semantics | 672 / 62 | 1008 | same |
| `__builtin_assume(pitchBits != 0x7f800001)` (non-volatile) | 702 / 62 | **1000** | integer load + original schedule, RA `race=r4` |
| same assume after near / before far | 702 / 62 | 1000 | RA change is IR-wide |
| same assume with volatile load | 43 / 12 | 1008 | assume optimised away |
| pitch non-volatile, yaw volatile | 666-669 / 60 | 1008 | pitch folds |
| far y = `yaw` local (both live) | 604 / 71 | 984 | reload removed, size-fatal |
| far y = `yawTable[race]` local (single select) | 604 / 71 | 984 | size-fatal |
| `float farYaw = yawTable[race]` before near call | 604-499 / 71-85 | 984 | size-fatal |
| `bool ipad` local reused by far select | 604 / 71 | 984 | second select removed |
| far z = `yaw` (both live, keeps reload) | 736 / 91 | 1016 | size-fatal |
| far z = `pitch` | 533 / 85 | 1008 | schedule wrecked |
| volatile float arrays (`const volatile float *`) | 753 / 52 | 1016 | size-fatal |
| volatile pointer local (`const float *volatile`) | 278 / 149 | 1008 | extra loads |

Automated sweeps (all full-link measurements, ~1 s each):
128 combos of {pitch/yaw volatile mask} × {table decl order} × {bits decl
order} × {float decl order} × {near arg order} × {far y inline/`yawTable`} ×
{`(void)yaw`} for `far=pitchBits`: **floor 43**.
128 combos of the same grid for `far=yawBits` (HEAD semantics): **floor 11**.
64 combos of {volatile mask} × {`iPad`/`==1`/`!=1`/`==0`} × {far pitch/yaw} ×
{near arg order}: **floor 11**.

## Why the two facts stay coupled

The far operand is the only IR lever that flips the near-block ISel LDRLIT
order (w13586 proved this with `-print-machineinstrs`: the MOVCCr operand
order is identical, only the `tLDRLIT` emission order changes).  Making the
pitch a non-volatile `int` (so it stays `ldr`) flips the pool back to the
correct order *and* gives the right schedule, but the load folds to
`vldr`+`vmov` unless it has a non-bitcast integer use; every such use
(`volatile`, `__builtin_assume`) either re-couples the pool to the far
operand or perturbs greedy-RA (size/roles).  The `pitch volatile / yaw
non-volatile` split gets the correct pool **and** `mov r4,r2` **and** the
direct `ldr.w r3`, but the non-volatile yaw LDRLIT sinks below the `cmp`
and the spill/save stay early — 44.

## Conclusion

The 11-byte residual is the measured floor for clang 7.0.2 on this TU with
source-only changes.  The landed HEAD form trades the far-key pitch semantics
(it passes `yawBits`) for the correct literal-pool order; the faithful form
buys the `mov r4,r2` / direct `ldr.w r3` but loses the pool and the schedule.
No source shape reached both.  No source change landed; tree clean; `check`
green.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13605
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
