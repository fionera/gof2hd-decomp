# HangarWindow::render — wave 190 (w13434): switch-dispatch `bic` and the 2-byte row-40 gap

Worktree `wt/w13434`, branch `wave190-w13434`, base master `be282d6e9`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result

**No size-exact raw win landed this wave.** `src/game/ui/HangarWindow.cpp` is
left at the wave-190 base (the w13379 + w13416 body plus the
`setVisible(this->upgradeMode)` / `(signed char) count` harvest in `2ca5121db`).
This note records one new codegen fact (the switch-dispatch `bic`) and the
negative sweep, so the next wave does not repeat it.

Entry state (re-measured on this branch):

| metric | value |
|---|---:|
| raw same-address diff | 1,336 |
| instruction-index alignment | 680 |
| function size | 9,700 / 9,700 |
| instructions | 3,729 / 3,734 |
| `.text` section diff | 72,884 |
| `.ARM.extab` section diff | 601 |
| render's `.ARM.extab` record diff | 56 (948 B / 156 rows) |
| `.note.gnu.build-id` | 20 |
| total differing bytes | 73,505 |

Gate after this (note-only) pass: `check` RATCHET PASS — byte_exact
4507/4524, linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.

## New evidence: the original's buy-list switch dispatch is `bic`-based

The original's `switch` on `count` at `0x14a960` (function offset `0x1888`) is

```
1888  bic.w  r0, r9, #0x80000000     ; 29 f0 00 40
188c  cmp    r0, #4                  ; 04 28
188e  bhi.n  14a9aa
1890  tbb    [pc, r0]
```

i.e. the range check is `(unsigned)(count & 0x7fffffff) > 4`, the shape clang
emits for a **signed** switch value whose non-negativity it cannot prove.
The base (`switch ((signed char) count)`) instead emits

```
1888  ldr.w  sl, [sl]                ; da f8 00 a0   (the `lay` value form)
188c  cmp.w  r9, #4                  ; b9 f1 04 0f
1890  bhi.n  14aa1c
1892  tbb    [pc, r9]
```

Both are 10 bytes, which is why the base is size-exact: the `ldr.w sl,[sl]`
of the `Layout *lay` value form exactly replaces the `bic`.

Writing the switch as a plain `switch (count)` **does** recover the original
`bic` (verified byte-for-byte: `29 f0 00 40 / 04 28 / 58 d8 / df e8`), but it
then carries *both* the `bic` and the `ldr.w sl,[sl]`, and the extra 4 bytes are
recovered elsewhere — raw rises to 1,353. `(short) count` is byte-identical to
`(signed char) count`; `(unsigned char)`, `(char)`, `(count & 0xff)` all give
9,696 B.

Removing the `lay` value form (`Globals::layout->...` at both use sites) does
reproduce the address form, but the loop preheader then takes `sl` for the
second `String("")` temporary (`add.w sl, sp, #300`, 4 B, vs the original's
`add r5, sp, #300`, 2 B). Combining `nolay` with the plain switch (which buys
the `bic` back) still lands at **9,696 B** — 4 bytes short of size-exact.

## Negative sweep (all size 9,700 B unless stated)

