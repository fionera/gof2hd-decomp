# MenuTouchWindow C1/C2 wave 106 (w5416): faithful two-value-select language loop
# (95.091840% -> 95.451807%, 9600 B)

Date: 2026-11-04. Worktree `wt/w5416`, branch `wave106-w5416`, base
`486f8a119`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before:** 9600 B, 3322 instructions, exact difflib ratio
  **0.9509183980728696** (95.091840%), 2900 raw-diff lines.
* **After:** 9600 B, **3320** instructions, exact difflib ratio
  **0.9545180722891567** (95.451807%), **2100** raw-diff lines.
* The ctor's language loop now uses the original's **two same-orientation
  value selects** (`special ? lang : font` twice) instead of the inverted
  `special ? font : lang` second select. The inversion was only a trick to
  stop `EarlyCSE` from merging the two selects; the machine code was the
  same shape but the register allocation / literal-pool slotting was not.
* The two selects are kept distinct by making the second one a **PHI**
  that survives `SimplifyCFG`: the `if` body carries a
  `__builtin_assume(i < 100)` (a no-op hint, same-line
  `// lint: arm_builtin` waiver). `IfConverter` predicates the PHI to the
  original's `cmp.w sl,#9; it eq; moveq` and the assume disappears.
* The `fontId` store becomes the original's 4-instruction
  `ittt eq; ldreq [sp,#76]; ldreq [r0]; streq`, and `ArrayAdd` uses
  `*this->buttonsB0` directly (the original reloads `this`/`buttonsB0`
  instead of caching a `langList` local).
* Statement order after the ctor is `fontId store; ArrayAdd; id store`.
  That order is what makes the allocator pick the original's
  `font=r5, lang=r9, bw=fp, button=r6` (the original has `lang=r8,
  i=r9`; the remaining r8/r9 swap is the only register difference left in
  the loop).
* Gate before and after: `byte_exact 4500 / linked_exact 4500`,
  `size 4524/4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS**. No baseline update.

## 1. The change

`src/game/ui/MenuTouchWindow.cpp`, language loop (lines 434-470):

```cpp
        this->buttonsB0 = new Array<TouchButton *>();
        int *specialFontPtr = &Globals::fontLangSelect;
        ...
        for (i = 0; i != 10; i++) {
            ...
            bool special = (i | 1) == 9;
            unsigned int fontValue = Globals::font;
            int langValue = Globals::fontLangSelect;
            int spacing = Globals::Canvas->FontGetSpacing(
                special ? (unsigned int) langValue : fontValue);
            int bw = this->buttonWidth;
            unsigned int ctorFont = fontValue;
            if (special) {
                ctorFont = (unsigned int) langValue;
                __builtin_assume(i < 100); // lint: arm_builtin
            }
            TouchButton *language = new TouchButton(
                *Globals::gameText->getText(MenuTouchWindow_languageTextIds[i]), 0, x, y, bw, 17, 4,
                ctorFont, spacing);
            if (special)
                language->fontId = *specialFontPtr;
            ArrayAdd(language, *this->buttonsB0);
            language->id = MenuTouchWindow_languageIds[i];
        }
