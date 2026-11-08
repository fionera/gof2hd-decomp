# Extab LP-identity ties (wave 153, w9919): Radar tie sweeps exhausted;
# HangarWindow row 39/40 needs `padX` inside the inner `if` (size 9676)

Status: **analysis only, no landed `src/` change**. Worktree `wt/w9919`, base
`4a6104a43` (wave 152). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

| metric | base |
|---|---:|
| `.text` differing bytes | 87,131 |
| `.ARM.extab` differing bytes | 1,324 |
| `Radar::Radar` raw / aligned / extab | 0 / 0 / 7 |
| `HangarWindow::render` raw / aligned / extab | 1965 / 291 / 676 |

## 0. Re-derived mechanism (MBP trace, instrumented llc)

`MBP_TRACE=1 /tmp/llvm_android_702b/build2/bin/llc -mtriple=armv7-none-linux-androideabi16
-mattr=+neon -float-abi=soft -Oz -debug-only=block-freq Radar.ll -o /dev/null`

The final `EHPadWorkList` is
`%bb.61 %bb.63 ... %bb.75 %bb.60 %bb.76 %bb.77 %bb.78 %bb.88 %bb.87 ... %bb.201 %bb.194 %bb.195 %bb.196`.
`%bb.60` (16-pred String pad) and `%bb.87` (40-pred getEnemyGuns+Image2D pad) both
report `freq=519`. `selectBestCandidateBlock` for EHPads keeps the **last**
minimal entry (`IsEHPad ^ (BestFreq >= CandidateFreq)` is false on a tie), so
`%bb.87` is placed first at the lower address; the original places `%bb.60`
first. Confirmed by the LSDA: rows 1/3/11/13/15 (orig LP 2462, ours 2464) and
rows 19/21 (orig 2464, ours 2462) — the two pads are two adjacent 2-byte
`b.n` trampolines, so the 7 diff bytes are *only* the swapped LP offsets.

Scaled frequencies: `%bb.60 = 519.2697`, `%bb.87 = 519.5021`; the common
normaliser is `min = P(autoTurret != nullptr) * T = 0.625 * 0.2988999`.
`floor(519.2697 k) < floor(519.5021 k)` holds for `k in [0.999042, 0.999479)`
(min +0.052..0.096 %, i.e. `T` +0.048..0.089 %) or for `k = 2,4,5` (P halved,
P/4, P/5). Every body-exact spelling that moves `T` moves it *down*; every
spelling that halves `P` adds a condition and moves `.text`.

## 1. Radar::Radar — new sweep (all measured, body raw / aligned / extab)

| variant | raw | aligned | extab |
|---|---:|---:|---:|
| base | **0** | **0** | **7** |
| `goto` skip: `if (autoTurret == nullptr) goto done;` | 109 | 34 | 29 |
| `goto` skip after body | 109 | 34 | 29 |
| `goto` in `else` arm | 109 | 34 | 29 |
| `if (...) { } else { }` (empty else) | 109 | 34 | 29 |
| `while (autoTurret != nullptr) { ...; break; }` | 109 | 34 | 29 |
| `autoTurret != nullptr && this->field_0x1b0 == 1` (P=0.3125, k=2) | 175 | 37 | 43 |
| `autoTurret != nullptr && this->players == nullptr` (P=0.39, k=1.6) | 164 | 35 | 42 |
| `autoTurret != nullptr && this->enabled` (P=0.39) | 173 | 36 | 42 |
| `autoTurret != nullptr && this->field_0x1b0` (P=0.39) | 173 | 36 | 42 |
| `autoTurret != nullptr && autoTurret->getAttribute(55) >= 0` | 183 | 45 | 46 |
| nested `if (autoTurret) { if (players == nullptr) ... }` | 164 | 35 | 42 |
| `__builtin_expect(autoTurret != nullptr, 0)` | 69 | 28 | 24 |
| `__builtin_expect(this->field_0x1b0 == 1, 1) && ...` | 174 | 38 | 43 |
| `__builtin_expect(this->players == nullptr, 1) && ...` | 141 | 33 | 31 |
| `ArraySetLength(..., *players)` (local instead of member) | 7 | 6 | 7 |
| drop the dead `players` local | 109 | 34 | 29 |
| extra `{ }` scope around `new Array` | 109 | 34 | 29 |

The `P = 0.3125` / `P = 0.39` two-condition guards **do** break the tie (the
scaled pads separate), but the extra compare changes the machine layout and the
LSDA rows shift, so the extab diff rises to 42-46. The `goto`/empty-else/while
spellings all shrink the function by 4 B (2488 -> 2484) because the empty
successor block is folded, again shifting rows.

**Conclusion unchanged:** the 7 bytes need a ~0.05 % tail-weight nudge that
clang 7 cannot express from source (`handleBrSelExpect` is 2000:1 only;
`__builtin_expect_with_probability` does not exist in 7.0.2). This is a PGO
weight residual.

## 2. HangarWindow::render — row 39/40

Original rows 39/40: `(2508, 150, 9664)` and `(2696, 140, 9562)`. Ours:
`(2508, 118, 9664)` and `(2654, 182, 9514)`. `getShip` is at fn offset 2654 in
both, so in the original its block unwinds to the **outer** cleanup (9664) and
in ours to the padX cleanup (9514). The optimised IR shows why: `padX` is
declared before the `switch i32 %93` (the `if (tab != 0)` / `tab == 3` chain),
so `getShip` (`%1724`) unwinds to `%1765` (the padX landing pad).

New sweep (raw / aligned / extab / size):

