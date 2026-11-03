# MenuTouchWindow C1/C2 pass 7: the `__builtin_assume` PHI route to two
# same-orientation value selects, and why it still loses on register
# allocation (94.247% -> 94.247%, no code change landed)

**Status:** analysis only, 2026-11-03. Worktree `wt/w4418`, branch
`wave94-w4418`, base `f262bd4b`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). `_ZN15MenuTouchWindowC1Ei` / `C2` are 9600 B, 3320
instructions, **94.24698795180723%** (exact difflib ratio
`0.9424698795180723`). Gate at this pass's start and end is identical and
PASSing: `byte_exact 4499 / linked_exact 4499`, `size 4524/4524`,
`stub 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data 843/843`,
imports 207, lint CLEAN, drift 0.

No source shape found this pass beats HEAD. The note records the first
source construct found that reproduces the original's **two
same-orientation value selects** (the `__builtin_assume` PHI route), the
exact machine code it produces, and the register-allocation reason it
still loses to HEAD.

## TL;DR

* The original loop has **two same-orientation value selects**
  (`cmp.w sl,#9; it eq; moveq` twice, both `eq ? lang : font`). HEAD has
  one `eq ? lang : font` (spacing) and one `eq ? font : lang` (ctor arg).
* A two-entry PHI that survives `SimplifyCFG` is the only way to get a
  second same-orientation value select (EarlyCSE merges two identical
  IR `select`s; the DAG would CSE identical `select` nodes too).
* **New this pass:** putting `__builtin_assume(i < 100);` in the second
  `if` body makes the if-body non-speculatable, so `SimplifyCFG` keeps
  the PHI, `IfConverter` predicates it to `moveq`, and the assume is
  removed. This is the first source shape that produces the original's
  two `cmp.w sl,#9; it eq; moveq` value selects in the real TU.
* It measures **0.9191143244464528** (3319 instructions, 9540 B), not
  better than HEAD, because the register allocation is different:
  `r8=i, r9=&fontLangSelect, fp=lang, r6=font` instead of the original
  `r9=i, r8=lang, r5=font`, and the `fontId` store is 2 instructions
  (`itt eq; ldreq.w [r9]; streq`) instead of the original's 4
  (`ittt eq; ldreq [sp,#76]; ldreq [r0]; streq`). The loop is 4 bytes
  shorter than HEAD and 12 bytes shorter than the original, so every
  post-loop branch target shifts.
* **New this pass:** a different but equivalent condition,
  `(i >> 1) == 4`, also produces two same-orientation value selects
  without any assume. It measures **0.9299804246348442** (3321
  instructions). The second predicate is `cmp r2,#4; it eq; moveq`
  where `r2` is a spilled `i >> 1` reload, so it does not match the
  original's `cmp.w sl,#9` and costs an extra load.
* A 192-way permutation search of the loop body (declaration order,
  swapped/same second select, pointer/direct-global store, id/store
  order) found **no variant above HEAD**; the best is HEAD itself.
* A 16-way combination matrix of `__builtin_assume` × `int *volatile`
  × `optionByte` removal × store form also found **no variant above
  HEAD**; the best is HEAD itself.

## 1. Original language loop (verbatim, VMA 0x1233fa..0x1234ae)

67 instructions, `_ZN15MenuTouchWindowC1Ei` = 0x12299c, size 9600 = 0x2580.
Real addresses from `arm-linux-androideabi-objdump`:

