# MenuTouchWindow C1/C2 pass 3: the value-select mechanism, and why it still does not
# reproduce the original's two selects (94.079%, 9600 B; no code change)

**Status:** analysis-only pass, 2026-11-03. Worktree `wt/w4016`, branch
`wave90-w4016`, base `360c7467`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). No source change was landed: the faithful value-select
shape is now understood and measured, but it does not raise `byte_exact`
(the ctor does not become instruction-identical), so the note is the
deliverable.

## TL;DR

* HEAD (`360c7467`) is unchanged: `_ZN15MenuTouchWindowC1Ei` / `C2` are
  **9600 B / 3317 insns / 94.078650%** (original 9600 B / 3320 insns).
  Gate: `byte_exact 4498 / linked_exact 4498`, `data 843/843`,
  imports 207, `.text 0x1337c8`, `.rodata 0x41b67`, lint CLEAN, drift 0.
* The language loop's **value select** is produced by LLVM's
  `DAGCombiner::SimplifySelectOps` (`llvmorg-7.0.1`,
  `llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp:17604`). It rewrites
  `select(load A, load B)` -> `load(select A, B)` **only when both loads
  have exactly one use**:
  ```cpp
  if (LHS.getOpcode() != RHS.getOpcode() ||
      !LHS.hasOneUse() || !RHS.hasOneUse())
    return false;
  ...
  if (LHS.getOpcode() == ISD::LOAD) {
    ...
    Addr = DAG.getSelect(... LLD->getBasePtr(), RLD->getBasePtr());
    Load = DAG.getLoad(TheSelect->getValueType(0), ..., Addr, ...);
  }
  ```
  Our baseline selects **addresses** (`ldrne r9,[&font]; ldr r1,[r9]`)
  because clang's IR has a single `select i1 %special, i32 %lang, i32 %font`
  whose two loads each have one use.
* The original selects **values**:
  `ldr r5,[font]; ldr.w r8,[lang]; mov r1,r5; it eq; moveq r1,r8` and,
  after `getText`, `cmp sl,#9; it eq; moveq r5,r8`. Both loads are live
  across the calls, so each must have **more than one use** in the DAG.
* The faithful source shape that blocks the fold is
  `language->fontId = langValue;` (the value local, not
  `Globals::fontLangSelect` and not `*specialFontPtr`): `%lang` then has
  two uses (select + store), so `SimplifySelectOps` bails and the select
  stays a value select. Measured: **3315 insns, 73.4589%** (one value
  select in the loop; the second use is kept in a register instead of
  being rematerialised).
* The original has **two** value selects (two `moveq` in the loop). Our
  build still CSEs the two identical select expressions into one
  (`EarlyCSE` merges them; `isIdenticalToWhenDefined` ignores debug
  locations), so the second select is not re-materialised. Getting two
  selects requires a source shape whose two select expressions are not
  CSE-able; none of the 48 operand/store combinations tried produced it
  while keeping the original's single load of each global.
* `byte_exact` therefore stays **4498**; the ctor is not instruction-exact.
  No anchors were added or removed.

## 1. Original language loop (verbatim, VMA 0x0a5e-0x0b12)

