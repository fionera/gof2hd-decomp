# LevelScript::process — wave 191b / w13731: mission-0/1, mission-89, mission-106, case-154 store order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13731`, branch
`wave191b-w13731`, base master `dc50e1082` (raw **51,956**, aligned **20,871**,
`.text` diffbytes 72,647, `.ARM.extab` 564, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff left the early mission-0/1 intro, the mission-89
supernova intro and the mission-106 cloaked-fighter block as the largest
un-reconstructed clusters outside the mid-game switch.  This pass re-derived
those blocks (plus the case-154 state-0/1 store order) from the Thumb
disassembly and moved them into the original statement order.  Every landed
edit is size-neutral; the function stays 57,232 B and `.rel.plt` is unchanged.

## Original disassembly and landed changes

### mission-0 state 0 (`0x137e9c`)

Original stores `m_nState = 1` *before* `player->setPosition(18000,-12000,
-40000)` and `m_pCamera->setPosition(-12000,2000,-500)`; our source stored it
between the two.  Moved to the top of the block.

### mission-0 state 1/2 (`0x138440`, `0x138482`)

Original state-1 conclude block is `m_nState = 2`, `setTarget(enemies[0])`,
`setPosition(-5000,300,-5000)`; ours ran `setPosition` first.  Original
state-2 body runs the `isPlaying(142)` / `stop` / `play` sound block **before**
`m_pCamera->translate(delta*0.2, 0, delta*2.2)`; ours translated first.
Both reordered.

### mission-89 supernova intro (`0x138574`, `0x138678`)

Original order: `lodManager->forceUpdate(delta, true)` then
`m_pCamera->setTarget(m_pGeometry4)` then `m_bCinematicBreak = 1`; ours had
`setTarget` first.  The state-1 body (`0x138678`) is `m_nState = 2`,
`m_tempVec = m_pCamera->getPosition()`, `y += 500`, `enemies[0]->setPosition`,
`enemies[0]->geometry->moveForward(-10000)`, `SetAnimationState(3)`,
`SetAnimationState(1)`, `m_pGeometry6->setDirection(...)`, and only then
`m_pGeometry6->setPosition(enemies[0]->geometry->getPosition())`.  Our source
had the `setPosition` first, the animation states 1-then-3, and `m_nState`
last.  State 5 (`0x13b0ea`) runs `layout->enableFillScreen(true)` before
`status->nextCampaignMission(true)`; ours had them swapped.

### mission-106 states 2 (`0x13be96`)

Original sets the camera position before `m_nState = 3` / `m_nScriptTimerA = 0`;
ours stored the timer first and the state last.  Reordered.

### case 154 state 0/1 (`0x13a792`, `0x1444a2`)

Original rocket-control branch calls `resetCamera(m_pLevel)` before
`setRocketControl(nullptr,nullptr)`; ours had them swapped.  The state-1
`shipGroup == 9` teardown loop calls `setActive(false)` then
`setVisible(false)`; ours had the reverse.  Both reordered.

## Measured result

Per-edit deltas (baseline raw 51,956 / aligned 20,871):

| edit | raw | aligned |
|---|---:|---:|
| mission-0 s0 `m_nState=1` first | 51,960 | 20,865 |
| mission-0 s1/s2 order | 51,962 | 20,820 |
| case-154 s0/s1 order | 51,964 | 20,820 |
| mission-89 forceUpdate-first | 51,966 | 20,819 |
| mission-89 s1 faithful | 51,997 | 20,757 |
| mission-89 s5 enableFillScreen-first | 51,999 | 20,757 |
| mission-106 s2 order | 52,000 | 20,757 |
| **all landed** | **52,000** | **20,757** |

```
raw        51,956 -> 52,000  (+44)
aligned    20,871 -> 20,757  (-114)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,647 -> 72,691 (+44); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 564 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Investigated, not applied (measured)

| candidate | raw | aligned | note |
|---|---:|---:|---|
| mission-0 s3 full faithful order | 51,966 | 20,820 | raw +2 for 0 aligned |
| mission-0 s4 full faithful order | 51,986 | 20,821 | raw +24 / aligned +1 |
| mission-0 s5 `sound->stop` after update158 | 51,967 | 20,820 | raw +5 / 0 |
| case-154 s0 full faithful order | 52,009 | 20,829 | raw +47 / aligned +9 |
| case-154 s1 full faithful order | 51,998 | 20,849 | raw +36 / aligned +29 |
| case-154 s2 faithful order | 52,459 | 20,591 | size **57,228** (-4), rejected |
| mission-106 s1 faithful order | 52,048 | 20,747 | raw +49 / aligned -10 |
| mission-106 s4 faithful order | 53,717 | 20,754 | raw +3,717 / aligned -3 |
| mission-106 s6 faithful order | 52,006 | 20,756 | raw +6 / aligned -1 |

The mission-106 state-1 reorder is semantically faithful but costs +49 raw for
-10 aligned, so it is left for a raw-weighted pass.  The case-154 state-2
faithful order changes the block size by -4 B and would need compensation.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13731/` (`clusters.py`,
`shapeclusters.py`, `callseq.py`, `srcmap2.py`, `measure.sh`, `good.py`,
`base_LevelScript.cpp`, `orig.dis`, `ours.dis`).
