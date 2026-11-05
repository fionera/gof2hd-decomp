# Level::createShip — wave 119 (w6833): jitter call order XZY reaches 70.3 % at the exact 336 B LSDA

**Status:** source change landed (2026-11-05). Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns). Branch
`wave119-w6833`, base `fdb68f8ee`.

## Result

```
                                   before (HEAD)        after (landed)
createShip fuzzy (official)        70.200000            70.300000
createShip size                    2764 B               2764 B
createShip insns (orig/ours)       1007 / 1006          1007 / 1005
createShip LSDA record             336 B (exact)        336 B (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
.ARM.exidx                         0x19cc08 / 0x6f70    0x19cc08 / 0x6f70
.ARM.extab                         0x26290              0x26290
.rodata                            0x41c27              0x41c27
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
```

`check` (final): **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0
missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: imports
207`, lint CLEAN, drift 0.  No section size/base/import/data/parity
movement; the `.ARM.extab` stays 8 B short (createShip's own 336 B record
is exact; the delta is `Generator::createMission` -4 and `0x1490d8` -4,
both other TUs).

## Landed source change (ordinary C++)

```diff
     int jitterX = Globals::rnd->nextInt(40000);
-    int jitterY = Globals::rnd->nextInt(40000);
     int jitterZ = Globals::rnd->nextInt(40000);
+    int jitterY = Globals::rnd->nextInt(40000);
```

The three `nextInt(40000)` calls are independent and the jitter values are
only added to their own coordinate, so permuting the middle two is
semantically a re-labeling of two i.i.d. draws.  The XZY call order makes
the two adjacent spill slots hold `jitterX`/`jitterY`, so the backend
fuses the two reloads into one `ldrd` (1006 -> 1005 insns) and the
difflib alignment against the original improves by one row.

## Real-`new` register-allocation search (the actual target)

The wave-119 base (real `new` for Player/Fighter/Fixed/Array/24 AABs,
336-byte cleanup-only LSDA) is a strong local optimum.  ~900 variants were
built and scored with the single-function official scorer (a mini one-TU
`.so` reproduces the linked function byte-for-byte, so scores are exact):

* every single-block move / adjacent swap of the 13 preamble statements
  (200 random valid topological orders, 42 single moves): best tie 70.2,
  most 58-66;
* every `wp` declaration/assignment/else permutation (216) x jitter call
  order: only the jitter call order moves the score;
* 128-combination sweep of {one-call/two-call level} x {one-call/two-call
  empPoints} x {nc/no-nc} x {faithful/tuned spawn} x {XYZ/XZY} x
  {wzy/zyx} x {hostile/group}: best 70.3, two-call clamps cost size;
* `cls`/`kind`/`bool isCapital`/`Status *st` copies at every anchor: all
  coalesced away (bit-identical code);
* `int`/`unsigned`/`long` retypes of every local, expression
  reassociations, hitpoint switch/if-chain orders, mission-range forms,
  `armor` if-form, `try` placements, direct-return and two-variable
  dispatch shapes: no gain;
* AAB statement-group reversals (24 AABs, 5 groups), `switch (race)`
  case/default permutations (120): the checked-in order is best;
* null-check placement on every statement boundary: only the current
  position (after `setEmpData`, before `ship`) keeps 2764 B and 70.2.

The remaining gap is the same Greedy-RA cascade documented in
`level-createship-wave119-w6785.md` / `w6799.md` / `w6813.md`: the
original keeps `shipClass` in `r5 -> sl -> r6` and `type` in `fp`, while
the real-`new` build spills `shipClass` (`stack.3`) and keeps `type` in
`sl`; the landing-pad exception pointer is `r6` (original `r8`) and the
fighter/fixed delete stubs coalesce to one register (original has four
stubs).  No source-level rewrite found here changes that first Greedy
decision.

## The higher-scoring placement-new shape (not landed: LSDA regresses)

The wave-119 `w6729` placement-new + `try`/`__builtin_trap` body still
scores **74.4 %** at 2764 B (1007/1007 insns) and reproduces the original
prologue, but its createShip LSDA record is **328 B** (54 rows, TType
0x00) instead of the original's 336 B (57 rows, TType 0xff), so
`.ARM.extab` would go 0x26290 -> 0x26288.  It was not landed because the
coordinator's structural state pins the exact 336 B LSDA.  Measured
alternatives on that body:

| variant | raw % | size | LSDA |
|---|---:|---:|---:|
| placement-new + try (w6729 body) | 74.4 | 2764 | 328 |
| + faithful spawn | 73.9 | 2764 | 328 |
| + one-call clamps (both) | 71.9 | 2764 | 328 |
| real new for both ships, no try | 64.6 | 2768 | 336 |
| real new for both ships + try | 63.7 | 2772 | 352 |
| placement-new, no try | 70.5 | 2776 | 316 |

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6833/{harness,score_mini,
blocks,runv,gen}.py` (single-TU fast scorer + variant sweeps).
