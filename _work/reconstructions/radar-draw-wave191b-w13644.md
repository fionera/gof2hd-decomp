# Radar::draw wave 191b (w13644) — blip-draw / store-order blocks: five more real
# divergences, no size-exact raw win

**Status:** complete, 2026-11-13. Worktree `wt/w13644`, branch `wave191b-w13644`,
base `master` `cb3afdf0f`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13632.md` (four structural divergences),
`-w13614.md` (RA flip), `-w13606.md` (IR stability), `-w13593.md` (post-RA map),
`-w13544.md` (enemy guard / adjacent swaps), `-w13440.md` (blip-order sweep),
`-w13489.md` (station-blip pointer spelling).
**No source change landed**: `src/` is untouched; every measured cell is raw-neutral
(6,013) or worse, size-exact unless noted.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,197 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.  `check` is green on the untouched base:
byte_exact 4,508 / linked_exact 4,508 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370.

## 1. New evidence: five more confirmed source-order divergences

This pass reconstructed every blip-draw block and the tractor store blocks from the
Thumb disassembly.  Five places differ from the current source.  Each is a genuine
source-order/association error (not a compiler artefact), and each is size-exact
alone except where noted — but every one is raw-neutral or raw-worse on this base.

### 1.1 Station-lock blip: `setFrame -> setRefPixelPosition -> draw`

Source (`Radar.cpp:516-519`) is `setFrame; draw; setRefPixelPosition`.  The original
(`0x130550`–`0x130588`) is:

```
130550  setFrame(frame)
130556  setRefPixelPosition(crosshairPos.x, crosshairPos.y)
13057c  draw(1.0f, 1.0f)
```

Fix: swap `draw` and `setRefPixelPosition` (keep `blip->draw`).  Size-exact, raw +6.

### 1.2 Station-lock frame: `(timer / field) * (n-1)`, not `(timer * (n-1)) / field`

Source (`Radar.cpp:511-512`) is `(float) stationLockTimer * (count-1) / (float) field_0x1b8`.
The original (`0x130416`–`0x130434`) computes

```
s0 = (float) stationLockTimer
s2 = (float) field_0x1b8
vdiv s0, s0, s2          ; timer / field
s2 = (float) (count-1)
vmul s0, s0, s2          ; (timer / field) * (n-1)
```

i.e. `(float) stationLockTimer / (float) field_0x1b8 * (float) (count-1)`.
Size-exact, raw +1.

### 1.3 Planet-lock frame: same reassociation

Source (`Radar.cpp:661-662`) has the same `(timer * (n-1)) / field` form; the original
(`0x1304ae`–`0x1304d4`) computes `(planetLockTimer / field_0x1b8) * (n-1)`.
Size-exact, raw +5.

### 1.4 Enemy-lock blip: `setFrame -> setRefPixelPosition -> draw`

Source (`Radar.cpp:934-937`) is `setRefPixelPosition; setFrame; draw`.  The original
(`0x12ffe2`–`0x13007a`) is:

```
12ffe2  getRawFrameCount
130044  setFrame(frame)
13006a  setRefPixelPosition(crosshairPos.x, crosshairPos.y)
13007a  draw(1.0f, 1.0f)
```

Fix: swap the first two calls (keep `this->blipSprite` for `setRefPixelPosition`/`draw`,
`blip` for `frames`).  Size-exact, raw +4, **aligned −20** (1,945 vs 1,965).
This is the same shape w13440/w13489 measured; it is the best alignment seen on this
base but not a raw win.

### 1.5 Gas-cloud recheck: `isDead() || isDying()`; `field_0x76` before `field_0x73`

Source (`Radar.cpp:991`) is `cloud->isDying() || cloud->isDead()`; the original
(`0x12f5ee`–`0x12f602`) tests `isDead` first.  Size-exact, raw +1 (same as
w13535-pass2 §2.1).

Source (`Radar.cpp:998-999`) is `cloud->field_0x73 = 0; cloud->field_0x76 = 1;`.  The
original stores `field_0x76 = 1` at `0x12f61a` and the (unconditional) `field_0x73 = 0`
at `0x12f6ca`, *after* the `DrawImage2D` at `0x12f6b2`.  Swapping the two source stores
(`field_0x76` first) is size-exact and **raw-neutral 6,013 with aligned 1,962 (−3)**.

### 1.6 Asteroid tractor store order (both blocks)

Source stores `field_0x1c` then `dockNavPtr` in both asteroid tractor blocks
(`Radar.cpp:1110-1111` and `1165-1166`).  The original stores `dockNavPtr` first in
both (`0x12fce2`/`0x12fce6` and `0x12fcc4`–`0x12fcce`).  Each swap is size-exact and
raw-neutral (6,013); the first is aligned 1,969, the second 1,965.

### 1.7 Gas-cloud `targetPos` before `DrawImage2D` (size-fatal)

Source (`Radar.cpp:1000-1003`) is `egoPos; DrawImage2D; targetPos`; the original
(`0x12f626`, `0x12f63c`, `0x12f6b2`) is `egoPos; targetPos; DrawImage2D`.  Moving
`targetPos` up is size-fatal (+4 B, 9,912) on this base.  Moving only the
`field_0x73 = 0` store after the draw is size-exact but raw +3 (6,016).

### 1.8 Enemy tractor store order (size-fatal)

Source (`Radar.cpp:841-842`) stores `field_0x1c` then `dockNavPtr`; the original
(`0x12f1f2`/`0x12f1f6`) stores `dockNavPtr` first.  The swap is size-fatal (−4 B,
9,904) and re-shifts the whole `.text` (text diff 383,026), raw 8,313.

## 2. Cells measured this pass (all reverted; base 6,013 / 1,965 / 9,908)

| cell | raw | aligned | size | insn | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base | **6,013** | **1,965** | 9,908 | 3541 | 71,197 | 577 |
| 1.1 station blip `sf,rp,dr` | 6,019 | 1,960 | 9,908 | 3541 | 71,203 | 577 |
| 1.2 station frame reassoc | 6,014 | 1,966 | 9,908 | 3541 | 71,198 | 577 |
| 1.3 planet frame reassoc | 6,018 | 1,967 | 9,908 | 3541 | 71,202 | 577 |
| 1.4 enemy blip `sf,rp,dr` | 6,017 | **1,945** | 9,908 | 3541 | 71,201 | 577 |
| 1.5a gas `isDead`-first | 6,014 | 1,965 | 9,908 | 3541 | 71,198 | 577 |
| 1.5b gas `field_0x76`-first | **6,013** | 1,962 | 9,908 | 3541 | 71,197 | 577 |
| 1.6a asteroid tractorAuto swap | **6,013** | 1,969 | 9,908 | 3541 | 71,197 | 577 |
| 1.6b asteroid tractorMode2 swap | **6,013** | 1,965 | 9,908 | 3541 | 71,197 | 577 |
| 1.7a gas `targetPos` up | 8,212 | 2,142 | **9,912** | 3542 | – | – |
| 1.7b gas `field_0x73` after draw | 6,016 | 1,964 | 9,908 | 3541 | 71,200 | 577 |
| 1.7c gas full original order | 6,105 | 1,950 | 9,908 | 3541 | 71,289 | 577 |
| 1.8 enemy tractorAuto swap | 8,313 | 2,026 | **9,904** | 3540 | 383,026 | 577 |
| 1.8 + 1.7a | 6,204 | 1,989 | 9,908 | 3541 | 71,388 | 577 |
| 1.1 + 1.4 | 6,023 | 1,940 | 9,908 | 3541 | 71,207 | 577 |
| 1.2+1.3+1.1+1.4+1.5a | 6,030 | 1,945 | 9,908 | 3541 | 71,214 | 577 |
| 1.5b+1.6a+1.6b | **6,013** | 1,966 | 9,908 | 3541 | 71,197 | 577 |
| 1.5b+1.4 | 6,017 | 1,942 | 9,908 | 3541 | 71,201 | 577 |

No cell reduces raw below 6,013; the best structural-alignment cell (1.4, aligned
1,945) costs +4 raw and leaves `.ARM.extab` unchanged at 577.

## 3. Why no raw win

The residual is still the single greedy-RA colouring described in w13593/w13606: the
shared zero/`missionBlocksPlanets` range takes `sl` (original `fp`) and `campaign` is
spilled to `sp+0x90` (original `sl`).  The first byte difference is the `mov.w sl,#0`
vs `mov.w fp,#0` at `0x12e07a`; the first *length* difference is at `0x12e38a`, where
ours emits `str r0,[sp,#152]; ldr r0,[sp,#144]; cmp r0,#2` (10 B) for the original's
`cmp.w sl,#2` (4 B).  Every block-level source fix above changes the instruction
stream only locally; none flips the RA, so the +6 B seed and the resulting
`0x12e3d4`–`0x12f710` address shift remain.  The per-`0x100` equal-byte profile is
flat (~10–30 equal bytes) across that whole window, so a local +4…+6 B cost is never
recovered by a matching local sequence.

## 4. Conclusion

Five more genuine source divergences are confirmed and documented above
(station blip call order, station/planet frame reassociation, enemy blip call order,
gas `isDead`-first and `field_0x76`-first, asteroid tractor store order).  All are
size-exact except the gas `targetPos` move (+4 B) and the enemy tractor store swap
(−4 B); every one is raw-neutral or raw-worse on this base.  No size-exact raw win
was found; `src/` is untouched and the worktree is clean.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13644/`: `cell.py`/`cellext.py`
(0.9–1.0 s compile+link+strip+`fn_diff`(+section-diff) cell harness),
`Radar.base.cpp` (pristine source), `adiff.py`/`adiff2.py`/`adiff3.py`
(address-annotated instruction alignment), `profile.py` (per-`0x100` equal-byte
profile), `secdiff.py`, `mir.sh` + `mir.txt` (`-print-machineinstrs`), `orig.dis` /
`ours.dis`, `c1.py`–`e10.py` (the cells above).
