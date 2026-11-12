# LevelScript::process — wave 190 / w13442: mission state-6 Hud store sink

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13442`, branch
`wave190-w13442`.  Base after the d2 `m_bCinematicBreak` sink: raw **51,713**,
`.text` 72,848.

A fresh full `d=3` rotation sweep (1,538 candidates, exact NDK r18b `-Oz`
compile + gold link, 4 workers) found two size-exact `-1` cells.  One is the
known read-after-write `up 887,890` (`setPosition(m_tempVec)` before the
`m_tempVec` producer).  The other is landed here.

## Landed change

Source lines 2668-2671 (mission state-6 setup block):

```cpp
                     m_pCamera->setLookAtCam(true);
-                    m_pHud->visible = 0;
                     m_tempVec = Vector{-28855.0f, -30075.0f, 63820.0f} / 1.7f;
                     m_pCamera->setPosition(m_tempVec);
                     Vector facing = m_pGeometry6->getPosition() - m_tempVec;
+                    m_pHud->visible = 0;
```

The moved statement is a `Hud` byte store; it crosses only the `m_tempVec`
producer, the camera `setPosition` reader and the `Vector facing` subtraction,
none of which touch the `Hud`.  Semantics-preserving.

Measured alone on the real build:

```
raw        51,713 -> 51,712  (-1)
size       57,232 / 57,232   (exact)
.text      72,848 -> 72,847  (-1)
.ARM.extab 597 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Note on the original block

The original state-6 block (`0x14156a`) emits
`m_pGeometry6->setPosition({12487,-11451,5958})`, `setLookAtCam(true)`,
`setTarget(m_pGeometry6)`, the `m_tempVec` division, `setPosition(m_tempVec)`
and the `facing` subtraction, but has **no** `m_pHud->visible = 0` store in
this block (dead-store eliminated in the original).  Our source keeps the
store; sinking it past the `facing` producer is the measured raw optimum.
