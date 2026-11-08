# Radar::draw wave 157 (w10405) — `bool otherLock` reproduces campaign-in-`sl`
# but 68 stays in `r9`; no shape reaches 68-in-`sl` at 9,908 B

**Status:** analysis pass, no source change landed, 2026-11-08. Worktree `wt/w10405`,
based on `master` (`9e4795206`, wave 157, w10305 landed). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`.

Read with `radar-draw-wave157-w10306.md`, `radar-draw-wave157-w10305.md`,
`radar-draw-wave154-w10002.md`.

Baseline (HEAD, w10305 landed): original 9,908 B; ours 9,908 B; raw same-address
diff **8,181**; instruction-index alignment **1,278**; all 27 section sizes exact;
file size 2,187,328 B.

## 1. Ground truth re-check: the original's register map

Manual disassembly of the original (base 0x10000, function 0x12e050, size 9,908):

| value | original |
|---|---|
| `campaign` | `sl` (12e0f8 `mov sl,r0`), spilled to `[sp,#144]` at the landmark preheader 12e7ba, restored 12eb4c |
| `missionBlocksPlanets` | `fp` (12e07a / 12e0d8 `mov.w fp,#0`), spilled to `[sp,#112]` at 12e3ba |
| planet-loop index | `fp` (12e3c8 `mov.w fp,#0`) |
| landmark-loop index | `sl` (12e7be `mov.w sl,#0`) |
| landmark `68` | `r6` remat (12e918, 12ea10 `movs r6,#68`) |
| enemy `68` | `sl` (12ecd0, 12eea2, 12f128, 12f196 `mov.w sl,#68`) |
| `&lockLabel` | `r9` (12e284, 12e7b6, 12ecc8, 12f436 `add.w r9,r4,#396`) |
| enemy-loop index | `fp` (12ecd8 `mov.w fp,#0`) |
| `bossSeen` | `r6` (12ece0 `movs r6,#0`) |

So the original is: `campaign` in `sl` (spilled only across the landmark block),
`missionBlocksPlanets` in `fp`, `&lockLabel` in `r9`, enemy `68` in `sl`
(reusing the dead `campaign` interval).

## 2. `bool otherLock` reproduces the whole prologue, but not the enemy loop

`int otherLock` -> `bool otherLock` (pure type change on the w10305 base):

| shape | size | raw | aligned | campaign | missionBlocks | 68 (landmark) | 68 (enemy) | `&lockLabel` |
|---|---:|---:|---:|---|---|---|---|---|
| HEAD (`int otherLock`) | 9908 | 8181 | 1278 | `[sp,#144]` | `sl` | `fp` (hoist) | `r6` | stack |
| `bool otherLock` | 9792 | 9142 | 1633 | **`sl`** | **`fp`** | `r9` (hoist) | `r9` | stack |

The bool shape is a *structural* match for the prologue and the planet loop:
`mov.w fp,#0` (missionBlocks) at 12e07a, `mov sl,r0` (campaign) at 12e0f6,
`str.w fp,[sp,#116]` + `mov.w fp,#0` (planet index) at 12e3ac/12e3ba.  It is
116 B short and the enemy loop assigns `r9` to `68` (and spills `&lockLabel`)
instead of `r9`=`&lockLabel` / `sl`=`68`.

## 3. Full measured matrix (all on the w10305 base)

Single toggles on `bool otherLock` (base 9,792 / 9,142 / 1,633):

| toggle | size | raw | aligned |
|---|---:|---:|---:|
| `int bossSeen` | 9800 | 9163 | **1404** |
| `int marked` | 9800 | 9140 | 1617 |
| `int enemyHit` | 9812 | 9155 | 1673 |
| `int landmarkHit` | 9784 | 9089 | 1754 |
| `int planetHit` | 9816 | 8998 | 1635 |
| `int gasHit` | 9792 | 9143 | 1633 |
| `int asteroidHit` | 9804 | 9162 | 1833 |
| `bool wreck` (enemy, was int) | 9780 | 9150 | 2878 |
| `bool wreck` (asteroid) | 9788 | 9156 | 1628 |
| `int topOk` | 9784 | 9152 | 1647 |
| `bool notOtherLock` | 9800 | 9117 | 1628 |
| `bool notTurretMode` | 9780 | 9082 | 1700 |
| `int otherLockFlag` | 9808 | 9148 | 1628 |
| `PlayerFighter *enemy` cast | 9792 | 9142 | 1633 |
| `KIPlayer *const enemy` | 9792 | 9142 | 1633 |
| `PlayerAsteroid *const asteroid` | 9792 | 9142 | 1633 |
| `String &lockLabel` ref | 9792 | 9142 | 1633 |
| `String *lockLabelP` (function scope) | 9590 | 9908 | 2862 |

Combinations on `bool otherLock`:

| shape | size | raw | aligned |
|---|---:|---:|---:|
| `int bossSeen` + `int marked` | 9808 | 9149 | **1383** |
| `int bossSeen` + `int planetHit` | 9824 | 8984 | 1445 |
| `int bossSeen` + `int enemyHit` | 9816 | 9170 | 1490 |
| `int bossSeen` + `int planetHit` + `int marked` | 9832 | 9113 | 1430 |
| scoped else arm (w10304) | 9788 | 9127 | 1687 |
| no tail `String` | 9784 | 9156 | 1631 |
| no tail `String` + no plasma store | 9780 | 9158 | 1629 |

Other structural attempts on the base:

| shape | size | raw | aligned | campaign |
|---|---:|---:|---:|---|
| planet block moved before landmark block (`int`) | 9836 | 9492 | 2024 | `r9` |
| planet block moved before landmark block (`bool`) | 9804 | 9407 | 1936 | `r9` |
| `int missionBlocksPlanets` | 9904 | 8245 | 1326 | `[sp,#144]` |
| `unsigned char missionBlocksPlanets` | 9904 | 8245 | 1326 | `[sp,#144]` |

Every `campaign`->`sl` shape keeps the enemy `68` in `r9` and spills
`&lockLabel`; every 9,908 B shape keeps `campaign` on the stack.  Moving the
planet block before the landmark block (so `campaign`'s live range ends before
the landmark loop) *loses* `campaign` from `sl` (it goes to `r9`), i.e. the
opposite of the hypothesis.

## 4. Why 68 does not reach `sl`

The allocator has to choose, at the enemy-loop preheader, between `&lockLabel`
(2 uses, live across `String::operator=` and `DrawString`) and the constant 68
(4 uses, cheaply rematerialisable).  In the original `&lockLabel` wins `r9` and
68 takes the dead `campaign` interval in `sl`; in every source shape tried here
68 wins `r9` and `&lockLabel` is spilled to `[sp,#76]` (which also shifts the
whole precomputed-pointer stack frame by 4 bytes).  `const`, references,
function-scope pointer locals, `PlayerFighter`/`PlayerAsteroid` locals, pointer
loops and the `bool`/`int` mixes listed above do not change the tie-break.
The `mov.w r9,#68` hoist in the landmark loop is the same phenomenon one loop
earlier (the original rematerialises into `r6` because `r9` holds `&lockLabel`).

## 5. Conclusion / open questions

1. `bool otherLock` is confirmed as the campaign-in-`sl` lever, but the raw cost
   is large (8,181 -> 9,142) and it does not fix the enemy-loop `r9`/`sl`
   tie-break.  Landing it alone would regress both metrics, so the base was kept.
2. The remaining 116 B is not a single missing statement: the original has
   3,532 instructions (2,110 16-bit / 1,422 32-bit), the bool shape ~3,480, and
   the widths are similar; the gap is a mix of ~50 instructions and the
   4-byte stack-frame shift caused by spilling `&lockLabel`.
3. The next lever to try is a source shape that makes `&lockLabel` *live*
   across the enemy loop in a callee-saved register (e.g. an explicit
   `String *` that the allocator cannot fold) while keeping `campaign` in `sl`.
   The function-scope `String *lockLabelP` attempt moved `campaign` to `r9`
   instead, so the ordering of the two intervals matters.
4. The original's duplicated `stationLockTimer = 0` store in both arms of the
   landmark-lock else (12ebba and 12fe62) is a real +8 B body difference still
   missing from our source (`if (lockedStation == nullptr) { field_0x28 = 0;
   stationLockTimer = 0; } else { stationLockTimer = 0; }`).  It was not
   combined with the bool shape in the time available.

## 6. Gate

No source change landed; HEAD gate unchanged (byte_exact 4507, linked_exact
4507, size 4524, data 843, imports 207, lint clean, drift 0).
Radar::draw stays raw **8,181** / aligned **1,278** at 9,908 B.
