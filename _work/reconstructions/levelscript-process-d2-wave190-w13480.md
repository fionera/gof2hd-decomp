# LevelScript::process — wave 190 / w13480: d=2 rotation, fighter wake setters

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13480`, branch
`wave190-w13480`, base after the d=1 `setActive`/`camPos` swap: raw **51,690**,
`.text` 71,860.

## Landed change

Source lines 5057-5059 (mission-158 state-9 fighter wake-up):

```cpp
                     if (m_nScriptTimerB < 9000 && m_nScriptTimerB + delta >= 9000) {
-                        fighter->setPosition(*m_pCamera->getPosition() + Vector{4000.0f, -600.0f, -2000.0f});
                         fighter->setVisible(true);
                         fighter->setActive(true);
+                        fighter->setPosition(*m_pCamera->getPosition() + Vector{4000.0f, -600.0f, -2000.0f});
                     }
```

`KIPlayer::setVisible`, `setActive` and `setPosition` write disjoint fields of
the same fighter (visibility flag, active flag, geometry/transform position);
none reads what another writes, and `m_pCamera->getPosition()` is a pure read.
The rotation is semantics-preserving.

Measured alone on the real build:

```
raw        51,690 -> 51,688  (-2)
aligned    21,967 -> 21,994
size       57,232 / 57,232   (exact)
.text      71,860 -> 71,858  (-2)
.ARM.extab 575 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS.

## Sweep notes

Full `d=2` down-rotation sweep on the current body (1,408 candidates, exact
NDK r18b `-Oz` compile + gold link, 4 workers).  Two cells beat the base:

| move | source lines | raw | verdict |
|---|---|---:|---|
| `down 4964,4966` | 4964-4966 | 51,689 | moves `m_tempVec +=` below both `setPosition(m_tempVec)` calls; read-after-write, not landed |
| `down 5057,5059` | 5057-5059 | 51,688 | landed |

The `down 4964,4966` cell is semantically wrong: `chaser->setPosition` and
`m_pCamera->setPosition` would both see the un-offset `m_tempVec`.
