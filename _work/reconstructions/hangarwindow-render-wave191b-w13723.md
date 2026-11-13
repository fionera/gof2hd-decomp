# HangarWindow::render — wave 191b (w13723): guard `cbz` / row-39 ULEB trade, no net win

Worktree `wt/w13723`, branch `wave191b-w13723`, base master `e7548b51f`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed; `src/` untouched

Baseline re-measured and left pristine:

| metric | value |
|---|---:|
| raw same-address diff | **1,040** |
| instruction-index alignment | **516** |
| function size | 9,700 / 9,700 |
| instructions | 3,727 / 3,734 |
| `.text` section diff | 72,159 |
| `.ARM.extab` section diff | **564** |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 |

`check` RATCHET PASS: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub/missing/wrong_type/extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

## New evidence 1: the current base's LSDA row 39 is 172, not 150

The w13707 note says the base row 39 is `(2508,150,9664)`.  On current
master it is **`(2508,172,9664)`** (original `(2508,150,9664)`); the
HangarWindow record contributes only **38** of the 564 extab bytes
(rows 1, 39, 40, 98-135).  This matters because the base's ship-body
order is `setVisible, draw, setPosition` (V,D,P): the first body call
(`setVisible`, offset 2668) is grouped with the `getShip` condition
call, so row 39 ends at 2680 — still `>=128`, so its call-site length
stays a 2-byte ULEB.

The original's row 39 ends at 2658 (right after `getShip`, offset 2654).
Its first body call is `setPosition` (2698), which is in row 40.

## New evidence 2: `(P,V,D)` body in the *base* `else if` shape makes row 39 exact

Keeping the base `if (tab == 0) { } else if (cond) { ... }` shape and
only reordering the ship body to the original's `P,V,D` (with
`int upgTab = tab;` before `P` and the base upgrade-body order) gives:

```
PVD 0 base: raw 1191  aligned 499  size 9700  .text 72310  extab 573
```

The ship-body calls land at exactly the original offsets
(`2698,2712,2724,2736`), row 39 becomes `(2508,150,9664)` **exact**, and
the only structural residue in the guard is the 4-byte
`cmp r0,#0; beq.w 14a6d0` vs the original's `cbz r0,0x149b96`:

```
orig 149b24  cbz   r0, 149b96          ; 2 B, target = upgrade check
ours 149b24  cmp   r0, #0              ; 2 B
ours 149b26  beq.w 14a6d0              ; 4 B, target = end of block
```

The +4 guard shift moves the whole body +4 and costs ~150 raw bytes
(`.text` 72,159 -> 72,310), so it is a net loss (+151 text, +9 extab).

## New evidence 3: why `cbz` is not reachable from source

The original's `cbz` target is the upgrade check at `0x149b96` (114 B
away), and that block *reloads* `tab` (`ldr r0,[sp,#108]`) before
`cmp r0,#1`.  The reload creates a phi at the merge, so clang cannot
prove `tab == 0` there and does not fold the check away.  In every
source shape tried here, clang keeps `tab` in a register across the
ship body, proves `tab == 0 => upgTab != 1`, and branches straight to
the far end (`0x14a6d0`, 2988 B), forcing the 32-bit `beq.w`.

Shapes tried to defeat that propagation (all size-exact unless noted):

| shape | raw | aligned | size | note |
|---|---:|---:|---:|---|
| `if (tab != 0) { if (cond) {...} } if (upg) {...}` (`ne`) | 3300 | 707 | 9704 | still `cmp/beq.w` |
| `if (tab == 0) {} else if (cond) {...} if (upg) {...}` (`eq`) | 928 | 469 | 9700 | sibling, still `cmp/beq.w` |
| same + `tab == 1` instead of `upgTab` | 6882 | 845 | 9700 | |
| `if (tab != 0 && cond)` | 3300 | 707 | 9704 | |
| `if (tab == 0) { buttonOffset = 0; } else if (cond) {...}` | 4652 | 639 | 9704 | |
| `volatile int tab` | 9014 | 2856 | 9692 | |
| `volatile int upgTab = tab;` | 8664 | 832 | 9672 | |

