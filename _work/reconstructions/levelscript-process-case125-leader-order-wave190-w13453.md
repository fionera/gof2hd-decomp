# LevelScript::process — wave 190 / w13453: case-125 leader camera offset order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13453`, branch
`wave190-w13453`, base master `83d52fe52` (raw **51,701**, aligned **21,920**,
`.text` 72,257, `.ARM.extab` 584, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

Lens: literal-pool ordering / branch-target-cold-block layout / prologue
register allocation.  A fresh full `d=4` rotation sweep (1,038 candidates) and
a `d=5` both-direction sweep (1,696 candidates) on the current body found one
size-exact, `.rel.plt`-clean raw win; it also fixes a real source-order
divergence from the original in the mission-125 leader-camera block.

## Landed change

Source lines 4034-4040 (case 125, `(*enemies)[4]` leader camera framing):

```cpp
 AEGeometry *leader = (*enemies)[4]->geometry;
 m_tempVec = leader->getPosition();
+m_tempVec += leader->getDirection() * 4000.0f;
 m_pCamera->setTarget(leader);
 m_tempVec += leader->getUpVector() * -500.0f;
 m_tempVec += leader->getRightVector() * -500.0f;
 m_pCamera->setPosition(m_tempVec);
-m_tempVec += leader->getDirection() * 4000.0f;
```

The original's block (`0x13bed6`..`0x13bf72`) emits
`setTarget`, `getPosition`, `getDirection`, `getUpVector`,
`getRightVector`, `setPosition` — i.e. its `setPosition` sees the
`+direction*4000` offset.  Our old source applied that offset *after*
`setPosition`, so the camera position was wrong.  Moving the direction add
above `setTarget` makes `setPosition` see the same vector as the original;
the final `m_tempVec` value is unchanged (vector addition is commutative) and
`setTarget` only stores the geometry pointer, so the reorder is
semantics-preserving (and faithful to the original).

Measured alone on the real build:

```
raw        51,701 -> 51,699  (-2)
aligned    21,920 -> 21,923
size       57,232 / 57,232   (exact)
.text      72,257 -> 72,255  (-2)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      72,861 -> 72,859  (-2)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Sweep notes

* Full `d=4` rotation sweep (1,038 candidates, 4 workers): best size-exact
  candidate equals the base (51,701); no win.
* Full `d=5` both-direction sweep (1,696 candidates, 4 workers): the only
  candidates below the base are this cell (`-2`) and a `-1` variant of the
  same block that moves `m_tempVec = leader->getPosition()` below
  `setPosition` (semantically wrong: `setPosition` would read the old
  vector).  All other `d=5` cells are >= 51,701.
* Matching the original's full order (`setTarget` before `getPosition`) is
  size-exact but `+7` raw (51,708); the raw metric prefers `setTarget` after
  the position/direction producers, so only the direction-add hoist is
  landed.
