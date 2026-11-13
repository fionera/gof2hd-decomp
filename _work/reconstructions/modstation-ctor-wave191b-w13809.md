# ModStation constructor — wave 191b (w13809): near-tail scheduler probe (checkpoint 2)

Worker `w13809`, branch `wave191b-w13809`, base master `088c6e950`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13809`.

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  **Result: no source change landed; no shape below the landed raw 11 /
aligned 5 was found.**  Tree clean; `check` green at the entry counts.

Predecessor notes (read first): `modstation-ctor-wave191b-w13786.md` (full
prior analysis, ~250 variants), `-w13779`, `-w13738`, `-w13710`, `-w13693`,
`-w13653`, `-w13641`, `-w13616`, `modstation-extab-lp-wave149-w9520.md`,
`modstation-ctor-pass4-w4220.md`.

## Entry / exit gate (full `check`, `-j2`, `ulimit -v 12000000`)

`RATCHET PASS`, byte_exact **4508/4524**, linked_exact **4508/4524**,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data **843/843**, imports **207**, lint **CLEAN ✓**, drift **0**,
parity **5370/5370**.  Exit state is identical (no source change).

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

Measured fresh this session with
`python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so
_ZN10ModStationC1Ev _ZN10ModStationC2Ev` after a clean `link_so`.

Section facts (direct ELF byte compare, sizes all exact, `.rel.plt` 0):
`.text` **75,937** differing bytes, `.ARM.extab` **594**, `.ARM.exidx` **0**,
`.rodata` **0**, `.data`/`.bss` **0**, `.note.gnu.build-id` **20**.
(The task baseline quoted `.text` 75,146; the 791-byte gap is the
relocation/literal-pool masking convention used for that number — the direct
section compare is 75,937.)

## The residual, exactly

Everything from the function start through `0xd5454` is byte-identical.  The
whole far block (`0xd5474..0xd5506`) is byte-identical.  The only difference is
the near `MatrixSetRotation` tail (`0xd5456..0xd5470`, 30 bytes):

```
original                              landed 11/5
d5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; yaw   f8cd 80b8  str.w r8,[sp,#184]  ; race spill
d545a: 2002       movs  r0,#2              ; order f850 4028  ldr.w r4,[r0,r8,lsl#2]
d545c: f8cd 80b8  str.w r8,[sp,#184]       ; race  2002       movs  r0,#2
d5460: 2100       movs  r1,#0                     2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256                 f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]                 e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320                 a950       add   r1,sp,#320
d546c: 4640       mov   r0,r8                      4623       mov   r3,r4
d546e: 4614       mov   r4,r2              ; pitch  4640       mov   r0,r8
d5470: f78d edf6  blx   MatrixSetRotation          f78d edf6  blx
```

Both tails are 30 bytes / 10 instructions; the 11 raw bytes are a pure
schedule/register shift, not a size change.  The literal pool at
`0xd56b0..0xd56bc` is byte-identical in the landed form
(`f08b1200 168c1200 388c1200` = pitch `0x1fe030`, yaw-iPad `0x1fe058`,
yaw `0x1fe080`).

Two coupled differences:

1. **Which value is live across the near call.**  The original keeps
   `pitchBits` in `r2` and copies it to `r4` (`mov r4,r2`) at the call; the
   yaw load goes straight into the call's arg register `r3`.  The landed form
   instead keeps `yawBits` live across the call (loaded into callee-saved
   `r4`, then `mov r3,r4` for arg2) — because the landed source passes
   `yawBits` as the *far* call's arg1.
2. **Store placement.**  The original emits the `race` spill *after* the yaw
   load; the landed form emits it *before*.

## Semantic bug in the landed form (unchanged from w13786)

Ghidra (`scratch/w13779/decomp_ctor.txt`) shows both calls take the same
`uVar19` (= pitch-table value) as arg1:

```
near: MatrixSetRotation(&nearKey, pitch /*uVar19*/, yaw[race], 0, 2)
far : MatrixSetRotation(&farKey,  pitch /*uVar19*/, yaw[race], 0, 2)
```

The landed 11/5 source swaps the float local names so the far call's arg1 is
`yawBits`:

```cpp
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits = *(const volatile int *) &yawTable[race];
float yaw = *(const float *) &pitchBits;   // swapped names
float pitch = *(const float *) &yawBits;
MatrixSetRotation(nearKey, yaw, pitch, 0.0f, (RotationOrder) 2);
...
MatrixSetRotation(farKey, pitch, (iPad ? gHangarCamYawIPad : gHangarCamYaw)[race],
                  0.0f, (RotationOrder) 2);
```

The best *faithful* form remains **BASE35** (raw 35 / aligned 10, size 1008),
whose near tail has the correct instruction set (`ldr.w r3` + `mov r4,r2`) but
the post-RA scheduler hoists `str.w r8` and `mov r4,r2` above the load.

## Why clang reorders it (MIR evidence, fresh `-print-after-all` dump)

Pass order observed: `... -> Greedy Register Allocator -> Virtual Register
Rewriter -> Stack Slot Coloring -> ... -> Thumb2 instruction size reduce ->
Post RA top-down list latency scheduler -> ...`

* **Pre-RA Machine Instruction Scheduler** (`bb.15`): `pitch load -> yaw load
  -> $r2 = COPY pitch -> bl`.  No spill yet (spills are post-RA).
* **Greedy RA** (`bb.15`), landed source:

  ```
  %253 = t2LDRs pitchTable, race, 2      ; pitchBits
  ...
  t2STRi12 %247(race), %stack.9, 0       ; race spill  <-- already before the load
  %122 = t2LDRs yawTable, %247, 2        ; yawBits
  %252 = COPY %253                       ; pitch copy
  ```

  The spiller inserts the `race` store immediately before the instruction that
  last uses `race` (the yaw load).  This is the same spiller decision in every
  source shape tried, including BASE35.
* **Thumb2 size reduce**: order unchanged (`str.w r8` -> `ldr.w` -> `mov`).
* **Post RA top-down list latency scheduler**: for BASE35 it additionally
  hoists `mov r4,r2` above the yaw load, giving `str, mov, ldr, ...`; the
  original is `ldr, movs #2, str, ..., mov r4,r2`.

So the residual is the spiller's store placement (before vs after the last
use) plus the post-RA scheduler's live-out copy hoist.  Both are deterministic
given the MIR; ~250 prior source permutations and ~2,000 per-TU flag cells did
not change them, and this session's new shapes did not either.

## New variants measured this session

Fast per-TU triage: `scratch/w13809/cc.sh <out.o>` (single TU, ~0.45 s) +
`scratch/w13809/fastcmp.py <obj.o>` (masks R_ARM_* reloc sites and PC-relative
literal-pool words).  Calibration checked repeatedly: fastcmp `N` equals the
linked `raw` count exactly, except ±2 when a PC-relative literal word moves
(e.g. BASE35 fastcmp 33 vs linked 35; both-vol 41 vs linked 43).  Every
candidate below is size 1008 and 374 insns.

| variant (faithful = far arg1 is `pitchBits`) | fastcmp diff |
|---|---:|
| landed HEAD (both volatile, names swapped, far arg1 = `yawBits`) | **11** |
| landed + `__atomic_signal_fence` before the near call | **11** |
| landed, far arg1 = inline `*(const float *) &yawBits` | **11** |
| landed, far arg1 = `yaw` with unswapped names | **11** |
| faithful, `int raceIdx = race;` (non-vol) after the yaw load, far yaw index `raceIdx` | 41 |
| faithful, `volatile int raceIdx = race;` after the yaw load | 505 |
| faithful, `int raceIdx = race;` before the loads | 41 |
| faithful, `float yaw` declared before `float pitch` | 41 |
| faithful, `const float` locals | 41 |
| faithful, `const RotationOrder order = ...` after the loads | 41 |
| faithful, far arg1 = inline `*(const float *) &pitchBits` | 41 |
| faithful, `int yawBits` declared before `int pitchBits` | 643 |
| faithful, `float pitch = pitchTable[race];` (plain float load) | 574 |
| faithful, `int pb = pitchBits; float pitch = *(const float *) &pb;` | 41 |
| faithful, `intptr_t` index | 41 |
| faithful, single `float pitch = ..., yaw = ...;` declaration | 41 |
| faithful, separate `int pitchBits; ... pitchBits = ...;` statements | 41 |
| faithful, `__builtin_expect(Globals::iPad, 0)` in the yaw ternary | 41 |
| faithful, `__atomic_signal_fence` after both loads | 39 |
| faithful, `__atomic_signal_fence` before both loads | 39 |
| faithful, `__atomic_signal_fence` between the two loads | 42 |
| faithful, far yaw index uses the local `yawTable` pointer | 503 |

Prior-session rows (same scratch, same calibration; all size 1008):

| variant | fastcmp | linked raw/aligned |
|---|---:|---:|
| landed HEAD | 11 | **11 / 5** |
| BASE35 (pitch non-vol, yaw vol, `assume(pitch)`, fence) | 33 | **35 / 10** |
| both volatile, unswapped, far = `pitch` | 41 | 43 / 12 |
| both volatile, unswapped, far = `yaw` | 11 | same bytes as landed |
| vol pitch / non-vol yaw (any assume/fence) | 41 | — |
| vol/vol + fence (or assume+fence) | 39 | — |
| non-vol/non-vol (no volatile at all) | 548–589 | — |
| yawTable declared before pitchTable / loads before table decls | 583–589 | — |
| `!iPad`, `iPad != 0`, if/else forms of the yaw-table select | 41 | — |
| direct `(iPad ? yawIPad : yaw)[race]` in both places | 585 | — |
| explicit `pitchFar` copies (before/after near), `const float`, `volatile float`, inline bitcasts, order local, near tmp `z` | 41 (volatile ones 412–584) | — |

## Conclusion

The landed **11 / 5** remains the raw/aligned floor; no faithful form beats
**35 / 10** (BASE35).  The whole residual is the near `MatrixSetRotation` tail:
the clang-7 spiller puts the `race` spill before the yaw load and the post-RA
top-down list latency scheduler hoists the live-out pitch copy, whereas the
original has `ldr.w r3` first, `str.w r8` second and `mov r4,r2` last.  No
source-only shape found (this session or the ~250 prior) flips either
decision without changing the instruction set/size.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13809
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374 (both symbols)
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```

Fast triage helpers (scratch): `cc.sh <out.o>`, `fastcmp.py <obj.o>`,
`fsweep.py <variants.json>` (splices a block between the pitch-table
declaration and `this->cameraTween`), `ModStation.cpp.landed` (pristine
landed source), variant sets `v1..v7.json`.
