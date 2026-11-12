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

## Additional negative results (same wave, after the landed commit)

All measured on the landed base (`raw 1038`, `size 9,700`, `.text` 71,334,
`.ARM.extab` 577, render LSDA record diff 38).  Every variant below is
size-exact 9,700 and extab-record 38 unless stated.

Free-credits `textY` / `textX` / declaration shapes (all neutral at 1,038
or worse): `volatile int textY` 1,040; `PaintCanvas *canvas` before
`textY` 1,040; `textX` after `textY` 1,038; `height` after `textX`
1,038; `const volatile int &` 1,038; `volatile int *` 1,038; `textX`
re-associated `width+rh+baseX` / `baseX+width+rh` / `baseX+rh+width`
1,038; `const int rh` / `const int width` 1,038; alias moved after
`SetColor` 1,038 (byte-identical); `*(volatile int *)&textY` inline
1,038.  Removing the volatile read: plain `int` 9,744 (not size-exact);
`volatile int textYv = textY` 9,716; plain copy 9,744; `textY` computed
after `SetColor` + alias after 9,692.

Free-credits button call order (all size-exact, extab 38): `DVP`
(current, draw/setVisible/setPosition) **1,038**; `VDP` 1,044;
`VPD` 1,072 (`.text` 71,368); `PDV` 1,077; `PVD` 1,078.

Ship guard / upgrade-check shapes: current merged guard + body-scope
`upgTab` **1,038**; w13527 `setPosition`-first body 1,191; two-if
(upgrade check outside the guard) `V_2` 9,700 but raw 2,563 / extab 695;
`P_2` 9,676; nested `if (tab != 0)` 9,680-9,704; hoisted `upgTab` +
two-if 928 / extab record 677 but `.text` 71,280 / `.ARM.extab` 1,216;
hoisted `upgTab` + merged guard (`setPosition`-first) raw 928 / extab
record 677, `.text` 71,224 / `.ARM.extab` 1,216, total 72,462
(+529 vs 71,933) -- a net **loss** once the section extab is counted
(the earlier handoff's "net -13" compared the record diff, not the
section diff).

Blueprint `isBluePrint` body: hoisting `this->hintOffsetX` 9,696;
hoisting `Globals::layout->buttonInsetX` raw 1,406 / extab 796;
hoisting `(int)(rate*progressBarWidth)` 9,708; hoisting
`progressBarBgImage` 9,704; `(float)progressBarWidth * rate` 1,038
(byte-identical).

Buy-list loop: moving `String name;` / `int buttonOffset;` out of the
`for` body 9,672 (raw 9,171).

Residual is unchanged: the `sl`/`r9` role swap in the blueprint run
(0x0602), the buy-list `String`-temp/`sl` tie (`.text` 2-byte phase
shift at `add.w sl,sp,#300`), the free-credits `width`/`rh` register
roles plus the volatile `textY` store, and the cleanup landing-pad
block order.  No shape tried this wave beats 1,038 while keeping the
render LSDA record at 38 differing bytes.
