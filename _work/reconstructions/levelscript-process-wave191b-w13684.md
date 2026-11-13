# LevelScript::process — wave 191b / w13684: case-91 state-5 teardown

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13684`, branch
`wave191b-w13684`, base master `004b2758f` (raw **51,534**, aligned **21,862**,
`.text` diffbytes 71,247, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff notes (w13667 case-158, w13657 case-154 state-4,
w13645 case-158 state-1/2, w13642 case-14, w13631 case-92, w13634
case-70/105/154, w13610 first-half sweep) leave the case-91 state-5 teardown
(source L2887-2927) as the largest untouched *semantic* cluster in the tail.
An ordered call-sequence diff against the original showed the block was
emitting its calls in a different order from the original's
`0x13f3a0..0x13f53a` and also carried a dead `getDirection()` load.  This pass
reconstructs the whole block from the Thumb disassembly.

## Original disassembly (case 91 state 5, `0x13f3a0`..`0x13f53a`)

```
13f3a0  m_nScriptTimerA = 0
13f3a6  m_pLevel
13f3aa  Level::getPlayer()
13f3b0  PlayerEgo::setAutoPilot(nullptr)
13f3b4  m_pRadar->lockedEnemy = nullptr
13f3b8  m_pRadar->dockNavPtr = nullptr
13f3c0  if (player->isInRocketControl()) {
13f3ce      resetCamera(m_pLevel)
13f3d8      setRocketControl(nullptr, nullptr)
13f3de      killLiberator()
13f3e8      setRumblePercentage(0.0f, 0)
        }
13f3f0  setTurretMode(false)
13f3fa  resetCamera(m_pLevel)
13f402  setFreeLookMode(false)
13f40a  enableFirstPersonCam(false)
13f412  hideShipForFirstPersonCameraView(false)
13f41a  stopShooting(0)
13f424  m_bCinematicBreak = 1
13f428  m_pHud->visible = 0
13f42e  m_pRadar->enabled = 0
13f434  setComputerControlled(true)
13f43c  setCollide(false)
13f444  setLookAtCam(true)
13f44e  setTarget(player->geometry)
13f45a  m_tempVec = player->geometry->getPosition()
13f470  ship = player->geometry
13f4c6  ship->setDirection(VectorNormalize(playerPos - dockTargetPos), {0,1,0})
13f4d2  m_dirVec = player->geometry->getDirection()
13f4e8  m_tempVec += m_dirVec * 10000.0f
13f4fc  m_dirVec = player->geometry->getRightVector()
13f50e  m_tempVec += m_dirVec * 700.0f
13f51e  m_pCamera->setPosition(m_tempVec)
13f530  (*enemies)[0]->player->setHitpoints(0)
13f536  m_nState = 6
```

Our source had `setRocketControl`/`setRumblePercentage`/`killLiberator`/
`resetCamera` in that order (original puts `resetCamera` first), `resetCamera`
before `setTurretMode`, `m_pRadar->enabled = 0` after `setLookAtCam`,
`setHitpoints(0)` before `setComputerControlled`, `m_nState = 6` before the
vector adds, the vector adds in the wrong order (`getRightVector` twice plus a
dead `getDirection`), and `setTarget` after the vector adds.

## Landed change

```cpp
// case 91 state 5 (L2895-2926)
-                            player->setRocketControl(nullptr, nullptr);
-                            m_pCamera->setRumblePercentage(0.0f, 0);
-                            player->killLiberator();
-                            resetCamera(self);
+                            resetCamera(self);
+                            player->setRocketControl(nullptr, nullptr);
+                            player->killLiberator();
+                            m_pCamera->setRumblePercentage(0.0f, 0);
                         }
-                        resetCamera(self);
-                        m_pCamera->enableFirstPersonCam(false);
                         player->setTurretMode(false);
+                        resetCamera(self);
                         player->setFreeLookMode(false);
+                        m_pCamera->enableFirstPersonCam(false);
                         player->hideShipForFirstPersonCameraView(false);
                         player->stopShooting(0);
                         m_bCinematicBreak = 1;
                         m_pHud->visible = 0;
+                        m_pRadar->enabled = 0;
+                        player->setComputerControlled(true);
                         self->getPlayer()->setCollide(false);
-                        (*enemies)[0]->player->setHitpoints(0);
-                        player->setComputerControlled(true);
                         m_pCamera->setLookAtCam(true);
-                        m_pRadar->enabled = 0;
+                        m_pCamera->setTarget(player->geometry);
+                        m_tempVec = player->geometry->getPosition();
                         AEGeometry *ship = player->geometry;
