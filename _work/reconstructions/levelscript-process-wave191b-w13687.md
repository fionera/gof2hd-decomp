# LevelScript::process — wave 191b / w13687: case-105 state-6 teardown + camera block

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13687`, branch
`wave191b-w13687`, base master `b3fd7e6a2` (raw **51,535**, aligned **21,707**,
`.text` diffbytes 71,270, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff notes cover the tail half (w13561/w13599), the first-half
d1–d6 transposition sweeps (w13570), case-14/16/24/29/40/41/42/56/64/67/69/70/73
line buckets (w13570), case-92 state 4 (w13631), case-14 state 1 (w13642),
case-158 state 1/2 (w13645/w13667), case-154 state 4 (w13657/w13668) and
case-24 state 1 (w13676).  Case 105 had only been transposition-swept and
timer-ordered (w13599); its **state-6 teardown + camera block** (source
L3712-3742, original `0x142582`..`0x1426c0`) had never been reconstructed from
the Thumb disassembly.  This pass re-derives that contiguous block.

## Original disassembly (case 105 state 6, `0x142582`..`0x1426c0`)

```
142582  player->setTurretMode(false)
14258e  player->setFreeze(true)
14259a  player->setVisible(false)
1425a2  resetCamera(m_pLevel)
1425ac  if (player->isInRocketControl()) {
1425b4      player->setRocketControl(nullptr, nullptr)
1425be      player->killLiberator()
1425c4      m_pCamera->setRumblePercentage(0.0f, 0)
        }
1425ce  player->setFreeLookMode(false)
1425d6  m_pCamera->setLookAtCam(true)
1425de  m_pCamera->enableFirstPersonCam(false)
1425e6  player->hideShipForFirstPersonCameraView(false)
1425ee  player->stopShooting(0)
1425f6  m_bCinematicBreak = 1
1425fe  m_pHud->visible = 0
142604  m_pRadar->enabled = 0
142608  ref2 = (*enemies)[2]->geometry
142614  m_pCamera->setTarget(ref2)
142620  m_tempVec = ref2->getPosition()
14263c  m_dirVec = ref2->getDirection()
142656  m_tempVec += m_dirVec * 6800.0f
14266e  m_dirVec = ref2->getRightVector()
142682  m_tempVec += m_dirVec * 1200.0f
142692  m_dirVec = ref2->getUpVector()
1426a4  m_tempVec += m_dirVec * 400.0f
1426ac  m_pCamera->setPosition(m_tempVec)
1426b6  m_nScriptTimerB = 0; m_nScriptTimerA = 0   (one 16-byte vst1.64)
1426c0  m_nState++ (via 0x144e12)
```

Our source had four real differences:

1. **Missing `resetCamera(m_pLevel)`** after `setVisible(false)`.
2. An **extra `m_pCamera->setTarget(player->geometry)`** before `setTurretMode`
   that the original does not emit at this site.
3. The **camera position was set before the up-vector `* 400.0f` add**, so the
   last term was dead and the camera was positioned without it.
4. The rocket-control block, the free-look/look-at/first-person/hide/stop group,
   and the hud/radar/stop/cinematic group were in a different statement order.

## Landed change

The whole block is reordered to the original statement order; `resetCamera` is
added, the extra `setTarget(player->geometry)` is removed, and the up-vector
term is added before `setPosition`.  All moved statements touch disjoint state
(camera target/transform, player control/visibility/speed flags, HUD/radar
bytes, LOD manager, cinematic flag), so the reorder is semantics-preserving and
matches the original.  The resulting block is instruction-for-instruction
identical to the original modulo register names.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,535 -> 51,614  (+79)
aligned    21,707 -> 21,594  (-113)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,270 -> 71,349 (+79); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

This is an aligned-weighted win of the kind accepted for case-154 state 4
(w13657: raw +17 / aligned -262) and case-158 state 2 (w13667: raw +10 /
aligned -56).  The block is now instruction-identical to the original; the raw
cost is the block's +0x10 address shift (clang picks high registers for four
stack temporaries where the original uses low ones, so four `add` encodings
grow 2 -> 4 B), not a statement-order difference.

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab | rel.plt |
|---|---:|---:|---:|---:|---|
| baseline | 51,535 | 21,707 | 57,232 | 577 | ok |
| **case-105 s6 full faithful (landed)** | **51,614** | **21,594** | **57,232** | **577** | **ok** |
| case-105 s6 teardown reorder only | 52,707 | 21,609 | 57,256 | 577 | ok |
| case-105 s6 teardown + rocket + camera (keep free-look group order) | 52,562 | 21,635 | 57,248 | 577 | ok |
| case-105 s6 camera up*400 before `setPosition` only | 51,537 | 21,706 | 57,232 | 577 | ok |
| case-105 s6 add `resetCamera`, drop extra `setTarget` only | 52,570 | 21,644 | 57,248 | 577 | ok |
| case-105 s9 revive before the three `nextInt` (y/z un-swap) | 51,542 | 21,699 | 57,232 | 577 | ok |
| case-105 s9 above + `right * -30000.0f` | 51,543 | 21,698 | 57,232 | 577 | ok |
| case-105 s7 `setFixed` before `MatrixSetTranslation` | 51,544 | 21,730 | 57,232 | 577 | ok |
| case-105 s10 right-vector `* 1000.0f` restored | 51,536 | 21,704 | 57,232 | 577 | ok |
| case-105 s10 teardown faithful order | 51,573 | 21,686 | 57,232 | 577 | ok |
| case-102 s3 right-vector used for the `* 0.04` add | 51,537 | 21,707 | 57,232 | 577 | ok |

## Other semantic bugs found (not landed)

* **case-105 state 9** (source ~3810): the original calls `revive()` *before*
  the three `nextInt(1000)` draws and assigns them x, y, z in that order; our
  source draws x, z, `revive()`, y, so the y/z offsets are swapped.  Fixing it
  is size-exact, raw +7 / aligned -8.
* **case-105 state 10** (source ~3862): the original scales the right-vector
  term by `1000.0f`; our source adds the unscaled right vector and applies the
  `* 1000.0f` to the up vector only.  Size-exact, raw +1 / aligned -3.
* **case-102 state 3** (source ~3455): the original uses `getRightVector()` for
  the second (`* 0.04f`) add and `getUpVector()` for the third (`* 0.004f`)
  add; our source leaves a dead `getRightVector()` and uses the direction
  vector for the second add.  Size-exact, raw +2 / aligned 0.

These are recorded for a future pass that can compose them with a raw-reducing
change.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13687/` (`orig.dis`, `ours.dis`,
`base.cpp`, `full.cpp`, `state9.cpp`).
