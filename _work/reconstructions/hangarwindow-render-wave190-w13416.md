# HangarWindow::render — wave 190 (w13416): buy-list `sl`/String-temp tie and LSDA row-40

Worktree `wt/w13416`, branch `wave190-w13416`, base master `001d5c971`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.
Entry state is the wave-190 w13379 body: raw **1,355**, aligned **617**,
`.text` 72,944, `.ARM.extab` 603, build-id 20, total 73,567.

## Result: no size-exact raw win — `src/` left at the base

All variants below are size-exact (9,700 B) unless stated.  The base is a very
strong local optimum: ~120 source-only variants were measured and only one
reached raw < 1,355, and it is an extab net loss (see below).

## New evidence 1: the buy-list loop is exactly a `sl`-for-String-temp tie

The original buy-list loop (`0x14a8ec..0x14ac74`) keeps `sl = &Globals::layout`
across the loop and derefs at both use sites:

```
orig 14ab02  ldr.w r1, [sl]        ; r1 = Globals::layout
     14ab24  ldr   r1, [r1, #32]   ; layout->field_0x20_top
     14abc2  ldr.w r2, [sl]
     14abc8  ldr   r2, [r2, #44]   ; layout->field_0x2c_rowHeight
```

The base keeps the *value* in `sl` (`ldr.w sl,[sl]` at `0x14a960`) and restores
the address at the loop tail (`ldr.w sl,[sp,#128]` at `0x14ac64`).  The two
forms are exactly size-neutral: the base spends +4 (loop-head load) and -4 (two
2-byte derefs become two 4-byte derefs), and the tail restore is 4 B vs the
original's `ldr.w fp,[sp,#136]` (also 4 B).

Removing the loop-local `Layout *lay = Globals::layout;` ("nolay") reproduces
the original's address form, but clang then gives `sl` to the second
`String("")` temporary and emits `add.w sl, sp, #300` (4 B) instead of the
original's `add r5, sp, #300` (2 B).  The loop preheader is therefore 2 B long
and the whole loop body shifts +2, which dominates the 767-byte buy-list raw
region.  `nolay` = 9,696 B; `nolay + expanded-x` (`mla` form) = 9,692 B.

The original's preheader uses `add r6,sp,#312` / `add r5,sp,#300`; the base
already uses `add r5,sp,#312` / `add r6,sp,#300` (both low regs, same size).
So the base's preheader is already the original's shape — only the
`ldr.w sl,[sl]` at `0x14a960` is extra.

### `K1`: the original preheader is reachable, but reallocates the whole function

Hoisting `int top = Globals::layout->field_0x20_top;` to the top of the loop
body (before the two `String("")` declarations) and using `3 * top` in `y`
makes clang keep `&Globals::layout` live across the constructors and use
`add r5,sp,#312` / `add r6,sp,#300` — exactly the original preheader.  But the
extra live value changes the *global* allocation: the first differing byte
moves to offset `0x10` and raw becomes 7,850 (`.text` 79,439).  Not usable.

### `nolay + expanded-x + f4a` is 9,700 but +2 shifted

`Layout *flay = Globals::layout;` at the top of the free-credits block, used
for the empty-string `DrawString`, restores the missing bytes on the wave-189
base; on the current wave-190 base it does **not** add the 4 B (the free-credits
block already reloads `&Globals::layout` from `sp+128`).  `nolay + exp2b + f4a`
stays 9,692 B here.  With the wave-189 body it was 9,700 B but the `add.w sl`
2-byte shift remained (raw 1,727).

## New evidence 2: ship-body order `D,P,V,B` is a raw win but an extab net loss

All 24 permutations of the ship-guard body statements
(`setVisible(V)`, `draw(D)`, `setPosition(P)`, `buttonOffset(B)`, with the
`upgTab` snapshot at position 1) were measured:

| order | raw | aligned | size | `.text` | `.ARM.extab` | build-id | total |
|---|---:|---:|---:|---:|---:|---:|---:|
| **`V,D,P,B` (base)** | **1,355** | **617** | **9,700** | **72,944** | **603** | **20** | **73,567** |
| `D,P,V,B` | **1,345** | 692 | 9,700 | **72,934** | 1,217 | 19 | 74,170 |
| `V,P,D,B` | 1,353 | 694 | 9,700 | 72,942 | 1,217 | 20 | 74,179 |
| `D,V,P,B` | 1,356 | 617 | 9,700 | 72,945 | 604 | 20 | 73,569 |

`D,P,V,B` is the best raw/`.text` result ever measured on this function
(raw 1,345, −10) but it is an extab net loss: the HangarWindow LSDA record
shifts internally by one byte.

