# Radar::draw wave 191b (w13784) — two more hint removals: aligned 1,397 → 1,380

**Status:** complete, 2026-11-13. Worktree `wt/w13784`, branch `wave191b-w13784`,
base `master` `5968bc1ac`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13744.md` (the previous Radar pass) and the
wave-191b `docs/IDENTICALITY.md` update section.

**Landed:** two independent `__builtin_expect`-hint removals, size-neutral
(9,908 B exact), and a strict win on both raw and aligned.

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 6,960 | 1,397 | 9,908 | 74,651 | 562 |
| 1 (`w13784`) | drop hints on `!lockConfirmed` (planet lock) and `alwaysEnemySeen` (`field_0x1bc < 5` music branch) | **6,927** | **1,380** | **9,908** | **74,618** | **562** |

`.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical (29,392 B). Full `check`
gate green after the change: byte_exact 4508, linked_exact 4508, size-exact
4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0, data 843/843,
imports 207, lint CLEAN, parity 5370/5370.

## 1. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,960 |
| instruction-index alignment | 1,397 |
| function size | 9,908 / 9,908 |
| `.text` differing bytes | 74,651 |
| `.ARM.extab` differing bytes | 562 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |

Build: `(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)`.

## 2. Landed pass — sites 8 and 21

The 24 `__builtin_expect` sites in the current base are numbered in file order
(0..23). Removing exactly two of them:

* **site 8** — line 638, `if (__builtin_expect(!this->lockConfirmed, 0))` on the
  planet-lock `Globals::sound->play(26, …)` guard → `if (!this->lockConfirmed)`.
* **site 21** — line 1283, `if (__builtin_expect(alwaysEnemySeen, 1))` in the
  `field_0x1bc < 5` music-selection branch → `if (alwaysEnemySeen)`.

Measured: raw 6,960 → **6,927** (-33), aligned 1,397 → **1,380** (-17), size
9,908 exact, `.text` 74,651 → **74,618**, `.ARM.extab` 562 unchanged.

## 3. Full single-site sweep (24 sites, current base)

Each row removes exactly one hint from the base source and rebuilds. Sizes are
9,908 unless marked fatal. `—` means not measured because the size changed.

| idx | line | site | raw | aligned |
|---|---|---|---:|---:|
| 0 | 349 | `!mission->isEmpty()` | 6,961 | 1,397 |
| 1 | 422 | `landmarks != nullptr && …` | — | — (9,896 fatal) |
| 2 | 428 | `landmarks->data()[i] == nullptr` | — | — (9,972 fatal) |
| 3 | 432 | `onScreen` | — | — (9,972 fatal) |
| 4 | 433 | `screenX > centerX - field_0x128` | 6,960 | 1,397 |
| 5 | 482 | `(i & 0x7fffffff) == 3` | — | — (9,904 fatal) |
| 6 | 558 | campaignMission chain | — | — (9,980 fatal) |
| 7 | 564 | `getCurrentCampaignMission() != 120` | 6,968 | 1,397 |
| 8 | 638 | `!lockConfirmed` | 6,964 | 1,381 (9,904 alone) |
| 9 | 650 | `field_0x18 != 0` | 6,944 | 1,398 |
| 10 | 655 | `isDockingToPlanet()` | 6,945 | 1,389 |
| 11 | 701 | `enemyFlagsLo` | 6,963 | 1,398 |
| 12 | 726 | nested screenX/screenY inSight | — | — (9,904 fatal) |
| 13 | 787 | `enemyFlagsLo` | 6,960 | 1,397 |
| 14 | 823 | `enemyFlagsLo` | 7,161 | 1,485 |
| 15 | 825 | `carriesFriendCargoFlag` | 7,145 | 1,505 |
| 16 | 835 | `!isDockedToDockingPoint` | 7,086 | 1,453 |
| 17 | 837 | `enemy->field_0x24` | — | — (9,904 fatal) |
| 18 | 899 | `cargo == nullptr` | 6,958 | 1,382 |
| 19 | 1005 | `cloud == locked` | 6,960 | 1,398 |
| 20 | 1268 | `current != 151 && alwaysEnemySeen` | 7,016 | 1,390 |
| 21 | 1283 | `alwaysEnemySeen` | 6,954 | 1,399 (9,912 alone) |
| 22 | 1285 | `bossSeen` | 6,881 | 1,393 |
| 23 | 1291 | `alwaysEnemySeen` | 6,934 | 1,405 (9,904 alone) |

Combinations measured (all size 9,908 unless noted):

| combo | raw | aligned |
|---|---:|---:|
| 8+21 (landed) | **6,927** | **1,380** |
| 8+21+20 | 6,983 | 1,373 (size 9,904 on re-measure) |
| 10+18 | 6,938 | 1,372 (9,904 fatal) |
| 10+20 | 7,001 | 1,382 |
| 18+20 | 7,014 | 1,375 |
| 20+22 | 6,937 | 1,386 |
| 18+22 | 6,879 | 1,376 (9,904 fatal) |

`8+21` is the only combination found that improves **both** raw and aligned
while staying size-exact. `8+21+20` lowers aligned a further 7 but costs +56
raw and was re-measured at 9,904 (fatal) by the greedy pass, so it was not
landed. A greedy extension of `8+21` over all size-neutral sites found no
further size-exact aligned improvement.

## 4. Source-shape experiments (all reverted)

| variant | raw | aligned | size | verdict |
|---|---:|---:|---:|---|
| `volatile bool marked` | 9,721 | 3,531 | 10,200 | fatal |
| `bool marked` hoisted out of the planet loop + per-iteration reset | 6,927 | 1,380 | 9,908 | byte-identical (neutral) |
| `marked = getCurrentCampaignMission() != 120; if (marked) DrawImage2D` (Ghidra shape) | 6,935 | 1,380 | 9,908 | +8 raw, no aligned gain |
| `bool missionBlocksPlanets` (+ drop `!= 0`) | 6,929 | 1,377 | 9,912 | +4 B fatal; `.ARM.exidx` 0 → 25 |

## 5. Root cause of the remaining residual (unchanged)

The residual is still dominated by the `campaign` register-allocation artefact:
the original keeps `campaign` in `sl` from `0x12e0f8` to `0x12e38a`
(`mov sl, r0` / `cmp.w sl, #2`), with `missionBlocksPlanets` in `fp` and
`marked` spilled to `sp+0x84`; ours spills `campaign` to `sp+0x90`, puts
`missionBlocksPlanets`/`i` in `sl`, and keeps `marked` in `fp`. That one swap
shifts the whole stack-slot layout and cascades into the campaign block
(`0x12e472`), the landmark `String` temporary (`0x12e9aa`), the enemy-wreck
branch (`0x12f1e6`), the asteroid shared tail (`0x1304d8`) and the music-tail
landing pad (`0x1306a2`). Prior waves (w13730/32/44, w11106/11206) swept
declaration order, types (`int`/`bool`/`char`/`uint8_t`), `const`, and the
`__builtin_expect` cells without moving the allocation; this pass confirms the
same on the wave-191b base.

## 6. Reproduction (scratch, not committed)

`/home/debian/GalaxyOnFire2/scratch/w13784`: `measure.py` (fn_diff + section
byte diff), `align.py` (aligned instruction diff), `sweep*.py` (single-site
hint sweep), `greedy*.py` (greedy size-exact hint removal), `Radar.base.cpp`
(pristine base), `Radar.win.cpp` (landed source).
