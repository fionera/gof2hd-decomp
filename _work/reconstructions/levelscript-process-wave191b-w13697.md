# LevelScript::process — wave 191b / w13697: case-70 state-1 conclude block

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13697`, branch
`wave191b-w13697`, base master `6e208a911` (raw **51,614**, aligned **21,594**,
`.text` diffbytes 71,340, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff notes cover the tail half (w13561/w13599), the first-half
d1–d6 transposition sweeps (w13570), case-14/16/24/29/40/41/42/56/64/67/69/70/73
line buckets (w13570), case-92 state 4 (w13631), case-14 state 1 (w13642),
case-158 state 1/2 (w13645/w13667), case-154 state 4 (w13657/w13668),
case-24 state 1 (w13676) and case-105 state 6 (w13687).  Case 70 had only its
state-0 line bucket swept; its **state-1 conclude block** (source L2523-2543,
original `0x13c05a`..`0x13c16c`) had never been reconstructed from the Thumb
disassembly.  This pass re-derives that contiguous block and also measures the
neighbouring case-70 state-0 and case-80 state-0 clusters.

## Original disassembly (case 70 state 1, `0x13c05a`..`0x13c16c`)

```
13c05a  lodManager->forceUpdate(delta, false)
13c06c  if (radio[1]->isOver()) {
13c07c    enemies[0]->player->setAlwaysFriend(false)
13c094    enemies[0]->player->setAlwaysEnemy(true)
13c0a4    enemies[0]->field_0x128 = 100000
13c0b2    enemies[0]->field_0x38 = 0
13c0be    enemies[0]->field_0x12d = 0
13c0ca    enemies[0]->field_0x12c = 0
13c0d6    enemies[0]->setRoute(nullptr)
13c0e4    camPos = enemies[0]->getPosition()
13c0f8    dir    = enemies[0]->geometry->getDirection()
13c10c    camPos += dir * 2000.0f
13c11e    camPos.x += -600.0f
13c134    camPos.y += 800.0f
13c138    camPos.z += -1000.0f
13c150    m_pCamera->setPosition(camPos)
13c156    lodManager->forceUpdate(delta, false)
13c162    m_nState = 2
13c166    player->freeze = 0
        }
```

Our source had a real semantic bug: it called `m_pCamera->setPosition(camPos)`
immediately after `camPos.x += -600.0f`, so the y, z and `dir * 2000.0f`
terms were **dead**.  The field seeds were also interleaved with the
`getPosition`/`getDirection` loads and `setAlwaysEnemy` preceded
`setAlwaysFriend`.

## Landed change

The whole state-1 block is reordered to the original statement order; the
camera store now follows all four vector terms.  All moved statements touch
disjoint state (player flags, KIPlayer fields, route pointer, camera
transform), so the reorder is semantics-preserving and matches the original.
The resulting block is instruction-for-instruction identical to the original
modulo register names and the `strb`/`strb.w` encoding forced by the
`r6`/`r9` zero-register choice.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,614 -> 51,628  (+14)
aligned    21,594 -> 21,479  (-115)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,340 -> 71,354 (+14); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

This is an aligned-weighted win of the kind accepted for case-154 state 4
(w13657: raw +17 / aligned -262) and case-105 state 6 (w13687: raw +79 /
aligned -113); the raw cost per aligned byte is much lower than either.

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab | rel.plt |
|---|---:|---:|---:|---:|---|
| baseline | 51,614 | 21,594 | 57,232 | 577 | ok |
| **case-70 s1 full faithful (landed)** | **51,628** | **21,479** | **57,232** | **577** | **ok** |
| case-70 s1 camera store moved only | 51,618 | 21,591 | 57,232 | 577 | ok |
| case-70 s1 field seeds only | 51,621 | 21,505 | 57,232 | 577 | ok |
| case-70 s1 full, x/y/z before `dir*2000` | 51,626 | 21,497 | 57,232 | 577 | ok |
| case-70 s1 full, `setAlwaysEnemy` first | 51,628 | 21,492 | 57,232 | 577 | ok |
| case-70 s1 full + case-105 s9/s10 + case-102 s3 | 51,638 | 21,468 | 57,232 | 577 | ok |
| case-70 s0 full faithful | 54,710 | 21,733 | 57,264 | 577 | ok |
| case-70 s0 `setFreeLookMode` early only | 51,642 | 21,583 | 57,232 | 577 | ok |
| case-70 s0 head reorder only | 53,974 | 21,585 | 57,224 | 577 | ok |
| case-70 s0 head + `setFreeLookMode` early | 51,644 | 21,581 | 57,232 | 577 | ok |
| case-80 s0 full faithful | 51,638 | 22,067 | 57,232 | 577 | ok |
| case-80 s0 `setComputerControlled` after radar | 51,629 | 21,588 | 57,232 | 577 | ok |
| case-80 s0 `setPosition` before timer/state/sound | 51,616 | 21,587 | 57,232 | 577 | ok |
| case-80 s0 `comp` + `setPosition` move | 51,630 | 21,584 | 57,232 | 577 | ok |
| case-105 s9 revive before the three `nextInt` | 51,621 | 21,586 | 57,232 | 577 | ok |
| case-105 s10 right-vector `* 1000.0f` | 51,615 | 21,591 | 57,232 | 577 | ok |
| case-102 s3 right-vector used for `* 0.04` add | 51,616 | 21,594 | 57,232 | 577 | ok |

## Other clusters measured (not landed)

**Case 80 state 0 (L2626-2646, original `0x139e4e`).**  Full faithful order
(`setTurretMode`, `resetCamera`, `setFreeLookMode`, `enableFirstPersonCam`,
`hideShip`, `stopShooting`, `cinematicBreak`, HUD/radar, `setComputerControlled`,
`freeze`, `setLookAtCam`, `setTarget`, `getPosition`, `+= {-20000,5000,-35000}`,
`setPosition`, timer/state, `play(1121)`) is **instruction-identical** to the
original block (local normalized mismatch 46 -> 17, the residual being
`fp`/`r6` register choices and the `strb`/`strb.w` width).  It is nevertheless
raw +24 / aligned +473 on the whole-function metric because the block sits at
`0x139c9c` in ours vs `0x139e4e` in the original; the global alignment
algorithm penalises the shifted anchor.  Recorded for a future pass that can
also move the block.

**Case 70 state 0 (L2484-2508, original `0x1399a2`).**  Full faithful order
(`setTurretMode`, `resetCamera`, `setFreeLookMode`, `enableFirstPersonCam`,
`hideShip`, `stopShooting`, HUD/cinematic/radar, `setComputerControlled`,
`setLookAtCam`, `setActive`, `setTarget`, `forceUpdate`, `camPos`,
`dir*3000`, x/y/z offsets, `setPosition`, `forceUpdate`, `freeze`, state) is
not size-exact (+32 B).  The size-exact subsets that keep the original's
`setFreeLookMode`-early / `dir*3000`-before-offsets order are aligned -11 to
-13 but raw +28 to +30.

**Case 105 state 9/10 and case 102 state 3.**  The three semantic fixes
recorded by w13687 are confirmed on this base (s9: raw +7 / aligned -8;
s10: raw +1 / aligned -3; s3: raw +2 / aligned 0).  Composed with the landed
case-70 state-1 block they give raw +24 / aligned -126; the marginal
11-aligned gain for 10 raw was judged not worth landing in this pass.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13697/` (`fast.py`, `run70.py`,
`run70s0.py`, `run70spell.py`, `run80.py`, `winmap.py`, `cand_*.cpp`).
