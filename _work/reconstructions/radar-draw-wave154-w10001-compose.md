# Radar::draw wave 154 (w10001) — composing the 0x30-pad String shape with the scoped copy

**Status:** source + build change landed, 2026-11-08. Worktree `wt/w10001`, based on
`master` `4417db0b4` (wave 153). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave152-w9901-pad30-shape.md`, `radar-draw-wave152-w9902.md`,
`radar-draw-wave152-w9903.md`, `radar-draw-wave151-w9801-pad30.md`.

## 1. The composition

w9902 landed the scoped `String` copy (19-row LSDA, 8,577 / 1,484, all sections
exact).  w9901 landed the same function with the literal else-arm copy plus the
0x30 gold pad (21-row LSDA, 8,570 / 1,345, `.text` -16 / `.ARM.extab` +12).
The two are source-compatible: the scoped copy and the literal copy can both be
present, and clang folds the scoped `calcDistance` temporary into the direct
assignment, so the emitted body is exactly w9901's.

The landed else arm is:

```cpp
KIPlayer *landmark = this->landmarks->data()[i];
{
    String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                           this->cameraX, this->cameraY, this->cameraZ);
    this->lockLabel = labelCopy2;          // w9902 scoped copy (destroyed before DrawString)
}
String labelCopy3 = this->lockLabel;       // w9901 literal else-arm copy
Globals::Canvas->DrawString(radarFont(), labelCopy3, this->screenX + offset,
                            this->screenY + rowHeight, false);
