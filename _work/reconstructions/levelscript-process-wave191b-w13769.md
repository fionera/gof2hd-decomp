# LevelScript::process — wave 191b / w13769: mission-0x4e state order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13769`, branch
`wave191b-w13769`, base master `a4a497605`.

## Session baseline

| metric | value |
|---|---:|
| raw | 53,419 |
| aligned | 20,308 |
| size | 57,232 / 57,232 |
| `.text` diff bytes | 74,990 |
| `.ARM.extab` diff bytes | 561 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |

## Landed: mission-0x4e substate statement order (three edits)

All three are verified directly against the Thumb disassembly of the original.

1. **s12 @0x13b7f2** — the original stores `m_nState = 13`
   (`str r0,[sl,#28]`, `r0=13`, `0x13b8b8`) *before* branching to the shared
   epilogue (`b.w 0x144f5a`).  Source had `return m_bCinematicBreak != 0;`
   ahead of the store.
2. **s10 @0x13b948** — the original emits the camera `translate`
   (`0x13b984`) before `m_nScriptTimerA += delta` (`0x13b990`).
3. **s1 @0x1383a0** — the original emits `m_pCamera->setTarget(...)`
   (`0x1383b2`) before the `TransformGetTransform` / `SetAnimationState`
   pair (`0x1383d2`).

Measured, each step separately:

| step | raw | aligned | `.text` | `.ARM.extab` | size |
|---|---:|---:|---:|---:|---:|
| baseline | 53,419 | 20,308 | 74,990 | 561 | 57,232 |
| s12 store before return | 53,057 | 20,292 | 74,628 | 561 | 57,232 |
| s10 translate before timer | 53,056 | 20,292 | 74,627 | 561 | 57,232 |
| s1 setTarget before transform | 53,421 | 20,293 | 74,992 | 561 | 57,232 |
| **final (all three)** | **53,056** | **20,292** | **74,627** | **561** | **57,232** |

Net: raw **-363**, `.text` **-363**, aligned **-16**, size exact,
`.rel.plt` still byte-identical, `.ARM.extab` unchanged at 561.

## Verified-but-already-matching mission-0x4e substates

s2 `0x13ba5c`, s3 `0x13b8dc`, s4 `0x13baba`, s5 `0x13b8c0`, s6 `0x13bb7c`,
s7 `0x13bc18`, s8 `0x13baf2`, s9 `0x13bc34`, s11 `0x13bd2c` were re-derived
from the Thumb this session and already match the source statement-for-
statement (including the inline `m_tempVec.x/.y/.z +=` adds at
`0x13be3c..0x13be6e`, which the compiler emits as three `vadd.f32` +
`vstr`, not a `Vector::operator+=` call).

## Not landed (no measured win)

* s2 `camera2->setPosition(20000,5000,30000)` shape: the original loads the
  three literal-pool floats and tail-branches to the shared
  `setPosition(float,float,float)` call site (`b.w 0x13c888`); the current
  source already produces that shape.
* s6/s8 `(float) delta * 0.3f + (float) m_nScriptTimerA`: the original
  computes the float delta term first, then `__aeabi_l2f`, then `vadd.f32`;
  the source order already matches.

## Landed: mission-0x4e state-0 body order (case 94, state 0)

Verified directly against the Thumb (`0x13ad3c..0x13adf6`):

1. enemy loop: `setPosition(...)` (`0x13ad3c`) **before** `awake()`
   (`0x13ad4a`); then `setVisible`, `setDirection`, `setAIDisabled`.
2. rocket-control block: `setRocketControl(nullptr,nullptr)` (`0x13adc6`),
   `killLiberator()` (`0x13adcc`), `setRumblePercentage(0.0f,0)`
   (`0x13add6`).
3. teardown: `setFreeLookMode(false)`, `enableFirstPersonCam(false)`,
   `hideShipForFirstPersonCameraView(false)`, `stopShooting(0)`,
   `m_bCinematicBreak = 1`, `m_pHud->visible = 0`, `m_pRadar->enabled = 0`,
   `setLookAtCam(true)` (`0x13adbe..0x13adf6`).

| metric | before | after |
|---|---:|---:|
| raw | 53,056 | **53,057** (+1) |
| aligned | 20,292 | **20,271** (-21) |
| size | 57,232 | 57,232 (exact) |
| `.text` | 74,627 | 74,628 (+1) |
| `.ARM.extab` | 561 | 561 |

A real aligned win; the single raw byte is a downstream shift, not a
structural change.  `.rel.plt` byte-identical, gate green.

## Landed: case-125 state-0 route/timer order

Original Thumb `0x139dde..0x139df8`: `RouteC1` (via `new Route`) ->
`Level::setPlayerRoute` -> `PlayerEgo::setRoute` -> `strd r0,r0,[fp,#144]`
(`m_nScriptTimerA = 0`, a 64-bit zero store) last.  Source had the timer
store between the allocation and `setPlayerRoute`.

| metric | before | after |
|---|---:|---:|
| raw | 53,057 | **53,058** (+1) |
| aligned | 20,271 | **20,270** (-1) |
| size | 57,232 | 57,232 (exact) |
| `.text` | 74,628 | 74,629 (+1) |
| `.ARM.extab` | 561 | 561 |

