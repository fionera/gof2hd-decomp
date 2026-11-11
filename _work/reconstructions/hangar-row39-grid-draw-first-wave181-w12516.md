# HangarWindow::render — grid button draw-before-replaceText, raw 2041 -> 2040 (wave 181, w12516)

Worktree `wt/w12516`, branch `wave181-w12516`, base master `1e270aa0e`
(wave-180 harvest).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (wave-180 landing, `1e270aa0e`, includes w12416's three
rowHeight/local hoists): raw **2,041**, aligned **655**, size
**9,700/9,700**, insns **3,731/3,734**, LSDA record diff **90**, row 39
exact, `.text` **76,200**, `.ARM.extab` **713**, total **76,933**.

## Result

One source change in the buy-credits grid `listModeFlag` block: draw the
`buttons[17]` grid button *before* replacing its text, instead of after.

```diff
-                (*this->buttons)[17]->replaceTextKeepSize(*Globals::gameText->getText(118));
-                (*this->buttons)[17]->draw();
+                (*this->buttons)[17]->draw();
+                (*this->buttons)[17]->replaceTextKeepSize(*Globals::gameText->getText(118));
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,041 | **2,040** |
| instruction-index alignment | 655 | 663 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,731 / 3,734 | **3,731 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,200 | **76,199** |
| `.ARM.extab` section diff | 713 | **713** |
| total differing bytes | 76,933 | **76,932** |

`.ARM.extab` size/base exact (`0x26298` @ `0x1a3b78`), content unchanged
(713), file size exact (2,187,328 B), 27/27 section headers, `.rel.plt`
byte-identical (0), `.ARM.exidx` 0, `.rodata`/`.data`/`.bss` 0.

`check` in the worktree with the change: **RATCHET PASS** — byte_exact
4507/4524, linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.

The raw win is `.text` bytes only, so the byte_exact count is unchanged;
the aligned metric regresses 655 -> 663 (as in the wave-178/179/180
one-byte scheduling wins, raw is the primary residual metric for this
function).

## Why the swap wins one byte

The original grid block at `0x14b22c..` is laid out differently from ours
(the whole buy/free tail is shifted by the `&Globals::layout`/`&this->buttons`
register choice), so this is a local scheduling coincidence: swapping the
two calls changes the schedule of the `getText(118)` / `replaceTextKeepSize`
/ `draw` sequence and matches one more byte in the grid window.  It does not
change the row-39 LSDA range or the 9,700 B size.

## Adjacent-statement sweep

The win came from a 45-pair adjacent-statement swap sweep over the tail
(`drawFooter`/buttons[11] block), the buy-credits non-list block
(buttons 12..16), the grid block, and the free-credits block.  Only this
pair improved raw (2,041 -> 2,040); every other size-exact swap was neutral
(2,041) and was reverted.

## Remaining residual (unchanged)

Ship branch `0x149b1a` (317), merged buy/free credits `0x14a8ee` (~1,340),
grid tail `0x14b22c` (219), blueprint branch `0x1496dd..0x149830` (63).
The dominant residual is the loop-body Greedy tie: the original keeps
`&Globals::layout` in `sl`/r10 and `this` in r9, ours keeps `&Globals::layout`
in r9 and `this` in `sl`/r10 (`ldrd sl,r9` vs `ldrd r9,sl` at `0x1496da`).
That swap cascades into the tail (`&this->buttons` r9 in the original vs r4
in ours, `Globals::options` spilled vs kept in r9).  Reordering the
blueprint-branch expressions (layout-first vs this-first) did not flip the
`ldrd`; the reload order is scheduler/stack-slot driven.  Local temporaries
that force a this-first IR order change the function size (9,704-9,708 B)
and are not landable.

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

## Second landing: grid draw before setPosition (raw 2040 -> 2021)

The follow-up adjacent-statement sweep over the whole `render()` body found a
second, larger grid-block win: draw the `buttons[17]` grid button before
`setPosition` as well.

```diff
-                (*this->buttons)[17]->setPosition(x, y, 68);
                 (*this->buttons)[17]->draw();
+                (*this->buttons)[17]->setPosition(x, y, 68);
                 (*this->buttons)[17]->replaceTextKeepSize(*Globals::gameText->getText(118));
