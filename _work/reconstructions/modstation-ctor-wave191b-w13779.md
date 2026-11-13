# ModStation constructor — wave 191b (w13779): faithful-form re-probe

Worker `w13779`, branch `wave191b-w13779`, base master `976464874`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13779`.

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no raw win below 11 found.**
Tree clean; `check` green at the entry counts.

Predecessor notes (read first): `modstation-extab-lp-wave149-w9520.md`,
`modstation-ctor-wave191b-w13738.md`, `-w13710`, `-w13693`, `-w13653`,
`-w13641`, `modstation-ctor-pass4-w4220.md`.

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
`RATCHET PASS`, byte_exact **4508/4524**, linked_exact **4508/4524**,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint **CLEAN ✓**, drift **0**, parity 5370/5370.

Section content deltas re-measured this pass (all section *sizes* exact,
`.rel.plt` byte-identical): `.text` **74,628**, `.ARM.extab` **562**,
`.note.gnu.build-id` **20**; `.ARM.exidx`, `.rodata`, `.data`, `.bss` all **0**.

## The residual is exactly the near `MatrixSetRotation` tail

`tools/fn_diff.py --json` reports raw 11 / aligned 5 with categories
`opcode:ldr.w/str.w` 1, `extra-ours:ldr.w` 1, `extra-orig:str.w` 1,
`extra-ours:mov` 1, `extra-orig:mov` 1.  The differing bytes are function
offsets `0x186, 0x188..0x18f, 0x19c, 0x19e` (absolute `0xd5456..0xd545f`,
`0xd546c`, `0xd546e`).  Everything before `0xd5456` and the whole far block
from `0xd5474` are byte-identical.

```
orig 0xd5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; yawBits, direct into r3
orig 0xd545a: 2002       movs  r0,#2
orig 0xd545c: f8cd 80b8  str.w r8,[sp,#184]       ; race spill AFTER movs
orig 0xd5460: 2100       movs  r1,#0
orig 0xd5462: f50d 7880  add.w r8,sp,#256
orig 0xd5466: e9cd 1000  strd  r1,r0,[sp]
orig 0xd546a: a950       add   r1,sp,#320
orig 0xd546c: 4640       mov   r0,r8
orig 0xd546e: 4614       mov   r4,r2               ; save pitchBits for far

ours 0xd5456: f8cd 80b8  str.w r8,[sp,#184]       ; spill BEFORE the load
ours 0xd545a: f850 4028  ldr.w r4,[r0,r8,lsl#2]   ; yawBits -> r4
ours 0xd545e: 2002       movs  r0,#2
ours 0xd5460: 2100       movs  r1,#0
ours 0xd5462: f50d 7880  add.w r8,sp,#256
ours 0xd5466: e9cd 1000  strd  r1,r0,[sp]
ours 0xd546a: a950       add   r1,sp,#320
ours 0xd546c: 4623       mov   r3,r4               ; copy yawBits into arg2
ours 0xd546e: 4640       mov   r0,r8
ours 0xd5470: f78d edf6  blx   MatrixSetRotation
```

## Ghidra evidence: both calls take pitchBits as arg1

`decompile_function` at `0xe52d0` (file VMA `0xd52d0`; saved at
`scratch/w13779/decomp_ctor.txt`) shows:

```
uVar19 = *(undefined4 *)(DAT_000e56b0 + 0xe5440 + iVar5 * 4);   // pitchBits
...
MatrixSetRotation(&local_b8, &local_78, uVar19, *(iVar6 + iVar5*4), 0, 2);  // near
...
MatrixSetRotation(aAStack_f4, &local_b8, uVar19, *(*piVar1 + local_100*4), 0, 2); // far
```

So the **near** call is `(pitchBits, yawBits)` and the **far** call is
`(pitchBits, reloadedSelectedYaw)`.  The landed 11-byte form is semantically
wrong: it swaps the two float locals so the far arg1 becomes `yawBits`
(`mov r2,r4` in the original far block is `pitchBits`; landed keeps `yawBits`
live instead).  The landed form trades the far operand for the correct
literal-pool order `Pitch,YawIPad,Yaw`.

The faithful form (`far=pitchBits`) is the mirror allocation: `pitchBits`
live across the near call (callee-saved `r4`, `mov r4,r2`), `yawBits` dead
(direct `ldr.w r3`).  It reaches the original instruction multiset but costs
35–43 raw bytes because clang 7's post-RA scheduler hoists `str.w r8` and
`mov r4,r2` above the yaw load and emits the `Yaw` LDRLIT after `add r0,pc`.

## Measured variants this pass

All full `link_so`, size 1008 unless noted.  "landed" = far arg1 is
`yawBits`; "faithful" = far arg1 is `pitchBits`.

| variant | raw / aligned | size | note |
|---|---:|---:|---|
| landed HEAD (base) | **11 / 5** | 1008 | far=yawBits, both loads volatile |
| landed + assume(pitch) | 11 / 5 | 1008 | no change |
| landed + assume(yaw) | 11 / 5 | 1008 | no change |
| landed + fence | 11 / 5 | 1008 | no change |
| landed + assume+fence | 11 / 5 | 1008 | no change |
| grid: far=yaw, any volatile/assume/fence cell | **11 / 5** | 1008 | 32 cells, all 11 |
| faithful both volatile (`faithful`) | 43 / 12 | 1008 | pool flips |
| faithful inline int bitcasts (`inline_int_v`) | 43 / 12 | 1008 | direct `ldr.w r3`, `mov r4,r2` |
| faithful pitch-float local (`pitch_float`) | 43 / 12 | 1008 | |
| faithful pitch-float + fence | 42 / 12 | 1008 | |
| faithful inline + fence | 42 / 12 | 1008 | |
| faithful inline + assume + fence (`inline_assume_fence`) | 35 / 10 | 1008 | == w13641/w13693 35 form |
| faithful fence after yaw load (`f_after_yaw_v`) | 35 / 10 | 1008 | |
| faithful two fences (`f_two`) | 36 / 10 | 1008 | |
| faithful fence after yaw, no assume | 702 / 62 | **1000** | size-fatal |
| faithful fence between loads, no assume | 700 / 63 | **1000** | size-fatal |
| faithful local yawTable in far | 604 / 71 | **984** | size-fatal (kills far reload) |
| natural `float pitch = gHangarCamPitch[race]` | 667 / 59 | 1008 | 375 insn, vldr+vmov |
| natural, volatile | 352 / 116 | **1000** | size-fatal |
| natural, far reloads pitch | 667 / 59 | 1008 | 375 insn |
| natural, far reloads pitch, volatile | 750 / 112 | **1000** | size-fatal |
| non-volatile int bitcasts (`inline_int`) | 667 / 59 | 1008 | 375 insn |
| non-volatile pitch, volatile yaw (`inline_int_pv`) | 666 / 60 | 1008 | 375 insn |
| landed naming, far=pitchBits (`landed_far_pitch`) | 43 / 12 | 1008 | |

Prior passes additionally measured ~200 source permutations and ~2,000
per-TU flag cells (w13738: 42 flags + 20 two-flag combos; w13710: 120
permutations, 20 fence positions × 5 orderings, assume expression variants).
No cell beats raw 11.

## Conclusion

The 11-byte landed form is the measured floor.  The original is the faithful
form (`far=pitchBits`), whose clang-7 post-RA schedule differs by 35 bytes
(`str.w r8`/`mov r4,r2` hoisted above the yaw load; `Yaw` LDRLIT emitted
after `add r0,pc`).  The landed form is a semantically-wrong mirror that
happens to keep the correct literal-pool order and the smallest byte delta.
No source change; tree clean; `check` green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13779
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```
