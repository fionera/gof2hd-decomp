# LevelScript::process — wave 191b / w13754: mission-0 s3/s5 dispatch + state-3 hand-back

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13754`, branch
`wave191b-w13754`, base master `2655cec94`.

## Lens

The handoff focused this pass on the mission-0 state-3 hand-back block, the
state-3/5 dispatch, and mission-0 states 4-6.  w13746 had landed the *faithful*
`cmp 4 / cmp 3 / cmp 5` dispatch (source order 4,3,5) but at a large raw cost
(53,589 vs the prior 52,136).  This pass searched for a size-exact source shape
that keeps the faithful hand-back statement order without the branch-layout
shift.

## Landed change: source order 4,5,3 + s5 stop placement

Reordering the three mission-0 `if`/`else-if` bodies to 4,5,3 (state 4, then
state 5, then state 3) and moving the state-5 `sound->stop(player->field_0x1c)`
after `updateEvent3DAttributes(158)` (the original `0x139010..` order) is a
measured win on raw, `.text`, `.ARM.extab` and `.rel.plt`:

| metric | baseline (w13746) | this pass |
|---|---:|---:|
| raw | 53,589 | **52,135** |
| aligned | 20,577 | 20,653 |
| size | 57,232 | 57,232 |
| `.text` diff bytes | 74,637 | **73,183** |
| `.ARM.extab` diff bytes | 564 | **561** |
| `.rel.plt` diff bytes | 10 | **0 (byte-identical)** |

Aligned regresses by 76 (the 0x13c000 page improves 2,734 -> 1,644 while the
0x13e000 page regresses 1,547 -> 2,704); raw, `.text`, `.ARM.extab` and
`.rel.plt` all improve, so the batch is a net win.  The state-3 hand-back
statement order itself is unchanged from the faithful w13746 reconstruction
(`0x138a10` order: timer=0, state=4, setComputerControlled(false), field_5e=0,
radar=1, setCollide(true), m_bCinematicBreak=0, setLookAtCam(false), setTarget,
m_bStartSequence=0, field_0x12=1).  Swapping the last two stores is a codegen
no-op (clang reorders); removing the duplicate trailing `m_nScriptTimerA = 0`
shrinks the function to 57,224 (gate-fatal), so the duplicate is kept.

## Dispatch permutation sweep (all size-exact unless noted)

| source order (s4/s3/s5) | raw | aligned | size | `.text` | `.ARM.extab` | `.rel.plt` diff |
|---|---:|---:|---:|---:|---:|---:|
| 435 (w13746, faithful test order) | 53,589 | 20,577 | 57,232 | 74,637 | 564 | 10 |
| **453 (landed)** | **52,135** | 20,653 | 57,232 | **73,183** | **561** | **0** |
| 354 | 52,598 | 20,670 | 57,232 | 73,662 | 562 | 7 |
| 345 | 55,187 | 20,765 | **57,248** | — | — | 10 |
| 534 | 54,896 | 23,084 | **57,240** | — | — | 40 |
| 543 | 54,896 | 23,084 | **57,240** | — | — | 40 |

`__builtin_expect` on the dispatch conditions (with the `// lint: arm_builtin`
waiver) all grew the function to 57,256-57,260, so they were rejected.

## State 4 (orig `0x138d2a`) — measured, not landed

Original order: `setTurretMode(false)`, `resetCamera`, `setFreeLookMode(false)`,
`enableFirstPersonCam(false)`, `hideShipForFirstPersonCameraView(false)`,
`m_bCinematicBreak=1`, `getPlayer()->setCollide(false)`, `radar->enabled=0`,
`setComputerControlled(true)`, `removeAllGuns`, `setLookAtCam(true)`,
`setTarget`, `setRotation(0, pi/2, 0)`, `getPosition`/`setPosition`,
`translate(25000,-200,-1000)`, `m_nState=5`.

Per-step measurements on top of 453 (all size-exact):

| step | raw | aligned |
|---|---:|---:|
| turret | 52,146 | 20,653 |
| cine | 52,142 | 20,653 |
| cc | 52,151 | 20,656 |
| rot | 52,149 | 20,651 |
| turret+cine | 52,148 | 20,653 |
| turret+cine+cc | 52,163 | 20,656 |
| all four | 52,160 | 20,654 |

None beats 453's 52,135 raw; `rot` alone trades +13 raw for -2 aligned, so no
state-4 reorder was landed.

## State 5 (orig `0x138fd8`) and state 6 (orig `0x139b70`)

State 5 original sound block: `play 157`, `update(157)`, `play 158`,
`update(158)`, **`stop(player->field_0x1c)`**, `play 161`; then `update(161)`,
`getSpeed`, `setSpeed(*0.98f)`, msg13 translate, msg14 -> timer=0/state=6.
Moving the `stop` after `update(158)` on top of 453 is the landed variant
(raw 52,135 vs 52,136).

State 6 original order: `getPosition`, `update(161)`, translate,
`m_nScriptTimerA += delta`, rumble, msg15 isOver -> `m_nState=7`,
`m_pGeometry5->setPosition(player->getPosition())`, `SetAnimationState(3)`,
`SetAnimationState(1)`, `play 160`, **`stop 161`**, `m_nScriptTimerA = 0`.
On top of 453+s5: moving just `stop(161)` after `play 160` -> raw 52,138;
moving only anim-state-1 before `play 160` -> 52,139; both -> 52,141.  All
worse than 52,135, so state 6 is unchanged.

## Gate

`check` green: byte_exact **4508/4524**, linked_exact **4508/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, parity **5370/5370**.  `LevelScript::process` stays
size-exact at 57,232.
