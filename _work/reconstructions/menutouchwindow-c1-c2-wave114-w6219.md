# MenuTouchWindow C1/C2 wave 114 (w6219): signed loop bound restores `bcc` backedge
# (97.2143% -> 97.2444%, 9600 B)

Date: 2026-11-05. Worktree `wt/w6219`, branch `wave114-w6219`, base
`d8557f7e2` (wave-113 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size, same score).
Secondary target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` was re-probed;
no improvement found (98.092643% stays best).

## TL;DR

* **Ctor before:** 9600 B, 3320 orig / 3321 ours instructions, exact
  difflib ratio **97.214275%**, **67** diff hunks.
* **Ctor after:** 9600 B, 3320 / **3321** instructions, exact difflib
  ratio **97.244391%**, **66** diff hunks. C2 scores identically.
* One-line source change in `src/game/ui/MenuTouchWindow.cpp`:

```diff
-        for (unsigned int i = 0; i != 10; i++) {
+        for (unsigned int i = 0; (int) i < 10; i++) {
```

* **OnTouchBegin:** unchanged at 1896 B, 734 / 734 instructions,
  **98.092643%**, 12 hunks (all pure register-allocation renames).
* Gate after: `byte_exact 4500 / linked_exact 4500`, `size 4524/4524`,
  `data 843/843`, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.
  Fuzzy-only change, so no baseline update. No section/extab change:
  27 sections, `.so` exactly 2,187,328 B, `.text` 0x1337c8 (size- and
  base-exact), `.ARM.extab` 0x26298 (size- and base-exact), `.rodata`
  0x41c27 before and after (its 0xC0 tail beyond the original's
  0x41b67 is pre-existing and falls inside the 0x20cba0 alignment
  padding, so the file size is unchanged).

## 1. Mechanism (measured)

The wave-113 tree used `for (unsigned int i = 0; i != 10; i++)`, which
clang lowers to the 2-instruction equality backedge

```
cmp.w r8, #10
bne.w <loop head>
```

The original instead ends the loop with the range backedge

```
cmp.w r8, #10
bcc.w <loop body>          ; unsigned "below"
```

Writing the bound as a *signed* comparison `(int) i < 10` reproduces the
`bcc` backedge exactly (verified: instruction 999/1000 of the loop
region is identical to the original, and the whole function's first
divergence moves from the loop to the mission block). `bcc` is emitted
because clang knows `i` is non-negative, so the signed comparison is
canonicalised to the unsigned predicate while keeping the `<` form
(which, unlike `!= 10`, gives the loop a finite trip count).

Crucially this is a *different* rewrite from plain `i < 10` / `i <= 9` /
`i < 10u` / `i < (unsigned) 10`: those four all collapse the loop to
9552 B / 3331 instructions / **72.680800%** / 489 hunks. The `(int)`
cast is load-bearing.

The `char row2` local and the `__builtin_assume(i < 100)` inside the
`if (special)` branch from wave 113 remain required:

* `char`/`unsigned char row2` -> 9600 B / 97.244391%; `int`/`short`/
  inline `row+2` -> 9596 B / 93.206808% (frame loses 4 bytes).
* Removing the assume, or hoisting it out of `if (special)` to the loop
  top / just after the `if`, drops to 9552 B / 72.680800%. Its constant
  value is irrelevant (10..1000 all identical).

## 2. Measured table (official verifier, full TU)

All rows are on top of the pristine wave-113 tree unless noted; the
winning row is the one committed.

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| wave-113 baseline (`i != 10`, `char row2`) | 9600 | 3321 | 97.214275 | 67 |
| **`(int) i < 10` (committed)** | **9600** | **3321** | **97.244391** | **66** |
| `(int) i <= 9` | 9600 | 3321 | 97.244391 | 66 |
| `int i` loop (`for (int i = 0; i < 10; i++)`) | 9552 | 3331 | 72.680800 | 489 |
| plain `i < 10` / `i <= 9` / `i < 10u` / `i < (unsigned) 10` | 9552 | 3331 | 72.680800 | 489 |
| `(int)i<10` + `GameText *const cinematicText` | 9600 | 3321 | 97.214275 | 66 |
| `(int)i<10` + `GameText *cinematicText` (plain local) | 9600 | 3321 | 97.184159 | 67 |
| `(int)i<10` + `GameText *const &` | 9600 | 3321 | 97.214275 | 66 |
| `(int)i<10` + `GameText *&` | 9592 | 3318 | 95.058753 | 108 |
| `(int)i<10` + `GameText *volatile` | 9596 | 3320 | 94.608434 | 118 |
| `(int)i<10` + direct `Globals::gameText` (no local) | 9592 | 3318 | 95.088882 | 107 |
| `(int)i<10` + local first / global second | 9592 | 3318 | 95.088882 | 107 |
| `(int)i<10` + global first / local second | 9588 | 3316 | 93.640747 | 94 |
| `(int)i<10` + `int row2` / inline `row+2` | 9596 | 3319 | 93.206808 | 163 |
| `(int)i<10` + `short row2` | 9596 | 3319 | 93.206808 | 163 |
| `(int)i<10` + `unsigned char row2` | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + `(char)(row+2)` / `(unsigned char)(row+2)` | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + `char row2 = row; row2 += 2;` | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + `__builtin_assume(i<N)`, N in 10..1000 | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + assume hoisted to loop top / after `if` | 9552 | 3331 | 72.680800 | 489 |
| `(int)i<10` + assume before `ctorFont` (inside `if`) | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + assume in `else` branch | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + `special = i == 8 \|\| i == 9` | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + `special = (i & ~1u) == 8` | 9608 | 3324 | 93.136665 | 162 |
| `(int)i<10` + `special = (i & 0xfffffffeu) == 8` | 9608 | 3324 | 93.136665 | 162 |
| `(int)i<10` + `special = (i \| 1u) == 9` | 9600 | 3321 | 97.244391 | 66 |
| `(int)i<10` + mission `switch` | 9600 | 3321 | 97.214275 | 67 |
| `(int)i<10` + mission bitmask `(mission-154)<5 && (1<<..)&0x19` | 9592 | 3319 | 92.092183 | 184 |
| `(int)i<10` + mission range `>=154 && <=158 && !=155 && !=156` | 9588 | 3318 | 90.840615 | 165 |
| `(int)i<10` + slider[2] faithful Ghidra tree | 9600 | 3321 | 96.852884 | 76 |
| `(int)i<10` + slider[2] tree + scrollExtraButton reorder | 9592 | 3318 | 94.877975 | 109 |
| `(int)i<10` + scrollExtraButton flat/gap-first variants | 9600 | 3321 | 97.184159 | 68 |
| cinematic y `308-first` (first button) | 9600 | 3321 | 97.184159 | 66 |
| cinematic y `308-first` (both buttons) | 9600 | 3321 | 97.123927 | 67 |

OnTouchBegin (secondary target, all re-probed with the winning ctor
loop in place; ctor score unaffected):

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| **baseline (committed)** | **1896** | **734** | **98.092643** | **12** |
| case-7 loop `(int) i < (int) sliders->size()` | 1896 | 734 | 97.956403 | 13 |
| case-4 loop cast | 1896 | 734 | 97.956403 | 13 |
| case-15 loop cast | 1896 | 734 | 97.956403 | 13 |
| all three loop casts | 1896 | 734 | 97.683924 | 15 |
| `int inset` / `int entryWidth` / `int top` locals | 1892-1888 | 731-733 | 76.4-88.2 | 75-116 |
| y-term permutations of leftMargin/top/listEntryWidth | 1896 | 734 | 98.092643 (4) / 97.683924 (2) | 12/13 |
| `Layout *` / `const Layout &` / no layout local | 1896-1892 | 734-732 | 90.3-98.09 | 12-59 |
| `width` / `width`+`height` locals | 1888-1892 | 732 | 86.5-89.6 | 49-58 |
| swap the two scrollbarHit conditions | 1892 | 732 | 89.085948 | 55 |
| reversed `Globals::w - inset - width < x` | 1896 | 734 | 98.092643 | 12 |

## 3. Residual analysis (66 hunks, committed variant)

1. **Mission block (first divergence, 0x40c).** Original hoists
   `adds r4, r5, #2` *before* `blx getText` and passes `mov r3, r4`;
   ours sinks it and needs `uxtb r3, r0` (same size, different
   schedule). The original's `row2` is an `int` value, but every `int`
   spelling loses the 9600-byte frame (9596 B / 93.2%), so the narrow
   local is kept.
2. **iPad y-subtraction order** (`yBase - gap - height` vs ours
   `yBase - height - gap`). LLVM reassociates; every spelling measured
   identical.
3. **Cinematic-button block (~20 hunks).** Ours keeps
   `Globals::gameText` in `r9`/`sl` across the first `TouchButton`
   ctor, inserts `mov sl, r4`, and swaps r5/r6. The original reloads
   `[sp,#96]` before the 496 call. Every spelling that forces the
   reload shrinks the frame (9588-9592 B -> 93.6-95.1%).
4. **Slider `setPosition` y trees** (indices ~2495-2515,
   ~2621-2629). Original groups `(0x15c + 0x2c)` *before* adding
   `0x2fc` and `sliderRowHeight` inside the doubled term; ours computes
   them in a different order. Splitting/regrouping the doubled term
   gives 97.15-97.18 (worse); the faithful Ghidra slider[2] tree gives
   96.85.
5. **Literal-pool displacements** (indices ~1269, 1342, 1379, 1382,
   1781, 1815, 1856, 1960, 2441, 2467, 2479) — consequences of pool
   placement, not source shape.
6. **Exception cleanup tail** (indices 3263-3265): `b.n` targets
   `loc_2566` (orig) vs `loc_2560` (ours); delete-path register r5 vs
   r6.

OnTouchBegin's 12 hunks are all pure register-name differences
(`ip` vs `r4` in the case-8 first condition; `r2`/`r3` and `r6`/`r4`
swapped plus `r4`/`r5` swapped in the case-15 scrollbar block; `r5`
vs `r4` in the trailing Height/Width comparisons). No source-shape
variant found that changes the allocation without regressing other
blocks.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6219
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN15MenuTouchWindowC1Ei
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Expected after the committed edit: ctor/C2 97.244391% (66 hunks,
9600 B, 3320/3321 insns), OnTouchBegin 98.092643% (12 hunks, 1896 B,
734/734), gate byte 4500 / linked 4500, size 4524, data 843, imports
207, lint CLEAN, drift 0, file size 2,187,328 B.
