# `MGame::OnTouchEnd` — wave 177 (w12115): `== 1` branch-weight fix for the freeCam tail

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp`
(original 8,080 B @ `0x17a144`). Worktree `/home/debian/GalaxyOnFire2/wt/w12115`,
branch `wave177-w12115`, base `9445ac5fb` (wave-176 merge). Scratch
`/home/debian/GalaxyOnFire2/scratch/w12115`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | **3,621** | **3,581** |
| aligned (instruction) diff | 1,205 | **1,194** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| epilogue `popeq` (ours) | `0x17b014` | `0x17b014` |
| `.rel.plt` diff | 0 | 0 |

Gate after the change (full `check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, data_exact **843/843**,
imports **207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370**.
`.text` differing bytes **79,591 -> 79,551** (-40); `.ARM.extab` and
`.rel.plt` unchanged.

## The change

```cpp
uint8_t &soundsMuted = reinterpret_cast<uint8_t *>(&this->field_0x1a4)[0];
// The window returns 0/1; testing `== 1` keeps this branch 50/50
// (a plain `!= 0` test is predicted 5:3 by the zero heuristic).
if (this->menuWindow->OnTouchEnd(x, y, touchId) == 1) {
```

`MenuTouchWindow::OnTouchEnd` returns `int` but only ever returns 0 or 1
(`src/game/ui/MenuTouchWindow.cpp`: every `return` in the body is `return 0;`
except the cinematic-steer case 18, which is `return 1;`), so `== 1` is
semantically identical to the old truthiness test.

## Why it works

The cold blocks after the epilogue are laid out by MachineBlockPlacement's
greedy chain walk (`selectBestSuccessor` / `selectBestCandidateBlock`), driven
by `BranchProbabilityInfo`. The old test lowered to `icmp ne i32 %call, 0`,
which `calcEstimatedHeuristics`/`calcZeroHeuristics` predicts 5:3 (the
non-zero "then" edge gets 62.5%, the "else" edge 37.5%). The freeCam tail
block (`if (!this->freeCamMode && this->menuWindow->inCinematicMode())`, the
`else` of the call) therefore got only 3/8 of the menuTouchOpen path and MBP
placed it *after* the mining-plant `getText(292)` block:

```
base  0xf0c: getText(292) block       0xf2e: freeCam test
orig  0xf0c: freeCam test             0xf18: getText(292) block
```

Changing the comparison to `icmp eq i32 %call, 1` leaves the constant non-zero,
so no zero heuristic matches and BPI falls back to 50/50. The freeCam block's
frequency rises above `getText(292)`'s and MBP now emits the original order.
The two blocks total 50 B either way, so everything after them stays aligned;
the only new hot-region cost is the `cmp r0,#1; bne.w` vs `cmp r0,#0; beq.w`
pair at `0xd88/0xd8a` (2 bytes). Net -40 raw, -11 aligned.

MIR evidence (`-mllvm -print-machineinstrs`, section 45 = after "Branch
Probability Basic Block Placement"; section 55 = final; block positions are
the MIR layout order, frequencies from a Gauss-Seidel solve of the printed
successor weights):

| block | IR | base pos | after pos | after freq |
|---|---|---:|---:|---:|
| freeCam tail | ir.1745 | 243 | **240** | 6.70e-4 |
| getText(292) | ir.466 | 240 | 241 | 5.80e-4 |
| Play (`soundsMuted`) | ir.1665 | 248 | 248 | 4.19e-4 |
| `setFreelanceMission` | ir.864 | 252 | 252 | 8.60e-4 |

## Remaining residuals (not landed)

1. **Play vs `setFreelanceMission` order.** Ours emits the `soundsMuted`
   Play block at `0xf78` and the mission-completion `setFreelanceMission`
   block at `0xfaa`; the original emits `setFreelanceMission` at `0xf72`
   (right after `resetStartSequenceOver`) and Play at `0x10a4`, i.e. after
   the first few campaign-chain blocks. This ~54 B shift is the dominant
   cause of the remaining cold-region diff. Both are separate MBP chains;
   the Play chain head (ir.1639/1665) is placed before the `setFreelanceMission`
   chain (ir.864) even though the latter's modelled frequency is higher, so
   the lever is chain-worklist order, not raw frequency. Tried without a
   size-exact win: moving the whole `soundsMuted` block to 9 different
   statement positions (best is still the base position), reordering the
   `if/else if/else` sound arms (raw 5,693), `soundsMuted == 1` (size 8,084),
   `soundsMuted & 1` (size 8,084).
2. **Campaign-chain order.** Ours is `22,15,43,42,74,65,95,81,96|127,100,162,
   110,120,126,134,161,144,155`; the original binary's block order is
   `15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,155,161,162`.
   A full adjacent-swap sweep on the new base found no raw improvement below
   3,581 (ties at swaps 1-2 and 12-13). The original order with 96|127 kept
   combined measures raw 3,697 / aligned 796; splitting 96/127 costs +12 B
   (size 8,092, fatal).
3. **Hot-region content residuals.** String-ctor scheduling at
   `0x4a4..0x4ba` (22 B: `add r8,sp,#80` hoisted before the first
   `operator+` in ours), the deliberate campaign/instant dispatch swap at
   `0xe3a..0xe7a` (~65 B, w12114's area), and branch-displacement bytes at
   `0x52c/0x57c/0x6b4/0xba4/0xc22..0xc50/0xdba/0xdc0/0xdd4/0xdec` plus the
   literal-pool bytes at `0x6b4/0xe90/0xeb6`.

## Other measured shapes (all size 8,080 / `.rel.plt` 0 unless noted)

| experiment | raw | aligned | note |
|---|---:|---:|---|
| base (wave-176) | 3,621 | 1,205 | |
| **`mto_eq1` (landed)** | **3,581** | **1,194** | |
| `count != 0` / `count` / `amount != 0` | 6,588 | 1,597 | size 8,084 |
| `count > 0` | 3,623 | 1,206 | |
| `won` `expect(...,1)` | 3,744 | 1,048 | size 8,076 |
| dispatch instant-first | 4,751 | 1,478 | size 8,076 |
| dispatch instant-first `== 1` | 4,802 | 1,444 | size 8,084 |
| dispatch nested `!campaign` | 3,581 | 1,194 | codegen-identical |
| `camp` `expect(...,1)` | 4,269 | 1,177 | size 8,088 |
| `ia` `expect(...,1)` | 4,766 | 1,252 | size 8,080, relplt 0 |
| `_bc8` store reorder (3 forms) | 3,623-3,624 | 1,207 | |
| freeCam tail-test swap | 3,893 | 1,069 | size 8,072 |
| `soundsMuted` block relocation (9 sites) | 4,016 min | 1,213 | |
| String expression variants (v4) | 3,619 | 1,332 | |
| chain `orig_comb` | 3,697 | 796 | |

All builds used `ulimit -v 12000000` and `-j2`.

## Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w12115
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 /home/debian/GalaxyOnFire2/scratch/w12115/measure.py   # 3581/1194/8080/0
cmake --build cmake-build-match --target check -j2            # RATCHET PASS
cmake --build cmake-build-match --target drift -j2            # 0 fields
```

Scratch helpers: `measure.py`, `runvar.py`, `dis.py`, `hotruns.py`,
`chainreorder.py`/`runchain.py` (campaign-chain permutation), `smmove.py`
(`soundsMuted` block relocation), `dumpmir.sh`, `mirorder.py`.
