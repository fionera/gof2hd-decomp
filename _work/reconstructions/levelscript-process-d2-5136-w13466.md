# LevelScript::process — wave 191 / w13466: mission-158 explosion-exit animation sink (d2)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466 d1
mission-81 change: raw **51,675**, aligned 22,003, `.text` 71,807.

## Landed change

Mission-158 explosion state (`m_nState == 10`, `m_nScriptTimerA >= 3001`),
source lines 5135-5138:

```cpp
-                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
-                                    ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                             m_nScriptTimerA = 0;
                             m_pRadar->lockedEnemy = nullptr;
+                            ((AbyssEngine::Transform *) Globals::Canvas->TransformGetTransform(m_pGeometry1->transform))
+                                    ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                             m_nState++;
```

`Transform::SetAnimationState(mode 1)` only stores `flags = 1` (it does not
touch `animating`); `m_nScriptTimerA` is a member int and
`m_pRadar->lockedEnemy` a radar pointer, all disjoint from the geometry
transform.  The reorder is semantics-preserving.

Measured alone on the real build:

```
raw        51,675 -> 51,669  (-6)
aligned    22,003 -> 22,014
size       57,232 / 57,232   (exact)
.text      71,807 -> 71,801  (-6)
.ARM.extab 156,312 B, content diff 575 (unchanged; size/base exact @0x1a3b78)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
