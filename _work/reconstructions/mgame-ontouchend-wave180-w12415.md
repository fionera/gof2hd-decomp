# MGame::OnTouchEnd — wave 180 (w12415): statement-order sweep

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`), original 8,080 B at
`0x17a144` (Thumb), NDK r18b clang 7.0.2 `-Oz`.

## Landed wins

Two independent passes on this worker's branch, both pure statement reorders inside
independent straight-line runs (no size change, no `.rel.plt` change, no `.ARM.extab` change):

| commit | change | raw | `.text` diff |
|---|---|---:|---:|
| `a5735d098` | weaponSlot tail order | 3049 -> 3048 | 76870 -> 76869 |
| this commit | nine adjacent-statement swaps | 3048 -> **3034** | 76869 -> **76855** |

Final: raw **3034**, aligned **1445**, size **8080/8080**, `.rel.plt` diff **0**, epilogue
`0x17b014`, `.ARM.extab` **size- and base-exact 156,312 B @ 0x1a3b78, content diff 709**
(unchanged), `.ARM.exidx` diff **0**, build-id 20, total **77,584** (from 77,598).

### The nine reorders (this commit)

Each is a transposition of two adjacent independent statements.  The original binary emits
them in the swapped order; the wave-179 source had the reverse.

1. Freelance arm of the won-mission dispatch:
   `showMissionRewardMessage(...)` now precedes `setFreelanceMission(Mission::empty)`.
2. Mission-65 block: `FileRead *reader = new FileRead();` now precedes
   `Level::initStreamOutPosition = 1;`.
3. Mission-96/127 block: `field_60 = ...getArmorHP()` now precedes
   `field_5c = ...getShieldHP()`.
4. Mission-100 block: `field_64 = ...getHitpoints()` now follows
   `field_5c = ...getShieldHP()`.
5. `finishMission` / `!keepObjectives`: `this->player->setRoute(nullptr)` now precedes
   `Globals::status->setMission(Mission::empty)`.
6. Escort block: `Item *escortItem = ...` now precedes
   `this->field_0x1e0 = this->level->killCountB`.
7. `cinematicSteerActive` switch, `case 1`: `SetCurrentApplicationModule(5)` now precedes
   `this->active = 0`.
8. Cinematic-mode entry: `this->hudTouchFlags = 0` now precedes `setCinematicMode(true)`.
9. `_bc8` tail: `resumeSounds()` now precedes `this->pauseOpen = 0`.

## Method

`OnTouchEnd` is a ~1,000-line function whose cold-region block layout is already tuned
(wave-179 `w12314` chain permutation).  Rather than re-running whole-block permutations, a
**systematic adjacent-statement transposition sweep** was run over every pair of consecutive
single-line statements with equal indentation (253 pairs; statements containing
`return`/`break`/`goto`/`if`/`for`/`while`/`switch`/`else`/braces excluded).  Each candidate
was compiled and scored with `tools/fn_diff.py`.

Round 1 (on the wave-179 base) found 9 improving swaps; all 9 stack additively
(3049 -> 3046 -> 3043 -> 3041 -> 3039 -> 3038 -> 3037 -> 3036 -> 3035).  A second pass on
the combined source found one more (`field_64`/`field_5c` in the mission-100 block,
3035 -> 3034).  A third pass found no further improvement: **3034 is a local optimum for the
adjacent-transposition neighbourhood.**

## Rejected candidates (measured, not landed)

- Campaign-chain permutation `[162,15,74,43,42,22,65,120,96,100,95,110,81,134,144,126,155,161]`
  (a single-element move of unit 74, or equivalently the swap `43`/`15` of the move order):
  raw **3047** (best chain-order result found; single-move and swap sweeps from both the
  base and the new order were exhaustive, and a full 3-cycle sweep found nothing better).
  **Rejected**: it shrinks `.ARM.extab` from 156,312 to 156,308 B (OnTouchEnd's LSDA call-site
  table drops from `0x84` to `0x83`), which shifts every later LSDA by 4 and grows the
  `.ARM.exidx` diff from 0 to 429 and the `.ARM.extab` diff from 709 to 7,913.  Net loss.
- `count >= 0` -> `count != 0` / `amount != 0` / `if (count)`: emits the original's `beq.n`
  instead of `bgt.n` at `0x17a5ce`, but changes register allocation (`amount` moves r9->r8,
  `ldr.w r9,[sp,#24]` appears) and the function grows to 8,084 B.  Size-fatal.
- `particleQuality` / `initLight` / touch-id store reorder: 3,049-3,056 (no win).
- Sound-arm reorders (`soundsMuted` first, nested, duplicated Play): 4,115-6,008.
- `String("t ")` construction scheduling (the `add.w r8, sp, #80` hoist at `0x17a5e8`):
  robust to every source spelling tried (`String("t ", false)`, named local, parenthesised
  association, `report = report + ...`); the hoist is a PostRA latency-scheduler choice.
- `__builtin_expect` on the `count` test: 6,993-7,486 (layout blow-up).

## Residual map (unchanged, for the next pass)

- Dominant residual `0x17b0bc-0x17b93c`: the sound Play block is emitted at `0x17b0c2`
  (original `0x17b1ee`), pushing the freelance/reward block and the whole campaign chain
  ~0x38 late.  MBP (`bb.214`/`bb.215`) places the Play block right after the
  `soundsMuted` test because the fall-through edge is 62.5%; moving the source block later
  only makes raw worse (4,373-4,412).
- Hot-region residuals are only: `beq`/`bgt` at `0x17a5ce` (1 B), the 22-byte String-ctor
  schedule at `0x17a5e8`, and the resulting 4-byte literal-pool delta at `0x17a7f8`.
- Original campaign-chain emission order (by `getCurrentCampaignMission` cmp):
  `15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162`.

## Gate (this commit)

byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact **4524/4524**,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact **843/843**, imports **207**,
lint **CLEAN**, drift **0**, symbol parity **5370/5370**, `.rel.plt` byte-identical,
27/27 section headers, file size 2,187,328 exact.
