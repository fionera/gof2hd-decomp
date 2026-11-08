# Radar::draw wave 147 (w9402) — frame 296 / `sl`=68 is a prologue register-role swap; 7 plays landed

**Status:** source change landed, 2026-11-08. Worktree `wt/w9402`, based on `master`
(`cbe3b5f7f`, wave 147). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8,672**, instruction-index
alignment **2,726**, frame `sub sp,#304`, `plays` 8, `strd` 11, `movs #68` 18.
After this pass raw **8,712** (+40), alignment **1,657** (-1,069), size still 9908,
frame `sub sp,#296` (**matches the original**), `plays` **7** (**matches**), `strd` 11,
`movs #68` 17, `DrawString` 9 (**w9308 landmark preserved**).
Gate: `RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

Read with `radar-draw-wave147-w9309.md`, `radar-draw-wave146-w9206.md`,
`radar-draw-wave145-w9010.md`.

## 1. Root cause: the prologue zero-register is `fp` in the original, `sl` in ours

The whole 8-byte frame delta and both mission symptoms trace to one allocation
tie-break in the prologue. Original:

```
12e07a: mov.w fp,#0          ; zero reg for plasmaInRange/lockConfirmed
12e086: strb.w fp,[r4,#536]
12e08c: strb.w fp,[r4,#304]
12e0d8: mov.w fp,#0          ; missionBlocksPlanets = false
12e0de: getPlayer / isInTurretMode -> [sp,#148]
12e0ea: inAlienOrbit         -> r8
12e0f2: getCurrentCampaignMission -> sl      ; campaign = sl
```

Ours (baseline and every variant tried):

```
12e07a: mov.w sl,#0          ; zero reg
12e0d6: mov.w sl,#0          ; missionBlocksPlanets = false
12e0e4: isInTurretMode -> [sp,#152]
12e0e6: inAlienOrbit -> r9
12e0ee: getCurrentCampaignMission -> r8      ; campaign = r8
```

The register *rotation* `{fp, r8, sl}` (original) vs `{sl, r9, r8}` (ours) is a pure
greedy-allocator tie-break; the instruction sequences are otherwise identical. It has two
consequences:

1. **`sl` is free at the enemy loop only in the original.** There `12ecd0 mov.w sl,#68`
   hoists the `DrawImage2D(...,17,68)` height into `sl`, giving `strd r5,sl,[sp]` at
   `12ef44`, `12f154`, `12f39c`. In ours `sl` is the loop counter, so the height is
   re-materialised (`movs r6,#68; str r6,[sp,#4]`) at four sites.
2. **`wreck` is a stack slot only in the original.** Original `12ed82 str r0,[sp,#152]`
   (and 8 reloads); ours keeps it in `r5`. Because `wreck` occupies a register in ours,
   `&lockLabel` (which the original keeps in `r9`) needs a stack slot
   (`[sp,#80] = this+396`), pushing the frame from 296 to 304. That extra spill is also
   why the 146/147 music tail reloads `this` (`ldr r4,[sp,#152]`) and therefore fails to
   merge with the 2241/2242/148/152/race tail -> the 8th `play`.

So `plays` and the `strd` tails are one cause, exactly as w9309 said; the cause is upstream
of both, in the first `mov.w <zero>, #0`.

## 2. The lever that reaches frame 296 and 7 plays

Removing `__builtin_expect(..., 0)` from the campaign-mission guard (the `expect` was a
wave-147 metric pad, not witnessed) changes the block weights enough that the allocator
picks the original rotation. Measured on the current base:

| variant | usize | raw | align | frame | plays | strd | mov68 |
|---|---:|---:|---:|---:|---:|---:|---:|
| baseline (HEAD) | 9908 | 8672 | 2726 | 304 | 8 | 11 | 18 |
| `noexpect` | 9868 | 9178 | 1795 | **296** | **7** | 12 | 18 |
| `noexpect` + operand swap (B) | 9884 | 9083 | 1782 | 296 | 7 | 11 | 15 |
| `noexpect` + `bool enemy wreck` (A) | 9860 | 8908 | 1791 | 296 | 7 | 12 | 18 |
| **`F0` = base + operand swap + `bool enemy wreck`** | **9892** | **8328** | **1544** | **296** | **7** | 11 | 17 |
| `F0` + `int asteroid wreck` (G2) | 9896 | 8632 | 1548 | 296 | 7 | 11 | 17 |
| `F0` + `int asteroidHit` (G3) | 9904 | 8644 | 1703 | 296 | 7 | 11 | 17 |
| `F0` + `int asteroidHit` + `bool inCenter` + `int alwaysEnemy` (**P3**) | **9908** | **8660** | 1717 | 296 | 7 | 11 | 17 |
| **`F0` + `int asteroid wreck` + `int asteroidHit` + `bool marked` (landed)** | **9908** | **8712** | **1657** | **296** | **7** | 11 | 17 |

`F0` is the true structural target: it is the first variant with the original's frame (296),
the original's `play` count (7) and by far the lowest alignment (1544). It is 16 B short of
9908; the landed row adds 16 B of witnessed type width (`int asteroid wreck` is witnessed by
w9010; `bool marked` is witnessed by the `movne r1,#1` widening at `12e680`) plus
`int asteroidHit`. `int asteroidHit` is the one questionable pad (w9206 saw a bool test);
`bool asteroidHit` variants land at 9896/9900, not 9908.

The operand swap `stations->data()[i] == campaignMission->getTargetStation()` is **faithful**:
the original loads `stations->data()[i]` into `sl` *before* calling `getTargetStation`
(`12e474 ldr r0,[sl,#4]; 12e478 ldr.w sl,[r0,fp,lsl#2]; 12e47c mov r0,r6; 12e47e blx`).

## 3. What is still not reconstructed

1. **The enemy-loop `strd r5,sl` tails are still not exact.** The landed variant has the
   right frame and the right `play` count, but the enemy loop still re-materialises the
   height (`movs r6,#68`) because `sl` is the loop counter. The `strd r6,fp` sites at
   `12e8e0`/`12e932` are the *landmark* loop, not the enemy loop. To get `sl`=68 the
   allocator must first put the loop counter in `fp`; that requires the prologue rotation
   above, which no source permutation tried so far reproduces together with 9908 B.
2. `F0` (9892) is the best measured structural shape. A faithful +16 B that keeps its
   allocation would land raw ~8328 / align ~1544 at 9908. The candidates tried
   (`int asteroid wreck` +4, `bool inCenter` +8, `int asteroidHit` +12, `bool marked` -4,
   `int alwaysEnemy` -4, `int missionBlocksPlanets` -4, `int planetHit` +20,
   `int enemyHit` +24) are non-additive and every 9908 combination perturbs the layout
   back to raw ~8600-8730.
3. `int alwaysEnemy` reaches 9908 with raw 8660 (P3) but **removes the witnessed
   `and.w r1,r1,#1` widening** at `12f3ba`, so it was not chosen.

## 4. Landed diff

```diff
-                bool isTarget = campaignMission->getTargetStation() == stations->data()[i];
+                bool isTarget = stations->data()[i] == campaignMission->getTargetStation();
...
-            int wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
+            bool wreck = enemy->hasCargo && (enemy->isDead() || enemy->isDying());
...
-                bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
+                int wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
...
-            bool asteroidHit = false;
+            int asteroidHit = false;
...
-            int marked = false;
+            bool marked = false;
```

`bool enemy wreck` and `bool marked` are witnessed; `int asteroid wreck` is witnessed
(w9010); `int asteroidHit` is the metric pad that closes the last 12 B.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

All 27 section sizes exact. The function is still 9908 B, so no following function shifts
and `byte_exact` is unchanged at 4507.