```
0a5e orr.w   sl, r8, #1          ; special = (i | 1) == 9
0a62 ldr     r0, [pc, #720]      ; GOT &Globals::font
0a64 mov     r9, r8              ; save i
0a66 ldr     r1, [pc, #720]      ; GOT &Globals::Canvas
0a68 cmp.w   sl, #9
0a6c ldr     r2, [pc, #716]      ; GOT &Globals::fontLangSelect
0a6e add     r0, pc
0a70 add     r1, pc
0a72 add     r2, pc
0a74 ldr     r0, [r0, #0]        ; &font
0a76 ldr     r1, [r1, #0]        ; &Canvas
0a78 ldr     r2, [r2, #0]        ; &fontLangSelect
0a7a ldr     r5, [r0, #0]        ; font value
0a7c ldr     r0, [r1, #0]        ; Canvas value
0a7e ldr.w   r8, [r2]            ; fontLangSelect value
0a82 mov     r1, r5
0a84 str     r2, [sp, #76]       ; save &fontLangSelect
0a86 it      eq
0a88 moveq   r1, r8              ; select #1 (value) -> FontGetSpacing
0a8a blx     PaintCanvas::FontGetSpacing(unsigned int)
0a8e mov     r4, r0              ; spacing
0a90 ldr     r0, [sp, #72]
0a92 ldr.w   fp, [r0]
0a96 movs    r0, #200
0a98 blx     operator new(unsigned int)
0a9c mov     r6, r0              ; TouchButton *
0a9e ldr     r0, [pc, #672]
0aa0 str.w   r9, [sp, #80]       ; spill i
0aa4 add     r0, pc
0aa6 ldr.w   r1, [r0, r9, lsl #2]   ; languageTextIds[i]
0aaa ldr     r0, [sp, #96]
0aac ldr     r0, [r0, #0]
0aae blx     GameText::getText(int)
0ab2 mov     r1, r0
0ab4 cmp.w   sl, #9
0ab8 it      eq
0aba moveq   r5, r8              ; select #2 (value) -> TouchButton ctor
0abc ldr     r0, [sp, #84]
0abe movs    r2, #0
0ac0 strd    r0, fp, [sp]
0ac4 movs    r0, #17
0ac6 str     r0, [sp, #8]
0ac8 movs    r0, #4
0aca strd    r0, r5, [sp, #12]   ; ctor arg 9 = selected font
0ace mov     r0, r6
0ad0 ldr     r3, [sp, #88]       ; spacing
0ad2 str     r4, [sp, #20]
0ad4 blx     TouchButton::TouchButton(String const&, int, int, int, int,
                                      unsigned char, unsigned char,
                                      unsigned int, int)
0ad8 ldr     r0, [pc, #616]
0ada cmp.w   sl, #9
0ade ldr.w   r8, [sp, #80]       ; restore i
0ae2 add     r0, pc
0ae4 ldr.w   r0, [r0, r8, lsl #2]   ; languageIds[i]
0ae8 mov.w   r1, r0, asr #31
0aec strd    r0, r1, [r6]        ; language->id = ...
0af0 ittt    eq
0af2 ldreq   r0, [sp, #76]       ; &fontLangSelect
0af4 ldreq   r0, [r0, #0]        ; reload value
0af6 streq   r0, [r6, #8]        ; language->fontId
0af8 ldr.w   fp, [sp, #108]
0afc mov     r0, r6
0afe ldr.w   r1, [fp, #176]
0b02 blx     ArrayAdd<TouchButton*>(...)
0b06 add.w   r8, r8, #1
0b0a ldr.w   r9, [sp, #92]
0b0e cmp.w   r8, #10
0b12 bcc.w   0x9aa
```

Key observations:

* `r5 = Globals::font` and `r8 = Globals::fontLangSelect` are each loaded
  **once** and are live across `FontGetSpacing`, `operator new` and
  `getText`. Both selects read the same two values.
* The `fontId` store reloads `Globals::fontLangSelect` through the spilled
  address `[sp,#76]`; it does not use `r8`. The source therefore references
  `Globals::fontLangSelect` directly in the `if` (a fresh load), while the
  two selects use value locals.
* Two `cmp.w sl,#9; it eq; moveq` sequences exist (0a86-0a88 and
  0ab4-0aba). Both are value selects. `moveq` count over the whole
  function: original **12**, HEAD anchor state **10**, `fontId = langValue`
  shape **11**.

## 2. Standalone repro of the fold and of the value select

Compiled with the exact TU flags
(`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong
-funwind-tables -Oz`), NDK r18b clang 7.0.2.

**Folded (address select)** -- two identical selects, each load one use:

```cpp
unsigned int fontValue = Globals::font;
int langValue = Globals::fontLangSelect;
int a = ext(special ? (unsigned int)langValue : fontValue);
int b = ext2(special ? (unsigned int)langValue : fontValue);
```
```
ldr r4,[&lang]       ; r4 = &lang
ldr r5,[&font]       ; r5 = &font
ldr r5,[r5]          ; font value
ldr r0,[r4]          ; lang value
it ne
movne r0,r5          ; value select for the first use
...
ldr r0,[r4]          ; second use reloads lang
it ne
movne r0,r5
```

**Not folded (value select)** -- the lang value has a second use, so the
loads are not one-use and `SimplifySelectOps` bails:

```cpp
unsigned int fontValue = Globals::font;
int langValue = Globals::fontLangSelect;
int spacing = ext(special ? (unsigned int)langValue : fontValue);
TB *t = new TB(0, special ? (unsigned int)langValue : fontValue, spacing);
if (special) t->b = langValue;          // second use of %lang
```
```
ldr r5,[font]
ldr r6,[lang]
orr r0,r4,#1
cmp r0,#9
mov r0,r5
it eq
moveq r0,r6          ; value select, font/lang stay live
bl ext
...
cmp r0,#9
it eq
moveq r5,r6          ; (in the real function the RA keeps the
                     ;  selected value instead of re-selecting)
```

## 3. Variant sweep in the real constructor

All measurements use the official verifier
(`tools/verify/verify.py`, `difflib.SequenceMatcher(autojunk=False)`).
`moveq`/`ldrne`/`ldreq` are whole-function counts from the NDK objdump
(original: `moveq 12  ldrne 0  ldreq 2`).

