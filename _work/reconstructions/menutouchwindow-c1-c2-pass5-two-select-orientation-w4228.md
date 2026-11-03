# MenuTouchWindow C1/C2 pass 5: why the two value selects cannot both be
# `eq ? lang : font`, and the register-allocation chain that keeps the loop
# 4 instructions short (94.247% -> 94.247%, no code change landed)

**Status:** analysis only, 2026-11-03. Worktree `wt/w4228`, branch
`wave92-w4228`, base `b379d00d`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). `_ZN15MenuTouchWindowC1Ei` / `C2` are 9600 B, 3320
instructions, **94.24698795180723%** (exact difflib ratio
`0.9424698795180723`). Gate is unchanged and PASSing:
`byte_exact 4498 / linked_exact 4498`, `size 4524/4524`, `stub 0`,
`missing 0`, `wrong_type 0`, `extra 0`, `data 843/843`, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
`.ARM.extab 0x26290`, lint CLEAN, drift 0.

No source shape found this pass beats HEAD. The note records the new
evidence, the mechanism that blocks the obvious fix, and the exact
register-allocation chain that the remaining loop diff reduces to.

## TL;DR

* The original loop has **two same-orientation value selects**
  (`cmp.w sl,#9; it eq; moveq` twice, both `eq ? lang : font`). HEAD has
  one `eq ? lang : font` (spacing) and one `eq ? font : lang` (ctor arg).
* Two *identical* selects cannot survive to codegen:
  `EarlyCSE` (LLVM 7 `isIdenticalToWhenDefined`, debug locs ignored)
  merges them at IR level; and even if they survived IR, SelectionDAG
  CSE would merge two identical `select` nodes. The only route to two
  same-orientation selects is a **PHI that survives `SimplifyCFG`**, so
  that ARM's `IfConverter` emits the second one as a predicated move
  from a `PHI` machine instruction rather than from a DAG `select`.
* `SimplifyCFG::FoldTwoEntryPHINode` folds a trivial two-entry PHI to a
  select unless (a) the merge block has **4 or more** PHIs
  (`NumPhis > 2` check runs before the increment) or (b) the if-body
  contains a non-speculatable instruction (`DominatesMergePoint` /
  `AggressiveInsts`). The original loop body contains neither a store
  nor a call in the second if-body, so a faithful source shape that
  forces the PHI was not found.
* The remaining loop diff is **not** the selects themselves but the
  register allocation: the original keeps `i` in `r9`, `lang` in `r8`
  and spills `&Globals::fontLangSelect` to `[sp,#76]`; HEAD keeps `i`
  in `r8`, `&fontLangSelect` in `r9` and `lang` in `r6`. That is worth
  **4 extra instructions** in the original loop (67 vs 63) plus one
  extra `ldreq` in the `fontId` store.
* Forcing the pointer to memory with `int *volatile` reproduces the
  original 4-instruction `ittt eq; ldreq; ldreq; streq` store shape
  exactly, but changes the surrounding RA and is a net regression
  (90.37%). It is evidence for the mechanism, not a landing candidate.

## 1. Original language loop (verbatim, VMA 0x1233fa..0x1234ae)

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

Loop length: **67 instructions**. HEAD's loop is **63 instructions**
(`orr.w sl,r8,#1` .. `bne.w`). The four missing instructions are exactly
`mov r9,r8`, `str r2,[sp,#76]`, `str.w r9,[sp,#80]` and
`ldr.w r8,[sp,#80]`; the fifth difference (the extra `ldreq` in the
`fontId` store) is compensated by one instruction HEAD has elsewhere in
the loop.

## 2. Mechanism: identical selects cannot survive

Verified against the LLVM 7.0.1 sources (downloaded to
`/dev/shm/w4228/{EarlyCSE,Instruction,SimplifyCFG,SelectionDAG}.cpp`):

