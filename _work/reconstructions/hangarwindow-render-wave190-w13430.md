# HangarWindow::render — wave 190 (w13430): upgradeMode/signed-char harvest + raw hunt

Worktree `wt/w13430`, branch `wave190-w13430`, base master `4be393e1b`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.
Entry state (this branch, before the change): raw **1,355**, aligned **617**,
`.text` 72,921, `.ARM.extab` 603, build-id 20, total 73,544.

## Landed: master's `upgradeMode` + `(signed char) count` pair (raw 1,355 -> 1,336)

This branch was cut from `4be393e1b`, which predates master's `2ca5121db`
("createMission goods-before-route + HangarWindow upgradeMode/signed-char").
The two HangarWindow lines from that commit are re-applied here:

```cpp
(*this->buttons)[2]->setVisible(this->upgradeMode);   // guarded by upgTab==1 && upgradeMode
...
switch ((signed char) count) {                       // buy-credits price/description switch
```

Both are semantics-preserving: the `setVisible` call sits inside
`__builtin_expect(upgTab == 1 && this->upgradeMode, 1)`, and `count` is in
`[0,4]` so the `signed char` cast is the identity.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,355 | **1,336** |
| instruction-index alignment | 617 | 680 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| instructions | 3,730 / 3,734 | 3,729 / 3,734 |
| `.text` section diff | 72,921 | **72,902** |
| `.ARM.extab` section diff | 603 | **602** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,544 | **73,524** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.

### Why `(signed char) count` helps

The original buy-credits dispatch is the *signed int* switch shape:

```
orig 14a960  bic.w r0, r9, #0x80000000   ; clear sign bit
     14a964  cmp   r0, #4
     14a966  bhi.n 14a9aa
     14a968  tbb   [pc, r0]
```

With a plain `int count` our base reproduces that shape but keeps the extra
`ldr.w sl,[sl]` at `0x14a960` (the buy-list `lay` value form), shifting the
whole switch body +4.  With `(signed char) count` clang knows the range and
emits the shorter `cmp.w r9,#4` + `tbb [pc,r9]`, which removes 2 bytes from
the dispatch and moves the following code -2 instead of +4.  That phase shift
happens to align the switch case bodies better: region `0x1800` drops
136 -> 125 diff bytes and `0x1900` 264 -> 258.

`(short)`, `(signed short)`, `(signed char)` all produce the identical
1,336 / 72,902 / 602 result; `(unsigned short)` and `(long)` give
1,353 / 72,919 / 603 (worse than the landed form).  Declaring `count` itself
as `signed char`/`char`/`unsigned char` is 9,704 B (raw 1,899); `short`
is 9,732 B.

## Negative results measured this wave (all size-exact 9,700 unless stated)

On the landed base (raw 1,336 / `.text` 72,902 / extab 602):

| variant | raw | aligned | `.text` | extab |
|---|---:|---:|---:|---:|
| **landed base** | **1,336** | **680** | **72,902** | **602** |
| free-credits `textX` declared after `textYv` | 1,336 | 680 | 72,902 | 602 |
| free-credits `textX` before `height` | 1,336 | 680 | 72,902 | 602 |
| free-credits `Layout *lay` at y only, `Globals::layout` at DrawImage2D | 1,697 | 567 | 73,263 | 610 |
| free-credits `Layout *lay` at DrawImage2D only, `Globals::layout` at y | 9,704 B | — | — | — |
| free-credits `textX` volatile / `textYv` volatile value / no canvas volatile | 6,434 / 3,093 / 3,480 | — | 9,704-9,716 B | — |
| free-credits `width`/`rh` declaration swap | 1,364 | 620 | 72,930 | 603 |
| free-credits `width`,`height`,`rh` order | 1,367 | 619 | 72,933 | 603 |
| sibling ship guard (`if (guard) {body} if (expect(upgTab==1&&upgradeMode,1)) {upg}`) | 4,593 | 794 | 76,159 | 1,223 |
| sibling + `upgradeMode && upgTab == 1` | 6,209 | 681 | 9,696 B | 1,254 |
| sibling + `tab` used in the check | 4,593 | 794 | 76,159 | 1,223 |

The sibling ship-guard shape still reallocates the whole function on this
base (its `tab == 0` branch folds to the far continuation instead of the
nearby upgrade check), so it is not usable.

## Residual after this wave

Unchanged from the wave-189/190 notes:

* ship/blueprint run (`0x1496da..0x149bf0`): pure `r10`/`r9` physreg tie between
  `&Globals::layout` and `this` at `0x1496da` (`ldrd sl,r9` vs `ldrd r9,sl`).
* buy-list run (`0x14a8ec..0x14ac74`): the value-form `lay`'s 4-byte
  `ldr.w sl,[sl]` at the loop head; the address form is 4 B short and/or takes
  `sl` for a `String` temporary (`add.w sl,sp,#300`).
* free-credits run (`0x14ac74..0x14ae7e`): `textX` is sunk after `getText` in
  ours, so `width`/`rh` stay live and canvas/textY spill to `sp+204`/`sp+192`.
* tail (`0x14ae7e..0x14b6bc`): landing-pad table shifted by the buy-list 4-byte
  delta; the cleanup landing-pad order also differs.

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

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13430/`.

## Landed: buy-list address form + non-volatile canvas (raw 1,336 -> 1,324)

The buy-list loop's `Layout *lay = Globals::layout;` local made clang keep the
*value* of `Globals::layout` in `sl` (`ldr.w sl,[sl]` at the switch head,
`ldr.w sl,[sp,#128]` reload at the loop tail).  The original keeps the
*address* of the global and dereferences it (`ldr.w r1,[sl]` /
`ldr.w r2,[sl]` at `0x14ab02`/`0x14abc2`), i.e. it uses `Globals::layout->`
directly.  Dropping the local alone is 8 B short (9,692), but pairing it with
the free-credits block's canvas local losing its `volatile` qualifier
(`PaintCanvas *canvas = Globals::Canvas;` instead of
`PaintCanvas *volatile canvas`) puts the function back at exactly 9,700 B and
matches the original's `Globals::Canvas` address-form codegen there:

```cpp
// buy-list loop: no `lay` local, use Globals::layout directly
int y = (int) ((float) (Globals::h / 2 - this->gridButtonHeight / 2 - 3 * Globals::layout->field_0x20_top) - ...);
Globals::Canvas->DrawImage2D(this->tabIcons[count], x, y - Globals::layout->field_0x2c_rowHeight, 17, 68);
...
// free-credits: plain pointer local (each read is still a fresh load of the local)
PaintCanvas *canvas = Globals::Canvas;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,336 | **1,324** |
| instruction-index alignment | 680 | 708 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| instructions | 3,729 / 3,734 | 3,728 / 3,734 |
| `.text` section diff | 72,902 | **72,890** |
| `.ARM.extab` section diff | 602 | **598** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,524 | **73,508** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

### Why the pair works

The two changes are individually size-neutral-ish but only the combination is
size-exact: `nolay` alone is 9,692 B (raw 3,037 — the whole tail shifts -8),
`canvas` non-volatile alone is 9,708 B (raw 3,480).  Together they cancel to
9,700 B and both regions move toward the original's address-form codegen.
Other combinations measured this wave: `nolay` + direct `Globals::Canvas->`
9,716 B (raw 2,938); `nolay` + free-offer call reorder 9,692 B; `nolay` +
`textY`-before-`textX` 9,692 B; `nolay` + `rh`-before-`width` 9,692 B.

## Landed: free-credits `rh` before `width` (raw 1,324 -> 1,317)

In the free-credits block the declaration order was `width`, `rh`, `height`;
moving `rh` ahead of `width` matches the original's evaluation order:

```cpp
int rh = Globals::layout->field_0x2c_rowHeight;
int width = button->getWidth();
int height = button->getHeight();
int textX = width + baseX + rh;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,324 | **1,317** |
| instruction-index alignment | 708 | 717 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| instructions | 3,728 / 3,734 | 3,726 / 3,734 |
| `.text` section diff | 72,849 | **72,842** |
| `.ARM.extab` section diff | 597 | **596** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,466 | **73,458** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

### Negative results around this cell

Other declaration orders in the same block were neutral at 1,324:
`textX` after `width`; `width`/`textX`/`rh`/`height`; `width`/`height`/`rh`;
`height` before `rh`. `textY`-first was 1,327 and `rh`-first (before the
free-credits block existed in this order) had been measured earlier at 1,345
on the 1,324 base with different surrounding code.

## Landed: buy-list String declaration/assignment order (raw 1,317 -> 1,314)

Swapped the two buy-list `String` declarations so `price` is constructed
before `description`, and swapped every switch-case assignment to match
(description first):

```cpp
String price("");
String description("");
...
case 0:
    description = String(Globals::cItemListDescription_00);
    price = String(Globals::cItemListPrice_00);
    break;
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,317 | **1,314** |
| instruction-index alignment | 717 | 727 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 | 3,726 / 3,734 |
| `.text` section diff | 72,842 | **72,839** |
| `.ARM.extab` section diff | 596 | 596 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,458 | **73,455** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

Declaration swap alone is 1,319 (+2); assignment swap alone is 1,316 (-1);
the pair is 1,314.  The buy-list String temp still lands in `sl`
(`add.w sl,sp,#300`), so the 2-byte preheader shift is unchanged; the win is
in the switch-case body layout.

## Landed: ship guard `tab == 0` empty arm (raw 1,314 -> 1,179)

The ship guard was `if (tab != 0 && __builtin_expect(...))`.  Rewriting the
zero-tab test as an empty leading arm is semantically identical but gives the
zero test its own basic block:

```cpp
if (tab == 0) { } else if (__builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1)) { // lint: arm_builtin
    ...
}
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,314 | **1,179** |
| instruction-index alignment | 727 | 632 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 | 3,726 / 3,734 |
| `.text` section diff | 72,839 | **72,704** |
| `.ARM.extab` section diff | 596 | **583** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,455 | **73,307** |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical, 27/27 headers, file size 2,187,328 exact.

### Why it works

The empty arm splits the CFG at the zero-tab test, which changes the global
register allocation: the buy-list switch case bodies move from
`mov r0,sl` (price String in `sl`) to the original's `mov r0,r6`, and the
whole buy-list switch region (function offsets `0x1900..0x1a00`) becomes
byte-aligned: its diff drops from 71 to 6 bytes, and `0x1800` from 150 to
83.  The preheader still emits `add.w sl,sp,#300` instead of `add
r5,sp,#300`, but the case bodies now line up.

Equivalent forms measured: `if (tab != 0) { if (expect(...)) {...} }` is
1,314 (no change); `if (tab > 0 && expect(...))` is also 1,179 but is only
equivalent for non-negative tabs, so the `tab == 0` empty arm is used.

## Landed: free-credits `textY` before `canvas` (raw 1,179 -> 1,177)

```cpp
int textX = width + baseX + rh;
int textY = (int) ((float) y + (float) height * 0.2f);
volatile int &textYv = textY;
PaintCanvas *canvas = Globals::Canvas;
canvas->SetColor(0xffffffffu);
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,179 | **1,177** |
| instruction-index alignment | 632 | 634 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 72,704 | **72,702** |
| `.ARM.extab` section diff | 583 | 583 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,307 | **73,305** |

Gate: `check` RATCHET PASS.  `textY` before `textX` is the same 1,177;
`textY` after `SetColor` is 2,887 (9,692 B).

## Landed: buy-list button call order (raw 1,177 -> 1,144)

The buy-list loop issued `setPosition, draw, replaceTextKeepSize,
setSplitText`.  The original issues `setPosition, replaceTextKeepSize,
setSplitText, draw` (its `draw` call is at `0x14aae4`, after both String
setters), so the source now matches:

```cpp
(*this->buttons)[12 + count]->setPosition(x, y, 68);
(*this->buttons)[12 + count]->replaceTextKeepSize(description);
(*this->buttons)[12 + count]->setSplitText(price);
(*this->buttons)[12 + count]->draw();
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,177 | **1,144** |
| instruction-index alignment | 634 | 619 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 72,702 | **72,669** |
| `.ARM.extab` section diff | 583 | 583 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,305 | **73,272** |

Gate: `check` RATCHET PASS.  Moving the `DrawImage2D(tabIcons[count], ...)`
call into the reordered block as well (`bl_rsd2`) is 9,688 B — not usable.

## Landed: buy-list `x` expanded (raw 1,144 -> 1,077)

The buy-list grid x was written as a factored product.  Expanding it by the
distributive law is semantically identical and matches the original's codegen:

```cpp
int x = Globals::w / 2 - this->buttonWidth - this->gridSpacingX +
        col * (this->buttonWidth + this->gridSpacingX);
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,144 | **1,077** |
| instruction-index alignment | 619 | 604 |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 72,669 | **72,602** |
| `.ARM.extab` section diff | 583 | 583 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 73,272 | **73,205** |

Gate: `check` RATCHET PASS.  Swapping the multiplication operands is neutral
(1,144); `(col - 1)` forms are 1,388.
