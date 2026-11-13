# Radar::draw wave 191b (w13727) — station lock-blip draw order landed (aligned 1892→1887)

**Status:** complete, 2026-11-13. Worktree `wt/w13727`, branch `wave191b-w13727`,
base `master` `6fbc91363` (the w13717 gas-spark X-first commit). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`),
9,908 B. Read with `radar-draw-wave191b-w13717.md` (the current base), `-w13714.md`,
`-w13699.md`, `-w13689.md`, `-w13678.md`, `-w13669.md`, `-w13665.md`, `-w13660.md`.

**Landed:** the station lock-blip tail now emits `setFrame`, `setRefPixelPosition`,
`draw` instead of `setFrame`, `draw`, `setRefPixelPosition`:

```cpp
this->blipSprite->setFrame(frame);
this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                      (int) PlayerEgo::crosshairPos.y);
blip->draw(1.0f, 1.0f);
```

This is the original's order at both station-blip arms — `0x1304f8`/`0x13051e`/`0x13052e`
and `0x130552`/`0x130578`/`0x130588` (the `stationLockTimer` block, `[r4,#420]`).  The
gas-blip block (`0x12f922`/`0x12f948`/`0x12f958`) already matched; the station block did not.

`raw` 6,037 → 6,043 (+6), `aligned` 1,892 → **1,887** (−5), size 9,908/9,908,
`.text` differing bytes 72,175 → 72,181 (+6), `.ARM.extab` 564 (unchanged),
`.ARM.exidx`/`.rodata` 0/0.  Full `check` gate green (byte_exact 4508, linked_exact 4508,
size-exact 4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0, data 843/843,
imports 207, lint CLEAN, parity 5370/5370).  `.rel.plt` unchanged (no new imports).

The +6 raw is a tail-shift artefact, not a local regression: the block itself becomes
instruction-identical, but the reorder changes the length of a pre-sync region and re-shifts
the bytes after it (see §2).  The aligned metric is the one that measures the block.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,037 |
| instruction-index alignment | 1,892 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 72,175 |
| `.ARM.extab` differing bytes | 564 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |

## 1. Remaining root cause: the `campaign` spill RA invariant

The prologue is instruction-identical up to `0x12e07a`, where the shared zero constant is
coloured `fp` in the original and `sl` in ours.  The original keeps `campaign` in `sl`
(`0x12e0f8` … `cmp.w sl,#2` at `0x12e38a`) and `missionBlocksPlanets` in `fp`; ours keeps
`missionBlocksPlanets` in `sl`, `marked` in `fp`, and spills `campaign` to `sp+0x90`
(`str r0,[sp,#144]` at `0x12e0f8`, reloaded at `0x12e38c`).  At the `stations` spill this
makes ours 4 B shorter (`str r0,[sp,#132]` vs the original's `str.w sl,[sp,#116]`), shifting
everything after `0x12e3c6` by −4 until the next re-sync.  All source spellings tried this
pass leave the colouring:

| cell | raw | aligned | size |
|---|---:|---:|---:|
| `camp_const` (`const int campaign`) | 6,037 | 1,892 | 9,908 |
| `camp_sep` (declare then assign) | 6,037 | 1,892 | 9,908 |
| `camp_unsigned` | 6,038 | 1,892 | 9,908 |
| `camp_before_alien` | 6,044 | 1,893 | 9,908 |
| `camp_after_pt` | 6,114 | 1,897 | 9,908 |
| `camp_before_turret` | 8,556 | 1,888 | 9,900 (size-fatal) |
| `alien_bool` | 6,046 | 1,940 | 9,908 |
| `turret_int` | 9,226 | 2,379 | 9,860 (size-fatal) |
| `mb_bool` / `mb_uint` / `mb_char` | 6,196 / 6,037 / 6,037 | 1,892 | 9,912 / 9,908 / 9,908 |
| `marked_int` / `planetHit_int` | 8,464 / 9,070 | 1,952 / 2,230 | 9,912 / 9,956 (size-fatal) |
| `notflags_bool` / `notflags_inline` | 9,505 / 9,553 | 2,076 / 2,045 | 9,876 / 9,868 (size-fatal) |

## 2. Fresh angles measured this pass (all size 9,908 unless noted)

Base = 6,037 / 1,892 / 9,908, `.text` 72,175, `.ARM.extab` 564.

| cell | raw | aligned | `.text` | extab | notes |
|---|---:|---:|---:|---:|---|
| base | **6,037** | **1,892** | 72,175 | 564 | |
| **`stnswap` station blip setRefPixel before draw (landed)** | 6,043 | **1,887** | 72,181 | 564 | orig `0x1304f8/0x13051e/0x13052e` |
| `stnswap`+`L2` | 6,043 | 1,887 | 72,181 | 564 | `L2` alone is byte-identical |
| `L2` landmark bracket `(int)left < screenX && (int)left+2s > screenX` | 6,037 | 1,892 | 72,175 | 564 | already matches semantically |
| `swap_ao_camp` | 6,044 | 1,893 | 72,182 | 564 | |
| `stnswap`+`swap_ao_camp` | 6,050 | 1,888 | 72,188 | 564 | |
| `ol1` gas-loop `otherLock` dockNav clause first | 6,046 | 1,892 | 72,184 | 564 | |
| `stnswap`+`ol1` | 6,052 | 1,887 | 72,190 | 564 | |
| `gas_move` gas `DrawImage2D` after the 24000 test | 6,135 | 1,884 | 72,273 | 564 | faithful order, but keeps the extra `field_0x73=0` store |
| `stnswap`+`gas_move` | 6,141 | **1,879** | 72,279 | 564 | lowest aligned seen |
| `stnswap`+`gas_move`+`ol1` | 6,150 | 1,880 | 72,288 | 564 | |
| `gas_rm73` drop unconditional `cloud->field_0x73 = 0` | 8,250 | 2,061 | 382,945 | 564 | size 9,900, exidx 1,047 (size-fatal) |
| `gas_both` move draw + drop store | 8,246 | 2,053 | 382,941 | 564 | size 9,900 (size-fatal) |

`stnswap` alone is the best aligned-per-raw-cost single faithful block fix and the only one
that makes a whole residual block's call order match the original, so it is the landed change.
`stnswap`+`gas_move` reaches aligned 1,879 but costs +104 raw and leaves the extra
`field_0x73` store, so it is not landed.

### 2.1 Size-fatal cells re-confirmed (do not re-try blindly)

| cell | raw | aligned | size |
|---|---:|---:|---:|
| `rm76` drop redundant wreck `enemy->field_0x76 = 1` | 8,351 | 2,060 | 9,900 |
| `gas_rm73` | 8,250 | 2,061 | 9,900 |
| `gas_both` | 8,246 | 2,053 | 9,900 |
| `camp_before_turret` | 8,556 | 1,888 | 9,900 |
| `turret_int` | 9,226 | 2,379 | 9,860 |
| `marked_int` / `planetHit_int` | 8,464 / 9,070 | 1,952 / 2,230 | 9,912 / 9,956 |
| `notflags_bool` / `notflags_inline` | 9,505 / 9,553 | 2,076 / 2,045 | 9,876 / 9,868 |

## 3. Call-sequence divergence inventory (unchanged, for the next pass)

`tools/fn_diff`-style call-sequence diff (`calls.py`) still shows these genuine order/placement
divergences, none of which is source-controllable without changing size or the RA colouring:

- planet loop: `getTargetStation`/`getCurrentCampaignMission` block placed far from the range
  check (orig `0x12e47e`, ours `0x12e5d4`); `DrawImage2D(Ejii)` placement (`0x12e676` vs
  `0x12e66e`); `Mission::getType` placement (`0x12ebdc` vs `0x12eb10`).
- enemy wreck branch: ours emits an extra `DrawImage2D` earlier (`0x12ef46`) and `isDead`
  placement differs (`0x12f040`/`0x12f158` vs `0x12f058`/`0x12f18e`).
- gas loop: `isDead`/`isDying` order (orig `0x12f5f0` isDead then isDying; ours reverse —
  the `gasdead` cell is faithful but raw +1, aligned unchanged); `getPosition`/`operator=`
  placement (orig `0x12f6fe`, ours `0x12f660`) is the `gas_move` cell above.
- enemy/asteroid lock-blip block: ours emits the whole
  `getRawFrameCount`/`setFrame`/`setRefPixelPosition`/`draw` earlier (`0x12ff62`) than the
  original (`0x12ffe2`); the block is structurally identical, just shifted.
- music tail: `inAlienOrbit` placement (`0x1302ac` vs `0x1302b4`), an extra
  `getCurrentCampaignMission` in the original at `0x1302fa`, `FModSound::stop`/`inAlienOrbit`/
  `getStation`/`isAttackedByAliens` placement (`0x13038e` vs `0x130378`), and
  `hudEvent`/`getPlayer` order (`0x1305dc` vs `0x1305ea`).

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6043 aligned= 1887 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13727/`: `measure.py` (build + `fn_diff` +
section diff), `cell.py` (named source-edit cells), `dump.py` (`orig.dis`/`ours.dis`),
`calls.py`, `rawruns.py`, `shiftmap.py`, `byline.py`, `ann.py`.
