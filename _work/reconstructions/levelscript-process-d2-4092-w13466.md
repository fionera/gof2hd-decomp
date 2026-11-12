# LevelScript::process — wave 191 / w13466: guard wake after placement (d2)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466
landmark-3 route hoist: raw **51,665**, `.text` 71,797.

## Landed change

Guard spawn loop (`for (int i = 6; i < 8; ++i)`), source lines 4091-4094:

```cpp
-                                (*enemies)[i]->awake();
                                 KIPlayer *guard = (*enemies)[i];
                                 guard->setPosition(player->getPosition()
                                                    + Vector{(float) xOff, (float) yOff, 25000.0f});
+                                (*enemies)[i]->awake();
```

`KIPlayer::awake` only sets `state = 1` and `player->setActive(true)`;
`KIPlayer::setPosition` writes the geometry transform and the player's
transform matrix.  The two touch disjoint fields and neither reads the
other's result, so the reorder is semantics-preserving.

Measured alone on the real build:

```
raw        51,665 -> 51,663  (-2)
aligned    22,038 -> 22,056
size       57,232 / 57,232   (exact)
.text      71,797 -> 71,795  (-2)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
