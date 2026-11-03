# MenuTouchWindow C1/C2 pass 4: the two-value-select mechanism, and the
# swapped-operand shape that finally reproduces it (94.079 -> 94.2%, 9600 B)

**Status:** code change landed, 2026-11-03. Worktree `wt/w4116`, branch
`wave91-w4116`, base `a4557300`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). The two-select mechanism is understood and the real TU now
emits the original loop's two `cmp.w sl,#9; it eq; moveq` value selects. The
landed shape is the swapped-operand second select plus removal of the six
`__builtin_prefetch` anchors: **94.079 -> 94.2%**, still 9600 B. The second
select is still `eq ? font : lang` (original: `eq ? lang : font`), so the
function is not byte-identical yet; `byte_exact` stays 4498.

## TL;DR

* Landed: `_ZN15MenuTouchWindowC1Ei` / `C2` are **9600 B / 94.2%** (was
  94.078650%; original 9600 B / 3320 insns). Gate unchanged:
  `byte_exact 4498 / linked_exact 4498`, `data 843/843`, imports 207,
  `.text 0x1337c8`, `.rodata 0x41b67`, lint CLEAN, drift 0.
* Pass 3 proved that `EarlyCSE` merges the two identical
  `select i1 %special, i32 %lang, i32 %font` expressions. This pass proves
  **which** two selects survive: two selects that differ **only by swapped
  operands** (`select %c, %lang, %font` and `select %c, %font, %lang`) are
  *not* merged by GVN and both lower to `cmp; it eq; moveq`. This is the
  first source shape that reproduces the original loop's two
  `cmp.w sl,#9; it eq; moveq` sequences in the real TU (measured 91.2%,
  size 9624 B; loop is structurally correct).
* The original's two selects are **same-orientation** (`eq ? lang : font`
  twice). Reproducing that pair requires either a PHI that survives both
  SimplifyCFG *and* the second `Early CSE w/ MemorySSA`, or two DAG operands
  that differ but coalesce to the same registers. Neither was found; the
  PHI route is blocked because the late SimplifyCFG folds the PHI to a
  select and the second EarlyCSE then merges it with the first select.
* Landed source shape: keep `int *specialFontPtr = &Globals::fontLangSelect;`
  and `optionByte(13/14)`, delete all six `__builtin_prefetch` anchors, and
  use value locals with a swapped-operand second select:

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
  ```

  This keeps two selects alive (`select %c, lang, font` and
  `select %c, font, lang`), so the loop emits two `cmp.w sl,#9; moveq`.
  `byte_exact` stays **4498** because the function is still not identical.

## 1. Original language loop (verbatim, VMA 0x1233fa..0x1234ae)

Real addresses from `arm-linux-androideabi-objdump`; the pass-3 note's
`0a5e..0b12` are the same instructions relative to the function start
(`_ZN15MenuTouchWindowC1Ei` = 0x12299c, size 9600 = 0x2580).

```
1233fa  orr.w   sl, r8, #1          ; special = (i | 1) == 9
1233fe  ldr     r0, [pc, #720]      ; GOT &Globals::font
123400  mov     r9, r8              ; save i
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
12342e  ldr.w   fp, [r0]
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
12348e  ldreq   r0, [sp, #76]       ; &fontLangSelect
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

Key facts (all re-verified this pass):

* `r5 = Globals::font` and `r8 = Globals::fontLangSelect` are each loaded
  **once** and are live across `FontGetSpacing`, `operator new` and
  `getText`. Both selects read the same two values.
* Both selects are **`eq ? lang : font`**: select #1 is
  `mov r1, r5; moveq r1, r8` (r5=font, r8=lang); select #2 is
  `moveq r5, r8` (r5 still font, r8 still lang). Same condition, same
  operands, same orientation.
* The `fontId` store reloads `Globals::fontLangSelect` through the spilled
  address `[sp,#76]`; it does not use `r8`. The source therefore references
  `Globals::fontLangSelect` directly in the `if` (a fresh load), while the
  two selects use value locals.
* Three `cmp.w sl,#9` (0x123404, 0x123450, 0x123476); the `orr.w sl,r8,#1`
  is computed once. The flags are clobbered by the calls, so each use
  re-emits the compare.

Ghidra's decompilation (`DecFn.java 13299c`, cached at
`/dev/shm/w4116/dec.c`) agrees:

