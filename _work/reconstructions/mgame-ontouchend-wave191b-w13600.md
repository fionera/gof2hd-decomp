# MGame::OnTouchEnd — wave191b w13600: statement-order / declaration / chain-order sweep (note only)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13600`, branch
`wave191b-w13600`, base master `d6348b79b`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13600`.

Prior notes: `mgame-ontouchend-wave191-w13571.md` (pre-chain/block-order),
`-wave191b-w13577.md` (case-100 order), `-w13539.md` (back-to-station tail).

## Result

Base unchanged. `check` green (byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370). `MGame::OnTouchEnd` raw **2,878**, aligned **1,661**,
size **8,080/8,080**.

Note: the task brief quoted raw 2,882 / aligned 1,561 — that predates the landed
w13577 case-100 permutation (`473b034c7`), which traded aligned +100 for raw -4.
The current base is 2,878 / 1,661.

## New evidence

### 1. `won` / `keepObjectives` / `mission` in the base are exact

Original `0x17adc8`..`0x17ae06` decodes to exactly the base's three declarations:

```
won            = getMission()->hasWon() || getCampaignMission()->hasWon()
keepObjectives = getCampaignMission()->hasWon() ? !getMission()->isCampaignMission() : false
mission        = getMission()->hasWon() ? getMission() : getCampaignMission()
```

(`cbnz` on the first `hasWon`, `beq 0xb0aa` to `resetStartSequenceOver`, `eor sl,r0,#1`
for `keepObjectives`). No change needed.

### 2. The single structural divergence is at `0xaf82` (offset `0xe3e`)

The base is **byte-identical to the original from `0x17a144` to `0x17af82`** (all
earlier diffmap hits are branch/pool offsets, not different instructions). At
`0xaf82`:

```
orig: bl isInstantActionMission; cbz r0,0xafa6; <instant body 0xaf84..0xafbe>; if (campaign) ...
base: bl isInstantActionMission; mov r4,r0; mov r0,r5; bl isCampaignMission; cbz ...
```

i.e. the original tests `isInstantActionMission()` first with the body as the
fall-through; the base tests `isCampaignMission()` first and keeps `instantAction`
live in `r4` for the `else if (!instantAction)`.

Every source form that emits instant-action-first (separate `if`, `else if`,
nested, inverted) gives raw **4,467..5,345**, because the *global* block layout
shifts: the starMap-exit block moves from `0xaf32` to `0xaf7e`, so the
byte-identical pre-`0xaf82` prefix is lost. The base's campaign-first form is the
strict local minimum even though it is locally "less faithful".

### 3. Chain physical order == source order (confirmed both sides)

Original `getCurrentCampaignMission` + `cmp` sites, ascending address:

```
15,22,43,42,65,81,74,[cinematic switch],95,96,100,110,120,126,127,134,144,155,161,162
```

Ours, ascending:

```
74,162,15,43,42,22,65,[switch],120,96,127,100,95,110,81,134,144,126,155,161
```

which is exactly the base source order. So clang 7 lays the arm chain out in
source order; the original source order was the logical mission order. But
re-ordering to it is **size-fatal**: the faithful order gives size 8,088
(and raw 4,333), and 120 random chain permutations were all size-fatal. The base
order is one of the few size-exact orders.

### 4. Exhaustive arm-body sweeps (all on this base)

All size-exact unless noted; `.rel.plt` unchanged.

| sweep | result |
|---|---|
| 95/96/100/110/126: all 72 single-statement moves each | best raw 2,878 (tie) |
| 95/96/100/110/126: all 36 pair swaps each | best raw 2,878 (tie) |
| 155: all 120 permutations | every permutation 2,878 / 1,661 (fully saturated) |
| case 0: all 24 tail permutations | base `active,switch,init,module` unique raw min 2,878; next 2,880 |
| case 1: all 6 permutations | base `module,switch,active` = 2,878; others 2,882 or 8,076 fatal |
| 16 pre-chain forms (instant-first × else-swap × nested × inverted × body order) | best tie 2,878 (`swapelse` with base body order canonicalises to base); instant-first 4,467+ |
| chain order: faithful original + 120 random permutations | all size-fatal (8,088) except base |

