# Radar::draw wave 191b (w13717) — gas-spark in-sight geometry order landed (raw +16, aligned −25)

**Status:** complete, 2026-11-13. Worktree `wt/w13717`, branch `wave191b-w13717`,
base `master` `c83a0131e`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13699.md` (enemy lock-blip `setFrame` order,
the current base), `-w13689.md`, `-w13678.md`, `-w13669.md`, `-w13665.md`,
`-w13660.md`, `-w13656.md`, `-w13636.md`, `-w13614.md`.

**Landed:** the gas-cloud / turret spark in-sight test is the original's
X-first form with an `int top`:

```cpp
int size = this->turretScopeHalfWidth;
int left = (int) (PlayerEgo::crosshairPos.x - (float) size);
int top = (int) (PlayerEgo::crosshairPos.y - (float) size);
inSight = this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
          this->screenY < top + size * 2;
```

instead of the Y-first / `float top` / `left > screenX - 2s` spelling.  This is
semantically identical (`left < screenX < left+2s` ⇔ `screenX-2s < left <
screenX`, same for `top`) and makes the whole block instruction-for-instruction
identical to the original `0x12f4a8`–`0x12f4f8` (verified by dumping the patched
build and comparing against `orig.dis`).

`raw` 6,018 → 6,034 (+16), `aligned` 1,927 → **1,902** (−25), size stays
9,908/9,908, `.text` differing bytes 72,116 → 72,132 (+16), `.ARM.extab` 564
(unchanged), `.ARM.exidx` / `.rodata` 0 / 0.  Full `check` gate green
(byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843, imports 207,
lint CLEAN, parity 5370/5370, drift 0).

The raw cost is not a local regression: the block itself becomes byte-identical,
but it is 16 B longer than before, which moves the next re-sync point and
re-shifts the tail of the function (see §2).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,018 |
| instruction-index alignment | 1,927 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 72,116 |
| `.ARM.extab` differing bytes | 564 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Why the raw metric is still dominated by one 4-byte RA shift

The prologue is instruction-identical up to `0x12e3b8`; the first length
divergence is at the `stations` spill:

```
orig  12e3c6  4682       mov   sl, r0            ; stations -> sl
      12e3c8  f04f 0b00  mov.w fp, #0
      12e3cc  2000       movs  r0, #0
      12e3ce  f8cd a074  str.w sl, [sp, #116]    ; 4 B (sl needs the wide store)
      12e3d2  9026       str   r0, [sp, #152]
ours  12e3c6  9021       str   r0, [sp, #132]   ; 2 B (r0 is low, narrow store)
      12e3c8  f04f 0a00  mov.w sl, #0
      12e3cc  2000       movs  r0, #0
      12e3ce  9024       str   r0, [sp, #144]
```

The original keeps `campaign` in `sl` from `0x12e0f8` to its last use
`0x12e38a` (`cmp.w sl, #2`) and only then reuses `sl` for `stations`; the
shared zero lives in `fp`.  Our IR spills `campaign` to `sp+0x90` immediately
and puts the shared zero in `sl`, so `stations` is materialised in `r0` and
stored with the narrow encoding — ours is 4 B shorter here, which shifts every
following instruction by −4 until the next re-sync (gas-cloud loop head
`0x12f40e`, then the music tail `0x130264`).  ~6,005 of the 6,018 raw bytes
are after this address.  Every source spelling tried in w13562/w13614/w13689/
w13699 and this pass leaves that pre-RA invariant; the only shapes that flip
the colouring (`T_update`, hoisted loop bound, `camp_first`) change the loop
IR (the original reloads `this->planetTargets` from `[r4,#320]` every
iteration) and are raw-worse (6,697+).

## 2. Fresh angles measured this pass (all size 9,908 unless noted)

Base = 6,018 / 1,927 / 9,908, `.text` 72,116, `.ARM.extab` 564.

| cell | raw | aligned | `.text` | extab | notes |
|---|---:|---:|---:|---:|---|
| base | **6,018** | **1,927** | 72,116 | 564 | |
| **`gf` gas-spark X-first, `int top`, `screenX<left+2s` (landed)** | 6,034 | **1,902** | 72,132 | 564 | block instruction-identical to orig `0x12f4a8`-`0x12f4f8` |
| `stnswap` station blip `setRefPixelPosition` before `draw` | 6,024 | 1,922 | 72,122 | 564 | orig `0x12f922/948/958` order |
| `os` planet-loop `onScreen` guard before `stationId`/`currentStation` | 6,019 | 1,923 | 72,117 | 564 | orig `0x12e3f2` order |
| `ol1` gas-loop `otherLock` dockNav clause first | 6,029 | 1,924 | 72,127 | 564 | orig `0x12f8a6`-`0x12f8ae` |
| `of` landmark offset ternary polarity (`i==0 ? originX : originY`) | 6,019 | 1,927 | 72,117 | 564 | semantically identical |
| `p` planet bracket X-first | 6,024 | 1,966 | 72,122 | 564 | aligned worse on this base |
| `gasdd` gas `isDead()||isDying()` order | 6,019 | 1,927 | 72,117 | 564 | orig `0x12f5f0/0x12f5fc` |
| `gf`+`stnswap` | 6,040 | 1,897 | 72,138 | 564 | |
| `gf`+`os` | 6,035 | 1,898 | 72,133 | 564 | |
| `gf`+`ol1` | 6,045 | 1,899 | 72,143 | 564 | |
| `gf`+`stnswap`+`os` | 6,041 | 1,893 | 72,139 | 564 | |
| `gf`+`stnswap`+`os`+`of`+`ol1` | 6,053 | **1,890** | 72,151 | 564 | lowest aligned seen |

`gf` alone is the best aligned-per-raw-cost single faithful block fix
(−25 aligned for +16 raw) and the only one that makes a whole block
instruction-identical, so it is the landed change.

### 2.1 Size-fatal cells re-confirmed (do not re-try blindly)

| cell | raw | aligned | size |
|---|---:|---:|---:|
| `rm76` drop redundant wreck `enemy->field_0x76 = 1` | 8,349 | 2,095 | 9,900 |
| `rmred` drop redundant `this->lockedAsteroid = nullptr` in asteroid else-arm | 6,728 | 1,935 | 9,904 |
| `f2` add `asteroidLockTimer += mode` in else-arm | 6,781 | 1,951 | 9,916 |
| `f1` `= 0` in `best > -1` arm | 9,112 | 2,178 | 9,888 |
| `rm76`+`f2` | 7,629 | 2,030 | 9,908 |
| `stnblip` station blip via `blip->` all three | 6,146 | 2,004 | 9,904 |
| `plblip` planet blip via `blip->` all three | 6,177 | 1,926 | 9,904 |
| `camp_first` campaign declared before `turretMode` | 8,557 | 1,923 | 9,900 |
| `pf_vol` / `pf_volint` hoisted volatile 68 local | 9,142 / 9,124 | 2,103 / 2,112 | 9,888 |
| `exp_camp1` / `exp_camp0` | 8,649 / 9,087 | 2,226 / 3,780 | 9,920 / 9,936 |

### 2.2 The `field_0x76` / `{zero,68}` cascade (angle b)

The original stores `enemy->field_0x76` exactly four times (`0x12ee00`,
`0x12eed6`, `0x12f61a`, `0x12f774`); ours stores it five times — the extra one
is the wreck branch (`Radar.cpp:721`), which the on-screen store at `0x12ee00`
already dominates.  Removing it is faithful but −8 B (size-fatal).  It cannot
be compensated by the 68-literal encodings: orig has four `mov.w sl,#68`
(4 B each) and two `movs r6,#68`, ours has two `mov.w fp,#68` and four
`movs r6,#68`, so the whole 12-site group is only 4 B apart, not 8.  The
`rm76`+`f2` compensation is size-exact but raw 7,629.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6034 aligned= 1902 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13717/`: `measure.py`
(build+`fn_diff`+section-diff), `runpatch.py`/`patches.py` (named source-edit
cells), `dumpfn.py` (build a cell and dump the function), `secdiff.py`,
`shift.py`, `align.py`, `orig.dis` / `ours.dis` / `gf.dis`.
