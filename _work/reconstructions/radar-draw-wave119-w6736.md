# Radar::draw pass (wave119, w6736) — block placement is the whole game

**Status:** research note, 2026-11-05. Worktree `wt/w6736`, branch
`wave119-w6736`, based on current `master` (after
`radar-draw-wave119-w6721.md`, which landed the local `String` in the
landmark `i == 0` arm). **No source change landed**; this commit contains
this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908
official verify score 74.4  (exact difflib ratio 74.426606 %)
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

The official `verify.py` score is rounded to one decimal, so a variant must
beat **74.45 %** to display as 74.5. Every score below is the unrounded
`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`.

## 1. What is actually different

The body is ~90 % instruction-identical to the original. The residual is
almost entirely **basic-block placement** plus the register/stack-slot
colouring that follows from it, not source semantics. Confirmed differences:

### 1.1 The "invented" recompute block is load-bearing

Our source has two `onPlanetCourse` recomputes: the prologue one (source
lines 369-386) and a second `if (!alienOrbit) { ... }` after the planet loop
(lines 612-624). The original has only **one** (call counts: one
`getAutoPilotTarget`, one `goingToPlanet`, one `isDockingToPlanet`, one
`getStationEnumIndex`, two `getStations`, five `getSystem`).

Deleting the second block makes the call multiset exact and makes the planet
loop's exit branch land on the planet-lock tail exactly as the original does,
but clang then re-lays-out the *whole* function: the enemy loop moves from
`0x12ec60` to `0x12e7ec`, and three 0x100-byte windows (`0x12e850`,
`0x12e950`, `0x12ea50`) drop to **0 %** positional match. Score
**63.746398 %** (9824 B).

The reason is visible in the block graph. Original:

```
0x12e3d4 planet loop ... 0x12e3dc bcs.w 0x12e748   ; exit -> planet tail
0x12e6ee recompute (cold, reached from 0x12e1b6)    ; falls through
0x12e748 planet-lock tail                           ; -> enemies
0x12e7a8 landmark loop
```

Ours:

```
0x12e3d4 planet loop ... 0x12e3dc bcs.w 0x12e72c   ; exit -> recompute
0x12e72c recompute (also reached from 0x12e1b6)     ; b 0x12ebe8
0x12e780 landmark setup / loop
0x12ebd8 planet-lock tail                           ; -> enemies
```

So in the original the loop-exit successor is the planet tail and the cold
recompute is merely *adjacent* to it; in ours the loop-exit successor is the
recompute block and the planet tail is placed after the landmark loop. That
single block-placement decision is worth ~10.7 points.

### 1.2 Prologue: `vpush {d8-d11}` vs `{d8-d13}`

Original `vpush {d8-d13}` (6 float regs); ours `{d8-d11}` (4). The two extra
are `s24`/`s26` in the enemy `inSight` block (`0x12ef5a..0x12efa4`): the
original converts `crosshairPos.x/y` to `int` **before** `VectorLength` and
keeps them live across the call. The original's condition order is

```
VectorLength(egoPos - targetPos) < 60000.0f
  && screenX > cx - size && screenX < cx + size
  && screenY > cy - size && screenY < cy + size
```

Reordering to this exact shape (`insight` below) does reproduce
`vpush {d8-d13}` and matches the `inSight` block instruction-for-instruction,
but it re-colours everything and the score drops to **66.743251 %**
(9864 B). The `mode` slot moves from `[sp,#112]` to `[sp,#104]` (original
`[sp,#100]`) and the enemy-loop tail `0x12fe50`/`0x12ff50` falls to
9.5 % / 16.3 %.

### 1.3 `mode` stack slot

Original `str r3,[sp,#100]`; baseline `[sp,#112]`. `hud` (`[sp,#136]`) and
the canary (`[sp,#292]`) match, so the 296-byte frame is right. `mode` is
loaded fresh at the five `+= mode` sites in both. The `insight` reorder gets
it to `[sp,#104]`, i.e. the two lowest 4-byte slots are *swapped* relative
to the original (original: `mode`=100, hot slot=104; insight: hot slot=100,
`mode`=104). Moving the declaration of `otherLock`, `landmarkHit` or
`onPlanetCourse` to the top of the function does not move it (all ties).

### 1.4 `otherLock` register split

Original keeps `otherLock` in **r5** and re-materialises `Globals::status`
into **r6** at `0x12e31a`. Ours keeps `status` in **r5** and `otherLock` in
**r9**. The original also does not merge the second/third null checks
(`ldr [r4,#56]; cbnz; ldr [r4,#28]; cmp; beq`), while ours if-converts them
into `itt eq; ldreq; cmpeq; beq.w`. Every re-spelling tried here (plain
`&&`, `||` chain, `bool`, per-term `__builtin_expect`, `int otherLock = 1`
init) is a tie or worse — see §3.

