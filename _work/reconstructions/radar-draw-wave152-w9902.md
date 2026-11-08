# Radar::draw wave 152 (w9902) — a scoped `String` copy keeps the 19-row LSDA, aligned 2,188 -> 1,484

**Status:** source change landed, 2026-11-08. Worktree `wt/w9902`, based on `master`
(`4a6104a43`, wave 152). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave152-w9802.md`, `radar-draw-wave151-w9801-pad30.md`,
`radar-draw-wave151-w9702.md`, `radar-draw-wave151-w9703.md`.

Baseline (HEAD): original 9,908 B; ours 9,908 B; raw same-address diff **8,620**;
instruction-index alignment **1,530**; ours LSDA record 131 B / 19 rows
(call-site length 111); `.text` 87,131 differing bytes; `.ARM.extab` 1,324;
`.rodata` 0x1c9e10; byte_exact 4507.

## 1. The answer: a scoped `String` copy is destroyed before `DrawString`

w9802's `String labelCopy2 = calcDistance(...); lockLabel = labelCopy2;
DrawString(..., labelCopy2, ...)` keeps the local live across `DrawString`, so the
destructor moves after the call and the whole function's register allocation
changes (`&labelCopy2` in `r9`; aligned 2,188).  The original instead destroys the
calcDistance temporary **before** `DrawString` (`12e9b4 blx StringD1Ev` then
`12e9e0 blx DrawString` with `r2 = r6 = &this->lockLabel`).

Putting the copy in its own scope makes clang emit exactly that shape:

```cpp
KIPlayer *landmark = this->landmarks->data()[i];
{
    String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                           this->cameraX, this->cameraY, this->cameraZ);
    this->lockLabel = labelCopy2;
}
Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                            this->screenY + rowHeight, false);
```

The copy ctor is elided (direct prvalue initialisation), the destructor is emitted
at the closing brace, and `DrawString` consumes the member.  Our else-arm
instruction sequence is then identical to the original's (modulo the r5/fp temp
register and the sp offsets), and the LSDA keeps **19 rows** — and now the record
is **132 B with call-site length 112**, i.e. the same size as the original's
record (the HEAD shape was 131 B / 111).

The shape needs no pad hack (`.ARM.extab` stays 0x26298, `.rodata` stays
0x1c9e10).

## 2. Size companions: the scoped copy is -24 B, so three type toggles land 9,908

The scoped copy alone is 9,884 B.  A sweep of the function's bool/int locals on
this base found only two 9,908 B combinations; the best on both metrics is

```diff
-    int landmarkHit = false;
+    bool landmarkHit = false;
...
-            int marked = false;
+            bool marked = false;
...
-    bool enemyHit = false;
+    int enemyHit = false;
...
-                bool wreck = asteroid->hasCargo && (...);
+                int wreck = asteroid->hasCargo && (...);
```

(`bool planetHit` is unchanged; `int planetHit` rotates the prologue and jumps
aligned to ~2,000.)

## 3. Variant matrix (all measured on the wave-152 HEAD base)

| else-arm shape | usize | raw | aligned | LSDA rows / reclen / cst_len |
|---|---:|---:|---:|---|
| HEAD (`lockLabel = calcDistance(...)`) | 9908 | 8620 | 1530 | 19 / 131 / 111 |
| w9802 (`String local; store; DrawString(local)`) | 9908 | 8588 | 2188 | 19 / 131 / 111 |
| `String local; DrawString(local)` (no store) | 9896 | 8803 | 2206 | 19 / 131 / 111 |
| `store; const String& r = lockLabel; DrawString(r)` | 9920 | 8957 | 2149 | 19 / 131 / 111 |
| `String local; store; DrawString(member)` (no scope) | 9884 | 8634 | 1530 | 19 / 131 / 111 |
| **scoped copy + `bool landmarkHit`/`bool marked`/`int enemyHit`/`int wreck` (landed)** | **9908** | **8577** | **1484** | **19 / 132 / 112** |

The scoped copy alone (9,884 B) is byte-for-byte the HEAD else-arm codegen; the
four type toggles are what move the size to 9,908 and the metrics to 8,577 / 1,484.
Other 9,908 B hits on this base: `wreck_ast+enemyHit+alwaysEnemy` (8589 / 1522),
`wreck_ast+enemyHit+alienOrbit` (8600 / 1543), `wreck_ast+asteroidHit`
(8601 / 1631).

## 4. LSDA row tables (reported as requested)

Original record @ 0x1c203c, **132 B / 19 rows** (11 LP!=0):

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

Landed record @ 0x1c203c, **132 B / 19 rows** (11 LP!=0):

```
(0,568,0) (568,8,9810) (576,1776,0) (2352,12,9854) (2364,136,0)
(2500,10,9850) (2514,16,9856) (2530,14,9840) (2544,12,9830) (2568,20,9820)
(2588,78,0) (2666,10,9812) (2676,1294,0) (3970,12,9874) (3982,1768,0)
(5750,8,9868) (5758,872,0) (6630,8,9808) (6638,3270,0)
```

The record size and call-site table length now match the original exactly; only
the row offsets differ (the body is still 1,484 instructions off).

## 5. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

All 27 section sizes and VMAs are identical to the original (`.text` 0x1337c8,
`.ARM.exidx` 0x19cc08, `.ARM.extab` 0x26298, `.rodata` 0x1c9e10).
`.ARM.exidx` / `.rodata` / `.data` / `.bss` / `.rel.plt` are byte-identical.

* raw same-address **8,620 -> 8,577** (-43)
* instruction-index alignment **1,530 -> 1,484** (-46)
* `.text` differing bytes **87,131 -> 87,088** (-43)
* `.ARM.extab` differing bytes **1,324 -> 1,286** (-38)

## 6. Open questions

1. `bool landmarkHit` is the biggest single lever (`int -> bool` alone is
   9,892 B / 7,988 / 1,377 on the scoped base).  A 9,908 B shape that keeps
   `int landmarkHit` and the scoped copy was not found in the 128-combination
   sweep — every +16 B companion tried was a prologue rotation.
2. `enemyHit` / `wreck` are witnessed `bool` in w9308/w9401; this shape widens
   them to `int` for size.  If a witnessed +16 B body addition is found, the
   faithful `bool` set plus `bool landmarkHit` (9,892 B, 7,988 / 1,377) is the
   better landing.
3. The scoped copy's else-arm codegen is identical to HEAD's; the remaining
   1,484 aligned mismatches are outside the landmark else arm (planet loop,
   enemy loop, gas/asteroid tail).
