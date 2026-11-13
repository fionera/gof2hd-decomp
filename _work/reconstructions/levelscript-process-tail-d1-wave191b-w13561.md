# LevelScript::process — wave 191b / w13561: tail d1/d2/d3/d4 statement sweeps

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13561`, branch
`wave191b-w13561`, base master `4a865a92e` (raw **51,540**, `.text` 71,240,
`.ARM.extab` 576, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

## Lens

The three handoff notes (`levelscript-process-d5-wave191-w13516`,
`cold-blocks-wave191b-w13517`, `cold-blocks-wave191b-w13524`) cover the tail's
cross-control-boundary moves, d5/d6 rotations and distance-3/4 transpositions
on older bases.  This pass re-runs the *plain* distance-1/2/3/4 transposition
sweeps on the merged wave-191b base, restricted to the tail half (source lines
2600-5200: cases 80/91/92/102/105/125/131/135/139/142/144/145/154/157/158).

Harness: `scratch/w13561/{fast,sweep,lines}.py` compiles one candidate with the
exact NDK r18b `-Oz` command from `compile_commands.json` and gold-links it with
the checked-in object list (LevelScript object swapped in place), then measures
same-address diff bytes/size/`.rel.plt`.  Four candidates run concurrently.

## Landed changes

### 1. case-157 state-9 flash block (source ~5065)

```cpp
                     if (messages[13]->isOver() && m_pExplosion == nullptr) {
+                        m_nFlashCountdown = 300;
                         m_pExplosion = new Explosion(0);
-                        m_nFlashCountdown = 300;
                         m_nScriptTimerA = 0;
```

`m_nFlashCountdown` is a plain member store; `new Explosion(0)` reads nothing
from `this`.  Semantics preserving.

```
raw        51,540 -> 51,539  (-1)
size       57,232 / 57,232   (exact)
.text      71,240 -> 71,239  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```