1. `Instruction::isIdenticalToWhenDefined` compares opcode, type and
   operands only; `haveSameSpecialState` has no `SelectInst` case, so
   debug locations are ignored. `EarlyCSE`'s `DenseMapInfo<SimpleValue>`
   hashes a plain select by `hash_combine_range(value_op_begin(),
   value_op_end())` and `isEqual` calls `isIdenticalToWhenDefined`.
   Therefore `select %c, %lang, %font` twice -> one select.
   (Swapped operands hash differently and both survive; that is the
   HEAD shape.)
2. `EarlyCSE` runs on the dominator tree with a scoped hash table, so it
   merges across the `invoke` that splits the loop body into the
   `FontGetSpacing` block and the `getText` block. Measured directly:
   `q1` (both selects `special ? langValue : fontValue`) collapses to
   one select and then `DAGCombiner::SimplifySelectOps` turns it into
   an *address* select (`ldrne r2,[r1,#0]`), because each load then has
   a single use. 57 loop instructions, ratio 0.72273.
3. Even if two identical selects survived IR, `SelectionDAG::getNode`
   CSEs identical `select` nodes. So the original's second select must
   not be a DAG `select` node: it must come from a **PHI** that ARM's
   `IfConverter` (a MachineFunction pass) converts into a predicated
   move.
4. `SimplifyCFG::FoldTwoEntryPHINode` (LLVM 7) folds a two-entry PHI to
   a select unless:
   * the merge block has `NumPhis > 2` **before** the increment, i.e.
     **four or more** PHIs (`SimplifyCFG.cpp:2285-2288`), or
   * a value in the if-body is not in `AggressiveInsts`, i.e. it is not
     safe to speculate (`SimplifyCFG.cpp:2339-2357`); a store or a call
     in the if-body is enough.
   A trivial `if (special) b = lang;` has an empty if-body and one PHI,
   so it always folds. `o4` (exactly that) measures 0.72273 / 3327.

## 3. Register-allocation chain (the real remaining diff)

| value | original | HEAD |
|---|---|---|
| `i` | `r9` (saved by `mov r9,r8`, spilled to `[sp,#80]` across `getText`, restored into `r8`) | `r8` (never spilled) |
| `Globals::font` value | `r5` | `fp` |
| `fontLangSelect` value | `r8` | `r6` |
| `&Globals::fontLangSelect` | spilled to `[sp,#76]` | `r9` |
| `this->buttonWidth` | `fp` (loaded from `[sp,#72]`) | stack `[sp,#80]` |
| spacing / button | `r4` / `r6` | `r4` / `r5` |

Both allocations use six callee-saved registers (`r4,r5,r6,r8,r9,fp`),
so this is not a register-pressure difference in count; it is which of
`{i, &fontLangSelect, buttonWidth}` wins a register. The original gives
`i` and `buttonWidth` registers and spills the pointer; HEAD gives `i`
and the pointer registers and spills `buttonWidth`.

Consequences visible in the diff:

* second select orientation: original `moveq r5, r8` (dest = font reg,
  `eq ? lang : font`), HEAD `moveq r6, fp` (dest = lang reg,
  `eq ? font : lang`);
* `fontId` store: original 4 instructions
  `ittt eq; ldreq [sp,#76]; ldreq [r0]; streq [r6,#8]`, HEAD 3
  instructions `itt eq; ldreq [r9]; streq [r5,#8]`.

### Forcing the pointer to memory

`int *volatile specialFontPtr` declared inside the loop (`v1`) makes
clang keep the pointer in a stack slot and reproduces the original
4-instruction store shape **exactly**:

```
ittt eq
ldreq  r0, [sp, #348]
ldreq  r0, [r0, #0]
streq  r0, [r5, #8]
```

but the surrounding allocation changes (`sl` becomes the select result
register, `r9` becomes the condition), the loop is 63 instructions, and
the function is 3317 instructions / **0.90372**. Declaring the volatile
pointer outside the loop (`v3`) gives 3319 / 0.90586. So the store shape
is necessary but not sufficient: the whole RA has to fall out of the
same IR, and `volatile` perturbs it in the wrong direction.