| variant | raw | `.text` | `.ARM.extab` | total |
|---|---:|---:|---:|---:|
| **base** | **1,336** | **72,884** | **601** | **73,505** |
| `switch (count)` (plain, recovers `bic`) | 1,353 | 72,901 | 602 | 73,523 |
| `switch ((short) count)` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| `switch ((unsigned int) count)` / `(count & 0x7fffffff)` | 1,353 | 72,901 | 602 | 73,523 |
| `switch ((unsigned char) count)` / `(char)` / `(count & 0xff)` | 3,342 | 118,657 | 963 | 9,696 B |
| `count` declared `signed char` | 1,899 | 378,737 | 1,400 | 9,704 B |
| `count` declared `unsigned char` | 3,283 | 74,831 | 958 | 75,809 |
| `nolay` (direct `Globals::layout`) | 3,037 | 332,214 | 736 | 9,692 B |
| `nolay + switch (count)` | 3,376 | 118,691 | 747 | **9,696 B** |
| `nolay + switch (short)` | 3,037 | 332,214 | 736 | 9,692 B |
| `lay` after the switch | 3,188 | 332,273 | 740 | 9,688 B |
| `lay` before `x` / before `y` | 3,188 | 332,273 | 740 | 9,688 B |
| `Layout *const lay` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| `Layout *volatile lay` + plain switch | 1,825 | 398,590 | 19,942 | 73,505 (tie) |
| `lay` used only in `y` | 8,007 | 79,555 | 782 | 73,505 (tie) |
| `lay` used only in `DrawImage2D` | 1,732 | 73,280 | 608 | 73,908 |
| `lay` unused (both direct) | 3,037 | 332,214 | 736 | 9,692 B |
| plain-switch + `lay` used only in `DrawImage2D` | 1,695 | 73,243 | 609 | 73,872 |
| plain-switch + `lay` used only in `y` | 8,674 | 385,512 | 1,580 | 9,704 B |
| `row`/`col` without `(unsigned char)` cast | 1,336 | 72,884 | 601 | 73,505 (tie) |
| `3 * top` -> `top * 3`; `(gsy + gh) * row` -> `row * (...)`; `0.5f * gsy` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| `y - rh` -> `-rh + y` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| buy-list `String price` declared before `description` | 1,338 | 72,886 | 601 | 73,507 |
| buy-list `DrawImage2D` moved before `setSplitText` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| buy-list `DrawImage2D` before `replaceTextKeepSize`/`setPosition`/`draw` | 1,366 / 1,392 / 1,383 | — | 601 | worse |
| buy-list `draw` last (`p,r,s,d`) | 1,364 | 72,912 | 601 | 73,533 |
| buy-list `replaceTextKeepSize` after `setSplitText` | 1,344 | 72,894 | 601 | 73,515 |
| `count <= 4` -> `count < 5` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| `count != 5` | 8,267 | 391,692 | 1,752 | 9,640 B |
| `i < 5` -> `i <= 4` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| `i != 5` | 1,338 | 72,886 | 601 | 73,507 |
| `options[..]` chain reversed / ascending / offer-first | 1,352 / 1,340 / 1,352 | — | 601 | worse |
| `options[..] == 0` form | 1,336 | 72,884 | 601 | 73,505 (tie) |
| free-credits `textX` operand reorder (`rh+baseX+width`, `baseX+width+rh`) | 1,336 | 72,884 | 601 | 73,505 (tie) |
| free-credits `textX` moved after `textY` / before `width` / before `height` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| free-credits `canvas` declared before `textX` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| free-credits `canvas` after `textYv` | 1,338 | 72,886 | 601 | 73,507 |
| free-credits `rh` before `width` | 1,345 | 72,893 | 601 | 73,514 |
| free-credits `height` first | 1,458 | 73,006 | 609 | 73,635 |
| free-credits `textY` after `SetColor` | 1,439 | 72,987 | 609 | 73,616 |
| free-credits `volatile int textY` (no `textYv`) | 1,338 | 72,886 | 601 | 73,507 |
| free-credits no `textYv` at all | 3,203 | 383,043 | 1,450 | 9,716 B |
| free-credits `PaintCanvas *canvas` (non-volatile) | 3,480 | 380,347 | 1,441 | 9,708 B |
| free-credits `PaintCanvas *volatile &canvas` / `*const &` / direct `Globals::Canvas` | 3,492 | 383,821 | 1,462 | 9,720 B |
| free-credits `visible` as `int` 1/0 | 1,336 | 72,884 | 601 | 73,505 (tie) |
| free-credits `visible` chain reversed | 3,211 | 380,078 | 1,432 | 9,708 B |
| free-credits `expect(i==0,0)` -> plain `i==0` | 1,342 | 72,890 | 601 | 73,511 |
| free-credits `expect(i==0,1)` | 3,231 | 380,098 | 1,437 | 9,708 B |
| buttons[17] `gsy` declared first | 1,336 | 72,884 | **19** | **73,504** (build-id noise) |
| buttons[17] `top`/`rh`/`gsy` reorder | 1,336 | 72,884 | 601 | 73,505 (tie) |
| buttons[17] `y` without `* 1` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| buttons[17] `x` operand swap | 1,336 | 72,884 | 601 | 73,505 (tie) |
| buy-list `DrawImage2D` `17u,68u` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| buy-list `DrawImage2D` `68,17` | 3,298 | 118,613 | 745 | 9,696 B |
| `showBestDeal && count == 4` swap | 1,336 | 72,884 | 601 | 73,505 (tie) |
| ship guard `tab > 0` / `tab >= 1` | 1,342 | 72,890 | **598** | 73,508 |
| ship guard `(unsigned) tab != 0` | 1,336 | 72,884 | 601 | **73,504** (build-id noise) |
| ship guard `tab` truthy / `!(getShip() == ship)` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| ship guard `expect(...)` on the whole condition | 7,505 | 390,825 | 1,565 | 9,652 B |
| ship guard no `expect` | 4,280 | 381,118 | 1,438 | 9,704 B |
| ship guard compare operands swapped | 1,341 | 72,889 | 601 | 73,510 |
| ship guard sibling form (`int upgTab` before guard, two sibling `if`s) | 6,394 | 121,709 | 1,253 | **9,696 B** |
| sibling + plain switch | 4,593 | 76,141 | 1,222 | 9,700 B |
| sibling + `lay` used only in `y` | 8,000 | 79,548 | 1,275 | 9,700 B |
| upgrade check `upgradeMode && upgTab == 1` | 1,340 | 72,888 | **598** | 73,506 |
| upgrade check plain (no `expect`) | 1,380 | 72,928 | 604 | 73,552 |
| upgrade check `expect(...,0)` | 1,461 | 73,009 | 611 | 73,640 |
| ship body `18u` / `17u` | 1,336 | 72,884 | 601 | 73,505 (tie) |
| ship body `rowHeight + getWidth()` swap | 1,485 | 73,033 | 605 | 73,658 |
| blueprint `x` operand reorder (4 forms) | 1,336 | 72,884 | 601 | 73,505 (tie) |
| blueprint `y` operand reorder (3 forms) | 1,336 | 72,884 | 601 | 73,505 (tie) |
| blueprint `x` hint-first (`textX + 2 + hintOffsetX + insetX`) | 7,609 | 384,447 | 1,513 | 9,704 B |