```
1233fa  orr.w   sl, r8, #1          ; special = (i | 1) == 9
1233fe  ldr     r0, [pc, #720]      ; GOT &Globals::font
123400  mov     r9, r8              ; save i  (r8 will hold lang)
123402  ldr     r1, [pc, #720]      ; GOT &Globals::Canvas
123404  cmp.w   sl, #9
123408  ldr     r2, [pc, #716]      ; GOT &Globals::fontLangSelect
12340a  add     r0, pc
12340c  add     r1, pc
12340e  add     r2, pc
123410  ldr     r0, [r0, #0]        ; &font
123412  ldr     r1, [r1, #0]        ; &Canvas
123414  ldr     r2, [r2, #0]        ; &fontLangSelect
123416  ldr     r5, [r0, #0]        ; font value
123418  ldr     r0, [r1, #0]        ; Canvas value
12341a  ldr.w   r8, [r2]            ; fontLangSelect value
12341e  mov     r1, r5
123420  str     r2, [sp, #76]       ; spill &fontLangSelect
123422  it      eq
123424  moveq   r1, r8              ; select #1 (value) -> FontGetSpacing
123426  blx     PaintCanvas::FontGetSpacing(unsigned int)
12342a  mov     r4, r0              ; spacing
12342c  ldr     r0, [sp, #72]
12342e  ldr.w   fp, [r0]            ; buttonWidth (spilled &this->buttonWidth)
123432  movs    r0, #200
123434  blx     operator new(unsigned int)
123438  mov     r6, r0              ; TouchButton *
12343a  ldr     r0, [pc, #672]      ; languageTextIds
12343c  str.w   r9, [sp, #80]       ; spill i
123440  add     r0, pc
123442  ldr.w   r1, [r0, r9, lsl #2]   ; languageTextIds[i]
123446  ldr     r0, [sp, #96]
123448  ldr     r0, [r0, #0]
12344a  blx     GameText::getText(int)
12344e  mov     r1, r0
123450  cmp.w   sl, #9
123454  it      eq
123456  moveq   r5, r8              ; select #2 (value) -> TouchButton ctor
123458  ldr     r0, [sp, #84]
12345a  movs    r2, #0
12345c  strd    r0, fp, [sp]
123460  movs    r0, #17
123462  str     r0, [sp, #8]
123464  movs    r0, #4
123466  strd    r0, r5, [sp, #12]   ; ctor arg 9 = selected font
12346a  mov     r0, r6
12346c  ldr     r3, [sp, #88]
12346e  str     r4, [sp, #20]
123470  blx     TouchButton::TouchButton(String const&, int, int, int, int,
                                          unsigned char, unsigned char,
                                          unsigned int, int)
123474  ldr     r0, [pc, #616]      ; languageIds
123476  cmp.w   sl, #9
12347a  ldr.w   r8, [sp, #80]       ; restore i
12347e  add     r0, pc
123480  ldr.w   r0, [r0, r8, lsl #2]   ; languageIds[i]
123484  mov.w   r1, r0, asr #31
123488  strd    r0, r1, [r6]        ; language->id = ...
12348c  ittt    eq
12348e  ldreq   r0, [sp, #76]       ; &fontLangSelect (spilled pointer!)
123490  ldreq   r0, [r0, #0]        ; reload value
123492  streq   r0, [r6, #8]        ; language->fontId
123494  ldr.w   fp, [sp, #108]
123498  mov     r0, r6
12349a  ldr.w   r1, [fp, #176]
12349e  blx     ArrayAdd<TouchButton*>(...)
1234a2  add.w   r8, r8, #1
1234a6  ldr.w   r9, [sp, #92]
1234aa  cmp.w   r8, #10
1234ae  bcc.w   0x123346
```

Both selects are `eq ? lang : font`: `r5` = `Globals::font`, `r8` =
`Globals::fontLangSelect`, and each select writes the selected value into a
register that already holds `font` (`r1` for the call argument, `r5` for the
ctor argument). There is exactly **one** load of each global (0x123416,
0x12341a); the `fontId` store reloads `fontLangSelect` through the spilled
GOT pointer (0x12348e/0x123490).

## 2. HEAD loop (63 instructions) and the delta

HEAD (`src/game/ui/MenuTouchWindow.cpp` lines 452-462) is:

