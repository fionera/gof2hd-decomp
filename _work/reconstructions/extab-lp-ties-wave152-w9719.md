# Extab LP-identity ties (wave 152, w9719): Radar tie arithmetic and a
# HangarWindow `lay` landing (raw .text -26, extab unchanged)

Status: **landed one source change** (`HangarWindow::render` M1, raw win,
aligned trade-off). Worktree `wt/w9819`, base `9d491d46a` (wave 151). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

| metric | before | after |
|---|---:|---:|
| `.text` differing bytes | 87,271 | **87,245** |
| `.ARM.extab` differing bytes | 1,327 | 1,327 |
| `HangarWindow::render` raw same-address diff | 1965 | **1939** |
| `HangarWindow::render` align_diff non-matching insns | 291 | 418 |
| `HangarWindow::render` extab record | 676 | 676 |
| `Radar::Radar` raw / aligned / extab | 0 / 0 / 7 | 0 / 0 / 7 |

## 1. Radar::Radar — the tie is `S16 < S40` at integer precision

The two cleanup pads are IR blocks `%bb.60` (16 invoke preds, label 175) and
`%bb.87` (40 invoke preds, label 372).  The MBP `selectBestCandidateBlock`
frequency is `MBFI->getBlockFreq()`, whose integer value is
`8 * BBfloat / min` where `min` is the `new Array<KIPlayer*>` landing pad
(`%bb.195`).  Since both pads share the same `min`, the comparison is simply

```
scaled16 < scaled40   <=>   S16 < S40
S16 = sum(freq of the 16 early invokes)          = 12.1248  (constant)
S40 = freq(getEnemyGuns) + 39 * T                = 12.1302
T   = tail frequency (the Image2DCreate block)   = 0.29889
```

but the `BlockFrequency` integers are truncated, and today both truncate to
**519** (519.2697 vs 519.5021).  Solving `floor(519.2697 k) < floor(519.5021 k)`
for the common scale `k = 8/min` gives two windows:

* `min` **increase** by 0.052 % .. 0.096 %  (T increase 0.048 % .. 0.089 %), or
* `min` **decrease** by 0.096 % .. 0.140 %.

`min = P(autoTurret != nullptr) * T * EH`; `expect(autoTurret != nullptr, 1)`
sets `P = 2000/2001`, which moves the pair to 324.705 / 324.851 (still both
324) and needs a **T decrease of 0.046 % .. 0.091 %** instead.

### New sweep this wave (all measured; body diff vs the byte-exact base)

