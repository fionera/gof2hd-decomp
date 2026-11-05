# HangarWindow::render — wave 119 (w6710): exhaustive local-variant sweep, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6710`, branch `wave119-w6710`, base `47a1396bd`.

Builds on `_work/reconstructions/hangarwindow-render-wave118-w6612.md`
(91.199559 landed). This pass: **91.199559 -> 91.199559** (no improvement
found). The deliverable is this measurement record plus the new negative
results below.

## Result

| metric | before (HEAD 47a1396bd) | landed |
|---|---:|---:|
| `render` fuzzy (fresh Elf32/Resolver, same normalisation as `verify.py`) | 91.199559 | **91.199559** |
| `render` size | 9700 | 9700 |
| normalised insns (ours/orig) | 3635 / 3626 | 3635 / 3626 |
| byte_exact / linked_exact | 4503 / 4503 | 4503 / 4503 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |
| ELF section count / file size | 27 / 2,187,328 | same |

No section, base or size changed. Gate: **RATCHET PASS**.

## Method

Every variant below was applied to a pristine copy of
`src/game/ui/HangarWindow.cpp`, rebuilt with
`cmake --build cmake-build-match --target link_so -j6` (1.1 s) and scored with a
harness that rebuilds the `Elf32`/`Resolver` pair on every measurement (the
same way `tools/verify/verify.py` does — reusing resolvers across variants
mis-names branches and gives wrong scores). Baseline is 91.199559 %,
9700 B, 3635/3626 insns.

## Residual map (unchanged from wave 118)

| region (fn-relative) | what |
|---|---|
| `0x570`-`0x592` | selected-item amount block: orig keeps `item` in `r4` across `getStationAmount`, we rematerialise `item->item` from `sp+120`; orig loads `this` (`r6`) before the branch and `&Globals::layout` (`r5`) after the call, we `ldrd r5,r6` both before |
| `0x602`-`0x768` | blueprint block: orig `ldrd sl,r9` (`sl`=`&Globals::layout`, `r9`=`this`) vs ours `ldrd r9,sl` — the global `r8`/`sl`/`r9` role swap |
| `0xa48`-`0xae8` | ship branch: orig `ldrd r5,r4` + `cbz r0,0xabe`; ours `ldrd r6,r5` + `cmp/beq.w 0x15f8` (branch-folded past the redundant `tab==1` test) |
| `0xbb4`-`0xbd4` | else-arm `item` reload |
| `0x1766`-`0x1a2a` | `pV` store (4 B) and the `mov r8,sl` copy in the buy-credits preheader (2 B) |
| `0x1b0e`-`0x1e60` | buy-credits tail + free-credits block (role swap; `mov sl,r8`/`mov r8,sl`) |

Everything between `0x149cac` and `0x14a83e` (2962 B) is instruction-identical.
The cumulative byte-delta trace (orig vs ours at each diff block) peaks at
**+40 B** around `0x1e32` (inside the free-credits block) and returns to 0 by
`0x240c`; the free-credits `getWidth`/`getHeight` region alone contributes
+12 B.

## New measurements this wave (all <= 91.199559)

Baseline = 91.199559 / 9700 B / 3635:3626 unless noted.

### Free-credits statement order — exhaustive permutation sweep

Source statements: `L` = `Layout *lay = Globals::layout;`,
`H` = `height = button->getHeight()`,
`X` = `textX = button->getWidth() + baseX + lay->field_0x2c_rowHeight`,
`C` = `SetColor(0xffffffff)`,
`Y` = `volatile float textYf = ...; int textY = (int) textYf;`.
Every valid permutation of `H X C Y` after `L` (H before Y) was measured:

| order (after `L`) | fuzzy |
|---|---:|
| **H X C Y (landed)** | **91.199559** |
| H C X Y | 90.723639 |
| C H X Y | 90.723639 |
| H C Y X | 90.696072 |
| H Y C X | 88.459948 |
| H Y X C | 88.309898 |
| H X Y C | 88.282327 |
| C H Y X | 88.432373 |
| X H C Y | 76.937904 |
| X C H Y | 76.325575 |
| C X H Y | 76.380664 |
| X H Y C | 75.995042 |

Without `L` at all, the same sweep tops out at `H X C Y` = 90.806340 — i.e.
the `lay` local itself is worth +0.39 and every `getWidth`-first (faithful)
order costs >= 13 points.

`lay` position sweep (with `X` using `Globals::layout` when `L` comes later):

