# MGame::OnTouchEnd (8080 B) — w13818 (wave 191b): non-cinematic cases + shared tail

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, 8080 B).
Branch base `342cf7a86`.  w13805 also owns this function; this pass is the
**non-cinematic touch cases + shared dispatch tail** slice (no case-0 /
cinematic-steer work, that is w13774/w13798).

## 0. Metric direction (corrects the w13798 table)

`tools/fn_diff.py` "aligned" is the **count of non-matching instructions**
after branch-target normalisation (`sum(c.values())`, `replace` counts
`max(i2-i1, j2-j1)`).  **Lower is better.**  The w13798 note labelled
`aligned 545 -> 799` and `545 -> 601` as "wins"; they are regressions.
Current baseline: raw **3,665** / aligned **545** / size **8,080** /
`.text` 75,919 / `.ARM.extab` 594 / `.ARM.exidx` 0 / `.rodata` 0.

## 1. Measured table (this pass, `tools/fn_diff.py` + `secdiff.py`)

| variant | raw | aligned | size | .text | .ARM.extab |
|---|---:|---:|---:|---:|---:|
| baseline (pristine) | 3,665 | **545** | 8,080 | 75,919 | 594 |
| `isDockingToStream` arm before asteroid arm | 3,667 | 601 | 8,080 | 75,921 | 594 |
| 95 clause moved after 74 (`mv95`) | 3,751 | 799 | 8,080 | 76,005 | 596 |
| 95 body `switch_to_target_setting=1` first | 4,071 | 733 | **8,084** | — | — |
| instant-action `if` before campaign `if` | 4,716 | 1,140 | **8,092** | — | — |
| `hudMenuOpen=0` before `closeHudMenu()` (hudTouchFlags==0) | 3,677 | 549 | 8,080 | 75,931 | 594 |
| `setFreelanceMission` before reward message (w13798) | 3,667 | 545 | 8,080 | 75,103* | 587* |

\*w13798 numbers are on the pre-case-0 branch base; the aligned value (545)
is unchanged on the current base.

**No size-exact aligned win was found.**  Every block/statement reorder that
matches the original source order either grows the function (+4/+12) or
increases the non-matching-instruction count.

## 2. Cluster analysis (pristine)

`aldiff` reports **196 clusters / 545 non-matching instructions**:

- **138 "local" clusters (sum 271)** — first orig and ours instruction within
  0x40 of each other.  These are still dominated by literal-pool placement
  (runs of `ldr rX,[pc,#N]` + `movs r1,r1` padding) and cold-block splitting,
  not statement order.
- **58 "far" clusters (sum 274)** — block placement (orig/ours blocks > 0x40
  apart).  Biggest: the 95/96 campaign bodies, the weapon-slot body, and the
  `PlayEngineSound` block.

Top clusters (size, orig/ours first addr):

| size | kind | O | U | what |
|---|---:|---|---|---|
| 63 | insert | 17b37a | 17b37a | 95 body placed between 42 and 65 in ours, after 74 in orig |
| 46 | replace | 17b6ec | 17b758 | 96 body / shared `b.n 17b77c` tail |
| 31 | insert | 17ba7a | 17ba7a | weapon-slot body after 110 in ours, between 100 and 110 in orig |
| 25 | replace | 17b9d8 | 17b9c0 | weapon-slot body |
| 22 | replace | 17b3b4 | 17b456 | cargo loop literal pool (`b.n` over 12-word pool) |
| 21 | replace | 17b1e2 | 17b21e | `PlayEngineSound` block |
| 16 | replace | 17b0c2 | 17b0c6 | `PlayEngineSound` block |
| 14 | delete | 17b910 | 17b922 | `isMining()` cold block before 100 body |

## 3. Original statement orders confirmed (from objdump, for a future pass)

- **Campaign clause test order**: 15, 22, 43, 42, 65, 81, 74, 95, 96, 100,
  110, 120, 126, **127**, 134, 144, 155, 161 — bodies at increasing
  addresses (`cmp r0,#N` at 17b114, 17b13e, 17b154, 17b22e, 17b428, 17b4b6,
  17b562, 17b6a4, 17b71c, 17b94e, 17ba3a, 17bad8, 17bbbc, 17bc34, 17bc4c,
  17bd10, 17bd94, 17bdcc).  Ours differs only at 95 (before 65) and 96||127
  (combined).  Reordering 95 after 74 is size-exact but aligned 799 (worse);
  splitting 127 costs +20 B.
- **95 body** (17b6a8): `switch_to_target_setting=1` **first**, then
  `field_64/5c/60/68`, `initStreamOutPosition=1`, `departStation(getStation(10))`,
  `active=0`, `SetCurrentApplicationModule(2)`.  Ours puts the switch store
  after `field_60`.  Making it first grows size to 8,084.
- **Instant-action vs campaign** (17af7e): `isInstantActionMission` is tested
  **first** (`cbz r0,17afa6`), then `isCampaignMission` (17afa6).  Ours tests
  campaign first.  Swapping grows size to 8,092 (a literal pool lands in the
  middle of the `won` block).
- **Freelance-failed else** (17b0b6): `setFreelanceMission(Mission::empty)`
  (17b0be) **before** `showMissionRewardMessage` (17b0de).  Ours is reversed;
  swapping costs +2 raw, aligned unchanged (545).
- **Weapon body tail** (17ba0c): `pauseOpen=0`, `hudMenuOpen=0`,
  `resumeSounds()`, `closeHudMenu()` — matches the current source order.
- **Wingman tail** (17bf2e): `hudMenuOpen=0`, `closeHudMenu()`,
  `pauseOpen=0`, `resumeSounds()` — matches the current source order.
- **dockToAsteroid stream branch** (17bf56): 4 insns then `b.n 17bcf4` to the
  shared `blx`; ours emits 5 insns with its own `blx` (17bf5e).

## 4. Conclusion

The remaining 545 non-matching instructions are dominated by clang 7.0.2 `-Oz`
machine-block and literal-pool placement, which source reordering does not
reach without changing the emitted size (the original's block order is not
reproducible from a size-exact source reorder).  No change landed; `src/` is
clean.
