# HangarWindow::render — wave 191b (w13572): cluster map, no net win

Worktree `wt/w13572`, branch `wave191b-w13572`, base master `cb9c59ca0`.
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, size-exact.

Base (unchanged, gate green): raw **1,038**, aligned **620**,
`.text` **71,232**, `.ARM.extab` **577**, `.rodata` 485, build-id 20,
total **72,314**.  Render LSDA cst 927, record diff 38.
`check` RATCHET PASS: byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub/missing/wrong_type/extra 0, data_exact
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Diff-cluster map (base)

| offset | bytes | insn O/U | area |
|---|---:|---:|---|
| +0x1c42 | 233 | 102/97 | free-credits `visible` block (getWidth/rh/getHeight/SetColor/textY/textX/DrawString) |
| +0x0a48 | 159 | 72/71 | ship guard / upgrade block |
| +0x1b7a | 149 | 51/51 | buy-list String dtors + free-credits loop entry |
| +0x2154 | 100 | 31/30 | free-credits `buttons[17]` y computation |
| +0x21ce | 99 | 37/36 | free-credits `buttons[17]` draw |
| +0x183b | 77 | 34/33 | buy-credits grid switch |
| +0x1d60 | 52 | 30/29 | free-credits String temp dtor |
| +0x258c | 40 | 18/18 | cleanup landing pads |
| +0x0632 | 38 | 16/16 | blueprint `sl`/`r9` role swap |

The residual is register allocation, not statement order: the free-credits
run has `rh` in `sl`/`r9` instead of the original's `r9`, and the
buy-list String temporaries land in `sl`/`r5` instead of `r5`/`sp+312`.

## Key LSDA facts (measured)

`rowmap.py` maps rows to code.  Row 39 is the cleanup range for the
ship-row block (`Ship::getIndex`..`isShip`/`getShip`); row 40 is the
range for the selected-ship body (`setPosition`..end of upgrade block).

* base VDP body: r39 `(2508,172,9664)`, r40 `(2688,148,9550)`, cst 927.
* DPV body (`draw; upgTab; setPosition; setVisible`): raw **1,028**,
  `.text` **71,222**, r39 `(2508,170,9664)`, r40 `(2710,126,9550)`.
  r40 length 126 < 128 encodes in one uleb byte -> cst 926 ->
  `.ARM.extab` **1,210**.  Net total 72,937 (+623).
* hoisted `upgTab` + PVD body (w13522): raw **928**, `.text` **71,122**,
  r39 `(2508,118,9664)` (the `getShip` call moves into row40 because
  `upgTab`'s scope spans it), cst 926, `.ARM.extab` **1,216**.
  Net total 72,843 (+529).
* DPV with `hintOffsetX` first in `setPosition` x: raw **1,026**,
  `.text` **71,220**, r40 still 126, extab 1,210.

To turn DPV (or the hoisted PVD) into a net win, one row length must
cross 128 so cst stays 927.  DPV needs +2 B in the row-40 range; the
hoisted PVD needs +10 B in the row-39 range.  Every attempted shape
that adds those bytes also changes the function size (not size-exact)
or destroys the body register match.

## Negative results measured this wave (all size 9,700 unless noted)

Free-credits `visible` block:
* decl orders of `rh`/`width`/`height`: base `rh,width,height` **1,038**;
  `rh,height,width` 1,040; `height,rh,width` 1,040;
  `width,rh,height` 1,088 (merges the free-credits and buy-list
  clusters into one 490-byte cluster); `width,height,rh` 1,088;
  `height,width,rh` 9,704.
* `const` on any of `rh`/`width`/`height`: byte-identical.
* `textY`/`textX`/`canvas` decl permutations, `textX` split/reassociated,
  `canvas` before the decls, `SetColor` before the decls: base best
  (`SetColor` first -> 9,692).
* `volatile int textY`, `volatile int textY; textY = ...` after
  `SetColor`, `volatile int *`, `const volatile int &`, plain copy,
  `int ty = textYv` read-once: 9,692-9,744 or raw >= 2,700.
* `textX` as `volatile int`, assigned after `SetColor`: 9,708 / 9,636.
* `String plus` / `String amount` named temporaries: 9,660-9,684.
* free-credits `baseX`/`baseY` order and reassociation: base best
  (`x_reassoc` 1,039).
* free-credits y expression reassociation / `0.2F` / `/5.0f`: base best
  (`/5.0f` 1,081).

Ship guard / body:
* all 6 body orders x 4 `upgTab` insertion positions x 2 upgrade orders:
  base `VDP` with `upgTab` between `setVisible` and `draw`, `buttons[10]`
  first, is the only cst-927 shape below 1,039 raw (1,038).
* `upgTab` types (`long`, `unsigned`, `const`, `const &`, `int &`,
  `bool = tab == 1`, `bool = tab != 0`): byte-identical; `signed/unsigned
  char`, `short` 9,708; `bool = tab != 0` 9,692.
* `upgTab` declared at function top / loop body / before `isSelectable`
  / before `item == selectedItem`: 9,676-9,704, raw 6,88x, cst 914.
* ship-guard operand flip (`selectedItem->ship != getShip()`): hoisted
  PVD 942 (vs 928); base VDP 1,043.
* two-if (upgrade check outside the guard), nested `if (tab != 0)`,
  `goto`, `switch (tab)`, `__builtin_expect(tab == 0, ...)`: 9,668-9,720,
  raw >= 2,749; no `cbz` shape ever appears.
* `int shipIdx = item->ship->getIndex();`, extra `SetColor` before the
  ship name, `name = String("")` first: 9,680-9,708.
* `if (item->isShip() && item->isSelectable())`: r39 becomes 128 but
  the function grows to 9,716 and raw jumps to 6,796.

Upgrade block:
* all 24 permutations of `setPosition(10)` / `setVisible(2)` /
  `setVisible(10)` / `draw(10)` with the DPV body: those starting with
  `setPosition(10)` keep r40 126 (raw 1,028-1,052); the rest give r40 128
  but raw 1,134-1,179.
* moving `buttonOffset = getWidth()+...` after the upgrade block:
  9,668-9,696, cst 920-921.

Buy list:
* `leftText`/`rightText` declaration swap, `SetColor` before them,
  `String` vs `"t"` construction: base best (swaps 9,644-9,664).
* moving `String name;`/`int buttonOffset;` out of the loop: 9,672.

Buy-credits grid:
* `switch ((signed char) count)` is load-bearing; `(unsigned char)` /
  `(short)` / `count & 0x7fffffff` / plain `count` / `count < 5` /
  `count != 5`: byte-identical or 9,644-9,704.
* `Globals::h/2 - gridButtonHeight/2 - 3*top` parenthesisation and
  hoisted half-values: byte-identical.

## Conclusion

No size-exact net win found.  Base retained; tree left clean.
The irreducible residual is the free-credits register allocation
(`rh`/`width`/`height` roles and the volatile `textY` spill), the
buy-list `String`-temp register roles, and the row-39/row-40 split
caused by `upgTab`'s clang cleanup scope.
