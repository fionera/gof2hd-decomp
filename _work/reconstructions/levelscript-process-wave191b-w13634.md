# LevelScript::process — wave 191b / w13634: case-70 state-0 teardown

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13634`, branch
`wave191b-w13634`, base master `57ce1a4c8` (raw **51,513**, aligned 22,816,
`.text` 71,197, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Cluster

Case 70 (`// 0x1399a2`) state-0 cutscene-exit teardown, source lines
~2480-2510.  The original disassembly (`0x1399a2..0x139aba`) is:

```
0x1399c6  player->setTurretMode(false)
0x1399ce  resetCamera(m_pLevel)
0x1399d6  player->setFreeLookMode(false)
0x1399de  m_pCamera->enableFirstPersonCam(false)
0x1399e6  player->hideShipForFirstPersonCameraView(false)
0x1399ee  player->stopShooting(0)
0x1399fa  m_bCinematicBreak = 1
0x139a00  hud->visible = 0
0x139a06  radar->enabled = 0
0x139a0c  player->setComputerControlled(true)
0x139a14  m_pCamera->setLookAtCam(true)
0x139a1c  m_pCamera->setActive(true)
0x139a34  m_pCamera->setTarget((*enemies)[0]->geometry)
0x139a4a  camPos = enemies[0]->getPosition()
0x139a5a  dir = enemies[0]->geometry->getDirection()
0x139a66  camPos += dir * 3000
0x139a72  camPos.x/y/z += {-600,-300,-1000}
0x139aa4  m_pCamera->setPosition(camPos)
0x139ab2  m_pLevel->lodManager->forceUpdate(delta, false)
0x139ab6  player->freeze = 1
0x139aba  m_nState = 1
```

Our source had `forceUpdate` before `setTarget` and `setFreeLookMode` at the
end of the block.  A full faithful reorder measures **53,983 / 22,787 /
57,224** (size not exact), so it is not landable.  The landed change is a
semantics-preserving codegen-coincidence move of the independent LOD update
to just after `setTarget` (the original puts it after `setPosition`; the
intervening `camPos` computation reads neither the LOD manager nor the
camera target).

## Landed change

```cpp
                     radar->enabled = 0;
-                    m_pLevel->lodManager->forceUpdate(delta, false);
                     m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
+                    m_pLevel->lodManager->forceUpdate(delta, false);
```

Measured alone on the real build:

```
raw        51,513 -> 51,515  (+2)
aligned    22,816 -> 22,173  (-643)
size       57,232 / 57,232   (exact)
.text      71,197 -> 71,199  (+2)
.ARM.extab 577 (size/base exact, unchanged)
.ARM.exidx 0, .rodata 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

This is an aligned-weighted win of the kind already accepted for `Layout`
C1/C2 (w13607: raw +3, aligned −32): the raw count moves by two bytes while
the normalized instruction diff drops by 643.

## Candidate table (all exact NDK r18b `-Oz` compile + full gold link)

| cluster / candidate | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline | 51,513 | 22,816 | 57,232 | 577 |
| **case-70 forceUpdate after setTarget (landed)** | **51,515** | **22,173** | **57,232** | **577** |
| case-70 forceUpdate after camPos add | 51,522 | 22,808 | 57,232 | 577 |
| case-70 forceUpdate after setPosition (faithful) | 51,534 | 22,808 | 57,232 | 577 |
| case-70 forceUpdate before resetCamera | 51,547 | 22,817 | 57,232 | 577 |
| case-70 full faithful reorder | 53,983 | 22,787 | 57,224 | 577 |
| case-105 right-vector add reorder | 51,515 | 22,816 | 57,232 | 577 |
| case-154 state-4 right-vector add (minimal) | 51,519 | 22,802 | 57,232 | 577 |
| case-154 state-4 faithful order | 51,532 | 22,553 | 57,232 | 577 |
| case-92 getEnemies loop-condition reload | 51,515 | 22,818 | 57,232 | 577 |
| case-92 getDirection/add reorder | 52,865 | 22,809 | 57,224 | 577 |
| case-70+105+154 combined | 51,523 | 22,804 | 57,232 | 577 |

## Semantic fixes found but not landed (size not exact or raw loss)

* **case 92 state 4** (`~3055`): `m_dirVec = leader->getDirection();` is dead
  in our source; the original (`0x13fd68..0x13fd8e`) uses it for
  `m_tempVec += m_dirVec * ((float)delta * 1.7f)` before reloading the right
  vector.  Moving the add between the two assignments is the faithful form:
  raw 52,865, aligned 22,809, **size 57,224 (−8)**.
* **case 105 state 3** (`~3462`): `m_dirVec = leaderShip->getRightVector();`
  is dead in our source; the original (`0x140754..0x140780`) adds
  `m_tempVec += m_dirVec * (step * 0.04f)` before reloading the up vector.
  The pure reorder is size-exact but raw +2.
* **case 154 state 4** (`~4648`): `m_dirVec = getRightVector();` is dead; the
  original (`0x1431e6..0x143208`) adds the right-vector term
  `m_tempVec += m_dirVec * (step * -0.05f)`.  The minimal add is size-exact
  but raw +6 / aligned −14; the full faithful order is raw +19 / aligned
  −263.
* **case 93 loop** (`~1618`): the original reloads
  `m_pLevel->getEnemies()` for the loop condition (`0x13aad8`); our source
  uses the cached `enemies` pointer.  Adding the reload is size-exact but
  raw +2.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13634/` (`diffmap.py`, `callseq.py`,
`pltseq.py`, `sweep4.py`, `case70*.py`, `combos.py`).
