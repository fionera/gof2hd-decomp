# ModStation constructor — wave 191b (w13641): assume+fence / non-volatile-pool re-probe

Worker `w13641`, branch `wave191b-w13641`, base master `57ce1a4c8`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13641`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`modstation-ctor-wave191b-w13575.md`, `modstation-ctor-wave191b-w13586.md`,
`modstation-ctor-wave191b-w13595.md`, `modstation-ctor-wave191b-w13605.md`,
`modstation-ctor-wave191b-w13616.md`, `modstation-ctor-wave191b-w13630.md`.

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
Section *sizes* are all exact.  Content deltas re-measured this pass:
`.text` **71,197**, `.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**,
`.data` **0**, `.rel.plt` **0** (byte-identical), `.rel.dyn`/`.dynsym`/
`.dynstr`/`.hash`/`.got` **0**, `.note.gnu.build-id` **20**.

## The residual (unchanged)

Original near block `0xd5436..0xd5470`:

```
ldr r0,Pitch ; ldr r3,YawIPad ; ldr r4,Yaw
add r0,pc ; add r3,pc ; ldrb r1,[sl] ; add r4,pc
ldr.w r2,[r0,r8,lsl#2]      ; pitchBits
mov r0,r3 ; cmp r1,#0 ; str r3,[sp,#180] ; str r4,[sp,#176]
it eq ; moveq r0,r4
ldr.w r3,[r0,r8,lsl#2]      ; yawBits -> r3 (direct)
movs r0,#2
str.w r8,[sp,#184]          ; spill race, AFTER movs r0
movs r1,#0 ; add.w r8,sp,#256
strd r1,r0,[sp] ; add r1,sp,#320 ; mov r0,r8
mov r4,r2                   ; save pitchBits for the far call
blx MatrixSetRotation
```

Landed HEAD form (`yaw = pitchBits`, `pitch = yawBits`, `near(yaw,pitch)`,
`far(pitch,...)`; far operand = `yawBits`) is byte-identical up to `moveq`,
then:

```
str.w r8,[sp,#184]          ; spill BEFORE the yaw load
ldr.w r4,[r0,r8,lsl#2]      ; yawBits -> r4 (not r3)
movs r0,#2 ...
mov r3,r4                   ; copy yawBits into near arg2
mov r0,r8
blx
```

The 11 raw bytes are exactly the `str.w`/`ldr.w` slot rotation, the
`ldr.w r3`/`ldr.w r4` register field, and the `mov r0,r8`/`mov r3,r4` +
`mov r4,r2`/`mov r0,r8` swap.  The landed form trades the far-key pitch
semantics (it passes `yawBits`) for the correct literal-pool order.

## New evidence this pass

### 1. A non-volatile pitch gives the *correct* pool; the fold is a register choice

`float pitch = pitchTable[race]` (non-volatile) keeps the pool
`Pitch,YawIPad,Yaw` (verified by `objdump -r`), but the value is live across
the near call and greedy RA assigns it to the callee-saved VFP register
`s0`, producing `vldr s0` + `vmov r1,s0` instead of `ldr.w r2`, and moves
`race` from `r8` to `r4`/`r9`.  The int load is therefore not an InstCombine
fold in isolation (a lone `*(const float*)&tab[i]` compiles to `ldr.w`), it
is a register-class choice driven by cross-call liveness.  This is the
mechanism behind angle (a): "correct pool + `mov r4,r2` without `volatile`"
requires keeping the value in a core register, and every int-forcing device
tried (`__builtin_assume`, `__builtin_memcpy`, int table view, union) either
folds or perturbs greedy RA.

### 2. `__builtin_assume` + a trailing fence recovers the correct pool *and* the faithful registers at 1008 B

