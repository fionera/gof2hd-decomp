# `Level::createShip` — wave 191b (w13741): four-block order pass, aligned 385 -> 366

Worktree `wt/w13741`, branch `wave191b-w13741`, base master `4c4d81cf7`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13741`.

**Status: landed.**  Raw **1,165 -> 1,190** (+25), aligned **385 -> 366**
(-19), size **2,764/2,764** exact, insns 1,117/1,117.  `.text`
72,954 -> 72,979, `.ARM.extab` contentdiff **564 -> 562** (size stays
156,312), `.ARM.exidx` contentdiff **0**, `.rel.plt` byte-identical.
Gate: RATCHET PASS, byte_exact 4508/4524, linked 4508, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.

## 0. Baseline re-verified

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1165 aligned=385 size=2764/2764
# .text diff 72,954; .ARM.extab diff 564; .ARM.exidx diff 0
```

## 1. The four landed blocks (each measured alone and in combination)

All variants keep size 2,764/2,764 exact.

| variant | raw | aligned | `.text` | extab diff | exidx |
|---|---:|---:|---:|---:|---:|
| baseline (top + setEmpData + setShipGroup guards) | **1,165** | **385** | 72,954 | 564 | 0 |
| h1: guard `getShipGroup`, unguard `setShipGroup` | 1,166 | 383 | 72,955 | 563 | 0 |
| tailF: `setBV` before `setShipGroup` | 1,171 | 381 | 72,960 | 564 | 0 |
| emp40: faithful `min(getLevel(),20)` clamp | 1,175 | 381 | 72,964 | 564 | 0 |
| ASL: race-0 else `ArraySetLength(3)` first | 1,173 | 376 | 72,962 | 563 | 0 |
| emp40 + tailF + h1 | 1,182 | 375 | 72,971 | 563 | 0 |
| ASL + emp40 + h1 | 1,184 | 370 | 72,973 | 562 | 0 |
| ASL + emp40 + tailF | 1,189 | 368 | 72,978 | 563 | 0 |
| **ASL + emp40 + tailF + h1 (landed)** | **1,190** | **366** | **72,979** | **562** | **0** |

### (a) ASL — race-0 non-14 hull: `ArraySetLength(3)` first

Original `0xc086a` calls `ArraySetLength<BoundingVolume*>(3, volumes)` and
`0xc0872` is the first `operator new`; the three `BoundingAAB` news follow.
Our source had the two news before `ArraySetLength`.  Moving it ahead is a
real store/branch-order fix (both the temp-variable and natural forms give
the same code): aligned 385 -> 376, extab 564 -> 563.

### (b) emp40 — faithful clamp shape

Original `0xc0100..0xc011a`:
`blx getLevel / cmp #20 / ble / movs #20 / b / blx getLevel /
add.w r0,r0,r0,lsl#2 / add.w sl,r0,#40`.  Source:

```cpp
int e = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
int empPoints = 40 + e + e * 4;
```

is algebraically identical to the old `(e <= 20 ? e*5 : 100) + 40` for the
whole integer range and reproduces that instruction sequence exactly.  The
`str.w r9,[sp,#56]` spill stays hoisted to `bb.17` (`0xc0108`) instead of the
merge block (`0xc0332`), so this is not a full block match; it still buys
aligned 376 -> 372 on top of ASL.

### (c) tailF — `setBV` before `setShipGroup`

Original `0xc0b38` calls `setBV(volumes)`, then `0xc0b4a` `getShipGroup`,
then the virtual `setShipGroup`, then `geometry`/`addObject`, then
`field_0x40 = 1`.  Our source had `setBV` after `field_0x40 = 1`.  Moving it
to the head of the tail is the faithful order: aligned 372 -> 368 (on top of
ASL+emp40).

### (d) h1 — guard swap

Drop the `EmpGuard` around `fighter->setShipGroup` and guard the
`getShipGroup` call instead.  The LSDA call-site table keeps 57 rows / 336 B
(the guarded call is still at `start >= 128`), so `.ARM.exidx` stays
byte-identical; the record contentdiff drops 563 -> 562 and aligned
368 -> 366.  On the old baseline this swap was a perfect section-byte tie
(72,155 + 563 = 72,718 vs 72,154 + 564 = 72,718); on top of (a)-(c) it is
again a tie (72,979 + 562 = 73,541 vs 72,978 + 563 = 73,541), so the
aligned -2 costs no section bytes.

## 2. Measured but not landed

* `case-reorder`: source `if (0x2c) ... else if (0x33) ... else if (0x31)`
  (the original's compare order at `0xc073c..0xc074a` is 0x2c, 0x33, 0x31,
  with 0x2c and 0x31 sharing the cargo-delete body).  Byte-identical output
  to the current `0x2c/0x31/0x33` order (raw 1,165 / aligned 385 alone;
  identical in the combo), so it is a neutral source-fidelity fix, not a
  measured win; left out to keep the diff minimal.
* `meshId` hoist: a single `fixed->setWreckedMeshId(meshId)` after the
  race-0 if/else would reproduce the original's shared call site at
  `0xc0b2c` (our else branch has an extra inline `mov r0; blx; b.n`), but it
  changes the size (2,776, fatal) and blows the LSDA table, so it is not
  admissible.
* `setShipGroup`-body-first / `shipClass == 1` first: size 2,788 (fatal,
  w13721).
* Faithful all-int spawn before `setEmpData`: size 2,752 (12 B short);
  the landed non-faithful spawn (`z = wpx+wpy`, `x = wpz+jitter3`,
  `y = j1+j2+j3`) remains the only shape that hits 2,764.

## 3. Residual

The remaining aligned bytes are the register rotation in the race-0
type-14 hull block (11 `BoundingAAB` news) and the race-1/2/3 hull blocks.
The original keeps `fixed` in `r9` and `volumes` in `fp`; we keep `fixed` in
`fp` and `volumes` in `r9`.  The root cause is `this`: the original reloads
it from `[sp,#56]` into `r6` at `0xc03fe`, we reload it into `sl` at
`0xc0400`.  That frees `r6` for `fixed` in our allocation and rotates the
whole hull argument setup (`fx`/`y`/`z` = `sl`/`r8`/`r4` in the original vs
`r6`/`r8`/`sl` in ours).  The spawn shape decides which registers are live
across the `new` calls, so the faithful spawn (2,752) and the 2,764 size
constraint are coupled; no source lever found this pass flips `this` back
to `r6` while keeping the size.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13741
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1190 aligned=366 size=2764/2764
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Scratch helpers (all in `scratch/w13741`): `tr.py` (splice + build +
measure), `secdiff.py`, `d.py`, `dd.py`, `c1.py`..`c6.py` (variant
combinations), `Level.cpp.base` (pristine source copy).

## 5. Files changed

* `src/game/world/Level.cpp` — the four blocks above.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md`, `AGENTS.md` — section deltas
  and the residual table row.
* this note.
