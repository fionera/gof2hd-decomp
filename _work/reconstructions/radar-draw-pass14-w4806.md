# Radar::draw pass 14 — the gas-cloud preamble slot is a loop-header **phi order**,
# and closing it makes the `onPlanetCourse` variant byte-identical to the baseline (w4806)

**Status:** research note, 2026-11-04. Worktree `wt/w4806`, branch `wave98-w4806`,
base `fe4f5e7cf`. **No source change landed**: the source shape that closes the
slot is identified and measured, but it turns the pass-13 `q2` variant into the
landed baseline byte-for-byte, so it cannot be a strict improvement. This commit
contains this note only.

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

The scoped `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1`
override on `Radar.cpp` is still in `CMakeLists.txt`.

## 1. Reproducing pass-13 `q2` exactly

The pass-13 addendum (`radar-draw-pass13-w4602.md`, §A3.10) describes `q2` as a
`bool onPlanetCourse` local that replaces the direct `field_0x1a8` stores in the
top-level autopilot block and is recomputed/stored again in the campaign block.
Reconstructed verbatim, it measures **9908 B / 73.868195 %** (norm 3492/3488),
matching the note's 73.8682.

The source diff from the landed baseline is:

```diff
@@ top-level autopilot block @@
+    bool onPlanetCourse;
     if (alienOrbit) {
-        this->field_0x1a8 =
+        onPlanetCourse =
                 this->level->getPlayer()->goingToPlanet() && !this->level->getPlayer()->isDockingToPlanet();
     } else {
         if (this->level->getPlayer()->goingToPlanet() && !this->level->getPlayer()->isDockingToPlanet()) {
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
```

**Key measurement.** `q2` and the landed baseline differ in **exactly four
instructions** of the whole 9908-byte body (objdump of both linked `.so`s,
`diff` of the disassembly is a single 4-line hunk):

```
base 1797 movs  r5, #0
base 1798 movs  r1, #0
base 1799 str.w r9, [sp, #144]
base 1800 str   r1, [sp, #96]
q2   1797 movs  r1, #0
q2   1798 movs  r5, #0
q2   1799 str   r1, [sp, #96]
q2   1800 str.w r9, [sp, #144]
```

The baseline order is the original's (`orig 1808..1811`); `q2` swaps the two
`movs` and the two `str`s. This is the "one gas-cloud-preamble scheduling slot"
of pass 13. Everything else in `q2` is byte-identical to the baseline, so the
pass-13 statement "the local `onPlanetCourse` copy restores the original block
order" is a statement about *source* order only: the binary block order of `q2`
and the baseline is identical.

## 2. Root cause: the loop-header PHI order, not the scheduler

`-emit-llvm -g0` shows the difference is not a post-RA schedule at all — the
gas-cloud loop header's `PHINode` order differs:

```llvm
; baseline
%1885 = phi %class.Array.23* [ %2102, %2099 ], [ %1846, %1848 ]   ; gasClouds
%1886 = phi i32            [ %2101, %2099 ], [ 0,     %1848 ]     ; i
%1887 = phi i1             [ %2100, %2099 ], [ false, %1848 ]     ; gasHit

; q2
%1881 = phi %class.Array.23* [ %2098, %2095 ], [ %1843, %1845 ]   ; gasClouds
%1882 = phi i1             [ %2096, %2095 ], [ false, %1845 ]     ; gasHit
%1883 = phi i32            [ %2097, %2095 ], [ 0,     %1845 ]     ; i
```

The incoming constants (`i = 0`, `gasHit = false`) are materialised in the
predecessor block in PHI order. The baseline emits `i` first, then `gasHit`;
`q2` emits `gasHit` first, then `i`. The two spills (`&this->lockLabel` to
`[sp,#144]`, `gasHit` to `[sp,#96]`) follow the same order. So the "scheduling
slot" is really the order in which the loop's induction variable and the
loop-carried `bool` are added to the header PHI list.

## 3. The source shape that closes the slot

The PHI order is controlled by the *scope/order of the loop variable's
declaration*. Declaring `i` before `gasHit` and initialising it in the `for`
clause restores the baseline order:

```diff
@@ gas-cloud loop @@
     if (this->gasClouds != nullptr) {
+        uint32_t i = 0;
         bool gasHit = false;
-        for (uint32_t i = 0; i < this->gasClouds->size(); ++i) {
+        for (; i < this->gasClouds->size(); ++i) {
```