| variant | raw | aligned | extab | size |
|---|---:|---:|---:|---:|
| base (`padX` before the switch) | **1965** | **291** | **676** | **9700** |
| `padX` first stmt of the inner `if` body | 8610 | 799 | 760 | 9676 |
| `padX` after the whole `if (tab != 0)` block | 6606 | 838 | 706 | 9708 |
| `padX` in its own `{ }` before the switch | 6731 | 734 | 718 | 9700 |
| drop `padX` entirely | 6731 | 734 | 718 | 9700 |
| `padX` before switch + dead `padY` inside inner | 8610 | 799 | 760 | 9676 |
| `padX` before switch + `padX = &tab` inside inner (dead) | 1965 | 291 | 676 | 9700 |
| `padX` before switch + `padX = &tab` after switch (dead) | 1965 | 291 | 676 | 9700 |
| `volatile int padX` instead of pointer, before switch | 7317 | 728 | 733 | 9708 |
| `int *padX` (non-volatile pointee), before switch | 1965 | 291 | 676 | 9700 |
| `bool same = ...; if (!same)` with `padX` before switch | 6693 | 601 | 715 | 9692 |
| `bool same` + `padX` inside inner | 8624 | 759 | 771 | 9676 |
| split `if (tab == 3) ... else if (tab != 0)` | 7455 | 1089 | 638 | 9772 |
| M1 (`y` reads `Globals::layout`), padX before switch | 1939 | 418 | 676 | 9700 |
| M1 + `padX` inside inner | 8610 | 740 | 760 | 9676 |

Only moving `padX` *inside* the inner `if` body moves the row-39 boundary the
right way: row 39 length becomes **146** (original 150) instead of 118. But the
function shrinks to **9676 B** (24 B short), the prologue grows by 2 B
(row 0 `(0,686)` vs `(0,684)`), and every later row shifts, so the extab byte
diff rises from 676 to 760. The row-39/40 boundary and the 9700-byte register
allocation are coupled through `padX`'s `llvm.lifetime.start`. The 24-byte
deficit matches the original's per-access `&this->buttons` reloads
(`ldr r0,[sp,#88]; ldr r0,[r0]; ldr r0,[r0,#4]` 8 B vs ours `ldr r0,[r6]; ...`
6 B), i.e. the original keeps `this->buttons` spilled and reloads it; that is
the same residual as rows 96-135.

### Row table (base, 42 differing rows)

```
row  orig                     ours                     note
  1  (684, 20, 9544)          (684, 20, 9534)          LP offset only
 39  (2508, 150, 9664)        (2508, 118, 9664)        list-render boundary (getShip)
 40  (2696, 140, 9562)        (2654, 182, 9514)        getShip call-site row
 60  (3806, 20, 9514)         (3806, 20, 9516)         LP offset only
 96  (5918, 198, 0)           (5918, 196, 0)           credit-loop start
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
125  (6678, 146, 0)           (6682, 144, 0)
126  (6824, 210, 9654)        (6826, 202, 9654)        merged, 8 B shorter
127  (7034, 390, 0)           (7028, 390, 0)
128  (7424, 8, 9644)          (7418, 8, 9644)
129  (7432, 10, 9634)         (7428, 10, 9634)
130  (7446, 10, 9624)         (7442, 8, 9624)
131  (7456, 12, 9614)         (7450, 12, 9614)
132  (7474, 20, 9604)         (7470, 20, 9604)
133  (7494, 52, 0)            (7490, 52, 0)
134  (7546, 20, 9602)         (7542, 22, 9602)
135  (7566, 156, 0)           (7564, 158, 0)
```

### Row table with `padX` inside the inner if body (row 39 boundary nearly right)

```
  0  orig (0, 684, 0)         ours (0, 686, 0)         prologue +2
 39  orig (2508, 150, 9664)   ours (2508, 146, 9664)   length 146 vs 150
 40  orig (2696, 140, 9562)   ours (2678, ...)         shifted
```
All 156 rows differ on LP offset / start because the function is 24 B short.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9919
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w9919/score.py _ZN5RadarC2EP5Level          # raw 0, extab 7
python3 /dev/shm/w9919/score.py _ZN12HangarWindow6renderEv   # raw 1965, extab 676
python3 /dev/shm/w9919/rows.py 0x1490d8                      # row table
python3 /dev/shm/w9919/radar_sweep.py                        # Radar variant sweep
python3 /dev/shm/w9919/hw_sweep.py                           # HangarWindow variant sweep
```

Scratch (not committed), `/dev/shm/w9919/`: `score.py`, `score.sh`, `rows.py`,
`fdiff.py`, `align_diff.py`, `mkir.sh`, `radar_sweep.py`, `hw_sweep.py`,
`hw_sweep2.py`, `Radar.base.cpp`, `HangarWindow.base.cpp`,
`HangarWindow.M1.cpp`, `HangarWindow.M1inside.cpp`, `Radar.ll`, `HW.ll`,
`mbp_base.txt`, `extab_base.txt`.

## 4. Open questions

1. Radar: a source shape that raises `T` by 0.05 % while keeping `.text`
   byte-exact. All clang-7 source weights are coarse (20/32, 1/2, 124/128,
   2000:1); only PGO can produce the needed value.
2. HangarWindow rows 39/40: a shape whose `padX` lifetime starts after
   `getShip` **and** whose register allocation stays at 9700 B. The 24-byte
   gap is the original's per-access `&this->buttons` reload; reproducing that
   reload (a spilled `Array<TouchButton*> *` local) may recover the size and
   let `padX` move inside.
