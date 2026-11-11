# MGame::OnTouchEnd — wave 176 (w12012): `bool hasWon` + campaign-first dispatch + `expect` + cold-chain reorder

Status: **code win landed**, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w12012`, branch `wave176-w12012`, base `b21088985`
(wave-175 merge). Target `_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @
`0x17a144`. Scratch `/home/debian/GalaxyOnFire2/scratch/w12012`.

## Result

| | raw | aligned | size | epilogue `popeq` | `.rel.plt` |
|---|---:|---:|---:|---:|---:|
| base | 3,771 | 1,453 | 8,080/8,080 | `0x17ba32` | 0 |
| **landed** | **3,621** | **1,244** | **8,080/8,080** | **`0x17b014`** | **0** |

`.text` differing bytes **80,166 -> 80,016** (-150). Gate after the change (full
`check`, `RATCHET PASS`): byte_exact **4507**, linked_exact **4507**,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370.
The epilogue moves from the end of the function (`0x17ba32`) to the original's
address (`0x17b014`).

## The landed shape (five coupled edits)

1. **`Mission::won` / `Mission::hasWon()` are `bool`** (`src/game/mission/Mission.h`,
   `Mission.cpp`). This is wave-175's lever: with `unsigned char` the `won`
   computation lowers the second test to `icmp eq i8 %x, 1` (50/50, `cmp #1;
   bne.w`); with `bool` it becomes a branch on the i1 value (`cmp #0`). The
   callee body `ldrb r0,[r0,#5]; bx lr` stays 4 B and byte-exact.
2. **Campaign-first mission-completion dispatch** (the wave-173 `C1`/`B` form):
   ```cpp
   if (mission->isCampaignMission()) {
       Globals::status->nextCampaignMission(true);
   } else if (!mission->isInstantActionMission()) {
       Globals::status->setFreelanceMission(Mission::empty);
       Globals::layout->showMissionRewardMessage(mission->getReward() + mission->getBonus(), false);
   } else {
       Globals::switch_to_target_setting = 2;
       this->active = 0;
       this->applicationManager->SetCurrentApplicationModule(1);
       return;
   }
   ```
   (base is instant-first). This is what puts the epilogue at `0x17b014`; the
   dispatch itself differs from the original by the deliberate campaign/instant
   swap at `0x17af7e..0x17afbe`.
