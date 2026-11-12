# HangarWindow::render — wave 190 (w13438): ship-body permutation re-sweep + DUPVB row-40

Worktree `wt/w13438`, branch `wave190-w13438`, base master `8d495265c`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result: no size-exact raw win found — `src/` left at the base

Entry state (re-measured on this branch):

| metric | value |
|---|---:|
| raw same-address diff | **1,336** |
| instruction-index alignment | 680 |
| function size | 9,700 / 9,700 |
| instructions | 3,729 / 3,734 |
| `.text` section diff | 72,878 |
| `.ARM.extab` section diff | 601 |
| `.note.gnu.build-id` | 20 |
| total differing bytes | **73,499** |

`check` is green (RATCHET PASS, byte_exact 4507, linked_exact 4507,
data_exact 843/843, imports 207, lint CLEAN).

Residual region map (unchanged from the wave-190 w13416 note):

```
0x0605-0x0619   4   blueprint ldrd sl/r9 tie
0x0632-0x0674  39   blueprint (same tie)
0x068a-0x06ce  16   blueprint (same tie)
0x0758-0x0769   4   blueprint (same tie)
0x0a48-0x0afa 159   ship body (sibling guard / cbz)
0x0ba8          1   GOT pool offset
0x1782/0x17a2/0x17c4  1 each  literal-pool offsets
0x1815-0x181d   4   buy-list preheader (`strd r9,sl`)
0x1832-0x186d   9   buy-list preheader
0x1888-0x1af2 519   buy-list loop (`sl` = layout value vs &layout)
0x1b10/0x1c10/0x2134/0x2314  1 each  literal-pool offsets
0x1b7a-0x1bd5  70   buy-list loop tail
0x1c54-0x1d33 166   free-credits (canvas/textY spills)
0x1d48-0x1df4  85   free-credits tail + jump-table shift
0x253c-0x2567  12   landing pads
0x258c-0x25b5  40   landing-pad cleanup order
```

## New evidence 1: the ship-body 120-permutation sweep re-confirmed

Full sweep of the five ship-guard body statements
(`setVisible(true)`, `int upgTab = tab;`, `draw()`, `setPosition(...)`,
`buttonOffset = getWidth() + rowHeight`) on *this* base.  Only 12 of the 120
permutations are size-exact:

| raw | `.ARM.extab` | note |
|---:|---:|---|
| **1,336** | **601** | `V,U,D,P,B` (base) |
| 1,326 | 1,216 | `D,U,P,V,B` (best raw, extab net loss) |
| 1,334 | 1,216 | `V,U,P,D,B` |
| 1,337 | 602 | `D,U,V,P,B` |
| 1,337 | 1,216 | `V,D,U,P,B` |
| 1,338 | 1,216 | `D,V,U,P,B` |
| 1,515-1,520 | 624-625 | eight `U`-first orders |

The base is the unique minimum on total (text+extab+build-id).  This
reproduces the w13416 finding on the current base.

## New evidence 2: DUPVB row-40 is one `movs` away from the 2-byte ULEB

`D,U,P,V,B` gives `.text` 72,868 (-10) but `.ARM.extab` 1,216.  Decoded LSDA
(`_work/reconstructions/extab_lsda_decode.py`):

```
row  orig                       D,U,P,V,B
39   (2508, 150, 9664)          (2508, 170, 9664)
40   (2696, 140, 9562)          (2710, 126, 9574)
41-45 exact                     exact
```

Row 40 is `[0x149b6e, 0x149bec)` = 126 bytes; the start is the `movs r3,#18`
immediately before the button-1 `setPosition` call.  The call-site table
length drops 927 -> 926, which shifts the rest of the record and is the whole
1,216-byte extab diff.  A start of `0x149b6c` or earlier (length >= 128) would
restore the 2-byte ULEB and the 927-byte table.

Measured ways to move the start, all with `D,U,P,V,B`:

| variant | raw | `.text` | extab |
|---|---:|---:|---:|
| base order `p,v,w,d` | **1,326** | **72,868** | 1,216 |
| `buttonOffset = rowHeight + getWidth()` | 1,475 | 73,017 | **605** |
| upgrade block `v,p,w,d` | 1,475 | 73,017 | **605** |
| upgrade block `v,p,d,w` | 1,467 | 73,009 | **605** |
| upgrade block `d,p,w,v` | 1,432 | 72,974 | **605** |
| upgrade block `d,p,v,w` | 1,434 | 72,976 | **605** |
| upgrade block `w,v,p,d` | 1,472 | 73,014 | **605** |

Every form that fixes the ULEB costs >= 106 `.text` bytes (the cheapest,
`d,p,w,v`, is 72,974 = +106).  All 24 upgrade-block orders were measured on
both the base and `D,U,P,V,B` ship bodies.  Casts on the `18` literal
(`(short)`, `(signed char)`, `(unsigned char)`), x/y operand swaps and
`getWidth`-first `buttonOffset` are all codegen-neutral.

## New evidence 3: `nolay` / `Layout *&lay` collapse the switch shape

Removing the loop-local `Layout *lay = Globals::layout;` (or binding
`Layout *&lay = Globals::layout;`, which is identical) reproduces the
original's address form inside the buy-list loop but is **9,692 B**:

* preheader: `add.w sl, sp, #300` (4 B) instead of `add r5, sp, #300` (2 B)
  — clang hands `sl` to the second `String("")` temporary;
