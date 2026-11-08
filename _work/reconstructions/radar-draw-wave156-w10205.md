# Radar::draw wave 156 (w10205) — `bool enemyHit` + `int asteroidHit` lands 9908 B, raw 8490 -> 8293; `bool otherLock` is the only campaign-in-`sl` lever but is 112 B short

**Status:** source change landed, 2026-11-08. Worktree `wt/w10205`, based on `master`
(`cf74f4ee6`, wave 155). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave155-w10105.md`, `radar-draw-wave154-w10002.md`,
`radar-draw-wave153-w10003.md`.

Baseline (HEAD, wave 155): original 9,908 B; ours 9,908 B; raw same-address diff
**8,490**; instruction-index alignment **1,456**; all 27 section sizes exact;
file size 2,187,328 B.

## 1. The prologue knot, re-derived on the wave-155 HEAD

The wave-155 HEAD is **not** the w10105 shape: it is w10104's `noscope` String
temp + tail getter, and it has `bool marked`, `bool alwaysEnemy`, `int wreck`.
On this base the prologue is

```
12e07a  mov.w sl, #0        ; missionBlocksPlanets -> sl
12e0f8  str r0, [sp,#144]   ; campaign -> [sp,#144]   (spilled)
12e38a  str r0, [sp,#148]   ; otherLock -> [sp,#148]
```

while the original is

```
12e07a  mov.w fp, #0        ; missionBlocksPlanets -> fp
12e0f8  mov sl, r0          ; campaign -> sl
12e38e  str r0, [sp,#144]   ; otherLock -> [sp,#144]
```

i.e. the original has **campaign in `sl`** and `missionBlocksPlanets` in `fp`;
ours has `missionBlocksPlanets` in `sl` and campaign spilled.  The stack slots
shift by exactly one word (`otherLock` 144 vs 148, `turretMode` 148 vs 152).

## 2. `bool otherLock` is the only campaign-in-`sl` lever (confirmed)

`int otherLock` -> `bool otherLock` is still the only single toggle that
reproduces the original prologue map (`campaign`->`sl`, `missionBlocksPlanets`
->`fp`).  Measured on this HEAD:

| shape | size | raw | aligned | campaign | 68 |
|---|---:|---:|---:|---|---|
| HEAD | 9908 | 8490 | 1456 | `[sp,#144]` | remat `r6` |
| `bool otherLock` | 9796 | 9150 | 1700 | **`sl`** | `r9` (remat) |

The 112 B shortfall is the **int-vs-bool codegen of the `otherLock` boolean
expressions themselves** (word `orr`/`eor`/`cmp` vs byte forms), not a missing
body statement: the two disassemblies are instruction-for-instruction identical
in the `otherLock` region (`12e376`-`12e38e`, `12ebce`-`12ebd6`) apart from the
register/slot choice.  So the original's `otherLock` is `int` (word store at
`[sp,#144]`), and `bool otherLock` is a different variable, not the answer.

## 3. Why 68 does not reach `sl` at 9,908 B

In the original enemy loop all eight callee-saved registers are live:
`r4`=this, `r5`=locked, `r6`=bossSeen, `r8`=enemy, `r9`=&this->lockLabel,
`sl`=68, `fp`=i, `r7`=frame.  `wreck` is therefore spilled to `[sp,#152]`
(word store) and reloaded 8 times.  Ours keeps `wreck` in `sl` and spills
`&lockLabel`/rematerialises 68.  Every shape that frees `sl` for 68 also moves
`campaign` off the stack and is 100+ B short (see §2).

The 68 hoist in the **landmark** loop (`12e77e mov.w fp,#68` in ours, absent in
the original) is the direct cause: with `fp` reserved for 68,
`missionBlocksPlanets` cannot take `fp`, so `sl` is taken and campaign spills.
Preventing that hoist is the remaining lever; it is driven by MachineLICM
register pressure, not by a source statement we have found.

## 4. New hypotheses measured (all on the wave-155 HEAD)

Single type toggles (size / raw / aligned):

| toggle | size | raw | aligned |
|---|---:|---:|---:|
| `int alwaysEnemy` | 9904 | **8299** | **1438** |
| `bool enemyHit` | 9892 | 8474 | **1308** |
| `int asteroidHit` | 9900 | 8517 | 1448 |
| `int bossSeen` | 9876 | 8651 | 1422 |
| `bool wreck` | 9900 | 8567 | 1460 |
| `int landmarkHit` | 9904 | 8653 | 1783 |
| `int missionBlocksPlanets` | 9904 | 8526 | 1503 |
| `int otherLockFlag` | 9896 | 8835 | 1590 |
| `bool notOtherLock` | 9884 | 9039 | 1591 |
| `bool notTurretMode` | 9848 | 9441 | 1478 |

`int alwaysEnemy` is a genuine both-metric win (the original word-stores
`alwaysEnemy` at `[sp,#80]` and emits `movne r0,#1` rather than ours'
`mov.w r1,#1; movne r0,r1`), but it is exactly **4 B short**; two tail
`getCurrentCampaignMission()` companions overshoot to 9912 (raw 8260 /
aligned 1437).  No 4-B genuine companion was found.

New structural hypotheses tried and falsified (all codegen-identical):
`const` on `campaign`/`wreck`/`enemy`/`locked`/`target`; declaration-order moves
of `campaign`/`missionBlocksPlanets`/`turretMode`/`alienOrbit`; `char`/`short`/
`unsigned` campaign; `char`/`short`/`unsigned` otherLock; `int otherLockFlag`;
`String &lockLabel = this->lockLabel` (10044 B, worse); caching
`this->enemies`/`this->enemies->data()` in a local (campaign does move to `sl`,
but +56..+80 B and aligned 2400-3200); pointer loop over `enemies` (same);
`bossSeen = enemy->shipGroup == 10` (9920 B).

## 5. Landed change

```diff
-    int enemyHit = false;
+    bool enemyHit = false;
...
-            bool asteroidHit = false;
+            int asteroidHit = false;
```

`bool enemyHit` is the single biggest alignment win found (1456 -> 1308); it is
16 B short, and `int asteroidHit` is the only companion that lands exactly
9,908 B.  The pair also reproduces the original's `r9 = &this->lockLabel`
hoist in the enemy loop (`12ec90 add.w r9, r4, #396`), which HEAD spilled.

* raw same-address **8,490 -> 8,293** (-197)
* instruction-index alignment **1,456 -> 1,552** (+96; the `int asteroidHit`
  companion rotates the asteroid-loop block layout)
* size 9,908 B; all 27 section sizes exact; file size 2,187,328 B.

Register map of the landed shape: `campaign` `[sp,#144]`, `missionBlocksPlanets`
`sl`, `turretMode` `[sp,#152]`, `otherLock` `[sp,#148]`; enemy loop `r9` =
`&this->lockLabel`, `i` = `sl`, `68` rematerialised into `r6`; asteroid loop
`asteroidHit` word.

## 6. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 7. Open questions

1. `int alwaysEnemy` (9904 / 8299 / 1438) needs exactly +4 B of witnessed body:
   that would be a both-metric win and the correct `alwaysEnemy` type.
2. The landmark-loop `mov.w fp,#68` hoist is the structural cause of the
   campaign spill.  A source shape that stops MachineLICM hoisting it should
   snap the whole prologue + enemy loop to the original map at 9,908 B.
3. `bool otherLock` proves campaign-in-`sl` is reachable, but only at 9796 B;
   the 112 B is the int-vs-bool `otherLock` codegen, so the original's
   `otherLock` must stay `int`.
