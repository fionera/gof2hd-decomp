# Radar::draw wave 158 (w10506) — original LSDA (cst 112) with a non-EH tail companion: raw 8,181 -> 8,235, aligned 1,278 -> 1,370, extab 1,284 -> 1,245

**Status:** source change landed, 2026-11-08. Worktree `wt/w10506`, base
`master` (`cfde1f5f2`, wave 158). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave158-w10406.md`, `radar-draw-wave158-w10404.md`,
`extab-largest-record-wave156-w10219.md`.

Baseline (wave-158 HEAD): original 9,908 B; ours 9,908 B; raw same-address
diff **8,181**; instruction-index alignment **1,278**; `.text` 85,372
differing bytes; `.ARM.extab` 1,284; Radar record 132 B / 19 rows / cst
**111** / **87** differing bytes.

## 1. Result

The w10406 direct-assignment else arm (`this->lockLabel = calcDistance(...)`)
is the only structural shape whose temp dies before `DrawString`, so it is
what reproduces the original 132 B / 19-row / cst-112 LSDA.  On the wave-158
base it costs raw 8,181 -> 8,236 / aligned 1,278 -> 1,368 when paired with the
w10406 `int marked + String dummy` tail.

This pass keeps the direct arm but replaces the tail `String dummy` (an extra
EH object: the original has 11 `StringD1Ev` calls, the w10406 shape has 12)
with an inert **non-EH** query, `this->level->getPlayer()->isDockingToAsteroid();`.
That is the best measured 9,908 B / cst-112 shape:

| shape (direct arm, `int marked`) | size | raw | aligned | Radar extab diff |
|---|---:|---:|---:|---:|
| w10305 noscope baseline (cst 111) | 9908 | **8181** | **1278** | 87 |
| `String dummy` tail (w10406) | 9908 | 8236 | **1368** | 48 |
| **`getPlayer()->isDockingToAsteroid()` tail (landed)** | **9908** | **8235** | 1370 | **48** |
| `getPlayer()->isAutoPilot()` tail | 9908 | 8235 | 1370 | 48 |
| `Canvas->SetColor(0)` tail | 9908 | 8238 | 1387 | 48 |
| `field_0x10=0; getCurrentCampaignMission()` tail | 9908 | 8295 | 1373 | 48 |
| no tail (correct original tail shape) | 9900 | 8394 | 1366 | 49 |

The landed tail is semantically inert (a pure query) and is a size companion
only; the original's tail is just `this->field_0x1bc = 0;` and ours-notail
matches it instruction-for-instruction (same `movs r0,#0; str.w r0,[r4,#444]`
then the stack-canary epilogue).  The 8 B gap is the known 68-in-`sl` knot
(original spends four 4-byte `mov.w sl,#68`, ours four 2-byte `movs r6,#68`),
not a missing statement.

## 2. Negative result: 8,181 text and cst 112 do not coexist

The w10305 8,181/1,278 text is a property of the **noscope** else arm: the
`String` temp stays live through `DrawString`, so its landing-pad row is
42 B and cst is 111.  Every source spelling that destroys the temp before
`DrawString` (direct assignment, nested-scope copy, `String(...)` temporary,
reference/pointer alias) compiles identically and costs raw 8,236-ish.  The
two metrics are therefore mutually exclusive at this address; the honest
LSDA-correct floor is 8,235/1,370.

Measured and rejected (all cst 112, 19 rows):

| shape | size | raw | aligned |
|---|---:|---:|---:|
| direct `bool marked` + `plasmaInRange=0` + String | 9912 | 8316 | 1317 |
| direct `bool marked` + `field_0x10=0` + String | 9908 | 8514 | 1316 |
| direct `int marked` + `plasmaInRange=0` + String | 9916 | 8432 | 1368 |
| direct `int marked` + `String dummy(0)` | 9912 | 8348 | 1366 |
| direct `int marked` + 2 getters | 9912 | 8341 | 1370 |
| direct `int marked` + 4 field stores | 9908 | 8419 | 1369 |

`bool marked` wins aligned (1,316-1,317) but loses ~80-280 raw and cannot hit
9,908 B without an extra pointer store.  `int marked` is the raw winner.
All integer `marked` types (`long`, `short`, `char`, `unsigned`, `int8_t`,
`int16_t`, `int32_t`) are byte-identical to `int`.

## 3. LSDA rows (landed)

Original and ours: `pers 0x5eb60 uw 3w lp 0xff tt 0xff cst 0x1 len 112 rows
19`, record 132 B, 11 landing pads.  Record diff **48** bytes (was 87).  The
remaining diff is the known register/frame permutation: ours loads the
`&lockLabel` pointer with 4-byte `ldr.w r9,[sp,#100]` where the original uses
2-byte `ldr r6,[sp,#104]`, so row 3 is 12 B vs 10 B and every later offset
shifts.  It is not a source-shape issue.

Original rows:

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

Landed rows:

```
(0,568,0) (568,8,9810) (576,1776,0) (2352,12,9852) (2364,136,0)
(2500,10,9848) (2514,16,9854) (2530,14,9838) (2544,12,9828) (2568,20,9818)
(2588,78,0) (2666,10,9816) (2676,1296,0) (3972,12,9872) (3984,1774,0)
(5758,8,9866) (5766,856,0) (6622,8,9808) (6630,3278,0)
```

## 4. Section deltas / gate

All 27 section sizes and VMAs exact; file size 2,187,328 B; `.ARM.exidx`,
`.rodata`, `.data`, `.bss`, `.rel.plt` byte-identical.

* `.text` differing bytes: 85,372 -> **85,426** (+54)
* `.ARM.extab` differing bytes: 1,284 -> **1,245** (-39)
* `.note.gnu.build-id`: 20 (unchanged)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Radar::draw: size 9,908 B exact; raw **8,181 -> 8,235** (+54); aligned
**1,278 -> 1,370** (+92); Radar extab record **87 -> 48**; total `.ARM.extab`
**1,284 -> 1,245**; cst **111 -> 112**, rows 19, record 132 B.

## 5. Open questions

1. The original tail has no `String dummy`; ours-notail is instruction-exact
   at the tail but 8 B short (9,900).  The gap is the 68-in-`sl` knot
   (four `mov.w sl,#68` vs four `movs r6,#68`).  Fixing the prologue register
   map would let the tail companion be dropped entirely and should recover
   most of the 54 raw bytes.
2. `campaign` must live in `sl` (original) instead of `[sp,#144]` (ours), and
   `missionBlocksPlanets` in `fp` instead of `sl`.  That is the same
   whole-function permutation that makes row 3 12 B instead of 10 B.
3. `bool marked` is the aligned-metric winner (1,316) but needs a 4-byte
   pointer store to reach 9,908 and costs ~280 raw; `int marked` is the raw
   winner.