Measured (official verifier, `autojunk=False`):

| variant | usize | ratio | norm |
|---|---:|---:|---|
| baseline | 9908 | **73.925501** | 3492/3488 |
| `q2` (gasHit before `i`) | 9908 | 73.868195 | 3492/3488 |
| **`q2fix`** (`i` declared first, `for (; ...)`) | 9908 | **73.925501** | 3492/3488 |

`diff` of the `q2fix` and baseline disassemblies is **empty**: the whole
`Radar::draw` body is byte-identical. Equivalently, the source shape is robust:
`int`/`unsigned`/`size_t` for `i`, `bool gasHit = 0`, and `uint32_t i;` +
`for (i = 0; ...)` all produce the baseline; only the `gasHit`-before-`i`
ordering produces the `q2` slot. Moving `gasHit` outside the `if` also reverts
to the `q2` order even with `i` declared first.

**Conclusion: closing the slot in `q2` yields the landed baseline exactly.**
There is no strict improvement available along this axis; pass 13's "what
remains" item 1 is now closed.

## 4. Variant sweep this pass (all official verifier; baseline 9908 / 73.925501)

Unless noted, each row is `q2fix` plus the listed edit. All variants that were
not already known from pass 13 were measured here.

### 4.1 The slot / loop-variable shapes

| variant | usize | ratio |
|---|---:|---:|
| `q2fix` (`uint32_t i` first, `for (; ...)`) | 9908 | **73.925501** |
| `int i` / `unsigned i` / `size_t i` first | 9908 | 73.925501 |
| `uint32_t i;` + `for (i = 0; ...)` | 9908 | 73.925501 |
| `bool gasHit = 0` | 9908 | 73.925501 |
| `q2` (`gasHit` first) | 9908 | 73.868195 |
| `gasHit` first + `int`/`unsigned`/`size_t i` | 9908 | 73.868195 |
| `gasHit` first + `bool gasHit = 0` | 9908 | 73.868195 |
| `gasHit` first + `int gasHit` | 9908 | 73.839542 |
| `bool gasHit` outside the `if`, `i` first | 9908 | 73.868195 |
| `gasHit` outside the `if`, both outside | 9908 | 73.868195 |
| `bool gasHit` before `i`, `for (i = 0; ...)` | 9908 | 73.868195 |
| cached `n = gasClouds->size()` | 9912 | 68.642612 |

The same `i`-first declaration trick applied to the enemies, planet and
landmark loops is neutral (the baseline already has those PHI orders right).

### 4.2 `q2` local type / campaign-block spellings (all with the slot closed)

| variant | usize | ratio |
|---|---:|---:|
| `q2fix` | 9908 | **73.925501** |
| `int onPlanetCourse` | 9908 | 73.925501 |
| `int onPlanetCourse = 0` | 9908 | 73.925501 |
| `bool onPlanetCourse = false` | 9908 | 73.925501 |
| store field first, then local | 9908 | 73.925501 |
| bare `{ ... }` instead of `if (!alienOrbit)` | 9908 | 73.925501 |
| no `system` temporary (inline `getSystem()`) | 9908 | 73.925501 |
| campaign `getStarSystem()->getPlanetTargets()` | 9916 | 72.118001 |
| campaign store only, no recompute (`q1`) | 9744 | 62.136764 |
| remove the campaign block entirely (`nodup`) | 9740 | 62.290341 |
| `nodup` + planet-lock store swap (`m1`) | 9748 | 68.795146 |
| hoist `goingToPlanet` only | 9932 | 58.237658 |
| hoist `goingToPlanet` + `isDockingToPlanet` | 9840 | 52.792170 |
| hoist `autoPilotTarget`/`stationIndex` to function scope | 9796 | 55.211186 |
| read `onPlanetCourse` instead of `field_0x1a8` at the planet lock | 9896 | 72.690763 |
| read `onPlanetCourse` at all four `field_0x1a8` sites | 9892 | 64.258065 |

### 4.3 Other known levers re-measured on `q2fix`