### 1.5 Landmark hit-test order is already correct

The previous handoff claimed the original's landmark hit-test evaluates
`screenX > left`, `screenX < left+2*size`, `screenY > top`,
`screenY < top+2*size` while ours uses a `topOk` hoist. That is **not** the
case for the current baseline. The original at `0x12e898..0x12e8b2` is

```
cmp  r0, r3          ; screenX > left
add  r5, r6, r2<<1   ; top + 2*size
cmp  r1, r5          ; screenY < top+2*size
cmp  r1, r6          ; screenY > top
add  r1, r3, r2<<1   ; left + 2*size
cmp  r0, r1          ; screenX < left+2*size
```

and the baseline emits the identical six-instruction sequence at
`0x12e874..0x12e88e` (same order, same registers, same branches). No change
is needed here.

### 1.6 Other blocks are already instruction-identical

The enemy lock-timer block (`0x12f8b0` original vs `0x12f8e0` ours) and the
gas-cloud block (`0x1301c6..0x130250` in both) match instruction for
instruction, including the `Sprite *blip` locals. The station lock-timer
block at `0x1303be` differs only in that ours uses `r5 = &this->blipSprite`
with `[r5,#176]`/`[r5,#196]` for `stationLockTimer`/`field_0x1b8` while the
original uses `[r4,#420]`/`[r4,#440]`; adding a `Sprite *blip` local there
makes it *worse* (71.596783 %).

## 2. Per-0x100-byte positional match map (baseline)

| address | match | | address | match |
|---|---:|---|---|---:|
| 0x12e050 | 92.5 % | | 0x12f050 | 66.7 % |
| 0x12e150 | 97.6 % | | 0x12f150 | 81.9 % |
| 0x12e250 | 71.9 % | | 0x12f250 | 85.9 % |
| 0x12e350 | 74.4 % | | 0x12f350 | **41.8 %** |
| 0x12e450 | 71.1 % | | 0x12f450 | 87.2 % |
| 0x12e550 | 79.2 % | | 0x12f550 | 73.9 % |
| 0x12e650 | 60.4 % | | 0x12f650 | 86.0 % |
| 0x12e750 | **44.3 %** | | 0x12f750 | 81.2 % |
| 0x12e850 | 68.2 % | | 0x12f850 | 83.7 % |
| 0x12e950 | 48.4 % | | 0x12f950 | 90.8 % |
| 0x12ea50 | 70.4 % | | 0x12fa50 | 73.9 % |
| 0x12eb50 | 76.8 % | | 0x12fb50 | 67.8 % |
| 0x12ec50 | 62.1 % | | 0x12fc50 | 84.8 % |
| 0x12ed50 | 68.4 % | | 0x12fd50 | 70.7 % |
| 0x12ee50 | 55.2 % | | 0x12fe50 | 67.3 % |
| 0x12ef50 | 50.6 % | | 0x12ff50 | 82.4 % |
| 0x130050 | 84.5 % | | 0x130350 | 81.9 % |
| 0x130150 | 98.9 % | | 0x130450 | 61.4 % |
| 0x130250 | 84.7 % | | 0x130550 | 88.5 % |
| | | | 0x130650 | 100.0 % |

`0x12e750` (planet tail / recompute) and `0x12f350` (enemy lock-image
selection) are the two worst windows; both are block-placement casualties,
not source errors.

## 3. Variants measured this wave

