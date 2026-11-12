# HangarWindow::render — buy-list price-before-description order (wave 186, w13017)

Worktree `wt/w13017`, branch `wave186-w13017`, base master `2d02567f6`
(wave-185 notes commit).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (wave-185 w12817 body): raw **1,512**, aligned **723**, size
**9,700/9,700**, insns **3,730/3,734**, `.text` **74,402**, `.ARM.extab`
**647**, build-id **20**, total **75,069**, file size 2,187,328.

## Result

One source change in the `buyCreditsActive && listModeFlag` buy-credit
switch: assign `price` before `description` in all six arms (five real
cases + `default`).

```diff
-                        description = String(Globals::cItemListDescription_00);
                         price = String(Globals::cItemListPrice_00);
+                        description = String(Globals::cItemListDescription_00);
                         break;
...
                     default:
-                        description = String("");
                         price = String("");
+                        description = String("");
                         break;
```

The two `String` assignments are independent (neither reads the other),
so the reorder is semantically neutral.  It changes the order in which
the two temporary `String` objects are destroyed/assigned and therefore
the instruction schedule inside each switch arm; the aligned
instruction-stream diff drops from 723 to 643.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,512 | **1,512** |
| instruction-index alignment | 723 | **643** |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,730 / 3,734 | **3,730 / 3,734** |
| its `.ARM.extab` record diff | 81 | **81** |
| row 39 (orig / ours) | (2508,150,9664) / (2508,152,9664) | **(2508,150,9664) / (2508,152,9664)** |
| `.text` section diff | 74,402 | **74,402** |
| `.ARM.extab` section diff | 647 | **647** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 75,069 | **75,069** |
| file size | 2,187,328 | **2,187,328** |

This is an **aligned-only** win: the raw same-address byte count, the
section byte counts and the total are unchanged; the instruction stream
matches 80 more instructions after branch-target normalisation.  It is
size-exact and does not grow `.ARM.extab`.

## Gate

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type
0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  `.ARM.extab` size/base exact (`0x1a3b78`, 156,312 B),
`.rel.plt` byte-identical, 27/27 section headers, file size exact.

## Residual after this change

The residual is still register allocation in the free-credits and
buy-list blocks (raw 1,512).  Function-region same-address diffs:
blueprint 59, ship 308, footer 16, buy-list 569, free-credits 183,
non-list 0, buttons-17 201, tail 52.

Root cause of the dominant buy-list run: ours materialises
`Globals::layout` into `sl` at the top of each loop iteration
(`ldr.w sl,[sl]` at `0x14a960`) and restores `&Globals::layout` from
`[sp,#128]` at the loop tail (`0x14ac64`).  The original keeps
`&Globals::layout` in `sl` across the whole function and dereferences it
at the two use sites (`ldr.w r1,[sl]` at `0x14ab02`, `ldr.w r2,[sl]` at
`0x14abc2`).  Both forms are 8 bytes of dereference/restore; ours is
placed at the loop head/tail, so the whole switch arm run is shifted by
4 bytes relative to the original.  Other residuals: the `sl`/`r9`
Greedy tie at `0x1496da` (`ldrd sl,r9` vs `ldrd r9,sl`), the
`String description`/`String price` declaration register swap
(orig `r5`=description/`r6`=price, ours `r6`=description/`r5`=price),
and the ship branch keeping the non-ship `switch` inline (orig has it
out of line at `0x149bf0`).

## Other forms measured on the same base (all reverted)

The sweep (roughly 150 variants) found no raw improvement below 1,512.
Selected results (raw / aligned / size):

| form | raw | aligned | size |
|---|---:|---:|---:|
| **price-before-description, all 6 arms (landed)** | **1,512** | **643** | **9,700** |
| price-before-description, 5 non-default arms | 1,512 | 645 | 9,700 |
| `String price` declared before `description` | 1,514 | 735 | 9,700 |
| `int row/col = count / 3` (no `unsigned char` cast) | 1,512 | 643 | 9,700 |
| `Layout &lay = *Globals::layout` | 1,512 | 643 | 9,700 |
| `Layout *const lay` / `const Layout *lay` | 1,512 | 643 | 9,700 |
| `lay` before the `String` declarations | 1,542 | 643 | 9,700 |
| `lay` moved after the switch / before `int row` | 3,559-3,563 | 681-686 | 9,692 |
| lazy `lay` assignment before `int row`/`x`/`y` | 3,563 | 681 | 9,692 |
| no `lay` (direct `Globals::layout` at both uses) | 3,532 | 657 | 9,696 |
| `lay` outside the buy-list `if` | 2,524 | 804 | 9,700 |
| `textY` before `SetColor` + swap | 1,549 | 621 | 9,700 |
| expanded buy `x` form | 1,514 | 641 | 9,700 |
| `rh` local + expanded `x` | 1,515 | 591 | 9,700 |
| `rh` local + `w/2 - ...` form | 1,516 | 583 | 9,700 |
| `lay` removed entirely | 3,532 | 657 | 9,696 |
| free-credits statement permutations | 1,512-1,552 | 656-736 | 9,700 |
| `canvas` / `textY` forms | 1,512-8,744 | 708-1,412 | 9,700/9,704 |
| blueprint `bp` local | 8,286 | 2,204 | 9,624 |
| `isShip` expect forms | 1,512 | 723 | 9,700 |

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target diff -j2
cmake --build cmake-build-match --target drift -j2
```
