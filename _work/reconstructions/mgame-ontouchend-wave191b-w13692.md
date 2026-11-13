# MGame::OnTouchEnd — wave191b w13692: faithful campaign chain order + store-order pass

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13692`, branch
`wave191b-w13692`, base master `b3fd7e6a2`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13692`.

Prior notes: `mgame-ontouchend-wave191b-w13681.md` (campaign-65 + back-to-station
store orders), `-w13672.md` (arm-81 condition, arm-126 switch), `-w13662.md`
(mining-report `count > 0`), `-w13626.md` (arm-134 store order).

## Result (landed)

The campaign else-if chain is restored to the original's test order and the
remaining independent-store bodies are put in the binary's order:

1. **Chain order.** The original false-branch chain (traced from `0x17b104`
   through `0x17be4a`) is `15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120,
   126, 127, 134, 144, 155, 161, 162`.  The source's combined `96 || 127` arm
   stands in for the two original tests.  The fully faithful order is **8,084 B**
   (4 B too big: two Thumb branches widen); moving arm 95 to before arm 65
   (`15,22,43,42,95,65,81,74,96,...`) is the best size-exact order found.
   The mission IDs are mutually exclusive, so the test order is semantically
   irrelevant.
2. **Campaign-first conditions** in every arm (the original tests
   `Mission::isCampaignMission()` first, then `Status::getCurrentCampaignMission()`).
3. **Arm-81 body** (`0x17b4ba`): `switch=1`, then getters `64,5c,60,68`, then
   `initStreamOutPosition=1`, `setStation(playerStation)`, `departStation`,
   `active=0`, module 2.
4. **Arm-110 body** (`0x17ba3e`): getters `64,5c,60,68`, then `switch=0`,
   `initStream=0`, `departStation(getStation(10))`, module 5,
   `enterSpaceLounge=1`, `active=0`.
5. **Arm-134 body** (`0x17bc50`): `switch=0`, then getters `64,5c,60,68`
   (the base had w13626's aligned-only `5c,60,64,68` order).
6. **Arm-162 body** (`0x17be4e`): getters `64,5c,60,68`, then `initStream=0`,
   `switch=0`, `departStation(getStation(93))`, `active=0`, module 5.
7. **Arm-96/127 body**: `64,5c,60,68` (the base had `64,60,5c,68`).

All stores/reads are independent `Status` fields or pure getters, so behaviour
is unchanged.  Arm 95's own store order (`64,5c,60,sw,68`) is left as-is: the
faithful `sw,64,5c,60,68` form is size-fatal (8,088) in this layout.

| | raw | aligned | size | insn | `.text` diff | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base | **2,910** | 1,479 | 8,080/8,080 | 2,862/2,865 | 71,270 | 156,312 (577 diff) |
| **landed** | **3,650** | **694** | **8,080/8,080** | **2,865/2,865** | 72,010 | 156,312 (564 diff) |

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.ARM.exidx` 0 diff, `.rodata` 0 diff,
`.rel.plt` unchanged.  `.ARM.extab` content diff improves 577 -> 564.

## Chain-order measurements (all with the store-order fixes above)

| order | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,910** | 1,479 | 8,080 |
| fully faithful `...65,81,74,95,96...` | 4,278 | 922 | **8,084** (fatal) |
| faithful + arm-155 before 134/144 (`...126,155,134,144,161,162`) | 4,242 | 999 | 8,080 |
| faithful + 162 before 126 (`...120,162,126,134,144,155,161`) | 3,956 | 1,043 | 8,080 |
| **landed: 95 before 65** (`15,22,43,42,95,65,81,74,96,...`) | **3,650** | **694** | **8,080** |
| 74 before 65 | 3,656 | 726 | 8,080 |
| 96 before 65 | 3,694 | 894 | 8,080 |

The landed order is a local minimum: a hill-climb over all 306 single-arm moves
from it (size-filtered, then aligned) found no better size-exact order
(next best 706).  A 300-sample random search over permutations of the first
nine arms also failed to beat 694.  The fully faithful order is the best
*aligned* layout (922) but is 4 B over 8,080; no source tweak tried (arm-81
condition, arm-126 switch, tail order, `__builtin_expect` removal, arm-100
partial reorders) shaved the 4 B.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13692`:
`measure.sh` (build + `tools/fn_diff.py` + section diffs), `chain_order.py`
(identity-checked else-if chain parser/rebuilder), `variant.py` + `spec_*.py`
(store-order replacements), `sweep_orders.sh` / `sweep_tail.py` /
`sweep_moves.py` / `hill.py` / `rand_search.py` (order sweeps).
