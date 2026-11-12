# LevelScript::process — wave 190 / w13380: fresh d=1 harvest on 51,803

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
57,232 B, size-exact).  Worktree `wt/w13380`, branch `wave190-w13380`,
base master `5cd492611` (raw **51,803**, `.text` 73,487, `.ARM.extab` 610,
`.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

A fresh independent-adjacent-statement (`d=1`) sweep was run on the current
51,803 body with the exact NDK r18b compile + gold link (harness in
`/home/debian/GalaxyOnFire2/scratch/w13380/h/`).  The first landed winner:

## Landed change 1

Source lines 1491-1492 (mission 95 / pre-switch teardown):

```c++
-            m_bStartSequenceOver = 1; // startSequenceIsOver = true
-            m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11)
+            m_bCinematicBreak = 0; // clear cinematicBreak_ (high byte @0x11)
+            m_bStartSequenceOver = 1; // startSequenceIsOver = true
```

Two independent plain flag stores.  Measured alone on the real build:

```
raw        51,803 -> 51,802  (-1)
aligned    22,193 -> 22,193
size       57,232 / 57,232   (exact)
.text      73,487 -> 73,486  (-1)
.ARM.extab 610 (unchanged)
total      74,117 -> 74,116  (-1)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

## Landed change 2

Source lines 1609-1610 (mission 14 state-1 teardown):

```c++
-                    m_pRadar->enabled = 0;
-                    player->stopShooting(0);
+                    player->stopShooting(0);
+                    m_pRadar->enabled = 0;
```

Independent receivers.  Measured on the post-change-1 body:

```
raw        51,802 -> 51,801  (-1)
aligned    22,193 -> 22,193
size       57,232 / 57,232   (exact)
.text      73,486 -> 73,485  (-1)
.ARM.extab 610 (unchanged)
total      74,116 -> 74,115  (-1)
.rel.plt   byte-identical
```

## Landed change 3

Source lines 2496-2497 (mission 67? state block):

```c++
-                    player->setFreeLookMode(false);
-                    m_pLevel->lodManager->forceUpdate(delta, false);
+                    m_pLevel->lodManager->forceUpdate(delta, false);
+                    player->setFreeLookMode(false);
```

Independent receivers; the `forceUpdate` does not read the player's free-look
flag.  Measured on the post-change-2 body:

```
raw        51,801 -> 51,799  (-2)
aligned    22,193 -> 22,193
size       57,232 / 57,232   (exact)
.text      73,485 -> 73,483  (-2)
.ARM.extab 610 (unchanged)
total      74,115 -> 74,113  (-2)
.rel.plt   byte-identical
```

## Landed change 4

Source lines 1941-1942:

```c++
-                    player->setFreeLookMode(false);
-                    player->player->setVulnerable(false);
+                    player->player->setVulnerable(false);
+                    player->setFreeLookMode(false);
```

Measured on the post-change-3 body:

```
raw        51,799 -> 51,798  (-1)
aligned    22,193 -> 22,193
size       57,232 / 57,232   (exact)
.text      73,483 -> 73,482  (-1)
.ARM.extab 610 (unchanged)
total      74,113 -> 74,112  (-1)
.rel.plt   byte-identical
```

## Landed change 5

Source lines 3538-3539 (mission 102 state-0 block):

```c++
-                        wcamera->setLookAtCam(true);
-                        player->player->setVulnerable(false);
+                        player->player->setVulnerable(false);
+                        wcamera->setLookAtCam(true);
```

Independent receivers.  Measured on the post-change-4 body:

```
raw        51,798 -> 51,796  (-2)
aligned    22,193 -> 22,187  (-6)
size       57,232 / 57,232   (exact)
.text      73,482 -> 73,480  (-2)
.ARM.extab 610 (unchanged)
total      74,112 -> 74,109  (-3)
.rel.plt   byte-identical
```

## Landed change 6

Source lines 4219-4220:

```c++
-                        m_pCamera->setLookAtCam(false);
-                        m_pRadar->enabled = 1;
+                        m_pRadar->enabled = 1;
+                        m_pCamera->setLookAtCam(false);
```

Independent receivers.  Measured on the post-change-5 body:

```
raw        51,796 -> 51,795  (-1)
aligned    22,187 -> 22,187
size       57,232 / 57,232   (exact)
.text      73,480 -> 73,479  (-1)
.ARM.extab 610 (unchanged)
total      74,109 -> 74,109  (build-id hash fluctuation)
.rel.plt   byte-identical
```

## Landed change 7 (d=2..4 sweep)

Source lines 3269-3272 (mission 102 state-1 block), move
`m_pRadar->enabled = 1;` below `Globals::sound->stop(...)`:

```c++
-                            m_pRadar->enabled = 1;
-                            m_pHud->visible = 1;
-                            m_bCinematicBreak = 0;
-                            Globals::sound->stop(Globals::sound->currentMusicEvent);
+                            m_pHud->visible = 1;
+                            m_bCinematicBreak = 0;
+                            Globals::sound->stop(Globals::sound->currentMusicEvent);
+                            m_pRadar->enabled = 1;
```

Measured on the post-change-6 body:

```
raw        51,795 -> 51,792  (-3)
aligned    22,187 -> 22,186  (-1)
size       57,232 / 57,232   (exact)
.text      73,479 -> 73,476  (-3)
.ARM.extab 610 (unchanged)
total      74,109 -> 74,106  (-3)
.rel.plt   byte-identical
```

## Landed change 8 (d=2..4 sweep)

Source lines 2372-2374, sink `resetCamera(self)` below the flag/state stores:

```c++
-                    resetCamera(self);
-                    m_bCinematicBreak = 0;
-                    m_nState = 6;
+                    m_bCinematicBreak = 0;
+                    m_nState = 6;
+                    resetCamera(self);
```

Measured on the post-change-7 body:

```
raw        51,792 -> 51,790  (-2)
aligned    22,186 -> 22,186
size       57,232 / 57,232   (exact)
.text      73,476 -> 73,474  (-2)
.ARM.extab 610 (unchanged)
total      74,106 -> 74,104  (-2)
.rel.plt   byte-identical
```

## Landed change 9 (d=2..4 sweep)

Source lines 2296-2298, sink `(*enemies)[0]->field_0x24 = 0;` below the
`Route` construction:

```c++
-                    (*enemies)[0]->field_0x24 = 0;
-                    int routePoints[3] = {500000, 500000, 500000};
-                    Route *route = new Route(routePoints, 3);
+                    int routePoints[3] = {500000, 500000, 500000};
+                    Route *route = new Route(routePoints, 3);
+                    (*enemies)[0]->field_0x24 = 0;
```

Measured on the post-change-8 body:

```
raw        51,790 -> 51,789  (-1)
aligned    22,186 -> 22,186
size       57,232 / 57,232   (exact)
.text      73,474 -> 73,473  (-1)
.ARM.extab 610 (unchanged)
total      74,104 -> 74,103  (-1)
.rel.plt   byte-identical
```

## Landed change 10 (fresh d=1 sweep on 51,789, corrected 0-based indices)

Source lines 3882-3883 (mission 142? camera marker block):

```c++
-                            marker->setPosition(player->geometry->getPosition());
-                            wcamera->setTarget(marker);
+                            wcamera->setTarget(marker);
+                            marker->setPosition(player->geometry->getPosition());
```

Independent receiver; `setTarget` does not read the marker position.  The
earlier d=1 sweep reported this as `['down',3881,3882]`; the harness indices
are 0-based, so the real source lines are 3882/3883 (the previous
`marker/setTarget/sound` rotation at 3881 was a different, +4-byte variant).

Measured on the post-change-9 body:

```
raw        51,789 -> 51,788  (-1)
aligned    22,186 -> 22,178  (-8)
size       57,232 / 57,232   (exact)
.text      73,473 -> 73,472  (-1)
.ARM.extab 610 (unchanged)
total      74,103 -> 74,102  (-1)
.rel.plt   byte-identical
```

## Landed change 11 (fresh d=1 sweep, corrected 0-based indices)

Source lines 2532-2533 (mission 70 state-1 camera reframe):

```c++
-                        camPos.y += 800.0f;
-                        m_pCamera->setPosition(camPos);
+                        m_pCamera->setPosition(camPos);
+                        camPos.y += 800.0f;
```

The original emits all four `camPos` adjustments (x += -600, y += 800,
+= dir*2000, z += -1000) before the `setPosition` call (0x13c11e-0x13c152);
the compiler hoists the pure local-vector adds across the call either way.
The swap moves one more add across the call and matches one more text byte.

Measured on the post-change-10 body:

```
raw        51,788 -> 51,787  (-1)
aligned    22,178 -> 22,178  (unchanged)
size       57,232 / 57,232   (exact)
.text      73,472 -> 73,471  (-1)
.ARM.extab 610 (unchanged)
total      74,102 -> 74,101  (-1)
.rel.plt   byte-identical
```

## Landed change 12 (fresh d=1 sweep, corrected 0-based indices)

Source lines 3536-3537 (mission 102 state-0 teardown):

```c++
-                        m_bCinematicBreak = 1;
-                        m_pRadar->enabled = 0;
+                        m_pRadar->enabled = 0;
+                        m_bCinematicBreak = 1;
```

Independent stores (different receivers).  The old wave-189 note called a
3535/3536 variant neutral; the harness index is 0-based, so the real lines
are 3536/3537 and it is a one-byte win.

Measured on the post-change-11 body:

```
raw        51,787 -> 51,786  (-1)
aligned    22,178 -> 22,178  (unchanged)
size       57,232 / 57,232   (exact)
.text      73,471 -> 73,470  (-1)
.ARM.extab 610 (unchanged)
total      74,101 -> 74,100  (-1)
.rel.plt   byte-identical
```

## Landed change 13 (fresh d=1 sweep, corrected 0-based indices)

Source lines 3735-3736:

```c++
-                        *camPos += *tmp * 400.0f;
-                        m_pCamera->setPosition(*camPos);
+                        m_pCamera->setPosition(*camPos);
+                        *camPos += *tmp * 400.0f;
```

The add is a pure local-vector update; the compiler hoists it across the
call either way.

Measured on the post-change-12 body:

```
raw        51,786 -> 51,785  (-1)
aligned    22,178 -> 22,178  (unchanged)
size       57,232 / 57,232   (exact)
.text      73,470 -> 73,469  (-1)
.ARM.extab 610 (unchanged)
total      74,100 -> 74,099  (-1)
.rel.plt   byte-identical
```

## Landed change 14 (fresh d=1 sweep, corrected 0-based indices)

Source lines 4542-4543 (mission 154 state-0):

```c++
-                    m_tempVec += Vector{300.0f, 300.0f, 5800.0f};
-                    m_pCamera->setPosition(m_tempVec);
+                    m_pCamera->setPosition(m_tempVec);
+                    m_tempVec += Vector{300.0f, 300.0f, 5800.0f};
```

Pure local-vector add hoisted across the call.

Measured on the post-change-13 body:

```
raw        51,785 -> 51,784  (-1)
aligned    22,178 -> 22,178  (unchanged)
size       57,232 / 57,232   (exact)
.text      73,469 -> 73,468  (-1)
.ARM.extab 610 (unchanged)
total      74,099 -> 74,098  (-1)
.rel.plt   byte-identical
```

## Landed change 15 (fresh d=1 sweep, corrected 0-based indices)

Source lines 4963-4964:

```c++
-                        m_tempVec += Vector{45000.0f, 2000.0f, 11000.0f};
-                        chaser->setPosition(m_tempVec + Vector{15000.0f, -200.0f, 1000.0f});
+                        chaser->setPosition(m_tempVec + Vector{15000.0f, -200.0f, 1000.0f});
+                        m_tempVec += Vector{45000.0f, 2000.0f, 11000.0f};
```

Pure local-vector add hoisted across the call.

Measured on the post-change-14 body:

```
raw        51,784 -> 51,783  (-1)
aligned    22,178 -> 22,178  (unchanged)
size       57,232 / 57,232   (exact)
.text      73,468 -> 73,467  (-1)
.ARM.extab 610 (unchanged)
total      74,098 -> 74,097  (-1)
.rel.plt   byte-identical
```

## Landed change 16 (fresh d=1 sweep, corrected 0-based indices)

Source lines 3049-3050:

```c++
-                    wcamera->setTarget(leader);
-                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
+                    AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
+                    wcamera->setTarget(leader);
```

Independent statements.  `aligned` moves +5 but raw (byte-identicality
ground truth) improves by 1.

Measured on the post-change-15 body:

```
raw        51,783 -> 51,782  (-1)
aligned    22,178 -> 22,183  (+5)
size       57,232 / 57,232   (exact)
.text      73,467 -> 73,466  (-1)
.ARM.extab 610 (unchanged)
total      74,097 -> 74,096  (-1)
.rel.plt   byte-identical
```

## Landed change 17 (fresh d=1 sweep, corrected 0-based indices)

Source lines 890-891:

```c++
-                        m_tempVec += Vector{-2000.0f, -2000.0f, -5000.0f};
-                        m_pCamera->setPosition(m_tempVec);
+                        m_pCamera->setPosition(m_tempVec);
+                        m_tempVec += Vector{-2000.0f, -2000.0f, -5000.0f};
```

Pure local-vector add hoisted across the call.  `aligned` +5, raw -1.

Measured on the post-change-16 body:

```
raw        51,782 -> 51,781  (-1)
aligned    22,183 -> 22,188  (+5)
size       57,232 / 57,232   (exact)
.text      73,466 -> 73,465  (-1)
.ARM.extab 610 (unchanged)
total      74,096 -> 74,095  (-1)
.rel.plt   byte-identical
```

## Landed change 18 (fresh d=1 sweep, corrected 0-based indices)

Source lines 2366-2368 (mission 95 state-5 setup):

```c++
-                    (*enemies)[10]->setSpeed(8.0f);
-                    m_pCamera->setTarget(player->geometry);
-                    player->setComputerControlled(false);
+                    player->setComputerControlled(false);
+                    (*enemies)[10]->setSpeed(8.0f);
+                    m_pCamera->setTarget(player->geometry);
```

Independent statements.  Measured on the post-change-17 body:

```
raw        51,781 -> 51,780  (-1)
size       57,232 / 57,232   (exact)
```

## Landed change 19 (fresh d=1 sweep on 51,780, `res_d1e.jsonl`)

Source lines 829-830 (mission-0 state 9 intro setup):

```c++
-                    m_pLevel->switchSkyboxForIntro();
-                    m_pLevel->particleRenderBoolPtr->reset();
+                    m_pLevel->particleRenderBoolPtr->reset();
+                    m_pLevel->switchSkyboxForIntro();
```

Independent `Level` calls.  Measured on the post-change-18 body:

```
raw        51,780 -> 51,779  (-1)
size       57,232 / 57,232   (exact)
```

## Landed change 20 (same `res_d1e.jsonl` sweep)

Source lines 3883-3884:

```c++
-                            wcamera->setTarget(marker);
-                            Vector zero{0.0f, 0.0f, 0.0f};
+                            Vector zero{0.0f, 0.0f, 0.0f};
+                            wcamera->setTarget(marker);
```

Local-vector declaration hoisted above the call.  Measured on the
post-change-19 body:

```
raw        51,779 -> 51,778  (-1)
size       57,232 / 57,232   (exact)
```