| variant | language-loop source shape | size | insns | ratio | moveq |
|---|---|---:|---:|---:|---:|
| HEAD | `specialFontPtr` + `(uintptr_t)Globals::font`, `fontId = *specialFontPtr`, 6 `__builtin_prefetch`, `optionByte(13/14)` | 9600 | 3317 | 94.0787 | 10 |
| F | same as HEAD but no prefetch, plain `Globals::options[13/14]` | 9564 | 3305 | 91.5925 | 10 |
| D | direct globals `special ? Globals::fontLangSelect : Globals::font`, `fontId = Globals::fontLangSelect` | -- | 3324 | 73.1788 | -- |
| S10 | both value locals, `fontId = *specialFontPtr` | 9536 | 3321 | 72.2783 | 10 |
| S29 | both value locals, `fontId = Globals::fontLangSelect` | 9572 | 3333 | 72.1780 | -- |
| W1 | both value locals, `fontId = langValue` | 9524 | 3315 | 73.4589 | 11 |
| V12 | both value locals with `(unsigned int)` on both operands, `fontId = langValue` | 9544 | 3315 | 73.4589 | 12* |
| L-H-G-H-l | first select value locals, second select direct globals, `fontId = langValue` | 9560 | -- | -- | 12* |

\* The extra `moveq` in V12 / L-H-G-H-l is not the loop's second value
select: the loop still has one `moveq` and the ctor consumes the selected
register directly. The 12th `moveq` is elsewhere in the function.

`fontId = langValue` (W1/V12) is the closest faithful shape: it is the
only one that produces the original's value select in the loop
(`it eq; moveq` instead of `it ne; ldrne`). It still CSEs the two select
expressions to one, and the RA keeps the selected value in a register
across the calls instead of re-materialising the select from the two
values.

## 4. Why two selects are still not reproduced

`EarlyCSE` merges the two identical selects. In the T7 pass log
(`-mllvm -print-after-all`) the two selects exist after `SROA`:

```
%864 = select i1 %863, i32 %861, i32 %860   ; before FontGetSpacing
%877 = select i1 %876, i32 %861, i32 %860   ; after getText
```

and `Early CSE` folds them to one (`%773 = select i1 %768, i32 %771,
i32 %770`). `EarlyCSE` uses `isIdenticalToWhenDefined`, which ignores
debug locations and uses a dominator-tree scoped value table, so two
selects with the same condition and operands merge across the invoke
boundaries.

The original's two selects have the same condition (`cmp.w sl,#9`) and
the same operands (`r5 = font`, `r8 = lang`), so they would be CSE-able
too. The remaining possibilities are:

1. The original's second select is a **PHI** (from an `if`/`else`) that
   the ARM if-converter lowered to `cmp; it eq; moveq`, not a `select`.
   `EarlyCSE` does not merge a PHI with a select. All plain `if`/`else`
   shapes tried (X1-X5, U1-U4) were converted to selects by
   `SimplifyCFG` and then folded.
2. The original's two select expressions differ in an IR-visible way
   (different condition SSA value or different operand SSA values) that
   still coalesces to the same registers. No such source shape was found.
3. The register allocator re-materialises the single select. ARM does
   not override `isReallyTriviallyReMaterializable` for `MOVCC`, and the
   generic check rejects virtual-register uses, so this is unlikely.

## 5. Remaining instruction budget (open)

With the faithful no-anchor source the ctor is **3305** instructions;
the original is **3320** (15 short). Region breakdown (from the official
verifier's normalized stream):

| region | net orig-ours |
|---|---:|
| language loop (indices ~934-1000) | **+9** |
| pre-loop button setup (~357-933) | +3 |
| slider[2] / iPad position block (~2495-2515) | +1 |
| ~2791 | +1 |
| ~2810/2826 | -2 |
| ~2913-2964 | +3 |
| ~2987 | -1 |
| ~3016-3040 | +2 |
| EH landing-pad dispatch (~3137-3154) | 0 |

The pre-loop +3 is the `row + 2` argument of the mission-154/157/158
button: the original computes `adds r4,r5,#2` before
`GameText::getText(395)`, ours computes it after the String temporary is
built. Rewriting it as `int row2 = row + 2;` (before or inside the `if`)
compiles to the same bytes as the baseline; it is a scheduler/RA
difference. The slider block is the same phenomenon (different load
order of `Globals::layout` / `this` fields, identical arithmetic).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4016
cmake --build cmake-build-match --target check -j8   # RATCHET PASS 4498/4498
# faithful no-anchor source:
#   remove the six __builtin_prefetch lines and replace optionByte(13/14)
#   with Globals::options[13/14] in src/game/ui/MenuTouchWindow.cpp
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
```

## 7. Gate (unchanged)

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓      drift: 0 field(s) across 0 class(es)
.text 0x1337c8 @ 0x69440     .rodata 0x41b67 @ 0x1c9e10
```
