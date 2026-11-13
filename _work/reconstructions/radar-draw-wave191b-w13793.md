# Radar::draw wave 191b (w13793) — planet-lock bitwise-OR test: aligned 1,339 → 1,336

**Status:** complete, 2026-11-13. Worktree `wt/w13793`, branch `wave191b-w13793`,
base `master` `20d4c73fe`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13784.md` and `radar-draw-wave191b-w13758.md`
(the two preceding Radar passes) and the wave-191b `docs/IDENTICALITY.md` section.

**Landed:** one source-level change in the planet-lock tail, size-neutral
(9,908 B exact), and a strict win on raw, aligned and `.text`.

| pass | change | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---|---:|---:|---:|---:|---:|
| base (session start) | — | 7,089 | 1,339 | 9,908 | 75,750 | 590 |
| 1 (`w13793`) | planet-lock early-out `\|\|` → bitwise `\|` | **6,955** | **1,336** | **9,908** | **75,616** | **590** |

`.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical (29,392 B). Full `check`
gate green after the change: byte_exact 4508, linked_exact 4508, size-exact
4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0, data 843/843,
imports 207, lint CLEAN, parity 5370/5370.

## 1. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 7,089 |
| instruction-index alignment | 1,339 |
| function size | 9,908 / 9,908 |
| `.text` differing bytes | 75,750 |
| `.ARM.extab` differing bytes | 590 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |

Build: `(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)`.

## 2. Landed pass — planet-lock tail

Source (line ~647):

```cpp
if ((missionBlocksPlanets != 0) || (this->planetLockTimer < 1)) {
    goto enemies;
}
```

→

```cpp
if ((missionBlocksPlanets != 0) | (this->planetLockTimer < 1)) {
    goto enemies;
}
```

Original (image base 0x10000):

```
130486  ldr.w r0, [r4, #416]   ; planetLockTimer
13048a  movs  r1, #0
13048c  cmp   r0, #1
13048e  it    lt
130490  movlt r1, #1
130492  orrs.w r0, r5, r1      ; r5 = missionBlocksPlanets
130496  bne.w enemies
```

Baseline ours (`cmp r0,#1; blt.w enemies`) did not materialise the OR at all.
The bitwise form produces:

```
130480  ldr.w r0, [r4, #416]
130484  cmp   r0, #1
130486  it    lt
130488  movlt r5, #1
13048a  cmp   r5, #0
13048c  bne.w enemies
```

which is still not the original's `orrs.w r0, r5, r1` shape (the bool temp is
allocated to `r5` rather than `r1`, and clang drops the `movs r1,#0`), but it is
the closest measured form and a strict improvement.

## 3. Measured variants (all size 9,908 unless noted)

| variant | raw | aligned | `.text` | extab | verdict |
|---|---:|---:|---:|---:|---|
| baseline `\|\|` | 7,089 | 1,339 | 75,750 | 590 | — |
| `bool missionBlocksPlanets` only | 7,033 | 1,344 | 75,694 | 590 | raw win, aligned +5 |
| `planetLockTimer < 1 \|\| missionBlocksPlanets` | 6,953 | 1,346 | — | — | raw win, aligned +7 |
| **`(missionBlocksPlanets != 0) \| (planetLockTimer < 1)`** | **6,955** | **1,336** | **75,616** | **590** | **landed** |
| `missionBlocksPlanets \| (planetLockTimer < 1)` | 6,955 | 1,336 | — | — | identical codegen |
| `(planetLockTimer < 1) \| (missionBlocksPlanets != 0)` | 6,955 | 1,336 | — | — | identical codegen |
| `(missionBlocksPlanets != 0) \| (planetLockTimer <= 0)` | 6,955 | 1,336 | — | — | identical codegen |
| `bool timerDone; mbp \| timerDone` | 6,955 | 1,336 | — | — | identical codegen |
| `int timerDone; mbp \| timerDone` | 6,955 | 1,336 | — | — | identical codegen |
| `int mbp = missionBlocksPlanets; mbp \| ...` | 6,955 | 1,336 | — | — | identical codegen |
| `... \| 0` | 6,955 | 1,336 | — | — | identical codegen |
| `missionBlocksPlanets + (planetLockTimer < 1)` | 6,935 | 1,343 | — | — | size 9,912 — fatal |
| `bool mbp` + `mbp \| (timer<1)` | 6,928 | 1,342 | 87,963 | 589 | size 9,912 — fatal |
| `!(!mbp & !(timer<1))` (De Morgan) | 6,868 | 1,345 | 75,529 | 590 | best raw, aligned +9 |
| `(mbp != 0) ^ (timer<1) ^ 1` | 6,871 | 1,342 | 75,532 | 590 | aligned +6 |
| `bool mbp` + `mbp \|\| (timer<1)` | 7,033 | 1,344 | 75,694 | 590 | neutral vs bool-only |
| `bool mbp` + `int planetHit` | 8,489 | 1,939 | 436,267 | 1,233 | bad |
| `bool mbp` + `int marked` | 7,804 | 1,464 | 88,839 | 588 | bad |
| `bool mbp` + `bool otherLock` | 9,235 | 2,886 | 440,897 | 1,252 | bad |
| `bool mbp` + `int otherLockFlag` | 9,353 | 1,571 | 389,496 | 552 | bad |
| `bool mbp` + `int turretMode` | 9,194 | 2,912 | 441,170 | 1,252 | bad |
| `bool mbp` + `bool alienOrbit` | 9,442 | 1,495 | 389,898 | 553 | bad |
| `2 <= campaign` / `unsigned campaign` / loop `i` as int / moved `alwaysEnemy*` | 7,033 | 1,344 | 75,694 | 590 | neutral |
| `campaign` decl moved before `missionBlocksPlanets` | 8,680 | 1,372 | 384,619 | 552 | size 9,900 — fatal |

