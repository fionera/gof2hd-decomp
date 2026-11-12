# LevelScript::process — wave 191 / w13466: intro ship 2/0 park order (d1)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Base after the w13466 guard
wake d2 change: raw **51,663**, `.text` 71,795.

## Landed change

Mission-0 state-1 intro ship parking, source lines 2956-2957:

```cpp
-                        (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
                         (*enemies)[2]->setPosition(ref + Vector{-79000.0f, 4900.0f, 2500.0f});
+                        (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
```

The two calls write different `KIPlayer` objects and both only read the local
`ref` vector, so their order is unobservable.  Semantics-preserving.

Measured alone on the real build:

```
raw        51,663 -> 51,662  (-1)
aligned    22,056 (unchanged)
size       57,232 / 57,232   (exact)
.text      71,795 -> 71,794  (-1)
.ARM.extab 156,312 B, content diff 575 (unchanged)
.rel.plt   29,392 B, byte-identical
.ARM.exidx byte-identical
```