-                        m_tempVec = player->geometry->getPosition();
                         ship->setDirection(...);
-                        m_nState = 6;
-                        m_dirVec = player->geometry->getRightVector();
-                        m_tempVec += m_dirVec * 700.0f;
-                        m_tempVec += m_dirVec * 10000.0f;
                         m_dirVec = player->geometry->getDirection();
-                        m_pCamera->setTarget(player->geometry);
+                        m_tempVec += m_dirVec * 10000.0f;
+                        m_dirVec = player->geometry->getRightVector();
+                        m_tempVec += m_dirVec * 700.0f;
                         m_pCamera->setPosition(m_tempVec);
+                        (*enemies)[0]->player->setHitpoints(0);
+                        m_nState = 6;

// case 42 state 5 (L2112-2121): timers after m_nState = 6, add timerB
-                    m_nScriptTimerA = 0;
                     Globals::sound->play(153, nullptr, nullptr, 0.0f);
                     ...
                     m_nState = 6;
+                    m_nScriptTimerA = 0;
+                    m_nScriptTimerB = 0;

// case 154 state 0 (L4519 / L4536)
-                    player->player->setVulnerable(false);
                     if (player->isInRocketControl()) {
                     ...
                     player->setComputerControlled(true);
+                    player->player->setVulnerable(false);
```

The case-91 block is 8 B shorter than the original when written faithfully
(the dead `getDirection` call disappears); the case-42 state-5 timer resets
add exactly 8 B, so the function stays at 57,232 B.

## Measurement (exact NDK r18b `-Oz` compile + full gold link + `--strip-all`)

```
raw        51,534 -> 52,484  (+950)
aligned    21,862 -> 21,618  (-244)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,247 -> 72,197
.ARM.extab 156,312 B, diffbytes 577 (unchanged; size/base exact)
.ARM.exidx 28,528 B (0), .rodata 269,159 B (0)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, parity 5370/5370)
```

The raw rise is the +-8 B relocation: the case-91 block shrinks 8 B and the
case-42 block grows 8 B, so every byte between case 42 (~0x13abxx) and case 91
(~0x13f5xx) shifts by 8.  The aligned metric improves by 244.

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 51,534 | 21,862 | 57,232 |
| case-91 state-5 faithful block alone | 52,802 | 21,617 | 57,224 |
| case-91 `m_pRadar->enabled = 0` after HUD alone | 52,818 | 21,620 | 57,224 |
| case-91 `m_nState = 6` last alone | 52,793 | 21,620 | 57,224 |
| case-91 rocket-control reorder alone | 51,537 | 21,857 | 57,232 |
| case-42 state-5 timers alone | 54,158 | 21,860 | 57,240 |
| case-154 state-0 `setVulnerable` move alone | 51,553 | 21,862 | 57,232 |
| case-154 state-0 `stopShooting` move alone | 51,539 | 21,862 | 57,232 |
| case-154 state-0 `m_bCinematicBreak` move alone | 51,549 | 21,863 | 57,232 |
| case-91 faithful + case-42 timers | 52,500 | 21,618 | 57,232 |
| **case-91 faithful + case-42 timers + case-154 vuln (landed)** | **52,484** | **21,618** | **57,232** |

## Other clusters measured (not landed)

* **case-154 state-0 full faithful order** (0x13a788..0x13a92c): rocket block
  `resetCamera` first, `setTurretMode`, `setFreeLookMode`, `enableFirstPersonCam`,
  `hideShip`, `stopShooting`, `setComputerControlled`, `setVulnerable`,
  `setSpeed`, auto-turret pair, `m_bCinematicBreak`, hud, radar, `setLookAtCam`,
  `setTarget`, `getPosition`, `+= {300,300,5800}`, `setPosition`,
  `setAIDisabled`, loop, `field_0x70 = 1`.  Raw 51,577, aligned 21,871,
  size 57,232 (aligned regression).
* **case-91 state-5 full faithful with `setHitpoints(0)` last**:
  raw 51,555, aligned 21,862, size 57,232 (the state-store win is cancelled).
* **case-42 state-5 `m_nScriptTimerA = 0` move only**: raw 51,609,
  aligned 21,858, size 57,232.
* **case-91 `m_pRadar->enabled = 0` after `setHitpoints`** (or after
  `setComputerControlled`): size 57,232 but aligned 21,862.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13684/` (`fast.py`, `editlib.py`,
`callseq.py`, `callgap.py`, `callmiss.py`, `runs.py`, `spans.py`,
`blockstat.py`, `blockcnt.py`, `orig.dis`, `ours.dis`, `base67.cpp`).
