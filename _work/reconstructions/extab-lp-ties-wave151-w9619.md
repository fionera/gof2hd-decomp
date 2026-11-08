# Extab LP-identity ties (wave 151, w9619): the Radar tie is a tail-frequency
# lever; `expect(isRocketGun(),0)` flips it (extab 3) but costs 116 body bytes

Status: **analysis only, no landed `src/` change**. Worktree `wt/w9719`, base
`db9658a51` (wave 150). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.ARM.extab` 156,312 B, total differing bytes
**1,326** (unchanged).

Targets and unchanged metrics (raw same-address diff / instruction-index
alignment / extab bytes):

| fn | bodyraw | aligned | extab |
|---|---:|---:|---:|
| `Radar::Radar` (`_ZN5RadarC2EP5Level`, `0x12d484`) | **0** | **0** | **7** |
| `HangarWindow::render` (`0x1490d8`) | 1965 | 291 | 676 |

## 1. Radar::Radar — the tie is a *tail-frequency* lever, not a pad-frequency one

The two cleanup pads are `%175` (16 invoke preds, the function-level String
cleanup) and `%372` (40 invoke preds, the `getEnemyGuns` + `Image2DCreate`
cleanup).  In the final MBP pass the worklist is
`bb.200, bb.202, bb.201, bb.199` (`bb.200` = the 16-pred pad).  For EHPads
`selectBestCandidateBlock` keeps the *smallest* frequency and on a tie keeps the
*last* worklist entry, so `bb.199` (40-pred) is laid out first today.  The
original lays the 16-pred pad first.

The scaled frequencies are `s60/2^20 = 519.2697` (16 preds) and
`s87/2^20 = 519.5021` (40 preds).  Both truncate to 519.

**Key structural fact (new).**  `s60` is the sum of 16 early invoke
frequencies (mostly `1.0`, plus the `inAlienOrbit() && dlc1Won() &&
!inEmptyOrbit()` chain at 0.5/0.25/0.125).  `s87` is `getEnemyGuns` (0.4734)
plus 39 `Image2DCreate` invokes all at the *tail* frequency `T = 0.2989`.
The MBFI minimum is the `new Array<KIPlayer*>` delete pad `%771`, whose
frequency is `P(autoTurret != nullptr) * T = 0.625*T`.  Therefore, with the
min unchanged,

```
scaled(16-pred)  ∝ 1/T          scaled(40-pred)  = const
```

i.e. **only the tail frequency `T` can break the tie**, and it must *increase*
by >= 0.052 % (to push 519.2697 below 519.0 while 519.5021 stays).  This is a
much smaller target than the 0.1 % window quoted in w150 because the *lower*
window `k in [0.999042, 0.999479)` is also valid and is reached by increasing
`T`.

`__builtin_expect` on the `autoTurret` guard cannot help: it changes `P`, hence
the min and both scaled values by the same factor (measured: `expect(autoTurret
!= nullptr, 1)` -> `324.705 / 324.851`, still tied; `expect(...,2)` and
`expect(...,-1)` are byte-identical to 1, confirming w150's
`handleBrSelExpect` analysis).  `expect(inAlienOrbit(), 1)` and
`expect(dlc1Won(), 1)` are body-exact but *increase* the 16-pred pad (567 /
525), the wrong direction.

### The one spelling that flips the tie

`expect((*playerGuns)[i]->isRocketGun(), 0)` makes the playerGuns loop body
colder, which raises `T`:

| variant | raw | size | extab | 16-pred scaled | 40-pred scaled |
|---|---:|---:|---:|---:|---:|
| base | **0** | 2488 | **7** | 519.2697 | 519.5021 |
| `expect(isRocketGun(), 0)` (player loop) | 116 | 2488 | **3** | **511.9835** | 519.5021 |
| `expect(isMineGun(), 0)` (player loop) | 122 | 2488 | **3** | 499.2859 | 519.5021 |
| `expect(isRocketGun(), 0)` (enemy loop) | 1526 | 2488 | 57 | 473.5020 | 517.7151 |
| `expect(i < playerGuns->size(), 0)` | 1528 | 2508 | 83 | 245.8542 | 519.5021 |
| `expect(i < enemyGuns->size(), 0)` | 148 | 2488 | 26 | 327.8503 | 512.0280 |

With `expect(isRocketGun(), 0)` the tie **is fixed**: rows 1/3/11/13/15/21 all
match and only rows 18/19 differ, because the 116-byte body change shifts the
call-site ranges by 4 B:

```
row 18 orig (798, 84, 2468)  ours (802, 90, 2468)
row 19 orig (892,  6, 2464)  ours (894,  6, 2464)
```

So the frequency mechanism is proven; the remaining problem is purely the
**body cost**.  The `isRocketGun` branch is `cbz r0, <skip>` (true path is the
fallthrough); `expect(...,0)` makes the false path likely and inverts it,
reordering the whole loop.  No body-exact spelling that raises `T` was found.

### Full body-exact sweep (all leave extab 7)

| variant | raw | extab |
|---|---:|---:|
| `expect(dlc1Won(), 1)` | 0 | 7 |
| `expect(inAlienOrbit(), 1)` | 0 | 7 |
| `!expect(inEmptyOrbit(), 0)` | 0 | 7 |
| `expect(scanner/turret/getAttribute(50), 1)` | 0 | 7 |
| `goto done;` after the autoTurret block | 0 | 7 |
| `&&` chain reordered (3 orders) | 4-10 | 7 |
| `autoTurret > (Item*)0` | 0 | 7 |
| `expect(enemyGuns != nullptr, 1)` | 43 (trampolines only) | 25 |
| `expect(playerGuns != nullptr, 1)` | 57 | 29 |
| `expect(autoTurret != nullptr, 1)` | 6 | 10 |
| named `String a,b` temporaries | 1968 | 138 |
| `players` local re-scoped (4 shapes) | 10-109 | 28-29 |

`expect(autoTurret != nullptr, 1)` + `expect(inAlienOrbit(), 1)` gives
`354.80 / 324.85` (16-pred now *bigger*), so combinations do not help.

## 2. HangarWindow::render — row table unchanged (43 differing rows, 676 B)

156 rows, 948-byte record, `reclen` 948 in both; ours LSDA payload is 1 byte
shorter (`len 926` vs 927).  Row format `(start, length, landingPadOffset,
action)`.

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

Rows 100, 136-155 are identical.

### getShip row 39/40 (confirmed against disassembly)

The `getShip` invoke is at `0x149b36` = function offset **2654**.  Original
row 39 is `2508..2658` (LP 9664), so `getShip` is inside row 39; row 40 starts
at 2696.  Ours row 39 is `2508..2626` and row 40 starts exactly at `getShip`
(2654, LP 9514).  So the original's `getShip` unwinds to the *outer* cleanup:
the `volatile int *padX` lifetime must start **after** `getShip`.  Measured
placements (this wave):

| placement | raw | align | extab | size | row 39 len |
|---|---:|---:|---:|---:|---:|
| base (padX before `if (tab != 0)`) | 1965 | 291 | 676 | 9700 | 118 |
| padX first stmt of inner if body | 8610 | 799 | 760 | 9676 | 146 |
| padX after inner if block | 6731 | 734 | 718 | 9700 | 362 |
| padX after `if (tab != 0)` block | 6606 | 838 | 706 | 9708 | 236 |

Moving `padX` after `getShip` does move row 39 in the right direction
(118 -> 146) but every placement either overshoots or breaks other rows.

### Rows 96-135: `sl` holds `&Globals::layout` in the original

Original credit loop (`0x14abc0`): `ldr.w r2,[sl]` then `ldr r2,[r2,#44]`
(double load, `sl = &Globals::layout`).  Ours: `ldr.w r2,[sl,#44]` (folded,
`sl = layout`).  The 4-byte difference is the uniform 2-6 B shift in rows
97-135.  The non-loop twin at `src/game/ui/HangarWindow.cpp:1000` already uses
`Globals::layout->field_0x20_top` directly, so the `Layout *lay` local at line
918 is an invention.  Measured this wave:

| variant | raw | align | extab | size | rows |
|---|---:|---:|---:|---:|---:|
| base (`lay` local) | 1965 | 291 | 676 | 9700 | 156 |
| `y` uses `Globals::layout` (M1) | **1939** | 418 | 676 | 9700 | 156 |
| draw uses `Globals::layout` | 8553 | 1279 | 761 | 9696 | 156 |
| no `lay`, fully direct | 3308 | 735 | 710 | 9704 | 145 |
| `Layout **layp = &Globals::layout` | 3308 | 735 | 710 | 9704 | 145 |
| `lay` moved before `int y` | 7869 | 1271 | 729 | 9700 | 156 |

M1 is a raw win (1965 -> 1939) but an align regression (291 -> 418) and does
not change extab, so it was not landed.  The register allocator gives `sl` to
`description` whenever `lay` is removed, and to the layout *value* whenever
`lay` is kept; no shape tried yields `sl = &Globals::layout` with
`description = r6`.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9719
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w9719/fdiff.py _ZN5RadarC2EP5Level          # diff 0, 2488
python3 /dev/shm/w9719/align_diff.py _ZN5RadarC2EP5Level     # TOTAL 0
python3 /dev/shm/w9719/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                            # 1326 total, 7 @ 0x12d484
```

MBP trace (instrumented clang, not committed):

```sh
/dev/shm/w9719/mkir.sh src/game/weapons/Radar.cpp /dev/shm/w9719/Radar.ll
MBP_TRACE=1 /tmp/llvm_android_702b/build2/bin/llc \
  -mtriple=armv7-none-linux-androideabi16 -mattr=+neon -float-abi=soft -Oz \
  -debug-only=block-freq /dev/shm/w9719/Radar.ll -o /dev/null > mbp.txt
python3 /dev/shm/w9719/mbp4.py   # prints the 16- and 40-pred pad scaled freqs
```

Scratch (not committed), `/dev/shm/w9719/`: `fdiff.py`, `align_diff.py`,
`extab_lsda_decode.py`, `diffoff.py`, `mkir.sh`, `mbp.sh`, `mbp4.py`,
`batch.py`, `full.py`, `hwtest.py`, `Radar.base.cpp`, `HangarWindow.base.cpp`.

## 4. Open questions

1. A body-exact spelling that raises the tail frequency `T` by >= 0.052 %.
   The only working lever is making the playerGuns loop body colder, which
   inverts the `cbz` and costs 116 B.  A source shape that makes the loop body
   colder while keeping the true path as fallthrough would land extab 0.
2. The original PGO profile would give `T` directly; `expect(isRocketGun(), 0)`
   is the heuristic approximation of the profile's cold `isRocketGun` branch.
3. Rows 39/40: a lifetime-tracked local declared after `getShip` that does not
   shrink the 9700-byte register allocation.
4. Rows 96-135: a shape that puts `&Globals::layout` in `sl` while keeping
   `description` in `r6`.
