# ModStation constructor — wave 191b (w13710): far-operand/pool decoupling + alias-spelling re-probe

Worker `w13710`, branch `wave191b-w13710`, base master `d4e8f032d`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13710`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13550.md`,
`-w13575`, `-w13586`, `-w13595`, `-w13605`, `-w13616`, `-w13630`,
`-w13641`, `-w13653`, `-w13693`.

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
Section content deltas re-measured this pass: `.text` **71,351**,
`.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**, `.data` **0**,
`.rel.plt` **0** (byte-identical), `.note.gnu.build-id` **20**.
All section *sizes* are exact.

## The residual (unchanged, 11 bytes)

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
`mov r4,r2`/`mov r0,r8` swap.  The landed form is semantically wrong (far
arg1 = `yawBits`, original = `pitchBits`); it trades the far operand for the
correct literal-pool order `Pitch,YawIPad,Yaw`.

## New evidence this pass

### 1. The far operand is the only thing coupling the pool and the schedule

The faithful form (`pitch = pitchBits`, `yaw = yawBits`, `near(pitch,yaw)`,
`far(pitch,...)`, both loads `volatile`) measures **43 / 12** and flips the
pool to `Pitch,Yaw,YawIPad`.  Its near block is otherwise the original:
`ldr.w r2` (pitchBits), `ldr.w r3` (yawBits direct), `mov r4,r2` (save
pitchBits).  So the far operand choice alone flips the LDRLIT emission order
of the `YawIPad`/`Yaw` pair: `far=yawBits` -> `YawIPad` first (correct pool),
`far=pitchBits` -> `Yaw` first (flipped pool).  This is a SelectionDAG
topological-sort tie driven by the live-out pressure of `yawBits` vs
`pitchBits` (confirmed from `-mllvm -print-machineinstrs` at "After
Instruction Selection" and "After Machine Instruction Scheduler").

### 2. Re-using the loaded yaw value in the far call is size-fatal

The original far block re-loads the table (`ldr.w r3,[r1,r0,lsl#2]` from the
spilled pointers), so the far arg2 is a fresh table load, not the `yawBits`
value.  Writing `far(farKey, pitch, yaw, ...)` or
`far(farKey, pitch, *(const float*)&yawBits, ...)` re-uses the loaded value,
kills the far re-load and measures **604 / 71, size 984** (size-fatal).  The
source must re-index the ternary in the far call.

### 3. The 35 form is a hard local optimum; only a yawBits use after the call moves it

The w13641/w13693 35 form is reproduced exactly:

```cpp
int pitchBits = *(const int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
__builtin_assume(pitchBits != 0x7f800001);
__atomic_signal_fence(__ATOMIC_SEQ_CST);
float pitch = *(const float *) &pitchBits;
float yaw   = *(const float *) &yawBits;
near(nearKey, pitch, yaw, 0.0f, (RotationOrder) 2);
far(farKey, pitch, (iPad?YawIPad:Yaw)[race], 0.0f, (RotationOrder) 2);
```

= **35 / 10, size 1008, `.text` 71,375, `.ARM.extab` 577**, pool
`Pitch,YawIPad,Yaw`, direct `ldr.w r3`, `mov r4,r2`.  Its instruction
*multiset* is identical to the original; only the order differs (the `Yaw`
LDRLIT is emitted after `add r0,pc`; `str.w r8`/`mov r4,r2` are hoisted
before the yaw load).  Sweeps that all reproduce 35 byte-identically:

- 120 valid permutations of the six load/bitcast/assume/fence statements
  (constraints: pointer decl before load, assume after pitch load, near last);
- 20 fence positions x 5 orderings (`SEQ_CST`/`ACQUIRE`/`RELEASE`/`ACQ_REL`
  all 35; `RELAXED` size-fatal 1000); two fences identical;
- assume expression (`!= 0x7f800001`, `!= 0x7f800000`, `!= -1`, `>= 0`,
  `< 0`, `!= yawBits`, `(pitchBits & 0x7f800000) != 0x7f800000`, compound);
