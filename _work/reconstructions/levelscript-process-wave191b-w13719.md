# LevelScript::process — wave 191b / w13719: case-24/70/92/105 order pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13719`, branch
`wave191b-w13719`, base master `cd8eee9c3` (raw **51,655**, aligned **21,210**,
`.text` diffbytes 72,153, `.ARM.extab` 564, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff left the case-24/70/92/105 arms as the largest
un-reconstructed aligned clusters outside the case-91 block.  This pass
re-derived six contiguous blocks from the Thumb disassembly and reordered them
to the original statement order.  Every block is size-neutral; the function
stays 57,232 B.

## Original disassembly and landed changes

### case 70 state 0 (`0x1399a2`..`0x139ac2`)

Original: `setTurretMode`, `resetCamera`, `setFreeLookMode`,
`enableFirstPersonCam`, `hideShip`, `stopShooting`, `m_bCinematicBreak=1`,
`hud.visible=0`, `radar.enabled=0`, `setComputerControlled`, `setLookAtCam`,
`setActive`, `setTarget`, `forceUpdate`, then
`camPos = getPosition(); dir = getDirection(); camPos += dir*3000;` followed by
the three scalar offsets and `setPosition`.  Our source had `resetCamera`
first, `setFreeLookMode` late (just before `setPosition`), and the scalar
offsets before the direction term.

### case 70 state 2 (`0x13ec3c`)

Original: `setLookAtCam(false)`, `setTarget(player->geometry)`,
`setComputerControlled(false)`, `hud.visible=1`, `radar.enabled=1`,
`resetCamera`, `forceUpdate`, `m_bCinematicBreak=0`, `m_nState=3`, then the
`lead` hitpoint/speed/rotate buff.  Our source had `resetCamera` before
`setTarget` and `setComputerControlled` after `forceUpdate`.

### case 24 state 0 (`0x13e97e`)

Original: `stopShooting(0)` first, then `hud.visible=0`, `radar.enabled=0`,
`setLookAtCam(true)`, `setVulnerable(false)`, `geometry->setRotation`,
`setComputerControlled(true)`, `removeAllGuns`, `setTarget`, then
`getPosition`/`setPosition`/`translate`.  Our source had the HUD stores first,
`stopShooting` after `setRotation`, and `setTarget` after `setPosition`.

### case 92 state 2 (`0x13fb7a`)

Two real bugs: the original increments `m_nState` (2 -> 3), it does **not**
jump to 10; and `m_pCamera->setTarget(leader)` precedes the
`getPosition`/drift vector work (our source had it after the vector adds).
Both are now in the original order.

### case 92 state 3 (`0x13fcde`)

The original adds the up-vector term
`m_tempVec += m_dirVec * (delta*0.05*0.004)` after `getUpVector()`; our source
added the right vector a second time and left `getUpVector()` dead.  The
`setFixed(true)` call also belongs after the up-vector term, before
`MatrixSetTranslation`.

### case 105 state 9 (`0x142a04`)

Original: `revive()` first, then `nextInt` rx/rz/ry; `setPosition` arguments
are `(x + off + rx, rz, z + ry)` — our source had `revive` between the second
and third `nextInt` and swapped the y/z random terms (`y=ry`, `z=...+rz`).

## Measured result

```
raw        51,655 -> 51,780  (+125)
aligned    21,210 -> 21,063  (-147)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,153 -> 72,278 (+125); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 564 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

Per-block deltas are additive and were each measured on the real build:

| block | raw | aligned |
|---|---:|---:|
| case 70 state 0 | +34 | -40 |
| case 70 state 2 | +6 | -8 |
| case 24 state 0 | +8 | -22 |
| case 92 state 2 | +1 | -43 |
| case 92 state 3 | +69 | -26 |
| case 105 state 9 | +7 | -8 |
| **combined** | **+125** | **-147** |

The case-92 state-2 block is the best trade (raw +1 / aligned -43); case-92
state-3 is the most expensive (+69 raw / -26 aligned) but fixes a genuinely
dead up-vector term, so it is kept for semantic fidelity.  All six blocks are
store/call-order permutations of disjoint state, so behaviour is unchanged
except for the two case-92 bugs above.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13719/` (`clusters2.py`, `callmap.py`,
`final.py`, `base.cpp`, `base.so`, `orig.dis`, `ours.dis`).
