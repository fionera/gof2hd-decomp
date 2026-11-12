# MGame::OnTouchEnd — wave 191 (w13496): campaign-126 store rotation (raw 2889 -> 2888)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13496`, branch
`wave191-w13496`, base master `f325e2d87`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13496`.

Prior notes: `mgame-ontouchend-wave191-w13497.md` (campaign-15 block move),
`-w13460.md` (81/134 operand swaps), `-w13447.md` / `-w13441.md` (block-65 order),
`-w13421.md` (block-100 getter order, String schedule), `-w13405.md` (chain order,
mining loop), `-w13318.md` (full body).

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,889 | **2,888** |
| aligned (instruction) diff | 1,712 | 1,712 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 71,811 | **71,810** |
| `.ARM.extab` size / diff | 156,312 / 575 | 156,312 / **575** |
| `.note.gnu.build-id` diff | 4 | 4 |
| total (text+extab+build-id) | 72,390 | **72,389** |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**.

## Win — campaign-126 `switch_to_target_setting = 1` store after `initStreamOutPosition`

The campaign-126 arm originally read

```cpp
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::switch_to_target_setting = 1;
Globals::status->field_68 = this->player->player->getGammaHP();
Level::initStreamOutPosition = 0;
```

The original's emitted store order (w13497 analysis) puts the
`switch_to_target_setting = 1` store *after* the
`Level::initStreamOutPosition = 0` store.  Rotating the two independent
global stores (different objects, no aliasing) is behaviour-preserving:

```cpp
Globals::status->field_5c = this->player->player->getShieldHP();
Globals::status->field_68 = this->player->player->getGammaHP();
Level::initStreamOutPosition = 0;
Globals::switch_to_target_setting = 1;
```

Function stays exactly 8,080 B, `.rel.plt` byte-identical, `.ARM.extab`
content unchanged (575), raw **2,889 -> 2,888**, a net -1 total.

## Re-evaluation of the w13479 head-chain 3-cycle (net-negative here)

The stopped w13479 move `[15,162,43] -> [162,43,15]` (order
`74,162,43,15,42,22,65,120,96,100,95,110,81,134,144,126,155,161`) reproduces
in this worktree as raw **2,887** / `.text` 71,809 / `.ARM.extab` **582**,
i.e. net 2,887+582 = **3,469** vs the base's 2,889+575 = **3,464** — a net +5
loss, exactly as w13479 recorded.  It was not landed.

Re-measured on the current base, the w13497 campaign-15-after-162 move
(`74,162,15,43,...`) also gives raw 2,887 / extab 582 (net +5).  Exhaustive
chain-order sweeps in the scratch dir (all 153 pair swaps, 1,610 4-cycles over
positions 0-7, tail 3-cycles, head-fixed rest moves, head/tail 4-cycles, pair
placement, simulated annealing on raw and on raw+extab) confirm the base order
is the unique net minimum at 3,464; every raw-2,887 order has extab 582 and
every lower-extab order (min 566) has raw >= 3,725.  No chain-order
permutation is net-positive.

## Campaign-162 store rotation on this base (not landed)

The w13497 follow-up that moves `Level::initStreamOutPosition = 0` after the
Gamma getter in arm 162 is **raw-hostile on the base without the campaign-15
move**: alone it measures raw 2,893 / extab 575 (net +4).  Applying both
w13497 body rotations together gives raw 2,892 / extab 575.  Only the 126
rotation is a win here, so it is landed alone.

## Reproduction

Scratch (`/home/debian/GalaxyOnFire2/scratch/w13496/`): `build.sh` +
`measure.py` (raw/aligned/size + text/extab/build-id), `chainlib.py` /
`runorder.py` (chain-order render/sweep), `sweep*.jsonl` (exhaustive order
sweeps), `sa_net.py` / `sa_raw.py` (annealing), `runs.py` (residual run map),
`lsdiff2.py` (MGame LSDA record diff).  All builds `cmake --build
cmake-build-match --target link_so -j2` under `ulimit -v 12000000`.