### 5. Declaration / block-scope moves

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,878 | 1,661 | 8,080 |
| `bool won` inlined into `if (...)` | 2,878 | 1,661 | 8,080 |
| `Mission *mission` declared before `keepObjectives` | 4,615 | 1,714 | 8,084 |
| `instantAction` declared before `keepObjectives` | 4,667 | 1,718 | 8,088 |
| drop `instantAction` var, inline `!mission->isInstantActionMission()` | 2,883 | 1,660 | 8,080 |
| `OnTouchEnd(...) == 1` -> `!= 0` / `> 0` | 2,878 | 1,661 | 8,080 (identical code) |
| `player != nullptr` -> `player` | 2,878 | 1,661 | 8,080 (identical code) |

### 6. `soundsMuted` block restructures (the block whose placement shifts the chain)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,878 | 1,661 | 8,080 |
| test `soundsMuted` first | 6,006 | 1,806 | 8,076 fatal |
| nested `if (!IsCategoryEnabled) { if(muted) Play else Stop } ...` | 4,037 | 1,682 | 8,120 fatal |
| hoist `enemies = getEnemies()` out of the if/else | 4,007 | 1,668 | 8,060 fatal |
| reorder `!enabled && !muted` -> `!muted && !enabled` | 6,032 | 1,812 | 8,088 fatal |

The base structure (`if (!enabled && !muted) Stop; else if (muted) Play; else Resume`)
is semantically identical to the original's disassembly (`cmp r1,#0; beq 0xb1e8`;
`0xb1e8: cmp r2,#0; beq Stop; Play`), so the difference is purely
MachineBlockPlacement of the cold Stop/Play blocks.

### 7. Mining-loop condition and report expression

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base `if (count >= 0)` | 2,878 | 1,661 | 8,080 |
| `if (amount < 0)` / `if (count > 0)` | 2,880 | 1,662 | 8,080 |
| `if (count != 0)` / `if (amount != 0)` | 6,638 | 2,188 | 8,088 fatal |
| report `+=` split into 2/3/4 statements | 6,437..6,581 | — | 8,056..8,064 fatal |
| `refined + 1274` / single-line | 2,878 | 1,661 | 8,080 (identical) |

Original emits `cmp r9,#0; beq 0x17a652` (skip report when `count == 0`); the base
emits `bgt`. The semantically-correct `!= 0` form is +8 B (fatal) and the `> 0`
form is +2 raw, so the 1-byte local mismatch cannot be bought without a net loss.

### 8. `__builtin_expect` on the pre-chain tests

`expect(campaign,0/1)` and `expect(!instantAction,0/1)` are all size-fatal
(8,084..8,092) and raw 4,251..4,756.

## Conclusion

The base is a strict local minimum for every statement-order, declaration,
block-scope and chain-order move tested. The remaining 2,878 raw bytes are
dominated by *global block placement*: branch-offset bytes and pc-relative
literal-pool offsets differ because a handful of cold blocks (the pause-menu
Play/Stop loops, the arm-144 tail, the EH cleanup landing pads) are placed in a
different order than the original. The one structural divergence (`0xaf82`,
instant-action-first) is locally correct but cannot be reached without perturbing
the byte-identical `0x17a144..0x17af82` prefix, which costs far more than it
saves. Further progress needs a way to pin MachineBlockPlacement's cold-block
order (e.g. reconstructing the exact original top-level statement order around
the pause-menu `soundsMuted` handling), not arm-body permutations.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13600`: `measure.sh` (build + fn_diff),
`armtool.py` (arm body extract/permute), `climb.py` (single-statement hill climb),
`brute.py` (exhaustive perm), `pairswap.py`, `prechain.py`/`pre2.py` (pre-chain forms),
`decl.py`, `sounds.py`, `hoist.py`, `case0.py`/`case01.py`, `chain.py`/`chainsearch.py`,
`diffmap.py`, `blocksig.py`, `blockdiff.py`, `seqdiff.py`.
