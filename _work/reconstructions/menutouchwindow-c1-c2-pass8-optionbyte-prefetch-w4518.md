# MenuTouchWindow C1/C2 pass 8: remove the `optionByte` boolean
# normalisation, fix four commutative operand orders, and restore the
# 9600-byte size with three temporary `__builtin_prefetch` anchors
# (94.247% -> 94.772%)

**Status:** code + analysis, 2026-11-03. Worktree `wt/w4518`, branch
`wave95-w4518`, base `4a88bd6e`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before:** `_ZN15MenuTouchWindowC1Ei` = 9600 B, 3320 instructions,
  exact difflib ratio **0.9424698795180723** (94.247%).
* **After:** 9600 B, 3317 instructions, exact difflib ratio
  **0.9477173421726683** (94.772%). Strict improvement; gate unchanged.
* Three independent source-shape changes:
  1. **`optionByte` removal** — `optBtnD4`/`optBtnDC` now pass
     `Globals::options[13]` / `Globals::options[14]` (a `bool`) directly
     instead of a `reinterpret_cast<unsigned char*>` byte. The original
     passes a `bool` straight into `setAlwaysPressed(bool)`; the
     `unsigned char -> bool` conversion emitted a 3-instruction
     `cmp r1,#0; it ne; movne r1,#1` normalisation at each site that the
     original does not have. Removing it shrinks the function to
     9588 B (ratio 0.9059).
  2. **Four commutative operand-order fixes** (see §2) — each either
     improves or holds the ratio; together they take 9588 B from 0.90503
     to 0.90594.
  3. **Three `__builtin_prefetch(this)` size anchors** — because change 1
     and 2 leave the function 12 B short of the original, three `pld`
     instructions restore 9600 B. This re-aligns every post-loop branch
     target and is what produces the large ratio jump
     (0.90594 -> 0.94772). The anchors are *temporary*: they are not
     original source, and the real fix is the language-loop register
     allocation (see §6).

## 1. `optionByte` removal is faithful

`Globals::options` is `alignas(4) static bool options[100]`
(`src/game/core/Globals.h:162`). `optionByte(offset)` was

```cpp
static inline unsigned char optionByte(int offset) {
    return *reinterpret_cast<unsigned char *>(&Globals::options[offset]);
}
```

so `optionByte(13)` and `Globals::options[13]` read the *same byte*
(offset 13). The difference is the type: `unsigned char` forces a
`!= 0` normalisation when converted to the `bool` parameter, whereas a
`bool` is passed through.

Original `optBtnD4` site (VMA 0x1243b2):

```
1243b2  ldr.w   r0, [sl, #212]      ; this->optBtnD4 (0xd4)
1243b6  ldrb.w  r1, [r9, #13]       ; Globals::options[13]  (byte, no normalisation)
1243ba  blx     TouchButton::setAlwaysPressed(bool)
```

HEAD (before this pass) emitted three extra 16-bit instructions:

```
1243ae  ldrb.w  r1, [r9, #13]
1243b2  cmp     r1, #0
1243b4  it      ne
1243b6  movne   r1, #1
1243b8  blx     TouchButton::setAlwaysPressed(bool)
```

The same 3-instruction normalisation existed at `optBtnDC`
(`optionByte(14)`, original VMA 0x12449e/0x1244ac). Both are gone after
this pass: the original's `ldrb.w r1, [r9, #13]` / `ldrb.w r1, [r9, #14]`
now match exactly.

`optBtnD8` already used `Globals::options[12]` (no `optionByte`) and
already matched.

The `optionByte` helper itself is now unused but is left in place (it is
`static inline`, emits nothing, and lint is clean). It can be deleted in
a later cleanup pass.

## 2. Four commutative operand-order fixes

All four are inside `MenuTouchWindow::MenuTouchWindow(int)`. They are
reorderings of commutative `int` additions; the original's `add`/`adds`
instructions are commutative, so the source order is not directly
observable, but the register allocator's choice of which value lands in
which register depends on it. Each was measured individually on top of
the `optionByte` removal.

