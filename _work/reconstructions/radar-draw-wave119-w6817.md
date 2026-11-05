# Radar::draw pass (wave119, w6817) — recompute block, call counts, and a wide variant sweep

**Status:** research note, 2026-11-05. Worktree `wt/w6817`, branch
`wave119-w6817`, based on current `master` (after `radar-draw-wave119-w6721.md`
and `radar-draw-wave119-w6736.md`). **No source change landed**; this commit
contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.426606 %** (official `verify.py` display 74.4). 2596 instructions match.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0.

## 1. New source-level finding: `goingToPlanet()` is called twice by us, once by the original

`grep goingToPlanet` on the two disassemblies:

```
original:  12e18c  blx PlayerEgo::goingToPlanet      (1 call)
ours:      12e18c  blx PlayerEgo::goingToPlanet
           12e760  blx PlayerEgo::goingToPlanet      (2 calls)
```

The original has exactly **one** `getAutoPilotTarget`, **one** `goingToPlanet`,
**one** `isDockingToPlanet`, **one** `getStationEnumIndex`, **two**
`getPlanetTargets` (one setup at `0x12e13c`, one in the prologue computation
at `0x12e700`) and **one** store to `field_0x1a8` (`strb.w r0,[r4,#424]` at
`0x12e1d0`).

Ours has the same call counts except for a second `goingToPlanet` and a second
`field_0x1a8` store (`0x12ebe8`), both coming from the *invented* second
`onPlanetCourse` recompute after the planet loop (source lines 612-624).
The original's prologue computation is at `0x12e6ee`-`0x12e746`; ours is at
`0x12e6d2`-`0x12e72a` and is instruction-for-instruction identical apart from
stack slots (`[sp,#152]` vs `[sp,#148]` for `autoPilotTarget`).

So the original genuinely has **no recompute after the planet loop**. The
recompute is a reconstruction artefact that is nevertheless load-bearing for
`difflib` (see §2).

Hoisting `goingToPlanet()` into a local in the prologue (`bool goingToPlanet
= this->level->getPlayer()->goingToPlanet();`) does **not** change the emitted
code at all: clang had already CSE'd the prologue call, and the second call
is the recompute.

## 2. Why the recompute cannot simply be deleted

Deleting the recompute block (`if (!alienOrbit) { ... field_0x1a8 = ...; }`)
makes the call multiset exact and moves the planet-lock tail inline to
`0x12e764` (original `0x12e748`), but clang then moves the **entire landmark
loop** out of line to `0x12f45a`. `SequenceMatcher` sees one 411-instruction
block replaced by a 325-instruction block at a different position:
`replace orig[668:1079] ours[665:666]` and `replace orig[2199:2200]
ours[1789:2114]`. Score **63.746398 %** (9824 B).

The recompute is therefore an anchor that keeps the landmark loop inline.
Moving it around the source does not help: clang's block placement is
independent of the source order of the recompute and the
`if (!turretMode && !landmarkHit)` block (moving the latter before the
recompute emits byte-identical code).

## 3. The real residual: three block-placement / colouring differences

1. **inSight block** (`orig[1369:1411]`): the original evaluates
   `VectorLength(egoPos - targetPos) < 60000.0f` **first**, then the four
   screen-bounds tests, and caches `crosshairPos.x/y` in `s26`/`s24` across
   the call (hence `vpush {d8-d13}`). Ours does the bounds first and has
   `vpush {d8-d11}`. The faithful reorder reproduces the original block
   instruction-for-instruction but re-lays-out the function and scores
   **66.743251 %** (9864 B).
2. **`otherLock` chain** (`orig[235:258]`): the original emits a straight
   `cbnz / cbnz / cmp+beq` chain into a shared `movs r5,#1`. Ours if-converts
   the 2nd/3rd null checks (`itt eq; ldreq; cmpeq`) and keeps `otherLock` in
   `r9` instead of `r5`. Removing the inner `__builtin_expect(...,1)` gives
   the straight chain but scores 74.397936 %.
3. **`if (!turretMode && !landmarkHit)` tail** (`orig[637:654]`): the
   original has this 17-instruction block inline at `0x12e748`; ours has it
   out of line at `0x12ebf4` because the recompute occupies the inline slot.
   Splitting the condition into nested `if`s emits identical code.

Stack-slot colouring is also off by one or two slots in places:
`mode` is `[sp,#100]` (orig) vs `[sp,#112]` (ours); `turretMode` is
`[sp,#148]` vs `[sp,#152]`; `planetHit` is `[sp,#152]` vs `[sp,#148]`
(swapped). The frame size (296 B) and `vpush`-adjusted SP are identical.

