# HangarWindow::render — wave 191b (w13845): free-credits declaration order + volatile canvas

Worktree `wt/w13845`, branch `wave191b-w13845`, base master `6382c7abf`
(includes w13799's signed-16-bit `switch ((short) count)` and w13823's
`textY`/`textYv` arm swap).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Landed wins (four commits)

All four touch only the free-credits `if (visible)` block and the buy-list
`String` declarations; the signed-short dispatch and the `textY`/`textYv` arm
assignment are unchanged.

1. **`rh` local last.**  Declare the free-credits `int rh =
   Globals::layout->field_0x2c_rowHeight;` after `getWidth()`/`getHeight()`,
   matching the original's width -> height -> rowHeight schedule.
   raw 1,456 -> 1,452, aligned 386 -> 379, `.text` 75,882 -> 75,878.
2. **`PaintCanvas *volatile canvas = Globals::Canvas;`.**  Making the canvas
   local volatile reproduces the original's re-load of the canvas pointer for
   each `SetColor`/`DrawString` instead of pinning it in a callee-saved
   register; this frees the register that was spilling the row-height locals.
   raw 1,452 -> 1,411, aligned 379 -> 462, `.text` 75,878 -> 75,837,
   `.ARM.extab` 552 -> 544.
3. **Free-credits declaration order C,W,R,H.**  `volatile canvas`, `width`,
   `rh`, `height` (canvas first).  Clang hoists the canvas load to the top of
   the visible block instead of sinking it past both accessor calls, putting
   the store/load pair where the original's first canvas load is.
   raw 1,411 -> 1,393, aligned 462 -> 461, `.text` 75,837 -> 75,819.
4. **Buy-list `String` order: `price` before `description`.**
   ```cpp
   String price("");
   String description("");
   ```
   (previously `description` first).  The original builds the `sp+312` temp in
   `r6` first, then the `sp+300` temp in `r5`; this declaration order is what
   makes clang pick that pair.  raw 1,393 -> **1,391**, `.text` 75,819 ->
   **75,817**, size exact.  Cost: aligned 461 -> 472 (+11).

### Measured (full `link_so`, `-j2`)

| metric | base (w13823) | landed |
|---|---:|---:|
| raw same-address diff | 1,456 | **1,391** |
| instruction-index alignment | 386 | 472 |
| function size | 9,700 | 9,700 |
| instructions | 3,733 / 3,734 | 3,732 / 3,734 |
| `.text` diff | 75,882 | **75,817** |
| `.ARM.extab` diff | 552 | **544** |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 | 0 / 0 / 0 |

Net: raw **-65**, `.text` **-65**, `.ARM.extab` **-8**, size exact,
`.rel.plt` byte-identical.  The aligned metric rises (386 -> 472) because the
volatile canvas changes the whole free-credits register schedule; raw is the
primary residual metric and the `.text` byte delta follows it exactly.

## Volatile canvas is required for the size (local optimum)

The original's free-credits code keeps `&Globals::Canvas` in `[sp,#140]` and
re-loads `[r6,#0]` before each call (`0x14ad3c ldr r6,[sp,#140]`,
`0x14ad44 ldr r0,[r6,#0]`, `0x14ad7c ldr r5,[r6,#0]`, `0x14ada6`/`0x14adb8`).
A volatile local reproduces that reload; anything weaker lets clang pin the
pointer and the function comes out short.  Variants measured on the landed
base (all keep the signed-short switch and the pf buy-list order):

| canvas form | raw | aligned | size |
|---|---:|---:|---:|
| `PaintCanvas *volatile canvas` (landed) | **1,391** | 472 | **9,700** |
| `PaintCanvas *canvas` (plain local) | 9,153 | 1,618 | 9,624 |
| `Globals::Canvas->` direct, no local | 3,739 | 554 | 9,732 |
| volatile for first `SetColor`, plain copy after | 2,950 | 734 | 9,712 |

Only the fully volatile local keeps the size at 9,700, so the current shape is
a local optimum for this block.

## Remaining residual (raw 1,391 / aligned 472)

Dominant clusters (`clusters.py`):

* `0x14b524` insert (cost 30) and `0x14b634`/`0x14b544`/`0x14b4be`/`0x14b59e..0x14b624`
  — LSDA landing pads and jump-table `b.n` offsets (+2 B); ours has an extra
  `mov r4,r0` landing pad at `0x14b526`.  Not source-orderable without a size
  change.
* `0x149b3c..0x149b96` ship guard (cost 12/11): the original hoists
  `ldrd r5,r4,[sp,#132]` before the `cbz` and orders the body
  `setPosition -> setVisible -> draw`; ours branches straight to `0x14a6d0`
  and keeps `setVisible -> draw -> setPosition`.  Every `{P,V,D}` permutation is
  size-fatal (9,704-9,708).
* `0x14ad20..0x14ae52` free-credits: the original keeps `textY` in `r6`
  (`0x14ad8c vmov r6,s0`), `rh` in `r9`, and `width` in `sl`; ours still
  spills `textY` to `[sp,#192]`/`[sp,#204]` and uses `s18`, and keeps the
  volatile store/load pair (`str [sp,#204]; ldr [sp,#204]`) where the original
  does `ldr r0,[r6,#0]` from the cached `&Globals::Canvas`.
* `0x14b22e..0x14b250` `buttons[17]` scheduling: the original hoists
  `sub.w r1,r6,r6,lsl#2` before `vmov s0,r5`.
* `0x1496da` `ldrd sl,r9` vs `ldrd r9,sl` — `this` <-> `&Globals::layout`
  RAGreedy tie; post-RA, not source-orderable (w13740/w13776/w13823).

## Sweeps already exhausted (do not repeat)

* All 24 permutations of the free-credits `C,W,R,H` declaration order
  (`patch21.py`/`sweep11.sh`), with and without the pf buy-list order: best is
  `CWRH` (landed); `CHRW` 1,394, `RWCH` 1,398, `WCRH` 1,399, the rest >= 1,410.
* All 8 permutations of `C,W,R,H,S,X,Y,V` (declaration order including the
  first `SetColor` and the `textX`/`textY`/`textYv` declarations): `CWRHSXYV`
  is the landed shape.
* `textX` declaration permutations (wrb/rwb/bwr/brw) all canonicalise to the
  same code.
* Buy-list `String description; String price;` (default ctor) is size-fatal
  (9,692); the two `String("")` temporaries are required.
* `textYv` variants: `volatile int` + volatile ref (9,696), `volatile int *`
  (9,696), `int textYv` (9,712), `textYv` removed from the first `DrawString`
  (9,704), all uses after the first `SetColor` direct (9,736).

## Harvest measurement (2026-11-13, on master 055321398)

On the current tree the `price`-before-`description` declaration order measures
raw **1,393 -> 1,391** (-2), aligned **461 -> 472** (+11), size 9,700 exact,
`.text` 75,770 -> 75,768.  The 2-byte `.text` gain costs 11 aligned slots, so
the code change was kept as this note; re-land it only if a later shape removes
the aligned regression.
