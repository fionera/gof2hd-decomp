# Radar::draw wave 157 (w10305) — `int alwaysEnemySeen` + `int alwaysEnemy` + tail store: raw 8,320 -> 8,181, aligned 1,296 -> 1,278 at 9,908 B

**Status:** source change landed, 2026-11-08. Worktree `wt/w10305`, based on
`master` (`08575efff`, wave 156). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave156-w10205.md`, `radar-draw-wave156-w10204.md`,
`radar-draw-wave155-w10105.md`.

Baseline (wave-156 HEAD, w10204 landed shape): original 9,908 B; ours 9,908 B;
raw same-address diff **8,320**; instruction-index alignment **1,296**; all 27
section sizes/VMAs exact; file size 2,187,328 B.

## 1. Mission premise re-measured on the landed base

The w10205 note's `int alwaysEnemy` = 9,904 / 8,299 / 1,438 was measured on the
*wave-155 HEAD* (which already had `int alwaysEnemySeen`, `int enemyHit`,
`bool marked`, `int wreck`, tail getter).  On the **w10204 landed shape**
(`bool alwaysEnemySeen`, `bool enemyHit`, tractorAuto store swap, asteroid store
reorder, tail `String dummy`) the same toggle is only:

| shape | size | raw | aligned |
|---|---:|---:|---:|
| landed baseline | 9908 | 8320 | 1296 |
| landed + `int alwaysEnemy` | 9904 | 8406 | 1310 |
| landed + `int alwaysEnemy` + `String dummy(0)` (4 B companion) | 9908 | 8424 | 1310 |
| landed + `int alwaysEnemy` + `plasmaInRange=0` (8 B, overshoots) | 9912 | 8213 | 1311 |

So the mission's literal combination does land 9,908 with `String(0)`, but it is
**worse than the landed baseline on both metrics**.  The real lever is the flag
type w10204 changed away from: `alwaysEnemySeen`.

## 2. `int alwaysEnemySeen` is the lever

The original stores `alwaysEnemy` as a **word** at `[sp,#80]`
(`12edee str r0,[sp,#80]`, `12edea movne r0,#1`) and coalesces
`alwaysEnemySeen` into the same slot (`12f3b6 ldr r1,[sp,#80];
12f3ba and r1,#1`).  w10204 changed `alwaysEnemySeen` from `int` (wave-155
HEAD) to `bool`; restoring `int` lets clang coalesce the two locals again.

Measured on the pristine w10204 landed base:

| shape | size | raw | aligned |
|---|---:|---:|---:|
| landed baseline (`bool` AES, `bool` AE) | 9908 | 8320 | 1296 |
| `int alwaysEnemySeen` only | 9908 | 8286 | 1303 |
| `int alwaysEnemySeen` + `int alwaysEnemy` | 9900 | **8178** | **1281** |
| `int alwaysEnemySeen` + `int alwaysEnemy` - tail String | 9892 | **7914** | **1275** |
| **+ `this->plasmaInRange = 0;` before the tail String (landed)** | **9908** | **8181** | **1278** |
| + `this->field_0x54 = 0;` before the tail String | 9908 | 8182 | 1278 |
| + `this->lockConfirmed = 0;` before the tail String | 9908 | 8182 | 1278 |
| + `this->field_0x1a8 = 0;` before the tail String | 9908 | 8181 | 1278 |
| + `this->field_0x11d/11e/11f = 0;` before the tail String | 9908 | 8181 | 1278 |

The 8 B companion is a real byte store (`movs r0,#0; strb.w r0,[r4,#304]`).
`plasmaInRange` is reset to 0 at the top of `draw`, so the tail store is
semantically idempotent for the next frame; it is not witnessed as a separate
original store (the original stores `[r4,#0x130]` only at `12e08c` and
`12f56a`), so it is a size companion in the same spirit as the w10204 tail
`String dummy`.  The tail `String dummy` is kept.

## 3. Full matrix (all on the w10204 landed base unless noted)

Single toggles on `landed + int alwaysEnemy` (base 9,904):

| toggle | size | raw | aligned |
|---|---:|---:|---:|
| `int marked` | 9908 | 8390 | 1364 |
| `int asteroidHit` | 9924 | 8630 | 1541 |
| `int planetHit` | 9944 | 8648 | 2165 |
| `int gasHit` | 9904 | 8406 | 1311 |
| `int landmarkHit` | 9904 | 8600 | 1632 |
| `int bossSeen` | 9868 | 8669 | 1327 |
| `int otherLockFlag` | 9892 | 9039 | 1446 |

On `landed + int alwaysEnemySeen + int alwaysEnemy` (base 9,900):

| toggle | size | raw | aligned |
|---|---:|---:|---:|
| `int marked` | 9904 | 8508 | 1334 |
| `int asteroidHit` | 9912 | 8361 | 1527 |
| `int planetHit` | 9936 | 8677 | 2149 |
| `int gasHit` | 9900 | 8178 | 1282 |
| `int landmarkHit` | 9896 | 8627 | 1605 |
| `int bossSeen` | 9856 | 8685 | 1287 |
| `int otherLockFlag` | 9888 | 9113 | 1415 |
| 1 tail getter (replaces String) | 9896 | 8228 | 1282 |
| 2 tail getters | 9904 | 8212 | 1280 |
| 3 tail getters | 9912 | 8281 | 1295 |
| no tail companion | 9892 | 7914 | 1275 |
| `String dummy(0)` instead of `String` | 9904 | 8240 | 1277 |
| String + 1 getter | 9912 | 8154 | 1283 |

The `int marked` / `int asteroidHit` / `int planetHit` / `int bossSeen` toggles
were re-tested here (w10105/w10204 open questions); none lands 9,908 with both
metrics under the baseline.

## 4. Register map (landed shape)

Prologue is unchanged from the w10204 landed shape: `missionBlocksPlanets` ->
`sl` (`12e07a mov.w sl,#0`), `campaign` spilled to `[sp,#144]`
(`12e0f8 str r0,[sp,#144]`), `otherLock` `[sp,#148]`.  The enemy loop still
rematerialises 68 into `r6` (`12ef08/12f11c/12f146/12f3a6 movs r6,#68`) while
the original keeps `68` in `sl` (`12ecd0/12eea2/12f128/12f196 mov.w sl,#68`);
the landmark loop still hoists 68 into `fp` (`12e77e/12e9f0/12eb02`), which is
the w10205 open question and is *not* fixed by this pass.

`alwaysEnemy` and `alwaysEnemySeen` now share the word slot `[sp,#80]` as in the
original (`12edee str r0,[sp,#80]`, `12f3b6 ldr r1,[sp,#80]; and #1`).

## 5. Landed change

```diff
-    bool alwaysEnemySeen = false;
+    int alwaysEnemySeen = false;
...
-        bool alwaysEnemy = false;
+        int alwaysEnemy = false;
...
     this->field_0x1bc = 0;
+    this->plasmaInRange = 0;
     String dummy;
```

* raw same-address **8,320 -> 8,181** (-139)
* instruction-index alignment **1,296 -> 1,278** (-18)
* size 9,908 B; all 27 section sizes/VMAs exact; file size 2,187,328 B.

## 6. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 7. Open questions

1. `int alwaysEnemySeen` + `int alwaysEnemy` without any tail companion is
   9892 / **7914** / **1275** (16 B short) - the lowest raw measured.  The
   original has 11 `String::~String` calls; ours without the tail String also
   has 11, so the tail String is definitely an extra EH pad.  A witnessed
   +16 B body statement would beat this pass outright.
2. The 16 B is largely the 68-in-`sl` knot: the original spends 16 B on four
   `mov.w sl,#68` (4 B each) while ours spends 8 B on four `movs r6,#68`
   (2 B each), and the original's `strd r5,sl,[sp]` (4 B) beats ours'
   `str r6,[sp]; movs r6,#68; str r6,[sp,#4]` (6 B).  Fixing the prologue map
   (campaign -> `sl`, `missionBlocksPlanets` -> `fp`) is still the top lever.
3. `int alwaysEnemy` on the w10204 landed shape is *not* a both-metric win
   (9,904 / 8,406 / 1,310); the w10205 8,299/1,438 figure belongs to the
   wave-155 HEAD and does not merge.