| variant | usize | ratio |
|---|---:|---:|
| `otherLock` positive-OR (`o6`) | 9904 | 72.169676 |
| `otherLock` `expect(A||B||C, 1)` (`o11`) | 9908 | 73.925501 |
| `otherLock` OR including `field_0x14 != 0` (`f4`) | 9904 | 72.169676 |
| `otherLock` init-then-OR (`f1`) | 9928 | 69.621343 |
| `planetHit` as `int` | 9896 | 71.301606 |
| on-screen in-sight distance-first (`i2`) | 9920 | 69.951303 |
| on-screen in-sight distance-first, single assignment | 9916 | 69.961324 |
| off-screen in-sight distance-first, float locals | 9928 | 69.855363 |
| off-screen in-sight distance-first, int locals | 9952 | 63.165447 |
| off-screen in-sight nested-if | 9940 | 61.860999 |
| hoist crosshair floats before `getPosition()` | 9928 | 64.546627 |
| `notOtherLock`/`notTurretMode` swapped | 9908 | 73.925501 |
| `notOtherLock`/`notTurretMode` as `bool` | 9900 | 71.989679 |
| `notOtherLock`/`notTurretMode` outside the `if` | 9916 | 71.268977 |
| `bossSeen`/`enemyHit` as `int` | 9912 | 67.048546 |
| `alwaysEnemy` as `int` | 9904 | 71.539123 |
| `wreck` as `int` | 9916 | 71.878580 |
| `inSight` as `int` (off-screen) | 9908 | 73.925501 |
| `__builtin_expect(gasClouds != nullptr, 1)` | 9892 | 67.020361 |
| `__builtin_expect(gasClouds != nullptr, 0)` | 10084 | 34.080590 |
| `__builtin_expect(campaign >= 2 ..., 1)` | 9868 | 60.376275 |
| `__builtin_expect(campaign >= 2 ..., 0)` | 9900 | 43.940264 |
| `__builtin_expect(onScreen, 0)` | 9860 | 65.376344 |
| `__builtin_expect(i == 1 || i == 3, 1)` | 9908 | 73.925501 |
| drop the `onScreen` anchor | 9896 | 71.021344 |
| drop the `i == 1 || i == 3` anchor | 9912 | 72.110013 |
| drop the `otherLock` anchor | 9904 | 72.169676 |
| `mission`/`missionBlocksPlanets`/`alwaysEnemySeen` reordered | 9904-9908 | 70.879908-73.925501 |
| `turretMode`/`alienOrbit`/`campaign` reordered | 9908 | 71.948424-73.839542 |
| `mode` cached in a local | 9908 | 73.925501 |
| `stations->data()[i]` replaced by `stationId` | 9804 | 68.261747 |

No variant beats 73.925501.

## 5. Remaining residuals (unchanged from pass 13)

The baseline remains a strong local optimum. The dominant residuals are still:

1. the ARM constant-pool island at `0x84c` (ours) vs `0x74c` (orig);
2. `vpush {d8-d11}` vs `{d8-d13}` (the original keeps the crosshair floats in
   `s24`/`s26` across `operator-`/`VectorLength`; the on-screen in-sight block
   in our source has no distance test, and adding it re-rolls the whole
   allocation and grows the frame);
3. the `mode` stack slot (`[sp,#100]` orig vs `[sp,#112]` ours) and the
   `planetHit`/`turretMode` swap (`#148`/`#152`);
4. `stations` parked in `r6` instead of `sl` (+1 `mov`);
5. `otherLock#1`'s cold half at `0x2480` (ours) vs `0xb72` (orig);
6. the landmark-filter 109/110 block shape (orig computes
   `notOtherLock`/`notTurretMode` in a shared 109/110 block; ours jumps straight
   to the loop preamble).

The gas-cloud preamble slot is no longer on this list.

## 6. Evidence / scratch (all under `/dev/shm/w4806`, not committed)

* `measure.py` — official-verifier ratio for one symbol (same API as
  `tools/verify/verify.py`, `autojunk=False`).
* `dump.py`, `normdump.py` — annotated / normalized disassembly dumps.
* `try.sh` — copy source, build `link_so`, measure.
* `Radar.base.cpp`, `Radar.q2.cpp`, `Radar.q2fix.cpp` — the three key sources.
* `radar_base0.ll`, `radar_q20.ll` — `-g0 -emit-llvm` IR; the loop-header PHI
  order is the evidence in §2.
* `orig.dis`, `ours_base.dis`, `ours.dis` — full annotated dumps.
