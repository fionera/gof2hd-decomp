# Radar::draw pass 13 — the ARM constant-pool island is the missing layout key,
# and `dupident` + block moves cannot hold 9908 B (w4704)

**Status:** research note, 2026-11-03. Worktree `wt/w4704`, branch `wave97-w4704`,
base `9b58a39d5`. **No source change landed**: no variant that keeps `Radar::draw`
at 9908 bytes beats the baseline verifier ratio (0.73925501). This commit contains
this note only.

## 0. Baseline (measured before and after this pass)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN5Radar4drawEP6PlayerP3Hudi`:

```
osize 9908  usize 9908  byte False linked False
raw SequenceMatcher ratio 73.925501 %   normalized stream 3492 (orig) / 3488 (ours)
```

`Radar.cpp` still carries the scoped `-mllvm -likely-branch-weight=80
-mllvm -unlikely-branch-weight=1` override (`CMakeLists.txt:86-87`).

## 1. What pass 12 left open, and what this pass adds

Pass 12 (`radar-draw-pass12-w4506.md`) proved the duplicate campaign autopilot
block is extra (one `strb [this,#0x1a8]` in the original, two in ours) and listed
three next steps:

1. `dupident` + move the duplicate block after the planet-lock tail,
2. a "semi-identical" duplicate that keeps `goingToPlanet`/`isDockingToPlanet`
   and `getStarSystem()->getPlanetTargets()` so it is not tail-merged,
3. `delcamp` + remove the 80:1 override.

All three are measured here (§3, §4) and none improves. This pass adds one new
structural finding that explains *why* none of them can: **the ARM constant-pool
island placement** (§2), plus a 60-variant sweep (§5) including every
`-mllvm` branch-weight value from 2 to 2000 (§5.6).

## 2. New: the constant-pool island is the missing layout key

The planet loop's constant pool holds `StarSystem::orbitPlanetIndex`,
`PlayerEgo::crosshairPos` (twice), `Globals::font` and the four float literals
`0x46bb8000 / 0x476a6000 / 0xc6bb8000 / 0x42c80000`. Its position in the function
is the single biggest block-placement difference between the original and every
variant we can produce:

| build | pool address (fn-relative) | what immediately precedes the pool | what immediately follows |
|---|---|---|---|
| **original** | **0x74c** | planet-lock tail (`b.n 0xbd4` @ 0x748) | landmark loop @ 0x758 |
| base (landed) | 0x84c | planet-hit block (`b.n 0x88a` @ 0x84a) | rest of the planet loop body @ 0x85c |
| `delcamp` | 0x6ec | top-level autopilot cold block (`b.n 0x16c` @ 0x6ea) | planet-lock tail @ 0x708 |
| `dupident` | 0x6dc | top-level autopilot cold block (`b.n 0x16c` @ 0x6da) | duplicate autopilot block @ 0x6f8 |

The original places the island **after the planet-lock tail and immediately
before the landmark loop**, because the landmark loop is the next large user of
`Globals::font` / `crosshairPos` and it is laid out at 0x758. In the base build
the landmark loop *is* also laid out at 0x774 (close to the original), but the
island is pulled back into the middle of the planet loop body at 0x84c. In
`delcamp` the landmark loop is pushed to 0x1438 (after the enemies block), so the
island settles at 0x6ec. This is `ARMConstantIslandPass`'s water heuristic
reacting to `MachineBlockPlacement`; it is not reachable from a statement
reordering, which is why every "remove the duplicate block" variant re-rolls the
whole layout.

The original's planet-lock tail is therefore *sandwiched* between two
unconditional branches:

```
0x69e .. 0x6f6   top-level autopilot cold block   (ends b.n 0x16c)
0x6f8 .. 0x748   planet-lock tail                 (ends b.n 0xbd4)
0x74a            nop
0x74c .. 0x758   constant-pool island
0x758 ..         landmark loop
```

and the loop-exit branch `0x38c bcs.w 0x6f8` jumps *over* the cold block onto the
planet tail. Reproducing that exact sandwich is the whole problem.

## 3. `dupident` + block moves (pass-12 step 1)

`dupident` = the base source with the duplicate block rewritten to be identical
to the top-level else-branch body (`targets = getStarSystem()->getPlanetTargets()`,
no `goingToPlanet`/`isDockingToPlanet`). It is the only variant found so far
whose loop-exit branch has the original's target address:

```
dupident: 0x038c  bcs.w  0x6f8        <- exactly the original's target
base:     0x038c  bcs.w  0x6dc
```

but the code at 0x6f8 is the *duplicate block*, not the planet tail, and the
duplicate block's tail is merged with the top-level block (7 calls collapse), so
the body shrinks to 9888 B / 65.58 %.

Moving the duplicate block around the campaign block (all official verifier):

| variant | usize | ratio | stream | loop-exit target |
|---|---:|---:|---|---|
| base | 9908 | **73.925501** | 3492/3488 | 0x6dc (duplicate block) |
| `dupident` in place | 9888 | 65.576123 | 3492/3477 | 0x6f8 (duplicate block) |
| `dupident` before the loop | 9836 | 61.985612 | 3492/3458 | — |
| `dupident` at campaign start | 9836 | 61.985612 | 3492/3458 | — |
| `dupident` after the planet tail | 9936 | 62.921026 | 3492/3485 | 0x718 (planet tail) |
| `dupident` after the campaign `if` | 9908 | 62.510773 | 3492/3470 | — |
| full duplicate before the loop | 9856 | 63.908046 | 3492/3468 | — |
| full duplicate at campaign start | 9844 | 64.135749 | 3492/3462 | — |
| full duplicate after the planet tail | 9948 | 70.189003 | 3492/3492 | 0x708 (planet tail) |
| full duplicate after the campaign `if` | 9932 | 62.008608 | 3492/3478 | — |

Note the "full duplicate after the planet tail" row: it reproduces the original's
instruction *count* exactly (3492/3492) but is 40 B too big and the block order is
wrong. Moving the duplicate after the tail does move the loop-exit target onto
the planet tail (0x718 / 0x708), but the planet tail no longer lands on 0x6f8
because the loop body grows (0x38c → 0x3a0) and the cold block shrinks
(0x5a → 0x50) as soon as the duplicate block stops being the loop-exit successor.

## 4. The "semi-identical" duplicate (pass-12 step 2)

Keeping `goingToPlanet`/`isDockingToPlanet` and the
`getStarSystem()->getPlanetTargets()` call, but simplifying the final assignment
to `autoPilotTarget == targets->data()[stationIndex]`:

| variant | usize | ratio | stream |
|---|---:|---:|---|
| base (full duplicate) | 9908 | **73.925501** | 3492/3488 |
| semi (keeps `goingToPlanet`/`isDocking`, `targets` local) | 9924 | 72.576257 | 3492/3491 |
| semi + after the planet tail | 9956 | 70.187491 | 3492/3495 |
| semi2 (keeps `goingToPlanet`/`isDocking`, `this->planetTargets`) | 9908 | 73.925501 | 3492/3488 |

`semi2` is byte-identical to the base build (the `targets` local is the only
difference, and it is not used in the base). `semi` grows 16 B and loses 1.35
points. Neither reaches the original.

## 5. Variant sweep (official verifier, `autojunk=False`; baseline 9908 / 73.925501)

### 5.1 Enemy in-sight test (the `vpush {d8-d13}` lever)

The original keeps `crosshairPos.x`/`.y` in `s26`/`s24` across the
`AEMath::operator-` call (hence `vpush {d8-d13}`); ours converts to `int` first
and only ever uses `s16..s22` (`vpush {d8-d11}`).

| variant | usize | ratio |
|---|---:|---:|
| baseline (bounds-first, interleaved, `int cx/cy`) | 9908 | **73.925501** |
| distance-first, x-pair/y-pair, `int cx/cy` | 9924 | 71.189352 |
| bounds x-pair/y-pair, distance last, `int cx/cy` | 9936 | 70.711117 |
| distance-first, inline `(int)crosshairPos.x` | 9948 | 62.157116 |
| bounds x-pair/y-pair, inline `(int)crosshairPos.x` | 9928 | 72.651775 |

### 5.2 Asteroid dead predicate (the `isDead`/`isDying` CSE)