```

plus w9901's type witnesses (`int marked`, `bool enemyHit`; `bool landmarkHit`,
`int asteroid-wreck` and `int enemy-wreck` were already the wave-153 types).

## 2. Both merge orders (identical codegen)

| order | source | usize | raw | aligned | `.text` | `.ARM.extab` | `.rodata` |
|---|---|---:|---:|---:|---:|---:|---:|
| baseline (w9902) | scoped copy only, stock 0x40 pad | 9908 | 8577 | 1484 | 0x1337c8 | 0x26298 | 0x1c9e10 |
| **A: w9902 shape + w9901 else arm (landed)** | scoped copy + literal copy | **9908** | **8570** | **1345** | 0x1337b8 | 0x262a4 | 0x1c9e10 |
| B: w9901 shape + w9902 scope | literal copy inside a scope | 9908 | 8570 | 1345 | 0x1337b8 | 0x262a4 | 0x1c9e10 |

Both orders compile to the same instruction stream.  The scoped block is folded
(`lockLabel = calcDistance(...)`), so the codegen is the literal shape; the
composition therefore carries the scoped copy in source while keeping w9901's
register allocation.

Other compositions measured (all pad 0x30):

| shape | usize | raw | aligned | note |
|---|---:|---:|---:|---|
| scoped else only (19 rows) | 9908 | 8577 | 1484 | `.rodata` -16 (0x1c9e00) -> byte_exact 4044 |
| scoped else + literal **if**-arm copy | 9904 | 8592 | 1385 | 4 B short |
| scoped else + literal **enemy**-arm copy | 9908 | 8606 | **1344** | best aligned, raw +36 |
| scoped else + literal **route**-arm copy | 9912 | 9365 | 1447 | raw regression |
| literal else + literal if-arm copy | 9912 | 8698 | 1366 | 23 rows, `.rodata` +16 |
| literal else + `alwaysEnemySeen` bool + `slotCount` bool | 9908 | **8426** | 1367 | best raw, aligned +22 |
| literal else + `alienOrbit` int | 9904 | 8455 | 1349 | 4 B short |

The last two are the only Pareto alternatives: `alwaysEnemySeen`/`slotCount`
trades 144 raw bytes for 22 aligned instructions.  The landed shape is the one
that improves **both** metrics over the wave-153 baseline.

## 3. LSDA row tables

Original record @ 0x1c203c, **132 B / 19 rows** (11 LP != 0):

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

w9902 scoped record, **132 B / 19 rows** (11 LP != 0):

```
(0,568,0) (568,8,9810) (576,1776,0) (2352,12,9854) (2364,136,0)
(2500,10,9850) (2514,16,9856) (2530,14,9840) (2544,12,9830) (2568,20,9820)
(2588,78,0) (2666,10,9812) (2676,1294,0) (3970,12,9874) (3982,1768,0)
(5750,8,9868) (5758,872,0) (6630,8,9808) (6638,3270,0)
```

Landed composed record, **144 B / 21 rows** (13 LP != 0).  The two extra rows are
the literal copy-ctor LP `(2376,26,0)` and the `DrawString` LP `(2420,110,0)`
that split the scoped record's `(2364,136,0)` gap:

```
(0,568,0) (568,8,9806,0) (576,1790,0) (2366,10,9852,0) (2376,26,0,0)
(2402,18,9846,0) (2420,110,0,0) (2530,10,9848,0) (2544,18,9854,0) (2562,14,9836,0)
(2576,12,9826,0) (2600,20,9816,0) (2620,78,0,0) (2698,10,9808,0) (2708,1274,0,0)
(3982,12,9872,0) (3994,1780,0,0) (5774,8,9866,0) (5782,856,0,0) (6638,8,9804,0)
(6646,3262,0,0)
```

The 12 B extab growth is what keeps `.rodata` at 0x1c9e10 under the 0x30 pad
(the pad shrinks `.text` by 16; the 4 B alignment gap before `.rodata` absorbs
the -16/+12 pair).

## 4. Build change: `GOF2_GOLD_PAD_CAP`

`CMakeLists.txt` now exposes `GOF2_GOLD_PAD_CAP` (hex, default `30`; `40` =
stock) and copies the NDK `ld.gold` into the build dir, rewriting the two
`movl $0x40,-0x44(%rbp)` stub-table padding-cap immediates with
`tools/patch_gold_pad.py`.  The custom command is a dependency of the link and
the patched binary is the linker input.  This is a build-time patch of the
linker, not a linker flag: the single plain gold link is unchanged.

## 5. Section deltas

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | **0x1337b8** | **-16** |
| `.ARM.exidx` | 0x19cc08 / 0x6f70 | 0x19cbf8 / 0x6f70 | base -16, size exact |
| `.ARM.extab` | 0x1a3b78 / 0x26298 | 0x1a3b68 / **0x262a4** | **+12** |
| `.rodata` | 0x1c9e10 / 0x41b67 | **0x1c9e10** / 0x41b67 | exact |
| `.data` / `.bss` / `.got` / `.rel.plt` / `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` | — | — | exact |
| file size | 2,187,328 | 2,187,328 | exact |

All function addresses are unchanged (the pad is at the end of `.text`).
`.text` differing bytes **86,623 -> 86,616** (-7).  `.rel.plt` stays
byte-identical.

## 6. Gate

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint:   CLEAN (hard invariants 0)
drift:  0 field(s) across 0 class(es)
parity: 5370/5370 (0 extra / 0 missing)
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,577 -> 8,570** (-7); aligned
**1,484 -> 1,345** (-139).  Aligned total **23,978 -> 23,839**.

## 7. Open questions

1. A 19-row shape that keeps `.rodata` exact under a shrunken pad needs the pad
   cap at `0x34`/`0x38`/`0x3c` (`.text` -12/-8/-4, `.rodata` exact) — but the
   body is unchanged, so the metrics stay 8,577 / 1,484.  Not a win.
2. `alwaysEnemySeen` bool + `slotCount` bool is the raw-best 9,908 B shape
   (8,426 / 1,367).  If the coordinator prefers raw over alignment, it is a
   one-line toggle swap on this landing.
3. The remaining 1,345 aligned mismatches are outside the composed else arm
   (planet loop, enemy loop, gas/asteroid tail).
