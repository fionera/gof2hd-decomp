# MenuTouchWindow C1/C2 wave 109 (w5719): hoist the language-loop y base/pitch
# (95.6% -> 95.7%, 9600 B)

Date: 2026-11-04. Worktree `wt/w5719`, branch `wave109-w5719`, base
`7c34c4098`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before:** 9600 B, 3320 instructions, exact difflib ratio
  **95.6325%**, **173** raw-diff lines (this worker's rawdiff metric:
  changed lines from `difflib` after `verify.reconcile`).
* **After:** 9600 B, 3320 instructions, exact difflib ratio
  **95.6928%**, **169** raw-diff lines.
* Source change (language-selection loop, `src/game/ui/MenuTouchWindow.cpp`):
  hoist the loop-invariant y origin and row pitch into locals, and
  compute `y` before `x`:

```cpp
+            int yBase = Globals::h / 2 - this->buttonYBias;
+            int rowPitch = rowHeight + Globals::layout->field_0x30_rowHeight;
+            int y = (yBase - 12 - Globals::layout->field_0x29c_buttonRowGap) + rowPitch * (i >> 1);
             int x = (int) ((float) ((i & 1) ? Globals::w - Globals::w / 4 : Globals::w / 4) +
                            (float) this->buttonWidth * -0.5f);
-            int y = (Globals::h / 2 - this->buttonYBias - 12 - Globals::layout->field_0x29c_buttonRowGap) +
-                    (rowHeight + Globals::layout->field_0x30_rowHeight) * (i >> 1);
             if (Globals::iPad) {
                 x = Globals::w / 2 - this->buttonWidth / 2;
-                y = (Globals::h / 2 - this->buttonYBias - Globals::layout->field_0x29c_buttonRowGap -
-                     Globals::layout->field_0x30_rowHeight) +
-                    (rowHeight + Globals::layout->field_0x30_rowHeight) * i;
+                y = (yBase - Globals::layout->field_0x29c_buttonRowGap -
+                     Globals::layout->field_0x30_rowHeight) + rowPitch * i;
             }
```

