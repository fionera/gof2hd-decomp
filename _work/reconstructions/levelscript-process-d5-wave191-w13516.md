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