| `lay` position | fuzzy |
|---|---:|
| **before `H`, used in `X` (landed)** | **91.199559** |
| between `H` and `X` | 90.806340 |
| between `X` and `C` | 90.806340 |
| between `C` and `Y` | 90.806340 |
| after `Y` | 90.806340 |
| `Layout &lay = *Globals::layout` | 91.199559 |

### Free-credits `textY` / `textYf`

| variant | fuzzy | size |
|---|---:|---:|
| `volatile float textYf; int textY = (int) textYf;` (landed) | **91.199559** | 9700 |
| `float textYf` (non-volatile) + `int textY` | 76.012118 | 9696 |
| `const float textYf` + `int textY` | 76.012118 | 9696 |
| `int textY = (int)((float)y + (float)height*0.2f)` (no `textYf`) | 76.012118 | 9696 |
| `volatile float textYf; textYf = ...; int textY = (int)textYf;` | 91.199559 | 9700 |
| `volatile const float textYf` | 91.199559 | 9700 |
| `int textY = (int)(textYf + 0.0f)` | 88.986784 | 9700 |
| `float ty = textYf; int textY = (int)ty;` | 91.199559 | 9700 |
| `int textY = (int)(float)textYf` | 91.199559 | 9700 |
| `int textY = (int)textYf + 0` | 91.199559 | 9700 |
| `int textY = (int)textYf; volatile int tyv = textY;` | 88.888889 | 9700 |
| `float textYf; volatile int textY = (int)textYf;` | 88.552142 | 9696 |

The `volatile` forces the `vstr s2,[sp,#204]; vldr s18,[sp,#204]` pair that the
original does not have, but removing it reallocates the whole function and costs
15 points.

### Free-credits loop / misc

| variant | fuzzy |
|---|---:|
| `for (int i = 0; i != 5; i++)` | 91.144470 |
| `bool visible` -> `int visible` | 91.144470 |
| `visible = true; ... else if (i == 4 && options[76]) visible = false;` | 91.199559 |
| `baseX = Globals::layout->buttonInsetX + this->dialog->x` | 91.172015 |
| `y` computed before `setVisible` | 90.380375 |
| `row++` after the `if (visible)` body | 72.897454 |
| `PaintCanvas *cv = Globals::Canvas;` used in the block | 69.876204 |
| `Layout *lay` hoisted before the loop / before `baseX` | 90.052356 |
| `Layout *lay = Globals::layout;` at top of `render()` (one use) | 67.970256 |

### Selected-item amount block / `item`

| variant | fuzzy | size |
|---|---:|---:|
| landed (`item->item->...` in both arms) | **91.199559** | 9700 |
| `Item *it = item->item;` then-arm only | 87.291753 | 9704 |
| `Item *it = item->item;` then-arm, `item->item` else-arm | 87.291753 | 9704 |
| `ListItem &it = *item;` then-arm | 86.611570 | — |
| `left`/`right` assignment order swapped | 86.111493 | — |
| `Item *it = item->item;` both arms | 62.351969 | 9680 |
| `Item *it = item->item;` else-arm only | 62.417401 | — |
| `ListItem *volatile item` | 59.689603 | — |
| `ListItem *const item` | 91.199559 | 9700 |
| `ListItem *item;` declared outside the loop, assigned inside | 91.199559 | 9700 |
| `items->data()[i]` instead of `(*items)[i]` | 91.199559 | 9700 |
| `this->selectedItem == item && item->isTextButton() == 0` | 91.172015 | — |
| `item->inTabIndex >= 0 && tab == 0` (operand order) | 82.819383 | — |
| `item->inTabIndex >= 0` instead of `> -1` | 91.199559 | 9700 |
| `item->isSelectable() != 0` | 91.199559 | 9700 |
| `(*this->buttons)[8]->setPosition(hintOffsetX + buttonInsetX, ...)` | 91.144470 | — |
| `(*this->buttons)[9]->setPosition(hintOffsetX + buttonInsetX + listWidth, ...)` | 91.199559 | 9700 |

### `pad3` anchor (`if (left > 0)`)

| variant | fuzzy |
|---|---:|
| `volatile int pad3 = left;` before `setPosition` (landed) | **91.199559** |
| after `setPosition` | 86.863123 |
| after `setVisible` | 87.055907 |
| after `draw` | 86.942149 |
| `volatile long` / `volatile unsigned int` | 91.199559 |
| `volatile short` / `volatile char` | 87.331314 |
| `volatile float` | 87.004405 |
| split declaration + assignment | 91.199559 |
| `(void) pad3;` after | 87.319290 |
| `pad3 = left - 0` | 91.199559 |

