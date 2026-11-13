# HangarWindow::render — wave 191b (w13548): free-credits / buy-list / upgrade-body sweeps

Worktree `wt/w13548`, branch `wave191b-w13548`, base master `6e55ef9d6`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed

Baseline restored and re-confirmed: raw **1,038**, size 9,700/9,700,
render LSDA record diffbytes **38**, `.text` 71,259, `.ARM.extab` 576,
`.rodata` 485, `.rel.plt` identical, `check` RATCHET PASS.

Per-region raw diff (unchanged): blueprint 63, ship 159, grid_head 36,
grid_body 76, grid_tail 27, freecredits 416, buttons17 200, cleanup 52.

## The one big raw win is a net loss (confirmed, not landed)

Re-measured the w13522 shape (hoist `int upgTab = tab;` above the ship
guard, PVD body order, `buttons[2]`-setVisible-first upgrade order), and
the slightly better `upgTab`-without-`__builtin_expect` variant:

| variant | raw | size | extab record | `.text` | `.ARM.extab` |
|---|---:|---|---:|---:|---:|
| base | 1,038 | 9,700 | 38 | 71,259 | 576 |
| w13522 hoist (ABCE) | 928 | 9,700 | 677 | 71,149 | 1,215 |
| hoist + no upgrade `__builtin_expect` | **922** | 9,700 | 676 | 71,143 | 1,214 |

Both RATCHET PASS.  Raw saves 110–116 B but `.ARM.extab` grows 638–639 B
→ **net −522…−529** by the "extab growth must not exceed raw savings"
rule, so neither was landed (w13522 was already landed once and reverted
in w13527 for exactly this reason).

Region split of the 922 variant: blueprint 63, ship **21** (from 159),
grid_head 36, grid_body 76, grid_tail 27, freecredits 416, buttons17 200,
cleanup 53.  So the hoist genuinely fixes the ship/upgrade body; the
remaining cost is entirely the LSDA.

## Root cause of the extab blowup (new, precise)

The hoist gives `upgTab` its own cleanup scope across the `getShip`
invoke (clang 7.0.2 does this even for a trivial `int`), so `getShip`
unwinds to the ship-guard landing pad instead of the function pad.  That
splits LSDA row 39:

```
orig  row 39 (2508, 150, 9664)      row 40 (2696, 140, 9562)
hoist row 39 (2508, 118, 9664)      row 40 (2654, 182, 9598)
```

`118 < 128`, so the row-length ULEB shrinks from 2 bytes to 1 and the
whole call-site table shifts by one byte → 677 differing record bytes
(rows 0–38 are otherwise *byte-identical* to the original).  **If row 39
could be kept ≥128 bytes the 922 shape would become landable.**  The last
call in row 39 is `ListItem::isShip` at `0x9cc+118 = 0xa42`; the original
additionally covers `getShip` at `0xa5e`.  Adding a harmless call between
`DrawString` and the `if (item == selectedItem)` guard would extend the
row, but every such call tried so far also changes the function size.
An inserted `Globals::Canvas->SetColor(0xffffffffu);` before the
`if (item == selectedItem)` guard in the 922 shape does exactly this:
extab record 676 -> **332**, but size 9,688 (-12) and raw 8,711 — so the
mechanism is confirmed but a size-neutral way to extend row 39 is still
needed.

## Negative results measured this wave (all size 9,700 unless noted)

Free-credits block (`int rh/width/height`, `textX`, `textY`, `canvas`):

* Declaration permutations × `SetColor` placement (18 combos):
  `rh,width,height` + `SetColor` after `textY` = 1,038 (base);
  `rh,width,height` + `SetColor` before `textX` = 2,756 / **9,692**
  (size-fatal); `width,rh,height` = 1,088; `height,rh,width` = 1,040;
  `height,width,rh` = 2,809 / 9,704.
* `textX` after `textY` = 1,038 (neutral).  `canvas` declared before
  `textY` = 1,040.  `volatile int textY` = 1,040.  `volatile int
  *textYv = &textY` = 1,038 (neutral).  `int textY; textY = …` = 1,038.
* `no_rh_var` (inline `Globals::layout->field_0x2c_rowHeight`) = 1,088;
  `no_width_var` = 1,040.
* Faithful oracle order (`setVisible, y, setPosition, draw` +
  `width,rh,height` + `SetColor` before `textY`) = 2,792 / **9,692**.
  The `SetColor`-before-`textY` shape is consistently −8 B; the base's
  `SetColor`-after-`textY` is what keeps 9,700.

Ship / upgrade-button body:

* All 24 upgrade-op permutations (A=setVisible(2), B=setPosition(10),
  C=setVisible(10), E=draw(10)) with the single merged guard: only the
  base `BACE` = 1,038; next best `BCAE` = 1,041; `ABCE` (oracle order) =
  1,187.
* `setVisible(2, true)` literal instead of `this->upgradeMode` = 1,040.
* Two-if (upgrade check outside the guard) + ABCE = 6,875 / extab 718;
  + hoisted `upgTab` = 928 / 677.
* Body order `PVD` with body-scope `upgTab` = 1,191 / extab 47;
  `VDP`+ABCE with body-scope `upgTab` = 1,186 / 48; without the upgrade
  `__builtin_expect` = 1,076 / 44.  All raw-worse than base.

Buy-list / cleanup:

* `switch (count)` (plain int) reproduces the original's
  `bic.w r0,r9,#0x80000000 / cmp r0,#4 / tbb [pc,r0]` guard but is
  +4 B and raw 3,442.  Combined with the −8 free-credits shape = 9,696.
* Removing the loop `__builtin_expect` = 9,736 / 377 (the "plain form
  9,700→9,696" anchor in the source comment refers to a different arm).
* `drawBox(3, …, String())` = 8,473 / 9,692; hoisting a shared
  `String emptyBox("")` = 9,237 / 9,772.
* Swapping the `drawBox(10)`/`drawBox(4)` arms = 1,581 / 51;
  swapping `drawBox(9)`/`drawBox(3)` = 8,632 / 9,716.
* `Layout *layout = Globals::layout;` local at the `viewMode == 0`
  scope with all `Globals::layout->` inside rewritten = 9,216 / **9,576**
  (size-fatal).  (Function-scope local was already known: 9,508.)

## Established (carried forward)

* The residual is dominated by one register-allocation tie-break:
  `&Globals::layout` is in `sl` and `this` in `r9` in the original, but
  the reverse in ours, from the `ldrd sl,r9,[sp,#132]` reload at
  `0x602` onward.  Everything before `0x605` is byte-identical; the
  `ldrd` operand order is the trigger.
* Free-credits `sl`/`r9` roles, buy-list `r5`/`sl` String temps, and the
  `sp+132` vs `sp+128` layout spill all follow from that same swap.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13548/` — `base.cpp` (pristine),
`batch31.py`…`batch55.py` (the sweeps above), `regions.py`, `sbs.py`,
`secdiff.py`, `sweep.py`.