Single edits on the pristine baseline unless noted. `usize` is the emitted
function size; `raw %` is the unrounded ratio.

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.426606** |
| planet-tail `else if (field_0x14 != 0)` swapped arms | 9908 | 74.426606 (identical code) |
| prologue `onPlanetCourse` as `else if` instead of nested `if` | 9908 | 74.426606 (identical code) |
| `int elapsed = mode` early/late (5 sites) | 9908 | 74.426606 (identical code) |
| `otherLock` declared at function top | 9908 | 74.426606 |
| `landmarkHit` declared at function top | 9908 | 74.426606 |
| `onPlanetCourse` declared at function top | 9908 | 74.426606 |
| `alwaysEnemySeen` moved next to `bossSeen` | 9908 | 74.426606 |
| `missionBlocksPlanets`/`alwaysEnemySeen` decl swap | 9908 | 74.426606 |
| `campaign` declared after `planetTargets` | 9908 | 74.340596 |
| planet tail `!(turretMode \|\| landmarkHit)` | 9908 | 74.426606 |
| planet tail `!landmarkHit && !turretMode` | 9908 | 74.426606 |
| planet tail `__builtin_expect(...,1)` | 9908 | 71.676466 |
| planet tail `__builtin_expect(...,0)` | 9908 | 61.650694 |
| `int inSight` | 9908 | 74.426606 |
| `int hasDockNav` | 9908 | 74.426606 |
| `int candidateWreck` | 9908 | 74.426606 |
| `int navWreck` | 9908 | 74.426606 |
| `int isTarget` | 9908 | 74.426606 |
| `int marked = 0` | 9908 | 74.426606 |
| `int ambient` | 9908 | 74.426606 |
| `bool otherLock` (decl only) | 9908 | 74.426606 |
| `int gashit`→`bool`→`int` | 9908 | 74.397936 |
| `otherLock` `__builtin_expect` on `field_0x1c` only | 9908 | 74.397936 |
| `otherLock` `int otherLock = 1` init form | 9908 | 74.397936 |
| `otherLock` both inner expects | 9908 | 74.426606 |
| `otherLock` plain `&&` (no expect) | 9908 | 73.193807 |
| `otherLock` `\|\|` chain | 9908 | 73.193807 |
| `otherLock` inner expect on `lockedAsteroid` | 9908 | 72.341645 |
| `otherLock` outer `expect(...,1)` | 9908 | 71.014076 |
| `bool otherLock` (whole var) | 9908 | 67.375377 |
| `int missionBlocksPlanets` | 9908 | 73.211982 |
| `bool alwaysEnemySeen` | 9908 | 72.323348 |
| `int landmarkHit` | 9908 | 70.508961 |
| `int planetHit` | 9908 | 71.887550 |
| `int enemyHit` | 9908 | 70.466915 |
| `int bossSeen` | 9908 | 72.594292 |
| `int wreck` | 9908 | 72.406877 |
| `int asteroidHit` | 9908 | 69.284793 |
| `bool notOtherLock` | 9908 | 71.311828 |
| `bool notTurretMode` | 9908 | 70.704751 |
| `bool notOtherLock` + `bool notTurretMode` | 9908 | 71.311828 |
| `unsigned i` in landmark loop | 9908 | 73.180516 |
| landmark gate `__builtin_expect(...,0)` | 9904 | 72.055030 |
| landmark gate `__builtin_expect(...,1)` | 9864 | 63.089944 |
| `!landmarkHit && notOtherLock && i != 3 && notTurretMode` | 9876 | 72.014925 |
| `!landmarkHit && i != 3 && notOtherLock && notTurretMode` | 9916 | 70.167886 |
| `Sprite *blip` local in station lock-timer block | 9860 | 71.596783 |
| **`insight`** VectorLength-first, x/x/y/y | 9864 | 66.743251 |
| **`norec`** delete second `onPlanetCourse` recompute | 9824 | 63.746398 |
| `i0locklabel` revert wave-119 local `String` | 9908 | 74.029509 |
| move landmark block after planet block, keep recompute | 9816 | 56.392496 |
| move landmark block after planet block + `norec` | 9728 | 67.448595 |
| planet tail hoisted out of the `campaign >= 2` block (shared `planetHit`) | 9924 | 55.757923 |
| `insight` + `i0locklabel` | 9908 | 67.517202 |
| `i0locklabel` + `bool otherLock` | 9908 | 67.853042 |
| `insight` + `bool otherLock` | 9908 | 48.219414 |
| `insight` + `i0locklabel` + `bool otherLock` | 9908 | 48.199684 |

## 4. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.426606 %**.
The baseline is a strong local optimum and, as `radar-draw-wave119-w6721.md`
found, the tuned shapes score above the faithful ones.

The one property that would unlock the faithful source (no second recompute,
original `inSight` order) is **block placement**: the original puts the
planet-lock tail at the planet loop's exit and the cold recompute adjacent to
it, while clang 7.0.2 at `-Oz` puts the recompute at the exit and the tail
after the landmark loop. Everything else (the `mode` slot, the
`otherLock`/`status` r5/r6 split, the `inSight` `s24`/`s26` liveness) is a
downstream consequence of that placement. A future pass should try to force
the original ordering from source (e.g. by making the planet tail a `goto`
target that the loop-exit path shares with the landmark-tail path) before
re-attempting the faithful `inSight` reorder.

## 5. Scratch

All scratch under `/dev/shm/w6736` (not committed): `Radar.base.cpp`
(pristine baseline source), `vscore.py` (unrounded scorer), `try.sh`,
`regionmatch.py` (per-0x100-byte positional map), `showblk.py` (largest
difflib opcodes with side-by-side instructions), `orig.dis` / `ours.dis`
(NDK objdump dumps of `0x12e050..0x130700`), and the variant scripts
`p_*.py` / `p_batch*.py` / `p_combo2.py` / `combo.py`.
