# Radar::draw wave 141 (w8609) — hunk classification + int-local extension: raw 8,279 -> 8,244

**Status:** source change landed, 2026-11-07. Worktree `wt/w8609`, based on `master`
(`e18e0c2a4`). Target `src/game/weapons/Radar.cpp`, function
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8279** (the w8509
`int alwaysEnemy` + `int asteroid wreck` state). After this pass **8244** (-35).
Section deltas: `.text` **86,472 -> 86,437**; `.ARM.extab` **1,411** (unchanged);
`.rel.plt` **0** (byte-identical, 29,392 B); `.note.gnu.build-id` 20. All 27
section sizes exact. Gate before and after: byte **4507**, linked **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

Read with `radar-draw-wave140-w8509.md`, `radar-draw-wave140-w8510.md`,
`radar-draw-wave139-w8409.md`, `radar-draw-wave138-w8309.md`.

## 1. Hunk classification (same-address raw diff)

The 8,279 bytes are dominated by block-placement shift, not wrong code. The
instruction streams are equivalent over most of the body; the shift delta
oscillates between about -40 and +50 bytes at block boundaries. The largest
contiguous raw run is **318 B at `0x13000a`**, but it is a pure shift artifact:
the original has the enemy lock-timer block there, ours has `Level::getPlayer`
(two different blocks at the same address). The next runs (240 B at `0x12fa66`,
226 B at `0x12f216`, 198 B at `0x13023e`) are the same phenomenon in the gas and
asteroid loops.

The genuinely *local* (non-shift) deltas found this wave:

1. **Top-level `onPlanetCourse` block (`0x12e6ee`)**. The original spills
   `autoPilotTarget` to `[sp,#152]` (`str r0,[sp,#152]`) and saves three
   registers around the block (`mov r9,fp; mov fp,r8; mov r8,sl; mov sl,r0` …
   `mov sl,r8; mov r8,fp; mov fp,r9`). Ours keeps `autoPilotTarget` in `r9` and
   saves only two (`mov r9,r8; mov r8,sl; mov sl,r0` … `mov sl,r8; mov r8,r9`).
   That is a 4-8 B register-allocation difference, not a source difference.
2. **The post-loop `if (!alienOrbit)` `onPlanetCourse` recompute is invented.**
   The original calls `getAutoPilotTarget` **once** (`0x12e6f4`) and
   `getStationEnumIndex` **once** (`0x12e72a`); ours calls each twice
   (`0x12e6d8`/`0x12e752` and `0x12e70c`/`0x12e746`). Removing the recompute is
   the faithful source shape, but the function drops to **9856 B** (52 short,
   raw 9154): the original has 52 B of cold-block code elsewhere that we have
   not recovered. This is the largest single structural lead for the next wave.
3. **The int-local select shapes.** w8509 established that `int alwaysEnemy`
   and `int asteroid wreck` reproduce the original's `cmp/select` shape instead
   of clang's `orrs` boolean fold. The same hypothesis applies to the other
   bool locals that feed a select or a cross-block phi.

## 2. Landed change

```cpp
int bossSeen = false;                 // enemy loop (was bool)
int inSight = false;                  // enemy loop inSight (was bool)
int asteroidHit = false;              // asteroid loop (was bool)
```

Measured matrix (single edits on the w8509 baseline unless noted):

| variant | usize | raw diff | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| **w8509 baseline** | **9908** | **8279** | 86,472 | 1,411 |
| `int bossSeen` | 9900 | 8017 | — | — |
| `int bossSeen` + `int asteroidHit` | 9912 | 8292 | — | — |
| `int bossSeen` + `int inSight` | 9904 | 8539 | — | — |
| **`int bossSeen` + `int asteroidHit` + `int inSight` (landed)** | **9908** | **8244** | **86,437** | **1,411** |
| + `int landmarkHit` | 9908 | 8241 | 86,434 | 1,461 |
| `int bossSeen` + `int enemyHit` + `int planetHit` | 9908 | 8306 | — | — |
| `int bossSeen` + `int enemyWreck` | 9908 | 8356 | — | — |

`int bossSeen` alone is the biggest raw win (-262) but is 8 B short. The landed
triple is the only combination that both restores 9908 B and leaves `.ARM.extab`
byte-diff unchanged. Adding `int landmarkHit` shaves 3 more raw bytes but grows
the `.ARM.extab` content diff by 50 B, so it was not landed.

## 3. Tried and ruled out

* Removing the invented post-loop recompute: 9856 B, raw 9154 (52 B short).
* Hoisting `autoPilotTarget` / `targets` / `system` to function scope: 9900,
  raw 8016 (no size recovery).
* `int` loop indices (`i`, `s`, `c`) in every loop: no size or raw change.
* `int gasHit`, `int navWreck`, `int hasDockNav`, `int candidateWreck`,
  `int ambient`, `int topOk`, `int dead`: no size or raw change.
* `int planetHit`: 9884, raw 8514 (16 B short).
* `int enemyHit`: 9920, raw 8139 (12 B over).
* `int enemy wreck`: 9908, raw 8356 (adds the 8 B but costs 77 raw in the enemy
  loop).
* `int missionBlocksPlanets`: 9900, raw 8038.

## 4. Open questions

* The 52-byte gap left by removing the post-loop recompute. The original's cold
  blocks total 52 B more than ours; the top-level `onPlanetCourse` spill is 4-8 B
  of it. Recovering the exact cold-block order (`cond -> cont -> navWreck ->
  clear -> otherLock recompute -> gas -> lock-timer`) without the recompute is
  the next target.
* `bossSeen`/`alwaysEnemy` still use a 4-byte `mov.w r1,#1; movne r0,r1`
  rematerialisation instead of the original's 2-byte `movne r0,#1`, and the
  stack slot is `sp#96` vs the original's `sp#80`.
* The `otherLock` guard remains the densest local hunk (213/250 bytes); w8410's
  `la_e_o0` is still 4 B too big.

## 5. Gate

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
