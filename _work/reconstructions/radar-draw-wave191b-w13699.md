# Radar::draw wave 191b (w13699) — enemy lock-blip `setFrame` order landed (raw +4, aligned −20)

**Status:** complete, 2026-11-13. Worktree `wt/w13699`, branch `wave191b-w13699`,
base `master` `6e208a911`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13689.md`, `-w13678.md` (enemy on-screen
`field_0x76` order), `-w13669.md` (notTurretMode/notOtherLock order),
`-w13660.md` (gas-blip store order), `-w13647.md` (target brackets),
`-w13644.md` (blip/store order).

**Landed:** reorder the enemy lock-blip block so `setFrame` precedes
`setRefPixelPosition` (and both precede `draw`), matching the original's call
order at `0x130044`/`0x13006a`/`0x13007a`.  `raw` 6,014 → 6,018 (+4),
`aligned` 1,947 → 1,927 (−20), size stays 9,908/9,908, `.text` differing
bytes 71,340 → 71,344 (+4), `.ARM.extab` 577 (unchanged),
`.ARM.exidx` / `.rodata` 0 / 0.  Full `check` gate is green (byte_exact 4508,
linked_exact 4508, size-exact 4524, data 843/843, imports 207, lint CLEAN,
parity 5370/5370, drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,014 |
| instruction-index alignment | 1,947 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,340 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Landed change: enemy lock-blip `setFrame` before `setRefPixelPosition`

Source (`Radar.cpp:936-939`) was:

```cpp
this->blipSprite->setRefPixelPosition((int) PlayerEgo::crosshairPos.x,
                                      (int) PlayerEgo::crosshairPos.y);
