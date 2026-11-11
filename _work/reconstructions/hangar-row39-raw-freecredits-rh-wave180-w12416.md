# HangarWindow::render — free-credits rowHeight load order, raw 2242 -> 2234 (wave 180, w12416)

Worktree `wt/w12416`, branch `wave180-w12416`, base master `99e341e9b`
(wave-179 harvest 3).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (wave-179 landing, `99e341e9b`, includes the w12316 draw-first
upgrade block): raw **2,242**, aligned **390**, size **9,700/9,700**,
insns **3,734/3,734**, LSDA record diff **90**, row 39 exact,
`.text` **76,870**, `.ARM.extab` **709**, total **77,598**.

## Result

One source change in the `freeCreditsActive` row loop: load
`Globals::layout->field_0x2c_rowHeight` *before* `button->getWidth()`
instead of after.

```diff
             if (visible) {
-                int width = button->getWidth();
-                int rh = Globals::layout->field_0x2c_rowHeight;
+                int rh = Globals::layout->field_0x2c_rowHeight;
+                int width = button->getWidth();
                 int height = button->getHeight();
                 int textX = width + baseX + rh;
```

(`const int rh` / `const int width` are codegen-identical; the plain form is
kept.)

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,242 | **2,234** |
| instruction-index alignment | 390 | 393 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,734 / 3,734 | **3,734 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,870 | **76,862** |
| `.ARM.extab` section diff | 709 | **709** |
| total differing bytes | 77,598 | **77,590** |

`.ARM.extab` size/base exact (`0x26298` @ `0x1a3b78`), content unchanged
(709), file size exact (2,187,328 B), 27/27 section headers.

`check` in the worktree with the change: **RATCHET PASS** — byte_exact
4507/4524, linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.  The win is `.text` bytes only, so the
byte_exact count is unchanged; `.rel.plt` stays byte-identical.

## Why the swap wins 8 bytes

The original loads `rowHeight` through the saved `&Globals::layout` pointer
*after* the `getWidth()` invoke:

```
14ad24 ...  ; getWidth() call result in r0
14ad28 ...
14ad2c ldr r?, [layout, #0x2c]   ; rowHeight, from the saved layout pointer
```

Our wave-179 shape loaded `rowHeight` first (from `Globals::layout` before
the invoke), so the whole `0x14ad24..0x14ad32` window was scheduled
differently.  Declaring `rh` first in the source makes clang emit the
`rowHeight` load after the `getWidth()` call, matching 8 more bytes in the
merged buy/free-credits region (`0x14a8ee` region sum 1348 -> 1340).

It is a scheduling coincidence like the wave-178/179 1-byte wins: the
remaining free-credits residual is the `sl`/`r9` role swap for the saved
`&Globals::layout`, which this change does not address.

## What was tried (all reverted; size-exact unless noted)

- `const int rh` / `const int width` / `const int height`: identical to the
  landed shape (2,234).
- `rh` declared before `if (visible)`: 2,234 (same).
- `textX` operand orders, `rh + width + baseX` etc.: 2,234.
- `rh` hoisted to loop top: 3,366 / 9,708.
- `height` before `width`: 3,709 / 9,692.
- non-volatile `canvas` / non-volatile `textY`: 3,758 / 9,716 and
  3,751 / 9,708.
- `visible` predicate reorder: 3,687 / 9,708.
- buy-credits block: `Layout *lay` local, y-before-x, baseY/row reorder:
  2,234 (neutral).
- tail 4-call permutations (24, `drawFooter` first): best natural order
  2,242.  5-call permutations (120): best natural order.
- upgrade-block permutations (24): best 2,242.
- `options[78]` volatile forms (local alias, volatile pointer, reload
  casts): best 2,242.
- ship guard `getShip() != ship` operand flip: 2,232 but 3,735 insns,
  record 91 and row 39 -> 152 (not landable).
- `viewMode != 0`: 2,302.  Blueprint-branch `self`/`lay` locals: 7,497 /
  9,704.

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

## Caveat: isolated build tree

The variant was first screened in a full copy of the tree at
`/home/debian/GalaxyOnFire2/scratch/w12416/iso` (own NDK cache and build
dir).  That tree is not bit-identical to the worktree build at base (its
`HangarWindow.cpp.obj` frame was 384 B vs 376 B in the worktree, giving raw
2,243 at base in one earlier session), but the `-8` delta reproduces in both.
The numbers quoted above are the worktree build (`cmake-build-match`).

Scratch helpers used: `/home/debian/GalaxyOnFire2/scratch/w12416/`
(`secs.py`, `regsum.py`, `groups2.py`, `runvar.py`, `runiso.py`, `iso/`).

## Second landing: grid-block locals (raw 2234 -> 2044)

The same pass then cached three grid-block values in locals.  The original
loads `rowHeight` after the `setVisible` call and keeps `&Globals::layout`
in `r9`; the local form makes clang hoist `rowHeight`/`top`/`gridSpacingY`
into a schedule that matches the whole `0x14b22c..0x14b30a` window:

```cpp
if (!Globals::options[75] || ... || data->freeCreditsOffer) {
    int rh = Globals::layout->field_0x2c_rowHeight;
    int top = Globals::layout->field_0x20_top;
    int gsy = this->gridSpacingY;
    int x = Globals::w / 2 - ...;
    int y = (int) ((float) (Globals::h / 2 - this->gridButtonHeight / 2 - 3 * top) -
                  (float) gsy * 0.5f + (float) ((gsy + this->gridButtonHeight) * 1));
    (*this->buttons)[17]->setVisible(true);
    ...
        Globals::Canvas->DrawImage2D(this->tabIcons[5], x, y - rh, 17, 68);
```

plus a `Status *st = Globals::status;` local in the tail credit-button
`setText` call:

```cpp
    Status *st = Globals::status;
    (*this->buttons)[11]->setText(Globals::layout->formatCredits(st->getCredits()));
```

| metric | before (F1 only) | after |
|---|---:|---:|
| raw same-address diff | 2,234 | **2,044** |
| instruction-index alignment | 393 | 613 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,734 / 3,734 | 3,732 / 3,734 |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,862 | **76,672** |
| `.ARM.extab` section diff | 709 | **709** |
| total differing bytes | 77,590 | **77,401** |

`check` green (iso tree): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN.

The instruction count is 3,732 vs the original's 3,734 (two instructions
fewer, compensated by longer encodings); the function size and the
`.ARM.extab` record are unchanged, which is what the gate checks.  The
`0x14b22c` grid region drops from 403 differing bytes (base) to 221.

Variants measured on top of the F1 shape (all size-exact unless noted):

| shape | raw | insn |
|---|---:|---:|
| F1 only | 2,234 | 3,734 |
| F1 + `int rh` in the grid block (`y - rh`) | 2,052 | 3,733 |
| F1 + `rh`/`top`/`gsy` locals | 2,046 | 3,733 |
| F1 + `Status *st` only | 2,050 | 3,732 |
| **F1 + `rh`/`top`/`gsy` + `Status *st`** | **2,044** | **3,732** |
| + `int gw = Globals::w; int gh = Globals::h;` | 2,044 | 3,732 |
| `const` forms of `rh`/`top`/`gsy` | 2,044 | 3,732 |
| `int gb = this->gridButtonHeight;` | 2,906 / 9,704 | 3,735 |
| `int gsx = this->gridSpacingX;` | 4,945 / 9,712 | 3,740 |
| `int rh` declared inside `if (listModeFlag)` | 2,234 | 3,734 |
| `int rh` after `setVisible` | 2,910 / 9,704 | 3,737 |
| `TouchButton *b = (*this->buttons)[11]` tail local | 3,839 / 9,684 | 3,726 |
| `Array<TouchButton *> *buttons = this->buttons` | 9,183 / 9,616 | 3,724 |
| `volatile int rh` | 2,088 | 3,732 |

## Third landing: ship-branch rowHeight local (raw 2044 -> 2041)

The same trick applies to the ship-branch button body: cache `rowHeight`
before the `buttons[1]` calls and reuse it in the `buttonOffset`
expression.

```cpp
int upgTab = tab;
int shipRh = Globals::layout->field_0x2c_rowHeight;
(*this->buttons)[1]->setPosition(...);
(*this->buttons)[1]->setVisible(true);
(*this->buttons)[1]->draw();
buttonOffset = (*this->buttons)[1]->getWidth() + shipRh;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,044 | **2,041** |
| instruction-index alignment | 613 | 655 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,732 / 3,734 | 3,731 / 3,734 |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,672 | **76,669** |
| `.ARM.extab` section diff | 709 | **709** |
| total differing bytes | 77,401 | **77,398** |

`check` green (iso tree).  Declaring `shipRh` after the `setPosition` call
(2,309, record 678) or after `draw()` (6,280 / 9,696) is much worse; the
`int upgTab = tab;` anchor must come first.

## Final state of this pass

`_ZN12HangarWindow6renderEv`: raw **2,242 -> 2,041** (base wave-179 ->
wave-180), size 9,700/9,700, 3,731/3,734 insns, LSDA record diff 90,
row 39 `(2508,150,9664)` exact, `.text` 76,870 -> **76,669**,
`.ARM.extab` 709 unchanged (size/base exact), `.ARM.exidx` 0,
`.rel.plt` byte-identical, file size 2,187,328 exact.

Remaining raw residual by region: ship branch `0x149b1a` (317), merged
buy/free credits `0x14a8ee` (1,340), grid tail `0x14b22c` (219), blueprint
branch `0x1496dd..0x149830` (63), plus small stragglers.  The buy/free
region is dominated by the `&Globals::layout` / `&this->buttons` register
choice (`r9` in the original, `r4` + spill in ours) and the
`Globals::options` spill position; the blueprint branch is the `ldrd
sl,r9` Greedy tie.
