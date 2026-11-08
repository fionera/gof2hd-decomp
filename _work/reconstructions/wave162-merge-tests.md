# Wave 162 merge tests (coordinator)

Base: `29248c1d0` (wave-161 final), gate green (byte_exact 4507/4524,
`.text` 85,374, `.ARM.extab` 725, `.rel.plt` byte-identical, aligned
total 24,178).

Baseline on the base tree:

| fn | raw | aligned | size |
|---|---:|---:|---:|
| `Layout::Layout` | 5,715 | 2,050 | 7,072 |
| `LevelScript::process` | 55,013 | 17,308 | 57,232 |
| `Hud::draw` | 1,233 | 256 | 9,332 |
| `MenuTouchWindow::OnTouchBegin` | 29 | 15 | 1,896 |

## Landed

* **w10803** (LevelScript: PLT-safe `__builtin_expect` cells
  `L1030:1`, `L1430:1`, `L1522:0`, `L1526:1`): raw **55,013 -> 54,698**
  (-315), aligned **17,308 -> 17,456** (+148), 57,232 B exact,
  `.rel.plt` byte-identical.  The whole w10705 expect table was re-swept
  with a `.rel.plt` byte check after every cell; `L1526:0` (the old
  w10705 winner) is now +16 B / 16 `.rel.plt` bytes on this base.
* **w10807** (Layout: `field_0x94` tail relocation): raw
  **5,715 -> 5,705** (-10), aligned **2,050 -> 2,082** (+32), 7,072 B
  exact.
* **w10817** (Hud: swap `barH`/`barW` declaration order in the
  docking-transfer bar): raw **1,233 -> 1,227** (-6), aligned
  **256 -> 248** (-8), 9,332 B exact.
* **w10818** (MenuTouchWindow: case-15 `top` from the reloaded
  `Globals::layout`): raw **29 -> 28** (-1), aligned **15 -> 16** (+1),
  1,896 B exact.

Totals after the landings: `.text` **85,042** (-332), `.ARM.extab` **725**
(0), `.rel.plt` 0, `.note.gnu.build-id` 20, total differing bytes
**85,787** (was 86,119, -332).  Aligned total **24,351** (+173): the rise is
w10803's block re-layout (+148) and w10807 (+32), partly offset by w10817
(-8).  `check` is green, byte_exact 4507/4524.

## Measured, not landed

| candidate | raw | aligned | size | why not |
|---|---:|---:|---:|---|
| w10801 (LevelScript `Status *&` + `stars4`) | 54,996 | 17,311 | 57,232 | does not combine with w10803 (57,224 B) |
| w10802 (LevelScript `Status *&` per-use reload) | 55,014 | 17,295 | 57,232 | +1 raw for -13 aligned; w10803 dominates |
| w10804 (LevelScript case-125 state-3 else-if) | 55,016 | 17,066 | 57,232 | does not combine with w10803 (57,248 B); best aligned but +3 raw |
| w10805 (LevelScript 0.8f tail block) | 55,013 | 17,308 | 57,232 | analysis: the 0.8f block is unreachable; moving it out is -20 B but rotates `.rel.plt` |
| w10806 (Layout whole store-order dump) | 5,715 | 2,050 | 7,072 | analysis: the 128-B residual is a code/pool split (6,186+886 vs 6,314+758); the n9 fast path is the next lever |
| w10808 (Radar faithful tail) | 8,367 | 1,333 | 9,908 | worse on both |
| w10809 (Radar `campaign`->`sl`) | 8,124 | 1,320 | 9,908 | analysis only |
| w10810 (MGame::OnTouchEnd 23rd `isCampaignMission`) | 5,816 | 1,379 | 8,080 | +1,689 raw for the 23rd call; 130-B epilogue shift |
| w10811 (MGame::OnTouchEnd `bool Mission::hasWon`) | 6,053 | 1,085 | 8,080 | `cmp #0;beq` at 8,080 B / relplt 0 but +1,926 raw |
| w10812 (createMission stack-slot permutation) | 5,454 | 786 | 17,172 | analysis only |
| w10813 (createMission case-183 if-conversion) | 5,454 | 786 | 17,172 | analysis: `Reassociate` swaps the `and` operands; the original's PGO order is not source-reachable |
| w10814 (createShip case order) | 1,560 | 460 | 2,764 | raw -3, aligned +11 |
| w10815 (Status nested dispatch) | 722 | 320 | 1,432 | analysis only |
| w10816 (HangarWindow `cbz` sweep) | 2,514 | 485 | 9,700 | no size-exact `cbz` shape |
| w10819 (MGame::OnUpdate last 37) | 37 | 109 | 11,440 | no source win |
| w10820 (build provenance round 3) | — | — | — | profile-reconstruction harness + note |

## Key open questions for wave 163

1. **LevelScript**: w10803's four expect cells are the best raw result
   (54,698). The remaining aligned cost is block re-layout; the
   `L1389:1`/`L1364:0` pair reaches 54,546 but at 57,208 B (24 B short).
   Find a -24 B companion.
2. **Layout**: the 128-B residual is a code/pool split; restructure the
   first 8 groups as an explicit `if (iPadHD) ... else if (n9) ... else`
   chain so the compiler shares the rest of the constructor.
3. **Radar**: the 4-B faithful tail is still open; the `campaign`->`sl`
   permutation is the other half.
4. **MGame::OnTouchEnd**: the 23rd `isCampaignMission` call is the 96/127
   split, but it shifts the shared epilogue by 130 B.
5. **createMission**: the `and` operand order is pinned by `Reassociate`;
   the original's PGO order is the only remaining lever.
6. **Build provenance**: round 3 built a profile-reconstruction harness
   (`prof_reconstruct_w10820.py`); the next step is to feed a synthetic
   profile to clang 7 and see if it reproduces the block layout.
