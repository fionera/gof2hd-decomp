# LevelScript::process — wave 190 / w13454: enableFirstPersonCam sink (d5)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13454`, branch
`wave190-w13454`, base after the w13454 d2 state-6 setPosition sink: raw
**51,699**, aligned **21,925**, `.text` 72,255, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`.

A fresh full `d=5` rotation sweep (684 candidates, exact NDK r18b `-Oz`
compile + gold link, 4 workers) found one size-exact, `.rel.plt`-clean raw
win.

## Landed change

Source lines 2441-2446 (mission-4-ish state-0 wake-up block):

```cpp
-                    m_pCamera->enableFirstPersonCam(false);
                     m_pCamera->setLookAtCam(true);
                     m_pCamera->setActive(true);
                     Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
                     Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
                     camPos += dir * 10000.0f;
+                    m_pCamera->enableFirstPersonCam(false);
```

`enableFirstPersonCam(false)` writes only the camera's `firstPerson`,
`fpOffset`, `shakeAccum` and `shakeReference`; the crossed statements are
three camera flag setters and local `camPos`/`dir` arithmetic that reads only
`enemies[0]`.  The rotation is semantics-preserving.

Measured alone on the real build:

```
raw        51,699 -> 51,698  (-1)
aligned    21,925 -> 21,925  (neutral)
size       57,232 / 57,232   (exact)
.text      72,255 -> 72,254  (-1)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      72,859 -> 72,858  (-1)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Sweep summary (w13454)

| sweep | candidates | size-exact below base | safe win |
|---|---:|---:|---|
| d=1 | 1,722 | 2 | none (both read-after-write) |
| d=2 | 1,274 | 2 | line 2669 (landed, -2) |
| d=3 | 994 | 0 | — |
| d=4 | 811 | 1 | line 4035 read-after-write (`m_tempVec` producer) |
| d=5 | 684 | 1 | line 2441 (landed, -1) |
