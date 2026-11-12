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

## Current-master harvest (wave 190)

The `radar enable below sound stop` d24 move is the only one of the three
that is a win on current master: raw **51,747 -> 51,744** (-3), aligned
22,483 -> 22,482, size exact 57,232 B. `.text` **73,003 -> 73,000** (-3);
`.ARM.extab` 603 and build-id 20 unchanged; total (text+extab+build-id)
**73,626 -> 73,623** (-3). `check` -> RATCHET PASS.

The w13368 `hideShip`/`m_pRadar->enabled` swap (`6d52e5601`) was measured
**+2 raw on current master** (51,747 -> 51,749) and was not harvested.
