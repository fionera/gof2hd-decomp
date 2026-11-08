# Radar::draw wave 155 (w10105) — `int marked` reproduces the planet-loop map (`fp`=i, `sl`=stations); 9908 B best raw 8506

**Status:** source change landed, 2026-11-08. Worktree `wt/w10105`, based on `master`
(`f4e0ea40b`, wave 154). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave154-w10002.md`, `radar-draw-wave151-w9803.md`,
`radar-draw-wave151-w9703.md`.

Baseline (HEAD, wave 154): original 9,908 B; ours 9,908 B; raw same-address diff
**8,577**; instruction-index alignment **1,484**; all 27 section sizes exact.

## 1. New structural lever: `int marked` (the planet-loop `marked` flag)

w10002 asked for a structural source difference that puts `campaign` in `sl`.
The missing piece is the **planet-targets loop's `marked` flag type**.  The
original's planet loop is

```
12e3c6  mov  sl, r0            ; stations = getStations()  -> sl (also spilled [sp,#116])
12e3c8  mov.w fp, #0           ; i -> fp
12e3da  cmp  fp, r1            ; loop bound
12e3e2  ldr.w r1,[r0,fp,lsl#2] ; planetTargets->data()[i]
...
12e4ec  add.w fp, fp, #1
```
i.e. **`fp` is the planet-loop index and `sl` is the `stations` array**; the
`marked` flag is spilled to `[sp,#132]`.

Our HEAD instead uses `sl` as the planet-loop index and keeps `marked` in `fp`
(`12e5b0 add.w sl,sl,#1`, `12e43a ldr.w fp,[sp,#156]`), which is the root of the
whole downstream permutation.  Changing only

```diff
-            bool marked = false;
+            int marked = false;
```

(`T`) reproduces the original planet-loop map exactly: `fp`=i, `sl`=stations,
`sl` spilled, `marked` spilled.  On the HEAD base `T` alone is 9,912 B
(+4, raw 8,641 / aligned 1,535); on the `bool otherLock` base (w10002) it is
9,808 B.  This is the "bool/int mix that keeps the witnessed types" the mission
asked for.

`bossSeen = enemy->shipGroup == 10;` (the w9803 `S` re-test) is also witnessed:
the original reloads `shipGroup` after the `field_0x1bc++` store
(`12ed52 ldr.w r0,[r8,#40]; cmp r0,#10; it eq; moveq r6,#1`).  The second
`bossSeen = true` site is a no-op when changed (the compiler already reloads).

## 2. 9,908 B shapes measured (all keep every section size exact)

| shape | size | raw | aligned |
|---|---:|---:|---:|
| HEAD (`int otherLock`, `bool marked`) | 9908 | 8577 | 1484 |
| `T` (`int marked`) | 9912 | 8641 | 1535 |
| `T + N` (`int alwaysEnemy`) | 9908 | 8589 | 1522 |
| `T + W` (`int alienOrbit`) | 9908 | 8600 | 1543 |
| `T + M` (`int missionBlocksPlanets`) | 9908 | 8698 | 1593 |
| **`T + ew + N + S` (landed)** | **9908** | **8506** | **1572** |
| `T + ew + W + S` | 9908 | 8608 | **1471** |
| `T + ew + N + S + R` | 9900 | **7941** | 1544 |
| `T + ew + N + S + E` | 9884 | 8482 | 1402 |

`ew` = enemy-loop `int wreck` -> `bool wreck`; `R` = `int asteroidHit`;
`E` = `bool enemyHit`.  `T+ew+N+S+R` (9900 / 7941 / 1544) is the best raw shape
found and is **8 B short**; `T+ew+N+S+E` improves both metrics but is 24 B short.
A witnessed +8 B body addition on top of `T+ew+N+S+R` is the remaining lever.

## 3. Register maps

Original enemy loop: `r5`=locked, `r6`=bossSeen, `r8`=enemy, `r9`=&this->lockLabel,
`sl`=68, `fp`=i, `wreck` spilled `[sp,#152]`, `&lockedEnemy` spilled `[sp,#128]`.

`T` (and `T+ew+N+S`): planet loop `fp`=i / `sl`=stations (matches original);
enemy loop `fp`=i, `r9`=68, `sl`=wreck, `r6`=&lockedEnemy scratch, `r5`=locked,
`bossSeen` spilled `[sp,#104]`, `&lockLabel` spilled `[sp,#76]`.
So the planet-loop half of the permutation is now reproduced; the enemy-loop
`r9`/`sl` swap (68 vs &lockLabel) is still open.

## 4. Landed change

```diff
-            bool marked = false;
+            int marked = false;
...
-            int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
+            bool wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
...
-        bool alwaysEnemy = false;
+        int alwaysEnemy = false;
...
-                    bossSeen = true;
+                    bossSeen = enemy->shipGroup == 10;
```

* raw same-address **8,577 -> 8,506** (-71)
* instruction-index alignment **1,484 -> 1,572** (+88)
* size 9,908 B; all 27 section sizes exact.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 6. Open questions

1. `T+ew+N+S+R` = 9900 / 7941 / 1544 needs exactly +8 B of witnessed body.
2. `T+ew+N+S+E` = 9884 / 8482 / 1402 improves both metrics; needs +24 B.
3. The enemy-loop `r9`/`sl` swap (68 vs `&lockLabel`) survives the planet-loop
   fix; the remaining lever is whatever makes `&lockLabel` outrank `68` for
   `r9` (or `68` for `sl`) once `wreck` is spilled.