## 4. Variants measured this pass (exact difflib ratio, official verifier)

All are full-TU rebuilds with the exact NDK r18b flags. `orig` is always
3320 instructions.

| variant | source change (loop body unless noted) | ratio | ours |
|---|---|---|---|
| **HEAD** | swapped second select, pointer local, `optionByte` | **0.9424698795180723** | 3320 |
| `p1` | `langValue = *specialFontPtr`, ctor arg `*specialFontPtr` | 0.7314285714285714 | 3330 |
| `p2` | `langValue` global, ctor arg `*specialFontPtr` | 0.7314285714285714 | 3330 |
| `p3` | `langValue = *specialFontPtr`, ctor arg local | 0.9424698795180723 | 3320 |
| `q1` | both selects `special ? lang : font` | 0.7227320595757485 | 3327 |
| `q2` | second condition `((i|1)==9)` | 0.7227320595757485 | 3327 |
| `q3` | second condition `(i==8||i==9)` | 0.7227320595757485 | 3327 |
| `o1` | `langValue` declared before `fontValue` | 0.9424698795180723 | 3320 |
| `o2` | `unsigned int spacingFont = ...; FontGetSpacing(spacingFont)` | 0.9424698795180723 | 3320 |
| `o3` | `int` font, casts on both select operands | 0.9424698795180723 | 3320 |
| `o4` | real `if (special) buttonFont = lang;` (1 PHI) | 0.7227320595757485 | 3327 |
| `r1` | `fontId` store after `ArrayAdd` | 0.9182105738816086 | 3319 |
| `r2` | `specialFontPtr` declared inside loop | 0.9424698795180723 | 3320 |
| `r3` | store `Globals::fontLangSelect` directly | 0.9424698795180723 | 3320 |
| `r4` | store `langValue` (register, no reload) | 0.9161893277057582 | 3314 |
| `r5` | both selects read `Globals::fontLangSelect` directly | 0.7314285714285714 | 3330 |
| `r6` | ctor arg reads `Globals::fontLangSelect` directly | 0.7314285714285714 | 3330 |
| `u1` | `unsigned int langValue` | 0.9424698795180723 | 3320 |
| `u2` | ctor condition `special` instead of `(i==9||i==8)` | 0.9424698795180723 | 3320 |
| `u3` | first condition `(i==9||i==8)` | 0.9424698795180723 | 3320 |
| `u4` | ctor select same orientation | 0.7227320595757485 | 3327 |
| `t_a` | `int special` | 0.9424698795180723 | 3320 |
| `t_b` | `special = (i==8||i==9)` | 0.9424698795180723 | 3320 |
| `w1/w2/w3` | hoist `languageTextIds[i]` / `languageIds[i]` into locals | 0.9424698795180723 | 3320 |
| `x1/x2` | separate `unsigned`/`int` loop variable `j` | 0.9424698795180723 | 3320 |
| `lt10` | `for (i = 0; i < 10; i++)` | 0.9421686746987952 | 3320 |
| `v1` | `int *volatile specialFontPtr` inside loop | 0.9037215609462106 | 3317 |
| `v2` | `volatile int *specialFontPtr` | 0.9424698795180723 | 3320 |
| `v3` | `int *volatile specialFontPtr` outside loop | 0.9058593161620726 | 3319 |
| `v4` | `v3` + `i < 10` | 0.9055580659737912 | 3319 |
| `z1` | `setAlwaysPressed(Globals::options[13/14])` | 0.9050346698824239 | 3314 |
| `z2` | `setAlwaysPressed(optionFlag(13/14))` | 0.9050346698824239 | 3314 |
| `z3` | `setAlwaysPressed(optionByte(13/14) != 0)` | 0.9424698795180723 | 3320 |
| `z1lt` | `z1` + `i < 10` | 0.9047331926439554 | 3314 |
| `p1lt` | `p1` + `i < 10` | 0.7311278195488722 | 3330 |
| `aa1/aa2` | reload `fontLangSelect` / `font` between the selects | 0.7319711538461539 | 3336 |
| `y1..y4` | `row + 2` precomputed / reordered | 0.9424698795180723 | 3320 |
| `s2a` | sliders[2] regrouped as `(0x20_top+leftMargin+listEntryWidth+rowHeight*3)+0x2fc+sliderRowHeight+(0x15c+0x1c+rowHeight)*2` | 0.9379518072289157 | 3320 |
| `s2b` | `s2a` with `field_0x2fc` first | 0.9388554216867470 | 3320 |
| `s2c` | split float additions | 0.9084888621312462 | 3324 |

