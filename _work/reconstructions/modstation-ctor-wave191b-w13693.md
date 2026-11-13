# ModStation constructor — wave 191b (w13693): far-operand/pool decoupling re-probe

Worker `w13693`, branch `wave191b-w13693`, base master `b3fd7e6a2`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13693`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`-w13575`, `-w13586`, `-w13595`, `-w13605`, `-w13616`, `-w13630`,
`-w13641`, `-w13653`.

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
Section content deltas re-measured this pass: `.text` **71,270**,
`.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**, `.data` **0**,
`.rel.plt` **0** (byte-identical), `.note.gnu.build-id` **20**.
All section *sizes* are exact.

## The residual is exactly 11 bytes, all in the near block

Byte-level diff of the landed HEAD form vs the original (differing bytes only):

```
vma 0xd5456 (1): orig 50       | ours cd          ; 2nd byte of instr at d5456
vma 0xd5458 (8): orig 28 30 02 20 cd f8 b8 80
                 ours b8 80 50 f8 28 40 02 20
vma 0xd546c (1): orig 40       | ours 23          ; mov r0,r8 vs mov r3,r4
vma 0xd546e (1): orig 14       | ours 40          ; mov r4,r2 vs mov r0,r8
```

i.e. the original schedules

```
d5456: ldr.w r3,[r0,r8,lsl#2]   ; yawBits -> r3
d545a: movs  r0,#2              ; RotationOrder
d545c: str.w r8,[sp,#184]       ; spill race
...
d546c: mov   r0,r8              ; nearKey sret
d546e: mov   r4,r2              ; save pitchBits for far
```

while the landed form schedules

```
d5456: str.w r8,[sp,#184]       ; spill race
d545a: ldr.w r4,[r0,r8,lsl#2]   ; yawBits -> r4 (far live-out)
d545e: movs  r0,#2
...
d546c: mov   r3,r4              ; copy yawBits into near arg2
d546e: mov   r0,r8
```

The landed form is a semantics hack: `float yaw = *(float*)&pitchBits;
float pitch = *(float*)&yawBits;` with `near(yaw,pitch)` (correct
`near(Pitch,Yaw)`) but `far(pitch, reload)` passes **Yaw** where the
original passes **Pitch**.  It trades the far operand for the correct
literal-pool order `Pitch,YawIPad,Yaw`.  The faithful far operand flips
the pool.

## New evidence this pass

### 1. The far operand and the literal-pool order are coupled in every
###    non-assume spelling

The near call is `(Pitch, Yaw)` in the original: `r2 = Pitch[race]`,
`r3 = Yaw[race]`.  The far call is `(Pitch, Yaw reload)`: `r4 = r2`
(`mov r4,r2`) and `r3 = selectedYaw[race]` (a fresh `ldr.w`).  So the
original source is *faithful*: `far(pitch, ...)`.

Every faithful spelling with both loads `volatile` measures **43 / 12**
and flips the pool to `Pitch,Yaw,YawIPad`.  Changing only the far operand
to the landed `yawBits` restores the pool and measures **11 / 5**.  The
loads are `ldr.w` (int), not `vldr`: a plain `float` load is folded to
`vldr`+`vmov` because the value is live across the call (667/59), so the
source must load the table entry as an integer bit pattern.

### 2. A faithful form with the correct pool exists at 35 / 10, but its
###    schedule is wrong

`int pitchBits = *(const int*)&pitchTable[race];` (non-volatile) +
`int yawBits = *(const volatile int*)&yawTable[race];` +
`__builtin_assume(pitchBits != 0x7f800001);` +
`__atomic_signal_fence(__ATOMIC_SEQ_CST);` + `near(pitch,yaw)` +
`far(pitch, direct ternary)` = **35 / 10, size 1008**, pool
`Pitch,YawIPad,Yaw`, `mov r4,r2`, direct `ldr.w r3`.  Its 35 bytes are
the wrong `Yaw` LDRLIT slot (`add r0,pc` before `ldr r4,Yaw`), the
`add r4,pc` slot, and the spill/`mov r4,r2` hoisted before the yaw load.
The assume/fence placement (11 positions), assume expression
(`!= 0`, `>= 0`, `< 0`, `!= -1`, `!= 0x7f7fffff`, on `race`, on
`yawBits`, both), float-local order, `const`/`register` locals, and
`MatrixSetRotation` z/order spellings (`0`, `0.0`, `(float)0`, `2u`,
`(int)2`, `0x2`) all reproduce **35 / 10** byte-identically.