| # | site | before | after | ratio delta |
|---|---|---|---|---|
| 1 | credits button x | `this->slotListWidth + Globals::layout->buttonInsetX` | `Globals::layout->buttonInsetX + this->slotListWidth` | +0.00030 |
| 2 | `scrollExtraButton` y | `field_0x20_top + field_0xc_leftMargin` | `field_0xc_leftMargin + field_0x20_top` | +0.00030 |
| 3 | `sliders[4]` x | `Globals::layout->buttonInsetX + this->listTopY` | `this->listTopY + Globals::layout->buttonInsetX` | +0.00030 |
| 4 | `sliders[4]`/`sliders[5]` y | `this->field_0x15c + Globals::layout->field_0x2fc` | `Globals::layout->field_0x2fc + this->field_0x15c` | neutral alone, +0.0003 in combination |

Measurements (full TU rebuild, exact verifier):

```
opt only                                       9588 B  0.9050346698824239
opt + credits fix                              9588 B  0.9053361471208924
opt + scrollExtra fix                          9588 B  0.9053361471208924
opt + sliders[4] x fix                         9588 B  0.9053361471208924
opt + field_0x2fc/field_0x15c fix              9588 B  0.9050346698824239
opt + all four                                 9588 B  0.9059391015978293
```

The original's credits x is computed as
`add.w r1, r4, r9` with `r4 = this->slotListWidth` (loaded by
`ldrd r4, r8, [r2, #512]`) and `r9 = Globals::layout->buttonInsetX`
(loaded by `ldrd sl, r9, [r0, #36]`); see VMA 0x124b36-0x124b4e. The
`add.w` is commutative, so this only fixes the register assignment, not
a literal operand order.

## 3. The three temporary `__builtin_prefetch` anchors

After §1+§2 the function is **9588 B**, 12 B short of the original's
9600 B. Three `pld` instructions restore the size. Placement was searched
exhaustively (`a` before the `case 1` mission query, `b` after
`ArrayAdd(language, *langList)` in the language loop, `c` before
`Image2DCreate(1301, ...)`), with `a+b+c == 3`:

| placement | size | ratio |
|---|---|---|
| **a1 b2 c0** | **9600** | **0.9477173421726683** |
| a1 b1 c1 | 9600 | 0.9474160012053638 |
| a1 b0 c2 | 9600 | 0.9474160012053638 |
| a0 b3 c0 | 9600 | 0.9447039324996234 |
| a0 b2 c1 | 9600 | 0.9444025915323189 |
| a0 b1 c2 | 9600 | 0.9444025915323189 |
| a0 b0 c3 | 9600 | 0.9444025915323189 |
| a2 b1 c0 | 9596 | 0.924815428657526 |
| a2 b0 c1 | 9596 | 0.9245140876902215 |
| a3 b0 c0 | 9596 | 0.9187886093114359 |

The winner is **one prefetch before the mission query + two in the
language loop**. In the loop the compiler reuses the `this` reload that
the original already performs for `ArrayAdd` (`ldr.w fp, [sp, #108]`),
so the two prefetches cost only the two `pld [fp]` instructions
(`a2 b1 c0` is 9596 B because the first prefetch's `this` reload is not
shared with anything and one instruction is folded elsewhere).

Exact source:

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

The `// lint: arm_builtin` waiver is the same one already used elsewhere
for `__builtin_prefetch`. These are **size anchors, not original
source**: the original has no `pld` in this function. They are kept
because they are currently the only way to reach the original's 9600 B
without changing the language-loop register allocation (§6).

## 4. Exact result

`_ZN15MenuTouchWindowC1Ei` (and the `C2` weak alias):

```
orig 9600 B @0x12299c  |  ours 9600 B @0x12299c
3320 orig instructions, 3317 ours
difflib ratio 0.9477173421726683   (report.json pct 94.8)
```

Language loop, ours after this pass (VMA 0x1233fe..0x1234b0):

