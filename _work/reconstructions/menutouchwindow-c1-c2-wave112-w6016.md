# MenuTouchWindow C1/C2 wave 112 (w6016): loop-scoped `i` + id-first tail
# (95.6928% -> 96.1602%, 9600 B)

Date: 2026-11-05. Worktree `wt/w6016`, branch `wave112-w6016`, base
`5d98bd391`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before:** 9600 B, 3320 instructions, exact difflib ratio
  **95.692771%**, **110** diff hunks.
* **After:** 9600 B, **3321** instructions (one extra `and.w`), exact
  difflib ratio **96.160217%**, **101** diff hunks. C2 scores the same.
* Two-line source change in the language-selection loop
  (`src/game/ui/MenuTouchWindow.cpp`):

```diff
-    unsigned int i;
...
-        for (i = 0; (i & 0xf) != 10; i++) {
+        for (unsigned int i = 0; (i & 0xf) != 10; i++) {
...
             TouchButton *language = new TouchButton(
                 *Globals::gameText->getText(MenuTouchWindow_languageTextIds[i]), 0, x, y, bw, 17, 4,
                 ctorFont, spacing);
+            language->id = MenuTouchWindow_languageIds[i];
             if (special)
                 language->fontId = *specialFontPtr;
             ArrayAdd(language, *this->buttonsB0);
-            language->id = MenuTouchWindow_languageIds[i];
...
-    for (i = 0; i < this->scrollSlots->size(); i++) {
+    for (unsigned int i = 0; i < this->scrollSlots->size(); i++) {
```

* Gate before and after: `byte_exact 4500 / linked_exact 4500`,
  `size 4524/4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS**. Fuzzy-only change, so no baseline update.
  No section/extab change: 27 sections, `.so` exactly 2,187,328 B,
  `.text` 0x1337c8, `.ARM.extab` 0x26298.

## 1. Mechanism (measured)

The original's loop tail is

```
123474: ldr   r0, [pc, #616]     ; languageIds
123476: cmp.w sl, #9             ; special
12347a: ldr.w r8, [sp, #80]      ; i
12347e: add   r0, pc
123480: ldr.w r0, [r0, r8, lsl #2]
123484: mov.w r1, r0, asr #31    ; 4-byte form: flags stay live
123488: strd  r0, r1, [r6]       ; language->id
12348c: ittt  eq
12348e: ldreq r0, [sp, #76]      ; specialFontPtr
123490: ldreq r0, [r0, #0]
123492: streq r0, [r6, #8]       ; language->fontId
...
1234a2: add.w r8, r8, #1
1234a6: ldr.w r9, [sp, #92]      ; &Globals::w
1234aa: cmp.w r8, #10
1234ae: bcc.w 123346
```

i.e. the `id` store precedes the `if (special) fontId` store and the
`ArrayAdd`, and the `cmp.w sl,#9` flags are still live across the id
store, so clang emits the 4-byte `mov.w r1,r0,asr#31` instead of the
2-byte `asrs r1,r0,#31`.

Rewriting the tail in that order alone (`T1`) reproduces the local tail
byte-for-byte but shifts the stack slots (i lands at `[sp,#104]`
instead of `[sp,#80]`, `&Globals::w` at `[sp,#88]` instead of
`[sp,#92]`) and grows the function to 9604 B: **89.7%**.

Deleting the function-scope `unsigned int i;` and declaring `i` inside
each of the two loops restores the stack-slot coloring (`i` at
`[sp,#80]`, `&Globals::w` at `[sp,#92]`, `y`/`x` at `[sp,#84]`/
`[sp,#88]`) and the 9600-byte size. The loop body then also reproduces
the original's r8/r9 split:

```
123400: mov   r9, r8             ; copy of i
12341a: ldr.w r8, [r2]           ; langValue -> r8
12343c: str.w r9, [sp, #80]      ; spill i
123442: ldr.w r1, [r0, r9, lsl #2]  ; languageTextIds[i]
12347a: ldr.w r8, [sp, #80]      ; reload i
```

The `and.w r0,r8,#15; cmp r0,#10; bne.w` backedge is still the wave-109
mask (the plain `i < 10` range fact makes clang fold the two value
selects into an address select and moves the loop counter to `fp`;
9544-9548 B / 72.1-72.7%). The extra `and` instruction is exactly the
one-instruction surplus (3321 vs 3320); it is compensated in size by
the deleted `ldr r4,[sp,#96]` in the cinematic-button block.

## 2. Measured table (official verifier, full TU)

`orig` is always 9600 B / 3320 instructions. `fz` is the exact difflib
ratio from `tools/verify/verify.py`'s normalised stream.

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| **wave-110 baseline (`(i & 0xf) != 10`, id last)** | **9600** | **3320** | **95.692771** | **110** |
| **id-first tail + loop-scoped i (this pass)** | **9600** | **3321** | **96.160217** | **101** |
| id-first tail only | 9604 | 3321 | 89.7455 | 257 |
| id-first tail + braces around `if` | 9600 | 3321 | 96.160217 | 101 |
| loop-scoped i only | 9600 | 3320 | 95.692771 | 110 |
| id-first + `unsigned int idx = i` local | 9604 | 3321 | 89.7455 | 257 |
| id-first + mission-block `row2` hoist | 9600 | 3321 | 96.160217 | 101 |
| `(i & 0xf) < 10` backedge (bcc, `and #14`) | 9600 | 3321 | 96.160217 | 101 |
| natural `GameText *cinematicText = Globals::gameText;` | 9600 | 3321 | 96.160217 | 101 |
| direct `Globals::gameText` both buttons | 9592 | 3319 | 94.0 | 142 |
| exact Ghidra slider[2] tree | 9600 | 3321 | 95.8 | 110 |
| `rowHeight*3 + rowHeight*2` slider[2] split | 9600 | 3321 | 96.0 | 102 |
| `0x2fc` first in slider[2] | 9600 | 3321 | 96.1 | 101 |

## 3. Remaining residual (101 hunks)

* The language-loop backedge is still the 3-instruction mask
  (`and/cmp/bne`) instead of the original 2-instruction
  `cmp #10; bcc`; plain `i < 10` is unusable (address select + `fp`
  loop counter + stack-slot shift).
* iPad y-subtraction order: original subtracts `buttonRowGap` then
  `field_0x30`; ours subtracts `field_0x30` then `buttonRowGap`
  (both narrow, same size; pure scheduling).
* Cinematic-button block (`~1600`): ours keeps `Globals::gameText` in
  `r9` across both `new TouchButton` calls and inserts one
  `mov sl,r4`; the original reloads `[sp,#96]` and uses `r5`/`r6`
  the other way round (post-RA coloring).
* Slider[2]/slider[3] `setPosition` y trees: the original loads the
  layout fields in a different order and groups the sum as
  `0x2fc + (0xc + 0x20 + 0x154 + 3*0x2c) + 0x164 + (0x15c + 0x1c + 0x2c)*2`;
  every tested faithful tree scores 95.7-96.1, i.e. below the current
  left-leaning grouping.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6016
TMPDIR=/dev/shm/w6016/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w6016/tmp cmake --build cmake-build-match --target check -j6
TMPDIR=/dev/shm/w6016/tmp cmake --build cmake-build-match --target drift -j6
```