### Ship branch

| variant | fuzzy |
|---|---:|
| landed `if (tab != 0) { if (!(tab == 3 && ...)) ... }` + `if (upgradeMode && tab == 1)` | **91.199559** |
| `if (tab != 0 && !(tab == 3 && ...))` (combined) | 91.199559 |
| `if (tab != 3 \|\| ship != getShip())` | 91.199559 |
| empty then-branch `if (tab==3 && ...) {} else {...}` | 91.199559 |
| `switch (tab) { case 1: if (upgradeMode) ... }` | 91.144470 |
| `if (tab == 1 && this->upgradeMode)` | 91.144470 |
| nested `if (upgradeMode) { if (tab == 1) ... }` | 91.199559 |
| `bool up = this->upgradeMode;` | 83.934968 |
| `padX` values `&y` / `&tab` / `&listWidth` / `&textX` / `&iconSpace` | 91.199559 |

### Buy-credits

| variant | fuzzy |
|---|---:|
| landed `row = (unsigned char)count/3; col = (unsigned char)count%3` | **91.199559** |
| `row = count/3; col = count%3` (no casts) | 91.199559 |
| `unsigned char uc = count; row = uc/3; col = uc%3` | 91.199559 |
| `col = (unsigned char)(count - row*3)` | 72.119879 |
| `pad0` before `setPosition` | 89.296046 |
| `x` product operands reversed | 91.199559 |
| `for (count = 0; count != 5; count++)` | 79.481165 |
| `pad0` values `&y` | 91.199559 |
| `volatile short *pad0` | 91.199559 |

Note: clang already turns `count % 3` after `count / 3` into a single
`__aeabi_uidiv` plus `row*3` / `sub` / `uxtb`, so the `col` source form is not
the difference — the buy-credits loop body is instruction-identical modulo the
`r8`/`sl` role swap and a 2-byte offset.

### `pV` anchor (`viewMode == 1`)

| variant | fuzzy |
|---|---:|
| `volatile int pV = 0;` after `viewMode = 0` (landed) | **91.199559** |
| before `viewMode = 0` | 91.199559 |
| after `render()` | 91.116926 |
| after `viewMode = 1` | 91.061837 |
| after `listItemWindow->draw()` | 91.144470 |
| split declaration + assignment | 91.199559 |
| `volatile short` / `volatile char` | 88.983751 |

### Other

| variant | fuzzy |
|---|---:|
| `for (int b = 0; b != 24; b++)` | 91.199559 |
| `unsigned int n = items->size(); for (i = 0; i < n; i++)` | 62.398458 |
| `barHeight` before `barPos` | 91.061837 |
| `listWidth -= ...` moved after `iconSpace` | 58.642484 |
| `switch (tab)` with `default: break;` | 91.199559 |
| `int y = ...` single expression | 91.116926 |
| `int y` before `int baseY` | 91.116926 |
| `if (this->dialogActive == 0) return;` | 91.199559 |
| `if (this->buyCreditsActive != 0)` / `freeCreditsActive != 0` | 91.199559 |
| `if (item->isItem() != 0)` | 91.199559 |
| `if (tab == 1 \|\| tab == 4)` / `(tab == 0 \|\| tab == 3)` | 91.199559 |
| `getCurrentTab() == 4 && ...` before `== 1` | 85.112243 |
| dead `PaintCanvas *cv` / `Layout *lay` / `HangarWindow *self` at top | 91.199559 |
| `volatile` dead `cv` / `lay` at top | 70.549360 / 70.494286 |
| `if (y >= 0 && y <= Globals::h) { ... }` instead of `continue` | 91.199559 |

## Conclusion

The landed source is a strong local optimum. This wave exhaustively covered the
free-credits statement order (all 12 valid permutations, with and without the
`lay` local) and the anchor positions; nothing beat 91.199559. The residual is
not a source-shape error: it is the global callee-saved register assignment
(`&Globals::layout` wants `sl`, `this` wants `r9`; ours is swapped) plus the
`item` rematerialisation and the branch-folder's `cbz` -> `cmp/beq.w` fold on
the ship branch. All three are allocator/CFG heuristics that a local source edit
cannot reach without reallocating the 2962-byte identical middle of the
function.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6   # RATCHET PASS byte 4503 linked 4503
# per-variant fuzzy: rebuild Elf32/Resolver each run, like tools/verify/verify.py
```
