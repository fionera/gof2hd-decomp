# Radar::draw pass 15 — the gas-cloud preamble slot is closed by the `i`-first
# source shape; the duplicate autopilot block and the constant-pool island are
# the remaining layout keys (w4904)

**Status:** landed source-fidelity pass + research note, 2026-11-04. Worktree
`wt/w4904`, branch `wave99-w4904`, base `44225192c`. The landed source change
(`bool onPlanetCourse` local + `uint32_t i` declared before `bool gasHit`) is
**byte-identical** to the previous baseline: `Radar::draw` SHA-1
`414321e9e05febb673ee0413ffb4a80392be6ae4`, 9908 B, official verifier ratio
**73.925501 %** before and after. No strict verifier improvement was found; the
gate counts are unchanged.

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
lint:   CLEAN ✓
drift:  0 field(s) across 0 class(es)
```

`_ZN5Radar4drawEP6PlayerP3Hudi`:

```
osize 9908  usize 9908  byte False linked False
raw SequenceMatcher ratio 73.925501 %   normalized stream 3492 (orig) / 3488 (ours)
function SHA-1 414321e9e05febb673ee0413ffb4a80392be6ae4
```

The scoped `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1`
override on `Radar.cpp` is still in `CMakeLists.txt`.

## 1. The landed source shape: `onPlanetCourse` + `i`-first

The pass-14 note (`radar-draw-pass14-w4806.md`) identified the four-instruction
gas-cloud preamble slot as a loop-header PHI order and showed that declaring the
loop counter before the loop-carried `bool` restores the original order.  This
pass applies that shape to the *current* tree (which has moved on since pass 14)
and re-verifies it:

```diff
@@ top-level autopilot block @@
+    bool onPlanetCourse;
     if (alienOrbit) {
-        this->field_0x1a8 =
+        onPlanetCourse =
                 this->level->getPlayer()->goingToPlanet() && !this->level->getPlayer()->isDockingToPlanet();
     } else {
         ...
-            this->field_0x1a8 = autoPilotTarget == targets->data()[stationIndex];
+            onPlanetCourse = autoPilotTarget == targets->data()[stationIndex];
         } else {
-            this->field_0x1a8 = false;
+            onPlanetCourse = false;
         }
     }
+    this->field_0x1a8 = onPlanetCourse;

@@ campaign block @@
-            this->field_0x1a8 = this->level->getPlayer()->goingToPlanet() &&
-                                !this->level->getPlayer()->isDockingToPlanet() &&
-                                autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
+            onPlanetCourse = this->level->getPlayer()->goingToPlanet() &&
+                             !this->level->getPlayer()->isDockingToPlanet() &&
+                             autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
+            this->field_0x1a8 = onPlanetCourse;

@@ gas-cloud loop @@
+        uint32_t i = 0;
         bool gasHit = false;
