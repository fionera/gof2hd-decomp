# Globals::Globals C1/C2 — wave 117 (w6508)

Status: landed, 2026-11-05. Worktree `wt/w6508`, branch `wave117-w6508`,
based on master `c22678d64`. Target `_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev`
(`src/game/core/Globals.cpp`), 612 B.

## TL;DR

The wave-115 "805" late order (fuzzy **80.5 %**) is a *tuned* schedule, not a
faithful one: its literal-pool order has `status` at slot 6 (original slot 26),
`movw r6,#583` at normalised index **139** (original **34**), and 405 of the
612 function bytes differ. This pass lands a **volatile-qualified settings
tail**: with the tail stores ordered by `volatile`, the compiler emits the
original's exact 13-store sequence and the whole 96-instruction prefix
(0..95) plus the settings tail (96..103) becomes byte-identical. The landed
shape scores **78.8 %** (first diff 104) but differs in only **193 of 612
bytes** (wave-115: 405; wave-114: 221) and drops `.text` differing bytes
88,975 -> **88,763**.

A scrambled late-block order pushes the fuzzy to **93.3 %** (94/612 bytes,
`.text` 88,666) but reorders the `.o` GOT relocations, which reorders
`.rel.dyn` / the `.got` slots for the Globals statics and makes
`Globals::init` non-byte-exact (2 pool deltas). That fails the ratchet
(`byte_exact 4501 -> 4500`), so it is recorded as a lead, not landed.

## Landed shape

`src/game/core/Globals.cpp`, `Globals::Globals()` body only:

- The `0x10` short store moves after `settings->glowR = 0.5f;` and is made
  `volatile`; this is the only way found to keep the compiler from
  re-grouping the float vector stores around it.
- The 13-store settings tail goes through `volatile GameSettingsRecord *vs`
  / `volatile CtorSecondaryObject *vsec`, in the original's emitted order:
  `shortAt40, intAt50, flagAt39, intAt35, intAt31, hints.slot1,
  hints.flagAt13, flagAt3f, intAt3b, flagAt4e, shortAt4c, intAt48,
  hints.slot0`.
- The late block is the wave-114 order (lastSpace, gameLoaded, gameSaving,
  initMemoryWarning, imageFactory, achievements, galaxy, ias, iaw,
  lastStation, lcmf, lcmfc, quality, items, ships, iapn, iat, status,
  shortAt60, soundResources). This is the order that keeps the `.o` GOT
  relocation order (and therefore `.rel.dyn` / `.got`) identical to the
  original.

The volatile tail is the mechanism that reproduces the original's store
schedule; without it the compiler reorders the 13 stores and the first
difference moves from index 104 back to 96.

## Measurements (all full compile + gold link, forced recompile)

| shape | fuzzy | first diff | fn diff bytes | `.text` diff | pool order | gate |
|---|---:|---:|---:|---:|---|---|
| wave-115 "805" (master) | **80.5** | 34 | 405 | 88,975 | status slot 6 | pass |
| wave-114 faithful | 78.8 | 96 | 221 | 88,791 | exact | pass |
| **volatile tail (landed)** | **78.8** | **104** | **193** | **88,763** | **exact** | **pass** |
| volatile tail + scrambled late block | **93.3** | 109 | **94** | 88,666 | differs | **FAIL** (byte 4500) |
| original | 100 | – | 0 | 0 | exact | – |

With the volatile tail the emitted stream matches the original for
instructions 0..103 (including the complete settings tail) and the literal
pool is 31/31 identical in order.

### Why the 93.3 % shape fails the ratchet

The `.o` relocation order for the Globals statics is the literal-pool order.
Changing the late-block source order changes that order, e.g. the landed
pool tail is `gameLoaded, gameSaving, initMemoryWarning, imageFactory,
achievements, galaxy, ias, iaw, status, iat, iapn, items, ships`, while the
93.3 % body produces `gameSaving, initMemoryWarning, items, ias,
imageFactory, gameLoaded, ships, achievements, galaxy, iat, iaw, status,
iapn`. Gold lays `.rel.dyn` / `.got` out in that order, so
`Globals::init`'s PC-relative GOT deltas at `+0x310` shift by 8/4 bytes
(2 bytes differ) and `byte_exact` drops 4501 -> 4500. Any late-block
reordering that keeps the original pool order is therefore the constraint
for landing the 93.3 % shape in a later wave.

## What was checked

- **Pool order.** The landed shape's GOT literal pool is identical to the
  original's 31 words in order:
  `Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
  shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
  layout, appManager, generator, hints, gameLoaded, gameSaving,
  initMemoryWarning, imageFactory, achievements, galaxy, instantActionScore,
  instantActionWave, status, instantActionType, instantActionPlayerName,
  items, ships`.
- **Settings tail.** `volatile` stores emit in source order, so the source
  order is the original's emitted order. Without volatile the compiler
  sinks `intAt3b`/`flagAt4e`/`shortAt4c`/`intAt48` past the global stores
  (first diff 96).
- **`0x10` store.** Making it volatile in its original source position
  breaks the float vector store grouping (`vst1` moves from `+0x14` to
  `+0x20`, function grows to 616 B); moving it after `glowR` restores the
  grouping and the 612 B size.
- **`movw r6,#583`.** The landed shape hoists it to index 34 exactly as the
  original; the 805 shape materialises it at 139.

### Searches (all full compile + gold link, forced recompile)

