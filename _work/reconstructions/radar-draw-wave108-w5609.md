# Radar::draw pass (wave108, w5609) — cold-block placement vs the faithful call multiset

**Status:** research note, 2026-11-04. Worktree `wt/w5609`, branch `wave108-w5609`,
base `8f98ec4d6`. No source change landed (no variant beat the baseline); this
commit contains this note only.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.

## 0. Baseline and gate (unchanged source)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  raw 74.029509 %  norm 3492/3489
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Per-variant scores use the official verifier API
(`/dev/shm/w5609/measure.py`, `difflib.SequenceMatcher(None, on, un,
autojunk=False).ratio()` at six decimals). The rounded report score stays 74.0
for everything in 73.95..74.04.

## 1. The original's block layout (function-relative addresses)

```
0x2b4..0x2c2  first otherLock chain  (all-null -> beq.w 0xb72)
0x2c4..0x338  landmark guard + second otherLock chain (all-null -> beq.w 0xb7e)
0x33a..0x6f6  planet block + initial onPlanetCourse cold half at 0x69e
0x6f8..0x748  planet tail  `if (!turretMode && !landmarkHit)`   <-- INLINE
0x74a..0x758  literal pool
0x758..0xaf2  landmark body (loop)
0xaf2..0xb72  landmark tail (station lock)
0xb72..0xb7e  cold block 1: otherLock = field_0x14 != 0   (r5)
0xb7e..0xb8a  cold block 2: otherLock = lockedStation != 0
0xb8a..0xb9e  mission-type cold block
0xb9e..0xbcc  literal pool
0xbcc..0xbd4  planet-tail else (planetLockTimer = 0; field_0x18 = 0)
0xbd4..       enemies block
```

Ours (baseline):

```
0x2ac..0x2e4  first otherLock chain (expect form, otherLock in r9)
0x2e8..0x338  landmark guard + second otherLock chain
0x33a..0x6da  planet block
0x6dc..0x72e  duplicate autopilot recompute block   <-- NOT in the original
0x730..0xab0  landmark body (loop, hot)
0xab0..0xb3c  landmark tail
0xb3c..0xb48  cold block 2
0xb48..0xb84  mission block + pool
0xb84..0xbe4  planet tail (whole, COLD)
0xbe6..       enemies block
0x25aa..      cold block 1 (end of function, r9)
```

The two structural divergences are the same as wave107: (a) the original's
planet tail is inline, ours is sunk to the cold region; (b) the original places
both `otherLock` cold blocks immediately after the landmark tail, ours places
the first one at the very end of the function.

## 2. Call-multiset evidence (the duplicate recompute is not faithful)

Baseline call counts, original vs ours:

```
DrawString           9 vs 8     (original has ONE MORE)
getStations          2 vs 3
getStationEnumIndex  1 vs 2
getPlayer           29 vs 32
getSystem            5 vs 7
isDead              17 vs 16    (original has ONE MORE)
isDying             13 vs 12    (original has ONE MORE)
goingToPlanet        1 vs 2
isDockingToPlanet    4 vs 5
getAutoPilotTarget   1 vs 2
```

The `+1` on `goingToPlanet`, `isDockingToPlanet`, `getAutoPilotTarget`,
`getStations`, `getStationEnumIndex` and the `+2` on `getSystem` are exactly the
post-planet-loop `if (!alienOrbit)` recompute block. REA confirms it: the
decompiled function has a single `PlayerEgo::getAutoPilotTarget` call (in the
initial onPlanetCourse cold block at `0x69e`) and a single write to
`this[0x1a8]` (`ghidra.c` line 198). So the faithful source has **no**
post-loop recompute; `nodup` is the faithful shape.

The remaining `+1` `getPlayer` and the `+1` `isDead` / `isDying` are the
faithful asteroid predicate (see §4). The extra `DrawString` is the landmark
`i<=1` else-branch call, which clang tail-merges with the `i==0 && !alienOrbit`
special block's second `DrawString` in our build (original keeps 4 separate call
sites in the landmark section: `0x90a`, `0x990`, `0xa5e`, `0xae6`; ours has 3:
`0x91e`, `0xa2e`, `0xab6`).

## 3. Measurements this wave (all on top of the landed source)

Baseline = **9908 B / 74.029509 % / 3492/3489**.

