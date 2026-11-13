# `Level::createShip` — wave 191b (w13778): clamp polarity, raw 1,190 -> 1,183

Worktree `wt/w13778`, branch `wave191b-w13778`, base master `15873387f`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13778`.  Ghidra MCP was
unavailable this session; evidence is NDK `objdump` side-by-side plus the
`-print-after-all` IR dump (`scratch/w13778/ra.txt`).

**Status: landed.**  Raw **1,190 -> 1,183** (-7), aligned **366 -> 368**
(+2), size **2,764/2,764** exact, insns 1,117/1,117.  `.text`
74,628 -> 74,621, `.ARM.extab` contentdiff **562 -> 562** (size stays
156,312), `.ARM.exidx` contentdiff **0**, `.rel.plt` byte-identical.
Gate: RATCHET PASS, byte_exact 4508/4524, linked 4508, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.

## 0. Baseline

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1190 aligned=366 size=2764/2764
# .text diff 74,628; .ARM.extab diff 562; .ARM.exidx diff 0
```

## 1. The landed change

Original `0xc0100..0xc011a` is `blx getLevel / cmp #20 / ble / movs #20 /
b / blx getLevel / add.w r0,r0,r0,lsl#2 / add.w sl,r0,#40` — i.e. the
`getLevel()` result is the fall-through arm of the clamp.  Our source was
the mirrored `> 20 ? 20 : getLevel()`, which emits `bgt` with the constant
arm falling through.  Flipping the polarity:

```cpp
-    int e = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
+    int e = Globals::status->getLevel() <= 20 ? Globals::status->getLevel() : 20;
```

is algebraically identical for every `int` and reproduces the original
`ble` + fall-through `getLevel()` shape.

## 2. Measured table

| variant | raw | aligned | size | `.text` | extab diff | exidx |
|---|---:|---:|---:|---:|---:|---:|
| baseline HEAD | 1,190 | 366 | 2,764/2,764 | 74,628 | 562 | 0 |
| **clamp-le (landed)** | **1,183** | **368** | **2,764/2,764** | **74,621** | **562** | **0** |

The aligned count rises by 2 because the branch polarity flip moves the
`str.w r9,[sp,#56]` spill: it now sits at `c031c` (between `cmp r0,#20`
and the branch) whereas the original spills at the merge block `c0332`.
The gate's hard count is `byte_exact` (raw), which improves by 7; `.text`
shrinks by the same 7 and `.ARM.extab`/`.ARM.exidx`/`.rel.plt` cost nothing.

## 3. Rejected variants (all size 2,764 unless stated)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `level` clamp polarity le (hitpoints clamp) | 1,204 | 371 | 2,764 |
| both clamps le | 1,204 | 371 | 2,764 |
| faithful spawn: float-from-int before guard | 2,525 | 1,065 | 2,772 |
| faithful spawn: float-from-int after guard | 2,592 | 1,072 | 2,780 |
| faithful ints before guard, `fx` only | 2,451 | 474 | 2,748 |
| faithful ints before guard + `fx/fy/fz` conversions | 2,451 | 474 | 2,748 |
| faithful formulas, landed structure | 1,197 | 374 | 2,764 |
| landed formulas, faithful structure | 2,379 | 453 | 2,768 |
| `x` before, `y,z` after | 2,272 | 470 | 2,756 |
| `z` float + `x,y` int before guard | 2,245 | 495 | 2,768 |
| `z` float before, `x,y` int after | 2,245 | 495 | 2,768 |
| `empTime` separate ternary | 2,204 | 449 | 2,768 |
| `empTime` ternary first | 2,230 | 451 | 2,768 |
| `if (e > 20) e = 20;` clamp form | 2,174 | 445 | 2,756 |
| `LODManager *lod = lodManager;` cache at top | 2,580 | 1,095 | 2,744 |
| clamp-le + `Level *self = this;` | 1,183 | 368 | 2,764 |
| clamp-le + `40 + e*5` / `empTime` first / `const int e` | 1,183 | 368 | 2,764 |
| clamp-le + `y` before `x` | 1,183 | 368 | 2,764 |
| clamp-le + `z` after guard | 1,354 | 404 | 2,764 |

Earlier-pass dead ends (unchanged): `int e = getLevel(); if (e > 20) e = 20;`
2,174/445/2,756; `empTime` before `empPoints` 1,190/366; `bool capital`
1,190/366; `x` before guard 1,191/366; `KIPlayer *ship` before armor
1,190/366; `empPoints = e*5+40` 1,190/366; `switch (shipClass)` 1,190/366;
`empTime = 15000` default-first 1,190/366; `empPoints` both branches
2,235/433/2,768; `const int e` 1,190/366; E1/E2/E3 caches 1,190/366.

## 4. Remaining structural delta

The original tail (`c0b5e..c0b7a`) reloads `this` from `[sp,#56]` into
`r6` before `addObject`, then stores `field_0x40 = 1` and calls
`setLevel(this)` through vtable `[+20]`.  Ours keeps `this` in `sl`
(reload at `c0b34`) and `fixed` in `fp`; the original keeps `fixed` in
`r9`.  The `%694 = COPY $r0` value in `bb.0` is live to the tail
`setLevel(this)` and RA cannot rematerialize it, so the spill placement
(`c031c` vs original `c0332`) is the residual.  This is the root cause of
the 696-byte race-0 type-14 hull rotation and remains open.

## Current-master recheck (2026-11-13, master 20d4c73fe)

The clamp-le change measures raw **1,183** / aligned **368** / size
2,764/2,764 exact on current master, versus the current **1,190 / 366**.
Raw improves by 7 but aligned regresses by 2, so the code change was not
landed. The note is retained for w13792's race-0/box-order work.