## 4. Variants measured this session (all on the pristine baseline)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.426606** |
| recompute deleted | 9824 | 63.746398 |
| recompute after `if (!turretMode...)` (source order swap) | 9908 | 74.426606 (identical code) |
| recompute at start of `if (!turretMode...)` | 9908 | 74.372580 |
| recompute at end of `if (!turretMode...)` | 9932 | 71.135940 |
| recompute before planet loop | 9928 | 65.300272 |
| recompute after `stations` decl | 9924 | 64.878818 |
| recompute at top of planet loop | 9928 | 65.300272 |
| `goingToPlanet` hoisted to local | 9908 | 74.426606 (identical code) |
| tail ternary hoisted to local `int music` | 9908 | 74.369266 |
| tail ternary as `if/else` assigning `music` | 9908 | 74.426606 (`it lt`, same score) |
| inSight faithful reorder (`VectorLength` first) | 9864 | 66.743251 |
| inSight reorder, y before x | 9864 | 66.714532 |
| inSight reorder, direct bool init | 9928 | 65.121602 |
| inSight nested-if form | 9928 | 65.121602 |
| inSight `distance` local before bounds | 9920 | 63.533458 |
| inSight reorder + plain `otherLock` `&&` | 9944 | 62.557471 |
| inSight reorder + outer-expect-only `otherLock` | 9944 | 62.891636 |
| `otherLock` plain `&&` (no expects) | 9908 | 73.193807 |
| `otherLock` outer expect only (inner removed) | 9908 | 74.397936 |
| `otherLock` inner expect 0 | 9900 | 71.887550 |
| `otherLock` outer expect 1 | 9900 | 71.014076 |
| `otherLock` both expects 1 | 9900 | 71.014076 |
| `otherLock` nested-if form | 9908 | 73.193807 |
| `otherLock` init-1 form | 9908 | 73.193807 |
| `bool otherLock` | 9868 | 67.375377 |
| `int landmarkHit` | 9896 | 70.508961 |
| `int missionBlocksPlanets` | 9904 | 73.211982 |
| `int turretMode` | 9856 | 66.455969 |
| `int alienOrbit` | 9908 | 74.415938 |
| `int onPlanetCourse` | 9908 | 74.426606 |
| `int planetHit` | 9896 | 71.887550 |
| `int asteroidHit` | 9908 | 69.284793 |
| `int hasDockNav` | 9908 | 74.426606 |
| `int candidateWreck` | 9908 | 74.426606 |
| `int navWreck` | 9908 | 74.426606 |
| `int dead` (asteroid loop) | 9908 | 74.426606 |
| `bool alwaysEnemySeen` | 9908 | 72.323348 |
| `int enemyHit` (with `bool bossSeen`) | 9924 | 70.466915 |
| `uint8_t bossSeen` | 9908 | 72.630824 |
| `enemyHit` declared before `bossSeen` | 9896 | 65.336014 |
| `int alwaysEnemy` | 9904 | 72.067680 |
| `uint8_t alwaysEnemy` | 9908 | 74.340596 |
| `alwaysEnemySeen` moved before `enemies:` | 9908 | 74.426606 |
| `alwaysEnemySeen` moved before scanner `if` | 9880 | 69.576454 |
| landmark `size()` hoisted to `int` | 9908 | 72.500716 |
| landmark loop `uint32_t i` | 9920 | 73.180516 |
| landmark loop `uint32_t i` + hoisted size | 9916 | 70.761741 |
| landmark `onScreen` inner expect removed | 9908 | 72.867995 |
| enemy-loop `onScreen` gets `expect(...,1)` | 9864 | 62.830840 |
| De Morgan enemy section condition | 9908 | 74.426606 |
| De Morgan gas-cloud condition | 9908 | 74.426606 |
| De Morgan asteroid condition | 9908 | 74.426606 |
| i==0 arm: `lockLabel = label; DrawString(lockLabel)` | 9908 | 74.115456 |
| i==0 arm: `lockLabel = label; DrawString(label)` | 9912 | 70.485465 |
| i==0 arm: `lockLabel = calcDistance(...); DrawString(lockLabel)` | 9908 | 74.029509 |
| i==0 arm faithful + outer-expect-only `otherLock` | 9908 | 74.086807 |
| `float hull` implicit `barWidth` | 9908 | 74.426606 |
| `float emp` implicit `barWidth` | 9908 | 74.426606 |
| `hull` with `100.0` (double) | 9896 | 71.188871 |
| `left/top` without `(float)size` cast | 9892 | 73.042730 |
| `if (!turretMode && !landmarkHit)` split into nested ifs | 9908 | 74.426606 |

## 5. Conclusion

The baseline is a strong local optimum: every faithful-source improvement
(the inSight reorder, the recompute deletion, the i==0 `lockLabel` store,
the `goingToPlanet` call-count fix) lowers the `difflib` ratio because it
changes clang's basic-block placement, and `difflib` is dominated by
long in-order instruction runs. The remaining ~25 % is essentially the
inSight block (28+28 instructions), the `otherLock` if-conversion, and the
out-of-line planet-lock tail, plus pervasive stack-slot/register colouring.

A future pass that wants to beat 74.426606 % should either
(a) find a source shape that keeps the landmark loop inline *and* deletes
the recompute (the 411-instruction reorder is the blocker), or
(b) reproduce the original's `s24`/`s26` crosshair caching without changing
the inSight branch layout.
