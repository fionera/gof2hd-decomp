# Radar::draw wave 155 (w10114) — structural diff map; asteroidLockTimer store order

**Status:** source change landed, 2026-11-08. Worktree `wt/w10114`, based on `master`
(`f4e0ea40b`, wave 154). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave154-w10002.md`, `radar-draw-wave153-w10003.md`,
`radar-draw-wave152-w9902.md`.

Baseline (HEAD, wave 154): original 9,908 B; ours 9,908 B; raw same-address diff
**8,577**; instruction-index alignment **1,484**; `.text` 86,347 differing bytes;
`.ARM.extab` 1,255; `.rel.plt` byte-identical; LSDA **132 B / 19 rows / cst_len 112**.

## 1. Per-region diff map (final build)

Clusters are contiguous `difflib` opcode-regions on the instruction-index-normalised
stream, merging equal gaps <= 3 instructions. `off`/`osz` are original function-relative
byte offset and size; `mism` is the aligned non-matching instruction count; `str` excludes
pure `regalloc`/`imm`/`memaddr`/`operand` cosmetic categories. Tool: `/dev/shm/w10114/clustermap.py`.

| # | off | osz | uoff | usz | mism | str | likely cause |
|---|-----|-----|------|-----|------|-----|--------------|
| 1 | 4878 (0x130e) | 211 | 4886 | 147 | 73 | 62 | `enemy == locked` block + **navWreck block placement** (orig inline at 0x1356, ours sunk to 0x1822); `68` in `sl` vs rematerialised |
| 2 | 5952 (0x1740) | 161 | 6026 | 37 | 56 | 52 | gas-cloud hit test: orig inline at 0x174c, ours at 0x16c8; literal-pool boundary |
| 3 | 5876 (0x16f4) | 13 | 5808 | 153 | 50 | 49 | `!(isAutoPilot()||otherLock||enemyHit)` guard: ours sinks the gas hit test here |
| 4 | 2246 (0x8c6) | 100 | 2192 | 110 | 46 | 45 | landmark label `DrawString`; `offset` kept as field byte offset in orig (`ldr.w ip,[r4,r9]`) vs loaded value |
| 5 | 7672 (0x1df8) | 189 | 7692 | 173 | 59 | 45 | `if (enemyHit)`/`else` polarity + **asteroidLockTimer if/else store order** |
| 6 | 1846 (0x736) | 118 | 1804 | 116 | 46 | 37 | planet-lock guard / literal pool |
| 7 | 2394 (0x95a) | 81 | 2352 | 71 | 40 | 35 | landmark else-arm scoped String copy (w9902 shape) |
| 8 | 2736 (0xab0) | 91 | 2666 | 109 | 38 | 35 | landmark `i==0 && !alienOrbit` arm |
| 9 | 1018 (0x3fa) | 169 | 1012 | 125 | 44 | 34 | campaign-mission block; `ldrd` fold of three bool spills |
| 10 | 1484 (0x5cc) | 168 | 1460 | 159 | 45 | 31 | planet loop body |

Total 1,484 (now 1,477) mismatched instructions, 186 clusters. Almost all clusters are
register allocation or block placement; the only genuine *missing stores* are in the
asteroid-lock tail (region #5).

## 2. The genuine structural difference: a hoisted `asteroidLockTimer = 0`

Original `0x1dd0` (`Radar::draw+0x1dd0`):

```
1dd0 cbz  r0, 1dda        ; (candidateWreck ? field_0x1c : lockedAsteroid) == nullptr
1dd2 movs r0, #0
1dd4 str.w r0, [r4, #412] ; else: asteroidLockTimer = 0
1dd8 b.n  130264
1dda movs r0, #0
1ddc str.w r0, [r4, #412] ; then: asteroidLockTimer = 0
1de0 str  r0, [r4, #16]   ;       field_0x10 = 0
```

Ours (before) hoisted the common store:

```
1dde movs r0, #0
1de0 str.w r0, [r4, #412] ; one store before the branch
1de4 cmp  r1, #0
1de6 bne.w 130264
1dea str  r0, [r6, #0]    ; field_0x10 = 0
```

The original source therefore has the two stores in the order
`field_0x10 = 0; asteroidLockTimer = 0;` inside the `nullptr` arm.  Swapping them
prevents clang's store sinking and emits the second `str.w [r4,#412]` (the original
has 3 `asteroidLockTimer` stores, ours had 1; the other two missing stores are
`dockNavPtr` and `enemyLockTimer`, still open).

## 3. Landing: reorder + `bool alwaysEnemySeen` (size companion)

The store-order fix alone is **+4 B** (9,912 B).  The size companion that lands exactly
9,908 B with the best metrics is the witnessed flag type `bool alwaysEnemySeen`
(only ever tested as a condition; `int -> bool` is **-4 B** here).

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD wave 154 | 9908 | 8577 | 1484 |
| store reorder only | 9912 | 8561 | 1479 |
| **store reorder + `bool alwaysEnemySeen` (landed)** | **9908** | **8482** | **1477** |
| `bool enemyHit` alone (no reorder) | 9892 | 8003 | 1335 |
| `bool enemyHit` + reorder | 9900 | 8474 | 1336 |
| `int alwaysEnemy` + reorder | 9908 | 8610 | 1465 |

`bool enemyHit` + reorder is the strongest shape (8,474/1,336 at 9,900 B) but is 8 B
short; no +8 B companion that keeps the layout was found in this pass.

## 4. LSDA rows

Original and ours both: `pers 0x5eb60 uw 3w lp 0xff tt 0xff cst 0x1 len 112`, **132 B /
19 rows / 11 LP!=0**.  Row offsets differ (body still 1,477 instructions off); the
record size and call-site length are identical.

Original: `(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)`

Ours: `(0,568,0) (568,8,9812) (576,1776,0) (2352,12,9854) (2364,136,0)
(2500,10,9850) (2514,16,9856) (2530,14,9840) (2544,12,9830) (2568,20,9820)
(2588,78,0) (2666,10,9814) (2676,1294,0) (3970,12,9874) (3982,1760,0)
(5742,8,9868) (5750,872,0) (6622,8,9810) (6630,3278,0)`

## 5. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

All 27 section sizes exact; file size 2,187,328 B.  `.rodata`/`.data`/`.bss`/
`.ARM.exidx`/`.rel.plt` byte-identical.

* raw same-address **8,577 -> 8,482** (-95)
* instruction-index alignment **1,484 -> 1,477** (-7)
* `.text` differing bytes **86,347 -> 86,252** (-95)
* `.ARM.extab` differing bytes 1,255 (unchanged)

## 6. Open questions

1. `bool enemyHit` + store reorder is 9,900 B / 8,474 / 1,336.  A witnessed +8 B body
   addition would land the best both-metric shape at 9,908 B.
2. The two other missing stores (`dockNavPtr` 3 vs 2, `enemyLockTimer` 4 vs 3) are the
   next structural levers; both are if/else hoists like the one fixed here.
3. The navWreck block placement (region #1) is still the largest cluster: the original
   keeps it inline at 0x1356, ours sinks it after the gas-cloud loop (0x1822).  A
   branch-weight/PGO source shape that keeps it inline is the biggest remaining win.