### 3. The landed form is a stable local optimum at 11

The landed form is unchanged by: `race` type (`unsigned`, `long`,
`int32_t`, `signed`, `unsigned int`), load order permutations, bitcast
vs direct arguments, `const`/`register` locals, and pointer qualifiers.
`short race` is size-fatal (1000).  `-0.0f` z is 554/24.  A volatile
copy of `pitch` is 586/172.

### 4. The spill is an RA live-range split, not a post-RA scheduler move

`-mllvm -print-machineinstrs` on the 35 form (scratch `mir35.txt`) shows
the spill is already in the *After Greedy Register Allocator* output,
placed immediately before the yaw load, and the yaw load is the last use
of `race` in the block.  The original's spill is *after* `movs r0,#2`
and the yaw load does not kill `r8`, i.e. the original's RA chose the
clobber point (`add.w r8,sp,#256`) rather than the last use as the split
point.  Adding `__builtin_assume(race >= 0)` after the yaw load,
`int farRace = race;` copies, explicit `int race3 = race * 3;`, and
`int idx = race` aliases are all coalesced and leave the 35 form
byte-identical.  The landed form's RA output likewise already has the
spill before the yaw load.

### 5. Direct table expressions (no pointer locals) lose the pointer spill

Using `gHangarCamPitch[race]` / `(ternary)[race]` directly (no
`pitchTable`/`yawTable` locals) with both loads volatile gives
**698 / 52, size 1000** — the yaw pointer is no longer spilled to
`[sp,#176]/[sp,#180]` and the far block differs.  The original's far
block re-selects from the spilled pointers (`ldrd r0,r1,[sp,#176]`), so
the pointer locals are part of the original source.

## Measured table (full link + `tools/fn_diff.py`; size 1008 unless noted)

| shape | raw / aligned | size | `.text` diff | `.ARM.extab` | note |
|---|---:|---:|---:|---:|---|
| HEAD landed form | **11 / 5** | 1008 | 71,270 | 577 | floor |
| faithful `far=pitch`, both volatile | 43 / 12 | 1008 | 71,302 | 577 | pool `Pitch,Yaw,YawIPad` |
| faithful + `__atomic_signal_fence` at top | 39 / 11 | 1008 | 71,298 | 577 | correct pool, wrong tail |
| faithful, pitch volatile / yaw non-volatile | 44 / 18 | 1008 | 71,303 | 577 | correct pool, Yaw LDRLIT sunk |
| 35 form (non-vol pitch + assume + fence) | 35 / 10 | 1008 | 71,294 | 577 | correct pool + `mov r4,r2`; Yaw LDRLIT late |
| 35 + assume/fence position/expression sweep | 35 / 10 | 1008 | 71,294 | 577 | identical |
| 35 + `farRace`/`race3`/`idx` copies | 35 / 10 | 1008 | 71,294 | 577 | coalesced |
| direct table exprs, both volatile | 698 / 52 | **1000** | — | — | pointer spill lost |
| plain `float` pitch/yaw (faithful) | 667 / 59 | 1008 | — | — | `vldr`+`vmov` |
| plain `float` pitch/yaw (landed) | 672 / 62 | 1008 | — | — | `vldr`+`vmov` |
| int-table view (`const int*`) + volatile | 43 / 12 | 1008 | — | — | same as 43 |
| int-table view + volatile yaw only | 35 / 10 | 1008 | — | — | same as 35 |
| landed + float decls pitch-first | 44 / 14 | 1008 | — | — | decl order matters |
| landed + `short race` | 727 / 62 | **1000** | — | — | size-fatal |
| landed + `-0.0f` z | 554 / 24 | 1008 | — | — | different literal |
| landed + volatile pitch copy | 586 / 172 | 1008 | — | — | extra stack traffic |

All rows keep `.ARM.exidx` and `.rodata` at **0** diff; `.ARM.extab` is
**577** for every size-1008 row measured (the near-block schedule does
not change the unwind tables).  The 35 form is a net `.text` loss
(71,294 vs 71,270), so it was not landed.

## Conclusion

The landed 11-byte form remains the measured floor.  The original source
is faithful (`far(pitch, ...)`); the landed form trades the far operand
for the literal-pool order.  The 35 form proves the correct pool and the
faithful `mov r4,r2`/direct `ldr.w r3` are simultaneously reachable at
1008 B, but its `Yaw` LDRLIT emission slot and the RA spill/`mov r4,r2`
placement remain wrong and measure 35 raw.  No source change landed;
tree clean; `check` green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13693
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