```cpp
bool special = (i | 1) == 9;
unsigned int fontValue = Globals::font;
int langValue = Globals::fontLangSelect;
int spacing = Globals::Canvas->FontGetSpacing(
    special ? (unsigned int) langValue : fontValue);
TouchButton *language = new TouchButton(
    *Globals::gameText->getText(MenuTouchWindow_languageTextIds[i]), 0, x, y,
    this->buttonWidth, 17, 4,
    (i == 9 || i == 8) ? fontValue : (unsigned int) langValue, spacing);
language->id = MenuTouchWindow_languageIds[i];
if (special)
    language->fontId = *specialFontPtr;
ArrayAdd(language, *langList);
```

HEAD's second select is `eq ? font : lang` (swapped), which keeps the two
selects distinct (EarlyCSE does not merge swapped operands). The loop is
63 instructions; the original is 67. The four missing instructions are
exactly `mov r9,r8`, `str r2,[sp,#76]`, `str.w r9,[sp,#80]` and
`ldr.w r8,[sp,#80]`; the fifth difference (the extra `ldreq` in the
`fontId` store) is compensated by one instruction HEAD has elsewhere.

## 3. New: the `__builtin_assume` PHI route

### 3.1 Source

```cpp
bool special = (i | 1) == 9;
unsigned int fontValue = Globals::font;
int langValue = Globals::fontLangSelect;
int spacing = Globals::Canvas->FontGetSpacing(
    special ? (unsigned int) langValue : fontValue);
unsigned int buttonFont = fontValue;
if (special) {
    buttonFont = (unsigned int) langValue;
    __builtin_assume(i < 100);
}
TouchButton *language = new TouchButton(
    *Globals::gameText->getText(MenuTouchWindow_languageTextIds[i]), 0, x, y,
    this->buttonWidth, 17, 4, buttonFont, spacing);
language->id = MenuTouchWindow_languageIds[i];
if (special)
    language->fontId = Globals::fontLangSelect;
ArrayAdd(language, *langList);
```

`__builtin_assume` lowers to `llvm.assume`, which `SimplifyCFG` treats as a
non-speculatable instruction in the if-body, so `FoldTwoEntryPHINode` does
not fold the second `if` to a select. The PHI survives to `IfConverter`,
which predicates it to `moveq`; the assume itself is removed. The first
select (the ternary for `spacing`) is a normal IR `select`. Because the
second is a PHI (not an IR `select`), EarlyCSE cannot merge them even
though the operands and condition are identical.

### 3.2 Exact loop (ours, 0.9191143244464528, 3319 instructions, 9540 B)

