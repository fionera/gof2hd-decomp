# MenuTouchWindow C1/C2 pass 9: remove the three `__builtin_prefetch`
# size anchors with a faithful `buttonWidth` local and a consistent
# `cinematicText` local (94.772% -> 95.092%)

**Status:** code + analysis, 2026-11-03. Worktree `wt/w4715`, branch
`wave97-w4715`, base `9b58a39d5`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before:** 9600 B, 3317 instructions, exact difflib ratio
  **0.9477173421726683** (94.7717%), with three temporary
  `__builtin_prefetch(this)` size anchors (one before the `case 1` mission
  query, two in the language loop).
* **After:** 9600 B, 3322 instructions, exact difflib ratio
  **0.9509183980728696** (95.0918%). All three anchors are gone; the size is
  restored by two faithful source-shape changes.
* Two independent changes:
  1. **`int bw = this->buttonWidth;`** before the `new TouchButton(...)` in
     the language loop, passing `bw` as the 5th ctor argument. This makes
     `buttonWidth` live across `operator new` and `GameText::getText`, so the
     allocator gives it a callee-saved register. That in turn reproduces the
     original loop's `mov r9,r8` / `str.w r9,[sp,#80]` / `ldr.w r8,[sp,#80]`
     `i`-copy-and-spill structure and the 4-instruction
     `ittt eq; ldreq [sp,#76]; ldreq [r0]; streq` `fontId` store.
  2. **`cinematicText->getText(496)`** instead of
     `Globals::gameText->getText(496)` in the iPad touch-control preview. The
     local `GameText *cinematicText = Globals::gameText;` already existed and
     was already used for `getText(497)`; using it for the second call is the
     faithful source shape and adds the missing 8 bytes after the loop, which
     re-aligns every post-loop branch target.
* Together the two changes take the anchor-free function from 9588 B /
  0.9053 to 9600 B / 0.9509 — a strict improvement over the anchored
  baseline.

## 1. Before: the three anchors

Pass 8 (`menutouchwindow-c1-c2-pass8-optionbyte-prefetch-w4518.md`) left
this source shape:

```cpp
int mission = Globals::status->getCurrentCampaignMission();
__builtin_prefetch(this); // lint: arm_builtin
if (mission == 154 || mission == 157 || mission == 158) {
...
            ArrayAdd(language, *langList);
            __builtin_prefetch(this); // lint: arm_builtin
            __builtin_prefetch(this); // lint: arm_builtin
        }
```

The three `pld` instructions are 4+4+4 = 12 bytes of non-original code.
Removing them alone gives 9588 B and 0.9052568157855099; the original's loop
is 184 bytes and the reconstructed loop was 180 bytes, and the post-loop
code was 8 bytes short.

## 2. Change 1: the `buttonWidth` local

The loop body now reads:

```cpp
bool special = (i | 1) == 9;
unsigned int fontValue = Globals::font;
int langValue = Globals::fontLangSelect;
int spacing = Globals::Canvas->FontGetSpacing(
    special ? (unsigned int) langValue : fontValue);
int bw = this->buttonWidth;
TouchButton *language = new TouchButton(
    *Globals::gameText->getText(MenuTouchWindow_languageTextIds[i]), 0, x, y, bw, 17, 4,
    (i == 9 || i == 8) ? fontValue : (unsigned int) langValue, spacing);
language->id = MenuTouchWindow_languageIds[i];
if (special)
    language->fontId = *specialFontPtr;
ArrayAdd(language, *langList);
```

The original loads `this->buttonWidth` into `fp` *before* `operator new`
(0x12342c-0x12342e):

```
12342a  mov   r4, r0              ; spacing
12342c  ldr   r0, [sp, #72]       ; &this->buttonWidth
12342e  ldr.w fp, [r0]            ; buttonWidth -> fp
123432  movs  r0, #200
123434  blx   operator new
```

so the value is live across `operator new` and `GameText::getText`. Passing
`this->buttonWidth` directly (the old shape) lets clang rematerialise the
load *after* `getText` into a caller-saved register. The explicit local
forces the original's callee-saved live range.

The local also changes the whole loop's register allocation in the right
direction. With the local the loop is 184 bytes (67 instructions plus the
backedge), exactly the original's size, and it has the original's
`&fontLangSelect` spill and `i` copy/spill:

