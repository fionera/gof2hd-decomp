# Wave 169 merge tests (20 agents)

Base master `ec3ceb07c` (wave 168).  All worktrees branched from it, built with
`-j2` and a 12 GB `ulimit -v`.  Each landed row was re-measured on the combined
master tree after cherry-picking, not just on the worker branch.

## Landed

| worker | target | change | raw before -> after | aligned before -> after | size | extab |
|---|---|---|---:|---:|---:|---:|
| w11403 | `LevelScript::process` | mission-0 state-4 `stop(currentMusicEvent)` + state-5 `stop(player->field_0x1c)` | 54,015 -> 54,012 | 17,788 -> 17,787 | 57,232 | 0 |
| w11401 | `LevelScript::process` | mission-94 state-11 hot / message-15 cold expects; drop stale state-4 timer expect | 54,012 -> 53,981 | 17,787 -> 17,796 | 57,232 | 0 |
| w11404 | `LevelScript::process` | narrow-scope `m_pLevel` aliases (state-5 timer, mission-0 msg-10 + state-7 dock, mission-1 case-1) | 53,981 -> 53,970 | 17,796 -> 17,818 | 57,232 | 0 |
| w11407 | `Radar::draw` | six condition-chain `__builtin_expect` cells | 7,486 -> 7,127 | 1,456 -> 1,606 | 9,908 | 668 -> 712 |
| w11410 | `Layout` C1/C2 | tail reorder `0x27c`/`0x285` | 5,656 -> 5,652 | 2,283 -> 2,284 | 7,072 | 0 |
| w11409 | `Layout` C1/C2 | three first-half statement reorderings | 5,652 -> 5,650 | 2,284 | 7,072 | 0 |
| w11414 | `HangarWindow::render` | snapshot `tab` before the ship guard (restores original block order) | 2,245 -> 1,970 | 620 -> 294 | 9,700 | 668 -> 1,247 |

Combined wave-169 result: `.text` **82,327 -> 81,642**, `.ARM.extab`
**668 -> 1,292**, build-id 20, total differing bytes **83,015 -> 82,954**.
Aligned total (15 functions) **25,713 -> 25,570**.  `check` green: byte_exact
4507/4524, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN.

The `HangarWindow` win is a raw/aligned win but a net `.ARM.extab` regression
(record diff 97 -> 676, rows 39-40 keep a different row grouping).  It is kept
because the function raw diff drops 275 bytes and the aligned diff drops 326;
the extab rows are a separate reconstruction target (w11420 round-9 note).

## Not landed (notes kept)

| worker | target | result |
|---|---|---|
| w11402 | `LevelScript::process` | call-histogram re-sweep, no source win |
| w11405 | `LevelScript::process` | mission-78 tail inline: raw 54,015 -> 54,113 (worse), note only |
| w11406 | `LevelScript::process` | block-order census, no source win |
| w11408 | `Radar::draw` | branch-layout / cleanup-pad re-sweep, local optimum 7,486 |
| w11411 | `Level::createMission` | guard icmp de-sink sweep, no source win |
| w11412 | `Level::createMission` | scope/lifetime + convoy-tail re-sweep, no source win |
| w11413 | `MGame::OnTouchEnd` | epilogue / chain-placement sweep, no source win |
| w11415 | `Level::createShip` | declaration/use-order sweep, no source win |
| w11416 | `Hud::draw` | show-order T-first 572 raw but extab shifts, not landed |
| w11417 | `Status::missionCompleted` | (type,flag) + ascending-order measurements, no win |
| w11418 | Gun/SpaceLounge/Generator | re-sweep, no source win |
| w11419 | ModStation/MenuTouchWindow/MGame::OnUpdate | re-sweep, no source win |
| w11420 | build provenance | round-9 flag families + extab rows, no raw win |

## Cherry-pick notes

* w11403 and w11404 both touched the mission-0 state-4 stop line.  Resolved
  by keeping w11403's `stop(currentMusicEvent)` and adding w11404's
  `Level *self = m_pLevel;` alias; the combined tree measures 53,970.
* w11407's docs commit conflicted with the w11414 `.ARM.extab` numbers; the
  wave-169 section of `docs/IDENTICALITY.md` is the combined authoritative
  table.
