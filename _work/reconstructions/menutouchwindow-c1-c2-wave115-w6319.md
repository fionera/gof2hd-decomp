# MenuTouchWindow C1/C2 wave 115 (w6319): mission-block scheduling is a legal dead end
# (97.244391% stays best; the `asm "+r"` shape target is 97.364855%)

Date: 2026-11-05. Worktree `wt/w6319`, branch `wave115-w6319`, base `b847d5d46`
(wave-114 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Targets:
`_ZN15MenuTouchWindowC1Ei` / `_ZN15MenuTouchWindowC2Ei` (weak alias, same
address/size/score) and `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`.

## TL;DR

* **No legal source change improved either target this wave.** The committed
  tree is unchanged (`src/game/ui/MenuTouchWindow.cpp` untouched).
* Ctor: **97.244391%**, 66 hunks, 9600 B, 3320/3321 instructions (unchanged).
* OnTouchBegin: **98.092643%**, 12 hunks, 1896 B, 734/734 (unchanged).
* Gate: `byte_exact 4500 / linked_exact 4500`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27 sections, `.so`
  exactly 2,187,328 B (no section/extab change).
* The only measured improvement found is the **forbidden** `asm` shape
  target below (+0.120464 pts, -3 hunks). It is recorded so a future legal
  rewrite can aim at it.

## 1. The one remaining structural divergence: the mission block

First divergence is still the `mission == 154 || 157 || 158` block
(`0x122d9e`). Original (real VMA; `C1` base is `0x12299c`):

```
122d9e  ldr.w  r0, [r8]        ; Globals::gameText
122da2  movw   r1, #395
122da6  adds   r4, r5, #2      ; row2 = row + 2  (int, in r4)
122da8  blx    GameText::getText
122dac  add    r5, sp, #372    ; String temp in r5
...
122dc2  mov    r0, sl
122dc4  movs   r1, #18
122dc6  mov    r2, r5
122dc8  mov    r3, r4          ; row2
122dca  blx    addButton
```

Committed tree (`char row2 = row + 2;`):

```
122d9e  ldr.w  r0, [r8]
122da2  movw   r1, #395
122da6  blx    GameText::getText
122daa  add    r4, sp, #372    ; String temp in r4
...
122dc0  adds   r0, r5, #2      ; row2 sunk past the calls
122dc6  uxtb   r3, r0          ; char truncation
```

Both blocks are **54 bytes**, so the function size (9600 B) and every branch
displacement match; the 3 hunks are pure placement/register allocation.
The original's `row2` is an `int` (no `uxtb`) materialised *before*
`getText`; ours is a `char` materialised at the use.

An `int row2 = row + 2;` statement puts `%row2 = add nsw i32 %row, 2`
before the calls in the LLVM IR (verified with `-emit-llvm -S`), but
clang 7's machine sinker always rematerialises it after the `String`
ctor. That costs 2 bytes in the block *and* removes the 2-byte literal-pool
alignment `nop` at `0x122f72`, giving **9596 B** and a 4-byte shift of every
downstream branch target -> 163 hunks / 93.206808%.

### The forbidden shape target

Adding `asm volatile("" : "+r"(row2));` after the `int` add pins the value
into a register and reproduces the original's placement
(`adds r4, r5, #2` before `getText`, String temp in `r5`):

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| **committed (`char row2`)** | **9600** | **3321** | **97.244391** | **66** |
| `int row2` + `asm volatile("" : "+r"(row2))` (forbidden) | 9600 | 3321 | 97.364855 | 63 |

The `asm` variant's instruction order is not perfect either: it emits
`adds r4,r5,#2 ; movw r1,#395 ; ldr.w r0,[r8]` where the original has
`ldr.w r0,[r8] ; movw r1,#395 ; adds r4,r5,#2` (hence 63, not 60, hunks).
Everything else in the function is byte-identical to the committed tree.

## 2. Measured table — ctor mission block (all legal variants)

Every row replaces only the 5-line mission block on top of the committed
tree; `try.sh` rebuilds `link_so` and scores with `tools/verify`.

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| **committed `char row2`** | **9600** | **3321** | **97.244391** | **66** |
| `unsigned char` / `uint8_t` / `char16_t` / `unsigned short row2` | 9600 | 3321 | 97.244391 | 66 |
| `(char)(row+2)` / `(unsigned char)(row+2)` | 9600 | 3321 | 97.244391 | 66 |
| `char row2 = row; row2 += 2;` | 9600 | 3321 | 97.244391 | 66 |
| `int row2 = (char)(row + 2);` (int type, truncated) | 9600 | 3321 | 97.244391 | 66 |
| `int` / `unsigned` / `short` / `long` / `long long` / `size_t` / `int32_t` / `uint32_t` / `const int` | 9596 | 3319 | 93.206808 | 163 |
| inline `row + 2` (no local) | 9596 | 3319 | 93.206808 | 163 |
| `int row2` declared before the `if` (hoisted) | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row; row2 += 2;` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row - -2;` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 1 + 1;` | 9596 | 3319 | 93.206808 | 163 |
| `int &rowRef = row; int row2 = rowRef + 2;` | 9596 | 3319 | 93.206808 | 163 |
| `int two = 2; int row2 = row + two;` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; row = 0;` (kill row) | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; row = row2 - 2;` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; if (row2) addButton(...)` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; if (row2 < 0) __builtin_unreachable();` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; __builtin_assume(row2 > row / != 0 / != 12345 / < 1000 / >= 0)` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; __builtin_expect(row2, row2 / 1)` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; if (__builtin_expect(row2 == 0, 0)) {}` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2;` + `std::atomic_signal_fence(seq_cst)` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2;` + `String &text = *getText(395);` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2;` + `String *text = getText(395);` | 9596 | 3319 | 93.206808 | 163 |
| `int row2` + `getText(395 + row2 - row2)` | 9596 | 3319 | 93.206808 | 163 |
| `int row2` + last arg `row2 - row2` / `row2 - row - 2` / `row - row` | 9596 | 3319 | 93.206808 | 163 |
| `int row2` + last arg `row2 > row ? 0 : 0` | 9596 | 3319 | 93.206808 | 163 |
| `int row2 = row + 2; row2 &= 0x7fffffff;` | 9600 | 3320 | 96.987952 | 76 |
| `volatile int row2 = row + 2;` | 9600 | 3321 | 96.913115 | 78 |
| `int row2` + `String(*getText(395), row2 < 0)` | 9596 | 3319 | 93.206808 | 163 |
| `int row2` + `String(*getText(395), row2 > row)` | 9596 | 3319 | 93.116433 | 163 |
| `int row2` + `String(*getText(395), row2 == 0)` | 9604 | 3322 | 92.923818 | 170 |
| `char row2` + `String(*getText(395), row2 == 0)` | 9604 | 3322 | 92.863595 | 171 |
| `char row2` + `String(*getText(395), (row2 & 0x80) != 0)` | 9600 | 3320 | 96.897590 | 76 |
| `char row2` + `String(*getText(395), row2 > 100)` | 9608 | 3325 | 92.972160 | 170 |
| `String text = *getText(395); addButton(18, text, row2, ...)` (named String) | 9620 | 3330 | 91.669173 | 191 |
| `switch (mission)` instead of `||` chain | 9600 | 3321 | 97.214275 | 67 |
| `(mission-154)<5 && (1<<..)&0x19` bitmask | 9592 | 3319 | 92.092183 | 184 |
| `mission >= 154 && <= 158 && != 155 && != 156` | 9588 | 3318 | 90.840615 | 165 |

`__builtin_assume` lowers to **nothing** in this clang 7 TU (no
`llvm.assume` in the emitted IR), which is why none of the assume variants
constrain the sinker.

## 3. Measured table — other ctor regions (all dead ends)

### Cinematic-button block (`GameText *cinematicText`)

The original reloads `&Globals::gameText` from `[sp,#96]` before the 496
`getText`; ours keeps the value in `r9` across the first `TouchButton`
ctor and inserts `mov sl, r4` (hunks 27-69). All legal spellings tried:

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| **committed `*(cinematicText = Globals::gameText)`** | **9600** | **3321** | **97.244391** | **66** |
| `GameText *cinematicText = Globals::gameText;` (separate init) | 9600 | 3321 | 97.214275 | 66 |
| `cinematicText = Globals::gameText;` on its own line | 9600 | 3321 | 97.214275 | 66 |
| `GameText *const cinematicText = Globals::gameText;` | 9600 | 3321 | 97.214275 | 66 |
| first use `Globals::gameText`, second local | 9588 | 3316 | 93.640747 | 94 |
| first local, second `Globals::gameText` | 9592 | 3318 | 95.088882 | 107 |
| no local (both `Globals::gameText`) | 9592 | 3318 | 95.088882 | 107 |

`-emit-llvm` confirms the local makes the second `getText` reuse the first
`load`; removing it emits a fresh `load` (matching the original) but shrinks
the code by 8 B.

### Slider `setPosition` y-trees (`(*this->sliders)[2]`, hunks 117-123)

Original groups `(field_0x15c + (field_0x1c + rowHeight)) * 2` and
`(field_0x20_top + leftMargin + listEntryWidth + 3*rowHeight)`; ours
reassociates to `(field_0x15c + rowHeight) + field_0x1c` and
`(sliderRowHeight + rowHeight)`. All 10 hand-written association/order
variants (including the faithful Ghidra tree) measured 96.79-97.24%
(72-76 hunks) or were neutral; the best is the committed 97.244391 / 66.

### `scrollExtraButton` doubled term (hunks 83-85)

Original pairs `(field_0x15c + rowHeight)`; ours pairs
`(sliderRowHeight + rowHeight)`. All **24 permutations** of the flat sum
`field_0x2fc + field_0x15c + rowHeight + sliderRowHeight` were built:
12 give 97.244391 / 66 (identical code) and 12 give 97.184159 / 68.
No permutation reaches the original pairing.

### iPad y-subtraction (hunks 11-13)

Original `(yBase - buttonRowGap) - rowHeight`; ours
`(yBase - rowHeight) - buttonRowGap`. Four source orders / explicit
parens all produce byte-identical code (97.244391 / 66): LLVM
reassociates and canonicalises.

### Literal-pool displacements and cleanup tail

Hunks 15/17/21/23/79/81/87/93/109/111/113/115 are `ldr [pc,#N]`
displacement consequences of the pool placement; hunk 131 is the
exception-cleanup `b.n` cascade (`loc_2566` vs `loc_2560`). Neither is
reachable from source shape.

## 4. OnTouchBegin (98.092643%, 12 hunks)

Re-probed this wave; no improvement. The 12 hunks are pure register
renames (`ip` vs `r4` in the case-8 first condition; `r2`/`r3`, `r6`/`r4`,
`r4`/`r5` in the case-15 scrollbar block; `r5` vs `r4` in the trailing
Height/Width comparisons). The wave-114 table still stands (baseline
`Layout &layout = *Globals::layout;` with two `Globals::layout->` accesses
is best; removing `touchA`/`touchB`, `previous`, the layout local, or
adding width/height locals all regress to 74.8-90.6%).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6319
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN15MenuTouchWindowC1Ei
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Expected: ctor/C2 97.244391% (66 hunks, 9600 B, 3320/3321),
OnTouchBegin 98.092643% (12 hunks, 1896 B, 734/734), gate byte 4500 /
linked 4500, size 4524, data 843, imports 207, lint CLEAN, drift 0,
file size 2,187,328 B.
