# LevelScript::process — wave 190 / w13442: state-6 camera `setFixed` hoist

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13442`, branch
`wave190-w13442`.  Base after the d3 Hud store sink: raw **51,712**, `.text`
72,847.

## Landed change

Source lines 4206-4209 (mission state-6 camera-local block):

```cpp
                     *scratch = player->geometry->getRightVector();
+                    m_pCamera->setFixed(true);
                     *tempVec += *scratch * ((float) delta * 0.03f);
                     AbyssEngine::AEMath::MatrixSetTranslation(local, *tempVec);
                     *tempVec += *scratch * ((float) delta * 0.01f);
-                    m_pCamera->setFixed(true);
                     m_pCamera->setLocal(local);
```

The moved statement is a `TargetFollowCamera` flag setter; it crosses only the
local `m_tempVec`/`m_dirVec` arithmetic and `MatrixSetTranslation`, which touch
the stack matrix/vector, never the camera.  Semantics-preserving.  The original
emits `setFixed` before `MatrixSetTranslation` (`0x138bac` vs `0x138bb6`), so
hoisting it is also direction-correct.

Measured alone on the real build:

```
raw        51,712 -> 51,710  (-2)
size       57,232 / 57,232   (exact)
.text      72,847 -> 72,845  (-2)
.ARM.extab 597 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical, 29,392 B)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).