```
1233fa  orr.w   sl, r8, #1
1233fe  ldr     r0, [pc, #708]      ; GOT &font
123400  cmp.w   sl, #9
123404  ldr     r1, [pc, #704]      ; GOT &Canvas
123406  ldr     r2, [pc, #708]      ; GOT &fontLangSelect
123408  add     r0, pc
12340a  add     r1, pc
12340c  add     r2, pc
12340e  ldr     r0, [r0, #0]
123410  ldr     r1, [r1, #0]
123412  ldr.w   r9, [r2]            ; &fontLangSelect (r9, not spilled)
123416  ldr     r6, [r0, #0]        ; font
123418  ldr     r0, [r1, #0]        ; Canvas
12341a  ldr.w   fp, [r9]            ; lang
12341e  mov     r1, r6
123420  it      eq
123422  moveq   r1, fp              ; select #1 (value, same orientation)
123424  blx     PaintCanvas::FontGetSpacing(unsigned int)
123428  mov     r4, r0
12342a  movs    r0, #200
12342c  blx     operator new(unsigned int)
123430  mov     r5, r0
123432  ldr     r0, [pc, #668]
123434  add     r0, pc
123436  ldr.w   r1, [r0, r8, lsl #2] ; i still in r8, never spilled
12343a  ldr     r0, [sp, #96]
12343c  ldr     r0, [r0, #0]
12343e  blx     GameText::getText(int)
123442  mov     r1, r0
123444  ldr     r0, [sp, #80]
123446  ldr     r0, [r0, #0]        ; buttonWidth (spilled &this->buttonWidth)
123448  cmp.w   sl, #9
12344c  it      eq
12344e  moveq   r6, fp              ; select #2 (value, same orientation)
123450  ldr     r2, [sp, #84]
123452  strd    r2, r0, [sp]
123456  movs    r0, #17
123458  str     r0, [sp, #8]
12345a  movs    r0, #4
12345c  strd    r0, r6, [sp, #12]
123460  mov     r0, r5
123462  ldr     r3, [sp, #88]
123464  movs    r2, #0
123466  str     r4, [sp, #20]
123468  blx     TouchButton::TouchButton(...)
12346c  ldr     r0, [pc, #612]
12346e  cmp.w   sl, #9
123472  ldr.w   fp, [sp, #108]
123476  add     r0, pc
123478  ldr.w   r0, [r0, r8, lsl #2]
12347c  mov.w   r1, r0, asr #31
123480  strd    r0, r1, [r5]
123484  itt     eq
123486  ldreq.w r0, [r9]            ; 2-instruction store
12348a  streq   r0, [r5, #8]
12348c  ldr     r4, [sp, #68]
12348e  mov     r0, r5
123490  mov     r1, r4
123492  blx     ArrayAdd<TouchButton*>(...)
123496  add.w   r8, r8, #1
12349a  ldr.w   r9, [sp, #92]
12349e  cmp.w   r8, #10
1234a2  bne.w   0x123344
```

Both selects are now `eq ? lang : font` (the original orientation). But
the allocation is `r4=spacing, r5=button, r6=font, r8=i, r9=&lang,
fp=lang`; the original is `r4=spacing, r5=font, r6=button, r8=lang,
r9=i, fp=buttonWidth`. The GOT pointer is not spilled, `i` is not
spilled, and the `fontId` store is 2 instructions instead of 4. The loop
is 0xa8 bytes (vs HEAD 0xac, original 0xb4), so every post-loop branch
target shifts by 4 bytes relative to HEAD and 12 bytes relative to the
original.

### 3.3 Why it loses

The two-select orientation is correct, but the loop is shorter and the
register allocation is different. The verifier compares intra-function
branch targets literally, so the 4-byte loop-size change alone shifts
every branch target after the loop and costs more than the two select
matches gain. The assume also changes the RA of the whole function (the
stack slots and the `optionByte` conversions are unchanged, but the
`fontId` store and the loop spills move).

## 4. New: `(i >> 1) == 4` — two value selects without an assume

For the loop index range 0..9, `(i >> 1) == 4` is equivalent to
`(i | 1) == 9` (both are true only for i = 8, 9). The compiler does not
canonicalize `i >> 1` to `i | 1`, so the second condition is a distinct
IR value and the two selects are not merged:

```cpp
int spacing = Globals::Canvas->FontGetSpacing(
    special ? (unsigned int) langValue : fontValue);
TouchButton *language = new TouchButton(
    *Globals::gameText->getText(MenuTouchWindow_languageTextIds[i]), 0, x, y,
    this->buttonWidth, 17, 4,
    ((i >> 1) == 4) ? (unsigned int) langValue : fontValue, spacing);
```

Exact loop (0.9299804246348442, 3321 instructions):

```
1233fe  orr.w   r6, r8, #1
123404  cmp     r6, #9
...
123414  ldr.w   sl, [r2]            ; &fontLangSelect
123418  ldr.w   fp, [r0]            ; font
12341e  ldr.w   r9, [sl]            ; lang
123422  mov     r1, fp
123424  it      eq
123426  moveq   r1, r9              ; select #1 (value)
123428  blx     FontGetSpacing
...
123448  ldr     r0, [sp, #76]
12344a  ldr     r0, [r0, #0]
12344c  ldr     r2, [sp, #88]       ; reload i >> 1 (spilled)
12344e  cmp     r2, #4
123450  it      eq
123452  moveq   fp, r9              ; select #2 (value, same orientation)
...
123472  cmp     r6, #9
...
123486  itt     eq
123488  ldreq.w r0, [sl]
12348c  streq   r0, [r5, #8]
```

