# MenuTouchWindow C1/C2 wave 118 (w6618): ctor + OnTouchBegin dead-end sweep

Date: 2026-11-05. Worktree `wt/w6618`, branch `wave118-w6618`, base
`3d4a98a3e` (wave-117 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Targets:
`_ZN15MenuTouchWindowC1Ei` / `_ZN15MenuTouchWindowC2Ei` (weak alias, same
address/size/score) and `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`.

## TL;DR

* **No legal source change improved either target this wave.** The committed
  tree is unchanged (`src/game/ui/MenuTouchWindow.cpp` untouched).
* Ctor: **99.668675 %**, 9600 B, 3 differing regions (11 instructions).
* OnTouchBegin: **98.092643 %**, 1896 B, 2 differing regions (18 instructions).
* Gate: `byte_exact 4501 / linked_exact 4501`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27 sections, `.so`
  exactly 2,187,328 B. `.ARM.extab` in this worktree's baseline is
  `0x2628c` @ `0x1a3b78` (the prompt/wave-117 notes say `0x26294`; a fresh
  out-of-tree build reproduced `0x2628c`, so this is the current master
  baseline, not a change made here). `.rodata` `0x41c27` @ `0x1c9e10`.

## 1. Ctor: the three remaining regions

The ctor is 3320 instructions. The only structural diffs are the three
regions below; everything else is byte-identical.

### 1a. Mission block `0x122da6` (6 insns)

Original materialises `int row2 = row + 2` **before** `GameText::getText`
into the callee-saved `r4`, which frees `r5` for the `String` temporary:

```
122d9e ldr.w r0,[r8]        ; Globals::gameText
122da2 movw  r1,#395
122da6 adds  r4,r5,#2       ; row2 = row + 2
122da8 blx   getText
122dac add   r5,sp,#372     ; String temp in r5
...
122dc2 mov   r0,sl
122dc4 movs  r1,#18
122dc6 mov   r2,r5
122dc8 mov   r3,r4          ; row2
122dca blx   addButton
```

Committed tree (`char row2 = row + 2;`) rematerialises the add after the
`String` ctor and truncates:

```
122da6 blx   getText
122daa add   r4,sp,#372     ; String temp in r4
...
122dc0 adds  r0,r5,#2
122dc6 uxtb  r3,r0
```

Both blocks are 54 B, so the function size (9600 B) and every branch
displacement match; the 3 hunks are pure placement/register allocation.
Measured variants (all legal C++):

| variant | size | fuzzy |
|---|---:|---:|
| committed `char row2 = row + 2;` | 9600 | **99.668675** |
| `row + 2` directly in the call | 9596 | 94.727328 |
| `int row2 = row + 2;` | 9596 | 94.727328 |
| `long long row2 = (long long)row + 2;` + `(int)row2` | 9596 | 94.727328 |
| `unsigned int row2 = (unsigned)row + 2;` + `(int)row2` | 9596 | 94.727328 |
| hoist `String &missionText = *getText(395);` then `int row2` | 9596 | 94.727328 |
| `row2` declared before the `if` | 9596 | 94.727328 |
| `row += 2;` then pass `row` | 9596 | 94.727328 |
| `int row2` + `if (row2 > 0)` | 9596 | 94.727328 |
| `int row2` + `if (row2 != 0)` | 9596 | 94.727328 |
| `int row2` + `if (row2 > row)` | 9596 | 94.727328 |
| `int row2` + `__builtin_assume(row2 > row)` | 9596 | 94.727328 |
| pass `row2 & 0x7fffffff` | 9600 | 99.412562 |
| pass `row2 & 0xff` | 9600 | 99.668675 (same as `char`) |
| hoist `Array<TouchButton*>* btnArr = this->buttons;` | 9660 | 84.739137 |

The `char` truncation is what keeps the block at 54 B (the `uxtb` replaces
the `mov r3,r4`); the original's `int` add is a machine-sinker remat target
and always lands after the `String` ctor in clang 7.0.2. The wave-115
`asm "+r"` shape target remains the only known way to pin it.

### 1b. `loadPreviewRecords` inlined iPad y-subtraction `0x12339e` (4 insns)

Original subtracts `buttonRowGap` first, then `rowHeight`; ours the reverse
(clang's reassociate pass canonicalises `(yBase - gap) - rowHeight` to
`(yBase - rowHeight) - gap`):

```
orig: subs r1,r6,r2 ; subs r1,r1,r3
ours: subs r1,r6,r3 ; subs r1,r1,r2
```

Measured variants (all 9600 B / 99.668675 unless noted — identical code):

* swap the two source operands (`yBase - rowHeight - gap`);
* split into `int yAdjust = yBase - gap; y = (yAdjust - rowHeight) + ...`;
* `yBase - (gap + rowHeight)`;
* `(unsigned)` casts on the whole subtraction;
* `(long long)` casts;
* `const int &rowGap = ...` / `const int *rowGap = ...` inside the branch;
* local `int rowGap` inside the branch.

Variants that change the subtract order but cost bytes:

| variant | size | fuzzy |
|---|---:|---:|
| `((yBase - gap) & 0x7fffffff) - rowHeight` | 9612 | 90.079783 |
| local `int rowGap` before the loop (spills to `[sp,#60]`) | 9608 | 93.798916 |
| `volatile int rowGap` inside the branch | 9612 | 90.397351 |
| use `this->buttonRowGap` instead of the layout field | 9604 | 94.489612 |

The local-before-the-loop variant does produce the original's
`subs gap` / `subs rowHeight` order, but the spill and the extra 8 B shift
every downstream branch target.

### 1c. `scrollExtraButton` y expression `0x123e44` (1 insn)

Original schedules `str r2,[sp,#4]` (the `66` height) before
`add r0,r3` (`field_0x2fc + sliderRowHeight`); ours has `add r0,r3` first.
The final value is identical; clang reassociates
`field_0x2fc + ((0x15c + rowHeight) & mask) + sliderRowHeight` to
`(field_0x2fc + sliderRowHeight) + (0x15c + rowHeight)`.

Measured variants (all 9600 B / 99.668675 — identical code):

* explicit parentheses around `(field_0x2fc + inner)` before `+ sliderRowHeight`;
* remove the `sliderRowHeight & 0x7fffffff` mask;
* add a mask on `field_0x2fc`.

Variants that move the add but cost bytes:

| variant | size | fuzzy |
|---|---:|---:|
| mask the whole `(field_0x2fc + inner)` group | 9596 | 95.707185 |
| local `int barHeight = field_0x15c + rowHeight` | 9624 | 95.110576 |
| local `int sx = listTopY + listBottomY - 25 - buttonInsetX` | 9612 | 93.620223 |

## 2. OnTouchBegin: the two remaining regions

18 differing instructions, all pure register renames / one hoisted load.

### 2a. Case-8 `listEntryWidth` `0x129fd2` (2 insns)

Original uses `ip` for `this->listEntryWidth`; ours uses `r4`:

```
orig: ldr.w ip,[sl,#340] ; add r3,ip
ours: ldr.w r4,[sl,#340] ; add r3,r4
```

The identical case-3 copy at `0x129d4a` uses `ip` on both sides. Adding a
`Layout &layout` local to case 8 (all four conditions, or only the fourth),
or a local `int lew = this->listEntryWidth`, produces byte-identical code
(98.092643, 1896 B). Swapping the operand order of the fourth condition
(`top + leftMargin + width`, `width + leftMargin + top`,
`leftMargin + width + top`, explicit groupings) also produces identical
code for the first and fourth forms and 92.569870 for the middle two.

### 2b. Case-15 scrollbar block `0x12a1e0` (16 insns)

The instruction sequence is identical; every register differs. Original
reloads `Globals::layout` from `[sp+16]` in the second condition and loads
both `field_0xc_leftMargin` and `field_0x20_top` from that reloaded pointer:

```
orig:
12a212 ldr  r2,[sp,#16]
12a214 ldr.w r1,[r0,r1,lsl#2]
12a218 ldr  r2,[r2,#0]      ; reload layout
12a21a ldr.w r0,[fp]
12a21e ldr  r5,[r2,#12]     ; leftMargin
12a220 ldr  r6,[r2,#32]     ; top
12a222 blx  GetImage2DHeight
12a226 adds r1,r6,r5        ; top + leftMargin
```

Committed baseline (best) caches the layout pointer in `r6` from the first
condition and uses it for `top`, while reloading only for `leftMargin`:

```
12a212 ldr  r2,[sp,#16]
12a214 ldr  r4,[r6,#32]     ; top from cached layout
12a216 ldr.w r1,[r0,r1,lsl#2]
12a21a ldr  r2,[r2,#0]
12a21c ldr.w r0,[fp]
12a220 ldr  r5,[r2,#12]     ; leftMargin
12a222 blx  GetImage2DHeight
12a226 adds r1,r5,r4
```

Measured variants (A = first-condition `buttonInsetX`, B = second-condition
`top`, C = second-condition `leftMargin`; L = local `Layout&`, G = global
`Globals::layout->`):

| A/B/C | size | fuzzy |
|---|---:|---:|
| **L/L/G (committed)** | **1896** | **98.092643** |
| G/L/G | 1896 | 98.092643 (same code) |
| L/G/L | 1896 | 97.956403 |
| G/G/L | 1896 | 97.956403 |
| L/G/G | 1892 | 90.313779 |
| G/G/G | 1892 | 90.313779 |
| L/L/L | 1892 | 90.859482 |
| G/L/L | 1892 | 90.859482 |
| swap `&&` operands | 1892 | 89.085948 |
| hoist `int barIndex = this->field_0x1e0` | 1892 | 90.661213 |
| drop the inner parentheses (left-assoc sum) | 1896 | 97.138965 |
| swap outer `+` operands | 1892 | 90.661213 |
| swap inner `+` operands | 1896 | 94.414169 |
| nested `if` with a second `Layout&` | 1896 | 90.102389 |
| nested `if` with `int h = GetImage2DHeight(...)` | 1896 | 88.147139 |
| Canvas call first (`w - width - buttonInsetX`) | 1892 | 89.768076 |
| `PaintCanvas *canvas = Globals::Canvas;` | 1892 | 90.524881 |
| Canvas local + baseline second condition | 1896 | 97.617427 |
| `volatile Layout &layout` | 1892 | 89.631651 |
| `Layout *layout = Globals::layout;` (pointer form) | 1896 | 98.092643 (same code) |
| move `Layout &layout` to the top of case 15 | 1896 | 98.092643 (same code) |

The original's second-condition reload is a register-allocation choice:
the compiler keeps the reference address in the callee-saved `r6` across
the `GetImage2DWidth` call in our build. Every source form that forces a
reload either also changes the GOT register assignment (`fp` vs `[sp+16]`)
or loses the 4 B that keeps the function at 1896 B.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6618
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```
