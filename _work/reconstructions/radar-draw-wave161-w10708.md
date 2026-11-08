# Radar::draw wave 161 (w10708) — faithful tail + `bool alwaysEnemySeen`/`bool alwaysEnemy`

**Status:** source change landed, 2026-11-08. Worktree `wt/w10708`, based on `master`
(`01c7f92d6`, wave 160). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave160-w10608.md`, `radar-draw-wave159-w10505.md`,
`wave160-merge-tests.md`.

## Baseline (wave-160 HEAD)

Original 9,908 B; ours 9,908 B; raw same-address diff **8,124**; instruction-index
alignment **1,320**; `.text` differing bytes 85,518; `.ARM.extab` 1,258;
`.rel.plt` byte-identical; all 27 section sizes exact; file size 2,187,328 B;
gate byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

The wave-160 HEAD carried the wave-159 filler tail
(`field_0x1bc = 0; plasmaInRange = 0; getCurrentCampaignMission(); ...`) and the
single shared `stationLockTimer = 0` store.

## 1. The lock/landmark block

The original's landmark-lock `else` stores `stationLockTimer` in **both** arms
(confirmed again from the binary, `12ebb0`-`12ebbe` and `12fe60`-`12fe68`):

```
12ebb0  ldr  r0,[r4,#36]      ; lockedStation
12ebb2  cmp  r0,#0
12ebb4  beq.w 12fe60
12ebb8  movs r0,#0
12ebba  str.w r0,[r4,#420]    ; stationLockTimer = 0   (non-null arm)
12ebbe  b.w  12e376
12fe60  movs r0,#0
12fe62  str.w r0,[r4,#420]    ; stationLockTimer = 0   (null arm)
12fe66  str  r0,[r4,#40]      ; field_0x28 = 0
12fe68  b.w  12e376
```

The landed source is the explicit duplicate-store form already validated by
w10608; it reproduces the cold/hot split instruction-for-instruction.

## 2. The faithful tail is 4 B short; the missing 2+2 is the bool type

With the duplicate store and the faithful tail (only `field_0x1bc = 0;`, no
`plasmaInRange` store, no getter calls) the function is **9,904 B** (4 short).
The 4 bytes are the two effects of `alwaysEnemySeen` being a `bool` in the
original:

* the assignment `alwaysEnemySeen = alwaysEnemy` emits the `and.w r1,r6,#1`
  normalization (`+4 B` at the assignment), and
* the sound-block guard lowers as the original's `eors.w` form (`+2 B`),
  offset by the literal-pool alignment nop disappearing (`-2 B`).

Net **+4 B** -> 9,908 B.  The original sound block is

```
130298  cmp  r1,#151
13029a  beq.n 130326
13029c  ldr  r2,[sp,#104]
13029e  eors.w r2,r2,#1
1302a2  bne.n 130326
```

and the landed build reproduces that shape (A variant below reproduces it
exactly; the landed B variant adds `lsls r2,r2,#31` because `alwaysEnemy` is
also bool).

`alwaysEnemy` is a boolean flag (`int alwaysEnemy = false;` in the old source)
and is made `bool` too, which is the human-style type and improves the raw
metric.

## 3. Landed change

```diff
-    int alwaysEnemySeen = false;
+    bool alwaysEnemySeen = false;
...
             } else {
                 if (this->lockedStation == nullptr) {
                     this->field_0x28 = 0;
+                    this->stationLockTimer = 0;
+                } else {
+                    this->stationLockTimer = 0;
                 }
-                this->stationLockTimer = 0;
             }
...
-        int alwaysEnemy = false;
+        bool alwaysEnemy = false;
...
     this->field_0x1bc = 0;
-    this->plasmaInRange = 0;
-    Globals::status->getCurrentCampaignMission();
-    Globals::status->getCurrentCampaignMission();
 }
```

## 4. Measured matrix (all size 9,908 B unless noted)

| variant | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| wave-160 HEAD (no dup store) | 8,124 | 1,320 | 85,518 | 1,258 |
| w10608 dup store + `plasmaInRange` tail | 8,536 | 1,314 | 85,930 | 1,256 |
| faithful tail only (int types) | 8,527 | 1,312 | — | — (9,904 B) |
| **landed: `bool alwaysEnemySeen` + `bool alwaysEnemy` + faithful tail** | **8,367** | **1,333** | **85,761** | **1,259** |
| `bool alwaysEnemySeen` only + faithful tail (exact `eors.w` shape) | 8,633 | 1,337 | 86,027 | 1,257 |
| `bool alienOrbit` + faithful tail | 8,599 | 1,307 | — | — |
| `bool alwaysEnemySeen` + `bool alwaysEnemy` + `plasmaInRange` tail | 9,912 B | — | — | — |

The landed variant is the best raw among the size-exact faithful-tail shapes
(-169 vs w10608, -266 vs the `alwaysEnemySeen`-only shape).  The aligned metric
is 19 worse than w10608 and 13 worse than the baseline; the register/stack
permutation (`campaign` on the stack instead of `sl`, `alwaysEnemySeen` at
`[sp,#116]` instead of the original `[sp,#104]`) is still the dominant residual
and is unchanged by this pass.

## 5. Gate

`check` green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, parity 5370/5370.  All 27 section sizes exact; file size 2,187,328 B;
`.rel.plt` byte-identical.  `.text` 85,761 (+243 vs wave-160 HEAD),
`.ARM.extab` 1,259 (+1), `.note.gnu.build-id` 20.
