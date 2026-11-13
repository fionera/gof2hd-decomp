# LevelScript::process — wave 191b / w13570: first-half transposition sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13570`, branch
`wave191b-w13570`, base master `cb9c59ca0` (raw **51,536**, aligned 22,818,
`.text` 71,232, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Diff-cluster map (first half, lines 605-2600)

Same-address byte diffs mapped through the unstripped link's debug line table
(`scratch/w13570/diffmap2.py`) give these campaign-case buckets:

| block | lines | raw diff bytes |
|---|---|---:|
| pre-campaign (mission 0/1, shared tail, pre-switch specials) | 605-1577 | 7,643 |
| case 14 | 1578-1667 | 1,006 |
| case 16 | 1668-1775 | 1,061 |
| case 21 | 1776-1799 | 31 |
| case 24 | 1800-1856 | 456 |
| case 29 | 1857-1928 | 604 |
| case 40 | 1929-1999 | 773 |
| case 41 | 2000-2108 | 1,001 |
| case 42 | 2109-2224 | 1,000 |
| case 56 | 2225-2249 | 102 |
| case 64 | 2250-2313 | 519 |
| case 67 | 2314-2424 | 977 |
| case 69 | 2425-2476 | 372 |
| case 70 | 2477-2565 | 593 |
| case 73 | 2566-2622 | 184 |

The aligned-cluster map shows the largest single first-half clusters are
block-placement artefacts already documented by the wave-169/170 notes
(mission-5 body at `0x13824e` vs the original `0x13812a`; the mission-81 body
before it).  The wave-170 w11606 note established that this is
MachineBlockPlacement interleaving, not source order, and that the separate-`if`
fix shrinks the function.  Re-measured on this base: separate `if` gives
57,412 B (+180) and a different `.rel.plt`; the big move of the mission-81/78
bodies into the mission-5 chain gives 57,560 B (+328).  Not landed.

## Sweeps run (first half only, lines 605-2600)

Fresh distance-1..6 transposition sweeps and distance-3..6 cyclic-rotation
sweeps on the current base (exact NDK r18b `-Oz` compile + full gold link per
candidate; `scratch/w13570/sweep.py`, `rot.py`).  Every size-exact,
`.rel.plt`-identical raw win is listed:

| kind | lines | raw | safe? |
|---|---|---:|---|
| d1 | 890/891 | 51,535 | no — `setPosition(m_tempVec)` before the `+=` offset (original `0x13d41c`-`0x13d434` adds first) |
| d3 | 1039/1042 | 51,535 | **landed** |
| d4 | 2009/2013 | 51,535 | **landed** |
| d4 | 2389/2393 | 51,535 | **landed** |
| d5 | 2010/2015 | 51,535 | **landed** |
| d5 | 2263/2268 | 51,535 | **landed** |
| d6 | 872/878 | 51,535 | no — `field_0x1c = 156` must precede `PlayEngineSound()` (it reads `field_0x1c`) |
| rot3 | 888-891 right | 51,535 | no — `setPosition` uses the stale `m_tempVec` |

No other transposition/rotation cell is below 51,536.

## Landed changes (all statement transpositions, semantics-preserving)

1. case-81 state-3 exit (~1039/1042): `Globals::layout->enableFillScreen(true);`
   moves after `Globals::status->departStation(...)`.  The intervening
   `nextCampaignMission(true)` and `switch_to_target_setting = 0` do not read
   the fill-screen flag; `departStation` only writes the status station.
2. case-41 teardown (~2009/2013): swap the two independent flag stores
   `m_pHud->visible = 0;` and `m_pRadar->enabled = 0;`.
3. case-67 route setup (~2389/2393): `m_pCamera->enableFirstPersonCam(false);`
   moves after `resetCamera(m_pLevel);`.  `enableFirstPersonCam` writes only
   `firstPerson`/`fpOffset`/`shakeAccum`/`shakeReference`; `resetCamera` sets
   the camera target/offsets and neither reads those fields.
4. case-41 teardown (~2010/2015): `player->stopShooting(0);` moves after
   `player->player->setVulnerable(false);`.  `stopShooting` early-returns only
   on turret-active/dead; `setVulnerable` is a plain `Player` store.
5. case-64 (~2263/2268): `m_pCamera->enableFirstPersonCam(false);` moves after
   `m_pCamera->setActive(true);` (both plain camera setters).

Measured together on the real build:

```
raw        51,536 -> 51,531  (-5)
size       57,232 / 57,232   (exact)
.text      71,232 -> 71,227  (-5)
.ARM.extab 577 (unchanged; size/base exact)
.ARM.exidx 0, .rodata 485, .data 0 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
check      RATCHET PASS (byte_exact 4507/4524, linked 4507, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Second landing: distance-2 cyclic rotations

Fresh distance-2 cyclic-rotation sweep on the post-first-landing body (raw
51,531).  Two semantics-preserving cells compose:

1. mission-0 state-9 intro planet (~828-830, left rotation):
   `player->rotate(0.7853982f, 0.7853982f, 0.7853982f);` moves after
   `m_pLevel->getStarSystem()->switchPlanetForIntro();` (before
   `switchSkyboxForIntro`).  The planet/skybox switches touch the star-system
   planet texture and skybox mesh; the player transform is independent.
2. case-41 teardown (~2008-2010, left rotation):
   `player->setTurretMode(false);` moves after the independent
   `m_pRadar->enabled = 0;` and `player->player->setVulnerable(false);`
   stores.  It still precedes `player->stopShooting(0);` (which early-returns
   when the turret is active).

Measured together on the real build:

```
raw        51,531 -> 51,529  (-2)
size       57,232 / 57,232   (exact)
.text      71,227 -> 71,225  (-2)
.ARM.extab 577 (unchanged; size/base exact)
.ARM.exidx 0, .rodata 485, .data 0 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
check      RATCHET PASS
```

Rejected in the same sweep (raw-lowering but semantically wrong):
`889-891 right` (`setPosition` before the `+=` offset), `1728-1730 right`
(`setPosition(camPos)` before the offset), `2411-2413 right` (swaps the
`nextInt(10000)` draws for `dy`/`dz`).
