# MGame::OnTouchEnd — wave 185 (w12916): distance-5..8 statement transpositions

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w12916`, branch
`wave185-w12916`, base master `f390b0e68`. Scratch `/home/debian/GalaxyOnFire2/scratch/w12916`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,972 | **2,936** |
| aligned (instruction) diff | 1,602 | 1,689 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| `.text` differing bytes | 74,591 | **74,555** |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.extab` diff | 647 | 647 |
| `.ARM.exidx` diff | 0 | 0 |
| build-id diff | 20 | 20 |
| total differing bytes | 75,258 | **75,222** |

Gate after the change (`check`, `RATCHET PASS`): byte_exact **4507/4524**,
linked_exact **4507/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, 27/27 section headers, file size 2,187,328 exact.

## The change

Three same-block statement transpositions, each moving independent stores/getters
(no size change, no `.rel.plt` change, no `.ARM.extab` change).

1. **Mission-100 block** — `SetCurrentApplicationModule(5)` moved from last (after
   `departStation`) to the head of the block, and `field_5c = getShieldHP()` moved
   from the head to just before `active = 0`:

   ```cpp
   } else if (... == 100) {
       Globals::appManager->SetCurrentApplicationModule(5);   // was after departStation
       Globals::status->field_64 = this->player->player->getHitpoints();
       Globals::status->field_60 = this->player->player->getArmorHP();
       Globals::status->field_68 = this->player->player->getGammaHP();
       Level::initStreamOutPosition = 0;
       Globals::switch_to_target_setting = 0;
       Globals::status->departStation(...getStation(120));
       Globals::status->field_5c = this->player->player->getShieldHP(); // was first
       this->active = 0;
       return;
   ```

   `SetCurrentApplicationModule` only writes `ApplicationManager::state`,
   `currentModuleId` and `pendingModule` (`src/engine/core/ApplicationManager.cpp:306`);
   `Status::departStation` and the four `Player::get*HP` getters touch
   `Status`/`Player`/`Galaxy` only, so the call order is behaviour-preserving.
   Raw 2,972 -> 2,969.

2. **Mission-110 block** — `enterSpaceLounge = 1` moved from after
   `SetCurrentApplicationModule(5)` to just after `switch_to_target_setting = 0`,
   and `field_68 = getGammaHP()` moved to the tail:

   ```cpp
   Globals::status->field_64 = ...getHitpoints();
   Globals::status->field_5c = ...getShieldHP();
   Globals::switch_to_target_setting = 0;
   Globals::enterSpaceLounge = 1;                 // was after SetCurrentApplicationModule
   Level::initStreamOutPosition = 0;
   Globals::status->field_60 = ...getArmorHP();
   Globals::status->departStation(...getStation(10));
   Globals::appManager->SetCurrentApplicationModule(5);
   Globals::status->field_68 = ...getGammaHP();   // was 4th
   this->active = 0;
   ```

   Independent stores/getters. Raw 2,969 -> 2,964.

3. **"Back to the station" tail** — `switch_to_target_setting = 0` moved from the
   head of the tail to after the four HP getter/store pairs:

   ```cpp
   // Back to the station.
   Globals::status->field_64 = ...getHitpoints();  // was last
   this->active = 0;
   Globals::status->field_5c = ...getShieldHP();
   Globals::status->field_60 = ...getArmorHP();
   Globals::status->field_68 = ...getGammaHP();
   Globals::switch_to_target_setting = 0;          // was first
   this->applicationManager->SetCurrentApplicationModule(5);
   return;
   ```

   Independent stores/getters. Raw 2,964 -> 2,961.

## Second round (iterative re-sweep)

After landing the three swaps above (raw 2,961), the same 1..4 transposition sweep was
re-run over lines 1630-1760 and iterated to a fixed point.  Six further independent
store/getter transpositions were found and stacked (each re-swept before the next):