### 3.1 The faithful shape (`nodup`) and its block-placement problem

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `nodup` (remove post-loop recompute) | 9756 | 62.314026 |
| `nodup` + plain `otherLock` | 9788 | 61.735871 |
| `nodup` + no onScreen expect | 9744 | 66.580012 |
| `nodup` + all expects plain | 9732 | 67.311580 |
| `nodup` + plain `otherLock` + all expects plain | 9756 | 67.601561 |
| `nodup` + `marked` normalization | 9780 | 61.944605 |
| `nodup` + extra landmark DrawString | 9756 | 62.314026 |
| `nodup` + asteroid 6-call predicate | 9776 | 61.924686 |
| `nodup` + distance-first in-sight | 9788 | 50.856731 |
| `nodup` + asteroid + distance-first | 9808 | 50.747986 |

With `nodup`, the planet tail becomes inline at `0x708` (original `0x6f8`) and
the landmark body prologue lands at `0x754` (original `0x758`) — the hot region
is then within 18 B of the original. But clang decides the landmark **loop** is
cold and sinks it to `0x13fe`, right after the enemies block, which destroys the
instruction-stream alignment. Removing the `__builtin_expect` hints moves the
landmark loop back hot (`0x6c2`) but then sinks the planet tail instead.

### 3.2 `otherLock` chain (cold-block placement) on the baseline

| variant | usize | raw % |
|---|---:|---:|
| landed (nested `__builtin_expect`, cold block 1 at `0x25aa`) | 9908 | **74.029509** |
| plain negative-AND (cold block 1 at `0xb3c`) | 9908 | 72.797593 |
| positive OR (cold block 1 at `0xb3c`) | 9908 | 72.797593 |
| `expect(whole,0)` on plain negative-AND | 9908 | 74.000859 |
| `expect(whole,1)` on plain negative-AND | 9912 | 70.915877 |
| `expect(whole,0)` + inner `expect(b,0)` | 9900 | 71.692705 |
| `expect(whole,1)` + inner `expect(b,1)` | 9912 | 70.915877 |
| outer `expect` only (no inner) | 9908 | 74.000859 |
| `expect(a,1)` / `expect(c,1)` only | 9904/9892 | 71.680275 / 71.625107 |
| outer + `expect(a,1)` + `expect(c,1)` | 9900 | 72.836676 |
| `bool otherLock` | 9880 | 67.853042 |

The landed nested-expect form is the unique 74.03 shape. Every plain form puts
cold block 1 at the right *relative* place (`0xb3c`, after the landmark tail)
but loses 1.23 points because the absolute hot region is `0x36` (54 B) short.

### 3.3 Planet-tail / recompute placement

| variant | usize | raw % |
|---|---:|---:|
| landed (recompute before tail) | 9908 | **74.029509** |
| recompute at tail start (`tailstart_expect`) | 9908 | 73.889367 |
| recompute at tail start + plain `otherLock` | 9904 | 72.821101 |
| recompute at tail start + `marked` normalization | 9936 | 71.827157 |
| recompute at tail start + asteroid predicate | 9928 | 71.438797 |
| recompute moved before planet block | 9868 | 60.879563 |
| recompute moved after tail (before `enemies:`) | 9936 | 62.067976 |
| tail `else` re-spellings (`swap`/`nested`/`goto`/`dowhile`) | 9908 | 74.029509 (all neutral) |

### 3.4 Assorted local spellings (all neutral or worse)

