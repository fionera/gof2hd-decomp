# Radar::draw wave 190 (w13398) — 24000-box `>` before `<` (original order);
# raw neutral, aligned 1,828 -> 1,841 (size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13398`, branch `wave190-w13398`, base
`master` `6493e7e74`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13381.md` (negative-result record), `-w13370.md`,
`-w13371.md`, `radar-draw-wave189-w13346.md` and `radar-draw-wave156-w10205.md`.

## 0. The change

The enemy-loop 24,000-unit box test evaluated the lower bound before the upper bound
in every axis. The original evaluates the **upper** bound first (`bgt` then `bmi` at
`12ee3a..12ee4c`); ours emitted `bmi` then `bgt`. Source edit (line 714):

```cpp
-                if (!(this->egoPos.x - this->targetPos.x < -24000.0f) && !(this->egoPos.x - this->targetPos.x > 24000.0f) &&
-                    !(this->egoPos.y - this->targetPos.y < -24000.0f) && !(this->egoPos.y - this->targetPos.y > 24000.0f) &&
-                    !(this->egoPos.z - this->targetPos.z < -24000.0f) && !(this->egoPos.z - this->targetPos.z > 24000.0f) &&
+                if (!(this->egoPos.x - this->targetPos.x > 24000.0f) && !(this->egoPos.x - this->targetPos.x < -24000.0f) &&
+                    !(this->egoPos.y - this->targetPos.y > 24000.0f) && !(this->egoPos.y - this->targetPos.y < -24000.0f) &&
+                    !(this->egoPos.z - this->targetPos.z > 24000.0f) && !(this->egoPos.z - this->targetPos.z < -24000.0f) &&
                     enemy->getType() != 16917) {
```

Pure operand swap on a float comparison chain; semantics unchanged.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,291 | 6,291 |
| instruction-index alignment | **1,828** | **1,841** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 73,007 | 73,007 |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,630 | 73,630 |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370. The function is already
non-byte-exact, so `byte_exact` is unchanged; the gain is the structural alignment of
the enemy-loop box test (the original's branch order).

## 1. Other cells measured this pass (all reverted)

Full incremental `link_so -j2` + `tools/fn_diff.py` (`ulimit -v 12000000`;
scratch `/home/debian/GalaxyOnFire2/scratch/w13398/`).

### 1.1 Asteroid-loop tractor guard (`field_0x1c == nullptr && wreck && tractorMode2`)

The original at `12fcb8` evaluates `tractorMode2`, then `field_0x1c`, then `wreck`, and
if-converts the body with `itttt eq`. All reorderings were measured (6 permutations):

| ordering | raw | aligned | size |
|---|---:|---:|---:|
| base (`1c, wreck, tractorMode2`) | **6,291** | 1,828 | 9,908 |
| `tractorMode2, 1c, wreck` | 6,293 | 1,818 | 9,908 |
| `tractorMode2, wreck, 1c` | 6,293 | 1,818 | 9,908 |
| `1c, tractorMode2, wreck` | 7,545 | 1,904 | 9,904 |
| `wreck, 1c, tractorMode2` | 7,545 | 1,902 | 9,904 |
| `wreck, tractorMode2, 1c` | 7,538 | 1,959 | 9,940 |

Nested `if`s, `!ptr`, `== 0`, and `__builtin_expect(...,0/1)` spellings all land at
6,293 / 1,818. The original's 26-byte block (branch on `tractorMode2`, branch on
`field_0x1c`, `eors`+`itttt eq` body) could not be reproduced; clang always folds
`field_0x1c == nullptr && wreck` into an `orrs` boolean (32-byte block).

### 1.2 Asteroid wreck x/y block (`field_0x11d/0x11e/0x11f/0x120`)

The original rematerialises `screenX` in four arms (`12fc6a/70/88/94`); ours CSE-hoists
it to one load at `12fc5e`, making the block 12 B shorter. Forcing a volatile read in
any single arm makes clang drop the hoist and emit all four loads: the block then
matches the original's size and end address (`12fcac`) exactly, but the function grows
+12 B (9,920) and raw worsens to ~6,930 because the tail is still 6 B early (the
following tractor block is 6 B longer than the original's). Measured variants:
`volB_11e` 6,930/9,920, `volC` 6,924/9,920, `volD`/`volE` 6,964/9,920, first-arm
volatile byte-identical, `int x = screenX; int y = screenY;` 7,050/9,916,
`y`-only init 7,144/9,928, branch reorders 6,291/9,908 or 7,008/9,904.

### 1.3 Music-tail condition order (`(current-136)>15 || (current!=151 && alwaysEnemySeen) || bitmask==0`)

The original evaluates the range test, then the bitmask, then `current != 151 &&
alwaysEnemySeen` (`130280..1302a4`). All 5 non-base orderings measured: best
`(range, bitmask, expect)` = 6,295 / 1,829 (worse); others 6,314 / 8,499 / 8,504.
Inverting `if (current == 143) return;` into `if (current != 143) { ... }` is
byte-identical.

### 1.4 Prologue declaration order / `otherLock` type

Moving `int campaign = ...` before `mission`, `missionBlocksPlanets`, or `turretMode`
puts `missionBlocksPlanets` in `fp` (matching the original's `mov.w fp,#0` at
`12e07a`) but always shrinks the frame by 8 B (9,900). `bool otherLock` alone puts
*campaign* in `sl` and `missionBlocksPlanets` in `fp` (the full original skeleton) but
is 48 B short (9,860, 24 fewer instructions). `unsigned char`/`signed char`/`short`/
`long`/`unsigned`/`uint8_t otherLock` are all byte-identical to base; only `bool`
changes codegen. `bool missionBlocksPlanets` = 6,428/9,912; `unsigned char` =
byte-identical.

### 1.5 Type toggles (single, on current base)

`bool`/`int` toggles for `onPlanetCourse` (identical), `wreck` (identical),
`campaign` (9,650/8,576), `turretMode` (9,206/9,860), `alienOrbit` (6,300/9,908),
`otherLockFlag` (9,416/9,900), `planetHit` (9,176/9,952), `enemyHit` (8,612/9,940),
`bossSeen` (9,084/9,936), `marked` (8,426/9,912), `alwaysEnemySeen` (9,054/9,924),
`alwaysEnemy` (8,573/9,920).

## 2. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6291 aligned= 1841 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13398/`: `Radar.base.cpp` (6,291 base),
`batch.py` / `runvar.py`, `m24.json`, `astperm.json`, `wreck*.json`, `music.json`,
`oltypes.json`, `decl*.json`, `types.json`, `lcs.py`, `regdiff.py`.

## Current-master harvest (wave 190)

Applied on master `21242c17f`: raw stays **6,291**, aligned **1,828 -> 1,841**
(+13), size exact 9,908 B. `.text` **73,000 -> 72,999** (-1, combined with
the w13384 LevelScript fresh-d1 win); `.ARM.extab` 603 and build-id 20
unchanged. `check` -> RATCHET PASS.