* switch: `cmp.w r9, #4; tbb [pc, r9]` (6 B) instead of
  `bic.w r0, r9, #0x80000000; cmp r0, #4; tbb [pc, r0]` (8 B);
* loop tail: `ldrd sl, fp, [sp, #132]` instead of `ldr.w fp, [sp, #136]`.

The 8-byte deficit is spread over these three sites; no source form tried
recovers it (see the negative table below).  `Layout *const lay`,
`Layout &lay`, and `Layout **layp = &Globals::layout` are all equivalent.

## New evidence 4: the buttons[17] y-expression reassociation is not source-controllable

The original computes the `y` value as
`A - gsy*0.5f + (gsy+gbh)` (`vmla s2,s0,s16` then `vadd s0,s2,s4`), while
ours computes `(gsy+gbh) + A - gsy*0.5f` (`vadd` then `vmla`).  All of
`y_split` (two float statements), `y_paren`, `y_nomul1`, `y_top3`,
`y_div2`, `y_half` are byte-identical to the base (1,336 / 601).  Only
`y_paren2` (2,133) and `y_int` (8,224) move, both worse.

## Negative results measured this wave (all 9,700 B unless stated)

| variant | raw | `.text` | extab |
|---|---:|---:|---:|
| **base** | **1,336** | **72,878** | **601** |
| buy-list `String description`/`price` swapped | 1,338 | 72,880 | 601 |
| buy-list Strings on one line (desc first) | 1,336 | 72,878 | 601 |
| buy-list Strings on one line (price first) | 1,338 | 72,880 | 601 |
| `String description = String("")` copy-init | 1,336 | 72,878 | 601 |
| explicit `String(..., false)` on all 10 buy-list ctors | 1,336 | 72,878 | 601 |
| switch case source order reversed / 0,2,4,1,3 / 0,2,1,4,3 | 1,336 | 72,878 | 601 |
| `for (count = 0; count < 5; ...)` with expect | 1,336 | 72,878 | 601 |
| `int buttonOffset` before `String name` | 1,336 | 72,878 | 601 |
| `lay` before `count` / before `if (listModeFlag)` | 2,645 | 74,187 | 632 |
| `lay` as `Layout *&` before `if` | 3,037 | 9,692 B | 736 |
| `lay` used only for `top` | 8,007 | 79,549 | 782 |
| `lay` used only for `rowHeight` | 1,732 | 73,274 | 608 |
| `ltop`/`lrh` locals | 8,633 | 9,688 B | 1,092 |
| free-credits decl permutations (all 24 of W,R,H,T) | 1,336-1,458 | — | 601-609 |
| free-credits `canvas` first / 2nd / after height / after textX | 1,353 / 1,347 / 1,336 / 1,336 | — | 601 |
| free-credits `PaintCanvas *const volatile canvas` | 1,336 | 72,878 | 601 |
| free-credits `textY` before `canvas` | 1,338 | 72,880 | 601 |
| free-credits `float textYf` / `volatile int *textYv` | 1,336 | 72,878 | 601 |
| free-credits `rh` inlined into `textX` | 1,348 | 72,890 | 601 |
| free-credits `textX` declared last | 1,336 | 72,878 | 601 |
| free-credits `if (i != 0) {...} else {...}` | 3,511 | 9,664 B | 1,511 |
| free-credits two independent `if`s | 3,505 | 9,720 B | 1,480 |
| free-credits `visible` as one `||` chain | 2,945 | 9,696 B | 736 |
| free-credits `visible` as `switch (i)` | 3,208 | 9,688 B | 824 |
| `int upgTab = this->hangarList->getCurrentTab()` | 6,151 | 77,693 | 718 |
| `int upgTab = tab & 1` | 8,718 | 9,708 B | 1,543 |
| buttons[17] `x = w/2 + (gsx + bw)` | 2,192 | 9,720 B | 1,425 |
| buttons[17] scrollHint `x`/`y` replaced by the locals | 2,774 | 9,584 B | 1,661 |
| buttons[17] scrollHint `x` only / `y` only | 8,726 / 8,256 | 9,572 B | 1,894 / 1,781 |
| buttons[17] `const int rh` / `top` first / `int &rhv` | 1,336 | 72,878 | 601 |
| blueprint `Layout *&lay` + all `lay->` | 7,640 | 9,704 B | 28,085 |
| blueprint `Layout **layp` + `(*layp)->` | 7,640 | 9,704 B | 28,085 |
| `String name` outside the loop | 9,126 | 9,656 B | 19,705 |

## Notes for the next pass

* The base is an extremely stable local optimum on every axis tried.
* The single cheapest known improvement remains `D,U,P,V,B` (`.text` -10) and
  it is blocked purely by the LSDA row-40 ULEB width.  Any source form that
  lengthens row 40 by 2 bytes *without* growing the function (e.g. a 2-byte
  shape change between `0x149b6e` and `0x149bec` compensated elsewhere) would
  land a net ~10-byte win.
* The blueprint `ldrd sl,r9` tie (63 bytes) is a Greedy RA tie-break
  (documented in waves 121-125); no source form tried flips it.

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

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13438/`
(`HangarWindow.base.cpp`, `sweep_*.py`, `seqdiff.py`, `side.py`, `regions.py`).