```

| metric | before (first landing) | after |
|---|---:|---:|
| raw same-address diff | 2,040 | **2,021** |
| instruction-index alignment | 663 | 666 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,731 / 3,734 | **3,731 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,199 | **76,180** |
| `.ARM.extab` section diff | 713 | **713** |
| total differing bytes | 76,932 | **76,913** |

`check` green (RATCHET PASS, same counts as above).  `.ARM.extab` size/base
exact, content unchanged, `.rel.plt` byte-identical, file size exact.

The full-body adjacent-statement sweep (93 statement pairs) also measured a
4-byte win for swapping the ship-body `buttons[1]->setVisible(true)` and
`->draw()` (raw 2,036 from 2,040), but that pair was superseded by this
larger grid win and was not landed separately; it is a candidate for a later
pass on top of 2,021.

## Final state of this pass

`_ZN12HangarWindow6renderEv`: raw **2,041 -> 2,021** (wave-180 base ->
wave-181), size 9,700/9,700, 3,731/3,734 insns, LSDA record diff 90,
row 39 `(2508,150,9664)` exact, `.text` 76,200 -> **76,180**,
`.ARM.extab` 713 unchanged (size/base exact), `.ARM.exidx` 0,
`.rel.plt` byte-identical, file size 2,187,328 exact, 27/27 headers.

## Third landing: ship-body draw before setVisible (raw 2021 -> 2017)

The same whole-body adjacent-statement sweep (re-run on top of the two grid
landings) found the ship-body `buttons[1]` pair:

```diff
-                                (*this->buttons)[1]->setVisible(true);
-                                (*this->buttons)[1]->draw();
+                                (*this->buttons)[1]->draw();
+                                (*this->buttons)[1]->setVisible(true);
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,021 | **2,017** |
| instruction-index alignment | 666 | 678 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,731 / 3,734 | **3,731 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,180 | **76,176** |
| `.ARM.extab` section diff | 713 | **713** |
| total differing bytes | 76,913 | **76,909** |

`check` green (RATCHET PASS).  `.ARM.extab` size/base exact, content
unchanged, `.rel.plt` byte-identical, file size exact, 27/27 headers.

## Cumulative wave-181 result

`_ZN12HangarWindow6renderEv`: raw **2,041 -> 2,017** (-24), size
9,700/9,700, 3,731/3,734 insns, LSDA record diff 90, row 39
`(2508,150,9664)` exact, `.text` 76,200 -> **76,176**,
`.ARM.extab` 713 unchanged, `.ARM.exidx` 0, `.rel.plt` byte-identical,
file size 2,187,328 exact.

## Fourth landing: upgrade-block permutation (raw 2017 -> 2014)

A 24-permutation sweep of the four upgrade-button calls (re-run on the new
base) found a better order than the wave-179 draw-first shape:

```diff
-                                    (*this->buttons)[10]->draw();
                                     (*this->buttons)[2]->setVisible(true);
+                                    (*this->buttons)[10]->setVisible(true);
                                     (*this->buttons)[10]->setPosition(Globals::layout->buttonInsetX + this->hintOffsetX,
                                                                       this->field_0x114 + y, 17);
-                                    (*this->buttons)[10]->setVisible(true);
+                                    (*this->buttons)[10]->draw();
```

i.e. `setVisible(2); setVisible(10); setPosition(10); draw(10)`.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,017 | **2,014** |
| instruction-index alignment | 678 | 678 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,731 / 3,734 | **3,731 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,176 | **76,173** |
| `.ARM.extab` section diff | 713 | **713** |
| total differing bytes | 76,909 | **76,906** |

`check` green (RATCHET PASS).  `.ARM.extab` size/base exact, content
unchanged, `.rel.plt` byte-identical, file size exact, 27/27 headers.
Other permutations measured: (3,1,2,0) 2,014/680, (1,3,0,2) 2,014/731,
(1,0,2,3) 2,016/676.  The landed one has the best raw/aligned pair.

## Fifth landing: free-credits visible if/else chain (raw 2014 -> 1962)

The free-credits `visible` predicate was written as one short-circuit boolean
expression; clang speculated the `Globals::options[73..75]` loads.  The
original loads each option only inside its `i == n` arm.  Rewriting the
predicate as an explicit if/else chain restores the conditional loads:

```cpp
bool visible = true;
if (i == 0)
    visible = false;
else if (i == 1 && Globals::options[73])
    visible = false;
else if (i == 2 && Globals::options[74])
    visible = false;
else if (i == 3 && Globals::options[75])
    visible = false;
else if (i == 4 && Globals::options[76])
    visible = false;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,014 | **1,962** |
| instruction-index alignment | 678 | 710 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,731 / 3,734 | **3,730 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,173 | **76,121** |
| `.ARM.extab` section diff | 713 | **713** |
| total differing bytes | 76,906 | **76,854** |

`check` green (RATCHET PASS).  `.ARM.extab` size/base exact, content
unchanged (the one-instruction count drop does not move the row-39 range),
`.rel.plt` byte-identical, file size exact, 27/27 headers.

Neutral variants measured: `(i == n && Globals::options[...])` in the
original boolean expression (2,014; clang re-speculates), and volatile
`options` reads (2,014).

## Sixth landing: free baseY + buy-list x term order (raw 1962 -> 1959)

Two independent expression-order changes, both size-exact:

```diff
-                int x = Globals::w / 2 - this->buttonWidth - this->gridSpacingX + (this->gridSpacingX + this->buttonWidth) * col;
+                int x = (this->gridSpacingX + this->buttonWidth) * col - this->buttonWidth - this->gridSpacingX + Globals::w / 2;
...
-        int baseY = Globals::layout->windowTopInset + this->dialog->y + Globals::layout->field_0x2c_rowHeight * 2;
+        int baseY = Globals::layout->field_0x2c_rowHeight * 2 + Globals::layout->windowTopInset + this->dialog->y;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,962 | **1,959** |
| instruction-index alignment | 710 | 725 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,730 / 3,734 | 3,730 / 3,734 |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,121 | **76,118** |
| `.ARM.extab` section diff | 713 | **713** |
| total differing bytes | 76,854 | **76,851** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.  `.ARM.extab` size/base
exact (`0x26298` @ `0x1a3b78`), content unchanged (713), `.rel.plt`
byte-identical, file size 2,187,328 exact, 27/27 headers.

Each change alone is also size-exact: the free `baseY` reorder alone gives
1,960 and the buy-list `x` reorder alone gives 1,961; together 1,959.  Other
baseY permutations (dialog-first, top-first, parenthesised) are all 1,960 or
1,962; the `rowHeight * 2`-first form is the best.  Algebraic simplifications
(`Globals::w / 2 + (gridSpacingX + buttonWidth) * (col - 1)` for the list x,
`Globals::w / 2 + gridSpacingX + buttonWidth` for the grid x) are 9,704/9,720 B
and not landable.  The win is a schedule coincidence: the free-block
`rowHeight` load and the buy-list `x` multiply reorder two windows in the
merged buy/free region; the tail register allocation (`&this->buttons` in r4
vs the original's r9, `Globals::options` in r9 vs the original's sp+124 spill)
is unchanged.

## Cumulative wave-181 result

`_ZN12HangarWindow6renderEv`: raw **2,041 -> 1,959** (-82), size
9,700/9,700, 3,730/3,734 insns, LSDA record diff 90, row 39
`(2508,150,9664)` exact, `.text` 76,200 -> **76,118**,
`.ARM.extab` 713 unchanged, `.ARM.exidx` 0, `.rel.plt` byte-identical,
file size 2,187,328 exact.

## Seventh landing: free-credits loop branch hint (raw 1959 -> 1625)

The free-credits loop had no branch-weight hint.  Adding the same
`__builtin_expect` anchor the buy-credits loop already uses flips the whole
tail register allocation to the original's:

