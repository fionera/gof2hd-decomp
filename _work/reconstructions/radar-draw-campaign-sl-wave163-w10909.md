# Radar::draw wave 163 (w10909) — `campaign` in `sl` via hot-guard expects

**Status:** source landed (gate green). Worktree `wt/w10909`, base master
`e51db2fc7` (wave 162). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi`. Original/ours both **9,908 B** exact.

Read with `wave162-merge-tests.md`,
`radar-draw-campaign-sl-wave162-w10809.md`.

## 1. Result

| metric | base (wave 162) | this shape |
|---|---:|---:|
| raw same-address diff | **8,124** | **9,611** |
| instruction-index alignment | **1,320** | **3,605** |
| function size | 9,908 | **9,908** |
| `.text` differing bytes | 85,042 | 86,529 |
| `.ARM.extab` differing bytes | 725 | 754 |
| `.note.gnu.build-id` | 20 | 20 |

The requested register assignment is achieved: `campaign` is in `sl`
(`mov sl, r0` after `getCurrentCampaignMission`, `cmp.w sl, #2` at the
`campaign >= 2` guard).  The cost is a block-layout change (below), so the
base remains better on both raw and aligned bytes.  This is a structural
stepping stone, not a raw win.

## 2. Source shape

Three hot-guard `__builtin_expect` cells, all with the Radar::draw lint
waiver:

```cpp
if (__builtin_expect(this->route != nullptr, 1)) {                 // route block hot
    ...
    if (__builtin_expect(this->onScreen, 1)) {                     // route onScreen hot
...
if (__builtin_expect(this->landmarks != nullptr, 0) &&             // landmark block cold
    !(Globals::status->getStation()->getIndex() == 109 || ...
```

`landmarks != nullptr` expected 0 is the cell that flips the allocator;
`route != nullptr` expected 1 and the route `onScreen` expected 1 are the
+8 B companion that restores the exact 9,908 B size.

## 3. Register map (ours, this shape)

| value | original | base ours | this shape |
|---|---|---|---|
| `campaign` | `sl` (r10) | `[sp,#144]` | **`sl` (r10)** |
| `alienOrbit` | `r8` | `r8` | `fp` (r11) |
| zero / `missionBlocksPlanets` | `fp` (r11) | `sl` (r10) | `r2`/`r1` (caller-saved) |
| mode | `[sp,#100]` | `[sp,#104]` | `[sp,#108]` |
| status ptr | `[sp,#156]` | `[sp,#156]` | `[sp,#164]` |
| frame | `sub sp,#296` | `sub sp,#296` | `sub sp,#304` |

Prologue (ours, this shape):
```
12e07a  movs r2,#0                 ; zero
12e07e  movs r1,#0
12e0f2  mov  fp,r0                 ; alienOrbit
12e0fa  mov  sl,r0                 ; campaign   <-- requested
12e32c  cmp.w sl,#2                ; campaign >= 2
```
Original:
```
12e07a  mov.w fp,#0                ; zero / mbp
12e0f0  mov  r8,r0                 ; alienOrbit
12e0f8  mov  sl,r0                 ; campaign
12e38a  cmp.w sl,#2
```

## 4. Why the flip needs a layout change

The greedy allocator enqueues global live ranges by **size** (`Prio =
(1<<29) + Size`, larger first; `RegAllocGreedy::enqueue`).  In the base,
the coalesced zero/`missionBlocksPlanets` vreg `%2482` is the larger range
(MIR liveness ≈ 25.5k slot units vs `campaign` `%418` ≈ 16.4k), so `%2482`
wins `sl` and `campaign` is the smallest range left and spills.  Extending
`campaign`'s range by hoisting its definition cannot exceed `%2482`'s span
(the use is fixed at the guard), so a pure source reorder cannot flip it.

The only size-exact shapes that flip it change the block layout so that
`%2482` is no longer the largest global range:

* mission block outlined (`__builtin_expect(!mission->isEmpty(), 0)`):
  `campaign` → `sl`, size 9,868 (-40) or 9,920 (+12) depending on the
  companion cell;
* landmark block cold (`__builtin_expect(landmarks != nullptr, 0)`):
  `campaign` → `sl`, size 9,900 (-8); + route/onScreen hot cells restores
  9,908.

All of them grow the frame from 296 to 304 and move `alienOrbit` off `r8`.

## 5. Measured, not landed

| shape | size | raw | aligned | note |
|---|---:|---:|---:|---|
| base | 9,908 | 8,124 | 1,320 | best raw + aligned |
| `landmarks` 0 + `route` 1 + route `onScreen` 1 (landed) | 9,908 | 9,611 | 3,605 | campaign `sl` |
| `landmarks` 0 + route `onScreen` 1 | 9,908 | 9,612 | 3,605 | campaign `sl` |
| `landmarks` 0 only | 9,900 | 9,592 | — | campaign `sl`, 8 B short |
| `!isEmpty` 0 + `campaign>=2` 1 | 9,920 | 9,590 | 1,709 | campaign `sl`, 12 B long |
| `!isEmpty` 0 only | 9,868 | 9,508 | — | campaign `sl`, 40 B short |
| `volatile missionBlocksPlanets` | 9,828 | 9,466 | 2,730 | campaign `sl`, 80 B short |
| `campaign` before `alienOrbit` | 9,908 | 8,131 | 1,325 | still spilled |
| `campaign` before `turretMode` | 9,900 | 8,498 | — | still spilled |
| `campaign` before mission block | 9,900 | 8,550 | — | still spilled |
| `long`/`unsigned`/`const`/`int64` campaign | 9,908 | 8,124 | 1,320 | still spilled |
| `int`/`char`/`long` `missionBlocksPlanets` | 9,904 | 8,242 | 1,366 | still spilled |
| `bool`/`char`/`long` `alwaysEnemySeen` | 9,908/9,912/9,908 | — | — | still spilled |
| declaration-only hoists | 9,908 | 8,124 | 1,320 | vreg created at assignment |

## 6. Gate

`check` green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, parity 5370/5370.  All 27 section sizes exact; file size
2,187,328 B; `.rel.plt` byte-identical.  Section *diff* bytes rise
(`.text` 85,042 → 86,529, `.ARM.extab` 725 → 754) because Radar::draw is
still one of the 17 non-exact functions.

## 7. Reproduction

```sh
python3 /dev/shm/w10909/fdiff.py _ZN5Radar4drawEP6PlayerP3Hudi
python3 /dev/shm/w10909/align_diff.py _ZN5Radar4drawEP6PlayerP3Hudi
```
