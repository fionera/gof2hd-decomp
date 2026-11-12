# HangarWindow::render — wave 190 (w13468): raw sweep, no size-exact win

Worktree `wt/w13468`, branch `wave190-w13468`, base master `8e3604cca`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result: no size-exact raw win — `src/` left at the base

Entry state (re-measured on this branch):

| metric | value |
|---|---:|
| raw same-address diff | **1,038** |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 |
| `.text` section diff | 72,218 |
| `.ARM.extab` section diff | 578 |
| build-id | 20 |
| total differing bytes | 72,816 |

`check` is green (RATCHET PASS, byte_exact 4507, linked_exact 4507,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370).  `.rel.plt` byte-identical.

## Region map (current base, function-relative offsets)

```
0x0605-0x0769  155   blueprint: `ldrd sl,r9` vs `ldrd r9,sl` + schedule
0x0a48-0x0afa  159   ship body: `cbz`/`cmp+beq.w` + call order
0x0ba8           1   literal-pool word (PC-relative, consequence of shift)
0x1815-0x1af8  ~330  buy-list: `sl`/`r6` String-temp tie + switch shape
0x1b7a-0x1df3  ~450  free credits: preheader order + textY/canvas schedule
0x1e40           1   stack slot (`sp+132` vs `sp+128`)
0x2134-0x2236  206   buttons[17]: y-float schedule + call order
0x2314/2338/2372 3   literal-pool word + stack slots
0x253c-0x25b5   84   cleanup landing-pad order
```

## New structural facts established this wave

### 1. The buy-list preheader +2 is exactly cancelled by the switch -2

The current base is the *nolay* (address-form) variant: the buy-list loop
uses `Globals::layout->` directly, so clang spills `&Globals::layout` to
`sp+128` and reuses `sl` for the second `String("")` temporary.  That makes
the preheader 2 B longer than the original:

```
orig  183a  add r6, sp, #312        ours  183a  add r5, sp, #312
      183e  add r5, sp, #300              183e  add.w sl, sp, #300   (+2)
```

The switch dispatch is 2 B shorter (`cmp.w r9,#4` + `tbb [pc,r9]`, 10 B)
than the original (`bic.w` + `cmp` + `tbb [pc,r0]`, 12 B), so the switch
body still lands at `0x1894` in both.  The loop destructor is 2 B shorter
(no `add r6,sp,#312`), and the free-credits preheader is 6 B shorter
(no `str.w sl,[sp,#128]`; `ldr r2,[r0,#8]` + `ldrd r3,r0,[r0,#40]` instead
of `ldr r3,[r1,#8]` + `ldrd r6,r1,[r1,#40]`).  The accounting is therefore
spread over four sites, not a single 2-byte shift.

### 2. The blueprint `ldrd` tie is per-reload, not global

At `0x0602` orig is `ldrd sl,r9,[sp,#132]` (`sl=&Globals::layout`,
`r9=this`) and ours is `ldrd r9,sl` (swapped).  The *same* pair is loaded
in the original order at `0x0b3e` in both binaries (`ldrd sl,r9`), so the
allocator choice is local to the `getCompletionRate()` reload, not a
function-wide assignment.  The two values are spilled at the same slots
(`sp+132`/`sp+136`) in both; only the physical-register order differs.

### 3. Free-credits preheader evaluation order

Orig loads `dialog->x/y` first, then `layout->windowTopInset` and
`buttonInsetX/rowHeight`, and interleaves `baseX` (`add.w r8,r6,r2`) with
`baseY` (`add r0,r3` then `add.w r0,r0,r1,lsl #1`).  Ours loads the layout
fields first and computes `baseY` fully before `baseX`.  `baseX` is stored
at `sp+108` in orig and `sp+116` in ours.

## Negative results measured this wave (all size 9,700 B unless stated)

Entry base raw **1,038**.  Every row below is a fresh build + `fn_diff`.

### Buy-list

| variant | raw |
|---|---:|
| String declaration swap (`description` first) | 1,040 |
| declaration + assignment swap | 1,052 |
| assignment swap only | 1,054 |
| `row`/`col` from plain `int` division | 1,038 |
| `row`/`col` declaration swap | 1,038 |
| `row` unsigned | 1,039 |
| `x` `(gridSpacingX + buttonWidth)` operand swap | 1,038 |
| `switch (count)` (recovers `bic`) | 3,442 (9,704 B) |
| `switch (count+0)` / `+count` / `count|0` / `count*1` / `count-0` | 3,442 (9,704 B) |
| `switch ((int)(signed char) count)` / `(signed short)` / `(short)(signed char)` | 1,038 |
| `switch (count)` + `Layout *lay` value form | 8,774 (9,764 B) |
| default `String("", false)` | 1,038 |
| default `break` (no assignments) | 3,564 (9,648 B) |
| `String name`/`int buttonOffset` swap | 1,038 |
| `baseY`/`y` swap | 1,046 |
| `if (y > h || y < 0)` | 1,052 |

### Blueprint

