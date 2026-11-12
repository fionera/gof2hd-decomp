# LevelScript::process — wave 191 / w13466: landmark-3 route hoist (d2)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466
mission-81 d2 change: raw **51,667**, `.text` 71,799.

## Landed change

Landmark-3 escort setup, `ship->shipGroup == 9` loop, source lines 1736-1742:

```cpp
+                            ship->setRoute(route->clone());
                             ship->setPosition(camPos.x, camPos.y - 2000.0f + (float) Globals::rnd->nextInt(4000),
                                               camPos.z - 2000.0f + (float) Globals::rnd->nextInt(4000));
                             ship->geometry->setDirection(
                                     AbyssEngine::AEMath::VectorNormalize((*m_pLevel->getLandmarks())[3]->getPosition()
                                                                          - ship->getPosition()),
                                     Vector{0.0f, 1.0f, 0.0f});
-                            ship->setRoute(route->clone());
```

`KIPlayer::setRoute` only deletes the old route and stores `route` /
`initialRoute`; it reads no position and starts no movement.  `setPosition`
and `geometry->setDirection` write the ship transform.  `route->clone()`
allocates but consumes no RNG, so hoisting it above the two transform writes
is semantics-preserving.

Measured alone on the real build:

```
raw        51,667 -> 51,665  (-2)
aligned    22,029 -> 22,038
size       57,232 / 57,232   (exact)
.text      71,799 -> 71,797  (-2)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
