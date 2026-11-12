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

### 6. lines 3573/3579 (d6, -2)

`m_pCamera->setPosition(m_dirVec);` moved after
`m_pCamera->setTarget(player->geometry);`.  `resetCamera` between them only
sets the camera target/offsets (not its position), and `m_dirVec` is not
touched between the two statements.

Measured: raw **51,803 → 51,801** (-2), aligned 21,033 → 21,032, size exact,
`.text` 73,499 → 73,497, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 7. lines 742/747 (d5, -1)

`m_nState = 4;` moved after `m_pLevel->getPlayer()->setCollide(true);`.
Independent state store vs player collision flag; the intervening
`setComputerControlled`, timer/field stores and radar enable touch neither.

Measured: raw **51,801 → 51,800** (-1), aligned 21,032 unchanged, size exact,
`.text` 73,497 → 73,496, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 8. lines 1697/1702 (d5, -1)

`cam1688->setLookAtCam(false);` moved after `resetCamera(self);` (same
independence argument as #3: `resetCamera` sets target/offsets only).

Measured: raw **51,800 → 51,799** (-1), aligned 21,032 → 21,054, size exact,
`.text` 73,496 → 73,495, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 9. lines 1717/1722 (d5, -1)

`player->hideShipForFirstPersonCameraView(false);` moved after
`player->setComputerControlled(true);`.  Independent player flags; the
intervening `m_bCinematicBreak`, `stopShooting`, radar/HUD stores are
independent.

Measured: raw **51,799 → 51,798** (-1), aligned 21,054 unchanged, size exact,
`.text` 73,495 → 73,494, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 10. lines 2270/2275 (d5, -1)

`m_pCamera->setTarget((*enemies)[0]->geometry);` moved after
`m_pLevel->lodManager->forceUpdate(delta, false);`.  Camera target vs LOD
update; the intervening `camPos` arithmetic and `setPosition(camPos)` do not
read the camera target.

Measured: raw **51,798 → 51,797** (-1), aligned 21,054 → 21,091, size exact,
`.text` 73,494 → 73,493, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 11. lines 2330/2336 (d6, -1)

`player->hideShipForFirstPersonCameraView(false);` moved after
`m_pCamera->setActive(true);`.  Independent player/camera state; the
intervening `setLookAtCam`, `stopShooting`, radar/computer/HUD stores are
independent.

Measured: raw **51,797 → 51,796** (-1), aligned 21,091 → 21,095, size exact,
`.text` 73,493 → 73,492, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 12. lines 3404/3414 (d5, -1)

`player->setVisible(false);` moved after
`player->hideShipForFirstPersonCameraView(false);`.  Independent player
visibility/first-person-hide flags; the intervening `setTurretMode`,
`setFreeze`, rocket-control block, radar and `setFreeLookMode` are independent.

Measured: raw **51,796 → 51,795** (-1), aligned 21,095 unchanged, size exact,
`.text` 73,492 → 73,491, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 13. lines 5009/5014 (d5, -1)

`fighter->geometry->setDirection(m_tempVec, Vector{0,1,0});` moved after
`player->setComputerControlled(true);`.  Different receivers (enemy fighter
geometry vs player control flag); the intervening `setAIDisabled`,
`setVisible`, `setActive` and the player's own `setDirection` are independent.

Measured: raw **51,795 → 51,794** (-1), aligned 21,095 → 21,108, size exact,
`.text` 73,491 → 73,490, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 14. lines 3520/3529 (d5, -1; second parser pass)

Second sweep with compound statements counted as single items (404 additional
pairs) found one more win on the post-#1 body:
`wcamera->enableFirstPersonCam(false);` moved after
`player->setFreeLookMode(false);`.  Independent camera/player flags; the
intervening `m_nState++`, `resetCamera`, `setTurretMode` and rocket-control
block are independent.

Measured: raw **51,794 → 51,793** (-1), aligned 21,108 unchanged, size exact,
`.text` 73,490 → 73,489, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

## Rejected on semantics

* **2757/2770**: the intervening `if (m_nScriptTimerA >= 4001)` reads the timer
  the moved `m_nScriptTimerA += delta` writes.
* **872/878**: `PlayEngineSound()` reads `field_0x1c`, which the moved store
  writes.
* **3026/3031**: `MatrixSetTranslation(local, m_tempVec)` reads `m_tempVec`
  updated between the two statements.
* **4712/4717**: `setPosition(m_tempVec)` reads `m_tempVec` assigned between.
* **3405/3415**: `stopShooting(0)` early-returns on `turretActive`; moving it
  before `setTurretMode(false)` skips the `player->stopShooting(0)` call.

### 15. lines 3533/3538 (d5, -2; post-landing re-sweep)

Full union re-sweep (3,869 pairs) on the post-#14 body found one more safe win:
`player->player->setVulnerable(false);` moved after
`wcamera->setLookAtCam(true);`.  Player vulnerability vs camera look-at flag;
the intervening `setFreeze`, `m_bCinematicBreak`, radar/HUD stores are
independent.

Measured: raw **51,793 → 51,791** (-2), aligned 21,108 → 21,114, size exact,
`.text` 73,489 → 73,487, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 16. lines 3532/3537 (d5, -1; post-landing re-sweep)

`player->stopShooting(0);` moved after `m_pHud->visible = 0;`.  Independent
player shooting vs HUD visibility; the intervening `setLookAtCam`,
`setFreeze`, `m_bCinematicBreak` and radar store are independent.

Measured: raw **51,791 → 51,790** (-1), aligned 21,114 → 21,110, size exact,
`.text` 73,487 → 73,486, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 17. lines 3531/3536 (d5, -2; second post-landing re-sweep)

`player->hideShipForFirstPersonCameraView(false);` moved after
`m_pRadar->enabled = 0;`.  Independent player/camera-hide flag vs radar
enable; the intervening HUD store, `setLookAtCam`, `setFreeze` and
`m_bCinematicBreak` are independent.

Measured: raw **51,790 → 51,788** (-2), aligned 21,110 → 21,108, size exact,
`.text` 73,486 → 73,484, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 18. lines 3520/3531 (d5, -1; third post-landing re-sweep)

`player->setFreeLookMode(false);` moved after `m_pRadar->enabled = 0;`.
Independent player free-look flag vs radar enable; `setTurretMode(false)`
between them touches neither.

Measured: raw **51,788 → 51,787** (-1), aligned 21,108 unchanged, size exact,
`.text` 73,484 → 73,483, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

### 19. lines 3535/3540 (d5, -1; fourth post-landing re-sweep)

`m_bCinematicBreak = 1;` moved after the camera `setTarget` of
`(*enemies)[0]->geometry`.  Independent flag store vs camera target; the
intervening hide-ship/stop-shooting/vulnerability/visibility calls are
independent.

Measured: raw **51,787 → 51,786** (-1), aligned 21,108 → 21,101, size exact,
`.text` 73,483 → 73,482, `.ARM.extab` 610 unchanged, `.rel.plt` unchanged,
RATCHET PASS.

## Final state

```
                         base (0f54626b3)   this pass
process raw diffbytes    51,820             51,786     (-34)
process aligned insns    20,981             21,101
orig / ours size         57,232 / 57,232    57,232 / 57,232   (size-exact)
.text diffbytes          73,516             73,482     (-34)
.ARM.extab diffbytes     610                610        (size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx               0                  0
.rel.plt                 md5 289b2a06…      md5 289b2a06…  (byte-identical, 29,392 B)
.rodata / .data / .bss   192 / 0 / 2        192 / 0 / 2
file size                2,187,328          2,187,328  (exact)
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN ✓            CLEAN ✓
drift                    0                  0
symbol parity            5370/5370          5370/5370
RATCHET                  PASS               PASS
```

19 landed d5/d6 swaps, all size-exact, `.rel.plt`-identical, `.ARM.extab`
unchanged.  Five further raw wins were rejected as read-after-write semantic
changes (see above); 1295/1334 was rejected for changing `.rel.plt`.
