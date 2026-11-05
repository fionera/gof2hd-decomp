# MenuTouchWindow C1/C2 wave 117 (w6518): sliders[2] grouping + mask

Date: 2026-11-05. Worktree `wt/w6518`, branch `wave117-w6518`, base `c22678d64`
(wave-116 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Targets:
`_ZN15MenuTouchWindowC1Ei` / `_ZN15MenuTouchWindowC2Ei` (weak alias, same
address/size/score) and `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`.

## TL;DR

* Ctor: **99.036145% -> 99.668675%** (9600 B unchanged, 3320/3320
  normalised instructions). The `sliders[2]` `setPosition` block is now
  byte-identical (was ~21 differing instructions).
* OnTouchBegin: **98.092643%** unchanged (all 12 hunks are register renames;
  no legal variant found this wave either).
* Gate: `byte_exact 4501 / linked_exact 4501`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27 sections, `.so`
  exactly 2,187,328 B, `.ARM.extab` still 0x26294 (4 B short, unchanged),
  `.rodata` 0x41c27 (unchanged).

## 1. The fix: restore the original left-associative grouping

The remaining `sliders[2]` divergence (wave 116 item 4) was the y expression.
The wave-116 tree was the *faithful* source order

```cpp
(field_0x15c + (field_0x1c + rowHeight)) * 2 +
rowHeight * 3 + sliderRowHeight + listEntryWidth + field_0x20_top +
field_0xc_leftMargin + field_0x2fc
```

but clang 7's reassociate pass canonicalised it to
`(field_0x15c + field_0x1c + rowHeight) * 2 + ...`, and the resulting
register allocation also moved `sliders` from r9 to r4 (saving 2 B and
deleting the literal-pool alignment `nop`), which is why every wave-116
mask-only attempt lost 4 B and collapsed the score.

The winning shape writes the *original disassembly's* grouping explicitly and
puts the `& 0x7fffffff` reassociation barrier on the inner `(field_0x1c +
rowHeight)` pair:

```cpp
(int) ((float) (((Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top) +
                 this->listEntryWidth + Globals::layout->field_0x2c_rowHeight * 3 +
                 Globals::layout->field_0x2fc) +
                this->sliderRowHeight +
                (this->field_0x15c + ((Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight) & 0x7fffffff)) * 2) +
       (Globals::iPadHD ? 19.6875f : (Globals::iPadLarge ? 28.0f : 14.0f))));
```

The mask is optimised away (no `bic`), but it pins
`field_0x15c + (field_0x1c + rowHeight)` so the reassociate pass cannot fold
`rowHeight` into the `*3` term. The explicit left grouping then reproduces the
original's `add.w r2, r3, lr` / `add r2, r6` / `add r2, r6` chain. The final
operand-order detail is `field_0xc_leftMargin + field_0x20_top` (not the
reverse): with `top + leftMargin` the block still differed by one
`add.w r2, lr, r3` vs `add.w r2, r3, lr`. Swapping the two source operands
fixed that last instruction.

Measured (ctor):

| variant | size | fuzzy |
|---|---:|---:|
| wave-116 committed | 9600 | 99.036145% |
| original source order + inner mask (S3) | 9592 | 94.289589% |
| original grouping, no mask (S1) | 9592 | 93.897845% |
| original grouping + inner mask (G2) | 9600 | 99.638554% |
| G2 + `leftMargin + top` operand swap (committed) | 9600 | **99.668675%** |
| `rowTerm` local instead of mask (S6) | 9596 | 95.556560% |
| mask whole `(f15c+rowHeight)` term (S4) | 9600 | 99.036145% |
| mask `f15c` instead (S5) | 9600 | 99.036145% |
| `(unsigned)` cast on inner term (S7) | 9600 | 99.006024% |

## 2. Remaining ctor differences (3 regions)

1. Mission block `0x122da6` (~6): `char row2 = row + 2` sinks past `getText`
   and emits `uxtb`; the original materialises `int row2 = row + 2` before the
   call and moves it to r3 at the call (`adds r4,r5,#2` ... `mov r3,r4`).
   Every legal `int`/`long long`/`const`/address-taken/`__builtin_assume`/
   `volatile`/comma/extra-use variant either sinks identically (9596 B,
   94.73%) or costs bytes (volatile: 9604 B, 94.10%). The wave-115
   `asm "+r"` target remains the only known way to pin it.
2. `loadPreviewRecords` inlined iPad y-subtraction `0x12339e` (4):
   original `(yBase - buttonRowGap) - rowHeight`, ours the reverse
   `(yBase - rowHeight) - buttonRowGap`. Split statements, explicit parens,
   `-rowHeight`, `(unsigned)` product, and masks on `buttonRowGap`/`rowHeight`
   all produce identical code or +4 B (mask survives as `bic`).
3. `scrollExtraButton` `0x123e44` (1): the `add r0, r3`
   (`+ sliderRowHeight` inside the `*2` group) is scheduled before
   `str r2,[sp,#4]`; the original schedules it after `add r2, r4`. Pairing is
   right, only the schedule differs. Moving `sliderRowHeight` after the mask,
   swapping `sliderRowHeight`/`sliderRowHeight/2`, a named `scrollExtraV`
   local, and hoisting the `*2` group all fail to move it (the named local
   grows the function to 9632 B).

## 3. OnTouchBegin (98.092643%, unchanged)

12 hunks, all pure register renames:
* `0x129d4a`/`0x129fd2`: original `ldr.w ip,[sl,#340]` + `add r3,ip`; ours
  `ldr.w r4` + `add r3,r4` (case-8 `listEntryWidth`).
* `0x12a1e0`: `r2`/`r3`, `r6`/`r4`, `r4`/`r5` in the case-15 scrollbar block.
* `0x12a2xx`: `ldr r6,[r2,#32]` vs `ldr r4,[r6,#32]`.

The wave-114/115/116 conclusion stands: the baseline
`Layout &layout = *Globals::layout;` with two `Globals::layout->` accesses is
the best shape; swapping the case-15 `&&` operands, hoisting
`int barIndex = this->field_0x1e0`, and using `Globals::layout->` for
`field_0x20_top` all regress.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6518
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN15MenuTouchWindowC1Ei
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```