### Why: LSDA row 40 length 126 vs 148

The `.ARM.extab` record (`0x1c30fc`, reclen 948) is the same size in both, but
row 40's call-site length encodes as one ULEB byte in `D,P,V,B`:

```
row 39  orig (2508, 150, 9664)   base (2508, 172, 9664)   DPVB (2508, 170, 9664)
row 40  orig (2696, 140, 9562)   base (2688, 148, 9574)   DPVB (2710, 126, 9574)
```

Row 40's end is fixed at `0x149bec` (`b.w 14a6d0`) in every order; only the
start moves.  `D,P,V,B` starts row 40 at `0x149b6e` (the `movs r3,#18` before
the button-1 `setPosition`), giving length 126 → one byte.  `V,D,P,B` starts at
`0x149b58` (the button-1 `draw`), giving 148 → two bytes.  The one-byte
difference shifts the rest of the record (bytes after record offset `0x104`),
so the section diff jumps 603 → 1,217 even though the record length is
unchanged.

A candidate fix for a future wave: make row 40's start ≤ `0x149b6c` (length
≥ 128) while keeping the `D,P,V,B` code order.  `buttonOffset` operand swap
(`getWidth() + rowHeight` → `rowHeight + getWidth()`) does restore the two-byte
encoding (extab 607) but costs 147 raw bytes (raw 1,492), so it is not a win
either.

## Negative results measured this wave (all 9,700 B unless stated)

| variant | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| **base** | **1,355** | **617** | **72,944** | **603** |
| `nolay` (direct `Globals::layout`) | 3,378 | 628 | 118,734 | 748 |
| `nolay + expanded-x` | 3,446 | 624 | 332,664 | 754 |
| `nolay + expanded-x + swap Strings` | 3,449 | 661 | 332,667 | 754 |
| `nolay + expanded-x + copy-init` | 3,446 | 624 | 332,664 | 754 |
| base + expanded-x (`mla`) | 3,315 | 627 | 118,671 | 746 |
| `K1` hoist `top` before Strings | 7,850 | 1,510 | 79,439 | 779 |
| `K2` move whole y-block before Strings | 9,231 | 1,775 | 342,280 | 27,126 |
| `K3` hoist `top`+`rh` before Strings | 8,441 | 1,436 | 340,769 | 898 |
| `H4` hoist `top` after Strings | 3,442 | 633 | 332,660 | 755 |
| `H3` hoist `rh` after Strings | 8,625 | 1,557 | 123,981 | 895 |
| `H1`/`H2` hoist both | 9,161 / 9,142 | 1,579 / 1,653 | 394,563 / 392,608 | 1,707 / 1,613 |
| free-credits `textX` before `height` | 1,355 | 617 | 72,944 | 603 |
| free-credits `rh` first | 1,364 | 620 | 72,953 | 603 |
| free-credits `height` first | 1,367 | 619 | 72,956 | 603 |
| free-credits `textX` inline `rh` | 1,367 | 619 | 72,956 | 603 |
| free-credits `textX` expression/paren/order/unsigned | 1,355 | 617 | 72,944 | 603 |
| free-credits `textX` inline `getWidth()` | 3,555 | 925 | 383,436 | 1,457 |
| blueprint `name` after the `if` | 1,880 | 690 | 73,469 | 638 |
| blueprint `Layout *lay` local | 7,848 | 1,152 | 388,517 | 28,088 |
| blueprint `HangarWindow *self` local | 7,622 | 776 | 384,713 | 28,080 |
| blueprint `bp` local | 9,059 | 2,255 | 392,277 | 28,132 |
| blueprint `rate` hoisted before `if` | 7,706 | 774 | 336,924 | 841 |
| blueprint `rate != 0.0f` | 1,508 | 580 | 73,097 | 630 |
| blueprint `borderX` reorder | 1,888 | 652 | 73,477 | 619 |
| blueprint `rowHeight + 0x260` swap | 1,384 | 626 | 72,973 | 603 |
| blueprint `drawItem` x reorder | 1,357 | 619 | 72,946 | 603 |
| ship `drawItem` x reorder | 1,357 | 619 | 72,946 | 603 |
| ship guard `tab != 0` after `expect(...)` | 8,626 | 895 | 340,954 | 893 |
| ship guard `selectedItem->ship != getShip()` | 1,360 | 614 | 72,949 | 603 |
| ship body `upgTab` first | 1,532 | 639 | 73,121 | 627 |
| ship body `draw` before `setVisible` | 1,534 | 639 | 73,123 | 627 |
| ship body `buttonOffset` swap | 1,502 | 631 | 73,091 | 607 |
| ship body `y + field_0x114` swap | 1,355 | 617 | 72,944 | 603 |
| upgrade body `p,v,v2,d` (base is `p,v2,v,d`) | 1,356 | 617 | 72,945 | 604 |
| upgrade body `p,d,v2,v` | 1,513 | 630 | 73,102 | 627 |
| upgrade check outside guard (J/K) | 8,590 | 970 | 341,457 | 19,054 (size 9,676) |
| upgrade check after guard with snapshot (L) | 4,593 | 794 | 76,182 | 1,223 |
| `if (this->viewMode == 1)` → `!= 0` | 1,790 | 771 | 125,663 | 603 |
| `barPos > 0 \|\| barHeight > 0` swap | 1,357 | 619 | 72,946 | 603 |
| `this->selectedItem == item` | 1,356 | 618 | 72,945 | 603 |
| `item->isShip() != 0` / `isItem() != 0 \|\| ...` | 1,355 | 617 | 72,944 | 603 |
| `if (!this->viewMode)` / `!= 24` / `if (items)` / `0 == tab` | 1,355 | 617 | 72,944 | 603 |
| `dialogActive == 0` / `buyCreditsActive != 0` / `freeCreditsActive != 0` / `listModeFlag != 0` / `listModeFlag == 0` / `dragging == 0` / `upgradeMode == 0` | 1,355 | 617 | 72,944 | 603 |
| blueprint operand reorders (P1–P20, 20 variants) | 1,355–1,392 | 617–632 | 72,944–72,981 | 603–605 |