| variant | usize | ratio |
|---|---:|---:|
| baseline (`bool dead = ...; if (dead && !hasCargo)`) | 9908 | **73.925501** |
| `if (!hasCargo && dead)` | 9908 | 73.771666 |
| `!(hasCargo && (isDead||isDying)) && (isDead||isDying)` (original 4-call shape) | 9928 | 71.246601 |
| nested `if (!hasCargo) { if (isDead||isDying) continue; }` | 9912 | 71.956459 |

### 5.3 First `otherLock` chain

| variant | usize | ratio |
|---|---:|---:|
| baseline (negative AND + `expect(...,0)`) | 9908 | **73.925501** |
| positive OR + `expect(...,0)` | 9912 | 70.782484 |
| positive OR, no `expect` | 9904 | 72.169676 |
| positive OR + `expect(...,1)` | 9908 | 73.925501 |
| `expect(!(A||B||C), 0)` | 9908 | 73.925501 |

### 5.4 Landmark-loop `DrawString` tail merge

The original keeps two separate `blx DrawString` sites for the else branch and
the TecLevel branch; ours tail-merges them (-1 call). Trying to break the merge
from source:

| variant | usize | ratio |
|---|---:|---:|
| baseline | 9908 | **73.925501** |
| else branch with explicit `i == 0 ? originX : originY` | 9920 | 73.435486 |
| else branch with a local `int off` | 9920 | 73.435486 |
| `y` operand order swapped (`field_0x4 + screenY`) | 9908 | 73.925501 |
| `y` via `layoutField(4)` helper | 9908 | 73.925501 |
| `x` operand order swapped (`offset + screenX`) | 9908 | 73.925501 |
| TecLevel `y` operand order swapped | 9908 | 73.925501 |
| `if (i <= 1)` -> `if (i < 2)` | 9908 | 73.925501 |
| `if (i <= 1)` -> `if (i == 0 \|\| i == 1)` | 9908 | 73.925501 |

The commutative-add rewrites are canonicalised away; the explicit-ternary forms
cost 12 B and 0.5 points. `-mllvm -tail-merge-size=0/1` and
`-mllvm -tail-dup-size=1` are all neutral; `-mllvm -tail-merge-threshold=1` and
`-mllvm -enable-tail-merge=false` blow the body up to 10444 B / 54.96 %.

### 5.5 Statement / declaration shapes (all neutral unless noted)

| variant | usize | ratio |
|---|---:|---:|
| duplicate block with the `if (!alienOrbit)` wrapper removed | 9908 | 73.925501 |
| duplicate block as `if (alienOrbit) {} else { ... }` | 9908 | 73.925501 |
| `if (!turretMode && !landmarkHit)` -> nested `if (!turretMode) { if (!landmarkHit)` | 9908 | 73.925501 |
| `!turretMode && !landmarkHit` -> `!landmarkHit && !turretMode` | 9908 | 73.925501 |
| `int offset` moved after the `DrawImage2D` | 9908 | 73.925501 |
| `bool marked` | 9928 | 70.890067 |
| `int planetHit` | 9896 | 71.301606 |
| `isDockingToPlanet()` as a nested `if` instead of `&&` | 9908 | 73.925501 |
| function-scope `planetHit` (3 positions) | 9908 | 73.925501 |
| function-scope `stations` uninitialised (4 positions) | 9908 | 73.925501 |
| function-scope `stations = nullptr` (2 positions) | 9908 | 73.868195 |
| function-scope `planetHit` + `stations` | 9908 | 73.925501 |
| landmark skip-test reorders (3) | 9864-9908 | 65.88-73.896848 |
| landmark hit-condition reorders (5) | 9904-9924 | 68.65-73.753582 |
| duplicate wrapper `if (this->planetTargets != nullptr)` | 9908 | 73.727963 |
| duplicate wrapper `if (campaign >= 2)` / `if (1)` / `if (!turretMode \|\| turretMode)` | 9908 | 73.925501 |

### 5.6 `__builtin_expect` additions and branch-weight sweep

Adding an `expect` on the planet-hit condition or on the inner bounds test is
always worse:

| variant | usize | ratio |
|---|---:|---:|
| `expect(!otherLock && !planetHit && !turretMode, 0)` | 9904 | 69.944102 |
| `expect(!otherLock && !planetHit && !turretMode, 1)` | 9900 | 71.522799 |
| `expect(inner bounds, 0)` | 9900 | 70.603411 |
| `expect(inner bounds, 1)` | 9900 | 70.527523 |