The second predicate is `cmp r2,#4; it eq; moveq` (with `r2` a spilled
`i >> 1` reload), not the original's `cmp.w sl,#9`. The extra reload and
the different condition keep the function at 3321 instructions and
0.92998, below HEAD.

## 5. Other condition variants measured this pass

All are full-TU rebuilds with the exact NDK r18b flags. `orig` is always
3320 instructions / 9600 B. The verifier row is
`_ZN15MenuTouchWindowC1Ei`; `pct` is the exact difflib ratio.

| variant | second condition | pct | ours insns |
|---|---|---|---|
| **HEAD** | `(i == 9 \|\| i == 8) ? font : lang` (swapped) | **0.9424698795180723** | 3320 |
| `assume` | `if (special) { buttonFont = lang; __builtin_assume(i < 100); }` | 0.9191143244464528 | 3319 |
| `shift` | `((i >> 1) == 4) ? lang : font` | 0.9299804246348442 | 3321 |
| `ge9` | `((i \| 1) >= 9) ? lang : font` | 0.9185118240698901 | 3319 |
| `gt8` | `((i \| 1) > 8) ? lang : font` | 0.9185118240698901 | 3319 |
| `and` | `(((i \| 1) & ~9) == 0) ? lang : font` | 0.9144835892803372 | 3322 |
| `mask` | `((i & ~1) == 8) ? lang : font` | 0.7395614298588165 | 3338 |
| `and_w` | `((i \| 1) == 9 && Globals::w > 0) ? lang : font` | 0.8713705431021513 | 3327 |
| `recompute` | `((i \| 1) == 9) ? lang : font` | 0.7227320595757485 | 3327 |
| `sub` | `(((i \| 1) - 9) == 0) ? lang : font` | 0.7227320595757485 | 3327 |
| `xor` | `(((i \| 1) ^ 9) == 0) ? lang : font` | 0.7227320595757485 | 3327 |
| `or8` | `(i == 8 \|\| i == 9) ? lang : font` | 0.7227320595757485 | 3327 |

`recompute`, `sub`, `xor`, `or8` all simplify to the same IR as `special`
and merge with the first select; the DAG then folds the merged select to
an address select (`ldrne r2,[r1,#0]`). `mask` compiles to `bic/cmp #8`
and changes the RA. `ge9`/`gt8` compile to `cmp #8; it hi; movhi` and
shrink the function to 3319. `shift` is the closest non-HEAD variant.

## 6. Exhaustive loop-body permutation search

A 192-way search over the loop body found no ratio above HEAD. The
permuted dimensions were:

* all 24 orders of `special`, `fontValue`, `langValue`, `spacing`
  declarations/computations;
* second select swapped (`font : lang`) vs same (`lang : font`);
* `fontId` store via `*specialFontPtr` vs `Globals::fontLangSelect`;
* `language->id` before vs after the `fontId` store.

Every variant measured either exactly `0.9424698795180723` (byte-identical
to HEAD) or worse. The best is HEAD itself.

A 16-way matrix of `__builtin_assume` × `int *volatile` × `optionByte`
removal × store form also found no ratio above HEAD:

