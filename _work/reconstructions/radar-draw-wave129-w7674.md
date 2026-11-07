# Radar::draw pass (wave 129, w7674) — block-order / branch-weight sweep, one 1-byte `.text` win

**Status:** source change landed, 2026-11-07. Worktree `wt/w7674`, based on current
`master` (`003a9623a`). Target `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`) in `src/game/weapons/Radar.cpp` (the original TU is
`Classes/Game/Radar.cpp`; the tree's folder is `game/weapons/`).

Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483. Baseline unrounded
`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` = **75.268817 %** (official
display 75.3). After this pass: **75.297491 %** (display still 75.3), same 9908 B.
Gate before and after: `RATCHET PASS`, byte **4506**, linked **4506**, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.94. Whole-`.text` differing bytes **85,441 ->
85,440**.

Read with `radar-draw-wave128-w7650.md`, `radar-draw-wave127-w7628.md`,
`radar-draw-body-wave122-w7210.md`, `radar-draw-three-shape-9908-w4105.md`.

## 1. Method

```sh
cd /home/debian/GalaxyOnFire2/wt/w7674
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~1 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Ghidra decompile of the original (`FUN_0013e050`, image base 0x10000) was regenerated with
`/home/debian/ghidra_scripts/DecompRadar.java`; the project is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra` (pass the project
**directory and name**, not `.`; Ghidra 12.1.2 rejects a `.` path element):

```sh
/tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless \
  /home/debian/GalaxyOnFire2/_work/bins android_2.0.16_libgof2hdaa.so_ghidra \
  -process android_2.0.16_libgof2hdaa.so -noanalysis \
  -scriptPath /home/debian/ghidra_scripts -postScript DecompRadar.java
```

Scratch in `/dev/shm/w7674` (not committed): `Radar.base.cpp` (pristine source),
`score.py` (single-symbol difflib harness reproducing 75.268817 exactly), `trymod.py`,
`batch.py`, `gen*.py`/`variants*.json` (the sweeps below), `orig.dis`/`ours.dis`,
`ghidra_radar.txt` (1858-line Ghidra decompile).

## 2. Exact block order from Ghidra / disassembly

The original's layout order is **planet section -> planet-lock tail -> landmark loop ->
enemies -> gas clouds -> asteroids -> music tail**, even though the source order is
landmark block then planet block. The planet section is the shared target of the landmark
guard's 109/110/111 skip paths, so clang outlines the landmark loop after it:

| block | original | ours (baseline) |
|---|---:|---:|
| route onScreen / scanner | `12e2b4` / `12e2e0` | `12e2b4` / `12e2e4` |
| first `otherLock` guard | `12e2fc` (`cbnz` chain) | `12e2fc` (`cbz`+`itt`) |
| first literal pool | `12e340` | `12e310` |
| landmark guard 111 | `12e354` | `12e35a` |
| planet section (shared skip target) | `12e376` | `12e376` |
| planet loop | `12e3d4` | `12e3d4` |
| prologue recompute (cold) | `12e6ee` | `12e6d2` |
| post-loop recompute | *(none)* | `12e72c` |
| planet-lock tail | `12e748` | `12e776` |
| landmark setup | `12e7a8` | `12e7c4` |
| landmark loop head | `12e7d4` (top-tested) | `12eb48` (bottom-tested) |
| enemies | `12ec24` | `12ec28` |

The **planet loop register split is almost exact in the baseline**: index `fp`, `stationId`
`r8`, `currentStation` `r9` (original: same).  Only `stations` is `r6` in ours vs `sl`
(spilled to `[sp,#116]`) in the original.  That is why the baseline (which keeps the
invented post-loop recompute) beats the faithful single-recompute shape: removing the
recompute moves the loop index to `sl` and spills `stations`, re-rolling the whole loop
(wave 127 measured 9864 B / 72.93 %).

## 3. What was tried this wave (all single edits on the baseline unless noted)

The assignment's hypothesis was **body block layout and branch weights**: reconstruct the
planet/ship/asteroid block order and test `__builtin_expect` / `if`-`else` shapes that move
blocks.  ~70 variants were built and scored.  None moved the score above the baseline except
the final reorder in §4.

### 3.1 `__builtin_expect` sweep (26 variants)

Every hot/cold condition in the planet / ship / gas / asteroid blocks was wrapped in
`__builtin_expect(..., 0|1)` with the same-line `// lint: arm_builtin` waiver.  Best results:

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.268817** |
| `route != nullptr` expect(1) | 9908 | 75.268817 (tie) |
| planet-tail `field_0x14 != 0` expect(1) | 9908 | 75.268817 (tie) |
| planet-lock timer expect(1) | 9908 | 75.250932 |
| gas-cloud guard expect(1) | 9892 | 74.282434 |
| scannerAvailable expect(1) | 9908 | 75.268817 (tie) |
| ship `onScreen` expect(1) | 9916 | 73.681193 |
| planet null / `!onScreen` expect(0) | 9876 | 72.421718 |
| asteroid dead expect(0) | 9900 | 72.891566 |
| `!otherLock && !planetHit && !turretMode` expect(1) | 9840 | 60.515330 |

The baseline is a strong local optimum: every hint that changes a block re-rolls the
planet-loop register split.

### 3.2 First `otherLock` guard: positive OR vs negated AND

The original's first guard is a `cbnz` chain (`if (lockedAsteroid || lockedGasCloud ||
field_0x1c) otherLock = 1; else otherLock = field_0x14 != 0;`).  The baseline source writes
the negated form `if (!A && !B && !C) otherLock = field_0x14 != 0; else otherLock = 1;`,
which clang canonicalises to `cbz` + `itt eq`.  The positive-OR form does produce the exact
`cbnz` chain and a contiguous hot block, but it moves the first literal pool from `12e310`
to `12e2e0` (displacing the scanner block) and scores **74.093190** (9908 B).  Adding
`__builtin_expect(...,1)` to the positive OR gives **75.240143** (9908 B), still below the
baseline.  The second `otherLock` computation is a plain `||` assignment and already
produces the original's `cbnz` chain in both builds.

### 3.3 Faithful single-recompute shape

Removing the post-loop recompute and making the campaign `isTarget` reload
`stations->data()[i]` reproduces the original's block addresses exactly (planet-lock tail
`12e748`, landmark setup `12e7a8`, top-tested loop head `12e7d4`, enemies `12ec28`) but is
9864 B / **72.934063 %** because the planet-loop index moves to `sl` and `stations` spills.
`volatile int autoPilotTarget`, loop-condition hints, campaign hints, and several
`stations`-local spellings all stay at or below 72.9 %.  This confirms the wave-127/128
conclusion: the baseline's post-loop recompute is the source-level lever that keeps the
loop index in `fp`.

### 3.4 Landmark loop / asteroid predicate / `inSight`

- Landmark loop: `while`, `for(;;)`+`break`, `!=`, unsigned, and loop-condition
  `__builtin_expect` all either tie or drop to 37–68 %; none un-rotates the loop.
- Asteroid `hasCargo && dead` reorder (the wave-4105 shape) is 9928 B / 72.70 %;
  `!hasCargo && dead` is 9912 B / 73.38 %; `dead && !hasCargo` (baseline) ties.
- Ship `inSight` distance-first (wave-122 A2) is 9896 B / 68.69 %; distance-first with
  integer crosshair is 9908 B / 64.68 %.  The original's `s26`/`s24` crosshair live range
  is what forces `vpush {d8-d13}`; adopting it re-rolls the rest of the function.

## 4. The landed change

In the post-loop recompute, compute the station index before `getAutoPilotTarget`:

```diff
         if (!alienOrbit) {
-            int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
             SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
             int stationIndex = system->getStationEnumIndex(
                     ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
                             ->data()[this->planetDockIndex]);
+            int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
             onPlanetCourse = autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
             this->field_0x1a8 = onPlanetCourse;
         }
