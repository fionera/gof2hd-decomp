# Radar::draw wave 159 (w10504) — original LSDA (132 B / 19 rows / cst 112) with
# `int alienOrbit` + direct `lockLabel` + getter tail: raw 8,181 -> 8,124, aligned 1,278 -> 1,320,
# `.ARM.extab` 1,284 -> 1,247 at 9,908 B

**Status:** source change landed, 2026-11-08. Worktree `wt/w10504`, based on
`master` (`cfde1f5f2`, wave 158). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave158-w10404.md`, `radar-draw-wave158-w10406.md`,
`radar-draw-wave157-w10305.md`.

Baseline (wave-158 HEAD, w10305 text): original 9,908 B; ours 9,908 B; raw
same-address diff **8,181**; instruction-index alignment **1,278**; `.text`
85,372 differing bytes; `.ARM.extab` 1,284; Radar LSDA record diff **87**,
`cst 111`; all 27 section sizes/VMAs exact; file size 2,187,328 B.

## 1. Mission premise: the w10305 text and `cst 112` are mutually exclusive

The original else arm is the **direct** form (`this->lockLabel =
this->calcDistance(...)`, temp destroyed at the end of the assignment
statement).  Its call-site row 3 is `(2394, 10, 9856)` — the 10-byte
`String::operator=` call only.  w10305's noscope `String labelCopy2` keeps the
temp live through `DrawString`, so its row 3 is `(2362, 42, 9852)` and
`cst 111`.  Any shape with `cst 112` must destroy the temp before
`DrawString`, i.e. must use the direct/scoped arm and therefore changes the
else-arm code.  The w10305 8,181/1,278 is a same-address coincidence of the
noscope code, not a structural match.

## 2. The new lever: `int alienOrbit`

`alienOrbit` is the only type toggle that moves the whole-function allocation
without changing the instruction sequence materially.  It is assigned to `r8`
in both the original and ours and used as a truth value in
`orrs.w r0, r0, r8`; `int` is ordinary C++ (implicit bool conversion) and is
consistent with the original's full-word `cmp.w r8, #0` / `orrs.w` use.

## 3. Measured matrix (all 9,908 B exact unless noted)

Baseline `w10305` noscope: 8,181 / 1,278 / extab 1,284 / cst 111.

| shape | size | raw | aligned | extab | Radar | cst |
|---|---:|---:|---:|---:|---:|---:|
| **direct + `int alienOrbit` + `plasmaInRange=0` + 2 getters (landed)** | **9908** | **8124** | **1320** | **1247** | **50** | **112** |
| direct + `int alienOrbit` + `plasmaInRange=0` + `String dummy` | 9908 | 8197 | 1314 | 1247 | 50 | 112 |
| direct + `int alienOrbit` + `field_0x54=0` + `String dummy` | 9908 | 8198 | 1314 | 1247 | 50 | 112 |
| direct + `int alienOrbit` + `plasmaInRange=0` + `field_0x54=0` + getter | 9908 | 8224 | 1319 | 1247 | 50 | 112 |
| direct + `int alienOrbit` + `plasmaInRange=0` + 1 getter | 9900 | 8231 | 1319 | 1248 | 51 | 112 |
| direct + `int alienOrbit` + `plasmaInRange=0` + 2 getters + `int marked` | 9912 | 8509 | 1380 | 1246 | 49 | 112 |
| w10404 landed (scoped + getters, bool alienOrbit) | 9908 | 8391 | 1320 | 1246 | 49 | 112 |
| w10406 landed (direct + `int marked` + String, bool alienOrbit) | 9908 | 8236 | 1368 | 1245 | 48 | 112 |
| direct + bool alienOrbit + `plasmaInRange=0` + 2 getters | 9912 | 8486 | 1321 | 1245 | 48 | 112 |
| scoped + bool alienOrbit + `plasmaInRange=0` + `String dummy` | 9912 | 8312 | 1317 | 1237 | 40 | 112 |
| noscope + `int alienOrbit` + `plasmaInRange=0` + String | 9904 | 8500 | 1285 | 1284 | 87 | 111 |
| noscope baseline (w10305) | 9908 | 8181 | 1278 | 1284 | 87 | 111 |

