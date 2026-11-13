# Radar::draw wave 191b (w13593) — fresh source-shape angles, definitive post-RA mapping, no win

**Status:** complete, 2026-11-13. Worktree `wt/w13593`, branch `wave191b-w13593`,
base `master` `2ee38c750`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13562.md` (RA root cause), `-w13574.md`
(eviction weights), `-w13582.md` (queue), `-w13587.md` (pair flips).
**No source change landed**: `src/` is untouched, every measured cell is
raw-neutral (6,013) or worse and size-exact unless noted.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,214 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` differing bytes | 0 |
| `.rodata` size delta | 485 |
| `.rel.plt` | byte-identical |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. New evidence: the definitive post-RA register map

The `-mllvm -print-machineinstrs` dump's *After Virtual Register Rewriter*
section (`NoVRegs`, `mir.txt:413436`) shows the exact physical assignment.
Decoded for ours:

| value | ours | original (from disasm) |
|---|---|---|
| `this` | r4 | r4 |
| `__stack_chk_guard` GOT slot | r5 (spilled to `%stack.30`) | spilled |
| `Globals::status` GOT slot | spilled (`%stack.31`) | spilled |
| `Globals::Canvas` GOT slot | spilled (`%stack.35`) | spilled |
| `mission` | r5 (after canary dies) | r5 |
| `alienOrbit` | r8 | r8 (`mov r8,r0` @0x12e0f0) |
| `turretMode` | `%stack.33` (sp+0x94) | sp+0x94 |
| `hud` | `%stack.32` (sp+0x54) | sp+0x54 |
| `player` | `%stack.34` (sp+0x88) | sp+0x88 |
| **shared zero + `missionBlocksPlanets`** | **r10 (sl)** | **r11 (fp)** |
| **`campaign`** | **`%stack.45` (sp+0x90)** | **r10 (sl)** |
| r6 / r9 / r11 | scratch only | r9 scratch; r11 = zero/mbp |

So the *whole* 9,908-byte residual is exactly **one range**: in ours
`campaign` is spilled to `sp+0x90` while the shared zero/`missionBlocksPlanets`
range takes `sl`; the original keeps `campaign` in `sl` and spills the
zero/mbp range to `sp+0x70` (reloaded into r5 at 0x12ffae).  Every other
register and frame slot agrees, and the `mode` slot difference
(sp+0x64 vs sp+0x68) is a downstream consequence of this one swap.

Note that in **ours r6, r9 and r11 are pure scratch registers** — the
allocator had free callee-saves and still chose to spill `campaign`.  That
means the decision is made by the greedy eviction cascade (campaign is the
cheapest evictee, weight 0.02551238 vs `turretMode` 0.02550917), not by a
register shortage at `%2487`'s dequeue.

## 2. Cells measured this pass (all reverted; base 6,013 / 1,965 / 9,908)

### 2.1 `missionBlocksPlanets` definition shape

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `if (type != 172) mbp = 1;` | 6,013 | 9,908 |
| `if (...) mbp = 1; else mbp = 0;` | 8,561 | 9,900 |
| `mbp = !(type == 172);` | 6,013 | 9,908 |
| `mbp = (type == 172) ? false : true;` | 6,013 | 9,908 |
| `bool mbpBool` + `int mbp = mbpBool` at guard | 6,172 | 9,912 |
| `mbp = (int)(intptr_t) mission & 0;` | 6,013 | 9,908 |
| `mbp = (int)(intptr_t) mission` + `else mbp = 0` | 6,013 | 9,908 |
| decl moved before `SetColor` | 6,013 | 9,908 |
| decl moved to function top (before `enabled`) | 6,013 | 9,908 |
| `bool zeroByte` / `int zeroByte` feeding both stores + mbp | 6,013 | 9,908 |
| `bool mbpFalse = false; int mbp = mbpFalse;` | 6,013 | 9,908 |
| `int mbp = 0;` (spelling) | 6,013 | 9,908 |

### 2.2 Zero-store shape

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `plasmaInRange = lockConfirmed = 0;` | 6,015 | 9,908 |
| `lockConfirmed = plasmaInRange = 0;` | 6,013 | 9,908 |
| zero stores moved after `getMission` (before mbp decl) | 6,055 | 9,908 |

### 2.3 `campaign`

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| campaign defined first (before zeros) | 8,646 | 9,900 |
| campaign defined just before the planet guard | 9,236 | 9,904 |
| extra guard term `campaign != -1` (proved away) | 6,013 | 9,908 |

### 2.4 Planet-loop preamble order (`onScreen` vs `stationId`)

