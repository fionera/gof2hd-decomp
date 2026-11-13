# HangarWindow::render — wave 191b (w13682): faithful upgrade-body `setVisible(true)`

Worktree `wt/w13682`, branch `wave191b-w13682`, base master `b221b3826`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Landed: one operand fix, aligned -104

```diff
-                                    (*this->buttons)[2]->setVisible(this->upgradeMode);
+                                    (*this->buttons)[2]->setVisible(true);
```

The enclosing guard is `if (__builtin_expect(upgTab == 1 && this->upgradeMode, 1))`,
so `this->upgradeMode` is provably `true` inside the body and the change is a
semantic no-op.  The original really does pass the literal:

```
149bd2  ldr  r0,[sp,#88]        ; this->buttons
149bd4  ldr  r0,[r0,#0]
149bd6  ldr  r0,[r0,#4]
149bd8  ldr  r0,[r0,#40]        ; buttons[10]
149bda  movs r1,#1              ; literal true
149bdc  blx  TouchButton::setVisible(bool)
```

With the literal, our `buttons[10]->setVisible(true)` at `0x149bd2` becomes
instruction-for-instruction identical to the original's, and the
`buttons[2]->setVisible(true)` block at `0x149bc4` becomes identical to the
original's `0x149ba6..0x149bb2` block (it is still at a different address
because the original emits the two `setVisible` calls in the order
V2, P10, V10, D while the source keeps P10, V2, V10, D — see below).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,038** | **1,040** (+2) |
| instruction-index alignment | 620 | **516** (-104) |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 | 3,727 / 3,734 |
| `.text` section diff | 71,247 | 71,249 (+2) |
| `.ARM.extab` section diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 | 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 | 20 |

`check` RATCHET PASS: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub/missing/wrong_type/extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

The raw +2 is a coincidence of byte values, not a structural regression: the
two forms are the same 16 B, but the base's `ldr r0,[r4,#36]` /
`ldrb.w r1,[r4,#285]` pair happened to share two bytes with the original at
those addresses.  The instruction-level diff (`tools/fn_diff.py --json`)
drops `operand:b.n` 128 -> 113 and `operand:ldr` 81 -> 46.

## Residual: three independent causes

Everything else in the 1,040 B is downstream of three things.

### 1. The `this` <-> `&Globals::layout` register tie (`0x1496da`, ~135 B)

```
orig 1496da  e9dd a921  ldrd sl, r9, [sp,#132]   ; sl=&Globals::layout, r9=this
ours 1496da  e9dd 9a21  ldrd r9, sl, [sp,#132]   ; r9=&Globals::layout, sl=this
```

The prologue stores are identical (`[sp+132]=&Globals::layout` at 0x1491b0,
`[sp+136]=this` at 0x14911a).  The whole blueprint run `0x1496de..0x149840`
is instruction-for-instruction identical apart from `r9`/`sl`.  `this`'s
reload interval is longer than `layout`'s (3,124 vs 3,068 slot units), so
LLVM 7's `(1<<29)+Size` queue priority would put `this` first (r9) — the
original.  Ours processes `layout` first, so `this` must be deferred as
`RS_Split`.  The stage is not observable in the r18b `-print-after-all` dump
(`-debug-only=regalloc` is compiled out; `-print-after=greedy` is not a valid
pass name in this build).  Every source shape that flips the pair also moves
code or re-lays the block (w13613/w13664 tables).

### 2. The buy-list `switch` mask (+2, shifts everything after `0x14a962`)

The original really has the sign-mask range check:

```
14a960  f029 4000  bic.w r0, r9, #0x80000000
14a964  2804       cmp   r0, #4
14a966  d820       bhi.n 14a9aa
14a968  e8df f000  tbb   [pc, r0]
```

`switch ((signed char) count)` (the current source) proves the range and emits
`cmp.w r9,#4; bhi; tbb`, 2 B shorter.  Every spelling of the masked switch —
`switch (count)`, `switch (count & 0x7fffffff)`,
`switch ((unsigned int) count)`, `switch (static_cast<unsigned int>(count))` —
emits the `bic` and costs **+4**: 2 B for the `bic`, plus 2 B for a `nop`
that clang inserts at `0x14aa7e` to re-align the literal pool (the pool moves
from `0x14aa88` to `0x14aa8c`).  Result: size 9,704.  Because the 2 B shift
starts inside the buy-list loop, the free-credits block, `buttons[17]` and
the cleanup landing pads all shift by 2, which is what makes those clusters
(373 + 206 + 60 B) differ at all — their instruction streams are otherwise
byte-identical.

### 3. The ship/upgrade body statement order

The original ship body is `setPosition, setVisible, draw` (P,V,D) and the
upgrade body is `setVisible(2), setPosition(10), setVisible(10), draw`
(V,P,V,D).  The source keeps `setVisible, draw, setPosition` (V,D,P) and
`setPosition(10), setVisible(2), setVisible(10), draw` (P,V,V,D) because
those are the size-exact shapes.  Adopting the faithful orders costs raw
(1,187-1,225) and/or `.ARM.extab` (581-586).

## Measured candidates (all size 9,700 unless noted)

`ldrd` column is the reload at `0x1496da` (`r9,sl` = ours, `sl,r9` = orig).

