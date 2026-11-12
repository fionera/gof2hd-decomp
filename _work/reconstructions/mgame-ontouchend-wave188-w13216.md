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

## Continuation (w13216 second session): further searches, all negative

All numbers below are on top of the landed pass-14 state (raw 2,928, aligned 1,670,
size 8,080, `.ARM.extab` 156,312 B). Every candidate was rebuilt and measured with
`tools/fn_diff.py` + `scratch/w13216/secdiff.py`; none was landed.

### Distance-5..8 statement-transposition sweep

Full sweep over every pair of same-indentation simple statements at distance 5..8
inside `OnTouchEnd` (158 candidates; `scratch/w13216/sweep_trans58.py`, log
`sweep_trans58.log`). Minimum raw is **2,928** (e.g. `swap 1605,1610`,
`swap 1743,1748`); no candidate reaches 2,927. Combined with the earlier
distance-1..4 sweep (679 candidates, `sweep_trans.json`), every single
transposition of two simple statements in the function is now covered. The landed
mission-162 `field_64`/`field_68` swap is the only transposition that lowers raw
below the pre-pass-14 2,929; every other candidate is >= 2,928 (neutral reorder)
or changes size / `.ARM.extab`.

### `if (count >= 0)` -> `if (count != 0)` (the +0x48b branch opcode)

The original emits `beq.n` at `+0x48a` (skip when `amount == 0`); our
`count >= 0` emits `bgt.n`. Writing `count != 0` (semantically identical here,
because `count = -amount` and `amount <= 0`) does produce the original `beq.n`,
but it changes the *global* register allocation: from the first instruction the
compiler swaps `sl` and `r9` (`mov sl, fp` vs `mov r9, fp`), grows the function
to **8,088** B and gives raw 6,591 / aligned 2,175. Fatal on size; not landed.

### String-expression variants (the +0x4a4 22-byte schedule diff)

The residual 22-byte region is the scheduler placing `add.w r8, sp, #80`
(the `String("t ")` temporary address) before the first `operator+` instead of
after it; the `+0x6b4` 1-byte pool-word diff is a consequence of that schedule
(the pool word differs by 4 because the `add r1, pc` is at a different address).
Equivalent source rewrites were tried and all leave the schedule unchanged
(raw 2,928 / aligned 1,670): `getText(refined + 1274)`, `String(-amount)`,
`0 - amount`, `-1 * amount`, `const char *sep = "t "`, `const char *nl = "\n"`,
`const int count`, `0 <= count`, `!(count < 0)`, `count > -1`, `-amount >= 0`.
Rewrites that do change the expression tree change the size and lose:
named `String line` temp -> size 8,080 but raw 6,566; right-assoc grouping ->
8,072; mid-group -> 8,076; separate `+=` statements -> 8,016; `report = report + ...`
-> 8,056.

### Structural finding: `== 127` is a separate chain block in the original

The original does **not** merge missions 96 and 127 into one condition. Its 96
block is at `0x17b70c` (`cmp r0, #96`, body at `0x17b722`), and there is a
separate `== 127` block at `0x17bc24` (between 126 and 134) whose
`beq.w 17b722` jumps into the shared 96 body (compiler tail-merge of two
identical source bodies). Our source merges them as
`(getCurrentCampaignMission() == 96 || ... == 127)` at the 96 position.
Splitting it into two blocks with duplicated bodies placed after 126 gives
**8,088** B (+8: the second `isCampaignMission()` check) and raw 3,889, so it
needs an -8 compensation elsewhere before it can be landed. This is the most
promising remaining structural target.

### Original emitted order of the campaign blocks (for future within-block work)

- 134 block: `switch_to_target_setting=0`, `field_64=Hitpoints`,
  `field_5c=Shield`, `field_60=Armor`, `field_68=Gamma`, `initStreamOutPosition=0`,
  `departStation(112)`. Our source already emits exactly this order.
- 162 block: `field_64=Hitpoints`, `field_5c=Shield`, `field_60=Armor`,
  `field_68=Gamma`, `switch_to_target_setting=0`, `initStreamOutPosition=0`,
  `departStation(93)`. Our source order is different, but reordering does not
  reduce raw because the block sits at a different chain offset.
- 96 body: `switch_to_target_setting=1`, `field_64`, `field_5c`, `field_60`,
  `field_68`, `initStreamOutPosition=1`, `departStation(98)`. Swapping our
  `field_60`/`field_5c` lines to match gives raw 2,929 (worse by 1), so the base
  order is kept.

### Aligned-only candidate (not landed)

Swapping `switch_to_target_setting = 0;` with
`field_5c = getShieldHP();` in the 134 block gives raw 2,928 (unchanged),
aligned **1,559**, size 8,080 and extab 156,312. However the base 134 block
already emits the exact original instruction sequence, so the aligned drop is a
difflib alignment artifact rather than a source-fidelity improvement; it was not
landed (raw is the primary metric and did not move).

### Original chain order

Reordering the chain to the original emitted order
`15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162` gives aligned
1,307 but size 8,084 (+4) and raw 4,273. The +4 is not one wide branch: a
difflib alignment of the two instruction streams shows many small schedule /
width differences across the chain, so the +4 has no single local fix yet.