Ship-body order permutations re-measured on this base (24 orders, `upgTab`
snapshot after the first statement): only `V,D,P,B` (base), `V,S,P,D,B`
(1,334 raw / extab **1,216**), `D,S,V,P,B` (1,337), `D,S,P,V,B`
(1,326 raw / extab **1,216**) are size-exact; every order with `B` not last
is 9,704-9,712 B. The faithful `P,V,D,B` order is 9,704 B.

## Residual after this wave

Unchanged from w13416: the base is a very strong local optimum.

* `0x602` `ldrd sl,r9` vs `ldrd r9,sl` — the `&Globals::layout`/`this`
  callee-saved tie; drives the 0x605-0x6cd and 0x758-0x768 runs (~155 B).
  Note that the *same* pair is loaded in the original order at `0x0b3e`
  in both binaries, so this is a per-reload allocator choice, not a global
  assignment.
* buy-list loop `0x1888..0x1ac5` (574 B): the `ldr.w sl,[sl]` value form.
  Its 4 bytes are exactly the `bic` of the original's switch dispatch, so
  the two are coupled; `nolay` + plain switch is 9,696 B.
* free-credits run `0x1c54..0x1d32` (223 B) plus the tail
  `0x1d48..0x1df3` (172 B): `textX` is sunk after `getText` and
  `canvas`/`textY` are spilled to `sp+204`/`sp+192`.
* `0x2154..0x2235` (206 B): buttons[17] scheduling.
* `0x253c..0x25b4` (84 B): cleanup landing-pad order.

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

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13434/`
(`HangarWindow.base.cpp`, `sweep.py`, `b1.py` … `b19.py`, `dump.py`,
`regions.py`, `side.py`, `t0.py`).
