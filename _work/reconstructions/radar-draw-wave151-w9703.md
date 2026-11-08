# Radar::draw wave 151 (w9703) — `sl = 68` is not reachable from the bool→int toggles; asteroid-wreck bool + `int marked` lands 8620/1530

**Status:** source change landed, 2026-11-08. Worktree `wt/w9703`, based on `master`
(`db9658a51`, wave 150). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave149-w9606.md`, `radar-draw-wave148-w9401.md`,
`radar-draw-wave147-w9309.md`, `radar-draw-wave149-w9501.md`.

Baseline (HEAD, wave 150): original 9908 B; ours 9908 B; raw same-address diff
**8,739**; instruction-index alignment **1,483**; `.text` **86,863** differing
bytes; all 27 section sizes exact; `.rodata`/`.data`/`.bss`/`.ARM.exidx`
byte-identical; `.rel.plt` byte-identical; `.note.gnu.build-id` 20.

## 1. The merged baseline no longer has the w9606 strd tails

The wave-150 baseline is the merge of w9606 (`int wreck` + `bool asteroidHit`) and
w9607 (drop the `otherLock` expect). On this base the enemy loop is

```
12ec5a add.w r1,r4,#396 ; str r1,[sp,#80]   ; &lockLabel spilled
12eca4 mov.w r9,#68                          ; 68 hoisted into r9
12ef1a movs r6,#68 ; str r6,[sp,#4]          ; not strd
```

i.e. the enemy-loop `strd r6,sl` tails are *gone*; only the planet-loop
`strd r6,fp` remains. The w9606 `r9 = 68` strd shape is `A`-family only.

## 2. `sl = 68` is not reachable from bool→int widening (new falsification)

Swept ~500 combinations of the source-level toggles:

* `A` `expect(enemies != nullptr, 1)` (the w9309 E2 lever)
* `b` enemy-loop `int wreck` → `bool wreck`
* `c` asteroid-loop `int wreck` → `bool wreck`
* `O` `int planetHit`, `R` `int asteroidHit`, `K` `int turretMode`,
  `W` `int alienOrbit`, `E` `int enemyHit`, `M` `int missionBlocksPlanets`,
  `S` `int bossSeen`, `H` `int gasHit`, `N` `int alwaysEnemy`,
  `I/L` `int inSight`, `P` `int navWreck`, `Q` `int candidateWreck`,
  `T` `int marked`, `F` `int otherLockFlag`, `G` `int onPlanetCourse`,
  `J` `int topOk`, `U` `int hasDockNav`, `V` `int isTarget`,
  `6` `bool otherLock`, `7` `bool landmarkHit`, `8` `bool notOtherLock`,
  `9` `bool notTurretMode`, `0` `bool alwaysEnemySeen`
* `1`–`5` `__builtin_expect` on the enemy `onScreen` / `wreck` / `enemy==locked`
  / music `field_0x1bc` / asteroid `asteroids != nullptr` guards
* `X` explicit `String *lockLabelPtr`, `Y` drop the `locked` local,
  `Z` `int spriteHeight = 68` local

**No combination emits `mov.w sl,#68`.** The closest is `bS` (bool wreck +
int bossSeen): **9864 B, 8775 / 1453**, which restores the original's
`r9 = &this->lockLabel` (`12e7b6`-style) and leaves `sl` completely unused in the
enemy loop — but the allocator still rematerialises 68 into `r6`
(`movs r6,#68`) instead of hoisting it into the free `sl`. So the original's
`sl = 68` is not a type-widening consequence; it needs a structural source
difference (or is an allocator artefact of a body we still do not have).

`A`-family shapes do hoist 68 (`r968 = 3`) but always into `r9`, never `sl`:
`ABrOKE` = 9908 / 8727 / 2137 (the w9606 best), `AOKE` identical.

## 3. New lever: the asteroid-loop `wreck` type

`c` (asteroid-loop `wreck` → `bool`, the type w9401 records as witnessed) is a
real win on both metrics at almost the right size:

| variant | usize | raw | align |
|---|---:|---:|---:|
| baseline | 9908 | 8739 | 1483 |
| `c` | 9904 | **8709** | **1481** |
| `cT` (landed) | **9908** | **8620** | 1530 |
| `c7` (`+bool landmarkHit`) | 9888 | 8540 | **1334** |
| `c7T` | 9892 | **7988** | **1377** |
| `c7E` | 9900 | 8332 | 1478 |
| `c7N` | 9880 | 8310 | 1317 |
| `c7TRE` | 9900 | 8079 | 1529 |

`c7T` is the best raw+aligned shape found (7988 / 1377) but is 16 B short; every
companion tried moved it to 9884/9888/9892/9896/9900/9904/9912, never 9908.
`c` alone is 4 B short.

## 4. Landed change

```diff
-            bool marked = false;
+            int marked = false;
...
-                int wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
+                bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
```

`cT` is the best 9,908 B shape that improves the raw metric substantially while
keeping alignment within +47 of the baseline, and it beats the w9606 strd-tail
best (`ABrOKE` 8727 / 2137) on **both** metrics.

* raw same-address **8,739 → 8,620** (−119)
* instruction-index alignment **1,483 → 1,530** (+47)
* `.text` differing bytes **86,863 → 86,744** (−119)
* size 9,908 B, frame 296, 7 `blx play`; all 27 section sizes exact;
  `.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical; `.rel.plt` byte-identical.

Other 9,908 B shapes measured (for a future merge): `bRW` 8458 / 1632,
`bR` 8461 / 1646, `HJRUb` 8461 / 1653, `MT` 8636 / 1588, `W` 8736 / 1488,
`ABrOKE` 8727 / 2137. None improves both metrics.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)   parity: 5370/5370
```

## 6. Open questions

1. `bS` (9864, 8775/1453) proves `r9 = &lockLabel` is reachable and leaves `sl`
   free; only the 68 hoist is missing. A structural source change that makes 68
   live across the whole enemy loop (not a literal rematerialised per site) is
   the remaining lever for the original `sl = 68` + `strd r6,sl` tails.
2. `c7T` (7988/1377) is 16 B short of 9908. A witnessed 16 B body addition on
   top of it would be the best shape by far.
3. `c` alone (9904, 8709/1481) is 4 B short and improves both metrics; a
   shift-free +4 B companion would land it.
