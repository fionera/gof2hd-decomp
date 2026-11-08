# Radar::draw wave 152 (w9903) — enemy-loop `sl = 68`: `bool wreck` frees `sl` but the RA hands it to `i`

**Status:** source change landed, 2026-11-08. Worktree `wt/w9903`, based on `master`
(`4a6104a43`, wave 152). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave151-w9803.md`, `radar-draw-wave151-w9703.md`,
`radar-draw-wave149-w9606.md`, `radar-draw-wave147-w9309.md`.

Baseline (HEAD, wave 152): original 9908 B; ours 9908 B; raw same-address diff
**8,620**; instruction-index alignment **1,530**; all 27 section sizes exact; file
size exact; `.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical; `.rel.plt`
byte-identical.

The w9803 `S,M,A` shape (`int missionBlocksPlanets`, `int alwaysEnemy`,
`bossSeen = enemy->shipGroup == 10` at the inactive-enemy site) reproduces exactly:
**9,908 B, raw 8,587, aligned 1,565**.

## 1. The original's enemy-loop register map (from objdump)

```
r4 = this        r8 = enemy       fp = i
r5 = locked      r9 = &lockLabel sl = 68
r6 = bossSeen (spilled to [sp,#104] across getPosition)   wreck -> [sp,#152]
```

Ours (all shapes tried) is a permutation:

```
r4 = this        r8 = enemy       r5 = i
r6 = locked      r9 = bossSeen   sl = wreck
&lockLabel -> [sp,#76]   68 rematerialised into r6 at every DrawImage2D site
```

The original's `strd r6,sl,[sp]` / `strd r5,sl,[sp]` argument stores are the visible
symptom: `sl` holds the constant 68 for the whole loop.

## 2. New falsifications (all on the w9803 `S,M,A` base, 9,908 B unless noted)

| hypothesis | usize | raw | align | `sl,#68` |
|---|---:|---:|---:|---|
| base | 9908 | 8587 | 1565 | no |
| `bool wreck` (enemy loop) — **frees `sl`** | 9892 | 8631 | 1596 | **no** |
| `const int wreck` / `const bool wreck` | 9908/9892 | 8587/8631 | 1565/1596 | no |
| `const KIPlayer *enemy` / `const locked` | 9908 | 8587 | 1565 | no |
| `PlayerFighter *enemy` (+include) | 9908 | 8587 | 1565 | no |
| `int i` / `unsigned i` loop counter | 9908 | 8587 | 1565 | no |
| `wreck` hoisted outside loop | 9908 | 8587 | 1565 | no |
| `Player *enemyPlayer = enemy->player` local | 9908 | 8587 | 1565 | no |
| **`wreck` before `update(enemy)`** (loop order) | **9908** | **8580** | 1582 | no |
| `wreck` after `field_0x75 = 0` | 9904 | 8730 | 1571 | no |
| **`wreck` before `update` + `int gasHit` (landed)** | **9908** | **8579** | **1583** | no |
| `bool missionBlocksPlanets` | 9912 | 8600 | **1511** | no |
| `bool missionBlocksPlanets` + `bool marked` | 9908 | 8755 | **1463** | no |
| `bool missionBlocksPlanets` + `bool marked` + `bool wreck` | 9892 | 8523 | 1491 | no |
| `expect(enemies != nullptr, 1)` (w9309 E2) | 9792 | 9122 | 1724 | no (68 in r9) |

### The key new fact

`bool wreck` **does** free `sl` — but the allocator immediately gives `sl` to the
enemy-loop index `i` (and moves `&lockLabel` into `r5`). The original keeps `i` in
`fp` and `&lockLabel` in `r9`, so `sl` is the only free callee-saved register for
68. The divergence is therefore a **whole-function register permutation**, not a
local type choice: the original's `campaign` lives in `sl` from the prologue to
`12e38a` (`mov sl,r0` at `12e0f8`), while ours spills it to `[sp,#144]` at
`12e0f8`. Every later allocation cascades from that.

`bool missionBlocksPlanets` is the only single toggle that changes the prologue
rotation (it is the `fp = 0` vs `sl = 0` choice at `12e0d8`), and it is the only
shape that improves **both** metrics vs HEAD (8600/1511) — but it is 4 B over
(9,912). The only -4 B companion found is `bool marked`, which perturbs the RA
again and lands 9,908 / 8755 / 1463 (alignment win, raw loss).

## 3. Landed change

```diff
-    bool missionBlocksPlanets = false;
+    int missionBlocksPlanets = false;
...
-        bool alwaysEnemy = false;
+        int alwaysEnemy = false;
...
-                    bossSeen = true;
+                    bossSeen = enemy->shipGroup == 10;   // witnessed re-test
...
-            this->update(enemy);
             int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
+            this->update(enemy);
...
-        bool gasHit = false;
+        int gasHit = false;
```

* raw same-address **8,620 -> 8,579** (-41)
* instruction-index alignment **1,530 -> 1,583** (+53)
* size 9,908 B (frame 296); all 27 section sizes exact; file size exact.

This is the best **raw** 9,908 B shape found. The best **alignment** 9,908 B shape
found is `bool missionBlocksPlanets` + `bool marked` (9,908 / 8,755 / 1,463); a
future pass can pick whichever metric the coordinator prefers.

## 4. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

All 27 section sizes exact; file size exact.

## 5. Open questions

1. `campaign` must live in `sl` from `12e0f8` to `12e38a` (the original). Every
   shape tried spills it to `[sp,#144]` at `12e0f8`. If a source change makes
   `campaign` win a callee-saved register, the whole enemy-loop permutation should
   follow and `sl = 68` should appear.
2. `bool missionBlocksPlanets` (9,912 / 8,600 / 1,511) is the only both-metric win
   found; it needs a faithful -4 B companion other than `bool marked`.
3. `bool wreck` frees `sl` but `i` takes it; a shape where `i` prefers `fp`
   (the original's home) is the remaining lever for the `strd r6,sl` tails.
