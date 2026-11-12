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
