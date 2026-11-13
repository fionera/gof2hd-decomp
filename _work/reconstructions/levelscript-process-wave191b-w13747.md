# LevelScript::process — wave 191b / w13747: case-70 s2, case-92 s3 store/call order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13747`, branch
`wave191b-w13747`, base master `4c4d81cf7`.

## Lens

The handoff focused this pass on case-24 s0, case-70 s0/s2, case-92 s2/3/9/10
and case-105 s7/s9/s10.  Re-deriving those blocks from the Thumb disassembly
confirmed almost all of them already match the original statement order; two
store/call-order divergences were found and landed.

## Landed changes (size-exact)

### case 70 state 2 (`0x13ec9c`..`0x13eca8`)

```
13ec9c  ldr   r0, [pc, #280]      ; Globals::...
13ec9e  movs  r1, #3
13eca0  strb  r4, [r6, #17]       ; m_bCinematicBreak = 0   <- BEFORE m_nState
13eca2  add   r0, pc
13eca4  ldr   r5, [r0, #0]
13eca6  ldr   r0, [r6, #24]
13eca8  str   r1, [r6, #28]       ; m_nState = 3
13ecaa  blx   r5
```

The source had `m_nState = 3;` before `m_bCinematicBreak = 0;`; swapping the two
independent stores makes clang emit the byte store first, matching the binary.
(Our case-70 s2 block sits at `0x13ecbe` because the state-1 arm is longer; the
remaining diffmap cluster at `0x13ec3c..0x13ed00` is that address shift plus
regalloc, not a semantic difference.)

### case 92 state 3 (`0x13feb6` / `0x13fec6`)

```
13feae  ldr   r0, [r6, #4]
13feb0  movs  r1, #1
13feb2  ldr.w r0, [r0, r4, lsl #2]
13feb6  blx   setCloakingPossible(true)     <- BEFORE cloak
13feba  ldr   r0, [r6, #4]
13febc  movw  r1, #18000
13fec0  movs  r2, #0
13fec2  ldr.w r0, [r0, r4, lsl #2]
13fec6  blx   cloak(18000, false)
13feca  adds  r4, #1
```

The source called `cloak(18000, false)` before `setCloakingPossible(true)`;
swapping them matches the original.

## Measured table

| candidate | raw | aligned | size | .text | .ARM.extab |
|---|---:|---:|---:|---:|---:|
| baseline (master 4c4d81cf7) | 52,008 | 20,733 | 57,232 | 72,954 | 564 |
| case-92 s3 swap alone | 52,008 | 20,725 | 57,232 | 72,954 | 564 |
| case-70 s2 swap alone | 52,008 | 20,724 | 57,232 | 72,954 | 564 |
| **both (landed)** | **52,008** | **20,724** | **57,232** | **72,954** | **564** |

`.note.gnu.build-id` 20, `.ARM.exidx` 0, `.rodata` 0 unchanged; `.rel.plt`
byte-identical; symbol parity 5370/5370.

## Verified faithful (no change)

* **case-24 s0** (`0x13e97e..0x13ea80`): statement order matches; only
  regalloc (`fp` vs `r5`) and stack-slot (`sp,#368` vs `sp,#280`) noise.
  The tail `m_nState = 1` is the shared store at `0x1438d6` in both.
* **case-70 s0** (`0x1399a2..0x139a60`): `setTurretMode, resetCamera,
  setFreeLookMode, enableFirstPersonCam, hideShipForFirstPersonCameraView,
  stopShooting, m_bCinematicBreak=1, hud->visible=0, radar->enabled=0,
  setComputerControlled, setLookAtCam, setActive, setTarget, forceUpdate,
  getPosition/getDirection, m_nState=1` — exact.
* **case-92 s2** (`0x13fb7a..0x13fbec`): the 2.0/2.08/2.05 moveForward ladder
  and the `timerA=0; timerB=0; m_nState=3` tail match instruction-for-instruction
  apart from regalloc.  (The original tail is `m_nState++`-shaped: `ldr/adds/str`
  at `0x13fbda..0x13fbe8`; see the failed experiment below.)
* **case-92 s9/10** (`0x143704..0x14384a`): full call order matches (getPosition,
  updateEvent3DAttributes(2249), moveForward, setFixed, setTarget, setLookAtCam,
  getPosition/GetDirVector, ml, mi, setPosition, resetCamera, forceUpdate,
  setComputerControlled, `m_bCinematicBreak=0`, setVulnerable, setActive,
  setVisible).
* **case-105 s7** (`0x1426xx`): documented order matches.
* **case-105 s9** (`0x142a04`): `revive()` before `nextInt(1000)` order matches.
* **case-105 s10** (`0x142bf2..`): `setTurretMode, resetCamera, rocket block,
  setFreeLookMode, enableFirstPersonCam, hideShipForFirstPersonCameraView,
  setVulnerable, hud=0, radar=0, stopShooting, m_bCinematicBreak=1,
  setLookAtCam, setComputerControlled, setPosition, getLightDirection/
  VectorNormalize, setDirection, getDirection/ml, getRightVector/ml,
  getUpVector/ml, getPosition, setPosition` — exact.

## Failed experiments (reverted)

| experiment | raw | aligned | size | note |
|---|---:|---:|---:|---|
| case-105 s7 tail: `m_nState++` before `m_nScriptTimerA = 0` | 52,666 | 20,722 | **57,244** | duplicates the shared `state++` tail (+12) |
| case-92 s2: `m_nState = 3` -> `m_nState++` | 53,580 | 20,707 | **57,248** | forces the inline `ldr/adds/str` (+16) |
| case-70 s2: `m_bCinematicBreak = 0` before `forceUpdate` | 52,009 | 20,725 | 57,232 | worse raw/aligned |
| case-70 s2: drop the `Player *lead` temp | 52,008 | 20,725 | 57,232 | no change |

The two size-growing candidates are semantically closer to the original but
need a compensating 12/16-byte shrink elsewhere before they can land; the
function is size-exact, so they are left out of this pass.
