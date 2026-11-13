# Radar::draw wave 191b (w13744) — `__builtin_expect` sweep: aligned 1718 → 1482

**Status:** complete, 2026-11-13. Worktree `wt/w13744`, branch `wave191b-w13744`,
base `master` `4c4d81cf7`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13732.md` (the current base), `-w13730.md`,
`-w13727.md`, `-w13720.md`, and the older Radar notes.

**Landed:** two independent `__builtin_expect`-hint removals, both size-neutral
(9,908 B exact), plus a note-only record of the full 27-site sweep (no further
size-safe aligned win found).

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 6,436 | 1,718 | 9,908 | 72,956 | 564 |
| 2 (`eaabbb49e`) | drop 3 expects on music-tail ladder (`field_0x1bc >= 1`, `< 3`, `< 5`) | 6,511 | 1,622 | 9,908 | 73,031 | 564 |
| 3 (`df17bdc74`) | drop 4 more hints (freighter status bit, nested `isDockingToPlanet`, `enemyLockTimer > …`, `dockNavPtr != current`) | **6,853** | **1,482** | **9,908** | **73,373** | **564** |

**Merge note (coordinator, 2026-11-13):** landed on top of `c1f2671c4`
(w13751 frame-math reassociation) rather than `eaabbb49e`.  Combined result on
`2655cec94` + w13751: raw **6,511 -> 6,858**, aligned **1,622 -> 1,459**
(-163), size 9,908 exact; `.text` 74,637 -> 74,984.

Honest accounting: **raw same-address diff grew +417** and **`.text` differing
bytes grew +417** while the instruction-index alignment fell **236**. The aligned
metric is the win metric used here (it is the one that tracks statement/branch
order); the raw/.text growth is the usual consequence of removing hints that
were pulling code into the wrong shape. `.ARM.extab` stayed at 564, `.ARM.exidx`
0, `.rodata` 0, `.rel.plt` byte-identical. Full `check` gate green after pass 3:
byte_exact 4508, linked_exact 4508, size-exact 4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.

## 1. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,436 |
| instruction-index alignment | 1,718 |
| function size | 9,908 / 9,908 |
| `.text` differing bytes | 72,956 |
| `.ARM.extab` differing bytes | 564 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |

Build: `(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)`.

## 2. Pass 2 — music-tail ladder (`eaabbb49e`)

Removed the three `__builtin_expect` hints on the music-selection ladder
`field_0x1bc >= 1`, `< 3`, `< 5` (the `play()` id ladder around `0x1302d8`).
This also moved the music `else` branch from ours `0x1304e2` to `0x1302de`,
matching the original's `0x1302f0` / shared tail `0x130326` placement.

| metric | before | after |
|---|---:|---:|
| raw | 6,436 | 6,511 |
| aligned | 1,718 | 1,622 |
| size | 9,908 | 9,908 |
| `.text` | 72,956 | 73,031 |
| `.ARM.extab` | 564 | 564 |

## 3. Pass 3 — four more hints (`df17bdc74`)

Dropped the hints at:

1. the campaign-125 freighter `getStatusValue() & bit` test,
2. the nested `isDockingToPlanet()` planet-dock test,
3. `enemyLockTimer > ((navWreck || field_0x24) ? 500 : 0) && dockNavPtr != nullptr`,
4. `dockNavPtr != current`.

| metric | before | after |
|---|---:|---:|
| raw | 6,511 | 6,853 |
| aligned | 1,622 | 1,482 |
| size | 9,908 | 9,908 |
| `.text` | 73,031 | 73,373 |
| `.ARM.extab` | 564 | 564 |

The old 47/46-byte enemy-lock cold-block clusters disappeared entirely after
this pass. The `((((…))))` parenthesisation produced while editing was cleaned
up before commit and re-measured byte-identical (6,853 / 1,482), so the committed
tree is clean.

## 4. Full single-site expect-removal sweep (27 sites)

Each row removes exactly one `__builtin_expect` from the pass-2 base source
(line numbers are from that file), rebuilds, and measures. All sizes 9,908
unless marked fatal. `—` means not measured because the size changed.

| idx | line | site | raw | aligned |
|---|---|---|---:|---:|
| 0 | 349 | `!mission->isEmpty()` | 6,512 | 1,622 |
| 1 | 422 | `landmarks != nullptr && …` | — | — (9,900 fatal) |
| 2 | 428 | `landmarks->data()[i] == nullptr` | 6,531 | 1,622 |
| 3 | 432 | `onScreen` | 6,530 | 1,684 |
| 4 | 433 | `screenX > centerX - field_0x128` | 6,511 | 1,622 |
| 5 | 482 | `(i & 0x7fffffff) == 3` | — | — (9,904 fatal) |
| 6 | 558 | campaignMission chain | — | — (9,968 fatal) |
| 7 | 564 | `!= 120` | 6,521 | 1,622 |
| 8 | 581 | freighter `getStatusValue` | 6,518 | **1,588** |
| 9 | 640 | `!lockConfirmed` | — | — (9,900 fatal) |
| 10 | 650 | `field_0x18 != 0` | 6,510 | 1,622 |
| 11 | 655 | nested `isDockingToPlanet` | 6,503 | 1,624 |
| 12 | 701 | `enemyFlagsLo` | 6,515 | 1,623 |
| 13 | 726 | bracket X test | — | — (9,904 fatal) |
| 14 | 787 | `enemyFlagsLo` | 6,511 | 1,622 |
| 15 | 824 | `enemyFlagsLo` (0) | 6,671 | 1,723 |
| 16 | 826 | `carriesFriendCargoFlag` | 6,698 | 1,721 |
| 17 | 836 | `!isDockedToDockingPoint` | 6,639 | 1,694 |
| 18 | 838 | `enemy->field_0x24` | — | — (9,904 fatal) |
| 19 | 900 | `cargo == nullptr` | — | — (9,904 fatal) |
| 20 | 916 | `enemyLockTimer > …` | 6,830 | **1,524** |
| 21 | 919 | `dockNavPtr != current` | 6,514 | 1,622 |
| 22 | 1007 | `cloud == locked` | 6,513 | 1,623 |
| 23 | 1275 | `current != 151 && alwaysEnemySeen` | — | — (9,904 fatal) |
| 24 | 1290 | `alwaysEnemySeen` | 6,512 | 1,624 |
| 25 | 1292 | `bossSeen` | — | — (9,904 fatal) |
| 26 | 1298 | `alwaysEnemySeen` | — | — (9,904 fatal) |

Combinations measured (all size 9,908):

| combo | raw | aligned |
|---|---:|---:|
| 8+20 | 6,837 | 1,490 |
| 8+20+11 | 6,848 | 1,484 |
| 8+20+11+21 (landed, pass 3) | **6,853** | **1,482** |

## 5. Remaining aligned clusters at 1,482

`python3 align.py` on the committed `.so` (orig address / ours address):

| n | orig | ours | note |
|---:|---|---|---|
| 27 | `0x1306a2` | `0x13066a` | music-tail landing-pad/jump-table placement. Original has `b.n 1306a4/1306aa/1306ac`; ours has three `b.n 1306e2` at `1306a2/1306a4/1306a6`. LSDA cleanup ordering, not a statement-order issue. |
| 27 | `0x1304d8` | `0x1304ce` | asteroid blip `setFrame/setRefPixelPosition/draw`. Original keeps `this` in `r4` and blipSprite in `r5` (`ldr.w r5,[r4,#244]` @`0x1304ea`); ours uses `r6` and `ldr.w r5,[r6,#244]!`. Register-allocation artefact from the shared tail. |
| 22 | `0x12e9aa` | `0x12e98e` | landmark / `String` temporary. |
| 22 | `0x12e472` | `0x12e476` | campaign range test (`campaign` spill vs `sl`). |
| 22 (delete) | `0x12f1e6` | `0x12f1e8` | enemy wreck branch `DrawImage2D` / `isDead`. |
| 21 | `0x130440` | `0x130456` | |
| 21 | `0x12f13a` | `0x12f15a` | |
| 17 | `0x12f10c` | `0x12f114` | |
| 16 | `0x12eb3c` | `0x12eaf4` | |
| 16 | `0x12e5f8` | `0x12e5c8` | |
| 16 (delete) | `0x13054a` | `0x13057a` | |
| 16 (delete) | `0x12ebc2` | `0x12ebba` | |
| 15 | `0x12e9e6` | `0x12e99c` | |
| 14 | `0x12f370` | `0x12f34a` | |
| 14 | `0x12ef1e` | `0x12eefc` | |
| 14 | `0x12e7e4` | `0x12e7bc` | |
| 13 | `0x12f66a` | `0x12f666` | |
| 12 | `0x12efba` | `0x12efbc` | |
| 12 | `0x12e798` | `0x12e772` | |

Known register-allocation artefacts still present: `campaign` spill vs `sl`;
`Globals::Canvas` fp vs `r5`; `inSight` fp vs `r6`; `bossSeen` clz shape;
`field_0x73` `r6` vs `fp`; `marked` `fp` vs stack.

## 6. Conclusion

The remaining aligned delta is dominated by LSDA/landing-pad placement and
ARM register-allocation artefacts rather than statement or condition order.
Further expect-hint removal from this base is size-fragile (11 of 27 single-site
removals changed the function size) and the only size-safe combinations found
have already landed. Next work on this function should target the exception
cleanup structure (which `String` temporaries exist and their scopes) or the
asteroid-blip shared-tail shape, not more hint sweeps.

## 7. Tooling (scratch, not committed)

`measure.py`, `align.py`, `deltamap.py`, `delta.py`, `sweep1.py`, `rmexp.py`,
`exp.py`, `gf.py`, `gl.py`, plus `orig_decomp.c` (full original Ghidra decompile,
1,681 lines) under `/home/debian/GalaxyOnFire2/scratch/w13744`.
