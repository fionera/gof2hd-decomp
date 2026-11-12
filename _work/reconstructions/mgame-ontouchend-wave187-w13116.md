# MGame::OnTouchEnd — wave 187 (w13116): chain reorder + 162-block store swap

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13116`, branch
`wave187-w13116`, base master `8b5d4281c`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13116`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,930 | **2,929** |
| aligned (instruction) diff | 1,704 | **1,670** |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 74,274 | **74,273** |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.extab` size | 156,312 | **156,312** (base exact) |
| `.ARM.extab` diff | 647 | **640** |
| `.ARM.exidx` diff | 0 | 0 |
| build-id diff | 20 | 19 |
| total differing bytes | 74,941 | **74,932** |
| file size | 2,187,328 | **2,187,328** |

Gate after the change (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers, file size 2,187,328 exact.

## The change

Two behaviour-preserving source edits, both in the campaign-mission `else if` chain:

1. **Chain order** — the mission-15 `FileRead` block was moved ahead of the
   mission-162 block.  The chain order changes from

   ```
   74, 162, 43, 15, 42, 22, 65, 120, 96, 100, 95, 110, 81, 134, 144, 126, 155, 161
   ```

   to

   ```
   74, 15, 162, 43, 42, 22, 65, 120, 96, 100, 95, 110, 81, 134, 144, 126, 155, 161
   ```

   The branches are mutually exclusive on `getCurrentCampaignMission()`, so the
   reorder is behaviour-preserving.  This alone is raw 2,930 (unchanged), aligned
   1,670, `.text` 74,274, `.ARM.extab` 156,312 / diff 640 (7 bytes better than base).

2. **Mission-162 block statement swap** — inside the 162 block,

   ```cpp
   Globals::status->field_64 = this->player->player->getHitpoints();
   Globals::switch_to_target_setting = 0;
   ```

   was swapped to

   ```cpp
   Globals::switch_to_target_setting = 0;
   Globals::status->field_64 = this->player->player->getHitpoints();
   ```

   `Player::getHitpoints()` is a pure getter and `switch_to_target_setting` is a
   namespace-scope store, so the swap is behaviour-preserving.  This is the raw
   2,930 -> 2,929 byte.

Both edits keep the function size exactly 8,080 B and leave `.rel.plt` byte-identical.

## Why the w13016 reorder was not landed

w13016's reorder (move 162 *after* 15/43, i.e. `74, 43, 15, 162, ...`) is also raw
2,929, but it shrinks OnTouchEnd's LSDA from 268 to 264 bytes: the call-site-table
gap entry between the mission-74 `FileRead` ctor and the mission-43 ctor is 72 bytes
(5-byte uleb length) instead of the base's 202 bytes (6-byte uleb length), so the
table length drops 260 -> 259 and the 4-byte-aligned LSDA loses 4 bytes.  That makes
`.ARM.extab` 156,308 (file size 2,187,324), shifts every later LSDA and produces
`.ARM.exidx` diff 429 / `.ARM.extab` diff 7,848.  Net loss ~7,629 bytes.  The
variant landed here instead moves 15 *before* 162 (the mission-43/15 ctors stay in
the same relative position), which keeps the table length 260 and the LSDA at 268
bytes, and then the 162-block store swap supplies the raw byte.

## Search context

- Full single-move sweep of the 18 chain blocks from the base order (306 variants):
  only `162 -> after 15` gave raw 2,929; every such order has the 156,308 LSDA.
- All 720 permutations of the first six chain blocks: the raw-2,929 orders are
  exactly the two `{74,43,15,162}` / `{74,15,43,162}` orders (both 156,308).
- 344 distance-1..4 statement transpositions inside the chain blocks on the
  `74,15,162,43,...` order: the unique raw-2,929 winners are the two 162-block
  swaps `field_64=getHitpoints()` <-> `switch_to_target_setting=0` and
  `field_64=getHitpoints()` <-> `field_68=getGammaHP()`; both keep `.ARM.extab`
  156,312 / diff 640 and `.ARM.exidx` diff 0.  The first was landed.
- Reordering the chain to the original emitted order
  `15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162` gives aligned
  1,303 but size 8,084 (+4, gate-fatal) and raw 4,275 (address shift).

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original emits
`beq.n` at `+0x48b`; then the 22-byte String-ctor schedule diff at `+0x4a4` (the
`add.w r8, sp, #80` hoist) and its 4-byte literal-pool consequence at `+0x6b4`.
The dominant residual is still the campaign-chain block order/placement from
`+0xe3e`; the tail-merge direction at `0x17bf60` still differs (original merges into
the `isDockingToAsteroid` site).  `count != 0` / `count` / `amount != 0` all emit the
original `beq.n` but grow the function to 8,088 B (size-fatal).