The original's order is `update(); if (!onScreen) continue; stationId;
currentStation; warpGate; textOffset` (0x12e3f2-0x12e418).  Ours evaluates
`stationId`/`currentStation` first.

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,013** | 1,965 | 9,908 |
| `onScreen` first (matches binary) | 6,014 | 1,961 | 9,908 |
| `onScreen` after `warpGate` | 6,023 | 2,044 | 9,908 |
| `stationId`/`currentStation` hoisted before `update` | 8,877 | 2,257 | 9,968 |
| `stationValue` hoist (`stations->data()[i]` once) | 8,877 | 2,257 | 9,968 |
| `targets` local for `this->planetTargets` | 6,718 | 2,087 | 9,908 |
| `targets` local (all uses) | 8,880 | 2,106 | 9,904 |

### 2.5 Planet-loop campaign-mission block

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `marked` declared before `campaignMission` | compile-fail (redecl) | – |
| `freelance` declared before `campaignMission` | 9,287 | 9,988 |
| `isTarget && stationId != currentStation` | 6,013 | 9,908 |
| nested `if (stationId != ...) { if (isTarget) ... }` (campaign) | 6,013 | 9,908 |
| same for freelance | 6,013 | 9,908 |
| same for warp gate | 6,013 | 9,908 |
| all three nested | 6,013 | 9,908 |
| `marked` as `int` | 8,464 | 9,912 |
| `textOffset` as `unsigned` | 6,013 | 9,908 |
| `for (int i ...)` / `!=` / `size_t` in planet loop | 6,013 | 9,908 |
| `for (uint32_t i ...)` in landmark loop | 6,013 | 9,908 |

### 2.6 `otherLock` / early-block reshapes

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `otherLockFlag = !(a==null && b==null && c==null) \|\| field_0x14 != 0` | 6,013 | 9,908 |
| `otherLockFlag = a!=null \|\| b!=null \|\| c!=null \|\| field_0x14 != 0` | 6,013 | 9,908 |
| `otherLock = (int)(a!=null \|\| ...)` (2nd computation) | 6,013 | 9,908 |
| `if (!enabled) return;` (braces removed) | 6,013 | 9,908 |
| `if (enabled == false)` | 6,013 | 9,908 |
| whole body wrapped in `if (this->enabled) { ... }` | 6,013 | 9,908 |
| `bool alwaysEnemySeen/alwaysEnemy` moved to `enemies:` label | 6,013 | 9,908 |
| (individually and together) | 6,013 | 9,908 |
| `onPlanetCourse` as `int` | 6,013 | 9,908 |
| `onPlanetCourse` `else if` restructure | 6,013 | 9,908 |
| `this->field_0x1a8` store moved after the 4 `DrawImage2D`s | 9,490 | 9,852 |
| last two `DrawImage2D` swapped | 6,049 | 9,908 |
| `turretMode` declared after `campaign` | 6,039 | 9,908 |
| route block moved after `otherLock` | 6,013 | 9,908 |

### 2.7 `missionBlocksPlanets` use-site copies (shorten its live range)

| cell | raw | size |
|---|---:|---:|
| `bool blocksPlanets = missionBlocksPlanets;` before planet guard | 6,013 | 9,908 |
| same, at top of planet-guard body | 6,013 | 9,908 |
| same, at top of the `planetHit` block | 6,013 | 9,908 |

All three coalesce away — the vreg is unchanged.

### 2.8 `__builtin_expect` removal sweep (29 sites, one at a time)

Removing each `__builtin_expect` in `draw` and re-measuring.  Nothing beats
6,013; the closest are:

| site removed | raw | size |
|---|---:|---:|
| mission guard `!isEmpty()` | 6,014 | 9,908 |
| landmark `onScreen` | 6,043 | 9,908 |
| `getCurrentCampaignMission() != 120` | 6,023 | 9,908 |
| `!lockConfirmed` | 6,019 | 9,908 |
| `field_0x18 != 0` | 6,015 | 9,908 |
| `!isDockedToDockingPoint` | 6,023 | 9,908 |
| `dockNavPtr != current` | 6,016 | 9,908 |
| everything else | 6,013–9,284 | 8,996–9,956 |

### 2.9 `__builtin_expect` added to previously unhinted branches (22 cells)

Wrapping the first operand of each of 11 unhinted `if`s with
`expect(..., 0)` and `expect(..., 1)`:

| site | e0 raw | e1 raw |
|---|---:|---:|
| `route != nullptr` | 9,188 (9,924) | 6,013 |
| `!turretMode` | 8,863 (9,960) | 8,453 (9,912) |
| `lockedStation != nullptr` | 8,556 (9,928) | 6,013 |
| `stationLockTimer > field_0x1b8` | 8,397 (9,936) | 6,013 |
| `stationLockTimer > 0` | 8,394 (9,940) | **6,159** (9,908) |
| `freelance != nullptr` | 8,540 (9,976) | 6,013 |
| `!otherLock` | 9,333 (9,972) | 9,086 (9,928) |
| `planetLockTimer > field_0x1b8` | 6,787 (9,912) | **6,035** (9,908) |
| `field_0x1a8` | 6,151 (9,912) | 6,013 |
| `planetLockTimer < 1` | 6,013 | 6,013 |
| `missionBlocksPlanets` | 6,836 (9,912) | 9,247 (9,948) |

Plus `expect(current == 143, 0)` → 6,015 and `, 1` → 6,235 (9,896).

### 2.10 Semantic fixes re-measured on this base

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,013** | 1,965 | 9,908 |
| music tail `A \|\| C \|\| B` (matches binary) | 6,036 | 1,963 | 9,908 |
| asteroid candidate arm `= 0` + `+= mode` in else (matches binary) | 6,190 | 1,960 | 9,908 |
| sound ladder `inAlienOrbit` first (matches binary) | 6,028 | 1,960 | 9,908 |
| all three semantic fixes together | 6,213 | 1,953 | 9,908 |
| all three + `onScreen`-first | 6,214 | 1,949 | 9,908 |

So the three known semantic divergences are all real and all raw-worse on
this base, exactly as w13562/w13582 reported.

### 2.11 Music-tail branch swap

`if (expect(field_0x1bc >= 1, 1)) {A} else {B}` → `if (expect(field_0x1bc < 1, 0)) {B} else {A}`:
raw 6,017, aligned 1,969, size 9,908.

### 2.12 `-mllvm` flag probes (harness of w13574 §5)

| flag | raw | aligned | size |
|---|---:|---:|---:|
| `-split-spill-mode=default` (SM_Partition) | 9,060 | **3,111** | 9,964 |
| `-huge-size-for-split=100/1000/20000/100000` | compile-fail | – | – |
| `-regalloc-csr-first-time-cost=1` | 6,013 | 1,965 | 9,908 |
| `-exhaustive-register-search` | 6,013 | 1,965 | 9,908 |

`SM_Partition` is the only knob seen so far that makes the instruction
*stream* look like the original (aligned 3,111 vs 1,965) but it costs
+56 B.

### 2.13 Block-order reorders (fresh)

The binary lays the planet loop out *before* the landmark loop
(planet 0x12e3d4, landmark 0x12e7a8) even though the source has the
landmark block first; the compiler reorders both builds the same way.
Moving the second `otherLock = ...` computation above the landmark block,
or swapping the landmark and planet blocks in the source, changes the
instruction *alignment* a lot but is size-fatal and raw-worse:

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,013** | 1,965 | 9,908 |
| second `otherLock` moved above the landmark block | 9,193 | 2,618 | 9,896 |
| landmark block and planet block swapped | 9,506 | 2,509 | 9,852 |
| `otherLock`-above + `onScreen`-first | 9,115 | 2,637 | 9,900 |
| `otherLock`-above + music `A,C,B` | 9,185 | 2,612 | 9,912 |
| `otherLock`-above + `onScreen`-first + music `A,C,B` | 9,138 | 2,625 | 9,916 |

So the alignment metric prefers the original's *block order*, but the raw
metric is dominated by the single RA colouring and every reorder only adds
byte differences.

### 2.14 Weight-perturbation attempts on `campaign`

Adding a provable-but-not-folded extra use of `campaign` (to raise its
weight above `turretMode`'s 0.02550917 and make `turretMode` the cheapest
evictee) is size-fatal in every form tried:

| cell | raw | size |
|---|---:|---:|
| `campaign >= 2 && campaign <= 1000` in the guard | 8,547 | 9,920 |
| `campaign >= 2 && campaign < 200` in the guard | 8,449 | 9,912 |
| `campaign >= 2 && campaign != 143` in the guard | 9,576 | 9,852 |
| `campaign >= 2 && campaign >= 0` (proved away) | 6,013 | 9,908 |

Also: inlining `notTurretMode`/`notOtherLock` into the landmark test
(9,561 / 9,868), `targets` local for `this->planetTargets`
(6,718 / 9,908 and 8,880 / 9,904), pointer-truthiness spellings (6,013).

## 3. Conclusion

The residual is a single greedy-RA eviction: `campaign` (weight
0.02551238, hinted) is the cheapest evictee of the first large unhinted
GOT-load range, while `turretMode` (0.02550917) — a 0.0126 % lighter
range — survives.  The original's colouring keeps `campaign` in `sl` and
puts the shared zero/`missionBlocksPlanets` range in `fp`.

Every source spelling tried this pass leaves the pre-RA IR byte-stable
(the ~160 cells above collapse to identical codegen).  The only cells that
change the IR are the ones that change block order or branch weights, and
all of them are raw-neutral or worse.  The correct source shapes (planet
preamble `onScreen`-first, music `A,C,B`, sound ladder `inAlienOrbit`-first,
asteroid `= 0`/`+= mode`) are all individually size-exact but raw-worse
because the RA cascade dominates.

The one lever that reaches the original's *instruction stream* is
`-mllvm -split-spill-mode=default`, but it is size-fatal (9,964).  No
size-exact raw win was found; `src/` is untouched and the worktree is clean.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13593/`: `cell.py`
(1 s compile+link+strip+fn_diff cell, `measure([(old,new)...], tag)`),
`Radar.base.cpp` (pristine source), `batchW.py`–`batchWC.py` (the sweeps
above, results in `cellsW*.json`), `calldiff2.py` (call-sequence diff),
`mir.txt` (`-print-machineinstrs`; pre-RA at 395123, post-RA at 404142,
VRR at 413436), `orig.dis` / `ours.dis`.
