# LevelScript::process — distance-5/6 adjacent-statement sweep (wave 190, w13368)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13368`, branch
`wave190-w13368`, base master `0f54626b3` (raw **51,820**, `.text` 73,516,
`.ARM.extab` 610, `.rel.plt` md5 `289b2a06…`).

Scratch `/home/debian/GalaxyOnFire2/scratch/w13368` (harness: parser + 4-way
compile/link/raw sweep, `gen.py` / `harness.py`).

## Sweep

Parsed the function body (lines 605–5372) into same-indent single-line
statements and swapped every pair 5 and 6 statements apart inside the same
straight-line block: **3,465 candidates**, 2,357 compiled, 1,108 declaration/
use reorders failed.  Raw metric measured on a full gold link per candidate.

Size-exact (57,232 B) raw wins on the base body (`.rel.plt` md5 unchanged
unless noted):

| lines | d | raw | note |
|---|---:|---:|---|
| 3520/3530 | 5 | **-7** | `Mission::setType(4)` / `enableFirstPersonCam(false)` |
| 2435/2440 | 5 | -3 | `setTurretMode(false)` / `m_pRadar->enabled = 0` |
| 4759/4765 | 6 | -3 | `setLookAtCam(false)` / `resetCamera(m_pLevel)` |
| 2146/2152 | 6 | -2 | `m_bCinematicBreak = 1` / camera `setTarget` |
| 2757/2770 | 5 | -2 | **rejected**: intervening `if` reads `m_nScriptTimerA` |
| 3533/3539 | 6 | -2 | `setVisible(false)` / `player->player->setVulnerable(false)` |
| 3573/3579 | 6 | -2 | camera `setPosition` / `setTarget` |
| 742/747 | 5 | -1 | `m_nState = 4` / `setCollide(true)` |
| 872/878 | 6 | -1 | **rejected**: `PlayEngineSound` reads `field_0x1c` |
| 1697/1702 | 5 | -1 | `setLookAtCam(false)` / `resetCamera(self)` |
| 1717/1722 | 5 | -1 | `hideShip…` / `setComputerControlled(true)` |
| 2270/2275 | 5 | -1 | camera `setTarget` / `lodManager->forceUpdate` |
| 2330/2336 | 6 | -1 | `hideShip…` / `setActive(true)` |
| 3026/3031 | 5 | -1 | **rejected**: `MatrixSetTranslation` reads `m_tempVec` updated between |
| 3404/3414 | 5 | -1 | `setVisible(false)` / `hideShip…` |
| 3405/3415 | 5 | -1 | `setTurretMode(false)` / `stopShooting(0)` |
| 4712/4717 | 5 | -1 | **rejected**: `setPosition(m_tempVec)` reads value updated between |
| 5009/5014 | 5 | -1 | `fighter->geometry->setDirection` / `setComputerControlled(true)` |

Also rejected: 1295/1334 (d6, raw -2) because it changes gold's first-reference
order and the `.rel.plt` md5 (`289b2a06…` → `9d6146d6…`).

## Landed

### 1. lines 3520/3530 (d5, -7)

`((Mission *) (intptr_t) Globals::status->getCampaignMission())->setType(4);`
moved after `wcamera->enableFirstPersonCam(false);`.  Independent receivers
(mission object vs camera); the intervening `m_nState++`, `resetCamera`,
`setTurretMode`, rocket-control block and `setFreeLookMode` touch neither.

Measured: raw **51,820 → 51,813** (-7), aligned 20,981 → 21,019, size
57,232/57,232 exact, `.text` 73,516 → 73,509, `.ARM.extab` 610 unchanged,
`.rel.plt` md5 unchanged, file size 2,187,328 exact, RATCHET PASS.

### 2. lines 2435/2440 (d5, -3)

`player->setTurretMode(false);` moved after `m_pRadar->enabled = 0;`.
Independent player flag vs radar enable; the intervening
`hideShipForFirstPersonCameraView`, `m_bCinematicBreak = 1`,
`enableFirstPersonCam(false)` and `setComputerControlled(true)` touch neither.

Measured: raw **51,813 → 51,810** (-3), aligned 21,019 → 21,028, size exact,
`.text` 73,509 → 73,506, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 3. lines 4759/4765 (d6, -3)

`m_pCamera->setLookAtCam(false);` moved after `resetCamera(m_pLevel);`.
`resetCamera` only sets the camera target/offsets, not the look-at flag; the
intervening HUD/radar stores, `lodManager->forceUpdate`, `player->setPosition`
and `player->setSpeed` are independent.

Measured: raw **51,810 → 51,807** (-3), aligned 21,028 → 21,031, size exact,
`.text` 73,506 → 73,503, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 4. lines 2146/2152 (d6, -2)

`m_bCinematicBreak = 1;` moved after the camera `setTarget(...)` of the first
landmark's root geometry.  Independent flag store vs camera target; the
intervening `setLookAtCam`, `setPosition`, `setComputerControlled`,
`setVisible` and `setVulnerable` calls touch neither.

Measured: raw **51,807 → 51,805** (-2), aligned 21,031 → 21,033, size exact,
`.text` 73,503 → 73,501, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 5. lines 3533/3539 (d6, -2)

`player->setVisible(false);` moved after `player->player->setVulnerable(false);`.
Different receivers (`PlayerEgo` visibility vs `Player` vulnerability); the
intervening `setFreeze`, `m_bCinematicBreak`, radar/HUD stores and
`setLookAtCam` are independent.

Measured: raw **51,805 → 51,803** (-2), aligned 21,033 unchanged, size exact,
`.text` 73,501 → 73,499, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

## Current-master harvest (wave 190)

The branch was based on `0f54626b3` (raw 51,820). Re-applied the five d5/d6
reorders on current master (`5ff423480`) after the w13369/w13361/w13384 harvest.
Current master before this group was raw 51,784; after it is raw **51,769**
(-15), aligned 22,259, size 57,232/57,232. `.text` 73,269 -> 73,254 (-15),
`.ARM.extab` 606 unchanged, total 73,895 -> 73,880 (-15). `check` green,
RATCHET PASS.

## Current-master harvest (wave 190, follow-up 3)

Re-applied the three follow-up d5/d6 reorders on current master (`b65d28c2b`):
camera `setPosition`/`setTarget`, `m_nState`/`setCollide`, and
`setLookAtCam`/`resetCamera`. LevelScript raw **51,769 -> 51,765** (-4), aligned
22,280, size 57,232/57,232. `.text` 73,254 -> 73,250 (-4), `.ARM.extab` 606
unchanged, build-id diff 20 -> 19, total 73,880 -> 73,875 (-5). `check` green,
RATCHET PASS.

## Current-master harvest (wave 190, follow-up 4)

Re-applied the last two d5 reorders on current master (`bea73c49b`):
`hideShipForFirstPersonCameraView`/`setComputerControlled` and camera
`setTarget`/`lodManager->forceUpdate`. LevelScript raw **51,765 -> 51,763** (-2),
aligned 22,287, size 57,232/57,232. `.text` 73,250 -> 73,248 (-2),
`.ARM.extab` 606 unchanged, build-id diff 19 -> 20, total 73,875 -> 73,874 (-1).
`check` green, RATCHET PASS.
