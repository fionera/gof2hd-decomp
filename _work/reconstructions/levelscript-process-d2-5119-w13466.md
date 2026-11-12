# LevelScript::process — wave 191 / w13466: explosion start after sound/rumble (d2)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466
rocket-control d2 change: raw **51,661**, `.text` 71,793.

## Landed change

Mission-158 explosion window (`m_nState == 10`, 9000 ms crossing),
source lines 5119-5123:

```cpp
                         m_pExplosion->setScaling(10.0f);
-                        m_pExplosion->start((*enemies)[stationIndex]->getPosition()
-                                            - m_pLevel->getStarSystem()->getLightDirection() * 3000.0f,
-                                            Vector{0.0f, 0.0f, 0.0f});
                         Globals::sound->play(2243, nullptr, nullptr, 0.0f);
                         m_pCamera->setRumblePercentage(100.0f, 30);
+                        m_pExplosion->start((*enemies)[stationIndex]->getPosition()
+                                            - m_pLevel->getStarSystem()->getLightDirection() * 3000.0f,
+                                            Vector{0.0f, 0.0f, 0.0f});
```

`m_pExplosion->start` writes the explosion system; `Globals::sound->play` and
`m_pCamera->setRumblePercentage` write the sound engine and camera rumble.
The explosion position expression is a pure read of the enemy/star-system
state, none of which the two intervening calls modify.  Semantics-preserving.

Measured alone on the real build:

```
raw        51,661 -> 51,660  (-1)
aligned    22,063 -> 22,079
size       57,232 / 57,232   (exact)
.text      71,793 -> 71,792  (-1)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
