# HangarWindow::render — free-credits textY before SetColor, raw 1594 -> 1558 (wave 182, w12617)

Worktree `wt/w12617`, branch `wave182-w12617`, base master `57e085059`
(wave-181 harvest 5).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (wave-181 landing, `57e085059`, includes w12516's ten landings):
raw **1,594**, aligned **680**, size **9,700/9,700**, insns
**3,729/3,734**, LSDA record diff **88**, row 39 exact, `.text` **75,365**,
`.ARM.extab` **655**, total **76,040**.

## Result

One source change in the `freeCreditsActive` row loop: compute `textY`
before the first `canvas->SetColor(0xffffffffu)` call instead of after it.

```diff
                 PaintCanvas *volatile canvas = Globals::Canvas;
-                canvas->SetColor(0xffffffffu);
                 volatile int textY = (int) ((float) y + (float) height * 0.2f);
+                canvas->SetColor(0xffffffffu);
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,594 | **1,558** |
| instruction-index alignment | 680 | 702 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| its `.ARM.extab` record diff | 88 | **80** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,365 | **75,329** |
| `.ARM.extab` section diff | 655 | **647** |
| total differing bytes | 76,040 | **75,996** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.ARM.extab` size/base exact (`0x26298` @ `0x1a3b78`), content *improves*
by 8 (655 -> 647), `.rel.plt` byte-identical (0), `.ARM.exidx` 0,
`.rodata`/`.data`/`.bss` 0, file size 2,187,328 exact, 27/27 section
headers.

## Why the swap wins 36 bytes

The original free-credits body computes `textY` (the `y + height*0.2f`
float sequence) *before* the `SetColor(0xffffffff)` call:

```
14ad28 getWidth
14ad30 ldr  [layout,#0x2c]     ; rowHeight
14ad38 getHeight
14ad46 SetColor(0xffffffff)
...
14ad74 vmov s0,height ; vcvt ; vmla (textY)   ; textY after SetColor
```

Wait — the original actually schedules the textY float math after the
SetColor call (the `vmov/vcvt/vmla` sequence at `0x14ad50..0x14ad6a`).
The source-level swap makes clang's scheduler emit the textY *value*
computation in the same window as the original, which realigns the whole
merged free-credits block (`0x14ad24..0x14ae62`).  The swap is a pure
scheduling coincidence: `textY` has no dependency on `SetColor`, so both
orders are semantically identical.

The residual is still the `sl`/`r9` role swap for the saved
`&Globals::layout` / `this` pair (`ldrd sl,r9` vs `ldrd r9,sl` at
`0x1496da`), which cascades through the ship branch, the buy-list loop and
the free-credits block.  This change does not address that tie.

## Adjacent-statement sweep

A 16-variant sweep over the free-credits block (statement swaps, the
`visible` if/else arm order, and `textX` operand orders) measured:

| variant | raw | size |
|---|---:|---:|
| base | 1,594 | 9,700 |
| **textY before SetColor (landed)** | **1,558** | **9,700** |
| `width`/`rh` swap | 1,603 | 9,700 |
| `rh`/`height` swap | 1,606 | 9,700 |
| `draw` before `if (visible)` | 1,605 | 9,700 |
| `visible` chain 1<->2 | 1,598 | 9,700 |
| `visible` chain reversed | 1,602 | 9,700 |
| `textX` operand orders (3 forms) | 1,594 | 9,700 |
| `textX`/`canvas` swap | 1,594 | 9,700 |
| `setVisible` before `y` | 3,382 | 9,720 |
| `setPosition` before `y` | (miss) | — |
| `setPosition`/`draw` swap | 3,391 | 9,720 |
| `SetColor(0x777777)` before `if` | 3,666 | 9,724 |

Also measured: non-volatile `canvas` local 1,596 / 9,700; non-volatile
`textY` 3,419 / 9,716; `Globals::Canvas->` with no local 3,636 / 9,720.

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

## Second landing: buy-list draw before setSplitText (raw 1558 -> 1546)

An adjacent-statement sweep over the buy-credits grid loop found one more
size-exact win: draw the grid button before setting its split text.

```diff
-                (*this->buttons)[12 + count]->setSplitText(price);
                 (*this->buttons)[12 + count]->draw();
+                (*this->buttons)[12 + count]->setSplitText(price);
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,558 | **1,546** |
| instruction-index alignment | 702 | 702 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| its `.ARM.extab` record diff | 80 | **80** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,329 | **75,317** |
| `.ARM.extab` section diff | 647 | **647** |
| total differing bytes | 75,996 | **75,984** |

`check` green (RATCHET PASS, same counts).  `.ARM.extab` size/base exact,
content unchanged (647), `.rel.plt` byte-identical, file size exact,
27/27 section headers.

Other buy-list adjacent swaps measured (all size-exact): `description`/
`price` 1,558 (aligned 656), `price` after `setVisible` 1,583,
`lay` before `setVisible` 1,573, `row`/`col` 1,558, `x`/`y` 1,558,
`replace`/`split` 1,564, `draw`/`DrawImage2D` 1,581.  `setPosition` after
`replaceTextKeepSize` is 9,636 B (not size-exact).

## Cumulative wave-182 result (so far)

`_ZN12HangarWindow6renderEv`: raw **1,594 -> 1,546** (-48), size
9,700/9,700, 3,729/3,734 insns, LSDA record diff **88 -> 80**, row 39
`(2508,150,9664)` exact, `.text` 75,365 -> **75,317**,
`.ARM.extab` 655 -> **647** (size/base exact), `.rel.plt` byte-identical,
file size 2,187,328 exact, 27/27 headers.

## Third landing: free-credits baseY dialog-first (raw 1546 -> 1539)

The free-credits `baseY` expression was laid out rowHeight-first; the
original computes the dialog y first:

```diff
-        int baseY = Globals::layout->field_0x2c_rowHeight * 2 + Globals::layout->windowTopInset + this->dialog->y;
+        int baseY = this->dialog->y + Globals::layout->windowTopInset + Globals::layout->field_0x2c_rowHeight * 2;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,546 | **1,539** |
| instruction-index alignment | 702 | 693 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| its `.ARM.extab` record diff | 80 | **80** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,317 | **75,310** |
| `.ARM.extab` section diff | 647 | **647** |
| total differing bytes | 75,984 | **75,977** |

`check` green (RATCHET PASS).  `.ARM.extab` size/base exact, content
unchanged (647), `.rel.plt` byte-identical, file size exact, 27/27 headers.

Other free-credits orders measured: `baseY` top-first 1,546,
`baseX`/`baseY` swap 1,546, `baseX` layout-first 1,546, `y` row-first
1,546, `textY + field_0x4` swapped 1,547, `i == 0 ? 3400 : i + 112`
ternary 1,546, `textY` after `canvas` 1,548.

## Cumulative wave-182 result (so far)

`_ZN12HangarWindow6renderEv`: raw **1,594 -> 1,539** (-55), size
9,700/9,700, 3,729/3,734 insns, LSDA record diff **88 -> 80**, row 39
`(2508,150,9664)` exact, `.text` 75,365 -> **75,310**,
`.ARM.extab` 655 -> **647** (size/base exact), `.rel.plt` byte-identical,
file size 2,187,328 exact, 27/27 headers.

## Fourth landing: buy-list draw before replaceTextKeepSize (raw 1539 -> 1523)

A whole-body adjacent-statement sweep (89 single-line pairs) found one more
size-exact win in the buy-credits grid loop: draw before
`replaceTextKeepSize`.

```diff
-                (*this->buttons)[12 + count]->replaceTextKeepSize(description);
                 (*this->buttons)[12 + count]->draw();
+                (*this->buttons)[12 + count]->replaceTextKeepSize(description);
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,539 | **1,523** |
| instruction-index alignment | 693 | 681 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| its `.ARM.extab` record diff | 80 | **80** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,310 | **75,294** |
| `.ARM.extab` section diff | 647 | **647** |
| total differing bytes | 75,977 | **75,961** |

`check` green (RATCHET PASS).  `.ARM.extab` size/base exact, content
unchanged (647), `.rel.plt` byte-identical, file size exact, 27/27 headers.

The landed buy-list grid order is now
`setPosition; draw; replaceTextKeepSize; setSplitText`.
The other 88 adjacent pairs were neutral (1,539) or size-changing.

## Cumulative wave-182 result (so far)

`_ZN12HangarWindow6renderEv`: raw **1,594 -> 1,523** (-71), size
9,700/9,700, 3,729/3,734 insns, LSDA record diff **88 -> 80**, row 39
`(2508,150,9664)` exact, `.text` 75,365 -> **75,294**,
`.ARM.extab` 655 -> **647** (size/base exact), `.rel.plt` byte-identical,
file size 2,187,328 exact, 27/27 headers.
