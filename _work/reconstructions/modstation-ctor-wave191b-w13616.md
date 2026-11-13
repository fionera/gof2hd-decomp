# ModStation constructor — wave 191b (w13616): RA-spill placement / condition-spelling re-probe

Worker `w13616`, branch `wave191b-w13616`, base master `3b09d6c0d`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13616`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`modstation-ctor-wave191b-w13575.md`, `modstation-ctor-wave191b-w13586.md`,
`modstation-ctor-wave191b-w13595.md`, `modstation-ctor-wave191b-w13605.md`.

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
Section content deltas re-measured this pass: `.text` **71,196**,
`.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**, `.data` **0**,
`.rel.plt` **0** (byte-identical), `.dynsym`/`.dynstr`/`.hash`/`.rel.dyn`
**0**.  All section *sizes* are exact (including `.text` and `.ARM.extab`).

## The residual (unchanged)

Original near block `0xd5436..0xd5470`:

```
ldr r0,Pitch ; ldr r3,YawIPad ; ldr r4,Yaw
add r0,pc ; add r3,pc ; ldrb r1,[sl] ; add r4,pc
ldr.w r2,[r0,r8,lsl#2]      ; pitchBits
mov r0,r3 ; cmp r1,#0 ; str r3,[sp,#180] ; str r4,[sp,#176]
it eq ; moveq r0,r4
ldr.w r3,[r0,r8,lsl#2]      ; yawBits
movs r0,#2
str.w r8,[sp,#184]          ; spill race (r8 is redefined by add.w r8,sp,#256)
movs r1,#0 ; add.w r8,sp,#256
strd r1,r0,[sp] ; add r1,sp,#320 ; mov r0,r8
mov r4,r2                   ; save pitchBits for the far call
blx MatrixSetRotation
```

Landed HEAD form (`yaw = pitchBits`, `pitch = yawBits`, `near(yaw,pitch)`,
`far(pitch,...)`):

```
...
ldr.w r4,[r0,r8,lsl#2]      ; yawBits loaded into r4, not r3
str.w r8,[sp,#184]          ; spill placed BEFORE the yaw load
movs r0,#2 ...
mov r3,r4                   ; copy yawBits into the near arg2 register
mov r0,r8
blx
```

The 11 raw bytes are exactly: the `str.w`/`ldr.w` slot rotation, the
`ldr.w r3`/`ldr.w r4` register field, and the `mov r0,r8`/`mov r3,r4` +
`mov r4,r2`/`mov r0,r8` swap.  If the spill slot alone could be moved after
`movs r0,#2` the residual would drop to **5** raw bytes (the landed semantics
still keeps the wrong live-out value); reaching 0 additionally needs the
faithful far operand (`far(pitchBits)`), which flips the literal pool.

## New evidence: the spill is an RA live-range split at the *last use*, not a post-RA scheduler choice

`-mllvm -print-machineinstrs` on the landed form (scratch `mirLanded.log`,
108 MB).  The spill is already in the **After Greedy Register Allocator**
output, before the yaw load, and the yaw load is marked `killed $r8`:

```
bb.15:
  renamable $r0 = tLDRLIT_ga_pcrel @_ZL15gHangarCamPitch
  renamable $r3 = tLDRLIT_ga_pcrel @_ZL17gHangarCamYawIPad
  renamable $r4 = tLDRLIT_ga_pcrel @_ZL13gHangarCamYaw
  renamable $r1 = t2LDRBi12 renamable $r10, 0
  renamable $r2 = t2LDRs killed renamable $r0, renamable $r8, 2   ; pitch
  t2CMPri killed renamable $r1, 0
  t2STRi12 renamable $r3, %stack.11, 0
  renamable $r0 = COPY killed renamable $r3
  t2STRi12 renamable $r4, %stack.12, 0
  renamable $r0 = t2MOVCCr killed renamable $r0, killed renamable $r4
  t2STRi12 renamable $r8, %stack.9, 0          ; <-- race spill, before the yaw load
  renamable $r4 = t2LDRs killed renamable $r0, killed renamable $r8, 2  ; yaw
  ...
  renamable $r8 = t2ADDri %stack.2, 0          ; r8 redefined (nearKey sret)
```

The splitter places the spill at the end of `race`'s live range (the yaw
load, its last use) and marks that load `killed $r8`; the post-RA
`ScheduleDAGRRList` then keeps the store before the load.  The original's
store is *after* the load, i.e. the original's RA split point is the
`add.w r8` clobber rather than the last use.  The pre-RA schedule is
identical in both forms (yaw load, EH_LABEL, ADJCALLSTACKDOWN, movs 2, ...),
so the difference is inside the RA's split-point choice, not something the
source can address with a statement reorder (see the measured table).

## New source shapes measured this pass (full link + `tools/fn_diff.py`)

`measure.sh` = `link_so -j2` under `ulimit -v 12000000` + `tools/fn_diff.py`
(~0.9 s per candidate; every row is a full link measurement).

