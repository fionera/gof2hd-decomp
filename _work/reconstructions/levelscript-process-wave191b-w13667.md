# LevelScript::process — wave 191b / w13667: case-158 state-1/2 faithful order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13667`, branch
`wave191b-w13667`, base master `2064dfd40` (raw **51,524**, aligned **21,918**,
`.text` diffbytes 71,227, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff notes (w13657 case-154 state-4, w13645 case-158 state-1
rotate/setTarget + state-2 setTarget/resetCamera, w13642 case-14 state-1,
w13631 case-92 state-4, w13634 case-70/105/154) leave two case-158 order
divergences open: the state-1 `approach` term is computed from the
*pre-increment* timer, and the state-2 teardown is not in the original order.
w13657 measured the full state-2 permutation at raw +5 / aligned -37 on its
base but landed the larger case-154 state-4 win instead.  This pass re-derives
both blocks from the Thumb disassembly and lands the combined faithful order.

## Original disassembly

### case 158 state 1 (`0x138878`..`0x13898c`)

The original updates `m_nScriptTimerA` before forming
`approach = timerA / -34000 + 1`; our source read the pre-increment value.
The rest of the state-1 block (translate, marker direction/moveForward,
enemy visibility, `setPosition`, `setTarget`, `rotate`) already matches.

### case 158 state 2 (`0x140414`..`0x1404a2`)

```
14041c  field_0x24 = 0
140420  m_pLevel->getPlayer()
140424  resetGunDelay()
14042c  setLookAtCam(false)
140436  setTarget(player->geometry)
14043e  setComputerControlled(false)
140446  setFreeze(false)
140452  setVisible(true)
14045c  setSpeed(2.0f)
140466  m_pHud->visible = 1
14046e  m_pRadar->enabled = 1
140474  setVulnerable(true)
14047c  resetCamera(m_pLevel)
140488  lodManager->forceUpdate(delta, false)
140492  m_bCinematicBreak = 0
140498  m_nScriptTimerA = 0
14049c  m_nScriptTimerB = 0
1404a2  setCloakingPossible(true)
```

Our source had `field_0x24` after `resetGunDelay`, `resetCamera` before
`setTarget` (the w13645 partial swap), `setSpeed` after `forceUpdate`,
`m_bCinematicBreak` before `setTarget`, and the two timers before
`forceUpdate`.

## Landed change

```cpp
// case 158 state 1
-                    float approach = (float) m_nScriptTimerA / -34000.0f + 1.0f;
                     m_nScriptTimerA += delta;
+                    float approach = (float) m_nScriptTimerA / -34000.0f + 1.0f;

// case 158 state 2
-                        m_pLevel->getPlayer()->resetGunDelay();
                         field_0x24 = 0;
+                        m_pLevel->getPlayer()->resetGunDelay();
                         m_pCamera->setLookAtCam(false);
-                        resetCamera(m_pLevel);
+                        m_pCamera->setTarget(player->geometry);
                         player->setComputerControlled(false);
                         player->setFreeze(false);
                         player->setVisible(true);
+                        player->setSpeed(2.0f);
                         m_pHud->visible = 1;
                         m_pRadar->enabled = 1;
-                        m_bCinematicBreak = 0;
-                        m_pCamera->setTarget(player->geometry);
                         player->player->setVulnerable(true);
+                        resetCamera(m_pLevel);
+                        m_pLevel->lodManager->forceUpdate(delta, false);
+                        m_bCinematicBreak = 0;
                         m_nScriptTimerA = 0;
                         m_nScriptTimerB = 0;
-                        m_pLevel->lodManager->forceUpdate(delta, false);
-                        player->setSpeed(2.0f);
```

All moved statements touch disjoint state (timer, camera target, player
control/visibility/speed flags, HUD/radar bytes, LOD manager, cinematic flag),
so the reorder is semantics-preserving and matches the original statement
order.  `resetCamera` already sets the camera target to `player->geometry`, so
the later `setTarget` is a no-op on the target and does not touch the offsets.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,524 -> 51,534  (+10)
aligned    21,918 -> 21,862  (-56)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,227 -> 71,237 (+10); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline | 51,524 | 21,918 | 57,232 | 577 |
| case-158 s2 full faithful alone | 51,531 | 21,870 | 57,232 | 577 |
| **case-158 s1 approach + s2 full faithful (landed)** | **51,534** | **21,862** | **57,232** | **577** |
| case-158 s1 approach alone (w13657: +3 / -8) | — | — | 57,232 | 577 |
| case-125 s3 full faithful order | 53,524 | 21,938 | 57,232 | 577 |
| case-125 s3 `setAutoPilot` before `isDocked` | 53,405 | 21,944 | 57,240 | 577 |
| case-125 s3 `resetGunDelay` after `isDocked` | 53,386 | 21,946 | 57,240 | 577 |
| case-125 s3 both (A+B) | 51,525 | 21,918 | 57,232 | 577 |
| case-125 s3 `setLookAtCam` before `setTarget` | 51,525 | 21,918 | 57,232 | 577 |
| case-125 s3 `resetCamera` after HUD/radar | 51,525 | 21,918 | 57,232 | 577 |
| case-125 s3 `setComputerControlled` after `setPosition` | 53,522 | 21,938 | 57,232 | 577 |
| case-91 s5 faithful vector order (direction*10000, right*700) | 52,789 | 21,675 | 57,224 | 577 |
| case-91 s5 full faithful order | 53,154 | 21,685 | 57,216 | 577 |
| case-91 s6 faithful (`setSpeed` inside `<100` branch) | 52,822 | 21,912 | 57,224 | 577 |
| case-91 s5 vector + duplicate `m_nState = 6` store | 51,543 | 21,917 | 57,232 | 577 |

The case-91 state-5 vector reorder is the strongest aligned improvement found
(-243) but shrinks the function by 8 B; the only size-exact compensation found
was a second `m_nState = 6` store, which is not faithful, so it is rejected.
case-125 state-3 faithful moves are size-exact but neutral or worse on the
aligned metric.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13667/` (`harness.py`, `variants*.json`,
`cmpblock.py`, `diffmap.py`, `linemap.py`, `callseq.py`).
