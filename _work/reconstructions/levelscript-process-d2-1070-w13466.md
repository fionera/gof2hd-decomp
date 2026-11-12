# LevelScript::process — wave 191 / w13466: mission-81 case-1 camera target sink (d2)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466
mission-158 d2 change: raw **51,669**, `.text` 71,801.

## Landed change

Mission-81 alien-orbit arrival, `case 1` (station approach), source lines
1069-1074:

```cpp
-                m_pCamera->setTarget(((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry);
                 AbyssEngine::Transform *t = (AbyssEngine::Transform *)
                         Globals::Canvas->TransformGetTransform(
                                 ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry->transform);
                 t->SetAnimationState((AbyssEngine::AnimationMode) 1, nullptr);
+                m_pCamera->setTarget(((PlayerStation *) (*m_pLevel->getLandmarks())[0])->rootGeometry);
                 m_nState = 2;
```

`TargetFollowCamera::setTarget` writes only the camera target; the moved call
crosses a pure `landmark[0]->rootGeometry->transform` read and a
`Transform::SetAnimationState(mode 1)` store of the transform `flags` field.
The camera and the landmark transform are disjoint objects, so the reorder is
semantics-preserving.

Measured alone on the real build:

```
raw        51,669 -> 51,667  (-2)
aligned    22,014 -> 22,029
size       57,232 / 57,232   (exact)
.text      71,801 -> 71,799  (-2)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
