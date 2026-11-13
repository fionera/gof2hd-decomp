# Radar::draw wave 191b (w13797) — music-ladder `bossSeen` hint removal

**Status:** complete, 2026-11-13. Worktree `wt/w13797`, branch `wave191b-w13797`,
base `master` `96c2c1627`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13793.md` (previous pass) and the wave-191b
`docs/IDENTICALITY.md` update section.

**Landed:** dropped the `__builtin_expect(bossSeen, 1)` hint on the
`field_0x1bc < 5` music-ladder rung (source line 1289). Size-neutral (9,908 B
exact) and a strict win on both raw and aligned.

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 6,948 | 1,335 | 9,908 | 75,139 | 587 |
| 1 (w13797) | drop `expect(bossSeen, 1)` at line 1289 | **6,844** | **1,330** | **9,908** | **75,035** | **587** |

`.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical. Full `check` gate green:
byte_exact 4508, linked_exact 4508, size-exact 4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, parity 5370/5370.

## 1. Full single-site `__builtin_expect` sweep on the current base

The wave-191b base carries 22 hints (0-indexed in file order). Each row removes
exactly one hint and rebuilds. `—` marks a fatal size change.

| idx | line | site | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 0 | 349 | `!mission->isEmpty()` | 6,949 | 1,335 | 9,908 |
| 1 | 422 | `landmarks != nullptr && …` | 8,184 | 1,411 | 9,900 |
| 2 | 428 | `landmarks->data()[i] == nullptr` | 9,256 | 1,766 | 9,976 |
| 3 | 432 | `onScreen` | 9,256 | 1,766 | 9,976 |
| 4 | 433 | `screenX > centerX - field_0x128` | 6,948 | 1,335 | 9,908 |
| 5 | 484 | `(i & 0x7fffffff) == 3` | 8,512 | 1,385 | 9,904 |
| 6 | 560 | campaignMission chain | 9,336 | 1,600 | 9,980 |
| 7 | 566 | `getCurrentCampaignMission() != 120` | 6,956 | 1,335 | 9,908 |
| 8 | 652 | `field_0x18 != 0` | 6,947 | 1,345 | 9,908 |
| 9 | 657 | `isDockingToPlanet()` | 6,918 | 1,368 | 9,904 |
| 10 | 704 | `enemyFlagsLo` (outer) | 6,952 | 1,336 | 9,908 |
| 11 | 729 | nested screenX/screenY inSight | 6,961 | 1,373 | 9,908 |
| 12 | 791 | `enemyFlagsLo` (bar) | 6,948 | 1,335 | 9,908 |
| 13 | 827 | `enemyFlagsLo` (frame) | 7,736 | 1,407 | 9,912 |
| 14 | 829 | `carriesFriendCargoFlag` | 7,132 | 1,442 | 9,908 |
| 15 | 839 | `!isDockedToDockingPoint` | 7,077 | 1,392 | 9,908 |
| 16 | 841 | `enemy->field_0x24` | 8,475 | 1,378 | 9,904 |
| 17 | 903 | `cargo == nullptr` | 6,954 | **1,320** | 9,908 |
| 18 | 1009 | `cloud == locked` | 6,948 | 1,336 | 9,908 |
| 19 | 1272 | `current != 151 && alwaysEnemySeen` | 7,004 | 1,328 | 9,908 |
| 20 | 1289 | `bossSeen` (`field_0x1bc < 5`) | **6,844** | **1,330** | **9,908** |
| 21 | 1295 | `alwaysEnemySeen` (`else`) | 6,956 | 1,336 | 9,908 |

Note the wave-191b base already contains w13784's `8+21` removals, so the
surviving hint cells renumber; the old wave-167/w13784 tables are stale.

## 2. Combinations with site 20 (all measured)