## New evidence 4: sibling `(P,V,D)` is extab-fatal because row 39 shrinks below 128

The `eq` sibling shape (`if (tab == 0) { } else { int upgTab; if (cond) {...} if (upg) {...} }`)
makes the ship body instruction-identical and drops raw to 928, but the
`getShip` call moves into the inner cleanup scope: row 39 becomes
`(2508,118,9664)`.  `118 < 128` makes the call-site length a 1-byte
ULEB, shortening the whole table by one byte and shifting every later
row: `.ARM.extab` 564 -> **1203** (HangarWindow record 38 -> 677).
All six P/V/D permutations of that shape measure extab 1203.

| variant | raw | aligned | size | `.text` | extab | net |
|---|---:|---:|---:|---:|---:|---:|
| **base (V,D,P)** | **1040** | 516 | 9700 | **72159** | **564** | **0** |
| sibling P,V,D (`eq`) | 928 | 469 | 9700 | 72047 | 1203 | +527 |
| sibling V,P,D | 965 | 481 | 9700 | 72084 | 1203 | +564 |
| sibling P,D,V | 942 | 471 | 9700 | 72061 | 1203 | +541 |
| base-shape P,V,D (`PVD 0 base`) | 1191 | 499 | 9700 | 72310 | 573 | +160 |
| base-shape V,P,D (`VPD 1 base`) | 1038 | 595 | 9700 | 72157 | 1197 | +631 |
| base-shape D,P,V (`DPV 1 base`) | 1030 | 593 | 9700 | ~72149 | 1210 | +636 |

The base's V,D,P order is therefore load-bearing: it is the only
size-exact order whose first body call keeps row 39 `>=128` (2-byte
ULEB) while still leaving the blueprint `this`/`&Globals::layout` tie
untouched.

## Other fresh shapes measured (all no better)

* `switch (count)` / `(unsigned)` / `& 0x7fffffff` (the original's real
  `bic.w` mask form): raw 3444, aligned 619, **size 9704** (+4: 2 B
  `bic`, 2 B literal-pool `nop`).  `switch ((unsigned char) count)`
  3411/578/9704.  No -4 lever found that re-aligns the buy-list loop.
* Blueprint `this` field hoists (`pbWidth`, `pbHeight`, `f114` locals):
  7627/793/9704, 7616/755/9704, 7626/818/9704.
* `HangarWindow *self = this;` / `Layout *lay = Globals::layout;` in the
  blueprint body: 7596/742/9704 and 7630/809/9704.
* `name = ...` moved after the blueprint `if`: 1565/588/9700.
* Free-credits `textY` after `SetColor`: 2758/559/9692; `canvas` before
  `textY`: 1042/516/9700.
* buttons[17] `y` parenthesisation / `top * 3` / direct
  `Globals::layout->field_0x20_top`: 1040/516 (neutral) or worse.
* `VDP 1 faithful` (faithful upgrade body V2,P10,V10,D): 1187/497/9700,
  extab 581.
* `PVD 0 faithful`: 6655/685/**9704**.

## Conclusion

The base remains the unique size-exact **net** minimum.  The residual is
still the `this` <-> `&Globals::layout` greedy-queue tie at `0x1496da`
(blueprint body + downstream free-credits/buttons[17]/landing pads), plus
the 4-byte `cbz` guard that the tie's register pressure keeps out of
reach.  Breaking the tie would fix row 39/40 and the downstream clusters
at once; every source shape that flips it re-lays the block or changes
the function size.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13723/` — `HangarWindow.base.cpp`,
`base.so`, `measure.py`, `clusters.py`, `clusterlines.py`, `sbs.py`,
`linemap2.py`, `calls.py`, `sweep_ship.py`, `sweep_base.py`,
`sweep_outer.py`, `sweep_cbz.py`, `sweep_cbz2.py`, `sweep_and.py`,
`sweep_tab0.py`, `sweep_voltab.py`, `sweep_volupg.py`,
`sweep_hoist.py`, `sweep_tie.py`, `sweep_switch.py`, `sweep_y.py`,
`sweep_fc.py`, `pfa.txt` (255 MB `-print-after-all` dump), `check.log`.
