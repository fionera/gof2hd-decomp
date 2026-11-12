# LevelScript::process — wave 191 / w13515: d1/d2 re-sweep + d3 store hoist

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13515`, branch
`wave191b-w13515`, base master `5c99979c4` (raw **51,639**, aligned 22,078,
`.text` 71,416, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

## Sweeps

Fresh full adjacent-statement sweeps on the current body, exact NDK r18b `-Oz`
compile + gold link, 4 workers:

* d=1: 2,911 windows x up/down.  No cell below 51,639 (all 5,822 builds
  measured >= 51,639).
* d=2: 2,113 windows x up/down.  Five sub-51,639 cells, all documented
  read-after-write / `.rel.plt`-fatal cells:
  `889/890` and `4543/4544` (`m_tempVec +=` before `setPosition(m_tempVec)`),
  `1238/1239` (`.y +=` before the producer), `3862/3863`
  (`offset *= 1000` after `*camPos += offset`), `4964/4965`
  (`m_tempVec +=` before `chaser->setPosition`), and `829/830`
  (`switchSkyboxForIntro` across `particleRenderBoolPtr->reset`, rel.plt
  md5 `d997d11f…`).
* d=3: 1,658 windows x up/down.  One new safe cell landed (below); the other
  sub-51,639 cells are read-after-write (`1238-1240`, `1728-1730`,
  `3768-3770`, `4542-4544`, `4604-4606`, `4964-4966`) or the RNG-order swap
  `2411-2413`.

## Landed change

Source lines 4717-4719 (mission-0 state 6 arrival):

```cpp
-                        player->setSpeed(0.0f);
-                        m_pCamera->setPosition(m_tempVec);
-                        m_nScriptTimerA = 0;
+                        m_nScriptTimerA = 0;
+                        player->setSpeed(0.0f);
+                        m_pCamera->setPosition(m_tempVec);
```

`m_nScriptTimerA = 0` is a plain store to a `this` field.  `player->setSpeed`
writes the player's speed, and `m_pCamera->setPosition(m_tempVec)` reads only
`m_tempVec` and writes the camera position; neither reads the script timer.
The hoist is semantics-preserving.

Measured on the real build:

```
raw        51,639 -> 51,638  (-1)
aligned    22,078 -> 22,079
size       57,232 / 57,232   (exact)
.text      71,416 -> 71,415  (-1)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
