# LevelScript::process — wave 190 / w13414: d3 statement reorders

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13414`, branch
`wave190-w13414`, base master `96f53fadf`.  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13414`.

Lens for this worker: adjacent-statement reorder sweeps on the current body
(raw 51,738), landing only semantics-preserving, size-exact,
`.rel.plt`-identical cells.  Sweeps run: distance 3, then 4, 2, 1, 5-6, 4,
7-10 and a final 3 on the progressively improved body (the other workers cover
the literal-pool / prologue / callee-saved angles).  Line numbers below are
**0-based** (source line = index + 1); the sweep harness is
`scratch/w13414/h/`.  All twelve changes are pure statement reorders of
disjoint receivers / independent local initialisers; each was measured alone
and the composition is additive.

## Landed changes

| # | move | source lines | statement order after | raw |
|---:|---|---|---|---:|
| 1 | down 5008,5011 | 5009-5012 | `setAIDisabled` / `setVisible` / `setActive` / `setComputerControlled` | -3 |
| 2 | up 741,744 | 742-745 | `field_5e = 0` / `setCollide(true)` / `setComputerControlled(false)` / `m_nScriptTimerA = 0` | -1 |
| 3 | up 3345,3348 | 3346-3349 | `resetGunDelay` / `setLookAtCam(false)` / `setVisible(true)` / camera `setTarget` | -1 |
| 4 | down 3789,3792 | 3790-3793 | `m_nState++` / `m_nScriptTimerA = 0` / `forceUpdate` / `m_bCinematicBreak = 0` | -1 |
| 5 | down 4679,4682 | 4680-4683 | enemy `field_0x13e=0` / enemy `setPosition` / `setAIDisabled(false)` / camera `setPosition(tv)` | -1 |
| 6 | down 2264,2268 | 2265-2269 | `m_pRadar->enabled=0` / `setComputerControlled(true)` / `camPos` init / `setActive(true)` / `m_bCinematicBreak=1` | -1 |
| 7 | down 3520,3522 | 3521-3523 | `resetCamera` / `setTurretMode(false)` / `m_nState++` | -1 |
| 8 | down 3534,3536 | 3535-3537 | `m_pHud->visible=0` / `setFreeLookMode(false)` / `m_bCinematicBreak=1` | -1 |
| 9 | down 1692,1693 | 1693-1694 | `lodManager->forceUpdate` / camera `translate` | -1 |
| 10 | down 3347,3348 | 3348-3349 | `camera->setTarget` / `player->setVisible(true)` | -1 |
| 11 | down 3525,3526 | 3526-3527 | `player->killLiberator()` / `wcamera->setRumblePercentage(0,0)` | -1 |
| 12 | down 2496,2503 | 2497-2504 | radar / `forceUpdate` / `setTarget` / `dir` / `camPos` init+offsets / `setFreeLookMode(false)` / `setPosition(camPos)` | -3 |

### 1. `player->setComputerControlled(true)` after `fighter->setActive(true)`

Mission-0 station hand-off block.  The four statements have disjoint receivers
(`player` vs `fighter`) and nothing between them reads what the other writes;
`setDirection(m_tempVec, ...)` stays last.

```
-                        player->setComputerControlled(true);
                         ((PlayerFighter *) fighter)->setAIDisabled(true);
                         fighter->setVisible(true);
                         fighter->setActive(true);
+                        player->setComputerControlled(true);
```

Measured alone: raw **51,738 -> 51,735** (-3), size 57,232/57,232 exact,
`.text` 72,944 -> 72,941, `.ARM.extab` 603 unchanged, `.rel.plt` md5
`289b2a06…` unchanged.

### 2. `player->player->field_5e = 0` before `setCollide(true)`

Mission-0 radio-handoff block.  Independent field store vs collision-flag
setter on the same object; neither reads the other's field.

```
+                player->player->field_5e = 0;
                 m_pLevel->getPlayer()->setCollide(true);
                 player->setComputerControlled(false);
                 m_nScriptTimerA = 0;
-                player->player->field_5e = 0;
```

Measured alone on the post-#1 body: raw **51,735 -> 51,734** (-1), size
57,232/57,232 exact, `.text` 72,941 -> 72,940, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 3. `m_pLevel->getPlayer()->resetGunDelay()` before `camera->setLookAtCam(false)`

Mission-3 arrival block.  Player gun-delay reset vs camera look-at flag; the
three statements between them touch `player` visibility and the camera target,
neither of which `resetGunDelay` reads.

```
+                        m_pLevel->getPlayer()->resetGunDelay();
                         camera->setLookAtCam(false);
                         player->setVisible(true);
                         camera->setTarget(player->geometry);
-                        m_pLevel->getPlayer()->resetGunDelay();
```

Measured alone on the post-#2 body: raw **51,734 -> 51,733** (-1), size
57,232/57,232 exact, `.text` 72,940 -> 72,939, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 4. `m_bCinematicBreak = 0` after `lodManager->forceUpdate`

State-7 -> 8 transition tail.  A plain flag store moved past the state bump,
timer clear and LOD update; none of them reads `m_bCinematicBreak`.

```
-                        m_bCinematicBreak = 0;
                         m_nState = m_nState + 1;
                         m_nScriptTimerA = 0;
                         m_pLevel->lodManager->forceUpdate(delta, false);
+                        m_bCinematicBreak = 0;
```

Measured alone on the post-#3 body: raw **51,733 -> 51,732** (-1), size
57,232/57,232 exact, `.text` 72,939 -> 72,938, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 5. `camera->setPosition(tv)` after the enemy `setAIDisabled(false)`

Mission-7 enemy hand-off.  `tv` is a local copy; the three intervening
statements only mutate `(*enemies)[0]`, so the camera store is independent.