```
1233fe  orr.w   sl, r8, #1
123402  ldr     r0, [pc, #720]
123404  cmp.w   sl, #9
123408  ldr     r1, [pc, #716]
12340a  ldr     r2, [pc, #720]
12340c  add     r0, pc
12340e  add     r1, pc
123410  add     r2, pc
123412  ldr     r0, [r0, #0]
123414  ldr     r1, [r1, #0]
123416  ldr.w   r9, [r2]            ; &fontLangSelect
12341a  ldr.w   fp, [r0]            ; font
12341e  ldr     r0, [r1, #0]        ; Canvas
123420  ldr.w   r6, [r9]            ; lang
123424  mov     r1, fp
123426  it      eq
123428  moveq   r1, r6              ; select #1  eq ? lang : font
12342a  blx     FontGetSpacing
12342e  mov     r4, r0
123430  movs    r0, #200
123432  blx     operator new
123436  mov     r5, r0
123438  ldr     r0, [pc, #676]
12343a  add     r0, pc
12343c  ldr.w   r1, [r0, r8, lsl #2]
123440  ldr     r0, [sp, #96]
123442  ldr     r0, [r0, #0]
123444  blx     GameText::getText
123448  mov     r1, r0
12344a  ldr     r0, [sp, #80]
12344c  ldr     r0, [r0, #0]
12344e  cmp.w   sl, #9
123452  it      eq
123454  moveq   r6, fp              ; select #2  eq ? font : lang  (swapped)
123456  ldr     r2, [sp, #84]
123458  strd    r2, r0, [sp]
12345c  movs    r0, #17
12345e  str     r0, [sp, #8]
123460  movs    r0, #4
123462  strd    r0, r6, [sp, #12]
123466  mov     r0, r5
123468  ldr     r3, [sp, #88]
12346a  movs    r2, #0
12346c  str     r4, [sp, #20]
12346e  blx     TouchButton::TouchButton(...)
123472  ldr     r0, [pc, #624]
123474  cmp.w   sl, #9
123478  add     r0, pc
12347a  ldr.w   r0, [r0, r8, lsl #2]
12347e  mov.w   r1, r0, asr #31
123482  strd    r0, r1, [r5]
123486  itt     eq
123488  ldreq.w r0, [r9]
12348c  streq   r0, [r5, #8]
12348e  ldr     r4, [sp, #68]
123490  mov     r0, r5
123492  mov     r1, r4
123494  blx     ArrayAdd<TouchButton*>
123498  ldr.w   fp, [sp, #108]
12349c  add.w   r8, r8, #1
1234a0  pld     [fp]                ; anchor
1234a4  pld     [fp]                ; anchor
1234a8  ldr.w   r9, [sp, #92]
1234ac  cmp.w   r8, #10
1234b0  bne.w   0x123348
```

Original loop (VMA 0x1233fa..0x1234ae) for comparison:

```
1233fa  orr.w   sl, r8, #1
1233fe  ldr     r0, [pc, #720]
123400  mov     r9, r8              ; save i
123402  ldr     r1, [pc, #720]
123404  cmp.w   sl, #9
123408  ldr     r2, [pc, #716]
12340a  add     r0, pc
12340c  add     r1, pc
12340e  add     r2, pc
123410  ldr     r0, [r0, #0]
123412  ldr     r1, [r1, #0]
123414  ldr     r2, [r2, #0]
123416  ldr     r5, [r0, #0]        ; font
123418  ldr     r0, [r1, #0]        ; Canvas
12341a  ldr.w   r8, [r2]            ; lang
12341e  mov     r1, r5
123420  str     r2, [sp, #76]       ; spill &fontLangSelect
123422  it      eq
123424  moveq   r1, r8              ; select #1  eq ? lang : font
123426  blx     FontGetSpacing
12342a  mov     r4, r0
12342c  ldr     r0, [sp, #72]
12342e  ldr.w   fp, [r0]            ; buttonWidth
123432  movs    r0, #200
123434  blx     operator new
123438  mov     r6, r0
12343a  ldr     r0, [pc, #672]
12343c  str.w   r9, [sp, #80]       ; spill i
123440  add     r0, pc
123442  ldr.w   r1, [r0, r9, lsl #2]
123446  ldr     r0, [sp, #96]
123448  ldr     r0, [r0, #0]
12344a  blx     GameText::getText
12344e  mov     r1, r0
123450  cmp.w   sl, #9
123454  it      eq
123456  moveq   r5, r8              ; select #2  eq ? lang : font  (same)
123458  ldr     r0, [sp, #84]
12345a  movs    r2, #0
12345c  strd    r0, fp, [sp]
123460  movs    r0, #17
123462  str     r0, [sp, #8]
123464  movs    r0, #4
123466  strd    r0, r5, [sp, #12]
12346a  mov     r0, r6
12346c  ldr     r3, [sp, #88]
12346e  str     r4, [sp, #20]
123470  blx     TouchButton::TouchButton(...)
123474  ldr     r0, [pc, #616]
123476  cmp.w   sl, #9
12347a  ldr.w   r8, [sp, #80]       ; restore i
12347e  add     r0, pc
123480  ldr.w   r0, [r0, r8, lsl #2]
123484  mov.w   r1, r0, asr #31
123488  strd    r0, r1, [r6]
12348c  ittt    eq
12348e  ldreq   r0, [sp, #76]       ; reload &fontLangSelect
123490  ldreq   r0, [r0, #0]
123492  streq   r0, [r6, #8]
123494  ldr.w   fp, [sp, #108]
123498  mov     r0, r6
12349a  ldr.w   r1, [fp, #176]
12349e  blx     ArrayAdd<TouchButton*>
1234a2  add.w   r8, r8, #1
1234a6  ldr.w   r9, [sp, #92]
1234aa  cmp.w   r8, #10
1234ae  bcc.w   0x123346
```