| search | best fuzzy | notes |
|---|---:|---|
| 120 random early-block permutations (805 late fixed) | 80.5 | no gain |
| 150 random early-block permutations (805 late fixed) | 80.5 | no gain |
| 120 random late-block permutations (805 early fixed) | 80.5 | no gain |
| 120 random settings-tail permutations (805) | 80.5 | no gain |
| 120 random early+late combinations (805) | 80.5 | no gain |
| quality-block position sweep (805 late block) | 80.5 at pos 0-5; 79.2 at 6-12; 77.9 at 13; 71.7-74.3 at 14+ | `movw@34` only at 14+ |
| status-position sweep (805 late block) | 77.4 max | moving status late always loses |
| `int v54` declaration position sweep (all 54 slots) | 80.5 everywhere | compiler sinks the constant |
| 100 random late-block permutations (114 early fixed) | 78.8 | no gain |
| 200 hill-climb moves from 805 (late block + tail) | 80.5 | plateau |
| 200 hill-climb moves from the volatile tail body | **93.3** | scrambles late block; pool order lost |

### Dead ends recorded

- Moving `status` out of the 805 early pointer block restores the pool order
  but costs >=3 fuzzy points (best 77.4 with the 805 late block).
- Moving the quality block late restores `movw@34` but costs >=6 fuzzy
  points.
- `v54`/`v58` declaration position is irrelevant (clang sinks the constant);
  `float fv` must stay in the quality block (moving it to the top gives 53.2).
- The two-if `{v54; v58;}` form and direct field-store form are byte-identical
  to the landed form.
- `int32_t` hints members merge into `strd` and break the match; the
  `uint8_t[4]` members in `CtorSecondaryObject` are required.
- Making the `0x10` store volatile in place grows the function to 616 B and
  changes the float vector grouping.
- Moving `secondary->slot0` out of the volatile tail (even by one line)
  changes the prologue register allocation (first diff 2-9).
- The 93.3 % body cannot be landed without also restoring the original
  `.o` GOT relocation order (see above).

## Final source body (self-contained)

```cpp
Globals::Globals() {
    GameSettingsRecord *settings = (GameSettingsRecord *) Globals::options;
    Globals::shipTemplate = 0;
    Globals::Canvas = 0;
    Globals::gameText = 0;
    Globals::font = 0;
    Globals::fontAlien = 0;
    Globals::fontLangSelect = 0;
    Globals::rnd = 0;
    Globals::globals = 0;
    settings->tintR = 0.6f;
    Globals::layout = 0;
    Globals::appManager = 0;
    Globals::generator = 0;
    Globals::recordHandler = 0;

    this->field_0x0 = 0;

    settings->colorR = 0.5f;
    settings->colorG = 0.5f;
    settings->colorB = 0.5f;
    reinterpret_cast<int16_t &>(settings->_region[0]) = 0x101;

    settings->tintG = 0.6f;
    settings->glowG = 0.5f;

    settings->tintB = 0.5f;
    settings->glowR = 0.5f;
    *reinterpret_cast<volatile int16_t *>(reinterpret_cast<uint8_t *>(settings) + 0x10) = 0;
    CtorSecondaryObject *secondary = (CtorSecondaryObject *) Globals::hints;
    settings->brightness = 1.0f;
    settings->contrast = 0.5f;
    settings->_region[2] = 1;
    settings->enableFlag30 = 1;
    volatile GameSettingsRecord *vs = settings;
    volatile CtorSecondaryObject *vsec = secondary;
    reinterpret_cast<volatile int16_t &>(vs->shortAt40) = 0;
    vs->intAt50 = 0;
    vs->flagAt39 = 0;
    reinterpret_cast<volatile int32_t &>(vs->intAt35) = 0;
    reinterpret_cast<volatile int32_t &>(vs->intAt31) = 0;
    reinterpret_cast<volatile int32_t &>(vsec->slot1) = 0;
    vsec->flagAt13 = 0;
    vs->flagAt3f = 0;
    reinterpret_cast<volatile int32_t &>(vs->intAt3b) = 0;
    vs->flagAt4e = 0;
    reinterpret_cast<volatile int16_t &>(vs->shortAt4c) = 0;
    vs->intAt48 = 0;
    reinterpret_cast<volatile int32_t &>(vsec->slot0) = 0;

    Globals::lastSpaceMusicPlayed = -1;
    Globals::gameLoaded = 0;
    Globals::gameSaving = 0;
    Globals::initMemoryWarning = 0;
    Globals::imageFactory = 0;
    Globals::achievements = 0;
    Globals::galaxy = 0;
    Globals::instantActionScore = 0;
    Globals::instantActionWave = 0;
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
    Globals::items = 0;
    Globals::ships = 0;
    Globals::instantActionPlayerName = 0;
    Globals::instantActionType = -1;
    Globals::status = 0;
    reinterpret_cast<int16_t &>(settings->shortAt60) = 0x100;
    this->soundResources = 0;
}
```

## Gate

`cmake --build cmake-build-match --target check -j6`:

```text
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`drift`: 0 fields. `lint`: CLEAN. 27 section headers, file size
2,187,328 B, `.ARM.extab` 0x26294 (unchanged; the 4-byte shortfall noted in
the task is pre-existing). `.text` size 0x1337c8, `.text` differing bytes
88,763, `.rodata` 0 differing bytes before the mission-case table, `.data`
0 differing bytes, `.got` content identical.