| variant | usize | raw % |
|---|---:|---:|
| `marked` explicit normalization | 9936 | 71.853547 |
| local `labelY` in landmark else DrawString | 9908 | 74.029509 |
| local `labelY2` in special second DrawString | 9908 | 74.029509 |
| both DrawString locals | 9908 | 74.029509 |
| `int` for `landmarkHit`/`planetHit`/`enemyHit`/`bossSeen` etc. | 9880..9936 | 66.1..72.3 |
| `bool` for `alwaysEnemySeen`/`wreck`/`inSight`/`gasHit`/`asteroidHit` | 9880..9936 | 66.1..72.3 |
| `int hasDockNav` / `int navWreck` | 9908 | 74.029509 |
| `int missionBlocksPlanets` / `int turretMode` | 9904/9888 | 72.815812 / 66.169511 |
| `int alienOrbit` | 9908 | 74.018906 |
| `int campaign` | 8784 | 24.61 |
| `marked * 14` instead of `marked ? 14 : 0` | 9900 | 72.065358 |
| `planet hit box y-first` / reversed / `size*2` | 9908/9908/9904 | 73.857614 / 73.972210 / 72.112353 |
| landmark loop `uint32_t i` | 9920 | 72.841804 |
| landmark `if (i==2 \|\| ...)` reordered | 9868 | 65.767962 |
| `notOtherLock`/`notTurretMode` as `bool` | 9900 | 72.180020 / 70.487106 |
| `otherLock` as `uint8_t` | 9908 | 74.029509 |
| `alwaysEnemy` int / `wreck` int | 9904/9916 | 71.672159 / 72.011453 |
| asteroid `!(hasCargo && dead) && dead` (faithful) | 9928 | 71.465369 |
| faithful: `nodup` + asteroid reorder + distance-first | 9828 | 56.479746 |
| faithful: asteroid reorder + distance-first | 9968 | 62.652330 |

## 4. The faithful asteroid predicate (proven by disassembly)

Original `0x1a56..0x1a84`:

```
1a56: ldrb r0,[r6,#76]      ; hasCargo
1a5a: cbz  r0, 0x1a6c
1a5c: isDead();  cbnz -> 0x1a84
1a64: isDying(); cbnz -> 0x1a84
1a6c: isDead();  cmp #0; bne -> 0x1cd2   ; continue
1a78: isDying(); cmp #0; bne -> 0x1cd2   ; continue
1a84: update(asteroid)
```

i.e. `if (!(hasCargo && (isDead() || isDying())) && (isDead() || isDying())) continue;`
— six `isDead`/`isDying` calls, with the `hasCargo` test first. The landed
source computes `dead` once and reuses it (four calls), which is 8 B smaller.
The faithful 6-call form is 9928 B / 71.90 % (or 9928 B / 71.47 % for the
`!(hasCargo && dead) && dead` spelling), both below the baseline because the
body grows and shifts the cold block 1 / music tail.

## 5. Proven vs hypothesis

**Proven (measured):**
* No single local source spelling tried this wave beats **9908 B / 74.029509 %**.
* `nodup` is the faithful call multiset (one `getAutoPilotTarget`), and it makes
  the planet tail inline at `0x708` and the landmark body prologue at `0x754`
  (original `0x6f8` / `0x758`), but clang then sinks the landmark **loop** cold
  (`0x13fe`), costing 11.7 points.
* The landed nested-`__builtin_expect` `otherLock` form is the only 74.03 shape;
  the plain form puts cold block 1 at the right relative place (`0xb3c`) but the
  absolute hot region is 54 B short, costing 1.23 points.
* The original's landmark section has four `DrawString` call sites; ours has
  three because clang tail-merges the `i<=1` else-branch call with the special
  block's second call (`0xab6`). Local `labelY`/`labelY2` spellings do not
  prevent the merge.
* The original's asteroid predicate is the six-call
  `!(hasCargo && (isDead() || isDying())) && (isDead() || isDying())` form.

**Hypothesis (not proven):**
* The original source is the faithful `nodup` shape plus the six-call asteroid
  predicate, the fourth landmark `DrawString`, and distance-first in-sight; the
  hot region then needs ~18 B more faithful code (the `marked` normalization
  and the unrotated landmark loop) to sit exactly at `0x758`, at which point
  clang keeps the landmark loop hot and the score should jump.
* Whether clang keeps the landmark loop hot is decided by the
  `__builtin_expect` polarity on the `otherLock` chain: the landed expect keeps
  the loop hot but sinks cold block 1; removing it un-sinks cold block 1 but
  sinks the planet tail. No local spelling tried here achieves both.

## 6. Evidence / scratch

All scratch under `/dev/shm/w5609` (not committed): `Radar.base.cpp` (pristine
source), `measure.py`, `try.sh`, `redump.sh`, `dump.py`, `view.py`, `count.py`,
`randsearch.py`, `orig.dis`, `ours.dis`, `orig.norm`, `ours.norm`, `diff.txt`,
`ghidra.c`, and the per-variant `*.cpp` files (`nodup`, `nd_*`, `ol_*`, `ola_*`,
`ts_*`, `tail_*`, `lbl_*`, `comb_*`, `ast_*`, `idf_*`, `mk_*`, …).