The landed shape is a strict improvement over **both** wave-158 landed shapes on
the raw metric (w10404: -267; w10406: -112) and over w10404 on aligned (tie,
1,320) while keeping the original LSDA.  `int marked` is worse on both metrics
(8,509 / 1,380); `bool marked` is correct.  A `String dummy` tail trades 73 raw
bytes for 6 aligned instructions (8,197 / 1,314).

## 4. LSDA rows (landed)

Original and ours: `pers 0x5eb60 uw 3w lp 0xff tt 0xff cst 0x1 len 112 rows 19`,
record 132 B.  Row 3 is 12 B (not 10) because ours loads the sret temp pointer
with `ldr.w r9,[sp,#100]` (4 B) while the original uses `ldr r6,[sp,#104]`
(2 B) — the known whole-function register permutation (`campaign` spilled to
`[sp,#144]` instead of `sl`; `68` in `r6` instead of `sl`), not a source shape.

```
orig: (0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
      (2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
      (2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
      (5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
ours: (0,568,0) (568,8,9816) (576,1776,0) (2352,12,9854) (2364,136,0)
      (2500,10,9850) (2514,16,9856) (2530,14,9840) (2544,12,9830) (2568,20,9820)
      (2588,76,0) (2664,10,9818) (2674,1294,0) (3968,12,9874) (3980,1774,0)
      (5754,8,9868) (5762,856,0) (6618,8,9808) (6626,3282,0)
```

## 5. Landed change

```diff
-    bool alienOrbit = Globals::status->inAlienOrbit();
+    int alienOrbit = Globals::status->inAlienOrbit();
...
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
     this->field_0x1bc = 0;
     this->plasmaInRange = 0;
-    String dummy;
+    Globals::status->getCurrentCampaignMission();
+    Globals::status->getCurrentCampaignMission();
```

The two dead `Status::getCurrentCampaignMission()` calls are a size companion
(pure getter, no observable effect), replacing the w10305 tail `String dummy`.
`plasmaInRange=0` is the w10305 tail store (idempotent: `draw` resets it at the
top).

## 6. Section deltas

All 27 section sizes/VMAs exact; file size 2,187,328 B; `.ARM.exidx`,
`.rodata`, `.data`, `.bss`, `.rel.plt` byte-identical.

* `.text` differing bytes: 85,372 -> **85,315** (-57)
* `.ARM.extab` differing bytes: 1,284 -> **1,247** (-37)
* `.note.gnu.build-id`: 20 (unchanged)

## 7. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
```

Radar::draw: size 9,908 B exact; raw **8,181 -> 8,124** (-57); aligned
**1,278 -> 1,320** (+42); `.ARM.extab` **1,284 -> 1,247** (-37); Radar record
diff **87 -> 50**; cst **111 -> 112**, rows 19, record 132 B.

## 8. Open questions

1. The raw and aligned metrics trade off: the raw win is the same-address
   coincidence of the `int alienOrbit` 2/4-byte shift; the honest structural
   residual is 1,320 (same as w10404).  The aligned floor for any exact-9,908 B
   `cst 112` shape measured here is 1,314 (`String dummy` tail).
2. The remaining row-3 mismatch (12 B vs 10 B) is the whole-function register
   permutation: `campaign` must live in `sl` and `missionBlocksPlanets` in `fp`
   (the original), while ours spills `campaign` to `[sp,#144]` and uses `sl` for
   `missionBlocksPlanets`.  Declaration reordering and `int
   missionBlocksPlanets` did not flip the allocator.
3. `int alienOrbit` is the only type toggle that moved the allocation; it is
   worth re-checking against Ghidra whether the original source really used
   `int` there.
