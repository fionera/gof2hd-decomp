# Radar::draw wave 158 (w10406) — original LSDA shape (132 B / 19 rows / cst 112)
# with the w10305 int-local text: raw 8,181 -> 8,236, aligned 1,278 -> 1,368, extab 1,292 -> 1,253

**Status:** source change landed, 2026-11-08. Worktree `wt/w10406`, based on
`master` (`9e4795206`, wave 157). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave157-w10304.md`, `radar-draw-wave157-w10305.md`,
`extab-largest-record-wave156-w10219.md`.

Baseline (wave-157 HEAD, w10305 landed): original 9,908 B; ours 9,908 B; raw
same-address diff **8,181**; instruction-index alignment **1,278**; `.text`
85,529 differing bytes; `.ARM.extab` 1,292; Radar record 132 B / 19 rows /
cst **111** / **87** differing bytes; all 27 section sizes/VMAs exact; file
size 2,187,328 B.

## 1. The result

The original else-arm is a **direct member assignment**:

```cpp
this->lockLabel = this->calcDistance(landmark->posX, ..., this->cameraZ);
Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                            this->screenY + rowHeight, false);
```

The temporary returned by `calcDistance` is destroyed at the end of the
assignment statement, so the LSDA call-site row is the 10-byte
`operator=` region, exactly as witnessed in the original
(`0x12e9a6 blx calcDistance`, `0x12e9b0 blx String::operator=`,
`0x12e9b6 blx StringD1Ev`, then `0x12e9e0 blx DrawString` with
`r2 = &lockLabel`).  The w10305 noscope local keeps the temp live through
`DrawString`, giving a 42-byte row and cst 111.

Landed diff (three hunks):

```diff
-                        String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
-                                                               this->cameraX, this->cameraY,
-                                                               this->cameraZ);
-                        this->lockLabel = labelCopy2;
-                        Globals::Canvas->DrawString(radarFont(), labelCopy2, this->screenX + offset,
+                        this->lockLabel = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
+                                                             this->cameraX, this->cameraY, this->cameraZ);
+                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                                                     this->screenY + rowHeight, false);
...
-            bool marked = false;
+            int marked = false;
...
-    this->plasmaInRange = 0;
     String dummy;
```

`int marked` + dropping the `plasmaInRange` tail store is the 9,908 B size
companion (the original tail is only `this->field_0x1bc = 0;`; the `String
dummy` is still needed for size).  The `int marked` choice is the only
9,908 B / cst-112 shape with raw under 8,300.

## 2. LSDA rows (landed)

Original and ours both: `24cbe97f c9539702 f0800085 b0b0b0ab` prefix, 132 B,
19 rows, call-site length **112**, 11 landing pads.  Record diff **48** bytes
(was 87).  Original rows:

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

Landed rows:

```
(0,568,0) (568,8,9812) (576,1780,0) (2356,12,9854) (2368,136,0)
(2504,10,9850) (2518,16,9856) (2534,14,9840) (2548,12,9830) (2572,20,9820)
(2592,78,0) (2670,10,9814) (2680,1296,0) (3976,12,9874) (3988,1774,0)
(5762,8,9868) (5770,856,0) (6626,8,9810) (6634,3274,0)
```

Row 3 is 12 B (not 10) because ours loads the sret temp pointer into `r9`
(`ldr.w r9,[sp,#100]`, 4 B) while the original uses `r6`
(`ldr r6,[sp,#104]`, 2 B).  This is the known whole-function register
permutation (campaign spilled to `[sp,#144]` instead of `sl`), not a source
shape; every else-arm spelling below compiles identically.

## 3. Measured matrix (all on the w10305 base, `bool`/`int` marked as noted)

| shape | size | raw | aligned | Radar extab diff |
|---|---:|---:|---:|---:|
| **w10305 baseline (noscope, bool marked, plasma+String tail)** | **9908** | **8181** | **1278** | **87** |
| noscope, int marked, plasma+String | 9908 | 8211 | 1356 | 87 |
| direct/scoped, bool marked, plasma+String | 9912 | 8312 | 1317 | 40 |
| direct/scoped, int marked, plasma+String | 9916 | 8424 | 1368 | 49 |
| direct/scoped, bool marked, String(0) tail | 9908 | 8514 | 1316 | 49 |
| **direct/scoped, int marked, String tail (landed)** | **9908** | **8236** | **1368** | **48** |
| direct/scoped, bool marked, String tail | 9904 | 8492 | 1316 | 50 |
| direct/scoped, bool marked, no tail | 9896 | 8502 | 1314 | 50 |

`scoped` (inner `{ String labelCopy2 = ...; lockLabel = labelCopy2; }`) and
`direct` (`lockLabel = calcDistance(...)`) are byte-identical; `landmark`
local vs inlined access, `(*landmarks)[i]`, `const KIPlayer*` are all
identical too.  Swapping the if/else branch order (`i != 0 || alienOrbit`)
costs 4 raw / 2 aligned and does not flip any LSDA row.

## 4. Section deltas

All 27 section sizes and VMAs exact; file size 2,187,328 B; `.ARM.exidx`,
`.rodata`, `.data`, `.bss`, `.rel.plt` byte-identical.

* `.text` differing bytes: 85,529 -> **85,584** (+55)
* `.ARM.extab` differing bytes: 1,292 -> **1,253** (-39)
* `.note.gnu.build-id`: 20 (unchanged)

## 5. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,181 -> 8,236** (+55); aligned
**1,278 -> 1,368** (+90); Radar extab record **87 -> 48**; total `.ARM.extab`
**1,292 -> 1,253** (-39); cst **111 -> 112**, rows 19, record 132 B.

## 6. Open questions

1. The original's else-arm sret temp is in `r6`; ours is in `r9`.  That is
   the only remaining row-length mismatch (row 3 is 12 B vs 10 B, row 11 is
   10 B vs 14 B — the two branches' temp registers are swapped).  Fixing it
   requires the whole-function register map (`campaign` -> `sl`, loop index
   -> `fp`), not a local source shape.
2. The w10305 `plasmaInRange = 0` tail store is not witnessed; the original
   tail is only `this->field_0x1bc = 0;`.  The landed shape drops it and uses
   `int marked` as the 9,908 B companion.  A witnessed +12 B body statement
   would let `bool marked` + no tail reach 9,908 with the same LSDA.
3. `int marked` is the raw-metric winner at 9,908 (8,236); `bool marked` +
   `String dummy(0)` is the aligned-metric winner (1,316) but costs 278 raw.
   The two metrics trade off; the landed choice prioritises raw.