this->blipSprite->setFrame((int) (elapsed / total * frames));
this->blipSprite->draw(1.0f, 1.0f);
```

The original (`0x12ffdc`-`0x13007a`) computes `frames`/`elapsed`/`total`,
then calls `setFrame` (`0x130044`), then `setRefPixelPosition` (`0x13006a`),
then `draw` (`0x13007a`):

```
130022 ldr.w r0,[r4,#436]    ; field_0x1b4
130026 sub.w r0,r0,#500
13002a vmov  s4,r0
13002e mov   r0,r5           ; blipSprite
130030 vcvt.f32.s32 s4,s4
130034 vdiv.f32 s2,s2,s4
130038 vmul.f32 s0,s2,s0
13003c vcvt.s32.f32 s0,s0
130040 vmov  r1,s0
130044 blx   Sprite::setFrame
130048 ldr   r0,[pc]         ; crosshairPos
13004e vldr  s0,[r0]
130052 vldr  s2,[r0,#4]
130056 ldr.w r0,[r4,#244]    ; blipSprite
13005a vcvt.s32.f32 s0,s0
13005e vcvt.s32.f32 s1,s1
13006a blx   Sprite::setRefPixelPosition
13006e ldr.w r0,[r4,#244]
13007a blx   Sprite::draw
```

With the reorder the block becomes instruction-for-instruction identical to
the original (modulo the surrounding address shift).  Size-exact; the only
metric movement is the local hunk.

## 2. Fresh angles measured this pass (all size 9,908 unless noted)

Base = 6,014 / 1,947 / 9,908, `.text` 71,340, `.ARM.extab` 577.

| cell | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| base | **6,014** | **1,947** | 71,340 | 577 |
| **`eblip` enemy lock-blip `setFrame` first (landed)** | 6,018 | **1,927** | 71,344 | 577 |
| `sblip` station blip `setRefPixelPosition` before `draw` | 6,020 | 1,942 | 71,346 | 577 |
| `os` planet-loop `onScreen` guard first | 6,015 | 1,943 | 71,341 | 577 |
| `UPD` enemy `update()` before `wreck`/`field_0x75` | 6,016 | 1,941 | 71,342 | 577 |
| `GF` gas-spark in-sight geometry (X-first, int `top`) | 6,030 | **1,922** | 71,356 | 577 |
| `P` planet target bracket X-first | 6,020 | 1,986 | 71,346 | 577 |
| `L` landmark target bracket X-first | 6,021 | 1,980 | 71,347 | 577 |
| `A` asteroid-slot `slotCount`/`field_0x1b0` swap | 6,030 | 1,952 | 71,356 | 577 |
| `ER` enemy range check `>24000` first | **6,014** | 1,960 | 71,340 | 577 |
| `eblip`+`os` | 6,019 | **1,923** | 71,345 | 577 |
| `eblip`+`UPD` | 6,020 | 1,921 | 71,346 | 577 |
| `eblip`+`sblip` | 6,024 | 1,922 | 71,350 | 577 |
| `eblip`+`sblip`+`GF` | 6,040 | **1,897** | 71,366 | 577 |
| `UPD`+`eblip`+`sblip`+`GF` | 6,042 | **1,891** | 71,368 | 577 |
| `RM` drop redundant wreck-branch `field_0x76 = 1` | 8,349 | 2,115 | 383,064 | 577 |
| `RM`+`exp28` (music `alwaysEnemySeen` expect 1→0) | 8,356 | 2,117 | 73,682 | 577 |

`eblip` is the best faithful aligned cell at the minimum raw cost found
(raw +4, aligned −20); `eblip`+`os` reaches aligned 1,923 for raw +5.  `GF`
and the `GF` combinations reach the lowest aligned counts seen (1,891-1,897)
but cost 26-28 raw bytes.

## 3. New evidence: the wreck branch has a redundant `enemy->field_0x76 = 1` (`RM`)

The original stores `enemy->field_0x76` (`+118`) exactly four times in
`Radar::draw`: `0x12ee00` (on-screen block, before `egoPos`), `0x12eed6`
(off-screen block), `0x12f61a` (gas on-screen), `0x12f774` (gas off-screen).
Ours stores it five times: the extra one is the wreck branch at `0x12f15e`
(`movs r0,#1; strb.w r0,[r8,#118]`), which the original does not have — the
on-screen store at `0x12ee00` already dominates it.

Source (`Radar.cpp:720-722`):

```cpp
if (wreck) {
    Globals::Canvas->DrawImage2D(this->image_0x9c, this->screenX, this->screenY, 17, 68);
    enemy->field_0x76 = 1;      // redundant: already set at the top of the on-screen block
} else {
```

Removing it is faithful and shrinks the function to 9,900 B (−8: the
`movs`+`ldr`+`strb.w` hunk).  It is size-fatal on its own because the
original's four `mov.w sl,#68` wide encodings (`0x12ecd0`, `0x12eea2`,
`0x12f128`, `0x12f196`) are exactly 8 B wider than our four `movs r6,#68`
narrow encodings; the extra store currently compensates that.  `RM` + a
size-+8 `__builtin_expect` flip (`exp28`, music tail) is size-exact (9,908)
but raw 8,356 / aligned 2,117, so it is not a win.  The 68-literal encoding
is the same RA `{zero, literal 68} × {r10, r11}` cascade documented in
w13597/w13614.

## 4. Other confirmed faithful divergences (re-measured on this base)

* `os` (planet-loop `onScreen` guard before `stationId`/`currentStation`):
  original `0x12e3ee`-`0x12e40c`; raw +1, aligned −4.
* `UPD` (enemy `update()` before `wreck`/`field_0x75`): original
  `0x12ed5e`-`0x12ed86` calls `update` first; raw +2, aligned −6.
* `GF` (gas-spark in-sight geometry X-first with `int top`): original
  `0x12f4a8`-`0x12f4f8`; raw +16, aligned −25.
* `P`/`L` (planet/landmark target brackets X-first): original
  `0x12e4f2`-`0x12e55a` / `0x12e86a`-`0x12e8b2`; raw +6/+7, aligned worse
  on this base (the RA shift dominates).
* `ER` (enemy range check `> 24000` before `< -24000`): original
  `0x12ee3a`-`0x12ee4c`; raw tie, aligned +13.

## 5. Why there is still no raw win

The raw metric is dominated by the single `campaign`-spill RA shift at
`0x12e3d4` (ours is 4 B shorter there; 6,005 of the 6,014 differing bytes are
after that address).  Every faithful block-level fix is raw-neutral or
raw-worse; the only raw-negative cell found is reverting the landed enemy
on-screen `field_0x76` order (6,013 / 1,955), which is an aligned regression.
`eblip` is landed as an aligned win (raw +4, aligned −20), following the
w13678 precedent (raw +1, aligned −8).

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6018 aligned= 1927 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13699/`: `cells.py`
(compile+link+strip+`fn_diff`+section-diff cell harness), `patches.py`
(candidate patch set), `swaps.py` / `swaps_all.py` (adjacent-statement and
whole-body swap sweeps), `dis.py` / `aln.py` / `shift.py` (disassembly,
instruction alignment and address-shift profile), `orig.dis` / `ours.dis`.