```
uVar42 = uVar37 | 1;
uVar32 = **(uint **)(iRam001336d0 + 0x13340e);   // Globals::font
uVar38 = *puVar13;                                // Globals::fontLangSelect
uVar23 = uVar32;
if (uVar42 == 9) uVar23 = uVar38;                 // select #1
iVar29 = FontGetSpacing(Canvas, uVar23);
...
if (uVar42 == 9) uVar32 = uVar38;                 // select #2
TouchButton::TouchButton(..., uVar32, iVar29);
...
if (uVar42 == 9) *(uint *)(pTVar7 + 8) = *puVar13; // fontId (fresh load)
```

## 2. The mechanism: swapped-operand selects survive GVN

Pass 3 established that `EarlyCSE` merges two **identical** selects
(`isIdenticalToWhenDefined` ignores debug locations). This pass found the
first shape that survives: two selects with the **same condition** but
**swapped operands**.

Standalone repro (exact TU flags, NDK r18b clang 7.0.2, `-Oz`):

```cpp
extern int ext(int); extern int ext2(int,int);
int f(int i,int font,int lang){
  bool s=(i|1)==9;
  int a=s?lang:font;            // select %s, lang, font
  int t=ext(a);
  int b=(i==9||i==8)?font:lang; // select %s', font, lang
  return ext2(b,t);
}
```

Final IR (`clang -Oz -S -emit-llvm`; GVN canonicalises `(i==9||i==8)` to the
same `%5` but keeps the swapped operands):

```llvm
%4 = or i32 %0, 1
%5 = icmp eq i32 %4, 9
%6 = select i1 %5, i32 %2, i32 %1     ; lang, font
%7 = tail call i32 @_Z3exti(i32 %6)
%8 = select i1 %5, i32 %1, i32 %2     ; font, lang
%9 = tail call i32 @_Z4ext2ii(i32 %8, i32 %7)
```

Assembly:

```
 8: orr.w   r6, r0, #1
 c: mov     r0, r1
 e: cmp     r6, #9
10: it      eq
12: moveq   r0, r2          ; select #1 (eq ? lang : font)
14: mov     r4, r2
16: mov     r5, r1
18: bl      ext
1c: cmp     r6, #9
1e: it      eq
20: moveq   r4, r5          ; select #2 (eq ? font : lang)
```

Two `cmp; it eq; moveq` sequences. The DAG CSE does not merge the selects
because the operand order differs; the ARM backend inverts the second
predicate so both still emit `moveq`.

Control experiment: if the second select is written with the **same**
orientation (`(i==9||i==8)?lang:font`), GVN value-numbers the two selects as
one and the function has a single `moveq` (verified). If the second
condition is a different expression that lowers to a *different* compare
(e.g. `(i&~1)==8` -> `bic;cmp r,#8`), two selects survive but the second
compare is wrong.

## 3. Real-TU measurements (official verifier, C1 row)

`evalone.sh <src>` copies the file into the tree, relinks and runs
`tools/verify/verify.py`. All rows use the faithful no-anchor source
(`mtw_w1.cpp` base: no `__builtin_prefetch`, plain `Globals::options[...]`).

| variant | second-select source | size | official pct | loop `cmp.w sl,#9` | loop `moveq` |
|---|---|---:|---:|---:|---:|
| HEAD | address select (`*specialFontPtr` / `Globals::font`), anchors | 9600 | **94.1** | 1 | 0 |
| W1 (pass 3) | `special ? langValue : fontValue`, `fontId = langValue` | 9560 | 73.5 | 1 | 1 |
| q1 | `(i==9||i==8) ? langValue : fontValue` | 9572 | -- | 0 | 0 |
| **q2** | `(i==9||i==8) ? fontValue : langValue` | **9624** | **91.2** | **2** | **2** |
| s1 | first select swapped + `(i==9||i==8) ? langValue : fontValue` | 9620 | 91.2 | 2 | 2 |
| q2b | q2 with `fontId = *specialFontPtr` | 9624 | -- | 2 | 2 |
| q2c | q2 with `fontId = langValue` | 9608 | 90.4 | 1 | 1 |
| `i>=8&&i<=9` | `(i>=8&&i<=9) ? langValue : fontValue` | 9600 | 76.5 | 0 (`bic;cmp`) | 2 |
| **`v_headq2_nopf` (landed)** | HEAD + value locals, second select `(i==9\|\|i==8) ? fontValue : langValue`, `fontId = *specialFontPtr`, no prefetch anchors | **9600** | **94.2** | **2** | **2** |

q2's loop (side by side with the original) is structurally the closest
source-level match found so far: two value selects, both `cmp.w sl,#9`,
both `moveq`, `font`/`lang` both live across the calls, and the `fontId`
store reloading the global. It is still 24 B too long and the second select
is `eq ? font : lang` (destination = lang register) instead of the
original's `eq ? lang : font` (destination = font register).

