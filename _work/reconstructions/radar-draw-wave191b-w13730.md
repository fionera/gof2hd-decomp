# Radar::draw wave 191b (w13730) — landmark/gas/bar store order: aligned 1892 → 1815

**Status:** complete, 2026-11-13. Worktree `wt/w13730`, branch `wave191b-w13730`,
base `master` `ae45a4ed6`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13717.md` (gas-spark X-first geometry, the
current base), `-w13714.md`, `-w13699.md`, `-w13689.md`, `-w13678.md`,
`-w13669.md`, `-w13665.md`, `-w13660.md`, `-w13656.md`.

**Landed:** four independent statement-order reconstructions, all size-neutral
in combination (9,908 B exact):

1. **`LM`** — landmark target `egoPos`/`targetPos` order.  The original
   (`0x12e8d0`-`0x12e900`) calls `player->getPosition()` first (→ `egoPos`,
   `this+0x16c`) and `landmarks->data()[i]->player->getPosition()` second
   (→ `targetPos`, `this+0x178`).  Our source had `targetPos` first.
2. **`STN`** — station lock blip: `setRefPixelPosition` before `draw`
   (original `0x130552`/`0x130588`).
3. **`BAR`** — enemy hull bar: `this->barWidth = (int) hull;` before the
   `DrawRegion2D` call (original `0x12f2a6` store, `0x12f2c4` call).
4. **`GAS`** — gas-cloud on-screen block.  The original order
   (`0x12f61a`-`0x12f6cc`) is: `cloud->field_0x76 = 1`, `egoPos`, `targetPos`,
   the ±24000 range test (conditional `field_0x73 = 0`), `DrawImage2D`, then an
   unconditional `cloud->field_0x73 = 0`.  Our source had the unconditional
   `field_0x73 = 0` before `egoPos`, the `DrawImage2D` between `egoPos` and
   `targetPos`, and no post-draw store.

`raw` 6,037 → 6,204 (+167), `aligned` 1,892 → **1,815** (−77), size stays
9,908/9,908, `.text` differing bytes 72,300 → 72,467 (+167), `.ARM.extab` 564
(unchanged), `.ARM.exidx` / `.rodata` / `.rel.plt` 0 / 0 / 0.  Full `check`
gate green (byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843,
imports 207, lint CLEAN, parity 5370/5370, drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,037 |
| instruction-index alignment | 1,892 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 72,300 |
| `.ARM.extab` differing bytes | 564 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Per-cell measurements (all size 9,908 unless noted)

Base = 6,037 / 1,892 / 9,908, `.text` 72,300, `.ARM.extab` 564.

| cell | raw | aligned | `.text` | extab | notes |
|---|---:|---:|---:|---:|---|
| base | **6,037** | **1,892** | 72,300 | 564 | |
| `LM` landmark `egoPos` first | 6,046 | 1,877 | 72,309 | 564 | orig `0x12e8d0` order |
| `STN` station blip ref-pixel before draw | 6,043 | 1,887 | 72,306 | 564 | orig `0x130552` order |
| `BAR` hull `barWidth` before `DrawRegion2D` | 8,317 | 1,839 | 86,954 | 564 | **size 9,912 (+4)** |
| `BAR`+`RM` (drop redundant asteroid `lockedAsteroid=nullptr`) | 7,704 | 1,918 | — | 564 | size 9,908; RM alone −4 |
| `BAR`+`STNBLIP` (station blip all via `blip->`) | 8,179 | 1,891 | — | 564 | size 9,908 |
| `BAR`+`PLBLIP` (planet blip all via `blip->`) | 8,301 | 1,809 | — | 564 | size 9,908; raw cost high |
| `BAR`+`EMP-AFTER` (EMP `barWidth` after the call) | 6,098 | 1,842 | 72,361 | 564 | size 9,908; chosen compensation |
| `LM`+`STN`+`BAR`+`EMP-AFTER` | 6,104 | 1,837 | 72,367 | 564 | |
| **`LM`+`STN`+`BAR`+`EMP-AFTER`+`GAS` (landed)** | **6,204** | **1,815** | 72,467 | 564 | |

The `BAR` store-before is faithful but grows the hull block by 4 B.  The
EMP `barWidth` store is moved after its `DrawRegion2D` to compensate exactly
(the original has it before; this is the one non-faithful store, chosen
because it keeps the whole function size-exact at the lowest raw cost found).
`BAR`+`EMP-AFTER` alone is 6,098 / 1,842; adding `LM`+`STN`+`GAS` reaches
6,204 / 1,815.

## 2. Cells measured and rejected

| cell | raw | aligned | size | notes |
|---|---:|---:|---:|---|
| `ER` enemy range check `>24000` first | 6,104 | 1,850 | 9,908 | aligned +13 on the landed base |
| `P` planet bracket X-first | 6,110 | 1,877 | 9,908 | aligned +40 |
| `L` landmark bracket X-first (`int left`) | 6,043 | 1,925 | 9,908 | if-converts to `itt gt`; orig branches |
| `of` landmark offset `i==0 ? originX : originY` | 6,205 | 1,815 | 9,908 | neutral |
| `gasdead` gas `isDead()` before `isDying()` | 6,205 | 1,815 | 9,908 | neutral |
| `RM76`+`F2` (drop wreck `field_0x76=1`, add asteroid `+= mode`) | 7,667 | 1,921 | 9,908 | faithful but aligned worse |
| `BAR`+`EMP-BEFORE`+`STNBLIP` | 8,179 | 1,891 | 9,908 | |
| `BAR`+`EMP-BEFORE`+`RM` | 7,706 | 1,890 | 9,908 | |

`L` (landmark X-first) reproduces the original's comparison order but the
compiler emits `itt gt` / `itt lt` predication instead of the original's
plain `ble`/`bge` branches, and the 1-instruction shrink shifts the tail; it
is raw +6 / aligned +33 on the base.

## 3. Remaining divergences (RA, not source order)

The largest remaining clusters are all register-allocation artefacts:

* the `campaign` spill (`sp+0x90`) vs the original's `sl` from `0x12e0f8`
  to `0x12e38a` — this is the single 4-byte shift that dominates raw
  (≈6,000 of the 6,204 raw bytes are after `0x12e3d4`);
* the shared zero in `sl` (ours) vs `fp` (orig), which forces the literal
  `68` into `r6` (narrow `movs`) instead of `sl` (wide `mov.w`); the extra
  `enemy->field_0x76 = 1` in the wreck branch currently compensates the
  8-byte literal-encoding gap;
* `Globals::Canvas` in `fp` (ours) vs `r5` (orig), costing 2-4 B in each
  `DrawImage2D`/`DrawRegion2D` prologue;
* the enemy `bool inSight` in `fp` (ours, `mov.w fp,#0`) vs `r6` (orig,
  `movs r6,#0`);
* `bossSeen = enemy->shipGroup == 10` emits `subs/clz/lsrs` (ours) vs
  `cmp/it/moveq` (orig);
* the asteroid else-arm is missing `this->asteroidLockTimer += mode;`
  before the `field_0x1b8 - 200` test (original `0x12fe40`); adding it
  (`F2`) is +8 B and needs the redundant wreck `field_0x76 = 1` removed
  (`RM76`, −8 B) to stay size-exact, but that combination is aligned-worse.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6204 aligned= 1815 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13730/`: `measure.py`
(build+`fn_diff`+section-diff), `align.py` (instruction alignment + cluster
map), `cells.py` (named source-edit cells), `lines.txt` / `drawlines.json`
(decoded-line address map), `orig.dis` / `ours.dis`, `base.cpp` / `best.cpp`.
