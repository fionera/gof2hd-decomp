# LevelScript::process — wave 190 / w13455: state-6 `facing` hoist

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13455`, branch
`wave190-w13455`, base master `83d52fe52` (raw **51,701**, aligned **21,920**,
`.text` 72,257, `.ARM.extab` 584, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

Lens: fresh full `d=1` and `d=2` adjacent-statement rotation sweeps on the
current 51,701 body (1,788 d=1 candidates; 2,978 d=2 rotations, exact NDK
r18b `-Oz` compile + gold link, 4 workers).  The d=1 space is exhausted (every
size-exact cell is a documented read-after-write rotation or the
`.rel.plt`-fatal mission-0 state-9 family).  One new safe d=2 cell landed.

## Landed change

Source lines 2669-2671 (mission state-6 camera-frame block):

```cpp
-                    m_pCamera->setPosition(m_tempVec);
                     Vector facing = m_pGeometry6->getPosition() - m_tempVec;
                     m_pHud->visible = 0;
+                    m_pCamera->setPosition(m_tempVec);
```

`TargetFollowCamera::setPosition(const Vector&)` only copies the three
components into the camera's own `position`; it does not write `m_tempVec`,
`m_pGeometry6` or the `Hud`.  `facing` reads `m_pGeometry6->getPosition()` and
`m_tempVec`, and `m_pHud->visible` is a disjoint byte store, so hoisting both
above the camera store is semantics-preserving.  (The w13442 d3 pass had
already sunk `m_pHud->visible` past the `facing` producer; this rotation moves
`facing` and the Hud store ahead of `setPosition`.)

Measured alone on the real build:

```
raw        51,701 -> 51,699  (-2)
aligned    21,920 -> 21,925
size       57,232 / 57,232   (exact)
.text      72,257 -> 72,255  (-2)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Sweep notes (for future passes)

The d=1 sweep on the current body found only five `-1`/`-2` cells, all
documented:
* 829/830 `switchPlanetForIntro` across `switchSkyboxForIntro` — `.rel.plt`
  md5 `d997d11f…`, rejected;
* 890/891, 3736/3737, 4855/4856 — `m_tempVec`/`*tmp` read-after-write;
* 5230/5231 — `approach` reads `m_nScriptTimerA` (read-after-write).

The d=2 sweep's other size-exact sub-51,701 cells are all unsafe:
* 3768-3770 up: `*acc += *tmp*0.03` before `*tmp = geo->getRightVector()`
  (uses the previous right vector) — raw 51,697;
* 5230-5232 down: `approach` before the timer increment — raw 51,698;
* 1238-1240 down / 1237-1239 up: `m_tempVec.y/z` increments before the
  producer — raw 51,699/51,700;
* 1728-1730 up: `setPosition(camPos)` before the `camPos +=` offset — raw
  51,699;
* 2411-2413 up: swaps the two `Globals::rnd->nextInt` calls (RNG order) —
  raw 51,700;
* 4604-4606 up: `MatrixSetTranslation` before the `m_tempVec` update — raw
  51,700;
* 4155-4157 down: `revive()` after `setPosition` on the same enemy — raw
  51,700 (same-object order, not verified safe).