`int pitchBits = *(const int*)&pitchTable[race];` +
`__builtin_assume(pitchBits != 0x7f800001);` keeps the integer `ldr.w`
(1000 B, `race` -> `r4`).  Adding a single
`__atomic_signal_fence(__ATOMIC_SEQ_CST)` **after** the two loads (before the
float locals / near call) pins `race` back to `r8` and restores size
**1008**; the result has the original pool `Pitch,YawIPad,Yaw`, the faithful
`mov r4,r2` live-out, and the direct `ldr.w r3` yaw load.  It still measures
**raw 35 / aligned 10** because the `Yaw` LDRLIT is emitted one slot late
(after `add r0,pc`) and the tail schedule (`str.w` before the yaw load,
`mov r4,r2` early) is the same post-RA choice as the other faithful shapes.
This is the closest faithful shape found (35 raw vs 42/43/44 for the other
faithful spellings), but it is still worse than the landed 11.

### 3. Fence-position sweep (faithful semantics)

`__atomic_signal_fence` between the table-pointer declarations and the loads
(position 2) gives the correct pool and faithful registers (42 raw); a
second fence does not help (12 diff lines).  Fences at other positions
either leave the pool flipped or degrade the schedule.  Two
`__atomic_signal_fence` calls with different orderings do not merge, but the
result is still 42 raw.  The `best16d` hand-IR result (11 raw) remains
unreachable from source.

## Measured table (full link + `tools/fn_diff.py`; all size-exact 1008 B unless noted)

| shape | raw / aligned | size | pool | note |
|---|---:|---:|---|---|
| HEAD landed form | **11 / 5** | 1008 | Pitch,YawIPad,Yaw | floor |
| landed + fence after table decls (wave-112 form) | 11 / 5 | 1008 | Pitch,YawIPad,Yaw | identical to HEAD |
| faithful `far=pitch` (both volatile) | 43 / 12 | 1008 | Pitch,Yaw,YawIPad | pool flips |
| faithful + fence after table decls | 42 / 12 | 1008 | Pitch,YawIPad,Yaw | correct pool+regs, Yaw LDRLIT sunk |
| faithful + 2 fences (SEQ/ACQUIRE) | 42 / 12 | 1008 | flipped | no gain |
| 44: pitch volatile / yaw non-volatile, far=pitch | 44 / 18 | 1008 | Pitch,YawIPad,Yaw | Yaw LDRLIT sunk |
| 44 + fence (any position) | 44 / 18 | 1008 | correct | no change |
| non-volatile pitch + `__builtin_assume` | 702 / 53 | **1000** | Pitch,YawIPad,Yaw | race -> r4, size-fatal |
| non-volatile pitch + assume + trailing fence | **35 / 10** | 1008 | Pitch,YawIPad,Yaw | race stays r8; best faithful shape |
| plain `float pitch = pitchTable[race]` | 666 / 60 | 1008 | Pitch,YawIPad,Yaw | `vldr`+`vmov`, race -> r4/r9 |
| `__builtin_memcpy` int / float pitch | 666 / 60 | 1008 | Pitch,YawIPad,Yaw | folds |
| int-table view (non-volatile) | 666 / 60 | 1008 | Pitch,YawIPad,Yaw | folds |
| value select `iPad ? YawIPad[race] : Yaw[race]` | 30 diff | 1008 | Pitch,YawIPad,Yaw | race -> r4 |
| pitch load before select (P,pb,Y,yb) | 30 diff | 1008 | Pitch,YawIPad,Yaw | race -> r4 |
| both live (far z = yaw) | 30 diff | 1008 | Pitch,YawIPad,Yaw | race -> r9 |
| `!iPad` / `==1` condition spellings | 43-44 / 12-13 | 1008 | flipped | no gain |
| `__builtin_expect(iPad,0/1)` | 43 / 12 | 1008 | flipped | no change |

Automated sweeps this pass (all full-link or quick-compile measurements):
20 valid statement permutations x 4 volatile masks x 8 fence positions x
2 far operands x 2 float-declaration orders; assume-placement x fence-placement
grid (81 combos); condition-spelling grid.  Floor remains **11**.

## Conclusion

The landed 11-byte form remains the measured floor for clang 7.0.2 on this
TU.  The new assume+fence shape proves the correct pool *and* the faithful
`mov r4,r2` live-out are simultaneously reachable at 1008 B (35 raw), but the
`Yaw` LDRLIT emission slot and the post-RA tail (`str.w` slot, `mov r4,r2`
placement) still differ; the landed form's byte-identical prefix is worth
more than the faithful tail.  No source change landed; tree clean; `check`
green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13641
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
