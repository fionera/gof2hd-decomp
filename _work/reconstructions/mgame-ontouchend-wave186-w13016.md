# MGame::OnTouchEnd — wave 186 (w13016): campaign-chain order

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13016`, branch
`wave186-w13016`, base master `2d02567f6`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13016`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,930 | **2,929** |
| aligned (instruction) diff | 1,704 | **1,672** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 74,402 | **74,401** |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.extab` diff | 647 | 647 |
| total differing bytes | 75,069 | **75,068** |

Gate after the change (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers, file size 2,187,328 exact.

## The change

The campaign-mission `else if` chain was reordered from

```
74, 162, 43, 15, 42, 22, 65, 120, 96, 100, 95, 110, 81, 134, 144, 126, 155, 161
```

to

```
74, 15, 43, 162, 42, 22, 65, 120, 96, 100, 95, 110, 81, 134, 144, 126, 155, 161
```

i.e. the mission-15 and mission-43 `FileRead` blocks were moved ahead of the
mission-162 HP/depart block.  The branches are mutually exclusive on
`getCurrentCampaignMission()`, so the reorder is behaviour-preserving.  It is a
single pair swap (positions 1 and 3) and keeps the function size exactly 8,080 B.
Raw 2,930 -> 2,929; aligned 1,704 -> 1,672.

## Search context

A full pair-swap sweep (153 candidates) and a full single-move sweep (306
candidates) from the wave-185 base order found this as the unique raw improvement.
All adjacent swaps, all pair swaps and all single moves from the new base are
>= 2,929.  The original emitted order
`15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162` remains
size-fatal (+4 B) and raw 4,275 with the current statement orders; its aligned
metric is better (1,303), so it is the natural target for a future pass that
re-tunes the per-block statement order.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`; then the 22-byte String-ctor schedule diff at `+0x4a4`
(the `add.w r8, sp, #80` hoist) and its 4-byte literal-pool consequence at
`+0x6b4`.  The dominant residual is the campaign-chain block order/placement from
`+0xe3e`; the tail-merge direction at `0x17bf60` still differs (original merges
into the `isDockingToAsteroid` site).
