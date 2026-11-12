# HangarWindow::render — wave 189 (w13348): ship-guard call order, raw −5 B

Worktree `wt/w13348`, branch `wave189-w13348`, base master `f258b29c0`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.
The `0x14a960` tail is the `Globals::layout` value materialization: the
original keeps `&Globals::layout` in `sl` and dereferences it at each use
(`ldr.w r1,[sl]` at `0x14ab02`, `ldr.w r2,[sl]` at `0x14abc2`); ours spills
the pointer and materialises the value at `0x14a960` (`ldr.w sl,[sl]`) with a
restore at `0x14ac64`.  Removing the `lay` local makes the function 9,696 B
(the buy-list `(1-col)` fold loses `mla`), so the tail is not landed here.

## Result (landed)

| metric | wave-189 base | landed |
|---|---:|---:|
| raw same-address diff | 1,494 | **1,360** |
| instruction-index alignment | 523 | 530 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 73,953 | **73,819** |
| `.ARM.extab` section diff | 615 | 612 |
| render LSDA record diff | 57 | 54 (948 B / 156 rows both) |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 74,588 | **74,451** |
| file size | 2,187,328 | 2,187,328 |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.

## What changed

Inside the `item->isShip()` guard the three independent `buttons[1]` calls
and the width read are reordered from `setPosition, setVisible, draw,
getWidth` to `setVisible, draw, setPosition, getWidth`:

```cpp
if (tab != 0 && __builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1)) { // lint: arm_builtin
    int upgTab = tab;
    (*this->buttons)[1]->setVisible(true);
    (*this->buttons)[1]->draw();
    (*this->buttons)[1]->setPosition(
        Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
        this->field_0x114 + y, 18);
    buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
    if (this->upgradeMode && upgTab == 1) { ... }
}
```

The original source order is `setPosition, setVisible, draw, getWidth`, so
this is a codegen-shape choice, not a faithful reorder.  It is a real
same-address raw win: the `setPosition` argument setup moves after the two
virtual calls, which lets clang reuse the already-live `layout`/`hintOffsetX`
values instead of re-materialising one of them.  The function stays exactly
9,700 B.

## LSDA

Row 39 (the `Status::getShip()` call site) is still exact
`(2508, 150, 9664)`.  The reorder moves the cleanup-scope start of row 40:

```
row  orig                          ours
40   (2696, 140, 9562)             (2680, 158, 9538)
41   (2854,   6, 9664)             (2856,   6, 9664)
42   (2874,  26, 9356)             (2876,  26, 9356)
43   (2912,   4, 9224)             (2914,   4, 9224)
44   (2918,  32, 9328)             (2920,  32, 9328)
```

The record diff is 58 bytes (base 57); `.ARM.extab` grows by 1 byte, far
below the 5-byte raw win.

## Negative results (all size 9,700 unless stated)

| variant | raw | aligned | extab | total |
|---|---:|---:|---:|---:|
| wave-189 base (`s,v,d,b`) | 1494 | 523 | 615 | 74,588 |
| **`v,d,s,b` (landed)** | **1489** | 545 | 616 | **74,583** |
| `v,d,s,b` + upgrade block `p,d,v2,v` | 1489 | 545 | 616 | 74,583 |
| `v,d,s,b` + upgrade block `p,d,v,v2` | 1489 | 538 | 616 | 74,584 |
| `d,s,v,b` / `d,v,s,b` / `s,d,v,b` / `s,v,d,b` | 1494-1514 | 523-545 | 615-616 | ≥74,588 |
| w13302 ship-guard shape (original call order) | 1244 | 481 | 1,234 | 74,957 (net loss) |
| `lay` local removed (address form) | 3515 | 537 | 760 | 9,696 B (size short) |
| `lay` qualifier/decl-order/assignment-order sweeps | — | — | — | neutral or 9,696 B |
| guard/upgrade-condition/for-hint/buy-list swaps | — | — | — | neutral or worse |

The w13302 candidate is not landed: it reaches raw 1,244 but grows
`.ARM.extab` by +595 (net loss).  The address form is 4 B short because the
buy-list `x` expression keeps `rsb`+`muls` instead of folding to `mla`; no
`x`-form/decl-order/assignment-order variant recovered 9,700 B.

## Second pass (landed): move `int upgTab = tab;` after `setVisible`

The first ship-guard reorder landed raw 1,489.  A second, larger win comes
from moving the `upgTab` snapshot between the two independent virtual calls
of the guard body:

```cpp
(*this->buttons)[1]->setVisible(true);
int upgTab = tab;
(*this->buttons)[1]->draw();
(*this->buttons)[1]->setPosition(...);
buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
```

