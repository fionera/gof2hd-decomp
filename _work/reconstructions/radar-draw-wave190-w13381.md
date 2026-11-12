# Radar::draw wave 190 (w13381) — enemy tractor-guard operand rotation;
# raw 6,301 -> 6,299 (size-exact, `.ARM.extab` unchanged)

**Status:** landed, 2026-11-12. Worktree `wt/w13381`, branch `wave190-w13381`, base
`master` `5cd492611`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13331.md`, `-w13335.md`, `-w13342.md`, `-w13346.md`,
`-w13356.md` and `-w13364.md`. This pass avoided the planet-loop register skeleton and
the landmark crosshair block, and worked the enemy-loop tractor guard.

## 0. The win

One source edit in the enemy tractor guard (line 835):

```cpp
-            if (!this->level->getPlayer()->isAutoPilot() &&
-                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) &&
-                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck) && (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&
-                (!wreck || enemy->isDead())) {
+            if (!this->level->getPlayer()->isAutoPilot() &&
+                (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&
+                (!wreck || enemy->isDead()) &&
+                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) &&
+                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck)) {
```

All five top-level operands are pure reads (three `PlayerEgo` predicates, two local
flags, two `KIPlayer` field reads), so the rotation is semantics-preserving.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | **6,301** | **6,299** |
| instruction-index alignment | 1,876 | 1,900 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 73,487 | **73,485** |
| `.ARM.extab` differing bytes | 610 | 610 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total differing bytes | 74,117 | **74,115** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370. The function was already non-byte-exact, so
`byte_exact` is unchanged; the gain is in the raw residual only.

## 1. How the win was found

The five-operand guard was permuted exhaustively (120 orderings). The winning ordering
moves the cheap `enemyHit`/`field_0x24` and `wreck`/`isDead` tests ahead of the
`isDockedToDockingPoint()` call and the `countsAsEnemyExcludeFlag` test, which lets clang
emit the original's short-circuit shape in the tractor block. The base ordering is the
first row of the table; the landed ordering is the best.

| guard ordering | raw | aligned | size |
|---|---:|---:|---:|
| base (`A B C D E`) | **6,301** | 1,876 | 9,908 |
| **`A D E B C` (landed)** | **6,299** | 1,900 | **9,908** |
| `A B E D C` | 6,301 | 1,892 | 9,908 |
| `A D C B E` | 6,301 | 1,903 | 9,908 |
| `A D C E B` | 6,301 | 1,911 | 9,908 |

where `A = !isAutoPilot()`, `B = expect(!isDockedToDockingPoint(),1)`,
`C = !(countsAsEnemyExcludeFlag != 0 && !wreck)`,
`D = (!enemyHit || expect(field_0x24,0))`, `E = (!wreck || isDead())`.

## 2. Other cells measured this pass (all reverted)

Every row is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13381/`).

### 2.1 Adjacent single-line statement swaps (134 candidates, whole function)

Automated swap of every pair of adjacent same-indent single-line statements in
`Radar::draw`. No ordering beat 6,301; the only equal-raw rows with different alignment
were `enemy->field_0x76 = 0`/`enemy->field_0x73 = 0` (aligned 1,896) and the
`dockNavPtr`/`field_0x1c` tractor stores (aligned 1,897).

### 2.2 `__builtin_expect` flips (31 hints, individually)

No single flip beat 6,301. Nearest size-exact rows: line 655 `0->1` = 6,303, line 564
`1->0` = 6,315, line 1289 `1->0` = 6,425, line 726 `1->0` = 6,383. Pairwise/triple
combinations of the nearest rows (`{564,650,655,726,787,826,1289}`) were also measured;
none beat 6,301.

### 2.3 Tractor-condition permutations (24 orderings)

`tractorMode2 && wreck && isDead() && field_0x1c == nullptr` and all 23 permutations:
best equal-raw 6,301 (`tractorMode2 && isDead() && wreck && field_0x1c == nullptr`,
aligned 1,786); no raw win.

### 2.4 Landmark / station / planet / asteroid statement rotations

Landmark crosshair `float left`/`int top`/`topOk` spellings (`float top`, `int left`,
`int left`+`int top`, inline `topOk`, `size*2` vs `size+size`) are byte-identical or
worse (6,304 for `int left`). Planet-loop declaration/statement rotations
(`stationId`/`currentStation`, `campaignMission`/`freelance`, `marked`, `textOffset`,
`planetDockIndex`/`field_0x18`, `planetHit`) are byte-identical or 6,302. Station-blip
and asteroid-blip local aliases change the function size (9,896-9,912). The wreck x/y
block was re-attempted with a volatile `screenX` read; it rematerialises only two of the
original's four loads and is 6,302 (size exact).

### 2.5 Campaign-in-`sl` family on the current base

The w13342 five-edit body (plain-OR `otherLock`, `bossSeen ||`, `int marked`,
`volatile int autoPilotTarget`, `int alienOrbit`) now measures **9,219 / 2,700 / 9,888**
on the post-`camZ`-hoist base (the w13342 note's 6,918 was on the pre-hoist 6,309 base),
so the family is no longer a viable route without a new register-skeleton fix.

## 3. Where the residual still lives (unchanged)

The planet-loop latch/block placement (`12e3ce-12e7aa`), the enemy loop
(`12ef6d-12f7cc`), the wreck x/y block (`12fc4c-12fca0`, 12 B shorter than the original
because clang CSE-hoists `screenX`), and the music tail (`130264+`, `r8` vs `r9` for
`FModSound*` and `sp+0x60` vs `sp+0x5c` for `alwaysEnemySeen`).

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6299 aligned= 1900 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13381/`: `Radar.base.cpp` (6,301 base),
`batch.py` + `cands*.py` (targeted cells), `autoswap.py` (134 adjacent swaps),
`flipexpect.py` (31 hint flips), `flipset.py` (pairwise/triple hint combinations),
`perm.py` (24 tractor orderings), `perm2.py` (120 five-operand guard orderings).

## Current-master harvest (wave 190)

Re-applied the enemy tractor-guard operand rotation on current master
(`5ff423480`). Radar raw **6,293 -> 6,291** (-2), aligned 1,828. It composes
with the w13370 landmark crosshair bound to raw **6,291** (-3 from 6,294).
`.text` 73,273 -> 73,254 (-19 across LevelScript + Radar), `.ARM.extab` 606
unchanged, total 73,899 -> 73,880 (-19). `check` green, RATCHET PASS.
