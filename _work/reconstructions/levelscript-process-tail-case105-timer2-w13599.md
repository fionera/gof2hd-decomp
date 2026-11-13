# LevelScript::process — wave 191b / w13599 pass 3: case-105 timer before dir read

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13599`, branch
`wave191b-w13599`, base after passes 1-2 (raw **51,520**, `.text` 71,203,
`.rel.plt` md5 `289b2a06…`).

## Landed change

case-105 supernova-expansion block (source ~3918).  Swap the local direction
read and the timer store:

```cpp
                         m_pCamera->setTarget(player->geometry);
+                        m_nScriptTimerA = 0;
                         Vector dir = player->geometry->getDirection();
-                        m_nScriptTimerA = 0;
                         player->geometry->setDirection(-dir, Vector{0.0f, 1.0f, 0.0f});
```

`m_nScriptTimerA` is a plain member store; `Vector dir = ...getDirection()`
reads only the player geometry.  No statement between them reads the timer.
Semantics preserving.

Measured alone on the real build:

```
raw        51,520 -> 51,519  (-1)
size       57,232 / 57,232   (exact)
.text      71,203 -> 71,202  (-1)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

## Rejected in the same d1 rerun

* `L4855<->L4856` (51,519): swaps `m_tempVec = getPosition()` with the `+=`
  offset (dead add).
* `L4964<->L4965` (51,519): same dead-add shape.
