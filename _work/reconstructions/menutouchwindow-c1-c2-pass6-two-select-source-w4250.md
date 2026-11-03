# MenuTouchWindow C1/C2 pass 6: the two-select source search — pointer-operand
# and condition variants, and why the original pair is a register-allocation
# fixed point (94.247% -> 94.247%, no code change landed)

**Status:** analysis only, 2026-11-03. Worktree `wt/w4250`, branch
`wave92-w4250`, base `b379d00d`, HEAD `37fd2487` (pass 5). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). `_ZN15MenuTouchWindowC1Ei` / `C2` are 9600 B, 3320
instructions, **94.24698795180723%** (exact difflib ratio
`0.9424698795180723`). Gate at this pass's start and end is identical and
PASSing: `byte_exact 4499 / linked_exact 4499`, `size 4524/4524`,
`stub 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data 843/843`,
imports 207, lint CLEAN, drift 0. (The wave-92 brief said 4498; the actual
baseline in this worktree is **4499**.)

No source shape found this pass beats HEAD. This note records the new
experiments, the exact machine-code target, and the mechanism that makes the
original pair a register-allocation fixed point rather than a source-shape
property.

## TL;DR

* The original loop has **two same-orientation value selects**
  (`cmp.w sl,#9; it eq; moveq` twice, both `eq ? lang : font`). HEAD has one
  `eq ? lang : font` (spacing) and one `eq ? font : lang` (ctor arg). The
  swapped second select is what keeps the two selects distinct in HEAD.
* Two *identical* `select i1 %c, i32 %lang, i32 %font` cannot survive to
  codegen: `EarlyCSE w/ MemorySSA` merges them (`Instruction::
  isIdenticalToWhenDefined` ignores debug locations), and `DAGCombiner::
  SimplifySelectOps` then folds the two single-use loads into one **address**
  select (`mov r2,sl; it ne; ldrne r2,[r1,#0]; ldr r4,[r2,#0]`).
* New this pass: a **pointer dereference** for one select operand
  (`*specialFontPtr`) does produce two same-orientation value selects, because
  the two loads are distinct SSA values — but it costs a second
  `ldr.w rX,[&fontLangSelect]` and re-colours the whole function
  (73.0%, 9564 B).
* New this pass: a **different but equivalent condition**
  (`(i|1) >= 9`, valid because the loop index is 0..9) also produces two
  same-orientation value selects with the original's operand pair, but the
  second predicate lowers as `cmp.w sl,#8; it hi; movhi` instead of
  `cmp.w sl,#9; it eq; moveq` (91.9%, 9596 B).
* New this pass: keeping `langValue` alive past the select (store
  `language->fontId = langValue`) prevents the address-select fold
  (`SimplifySelectOps` requires **both** loads to have one use), so the
  select stays a value select — but the compiler then keeps the select result
  in a register instead of rematerializing it (73.4%, 9536 B).
* The original's pair is a **register-allocation fixed point**:
  `r4=spacing, r5=font, r6=button, r8=lang, r9=i, fp=buttonWidth`, with
  `&fontLangSelect` spilled to `[sp,#76]` and the select rematerialized.
  HEAD's allocation is `r4=spacing, r5=button, r6=lang, r8=i, r9=&lang,
  fp=font`, with `buttonWidth` spilled. Every source-level attempt to move
  one of these values moves the whole allocation and loses more instructions
  than it gains.

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

## 2. Ghidra pseudocode for the same region (cached, `/dev/shm/w4116/dec.c`)

```c
uVar42 = uVar37 | 1;
puVar13 = *(uint **)(iRam001336d8 + 0x133412);   /* &Globals::fontLangSelect */
uVar32 = **(uint **)(iRam001336d0 + 0x13340e);   /* Globals::font value      */
uVar38 = *puVar13;                                /* Globals::fontLangSelect  */
uVar23 = uVar32;
if (uVar42 == 9) {
    uVar23 = uVar38;
}
iVar29 = AbyssEngine::PaintCanvas::FontGetSpacing(..., uVar23);
iVar31 = *(int *)pMVar22;                         /* this->buttonWidth        */
pTVar7 = (TouchButton *)FUN_0006eb24(200);        /* operator new             */
pSVar5 = GameText::getText(..., languageTextIds[uVar37]);
if (uVar42 == 9) {
    uVar32 = uVar38;
}
TouchButton::TouchButton(pTVar7, pSVar5, 0, iVar3, local_234, iVar31,
                         '\x11', '\x04', uVar32, iVar29);