This is semantically identical (`upgTab` is a pure copy of `tab`, used only
by the later `upgradeMode && upgTab == 1` check), but it changes the
lifetime.start placement of the body scope and the register pressure across
the `draw()` call.  Measured: raw **1,489 -> 1,404** (-85), aligned
545 -> 542, size **9,700**.  `.text` 73,948 -> 73,863 (-85), `.ARM.extab`
616 -> **615** (-1), build-id 19 -> 20, total 74,583 -> **74,498** (-85).
The render LSDA record diff drops 58 -> 57; row 39 becomes
`(2508, 172, 9664)` (orig `(2508, 150, 9664)`) and row 40
`(2688, 148, 9512)` (orig `(2696, 140, 9562)`), but rows 41-44 stay exact
and the section does not grow.

Position sweep (forced recompile; `upgTab` moved after each call):

| `upgTab` position | raw | size | extab | total |
|---|---:|---:|---:|---:|
| before `setVisible` (first pass) | 1,489 | 9,700 | 616 | 74,583 |
| **after `setVisible`, before `draw` (landed)** | **1,404** | **9,700** | **615** | **74,498** |
| after `draw`, before `setPosition` | 1,421 | 9,700 | 1,229 | 75,129 (net loss) |
| after `setPosition`, before `getWidth` | 7,503 | 9,716 | 1,975 | 396,373 |
| after `getWidth` | 7,503 | 9,716 | 1,975 | 396,373 |

The `after_draw` shape is a raw win (1,421) but grows `.ARM.extab` by 613,
so it is a net loss.  The landed `after_setVisible` shape keeps the extab
diff at 615.

## Third pass (landed): upgrade-button order `p,v2,v,d`

The upgrade block (`upgradeMode && upgTab == 1`) has four independent calls.
With the new `upgTab` placement, a forced-recompile sweep of all 24 orders
finds `setPosition, buttons[2]->setVisible, buttons[10]->setVisible, draw`
best:

```cpp
if (this->upgradeMode && upgTab == 1) {
    (*this->buttons)[10]->setPosition(...);
    (*this->buttons)[2]->setVisible(true);
    (*this->buttons)[10]->setVisible(true);
    (*this->buttons)[10]->draw();
}
```

Measured: raw **1,404 -> 1,381** (-23), aligned 542 -> 532, size **9,700**.
`.text` 73,863 -> 73,840 (-23), `.ARM.extab` stays 615, build-id stays 20,
total 74,498 -> **74,475** (-23).  The LSDA record diff stays 57.

Upgrade-order sweep (forced recompile, top rows):

| order | raw | aligned | size | extab | total |
|---|---:|---:|---:|---:|---:|
| **`p,v2,v,d` (landed)** | **1,381** | 532 | 9,700 | 615 | **74,475** |
| `p,v,v2,d` | 1,382 | 530 | 9,700 | 615 | 74,475 |
| `p,v,d,v2` | 1,396 | 530 | 9,700 | 615 | 74,490 |
| `p,v2,d,v` | 1,397 | 534 | 9,700 | 615 | 74,490 |
| `p,d,v,v2` / `p,d,v2,v` (previous) | 1,404 | 535/542 | 9,700 | 615 | 74,498 |
| `d,p,v2,v` / `d,p,v,v2` | 1,479 | 516 | 9,700 | 619 | 74,577 |

## Fourth pass (landed): hint the upgrade condition

Wrap the upgrade-block condition in `__builtin_expect(..., 1)`:

```cpp
if (__builtin_expect(this->upgradeMode && upgTab == 1, 1)) { // lint: arm_builtin
```

Measured: raw **1,381 -> 1,360** (-21), aligned 532 -> 530, size **9,700**.
`.text` 73,840 -> 73,819 (-21), `.ARM.extab` 615 -> **612** (-3), build-id
stays 20, total 74,475 -> **74,451** (-24).  The render LSDA record diff
drops 57 -> 54.

Upgrade-condition forms (forced recompile):

| form | raw | aligned | size | extab | total |
|---|---:|---:|---:|---:|---:|
| **`__builtin_expect(upgradeMode && upgTab == 1, 1)` (landed)** | **1,360** | 530 | 9,700 | 612 | **74,451** |
| plain (previous) | 1,381 | 532 | 9,700 | 615 | 74,475 |
| `upgTab == 1 && upgradeMode` | 1,400 | 530 | 9,700 | 615 | 74,494 |
| `__builtin_expect(..., 0)` | 1,435 | 529 | 9,700 | 615 | 74,529 |

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
        /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13348/`
(`HangarWindow.base.cpp`, `HangarWindow.w13302.cpp`, `HangarWindow.addr.cpp`,
`sweep_body2.py`, `secdiff.py`, MIR/IR dumps).
