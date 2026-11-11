# Wave 170 merge tests (20 agents)

Base master `00c403d5a` (wave 169).  All worktrees branched from it, built with
`-j2` and a 12 GB `ulimit -v`.  Landed rows were re-measured on the combined
master tree after cherry-picking.

## Landed

| worker | target | change | raw before -> after | aligned before -> after | size |
|---|---|---|---:|---:|---:|
| w11502 | `LevelScript::process` | case-12 `setPosition(Vector)`, case-8 state advance + music-stop order, mission-89 case-3 order | 53,970 -> 53,947 | 17,818 -> 17,820 | 57,232 |
| w11503 | `LevelScript::process` | mission-0 case-11 timer add, drop spurious `enableFillScreen`, state-16 block order | 53,947 -> 53,893 | 17,820 | 57,232 |
| w11501 | `LevelScript::process` | expect/swap sweep (state-5 timer, mission-2 msg1, mission-42 state-7, hacking gate, wormhole gate, mission-50) | 53,893 -> 53,876 | 17,820 -> 17,871 | 57,232 |
| w11504 | `LevelScript::process` | block-scope `m_pLevel`/`m_pGeometry*`/`m_pCamera` aliases (state-7 kept block-scoped) | 53,876 -> 53,932 net | 17,871 -> 17,900 | 57,232 |
| w11507 | `Radar::draw` | three sound-ladder / tractor expects | 7,127 -> 7,095 | 1,606 -> 1,595 | 9,908 |
| w11510+w11509 | `Layout` C1/C2 | `0x290`/`0x284`/`0x2d0` statement reorders | 5,650 -> 5,640 | 2,284 -> 2,282 | 7,072 |
| w11516 | `Hud::draw` | barH-first show-block order | 582 -> 548 | 228 -> 220 | 9,332 |
| w11514 | `HangarWindow::render` | ship-branch cleanup scope restores row-39 LSDA | 1,970 -> 2,266 | 294 -> 386 | 9,700 |
| w11519 | `MGame::OnUpdate` | drop `volatile st` anchor, move `Level *level` snapshot | 15 -> 16 | 111 -> 6 | 11,440 |

Combined wave-170 result: `.text` **81,642 -> 81,825**, `.ARM.extab`
**1,292 -> 711**, build-id 20, total differing bytes **82,954 -> 82,556**.
Aligned total (15 functions) **25,570 -> 25,618**.  `check` green: byte_exact
4507/4524, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN.

Tradeoffs kept deliberately:
* `HangarWindow::render` raw rises 296 but the `.ARM.extab` record diff falls
  676 -> 93 (section 1,292 -> 709), a net -287 total-byte win.  The w11520
  operand-order variant (1,970 -> 1,965) was superseded.
* `MGame::OnUpdate` raw rises 1 but aligned falls 111 -> 6 and the
  `Status *volatile st` codegen anchor is removed (human-style C++).

## Cherry-pick interaction (important)

w11501's `expect(player->isDockedToDockingPoint(), 1)` and w11504's hoisted
`Level *self = m_pLevel;` + `resetCamera(self)` in the mission-42 state-7
block interact: the combined shape shrinks `LevelScript::process` to
**57,224 B** (8 B short), which shifts every later function and drops
byte_exact to 3,694.  Keeping the state-7 alias block-scoped (as in w11501)
and using `resetCamera(m_pLevel)` outside restores 57,232 B and raw 53,932.

## Not landed (notes kept)

| worker | target | result |
|---|---|---|
| w11505 | `LevelScript::process` | case-11 translate arg, raw +9 |
| w11506 | `LevelScript::process` | block-order census, no source win |
| w11508 | `Radar::draw` | campaign CSR lever, no raw change |
| w11511 | `Level::createMission` | `combineSelectAndUseCommutative` guard, no code change |
| w11512 | `Level::createMission` | bool alienWave lead, no raw win |
| w11513 | `MGame::OnTouchEnd` | epilogue/preamble + tail 4-cycle, raw unchanged |
| w11515 | `Level::createShip` | switch/bitmask mission guard, aligned worse |
| w11517 | `Status::missionCompleted` | rank-chain/fallthrough producers, no win |
| w11518 | Gun/SpaceLounge/Generator | re-sweep, no source win |
| w11520 | build provenance | round-10 extab census + per-TU knob re-check |