- assume/fence before and after the near call (pitchBits assume after = 35);
- `race` assume before/after (`>= 0`, `>=0 && <10`, `(unsigned)<10`) = 35;
- far z (`0`/`(float)0`/`0.0`), RotationOrder spelling (`2`/`(int)2`/`0x2`/
  `2u`/enum cast), one-line near call, `const`/`register` float locals,
  `auto`/reference/`reinterpret_cast` bitcasts, int-pointer table views;
- pointer-decl reordering, `if/else` yaw-table selection, inline helper
  returning the pointer (helper = 36), direct table expressions;
- `int farRace = race;`/`race+0`/`race|0`/`race*1` copies for the far index
  (all coalesced), `(void)` filler between the calls;
- `Matrix nearKey`/`int race` declaration reorder, `race` init.

The only perturbation that changes the 35 result is a *second* live use of
`yawBits` after the near call (`__builtin_assume(yawBits != 0x7f800001)`):
**33 / 14, `.text` 71,373**, but it flips the pool back to
`Pitch,Yaw,YawIPad` (both bit values live-out).  Still > 11.

### 4. The landed 11 form is stable under every alias/volatile spelling tried

All of the following reproduce **11 / 5, size 1008** byte-identically:
`reinterpret_cast` bitcasts, `(const char*)table + race*4` pointer arithmetic,
`(const int*)table[race]` int-table views, `const float &`/`float &` aliases,
`auto` locals, `const`/`register` float and int locals, `const`/`volatile`/
`const volatile`/`const * const` table-pointer qualifiers (non-volatile
pointer), inline bitcast arguments (no float locals), float-decl order,
`RotationOrder`/`float zero` locals, near-call z spellings, one-line call,
`race` type (`int`/`unsigned`/`long`/`int32_t`), `Matrix nearKey`/`int race`
declaration order, and `__builtin_assume` on `race`/`iPad`/`pitchBits`/
`yawBits` (before or after the call).  `volatile` float locals (772/121,
size 1016) and a `const float * volatile` yaw pointer (765/152, size 1000)
are size-fatal.

## Measured table (full link + `tools/fn_diff.py`; size 1008 unless noted)

| shape | raw / aligned | size | `.text` | `.ARM.extab` | pool | note |
|---|---:|---:|---:|---:|---|---|
| HEAD landed form | **11 / 5** | 1008 | 71,351 | 577 | Pitch,YawIPad,Yaw | floor; far = yawBits |
| faithful `far=pitchBits`, both volatile | 43 / 12 | 1008 | 71,383 | 577 | Pitch,Yaw,YawIPad | `mov r4,r2`, direct `ldr.w r3` |
| 35 form (non-vol pitch + assume + fence) | 35 / 10 | 1008 | 71,375 | 577 | Pitch,YawIPad,Yaw | correct multiset, wrong order |
| 35 + yawBits assume after call | 33 / 14 | 1008 | 71,373 | 577 | Pitch,Yaw,YawIPad | both bits live-out |
| re-use loaded yaw in far | 604 / 71 | **984** | — | — | — | far re-load lost |
| 35 + helper fn for yaw table | 36 / 11 | 1008 | — | — | correct | |
| 35 + swapped pointer decls | 38 / 11 | 1008 | — | — | correct | |
| landed + volatile float locals | 772 / 121 | **1016** | — | — | correct | size-fatal |
| landed + `const float* volatile` yaw ptr | 765 / 152 | **1000** | — | — | correct | size-fatal |

All size-1008 rows keep `.ARM.exidx` and `.rodata` at **0** diff and
`.ARM.extab` at **577**; `.rel.plt` stays byte-identical.

## Conclusion

The landed 11-byte form remains the measured floor.  The far operand and the
literal-pool order are coupled through the live-out pressure of the two
loaded bit patterns: `far=yawBits` gives the correct pool but the
`mov r3,r4` tail; `far=pitchBits` gives the faithful `mov r4,r2`/direct
`ldr.w r3` but flips the pool.  The 35 form (assume+fence) reaches the
correct pool *and* the correct instruction multiset at 1008 B, but its
post-RA schedule (`Yaw` LDRLIT slot, `str.w r8`/`mov r4,r2` hoisted before
the yaw load) is a ScheduleDAGRRList/RA tie that 120 statement permutations
and ~150 fence/assume/alias spellings do not move.  No source change landed;
tree clean; `check` green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13710
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