A full sweep of **214** existing full-TU variants in `/dev/shm/w4228`
(including every variant from passes 2-4) found no ratio above
`0.9424698795180723`; all 0.94247 variants are byte-identical to HEAD.

## 5. Other known diffs (unchanged, for the next pass)

* `+3` pre-loop ordering at index 357: original computes
  `adds r4,r5,#2` before `getText(395)`; ours after. Precomputing
  `row + 2` (`y1`), `row + 1 + 1` (`y2`), `2 + row` (`y3`) or hoisting
  it out of the `if` (`y4`) is a no-op at IR level.
* `setAlwaysPressed(optionByte(13/14))` at source lines 710/724 emits
  `cmp r1,#0; it ne; movne r1,#1` (6 instructions) that the original
  does not have. The faithful form is `Globals::options[13/14]` or
  `optionFlag(...)` (`z1`/`z2`), but removing the 6 instructions
  re-colours the stack frame and drops the ratio to 0.905. HEAD is
  accidentally size-balanced by these 6 extra instructions plus the 4
  missing loop spills.
* Slider `setPosition` blocks (indices ~2495, ~2622): the original
  schedules the `[r0,#12]/[r0,#28]/[r0,#40]` layout loads before and
  interleaved with the `[sl,#340]/[sl,#348]/[sl,#356]` member loads;
  ours hoists all `[sl,...]` loads. `s2a`/`s2b` reproduce the original
  *expression tree* (verified by the same add/sub structure) but not
  the scheduler order, and both are net regressions.
* Stack slots: original `[sp,#60]`/`[sp,#72]` vs ours
  `[sp,#64]`/`[sp,#80]` for the `Globals::layout` and
  `&this->buttonWidth` spills; `[sp,#68]`/`[sp,#76]`/`[sp,#80]` shift
  correspondingly. This is a stack-colouring consequence of the live
  ranges above.
* Extra `nop` before the first literal pool (index 915 in ours): pool
  alignment padding; it disappears only when the preceding code size
  matches.
* `cinematicBtnB` (index 1535): ours hoists `Globals::gameText` before
  `operator new`; the original loads it after. Removing the
  `GameText *cinematicText` local is worse (0.91775).

## 6. What would close the loop

A source shape where **all** of the following hold at once:

1. the second value is produced by a two-entry PHI that survives
   `SimplifyCFG` (needs a non-speculatable instruction in the if-body or
   four PHIs in the merge block), so ARM `IfConverter` emits the
   `moveq` from a `PHI` instead of a DAG `select`; and
2. the resulting IR lets the register allocator pick
   `i -> r9`, `lang -> r8`, `&fontLangSelect -> [sp,#76]`,
   `buttonWidth -> fp`.

`int *volatile` proves (2) is reachable from the store side but breaks
(1) and the rest of the allocation. The most promising untried construct
is a real intervening use that makes the if-body non-speculatable while
still compiling to no extra loop instructions (e.g. a dead-but-not-DSE'd
store that SimplifyCFG sees before DSE, or a `volatile` read that is
CSE'd later). Nothing in the current reconstruction offers such a use
without adding an anchor, so this pass lands no code change.