## Residual after this wave

Unchanged from the wave-188/189 notes:

* ship/blueprint run (`0x1496da..0x149bf0`): pure `r10`/`r9` physreg tie between
  `&Globals::layout` and `this` at `0x1496da` (`ldrd sl,r9` vs `ldrd r9,sl`);
  every source reorder tried keeps the swap.  The first differing byte is still
  offset `0x602`.
* buy-list run (`0x14a8ec..0x14ac74`): the value-form `lay`'s 4-byte
  `ldr.w sl,[sl]` at the loop head; the address form is 4 B short and/or takes
  `sl` for a `String` temporary (`add.w sl,sp,#300`).
* free-credits run (`0x14ac74..0x14ae7e`): `textX` is sunk after `getText` in
  ours, so `width`/`rh` stay live and canvas/textY spill to `sp+204`/`sp+192`.
  No source shape tried (declaration order, parenthesisation, inlining,
  volatile, unsigned) moves the add.
* tail (`0x14ae7e..0x14b6bc`): landing-pad table shifted by the buy-list 4-byte
  delta; the cleanup landing-pad order also differs (the `sp+324` pad is before
  the six `sp+288` pads in ours, after them in the original).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 /home/debian/GalaxyOnFire2/scratch/w13416/bytediff.py _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13416/`
(`HangarWindow.base.cpp`, `sweep.py`, `vardefs*.py`, `bytediff.py`,
`lsdadump.py`, `shift.py`, `hdiff.py`).

## Wave-190 second pass (same worker): branch-target / cold-block / callee-saved lens

After the first note was committed (`bc98160c0`), a second battery of source-only
variants was measured against the same base (raw 1,355 / `.text` 72,944 /
`.ARM.extab` 603 / build-id 20 / total 73,567).  **No size-exact win landed**;
`src/` is untouched.

### Confirmed: row 40 is exactly 2 bytes short of a 2-byte ULEB

`D,P,V,B` (t1) row 40 is `(2710, 126)`; 126 < 128 so the length encodes as one
ULEB byte and the whole 948-byte record shifts by one byte.  Row 40's end is
fixed at `0x149bec` (end of the upgrade block's `buttons[10]->draw()`), so the
only way to fix it is to add 2 bytes **inside** row 40 (between the button-1
`setPosition` call at `0x149b70` and that final draw) while keeping the total
function size at 9,700.

Proof of concept: `buttonOffset` operand swap
(`getWidth() + rowHeight` → `rowHeight + getWidth()`) adds exactly 2 bytes in
that window and fixes the width — row 40 becomes `(2710, 128)`, `cst_len` 927,
`.ARM.extab` **607** — but it reallocates the `getWidth` sequence
(`strd r9,r8,[r4,#-88]!`) and costs 147 raw bytes (raw **1,492**,
`.text` 73,081, total 73,708).  Net loss.

### Best raw ever measured: 1,343 (still an extab net loss)

