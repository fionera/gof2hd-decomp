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
