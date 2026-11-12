# Radar::draw wave 183 (w12710) — campaign `sl` vs `sp+0x90` register-allocation pass

**Status:** analysis complete, 2026-11-11; **no size-exact raw win found**, `src/`
left at the base. Worktree `wt/w12710`, branch `wave183-w12710`, base `master`
`9555474d8`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave173-w11808.md` (the original weight accounting),
`radar-draw-wave182-w12609.md`/`-w12611.md` (the 6,322 base this pass starts
from). This note is self-contained: every number below was measured in this
worktree on `9555474d8`.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,322** |
| instruction-index alignment | 1,782 |
| instruction count | 3,543 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| `.text` / `.ARM.extab` / build-id differing bytes | 75,015 / 646 / 20 (total 75,681) |
| file size | 2,187,328 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this
pass; only this note.

## 1. Exact register / spill map (prologue and campaign check)

The residual is the classic one. Original vs ours, byte-for-byte:

```
                        ORIGINAL                              OURS
12e07a  mov.w  fp, #0                 (zero/missionBlocksPlanets)  mov.w  sl, #0
12e086  strb.w fp, [r4,#536]          lockConfirmed = 0            strb.w sl, [r4,#536]
12e08c  strb.w fp, [r4,#304]          plasmaInRange = 0            strb.w sl, [r4,#304]
12e0d8  mov.w  fp, #0                 missionBlocksPlanets = 0     mov.w  sl, #0
12e0e8  str    r0, [sp,#148] (0x94)   turretMode                   str    r0, [sp,#152] (0x98)
12e0f8  mov    sl, r0                 campaign                     str    r0, [sp,#144] (0x90)
...
12e38a  cmp.w  sl, #2                 campaign >= 2                str    r0, [sp,#148] (0x94)  otherLock
12e38e  str    r0, [sp,#144] (0x90)   otherLock                    ldr    r0, [sp,#144] (0x90)  campaign
                                                                   cmp    r0, #2
...
12e3ba  str.w  fp, [sp,#112] (0x70)   spill missionBlocksPlanets   str.w  sl, [sp,#116] (0x74)
12e3c6  mov    sl, r0                 stations -> sl               str    r0, [sp,#132] (0x84)
12e3c8  mov.w  fp, #0                 planet loop index -> fp      mov.w  sl, #0   (index -> sl)
```

So the whole station/planet loop and every later stack slot is rotated by one
4-byte slot in ours: `otherLock` `0x90`->`0x94`, `turretMode` `0x94`->`0x98`,
`missionBlocksPlanets` spill `0x70`->`0x74`, `stations` `sl`->`sp+0x84`, loop
index `fp`->`sl`.  That rotation is the bulk of the 6,322 raw diff; the
campaign check itself is only 2 B longer in ours
(`str`+`ldr`+`cmp` = 10 B vs `cmp.w`+`str` = 8 B).

## 2. Why the allocator picks `sl` for the shared zero

`campaign` is `%416`: one `COPY $r0` at 2128B (bb.1, freq 0.625) and one
`t2CMPri #2` at 24448B (freq 0.6185).  Live interval `[2128B, 24448B)`, size
22,320, totalWeight **1.2435**, weight **5.52e-5**.

The zero / `missionBlocksPlanets` is one phi-eliminated vreg `%2481`: `MOVi 0`
at 240B feeding both `STRBi12` stores (272B, 288B) in bb.1 (0.625 each), plus
the `MOVi 0` re-defs at 880/1088/1296/1504B and the `SUBri/MOVCCi` at 1664B,
plus 0.3173 of later contributions.  Live interval `[240B, 36608B)`, size
33,184, totalWeight **2.2412**, weight **6.71e-5**.

Greedy allocates the higher-weight interval first: the zero gets `r10` (`sl`);
`campaign` then cannot take `r10` and **cannot take `r11` (`fp`)** because its
single range `[2128B, 24448B)` overlaps an interval already living in `fp`, so
it is spilled to `sp+0x90`.  In the original the zero is in `fp` and campaign
in `sl`: campaign was allocated first (higher weight there), took `r10`, and the
zero then took the free `r11` — its range splits at the `12e3ba` spill, before
`fp`'s first post-prologue use at `12e45a`, so `fp` is legal for it.

Flip thresholds (from the interval sizes): campaign must exceed
`2.2412 * 21016/33384 = 1.411`, or the zero must drop below
`1.2435 * 33384/21016 = 1.975`.  Removing **one** of the three bb.1
contributions (0.625) would put the zero at ~1.62 and flip it.

## 3. What was tried this session (all reverted; size must stay 9,908)

Every variant was built with `link_so -j2` under `ulimit -v 12000000` and
measured with `tools/fn_diff.py`; the campaign register was read from the
instruction immediately after the first `getCurrentCampaignMission` `blx`.

### 3.1 Declaration-position sweep (80 positions)

Moving `int campaign = Globals::status->getCurrentCampaignMission();` before
every statement from the function entry to the asteroid loop:

| position | raw | size | campaign |
|---|---:|---:|---|
| base (after `inAlienOrbit`) | 6,322 | 9,908 | spill |
| before `Mission *mission` / between the two stores | 8,572–8,659 | **9,900** | spill |
| after `getEnemies`/`getLandmarks`/`getRoute`/`getAsteroids`/`getGasClouds` | 9,193 | **9,904** | spill |
| after `getPlanetTargets` | 6,399 | 9,908 | spill |
| after the transform / inverse | 9,055 | **9,912** | spill |
| after `onPlanetCourse` | 9,141 | **9,856** | spill |

No position gives campaign-in-`sl` at size 9,908.  (The wave-173 claim that
"campaign before mission block" puts it in `sl` is wrong: it is still spilled,
just at `12e0a6`; size 9,900.)

### 3.2 Full permutation sweeps

* 4-term `otherLock` OR (line 532): all 24 permutations. Base order `0123`
  is best at 6,322; next best 6,324; the `lockedStation`-first orders are
  8,522–8,708 and size 9,940.
* 3-term `otherLockFlag` AND (line 412): all 6 permutations. Base `0123` best
  at 6,322; the rest 6,326–8,447.
* 4-term campaign-check `&&` chain: all 24 permutations. Base best at 6,322;
  the shapes that move campaign into `r9`/`fp` are 9,427–9,537 (sizes
  9,860–9,916).

### 3.3 Zero-splitting attempts (the only lever that could reach 1.975)

The two stores and the phi share one `MOVi 0`; splitting them is the only way
to drop the zero's weight by 0.625.  Clang folds every integer zero form back
to the same constant: `x*0`, `x&0`, `x-x`, `x^x`, `x/256`, `x?0:0`,
`x==0?0:0`, `(uint8_t)0x100`, `(uint8_t)-256`, `(uint8_t)0x10000` — all
**6,322 / 9,908**, campaign still spilled.

Forms that do survive constant folding are all size-changing:

| shape | raw | size | campaign |
|---|---:|---:|---|
| `plasmaInRange = (uint8_t)(crosshairPos.x*0.0f)` | 6,476 | 9,908 | spill |
| `plasmaInRange = (uint8_t)(cameraX*0.0f)` | 6,473 | 9,908 | spill |
| `lockConfirmed = (uint8_t)(cameraX*0.0f)` | 9,457 | **9,896** | **sl** |
| `missionBlocksPlanets = (int)(crosshairPos.x*0.0f)` | 9,460 | **9,816** | spill |
| `missionBlocksPlanets = (int)(cameraX*0.0f)` | 9,249 | **9,932** | **sl** |
| `plasmaInRange = level->getPlayer()==0 && 0` | 6,467 | 9,908 | spill |
| `missionBlocksPlanets = mission->getType()==0 \|\| 0` | 7,906 | **9,916** | **sl** |
| `missionBlocksPlanets = mission->getType()==0 && 0` | 9,108 | **9,912** | spill |

### 3.4 Other campaign-in-`sl` shapes (all size-fatal or raw-worse)

* use cached `campaign` at line 426 instead of a fresh call: 9,548 / **9,892** / `sl`
* use cached `campaign` at line 557 (148/152 gate): 9,160 / **9,920** / `sl`
* remove/flip the existing `__builtin_expect` cells: `fl00` 9,562/**9,888**,
  `fl01` 9,515/**9,896**, `fl02` 9,251/**9,904**, `fl03` 9,532/**9,856**,
  `fl04` 9,590/**9,852** (all `sl`)

The best raw among campaign-in-`sl` shapes is ~9,160, i.e. *worse* than the
6,322 base: any shape that flips the allocation also shifts the following
code, and the raw metric is same-address.

### 3.5 One aligned-only finding (`li_swap`, kept out of `src/`)

Swapping the two `lockImage` arms (test `enemyFlagsLo` first, then
`carriesFriendCargoFlag`) is raw-neutral **6,322** but improves the
instruction-index alignment **1,782 -> 1,778** (4 differing bytes at
`0xbda/0xbe0/0x1306/0x1310`).  It is size-exact and `.rel.plt`-identical, but
does not move the `byte_exact` gate (raw unchanged), so it was not landed under
this pass's "raw win only" rule.  It is a free stepping stone for a later pass.

## 4. Conclusion

The campaign residual is *coupled*: every source shape that moves `campaign`
into `sl` also changes the function size (9,856–9,932) or the raw diff
(9,160–9,590).  There is no size-exact raw win available from the
register-allocation lever alone on this base.  `src/` is left at
`9555474d8`; only this note is committed.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6322 aligned= 1782 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check   # RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12710/` — `Radar.base.cpp`
(exact base), `try.sh`, `sweep_pos.py`, `batch2.py`…`batch6.py`, `perm.py`,
`perm2.py`, `bdiff.txt` (full block diff), and the variant dirs
`b2`…`b6`, `pos`, `pos2`, `perm`, `perm2`.
