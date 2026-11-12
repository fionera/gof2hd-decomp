# HangarWindow::render — wave 190 (w13468): ship-body upgrade-button call order

**Rejected on current master: raw -10 but `.ARM.extab` content +633, net +623.**

Worktree `wt/w13468`, branch `wave190-w13468`, base master `8e3604cca`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result (raw win, not landed: extab net loss)

In the `item->isShip()` selected-item body (the `tab != 3` upgrade-button
arm) reorder the three `(*this->buttons)[1]` calls to **draw, setPosition,
setVisible** and move `int upgTab = tab;` between `draw()` and
`setPosition()`:

```cpp
(*this->buttons)[1]->draw();
int upgTab = tab;
(*this->buttons)[1]->setPosition(
    Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
    this->field_0x114 + y, 18);
(*this->buttons)[1]->setVisible(true);
buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
```

| metric | base | landed |
|---|---:|---:|
| raw same-address diff | 1,038 | **1,028** |
| instruction-index alignment | 620 | **509** |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical. On current master the section deltas are
`.text` 71,859 -> 71,849 (-10) but `.ARM.extab` 575 -> 1,208 (+633),
so the total (text+extab+build-id) is 72,454 -> 73,077 (+623). It is a
net loss and was not landed.

This is a pure statement-order change in the ship body.  The three
`TouchButton` calls have no data dependence on each other, so the original
source order is recoverable from the call sequence: the original emits
`draw`, `setPosition`, `setVisible` (ours previously emitted
`setVisible`, `draw`, `setPosition`).  The `int upgTab = tab;` snapshot sits
between `draw()` and `setPosition()`; that placement is what keeps the
register allocator from sinking `tab` into the `upgTab == 1` test and is
worth 8 of the 10 bytes.  Size stays exactly 9,700 B.

## Permutation sweep (all 6 call orders x 4 `upgTab` positions)

All size-exact (9,700 B) unless stated; base raw 1,038.

| order | u0 | u1 | u2 | u3 |
|---|---:|---:|---:|---:|
| VPD | 1,191 | **1,036** | 6,621 (9,708) | 6,610 (9,708) |
| VDP | 1,186 | 1,038 | 1,070 | 6,615 (9,708) |
| PVD | 1,191 | 6,657 (9,704) | 6,650 (9,704) | 6,651 (9,704) |
| PDV | 1,189 | 6,658 (9,704) | 6,648 (9,704) | 6,649 (9,704) |
| DVP | 1,188 | 1,039 | 1,071 | 6,616 (9,708) |
| **DPV** | 1,187 | **1,028** | 6,617 (9,708) | 6,606 (9,708) |

`V`=`setVisible(true)`, `P`=`setPosition(...)`, `D`=`draw()`;
`uN` = position of `int upgTab = tab;` in the statement list (0..3).
`DPV u1` is the landed form.

## Residual after this pass

The ship body still differs in the guard (`cbz` vs `cmp`+`beq.w`) and in
the reload/scheduling of `this`/`&Globals::layout` (`ldrd r5,r4` vs
`ldrd r5,r4` at a shifted address), but the call order now matches.
Remaining regions: blueprint `ldrd` tie (0x0605-0x0769), buy-list
`sl`/String-temp tie (0x1815-0x1af8), free-credits preheader
(0x1b7a-0x1df3), buttons[17] (0x2134-0x2236), cleanup landing pads
(0x253c-0x25b5).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
```
