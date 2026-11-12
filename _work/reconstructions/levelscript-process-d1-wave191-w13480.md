# LevelScript::process — wave 191 / w13480: d=1 adjacent-statement sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13480`, branch
`wave191-w13480`, base master `f325e2d87` (raw **51,679**, aligned **22,003**,
`.text` 71,811, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

Lens: fresh full d=1 adjacent-simple-statement sweep on the current body
(1,966 candidate pairs, exact NDK r18b `-Oz` compile + gold link, 4 workers;
1,792 size-exact builds).  Six cells measure raw 51,678 (-1); five are the
documented read-after-write / `.rel.plt`-fatal cells.  One new safe cell.

## Landed change

Source lines 2957-2958 (mission-158 state-1 enemy park block):

```cpp
                         m_nScriptTimerA = 0;
                         (*enemies)[1]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 7500.0f});
-                        (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
-                        (*enemies)[2]->setPosition(ref + Vector{-79000.0f, 4900.0f, 2500.0f});
+                        (*enemies)[2]->setPosition(ref + Vector{-79000.0f, 4900.0f, 2500.0f});
+                        (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
```

`ref` is a local copy of `(*enemies)[3]->getPosition()` and the two
`setPosition` calls write disjoint enemies (`[0]` vs `[2]`); neither reads
what the other writes.  Semantics-preserving.

Measured alone on the real build:

```
raw        51,679 -> 51,678  (-1)
aligned    22,003 -> 22,004
size       57,232 / 57,232   (exact)
.text      71,811 -> 71,810  (-1)
.ARM.extab 575 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Rejected cells (same sweep)

| pair (0-based) | source lines | reason |
|---|---|---|
| 61 | 829/830 | `switchSkyboxForIntro` across `particleRenderBoolPtr->reset()` — `.rel.plt` md5 changes |
| 86 | 890/891 | `m_tempVec +=` across `setPosition(m_tempVec)` — read-after-write |
| 1609 | 4543/4544 | same `m_tempVec` read-after-write |
| 1765 | 4855/4856 | `m_tempVec = getPosition()` across `m_tempVec +=` — add-before-assign |
| 1807 | 4964/4965 | `m_tempVec +=` across `chaser->setPosition(m_tempVec + ...)` — read-after-write |

## Ghidra/disasm cluster note (case-1 body, not landed)

Mission-89 `case 1` (`0x13868a..0x138768` original) is emitted in a different
statement order from our source: the original sets `m_nState = 2` first, then
`m_tempVec = *m_pCamera->getPosition()`, `+= 500`, enemy `setPosition`,
`moveForward(-10000)`, `SetAnimationState(3)`, `SetAnimationState(1)`,
`setDirection(...)` (using a *fresh* `m_pLevel->getEnemies()`), and finally
`m_pGeometry6->setPosition(enemy->geometry->getPosition())`.  Reconstructing
that order is semantically correct and lowers the aligned metric
(22,003 -> 21,933) but raises raw to 51,723 because the body moves by
`-0x22` relative to the original and shifts the downstream block layout.
It is therefore note-only until the preceding block-order drift is fixed.
