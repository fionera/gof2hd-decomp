# MenuTouchWindow C1/C2 wave 108 (w5619): mask the language-loop backedge
# (95.5% -> 95.6%, 9600 B)

Date: 2026-11-04. Worktree `wt/w5619`, branch `wave108-w5619`, base
`8f98ec4d6`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before:** 9600 B, 3320 instructions, exact difflib ratio
  **0.955** (95.5%), 178 raw-diff lines (this worker's rawdiff metric:
  changed lines from `difflib` after `verify.reconcile`).
* **After:** 9600 B, 3320 instructions, exact difflib ratio
  **0.956** (95.6%), **173** raw-diff lines.
* One-line source change in the language-selection loop:

```cpp
-        for (i = 0; i != 10; i++) {
+        for (i = 0; (i & 0xf) != 10; i++) {
```

* Gate before and after: `byte_exact 4500 / linked_exact 4500`,
  `size 4524/4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS**. Fuzzy-only change, so no baseline update is needed
  and none was made. No section/extab change.

## 1. Why this works (mechanism, measured)

The original language-loop backedge is

```
1234a6: ldr.w r9, [sp, #92]
1234aa: cmp.w r8, #10
1234ae: bcc.w 123346
```

i.e. an unsigned `i < 10` test (2 instructions).  Writing `i < 10` in
the source makes clang prove `i` is in `[0,9]` and fold the
`special ? lang : font` value-select PHI into a conditional load; the
function shrinks to **9548 B / 3330 instructions** and the score drops
to **72.2%** (rawdiff 1118).  The `__builtin_assume(i < 100)` currently
sits inside `if (special)`, so it does not stop the global range
inference.  `i != 10` keeps the general codegen but emits a 2-insn
`cmp/bne` backedge.

`(i & 0xf) != 10` is semantically identical for this loop (i is
incremented from 0 and never modified in the body, so it takes
0..9 and exits at 10, where `10 & 0xf == 10`).  It emits

```
and.w r0, r8, #15
cmp   r0, #10
bne.w 123342
```

at the backedge.  That is one instruction longer than the original, but
the difflib alignment improves by 5 lines: the `nop` at 0x1236ce and the
following literal-pool slots line up.  Nothing else in the function
changes (size and instruction count are identical).

Equivalently-scoring spellings (all measured **95.6% / 173**):
`(i & 0xf) < 10`, `(i & 0xf) <= 9`, `(i & 0xf) != 0xa`,
`(i & 0xf) != (5+5)`, `(i & 0xf) != (unsigned)10`,
`((int) i & 0xf) != 10`, and the same with `i` declared `int` instead of
`unsigned int`.  `(i & 0xf) < 10` is canonicalised by clang to
`and #14; cmp #9; bls` and scores the same.

## 2. Measured table (official verifier, full TU)

`orig` is always 9600 B / 3320 instructions.  `fz` is the exact difflib
ratio from `tools/verify/verify.py`'s normalised stream.  Scratch base =
`/dev/shm/w5619/base.cpp` (pristine HEAD).

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| **HEAD (`i != 10`)** | **9600** | **3320** | **0.955** | **178** |
| **`(i & 0xf) != 10`** | **9600** | **3320** | **0.956** | **173** |
| `(i & 0xf) < 10` / `<= 9` | 9600 | 3320 | 0.956 | 173 |
| `i < 10` (or `<= 9`) | 9548 | 3330 | 0.722 | 1118 |
| `i != 10` with `assume` removed | 9548 | 3330 | 0.722 | 1118 |
| `i < 10` with `assume` removed | 9548 | 3330 | 0.722 | 1118 |

## 3. Negative results re-measured this wave (all with the loop fix)

These are the remaining RA/scheduling fixed points from wave 107, plus
new attempts.  None beat 95.6%.

### 3.1 Mission block `adds r4,r5,#2` hoist

Original hoists `adds r4, r5, #2` (row+2) **above** `getText(395)` and
keeps it in callee-saved `r4`; ours sinks it below the `String` ctor
into caller-saved `r3`.  The scheduler sinks the ADD because it is
rematerialisable from `row` (in `r5`).  Every spelling below is
**byte-neutral at 95.6% / 173**:

* `int row2 = row + 2;` before the `addButton` (and before the `if`)
* `const int row2 = row + 2;`
* `2 + row`, `row + 2u`, `(short) 2`, `(int) (2.0f)`
* `(row + 1) + 1`, `row += 2`
* `String` local for `*getText(395)` (this one is *worse*: 9624 B /
  3331 insns / 89.5%)

Forcing the ADD into the `getText` argument does hoist it but costs
instructions:

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| `getText(395 + ((row + 2) == 0))` | 9608 | 3323 | 0.921 | 295 |
| `String(*getText(395), (row + 2) == 0)` | 9608 | 3321 | 0.921 | — |
| `(row + 2) > 0` in the arg | 9600 | 3320 | 0.954 | — |

`-mllvm -pre-RA-sched=source` reproduces the hoist but changes the
frame (0x174 -> 0x184) and drops the TU to 46.7%, so the original was
not built with source scheduling (wave-107 result, re-confirmed).

### 3.2 Language-loop `lang`/`i` r8<->r9 swap

Original: `lang=r8`, `i=r9` (with a `mov r9,r8` copy and an `i` reload
into `r8` for the second array index).  Ours: `lang=r9`, `i=r8`.  About
20 raw-diff lines.  No source lever found.  All of the following are
**neutral at 95.6% / 173**:

* separate loop variable (`unsigned int j` / `idx`), with `(j & 0xf) != 10`
* `unsigned int idx = i;` used for one or both array indices
* renaming the later `scrollSlots` loop variable
* reordering `fontValue`/`langValue` declarations either way
* `int`/`unsigned`/`long`/`size_t` for either variable
* `bool`/`int`/`unsigned` for `special`
* `i++` vs `++i` vs `i += 1`; `(i >> 1)` vs `(i / 2)`; `(i & 1)` vs `(i % 2)`
* moving `__builtin_assume(i < 100)` to the top of the body (worse:
  9544 B / 3328 / 72.6%); assume bound 99/101/200/1000 (neutral)
* `language->fontId = Globals::fontLangSelect;` instead of
  `*specialFontPtr` (neutral — the compiler CSEs them)

### 3.3 Language-loop tail order (`id` / `fontId` / `ArrayAdd`)

Original order is `language->id = languageIds[i];` then
`if (special) fontId = ...;` then `ArrayAdd`.  Our source order is
`fontId`, `ArrayAdd`, `id`.  Reordering the source to the original
order reproduces the instruction order but costs an extra reload:

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| `id`, `fontId`, `ArrayAdd` | 9604 | 3321 | 0.897 | 369 |
| `id`, `ArrayAdd`, `fontId` | 9596 | 3320 | 0.931 | 257 |
| `fontId`, `id`, `ArrayAdd` | 9596 | 3320 | 0.928 | 269 |

### 3.4 iPad `sliders[2]` y-tree association

The original groups the sum as
`0x2fc + ((0xc + 0x20) + 0x154 + 3*0x2c) + 0x164 + 2*(0x15c + 0x1c + 0x2c)`.
Writing exactly that regrouping reproduces the original tree locally
(verified in the disassembly) but perturbs the surrounding RA:

| variant | size | insns | fuzzy | rawdiff |
|---|---:|---:|---:|---:|
| faithful regrouping alone | 9600 | 3320 | 0.951 | 183 |
| faithful regrouping + loop fix | 9600 | 3320 | 0.953 | 178 |

So the current (left-leaning) grouping scores better globally even
though it does not match the original tree.

## 4. Proven vs hypothesis

**Proven (measured):**
* `(i & 0xf) != 10` is semantically identical to `i != 10` for this
  loop and improves the verifier ratio from 0.955 to 0.956 with
  identical size (9600 B) and instruction count (3320).
* `i < 10` (and `i != 10` without the `__builtin_assume`) folds the
  value-select PHI and costs ~23 points; the assume is load-bearing.
* The mission-block ADD hoist, the r8/r9 swap, the loop-tail order and
  the `sliders[2]` tree are not reachable from source without a
  size/instruction-count change (tables above).

**Hypothesis (not proven):**
* The original's `bcc` backedge came from a plain `i < 10` in a source
  shape where clang did not have (or did not use) the `[0,9]` range
  fact for the `special` select.  The `__builtin_assume` in our
  reconstruction is what blocks that inference today; the mask is a
  source-level way to keep the general codegen while matching the
  backedge alignment.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5619
TMPDIR=/dev/shm/w5619/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5619/tmp cmake --build cmake-build-match --target check -j6
TMPDIR=/dev/shm/w5619/tmp cmake --build cmake-build-match --target drift -j6
# per-function score:
python3 /dev/shm/w5619/score.py _ZN15MenuTouchWindowC1Ei
```

Scratch (not tracked): `/dev/shm/w5619/` (`base.cpp`, `score.py`,
`run.sh`, `dump.py`, `diffblocks.py`, `orig.asm`, `on.txt`, `un.txt`,
`rea.json`, `ctor.c`, batch scripts `batch1.py`..`batch15.py`).

REA evidence id: `ev_5c2683c3de0b3e59e495e94516493b3b5106b7379d148749a66a6fb3791db9ab`.