`.rel.plt` byte-identical, gate green.  The +1 raw byte is a downstream
shift; the aligned count drops by one.

### Tried and reverted (case 125 state 2/3)

* state-2 `awake()` before `Vector playerPos = player->getPosition()`:
  size 57,240 (gate-fatal).
* state-2 teardown reorder (setFreeLookMode/enableFirstPersonCam/hideShip/
  stopShooting/m_bCinematicBreak/Hud/Radar/setLookAtCam): raw 53,062,
  aligned 20,275 (worse).
* state-3 `setAutoPilot` before `isDockedToDockingPoint` plus
  `resetGunDelay` after: raw 53,058, aligned 20,271 (no aligned change).
* state-3 `m_pHud`/`m_pRadar` before `resetCamera`: raw 53,058,
  aligned 20,271 (no aligned change).
* state-3 `setLookAtCam(false)` before `setTarget`: raw 53,668,
  aligned 20,299 (much worse).
* state-3 `setComputerControlled(false)` before `setTarget`/`setLookAtCam`:
  raw 53,059, aligned 20,270 (no aligned change).
* state-0 `} if (m_nState == 1) {` -> `} else if` : size 57,256 (gate-fatal).

## Landed: case 69/70/135/139 statement order (checkpoint 7)

Five edits, each verified against the original Thumb and measured separately
from the committed baseline (raw 53,058 / aligned 20,270 / `.text` 74,629):

1. **case 135** (source ~L4134): `getStatusValue()` before
   `getProductionGoodAmount()` — original `0x1391f4` then `0x139200`,
   `amount/2` emitted as `(amount + (amount>>31)) >> 1`.
2. **case 135 revive loop** (~L4152): `(*enemies)[i]->revive()` before
   `Vector playerPos = player->getPosition()` — original `0x139258`
   vtable+0x18 revive, then getPosition/`AEMath::operator+`/setPosition.
3. **case 139 state-0** (~L4243): `isDockedToDockingPoint()` before
   `hackingWon()` — original `0x13968c` then `0x139698`.
4. **case 69 state-0 teardown** (~L2425): full reorder to
   `0x139742..0x139826` — setTurretMode, resetCamera, setFreeLookMode,
   enableFirstPersonCam, hideShip, stopShooting, m_bCinematicBreak=1,
   Hud=0, Radar=0, setComputerControlled(true), setLookAtCam(true),
   setActive(true), setTarget, camPos=getPosition, dir=getDirection,
   `+= dir*10000`, x+600/y+300/z+1000, setPosition.
5. **case 70 state-1 conclude** (~L2540): reorder to
   `0x13bfc2..0x13c04a` — setLookAtCam(false), setTarget(player->geometry),
   setComputerControlled(false), Hud=1, Radar=1, resetCamera, forceUpdate,
   m_bCinematicBreak=0, m_nState=2, setActive(false), setToSleep,
   setPosition(0,0,-5000000), setVisible(false) (setActive precedes
   setToSleep).

| step | raw | aligned | `.text` | `.ARM.extab` | size |
|---|---:|---:|---:|---:|---:|
| committed baseline | 53,058 | 20,270 | 74,629 | 561 | 57,232 |
| 1 case-135 status/amount | 53,058 | 20,269 | 74,629 | 561 | 57,232 |
| 2 case-135 revive order | 53,058 | 20,239 | 74,629 | 561 | 57,232 |
| 3 case-139 predicate | 53,058 | 20,239 | 74,629 | 561 | 57,232 |
| 4 case-69 teardown | 53,077 | 20,195 | 74,648 | 561 | 57,232 |
| 5 case-70 conclude | **53,083** | **20,167** | **74,654** | **561** | **57,232** |

Net aligned **-103**, raw +25, `.text` +25, size exact, `.rel.plt`
byte-identical, gate green (`check` RATCHET PASS).  Steps 3 and 5 are
faithful-order changes that move the raw byte count but are part of the
same measured block; the aligned count is the primary metric.

### Tried and reverted this session

* case 94 state-1: `lookBehind()` before `setRotationAroundTarget(true)` —
  raw 53,062, aligned 20,270 (no win).
* case 94 state-1: `player->setFreeLookMode(true)` moved into the docked
  branch — raw 54,179 (bad).
* case 135 state-0 formation loop: setPosition before awake/setVisible —
  raw 53,061, aligned 20,270.
* case 154 (`~L4780`) `Level *lvl` variants: hoisting `lvl` + passing
  `m_pLevel` to `new Objective` -> raw 53,064/aligned 20,854;
  `lvl->objectivesB = new Objective(3,91000,m_pLevel)` -> neutral
  53,058/20,270; `delete`-then-`nullptr` / `Objective *obj` temporaries ->
  size 57,236..57,244 (gate-fatal).
* case 70 state-2: `m_bCinematicBreak = 0` before `m_nState = 3` (the
  original order, `0x13eca0` strb then `0x13eca8` str) — aligned 20,168,
  one worse than 20,167.  Reverted; noted as faithful but not a win.
