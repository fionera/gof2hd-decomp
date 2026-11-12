# MGame::OnTouchEnd — wave 191 (w13497): campaign-chain 74/162/15 reorder (raw 2889 -> 2887)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13497`, branch
`wave191-w13497`, base master `f325e2d87`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13497`.

Prior notes: `mgame-ontouchend-wave190-w13421.md` (block-100 getter order),
`-w13447.md` (block-65 getter order), `-w13441.md` (block-65 statement order),
`-w13405.md` (chain order / IR mining loop), `-w13378.md` (body reconstruction).

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,889 | **2,887** |
| aligned (instruction) diff | 1,316 | 1,321 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 72,049 | **71,809** |
| `.ARM.extab` size / diff | 156,312 / 582 | 156,312 / 582 |
| `.ARM.exidx` diff | 0 | 0 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| file size | 2,187,328 | 2,187,328 |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**.

## Win — move campaign-15 block after campaign-162

The base chain order is

```
74, 15, 162, 43, 42, 22, 65, 120, 96, 100, 95, 110, 81, 134, 144, 126, 155, 161
```

A full 153-pair swap sweep on the *current* base (the w13441/w13447/w13421 body
edits changed the packing minimum, so the w13405/w13378 sweeps no longer apply)
finds exactly one improvement: move the campaign-15 unit after campaign-162,

```
74, 162, 15, 43, 42, 22, 65, 120, 96, 100, 95, 110, 81, 134, 144, 126, 155, 161
```

The two tests are mutually exclusive (`getCurrentCampaignMission()` is a single
value), so the reorder is behaviour-preserving.  It is size-exact (8,080),
`.rel.plt` byte-identical, and drops raw **2,889 -> 2,887** and `.text`
**72,049 -> 71,809** (-240; the block move re-aligns a whole shifted run).
`.ARM.extab` content is unchanged (582).  The instruction-aligned metric rises
1,316 -> 1,321, but raw is the primary gate and `.text` is a large net win.

All other 152 pair swaps are >= 2,889; the next best is 2,895
(`81`<->`134`).  The base chain order was therefore *not* a strict local minimum
once the three body edits landed.

## Reproduction

`/home/debian/GalaxyOnFire2/scratch/w13497/chain.py` rebuilds the chain from
per-unit source ranges and sweeps permutations (`python3 chain.py pairs`).
All builds `cmake --build cmake-build-match --target link_so -j2` under
`ulimit -v 12000000` (1.2 s per incremental build).

## Win 2 — campaign-126 `switch_to_target_setting` store moved after `initStreamOutPosition`

On top of win 1, a single-statement move sweep over every chain block (move the
`Globals::switch_to_target_setting` store to each position in its block) finds
one improvement in campaign-126: move

```cpp
Globals::switch_to_target_setting = 1;
```

from between the Shield and Gamma getters to after `Level::initStreamOutPosition = 0;`.
Behaviour-preserving (independent stores).  Size 8,080, `.rel.plt` identical,
raw **2,887 -> 2,886**, `.text` **71,809 -> 71,808**, `.ARM.extab` unchanged (582).

The same -1 is also reachable by moving the campaign-162 switch store to
position 1 or 2 (raw 2,886), but the changes are not additive: 162 and 126 both
touch the same diff byte, so only one can be harvested.  The 126 form is the
smallest textual edit (one line moved two slots down).

## Win 3 — campaign-162 `initStreamOutPosition` store moved after the Gamma getter

Same single-statement move sweep, now for `Level::initStreamOutPosition`: in
campaign-162 move

```cpp
Level::initStreamOutPosition = 0;
```

from after the Hitpoints getter to directly after the Gamma getter.  Size 8,080,
`.rel.plt` identical, raw **2,886 -> 2,885**, `.text` **71,808 -> 71,807**,
`.ARM.extab` unchanged (582).

## Negative sweeps on the final base (raw 2,885) — all reverted

All size-exact unless noted; `.rel.plt` identical unless noted.

- **Chain order**: full 153 pair swaps (best tie 2,887), 306 single-block moves
  (best tie 2,887), exhaustive 816 3-cycles (best 2,892).  The order is a strong
  local minimum; do not re-sweep without changing a body first.
- **Getter permutations** (24 per block x 12 blocks): no block beats 2,885.
  Block 162 is completely insensitive to getter order (all 24 = 2,885).
- **Single-statement moves** of `switch_to_target_setting` (best 2,886 for 162/126,
  harvested), `Level::initStreamOutPosition` (best 2,885, harvested), `this->active`
  (tie 2,885), `SetCurrentApplicationModule` (best 2,887), `departStation`
  (best 2,887), `new FileRead` (2,932), `delete reader` (2,889).
- **Adjacent transpositions** over every same-indent simple-statement pair in
  `OnTouchEnd`: many ties at 2,885, no improvement.
- **`won` block**: `instantAction`-first (flat / `else if` / nested / `&&` /
  re-test) is 8,080 but raw 4,462-4,469 and **`.rel.plt` DIFF** (3-slot rotation
  of the finishMission imports).  Re-testing `isInstantActionMission()` in the
  `else if` (no saved bool) is 8,080 / rel.plt OK but raw 2,890.  Swapping the
  `switch=2` / `active=0` stores is 2,886.
- **Mining condition**: `count != 0` / `if (count)` / `amount != 0` / `||` forms
  all flip the induction variable to a positive counter: 8,088 / raw 6,638.
  `count > -1`, `amount < 1`, `amount <= 0` are byte-identical to the base.
  The original's `beq.n` (skip body when `amount == 0`) is unreachable without
  the +8 allocation flip; the base `count >= 0` is semantically different (it
  appends a spurious `0 t ...` line) but is the byte-match minimum.
- **Escort block**: full 720 permutations, base order optimal (2,885).
- **Back-to-station tail**: greedy single-statement hill climb, base optimal.
- **Engine-sound block**: `muted`-first / nested / enabled-first shapes are
  8,076-8,120 and raw 4,036-6,011.
