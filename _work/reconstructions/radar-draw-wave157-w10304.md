# Radar::draw wave 157 (w10304) — exact 9,908 B w10204+w10206 combination:
# raw 8,320 -> 8,437, aligned 1,296 -> 1,337, `.ARM.extab` 1,292 -> 1,254

**Status:** source change landed, 2026-11-08. Worktree `wt/w10304`, based on
`master` (`08575efff`, wave 156). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave156-w10204.md`, `radar-draw-wave156-w10206.md`,
`radar-draw-wave155-w10104.md`, `radar-draw-wave152-w9902.md`.

Baseline (HEAD wave 156, w10204 landed): original 9,908 B; ours 9,908 B; raw
same-address diff **8,320**; instruction-index alignment **1,296**; `.text`
85,831 differing bytes; `.ARM.extab` 1,292; all 27 section sizes/VMAs exact;
file size 2,187,328 B.

## 1. The answer: take w10206's scoped else arm, keep w10204's tractor swap

The two waves are not stackable as-is (9,912/9,916 B).  The exact 9,908 B
combination is obtained by **deduplicating the tail companion**: w10204's
`String dummy;` (+12) is replaced by w10206's tail getter (+8), and `int marked`
stays `bool marked` (the w10204 base already has `bool marked`).  Both orders
converge to the same source:

- w10204 + scoped else arm, String -> getter  = 9,908
- w10206 + tractorAuto store swap, int -> bool marked = 9,908

Landed diff (only two hunks):

```diff
                     } else {
                         KIPlayer *landmark = this->landmarks->data()[i];
-                        String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
-                                                               this->cameraX, this->cameraY,
-                                                               this->cameraZ);
-                        this->lockLabel = labelCopy2;
-                        Globals::Canvas->DrawString(radarFont(), labelCopy2, this->screenX + offset,
+                        {
+                            String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
+                                                                   this->cameraX, this->cameraY,
+                                                                   this->cameraZ);
+                            this->lockLabel = labelCopy2;
+                        }
+                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
                                                     this->screenY + rowHeight, false);
...
     this->field_0x1bc = 0;
-    String dummy;
+    Globals::status->getCurrentCampaignMission();
```

The scoped copy is the original's witnessed EH shape (`0x12e9a6 blx
calcDistance`, `0x12e9b0 blx String::operator=`, `0x12e9b6 blx StringD1Ev`,
then `0x12e9e0 blx DrawString` with `r2 = &lockLabel`).  The noscope shape
(w10204) keeps the temp live through `DrawString`, so LSDA row 3 is
`(2362,42,9850)` instead of the original `(2394,10,9856)` and `.ARM.extab`
stays at 1,292.  The scoped shape restores the original's 10-byte cleanup
range and drops `.ARM.extab` to **1,254**.

## 2. Measured matrix (all 9,908 B unless noted)

| shape | raw | aligned | extab | note |
|---|---:|---:|---:|---|
| HEAD wave 156 (w10204: noscope + String) | 8,320 | 1,296 | 1,292 | text win, wrong EH |
| w10204 + scoped else arm (String) | 8,523 | 1,333 | 1,254 | 9,912 B |
| w10206 landed (scoped + int marked + getter) | 8,449 | 1,392 | 1,254 | no tractor swap |
| **landed (scoped + tractor swap + bool marked + getter)** | **8,437** | **1,337** | **1,254** | **exact 9,908** |
| scoped + tractor + bool marked, no tail | 8,542 | 1,330 | 1,255 | 9,900 B |
| scoped + tractor + int marked + getter | 8,471 | 1,393 | 1,252 | 9,912 B |
| scoped + tractor + bool marked + String | 8,523 | 1,333 | 1,254 | 9,912 B |
| noscope + getter + bool marked | 8,282 | 1,297 | 1,292 | 9,904 B |
| scoped + tractor + `bool wreck` (enemy) + `int asteroidHit` | 8,302 | 1,591 | 1,254 | raw win, aligned loss |
| scoped + tractor + `bool wreck` (enemy) + `int landmarkHit` | 8,565 | 1,499 | 1,254 | |

The scoped else arm costs 203 raw / 37 aligned versus the noscope shape, but it
is the only shape that reproduces the original's EH record.  The landed variant
is the best aligned of all measured 9,908 B / extab-1,254 combinations (the
`wreck` toggles reach lower raw but rotate the whole enemies-loop register
allocation and lose 250+ aligned instructions).

## 3. LSDA rows

Original and landed record both start `24cbe97f c9539702 f0800085 b0b0b0ab`
(132 B / 19 rows / call-site length 112, 11 landing pads != 0); only the row
offsets differ.  Original rows:

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

Landed rows (same record size / cst as w10206's landed shape; only offsets
differ):

```
(0,568,0) (568,8,9810) (576,1780,0) (2356,12,9854) (2368,136,0)
(2504,10,9850) (2518,16,9856) (2534,14,9840) (2548,12,9830) (2572,20,9820)
(2592,78,0) (2670,10,9812) (2680,1294,0) (3974,12,9874) (3986,1772,0)
(5758,8,9868) (5766,856,0) (6622,8,9808) (6630,3278,0)
```

## 4. Section deltas

All 27 section sizes and VMAs exact; file size 2,187,328 B; `.ARM.exidx`,
`.rodata`, `.data`, `.bss`, `.rel.plt` byte-identical.

* `.text` differing bytes: 85,831 -> **85,948** (+117)
* `.ARM.extab` differing bytes: 1,292 -> **1,254** (-38)
* `.note.gnu.build-id`: 20 (unchanged)

## 5. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,320 -> 8,437** (+117); aligned
**1,296 -> 1,337** (+41); `.ARM.extab` **1,292 -> 1,254** (-38).

## 6. Open questions

1. The raw/aligned regression versus w10204 is entirely the scoped else arm.
   The original is scoped, so the noscope 8,320/1,296 is a same-address
   coincidence; the honest structural residual is 1,337.
2. The whole-function register permutation (`campaign` spilled to `[sp,#144]`
   instead of living in `sl`, loop index `i` in `sl` instead of `fp`) is the
   dominant remaining mismatch.  `const`, `unsigned`, `register`, declaration
   moves and `int alwaysEnemySeen` were all measured and do not flip it.
3. The original has **no** tail companion; the no-tail scoped shape is 9,900 B,
   i.e. 8 B of real body are still missing.  The tail getter is a size
   companion, not a witnessed statement.
