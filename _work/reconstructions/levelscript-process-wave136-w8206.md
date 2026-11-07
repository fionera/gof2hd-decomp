# LevelScript::process — wave 136 / w8206 (case-40 state-1 cold-block order)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `655e3bcc5` (wave-136 head, 58.9%).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.9
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54327             54323            (-4)
whole .text diffbytes    84709             84705            (-4)
.ARM.extab diffbytes     1395              1395
.ARM.exidx diffbytes     0                 0
.rodata / .data          0 / 0             0 / 0
.rel.plt diffbytes       4                 4   (unchanged; pre-existing symbol-value delta)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The whole 16-instruction case-40 state-1 cold block is now **byte-identical**
(verified by normalized-instruction search: original instruction indices
9526..9541 == ours 3406..3421, all 16 equal).  The fuzzy ratio moves from
58.9139 to 58.8841 (a 0.03 pp block-placement re-roll elsewhere); the official
verify pct is 58.9 either way and the ratchet's `byte_exact` is unchanged.

## Landed change — case 40 state 1 (`0x13e6c0`)

Original `0x13e6da`..`0x13e726` (Thumb, objdump of the original `.so`):

```
13e6da  blx TargetFollowCamera::translate(0,0,-2*delta)
13e6de  ldr  r0,[fp,#4] ; ldr r0,[r0,#16] ; blx RadioMessage::isOver   ; messages[4]
13e6ee  ldr  r6,[sp,#84] ; ldr r0,[sp,#88] ; ldr r1,[r6,#8] ; mov r5,r0
13e6f8  blx TargetFollowCamera::setTarget(player->geometry)            ; FIRST
13e6fc  ldr r0,[r5,#208] ; movs r1,#1 ; strb r1,[r0,#1]                ; m_pHud->visible = 1
13e704  ldr r0,[r5,#212] ; strb.w r1,[r0,#72]                          ; m_pRadar->enabled = 1
13e70c  movs r1,#0 ; strb r4,[r5,#17]                                  ; m_bCinematicBreak = 0
13e712  blx TargetFollowCamera::setLookAtCam(false)
13e71e  mov r0,r6 ; movs r1,#0 ; blx PlayerEgo::setComputerControlled(false)
13e722  blx Player::setVulnerable(true)
13e726  b.n 13eddc                                                     ; m_nState = 2
```

Source had `setTarget` **second** (after `m_bCinematicBreak = 0`) and
`setComputerControlled` **before** `setLookAtCam`.  Reordered to the exact
original sequence.  This is the "cold-chain mirror" of case-41 state-4's
`translate(delta, delta, -2*delta)`: both translate the camera and then tear
the cutscene down in the same statement order.

## Case-102 state-1 tail (`0x143b18`..`0x143bfe`) — analysed, NOT landed

The tail is entered from `0x13e8bc` (`translate(delta*0.6, delta*0.8,
-delta*2.5)`) and `0x143b18` (`if (m_nScriptTimerA >= 2001)`).  Original
statements, in order:

```
143b2c  fp = this
143b30  m_pLevel->getPlayer()->resetGunDelay()
143b3c  player->setFreeze(false)
143b4a  player->setVisible(true)
143b52  player->player->setVulnerable(true)
143b5a  m_pCamera->setLookAtCam(false)
143b64  m_pCamera->setTarget(player->geometry)
143b6e  m_pCamera->setPosition(player->getPosition() - player->GetDirVector() * 2400.0f)
143ba8  m_pHud->visible = 1
143bb0  m_pRadar->enabled = 1
143bb8  resetCamera(m_pLevel)                       ; <-- source has m_tempVec = enemies[3]...
143bc2  m_pLevel->lodManager->forceUpdate(delta, false)
143bd2  m_bCinematicBreak = 0
143bd8  m_nScriptTimerA = 0                        ; strd sl,sl,[r4], r4 = this+0x90
143bdc  Globals::sound->stop(currentMusicEvent)
143be6  Globals::sound->play(2241, 0, 0, 0.0f)
143bf8  m_nState++
```

The source's `m_tempVec = (*enemies)[3]->geometry->getPosition();` is spurious.
Replacing it with `resetCamera(m_pLevel);` makes the **entire tail block match
exactly** (only the `ldr r2,[pc,#...]` literal-pool distance differs).  Cost:
**-16 B**, so it needs a +16 B faithful companion to keep 57232.

## Case-102 state-0 (`0x13a658`) — analysed, NOT landed

Original order: `setTurretMode(false)`, `resetCamera(m_pLevel)`,
`if (player->isInRocketControl())`, `setFreeLookMode(false)`,
`enableFirstPersonCam(false)`, `hideShipForFirstPersonCameraView(false)`,
`stopShooting(0)`, `setFreeze(true)`, `setVisible(false)`,
`player->player->setVulnerable(false)`, `m_bCinematicBreak = 1`,
`m_pHud->visible = 0`, `m_pRadar->enabled = 0`, `setLookAtCam(true)`,
**`setTarget(enemies[0]->geometry)`** (`0x13a70e`),
`setPosition(enemies[0]->getPosition() + {9000,-7000,40000})`, sound
stop/play(2240), `m_nScriptTimerA = 0`, `m_nState++`.  There is **no
`translate` in state 0** (it is the state-1 entry at `0x13e8bc`).

The source has `setPosition` but no `setTarget` (+16 B), and carries the
`translate` in state 0 instead of state 1.  The faithful fix set is therefore
E (move `translate` to state 1 + add state-0 `setTarget`, -8 B) + B
(`resetCamera`, -16 B) + J (cached `player` receiver, -8 B) = **-32 B**.

## Size trade measured on this base

| variant | edits | size | proc diffbytes | exact ratio |
|---|---|---:|---:|---:|
| baseline | — | 57232 | 54327 | 58.9139 |
| **A (landed)** | case-40 state-1 reorder | **57232** | **54323** | **58.8841** |
| P | anchor Stop/Play swap | 57232 | 54327 | 58.9139 |
| B | `resetCamera` tail | 57216 | — | 58.7451 |
| E | move translate + state-0 setTarget | 57224 | — | 58.7009 |
| H | state-0 `setTarget` only | 57248 | — | — |
| ABH | A + B + H | 57232 | 54549 | 58.8657 |
| ABHP | ABH + anchor swap | 57232 | 54549 | 58.8657 |

`ABH` is the only size-exact combination that lands both hypotheses, but it
re-rolls `MachineBlockPlacement` (the state-0 register allocation shifts, so the
state-0 block still does not match) and raises diffbytes by 226 while dropping
the exact ratio by 0.05 pp.  **A alone** is the best size-exact landing
(diffbytes -4, exact block match) and is what was committed.  The B/E/J fixes
are left as a follow-up that must be paired with a +32 B faithful addition.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
```

Scratch helpers in `/dev/shm/w8206/` (not tracked): `variant.py` (variant
applier), `measure.py` (size/diffbytes/rel.plt), `ratio.py` (exact difflib
ratio), `callset.py` (call census), `blk.py`/`blk2.py` (normalized block
matcher), `findcall.py` (call-site context), `relplt2.py` (rel.plt byte diff).
