# LevelScript::process — wave 191b / w13635: mission-state timer/teardown reorders

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13635`, branch
`wave191b-w13635`, base master `57ce1a4c8` (raw **51,513**, `.text` 71,197,
`.ARM.extab` 577, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

## Lens

Second parallel angle on the largest residual: the per-mission state-machine
timer / teardown arms of the campaign `switch` (not the case-102 awake block or
the case-14/42 teardown already landed).  A normalised instruction diff
(`objdump` + `tools/verify/verify.py` `normalize`/`reconcile`, aligned with
`difflib`) shows only **143 differing instruction *blocks*** in the whole
57,232 B body, and they are all in source lines 605-1510 (the mission-0/1
intros, the shared active tail and the switch guard).  Every per-mission case
from line 1510 onward compiles to the same normalised instruction stream as the
original; the residual raw bytes there are register-allocation / block-placement
artefacts (the `this`→`sl` vs `this`→`fp` prologue swap documented by w13570),
not wrong statements.

Fresh distance-1..6 statement transposition and window-3..6 rotation sweeps
were re-run on this merged base (the earlier notes ran on older bases).  Every
size-exact / `.rel.plt`-identical raw win is listed below; the three
semantics-preserving cells were landed.

## Landed changes (all independent-statement reorders)

1. **case-14 teardown** (~1613): swap `player->setComputerControlled(true);`
   past the independent `m_pCamera->translate(...)` / `m_pHud->visible = 0;`
   pair.  `setComputerControlled` writes only the player control flag; the
   camera translate and HUD visibility are disjoint.
2. **case-157 state-2 teardown** (~5080): swap
   `m_pGeometry2->setPosition(...)` and `m_pGeometry2->setVisible(true)` (both
   plain `AEGeometry` setters; no statement between them reads the other).
3. **case-92 state-4 timer** (~3048): rotate
   `m_nScriptTimerA += delta;` from the front of the block to after
   `wcamera->setTarget(leader);`.  The timer add is independent of the leader
   geometry fetch / canvas fetch / camera target write.

Measured alone and composed on the real build:

```
                         base        landed
process raw diffbytes    51,513      51,509    (-4)
orig / ours size         57232       57232     (exact)
.text diffbytes          71,197      71,193    (-4)
.ARM.extab diffbytes     577         577       (size/base exact, 156,312 B @ 0x1a3b78)
.ARM.exidx diffbytes     0           0         (byte-identical)
.rodata diffbytes        0           0         (sh_size 269,159)
.rel.plt                 md5 289b2a06…  md5 289b2a06…  (byte-identical, 29,392 B)
byte_exact               4508        4508
linked_exact             4508        4508
size-exact               4524/4524   4524/4524
data_exact               843/843     843/843
imports                  207         207
lint                     CLEAN ✓     CLEAN ✓
drift                    0           0
symbol parity            5370/5370   5370/5370
RATCHET                  PASS        PASS
```

## Candidate table (exact NDK r18b `-Oz` compile + full gold link per cell)

| site | edit | raw | size | verdict |
|---|---|---:|---:|---|
| 1613/1615 | `setComputerControlled` ↔ `m_pHud->visible` | 51,512 | 57,232 | **landed** |
| 5080/5082 | `setPosition` ↔ `setVisible` (geometry2) | 51,512 | 57,232 | **landed** |
| 3048-3051 | rotate `m_nScriptTimerA += delta` after `setTarget` | 51,511 | 57,232 | **landed** |
| 890/891 | `setPosition(m_tempVec)` before the `+=` offset | 51,512 | 57,232 | rejected — stale vector (w13570) |
| 872/878 | `field_0x1c = 156` after `PlayEngineSound` | 51,512 | 57,232 | rejected — `PlayEngineSound` reads `field_0x1c` |
| 969/974 | `startFade` ↔ `SetCurrentApplicationModule` | 51,496 | 57,232 | rejected — moves `startFade` into the second `if` |
| 1323/1325 | `setVisible` ↔ hitpoint `for` | 51,512 | 57,232 | rejected — parser put the store inside the loop |
| 1334/1335 | `updateSupernova` ↔ fade `if` | 51,508 | 57,208 | rejected — not size-exact |
| 1707/1710 | `m_nState = 3` ↔ `m_nState = 4` | 51,512 | 57,232 | rejected — swaps two branch states |
| 2077/2078 | `useTargetsUpVector` after the enemy `for` | 51,516 | 57,232 | rejected (+3) |
| 2411/2413 | rotate the two `nextInt(10000)` draws | 51,512 | 57,232 | rejected — swaps `dy`/`dz` (w13570) |
| 3260/3261 | `lookBehind` ↔ `if (field_0xab)` | 54,512 | 57,224 | rejected — not size-exact |
| 3317/3318 | `enableFirstPersonCam` after rocket `if` | 51,514 | 57,232 | rejected (+1) |
| 3768/3770 | rotate the `1.96`/`0.03` vector adds | 51,509 | 57,232 | rejected — `0.03` term uses the wrong basis vector |
| 4351/4354 | `nextCampaignMission` after stream-out `if` | 51,515 | 57,232 | rejected (+2) |
| 4904/4908 | `setVisible` after the `shipGroupFlag==44` block | 51,918 | 57,232 | rejected (+405) |
| 4985/4989 | `moveForward` ↔ `setActive` across two `if`s | 51,491 | 57,232 | rejected — moves statements between different conditions |
| 5236/5238 | `setDirection` ↔ timer `if` | 55,127 | 57,204 | rejected — not size-exact |
| 5335/5336 | `m_nState = 1` after the always-enemy `for` | 51,514 | 57,232 | rejected (+1) |

## Other sweeps that produced no size-exact win on this base

* distance-1 adjacent-line swaps over the whole body (1,942 cells): only
  `L890/891` (rejected).
* statement transpositions k=1..6 over 605-1600 (2,025 cells) and k=1..4 over
  1600-2600 (2,258 cells) and k=1..3 over 2600-5487 (3,425 cells).
* window-3..6 rotations over 605-1600 (no win), 1600-2600 (`L2411` only,
  rejected) and 2600-5487 (`L3048` landed, `L3768`/`L4604` rejected).
* whole-condition `__builtin_expect` flips at all 396 `if` sites, both hint
  values (792 cells): none below 51,513.
* all 24 declaration-order permutations of `messages/player/enemies/status`:
  only the current order is 51,513 / 57,232; the next best is 51,517.
* the shared active tail deactivation block rewritten to the original's exact
  order (merged `strh` for `m_bStartSequence`/`m_bStartSequenceOver`, then
  `setLookAtCam`, `setVulnerable`, `m_bCinematicBreak`, `setCollide`):
  structurally closer (140 vs 143 diff blocks) but raw **51,551 (+38)** —
  rejected.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13635
ulimit -v 12000000
cmake --preset default -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -j2
cmake --build cmake-build-match --target verify -j2
cmake --build cmake-build-match --target check -j2
```

The per-cell harness (`fast.py`, `sweep.py`, `tsweep.py`, `rsweep.py`,
`esweep.py`, `permdecl.py`, `normdiff.py`) lives in
`/home/debian/GalaxyOnFire2/scratch/w13635/`.