| block | swap | raw after |
|---|---|---:|
| 100 | `Level::initStreamOutPosition = 0` <-> `field_5c = getShieldHP()` | 2,957 |
| 110 | `field_5c = getShieldHP()` <-> `Level::initStreamOutPosition = 0` | 2,955 |
| 126 | `switch_to_target_setting = 1` <-> `field_60 = getArmorHP()` | 2,953 |
| 155 | `switch_to_target_setting = 1` <-> `Level::initStreamOutPosition = 1` | 2,951 |
| tail | `field_60 = getArmorHP()` <-> `switch_to_target_setting = 0` | 2,950 |
| 110 | `field_64 = getHitpoints()` <-> `switch_to_target_setting = 0` | 2,945 |
| 110 | `SetCurrentApplicationModule(5)` <-> `field_68 = getGammaHP()` | 2,943 |
| 110 | `departStation(...getStation(10))` <-> `field_68 = getGammaHP()` | 2,938 |
| 110 | `field_64 = getHitpoints()` <-> `field_68 = getGammaHP()` | 2,937 |
| 110 | `field_68 = getGammaHP()` <-> `field_5c = getShieldHP()` | **2,936** |

The mission-110 block was re-permuted to
`switch_to_target_setting, initStreamOutPosition, field_5c, enterSpaceLounge,
field_68, field_60, field_64` before `departStation`; the block-100 order is
`SetCurrentApplicationModule, field_64, field_60, field_68, field_5c,
initStreamOutPosition, switch_to_target_setting, departStation`.  All are independent
stores or pure `Player::get*HP` reads, so the reorders are behaviour-preserving.
The aligned metric rises to 1,689 (the schedule moves), but raw — the primary
metric — falls by 36 bytes total.

## Method

Wave 184 exhausted same-block adjacent (distance 1..4) transpositions. This pass ran a
**distance-5..8** transposition sweep over every run of consecutive same-indentation
single-line statements in `OnTouchEnd` (87 runs, 158 candidates; statements containing
`return`/`break`/`goto`/`if`/`for`/`while`/`switch`/`else`/braces excluded). Each
candidate was compiled and scored with `tools/fn_diff.py`; only size-exact candidates
were kept. The three winners above stack additively (2,972 -> 2,969 -> 2,964 -> 2,961).

A full 4! permutation sweep of the mission-100 HP getter/store order confirmed the
current `field_5c, field_64, field_60, field_68` order is optimal among getter-only
permutations (all shield-first orders raw 2,964; hitpoints/armor/gamma-first 2,965).

## Rejected / measured, not landed

- Mission-100 `field_5c`/`field_64` swap to the original emitted order
  (hitpoints before shield): raw 2,973 (size exact) — worse than base despite matching
  the original's getter order, because the schedule shifts.
- Mission-100 `field_5c` moved after `departStation` but *not* `SetCurrentApplicationModule`
  (rotation instead of the transposition above): raw 2,968.
- Mission-100 `field_64`/`SetCurrentApplicationModule` swap: raw 2,965 (safe but weaker
  than the landed `field_5c` swap).
- Body reorder of the `won` dispatch to
  `if (isInstantActionMission()) {...} else if (isCampaignMission()) {...} else {...}`:
  raw 4,468 (size exact) — the instant body's fallthrough changes MBP's trace and moves
  the cold `soundsMuted` Play block.
- Reordering the campaign chain to the original emitted order
  `15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162`: aligned improves
  to 964 but raw 4,272 and size 8,084 (+4, gate-fatal); combined with the body reorder
  raw 4,728 / size 8,092. The chain body layout is still offset by the cold Play block.
- `count >= 0` -> `count != 0` / `amount != 0` / `count` / `count > 0 || count < 0`
  in the ore-refine loop: all emit the original's `beq.n` at `0x17a5ce` but grow the
  function to 8,088 B (raw 6,620). Size-fatal.

## Residual map (unchanged)

First differing byte remains `count >= 0` emitting `bgt.n` where the original emits
`beq.n` at `+0x48b`; then the 22-byte String-ctor schedule diff at `+0x4a4` (the
`add.w r8, sp, #80` hoist) and its 4-byte literal-pool consequence at `+0x6b4`. The
dominant residual is still the campaign-chain block order/placement from `+0xe3e`
(the cold `soundsMuted` Play block sits at `0x17b0c2` vs the original `0x17b1ee`,
pushing the chain ~0x38-0x40 late). Tail-merge direction at `0x17bf60` still differs
(original merges into the `isDockingToAsteroid` site).