| combo | raw | aligned | size |
|---|---:|---:|---:|
| **20** | **6,844** | **1,330** | **9,908** |
| 0+20 | 6,845 | 1,330 | 9,908 |
| 4+20 | 6,844 | 1,330 | 9,908 |
| 7+20 | 6,852 | 1,330 | 9,908 |
| 10+20 | 6,848 | 1,331 | 9,908 |
| 11+20 | 6,857 | 1,368 | 9,908 |
| 12+20 | 6,844 | 1,330 | 9,908 |
| 17+20 | 6,832 | **1,312** | 9,904 fatal |
| 18+20 | 6,844 | 1,331 | 9,908 |
| 19+20 | 6,900 | 1,323 | 9,908 |
| 21+20 | 6,944 | 1,330 | 9,904 fatal |
| 8+20 | 6,886 | 1,338 | 9,904 fatal |

`17+20` is the best aligned result (1,312) and also beats site 20 on raw, but it
is 4 B short (one 32-bit instruction relaxes to 16-bit), which is gate-fatal.
Site 20 alone is the only size-exact candidate that improves both metrics.

## 3. EMP block (distinct angle requested) — exhausted, no win

The original keeps the canvas slot address in `r5` (`ldr r5,[sp,#140]`) and
re-issues `ldr r0,[r5,#0]` immediately before each call; ours keeps the slot
address in `fp` and hoists the second deref (`ldr.w r6,[fp]` before
`getEmpDamageRate`), adding a `mov r0,r6`.  Every source shape that changes the
`empCanvas` local's lifetime/declaration order is neutral or size-fatal:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,948 | 1,335 | 9,908 |
| `empCanvas` declared after the `barWidth` store | 7,533 | 1,340 | 9,912 |
| `empCanvas` declared before the `getEmpPoints` guard | 7,533 | 1,340 | 9,912 |
| second call uses `Globals::Canvas` directly | 7,533 | 1,340 | 9,912 |
| `empCanvas = Globals::Canvas;` re-assigned before the call | 7,533 | 1,340 | 9,912 |
| `PaintCanvas *empCanvas(Globals::Canvas)` | 6,948 | 1,335 | 9,908 |
| `auto *empCanvas = Globals::Canvas` | 6,948 | 1,335 | 9,908 |
| inline `emp` expression twice (no `float emp`) | 8,471 | 1,509 | 9,940 |

The block is a stack-slot/regalloc artefact of the frame-size gap (see §4), not
a source-shape difference.

## 4. Planet-lock / turret-mode region — measured, no win

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `DrawImage2D(..., (unsigned char) 17, (unsigned char) 68)` (both sites) | 6,948 | 1,335 | 9,908 |
| `otherLock = a \| b \| c \| d` (bitwise) | 9,134 | 2,865 | 9,936 |
| `otherLock = a \|\| b \|\| c \|\| d` (implicit pointer-to-bool) | 6,948 | 1,335 | 9,908 |
| `(int)(a != nullptr) \| …` | 9,134 | 2,865 | 9,936 |
| `if (!otherLock && !turretMode && !planetHit)` (line 598 swap) | 9,179 | 2,822 | 9,948 |
| `if (!turretMode && !otherLock && !planetHit)` | 9,179 | 2,822 | 9,948 |

The `12e4dc` OR tree is `(otherLock|planetHit)|turretMode` in the original and
`(otherLock|turretMode)|planetHit` in ours, but the source is already in the
original's order — the tree shape is a vreg-numbering artefact of the same
frame-size gap.

## 5. Root cause (unchanged)

Ours allocates a 304-byte frame (`sub sp,#304`, canary `[sp,#300]`); the original
allocates 296 (`sub sp,#296`, canary `[sp,#292]`).  The 8 extra bytes sit below
the 64-byte `MatrixGetInverse` temporary (`add r5,sp,#240` vs `add r5,sp,#232`),
which shifts every named local by +8..+20 and drives the `campaign`-in-`sl`
allocation, the landmark `String` temporary slot, the `12f104` tractor-block
order and the `1306a2` music-tail landing pad.  Prior waves (campaign-sl
w162..w167, w13784) swept decl order/types/`const`/hint cells without moving it.

## 6. Reproduction (scratch, not committed)

`/home/debian/GalaxyOnFire2/scratch/w13797`: `probe.py`, `batch.py`,
`sweep_expect.py` (single-hint sweep), `combo.py` (multi-hint),
`try.sh` (`-j2`, 12 GB `ulimit -v`), `raw.py`/`range.py`/`cls.py`/`secdiff.py`.
