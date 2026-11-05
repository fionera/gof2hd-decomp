# Globals::Globals C1/C2 — wave 118 (w6607)

Status: landed, 2026-11-05. Worktree `wt/w6607`, branch `wave118-w6607`,
based on master `3d4a98a3e` (wave-117 harvest). Target `_ZN7GlobalsC1Ev` /
`_ZN7GlobalsC2Ev` (`src/game/core/Globals.cpp`), 612 B.

## TL;DR

The wave-117 volatile settings tail (fuzzy **78.8 %**, first diff 104) was still
a *tuned* late-block schedule: it kept the `.o` GOT relocation order but emitted
the late stores in the wrong order. This pass replaces the late-block statement
order with the original source order and the function becomes **byte-exact**
(612/612 B, 0 differing bytes), with the same volatile settings tail that was
already exact.

The original source order is:

```
gameLoaded, gameSaving, initMemoryWarning, lastSpaceMusicPlayed,
lastStationMusicPlayed, lastCampaignMissionFailed,
lastCampaignMissionFailCount, quality block (resWidth/resHeight),
instantActionScore, instantActionWave, instantActionType,
instantActionPlayerName, items, ships, status, galaxy, achievements,
imageFactory, shortAt60, soundResources
```

At `-Oz` clang reorders those stores into the original emitted schedule
`gameLoaded, gameSaving, lastSpace, initMemoryWarning, lastStation, lcmfc,
lcmf, quality, ias, iaw, iat, iapn, items, status, shortAt60, galaxy,
achievements, imageFactory, ships, soundResources` and materialises the GOT
pointers in the original literal-pool order (`lastSpace, lastStation, lcmf,
lcmfc, ..., gameLoaded, gameSaving, initMemoryWarning, imageFactory,
achievements, galaxy, ias, iaw, status, iat, iapn, items, ships`). Both the
instruction stream and the 31-word literal pool are now byte-identical.

## Measurements (full compile + gold link, forced recompile)

| shape | fuzzy | first diff | fn diff bytes | `.text` diff | gate |
|---|---:|---:|---:|---:|---|
| wave-117 landed | 78.8 | 104 | 193 | 89,185 | pass |
| best intermediate (tuned) | 97.3 | 125 | 8 | – | pass |
| **wave-118 (this pass)** | **100.0** | – | **0** | **88,992** | **pass** |

The intermediate 97.3 % shape had the right instruction stream except two
store-value registers (pool words `e33d0/e33d4/e3428/e3434/e343c/e3440`
assigned to the wrong globals). It was found by hill-climbing on raw differing
bytes; the final shape reaches 0 by putting the late-block stores back in the
original *source* order (the compiler, not the source, performs the reordering).

## Gate

`cmake --build cmake-build-match --target check -j6`:

```text
verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`byte_exact` / `linked_exact` **4501 -> 4503** (C1 and C2 both become exact;
they alias the same address). Baseline locked with
`python3 tools/ratchet.py --update-baseline`. `drift`: 0 fields. `lint`:
CLEAN. 27 section headers, file size 2,187,328 B. `.rel.dyn` and `.got`
content identical. `.ARM.extab` 0x2628c and `.rodata` 0x41c27 are unchanged
from the wave-117 base (the task text's 0x26294 extab figure predates the
current master; the base build is also 0x2628c, so this pass does not regress
it).

## What was checked

- **Volatile settings tail unchanged.** Instructions 0..103 (prologue, early
  pointer block, float/vector stores and the 13-store volatile settings tail)
  still match the original exactly.
- **Late-block source order.** The tuned wave-117 order started with
  `lastSpaceMusicPlayed` and kept `imageFactory/achievements/galaxy` early;
  that produced the correct GOT relocation order but the wrong store schedule.
  The original source order above produces both.
- **GOT / literal pool.** The 31 literal-pool words are identical in order and
  value; `Globals::init` stays byte-exact (800/800).
- **Searches.** Hill-climbing on fuzzy and then on raw differing bytes:
  the 97.3 % plateau (8 bytes) was broken by restoring the source order, not by
  further tuning. Moving `lastStationMusicPlayed` earlier than position 11 in
  the tuned order changes the prologue register allocation (zero/settings
  swap), which is why the tuned order could not reach 0.

## Final source body (late block, self-contained)

```cpp
    Globals::gameLoaded = 0;
    Globals::gameSaving = 0;
    Globals::initMemoryWarning = 0;
    Globals::lastSpaceMusicPlayed = -1;
    Globals::lastStationMusicPlayed = -1;
    Globals::lastCampaignMissionFailed = -1;
    Globals::lastCampaignMissionFailCount = 0;
    float fv = settings->qualityLevel;
    int v54 = 0x247;
    if (1.0f <= fv) v54 = 0x33e;
    if (fv <= 0.0f) v54 = 0x19f;
    settings->resWidth = v54;
    int v58 = 0x201;
    if (1.0f <= fv) v58 = 0x2da;
    if (fv <= 0.0f) v58 = 0x16d;
    settings->resHeight = v58;
    Globals::instantActionScore = 0;
    Globals::instantActionWave = 0;
    Globals::instantActionType = -1;
    Globals::instantActionPlayerName = 0;
    Globals::items = 0;
    Globals::ships = 0;
    Globals::status = 0;
    Globals::galaxy = 0;
    Globals::achievements = 0;
    Globals::imageFactory = 0;
    reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
    this->soundResources = 0;
```
