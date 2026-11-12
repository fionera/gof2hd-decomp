# HangarWindow::render — ship-guard operand flip, raw 1,514 -> 1,512 (wave 184, w12817)

Worktree `wt/w12817`, branch `wave184-w12817`, base master `4c06c2662`
(wave-183 harvest 6).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (wave-183 w12717 body): raw **1,514**, aligned **680**, size
**9,700/9,700**, insns **3,729/3,734**, LSDA record diff **80**, row 39
exact `(2508,150,9664)`, `.text` **74,842**, `.ARM.extab` **646**, total
**75,508**.

## Result

One source change in the ship-guard compound condition: flip the comparison
operands so the status getter is the left operand.

```diff
-    __builtin_expect(tab != 3 || this->selectedItem->ship != Globals::status->getShip(), 1))
+    __builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1))
```

The two forms are semantically identical (`!=` is symmetric; the getter is
side-effect-free).  The flip changes which value clang's register allocator
keeps live across the `getShip()` call and how the guard's `cmp` is formed,
shaving two bytes from the free-credits/ship-guard branch-offset run.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,514 | **1,512** |
| instruction-index alignment | 680 | **723** |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | **3,730 / 3,734** |
| its `.ARM.extab` record diff | 80 | **81** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,152,9664)** |
| `.text` section diff | 74,842 | **74,840** |
| `.ARM.extab` section diff | 646 | **647** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 75,508 | **75,507** |
| file size | 2,187,328 | **2,187,328** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.ARM.extab` size/base exact (`0x1a3b78`, 156,312 B), `.rel.plt`
byte-identical, 27/27 section headers, file size exact.

The row-39 LSDA range grows 150 -> 152 (record diff 80 -> 81), so the
`.ARM.extab` content grows by one byte.  That is a net win: `.text` drops
two bytes and total differing bytes drop one (75,508 -> 75,507).  The
section stays size- and base-exact.

## Residual after the flip

Function-region same-address diffs (raw 1,512 total): blueprint 59, ship
308, footer 16, buy-list 569, free-credits 183, non-list 0, buttons-17 201,
tail 52.  The dominant residual is still register allocation in the
free-credits and buy-list blocks: the `sl`/`r9` Greedy tie at `0x1496da`
(`ldrd sl,r9` vs `ldrd r9,sl`), the buy-list `lay` materialization
(`ldr.w sl,[sl]` at `0x14a960` and the `ldr.w sl,[sp,#128]` restore at
`0x14ac64`), and the free-credits `width`/`rh` role swap (orig width->sl,
rh->r9; ours the reverse).  The ship branch keeps the non-ship `switch`
inline (orig has it out of line at `0x149bf0`) because the row-39 LSDA
requires the two-hint compound guard.

## Other forms measured on the 1,514 base

All reverted (not size-exact or not better); raw / size where noted:

| form | raw | aligned | size |
|---|---:|---:|---:|
| **status-getter left operand (landed)** | **1,512** | **723** | **9,700** |
| `3 != tab` | 1,512 | 723 | 9,700 |
| `0 != tab` | 1,512 | 723 | 9,700 |
| `1 == upgTab` | 1,512 | 723 | 9,700 |
| nested flip | 1,512 | 723 | 9,700 |
| `vconst` / `texty_const` / `texty_unsigned` | 1,512 | 723 | 9,700 |
| no cast / `uchar` count / `count < 5` | 1,512 | 723 | 9,700 |
| nested flip, plain inner | 1,534 | — | 9,700 |
| nested expect guard | 1,514 | — | 9,700 |
| swap `String` declarations | 1,516 | — | 9,700 |
| expanded buy `x` | 1,516 | — | 9,700 |
| `rh` first | 1,523 | — | 9,700 |
| `lay` moved before setVisible | 1,529 | — | 9,700 |
| draw before split | 1,530 | — | 9,700 |
| buttons17 setpos/repl/draw | 1,534 | — | 9,700 |
| buttons17 setpos/draw/repl | 1,533 | — | 9,700 |
| draw-last in buy loop | 1,542 | — | 9,700 |
| move `lay` before strings | 1,544 | — | 9,700 |
| textY after declaration | 1,548 | — | 9,700 |
| textY after SetColor (volatile) | 1,550 | — | 9,700 |
| `upgradeMode && ...` | 1,582 | — | 9,700 |
| `u1 = global` | 1,852 | — | 9,700 |
| `lay` outside loop | 1,956 | — | 9,700 |
| no `lay` | 3,537 | — | 9,696 |
| `lay` reference | 3,537 | — | 9,696 |
| after switch | 3,561 | — | 9,692 |
| plain canvas | 3,608 | — | 9,708 |
| no canvas | 3,621 | — | 9,720 |
| free-credits no volatile textY | 3,377 | — | 9,716 |
| nested plain guard | 6,634 | — | 9,704 |
| swap `&&` operands (compound reversed) | 6,754 | — | 9,708 |
| self ref | 7,619 | — | 9,704 |
| local `lay` | 7,653 | — | 9,704 |
| repl/setpos/draw | 8,226 | — | 9,632 |
| row-only | 8,704 | — | 9,724 |
| both plain | 8,746 | — | 9,704 |
| `u2 = global` | 8,724 | — | 9,704 |
| draw first | 9,188 | — | 9,640 |
| free-credits local `lay` pointer | 8,949 | — | 9,676 |
| const free-credits `lay` | 8,949 | — | 9,676 |

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
