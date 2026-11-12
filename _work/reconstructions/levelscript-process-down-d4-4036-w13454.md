# LevelScript::process — wave 190 / w13454: direction-offset first (down d4)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13454`, branch
`wave190-w13454`, base after the w13454 d2/d5 wins: raw **51,698**, aligned
**21,925**, `.text` 72,254, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`.

The fleet's `d=N` sweeps only rotate the window *up* (first statement to the
end).  A fresh **down** rotation sweep (`[last, first, ...]`) over the current
body found one safe size-exact win.

## Landed change

Source lines 4036-4040 (case-125 camera-follow block):

```cpp
-                        m_pCamera->setTarget(leader);
-                        m_tempVec += leader->getUpVector() * -500.0f;
-                        m_tempVec += leader->getRightVector() * -500.0f;
-                        m_pCamera->setPosition(m_tempVec);
                         m_tempVec += leader->getDirection() * 4000.0f;
+                        m_pCamera->setTarget(leader);
+                        m_tempVec += leader->getUpVector() * -500.0f;
+                        m_tempVec += leader->getRightVector() * -500.0f;
+                        m_pCamera->setPosition(m_tempVec);
```

The three `+=` offsets are accumulated into the same `m_tempVec`; moving the
direction offset to the front only re-associates the floating-point sum.
Crucially, this matches the original's emission order: at `0x13befa`
`getDirection`, `0x13bf14` `VectorpL`, `0x13bf2c` `getUpVector`, `0x13bf3a`
`VectorpL`, `0x13bf4a` `getRightVector`, `0x13bf5a` `VectorpL`, `0x13bf72`
`setPosition`.  `setTarget` writes only the camera target and does not touch
`m_tempVec`, so its new position between the offsets is irrelevant.

Measured alone on the real build:

```
raw        51,698 -> 51,696  (-2)
aligned    21,925 -> 21,928
size       57,232 / 57,232   (exact)
.text      72,254 -> 72,252  (-2)
.ARM.extab 584 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
total      72,858 -> 72,856  (-2)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Down-sweep results (base 51,698)

| sweep | candidates | size-exact below base | safe |
|---|---:|---:|---|
| down d=2 | 1,274 | 3 | none (rnd-call reorder, `*tmp` read-after-write, `m_tempVec` consumer) |
| down d=3 | 994 | 1 | no — moves `m_pGeometry6->setPosition(enemies[0]->geometry->getPosition())` before the enemy reposition |
| down d=4 | 811 | 1 | line 4036 (landed, -2) |

The down `d=3` cell at line 1268 is the largest raw win found in this pass
(51,685, -13) but is not semantics-preserving.
