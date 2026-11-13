# ModStation constructor — wave 191b (w13786): faithful-form re-probe (checkpoint 12)

Worker `w13786`, branch `wave191b-w13786`, base master `c32384adf`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13786`.

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no faithful shape below raw 35 /
aligned 10 found.**  Tree clean; `check` green at the entry counts.

Predecessor notes (read first): `modstation-ctor-wave191b-w13779.md`,
`-w13738`, `-w13710`, `-w13693`, `-w13653`, `-w13641`, `-w13616`,
`modstation-extab-lp-wave149-w9520.md`, `modstation-ctor-pass4-w4220.md`.

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
`RATCHET PASS`, byte_exact **4508/4524**, linked_exact **4508/4524**,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint **CLEAN ✓**, drift **0**, parity 5370/5370.

Section content deltas, landed 11/5 form (all section *sizes* exact,
`.rel.plt` byte-identical): `.text` **74,651**, `.ARM.extab` **562**,
`.note.gnu.build-id` **20**; `.ARM.exidx`, `.rodata`, `.data`, `.bss` all **0**.

## The semantic bug (confirmed against Ghidra)

The landed 11/5 form swaps the two float locals:

```cpp
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
float yaw   = *(const float *) &pitchBits;   // NOTE: swapped names
float pitch = *(const float *) &yawBits;
MatrixSetRotation(nearKey, yaw, pitch, 0.0f, (RotationOrder) 2);
...
MatrixSetRotation(farKey, pitch, (iPad ? gHangarCamYawIPad : gHangarCamYaw)[race],
                  0.0f, (RotationOrder) 2);