Measured q2 loop:

```
orr.w   sl, r8, #1
cmp.w   sl, #9
ldr     r0, [pc, #712]
ldr     r1, [pc, #708]
ldr     r2, [pc, #712]
add     r0, pc
add     r1, pc
add     r2, pc
ldr     r0, [r0, #0]
ldr     r1, [r1, #0]
ldr.w   r9, [r2]            ; &fontLangSelect
ldr.w   fp, [r0]            ; font
ldr     r0, [r1, #0]        ; Canvas
ldr.w   r6, [r9]            ; lang
mov     r1, fp
it      eq
moveq   r1, r6              ; select #1
bl      FontGetSpacing
...
cmp.w   sl, #9
it      eq
moveq   r6, fp              ; select #2  <-- eq ? font : lang
```

## 4. Why the same-orientation pair is still elusive

The original needs two selects that are both `eq ? lang : font`. Since
`EarlyCSE`/GVN merge identical selects, the two selects must differ in an
IR-visible way that the DAG then canonicalises to the same `moveq`:

1. **Different conditions, same operands.** The DAG merges the two
   conditions if they lower to the same `SETCC` node, so the selects merge
   again. `(i==9||i==8)` is value-numbered to `(i|1)==9` by GVN; the DAG
   then merges the pair (verified: one `moveq`).
2. **Different operands, same orientation.** Requires two SSA values that
   are copies of `font`/`lang` but not value-numbered equal. No source
   shape found; `int font2 = font;` etc. are copy-propagated.
3. **A surviving PHI.** A PHI is not a `SelectInst`, so it survives
   `EarlyCSE`; the ARM if-converter then emits a MOVCC that bypasses DAG
   CSE. This is the most likely original shape, but the late SimplifyCFG
   folds a one-PHI diamond to a select and the **second** `Early CSE w/
   MemorySSA` merges it with the first select.

PHI evidence (`g3.cpp`, a 4-PHI diamond used to try to defeat
`FoldTwoEntryPHINode`):

```
*** IR Dump After Combine redundant instructions ***
  %9  = select i1 %8, i32 %6, i32 %5
  %14 = phi i32 [ %6, %11 ], [ %5, %12 ]     ; PHI still alive
*** IR Dump After Simplify the CFG ***
  %9  = select i1 %8, i32 %6, i32 %5
  %11 = select i1 %8, i32 %6, i32 %5         ; PHI folded to a select
*** IR Dump After Early CSE w/ MemorySSA ***
  %9  = select i1 %8, i32 %6, i32 %5         ; merged back to one
```

The 4-PHI trick fails because InstCombine simplifies the three trivial
PHIs (`phi [x,x]`) before the late SimplifyCFG, leaving one PHI for
`FoldTwoEntryPHINode` to fold. A diamond whose merge block still has 4+
non-trivial PHIs at the late SimplifyCFG would produce a real if-converted
MOVCC; no such shape was found that does not add instructions.

## 5. Gate (unchanged)

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓      drift: 0 field(s) across 0 class(es)
.text 0x1337c8 @ 0x69440     .rodata 0x41b67 @ 0x1c9e10
```

## 6. Reproduction

Build setup (this worktree's default paths are wrong):

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8     # RATCHET PASS 4498/4498
```

`FN=<mangled> ... --target verify-fn` is broken in this CMake (the env var is
not expanded); call the verifier directly:

```sh
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
```

Scratch harnesses (in `/dev/shm/w4116`): `measure2.py` (single-TU compile +
disassemble + difflib), `dumploop.py` (extract the language loop),
`loopcmp.py`, `mkvar.py` (patch the loop body), `evalone.sh` (official
verifier C1 row). The q2 variant is `v_q2.cpp`; the standalone
swapped-operand repro is `e5.cpp`/`f1.cpp`.

## 7. What remains

1. Find a source shape whose two selects are both `eq ? lang : font` and
   survive GVN: most likely a PHI diamond with 4+ non-trivial PHIs at the
   late SimplifyCFG, or two DAG operands that coalesce.
2. Align the rest of the RA (the original keeps `i` in r9 and spills it to
   `[sp,#80]`; the landed shape keeps it in r8) and make the second select
   same-orientation; only then can the ctor be byte-identical.
3. The six `__builtin_prefetch` anchors are now deleted (landed). The last
   two `optionByte(13/14)` sites remain; delete them only once the ctor is
   3320 insns / 9600 B byte-identical.
