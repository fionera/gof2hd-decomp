# Extab LP-identity ties (wave 150, w9619): the tie-break window is a 0.1 %
# frequency change; every body-exact spelling leaves it at 7 B

Status: **analysis only, no landed `src/` change** (new spellings tested, all
fail the byte-exact constraint). Worktree `wt/w9619`, base `7eed2966e` (wave
149). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 0x1337c8, `.ARM.extab` 0x26298 (156,312 B),
`.ARM.exidx` 0x6f70 — all size-exact.

Targets and unchanged metrics (raw same-address diff / instruction-index
alignment / extab bytes):

| fn | bodyraw | aligned | extab |
|---|---:|---:|---:|
| `Radar::Radar` (`_ZN5RadarC2EP5Level`, `0x12d484`) | **0** | **0** | **7** |
| `SpaceLounge::drawLounge` (`0x170d38`) | 176 | 42 | 14 |
| `ModStation::ModStation` (`0xd52d0`) | 43 | 12 | 2 |
| `HangarWindow::render` (`0x1490d8`) | 1965 | 291 | 676 |

Total `.ARM.extab` differing bytes: **1309** (unchanged).

## 1. Radar::Radar — the tie-break arithmetic, corrected

The MBP tie is between two cleanup pads (machine blocks `%bb.199`, `%bb.200`)
whose **scaled** frequencies are both 519.  The raw (float) frequencies are

```
outer / function pad   0.00001156318791  -> 519.2697   (16 invoke preds)  %bb.200 = %ir-block.189
inner / enemyGuns pad  0.00001156836373  -> 519.5021   (40 invoke preds)  %bb.199 = %ir-block.399
```

`selectBestCandidateBlock` picks the **least** frequent EHPad, and on an integer
tie keeps the **later** worklist entry.  The worklist is
`... %bb.200 ... %bb.199`, so `%bb.199` (519.5021) is placed first today.  The
original places `%bb.200` (519.2697) first, i.e. it needs
`floor(519.2697*k) < floor(519.5021*k)`.

**Correction to w9513:** the two pads are `%bb.200` (`%ir-block.189`, the
function-level pad, 16 invoke predecessors) and `%bb.199` (`%ir-block.399`, the
enemyGuns pad, 40 invoke predecessors); the layout order printed by
`-print-after=block-placement` is `... bb.198, bb.199, bb.200, bb.201 ...`, so
`bb.199` is at the lower address (0x12de22) today, matching rows 19/21.
Solving `floor(519.2697 k) < floor(519.5021 k)` gives, near each integer `n`,

```
k in ( n/519.2697 , n/519.5021 )   [non-empty only when the two floors differ]
```

The first window above k = 1 is

```
k in [1.000958, 1.001400)      ->  min in (7.9888, 7.9923]
```

and near k = 2:

```
k in [2.000008, 2.000900)  -> 1038.539 / 1039.004  ->  min in (3.9982, 3.99998]
```

So the tie can be broken by **either** a ~0.1 % reduction of the MBFI minimum
(the `new Array<KIPlayer*>` landing pad, freq 8.0) **or** a 2x reduction.  The
2x route needs `P(autoTurret != nullptr) = 0.3125`; the 0.1 % route needs any
small frequency perturbation of the min pad.

The min pad is `%bb.195`, `freq = 8.0`, single predecessor `%bb.190`
(`freq = 8,388,607`, the `new Array<KIPlayer*>` invoke block).  Its frequency is
`P(autoTurret != nullptr) * freq(cont) / 2^20`.  `P` is the pointer heuristic
20/32 = 0.625, so `freq(min) = 8.0` exactly.

## 2. New body-exact spellings tested (all leave extab at 7)

Every variant below keeps `Radar::Radar` byte-identical (bodyraw 0, size 2488)
and leaves the extab record at 7 differing bytes — the tie is untouched.

| variant | extab | bodyraw |
|---|---:|---:|
| base | **7** | **0** |
| `__builtin_expect(autoTurret != nullptr, 2)` | 10 | 6 |
| `__builtin_expect(autoTurret != nullptr, -1)` | 10 | 6 |
| `__builtin_expect(autoTurret, 2)` | 10 | 6 |
| `autoTurret > (Item *) 0` (pointer `>`, not equality) | **7** | **0** |
| `autoTurret > (Item *) 0 && level != nullptr` | 86 | 1744 |
| `autoTurret > (Item *) 0 && (unsigned) autoTurret > 2` | 40 | 165 |
| `autoTurret != nullptr && level > (Level *) 0` | 86 | 1744 |
| `autoTurret != nullptr & (this->players == nullptr)` (bitwise `&`) | 41 | 176 |
| `autoTurret > (Item *) 0 & (level != nullptr)` | 86 | 1744 |
| `if (autoTurret == nullptr) goto done; ... done: ;` (label after block) | **7** | **0** |
| `... goto done; } done: ;` (goto to end of function) | **7** | **0** |
| bare `done: ;` label after the `if` | **7** | **0** |
| `do { if (!autoTurret) break; ... } while (0);` | **7** | **0** |
| `while (autoTurret != nullptr) { ...; break; }` | **7** | **0** |
| `if (autoTurret == nullptr) { } else { ... }` | **7** | **0** |
| extra `{ }` scope around the body | **7** | **0** |
| `__builtin_expect(turret != nullptr, 1)` on the preceding guard | **7** | **0** |
| `__builtin_expect(isRocketGun(), 1)` (player or enemy loop, or both) | 11 | 12 |

Two facts this establishes:

1. **`__builtin_expect` values other than 0/1 are inert.**  clang 7 lowers
   `__builtin_expect(x, c)` in `LowerExpectIntrinsic::handleBrSelExpect`, which
   only compares `c` against the compared-to constant (`0` for a bare `br`).
   Any non-zero `c` yields the same 2000:1 metadata as `c == 1`.  `2` and `-1`
   are byte-for-byte identical to `expect(...,1)` (extab 10 / body 6).
2. **A `goto` cleanup is a no-op for the tie.**  A `goto` to a label after the
   block, to the end of the function, or a bare label all compile to the exact
   same CFG (`bodyraw 0`) and leave the 519/519 tie.

`autoTurret > (Item*)0` is interesting: it is body-exact (the code is the same
`cmp #0`), so the branch heuristic must still be 20/32 (or the layout is
insensitive), but it does not move the min.

## 3. Why the 0.1 % route is out of reach from source

The only source-level frequency levers are `__builtin_expect` (2000:1) and the
fixed heuristics (20/32, 1/2, 124/128, 4/68).  A 0.1 % change needs a *partial*
edge-probability change on one of the 40 invoke predecessors of the outer pad —
i.e. a branch whose probability changes by ~1/1000.  None of the heuristics
produces such a ratio, and any real second condition adds a block (see the
`&&` rows above, all body >= 165).  The `expect(isRocketGun(), 1)` family does
reach the right neighbourhood (extab 11, body 12) but costs 12 body bytes, so
`byte_exact` would drop 4507 -> 4505.

The remaining untested lever is the **worklist order**: if `%bb.199` were
pushed before `%bb.200`, the 519/519 tie would lay `%bb.200` first and the LSDA
would match with zero body change.  The worklist order follows the machine
layout of the predecessors, which is fixed by the byte-exact body; no source
spelling tried (including the goto forms, which change the IR block order)
moved it.

## 4. HangarWindow::render — row table (fn `0x1490d8`)

156 rows, 948-byte record, **676** differing bytes, 43 differing rows.
`reclen` 948 in both; ours LSDA payload is 1 byte shorter (`len 926` vs 927).
Row format `(start, length, landingPadOffset, action)`.

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

Rows 100, 136-155 are identical.  Row 39/40 is the only *structural* boundary
difference (row 39 is 32 B shorter and row 40 is 42 B longer, shifted 42 B);
rows 96-135 are a uniform 2-6 B address shift (the credit-loop RA/block order
identified in w9514), and rows 126/127 carry the 8 B length deficit (the `sl`
double-load + `&this->buttons` reload).

## 5. HangarWindow padX placement sweep (measured this wave)

`padX` is the `volatile int *padX = 0;` lifetime sink in the ship branch
(`src/game/ui/HangarWindow.cpp`, around line 814).  Its `llvm.lifetime.start`
is what puts `getShip` on the padX cleanup landing pad.  Placements measured:

| placement | extab | raw | align | size | row 39 len |
|---|---:|---:|---:|---:|---:|
| base (padX before `if (tab != 0)`) | **676** | **1965** | **291** | **9700** | 118 |
| padX first stmt of `if (tab != 0)` body | 783 | 8618 | 772 | 9676 | 118 |
| padX first stmt of inner `if (!(...))` body | 783 | 8618 | 772 | 9676 | 118 |
| padX after the inner `draw()` / `buttonOffset` | 718 | 6731 | 734 | 9700 | 362 |
| padX after the whole `if (tab != 0)` block | 706 | 6606 | 838 | 9708 | 236 |

Moving the declaration after `getShip` does move the row-39 boundary in the
right direction (118 -> 236/362 vs the original 150), but every placement
either overshoots the boundary or changes the function size, and all of them
*increase* the extab byte diff.  The 9700-byte size and the 150-byte row 39 are
coupled through padX's lifetime exactly as w9514 diagnosed; no placement keeps
both.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9619
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w9619/fdiff.py _ZN5RadarC2EP5Level          # diff 0, 2488
python3 /dev/shm/w9619/align_diff.py _ZN5RadarC2EP5Level     # TOTAL 0
python3 /dev/shm/w9619/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                            # 7 B @ 0x12d484
```

MBP trace (instrumented clang, not committed):

```sh
MBP_TRACE=1 /tmp/llvm_android_702b/build2/bin/llc \
  -mtriple=armv7-none-linux-androideabi16 -mattr=+neon -float-abi=soft -Oz \
  -debug-only=block-freq Radar.opt.ll -o /dev/null
```

Scratch (not committed), `/dev/shm/w9619/`: `fdiff.py`, `align_diff.py`,
`extab_lsda_decode.py`, `measure.sh`, `try_radar.py`..`try7.py`,
`try_hw.py`..`try_hw3.py`, `Radar.opt.ll`, `Radar.fn.ll`, `hw_rows.txt`,
`hw_diffrows.txt`.

## 7. Open questions

1. A source spelling that changes the MBFI minimum (`new Array<KIPlayer*>`
   landing pad, freq 8.0) by 0.1 % **or** by 2x without moving `.text`.  The
   2x route is `P(autoTurret != nullptr) = 0.3125`; the 0.1 % route needs a
   partial edge-probability change on one of the outer pad's 40 invoke
   predecessors.
2. A way to push `%bb.199` before `%bb.200` in the `EHPadWorkList` (then the
   519/519 tie would lay `%bb.200` first with zero body change).  The worklist
   order follows the machine layout of the predecessors, which is fixed by the
   byte-exact body.
3. The original PGO weights (`.profdata`/`.profraw`) would settle both this and
   the ModStation scheduler question directly.