```

The near call is correct (it passes `pitchBits` bits as arg1, `yawBits` as
arg2), but the far call passes `yawBits` as arg1.  Ghidra
(`scratch/w13779/decomp_ctor.txt`) shows both calls take the same `pitch`
value as arg1:

```
near: MatrixSetRotation(&local_b8, &local_78, uVar19 /*pitch*/, *(iVar6+iVar5*4) /*yaw*/, 0, 2)
far : MatrixSetRotation(aAStack_f4, &local_b8, uVar19 /*pitch*/, *(iVar6+iVar5*4) /*yaw*/, 0, 2)
```

So the landed form is a byte-cheap mirror with wrong far-key pitch semantics.

## The residual is exactly the near `MatrixSetRotation` tail

Original `0xd5456..0xd5470` (near call tail):

```
d5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; yawBits, direct into r3
d545a: 2002       movs  r0,#2
d545c: f8cd 80b8  str.w r8,[sp,#184]       ; race spill AFTER movs
d5460: 2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320
d546c: 4640       mov   r0,r8
d546e: 4614       mov   r4,r2               ; save pitchBits for far
d5470: f78d edf6  blx   MatrixSetRotation
```

Landed 11/5 form (far=yaw, both volatile, no assume/fence):

```
d5456: f8cd 80b8  str.w r8,[sp,#184]       ; spill BEFORE the load
d545a: f850 4028  ldr.w r4,[r0,r8,lsl#2]   ; yawBits -> r4
d545e: 2002       movs  r0,#2
d5460: 2100       movs  r1,#0
d5462: f50d 7880  add.w r8,sp,#256
d5466: e9cd 1000  strd  r1,r0,[sp]
d546a: a950       add   r1,sp,#320
d546c: 4623       mov   r3,r4               ; copy yawBits into arg2
d546e: 4640       mov   r0,r8
d5470: f78d edf6  blx   MatrixSetRotation
```

The whole far block (`0xd5474..0xd5506`) is byte-identical between the
original and both the 11/5 and the faithful 35/10 forms; the difference is
purely the near tail.

## Best faithful form: raw 35 / aligned 10 (BASE35)

`__builtin_assume(pitchBits != 0x7f800001)` + `__atomic_signal_fence`
(see the exact block at the end).  Its near tail:

```
d5456: f8cd 80b8  str.w r8,[sp,#184]
d545a: 4614       mov   r4,r2               ; pitch copy HOISTED
d545c: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; yaw load AFTER the copy
d5460: 2002       movs  r0,#2
d5462: 2100       movs  r1,#0
d5464: f50d 7880  add.w r8,sp,#256
d5468: e9cd 1000  strd  r1,r0,[sp]
d546c: a950       add   r1,sp,#320
d546e: 4640       mov   r0,r8
d5470: f78d edf6  blx   MatrixSetRotation
```

So BASE35 has the same instruction *set* as the original, only the schedule
differs: `str.w r8` and `mov r4,r2` are hoisted above the yaw load, and the
`Yaw` LDRLIT (`add r4,pc`) sinks.  Measured section deltas for BASE35:
`.text` **74,675** (landed +24), `.ARM.extab` **562** (unchanged),
`.rodata` **0**.

### Exact BASE35 byte-diff vs the original (35 bytes)

```
0xd543a..0xd543d (4)   O: 9f4c7844              U: 78449e4c
0xd5444..0xd5449 (6)   O: 7c4450f82820          U: 50f828207c44
0xd5456..0xd5456 (1)   O: 50                    U: cd
0xd5458..0xd545c (5)   O: 28300220cd            U: b880144650
0xd545e..0xd546e (17)  O: b88000210df58078cde9001050a9404614
                       U: 2830022000210df58078cde9001050a940
0xd56b0..0xd56b0 (1)   O: f0                    U: f2   (PC-relative literal)
0xd56b8..0xd56b8 (1)   O: 38                    U: 34   (PC-relative literal)
```

The two literal-pool bytes are only PC-relative offsets shifted by the
`add r0,pc` / `add r4,pc` slot moves; the actual `.rodata` tables are
identical.  For the landed 11/5 form the literal pool at
`0xd56b0..0xd56bc` is **byte-identical** to the original
(`f08b1200 168c1200 388c1200`), i.e. pitch table `0x1fe030`, yaw-iPad
`0x1fe058`, yaw `0x1fe080`.

## Why clang reorders it (MIR evidence)

`scratch/w13786/mir35.txt` is `-print-after` for BASE35.

* **Greedy RA** (pass "After Greedy Register Allocator"): the pitch value is
  live across both calls, so RA emits `%252:gpr = COPY %253:gpr` (pitch) right
  after the yaw load; the pre-RA value `%236` is copied to `$r2` at the near
  call and again at the far call.  The copy is unavoidable — the original also
  has `mov r4,r2`.
* **Thumb-IT pass** ("After Thumb IT blocks insertion pass"): order is already
  `str.w r8` → `ldr.w r3` → `mov r4,r2` → barrier → call setup.
* **Post-RA top-down list latency scheduler** ("After Post RA top-down list
  latency scheduler"): rewrites it to `str.w r8` → `mov r4,r2` → `ldr.w r3` →
  … The scheduler's height priority (pitch copy and race spill both feed the
  *far* call, the yaw load feeds only the *near* call) beats the load-latency
  heuristic.  **This single hoist is the whole 35-byte residual.**

The original's final order (`ldr.w r3` first, `mov r4,r2` last) is the
opposite: it is latency-first for the load and copy-last for the pitch save.
Since the instruction set is identical, the original's pre-scheduler MIR must
have differed in a way that flips the DAG priority; every source shape tried
converges to the same MIR.

## Flag sweep (per-TU, `cc35.sh` on BASE35)

Both post-RA schedulers produce the identical hoist:

| flag | ctor near-tail order | result |
|---|---|---|
| `-mllvm -enable-post-misched=true` | str, mov r4,r2, ldr r3 | same 35/10 |
| `-mllvm -misched-postra=true` | str, mov r4,r2, ldr r3 | same 35/10 |
| `-mllvm -post-RA-scheduler=false` / `-disable-post-ra` | str, ldr, mov r4,r2 (original order) | but drops `add r1,sp,#320`/`mov r0,r8`; per-TU 424/99 |
| `-break-anti-dependencies`, `-misched-topdown=false`, `-misched-bottomup`, `-misched-limit=1`, `-max-sched-reorder=0/1`, `-disable-postra-machine-sink`, `-disable-postra-machine-licm`, `-misched-postra=false`, `-enable-post-misched=false`, `-disable-sched-*`, `-enable-aa-sched-mi`, `-disable-dfa-sched`, `-misched-regpressure=false`, `-misched-cluster/-fusion/-cyclicpath`, `-pre-RA-sched=list-hybrid`, `-split-spill-mode=size`, `-twoaddr-reschedule=false`, `-disable-2addr-hack`, `-callsite-splitting-duplication-threshold=0` | no change | 35/10 |
| `-optimize-regalloc=false`, `-stress-regalloc=8` | wildly different | — |
| `-pre-RA-sched=source/list-burr/list-ilp` | dramatically different | — |

## Measured source variants (all size 1008 unless noted)

The landed 11/5 form is the raw floor; the faithful family bottoms out at
35/10.  Everything below was a full `link_so` + `tools/fn_diff.py`.

### Landed semantics (far = yawBits)

| variant | raw / aligned |
|---|---:|
| landed HEAD (both volatile, no assume/fence) | **11 / 5** |
| landed + assume(pitch), + assume(yaw), + fence, + assume+fence | 11 / 5 |
| 32-cell grid (far=yaw, any volatile/assume/fence) | 11 / 5 |
| `Globals::iPad != 0` spelling | 11 / 5 |

### Faithful semantics (far = pitchBits)

| variant | raw / aligned | size |
|---|---:|---:|
| **BASE35** (pitch non-vol, yaw vol, assume(pitch NaN), fence) | **35 / 10** | 1008 |
| faithful both volatile (`v.json[0]`) | 43 / 12 | 1008 |
| faithful both volatile + assume | 42 / 12 | 1008 |
| inline int bitcasts / pitch-float local | 43 / 12 | 1008 |
| pitch-float + fence / inline + fence | 42 / 12 | 1008 |
| `ASSUME_PITCH_NOTNAN`, `ASSUME_PITCH_RANGE` | 35 / 10 | 1008 |
| `ASSUME_RACE8/10/GE0` only (no pitch assume) | 628 / 70 | 1008 |
| `FENCE_THEN_ASSUME`, `FENCE_AFTER_ORDER`, `FENCE_BETWEEN_FLOATS`, `FENCE_PF_AND_ORDER`, `FENCE_TWICE_MID_END` | 35–36 / 10 | 1008 |
| `FAR_ORDER_LOCAL`, `ORDER_INT`, `CONST_FLOATS`, `PITCH2_COPY`, `EXPL_COPY_FAR/NEAR`, `YAW_COPY`, `PITCH_BITS_COPY` | 35 / 10 | 1008 |
| `PITCH_BITS_FAR_INLINE`, `NEAR_YAW_INLINE`, `PITCH_FLOAT_FIRST`, `BOTH_MATRICES_TOP` | 35 / 10 | 1008 |
| `RACE_LT8`, `RACE_RANGE`, `INTPTR_INDEX`, `UNSIGNED_RACE`, `ASSUME_UNSIGNED` | 35 / 10 | 1008 |
| `NO_ORDER_LOCAL`, `FAR_RELOAD_PITCHBITS`, `ORDER_BEFORE_LOADS`, `NEAR_COPY_VAR`, `FAR_COPY_VAR` | 35 / 10 | 1008 |
| `BOTH_VOL_ASSUME_BOTH` | 42 / 12 | 1008 |
| `PITCH_VOL_YAW_NV` | 44 / 18 | 1008 |
| `YAW_TABLE_FIRST` | 38 / 11 | 1008 |
| `PITCH_VOL_FLOAT_LOCAL` | 678 / 179 | 1008 |
| `PITCH_VOL_FLOAT` | 213 / 45 | 1008 |
| `ASSIGN_BOTH` (`x = MatrixSetRotation(x,…)`) | 487 / 57 | 1008 |
| `FAR_TR_BEFORE_NEAR` | 704 / 126 | 1008 |
| `NEAR_BEFORE_FENCE` | 700 / 63 | **1000** |
| `FAR_YAW_LOCAL` / `FAR_YAW_TABLEVAR` | 604 / 71 | **984** |
| `FAR_TR_CAMKEY` | 960 / 126 | **920** |
| `ASSUME_YAW` | 628 / 70 | 1008 (375 insn) |
| plain `float pitch = table[race]` (natural) | 667 / 59 | 1008 (375 insn) |
| `INT_TABLE_VIEW`, `VOL_TABLE_PTR`, `INT_PITCH`, `INTBITS_NOFENCE` | 667 / 59 | 1008 (375 insn) |
| `VOL_RACE`, `VOL_FLOAT_LOCALS`, `VOL_FLOAT_PTR` | 598–773 / 121–184 | 1000–1016 |

Prior passes (w13738 / w13710 / w13641 / w13693 / w13653 / w13616 /
w13779) additionally measured ~250 source permutations and ~2,000 per-TU
flag cells; none beats raw 35 for the faithful form.

## Conclusion

The 11/5 landed form is the raw floor but **semantically wrong** (far arg1 =
`yawBits`).  The best faithful form is **35/10** (BASE35: pitch non-volatile,
yaw volatile, `__builtin_assume` on `pitchBits`, `__atomic_signal_fence`),
size 1008 exact, `.ARM.extab` unchanged, `.text` +24.  It regresses *both*
raw (11→35) and aligned (5→10), so per the fleet's "raw floor wins" convention
it is not landed; this note records the shape so a future pass can land it if
semantic correctness is preferred over the 24-byte `.text` delta.

The residual is entirely the clang-7 post-RA top-down list latency scheduler
hoisting `str.w r8` + `mov r4,r2` above the yaw load; at the Thumb-IT pass the
order is already the original one, and both post-RA schedulers
(`ScheduleDAGPostRATDList` and the new `PostMachineScheduler`) produce the
same hoist.  No source-only shape flips it without changing size.

## Exact BASE35 block (drop-in for future landing)

Replace the block in `src/game/menu/ModStation.cpp` from
`const float *pitchTable = gHangarCamPitch;` through the far
`MatrixSetRotation(...)` with:

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    __builtin_assume(pitchBits != 0x7f800001); // lint: arm_builtin
    float pitch = *(const float *) &pitchBits;
    float yaw = *(const float *) &yawBits;
    const AbyssEngine::AEMath::RotationOrder order = (AbyssEngine::AEMath::RotationOrder) 2;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f, order);

    Matrix farKey;
    MatrixSetTranslation(farKey, (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3],
                         (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3 + 1],
                         (float) (Globals::iPad ? gHangarCamPosIPad : gHangarCamPos)[race * 3 + 2]);
    MatrixSetRotation(farKey, pitch, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
```

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13786
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# landed: raw=11 aligned=5 size=1008/1008 insn=374/374
# BASE35: raw=35 aligned=10 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```

Scratch helpers: `sweep.py <variants.json>`, `setblock.py <variants.json> <idx>`,
`cc35.sh <out.o> [flags]`, `bytediff.py`, `secdiff.py`, `difftail.py`,
`ModStation.cpp.base`, `mir35.txt`, variant sets `v*.json` / `vw1..vw18.json`.
