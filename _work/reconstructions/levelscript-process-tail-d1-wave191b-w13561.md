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

### 2. case-92 teardown camera pair (source ~3086)

```cpp
-                        m_pCamera->setTarget(player->geometry);
-                        m_pCamera->setLookAtCam(false);
+                        m_pCamera->setLookAtCam(false);
+                        m_pCamera->setTarget(player->geometry);
```

Both are independent camera setters (target pointer vs. look-at flag); no
statement between them reads what the other writes.  (The original emits
`setTarget` before `setLookAtCam` at this site, so this is a codegen-coincidence
win of the kind the wave-190/191 notes accept: the gate is the same-address
byte diff.)

```
raw        51,539 -> 51,538  (-1)
size       57,232 / 57,232   (exact)
.text      71,239 -> 71,238  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```

## d2/d3/d4 sweeps

On the resulting 51,538 body the distance-2/3/4 transposition sweeps were run
over the same tail range (828 / 666 / 590 candidates).  All measured wins are
listed; the producer/consumer ones are rejected.

| kind | lines | raw | safe? |
|---|---|---:|---|
| d2 | 3235/3237 | 51,537 | no — swaps `m_dirVec = getRightVector()` past `m_tempVec += m_dirVec*1200`, second add reads UP |
| d2 | 4964/4966 | 51,537 | no — `chaser->setPosition(m_tempVec+…)` then assigns `m_tempVec`, first uses stale vector |
| d3 | 3025/3028 | 51,537 | no — swaps getDirection past the `m_tempVec += m_dirVec*1.7` add |
| d3 | 4614/4617 | 51,537 | no — `setPosition(m_tempVec)` before `m_tempVec` is rebuilt |
| d4 | 3460/3464 | 51,537 | no — swaps getDirection past the two `m_tempVec += m_dirVec*step` adds |
| d4 | 4026/4030 | 51,537 | **landed** |

### 3. case-125 teardown flag group (source ~4026)

```cpp
                         player->hideShipForFirstPersonCameraView(false);
                         m_bCinematicBreak = 1;
+                        m_pRadar->enabled = 0;
                         m_pHud->visible = 0;
                         player->setFreeLookMode(false);
                         m_pCamera->enableFirstPersonCam(false);
+                        player->stopShooting(0);
-                        m_pRadar->enabled = 0;
```

`stopShooting(0)` early-returns only on turret-active/dead; none of the
intervening stores touch those.  Independent flag stores otherwise.

```
raw        51,538 -> 51,537  (-1)
size       57,232 / 57,232   (exact)
.text      71,238 -> 71,237  (-1)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS
```
