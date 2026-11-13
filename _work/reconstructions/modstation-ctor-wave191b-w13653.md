# ModStation constructor — wave 191b (w13653): fold-blocker / statement-permutation re-probe

Worker `w13653`, branch `wave191b-w13653`, base master `2f38a4bfd`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13653`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`modstation-ctor-wave191b-w13575.md`, `modstation-ctor-wave191b-w13586.md`,
`modstation-ctor-wave191b-w13595.md`, `modstation-ctor-wave191b-w13605.md`,
`modstation-ctor-wave191b-w13616.md`, `modstation-ctor-wave191b-w13630.md`,
`modstation-ctor-wave191b-w13641.md`.

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
**0**, `.note.gnu.build-id` **20**.  All section *sizes* are exact.

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
`mov r4,r2`/`mov r0,r8` swap.

## New evidence this pass

### 1. The original's own `OnInitialize` shows what a plain-float pitch looks like

`ModStation::OnInitialize` (`0xd5708`) contains the same source pattern
(`float pitch = gHangarCamPitch[race]; MatrixSetRotation(camera, pitch,
(iPad?YawIPad:Yaw)[race], -0.03f, 2)`), and there the pitch load is
`vldr s22` + `vmov r2,s22` (`0xd63fc`/`0xd6406`) because the same `pitch`
value is also stored into a `Vector` later.  The ctor's `ldr.w r2` is
therefore not reachable from a plain `float` local: the ctor source must
have loaded the table entry as an integer bit pattern (or otherwise kept it
in a core register).  This confirms the int-bitcast premise of all prior
waves but does not by itself give a new lever.

### 2. Fold-blocker sweep: only `volatile` and `__builtin_assume` keep `ldr.w`

Every non-volatile int-load spelling that is not protected by an assume
folds back to `vldr`+`vmov` (667/59 or 666/60, size 1008).  Tested:

`pitchBits ^= 0`, `+= 0`, `*= 1`, `&= -1`, `|= 0`, `-= 0`, `<<= 0`,
`__builtin_assume_aligned(pitchTable,4)`, `__builtin_assume(race>=0 && race<10)`,
`float pf = pitchTable[race]; int pitchBits = *(int*)&pf`,
`float pf = ...; union {float f; int i;} u; u.f = pf; int pitchBits = u.i`,
`__builtin_memcpy(&pitchBits, &pf, 4)`, and a `const int *` table view.
All fold.  `__builtin_assume(pitchBits != 0x7f800001)` and `volatile` are the
only two keepers found.

### 3. `assume`+`fence` 35 form reproduced; fence position/ordering is a dead end

The w13641 closest faithful shape is reproduced exactly:

```
int pitchBits = *(const int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
__builtin_assume(pitchBits != 0x7f800001);
__atomic_signal_fence(__ATOMIC_SEQ_CST);
float pitch = *(const float *) &pitchBits;
float yaw   = *(const float *) &yawBits;
near(pitch, yaw);  far(pitch, (iPad?YawIPad:Yaw)[race]);
```

= **35 / 10, size 1008, pool `Pitch,YawIPad,Yaw`, `mov r4,r2`, direct
`ldr.w r3`**.  Its residual is the `Yaw` LDRLIT emitted one slot late (after
`add r0,pc`) plus the tail schedule (`str.w`/`mov r4,r2` hoisted before the
yaw load).

Fence sweep this pass: 6 source positions (before pitch load, between the two
loads, after both loads, after the float locals, after the near call, none) x
5 orderings (`SEQ_CST`, `ACQUIRE`, `RELEASE`, `ACQ_REL`, `RELAXED`).  Only
positions "after both loads" and "after the float locals" keep size 1008; all
five orderings give byte-identical **35 / 10**.  A second fence is identical.
`__atomic_thread_fence` / `__sync_synchronize` give **498 / 35** (size 1008);
fence before the yaw load gives size **1000** (race -> r4).

### 4. `yawElem` / pointer-before-load does not hoist the sunk `Yaw` LDRLIT

Computing the yaw element address before the pitch load
(`const float *yawElem = &yawTable[race];` or `yawTable + race`, then loading
through it) is CSE'd/reordered back: the 44 form (pitch volatile / yaw
non-volatile) stays **44 / 18**; the 43 form stays **43 / 12**; the 35 form
stays **35 / 10**.  A `volatile` element pointer adds a store/reload
(**278 / 151** or **44 / 18**).

### 5. Full statement-permutation sweep of the six load/bitcast statements

All 60 valid permutations of
`{pitchTable decl, yawTable decl, pitch load, yaw load, pitch bitcast, yaw
bitcast}` (loads after their pointer decls, bitcasts after their loads) x
{far first = pitch, yaw} x {near arg order py, yp}, with both loads
`volatile`.  Floor **11 / 5** (the landed form); faithful floor **43 / 12**.
No permutation produced a new pool/schedule combination.

### 6. Condition-spelling sweep on the landed form

`Globals::iPad ? YawIPad : Yaw` (base), `!iPad ? Yaw : YawIPad`,
`iPad==0`, `iPad!=0`, `(bool)iPad`, `iPad>0`, `iPad<1`, `!!iPad`,
`__builtin_expect(iPad,0)`, `__builtin_expect(iPad!=0,0)` all give
**11 / 5**.  `iPad==0 ? Yaw : YawIPad` with `__builtin_expect(...,1)` gives
13/7; `iPad==1`/`iPad!=1` give 15/9 and 13/7; `(iPad&1)` gives 517/26.

### 7. Bit type and explicit-save variants

`int`, `unsigned`, `long`, `int32_t`, `unsigned long` for the bit locals are
all identical (**11 / 5** landed, **43 / 12** faithful).  Explicit
`int savedPitchBits = pitchBits;` / `float savedPitch = pitch;` after the near
call is CSE'd (**43 / 12**); volatile saves are much worse (619/178, 619/180).

## Measured table (full link + `tools/fn_diff.py`; all size-exact 1008 B unless noted)

| shape | raw / aligned | size | pool | note |
|---|---:|---:|---|---|
| HEAD landed form | **11 / 5** | 1008 | Pitch,YawIPad,Yaw | floor |
| faithful `far=pitchBits`, both volatile | 43 / 12 | 1008 | Pitch,Yaw,YawIPad | pool flips |
| faithful, pitch volatile / yaw non-volatile | 44 / 18 | 1008 | Pitch,YawIPad,Yaw | `mov r4,r2`, direct `ldr.w r3`; Yaw LDRLIT sunk below `cmp` |
| 44 + fence (any of 5 positions/orderings) | 44 / 18 | 1008 | correct | no change |
| 44 + `yawElem` before pitch load | 44 / 18 | 1008 | correct | reordered back |
| faithful + `__builtin_assume(pitchBits != ...)` | 702 / 53 | **1000** | correct | race -> r4 |
| faithful + assume + trailing fence | **35 / 10** | 1008 | Pitch,YawIPad,Yaw | best faithful shape; Yaw LDRLIT late |
| 35 + fence position/ordering sweep | 35 / 10 | 1008 | correct | identical |
| 35 + `yawElem` / pointer-before-load | 35 / 10 | 1008 | correct | identical |
| 35 + `float zero` / `RotationOrder order` locals | 35 / 10 | 1008 | correct | identical |
| 35 + `int farRace = race;` | 35 / 10 | 1008 | correct | identical |
| both non-volatile int bits, no assume | 667 / 59 | 1008 | correct | folds to `vldr`+`vmov` |
| both non-volatile + assume | 702 / 62 | **1000** | correct | race -> r4 |
| both non-volatile + fence only | 629 / 69 | 1008 | correct | race -> r4 |
| plain `float pitch/yaw` (landed usage) | 672 / 62 | 1008 | correct | `vldr`+`vmov` |
| plain `float pitch/yaw` (faithful usage) | 667 / 59 | 1008 | correct | `vldr`+`vmov` |
| int-table view (`const int*`) + volatile | 43 / 12 | 1008 | flipped | same as 43 |
| int-table view + volatile yaw only | 35 / 10 | 1008 | correct | same as 35 |
| `union` bitcast (volatile int member) | 525 / 136 | 1008 | correct | extra stack traffic |
| `__atomic_thread_fence` / `__sync_synchronize` | 498 / 35 | 1008 | correct | different barrier |
| landed + `yawElem` before pitch load | 11 / 5 | 1008 | correct | identical |
| landed + bit type variants | 11 / 5 | 1008 | correct | identical |
| landed + condition spellings | 11-15 / 5-9 | 1008 | correct | `expect(...,1)` etc. worse |
| landed + `int farYawBits` local | 29 / 7 | 1008 | correct | extra int load |
| landed + `volatile int race` | 873 / 140 | **1016** | correct | size-fatal |

## Conclusion

The landed 11-byte form remains the measured floor for clang 7.0.2 on this
TU.  The new sweeps (fold-blockers, all valid statement permutations, fence
position/ordering, condition spellings, bit types, explicit saves,
pointer-before-load) confirm the two facts are coupled through the far
operand: `far=pitchBits` gives the faithful `mov r4,r2`/direct `ldr.w r3`
but flips the literal pool; `far=yawBits` gives the correct pool but the
`mov r3,r4` tail.  The 35 form proves both can be reached simultaneously
(correct pool + `mov r4,r2` + direct `ldr.w r3` at 1008 B), but its `Yaw`
LDRLIT emission slot and post-RA tail remain wrong and measure 35 raw.
No source change landed; tree clean; `check` green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13653
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
