# HangarWindow::render — wave 113 (w6111): free-credits `Layout` alias

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6111`, branch `wave113-w6111`, base master
`a979ace18`.

Builds on `_work/reconstructions/hangarwindow-render-wave112-w6010.md`
(89.057332). This pass: **89.057332 -> 89.341779**.

## Result

| metric | before (HEAD a979ace18) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 89.057332 | **89.341779** |
| `render` fuzzy (verify, 0.1) | 89.1 | **89.3** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626 / 3630 | 3626 / 3636 |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` | same |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` | same |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4500 / 4500 | 4500 / 4500 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. `.rodata` remains `0x41c27`
(known post-mission-table delta); everything before `0x20b977` is unchanged.

## Landed diff (1 hunk, inside the free-credits loop)

```diff
             if (visible) {
+                Layout *lay = Globals::layout;
                 int height = button->getHeight();
-                int textX = button->getWidth() + baseX + Globals::layout->field_0x2c_rowHeight;
+                int textX = button->getWidth() + baseX + lay->field_0x2c_rowHeight;
```

A plain local alias for the global, used once. It is not a semantic change;
it steers `-Oz` register allocation in the free-credits loop, which is the
dominant remaining diff cluster. The diff-block count drops from 297 to 282,
and the free-credits `if (i != 0)` / `else` block placement improves (the
biggest single diff block shrinks from 27-vs-1 to 21-vs-1).

The alias is robust to spelling: `const Layout *`, `Layout *const`,
`Layout &` and `volatile Layout *` all measure the same 89.341779. Moving it
one statement later (after `button->draw()`, still before `if (visible)`)
also measures the same; moving it after `int height` costs ~2.9 points
(86.470102, 9696 B).

## Measurements (base = pristine 89.057332 unless noted)

All rows 9700 B / 3626:3630 unless noted.

### The winning family (free-credits alias)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| **`Layout *lay` in `if (visible)`, used for textX (LANDED)** | **89.341779** | 9700 | 3626:3636 |
| `const Layout *lay` / `Layout *const` / `Layout &lay` | 89.341779 | 9700 | 3626:3636 |
| `volatile Layout *lay` | 89.341779 | 9700 | 3626:3636 |
| alias after `button->draw()` (outside `if (visible)`) | 89.341779 | 9700 | 3626:3636 |
| alias after `button->setVisible()` | 89.161273 | 9700 | 3626:3635 |
| alias at top of free-credits block | 88.193966 | 9700 | 3626:3633 |
| alias before the `for` loop | 88.193966 | 9700 | 3626:3633 |
| alias after `int height` | 86.470102 | 9696 | 3626:3632 |
| alias before `TouchButton *button` | 84.987627 | 9712 | 3626:3648 |
| alias unused | 89.057332 | 9700 | 3626:3630 |
| alias + `int width` local | 89.341779 | 9700 | 3626:3636 |
| alias + `int rowHeight` local | 89.045060 | 9700 | 3626:3631 |
| alias + textX operand reorders (3 forms) | 89.341779 | 9700 | 3626:3636 |
| alias + `PaintCanvas *cv` alias | 74.559956 | 9704 | 3626:3646 |
| alias + `GameText *gt` alias | 75.488308 | 9716 | 3626:3644 |
| alias used for `Globals::layout->field_0x4` (both) | 74.559471 | 9712 | 3626:3638 |
| alias used for `field_0x4` (else only) | 76.387168 | 9700 | 3626:3637 |
| alias used for `field_0x4` (reward only) | 85.918986 | 9708 | 3626:3632 |

### Other free-credits shapes on top of the alias

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `if (i == 0)` empty branch first | 87.384531 | 9684 | 3626:3627 |
| `row++` before the `if` | 85.340674 | 9676 | 3626:3639 |
| `row++` before `SetColor` | 85.340674 | 9676 | 3626:3639 |
| `if (i)` | 89.341779 | 9700 | 3626:3636 |
| `String()` empty branch | 87.174542 | 9692 | 3626:3633 |
| `String("")` via `String((const char*)0)` | 87.095441 | 9696 | 3626:3635 |
| named `String empty("")` in else | 74.487691 | 9716 | 3626:3645 |
| named `String empty("")` before the `if` | 73.592715 | 9652 | 3626:3622 |
| named `String reward` in the reward branch | 72.752306 | 9680 | 3626:3637 |
| `visible != 0`, `i != 5`, `row += 1`, `button const` | 89.341779 | 9700 | 3626:3636 |
| `textYf` before `textX` | 86.560992 | 9696 | 3626:3629 |
| `textYf` after `textY` | 87.040353 | 9696 | 3626:3635 |
| `SetColor` after `textYf` | 86.533425 | 9696 | 3626:3629 |
| faithful `W X H` order (width, textX, height) | 76.596916 | 9700 | 3626:3638 |
| `W H X` | 76.139336 | 9696 | 3626:3637 |
| `H W X` | 89.341779 | 9700 | 3626:3636 |
| textX before height (no width local) | 76.596916 | 9700 | 3626:3638 |
| `textYf` non-volatile | 75.285695 | 9696 | 3626:3637 |

### Amount block (selected item, `int right, left`)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `ListItem *it = item` used in true branch | 89.057332 | 9700 | 3626:3630 |
| `ListItem *it` used in both branches | 89.057332 | 9700 | 3626:3630 |
| `Item *pi = item->item` cached, true branch | 86.414990 | 9704 | 3626:3632 |
| `Item *pi` second call only | 83.751724 | 9680 | 3626:3624 |
| `this->selectedItem->item` second call | 89.057332 | 9700 | 3626:3630 |
| `this->selectedItem->item` first call | 86.470102 | 9704 | 3626:3632 |
| `if (tab != 1)` else-if | 89.057332 | 9700 | 3626:3630 |
| `int right = 0, left = 0` | 89.057332 | 9700 | 3626:3630 |
| right/left at loop scope | 83.997243 | 9688 | 3626:3629 |
| (on top of the landed alias) `ListItem *it` | 84.987627 | 9712 | 3626:3648 |
| (on top of the landed alias) right/left init | 84.987627 | 9712 | 3626:3648 |

The amount block is insensitive on the pristine base: the original's
`ldr r4,[sp,#120]` / `ldr r6,[sp,#136]` / reload-`item->item` shape cannot be
coaxed out of any of these source forms. On top of the landed alias the
block becomes *worse*, so the alias and the amount block interact; the alias
was kept and the amount block left as-is.

### Buy-credits / other aliases (all worse or neutral)

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `Layout *blay` for `y` only | 84.805070 | 9720 | 3626:3633 |
| `Layout *blay` for `DrawImage2D` only | 86.533425 | 9704 | 3626:3629 |
| `Layout *blay` for both | 85.785192 | 9708 | 3626:3627 |
| `Layout *nlay` in non-listMode block | 89.341779 | 9700 | 3626:3636 |
| `Layout *dlay` for `dialog->setHeight` | 89.341779 | 9700 | 3626:3636 |
| `Layout *flay` for free `baseX` | 89.341779 | 9700 | 3626:3636 |
| `Layout *flay` for free `baseY` | 89.341779 | 9700 | 3626:3636 |
| `Layout *flay` for both free baseX/baseY | 89.341779 | 9700 | 3626:3636 |
| `Layout *flay` in the free-offer (button 17) block | 78.882159 | 9664 | 3626:3638 |
| `Layout *slay` in `if (item->isSelectable())` | 81.664141 | 9688 | 3626:3633 |
| `Layout *hlay` in the ship branch | 82.889839 | 9692 | 3626:3627 |
| `Layout *lay0` for `drawBG` only | 89.341779 | 9700 | 3626:3636 |
| `Layout *lay0` for `drawBG` + `drawHeader` | 60.873158 | 9684 | 3626:3635 |
| `Layout *blay` in the blueprint block | 85.619835 | 9704 | 3626:3634 |
| `Layout *alay` in the amount block | 85.435022 | 9704 | 3626:3638 |
| `PaintCanvas *cv` at function top | 89.341779 | 9700 | 3626:3636 |
| `GameText *gt` at function top | 89.341779 | 9700 | 3626:3636 |

### Anchors on top of the landed alias

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `pad3 = y` | 89.314239 | 9700 | 3626:3636 |
| `pad3 = 0` | 86.729075 | 9704 | 3626:3638 |
| `pad3 = right` | 86.438111 | 9704 | 3626:3637 |
| `pad3` after `setPosition` | 85.997522 | 9708 | 3626:3637 |
| `pad3` after `draw` | 86.076298 | 9704 | 3626:3635 |
| remove `pad3` | 86.997245 | 9696 | 3626:3634 |
| `padX = &y` / `pad0 = &y` / `pV` before `viewMode = 0` | 89.341779 | 9700 | 3626:3636 |
| remove `padX` | 86.197881 | 9700 | 3626:3641 |
| `b = 3 + (inTabIndex < 0)` | 85.545317 | 9708 | 3626:3645 |
| `b = inTabIndex < 0 ? 4 : 3` | 85.545317 | 9708 | 3626:3645 |

## What is proven

1. **A scoped local alias for `Globals::layout` inside the free-credits
   `if (visible)` block is worth +0.284 fuzzy** (89.057332 -> 89.341779) at
   constant 9700 B. It is a real C++ change (no anchor, no volatile sink, no
   dead store) and is robust to `const`/reference/`volatile` spelling and to
   being hoisted just above `if (visible)`.
2. The gain is a register-allocation / constant-pool-placement effect in the
   free-credits loop: the biggest diff block shrinks from 27-vs-1 to 21-vs-1
   instructions and total diff blocks drop 297 -> 282. The instruction count
   rises to 3636 (the alias costs a few 2-byte moves) while the body stays
   9700 B.
3. The amount block (`int right, left` + `getStationAmount`/`getAmount`) is
   insensitive to every source form tried on the pristine base; the original
   keeps `item` in `r4` across both calls and reloads `item->item`, which the
   allocator will not reproduce. On top of the landed alias the block gets
   worse, so it was left untouched.
4. The faithful free-credits order (`width`, `textX`, `height`) still costs
   ~12 fuzzy points, with or without the alias.
5. The full gate stays green: `check` RATCHET PASS, byte 4500 / linked 4500 /
   size 4524 / data 843 / imports 207, lint CLEAN, drift 0. No section size,
   base or count changed; `.ARM.extab` stays `0x26298`.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS

# raw fuzzy (same normalisation as the gate, finer than the 0.1 rounding)
python3 /dev/shm/w6111/score.py _ZN12HangarWindow6renderEv
```

Scratch helpers (`b.sh`, `score.py`, `batch.py`, `batch2.py`, `map.py`,
`addr.py`, `idx.py`, variant lists `v31.json`-`v45.json`) live under
`/dev/shm/w6111/`; they are not part of the commit.
