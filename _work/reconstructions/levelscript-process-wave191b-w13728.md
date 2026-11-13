# LevelScript::process — wave 191b / w13728: five faithful-order blocks

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13728`, branch
`wave191b-w13728`, base master `ae45a4ed6` (raw **51,780**, aligned **21,063**,
`.text` diffbytes 72,300, `.ARM.extab` 564, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

After w13719 landed the case-24/70/92/105 order pass, this pass re-derived five
more contiguous blocks from the Thumb disassembly and moved them into the
original statement order.  Every block is size-neutral; the function stays
57,232 B and `.rel.plt` content is unchanged.

## Original disassembly and landed changes

### mission-0 state 6 (`0x139b70`)

Original: `getPosition` -> `updateEvent3DAttributes(161)` -> `translate` ->
`m_nScriptTimerA += delta` -> rumble.  Our source incremented the timer at the
top of the block.  The timer now accumulates after the camera translate,
immediately before the rumble computation.

### mission-1 intro (`0x137d82`)

Original: `lodManager->forceUpdate(delta, true)` -> rotate -> `getPosition` ->
`setToSleep` -> depth math -> `moveForward`.  Our source had the force-update
after the enemy position sample and `setToSleep` before it; both are now in the
original order.

### mission-0 case 12 (`0x13b7f2`)

Original teardown at `m_nScriptTimerA >= 5001`: `setTarget(player->geometry)`,
hud=1, radar=1, `m_bCinematicBreak=0`, `setLookAtCam(false)`,
`setComputerControlled(false)`, `setVulnerable(true)`, `freeze=0`,
`setVisible(true)`, `forceUpdate`, then reads `m_bCinematicBreak` and returns
it.  Our source had `setComputerControlled` first and the camera target in the
middle.  The dead `m_nState = 13` store after the `return` is kept: removing
it grows the function.

### mission-0 case 11 (`0x13bd2c`)

Original: `setTarget(enemies[count-10])`, `m_tempVec = getPosition()`,
`x += 2000`, `y += 500`, `z += -20000`, `setPosition(m_tempVec)`,
`forceUpdate(30, false)`, `m_nState = 12`, `m_nScriptTimerA = 0`.  Our source
applied only the x offset (y/z were dead) and had the state/timer stores and
force-update in a different order.

### case-105 state 10 (`0x142bf2`)

Original rocket block: `setRocketControl`, `killLiberator`,
`setRumblePercentage`; then `setFreeLookMode`, `enableFirstPersonCam`,
`hideShip`, `setVulnerable(false)`, hud=0, radar=0, `stopShooting`,
`m_bCinematicBreak=1`, `setLookAtCam(true)`, `setComputerControlled(true)`,
`setPosition(...)`.  Our source had the rocket block reversed and
`setFreeLookMode` / `setVulnerable` / `setLookAtCam` out of order.

## Measured result

Per-edit deltas (baseline raw 51,780 / aligned 21,063):

| edit | raw | aligned |
|---|---:|---:|
| mission-0 s6 timer after translate | 51,788 | 21,055 |
| mission-1 intro faithful | 51,784 | 21,054 |
| case-12 reorder (keep dead store) | 51,785 | 21,053 |
| case-11 faithful + full offsets | 51,795 | 21,020 |
| case-105 s10 teardown | 51,818 | 21,042 |
| **all five** | **51,850** | **20,972** |

The combined raw (+70) exceeds the sum of the individual deltas (+38) because
of cross-block register-allocation interaction; the aligned count is a genuine
-91 and the function stays size-exact.

```
raw        51,780 -> 51,850  (+70)
aligned    21,063 -> 20,972  (-91)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,300 -> 72,370 (+70); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 564 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Investigated, not applied (measured)

| candidate | raw | aligned |
|---|---:|---:|
| case-154 state 0 full faithful order | 51,823 | 21,072 |
| case-105 s7 `setFixed` before `MatrixSetTranslation` | 51,789 | 21,086 |
| case-10 `translate` before timer | 51,783 | 21,063 |
| case-10 `m_nState=11` before `timerA=0` | 51,781 | 21,063 |
| case-154 `enemies[1]->field_0x70=1` before `m_bCinematicBreak` | 51,782 | 21,063 |

`case-12 m_nState = 13` made live (non-dead store) grows the function to
57,240 B (+8), and `return 0` grows it to 57,296 B (+64); both need
compensation and were not landed.