## Landed change 21 (rebase of `res_d24b` winner onto 51,778)

Source lines 3790-3793 (mission 105 state-7/8 tail):

```c++
-                        m_bCinematicBreak = 0; // clear cinematicBreak_ (byte @0x11 = 0)
-                        m_nState = m_nState + 1; // 7 -> 8
-                        m_nScriptTimerA = 0;
-                        m_pLevel->lodManager->forceUpdate(delta, false);
+                        m_nState = m_nState + 1; // 7 -> 8
+                        m_nScriptTimerA = 0;
+                        m_pLevel->lodManager->forceUpdate(delta, false);
+                        m_bCinematicBreak = 0; // clear cinematicBreak_ (byte @0x11 = 0)
```

Independent stores + a `LODManager` call.  Measured on the post-change-20
body:

```
raw        51,778 -> 51,777  (-1)
size       57,232 / 57,232   (exact)
```

## Landed change 22 (same `res_d24b` rebase)

Source lines 3552-3554:

```c++
-                    m_nScriptTimerA += delta;
-                    Vector zero{0.0f, 0.0f, 0.0f};
-                    Vector pos = (*enemies)[0]->geometry->getPosition();
+                    Vector zero{0.0f, 0.0f, 0.0f};
+                    Vector pos = (*enemies)[0]->geometry->getPosition();
+                    m_nScriptTimerA += delta;
```

Independent local declarations and a field increment.  Measured on the
post-change-21 body:

```
raw        51,777 -> 51,776  (-1)
size       57,232 / 57,232   (exact)
```

## Landed change 23 (same `res_d24b` rebase)

Source lines 3346-3349:

```c++
-                        camera->setLookAtCam(false);
-                        player->setVisible(true);
-                        camera->setTarget(player->geometry);
-                        m_pLevel->getPlayer()->resetGunDelay();
+                        m_pLevel->getPlayer()->resetGunDelay();
+                        camera->setLookAtCam(false);
+                        player->setVisible(true);
+                        camera->setTarget(player->geometry);
```

Independent receivers.  Measured on the post-change-22 body:

```
raw        51,776 -> 51,775  (-1)
size       57,232 / 57,232   (exact)
```

## Landed change 24 (rebase of `res_d24b` winner, mission-0 state 8)

Source lines 2695-2697:

```c++
-                        m_tempVec.x += 4500.0f;
-                        m_tempVec.z += 1000.0f;
-                        AbyssEngine::AEMath::MatrixSetTranslation(m_matrix, m_tempVec.x, m_tempVec.y, m_tempVec.z);
+                        AbyssEngine::AEMath::MatrixSetTranslation(m_matrix, m_tempVec.x, m_tempVec.y, m_tempVec.z);
+                        m_tempVec.x += 4500.0f;
+                        m_tempVec.z += 1000.0f;
```

The two `m_tempVec` component adds are dead for the translation (the
original emits the `MatrixSetTranslation` first; `m_tempVec` is reassigned
from `m_pGeometry6->getPosition()` immediately afterwards).  Measured on
the post-change-23 body:

```
raw        51,775 -> 51,774  (-1)
size       57,232 / 57,232   (exact)
.text      73,459 -> 73,458
.ARM.extab 610 (unchanged)
```

## Landed change 25 (same `res_d24b` rebase)

Source lines 3158-3160:

```c++
-                        v_dirVecA = (*enemies)[3]->geometry->getRightVector();
-                        m_tempVec += v_dirVecA * 4800.0f;
-                        m_pCamera->setPosition(m_tempVec);
+                        m_pCamera->setPosition(m_tempVec);
+                        v_dirVecA = (*enemies)[3]->geometry->getRightVector();
+                        m_tempVec += v_dirVecA * 4800.0f;
```

Measured on the post-change-24 body:

```
raw        51,774 -> 51,773  (-1)
size       57,232 / 57,232   (exact)
.text      73,458 -> 73,457
.ARM.extab 610 (unchanged)
```
