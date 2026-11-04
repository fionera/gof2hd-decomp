# Radar::draw pass (wave109, w5706) — planet-tail-before-recompute and the 64-byte gap

**Status:** research note, 2026-11-04. Worktree `wt/w5706`, branch `wave109-w5706`,
base `7c34c4098`. No source change landed (no variant beat the baseline); this
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
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Per-variant scores use the official verifier API
(`/dev/shm/w5706/measure.py`, `difflib.SequenceMatcher(None, on, un,
autojunk=False).ratio()` at six decimals). The rounded report score stays 74.0
for everything in 73.95..74.04.

## 1. REA re-confirmation: the original has ONE onPlanetCourse computation

`rea decompile` of the function (`/dev/shm/w5706/ghidra.c`) plus the raw Thumb
stream confirm the wave108 call-multiset result:

* exactly one `PlayerEgo::getAutoPilotTarget` call site, at `0x6a4`;
* exactly one write to `this[0x1a8]` (`strb.w r0,[r4,#424]`), at `0x180`;
* the `0x69e..0x6f6` cold block is the **else** branch of the *initial*
  `if (alienOrbit) ... else ...` computation (it ends `b.n 0x16c`, the
  `field_0x1a8 = onPlanetCourse` store).

So the post-planet-loop `if (!alienOrbit) { ... }` recompute in our source is
**not** in the original; `nodup` is the faithful call multiset.

## 2. Original vs ours block layout (function-relative)

Original:

```
0x2b4..0x2c2  first otherLock chain (flat cbnz/cmp chain) -> beq.w 0xb72
0x2c4..0x338  landmark guard + second otherLock chain      -> beq.w 0xb7e
0x33a..0x6f6  planet block (loop + initial onPlanetCourse cold half at 0x69e)
0x6f8..0x748  planet tail `if (!turretMode && !landmarkHit)`   <-- INLINE
0x74a..0x758  literal pool
0x758..0xaf2  landmark body (loop, hot)
0xaf2..0xb72  landmark tail
0xb72..0xb7e  cold block 1: otherLock = field_0x14 != 0
0xb7e..0xb8a  cold block 2: otherLock = lockedStation != 0
0xb8a..0xb9e  mission-type cold block
0xb9e..0xbcc  literal pool
0xbcc..0xbd4  planet-tail else (planetLockTimer = 0; field_0x18 = 0)
0xbd4..       enemies block
```

Ours (baseline):

```
0x33a..0x680  planet block
0x682..0x6da  initial onPlanetCourse cold half
0x6dc..0x72e  duplicate post-loop recompute (NOT in the original)
0x730..0xab0  landmark body (hot)
0xab0..0xb3c  landmark tail
0xb3c..0xb48  cold block 2
0xb48..0xb84  mission block + pool
0xb84..0xbe4  planet tail (COLD, split around the recompute compare)
0xbe6..       enemies block
0x25aa..      cold block 1 (end of function, r9)
```

## 3. New structural data point: tail-before-recompute

Moving the whole `if (!turretMode && !landmarkHit)` planet tail **before** the
`if (!alienOrbit)` recompute in source order (semantically safe for a
reconstruction: the tail then reads the initial `field_0x1a8`, exactly as the
original does) gives the best *structural* alignment seen so far for the two
competing hot blocks:

| block | original | `tail_before_dup` |
|---|---:|---:|
| planet tail | 0x6f8 | **0x6fa** (2 B off) |
| landmark body prologue | 0x758 | 0x798 |
| cold block 2 | 0xb7e | 0xba0 |
| enemies | 0xbd4 | 0xc04 |

usize **9948 B**, raw **70.293486 %**, norm 3492/3493.

This proves the two placements are individually reachable from source shape:
the planet tail can be inline (0x6fa) *and* the landmark body can stay hot
(0x798) at the same time. The residual is exactly the 80-byte duplicate
recompute block sitting between them (`0x746..0x796`); the landmark body is
`0x40` (64 B) late because of it.

Removing the recompute (`nodup`, 9756 B) makes the planet tail inline at
`0x708`, but clang then sinks the whole landmark loop to `0x13fe` (after the
enemies block) because the tail's fallthrough successor becomes the enemies
block. Adding `tail_before_dup`'s recompute restores the landmark body's
fallthrough chain — so the duplicate recompute is load-bearing for block
placement even though it is not faithful.

## 4. Measurements this wave (all on top of the landed source)

Baseline = **9908 B / 74.029509 % / 3492/3489**.

### 4.1 Planet-tail spellings (all neutral)

| variant | usize | raw % |
|---|---:|---:|
| baseline (`else if (field_0x14 == 0)`) | 9908 | **74.029509** |
| `else if (field_0x14 != 0)` (swapped bodies) | 9908 | 74.029509 |
| nested `else { if (field_0x14 == 0) ... }` | 9908 | 74.029509 |
| `goto enemies` in the hot else | 9908 | 74.029509 |
| `goto enemies` in the `!= 0` arm | 9908 | 74.029509 |
| `__builtin_expect(!turretMode && !landmarkHit, 1)` | 9904 | 71.281169 |