| variant | raw | aligned | `.text` | extab | ldrd |
|---|---:|---:|---:|---:|---|
| base | **1,038** | 620 | 71,247 | 577 | r9,sl |
| **`setVisible(true)` (landed)** | **1,040** | **516** | 71,249 | 577 | r9,sl |
| `setVisible(true)` + `description` first | 1,042 | 505 | 71,251 | 577 | r9,sl |
| `setVisible(true)` + `buttons[17]` P,R,D | 1,060 | 505 | 71,269 | 577 | r9,sl |
| `setVisible(true)` + free-credits V,P,D | 1,074 | 509 | 71,283 | 577 | r9,sl |
| `setVisible(true)` + ship body P,V,D | 1,191 | 499 | 71,400 | 586 | r9,sl |
| `setVisible(true)` + `unsigned int i` in for | 1,042 | 518 | 71,251 | 577 | r9,sl |
| `setVisible(true)` + `const float rate` | 1,040 | 516 | 71,249 | 577 | r9,sl |
| `description` first | 1,040 | 609 | 71,249 | 577 | r9,sl |
| `buttons[17]` setPosition,replaceText,draw | 1,058 | 609 | 71,267 | 577 | r9,sl |
| free-credits setVisible,setPosition,draw | 1,072 | 613 | 71,281 | 577 | r9,sl |
| ship body setPosition,setVisible,draw | 1,191 | 600 | 71,400 | 586 | r9,sl |
| upgrade body V2,P10,V10,D + `true` | 1,187 | 497 | 71,359 | 581 | r9,sl |
| free-credits D,P,V | 1,071 | 615 | 71,280 | 577 | r9,sl |
| free-credits V,D,P | 1,044 | 620 | 71,253 | 577 | r9,sl |
| free-credits V,P,D | 1,072 | 613 | 71,281 | 577 | r9,sl |
| free-credits P,D,V | 1,077 | 624 | 71,286 | 577 | r9,sl |
| free-credits P,V,D | 1,078 | 624 | 71,287 | 577 | r9,sl |
| `switch (count)` | 3,442 | 653 | 378,999 | 1,425 | r9,sl |
| `switch (count & 0x7fffffff)` | 3,442 | 653 | 378,999 | 1,425 | r9,sl |
| `switch ((unsigned int) count)` | 3,442 | 653 | 378,999 | 1,425 | r9,sl |
| `switch (count)` + `String price;` | 1,312 | 899 | 71,521 | 866 | r9,sl |
| `switch (count)` + `String description;` | 1,299 | 625 | 71,508 | 598 | r9,sl |
| `String price;` (default ctor) | 3,429 | 929 | 117,405 | 957 | r9,sl |
| `if (tab != 0 && expect(...))` guard | 1,173 | 715 | 71,382 | 590 | r9,sl |
| guard nested `if (tab != 0) { if (...) }` | 1,173 | 715 | 71,382 | 590 | r9,sl |
| guard `if (tab == 0) {} else { if (...) }` | 1,038 | 620 | 71,247 | 577 | r9,sl |
| `int upgTab` after `draw()` | 1,070 | 531 | 71,279 | 1,210 | r9,sl |
| `int upgTab` at end of body (9,708) | 6,606 | 717 | 382,192 | 1,929 | r9,sl |
| `float rate` declared before the `if` (9,692) | 7,656 | 785 | 335,552 | 841 | r9,sl |
| `int bgX` local before first DrawImage2D (9,704) | 7,548 | 815 | 383,315 | 28,317 | ? |
| `textX + 2 + hintOffsetX + buttonInsetX` (9,704) | 7,597 | 767 | 383,154 | 1,510 | fp,r5 |
| `textX = field_0x4c + field_0x2cc + iconSpace` | 1,047 | 623 | 71,256 | 577 | r9,sl |
| `baseY = field_0x20_top + field_0xc_leftMargin + ...` | 1,040 | 621 | 71,249 | 577 | r9,sl |
| `if (barPos > 0 \|\| barHeight > 0)` | 1,040 | 622 | 71,249 | 577 | r9,sl |
| `if (barHeight > 0 \|\| barPos != 0)` | 1,143 | 625 | 71,352 | 581 | r9,sl |

Neutral (byte-identical to base): `const float rate`, `const int borderX`,
`const int upgTab`, `if (this->listModeFlag != 0)`, `String price = String("")`,
`(textX + 3)`, `(rowHeight/2)` parenthesisations, `int i;` instead of
`unsigned int i;`, `int b` vs `unsigned int b`, `++count`, `++b`,
`(unsigned char)` casts dropped from `row`/`col`, `if (!this->viewMode)`,
`if (items)`, `if (this->buyCreditsActive != 0)`,
`if (this->freeCreditsActive != 0)`, `(void)` no-op guard spellings.

## Why the switch shift cannot be bought back

Adding the `bic` needs -4 somewhere.  The only -4 levers found are
`String price;`/`String description;` (default ctor, which re-lays the whole
buy-list: raw 1,299-1,312) and moving `setVisible(true)` before the two
`String`s (also re-lays the loop).  Both land the size at exactly 9,700 but
the code between the `bic` and the saving shifts, so the free-credits /
`buttons[17]` / landing-pad runs do not re-align.  The 2 B `nop` at
`0x14aa7e` is jump-table/literal-pool alignment, not a source shape.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13682/` — `base.cpp` (pristine),
`base.so`, `switch.so`, `measure.py`, `secdiff.py`, `sbs.py`, `seqdiff.py`,
`clusters.py`, `bytediff.py`, `bp.py`, `run_variant.py`, `batchA..J.py`,
`perm_results.json`, `allpass.txt` (80 MB `-print-after-all` dump).