```
1233fa  orr.w   sl, r8, #1
1233fe  ldr     r0, [pc, #720]    ; &font
123400  cmp.w   sl, #9
123404  ldr     r1, [pc, #716]    ; &Canvas
123406  ldr     r2, [pc, #720]    ; &fontLangSelect
123408  add     r0, pc
12340a  add     r1, pc
12340c  add     r2, pc
12340e  ldr     r0, [r0, #0]
123410  ldr     r1, [r1, #0]
123412  ldr     r2, [r2, #0]
123414  ldr.w   fp, [r0]          ; font
123418  ldr     r0, [r1, #0]      ; Canvas
12341a  ldr     r6, [r2, #0]      ; lang
12341c  mov     r1, fp
12341e  str     r2, [sp, #76]     ; spill &fontLangSelect
123420  it      eq
123422  moveq   r1, r6
123424  blx     PaintCanvas::FontGetSpacing
123428  mov     r4, r0             ; spacing
12342a  ldr     r0, [sp, #72]
12342c  mov     r9, r8             ; save i
12342e  ldr.w   r8, [r0]           ; buttonWidth
123432  movs    r0, #200
123434  blx     operator new
123438  mov     r5, r0             ; button
12343a  ldr     r0, [pc, #672]     ; languageTextIds
12343c  str.w   r9, [sp, #80]     ; spill i
123440  add     r0, pc
123442  ldr.w   r1, [r0, r9, lsl #2]
...
123450  cmp.w   sl, #9
123454  it      eq
123456  moveq   r6, fp             ; select #2 (swapped)
...
12345c  strd    r0, r8, [sp]       ; ctor arg 5 = buttonWidth
...
12347a  ldr.w   r8, [sp, #80]     ; restore i
...
123490  ittt    eq
123492  ldreq   r0, [sp, #76]
123494  ldreq   r0, [r0, #0]
123496  streq   r0, [r5, #8]       ; language->fontId
```

The remaining difference from the original is a pure register permutation:
ours is `font=fp, lang=r6, button=r5, bw=r8, i=r9`; the original is
`font=r5, lang=r8, button=r6, bw=fp, i=r9`. The instruction *sequence* and
all spill/reload slots match. A 192-way declaration-order search, type
changes (`int`/`unsigned`/`long`/`const`/`register`), and
`__builtin_assume(i < 10)` / `__builtin_assume(bw != 0)` all leave the
allocation unchanged; moving `bw` before `special`/`font`/`lang` changes it
but loses badly (9640 B, 0.760).

Measurements (full TU rebuild, exact verifier):

| variant | size | ratio |
|---|---|---|
| anchors removed, no other change | 9588 | 0.9052568157855099 |
| + `int bw = this->buttonWidth;` | 9592 | 0.931647 |
| + `cinematicText->getText(496)` (both changes) | **9600** | **0.9509183980728696** |

## 3. Change 2: the `cinematicText` local

In the iPad touch-control preview the source already had:

```cpp
GameText *cinematicText = Globals::gameText;
this->cinematicBtnB = new TouchButton(
    *cinematicText->getText(497), ...);
this->cinematicBtnA = new TouchButton(
    *Globals::gameText->getText(496), ...);
```

The original caches `Globals::gameText` in a callee-saved register across
the two calls (`ldr r4,[sp,#96]` at 0x123ba8, then `ldr r0,[r4,#0]` for
`getText(496)`), i.e. it uses one local for both. Using the existing
`cinematicText` local for the second call:

```cpp
this->cinematicBtnA = new TouchButton(
    *cinematicText->getText(496), 0, Globals::w >> 1, ...);
```

adds exactly the 8 bytes the function was short after change 1 and restores
the original's post-loop branch-target alignment. On its own (without the
`bw` local) it measures 9592 B / 0.9087; the two changes only pay off
together.

## 4. Result

```
orig 9600 B @0x12299c  |  ours 9600 B @0x12299c
3320 orig instructions, 3322 ours
difflib ratio 0.9509183980728696   (report.json pct 95.1)
```

The function is still not byte-exact. The remaining diffs are the register
permutation in the language loop (§2), the matching stack-slot renumbering
(`[sp,#56]` vs `[sp,#60]`, `[sp,#68]` vs `[sp,#64]`, …), and the post-loop
`Globals::gameText` caching register choices. None of them are source-shape
differences any more; the loop instruction sequence and sizes match.

## 5. Gate

Before and after the full `check` is green and identical:

```
byte_exact   4500 / 4524
linked_exact 4500 / 4524
size_exact   4524 / 4524
stub_zero_size 0, missing 0, wrong_type 0, extra 0
data_exact   843 / 843
imports      207
lint         CLEAN
drift        0
RATCHET PASS
symbol parity 5370/5370
```

The target moves from 94.8% (0.9477173421726683) to 95.1%
(0.9509183980728696). The `byte_exact` count does not change, so the
ratchet baseline is unchanged. The `.text` content-byte delta drops from
103,084 to 101,915 (updated in `docs/VALIDATION.md` and
`docs/IDENTICALITY.md` in the same commit).

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump "$OD" --show _ZN15MenuTouchWindowC1Ei
```

Scratch (not tracked): `/dev/shm/w4715/` (`base.cpp`, `best.cpp`,
`measure.sh`, `ratio.py`, `official.py`, `diff4.py`, `loops2.py`,
`run*.py`).
