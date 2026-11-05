# Globals::Globals C1/C2 — wave 115 (w6307)

Status: landed, 2026-11-05. Worktree `wt/w6307`, branch `wave115-w6307`.
Target `_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` (`src/game/core/Globals.cpp`),
612 B.

## TL;DR

Fuzzy **78.7611 % -> 80.5 %** (M 178 -> M 182) with the literal-pool order
still **31/31** and size 612 B. The winning shape is the wave-114 "805"
statement order (status moved into the early pointer block, `recordHandler`
second, quality block early, remaining late zero-init in the 805 order). The
one structural property given up is the `movw r6,#583` hoist: it moves from
normalised index **34** (wave-113/114 baseline) to **139**. The wave-113
pool-faithful shape (78.7611, pool 31/31, movw@34) remains the alternative if
the hoist is valued over the fuzzy score.

## Winning change (relative to the wave-114 baseline)

`src/game/core/Globals.cpp`, `Globals::Globals()` body only:

- Early pointer block: `recordHandler` moves to position 2 (right after
  `shipTemplate`); `status` moves from the late block to the end of the early
  pointer block (after `generator`).
- Late block becomes:

```cpp
    Globals::lastSpaceMusicPlayed = -1;
    Globals::gameLoaded = 0;
    Globals::gameSaving = 0;

    float fv = settings->qualityLevel;
    int v54 = 0x247;
    if (1.0f <= fv) v54 = 0x33e;
    if (fv <= 0.0f) v54 = 0x19f;
    settings->resWidth = v54;
    int v58 = 0x201;
    if (1.0f <= fv) v58 = 0x2da;
    if (fv <= 0.0f) v58 = 0x16d;
    settings->resHeight = v58;

    Globals::initMemoryWarning = 0;
    Globals::imageFactory = 0;
    Globals::achievements = 0;
    Globals::galaxy = 0;
    Globals::instantActionScore = 0;
    Globals::lastStationMusicPlayed = -1;
    Globals::instantActionType = -1;
    Globals::instantActionPlayerName = 0;
    Globals::items = 0;
    Globals::ships = 0;
    Globals::instantActionWave = 0;
    Globals::lastCampaignMissionFailed = -1;
    this->soundResources = 0;
    reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
    Globals::lastCampaignMissionFailCount = 0;
```

The early settings/colour block is untouched from the wave-113 baseline.

Measured: `pct 80.5  M 182  osize 612  usize 612  pool 31/31  movw583@139`.
First 34 normalised instructions match the original exactly; the wave-113
baseline matched 96 but lost more alignment in the tail.

## Why this shape

The late zero-init is a chain of independent stores; the post-RA scheduler
orders them by its own priority, so the source order is only a weak lever.
The one strong lever found here is **moving `status = 0` into the early
pointer block**: the compiler then keeps the status address live and stores
it late (emitted at `e3390`, immediately after `items`, matching the
original's `e3394`). Removing `status` from the early block and leaving it in
the late block collapses the score to 56.2. `recordHandler` must stay second
in the early block (position sweep: 80.5 at pos 1, 80.1 at pos 0, <=79.6
elsewhere). `status` must stay last in the early pointer block (sweep:
80.5 at pos 13, 80.1 for pos 3-12, <=79.6 below).

## Measurements (all scratch under /dev/shm/w6307)

| variant | pct | pool | movw@ | size |
|---|---:|---:|---:|---:|
| wave-114 baseline (committed) | 78.7611 | 31/31 | 34 | 612 |
| **landed: 805 shape + current structs** | **80.5** | **31/31** | **139** | **612** |
| 805 shape, `recordHandler` not 2nd (pos 0) | 80.1 | 31/31 | 139 | 612 |
| 805 shape, `recordHandler` after Canvas | 79.6 | 31/31 | 139 | 612 |
| 805 shape, `recordHandler` after generator | 79.2 | 31/31 | 139 | 612 |
| 805 shape, `status` removed from early block | 56.2 | 31/31 | 135 | 612 |
| baseline early + `status` after generator + 805 late | 79.6 | 31/31 | 139 | 612 |
| baseline early + `recordHandler`+`status` + 805 late | 79.2 | 31/31 | 139 | 612 |
| 805 early + `Canvas` first | 79.6 | 31/31 | 139 | 612 |
| 805 early + `imageFactory` moved to end | 78.8 | 31/31 | 139 | 612 |
| 805 early + `imageFactory` after `ships` | 79.6 | 31/31 | 139 | 612 |
| 805 early + `imageFactory`/`achievements`/`galaxy` early | 70.8 | 31/31 | 151 | 612 |
| 805 early + original late store order | 51.8 | 31/31 | 146 | 612 |
| baseline early + original late store order | 49.1 | 31/31 | 142 | 612 |
| 805 early + `v54`/`v58` declared at top | 80.5 | 31/31 | 139 | 612 |
| 805 early + quality block at top | 48.2 | 31/31 | 81 | 612 |

Searches (all full compile + gold link, scored with the wave-115 harness):

- **200 random permutations of the late block** (805 early fixed): best
  **80.5**, no improvement.
- **Single-move hill climb over the 14 early pointer statements**: converged
  to the landed order, **80.5**, no improvement.
- **300 random permutations of the 11 settings zero stores**: best **80.5**,
  no improvement.
- **300 random late-block permutations with the quality block pinned at
  position 13** (the only region that yields `movw@34`): best **71.7**.
  The `movw@34` constraint costs ~9 points of fuzzy alignment.
- Quality-block position sweep in the 805 late block: pos 0-4 = 80.5,
  pos 5-11 = 79.2, pos 12 = 77.9, pos 13-15 = 73.0/71.7/72.1 (movw@34),
  pos 16+ = 61-74.

## Dead ends (recorded so they are not re-tried)

- `Canvas` first, `recordHandler`/`generator` swaps, `status` position
  sweeps, `imageFactory`/`achievements`/`galaxy` moves and the original
  store order all score below 80.5.
- The original's exact store order (`gameLoaded, gameSaving, lastSpace,
  initMemoryWarning, lastStation, lcmfc, lcmf, quality, iAScore, iAWave,
  iAType, iAPlayerName, items, status, 0x60, galaxy, achievements,
  imageFactory, ships, soundResources`) is *not* reproducible by writing that
  order in source: the post-RA scheduler reorders it (49.1-51.8 %).
- The `movw r6,#583` hoist to index 34 and the 805 tail alignment are
  mutually exclusive in every shape tested: any source position of the
  quality block that hoists the constant drops the fuzzy score below the
  wave-113 baseline.

## Gate

`cmake --build cmake-build-match --target check -j6`:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`drift`: 0 fields. `lint`: CLEAN. 27 section headers, file size
2,187,328 B, `.ARM.extab` 0x26298 (unchanged).