Per-file `-mllvm -likely-branch-weight=W -mllvm -unlikely-branch-weight=1`
(the landed value is W=80):

| W | usize | ratio |
|---|---:|---:|
| 2 | 9900 | 72.560539 |
| 4 | 9904 | 70.783780 |
| 8 | 9904 | 70.869752 |
| 16 | 9904 | 72.255661 |
| 32 | 9904 | 70.869752 |
| 64 | 9908 | 73.810888 |
| 70 | 9908 | 73.810888 |
| 72, 73 | 9908 | 73.925501 |
| 74, 80, 82, 90, 100, 105, 110 | 9908 | **73.925501** |
| 75, 76, 78 | 9908 | 72.539751 |
| 112, 120 | 9908 | 72.511102 |
| 128 | 9908 | 73.896848 |
| 256 | 9908 | 72.511102 |
| 2000 (default) | 9908 | 72.482452 |

The plateau is not contiguous (72-74 and 80-110 are good; 75-78 dip to 72.54),
but no value beats 73.925501. This confirms the 80:1 choice is optimal and that
branch weights are not the remaining lever.

## 6. Conclusion

* The statement set is unchanged from pass 12 and still correct: the duplicate
  campaign autopilot block is extra; the original has exactly one
  `strb [this,#0x1a8]`.
* The residual is **not** a missing statement. It is:
  1. **the ARM constant-pool island placement** (§2) — the original's island is
     after the planet tail and before the landmark loop; every variant we can
     produce puts it elsewhere, which re-rolls the whole layout;
  2. the loop-exit target (planet tail vs duplicate block);
  3. `otherLock#1`'s cold half at 0x2480 (ours) vs 0xb72 (orig);
  4. the `planetHit`/`turretMode` stack-slot swap (`#148`/`#152`) and the
     `mode` slot rotation (`#100` orig vs `#112` ours);
  5. `stations` parked in `r6` instead of `sl` (+1 `mov` in the campaign
     preamble);
  6. the loop latch inline (orig) vs out of line (ours);
  7. `vpush {d8-d11}` vs `{d8-d13}` (needs the crosshair floats live across
     `AEMath::operator-`, which costs 16 B and 2.7 points).
* The landed baseline is a strong local optimum: ~200 variants across three
  passes (w4406 §7, w4506 §6, and this note §3-§5) leave it at 9908 / 73.9255.

### What remains

1. A source shape that puts the planet-lock tail immediately after the top-level
   autopilot cold block **and** keeps the body at 9908 B. `dupident` gets the
   loop-exit address right (0x6f8) but the wrong block lands there; moving the
   duplicate after the tail moves the target to 0x718/0x708. The gap is the
   0x14-byte loop-body growth plus the 0xa-byte cold-block shrink.
2. A source shape that makes `getStations()` land directly in `sl` (one fewer
   live value at the campaign-block preamble). Function-scope `stations` is
   neutral (uninitialised) or -0.06 points (`= nullptr`).
3. A source shape that reproduces the original's `s24`/`s26` crosshair floats
   **without** reordering the screen tests.

## 7. Evidence / scratch (all under `/dev/shm/w4704`, not committed)

* `measure.py` — official-verifier ratio for one symbol (same API as
  `tools/verify/verify.py`, `autojunk=False`).
* `try.sh`, `tryflag.sh` — copy source / patch `CMakeLists.txt`, build, measure,
  restore. Both `sleep 0.05` after the copy: a same-second `cp` lets `make` skip
  the recompile and silently re-measure the previous variant.
* `gen.py`, `gen2.py` … `gen13.py` — the variant generators in §3-§5.
* `runall*.sh`, `sweep*.txt` — batch runners and result logs.
* `dumpraw.py`, `bigdiff.py`, `calls2.py`, `callmulti.py`, `callgap.py`,
  `regions.py` — annotated disassembly, region and call-set diff tools.
* `orig.txt`, `ours_base.txt`, `ours_dc.txt`, `ours_dupident.txt`,
  `ours_dima.txt` — full annotated dumps of the original and the four key
  variants.
* `Radar.base.cpp`, `CMakeLists.base` — pristine baseline copies.
