# ModStation constructor — wave 191b (w13630): post-RA copy-swap / pool-vs-schedule re-probe

Worker `w13630`, branch `wave191b-w13630`, base master `64756f0a6`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13630`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`modstation-ctor-wave191b-w13575.md`, `modstation-ctor-wave191b-w13586.md`,
`modstation-ctor-wave191b-w13595.md`, `modstation-ctor-wave191b-w13605.md`,
`modstation-ctor-wave191b-w13616.md`.

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
Section content deltas re-measured this pass: `.text` **71,199**,
`.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**, `.data` **0**,
`.rel.plt` **0** (byte-identical), `.dynsym`/`.dynstr`/`.hash`/`.rel.dyn`
**0**, `.note.gnu.build-id` **20**.  All section *sizes* are exact.

## The residual (unchanged)

Original near block `0xd5436..0xd5470`:

```
ldr r0,Pitch ; ldr r3,YawIPad ; ldr r4,Yaw
add r0,pc ; add r3,pc ; ldrb r1,[sl] ; add r4,pc
ldr.w r2,[r0,r8,lsl#2]      ; pitchBits -> r2
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
`far(pitch,...)`; far operand = `yawBits`):

```
...
str.w r8,[sp,#184]          ; spill BEFORE the yaw load
ldr.w r4,[r0,r8,lsl#2]      ; yawBits -> r4 (not r3)
movs r0,#2 ...
mov r3,r4                   ; copy yawBits into near arg2
mov r0,r8
blx
```

The 11 raw bytes are exactly the `str.w`/`ldr.w` slot rotation, the
`ldr.w r3`/`ldr.w r4` register field, and the `mov r0,r8`/`mov r3,r4` +
`mov r4,r2`/`mov r0,r8` swap.

## New evidence: the landed form's last two copies are swapped by the *Post RA top-down list latency scheduler*

`-mllvm -print-machineinstrs` on the landed form (scratch `mirl.log`,
108 MB) shows the copy order is **r0 then r3** from ISel all the way through
`After Virtual Register Rewriter` / `After Stack Slot Coloring` /
`After PostRA Machine Sink`:

```
$r0 = COPY renamable $r8, debug-location !8602
$r3 = COPY renamable $r4, debug-location !8602
tBL ... MatrixSetRotation
```

The `After Post RA top-down list latency scheduler` stage is the first place
the order becomes `$r3 = tMOVr $r4` then `$r0 = tMOVr $r8`, i.e. the final
`mov r3,r4; mov r0,r8`.  The original has `mov r0,r8` then `mov r4,r2`.
Therefore **if the latency scheduler's swap could be suppressed, raw would
drop from 11 to 9** (the first copy would match; only the second 2-byte mov
would differ).  No source-only lever found to influence this post-RA pass:
changing the z/order arguments to locals (`float zero = 0.0f;`,
`RotationOrder ord = ...`), the near-call argument order, or the
`pitch`/`yaw` declaration order leaves the final code byte-identical (11/5).

## New evidence: the 44 form's *pool is already correct*; only the schedule is wrong

The `pitch volatile / yaw non-volatile / far=pitchBits` form (44 raw) was
re-dumped (`mir44.log`).  At `# After Instruction Selection:` the LDRLIT
order is already the original `Pitch, YawIPad, Yaw`:

```
%115 = tLDRLIT @gHangarCamPitch
%117 = tLDRLIT @gHangarCamYawIPad
%120 = tLDRLIT @gHangarCamYaw
```

so the literal pool is correct.  The `# After Machine Instruction Scheduler:`
(pre-RA) order is:

```
Pitch LDRLIT ; YawIPad LDRLIT ; ldrb iPad ; pitch load (volatile)
Yaw LDRLIT ; cmp ; select ; yaw load ; EH_LABEL ; ADJCALLSTACKDOWN ;
movs 2 ; movs 0 ; ...
```

i.e. the pre-RA scheduler sinks the `Yaw` LDRLIT below the pitch load because
the yaw load is non-volatile (the volatile pitch load is ordered after the
`ldrb`).  The final code sinks it further, below `cmp`.  This is why the 44
form has the correct pool and the correct instruction multiset
(`mov r4,r2`, direct `ldr.w r3`) yet is 44 raw: the sunk LDRLIT shifts the
whole near block.  Making the yaw load volatile hoists the LDRLITs but flips
the ISel pool order to `Pitch, Yaw, YawIPad` (the faithful 43 form).

## New evidence: why the `__builtin_assume` form is 1000 B

`pitch non-volatile + __builtin_assume(pitchBits != ...)` keeps the pitch
load as `ldr.w` and gives the original near-block schedule (yaw load ->
`movs r0,#2` -> spill -> save), but the non-volatile pitch load is hoisted
*before* the `ldrb` at ISel:

```
Pitch LDRLIT ; iPad LDRLIT ; pitch load ; iPad GOT load ; YawIPad ; Yaw ;
ldrb ; cmp ; select ; yaw load ...
```

Greedy RA then allocates `race` to `r4` (not `r8`) and pitchBits to `r1`
(-> `r4` -> `r2`, two moves), so `mov.w r8,#8`/`mov.w r8,#7` become 2-byte
`movs r4,#8`/`movs r4,#7` and the function is 1000 B.  A volatile pad
(`volatile int pad = 0;`) forces `race` back to `r8` and the spill after
`movs`, but adds a store+reload and is 490 raw.  Every assume spelling and
placement tried (constant, `>`, `<`, `^`, `-`, `*`, `&&`, after the near
call, before the declarations, multiple assumes) is byte-identical 702/62,
1000 B.

## Source shapes measured this pass (full link + `tools/fn_diff.py`)

`measure.py` = `link_so -j2` under `ulimit -v 12000000` + `tools/fn_diff.py`
(~1 s per candidate; every row is a full link measurement).  All rows
size-exact 1008 B unless noted.

| shape | raw / aligned | size | insn | note |
|---|---:|---:|---:|---|
| HEAD landed form | **11 / 5** | 1008 | 374 | floor |
| faithful `far(pitchBits)`, both volatile | 43 / 12 | 1008 | 374 | pool `Pitch,Yaw,YawIPad` |
| 44: pitch volatile / yaw non-volatile, far=pitch | 44 / 18 | 1008 | 374 | pool correct, `mov r4,r2`, direct `ldr.w r3`; Yaw LDRLIT sunk |
| 44 + `__builtin_assume(yawBits != ...)` | 44 / 18 | 1008 | 374 | no change |
| 44 + `__builtin_assume(pitchBits != ...)` | 44 / 18 | 1008 | 374 | no change |
| 44, yaw load as plain `float` | 44 / 18 | 1008 | 374 | same |
| 44, yaw-first source order | 44 / 18 | 1008 | 374 | scheduler reorders back |
| 44, near args swapped | 45 / 21 | 1008 | 374 | |
| faithful, `iPad == 1 ? YawIPad : Yaw` | 44 / 13 | 1008 | 374 | `cmp #1`, pool still `Pitch,Yaw,YawIPad` |
| faithful, `(iPad ^ 1) == 0` / `iPad - 1 == 0` / `1 == iPad` | 44 / 13 | 1008 | 374 | same family |
| faithful, `(iPad & 1) != 0` / `iPad % 2 == 1` | 517 / 26 | 1008 | 372 | extra `and`/`tst` |
| faithful, `>0` / `>=1` / `<1` / `<=0` / `(bool)` / `==false` | 43 / 12 | 1008 | 374 | canonicalised |
| faithful, `if/else` pointer select | 43 / 12 | 1008 | 374 | same IR |
| faithful, separate `yawIPadPtr`/`yawPtr` locals (both orders) | 43 / 12 | 1008 | 374 | CSE'd |
| faithful, `float farPitch` local before/after near | 43 / 12 | 1008 | 374 | same SSA |
| faithful, near inline bitcast / far inline bitcast | 43 / 12 | 1008 | 374 | same SSA |
| faithful, split `int pitchBits;` declaration | 43 / 12 | 1008 | 374 | same IR |
| faithful, volatile read of `Globals::iPad` | 43 / 12 | 1008 | 374 | same code |
| faithful, swapped `gHangarCamYawIPad`/`gHangarCamYaw` definitions | 43 / 12 | 1008 | 374 | pool order unchanged (IR, not address) |
| faithful, pointer locals before the camKey block | 564 / 83 | 1008 | 373 | schedule wrecked |
| faithful, loads before the camKey block | 710 / 145 | **992** | 373 | size-fatal |
| both non-volatile int bits | 667 / 59 | 1008 | 375 | pitch folds to `vldr`+`vmov` |
| plain `float pitch = table[race]` (all spellings) | 667 / 59 | 1008 | 375 | `vldr`+`vmov` |
| `const int *` table view (non-volatile) | 667 / 59 | 1008 | 375 | folds |
| `const int *` table view, volatile pitch | 44 / 18 | 1008 | 374 | |
| inline bitcast at both calls, non-volatile | 667 / 59 | 1008 | 375 | folds |
| `__builtin_expect(pitchBits, 0)` int use | 667 / 59 | 1008 | 375 | folds |
| `if (pitchBits == ...) __builtin_unreachable();` | 666 / 60 | 1008 | 375 | branch survives |
| non-volatile pitch + `__builtin_assume` (all spellings/positions) | 702 / 62 | **1000** | 373 | race -> r4 |
| non-volatile pitch + assume, yaw volatile | 702 / 53 | **1000** | 373 | race -> r4 |
| assume + condition spellings (`==1`, `!=0`, `>0`, …) | 702 / 62-63 | **1000** | 373 | no change |
| assume + multiple assumes (race/yaw/iPad) | 702 / 62 | **1000** | 373 | no change |
| assume + `volatile int pad = 0;` | 490 / 138 | 1008 | 374 | race -> r8, spill after movs, but pad store/reload |
| landed + `float zero`/`RotationOrder ord` locals | 11 / 5 | 1008 | 374 | same IR |

## Conclusion

The landed 11-byte form remains the measured floor for clang 7.0.2 on this
TU.  Two new facts sharpen the picture:

1. The 44 form's literal pool is already correct at ISel; its 44 raw is
   entirely post-ISel scheduling (the non-volatile yaw load lets the
   pre-RA scheduler sink the `Yaw` LDRLIT below the pitch load).  Making the
   yaw load volatile hoists it but flips the ISel pool order, so pool and
   schedule remain coupled through yaw volatility.
2. The landed form's final `mov r3,r4` / `mov r0,r8` order is produced by the
   *Post RA top-down list latency scheduler* (MIR order is r0 then r3).  If
   that swap could be suppressed, raw would fall to 9; no source-only lever
   was found.

No source change landed; tree clean; `check` green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13630
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