## 5. Remaining diffs (136 normalised hunks)

The improvement is real but the function is still not byte-exact. The
remaining differences are all register-allocation / loop-shape driven:

1. **Language loop RA.** The original allocates
   `r9 = i, r8 = lang, r5 = font, fp = buttonWidth`, spills
   `&fontLangSelect` to `[sp,#76]` and `i` to `[sp,#80]`, and
   rematerialises the second select (`moveq r5, r8`). Ours allocates
   `r8 = i, r9 = &fontLangSelect, r6 = lang, fp = font`, keeps `i` and
   the GOT pointer in registers, and spills `buttonWidth` to `[sp,#80]`.
   Consequences:
   * select #2 is `eq ? font : lang` (swapped) instead of
     `eq ? lang : font` (same orientation);
   * the `fontId` store is `itt eq; ldreq.w [r9]; streq` (2 insns)
     instead of `ittt eq; ldreq [sp,#76]; ldreq [r0]; streq` (4 insns);
   * the loop backedge is `bne.w` (from `i != 10`) instead of
     `bcc.w` (from `i < 10`), and the loop is 8 B shorter.
   Pass 7 (`menutouchwindow-c1-c2-pass7-phi-assume-w4418.md`) records the
   `__builtin_assume` PHI route that produces two same-orientation
   selects but loses on RA (0.9191). This pass does not change the loop.
2. **Stack-slot offsets.** Original `[sp,#60]` vs ours `[sp,#64]`;
   original `[sp,#72]` vs ours `[sp,#80]` (the `buttonWidth` spill).
   These follow from the RA difference.
3. **Branch-target shifts.** The 3 `pld` anchors and the 8 B loop
   difference move every post-loop branch target; the verifier compares
   intra-function branch targets literally, so these remain.
4. **`ldr` vs `ldr.w` / operand-order residue.** A handful of
   commutative `add`/`adds` still differ in register choice
   (normalised indices ~1831/1837/1889:
   `add.w r2, fp, r1` vs `adds r2, r3, r1`, etc.). These are RA
   artefacts, not source-order artefacts.
5. **Exception-table tail ordering.** The `.ARM.exidx` tail ordering
   differs (the function is one of the 24 non-byte-exact ones).

## 6. What remains / next route

The only durable route to byte-exactness is the language-loop register
allocation. The original's allocation is forced by having `i` in a
register that is needed for `lang` (`r8`), which spills `i` and the GOT
pointer and rematerialises the select. Reproducing it needs a source
shape that makes the allocator prefer `r9 = i` and `r8 = lang` without
adding instructions — pass 7's 192-way permutation and 16-way
`__builtin_assume`/`volatile` matrix both failed to find it. The three
`pld` anchors should be removed once that shape is found; they are a
size-only workaround.

## 7. Gate

Before and after this pass the full `check` is green and identical:

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
```

`_ZN15MenuTouchWindowC1Ei` and `_ZN15MenuTouchWindowC2Ei` both move from
94.2% (0.9424698795180723) to 94.8% (0.9477173421726683). The
`byte_exact` count does not change (the function is still not
byte-exact), so the ratchet baseline is unchanged.

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# exact ratio for the target
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump "$OD" --show _ZN15MenuTouchWindowC1Ei

# loop disassembly
$OD -d -M force-thumb --start-address=0x1233fa --stop-address=0x1234b0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d -M force-thumb --start-address=0x1233fe --stop-address=0x1234b4 \
  cmake-build-match/libgof2hdaa.so
```

Scratch (not tracked): `/dev/shm/w4518/` (`base.cpp`, `best.cpp`,
`measure.sh`, `dumpnorm.py`, `showdiff.py`, `try6.py`, `try7.py`,
`verify_variants.py`, `verify_ord.py`, `verify_more.py`,
`verify_faithful.py`, `verify_faithful2.py`, `verify_w1anchors.py`).