| variant | raw |
|---|---:|
| `if (0.0f < rate)` | 1,038 |
| `if (rate != 0.0f)` | 1,317 |
| `if (rate >= 0.0001f)` | 8,572 (9,708 B) |
| `float rate0 = ...; if (rate0 > 0)` | 7,596 (9,704 B) |
| `float rate` before the `if` | 7,626 (9,704 B) |
| `Layout *lay` at branch top / after `name` / before `if` | 7,596-7,641 (9,704 B) |
| `Layout *lay` at function top, used in blueprint x/y | 9,273 (9,740 B) |
| `Layout *lay` at function top, all blueprint uses | 9,277 (9,732 B) |
| `int bg` / `int hint` / `int pw` hoisted before the `if` | 7,596-7,693 (9,704-9,712 B) |
| `int bx` / `int by` / `int bg` locals for the first `DrawImage2D` | 7,548 (9,704 B) |
| any reorder of the first `DrawImage2D` x expression | 7,597 (9,704 B) |
| `this->progressBarBgImage + 0` | 1,038 |
| `progress` String operand order | 1,069 |
| `progress` x `width` last | 1,045 |
| `progress` y `1` last | 1,040 |
| `drawItem` `hintOffsetX` first | 1,040 |
| `borderX` before `DrawRegion2D` | 5,124 |
| `SetColor(0x777777ffu)` before `String progress` | 1,255 |

### Ship body / upgrade block

| variant | raw |
|---|---:|
| `selectedItem->ship != status->getShip()` operand swap | 1,043 |
| `!(selectedItem->ship == status->getShip())` | 1,043 |
| `const int upgTab` | 1,038 |
| `upgTab` setVisible after draw | 1,188 |
| `__builtin_expect(upgTab == 1 && upgradeMode, 0)` | 1,121 |

### Free credits

| variant | raw |
|---|---:|
| `baseY` declared before `baseX` | 1,038 |
| `baseY` faithful (`dialog->y + top + rh*2`) | 1,043 |
| `baseY` `top + dialog->y + rh*2` | 1,043 |
| `baseY` `rh*2 + top + dialog->y` | 1,038 |
| `baseX` dialog-first (revert pass 3) | 1,039 |
| `width` before `rh` | 1,088 |
| `height` before `width` | 1,040 |
| `textX` after `width` / `baseX+width+rh` / `rh+width+baseX` | 1,038 |
| `PaintCanvas *volatile canvas` | 2,760 (9,692 B) |
| `textY` before `textX` | 1,038 |
| `canvas` after `textYv` | 1,038 |
| `textY` after `canvas` | 1,038 |
| `volatile int textYv; textYv = textY;` | 2,853 (9,716 B) |
| `int textYc = textY; volatile int &textYv = textYc;` | 2,853 (9,716 B) |
| `const volatile int &textYv` | 1,038 |
| `*(volatile int *) &textY` | 1,038 |
| `Layout *lay` for the free-credits preheader | 1,038 |
| all 12 valid `{draw,setVisible,y,setPosition}` orders | base `DVYP` 1,038 is best; `VDYP` 1,044, `VYPD` 1,072, `YPDV` 1,077 |

### Buttons[17]

| variant | raw |
|---|---:|
| `rh`/`top` declaration swap | 1,038 |
| `gsy` first | 1,038 |
| `x` `(buttonWidth + gridSpacingX)` operand swap | 1,038 |
| `setPosition` before `draw` | 1,057 |
| `y` `(gsy+gbh)` term first | 1,895 (9,708 B) |

### viewMode block / prologue

| variant | raw |
|---|---:|
| `!station` / `nullptr == station` / `!viewMode` | 1,038 |
| `SetColor` after `station` | 8,464 (9,740 B) |
| `barHeight` before `barPos` | 1,041 |
| `listWidth` first | 8,926 (9,708 B) |
| `tab` before `drawBG` | 1,065 |
| `items` before `tab` | 9,091 (9,668 B) |
| `const int tab` / `const int items` / `const float rate` | 1,038 |

### `else if` chain permutations (adjacent swaps)

| swap | raw | size |
|---|---:|---:|
| `isMoveToCargoButton` <-> `isSellButton` | **1,044** | 9,700 |
| `isShip` <-> `isSlot` | 7,489 | 9,696 |
| `isSlot` <-> `isBluePrint` | 7,989 | 9,692 |
| `isBluePrint` <-> `isPendingProduct` | 7,870 | 9,684 |
| `isPendingProduct` <-> `isMoveToCargoButton` | 7,209 | 9,720 |
| `(ship,slot,blueprint,moveToCargo,sell,pending)` | 7,308 | 9,716 |
| `(moveToCargo,sell,ship,slot,blueprint,pending)` | 8,697 | 9,644 |

### Exhaustive pair / random-combination search

All 171 pairs of 19 individually-neutral edits (declaration order, casts,
operand swaps, `const`, `unsigned`, `Layout *lay`, textY forms) were built
and measured: no pair beat 1,038.  A further 400 random 1-3-edit
combinations from a 23-edit pool also produced no win (best 1,038).

## Residual after this wave

The base remains a strong local optimum.  The two live leads are unchanged:

* flip the blueprint `ldrd r9,sl` reload at `0x0602` (drives ~155 B directly
  and renames `sl`/`r9` through the block); the same pair loads in the
  original order at `0x0b3e`, so a per-site allocator perturbation is
  required, not a global one;
* get the buy-list second `String` temporary into a low register (`r6`)
  while keeping `sl = &Globals::layout`, which would fix the preheader
  (`add r5,sp,#300`), the destructor (`add r6,sp,#312`) and the switch
  (`bic`+`cmp`) together and align the whole buy-list loop.  Every source
  form tried so far either reallocates the loop or changes the size by
  +-4 B.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13468/`
(`HangarWindow.base.cpp`, `sweep.py`, `regions.py`, `diffside.py`,
`batch_*.py`, `pair.py`, `rand.py`, `greedy_render.txt`, `mi.txt`).
