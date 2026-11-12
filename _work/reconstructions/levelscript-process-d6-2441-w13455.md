# LevelScript::process — wave 190 / w13455: first-person flag sink

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13455`, branch
`wave190-w13455`.  Base after the d2 escort-revive sink: raw **51,698**,
`.text` 72,254.

A full `d=6` rotation sweep on the current body (1,694 candidates) found one
size-exact, `.rel.plt`-clean cell below the base.  (The d=5 sweep's two
sub-51,698 cells both move a `m_tempVec +=` read-modify-write across the
`setPosition(m_tempVec)` reader and are not semantics-preserving.)

## Landed change

Source lines 2441-2446 (mission-4-ish state-0 wake-up camera block):

```cpp
-                    m_pCamera->enableFirstPersonCam(false);
                     m_pCamera->setLookAtCam(true);
                     m_pCamera->setActive(true);
                     Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
                     Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
                     camPos += dir * 10000.0f;
+                    m_pCamera->enableFirstPersonCam(false);
```

`TargetFollowCamera::enableFirstPersonCam` writes only `firstPerson`,
`fpOffset`, `shakeAccum` and `shakeReference`; `setLookAtCam` writes only
`lookAtCam`; `setActive` writes only `active`.  The moved call crosses two pure
reads of `enemies[0]` (position/direction) and a stack-vector add, none of
which touch the camera.  The final camera placement is the later
`setPosition(camPos)`, so the reorder is semantics-preserving.

Measured alone on the real build:

```
raw        51,698 -> 51,697  (-1)
aligned    21,955 -> 21,955
size       57,232 / 57,232   (exact)
.text      72,254 -> 72,253  (-1)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