```

The reorder is semantically identical (both callees are getters) and changes the second
recompute's register allocation: the block now starts with `getSystem` (`ldr r0,[r5,#0]`)
instead of `getPlayer` (`ldr r0,[r4,#0]`), uses `r5` for both `system` and `stationIndex`,
and restores `r5` from `[sp,#116]` before the tail.  Measured: **75.268817 -> 75.297491**
(+0.0287 points), 9908 B, `.text` differing bytes **85,441 -> 85,440**.  Every other
spelling of the same reorder (local `targets`, direct `field_0x1a8` assignment, expect on
`!alienOrbit`, inlining `system`) ties at 75.297491; putting `getAutoPilotTarget` first
again returns to 75.268817.  The gain is small but deterministic and gate-neutral.

## 5. Root cause (unchanged)

The residual is the coupled fixed point described in waves 122/127/128:

1. the first `otherLock` guard's `cbz`+`itt` shape and the first pool at `12e310`;
2. the post-loop recompute (invented) at `12e72c`, which displaces the planet-lock tail to
   `12e776` and the landmark setup to `12e7c4`;
3. `stations` in `r6` instead of `sl`+spill;
4. the bottom-tested landmark loop (pool at `12e7f4` instead of `12e79c`);
5. the ship `inSight` scheduling (bounds-first vs distance-first).

The faithful single-recompute shape fixes (2)–(4) but breaks (3) and scores 72.93 %.
A real win still needs a source shape that keeps the single recompute *and* the
`fp`/`sl`+spill planet-loop split.

## 6. Gate / section deltas (after the change)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,440** differing bytes (size/base exact),
`.ARM.extab` 1,427, `.note.gnu.build-id` 20. `.rodata`, `.data`, `.bss`, `.got`,
`.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`, `.hash` are byte-identical.
`docs/IDENTICALITY.md` and `docs/VALIDATION.md` are updated to the new `.text` count.

## 7. Additional sweep after the landed change (wave-129b, same session)

The scratch tree (`/dev/shm/w7674`) was recreated after the worktree was pruned and
re-checked out from branch `wave130-w7674` (`ed8dc3591`); the raw score reproduced at
**75.297491** before these runs. All variants below start from the committed source
(swap already applied) and keep the 9908 B size unless noted.

### 7.1 Landmark-loop shape

| variant | usize | raw % |
|---|---:|---:|
| **committed** | 9908 | **75.297491** |
| `if (i != 2 && data != null && visible) { body }` (remove `continue`, if-wrapper) | 9908 | 75.297491 (tie) |
| `int i = 0; while (i < size) { ... ++i; }` + `++i` in the continue arm | 9852 | 64.867968 |

The if-wrapper is canonicalised by clang to the same rotated loop, so it does not
un-rotate the top-tested original. The `while` form re-rolls the whole loop.

### 7.2 Top-of-function `onPlanetCourse` recompute order

| variant | usize | raw % |
|---|---:|---:|
| **committed** | 9908 | **75.297491** |
| `targets` first (before `autoPilotTarget`) | 9908 | 75.068100 |
| `system` first, then `autoPilotTarget`, `targets` | 9852 | 71.535365 |
| `system`/`stationIndex` first, then `autoPilotTarget`/`targets` | 9860 | 71.217129 |

The original cold block at `12e6ee` keeps `getAutoPilotTarget` first; the committed
order is already the best.

### 7.3 First `otherLock` guard, more shapes

| variant | usize | raw % |
|---|---:|---:|
| **committed** (negated `&&`, expect 0) | 9908 | **75.297491** |
| positive `||` + `__builtin_expect(...,1)` | 9908 | 75.268817 |
| positive `||` plain | 9908 | 74.121864 |
| single-expression `A || B || C || field_0x14 != 0` | 9852 | 72.962484 |
| `otherLock = field_0x14 != 0; if (A||B||C) otherLock = 1;` | 9856 | 73.281565 |
| positive `||` + expect(0) | 9872 | 73.383156 |

None beats the committed negated form; the positive-OR `cbnz` chain still displaces the
first literal pool.

### 7.4 Ship `inSight` block

| variant | usize | raw % |
|---|---:|---:|
| **committed** | 9908 | **75.297491** |
| float `cx`/`cy` crosshair | 9920 | 68.196815 |
| distance-first (`VectorLength` before bounds) | 9904 | 65.421635 |
| drop the redundant `enemy->field_0x73 = 0` | 9864 | 64.421416 |

The committed integer-crosshair/bounds-first shape is the local optimum; the extra
`d12`/`d13` (s24-s27) live range is still not reproduced.

### 7.5 Removing existing `__builtin_expect` hints

| variant | usize | raw % |
|---|---:|---:|
| **committed** | 9908 | **75.297491** |
| drop landmark `onScreen` expect | 9856 | 70.557792 |
| drop landmark `screenX` expect | 9912 | 71.492045 |
| drop both landmark expects | 9864 | 71.825111 |
| drop first `otherLock` expects | 9908 | 74.121864 |
| drop only the inner `lockedGasCloud` expect | 9908 | 75.268817 |

The existing hints are load-bearing; the committed set is best.

### 7.6 `notOtherLock` / `notTurretMode` locals

Inlining `!otherLock`/`!turretMode` at the landmark use site (removing the two locals)
is 9844 B / 73.320869 %. The locals are required for the original's `eor`-computed
`[sp,#108]`/`[sp,#116]` booleans.

Conclusion unchanged: the committed swap is the only source win found; the residual is
the coupled planet-loop register split (`fp`/`sl`+spill) plus the `vpush {d8-d13}`
live-range difference.