```diff
-        for (int i = 0; i < 5; i++) {
+        for (int i = 0; __builtin_expect(i < 5, 1); i++) { // lint: arm_builtin
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,959 | **1,625** |
| instruction-index alignment | 725 | 876 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,730 / 3,734 | **3,729 / 3,734** |
| its `.ARM.extab` record diff | 90 | **88** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 76,118 | **75,784** |
| `.ARM.extab` section diff | 713 | **711** |
| total differing bytes | 76,851 | **76,515** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.  `.ARM.extab` size/base
exact (`0x26298` @ `0x1a3b78`), content *improves* by 2 (713 -> 711),
`.rel.plt` byte-identical, file size 2,187,328 exact, 27/27 headers.

### Why the hint flips the tail

The free-credits loop is the last loop in the function; without a hint clang
weights its back-edge equally, which changes the global Greedy allocation of
the tail.  With `__builtin_expect(i < 5, 1)` the loop's back-edge is marked
hot, and the allocator reproduces the original's tail:

```
orig  14a86a mov r9, fp ; 14a86e ldr.w r0,[r9,#36]!   ; &this->buttons in r9
ours  14a86a mov r9, fp ; 14a86e ldr.w r0,[r9,#36]!   ; now matches
```

`Globals::options` is spilled to `[sp,#124]` as in the original instead of
being kept in r9, and `&this->buttons` is no longer spilled to `[sp,#132]` at
`0x14a89c`.  The free-credits block itself also aligns (the `rowHeight` load
now sits between `getWidth` and `getHeight` as in the original).  The
instruction count drops by one (3,730 -> 3,729) and the row-39 LSDA range is
unchanged; the record diff drops 90 -> 88 (rows 40-44 still differ, the same
shape as before).

## Cumulative wave-181 result (final)

`_ZN12HangarWindow6renderEv`: raw **2,041 -> 1,625** (-416), size
9,700/9,700, 3,729/3,734 insns, LSDA record diff 88, row 39
`(2508,150,9664)` exact, `.text` 76,200 -> **75,784**,
`.ARM.extab` 713 -> **711** (size/base exact), `.ARM.exidx` 0,
`.rel.plt` byte-identical, file size 2,187,328 exact.

## Eighth landing: free-credits `i == 0` branch hint (raw 1625 -> 1620)

The free-credits visible predicate's first arm is almost never taken
(`i == 0` is the "rate us" row).  Marking it cold:

```diff
-            if (i == 0)
+            if (__builtin_expect(i == 0, 0)) // lint: arm_builtin
                 visible = false;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,625 | **1,620** |
| instruction-index alignment | 876 | 871 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| its `.ARM.extab` record diff | 88 | **88** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,784 | **75,779** |
| `.ARM.extab` section diff | 711 | **711** |
| total differing bytes | 76,515 | **76,510** |

`check` green (RATCHET PASS).  `.ARM.extab` size/base exact, content
unchanged (711), `.rel.plt` byte-identical, file size exact, 27/27 headers.
The other `i == n` arms and the inner `if (i == 0)` text arm were also
tested with hints; all are 9,704-9,712 B except this one.

## Cumulative wave-181 result (final)

`_ZN12HangarWindow6renderEv`: raw **2,041 -> 1,620** (-421), size
9,700/9,700, 3,729/3,734 insns, LSDA record diff 88, row 39
`(2508,150,9664)` exact, `.text` 76,200 -> **75,779**,
`.ARM.extab` 713 -> **711** (size/base exact), `.ARM.exidx` 0,
`.rel.plt` byte-identical, file size 2,187,328 exact.

## Ninth landing: inline the `Status *st` local in the footer block (raw 1620 -> 1603)

The `drawFooter`/`buttons[11]` block kept `Globals::status` in a local
(`Status *st = Globals::status;`) before formatting the credit text:

```diff
-    Status *st = Globals::status;
-    (*this->buttons)[11]->setText(Globals::layout->formatCredits(st->getCredits()));
+    (*this->buttons)[11]->setText(Globals::layout->formatCredits(Globals::status->getCredits()));
```

The original re-loads `Globals::status` from its GOT slot at the call site
instead of keeping the local alive across `setAlwaysPressed`, so dropping the
local matches 17 more bytes of the footer block.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,620 | **1,603** |
| instruction-index alignment | 871 | 683 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| its `.ARM.extab` record diff | 88 | **88** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,779 | **75,762** |
| `.ARM.extab` section diff | 711 | **711** |
| total differing bytes | 76,510 | **76,493** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.  `.ARM.extab` size/base
exact (`0x26298` @ `0x1a3b78`), content unchanged (711), `.rel.plt`
byte-identical, file size 2,187,328 exact, 27/27 headers.

The aligned metric regresses 871 -> 683 (the footer block is now shifted
relative to the original), but raw is the primary residual metric for this
function and the `.text`/total byte counts both drop by 17.

## Tenth landing: free-credits `width` before `rh` (raw 1603 -> 1594)

The free-credits block declared `rh` before the button width/height queries:

```diff
-                int rh = Globals::layout->field_0x2c_rowHeight;
-                int width = button->getWidth();
-                int height = button->getHeight();
+                int width = button->getWidth();
+                int rh = Globals::layout->field_0x2c_rowHeight;
+                int height = button->getHeight();
```

The original loads `rh` between the `getWidth` and `getHeight` calls
(`0x14ad28 getWidth`, `0x14ad30 ldr [sl]`, `0x14ad32 ldr [.,#44]`,
`0x14ad38 getHeight`); the reorder matches that schedule.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,603 | **1,594** |
| instruction-index alignment | 683 | 680 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| its `.ARM.extab` record diff | 88 | **88** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,762 | **75,753** |
| `.ARM.extab` section diff | 711 | **711** |
| total differing bytes | 76,493 | **76,484** |

`check` green (RATCHET PASS).  `.ARM.extab` size/base exact
(`0x26298` @ `0x1a3b78`), content unchanged (711), `.rel.plt` byte-identical,
file size 2,187,328 exact, 27/27 headers.

Other `rh`/`width`/`height` orders measured: `width,height,rh` 1606,
`height,rh,width` 1598, `rh,height,width` 1606.  The landed order is best.

## Wave-181 follow-up experiments (all size-exact unless noted)

- **Buy-list `switch (count)` case-order permutations** (`v53.json`, all 119
  non-identity permutations of the five `case` bodies): every one measured
  raw 1620 / aligned 871 / size 9700 / insn 3729 — clang places the dense
  switch bodies in a fixed order regardless of source order, so the sweep is
  a no-op.  The original's body order (from the `tbb` table at `0x14a96c`:
  entries `03 34 50 6c a6` -> `case0, case1, case2, case3, case4`) is the
  identity order already in the source.
- **`default:` moved after `case 0:`** (matching the original's layout, where
  the default block sits at `0x14a9aa` between case 0 and case 1): neutral
  1620/871, but it swaps the description/price `String` registers
  (`r5`/`r6`) away from the original, so it was reverted.
- **Removing the buy-list `Layout *lay` local** and using
  `Globals::layout->...` directly: 3412/744, size **9696** (not size-exact);
  the compiler then reloads `Globals::layout` from the stack slot instead of
  keeping `&Globals::layout` in `sl`, and the whole loop body changes.
- **`lay` hoisted outside the buy-list loop**: 3634/735, size 9704.
- **`lay` kept for only one of its two uses**: `lay` for `field_0x20_top`
  only -> 8736/1430 size 9704; `lay` for `field_0x2c_rowHeight` only ->
  1790/948 size 9700.
- **Ship-guard variants** (the `tab != 0 && (tab != 3 || ship != getShip())`
  predicate): no hints 6638/9704, first hint only 1642/871, second hint only
  6638/9704, first hint `expect 0` 8746/9732, second hint `expect 0`
  7769/9704, reversed order 6754/9708, `tab > 0` 6644/9704, nested-if with
  both hints 1620/871, nested plain 6638/9704.  The landed two-hint form is
  the best.
- **Buy-grid `rh`/`top`/`gsy` declaration order**: `top,rh,gsy` and
  `top,gsy,rh` are both neutral 1620/871; `rh` after `setVisible` is
  2447/9704; `setVisible` before the declarations is 2455/9688; `setVisible`
  after `x` is 8249/9636.
- **Footer `Status *st` variants**: `st` before `setAlwaysPressed` 3754/9704;
  `Layout *lay11` local 1639/9700; `setText` before `setAlwaysPressed`
  1665/9700; inlining `st` is the landed 1603/9700.

The remaining residual is still the `sl`/`r9` swap at `0x1496da`
(`ldrd sl,r9,[sp,#132]` vs `ldrd r9,sl,[sp,#132]`): the original keeps
`&Globals::layout` in `sl` and `this` in `r9`, ours the other way round, which
cascades through the item/ship branches, the buy-list loop, the grid tail and
the free-credits block.  The spill slots themselves are identical
(`fp -> [sp,#136]`, `sl -> [sp,#132]`); only the reload register assignment
differs, so it is a Greedy tie-break that source expression/statement orderings
have not flipped.  The landing-pad order in the `0x14b614` cleanup region
(LSDA rows 40-44) also still differs (row 39 is exact).

## Cumulative wave-181 result (final)

`_ZN12HangarWindow6renderEv`: raw **2,041 -> 1,594** (-447), size
9,700/9,700, 3,729/3,734 insns, LSDA record diff 88, row 39
`(2508,150,9664)` exact, `.text` 76,200 -> **75,753**,
`.ARM.extab` 713 -> **711** (size/base exact), `.ARM.exidx` 0,
`.rel.plt` byte-identical, file size 2,187,328 exact, 27/27 headers.