| shape | raw / aligned | size | insn | note |
|---|---:|---:|---:|---|
| HEAD landed form | **11 / 5** | 1008 | 374 | floor |
| faithful `far(pitchBits)`, both volatile | 43 / 12 | 1008 | 374 | pool flips |
| faithful, pitch volatile / yaw non-volatile | 44 / 18 | 1008 | 374 | pool correct, `mov r4,r2`, direct `ldr.w r3`; Yaw LDRLIT sinks below `cmp`, spill/save early |
| faithful, both non-volatile int bits | 667 / 59 | 1008 | 375 | folds to `vldr`+`vmov` |
| landed, pitch volatile / yaw non-volatile | 672 / 62 | 1008 | 375 | folds |
| landed, both non-volatile int bits | 672 / 62 | 1008 | 375 | folds |
| landed, pitch non-volatile / yaw volatile | 701 / 60 | **1000** | 373 | race moves to r4 |
| faithful + `__builtin_assume(pitchBits != 0x7f800001)` | 702 / 53 | **1000** | 373 | integer load kept, race moves to r4 |
| landed + same assume | 701 / 60 | **1000** | 373 | same |
| `!Globals::iPad ? Yaw : YawIPad` (faithful) | 696 / 54 | **1000** | 372 | far select CSE'd with near select; `mov r4,r2` + direct `ldr.w r3` but pool flips, spill/mov early |
| `!Globals::iPad ? Yaw : YawIPad` (landed) | 697 / 52 | **1000** | 372 | same |
| `Globals::iPad == 0 ? ...` (faithful / landed) | 696 / 54, 697 / 52 | **1000** | 372 | same as `!iPad` |
| `Globals::iPad != 0 ? ...` (faithful / landed) | 43 / 12, **11 / 5** | 1008 | 374 | canonicalises to HEAD |
| union `{int;float;}` bits (non-volatile) | 667 / 59 | 1008 | 375 | folds |
| `__builtin_memcpy` into int (non-volatile) | 667 / 59 | 1008 | 375 | folds |
| `(const char*)table + race*4` int load | 667 / 59 | 1008 | 375 | folds |
| `const int *` table view | 667 / 59 | 1008 | 375 | folds |
| `const volatile int *` table view | 11 / 5 | 1008 | 374 | same IR as HEAD |
| `const volatile float *` pointer, then int bitcast | 11 / 5 | 1008 | 374 | same IR |
| `volatile float` load then volatile int bitcast | 731 / 89 | 1008 | 377 | extra load |
| plain `float pitch = table[race]` (faithful) | 667 / 59 | 1008 | 375 | `vldr`+`vmov` |
| `int farRace = race;` used by far call (before/after near) | 11 / 5 | 1008 | 374 | CSE'd |
| `int race = 0;` / `unsigned race` / `long race` | 11 / 5 | 1008 | 374 | same IR |
| `volatile int race` | 912 / 139 | **1016** | 374 | size-fatal |
| `const bool ipad = Globals::iPad;` cached (landed) | 757 / 95 | **968** | 359 | size-fatal |
| `const int ipad = Globals::iPad;` cached (landed) | 757 / 95 | **968** | 359 | size-fatal |
| pitch/yaw loads moved before the camKey block | 758 / 115 | **992** | 371 | size-fatal |
| camKey table pointer local / camKey locals / camKey reorder | 11 / 5 | 1008 | 374 | same IR |
| `zero`/`order` locals before or after the loads | 11 / 5 | 1008 | 374 | same IR |
| `0` instead of `0.0f`, `ROTATION_ORDER_YXZ` enum | 11 / 5 | 1008 | 374 | same IR |
| yaw-first load order (landed) | 702 / 51 | **1000** | 373 | race moves to r4 |
| `Matrix` return value assigned | 629 / 992 (prior) | size-fatal | — | prior note |
| far yaw = `yaw` local (no reload) | 604 / 71 (prior) | 984 | — | size-fatal |
| far yaw = `yawTable[race]` local (2 uses) | 604 / 71 | 984 | — | size-fatal |

The `!Globals::iPad` / `== 0` spelling is the only new lever that changes the
near-block codegen at all: it makes the far select reuse the near select's
spilled pointers (saving 8 B, so 1000 B) and gives the original's
`mov r4,r2` + direct `ldr.w r3`, but the literal pool flips to
`Pitch, Yaw, YawIPad`, the spill stays before `mov r4,r2`, and the size is
wrong.

## Why the spill cannot be moved from source

The split point is chosen by `RegAllocGreedy` / `SplitEditor` from the live
interval, not by the pre-RA scheduler (which is identical in both forms).
Every source change that alters the live range enough to move the split
(`volatile int race`, cached `iPad`, loads before the camKey block, yaw-first
load, assume on the int bits) also changes the register assignment
(`race` leaves `r8` for `r4`) and is size-fatal.  The landed form's
`str.w r8` before the yaw load is therefore the measured floor for clang
7.0.2 on this TU, and the 11-byte residual is the landed-semantics floor.

## Conclusion

No source change landed.  The 11-byte residual is the measured floor for
clang 7.0.2 on this TU with source-only changes.  The landed HEAD form trades
the far-key pitch semantics (it passes `yawBits`) for the correct literal-pool
order.  The faithful form buys `mov r4,r2` / direct `ldr.w r3` but flips the
pool (43).  The `pitch volatile / yaw non-volatile` form (44) is the closest
faithful shape (correct pool + `mov r4,r2` + direct `ldr.w r3`) but still
loses the Yaw LDRLIT order and the spill/save slots.  Tree clean; `check`
green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13616
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