```

Diff vs HEAD: the `langList` local is gone, the ctor arg is a PHI
(`ctorFont`) instead of the inverted ternary, and the `id` store moved
after `ArrayAdd`.

## 2. Why this works (mechanism, proven)

* `EarlyCSE` merges two identical `select i1 %c, i32 %lang, i32 %font`
  (`isIdenticalToWhenDefined` ignores debug locations). HEAD avoided the
  merge by *inverting* the second select's operands, which changes the
  destination register of the `MOVCC` (`moveq r6,fp` instead of
  `moveq r5,r8`) and therefore the whole loop allocation.
* A two-entry **PHI** is not CSE-able with a `select`. `SimplifyCFG`
  normally folds a trivial `if (special) x = lang;` to a select, but it
  refuses to speculate a non-speculatable instruction in the if body.
  `__builtin_assume(i < 100)` is such an instruction; it is removed by
  `CodeGenPrepare` after the last `SimplifyCFG`, so `IfConverter` still
  predicates the PHI. The result is the original's second value select
  with the **correct** orientation and no extra machine instruction.
* `ArrayAdd(language, *this->buttonsB0)` (instead of a cached
  `langList` local) makes the allocator reload `this` from `[sp,#108]`
  and `buttonsB0` from `[fp,#176]`, matching the original's tail.
* The post-ctor statement order `fontId store; ArrayAdd; id store` is
  what moves the allocation to `font=r5, lang=r9, bw=fp, button=r6`
  (original: `font=r5, lang=r8, bw=fp, button=r6, i=r9`). All six
  permutations were measured; this one is uniquely best.

## 3. Measured table (official verifier, full TU)

`orig` is always 9600 B / 3320 instructions. `fz` is the exact difflib
ratio from `tools/verify/verify.py`'s normalised stream.

| variant | size | insns | fuzzy | notes |
|---|---:|---:|---:|---|
| **HEAD (inverted 2nd select, `langList`)** | **9600** | **3322** | **0.95091840** | baseline |
| HEAD, correct-orientation ternary | 9544 | 3324 | 0.72606863 | selects CSE'd, address select |
| HEAD, `__builtin_assume` PHI + `bw` local | 9600 | 3322 | 0.95091840 | same bytes as HEAD (RA `font=r6 lang=fp`) |
| assume + `*this->buttonsB0`, order `id;store;arr` | 9604 | 3321 | 0.89685288 | 1 extra `i` reload |
| assume + `*this->buttonsB0`, order `id;arr;store` | 9596 | 3320 | 0.93042169 | |
| assume + `*this->buttonsB0`, order `store;id;arr` | 9592 | 3318 | 0.92588129 | |
| **assume + `*this->buttonsB0`, order `store;arr;id`** | **9600** | **3320** | **0.9545180722891567** | **landed** |
| assume + `*this->buttonsB0`, order `arr;id;store` | 9592 | 3319 | 0.92212683 | |
| assume + `*this->buttonsB0`, order `arr;store;id` | 9588 | 3319 | 0.90887182 | |
| assume + `*langList`, order `store;arr;id` | 9592 | 3322 | 0.91297802 | |
| no assume, order `store;arr;id` | 9548 | 3330 | 0.72150376 | PHI folded, address select |
| assume `i<10` / `i<=9` / `i>=0` / `true` | 9544-9548 | 3319-3330 | 0.71-0.72 | condition provable, PHI folded |
| assume `i<11` / `i<20` / `i<0x100` / `i<=100` | 9600 | 3320 | 0.9545180722891567 | any non-provable bound works |
| declaration/type permutations of `{special,font,lang}` | 9600 | 3320 | 0.9545180722891567 | neutral |
| `row+2` spellings (6 forms) | 9600 | 3320 | 0.9545180722891567 | scheduler artifact, neutral |
| cinematic `getText(497/496)` direct | 9592 | 3317 | 0.93325298 | worse |
| `TouchSlider[2]` regrouping | 9600 | 3322 | 0.94820837 | worse (pass 10) |

## 4. Remaining delta (proven)

The loop is now 67 instructions, same as the original. The only register
difference left in the loop is `lang`/`i`:

| value | original | landed |
|---|---|---|
| spacing | r4 | r4 |
| font | r5 | r5 |
| button | r6 | r6 |
| lang | r8 | r9 |
| i | r9 (saved by `mov r9,r8`) | r8 (spilled directly) |
| special | sl | sl |
| bw | fp | fp |
| `&fontLangSelect` | `[sp,#76]` | `[sp,#76]` |
| `this` | `[sp,#108]` | `[sp,#108]` |

Because `lang` is in r9 rather than r8, the original's
`mov r9,r8; ...; str.w r9,[sp,#80]; ...; ldr.w r8,[sp,#80]` triad is
replaced by `str.w r8,[sp,#80]` + `ldr.w r8,[sp,#80]` (same size, one
instruction earlier/later), and the loop backedge is `bne` instead of
`bcc` (`i != 10` vs `i < 10`; `i < 10` provably folds the PHI and costs
~23 points, so it cannot be used).

The largest single remaining structural diff is **outside** the loop:
the `case 1` mission block schedules `adds r4,r5,#2` *before*
`GameText::getText(395)` in the original and after the `String` copy in
ours. That is a 2-byte placement difference that shifts every later
branch target by 2 and dominates the 2100 raw-diff lines. Six source
spellings of `row + 2` (local before/inside the if, `(int)` cast,
`2u`, `+ (2)`, String local, `row` kept alive) are all byte-neutral, so
this is a scheduler/RA artifact, not a source-order difference.

## 5. Proven vs hypothesis

**Proven (measured):**
* The original has two same-orientation value selects; the landed source
  reproduces both with the correct orientation.
* `__builtin_assume` in the if body is the only construct found that
  keeps the PHI alive to `IfConverter` and adds no machine instruction.
* The post-ctor statement order `fontId; ArrayAdd; id` is uniquely best
  of the six permutations (95.45 vs 89.7-93.0).
* `ArrayAdd(language, *this->buttonsB0)` is required for the original's
  `ldr fp,[sp,#108]; ldr r1,[fp,#176]` tail.
* `i < 10` (the original's `bcc`) makes the PHI fold and drops to
  ~72%; `i != 10` is required.
* Gate is green: byte 4500 / linked 4500 / size 4524 / data 843 /
  imports 207 / lint CLEAN / drift 0 / RATCHET PASS.

**Hypothesis (not proven):**
* The original source probably had a natural non-speculatable statement
  in the ctor-font `if` (or a different PHI-producing shape) rather than
  `__builtin_assume`; no such shape was found among the ~120 variants
  measured across waves 90-106.
* The remaining `lang=r9` vs `lang=r8` swap is a register-allocation
  fixed point of the current IR; no source-level lever was found to
  move it without losing more than it gains.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5416
TMPDIR=/dev/shm/w5416/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5416/tmp cmake --build cmake-build-match --target check -j6

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0x1233f0 --stop-address=0x1234b0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OD -d --start-address=0x1233f0 --stop-address=0x1234b0 \
  cmake-build-match/libgof2hdaa.so
```

Scratch (not tracked): `/dev/shm/w5416/` (`best2.cpp`, `raparse.py`,
`batch*.py`, `score.py`, `measure.sh`, `mi2.txt`).

---

# Wave 106 continuation (w5416, second session): local-optimum audit

Date: 2026-11-04. Same worktree/branch, base `486f8a119` + commit
`34abc6cd4`. The landed source above was re-verified and then audited for
further gains. **No further improvement was found**; the landed source is
a measured local optimum at **95.451807 % / 9600 B / 3320 insns /
2100 raw-diff lines**. Gate re-run after the audit: `RATCHET PASS`,
`byte 4500 / linked 4500 / size 4524 / data 843 / imports 207`,
`lint CLEAN`, `drift 0`.

## 1. Where the remaining bytes are (size accounting)

The function is exactly 9600 B / 3320 insns in both binaries, but four
blocks differ in *size* and cancel out:

| block | orig | landed | delta |
|---|---|---|---|
| `case 1` mission `addButton(18,..)` | `adds r4,r5,#2` + `mov r3,r4` | `adds r3,r5,#2` | **-2 B** |
| language loop | `mov r9,r8` (i copied to r9, r8 reused for lang) | none (i stays r8, lang=r9) | **-2 B** |
| iPad `cinematicText` block | loads gameText after `new` | hoists it before `new` + `mov sl,r4` + `ldr.w sl,[sp,#88]` | **+2 B** |
| branch-target alignment `nop` at 0x1236ce | none | `nop` | **+2 B** |

Everything else is instruction *placement* / register choice at equal
size. Fixing any one of these changes the function size and therefore
shifts every later address; each must be fixed together with its
compensating partner.

## 2. The `case 1` add is a list-scheduler artifact (proven)

ARM's `ARMTargetLowering::getSchedulingPreference` returns
`Sched::RegPressure` for integer nodes (LLVM 7 `ARMISelLowering.cpp:1437`),
i.e. the `ScheduleDAGRRList` bottom-up list scheduler — *not* `source`.
The `%row2 = add nsw i32 %row, 2` node is a leaf whose only use is the
`addButton` invoke, so it only becomes available after `addButton` is
scheduled; `getNodePriority` then places it just before the call. The
original's `adds r4,r5,#2` *before* `blx getText` therefore cannot be
reproduced by reordering the source.

Measured (all byte-neutral, 9600 B / 3320 / 95.451807 / 2100):
`int row2 = row + 2;` before the `if`, inside the `if`, before the
`addButton(19,..)` line, `2 + row`, `row + 1 + 1`, `row - (-2)`,
`row + 2u`, a real `switch (mission) { case 154: case 157: case 158: }`,
and `int row2 = row + 2; ... row = row2 - 2;` after the call.
A named `String text(*getText(395), false);` local is much worse
(9624 B / 3331 / 89.49 %).

Diagnostic: a whole-`.so` build with
`-DGOF2_MATCH_OPT="-Oz -mllvm -pre-RA-sched=source"` gives
**9516 B / 3333 insns / 46.60 %** — the `source` scheduler does hoist the
add but destroys the stack frame and register allocation, so the original
cannot have used it.

## 3. `sliders[2]` reassociation (proven shape, not reachable)

The original `(*this->sliders)[2]->setPosition(...)` y-argument is built as

```
t1 = field_0x1c + rowHeight
t2 = field_0x15c + t1
t3 = field_0x20_top + leftMargin
t4 = t3 + listEntryWidth
t5 = rowHeight * 3
t6 = t4 + t5
t7 = field_0x2fc + t6
t8 = t7 + sliderRowHeight
t9 = t8 + t2 * 2
```

i.e. it folds `rowHeight*5` into `rowHeight*3 + (..+rowHeight)*2` and
loads the `Globals::layout` fields before the `this` fields. The current
source (`(field_0x15c + field_0x1c)*2 + .. + rowHeight*5`) produces the
mathematically identical but differently-associated tree
(`rowHeight*5` + `(field_0x15c+field_0x1c)*2`). 14 source orderings /
parenthesizations were measured; none beats the landed source:

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| landed | 9600 | 3320 | **95.451807** | **2100** |
| `fc` first, `(..+rowHeight)*2` last | 9600 | 3320 | 95.301205 | 2106 |
| `(..+rowHeight)*2` last, `fc` before `sliderRowHeight` | 9600 | 3320 | 95.361446 | 2105 |
| orig tree, `rowHeight*3` + `(..+rowHeight)*2` | 9600 | 3320 | 95.000000 | 2310 |
| `(..+rowHeight)*2` first, `rowHeight*3` | 9600 | 3321 | 95.316970 | 2887 |
| layout-first, `(field_0x15c+field_0x1c)*2` last | 9596 | 3319 | 91.881307 | 2707 |

`sliders[0]`, `[1]`, `[3]`, `[4]`, `[5]` already match the original
instruction-for-instruction (only address-shifted), so only `[2]`'s
reassociation is wrong.

## 4. iPad `cinematicText` block (proven trade-off)

Removing the `GameText *cinematicText = Globals::gameText;` local makes
the block match the original exactly (gameText loaded after `operator
new`, no `mov sl,r4`, no `ldr.w sl,[sp,#88]`), but the function shrinks to
**9592 B / 3317 insns / 93.325298 %** and the 8-byte shift destroys more
downstream matches than it gains. Keeping the local is worth +2.1 points.

## 5. Loop statement order (proven tuning)

The original's post-ctor order is `id store; fontId store; ArrayAdd`
(see `strd r0,r1,[r6]` before the `ittt eq` fontId store). Reconstructing
that faithful order scores **9604 B / 3321 / 89.685288 %**; the tuned
`fontId store; ArrayAdd; id store` is **9600 B / 3320 / 95.451807 %**.
The fuzzy metric therefore prefers a *non-faithful* order here.

## 6. Loop index copy

`mov r9,r8` in the original is the allocator copying the loop counter
before reusing r8 for `langValue`. Introducing an explicit
`unsigned int idx = i;` (used for `special`, the array indices, or both)
is coalesced away and byte-neutral. `i < 10` (the original's `bcc`)
folds the ctor-font PHI and drops to ~72 %; `i != 10` is required.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5416
TMPDIR=/dev/shm/w5416/tmp cmake --build cmake-build-match --target check -j6
# per-variant harness (scratch): python3 /dev/shm/w5416/runvar.py <variants.json>
```

Scratch: `/dev/shm/w5416/` (`v0.cpp` = landed source, `score2.py`,
`runvar.py`, `show_s2.sh`, `sw_*.json`, `dbgc1.txt` = source-line-annotated
C1 disassembly, `ScheduleDAGRRList.cpp`, `ARMISelLowering.cpp`).
