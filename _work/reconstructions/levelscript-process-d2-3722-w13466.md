# LevelScript::process — wave 191 / w13466: cinematic flag/radar after camera target (d2)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466 intro
park d1 change: raw **51,662**, `.text` 71,794.

## Landed change

Rocket-control exit block, source lines 3722-3725:

```cpp
-                        m_bCinematicBreak = 1; // cinematicBreak_ high byte @0x11 = 1
-                        m_pRadar->enabled = 0;
                         AEGeometry *ref2 = (*enemies)[2]->geometry;
                         m_pCamera->setTarget((*enemies)[2]->geometry);
+                        m_bCinematicBreak = 1; // cinematicBreak_ high byte @0x11 = 1
+                        m_pRadar->enabled = 0;
```

`ref2` is a pure read of `(*enemies)[2]->geometry`; `setTarget` writes the
camera target.  `m_bCinematicBreak` is a flag byte and `m_pRadar->enabled` a
radar byte, both disjoint from the camera and the enemy geometry.  The reorder
is semantics-preserving.

Measured alone on the real build:

```
raw        51,662 -> 51,661  (-1)
aligned    22,056 -> 22,063
size       57,232 / 57,232   (exact)
.text      71,794 -> 71,793  (-1)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
