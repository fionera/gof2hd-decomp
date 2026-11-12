# MGame::OnTouchEnd — wave191b w13514

Function: `_ZN5MGame10OnTouchEndEiiPv` @ `0x17a144`, size 8,080 B.
File: `src/game/menu/MGame.cpp` (cinematic-steer switch, `case 0:`).

## Landed (win 1)

Move the `Level::initStreamOutPosition = 0;` store from the end of the
`cinematicSteerActive` `case 0:` body (after `SetCurrentApplicationModule(2)`)
to position 2 — directly after `Globals::status->setKills(3);`:

```cpp
case 0:
    Globals::status->nextCampaignMission(true);
    Globals::status->setKills(3);
    Level::initStreamOutPosition = 0;
    Globals::switch_to_target_setting = 1;
    this->active = 0;
    Globals::appManager->SetCurrentApplicationModule(2);
    break;
```

Position 3 (after `switch_to_target_setting = 1;`) measures identically
(raw 2,884 / aligned 1,726); position 2 is what is committed.

| metric | base (566374cfd) | after |
|---|---:|---:|
| size | 8,080 | 8,080 (size-exact) |
| raw diff | 2,885 | **2,884** |
| aligned diff | 1,717 | 1,725 |
| `.text` diff bytes | 71,434 | **71,433** |
| `.ARM.extab` diff | 577 | 577 |
| `.rel.plt` | OK | OK |

## Landed (win 2) — full 24-permutation sweep of the `case 0:` tail

On top of win 1, a full permutation sweep of the four independent `case 0:`
tail stores (`switch_to_target_setting = 1`, `active = 0`, `initStreamOutPosition
= 0`, `SetCurrentApplicationModule(2)`) finds one improvement: put
`this->active = 0;` first, i.e.

```cpp
case 0:
    Globals::status->nextCampaignMission(true);
    Globals::status->setKills(3);
    this->active = 0;
    Globals::switch_to_target_setting = 1;
    Level::initStreamOutPosition = 0;
    Globals::appManager->SetCurrentApplicationModule(2);
    break;
```

Size 8,080, `.rel.plt` identical, raw **2,884 -> 2,882**, `.text` **71,433 ->
71,431**, `.ARM.extab` unchanged (577).  All 24 permutations measured; the next
best are the base order and `(init, switch, active, module)` at 2,884, then
2,885.  `(init, module, switch, active)` is size-fatal (8,076).

| metric | after win 1 | after win 2 |
|---|---:|---:|
| size | 8,080 | 8,080 |
| raw diff | 2,884 | **2,882** |
| aligned diff | 1,725 | 1,731 |
| `.text` diff bytes | 71,433 | **71,431** |
| `.ARM.extab` diff | 577 | 577 |
| `.rel.plt` | OK | OK |

Gate: `check` => RATCHET PASS, byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.

## Swept and reverted this wave (do not repeat)

- 24-permutation getter sweep of every 4-stat-getter block (65,81,95,96,100,
  110,120,126,134,144,161,162): base order optimal; block 162 all 24 equal.
- Adjacent body-statement transposition sweep, all 18 chain blocks: base optimal.
- Chain adjacent-block swaps (17): base optimal (15<->43 ties 2,885).
- Full 153 pair-swap from base: base optimal (best others 2,891 / 2,892).
- Full 306 block-move sweep; 3-cycles in {74,162,15,43,42,22,65} and
  {96,100,95,110,81,134,144,126,155,161}; 400 random double-moves: base optimal.
- Per-arm operand swap (`mission->isCampaignMission()` first): 120/126/155
  neutral, all others worse.
- Single-statement moves of `switch_to_target_setting` /
  `Level::initStreamOutPosition` in every chain block: only neutral ties.
- `case 1:` all 6 permutations: base best.
- `case 0:` adjacent swaps + single moves: **only** `initStreamOutPosition`
  -> pos 2/3 gives 2,884 (pos 4 = 2,887, pos 1 = 2,887, pos 0 = 8,088
  size-fatal).
- Won-block variants: `instantAction`-first flat (A) raw 4,469 but `.rel.plt`
  DIFF; `if (campaign) ... else if (!isInstantActionMission())` (H) 2,890;
  `switch_to_target_setting` before `active` 2,886; `setFreelanceMission`
  before `showMissionRewardMessage` 2,887; `__builtin_expect(...,0)` neutral.
- `finishMission` original order 2,894; back-to-station original order 2,895.
- Original emitted chain order (15,22,43,42,65,81,74,95,96,100,110,120,126,
  134,144,155,161,162): size 8,076, raw 3,946 (w13405/w13421).

## Residual map

First diff byte is the `count >= 0` emission (`bgt.n` vs original `beq.n`) at
`+0x48b`, then the 22-byte `String("t ")` ctor schedule at `+0x4a4` and a
literal-pool word at `+0x6b4`. Everything from `+0xba6` on is campaign-chain
block placement.