```
-                        camera->setPosition(tv);
                         ((PlayerFighter *) (*enemies)[0])->field_0x13e = 0;
                         (*enemies)[0]->setPosition(Vector{-10000.0f, 0.0f, 22000.0f});
                         ((PlayerFighter *) (*enemies)[0])->setAIDisabled(false);
+                        camera->setPosition(tv);
```

Measured alone on the post-#4 body: raw **51,732 -> 51,731** (-1), size
57,232/57,232 exact, `.text` 72,938 -> 72,937, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

## Running total

```
                         base (HEAD)      this pass
process raw diffbytes    51,738           51,722     (-16)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          72,944           72,928     (-16)
.ARM.extab diffbytes     603              603        (size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx               0                0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
file size                2,187,328        2,187,328  (exact)
```

### 6. `m_bCinematicBreak = 1` after `m_pCamera->setActive(true)`

Mission-9 first-person hand-off.  A plain flag store moved past the radar
store, the computer-control flag, a local `camPos` initialiser and the camera
activation; none reads `m_bCinematicBreak`.

```
-                    m_bCinematicBreak = 1;
                     m_pRadar->enabled = 0;
                     player->setComputerControlled(true);
                     Vector camPos = (*enemies)[0]->getPosition();
                     m_pCamera->setActive(true);
+                    m_bCinematicBreak = 1;
```

Found by a fresh distance-4 adjacent-statement sweep (1,202 candidates) on the
post-#5 body; it was the only size-exact raw winner in that sweep (the other
distance-4 cells either break size or are semantically dependent).

Measured alone on the post-#5 body: raw **51,731 -> 51,730** (-1), size
57,232/57,232 exact, `.text` 72,937 -> 72,936, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 7. `m_nState++` after `player->setTurretMode(false)`

Mission-8/9 hand-off.  The state bump moves past `resetCamera(m_pLevel)` and
the turret-mode clear; `resetCamera` does not read `m_nState`.

```
-                        m_nState++;
                         resetCamera(m_pLevel);
                         player->setTurretMode(false);
+                        m_nState++;
```

Found by a fresh distance-2 sweep on the post-#6 body (2,049 candidates; 10
size-exact raw winners, the rest are `m_tempVec`/`v_dirVecA` read-after-write
rotations or change `.rel.plt`).

Measured alone on the post-#6 body: raw **51,730 -> 51,729** (-1), size
57,232/57,232 exact, `.text` 72,936 -> 72,935, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 8. `m_bCinematicBreak = 1` after `player->setFreeLookMode(false)`

Same hand-off block as #7.  A plain flag store moves past the HUD visibility
store and the free-look clear; neither reads `m_bCinematicBreak`.

```
-                        m_bCinematicBreak = 1;
                         m_pHud->visible = 0;
                         player->setFreeLookMode(false);
+                        m_bCinematicBreak = 1;
```

Measured alone on the post-#7 body: raw **51,729 -> 51,728** (-1), size
57,232/57,232 exact, `.text` 72,935 -> 72,934, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 9. `lodManager->forceUpdate` before the camera `translate`

Mission-8 state-1 pan.  Disjoint objects (`self->lodManager` vs `cam1688`);
neither reads the other's state.

```
-                    cam1688->translate((float) -delta, 0.0f, (float) (-3 * delta));
                     self->lodManager->forceUpdate(delta, false);
+                    cam1688->translate((float) -delta, 0.0f, (float) (-3 * delta));
```

Found by a fresh distance-1 sweep on the post-#8 body (1,534 candidates; 8
size-exact raw winners; four are `m_tempVec`/`camPos` read-after-write swaps
and one changes `.rel.plt`).

Measured alone on the post-#8 body: raw **51,728 -> 51,727** (-1), size
57,232/57,232 exact, `.text` 72,934 -> 72,933, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 10. `player->setVisible(true)` after `camera->setTarget`

Mission-3 arrival block (same block as #3).  Visibility flag vs camera target;
the camera target is `player->geometry`, which `setVisible` does not touch.

```
-                        player->setVisible(true);
                         camera->setTarget(player->geometry);
+                        player->setVisible(true);
```

Measured alone on the post-#9 body: raw **51,727 -> 51,726** (-1), size
57,232/57,232 exact, `.text` 72,933 -> 72,932, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 11. `player->killLiberator()` before `wcamera->setRumblePercentage`

Rocket-control teardown.  Disjoint objects; the rumble reset does not depend on
the liberator kill.

```
-                            wcamera->setRumblePercentage(0.0f, 0);
                             player->killLiberator();
+                            wcamera->setRumblePercentage(0.0f, 0);
```

Measured alone on the post-#10 body: raw **51,726 -> 51,725** (-1), size
57,232/57,232 exact, `.text` 72,932 -> 72,931, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.

### 12. `player->setFreeLookMode(false)` after `m_pCamera->setPosition(camPos)`

Mission-14 arrival block.  The free-look clear moves past the radar store, LOD
update, camera target, and the `camPos` local computation; none of them reads
the free-look flag, and `setPosition` does not depend on it.

```
-                    player->setFreeLookMode(false);
                     m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
                     Vector dir = ...;
                     Vector camPos = ...;
                     camPos += 3000.0f * dir;
+                    player->setFreeLookMode(false);
                     m_pCamera->setPosition(camPos);
```

Found by a distance-7..10 sweep on the post-#11 body (2,027 candidates); it was
the only landable winner (the other two move `switchSkyboxForIntro` across an
import first-reference and change `.rel.plt`).

Measured alone on the post-#11 body: raw **51,725 -> 51,722** (-3), size
57,232/57,232 exact, `.text` 72,931 -> 72,928, `.ARM.extab` 603 unchanged,
`.rel.plt` md5 unchanged.
