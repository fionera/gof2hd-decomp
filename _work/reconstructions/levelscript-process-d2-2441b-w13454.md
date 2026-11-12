# LevelScript::process — wave 190 / w13454: setLookAtCam sink (d2, second pass)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13454`, branch
`wave190-w13454`, base after the w13454 d2/d5/down-d4 wins: raw **51,696**,
aligned **21,928**, `.text` 72,252, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`.

A fresh `d=1`/`d=2` sweep on the post-win body re-found the known unsafe
`m_nScriptTimerA` cell (`d=2` line 5230, raw 51,693) and one new safe
size-exact cell.

## Landed change

Source lines 2441-2443 (mission-4-ish state-0 wake-up block):

```cpp
-                    m_pCamera->setLookAtCam(true);
                     m_pCamera->setActive(true);
                     Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
+                    m_pCamera->setLookAtCam(true);
```

`setLookAtCam` writes only the camera `lookAtCam` flag; `setActive` writes the
disjoint `active` flag, and `camPos` is a local read of `enemies[0]`.  The
rotation is semantics-preserving.

Measured alone on the real build:

```
raw        51,696 -> 51,695  (-1)
aligned    21,928 -> 21,928  (neutral)
size       57,232 / 57,232   (exact)
.text      72,252 -> 72,251  (-1)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      72,856 -> 72,855  (-1)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Rejected compositions

* `[setActive, camPos, setLookAtCam, dir]` (this win) is 51,695.
* `[camPos, setLookAtCam, setActive, dir]` is 51,697.
* `[camPos, dir, setLookAtCam, setActive]` and
  `[camPos, dir, setActive, setLookAtCam]` are both 51,697.