-        for (uint32_t i = 0; i < this->gasClouds->size(); ++i) {
+        for (; i < this->gasClouds->size(); ++i) {
```

The gas-cloud preamble in the linked binary is instruction-for-instruction the
original's (orig `0x17be..0x17c4`, ours `0x17aa..0x17b0`):

```
movs   r0, #1
str.w  sl, [r4, #60]
str    r0, [sp, #96]
```

so the slot is closed.  The whole `Radar::draw` body is byte-identical to the
pre-pass baseline; the change is source fidelity only (the local names the value
that the original stores once to `field_0x1a8`).

## 2. The duplicate autopilot block is extra, but it is load-bearing

The original has **one** `getAutoPilotTarget` call, in the top-level block's
cold half at function-relative `0x69e` (reached from the `beq.w` at `0x166`).
The campaign block does not recompute it.  Our source keeps a second copy in the
campaign block; removing it gives the semantically correct call multiset but
re-rolls the whole allocation and drops the ratio:

| variant | usize | ratio | norm |
|---|---:|---:|---:|
| baseline (duplicate present) | 9908 | **73.925501** | 3492/3488 |
| `nodup` (delete campaign autopilot) | 9740 | 62.290341 | 3492/3424 |
| `nodup` + move top-level block after campaign block | 9748 | 69.346443 | 3492/3424 |
| move campaign autopilot after planet-lock tail | 9948 | 70.189003 | 3492/3492 |
| move top-level block after campaign autopilot | 9908 | 71.284852 | 3492/3466 |
| campaign block = comparison only (`this->planetTargets`) | 9880 | 66.226496 | 3492/3475 |
| campaign block = top-level cold half (`getStarSystem()->getPlanetTargets()`) | 9888 | 65.576123 | 3492/3477 |

The original's single cold half at `0x69e` is exactly reproduced by `nodup`
(`getAutoPilotTarget` at ours `0x6a4`), but `nodup` is 168 B short: the original
campaign region carries ~25 instructions more than our planet loop, and the
baseline's duplicate block is a 9908-byte size/allocation crutch.  No source
shape tried here recovers both the size and the allocation.

## 3. The constant-pool island is the layout key

The two relevant PIC islands are:

| island | original | ours (baseline) |
|---|---|---|
| first (stack_chk_guard, Canvas, status, DrawImage2D, font) | `0x2f0` | `0x2c0` |
| second (orbitPlanetIndex, crosshairPos, font[, crosshairPos]) | `0x74c` (3 words) | `0x84c` (4 words) |

The first island is pushed 0x30 B early because our `otherLock` hot block ends
with an unconditional `b.n` to the landmark-filter merge point:

```
orig  02b4 cbnz r0, 0x2c2        ; lockedAsteroid != 0 -> otherLock = 1
      02b6 ldr  r0, [r4, #56]
      02b8 cbnz r0, 0x2c2
      02ba ldr  r0, [r4, #28]
      02bc cmp  r0, #0
      02be beq.w 0xb72           ; all-null -> cold
      02c2 movs r5, #1           ; otherLock = 1 (falls through to the filter)
      02c4 ldr.w r0, [r4, #312]  ; landmark filter
ours  02b4 cbz  r0, 0x2d4        ; lockedAsteroid == 0 -> continue null checks
      02b6 mov.w r9, #1          ; otherLock = 1
      02ba ldr  r5, [sp, #156]
      02bc b.n  0x2e8            ; <-- unconditional branch: pool goes here
      02c0 .word ...             ; first pool island
```

The branch polarity is the whole difference: the original treats the all-null
path as the fall-through and branches to `otherLock = 1`; ours treats
`otherLock = 1` as the fall-through and branches to the remaining null checks.
The `__builtin_expect(..., 0)` on the all-null chain is what selects our
polarity, and it is load-bearing (plain form is 9904 B / 72.169676 %).

The second island is pushed 0x100 B late by the duplicate autopilot block plus
the planet-lock tail placement; it also grows from 3 to 4 words because the
duplicate block moves the two `crosshairPos` uses far enough apart that the
assembler emits a second pool word.  `nodup` moves the island to `0x6ec` with
7 words (it absorbs the four float constants), so the pool shift is not a
one-line fix either.

## 4. Variant sweep this pass (official verifier, `autojunk=False`)

Baseline 9908 / 73.925501.  All rows are the landed source plus the listed edit.
`q2fix` = the landed shape.

### 4.1 `otherLock` first chain

| variant | usize | ratio |
|---|---:|---:|
| baseline / `q2fix` | 9908 | **73.925501** |
| plain if/else, no `expect` | 9904 | 72.169676 |
| positive `\|\|` form, no `expect` | 9904 | 72.169676 |
| `int otherLock = 1; if (all-null) ...`, no `expect` | 9904 | 72.169676 |
| `int otherLock = 1; if (expect(all-null, 0)) ...` | 9908 | 73.925501 |
| `\|\|` form + `expect(..., 1)` | 9908 | 73.925501 |
| ternary `(any) ? 1 : (f14 != 0)` | 9904 | 72.169676 |
| `otherLock \|= any` | 9892 | 65.367531 |
| `expect` on first term only | 9912 | 71.958733 |
| `expect` on last term only | 9908 | 73.925501 |
| `expect` on `\|\|` form, 0 | 9912 | 70.782484 |

### 4.2 Landmark `else if (i == 1 \|\| i == 3)`

| variant | usize | ratio |
|---|---:|---:|
| baseline `expect((i & 0x7fffffff) == 1 \|\| ... , 0)` | 9908 | **73.925501** |
| `expect(i == 1 \|\| i == 3, 0)` | 9908 | 73.925501 (byte-identical) |
| plain `i == 1 \|\| i == 3` (no `expect`) | 9908 | 73.925501 (byte-identical) |
| `(i & 0x7fffffff) == 1 \|\| ...` without `expect` | 9912 | 72.110013 |
| `expect(i == 1 \|\| i == 3, 1)` | 9908 | 73.925501 |
| `i == 1 \|\| expect(i == 3, 0)` | 9912 | 72.110013 |

The `& 0x7fffffff` mask is redundant: the compiler emits the same `bic.w r0,
sl, #0x80000000` for the plain form.  The load-bearing part is the `expect`.

### 4.3 Types / declaration order (all neutral unless noted)

| variant | usize | ratio |
|---|---:|---:|
| `uint32_t campaign` | 9908 | 73.925501 |
| `int` landmark loop counter (baseline) | 9908 | **73.925501** |
| `uint32_t`/`size_t`/`unsigned` landmark counter | 9916 | 72.128330 |
| `int` planet/enemy/asteroid counters | 9908 | 73.925501 |
| `bool missionBlocksPlanets` -> `int` | 9904 | 72.682997 |
| `bool turretMode` -> `int` | 9888 | 66.064257 |
| `bool alienOrbit` -> `int` | 9904 | 72.187993 |
| `bool landmarkHit` -> `int` | 9908 | 70.267947 |
| `bool planetHit` -> `int` | 9896 | 71.301606 |
| `bool notOtherLock` / `notTurretMode` | 9900 | 71.989679 / 70.573066 |
| `int alwaysEnemySeen` (baseline) | 9908 | **73.925501** |
| `int alwaysEnemySeen` -> `bool` | 9908 | 71.537029 |

### 4.4 Statement order in the planet loop / campaign block

| variant | usize | ratio |
|---|---:|---:|
| `textOffset` after `warpGate` | 9908 | 73.925501 |
| `currentStation` before `stationId` | 9908 | 73.896848 |
| `warpGate` first | 9908 | 71.284852 |
| `isTarget` uses `stations->data()[i]` | 9904 | 71.700817 |
| all `stationId` -> `stations->data()[i]` | 9948 | 70.975122 |
| `marked` -> `bool` | 9928 | 70.890067 |
| planet `update()` after the `onScreen` check | 9908 | 73.868195 |
| enemy `update()` after the `wreck` computation | 9908 | 73.782235 |
| `!landmarkHit && notTurretMode && notOtherLock && i != 3` | 9904 | 72.083692 |
| `notOtherLock && notTurretMode && !landmarkHit && i != 3` | 9892 | 70.202093 |
| landmark condition as positive `!=` chain | 9912 | 64.050415 |
| landmark condition with the 111&&93 term first | 9908 | 71.872761 |

### 4.5 Enemy in-sight block (distance-first / crosshair floats)

| variant | usize | ratio |
|---|---:|---:|
| baseline (screen-first, interleaved) | 9908 | **73.925501** |
| distance-first, interleaved | 9924 | 71.132102 |
| distance-first, x-pair/y-pair | 9936 | 70.711117 |
| float `crossX/crossY` + `Vector delta` + distance-first | 9916 | 64.603038 |
| float locals, inline subtraction, distance-first | 9924 | 71.132102 |
| `dist` local first, then int conversions | 9928 | 64.398055 |
| `dist` local first, interleaved bounds | 9940 | 62.004595 |
| `inSight = <distance-first chain>` | 9928 | 64.684015 |

## 5. Conclusion / remaining residuals

The landed change closes the gas-cloud preamble slot and makes the source match
the original's single-store `field_0x1a8` shape, at zero gate cost.  The verifier
ratio is unchanged because the remaining 26 % of the stream is block placement
and register/stack-slot allocation, not source statements:

1. the duplicate campaign autopilot block (correct source is `nodup`, but it is
   168 B short and re-rolls the allocation);
2. the first constant-pool island (`0x2c0` ours vs `0x2f0` orig), pushed by the
   `b.n` after `otherLock = 1`;
3. the second constant-pool island (`0x84c`, 4 words, ours vs `0x74c`, 3 words,
   orig), pushed by the duplicate block;
4. the planet-lock tail placed after the landmark loop (ours `0xaf6`) instead of
   immediately after the planet loop (orig `0x6f8`);
5. `otherLock#1`'s cold half at `0x2480` (ours) vs `0xb72` (orig);
6. `vpush {d8-d11}` vs `{d8-d13}` (the original keeps the crosshair floats in
   `s24`/`s26` across `operator-`/`VectorLength`; every distance-first source
   shape tried re-rolls the allocation and loses more than it gains);
7. the `mode`/`&lockLabel`/`missionBlocksPlanets` stack-slot rotation.

## 6. Evidence / scratch (all under `/dev/shm/w4904`, not committed)

* `measure.py` — official-verifier ratio for one symbol (`autojunk=False`).
* `normaddr.py` / `normdump.py` — annotated normalized disassembly with
  function-relative addresses.
* `tryv.py` — apply a source edit, build `link_so`, measure.
* `Radar.base.cpp`, `Radar.q2fix.cpp` — the committed baseline and the landed
  source shape.
* `orig_na.dis`, `ours_base_na.dis`, `ours_or_na.dis` — full annotated dumps.
