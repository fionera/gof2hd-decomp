# LevelScript::process — wave 190 / w13442: dead `m_tempVec.z` increment hoist

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13442`, branch
`wave190-w13442`.  Base after the d3 camera hoist: raw **51,710**, `.text`
72,845.

## Landed change

Source lines 2694-2698 (mission state-8 explosion block):

```cpp
                     m_pLevel->field_74->systemSetMatrix(m_pLevel->field_58, &m_pGeometry6->getMatrix());
+                    m_tempVec.z += 1000.0f;
                     m_pLevel->field_74->enableSystemEmit(m_pLevel->field_58, true);
                     m_pLevel->field_74->systemSetMatrix(m_pLevel->field_5c, &m_matrix);
                     AbyssEngine::AEMath::MatrixSetTranslation(m_matrix, m_tempVec.x, m_tempVec.y, m_tempVec.z);
                     m_tempVec.x += 4500.0f;
-                    m_tempVec.z += 1000.0f;
                     m_tempVec = m_pGeometry6->getPosition();
```

Both `m_tempVec` increments are dead: the vector is unconditionally
overwritten by `m_tempVec = m_pGeometry6->getPosition()` two statements later,
with no intervening read of the incremented components (the
`MatrixSetTranslation` read is *before* both increments in the original source
order).  Hoisting the `z` increment is therefore semantics-preserving.

Measured alone on the real build:

```
raw        51,710 -> 51,708  (-2)
size       57,232 / 57,232   (exact)
.text      72,845 -> 72,843  (-2)
.ARM.extab 597 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
