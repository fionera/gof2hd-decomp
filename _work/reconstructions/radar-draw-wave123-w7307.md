# Radar::draw pass (wave123, w7307) — literal-pool / block-order sweep, no source win

**Status:** research note, 2026-11-06. Worktree `wt/w7307`, based on current
`master` (`1c8c9b20e`, wave-122 final). **No source change landed**; this commit
contains this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.541284 %** (official `verify.py` display 74.5), `M = 2600` matching
instructions. Gate at start and end: `RATCHET PASS`, byte **4506**, linked
**4506**, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-body-wave122-w7210.md` and `radar-draw-wave122-w7209.md`;
this wave repeats their conclusions after a further targeted sweep of the
literal-pool placement and the planet-lock/landmark block order.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7307
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch harness `/dev/shm/w7307/score_radar.py` mirrors
`tools/verify/verify.py`'s `disassemble`/`reconcile` and reproduces the
official 74.541284 % exactly. Per-variant loop: edit `Radar.cpp`,
`cmake --build cmake-build-match --target link_so -j6`, re-score. A helper
(`pool1.py`) extracts the first `ldr [pc,#…]` island address from the linked
`.so`.

## 2. Baseline residual (re-measured this wave)

Original / ours function-relative highlights:

| property | original | baseline |
|---|---|---|
| first literal pool | `0x12e340` | `0x12e2e0` |
| `vpush` mask | `{d8-d13}` | `{d8-d11}` |
| `mode` stack slot | `[sp,#100]` | `[sp,#112]` |
| `otherLock` / `status` regs | `r5` / `r6` | `r9` / `r5` |
| planet-lock tail | `0x12e748` (before landmark loop) | `0x12ebec` (after) |
| landmark loop | `0x12e7a8` (top-tested) | `0x12e7b4` (bottom-tested) |
| cold `otherLock` | `0x12ebc2` | `0x1305fa` |

The first island is placed immediately after the **route block** in ours
(`12e2de b.n` then pool), whereas the original places it after the
**otherLock chain** (`12e33e` then pool, `12e340`). The route block and the
scanner/otherLock blocks are otherwise instruction-identical, so this is a
constant-island placement decision, not a missing constant: both islands are
the same five words in the same order (`__stack_chk_guard`, `Globals::Canvas`,
`Globals::status`, `PaintCanvas::DrawImage2D`, `Globals::font`).

## 3. Literal-pool placement experiments

All rows are single edits on the pristine baseline unless noted. `pool1` is
the first island address; `raw %` is the unrounded ratio.

| variant | pool1 | size | raw % |
|---|---|---:|---:|
| **baseline** | `12e2e0` | 9908 | **74.541284** |
| route `expect(onScreen,1)` | `12e1a8` | 9916 | 73.125985 |
| route `expect(onScreen,0)` | `12e1a8` | 9920 | 71.836033 |
| route `expect(scannerAvailable,1)` | `12e2e0` | 9908 | 74.541284 |
| route `expect(scannerAvailable,0)` | `12e2e0` | 9916 | 72.198338 |
| route `expect(onScreen,0)+expect(scanner,1)` | `12e1a8` | 9920 | 71.836033 |
| route `expect(onScreen,1)+expect(scanner,1)` | `12e1a8` | 9916 | 73.125985 |
| route `expect(route!=nullptr,1)` | `12e2e0` | 9908 | 74.541284 |
| route `expect(route!=nullptr,0)` | `12e2e0` | 9924 | 70.755123 |
| nested `else { if (scannerAvailable) … }` | `12e2e0` | 9908 | 74.541284 |
| `if (!onScreen) { if (scanner) … } else …` | `12e2e0` | 9908 | 74.541284 |
| scanner-check-first `if (!onScreen && scanner) … else if (onScreen)` | `12e2e0` | 9908 | 74.541284 |
| `onScreen != 0` / `scannerAvailable != 0` | `12e2e0` | 9908 | 74.541284 |

The scanner block is only moved **before** the island by hinting the route
`onScreen` branch, and that also re-rolls the whole body (score 71–73). The
spellings that keep the island at `12e2e0` are codegen-identical to the
baseline.

### 3a. `Status *status` declaration position

The local `status` is used only in the landmark guard (lines 424–427); all
other uses are `Globals::status` directly. Moving the local after the
`otherLock` chain moves the island to `12e310` but flips the `otherLock` chain
from the original's straight `cbnz` form to a `cbz` form and costs ~1.3
points:

| variant | pool1 | size | raw % |
|---|---|---:|---:|
| **baseline** (`status` before chain) | `12e2e0` | 9908 | **74.541284** |
| `status` after chain | `12e310` | 9920 | 73.284630 |
| `status` after `notTurretMode` | `12e310` | 9920 | 73.284630 |
| `status` after `landmarkHit` | `12e310` | 9920 | 73.284630 |
| no local (`Globals::status` in guard) | `12e310` | 9912 | 72.266017 |
| `otherLock` declared before `status` | `12e2e0` | 9908 | 74.541284 |
| `notTurretMode` declared first | `12e2e0` | 9908 | 74.541284 |
| `landmarkHit` declared first | `12e2e0` | 9908 | 74.541284 |

The coupling is exactly the wave-122 finding: the straight chain and the
original island slot cannot both be obtained from one source spelling.

### 3b. `otherLock` chain expect grid

Nine outer/inner expect combinations were measured. The island stays at
`12e2e0` for every form that keeps the straight chain; only `outer=1` moves it
to `12e2e8` and drops the score to 70.9. Baseline
(`outer expect 0`, `inner gas expect 1`) remains the best.

### 3c. Route block moved after the `otherLock` chain

Moving the whole `if (this->route != nullptr)` block to after the `otherLock`
chain moves the island to `12e328` (original `12e340`, only `0x18` short) but
re-orders the route/scanner/otherLock blocks and shrinks the body to 9844 B
(67.786873 %). The island is placed immediately after the block containing the
last `Globals::font` use, so the only way to reach `12e340` is to put the
route block *after* the `otherLock` chain in the final layout — which the
original does not do.

## 4. Block-order experiments (planet-lock tail vs landmark loop)

The original layout is: planet block → planet-lock tail (`12e748`) → island
(`12e79c`) → landmark loop (`12e7a8`) → landmark tail → cold `otherLock`
(`12ebc2`) → cold `missionBlocksPlanets` (`12ebda`) → enemies (`12ec24`).
The baseline layout is: planet block → invented recompute (`12e722`) →
landmark loop (`12e776`) → planet-lock tail (`12ebec`) → enemies.

Swapping the planet-lock tail and the invented recompute in source
(`tail; recompute` instead of `recompute; tail`) gives a much closer address
layout:

| anchor | original | baseline | tail-before-recompute |
|---|---:|---:|---:|
| landmark `isVisible` | `12e7f4` | `12e7c8` | `12e828` |
| enemy `getType` | `12ee90` | `12eeac` | `12eeb6` |
| gas `getSparks` | `12f460` | `12f454` | **`12f460`** |

but it scores only **71.451082 %** (9928 B) because the recompute is now
inserted between the planet-lock tail and the landmark loop: the largest
single diff run is 45 instructions (original `b.n` after the tail vs the whole
recompute). Removing the recompute instead moves the planet-lock tail to
`12e758` (original `12e748`) but outlines the landmark loop to `12f490`
(63.451850 %). The recompute is load-bearing for keeping the landmark loop
before the enemy section.

Other block-order edits measured this wave:

| variant | size | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.541284** |
| tail before recompute | 9928 | 71.451082 |
| tail before recompute + distance-first `inSight` | 9916 | 66.542431 |
| tail before recompute + asteroid `hasCargo`-first | 9948 | 70.663995 |
| recompute removed | 9832 | 63.451850 |
| route block after `otherLock` chain | 9844 | 67.786873 |
| recompute moved after the landmark block | 9916 | 58.192253 |

## 5. Other source-shape sweeps (all no win)

* **`bool` → `int` for 20 locals** (`missionBlocksPlanets`, `turretMode`,
  `alienOrbit`, `onPlanetCourse`, `landmarkHit`, `topOk`, `planetHit`,
  `bossSeen`, `enemyHit`, `alwaysEnemy`, enemy `wreck`, `navWreck`, `gasHit`,
  `inSight`, `hasDockNav`, `asteroidHit`, `dead`, asteroid `wreck`,
  `candidateWreck`, `ambient`): best tie `int_onPlanetCourse` /
  `int_navWreck` / `int_inSight` / `int_dead` / `int_candidateWreck` /
  `int_ambient` at 74.541284; every other one is lower (worst `int_turretMode`
  66.676234).
* **Loop forms**: `while` / `while (true) { break; }` / `for (;;) { break; }`
  for the landmark, planet, enemy, gas and asteroid loops. `for (;;)` with a
  top `break` is codegen-identical to the baseline for the landmark loop; all
  other forms re-roll (54.9–69.7). No form un-rotates the landmark loop.
* **Landmark guard order** `!landmarkHit && i!=3 && notTurretMode &&
  notOtherLock` → the original's `(!landmarkHit && notOtherLock) &&
  (i!=3 && notTurretMode)` order scores 72.792431; other orders 70.9–74.5.
* **`i==1 || i==3` spellings**: no mask / `(unsigned)i` tie; split
  `expect(...)||expect(...)` 72.78; `((i&0x7fffffff)&~2)==1` 72.76.
* **Planet-lock timer condition** `!missionBlocksPlanets &&
  planetLockTimer >= 1`: `> 0`, `!(missionBlocksPlanets ||
  planetLockTimer < 1)`, `(missionBlocksPlanets | (planetLockTimer < 1)) == 0`
  all tie or lower (72.5 for the `||` order). The original's `orrs r0,r5,r1`
  (missionBlocksPlanets kept live) is not reproducible: clang proves
  `missionBlocksPlanets == 0` on the fall-through path and emits only
  `cmp r0,#1; blt`.
* **Asteroid dead-check** `if (!(hasCargo && (isDead||isDying)) &&
  (isDead||isDying)) continue;` → 9928 B / 71.974796; the baseline's
  `bool dead; if (dead && !hasCargo)` is better.
* **Enemy `inSight`**: distance-first x-pair (faithful) 9896 B / 68.053364;
  distance-first with `int size` 9920 B / 65.025758; direct assignment
  9924 B / 64.625072; `float fcx/fcy` bounds-first ties at 74.541284 but does
  not restore `vpush {d8-d13}`.
* **Random combination search**: 120 random subsets (2–6 edits) of 25
  individually-neutral tweaks (types, declaration order, condition spellings,
  route expects, timer comparisons) — **none exceeded 74.541284 %**.

## 6. Remaining root cause

The function is a tuned local optimum. The residual is dominated by coupled
compiler decisions that source-level reshaping could not steer independently:

1. **First island** — placed after the route block because the route block's
   `Globals::font` use is the last constant use before the cold scanner block.
   The only source lever that moves it (`status` after the chain, route block
   after the chain, route `onScreen` expect) also re-rolls the `otherLock`
   chain or the route/scanner block order and costs 1–7 points.
2. **Planet-lock tail vs landmark loop** — the invented planet-course
   recompute is what keeps the landmark loop before the enemy section; it also
   pushes the planet-lock tail after the landmark loop. Removing it moves the
   tail into place but outlines the landmark loop to `12f490`.
3. **`vpush {d8-d13}`** — only the faithful distance-first `inSight` keeps the
   crosshair floats live in `s24`/`s26`, and that re-colours the whole body
   (68.05 %).
4. **`otherLock`=r5 / `status`=r6** — the original split needs the straight
   `cbnz` chain *and* a late `status` load; no spelling found achieves both.

No source-only edit found this wave improves the baseline.

## 7. Artifacts

Scratch (not committed, `/dev/shm/w7307`): `Radar.base.cpp` (pristine source),
`score_radar.py`, `pool1.py`, `tryv.py`, `runs.py`, `runs.txt`, `show.txt`,
`batch1.py`–`batch22.py`, `search.py`, `norec.dis`, `swap.dis`, `mv.dis`.
The analysis is self-contained in this note; the scratch files are
reproducible from the commands in §1.
