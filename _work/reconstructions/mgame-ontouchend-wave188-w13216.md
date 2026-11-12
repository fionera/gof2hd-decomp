# MGame::OnTouchEnd — wave 188 (w13216): mission-162 getter/store transposition

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13216`, branch
`wave188-w13216`, base master `ce16f45f1`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13216`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,929 | **2,928** |
| aligned (instruction) diff | 1,670 | 1,670 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 74,223 | **74,222** |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.extab` size | 156,312 | **156,312** (base exact) |
| `.ARM.extab` diff | 639 | **639** |
| `.ARM.exidx` diff | 0 | 0 |
| build-id diff | 20 | 20 |
| total differing bytes | 74,882 | **74,881** |
| file size | 2,187,328 | **2,187,328** |

Gate after the change (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers, file size 2,187,328 exact.

## The change

Inside the campaign-mission-162 block, the two independent stores

```cpp
Globals::status->field_64 = this->player->player->getHitpoints();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_68 = this->player->player->getGammaHP();
```

were reordered to

```cpp
Globals::status->field_68 = this->player->player->getGammaHP();
Globals::status->field_60 = this->player->player->getArmorHP();
Globals::status->field_64 = this->player->player->getHitpoints();
```

i.e. `field_64 = getHitpoints()` and `field_68 = getGammaHP()` are swapped around
the untouched `field_60 = getArmorHP()` line. `Player::getHitpoints()` /
`getGammaHP()` are pure getters and the three stores target distinct `Status`
fields, so the reorder is behaviour-preserving. It keeps the function exactly
8,080 B and leaves `.rel.plt` byte-identical.

This is the *second* of the two independent mission-162 transpositions w13116
identified (`field_64=getHitpoints()` <-> `field_68=getGammaHP()`); w13116 landed
the first (`field_64=getHitpoints()` <-> `switch_to_target_setting=0`). Stacking
the second on top of the first is the raw 2,929 -> 2,928 byte.

## Why the w13016 chain reorder was not used

The w13016 order `74,15,43,162,...` (and the equivalent `move43->1` order
`74,43,15,162,...`) is also raw 2,928, but it shrinks OnTouchEnd's LSDA from 268
to 264 bytes: the merged `FileRead` call-site range between the mission-15 and
mission-43 blocks is 40 bytes (1-byte uleb) instead of the base order's 174 bytes
(2-byte uleb), so the call-site table length drops 260 -> 259. That makes
`.ARM.extab` 156,308 and shifts every later LSDA (`.ARM.exidx` diff 429,
`.ARM.extab` diff 429) — a net loss. The transposition landed here keeps the
chain order at `74,15,162,43,...` and therefore the LSDA at 268 bytes / 156,312 B,
with `.ARM.extab` content diff unchanged at 639.

## Search context

- Full single-move (306) and pair-swap (153) sweep of the 18 chain blocks from
  the current `74,15,162,43,...` order: the only raw-2,928 orders are
  `74,15,43,162,...` and `74,43,15,162,...`, both with the 156,308 LSDA.
- Full distance-1..4 statement-transposition sweep over every run of consecutive
  same-indentation simple statements in `OnTouchEnd` (87 runs, 363 candidates):
  the unique raw-2,928 / size-8,080 / extab-156,312 winner is this mission-162
  `field_64` <-> `field_68` swap. All other candidates are >= 2,929.
- The original emitted chain order `15,22,43,42,65,81,74,95,96,100,110,120,126,
  134,144,155,161,162` still gives aligned 1,306 but size 8,084 (+4) and raw
  4,273 (address shift); it remains the natural target for a future pass that
  removes the 4-byte dispatch excess.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`; then the 22-byte String-ctor schedule diff at `+0x4a4`
(the `add.w r8, sp, #80` hoist) and its 4-byte literal-pool consequence at
`+0x6b4`. The dominant residual is still the campaign-chain block order/placement
from `+0xe3e`; the tail-merge direction at `0x17bf60` still differs (original
merges into the `isDockingToAsteroid` site). `count != 0` emits the original
`beq.n` but reallocates the loop count to r8 and grows the function to 8,088 B
(size-fatal).
