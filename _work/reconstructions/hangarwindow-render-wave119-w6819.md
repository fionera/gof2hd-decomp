# HangarWindow::render — wave 119 (w6819): `pad0` anchor -> natural locals

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6819`, branch `wave119-w6819`, base `3981b32c5`.

Builds on `hangarwindow-render-wave119-w6739.md` / `-w6710.md` (91.199559 landed).
This pass: **91.199559 -> 91.337281** (+0.137722).

## Result

| metric | before (HEAD 3981b32c5) | landed |
|---|---:|---:|
| `render` fuzzy (fresh Elf32/Resolver, same normalisation as `verify.py`) | 91.199559 | **91.337281** |
| `render` size | 9700 | 9700 |
| normalised insns (ours/orig) | 3635 / 3626 | 3635 / 3626 |
| byte_exact / linked_exact | 4505 / 4505 | 4505 / 4505 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |
| ELF section count / file size | 27 / 2,187,328 | same |

No section, base or size changed. Gate: **RATCHET PASS**.

## The change

The buy-credits `count == 4` hint block carried a fabricated anchor:

```cpp
(*this->buttons)[12 + count]->setPosition(x, y, 68);
// Codegen anchor: ...
volatile int *pad0 = &x;
(*this->buttons)[12 + count]->replaceTextKeepSize(description);
...
if (count == 4 && Globals::showBestDeal) {
    Globals::Canvas->DrawImage2D(
        this->scrollHintImageA,
        (int) ((float) x + (Globals::iPad ? 0.05f : 0.13f) * (float) this->buttonWidth),
        (int) ((float) y - (float) this->gridButtonHeight * 0.1f), 17, 17);
}
```

It is replaced by two ordinary locals inside the `if` block, and the anchor is
deleted:

```cpp
if (count == 4 && Globals::showBestDeal) {
    int iconX = (int) ((float) x + (Globals::iPad ? 0.05f : 0.13f) * (float) this->buttonWidth);
    int iconY = (int) ((float) y - (float) this->gridButtonHeight * 0.1f);
    Globals::Canvas->DrawImage2D(this->scrollHintImageA, iconX, iconY, 17, 17);
}
```

The two forms are **byte-identical** (`md5 0bfaa44bcf45eb172a93dfc43e14cc22`,
9700 B); the local-variable form is preferred because it is real C++ rather than
a dead-pointer anchor. The gain is entirely in the exception-cleanup region
(`0x240c`-`0x25e4`): its diff cost drops 72 -> 64 elements (total diff cost
639 -> 629). The extra local at that scope boundary perturbs the LSDA cleanup
block ordering just enough to match the original's dispatch table.

## Why this position (measurement)

A dead declaration (`int pad0;` — value/type irrelevant, only the *scope
position* matters) was swept over every line of `render()` (586 positions).
Only one position beats 91.199559:

| position (base `src/game/ui/HangarWindow.cpp`) | fuzzy |
|---|---:|
| first statement of `if (count == 4 && Globals::showBestDeal) {` | **91.337281** |
| just after that `if` line, before `Globals::Canvas->DrawImage2D(` | 91.337281 |
| after `(*this->buttons)[12 + count]->setPosition(x, y, 68);` (old pad0 site) | 91.199559 |
| after `replaceTextKeepSize(description)` | 91.116926 |
| anywhere else in `render()` (584 other lines) | 91.199559 |
| any line in the free-credits `if (visible)` block / buy tail | 91.199559 or worse |

Anchor form at the winning position (all identical codegen):
`volatile int *pad0 = &x;`, `&y`, `&row`, `&count`, `&this->buttonWidth`,
`int pad0;`, `int *pad0 = &x;` -> 91.337281. `volatile int pad0 = 0;` (a real
store) -> 89.026573.

A second dead declaration on top of the winner (swept over all 165 lines of the
buy-credits + free-credits region) never improves on 91.337281.

## Residual map (unchanged)

| region (fn-relative) | diff elems | what |
|---|---:|---|
| `0x570`-`0x592` | 13 | amount block: orig keeps `item` in `r4` across `getStationAmount`; we rematerialise from `sp+120`. Orig loads `this` (`r6`) before the branch and `&Globals::layout` (`r5`) after the call; we `ldrd r5,r6` both before |
| `0x602`-`0x768` | 60 | blueprint block: orig `ldrd sl,r9` (`sl`=`&Globals::layout`, `r9`=`this`) vs ours `ldrd r9,sl` |
| `0xa48`-`0xae8` | 38 | ship branch: orig `ldrd r5,r4` + `cbz r0,0xabe`; ours `ldrd r6,r5` + `cmp/beq.w` (SimplifyCFG threads the `tab==0` edge past the shared `tab==1` test) |
| `0xbb4`-`0xbd2` | 7 | else-arm `item` reload (`mov r0,r4` vs `ldr r0,[sp,#120]`) |
| `0x1766`-`0x1e32` | 311 | buy-credits block: `&Globals::layout` wants `sl`; ours moves it to `r8` (`mov r8, sl` at `0x183c`, `mov sl, r8` at `0x1b9e`/`0x1d34`) and gives `sl` to the `String` temporary |
| `0x1e32`-`0x240c` | 127 | free-credits/non-listMode tail: `&this->buttons` wants `r8`; ours uses `r5` (16/32-bit encoding knock-on) |
| `0x240c`-`0x25e4` | 64 | EH cleanup dispatch table: 2-byte / 16-byte shifts of the `b.n` entries |

Everything between `0x149cac` and `0x14a83e` (2962 B) is instruction-identical.
The call sequence is *identical* to the original (414 calls, same order) except
one swap: the free-credits block calls `getWidth` before `getHeight` in the
original, the other way round in ours.

## Dead ends measured this wave (all <= 91.337281)

Baseline = 91.337281 / 9700 B / 3635:3626.

### Buy-credits listMode loop

| variant | fuzzy |
|---|---:|
| baseline | **91.337281** |
| `String price("")` before `String description("")` | 91.034293 |
| `count < 5` | 91.337281 |
| `count != 5` | 79.481165 (3621 insns) |
| `this->buttons->data()[12 + count]->` for all 5 uses | 91.337281 |
| hoist `String description/price` out of the loop | 87.827045 |
| `TouchButton *button = (*this->buttons)[12 + count];` local | 54.037694 |
| `setVisible(true)` after the `switch` | 86.913444 |
| `row/col/x/y` declared before the `switch` | 69.286794 |
| `x, y` declared outside the loop | 91.337281 |
| `row, col` declared outside the loop | 91.337281 |
| `row, col, x, y` outside | 88.190712 |
| price assigned before description in every case | 90.703760 |
| `description = Globals::cItemListDescription_0X;` (no temp) | 63.015214 |
| `default:` case deleted | 87.434410 (3616 insns) |
| `default:` -> `case 5:` | 87.625602 |
| braced `String tmpD/tmpP` named temps per case | 87.043921 |
| `String(..., false)` explicit bool | 91.337281 |
| `(void) 0;` after each price assignment | 91.337281 |
| `iconY = y - layout->field_0x2c_rowHeight;` local (tabIcons draw) | 88.800110 |
| `iconX = x;` local | 88.800110 |
| `row = count/3; col = count - row*3;` | 84.487268 |
| hoist `rowHeight`/`top` out of the loop | 88.178562 |
| `volatile int *padL = &Globals::w;` before the loop | 91.337281 |

### Free-credits block

| variant | fuzzy |
|---|---:|
| baseline | **91.337281** |
| `getWidth` before `getHeight` (faithful to the original's call order) | 76.937904 |
| ... and `int width` local | 76.480308 |
| ... with `lay` removed | 77.494838 |
| `lay` removed (getHeight first) | 90.806340 (3629 insns) |
| `textYf` before `SetColor` | 88.282327 |
| non-volatile `textYf` | 76.012118 |
| `height` after `SetColor` | 76.325575 |
| `(void) textYf;` after `textY` | 89.038832 |
| `int width = button->getWidth();` before `textX` (getHeight first) | 91.337281 |
| `iconY` local for `tabIcons[5]` draw | 91.337281 |
| natural `(int)((float)x + ...)` / `(int)((float)y - ...)` for `scrollHintImageB` | 74.398007 (3600 insns) |

### Blueprint / ship / amount blocks

| variant | fuzzy |
|---|---:|
| baseline | **91.337281** |
| `float rate = item->bluePrint->getCompletionRate();` hoisted above the `if` | 86.064783 (3629) |
| `float rate;` assigned in the condition | 86.064783 |
| `Item *it = item->item;` used in both amount arms | 62.517213 |
| `Ship *ship = item->ship;` replacing the `padX` anchor | 87.670478 |
| `Ship *ship = item->ship;` in addition to `padX` | 91.337281 |
| `int padF;` at the top of `if (item->isShip())` | 91.337281 |
| `int padF;` at the top of `if (left > 0)` | 91.337281 |
| `int padF;` at the top of `if (right > 0)` | 89.574439 |
| `int padF;` at the top of `if (item == this->selectedItem)` | 65.264323 (3600) |

### Anchors / misc (re-confirmed on the new baseline)

| variant | fuzzy |
|---|---:|
| `int padF;` at every one of the 586 lines of `render()` (one at a time) | 91.337281 except the winner |
| `volatile int pad0 = 0;` at the winning position | 89.026573 |
| `if (count == 4) { if (Globals::showBestDeal) { ... } }` | 91.337281 |
| `if (Globals::showBestDeal && count == 4)` | 91.337281 |
| second dead decl. on top of winner, all 165 buy/free-credit lines | 91.337281 |

## Conclusion

`render` is still a strong local optimum: the residual is the global
callee-saved register assignment (`&Globals::layout` wants `sl` in the
buy-credits block; `&this->buttons` wants `r8` in the tail; `&Globals::layout`
wants `sl` and `this` `r9` in the blueprint block), plus the branch-folder's
`cbz` -> `cmp/beq.w` fold on the ship branch.  The free-credits
`getWidth`-before-`getHeight` order is the only *structural* difference left in
the call stream, but forcing it reallocates the whole function and costs
>= 13 points, so the tuned order is kept.

The one new lever found this wave: a **local variable at the top of the
`if (count == 4 && Globals::showBestDeal)` block** is worth +0.14 by fixing
8 elements of the EH cleanup dispatch table.  It replaces the old `pad0`
dead-pointer anchor with ordinary C++.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j6   # RATCHET PASS byte 4505 linked 4505
# per-variant fuzzy: rebuild Elf32/Resolver each run, like tools/verify/verify.py
```
