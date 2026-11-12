# HangarWindow::render — wave 191b (w13543): wave-190 ship body order, raw 1,191 -> 1,038

Worktree `wt/w13543`, branch `wave191b-w13543`, base master `23a6ebf8b`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result

Restore the wave-190 ship-body order (`setVisible`, `upgTab` snapshot,
`draw`, `setPosition`, `buttonOffset`) while keeping the w13527
body-scope `upgTab` and the `buttons[10]`-before-`buttons[2]` upgrade
order:

```diff
                             if (tab == 0) { } else if (__builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1)) { // lint: arm_builtin
-                                int upgTab = tab;
-                                (*this->buttons)[1]->setPosition(...);
                                 (*this->buttons)[1]->setVisible(true);
+                                int upgTab = tab;
                                 (*this->buttons)[1]->draw();
+                                (*this->buttons)[1]->setPosition(...);
                                 buttonOffset = ...;
```

| metric | before (w13527) | after |
|---|---:|---:|
| raw same-address diff | 1,191 | **1,038** |
| instruction-index alignment | 600 | 620 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,726 / 3,734 | 3,726 / 3,734 |
| `.text` section diff | 71,487 | **71,334** |
| `.ARM.extab` section diff | 586 | **577** |
| `.rodata` size delta | 485 | 485 |
| `.note.gnu.build-id` | 20 | 20 |
| total (text+extab+build-id) | 72,093 | **71,931** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0),
`.ARM.exidx` byte-identical (0), all section sizes and the file size
exact, 27/27 section headers.

## Why it works

The w13522 body order (`setPosition`, `setVisible`, `draw`) only pays
off when the `tab` snapshot is hoisted **above** the ship guard: that
shape keeps `this`/`layout` in callee-saved registers across the
`getShip` call and lands the body at the original `0x149b44`
(raw 928), but it gives `getShip` its own cleanup scope and grows the
render LSDA record to 677 differing bytes (`.ARM.extab` 1,216).

With the w13527 body-scope `upgTab` (row 39 = `(2508,150,9664)`,
extab 586) the guard is `cmp`+`beq.w` instead of `cbz`, so the body
starts at `0x149b48` (+4).  In that shape the `setPosition`-first order
costs 153 raw / 153 `.text` bytes relative to the wave-190
`setVisible`/`draw`/`setPosition` order.  The upgrade order
(`buttons[10]` first) is unchanged, so the function stays 9,700 B.

The render LSDA row 39 becomes `(2508, 172, 9664)` instead of the
original `(2508, 150, 9664)`; both encode as two uleb bytes, so the
call-site table stays aligned, and the render record's total differing
bytes fall (extab 586 -> 577).

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