iVar3 = languageIds[uVar37];
*(int *)pTVar7 = iVar3;
*(int *)(pTVar7 + 4) = iVar3 >> 0x1f;
if (uVar42 == 9) {
    *(uint *)(pTVar7 + 8) = *puVar13;
}
```

Ghidra sees **two locals**: `uVar23` (spacing font, initialised from the font
value) and `uVar32` (button font, the font value itself, reassigned on the
special path). Both are conditionally set to `uVar38` (the single loaded
`fontLangSelect` value). This is the shape a faithful reconstruction should
have; it is also the shape that EarlyCSE collapses.

## 3. HEAD loop (63 instructions) and the delta

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

Real-TU loop (0x1233fa..0x1234a4, 63 instructions):

```
orr.w   sl, r8, #1
ldr     r0, [pc, #712]           ; GOT &font
cmp.w   sl, #9
ldr     r1, [pc, #708]           ; GOT &Canvas
ldr     r2, [pc, #712]           ; GOT &fontLangSelect
add     r0, pc
add     r1, pc
add     r2, pc
ldr     r0, [r0, #0]             ; &font
ldr     r1, [r1, #0]             ; &Canvas
ldr.w   r9, [r2]                 ; &fontLangSelect
ldr.w   fp, [r0]                 ; font value
ldr     r0, [r1, #0]             ; Canvas value
ldr.w   r6, [r9]                 ; lang value
mov     r1, fp
it      eq
moveq   r1, r6                   ; select #1 (lang,font)
blx     FontGetSpacing
mov     r4, r0                   ; spacing
movs    r0, #200
blx     operator new
mov     r5, r0                   ; button
ldr     r0, [pc, #668]           ; languageTextIds
add     r0, pc
ldr.w   r1, [r0, r8, lsl #2]
ldr     r0, [sp, #96]
ldr     r0, [r0, #0]
blx     getText
mov     r1, r0
ldr     r0, [sp, #80]
ldr     r0, [r0, #0]             ; buttonWidth
cmp.w   sl, #9
it      eq
moveq   r6, fp                   ; select #2 (font,lang)  <-- SWAPPED
ldr     r2, [sp, #84]
strd    r2, r0, [sp]
movs    r0, #17
str     r0, [sp, #8]
movs    r0, #4
strd    r0, r6, [sp, #12]
mov     r0, r5
ldr     r3, [sp, #88]
movs    r2, #0
str     r4, [sp, #20]
blx     TouchButton ctor
ldr     r0, [pc, #616]           ; languageIds
cmp.w   sl, #9
ldr.w   fp, [sp, #108]
add     r0, pc
ldr.w   r0, [r0, r8, lsl #2]
mov.w   r1, r0, asr #31
strd    r0, r1, [r5]
itt     eq
ldreq.w r0, [r9]                 ; &fontLangSelect in r9
streq   r0, [r5, #8]
ldr     r4, [sp, #68]
mov     r0, r5
mov     r1, r4
blx     ArrayAdd
add.w   r8, r8, #1
ldr.w   r9, [sp, #92]
cmp.w   r8, #10
bne.w   0x123344
```

Delta vs original (67 - 63 = 4 instructions):

| original | HEAD |
|---|---|
| `mov r9, r8` (save `i`) | — |
| `str r2, [sp,#76]` (spill `&lang`) | — |
| `str.w r9, [sp,#80]` (spill `i`) | — |
| `ldr.w r8, [sp,#80]` (restore `i`) | — |
| `ittt eq; ldreq [sp,#76]; ldreq [r0]; streq` (4) | `itt eq; ldreq [r9]; streq` (3) |
| `ldr r0,[sp,#72]; ldr fp,[r0]` (buttonWidth, 2) | `ldr r0,[sp,#80]; ldr r0,[r0]` (2) |
| `ldr fp,[sp,#108]; mov r0,r6; ldr r1,[fp,#176]` (3) | `ldr r4,[sp,#68]; mov r0,r5; mov r1,r4` (3) |

The five original-only instructions are the `i`/pointer spills and the extra
`ldreq`; HEAD compensates with one instruction elsewhere (the `movs r2,#0`
placement / `buttonWidth` load position), so the net is +4.

## 4. Mechanism: why identical selects cannot survive

Verified against LLVM 7 sources (cached in `/dev/shm/w4228/`):

1. `EarlyCSE` hashes `SelectInst` by opcode + `value_op_begin()..
   value_op_end()` (`EarlyCSE.cpp:127-215`) and `isEqual` calls
   `isIdenticalToWhenDefined`; debug locations are ignored. Two identical
   `select i1 %c, i32 %lang, i32 %font` become one.
2. Once merged, the select has two uses and each load has one use.
   `DAGCombiner::SimplifySelectOps` (`DAGCombiner.cpp:17604`) folds
   `select c, (load A), (load B)` into `load (select c, A, B)` only when
   **both** loads have exactly one use. This is the address-select shape
   (`mov r2,sl; it ne; ldrne r2,[r1,#0]; ldr r4,[r2,#0]`) measured for
   `q1`/`body_same` (57 loop instructions, 0.72273).
3. `SimplifyCFG::FoldTwoEntryPHINode` (`SimplifyCFG.cpp:2260`) folds a
   two-entry PHI to a select unless the merge block has **4+ PHIs**
   (`NumPhis > 2` before increment) or an if-body instruction is not safe to
   speculate (`DominatesMergePoint`; stores, non-speculatable calls, PHIs).
   A trivial `if (special) b = lang;` always folds.
4. `EarlyCSE` runs at pipeline passes 4 and 22; `SimplifyCFG` runs at passes
   2, 14, 25, 28, 33, 52, 72, 92; `GVN` at 39. A PHI that survives to
   pass 52+ is folded to a select **after** all CSE passes and therefore
   survives. A PHI folded at pass 14/25/33 is merged by EarlyCSE/GVN.

## 5. New experiments this pass (official verifier, real TU)

All are full-TU rebuilds with the exact NDK r18b flags. `orig` is always
3320 instructions / 9600 B. The verifier row is
`_ZN15MenuTouchWindowC1Ei`; `pct` is the exact difflib ratio.

| variant | source change | pct | ours B |
|---|---|---|---|
| **HEAD** | swapped second select, `*specialFontPtr` store | **0.9424698795180723** | 9600 |
| `body_ge` | second condition `(i|1) >= 9` (valid for i in 0..9) | 0.919 | 9596 |
| `body_ge2` | `body_ge` + store `Globals::fontLangSelect` directly | 0.919 | 9596 |
| `body_localstore` | both selects `special ? lang : font`; store `language->fontId = langValue` | 0.734 | 9536 |
| `body_ptrsel` (= pass-5 `p2`) | second select `special ? *specialFontPtr : fontValue` | 0.730 | 9564 |
| `body_b` | first select local, second select `Globals::fontLangSelect` directly | 0.730 | 9564 |
| `body_nat` | both selects read the globals directly (pass-4 natural shape) | 0.731 | 9556 |
| `body_v1` | both selects same orientation, store global directly | (address select) | — |
| `body_a` | first select `*specialFontPtr`, second select local | (address select) | — |
| `body_c` | first select global, second select local | (address select) | — |
| `body_reassign` | `spacingFont` PHI + `fontValue` reassigned before ctor | (address select) | — |
| `body_bothstore` | `if/else` store of `langValue`/`fontValue` | (51-insn shape) | — |

### 5.1 Pointer operand (`tptr`, small-function proof)

```cpp
bool s = (i|1)==9;
int *p = &glang;
int f1 = gfont, l1 = glang;
int a = s ? l1 : f1;
int t = ext(a);
int b = s ? *p : f1;      // distinct load -> distinct select
return ext2(b,t);
```

Final IR keeps both selects:

```llvm
%6 = select i1 %3, i32 %5, i32 %4      ; l1, f1
%9 = select i1 %3, i32 %8, i32 %4      ; *p (reload), f1
```

Assembly:

```
ldr r5, [r1, #0]   ; f1
ldr r0, [r4, #0]   ; l1
it ne
movne r0, r5       ; select #1
bl ext
mov r1, r0
ldr r0, [r4, #0]   ; RELOAD l1
cmp r6, #9
it ne
movne r0, r5       ; select #2 (same orientation)
```

This is the first *value*-select pair with the original orientation, but it
needs the reload. In the real TU (`body_ptrsel`) the reload plus the pointer
live range re-colours the whole function: 9564 B, 0.730.

### 5.2 Equivalent condition (`body_ge`)

`(i | 1) >= 9` is equivalent to `(i | 1) == 9` because the loop index is
0..9, but LLVM keeps a distinct `icmp ugt i32 %or, 8`. Real-TU loop:

```
... ldr.w fp, [r9]                 ; lang
mov   r1, r6
it    eq
moveq r1, fp                       ; select #1 (lang,font)
blx   FontGetSpacing
...
cmp.w sl, #8
it    hi
movhi r6, fp                       ; select #2 (lang,font)  <-- cmp #8/it hi
```

Two same-orientation value selects with the correct operand pair, but the
second predicate is `cmp #8; it hi` instead of `cmp #9; it eq`, and the
function shrinks to 9596 B (91.9%). The 4-byte size difference alone shifts
the comparison enough to lose ~2.3 points.

### 5.3 Keeping the load alive (`body_localstore`)

`SimplifySelectOps` requires **both** loads to have one use. Storing
`language->fontId = langValue` gives the `fontLangSelect` load a second use,
so the merged select stays a value select:

```
ldr r4, [r0, #0]   ; font value
ldr.w r9, [r2]     ; lang value
it eq
moveq r4, r9       ; value select (not address select)
...
it eq
streq.w r9, [r5, #8]   ; store uses the local, no reload
```

But the compiler keeps the select result in `r4` instead of rematerializing
it, because `r4` is free (spacing moved to `r6`). The loop is 58
instructions and the function 9536 B (0.734).

## 6. The register-allocation fixed point

| value | original | HEAD |
|---|---|---|
| `i` | `r9` (save `mov r9,r8`, spill `[sp,#80]`, restore `r8`) | `r8` (never spilled) |
| `Globals::font` value | `r5` | `fp` |
| `fontLangSelect` value | `r8` | `r6` |
| `&Globals::fontLangSelect` | spilled `[sp,#76]` | `r9` |
| `this->buttonWidth` | `fp` (loaded `[sp,#72]`) | stack `[sp,#80]` |
| spacing / button | `r4` / `r6` | `r4` / `r5` |
| select result | rematerialized (no register) | kept in `r6` |

Both allocations use six callee-saved registers (`r4,r5,r6,r8,r9,fp`); the
difference is *which* of `{i, &fontLangSelect, buttonWidth}` wins a register.
The original gives registers to `i` and `buttonWidth` and spills the pointer;
HEAD gives registers to `i` and the pointer and spills `buttonWidth`.

The original's rematerialization is forced: `r5` and `r8` hold `font` and
`lang` for the *second* select, `r4` holds spacing, `r6` the button, `r9`
the loop index, and `fp` the button width. There is no seventh callee-saved
register, so the select is recomputed (`mov r1,r5; moveq r1,r8` for the call,
`moveq r5,r8` for the ctor). HEAD avoids this only because its second select
is a *different* value (`select c, font, lang`), which the compiler can keep
in `r6` (it overwrites `lang`, which is dead after the select).

## 7. What was tried and what remains

Tried this pass: pointer-dereference operands (two distinct selects but a
reload), equivalent-but-different conditions (`cmp #8; it hi`), keeping the
load alive to block the address-select fold (value select but no
rematerialization), `if`/`else` and reassignment shapes (address select),
direct-global store (no RA change), and the pass-4 natural shape (address
select). None is a strict improvement; all move the whole-function
allocation and lose more than the two-select orientation gains.

The remaining route is the PHI one: a two-entry PHI whose if-body carries a
non-speculatable instruction that is removed **between GVN (pass 39) and the
last `SimplifyCFG` (pass 92)**. A dead store is removed too early (pass 13,
`tdead`), and a store that survives (pass 14, `tdead2`) makes IfConverter
emit a branch rather than a predicated move. A non-speculatable instruction
that survives to the last `SimplifyCFG` and is then dropped by
CodeGenPrepare (e.g. a lifetime intrinsic, or a store CodeGenPrepare can
prove dead) would let the late `SimplifyCFG` create the second select after
all CSE passes, and IfConverter would predicate it to `moveq`. That is the
most promising untried construct; it needs a source-level use that adds no
machine instruction, which the current reconstruction does not have.

## 8. Reproduction commands

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

Scratch variants and the small-function harnesses used for this pass live in
`/dev/shm/w4250/` (`body_*.txt`, `try.sh`, `evalone.sh`, `tptr.cpp`,
`tcond2.cpp`, `tdead.cpp`, `tdead2.cpp`, `tlocal.cpp`, `tsep.cpp`).
