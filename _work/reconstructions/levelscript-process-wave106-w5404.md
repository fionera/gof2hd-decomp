# LevelScript::process — wave106 / w5404 (statement-reorder pass)

Status: research note, 2026-11-04. Branch `wave106-w5404`, base master `486f8a119`.
Target: `_ZN11LevelScript7processEi` (original `0x137d18`, 57232 B / 0xdf90,
20169 insns). Raw fuzzy ratio **54.155469 -> 54.222864** (rounded 54.2 both),
function size stays **57232 B (size-exact)**.

## Build / measurement commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5404
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5404/score.py          # raw ratio for this one function
cmake --build cmake-build-match --target check -j6
```

`score.py` is the same normalization as `tools/verify/verify.py` (literal
pools compared by value, branches by symbol, intra-function targets literal),
then `difflib.SequenceMatcher(autojunk=False)` over the instruction stream.

Gate after the change: **RATCHET PASS**, byte_exact 4500, linked_exact 4500,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0. No baseline
update, no `report.json`/doc change needed (the counts did not move).

## What was landed (9 same-size statement reorders, 1 file, +9/-9 lines)

All are adjacent-statement swaps inside `LevelScript::process`; none changes
the function size and none adds a global, anchor, asm, attribute or padding.

| # | site | swap |
|---|---|---|
| 1 | mission-0 state 4 (~L687) | `setTurretMode(false)` before `sound->play(143,…)` |
| 2 | mission-0 state 3 (~L716) | `getPlayer()->setCollide(true)` before `player->player->field_5e = 0` |
| 3 | case 69 (~L2160) | `m_nState = 2` before `m_bCinematicBreak = 0` |
| 4 | case 69 state 5 (~L2276) | `enemies[10]->setSpeed(8.0f)` before `player->freeze = 0` |
| 5 | case 29 / freighter (~L2559) | `KIPlayer *freighter = …` before `updateReferenceMatrix()` |
| 6 | case 125 (~L2865) | `m_pRadar->enabled = 0` before `AEGeometry *leader = …` |
| 7 | case 125 (~L3539) | `sideZ = rnd->nextInt(100)` before `offsetZ = rnd->nextInt(2000)` |
| 8 | case 91 (~L4627) | `Vector zero{…}` before `Vector pos = …` |
| 9 | case 91 (~L4631) | `m_nScriptTimerB = 1` before `sound->play(2282,…)` |

Items 1, 3, 6, 7 are the pre-existing (uncommitted) w5404 handoff reorders;
2, 4, 5, 8, 9 were found by this pass with adjacent-statement sweeps.

## Measured table (raw ratio, size)

| variant | size | raw % |
|---|---:|---:|
| master `486f8a119` | 57232 | 54.155469 |
| + handoff reorders 1,3,6,7 | 57232 | 54.211561 |
| + `field_5e`/`setCollide` (2) | 57232 | 54.216538 |
| + `freeze`/`setSpeed` (4) | 57232 | 54.217887 |
| + `freighter`/`updateReferenceMatrix` (5) | 57232 | 54.222864 |
| + `zero`/`pos` (8) and `timer`/`play` (9) | 57232 | **54.222864** |

Individual sweeps that produced the wins (each rebuilds + rescores one swap):
`python3 /dev/shm/w5404/sweep_region.py <start-marker> <end-marker>`.
Swept regions: mission-0 arms 4/3/5, case 158, case 69..70, case 29..56,
case 42..94, case 125. All other adjacent swaps in those regions were either
size-changing or <= the current score.

## Best size-changing variant found (not landed)

The faithful original mission-0 state-0 `setPosition` call order is
`enemy[0], enemy[1], enemy[2]` (Thumb `0x137eec`, `0x137f02`, `0x137f14`;
constants `0x1380ec=500`, `0x1380f0=-300`, `0x1380f4=-1700`,
`0x1380f8=-200`). Writing the source in the *non-faithful* order
`enemy[0], enemy[2], enemy[1]` changes global register allocation and scores
**54.945383 raw**, but the function shrinks to **57228 B** (4 short) and the
gate's `byte_exact` count drops (4500 -> ~4322) because every later function's
absolute literal-pool words shift. The 4 bytes are lost in the cold/EH tail
(first size delta at `0x144568`: the `mov r0,r8; blx r2` argument-setup pair
disappears), not in the state-0 block itself; no size-exact formulation of
that order was found (six permutations plus hoisted-temporary variants all
measured 57208..57240). Adding a single real store/call to compensate moves
size by 8, not 4, so it was not landed.

## Proven vs hypothesis

Proven (measured):
* The nine swaps are size-neutral and each is non-regressing; the cumulative
  raw gain is +0.067395.
* The mission-0 state-0 call order in the original is 0,1,2 (literal-pool
  decode of `0x1380c0..0x1380fc`), so the 0,2,1 variant is a tuned shape, not
  a faithful one.
* Function size must stay 57232 B: a 4-byte change drops `byte_exact` by
  ~178 because later functions' absolute pool words move.

Hypothesis / not established:
* The remaining ~45% sequence loss is global greedy register allocation +
  MachineBlockPlacement (consistent with the earlier w3802/w3822 notes), not
  missing logic: the instruction multiset is ~94.6% shared.
* The pre-switch mission-special chain in the binary is
  `5, 0, 81, 26, 78, 50, 89, 51`, whereas our source implements only
  `26, 5, 50, 51`; adding the missing 81/78/89 bodies is real missing code
  but would grow the function past 57232 B unless offsetting code is removed.
