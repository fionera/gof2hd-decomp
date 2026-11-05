# Globals::Globals C1/C2 — pool order *and* movw hoist (w6106, wave 113)

Status: landed, 2026-11-05. Worktree `wt/w6106`, branch `wave113-w6106`.
Target `_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` (`src/game/core/Globals.cpp`),
612 B.

## TL;DR

The wave-111 variant B had the faithful 31/31 GOT literal-pool order but
`movw #583` at normalised index 135 (original: 34). This pass found a pure
statement reorder of the late zero-init block that keeps the **31/31 pool
order** *and* moves **`movw r6,#583` to index 34**, with the original's
**single** `str r6,[r1,#84]` resWidth store (no duplicate store, size stays
612 B).

Official fuzzy stays at **78.7611 %** (M 178) — the same as variant B — but
the instruction stream now matches the original in both structural features
this function was missing. The tuned 80.5 % shape (pool 30/31, movw@139)
remains the best *fuzzy* shape; it is not pool-faithful.

## Winning change

Only the late block (everything after `settings->flagAt3f = 0;`) was
reordered. The early pointer/settings block is untouched from variant B.
Final late-block order:

```cpp
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
```

Relative to variant B this moves `initMemoryWarning`/`imageFactory`/
`achievements`/`galaxy`/`instantActionScore`/`instantActionWave`/
`lastStationMusicPlayed`/`lastCampaignMissionFailed`/
`lastCampaignMissionFailCount` above the quality block, moves `items`/`ships`
after it, puts `status` after `instantActionType`, and moves
`soundResources` last (after the `shortAt60` store).

Measured: `pct 78.7611  M 178  osize 612  usize 612  pool 31/31  movw583@34`.
`objdump` confirms one `str r6,[r1,#84]` (index 157) and one
`str r6,[r1,#88]` (index 166).

## Mechanism

The `movw #583` hoist is a **post-RA scheduler** decision (established in
wave 111: `-disable-post-ra` moves it back). What this pass adds: the
pre-RA machine schedule only contains the `t2MOVi16 583` before the
`lastCampaignMissionFailCount` / `lastCampaignMissionFailed` GOT loads when
the quality block sits *earlier in source than those two statements*. If
those two statements precede the quality block, the constant is sunk into
the quality block and the post-RA scheduler cannot hoist it. Placing the
quality block after `lcmf`/`lcmfc` and before `items`/`ships` gives both the
early constant and the original pool-hoist set.

The pool order is unchanged because the pool is emitted in first-`ldr [pc]`
order and the moved statements keep their relative GOT-reference order.

## Measurements

| variant | pct | M | pool | movw@ | size |
|---|---:|---:|---:|---:|---:|
| landed variant B (wave 111) | 78.7611 | 178 | 31/31 | 135 | 612 |
| wave-111 tuned baseline | 80.5 | 182 | 30/31 | 139 | 612 |
| **this pass (soundRes last)** | **78.7611** | **178** | **31/31** | **34** | **612** |
| this pass without soundRes-last | 78.3186 | 177 | 31/31 | 34 | 612 |
| this pass + settings-block reorder | 77.4336 | 175 | 31/31 | 34 | 612 |
| Ghidra literal order (resWidth store early) | 42.3 | – | 17/31 | 28 | 620 |
| Ghidra order with v54/v58 temps | 52.7 | – | – | 133 | 612 |
| original-store-order late block | 49.1 | 111 | 23/31 | 142 | 612 |
| QUAL position sweep from B, pos 0–3 | 78.8 | – | 31 | 135 | 612 |
| QUAL position sweep from B, pos 4–10 | 77.9 | – | – | 139 | 612 |
| QUAL position sweep from B, pos 12/13/14/15/19 | 75.7/75.2/74.3/74.8/72.1 | – | – | 34 | 612 |
| shared-if quality form | 77.4336 | 175 | 31/31 | 137 | 612 |
| v54/v58 declared at top | 78.7611 | 178 | 31/31 | 135 | 612 |
| early resWidth+resHeight pair store | 32.96 | 74 | 21/31 | 88 | 604 |
| 200 random late-block permutations (best) | 67.7 | – | 22 | 166 | 612 |
| 380 single moves from the new body | best 78.7611 (4 keep pool31 & movw34) | – | 31 | 34 | 612 |

## Gate

`cmake --build cmake-build-match --target check -j6`:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`drift`: 0 fields. 27 section headers, file size 2,187,328 B,
`.ARM.extab` 0x26298 (unchanged).