3. **`hudTouchFlags == 0` body store order** (wave-174's RS4): `this->pauseOpen = 0;`
   before `resumeSounds();`.
4. **Campaign mission-id chain reordered** into the original binary's cold-block
   order. The landed order is
   `22, 15, 43, 42, 74, 65, 95, 81, 96|127, 100, 162, 110, 120, 126, 134, 161, 144, 155`
   (each block is an independent `else if` on
   `mission->isCampaignMission() && getCurrentCampaignMission() == N`; the order
   is semantically irrelevant, it only controls cold-block placement). The
   `96 || 127` block stays combined.
5. **`__builtin_expect(mission->isCampaignMission(), 1)` on the `== 15` check**
   (same-line `// lint: arm_builtin` waiver). This is the new lever: it raises
   the chain's block frequency so MBP places the chain earlier in the cold
   region, which is what makes the reordered chain reach 8,080 B and drops the
   same-address diff by a further ~100 B.

The chain reorder alone (on the wave-175 base shape) is size 8,080 but raw
3,843 (a +72 regression). The `expect` is what turns it into a win.

## Why the `expect` helps

The cold blocks after the epilogue are placed by MachineBlockPlacement's
`BlockFilter`, which iterates blocks in decreasing frequency order (ties broken
by IR block number). The mission-id chain is deep in the `won` path and is
otherwise placed after the `soundsMuted` Play block and the freelance block.
Biasing the first chain check (`isCampaignMission()` on the `== 15` arm) raises
the chain's frequency enough to move it ahead of those blocks, matching the
original's cold order up to the chain. Measured effect on the wave-175
chain+bool+B shape: raw 3,843 -> 3,737 (win1), then 3,621 after the chain
reorder sweep.

## Chain-order sweep (all size 8,080, `.rel.plt` 0, epilogue `0x17b014`)

Starting from the wave-175 "layout order"
`15,22,43,42,65,81,74,95,96,127,100,110,120,126,134,144,155,161,162`
(raw 3,843 with `expect`), greedy adjacent-swap descent:

| order | raw |
|---|---:|
| layout order + expect | 3,843 |
| 161 before 95 (o4) | 3,712 |
| 161 before 81 | 3,695 |
| 162 before 110 | 3,685 |
| 161 before 144 | 3,672 |
| 15/22 swapped | 3,662 |
| 74/65/81 shuffled | 3,659 |
| **22,15,43,42,74,65,95,81,96,127,100,162,110,120,126,134,161,144,155** | **3,621** |

The last order is a local minimum under adjacent swaps (18 neighbours all
>= 3,621; the next best is 3,631). The original binary's own chain order is
`15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,155,161,162`
(96 and 127 are separate checks; the 127 check at `0x17bc30` jumps to the
shared 96 body at `0x17b722`). Splitting 96/127 in the source gives size
8,092/8,100 (not landable), so the `96 || 127` combined block is kept.

## Sweeps that did not beat the base (all measured on the wave-175 base)

* `expect` on every other dispatch/`won`/`hudTouchFlags`/`hudMenuOpen`/
  `pauseOpen`/`autopilot`/`cameraMode` test: best 3,771 (no change), most
  change size.
* `expect(freeCamMode)`, `expect(refinedAny)`, `expect(soundsMuted)`,
  `expect(menuTouchOpen)`, `expect(cutsceneActive)`, `expect(pauseOpen)`,
  `expect(menuWindow->OnTouchEnd(...))` on the chain+bool+B shape: all worse
  (3,843..7,758) or size-changing.
* `if (refinedAny)` branch swap, `else if (soundsMuted != 0)`, moving the
  `freeCamMode` block after the `soundsMuted` declaration: no change (3,843).
* Nested `if (!IsCategoryEnabled(2)) { if (soundsMuted) Play else Stop } else
  if (soundsMuted) Play else Resume` (the original's apparent source shape):
  4,012 / 8,120 (the compiler does not tail-merge the duplicated Play block).
* `goto`-shared Play block: 4,394 / 8,084.
* `soundsMuted`-first reorder: 6,035 / 8,072.
* `expect` on the dispatch `isCampaignMission`, the `keepObjectives` ternary,
  the mission-failed `isCampaignMission`, the escort check, the instant check:
  all worse or size-changing.
* Splitting the `96 || 127` block into two separate checks (original order,
  `127` after `126`): 4,014 / 8,092.

## Remaining residual

Hot region `0x000..0xed0`: **112** differing bytes (25 runs; branch targets,
the `count >= 0` `beq` vs `bgt` at `0x048b`, the 22-byte `String`-ctor
scheduling at `0x04a4..0x04ba`, and the deliberate dispatch swap).
Cold region `0xed0..0x1f90`: **3,509** differing bytes. The first cold
difference is the `freeCamMode` test / `getText(292)` block swap (ours puts
`getText(292)` at `0x0f0c`, the original at `0x0f18`), then the `soundsMuted`
Play block (ours `0x0f72`, original `0x10a4`), which shifts the whole chain
region by `0x38`. Fixing those two block placements is the next lever.

## Reproduction

```
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12012/MGame.chain_z7.cpp src/game/menu/MGame.cpp
# Mission.h: uint8_t won -> bool won; unsigned char hasWon() -> bool hasWon()
# Mission.cpp: unsigned char Mission::hasWon() -> bool Mission::hasWon()
# MGame.cpp: campaign-first dispatch, pauseOpen-before-resumeSounds,
#            __builtin_expect(mission->isCampaignMission(), 1) on the ==15 check
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j2
```

Scratch helpers: `gen_chain.py <name> <id,id,...>` writes
`MGame.chain_<name>.cpp` from the base MGame with the campaign chain reordered;
`runvar.py <json>` applies `{name, copy:{mgame,missionh,missioncpp},
reps:[[file,old,new],...]}`, builds with `-j2`, prints
raw/aligned/size/epilogue/relplt, restores the base files; `measure.py` prints
JSON metrics; `ranges.py <so>` prints same-address diff runs; `blocksdump.py`
/ `blockdiff.py` print the cold basic-block order. The landed MGame is
`MGame.chain_z7.cpp`; the landed Mission files are `Mission.h.s4` /
`Mission.cpp.s4`.