`D,P,V,B` with `buttons[2]->setVisible(this->upgradeMode)` (semantically
identical inside `if (upgradeMode && ...)`) gives raw **1,343**, `.text`
**72,932**, but row 40 is still `(2710, 126)` → `.ARM.extab` 1,217.  Total
72,932 + 1,217 + 20 = 74,169.  Same for the `buttons[10]` variant (1,353).
All eight combinations of `{buttons[1],buttons[2],buttons[10]} ×
setVisible(upgradeMode)` were measured; every one keeps `.ARM.extab` 1,217.

### `D,P,V,B` t0: row 39 becomes exact, but the ship body reallocates

`upgTab` before `draw` (`S_DPVB_t0`) gives row 39 `(2508, 150, 9664)` — an
**exact** match for the original's row 39 — and row 40 `(2680, 158, 9598)`
(2-byte width).  `.ARM.extab` 627 (24 worse than base), but raw **1,533**
(the ship-body/tail reallocation costs 178).  Net loss.

### The mid2 1-byte diff (`0xba8`) is a PC-relative GOT-pool offset, not a constant

`0xba8` is the literal-pool word used by the ship-guard `Globals::status`
access (`ldr r0,[pc,#340]; add r0,pc; ldr r0,[r0]`).  Original:
`ldr r0,[pc]; ldr r1,[r4,#104]; add r0,pc` → pool `0xc662a`, target
`0x21015c`.  Base: `ldr r0,[pc]; add r0,pc` → pool `0xc662c`, same target.
The 1-byte diff is purely the 2-byte-later `add r0,pc` in the original
(which schedules the `this->selectedItem` load before the `getShip` call).
Swapping the comparison (`this->selectedItem->ship != getShip()`) reproduces
the original's `ldr r1` placement but moves the `add` 2 bytes the *other* way
(pool `0xc6628`) and costs 5 raw bytes (raw 1,360).  Not compensable.

### Robustness checks (all neutral or worse)

* 84 pairwise/triple combinations of the neutral blueprint-branch operand
  reorders (`bgy`, `dry`, `rate*100`, `rate*width`, `rate*(width-4)`,
  `iconOffsetY+y`, ship/pp y-swaps): all raw **1,355**, extab 603.
* `switch (count)` → `if/else if` chain in the buy-credits block: clang
  re-recovers the same `tbb` table, byte-identical (1,355 / 603).
* Free-credits: `textX` computed after `SetColor` (L1) and before `height`
  (L5) are neutral; `textY` after `SetColor` (L2) and both after (L3) are
  1,396 / 611.
* `String description("")` → `String description;` (and `price`): 9,696 /
  9,696 (size short), extab 964 / 733.
* Blueprint `setVisible(upgradeMode)` / `setVisible(upgTab != 0)` /
  `!!upgTab` / `upgTab == 1` on `buttons[1]`: all 1,345 / 1,217 (the guard
  makes `tab != 0` known, so the boolean forms fold).
* `D,P,V,B` setPosition argument shapes (px/py locals, parenthesisation,
  `18u`/`18L`, y-swap, evaluation-order swap): all byte-identical to
  1,345 / 1,217.
* `D,P,V,B` upgrade-block `setPosition` x/y reorders, `17` via variable,
  parenthesisation: all 1,345 / 1,217.

### `vardefs19` results (first pass, previously unrecorded)

| variant | raw | aligned | `.text` | `.ARM.extab` | size |
|---|---:|---:|---:|---:|---:|
| `V1_rev_chain` | 1,361 | 625 | 72,950 | 625 | 9,700 |
| `V2_visible_init` | 1,361 | 623 | 72,950 | 623 | 9,700 |
| `V3_row_before_setcolor` | 5,046 | 1,051 | 77,505 | 1,051 | 9,700 |
| `V4_row_first` | 5,046 | 1,051 | 77,505 | 1,051 | 9,700 |
| `V5_tx_volref` | 6,448 | 1,718 | 332,664 | 1,718 | 9,716 |

### Residual after the second pass

Unchanged: the base is a strong local optimum.  The two live leads for a
future wave are (a) add exactly 2 bytes inside `D,P,V,B` row 40 without
reallocating the `getWidth` sequence, and (b) flip the blueprint-branch
`r9`/`sl` tie (which also drives the 767-byte buy-list shift) — both remain
unreached by source-only reorders.

## Wave-190 third pass (same worker): D,V,P,B ship body + `setVisible(upgradeMode)` — LANDED

After the second-pass note (`466a50f4f`), a third battery targeted the
remaining 2-byte gap in the `D,P,V,B` ship-body branch (raw 1,345 but a
1-byte LSDA row-40 length, `.ARM.extab` 1,217).  The landed shape keeps the wave-190 ship body
(`V,D,P,B`: setVisible, `upgTab`, draw, setPosition, buttonOffset) and adds
one source change inside the upgrade block:

