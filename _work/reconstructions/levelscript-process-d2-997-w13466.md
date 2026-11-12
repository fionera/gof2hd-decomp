# LevelScript::process — wave 191 / w13466: mission-81 arrival state/timer before anim (d2)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466
explosion d2 change: raw **51,660**, `.text` 71,792.

## Landed change

Mission-81 alien-orbit arrival, `case 0` (`m_nScriptTime >= 10001` block),
source lines 997-999, composing with the earlier w13466 d1 change at the same
site:

```cpp
-                    ((AbyssEngine::Transform *) canvas983->TransformGetTransform(m_pGeometry5->transform))
-                            ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                     m_nState = 1;
                     m_nScriptTimerA = 0;
+                    ((AbyssEngine::Transform *) canvas983->TransformGetTransform(m_pGeometry5->transform))
+                            ->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
                     Globals::sound->play(160, nullptr, nullptr, 0.0f);
                     return 1;
```

`Transform::SetAnimationState(mode 1)` only stores `flags = 1`; `m_nState` and
`m_nScriptTimerA` are member ints on the LevelScript, disjoint from the
geometry transform.  Semantics-preserving.

Measured alone on the real build:

```
raw        51,660 -> 51,657  (-3)
aligned    22,079 (unchanged)
size       57,232 / 57,232   (exact)
.text      71,792 -> 71,789  (-3)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