`__builtin_expect` on every remaining branch, both directions, plus the
`turret` two-condition guard (the mission's "two-condition guard" hypothesis):

| variant | body diff | 16-pred | 40-pred | T |
|---|---:|---:|---:|---:|
| base | 0 | 519.27 | 519.50 | 0.29889 |
| `expect(turret != nullptr, 1)` | **0** | 519 | 519 | 0.29889 |
| `expect(getAttribute(50) != 0, 1)` | **0** | 519 | 519 | 0.29889 |
| `expect(inAlienOrbit(), 1)` (landmark `if`) | **0** | 567 | 519 | 0.2737 |
| `expect(dlc1Won(), 1)` | **0** | 546 | 519 | 0.2842 |
| `expect(inEmptyOrbit(), 1)` | **0** | 535 | 519 | 0.2901 |
| `expect(isRocketGun(), 1)` (player loop) | 12 | 525 | 519 | 0.2956 |
| `expect(isRocketGun(), 1)` (enemy loop) | 12 | 555 | 519 | 0.2796 |
| `expect(isBombGun(), 1)` / `isMineGun(), 1` | 12 | 528 | 519 | 0.2939 |
| `expect(isRocketGun(), 1)` (both loops) | 12 | 562 | 519 | 0.2762 |
| `expect(autoTurret != nullptr, 1)` | 6 | 324.7 | 324.9 | 0.29889 |
| `expect(isRocketGun(), 0)` (player loop) | 116 | 511 | 519 | 0.3037 |
| `expect(isMineGun(), 0)` (player loop) | 122 | 499 | 519 | 0.3110 |
| `expect(isBombGun(), 0)` | 1647 | 499 | 519 | 0.3110 |
| `expect(isRocketGun(), 0)` (enemy loop) | 1526 | 517 | 473 | — |
| `expect(i < playerGuns->size(), 1)` | 43 | 652 | 519 | 0.2380 |
| `expect(i < enemyGuns->size(), 1)` | 43 | 858 | 532 | 0.1809 |
| `expect(i < playerGuns->size(), 0)` | 1504 | 245 | 519 | 0.6335 |
| `expect(i < enemyGuns->size(), 0)` | 148 | 327 | 194 | — |

**Every body-exact `expect` spelling decreases T** (hotter loop/chain → lower
loop exit mass → lower T); the only spellings that increase T are the
`expect(..., 0)` forms on the loop-body guards, and every one of those inverts
the `cbz` and moves the guarded block out of line (116 B for the player
`isRocketGun`, 122 B for `isMineGun`, 1526 B for the enemy `isRocketGun`).
`turret`-guard expectations are body-exact but sit *after* T and change only
the tail block frequencies (202/201), not `min`, so they leave the pads at
519/519.

**Conclusion:** the tie needs a *mild* branch weight (≈0.45–0.49 on a loop-body
guard) that clang 7 cannot express from source; `__builtin_expect` is 2000:1
and overshoots by 30x.  This is a PGO-weight residual, not a source-shape one.
`expect(isRocketGun(), 0)` remains the only proven flip (extab 7 -> 3) at a
116 B body cost.

## 2. HangarWindow::render — M1 landed (raw win, aligned trade-off)

`y` now reads `Globals::layout->field_0x20_top` directly instead of the
invented `Layout *lay` local (`lay` is still used for `field_0x2c_rowHeight`
on the next line).  The non-loop twin at `src/game/ui/HangarWindow.cpp:1000`
already spells the same expression directly, so this is the faithful form.

| variant | raw | align | extab | size | rows |
|---|---:|---:|---:|---:|---:|
| base | 1965 | 291 | 676 | 9700 | 156 (43 diff) |
| **M1 (landed)** | **1939** | 418 | 676 | 9700 | 156 (42 diff) |

M1 does **not** change the extab record (676 B) — it fixes only the row-125
length (144 -> 146) and shifts rows 126-135 by 2 B.  The aligned regression is
the register allocator spilling `description` differently; the raw diff is the
better metric here because the whole credit loop is a uniform 2-6 B shift.

### Row table with M1 (42 differing rows; row 135 now matches)

```
row  orig                     ours (M1)               note
  1  (684, 20, 9544)          (684, 20, 9534)         LP offset only
 39  (2508, 150, 9664)        (2508, 118, 9664)       list-render boundary (getShip)
 40  (2696, 140, 9562)        (2654, 182, 9514)       getShip call-site row
 60  (3806, 20, 9514)         (3806, 20, 9516)        LP offset only
 96  (5918, 198, 0)           (5918, 196, 0)          credit-loop start
 97  (6116, 8, 9216)          (6114, 8, 9216)
 98  (6124, 128, 0)           (6122, 132, 0)
 99  (6252, 10, 9652)         (6254, 10, 9652)
101  (6306, 8, 9586)          (6310, 8, 9586)
102  (6314, 8, 9574)          (6318, 8, 9574)
103  (6336, 8, 9560)          (6340, 8, 9562)
104  (6344, 8, 9542)          (6348, 8, 9550)
105  (6354, 10, 9584)         (6358, 10, 9584)
106  (6364, 8, 9572)          (6368, 8, 9572)
107  (6378, 8, 9558)          (6382, 8, 9560)
108  (6386, 8, 9540)          (6390, 8, 9548)
109  (6404, 8, 9582)          (6408, 8, 9582)
110  (6412, 8, 9570)          (6416, 8, 9570)
111  (6434, 8, 9556)          (6438, 8, 9558)
112  (6442, 8, 9538)          (6446, 8, 9546)
113  (6460, 8, 9580)          (6464, 8, 9580)
114  (6468, 8, 9568)          (6472, 8, 9568)
115  (6490, 8, 9554)          (6494, 8, 9556)
116  (6498, 8, 9536)          (6502, 8, 9544)
117  (6516, 8, 9578)          (6520, 8, 9578)
118  (6524, 8, 9566)          (6528, 8, 9566)
119  (6546, 8, 9552)          (6550, 8, 9554)
120  (6554, 8, 9534)          (6558, 8, 9542)
121  (6632, 8, 9576)          (6636, 8, 9576)
122  (6640, 8, 9564)          (6644, 8, 9564)
123  (6662, 8, 9550)          (6666, 8, 9552)
124  (6670, 8, 9532)          (6674, 8, 9540)
125  (6678, 146, 0)           (6682, 146, 0)          length now matches
126  (6824, 210, 9654)        (6828, 202, 9654)       merged, 8 B shorter
127  (7034, 390, 0)           (7030, 390, 0)
128  (7424, 8, 9644)          (7420, 8, 9644)
129  (7432, 10, 9634)         (7430, 10, 9634)
130  (7446, 10, 9624)         (7444, 8, 9624)
131  (7456, 12, 9614)         (7452, 12, 9614)
132  (7474, 20, 9604)         (7472, 20, 9604)
133  (7494, 52, 0)            (7492, 52, 0)
134  (7546, 20, 9602)         (7544, 22, 9602)
```

Rows 100, 135-155 are identical.  Row 39/40 remains the structural boundary:
the original's `getShip` (fn offset 2654) unwinds to the *outer* cleanup
(LP 9664), ours to the padX cleanup (LP 9514), i.e. the original's
lifetime-tracked local starts after `getShip`.

### padX placement re-check (new: three sinks)

Declaring `padX` as the first statement of the inner `if` body gives row-39
length **146** (original 150) but shrinks the function to **9676 B**; adding
`padY` and `padZ` sinks does not change the size (the extra pointers are dead
and eliminated).  So the 24 B deficit is not a missing lifetime marker count.

The original's `if (tab != 0)` block is laid out inline (`cbz r0, 149b96` at
fn offset 0x149b24, target = the `upgradeMode` block immediately after),
whereas ours branches far away (`beq.w 14a6d0`).  That block-placement
difference is what really moves `getShip` into row 40; it is a PGO-layout
residual, not a lifetime-marker one.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9819
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w9819/fdiff.py _ZN12HangarWindow6renderEv   # 1939, 9700
python3 /dev/shm/w9819/align_diff.py _ZN12HangarWindow6renderEv  # 418
python3 /dev/shm/w9819/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                            # 1327 total, 676 @ 0x1490d8
```

Scratch (not committed), `/dev/shm/w9819/`: `fdiff.py`, `align_diff.py`,
`extab_lsda_decode.py`, `mkir.sh`, `buildobj.sh`, `fnbytes.py`, `score.sh`,
`scorehw.sh`, `batch*.py`, `Radar.base.cpp`, `HW.base.cpp`.

## 4. Open questions

1. A source shape that yields a mild (0.45–0.49) weight on a loop-body guard.
   clang 7's `handleBrSelExpect` is 2000:1 only; `__builtin_expect_with_probability`
   does not exist in 7.0.2.
2. The original PGO profile would give `T` directly.
3. HangarWindow rows 39/40: reproduce the original's inline `upgradeMode` block
   (`cbz` to the immediately-following block) — a block-placement residual.
