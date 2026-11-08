# Radar::draw wave 152 (w9901) — the 0x30 pad + else-arm String copy lands 8570/1345 at 9,908 B

**Status:** source + build change landed, 2026-11-08. Worktree `wt/w9901`, based on `master`
`4a6104a43` (wave 152). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave151-w9801-pad30.md`, `radar-draw-wave152-w9802.md`,
`radar-draw-wave151-w9702-pad.md`, `radar-draw-wave151-w9703.md`.

Baseline (HEAD, stock 0x40 pad): original 9,908 B; ours 9,908 B; raw same-address diff
**8,620**; instruction-index alignment **1,530**; `.text` 0x1337c8; `.ARM.extab` 0x26298
(156,312); `.rodata` 0x1c9e10; byte_exact 4507.

## 1. What landed

`CMakeLists.txt` now exposes `GOF2_GOLD_PAD_CAP` (hex, default `30`) and links with a
build-dir copy of the NDK gold patched by `tools/patch_gold_pad.py` (the w9801 mechanism,
reproduced here because it was not on this worktree's base). `GOF2_GOLD_PAD_CAP=40` restores
the stock linker.

`Radar.cpp` takes the w9801 literal else-arm copy plus two type witnesses that make the body
exactly 9,908 B:

```diff
-    int landmarkHit = false;
+    bool landmarkHit = false;
...
-                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
+                        String labelCopy2 = this->lockLabel;
+                        Globals::Canvas->DrawString(radarFont(), labelCopy2, this->screenX + offset,
                                                     this->screenY + rowHeight, false);
...
-                bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
+                int wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
```

The `bool landmarkHit` + `int asteroid-wreck` pair is the size companion set: on this base
the String copy alone is 9,892 B (16 short, shifts every later function -16 and collapses
byte_exact); `bool landmarkHit` alone is 9,904 B; adding `int asteroid-wreck` lands exactly
9,908 B with `.rodata` back at 0x1c9e10.

## 2. Measured shapes (pad 0x30, current base)

| shape | usize | raw | align | extab | `.rodata` |
|---|---:|---:|---:|---:|---:|
| HEAD baseline (stock pad 0x40) | 9908 | 8620 | 1530 | 0x26298 | 0x1c9e10 |
| pad 0x30, no source change | 9908 | 8621 | 1530 | 0x26298 | **0x1c9e00** |
| w9801 landed (`String` + w9702 types) | 9908 | 8635 | 1788 | 0x262a4 | 0x1c9e10 |
| **landed: `String` + `bool landmarkHit` + `int asteroid-wreck`** | **9908** | **8570** | **1345** | **0x262a4** | **0x1c9e10** |
| `String` + `bool landmarkHit` only | 9904 | 8468 | 1344 | 0x262a4 | 0x1c9e10 |
| `String` alone | 9892 | 8658 | 1678 | 0x262a4 | 0x1c9e10 |
| w9802 prvalue copy + `bool landmarkHit` + `int enemyHit` + `int asteroid-wreck` | 9908 | 8593 | 1533 | 0x26298 | 0x1c9e00 |
| `String&` alias (no copy) | 9908 | 8621 | 1530 | 0x26298 | 0x1c9e00 |

The w9802 prvalue-copy shape keeps the LSDA at 19 rows but moves `.rodata` to 0x1c9e00
(pad 0x30 + exact extab), which changes every literal pool; its best size-exact aligned result
is 1533 (raw 8593), i.e. raw improves but aligned does not. The literal copy (21 rows) is the
only shape that keeps `.rodata` at 0x1c9e10 under the 0x30 pad, and it is the one that improves
both metrics.

## 3. LSDA row tables

Original record @ 0x1c203c, **132 B / 19 rows** (11 LP != 0):

```
(0,568,0,0) (568,8,9818,0) (576,1818,0,0) (2394,10,9856,0) (2404,158,0,0)
(2562,10,9852,0) (2576,16,9858,0) (2592,12,9842,0) (2604,16,9832,0) (2638,20,9822,0)
(2658,78,0,0) (2736,14,9820,0) (2750,1256,0,0) (4006,8,9872,0) (4014,1796,0,0)
(5810,8,9870,0) (5818,788,0,0) (6606,8,9816,0) (6614,3294,0,0)
```

Landed record @ 0x1c203c, **144 B / 21 rows** (13 LP != 0). The extra two rows are the
`String(String const&)` copy-ctor LP `(2376,26,0,0)` and the `DrawString` LP `(2420,110,0,0)`
that split the original `(2404,158,0,0)` gap:

```
(0,568,0,0) (568,8,9806,0) (576,1790,0,0) (2366,10,9852,0) (2376,26,0,0)
(2402,18,9846,0) (2420,110,0,0) (2530,10,9848,0) (2544,18,9854,0) (2562,14,9836,0)
(2576,12,9826,0) (2600,20,9816,0) (2620,78,0,0) (2698,10,9808,0) (2708,1274,0,0)
(3982,12,9872,0) (3994,1780,0,0) (5774,8,9866,0) (5782,856,0,0) (6638,8,9804,0)
(6646,3262,0,0)
```

## 4. Section sizes

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | **0x1337b8** | **-16** |
| `.ARM.exidx` | 0x19cc08 / 0x6f70 | 0x19cbf8 / 0x6f70 | base -16, size exact |
| `.ARM.extab` | 0x1a3b78 / 0x26298 | 0x1a3b68 / **0x262a4** | **+12** |
| `.rodata` | 0x1c9e10 / 0x41b67 | 0x1c9e10 / 0x41b67 | exact |
| `.data` / `.got` / `.rel.plt` / `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` | — | — | exact |
| file size | 2,187,328 | 2,187,328 | exact |

The 4 B alignment gap before `.rodata` absorbs the -16/+12 pair, so every section from
`.rodata` on keeps its original base and size. `.text` differing bytes **87,131 -> 87,097**
(-34); `.rel.plt` stays byte-identical.

## 5. Gate

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint:   CLEAN (hard invariants 0)
drift:  0 field(s) across 0 class(es)
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,620 -> 8,570** (-50); aligned **1,530 -> 1,345**
(-185).

## 6. Open questions

1. A 19-row shape that keeps `.rodata` at 0x1c9e10 under the 0x30 pad needs another 12 B of
   `.ARM.extab` growth elsewhere (or a 16 B larger body). The w9802 prvalue shape is 19 rows
   but moves `.rodata` -16; its best size-exact aligned is 1533.
2. `alwaysEnemy -> int` on top of the landed shape gives **7,878 / 1,270 at 9,896 B** (12 B
   short). A witnessed 12 B body addition there would beat this landing on both metrics.
3. The `bool landmarkHit` / `int asteroid-wreck` pair is a size companion; w9401 records the
   asteroid `wreck` as `bool`. The binary metric prefers `int` here.