clang canonicalises every `else if`/`goto`/nested spelling to the same CFG; the
tail's ending (fallthrough vs `b.n enemies`) is decided by block placement, not
by the source spelling.

### 4.2 `otherLock` chain spellings

| variant | usize | raw % |
|---|---:|---:|
| landed nested-`__builtin_expect` form | 9908 | **74.029509** |
| plain `if (a && b && c)` | 9908 | 72.797593 |
| `int otherLock = 1; if (a && b && c) ...` | 9908 | 72.797593 |
| nested `if (a) { if (b) { if (c) ... } }` | 9908 | 72.797593 |
| `if (!(a != null || b != null || c != null))` | 9908 | 72.797593 |
| `if/else if/else if/else` chain | 9908 | 72.797593 |
| `otherLock = 1; if (...) otherLock = lockedStation != null` (2nd chain) | 9916 | 72.144289 |
| both chains in the `= 1; if (...)` form | 9916 | 72.029774 |

Every plain form is byte-identical to every other plain form; the only
difference from the original is register colouring (`otherLock` in `r9` vs the
original's `r5`) and the resulting `cbz`/`itt eq` vs `cbnz`/`cmp`/`beq` chain
shape.

### 4.3 Landmark hit-test / marked / stations spellings

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `i != 3 && !landmarkHit && ...` (original order) | 9904 | 72.216650 |
| same + plain `otherLock` | 9904 | 72.102020 |
| same + plain `otherLock` + bool `notOtherLock`/`notTurretMode` | 9900 | 72.638670 |
| same + plain `otherLock` + `bool marked` | 9924 | 70.928899 |
| `bool marked` + plain `otherLock` | 9928 | 71.051877 |
| `i <= 1` -> `i < 2` | 9908 | 74.029509 |
| `Array<int> *const stations` | 9908 | 74.029509 |
| `reinterpret_cast` stations spelling | 9908 | 74.029509 |
| `int otherLock;` declared before the route block / at function top | 9908 | 74.029509 |

`bool marked + plain otherLock` is the closest *cold-region* alignment found:
landmark body prologue **0x74e** (original 0x758), cold block 1 **0xb50**
(original 0xb72), cold block 2 0xb5c (original 0xb7e), mission block 0xb6a
(original 0xb8a), enemies 0xc0a (original 0xbd4). It is 20 B too large
(9928 vs 9908) and scores 71.05 %.

### 4.4 The faithful (`nodup`) family

| variant | usize | raw % |
|---|---:|---:|
| `nodup` | 9756 | 62.314026 |
| `nodup` + tail `!= 0` spelling | 9756 | 62.314026 |
| `nodup` + tail `__builtin_expect(...,1)` | 9764 | 62.353791 |
| `nodup` + `marked bool` + plain `otherLock` + tail `!= 0` | 9852 | 63.009359 |
| `tail_before_dup` (recompute kept, after the tail) | 9948 | 70.293486 |

## 5. Proven vs hypothesis

**Proven (measured):**
* No source spelling tried this wave beats **9908 B / 74.029509 %**.
* The original has exactly one `getAutoPilotTarget` call and one `field_0x1a8`
  write; the post-loop recompute is not faithful.
* Moving the planet tail before the recompute puts the tail at **0x6fa**
  (original 0x6f8) and keeps the landmark body hot at **0x798** (original
  0x758); the residual is exactly the 80-byte recompute block.
* Every `else if`/`goto`/nested spelling of the planet-tail else compiles to the
  same CFG (9908 B, 74.029509 %).
* Every plain `&&` spelling of the first `otherLock` chain compiles identically
  (9908 B, 72.797593 %); the difference from the original is register
  colouring, not source shape.
* `bool marked + plain otherLock` is the closest cold-region alignment
  (landmark 0x74e, cold1 0xb50) but is 20 B too large and scores 71.05 %.

**Hypothesis (not proven):**
* The original source is the faithful `nodup` shape; the 64-byte hot-region gap
  between `tail_before_dup` and the original is the *only* thing preventing a
  near-exact match, and it must be filled by faithful code that clang keeps
  between the planet tail and the landmark body (e.g. the fourth landmark
  `DrawString`, the six-call asteroid predicate, distance-first in-sight) —
  none of which individually lands in that gap in the current build.
* The remaining score is dominated by `MachineBlockPlacement` decisions
  (landmark off-screen blip block at 0x85c vs the original's 0x99a; cold block
  1 at 0x25aa vs 0xb72) that no local statement spelling reached this wave.

## 6. Evidence / scratch

All scratch under `/dev/shm/w5706` (not committed): `Radar.base.cpp` (pristine
source), `measure.py`, `dump2.py`, `keys.py`, `exp.py`, `try.py`, `run.sh`,
`orig.norm`, `ours.norm`, `rea.json`, `ghidra.c`, and the per-variant sources
(`v_nodup.cpp`, `v_marked_plain.cpp`, `v_tailfirst.cpp`, `v_decl1.cpp`,
`v_decl2.cpp`, `v1.json`..`v10.json`).