```cpp
(*this->buttons)[2]->setVisible(this->upgradeMode);
```

This is semantically identical to `setVisible(true)` because the call is
guarded by `__builtin_expect(upgTab == 1 && this->upgradeMode, 1)`.
The extra read of `upgradeMode` lets clang drop two bytes from the
upgrade-block argument setup.

| metric | base (`V,D,P,B`) | landed (`V,D,P,B` + upgradeMode) |
|---|---:|---:|
| raw same-address diff | 1,355 | **1,353** |
| instruction-index alignment | 617 | 619 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,730 / 3,734 | 3,729 / 3,734 |
| `.text` section diff | 72,944 | **72,942** |
| `.ARM.extab` section diff | 603 | 603 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,567 | **73,565** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.

### Upgrade-block order sweep on `D,P,V,B`

All 12 orders of `{setPosition(10), setVisible(2), setVisible(10), draw(10)}`
with `setVisible(10)` before `draw(10)`.  Only orders with `setVisible(2)`
before `setPosition(10)` widen LSDA row 40 to a 2-byte ULEB; every such
order costs ~147 raw bytes (best `v2Pd` raw 1,491, total 73,707), so the
`D,P,V,B` + `D,V,P,B` route is the only one that keeps the text win.

### `D,V,P,B` visibility-form sweep

| form | raw | `.text` | `.ARM.extab` | total |
|---|---:|---:|---:|---:|
| **`setVisible(2, upgradeMode)` (landed)** | **1,353** | **72,942** | **603** | **73,565** |
| `setVisible(2, true)` (D,V,P,B base) | 1,356 | 72,945 | 604 | 73,569 |
| `setVisible(2, upgradeMode)` + `setVisible(10, upgradeMode)` | 1,364 | 72,953 | 604 | 73,577 |
| `setVisible(2, upgTab == 1)` / `upgTab != 0` | 1,356 | 72,945 | 604 | 73,569 |
| `setVisible(2, !!upgradeMode)` | 1,354 | 72,943 | 604 | 73,567 |

### Additional negative results after the landed pass

All on the landed base (raw 1,353 / `.text` 72,942 / `.ARM.extab` 603 /
total 73,565).  Every entry is size-exact 9,700 B unless stated.

| variant | raw | `.text` | `.ARM.extab` | total |
|---|---:|---:|---:|---:|
| landed base | 1,353 | 72,942 | 603 | 73,565 |
| `D,V,P,B` ship body + `setVisible(2, upgradeMode)` | 1,354 | 72,943 | 604 | 73,567 |
| `D,P,V,B` ship body + `setVisible(2, upgradeMode)` | 1,343 | 72,932 | 1,217 | 74,169 |
| upgrade block `v2,p,v,d` (faithful) | 1,492 | 73,081 | 607 | 73,708 |
| buy-list `String price` declared before `description` | 1,355 | 72,944 | 603 | 73,567 |
| buy-list description-first assignments | 1,353 | 72,942 | 603 | 73,565 (tie) |
| declaration swap + description-first assignments | 1,355 | 72,944 | 603 | 73,567 |
| free-credits `width/rh/height` declaration order (6 perms) | 1,353–3,481 | — | 603–1,439 | ≥73,565 |
| free-credits `textX` associativity (3 forms) | 1,353 | 72,942 | 603 | 73,565 (neutral) |
| free-credits `i < 5` / ternary / `0.2f` operand / `row += 1` | 1,353 | 72,942 | 603 | 73,565 (neutral) |
| free-credits direct `Globals::Canvas` (no local) | 3,474 | 383,844 | 1,464 | 9,720 B |
| free-credits `canvas` only for first two calls | 3,480 | 383,850 | 1,465 | 9,720 B |
| buy-list `count / 3` without `(unsigned char)` | 1,353 | 72,942 | 603 | 73,565 (neutral) |
| ship guard restructured as nested `if (tab != 0)` | 4,593–8,583 | — | 1,223–1,304 | 9,680–9,700 B |
| blueprint `unsigned bg = this->progressBarBgImage` local | 7,542 | 384,633 | 28,337 | 9,704 B |

The `D,P,V,B` route remains the only shape that lowers raw below 1,353,
but its 1-byte LSDA row-40 length costs 614 extab bytes; the landed
`V,D,P,B` + `setVisible(upgradeMode)` keeps row 40 at a 2-byte ULEB and is
the best total measured.
