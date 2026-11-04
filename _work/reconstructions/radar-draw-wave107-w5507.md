# Radar::draw pass (wave107, w5507) — planet/asteroid tail vs cold-block placement

**Status:** research note, 2026-11-04. Worktree `wt/w5507`, branch `wave107-w5507`,
base `f857b5171`. No source change landed (no variant beat the baseline); this
commit contains this note only.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.

## 0. Baseline and gate (unchanged source)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
cmake --build cmake-build-match --target lint -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  raw 74.029509 %  norm 3492/3489
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
lint:   CLEAN
```

Per-variant scores use the official verifier API (`/dev/shm/w5507/measure.py`,
`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`, printed at six
decimals; the rounded report score stays 74.0 for everything in 73.95..74.04).

## 1. The original's block layout (function-relative addresses)

```
0x2b4..0x2c2  first otherLock chain  (all-null -> beq.w 0xb72)
0x2c4..0x338  landmark guard + second otherLock chain (all-null -> beq.w 0xb7e)
0x33a..0x6f6  planet block (loop + shared autopilot cold half at 0x69e)
0x6f8..0x748  planet tail  `if (!turretMode && !landmarkHit)`   <-- INLINE
0x74a..0x758  literal pool
0x758..0xaf2  landmark body (loop)
0xaf2..0xb72  landmark tail (station lock)
0xb72..0xb7e  cold block 1: otherLock = field_0x14 != 0
0xb7e..0xb8a  cold block 2: otherLock = lockedStation != 0
0xb8a..0xb9e  missionBlocksPlanets block
0xb9e..0xbcc  literal pool
0xbcc..0xbd4  planet-tail else (planetLockTimer = 0; field_0x18 = 0)
0xbd4..       enemies block
```

Ours (baseline):

```
0x33a..0x6da  planet block
0x6dc..0x72e  duplicate autopilot recompute block
0x730..0xab0  landmark body
0xab0..0xb3c  landmark tail
0xb3c..0xb48  cold block 2
0xb48..0xb84  mission block + pool
0xb84..0xbe4  planet tail (whole, COLD)
0xbe6..       enemies block
0x25aa..      cold block 1 (end of function)
```

So the two structural divergences are: (a) the original's planet tail is inline
right after the planet loop, ours is sunk to the cold region; (b) the original
places **both** `otherLock` cold blocks immediately after the landmark tail, ours
places the first one at the very end of the function.

## 2. Measurements this wave (all on top of the landed source)

Baseline = **9908 B / 74.029509 % / 3492/3489**.

### 2.1 First `otherLock` chain (cold-block placement)

| variant | usize | raw % |
|---|---:|---:|
| landed (nested `__builtin_expect`) | 9908 | **74.029509** |
| plain `if (a==0 && b==0 && c==0)` | 9908 | 72.797593 |
| plain positive `if (a!=0 \|\| b!=0 \|\| c!=0) ... else` | 9908 | 72.797593 |
| nested `if` form | 9908 | 72.797593 |
| `int otherLock = 1; if (plain) ...` | 9908 | 72.797593 |
| ternary `noLock ? f14!=0 : 1` | 9908 | 72.797593 |
| `expect(whole, 0)` on the plain negative-AND | 9908 | 74.000859 |
| `expect(whole, 1)` on the positive-OR | 9908 | 74.000859 |
| `expect` on each positive term | 9912 | 70.915877 |
| `expect` on nested terms | 9912 | 70.915877 |

The **plain** form is the only one that puts cold block 1 at `0xb3c` (right
after the landmark tail, matching the original's *relative* placement); the
`expect` forms sink it to `0x261e`/`0x25aa` (end of function). But the plain form
loses 1.23 points because the absolute landmark-tail address is `0x36` early, so
every branch target in the rest of the function shifts.

### 2.2 Planet-tail / recompute placement

| variant | usize | raw % |
|---|---:|---:|
| landed (recompute before tail) | 9908 | **74.029509** |
| recompute moved **after** the tail | 9948 | 70.293486 |
| recompute moved to the **start of the tail** | 9908 | 73.889367 |
| recompute moved after the `field_0x14` block | 9912 | 72.607450 |
| recompute moved to the end of the tail | 9944 | 70.264853 |
| recompute moved before `enemies:` (outside campaign block) | 9936 | 62.067976 |
| recompute moved to the end of the function | 9856 | 47.973556 |
| recompute removed (`nodup`) | 9756 | 62.314026 |
| recompute removed + distance-first in-sight | 9808 | 56.458363 |
| recompute at tail start + plain `otherLock` | 9904 | 72.821101 |
| recompute at tail start + `expect(whole)` `otherLock` | 9908 | 73.860705 |
| recompute at tail start + distance-first in-sight | 9940 | 63.431443 |
| recompute at tail start, unconditional (small) | 9796 | 62.119683 |
| recompute at tail start, store only | 9764 | 62.113774 |

`recompute at tail start` is the best inline-tail shape: the planet tail lands
at `0x6e0` (original `0x6f8`) and the landmark body at `0x738` (original
`0x758`) — only `0x18`/`0x20` early — but the recompute's final comparison is
still sunk to `0xb88`, costing 0.14 points.

### 2.3 Enemy in-sight / vpush width

| variant | usize | raw % |
|---|---:|---:|
| landed (bounds first, `int cx/cy`) | 9908 | **74.029509** |
| `int inSight` | 9908 | 74.029509 |
| `bool inSight = bounds && dist` (direct) | 9908 | 73.972210 |
| direct assignment to `field_0x73` | 9924 | 63.350108 |
| distance-first (`VectorLength < 60000` first) | 9948 | 63.375431 |

Distance-first reproduces the original's `vpush {d8-d13}` (vs `{d8-d11}`) but
re-rolls the whole allocation and grows the body 40 B.

### 2.4 Assorted local spellings (all neutral or worse)

| variant | usize | raw % |
|---|---:|---:|
| `marked = 0` / `planet null` / `uint32_t` loop / `i != size` | 9908 | 74.029509 |
| `i <= 1` -> `i < 2` | 9908 | 74.029509 |
| `offset` if/else, blip-image local, landmark pointer local | 9908 | 74.029509 |
| planet-tail `!(tm \|\| lh)` / `tm==0 && lh==0` | 9908 | 74.029509 |
| music-tail `current != 151 && alwaysEnemySeen` re-spellings | 9908 | 74.029509 |
| `marked` as `bool` | 9928 | 71.023216 |
| `marked * 14` instead of `marked ? 14 : 0` | 9900 | 72.065358 |
| planet hit box y-first / reversed / `size*2` | 9908/9908/9904 | 73.857614 / 73.972210 / 72.112353 |
| landmark loop `uint32_t i` | 9920 | 72.841804 |
| landmark `if (i==2 \|\| ...)` reordered | 9868 | 65.767962 |
| `notOtherLock`/`notTurretMode` as `bool` | 9900 | 72.180020 / 70.487106 |
| `otherLock` as `uint8_t` | 9908 | 74.029509 |
| `alwaysEnemy` int / `wreck` int | 9904/9916 | 71.672159 / 72.011453 |
| `missionBlocksPlanets` uint8 / `planetHit` uint8 | 9904/9912 | 72.815812 / 71.477663 |
| landmark guard `expect(...,1)` / `expect(...,0)` | 9912/10068 | 63.117218 / 38.271431 |
| asteroid `!(hasCargo && dead) && dead` (faithful) | 9928 | 71.465369 |
| faithful: `nodup` + asteroid reorder + distance-first | 9828 | 56.479746 |
| faithful: asteroid reorder + distance-first | 9968 | 62.652330 |

## 3. Size accounting of the hot region

The original's hot region `0x33a..0xb72` is **764 instructions / 2104 bytes**;
ours `0x33a..0xb3c` is **747 / 2050** — 54 bytes smaller. The original's planet
loop body has 8 more instructions than ours and the landmark loop body 8 more.
The concrete per-instruction differences in the planet loop are:

* the `!otherLock && !planetHit && !turretMode` OR chain: the original emits
  three separate `ldr` + two `orrs`; ours folds two adjacent slots into one
  `ldrd` (the stack slots for `planetHit`/`turretMode` are swapped: original
  `turretMode=#148, planetHit=#152`, ours `planetHit=#148, turretMode=#152`);
