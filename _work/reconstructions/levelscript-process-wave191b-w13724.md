# LevelScript::process — wave 191b / w13724: case-92 s9/10, case-24 s0, case-154 s9, mission-1 intro

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13724`, branch
`wave191b-w13724`, base master `ea2ec2eec` (raw **51,655**, aligned **21,210**,
`.text` diffbytes 72,159, `.ARM.extab` 564, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff left the case-92 state-7/8/9/10, case-24, case-154 and
mission-1 intro blocks to this pass (the other two LevelScript agents took
case-91/105/158).  Re-deriving those blocks from the Thumb disassembly found
four statement-order divergences that are size-exact and aligned-positive.

## Original disassembly (the four blocks)

### case 92 state 9/10 (`0x143704`..`0x14384a`)

```
143704  pos = enemies[3]->geometry->getPosition()
14372a  sound->updateEvent3DAttributes(2249, &pos, &zero, false)
143732  m_nScriptTimerA += delta
143754  m_tempVec.x += delta * 0.006f
14376c  enemies[3]->geometry->moveForward(m_tempVec.x * delta)
143782  if (timerA >= 2001) {
143786      if (m_nState == 9) m_nState = 10
143790      if (timerA >= 7001) {
1437a2          cam->setFixed(false)
1437ac          cam->setTarget(player->geometry)
1437b6          cam->setLookAtCam(false)                 <- before setPosition
1437ca          cam->setPosition(player->getPosition() - 2400*player->GetDirVector())
1437fc          m_pHud->visible = 1
143804          m_pRadar->enabled = 1
14380c          resetCamera(lvl)
143814          lvl->lodManager->forceUpdate(delta, false)
143820          player->setComputerControlled(false)
143828          m_bCinematicBreak = 0
143832          player->player->setVulnerable(true)
14383c          enemies[3]->setActive(false)
143846          enemies[3]->setVisible(false)
14384a          b 0x13953c (shared tail: m_nState++)
```

Our source put `setLookAtCam(false)` last and ran the teardown in a different
order (`m_bCinematicBreak`/HUD/computer/reset/forceUpdate/radar/visible/
vulnerable/active).  Reordered to the original.

### case 24 state 0 (`0x13e97e`..`0x13ea80`)

Original order: `stopShooting`, HUD, radar, `setLookAtCam`, `setVulnerable`,
`setRotation`, `setComputerControlled`, `removeAllGuns`, `setTarget`,
`getPosition`, `setPosition`, `translate`, then the `m_tempVec = getPosition()`
/ `m_dirVec = getDirection()` / `*= 45000` / `+=` chain, `landmarks[3]->
setPosition`, enemy-list loop, `forceUpdate`, `m_bCinematicBreak = 1`, state=1.
Our source had HUD/radar/rotation/stopShooting first and `setTarget` after
`setPosition`.  Reordered to the original.

### case 154 state 9 (`0x14484e`..`0x144a12`)

Original teardown after the `shipGroup == 9` loop:
`enemies[1]->shipGroup = 8`, `enemies[1]->field_0x74 = 0`,
`setAlwaysFriend(false)`, `setAlwaysEnemy(true)`, `enemies[0]->player->
setEnemies(nullptr)`, `enemies[0]->player->setVulnerable(true)`,
`setFixed(false)`, `resetGunDelay`, `setLookAtCam(false)`,
`setTarget(player->geometry)` (unconditional), `setFreeze(false)`,
`setSpeed(2.0f)`, `setComputerControlled(false)`, `setVulnerable(true)`,
`setPosition(0,0,90000)`, HUD, radar, `resetCamera`, `forceUpdate`,
`m_bCinematicBreak = 0`, `if (field_0xaa) setAutoTurret(true)`, timerA=0,
state++.  Our source had a different order and kept `setTarget` inside the
`field_0xaa` branch; the original calls it unconditionally.

### mission-1 intro (`0x137d82`..`0x137dd6`)

Original order is `lodManager->forceUpdate(delta, true)`, `player->rotate`,
`enemies[0]->getPosition()`, `enemies[0]->setToSleep()`.  Our source ran
`rotate`, `setToSleep`, `getPosition`, `forceUpdate`.

## Landed change

All four blocks reordered to the original statement order.  Every moved
statement touches disjoint state, so the reorders are semantics-preserving;
case-154 state 9 additionally moves the unconditional `setTarget` out of the
`field_0xaa` branch, matching the binary.  The case-154 state-9 block keeps
only the `m_nScriptTimerA = 0` store (the original's `strd` also clears
timerB; adding it grows the block 4 B, so it is left for a pass with an 8 B
compensation).

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,655 -> 51,709  (+54)
aligned    21,210 -> 21,102  (-108)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,159 -> 72,213 (+54); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 564 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 51,655 | 21,210 | 57,232 |
| case-92 s9/10 faithful | 51,660 | 21,190 | 57,232 |
| case-24 s0 faithful | 51,663 | 21,188 | 57,232 |
| mission-1 intro faithful | 51,659 | 21,201 | 57,232 |
| case-154 s9 faithful (timerA only) | 51,692 | 21,153 | 57,232 |
| case-92 s7 faithful (timerA only) | 51,693 | 21,206 | 57,232 |
| case-92 s8 faithful (no timerB/no vec init) | 51,665 | 21,210 | 57,232 |
| case-102 s3 right-vector fix | 51,654 | 21,215 | 57,232 |
| case-92 s9 + case-24 s0 | 51,668 | 21,168 | 57,232 |
| **s9 + c24 + m1 + c154s9 (landed)** | **51,709** | **21,102** | **57,232** |
| landed + case-102 s3 | 51,708 | 21,107 | 57,232 |
| landed + case-92 s7 | 51,746 | 21,103 | 57,232 |

The case-92 state-7 faithful block (with the original's timerB clear) is
size-exact only when timerB is dropped; with timerB it grows 8 B and shifts
every following byte (+2,247 raw).  Case-92 state-8 is missing the original
`m_tempVec = getPosition()` and carries an extra `resetCamera`; the faithful
form is +16 B, so it is left for a pass with a compensation.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13724/` (`map.py`, `variants.py`,
`s7sweep.py`, `s9sweep.py`, `misc_sweep.py`, `c24c154.py`, `c154s9.py`,
`combo.py`, `combo2.py`, `measure.py`, `base_LevelScript.cpp`, `orig.dis`).

## Pass 2 — case-154 s3/s5 and case-105 s7/s9/s10

Second pass on top of the first commit (`5d745f6cc`, raw 51,709 / aligned
21,102).  Re-derived the remaining case-154 and case-105 state blocks from
the Thumb disassembly:

* **case 154 s3** (`0x1430d6`/`0x1445b2`): original order is `setSpeed`,
  `setTarget`, `getPosition`, `+= {-350,400,-1500}`, `setPosition`,
  `setFixed(false)`, `m_nState++`, `forceUpdate`, `timerA = 0`; ours had
  `m_nState++` first and `forceUpdate`/`setFixed`/`setPosition` in a
  different order.
* **case 154 s5** (`0x1446e0`): original order is `setSpeed(0.02)`,
  `setTarget`, `getPosition`, `+= {150,400,-1500}`, `setPosition`,
  `setFixed(false)`, `m_nState++`, `forceUpdate`, `field_0x13e = 0`,
  `enemies[0]->setPosition({-10000,0,22000})`, `setAIDisabled(false)`, then
  the `shipGroup == 9` loop; ours ran `setTarget` first and moved
  `setPosition`/`setFixed` after the loop.
* **case 105 s7** (`0x142910`..`0x1429d8`): the `timerA >= 10001` teardown
  is `setAIDisabled` loop, `setFreeze(false)`, `setVisible(true)`,
  `setFixed(false)`, `setTarget(player->geometry)` (missing from our
  source), `setLookAtCam(false)`, `getPosition`/`GetDirVector`/`setPosition`,
  HUD, radar, `resetCamera`, `forceUpdate`, `m_bCinematicBreak = 0`,
  `timerA = 0`, `m_nState++`.
* **case 105 s9** (`0x142a04`): the original calls `revive()` before the
  three `nextInt(1000)` draws and assigns them x, y, z; ours drew x, z,
  `revive()`, y.
* **case 105 s10** (`0x142bf2`): original teardown order is
  `setFreeLookMode`, `enableFirstPersonCam`, `hideShip`, `setVulnerable`,
  HUD, radar, `stopShooting`, `m_bCinematicBreak`, `setLookAtCam`,
  `setComputerControlled`; ours had `enableFirstPersonCam`/`setVulnerable`/
  `hideShip`/`setFreeLookMode` and `setLookAtCam` before `stopShooting`.

Measured (exact NDK r18b `-Oz` compile + full gold link + `--strip-all`):

```
raw        51,709 -> 51,800  (+91)
aligned    21,061 -> 21,061  (see below; 21,102 -> 21,061 = -41)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,213 -> 72,304 (+91); size/base unchanged
.ARM.extab diffbytes 564 (unchanged; size/base exact 156,312 B)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS
```

### Pass-2 candidate table (on top of pass 1; all size-exact)

| candidate | raw | aligned |
|---|---:|---:|
| pass-1 baseline | 51,709 | 21,102 |
| case-154 s3 | 51,717 | 21,089 |
| case-154 s5 | 51,716 | 21,088 |
| case-105 s10 | 51,737 | 21,092 |
| case-105 s9 | 51,716 | 21,094 |
| case-105 s7 | 51,750 | 21,094 |
| case-154 s3+s5 | 51,724 | 21,087 |
| case-154 s3+s5 + case-105 s10 | 51,752 | 21,077 |
| case-154 s3+s5 + case-105 s9+s10 | 51,759 | 21,069 |
| **case-154 s3+s5 + case-105 s7+s9+s10 (landed)** | **51,800** | **21,061** |
| landed + case-102 s3 right-vector fix | 51,799 | 21,066 |

The case-102 s3 right-vector fix (move `m_dirVec = getRightVector()` before
the second add) is a real semantic fix but is raw -1 / aligned +5 on this
base, so it is left for a raw-weighted pass.
