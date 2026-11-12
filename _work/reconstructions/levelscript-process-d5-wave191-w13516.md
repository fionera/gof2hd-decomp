# LevelScript::process — wave191 / w13516: d5/d6 continuation (multi-line statement cells)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13516`, branch
`wave191b-w13516`, base master `5c99979c4` (raw **51,639**, aligned **22,078**,
`.text` 71,416, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Sweeps run

1. **Exhaustive line-based d5/d6 rotation sweep** on the current body
   (1,141 five-line + 975 six-line windows x up/down = 4,232 candidates,
   NDK r18b `-Oz` compile + gold link for every size-exact candidate).
   Result: exactly two cells below base, both unusable —
   * `(5, line 2673) up` raw 51,638 (-1): moves `delete frame;` before the
     `frame->setDirection/setPosition/CameraSetLocal` uses (use-after-free).
   * `(6, line 829) down` raw 51,638 (-1): changes `.rel.plt` md5 to
     `d997d11f…` (fatal).
   No other cell is below 51,639.
2. **Multi-line-statement d5/d6 rotation sweep** (the remaining cells: the
   line-based parser cannot see statements whose `;` is on a continuation
   line).  Statements were re-joined across continuation lines (138 multi-line
   statements), then the same up/down rotation sweep was run (1,252 + 1,072
   windows).  This is the space the previous d5/d6 passes missed.

## Landed change

Mission-? state-10 flash block (source lines 5077-5083).  Rotate the
5-statement group up by one, moving the `m_pGeometry2` transform
`SetAnimationState(2)` to the front of the group:

```cpp
                         if (m_nFlashCountdown <= 0) {
+                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry2->transform))
+                                    ->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
                             m_nFlashCountdown = 0;
                             m_pGeometry2->setPosition(
                                     ((PlayerStation *) (*enemies)[stationIndex])->rootGeometry->getPosition());
                             m_pGeometry2->setVisible(true);
                             m_pGeometry2->setDirection(Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
-                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry2->transform))
-                                    ->SetAnimationState((AbyssEngine::AnimationMode) 2, nullptr);
                         }
```

`Transform::SetAnimationState` is a pure setter (`this->flags = mode` for
mode 2) on the geometry's *transform* object; the intervening statements write
`m_nFlashCountdown` and the geometry's position/visibility/direction.  No
statement between the old and new position reads the transform flags, and no
`Update` is called in between, so the reorder is semantics-preserving.

Measured alone on the real build:

```
raw        51,639 -> 51,636  (-3)
aligned    22,078 -> 22,183
size       57,232 / 57,232   (exact)
.text      71,416 -> 71,413  (-3)
.ARM.extab 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, diff 0)
```

`check` -> RATCHET PASS (byte_exact 4507/4524, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Other measured multi-line cells (not landed)

| merged line | dir | raw | note |
|---|---|---:|---|
| 3940 (line 4082) | down | 51,637 | `setDockingType(0)` moved after `m_nState++`; safe, -2 alone, but composing with the landed cell gives 51,650 (non-additive layout shift) |
| 771 (line 773) | down | 51,636 | moves `SetAnimationState(3)` after `m_nScriptTimerA = 0`, i.e. after `SetAnimationState(1)` — changes final animation state, rejected |
| 1130 (line 1157) | up | 51,639 | `sound->play(160)` before the two `SetAnimationState` calls; neutral on the real link |
| 2316 (line 2390) | down | 51,639 | `m_pHud->visible = 0` after `stopShooting(0)`; neutral on the real link |
| 740 (line 742) | down | 51,640 | `field_5e = 0` after `radar->enabled = 1`; +1 |
| 2597 (line 2673) | up | 51,638 | `delete frame` first — use-after-free, rejected |

Note: the multi-line object-file screen under-reports the link delta for
call-reorder cells (relocation bytes are zero in the object); every cell above
was re-measured with a full gold link.

## Second landing: d5 shift-2 rotation (mission-? pirate route block)

After the first landing (raw 51,636), the remaining cyclic shifts (shift 2/3/4,
i.e. the rotations the up/down sweep does not test) were screened on the
line-based d5/d6 windows and every size-exact candidate was re-measured with a
full gold link.

Landed: source lines 2377-2381, rotate the 5-statement group by 2:

```cpp
-                        Vector origin = (*enemies)[0]->getPosition();
-                        (*enemies)[10]->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
                         int routePoints[3] = {0, 0, 800000};
                         (*enemies)[10]->setSpeed(2.0f);
                         (*enemies)[10]->setActive(true);
+                        Vector origin = (*enemies)[0]->getPosition();
+                        (*enemies)[10]->geometry->setDirection(Vector{0.0f, 0.0f, 1.0f}, Vector{0.0f, 1.0f, 0.0f});
```

`routePoints` is a local array declaration and `origin` a local Vector; neither
is read by the intervening `setSpeed`/`setActive` calls on `enemies[10]`, and
`setDirection` on `enemies[10]->geometry` is independent of both.  Semantics
preserving.

Measured alone on the real build:

```
raw        51,636 -> 51,633  (-3)
aligned    22,183 -> 22,183
size       57,232 / 57,232   (exact)
.text      71,413 -> 71,410  (-3)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.

Other shift-2/3/4 cells measured on the 51,636 body: `(5,3325) shift 2`
raw 51,634 (-2, safe), `(5,1206) shift 2/3` 51,636 (neutral),
`(5,3221) shift 2` 51,637, `(5,689) shift 2` changes `.rel.plt` (rejected),
`(5,1236/1237) shift 2` 51,635 but move `m_tempVec.z +=` before the
`m_tempVec = getPosition()` assignment (rejected), `(5,2409) shift 3` and
`(5,3008) shift 3` reorder RNG/vector reads (rejected).

## Third landing: d5 shift-2 rotation (hud/radar/target group)

On the 51,633 body, the previously measured `(5,3325) shift 2` cell was
re-measured and landed.  Source lines 3326-3330:

```cpp
-                        m_bCinematicBreak = 1;
-                        player->player->setVulnerable(false);
                         m_pHud->visible = 0;
                         m_pRadar->enabled = 0;
                         m_pCamera->setTarget((*enemies)[0]->geometry);
+                        m_bCinematicBreak = 1;
+                        player->player->setVulnerable(false);
```

Independent flag stores (cinematic break, player vulnerability) vs HUD/radar
enables and the camera target; no statement reads what another writes.

Measured alone on the real build:

```
raw        51,633 -> 51,631  (-2)
aligned    22,183 -> 22,188
size       57,232 / 57,232   (exact)
.text      71,410 -> 71,408  (-2)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.

## Fourth landing: d6 shift-2 rotation (resetCamera to the tail)

On the 51,631 body, `(6,2388) shift 2` (source lines 2389-2394) was landed:

```cpp
-                        resetCamera(m_pLevel);
-                        m_pHud->visible = 0;
                         m_pCamera->enableFirstPersonCam(false);
                         m_pRadar->enabled = 0;
                         player->hideShipForFirstPersonCameraView(false);
                         player->stopShooting(0);
+                        resetCamera(m_pLevel);
+                        m_pHud->visible = 0;
```

`resetCamera` only sets the camera target/offsets; the intervening HUD store,
first-person flag, radar enable, hide-ship flag and stop-shooting call are
independent of it (same independence argument used by the wave-190 d5/d6
passes).  Semantics preserving.

Measured alone on the real build:

```
raw        51,631 -> 51,630  (-1)
size       57,232 / 57,232   (exact)
.text      71,408 -> 71,407  (-1)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.

## Fifth landing: d6 shift-2 rotation (free-look/FP flags to the tail)

On the 51,630 body, `(6,4023) shift 2` (source lines 4024-4029) was landed:

```cpp
-                        player->setFreeLookMode(false);
-                        m_pCamera->enableFirstPersonCam(false);
                         player->hideShipForFirstPersonCameraView(false);
                         m_bCinematicBreak = 1;
                         player->stopShooting(0);
                         m_pHud->visible = 0;
+                        player->setFreeLookMode(false);
+                        m_pCamera->enableFirstPersonCam(false);
```

Independent player/camera flags and HUD/shooting state; no statement reads what
another writes.  Semantics preserving.

Measured alone on the real build:

```
raw        51,630 -> 51,628  (-2)
size       57,232 / 57,232   (exact)
.text      71,407 -> 71,405  (-2)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.

Rejected in the same batch: `(6,3766) shift 2` raw 51,629 but rotates the
`*tmp = getDirection/getRightVector/getUpVector` reads against the
`*acc +=` uses (wrong vectors); `(6,3221) shift 2` raw 51,631 (+1).

## Sixth landing: d6 shift-3 rotation (turret/HUD/shoot group)

On the 51,628 body, `(6,2004) shift 3` (source lines 2005-2010) was landed:

```cpp
-                        player->setTurretMode(false);
-                        m_pHud->visible = 0;
-                        player->stopShooting(0);
                         player->freeze = 1;
                         camera41->setLookAtCam(true);
                         resetCamera(m_pLevel);
+                        player->setTurretMode(false);
+                        m_pHud->visible = 0;
+                        player->stopShooting(0);
```

Independent player/camera flags and HUD state; `setTurretMode(false)` still
precedes `stopShooting(0)` (which early-returns when the turret is active), so
the shooting semantics are unchanged.  Semantics preserving.

Measured alone on the real build:

```
raw        51,628 -> 51,625  (-3)
size       57,232 / 57,232   (exact)
.text      71,405 -> 71,402  (-3)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.

Rejected in the same batch: `(6,4154) shift 4` raw 51,626 but moves
`delete cargo` before `revive()` (cargo lifetime); `(5,2628) shift 2` moves
`setTurretMode(false)` after `stopShooting(0)` (early-return change);
`(5,2664) shift 3` moves `facing` before `m_pGeometry6->setPosition`;
`(6,1810) shift 3` moves `translate` before `setPosition`.

## Seventh landing: multi-line d5 shift-3 (transport wake group)

On the 51,625 body, the remaining multi-line-statement windows were swept over
*all* cyclic shifts (1,076 candidates, full gold link each).  Landed
`(5,1879) shift 3` (source lines 1933-1939):

```cpp
-                        ((PlayerFixedObject *) (*enemies)[0])->setMoving(true);
-                        KIPlayer *transport = (*enemies)[0];
-                        transport->setPosition(...);
                         player->setTurretMode(false);
                         (*enemies)[0]->setVisible(true);
+                        ((PlayerFixedObject *) (*enemies)[0])->setMoving(true);
+                        KIPlayer *transport = (*enemies)[0];
+                        transport->setPosition(...);
```

`transport` is a local declaration, `setTurretMode`/`setVisible` are independent
player/enemy flags; `setPosition` is on the same enemy but does not depend on
either.  Semantics preserving.

Measured alone on the real build:

```
raw        51,625 -> 51,623  (-2)
size       57,232 / 57,232   (exact)
.text      71,402 -> 71,400  (-2)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.