* the original normalises `marked` to 0/1 (`cmp; it ne; movne`) before the
  `== 125` branch; ours does not;
* the original keeps `stations` in `sl` throughout; ours reloads it into `r6`.

The original's landmark loop condition is at the top of the loop (`0x784`),
ours is rotated to the bottom (`0xab8`), which moves the whole landmark body.

## 4. Proven vs hypothesis

**Proven (measured):**
* No local source spelling tried this wave beats **9908 B / 74.029509 %**.
* The original's planet tail is inline at `0x6f8`; the duplicate post-loop
  autopilot block is the obstacle that makes clang sink it (moving the block
  inside the tail start gets the tail inline but costs 0.14 points; moving it
  after the tail costs 40 B and 3.7 points).
* The plain `otherLock` form is the only spelling that places the first cold
  block after the landmark tail (`0xb3c`, original `0xb72`), but it loses 1.23
  points because the absolute hot-region size is still `0x36` short.
* Removing the duplicate autopilot block (the faithful source, since the
  original has one `getAutoPilotTarget` call at `0x6a4`) gives 9756 B / 62.31 %:
  the landmark body is then placed cold at `0x13fe`.
* Distance-first in-sight is the only spelling that reproduces the original's
  `vpush {d8-d13}`, but it is 40 B larger and scores 63.38 %.

**Hypothesis (not proven):**
* The residual is decided by the absolute size of the hot region. Adding the
  original's missing ~54 B of faithful code (the three `ldr` OR chain, the
  `marked` normalisation, the `stations`-in-`sl` allocation, and the unrotated
  landmark loop) *while keeping* the planet tail inline would align the
  landmark body at `0x758` and the enemies block at `0xbd4`, at which point the
  branch-target labels would match and the ratio should jump. No single local
  edit achieves that; it needs the original's exact TU/source shape or a
  different `-mllvm` block-placement configuration.
* The stack-slot rotation (`mode` `#100` vs `#112`, `textOffset` `#128` vs
  `#116`) is a register-allocator artifact of the same size/liveness difference.

## 5. Evidence / scratch

All scratch under `/dev/shm/w5507` (not committed): `Radar.base.cpp` (pristine
source), `measure.py`, `runvar.py`, `dumpaddr.py`, `showrange.py`, `diffreg2.py`,
`orig_a.dis`, `ours_a.dis`, plus the per-variant `ours_*.dis` dumps.