| assume | volatile ptr | optionByte removed | store | pct |
|---|---|---|---|---|
| no | no | no | pointer/global | **0.9424698795180723** |
| no | no | yes | pointer/global | 0.9050346698824239 |
| no | yes | no | pointer | 0.9058593161620726 |
| no | yes | no | global | 0.9070643169151981 |
| no | yes | yes | pointer | 0.9078848183325795 |
| no | yes | yes | global | 0.9090909090909091 |
| yes | no | no | pointer/global | 0.9191143244464528 |
| yes | no | yes | pointer/global | 0.9163274536408865 |
| yes | yes | no | pointer | 0.9058593161620726 |
| yes | yes | no | global | 0.9070643169151981 |
| yes | yes | yes | pointer | 0.9078848183325795 |
| yes | yes | yes | global | 0.9090909090909091 |

The `int *volatile` pointer reproduces the original's 4-instruction
`ittt eq; ldreq [sp,#348]; ldreq [r0]; streq` store shape exactly, but it
re-colours the whole function (`fp=special, sl=font, r6=lang, r8=i,
r9=x`) and loses more than the store gains.

## 7. Loop-condition variants

The original loop backedge is `cmp.w r8,#10; bcc.w` (unsigned lower),
i.e. `i < 10`. HEAD uses `i != 10` and emits `cmp.w r8,#10; bne.w`.
Changing the source to `i < 10` makes clang emit `cmp r8,#9; bls`
(InstCombine canonicalizes `ult i,10` to `ule i,9`), which is two
instruction mismatches instead of one, and measures
`0.9421686746987952` (3320 instructions). `i <= 9`, `i < 10u`,
`10 > i`, `sizeof(...)/sizeof(int)`, and a `const`/`static const`/`enum`
bound of 10 all produce the same `cmp #9; bls` and the same 0.94217.
`while (i < 10)`, `do { ... } while (i < 10)`, `++i`, `i += 1`, and
`i = i + 1` do not produce `cmp #10; bcc` either. Only `i != 10` keeps
the `cmp #10` immediate, and it measures slightly better.

## 8. What remains

The two-select orientation is now reproducible (the `__builtin_assume`
PHI route), but the original's register allocation is the remaining
fixed point:

| value | original | HEAD | assume variant |
|---|---|---|---|
| `i` | `r9` (saved `mov r9,r8`, spilled `[sp,#80]`) | `r8` | `r8` |
| `Globals::font` value | `r5` | `fp` | `r6` |
| `fontLangSelect` value | `r8` | `r6` | `fp` |
| `&fontLangSelect` | spilled `[sp,#76]` | `r9` | `r9` |
| `this->buttonWidth` | `fp` (loaded `[sp,#72]`) | stack `[sp,#80]` | stack `[sp,#80]` |
| spacing / button | `r4` / `r6` | `r4` / `r5` | `r4` / `r5` |
| select result | rematerialized | kept in `r6` | kept in `r6` |

The original gives registers to `i` and `buttonWidth` and spills the GOT
pointer; HEAD and the assume variant give registers to `i` and the GOT
pointer and spill `buttonWidth`. The original's rematerialization is
forced: `r5` and `r8` hold `font` and `lang` for the second select, `r4`
holds spacing, `r6` the button, `r9` the loop index, and `fp` the button
width. There is no seventh callee-saved register, so the select is
recomputed (`mov r1,r5; moveq r1,r8` for the call, `moveq r5,r8` for the
ctor).

The remaining route is to make the assume/PHI variant's register
allocator pick the original's allocation. The volatile-pointer and
`optionByte`-removal combinations perturb it in the wrong direction.
A source-level use that forces `lang` into `r8` (so `i` moves to `r9`
and spills) without adding an instruction is still missing.

## 9. Reproduction commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# loop disassembly (original / ours)
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -M force-thumb --start-address=0x1233fa --stop-address=0x1234b0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d -M force-thumb --start-address=0x1233fa --stop-address=0x1234a6 \
  cmake-build-match/libgof2hdaa.so

# official verifier row
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump "$OD" --top 0
```

Scratch variants and the search scripts used for this pass live in
`/dev/shm/w4418/` (`b_*.txt`, `run_*.py`, `search_perm.py`,
`assume_loop.txt`, `shift_loop.txt`).