The De Morgan form has the best raw byte count (6,868) but worsens the aligned
metric (1,345 vs 1,336), so it was rejected. The landed form is the best on the
aligned metric, which is the metric the preceding Radar passes optimise.

## 4. Campaign / marked region — probed, no win

The assigned `campaign` / `missionBlocksPlanets` / `marked` register-allocation
region (0x12e0d8–0x12e4e4) was re-examined. The structural gap is that the
original keeps `stations` in `sl` and the loop index `i` in `fp`, while ours
spills `stations` to `[sp,#132]` and keeps `i` in `sl`; the original also places
the campaign target-station block inline at 0x12e474 whereas ours places it
out-of-line at 0x12e5ca (the `__builtin_expect(..., 0)` on the campaign
condition drives it cold). Probes, all size-exact and all neutral or worse:

| probe | raw | aligned | size | verdict |
|---|---:|---:|---:|---|
| drop `__builtin_expect(..., 0)` on the campaign condition | 9,341 | 1,623 | 9,980 | worse, size-fatal |
| nest the `ccm < 148 \|\| ccm >= 152` test inside the outer `if` | 7,089 | 1,339 | 9,908 | identical codegen |
| `int isTarget` instead of `bool isTarget` | 7,089 | 1,339 | 9,908 | identical codegen |
| swap `campaignMission` / `freelance` declarations | 9,151 | 1,613 | 9,980 | worse, size-fatal |
| `int marked` | 7,821 | 1,458 | 9,912 | worse, size-fatal |
| hoist `int ccm = getCurrentCampaignMission()` | 9,014 | 2,786 | 9,900 | worse, size-fatal |
| reorder `!otherLock && !planetHit && !turretMode` (5 permutations) | — | — | — | only `otherLock`-first survives; the other slot order is codegen-neutral |

Dropping the `field_0x18 != 0` `__builtin_expect(..., 1)` hint (which is what
produces the original's `cbz r0` at 0x13049c) also regresses: raw 7,022,
aligned 1,346.

`campaign` is dead after the single `cmp.w sl, #2` at 0x12e38a, so its
allocation to `sl` is a pure vreg-numbering artefact, consistent with the
wave-162..167 notes; nothing in this pass changed that.

## 5. EMP block

The EMP block (`getEmpPoints` / `getMaxEmpPoints` / `getEmpDamageRate`,
0x12f2a4–0x12f330) differs only in register allocation (`r5`/`r6` vs
`fp`/`lr`) and in the `DrawRegion2D` argument-setup store order; no
size-neutral source shape was found that improves it.

## 6. Verification

```
raw=6955 aligned=1336 size=9908/9908 insn=3538/3532
.text diff 75616   .ARM.extab diff 590   .ARM.exidx 0   .rodata 0   .rel.plt 0
check: RATCHET PASS (byte 4508 linked 4508 stubs 0 missing 0 extra 0; data 843/843; imports 207)
```
