# MenuTouchWindow C1/C2 wave 116 (w6418): faithful cinematic block + reassociation masks

Date: 2026-11-05. Worktree `wt/w6418`, branch `wave116-w6418`, base `d672d349f`
(wave-115 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Targets:
`_ZN15MenuTouchWindowC1Ei` / `_ZN15MenuTouchWindowC2Ei` (weak alias, same
address/size/score) and `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`.

## TL;DR

* Ctor: **97.244391% -> 99.036145%** (66 -> 45 differing instructions,
  9600 B unchanged, 3320/3320 normalised instructions).
* OnTouchBegin: **98.092643%** unchanged (15 differing instructions, all
  register renames; no legal variant found this wave either).
* Gate: `byte_exact 4501 / linked_exact 4501`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27 sections, `.so`
  exactly 2,187,328 B, `.ARM.extab` still 0x26294 (4 B short, unchanged),
  `.rodata` 0x41c27 (unchanged from wave 115).

## 1. The key step: remove the `cinematicText` local (faithful shape)

Wave 115 found that the original reloads `Globals::gameText` before *each* of
the 497/496 `getText` calls, while the committed tree kept the pointer in a
local (`GameText *cinematicText`). Removing the local makes the cinematic
block byte-identical, but the function shrinks (nocin baseline was 9592-9596 B
depending on the other blocks) and the score collapses because every later
branch target shifts.

The fix is to recover the missing bytes in the *other* reassociated blocks so
the function is 9600 B again with the faithful cinematic shape. Once the size
is restored, the score jumps far above the old "wrong-source but aligned"
97.244391%.

## 2. Reassociation barriers (`& 0x7fffffff`)

LLVM 7's reassociate pass canonicalises integer sums the same way regardless of
source order. In several y-coordinate expressions the original pairs operands
in an order our tree cannot produce. A `& 0x7fffffff` on a sub-sum acts as a
reassociation barrier; clang 7 then *optimises the AND away* (the values are
known non-negative in context), so it costs no instructions but pins the
pairing. This is ordinary C++ (a sign-bit mask on a coordinate) and adds no
asm/attribute/global.

Measured blocks:

| block | original pairing | committed pairing | fix |
|---|---|---|---|
| `scrollExtraButton` y | `(f2fc + (f15c + rowHeight)) + sliderRowHeight` | `(f2fc + sliderRowHeight) + (f15c + rowHeight)` | mask `(f15c+rowHeight)` (+2 B, restores the 2-byte pool-alignment `nop` at 0x124026) |
| `sliders[2]` y | `(f15c + (f1c + rowHeight))*2 + rowHeight*3 + sliderRowHeight + ...` | `(f15c + f1c)*2 + rowHeight*5 + ...` | reorder to the faithful tree (+4 B) |
| `sliders[4]` y (else) | `(f15c + f2fc) + sliderRowHeight` | `(f15c + sliderRowHeight) + f2fc` | mask `(f15c+f2fc)` (size-neutral) |
| `scrollUpButton` y | `(leftMargin + top) + listEntryWidth` | `listEntryWidth + (leftMargin + top)` | reorder to `(leftMargin + top) + listEntryWidth` (size-neutral) |

Together these restore 9600 B with the faithful cinematic block and leave only
45 differing instructions.

## 3. Measured table (ctor)

`try.sh` rebuilds `link_so` and scores with `tools/verify`.

| variant | size | fuzzy | differing |
|---|---:|---:|---:|
| committed (wave 115) | 9600 | 97.244391% | 66 |
| nocin only | 9592 | 95.014309% | 163 |
| nocin + `s2a` (slider[2] tree) | 9596 | 95.0% | ~ |
| nocin + `s2a` + scroll mask (`m1`) | 9600 | 98.915663% | 52 |
| + slider[4] mask (`d`) | 9600 | 99.006024% | 46 |
| + scrollUpButton reorder (`u2`) | 9600 | **99.036145%** | **45** |
| committed cinematic + all four fixes | 9608 | 94.1% | - |

The committed cinematic block is 8 B larger than the faithful one, so it
overshoots to 9608 B once the other blocks are fixed.

## 4. Remaining ctor differences (45 instructions)

1. Mission block `0x122da6` (5): `char row2` sinks past `getText` and emits
   `uxtb`; original materialises `int row2 = row + 2` before the call.
   `int row2` alone removes the 2-byte literal-pool `nop` at 0x122f72 (-4 B).
   `int row2 = (row + 2) & 0x7fffffff` keeps 9600 B but scores 98.8%.
2. `loadPreviewRecords` inlined iPad y-subtraction `0x12339e` (4):
   original `(yBase - buttonRowGap) - rowHeight`, ours the reverse. Source
   reorder / explicit parens / mask on `buttonRowGap` all produce identical
   code (or +4 B).
3. `scrollExtraButton` `0x123e44` (3): the `add r0, r3` (`+ sliderRowHeight`)
   is scheduled before `str r2,[sp,#4]`; original schedules it after
   `add r2, r4`. Pairing is now right; only the schedule differs.
4. `sliders[2]` block `0x124636` (19) + tail (2): the faithful tree is 4 B
   shorter than the `s2a` tree that keeps 9600 B. `s2a` keeps the size but
   accumulates in a different order (`(f15c+f1c)+rowHeight` instead of
   `f15c+(f1c+rowHeight)`, and `f2fc` folded at the end). Masks on
   `(f1c+rowHeight)` fix the pairing but cost 4 B; masking the whole `v` term
   adds a surviving `bic.w` (+4 B) and scores 98.96%.

## 5. OnTouchBegin (98.092643%, 15 differing, unchanged)

All 15 hunks are pure register renames:
* `0x129d4a`/`0x129fd2`: original `ldr.w ip,[sl,#340]` + `add r3,ip`; ours
  `ldr.w r4` + `add r3,r4` (case-8 `listEntryWidth`).
* `0x12a1e0`: `r2`/`r3`, `r6`/`r4`, `r4`/`r5` in the case-15 scrollbar block.
* `0x12a2xx`: `ldr r6,[r2,#32]` vs `ldr r4,[r6,#32]`.

Tried this wave (all regress):
* swap the two `&&` operands of the case-15 scrollbar test: 89.1% / 1892 B.
* hoist `int barIndex = this->field_0x1e0`: 90.7% / 1892 B.
* use `Globals::layout->field_0x20_top` instead of `layout.field_0x20_top`:
  90.3% / 1892 B.

The wave-114/115 conclusion stands: the baseline `Layout &layout =
*Globals::layout;` with two `Globals::layout->` accesses is the best shape.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6418
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN15MenuTouchWindowC1Ei
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Expected: ctor/C2 99.036145% (9600 B, 3320/3320), OnTouchBegin 98.092643%
(1896 B, 734/734), gate byte 4501 / linked 4501, size 4524, data 843,
imports 207, lint CLEAN, drift 0, file size 2,187,328 B, 27 sections,
`.ARM.extab` 0x26294.
