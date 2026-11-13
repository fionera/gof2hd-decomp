# MGame::OnTouchEnd — wave191b w13718: chain 15/22 swap + finishMission order

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original
8,080 B @ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree
`wt/w13718`, branch `wave191b-w13718`, base master `4f503f12a`.
Prior notes: `mgame-ontouchend-wave191b-w13692.md` (landed chain order and
store bodies), `-w13681.md`, `-w13672.md`, `-w13662.md`, `-w13626.md`.

## Result (landed)

Two source-order changes on top of w13692:

1. **Campaign arms 15 and 22 swapped.**  The chain now starts
   `22, 15, 43, 42, 95, 65, 81, 74, 96, 100, 110, 120, 126, 134, 144, 155,
   161, 162`.  Mission IDs are mutually exclusive, so the test order is
   semantically irrelevant.  This is the only size-exact single-arm move
   from the w13692 order that improved raw.
2. **`finishMission` order.**  Move `Globals::status->setMission(Mission::empty);`
   and `this->player->setRoute(nullptr);` before the
   `if (this->player->goingToWaypoint()) { setAutoPilot(nullptr); }` block.
   The original emits `removeObjectives; setMission; setRoute;
   goingToWaypoint; setAutoPilot; removeRoute; setPlayerRoute`
   (`0x17b266..0x17b2b0`); the old source emitted `goingToWaypoint` before
   `setMission`.  The resulting `finishMission` block is now
   instruction-for-instruction identical to the original (shifted +6 B
   because the surrounding block layout still differs).

| | raw | aligned | size | insn | `.text` diff | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| w13692 base | 3,650 | 694 | 8,080/8,080 | 2,865/2,865 | 72,114 | 564 |
| finishMission order only | 3,651 | 681 | 8,080/8,080 | 2,865/2,865 | 72,115 | 564 |
| **landed: 15/22 swap + finishMission** | **3,649** | **693** | **8,080/8,080** | **2,865/2,865** | **72,113** | **565** |

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact
4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.ARM.exidx` 0 diff, `.rodata` 0 diff, `.rel.plt` unchanged.  The raw win
is 1 B; `.ARM.extab` content diff grows 564 -> 565 (net-neutral, reported
per the task rule).

## Measured variants (all size-exact unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| w13692 base | 3,650 | 694 | 8,080 |
| finishMission order only | 3,651 | 681 | 8,080 |
| **landed: 15/22 swap + finishMission** | **3,649** | **693** | **8,080** |
| escort block order (field_0x1e0/makeItem/setEquipment before `new Mission`) | 3,662 | 657 | 8,080 |
| escort + first-four-arm permutations (best raw) | 3,662 | 657 | 8,080 |
| instantAction-first `if/else if` | 4,712 | 1,339 | 8,092 |
| fully faithful chain order (65,81,74,95) | 4,277 | 909 | 8,084 |
| arm-95 `sw,64,5c,60,68` (faithful store order) | 4,269 | 882 | 8,088 |
| `count != 0` (faithful mining-report test) | 6,320 | 1,432 | 8,088 |

## Arm-95 store-order sweep

All 120 permutations of the five independent arm-95 stores were built.
Every permutation with `switch_to_target_setting = 1` placed after at least
two getters is size-exact and produces the **same** code image
(raw 3,650, aligned 694); placing it first or immediately after the first
getter is size-fatal (8,088).  Putting it last is raw 3,653.  No arm-95
order beats the landed order.

## Why the tail still differs

The w13692 base already matches the original through `0x17b0c3` except for
the mining-report `count > 0` encoding and one scheduling slot.  From
`0x17b0c4` on, the original and our compiler place the cold blocks
differently: the original puts the `setFreelanceMission` block at
`0x17b0b6`, the common tail at `0x17b182`, the menuTouch
`PlayEngineSound` cold loop at `0x17b1e4`, and `finishMission` at
`0x17b266`; ours puts the `PlayEngineSound` loop first at `0x17b0b6` and
shifts the rest.  Reconstructing the original's instantAction-first
`won` dispatch is faithful but changes the global block layout
(8,072 B with the old `keepObjectives` placement, 8,092 B with the
baseline placement) and loses far more raw than the 56-byte local block
saves.  The fully faithful chain order remains 8,084 B (4 B too big).

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13718`:
`side.py` (side-by-side disasm), `rawmap.py` (per-64-byte raw diff),
`secdiff.py` (section diffs), `sweep95.py` (arm-95 permutations),
`sweep_chain.py` / `sweep_chain2.py` / `sweep_chain3.py` (single-arm
chain-order sweeps), `sweep_first4.py` (first-four-arm permutations),
`base_win1.cpp` (landed source).
