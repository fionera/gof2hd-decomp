# MenuTouchWindow C1/C2 pass 10: enumerate the remaining diff bytes and
# the select/ArrayAdd leads (95.092% stays the local optimum)

**Status:** analysis only, 2026-11-04. Worktree `wt/w4817`, branch
`wave98-w4817`, base `fe4f5e7cf`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before = after:** 9600 B, **3322 instructions** (orig 3320), exact
  difflib ratio **0.9509183980728696** (95.0918%), **125 hunks**. No code
  change landed: every variant tried this pass was neutral or worse.
* The three `__builtin_prefetch` anchors are still gone (pass 9); the
  current source shape is `src/game/ui/MenuTouchWindow.cpp` lines
  283-520.
* The remaining diff is **not** a single missing expression. It is one
  register permutation in the language loop (font/lang/button/bw/i),
  plus a handful of downstream scheduling/spill artifacts that all
  follow from it. The loop *instruction sequence* and *size* already
  match; only the register names and the early `mov r9,r8` placement
  differ.
* New negative results this pass (all measured with the official
  verifier):
  - ctor-arg select flipped to the original's orientation
    (`special ? lang : font`): clang CSEs the two selects and the
    function shrinks to 9544 B / 3324 insns / 72.61%. The original has
    **two** selects, so the flipped source alone is not the answer.
  - direct `ArrayAdd(language, *this->buttonsB0)` (no `langList`
    local): 9604 B / 3321 insns / 89.63%. It removes the `langList`
    spill and matches the original's `ldr fp,[sp,#108]; ldr r1,[fp,#176]`
    reload, but it also moves `font` to r9 and forces `i` to r8+spill,
    which costs more than it saves.
  - `TouchSlider[2]` y-expression regrouped to the original's
    `(field_0x15c + field_0x1c + rowHeight) * 2` shape: 9600 B / 3322
    insns / 94.64%. The arithmetic is right but the load order is
    reversed (ours loads the `this` fields before the `layout` fields).
  - `i < 10` (the original's `bcc` predicate): 95.0617%;
    `i != 10` (current, `bne`) is still the best difflib alignment.

## 1. Baseline and gate

```
size 9600 (orig 9600)  insns 3322 (orig 3320)
ratio 95.091840  hunks 125
```

Full `check` before and after: `byte_exact 4500/4524`,
`linked_exact 4500/4524`, `size_exact 4524/4524`, `stub_zero_size 0`,
`missing 0`, `wrong_type 0`, `extra 0`, `data_exact 843/843`,
`imports 207`, `lint CLEAN`, `drift 0`, `RATCHET PASS`.

## 2. The remaining non-branch diff bytes

`difflib` reports 125 hunks, but almost all are branch-target shifts
caused by the 2-instruction difference. The *structural* hunks are:

| # | region | original | ours |
|---|---|---|---|
| 1 | `case 1` mission block | `adds r4,r5,#2` **before** `getText` | `adds r3,r5,#2` **after** the `String` ctor |
| 2 | language loop prologue | `mov r9,r8` at 0x123400 (early) | `mov r9,r8` at 0x12342c (after `FontGetSpacing`) |
| 3 | language loop loads | `ldr r5..font`, `ldr r8..lang`, `ldr fp..bw`, `mov r6..button` | `ldr fp..font`, `ldr r6..lang`, `ldr r5..button`, `ldr r8..bw` |
| 4 | language loop 2nd select | `cmp sl,#9; it eq; moveq r5,r8` (`special ? lang : font`) | `cmp sl,#9; it eq; moveq r6,fp` (`special ? font : lang`) |
| 5 | `ArrayAdd` tail | `ldr fp,[sp,#108]; mov r0,r6; ldr r1,[fp,#176]` (reload `this`/`buttonsB0`) | `ldr r4,[sp,#60]; mov r0,r5; mov r1,r4` (cached `langList`) |
| 6 | loop backedge | `cmp r8,#10; bcc` | `cmp r8,#10; bne` |
| 7 | `cinematicBtnB` | `movs r0,#200; blx new; mov r5,r0; ldr r0,[sp,#96]; ldr r0,[r0]` | `mov sl,r4; blx Image2DCreate; ldr r0,[sp,#96]; ldr r9,[r0]; movs r0,#200; blx new; mov r6,r0; mov r0,r9` |
| 8 | `cinematicBtnA` | `ldr r4,[sp,#96]` before `operator new` | missing (the value is already in r9) |
| 9 | after `cinematicBtnA` | — | extra `ldr.w sl,[sp,#88]` reload |
| 10 | `TouchSlider[2]` y | `(top+left+listEntryWidth+rowHeight*3) + field_0x2fc + sliderRowHeight + (field_0x15c+field_0x1c+rowHeight)*2` | `(field_0x15c+field_0x1c)*2 + listEntryWidth + top + left + rowHeight*5 + sliderRowHeight + field_0x2fc` |

Items 2-4, 6, 7-9 are all consequences of the same register-allocation
decision; item 5 is the `langList` local; item 10 is a genuine source
grouping difference that is currently masked by the wrong allocation.

## 3. The language-loop register permutation (the main blocker)

Original register assignment in the `for (i = 0; i < 10; i++)` body:

```
r5 = font          (Globals::font)
r8 = lang          (Globals::fontLangSelect)
r6 = button        (new TouchButton)
fp = buttonWidth   (int bw = this->buttonWidth)
r9 = i             (copied early at 0x123400, spilled to [sp,#80])
r4 = spacing       (FontGetSpacing result)
sl = special       (i | 1) == 9
```

Ours:

```
fp = font
r6 = lang
r5 = button
r8 = buttonWidth
r9 = i             (copied late at 0x12342c, spilled to [sp,#80])
r4 = spacing
sl = special
```

The mapping ours -> original is the 4-cycle
`fp -> r5 -> r6 -> r8 -> fp`. The live ranges and the instruction
sequence are identical; only the allocator's choice differs.  The
early `mov r9,r8` in the original exists because `r8` is reused for
`lang`; in ours `r8` is reused for `bw`, so `i` can stay in `r8` until
after `FontGetSpacing`.

### 3.1 Why the obvious fixes fail

* **Direct `this->buttonsB0`** (no `langList`) removes the
  `str r4,[sp,#60]` spill and gives the original's
  `ldr fp,[sp,#108]; ldr r1,[fp,#176]` reload.  But it also frees a
  callee-saved register, so the allocator moves `font` to `r9` and
  leaves `i` in `r8` (spilled).  Result: 9604 B / 3321 insns / 89.63%.
  It gets `button=r6` and `bw=fp` right but rotates `font/lang/i`.
* **Flip the ctor select** to `special ? lang : font`.  The two
  selects become identical in the IR, so clang CSEs them (one
  `select` + pointer-select `ldrne`) and the function shrinks to
  9544 B / 3324 insns / 72.61%.  The original has *two* selects
  (`cmp sl,#9` twice), so the original source must keep them
  non-identical in the IR even though they compute the same value.
* **Mixed condition spellings** (`special` vs `(i|1)==9` vs
  `i==8||i==9`) still CSE.  Clang canonicalizes all of them to the
  same `icmp eq (or i,1), 9`.
* **Declaration/statement reordering** (`lang` before `font`,
  `special` last, `bw` before `spacing`, `bw` first) either leaves the
  allocation unchanged or loses badly (`bw` before `spacing`:
  9640 B / 76.1%).
* **Type changes** (`int`/`unsigned`/`short`/`const`/`long`/`register`
  on `fontValue`, `langValue`, `bw`, `spacing`, `special`,
  `showLanguages`, `i`) are all neutral.
* **`specialFontPtr`** variants (direct global, reference, const
  pointer, scoped) are all neutral.

## 4. The `bcc`/`bne` backedge

Original: `cmp.w r8,#10; bcc.w 0x123346`.
Ours: `cmp.w r8,#10; bne.w 0x123344`.

`-Oz` canonicalizes a counted `for (i = 0; i < 10; i++)` to
`icmp eq i32 %i, 10` (verified with `clang -emit-llvm`: both
`i < 10` and `i != 10` produce `icmp eq`).  The original's `bcc`
means its IR is `icmp ult i32 %i, 10`, which happens only when clang
cannot prove the trip count — e.g. a global/reused induction variable
or a loop with an exception exit that defeats `IndVarSimplify`.  In
this function the loop body *does* contain `invoke` (getText,
TouchButton ctor) and an `icmp ult` is produced by `i < 10` in the
IR, but the ARM backend then folds `ult 10` to `ule 9` and emits
`cmp #9; bls`.  Source spellings tried and their results:

| source | emitted tail | ratio |
|---|---|---|
| `i != 10` (current) | `cmp #10; bne` | **95.091840** |
| `i < 10` / `i < 10u` / `i <= 9` / `10 > i` | `cmp #9; bls` | 95.061728 |
| `i + 1 <= 10` / `i + 1 < 11` | `cmp #10; bls` (extra `add`) | 83.70% |
| `i - 10 < 0` | different loop shape | 60.19% |
| `while (i < 10)` | 9596 B | 92.4% |
| `do { } while (i < 10)` | 9576 B | 85.2% |

`i != 10` is the best difflib alignment (the `#10` immediate matches
even though `bne` does not).

## 5. The `TouchSlider[2]` grouping

The original computes (0x12462e-0x124674):

```
r2 = field_0x1c + rowHeight
r1 = field_0x15c + r2
r2 = top + leftMargin
r2 += listEntryWidth
r2 += rowHeight*3
r0 = field_0x2fc + r2
r0 += sliderRowHeight
r0 += r1*2
```

i.e. the source is

```cpp
(int) ((float) ((Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                 this->listEntryWidth + Globals::layout->field_0x2c_rowHeight * 3) +
                Globals::layout->field_0x2fc + this->sliderRowHeight +
                (this->field_0x15c + Globals::layout->field_0x1c +
                 Globals::layout->field_0x2c_rowHeight) * 2) +
       (Globals::iPadHD ? 19.6875f : (Globals::iPadLarge ? 28.0f : 14.0f)));
```

The current source instead has

```cpp
(this->field_0x15c + Globals::layout->field_0x1c) * 2 +
this->listEntryWidth + Globals::layout->field_0x20_top +
Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x2c_rowHeight * 5 +
this->sliderRowHeight + Globals::layout->field_0x2fc
```

which is numerically identical but groups `rowHeight*5` outside the
`*2`.  Replacing it with the original grouping gives 94.64% — the
arithmetic is right but the compiler schedules the `this` loads
before the `layout` loads, the reverse of the original.  This is a
downstream effect of the loop's wrong register allocation: the live
registers entering the post-loop code differ, so the allocator
orders the slider loads differently.  **If the loop allocation is
ever fixed, this grouping should be re-applied and re-measured — it
is the faithful source shape.**

The other three `TouchSlider` y-expressions (sliders 3, 4, 5) already
use the correct grouping and match the original instruction-for-
instruction.

## 6. The `cinematicText` region

The original loads `Globals::gameText` fresh for `getText(497)`
(0x123538-0x123539), then for `getText(496)` keeps `&Globals::gameText`
in r4 across `operator new` and reloads the value afterwards
(0x123605, 0x123610).  Our source uses one `cinematicText` local for
both calls; clang hoists the *value* into r9 before `operator new`,
which produces the extra `mov sl,r4` / `ldr r9,[r0]` and the later
`ldr sl,[sp,#88]` reload.

Tested variants (all measured):

| source | size | ratio |
|---|---|---|
| both `cinematicText` (current) | 9600 | **95.091840** |
| both `Globals::gameText` | 9592 | 93.417683 |
| first direct, second local | 9600 | 95.091840 |
| first local, second direct | 9592 | 93.206808 |
| local declared after first `new` | 9596 | 93.359434 |
| `GameText *&cinematicText` | 9592 | 93.417683 |

The first-direct/second-local shape is the faithful one (the original
only caches the address for the second call) but it is numerically
neutral with the current loop allocation.  It is a reasonable
candidate to re-test after the loop is fixed.

## 7. Leads for the next pass (in priority order)

1. **Get the loop allocation right.**  The original's
   `mov r9,r8` at 0x123400 means `i` is copied before the GOT loads
   because r8 is needed for `lang`.  A source shape that makes `lang`
   the value that reuses r8 (rather than `bw`) is the key.  The
   direct-`buttonsB0` variant already gets `button=r6` and `bw=fp`;
   combining it with something that moves `font` from r9 to r5 and
   `i` from r8 to r9 is the remaining 3-cycle.  Candidates not yet
   tried: making `langValue` the *last* load (after `bw`), or using
   `langValue` in the `fontId` store so its live range extends past
   the ctor.
2. **Re-apply the `TouchSlider[2]` grouping** once the loop
   allocation is fixed (see §5).  It is the faithful source shape and
   should then match.
3. **Re-apply first-direct/second-local `cinematicText`** once the
   loop is fixed (see §6).
4. **`ArrayAdd` reload.**  The original reloads `this`/`buttonsB0`;
   the `langList` local is a reconstruction convenience.  Do not
   remove it in isolation — it rotates the loop allocation (see §3.1).

## 8. Reproduction

```sh
cmake --build cmake-build-match --target check -j8

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump "$OD" \
  --show _ZN15MenuTouchWindowC1Ei
```

Scratch (not tracked): `/dev/shm/w4817/` (`base.cpp`, `base.h`,
`measure.sh`, `ratio.py`, `hunks.py`, `dumpidx.py`, `countdiff.py`,
`try_*.py`, `mtw.ll`, `c2.ll`).