* Gate before and after: `byte_exact 4500 / linked_exact 4500`,
  `size 4524/4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS**. Fuzzy-only change, so no baseline update is needed
  and none was made. No section/extab change (`.so` still +44 B,
  `.ARM.extab` still 0x26294).

## 1. Why this works (mechanism, measured)

The original iPad-branch y expression is

```
123416: ldr  r5, [r0, #0]        ; fontValue = Globals::font
123418: ldr  r0, [r1, #0]        ; langValue = Globals::fontLangSelect
...
1234aa: cmp  r8, #10
...
(subtraction block)
  subs r1, r6, r2                ; r2 = buttonRowGap (low reg, narrow)
  add.w r2, r5, r5, lsr #31
  subs r1, r1, r3                ; r3 = field_0x30 (low reg, narrow)
```

Before this pass our build allocated the two loop-invariant layout fields
the other way round:

```
  sub.w r1, r6, lr               ; lr = buttonRowGap (high reg, wide)
  add.w r2, r5, r5, lsr #31
  sub.w r1, r1, r2               ; r2 = field_0x30 (low reg)
```

i.e. 4-byte `sub.w` where the original has 2-byte `subs`, and the
subtraction operands were reversed.  Writing the y origin (`yBase`) and
the row pitch (`rowPitch`) as locals before `x`, and evaluating `y`
before `x`, makes clang keep `field_0x30` in `r2` and `buttonRowGap` in
`lr`, so the first subtraction becomes the narrow `subs r1, r6, r2`
(matching the original byte-for-byte) and the second stays `sub.w
r1, r1, lr`.  The code in this block is 2 bytes shorter than before,
which shifts the following literal-pool slots and removes 4 diff lines
overall.  The function size is unchanged (9600 B) because a later block
absorbs the 2 bytes.

The same source shape with `y` after `x`, or with only one of the two
locals, is neutral (see table): the allocator only flips when both
hoists and the `y`-before-`x` order are present.

## 2. Measured table (official verifier, full TU)

`orig` is always 9600 B / 3320 instructions.  `fz` is the exact difflib
ratio from `tools/verify/verify.py`'s normalised stream.  Scratch base =
`/dev/shm/w5719/base_pristine.cpp` (pristine HEAD); the winning base with
this pass applied is `/dev/shm/w5719/base_win1.cpp`.

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| **HEAD (wave 108)** | **9600** | **3320** | **95.6325** | **173** |
| **yBase + rowPitch + y-before-x (this pass)** | **9600** | **3320** | **95.6928** | **169** |
| y-before-x only | 9600 | 3320 | 95.6325 | 173 |
| yBase only | 9600 | 3320 | 95.6325 | 173 |
| rowPitch only | 9600 | 3320 | 90.9830 | 366 |
| yBase + rowPitch, y after x | 9600 | 3320 | 74.6699 | 1076 |
| yBase + rowPitch, declare y then if/else | 9600 | 3320 | 72.2931 | 1121 |
| swap yBase/rowPitch declaration order | 9600 | 3320 | 90.9830 | 366 |
| quarterW local (neutral) | 9600 | 3320 | 95.6928 | 169 |
| otherW / halfW / minus-half / float-div (neutral) | 9600 | 3320 | 95.6928 | 169 |
| hoist yBase+rowPitch out of the loop | 9600 | 3320 | 71.5359 | 1171 |

## 3. Negative results re-measured this wave (all on top of the pass)

These are the remaining RA/scheduling fixed points from waves 107-108,
re-measured with the new base.  None beat 95.6928%.

### 3.1 Mission block `adds r4,r5,#2` hoist

Original hoists `adds r4, r5, #2` (row+2) **above** `getText(395)` and
keeps it in callee-saved `r4`; ours sinks it below the `String` ctor
into caller-saved `r3`.  Every spelling below is **neutral at
95.6928% / 169**: `int row2 = row + 2;` before the `if`, inside the
`if`, `const int row2`, `(row + 1) + 1`, `row + 2u`, `2 + row`,
`(row + 2)`, `((int) row) + 2`.

### 3.2 Language-loop `lang`/`i` r8<->r9 swap

Original: `lang=r8`, `i=r9` (with a `mov r9,r8` copy and an `i` reload
into `r8` for the second array index).  Ours: `lang=r9`, `i=r8`.  About
20 raw-diff lines.  Still no source lever.  All of the following are
**neutral at 95.6928% / 169**: separate loop counter `k` with
`unsigned int i = k;`, `int k`, `idx` for the first/second/both array
indices, and swapping the `fontValue`/`langValue` declaration order.

### 3.3 Language-loop bound / `special` spelling

`i < 10`, `i <= 9`, `i < 10u` all fold the value-select PHI and drop to
**72.06% / 1123** (9548 B / 3330 insns).  `(int) i < 10` is 95.5120 /
175.  `i != 10` is 95.5422 / 174.  With `i != 10`, `special = i == 8 ||
i == 9` is neutral (95.6928 / 169); `(i & 0xe) == 8` is 95.5723 / 172;
`i >= 8`, `i > 7`, `(unsigned)(i - 8) < 2` are worse.  The
`__builtin_assume(i < 100)` is load-bearing.

### 3.4 iPad `sliders[2]` y-tree association

The original groups the sum as
`0x2fc + ((0xc + 0x20) + 0x154 + 3*0x2c) + 0x164 + 2*(0x15c + 0x1c + 0x2c)`.
Writing exactly that regrouping reproduces the original tree locally
(and is 4 bytes longer, matching the original block size) but perturbs
the surrounding RA:

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| faithful regrouping | 9600 | 3320 | 95.2711 | 178 |
| `field_0x2fc` first | 9600 | 3320 | 95.5422 | 169 |
| `rowHeight*4 + rowHeight` | 9600 | 3320 | 95.6627 | 169 |
| `(rowHeight+rowHeight)*2 + rowHeight` | 9600 | 3320 | 95.6627 | 169 |
| `rowHeight*3 + rowHeight*2` | 9600 | 3320 | 95.5723 | 175 |

### 3.5 `scrollExtraButton` y-tree association

Original accumulates `field_0x2fc + (field_0x15c + rowHeight) +
sliderRowHeight`; ours accumulates `field_0x2fc + (sliderRowHeight +
rowHeight) + field_0x15c`.  Reorderings (`field_0x2fc + 0x15c +
rowHeight + sliderRowHeight`, `(0x15c + rowHeight + sliderRowHeight)`,
`0x15c + rowHeight + 0x2fc + sliderRowHeight`, `0x2fc + 0x15c +
(rowHeight + sliderRowHeight)`, `0x2fc + (rowHeight + sliderRowHeight)
+ 0x15c`) are all **neutral at 95.6928 / 169**; the `0x15c`-first
variants drop to 95.6325 / 173.  A local `extraBase`/`fcPlus` is much
worse (91.6% / 90.3%).

### 3.6 Cinematic buttons

Removing the `cinematicText` temp and calling `Globals::gameText`
directly is 93.54% / 243.  `GameText *cinematicText =
Globals::gameText;` is 95.6627 / 171.  Swapping the `0x300`/`0x308`
addition order in the B expression is 95.6627 / 170; in the A
expression 95.6325 / 171.

## 4. Proven vs hypothesis

**Proven (measured):**
* The `yBase`/`rowPitch`/`y`-before-`x` source shape is a real
  verifier improvement: 95.6325 -> 95.6928, 173 -> 169 raw-diff lines,
  with identical size (9600 B) and instruction count (3320).
* The improvement is the narrow `subs r1,r6,r2` in the language-loop y
  expression; the second subtraction remains `sub.w r1,r1,lr` (one
  diff line).
* The mission-block ADD hoist, the r8/r9 swap, the loop-tail order, the
  `sliders[2]` tree and the `scrollExtraButton` tree are not reachable
  from source without a size/instruction-count change (tables above).

**Hypothesis (not proven):**
* The original's `bcc` backedge came from a plain `i < 10` in a source
  shape where clang did not have (or did not use) the `[0,9]` range
  fact for the `special` select.  The `__builtin_assume` in our
  reconstruction is what blocks that inference today; the mask is a
  source-level way to keep the general codegen while matching the
  backedge alignment.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5719
TMPDIR=/dev/shm/w5719/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5719/tmp cmake --build cmake-build-match --target check -j6
TMPDIR=/dev/shm/w5719/tmp cmake --build cmake-build-match --target drift -j6
# per-function score:
python3 /dev/shm/w5719/score.sh _ZN15MenuTouchWindowC1Ei
```

Scratch (not tracked): `/dev/shm/w5719/` (`base_pristine.cpp`,
`base_win1.cpp`, `sweep.py`, `mkvariants*.py`, `normdiff.py`,
`sbs2.py`, `classify2.py`, `diffop.py`).
