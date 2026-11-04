# MenuTouchWindow C1/C2 wave 110 (w5819): language-loop range-fact sweep
# (no improvement; 95.6928% / 9600 B held)

Date: 2026-11-04. Worktree `wt/w5819`, branch `wave110-w5819`, base
`390f58052`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* Baseline (wave 109, `aac84018d`): **9600 B, 3320 instructions,
  95.6928%**, 110 diff hunks (29 structural / 81 RA-only), size-exact.
* This wave found **no improvement**. The best measured variant is still
  the wave-109 baseline. No source change was committed; this note is the
  only change.
* Gate unchanged: `byte_exact 4500 / linked_exact 4500`,
  `size 4524/4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS**. No section/extab change (`.so` still +44 B,
  `.ARM.extab` still 0x26294, 28 sections).

## 1. The central result: `i < 10` is unusable because of the stack frame

The original backedge is

```
1234a6: ldr.w r9, [sp, #92]
1234aa: cmp.w r8, #10
1234ae: bcc.w 123346
```

i.e. a plain unsigned `i < 10` (2 instructions). Writing `i < 10` in the
source is **not** a drop-in replacement: clang proves `i in [0,9]` and
rewrites the `special ? lang : font` value-select PHI into an
*address* select (`mov r2,sl; it ne; ldrne r2,[r1,#0]; ldr r4,[r2,#0]`
instead of `mov r1,r5; it eq; moveq r1,r8`). It also re-allocates the
loop counter from `r8` to `fp`, which grows the live-range set and
**shifts the stack slot assignment by 8 bytes** (`str r1,[sp,#60]` ->
`[sp,#68]`, `[sp,#72]` -> `[sp,#80]`, `[sp,#100]` -> `[sp,#96]`, ...).
The whole function then diverges: **9548 B, 3330 instructions, 72.06%**.

The wave-108/109 `(i & 0xf) != 10` mask is what keeps the `[0,9]` fact
out of the optimizer while emitting a `cmp #10`-shaped test. It is 2
bytes longer than the original backedge, which exactly compensates the
2-byte deficit in the mission block (`adds r3,r5,#2` sunk vs. the
original's hoisted `adds r4,r5,#2` + `mov r3,r4`), so the frame and
literal-pool offsets stay aligned.

### 1.1 Weak-range-fact attempt: `(int) i < 10`

`(int) i < 10` with `i` unsigned gives a *weak* range fact (the cast
allows `i >= 2^31`) and therefore keeps the value select and the
original stack frame. But clang canonicalises it to `cmp.w r8,#9;
bls.w` (i.e. `i <= 9`) instead of `cmp.w r8,#10; bcc.w`. The body is
instruction-for-instruction the baseline body; only the backedge
differs. Score **95.512% / 117 hunks** (vs. baseline 95.6928 / 110):
the 2-byte-shorter backedge re-introduces the downstream pool shift.
Equivalently `(int) i <= 9`, `(int) i - 10 < 0`, `(int) (i - 10) < 0`,
`(int) i < 0xa`, `(int) i < 10 || i > 0x7fffffff`, and
`(int) i < 10 && i != 0xffffffff` are all 95.512 with the same
`cmp #9; bls` encoding.

## 2. Measured table (official verifier, full TU)

`orig` is always 9600 B / 3320 instructions. `fz` is the exact difflib
ratio from `tools/verify/verify.py`'s normalised stream. Baseline
scratch file = `/dev/shm/w5819/base.cpp`.

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| **baseline (`(i & 0xf) != 10`, assume in `if (special)`)** | **9600** | **3320** | **95.6928** | **110** |
| `i != 10` | 9600 | 3320 | 95.5422 | — |
| `(int) i < 10` (weak fact, `cmp #9; bls`) | 9600 | 3320 | 95.5120 | 117 |
| `i < 10` (strong fact, pointer select, stack shift) | 9548 | 3330 | 72.0602 | — |
| `i <= 9`, `i < 10u`, `i < 0xau` | 9548 | 3330 | 72.0602 | — |
| `(i & 0xf) < 10` / `<= 9` | 9600 | 3320 | 95.6928 | 110 |
| `(i & 0x1f/0x3f/0x7f/0xff) != 10` | 9600 | 3320 | 95.6928 | 110 |
| `(i & 0x1ff) != 10` | 9600 | 3322 | 91.9904 | — |
| `i % 16 != 10` | 9600 | 3320 | 95.5422 | — |
| `while ((i & 0xf) != 10) { ...; i++; }` | 9600 | 3320 | 95.6928 | 110 |
| `for (i = 0;; i++) { if ((i & 0xf) == 10) break; ... }` | 9600 | 3320 | 95.6928 | 110 |
| `do { ...; i++; } while ((i & 0xf) != 10)` | 9600 | 3323 | 88.1228 | — |

## 3. The `__builtin_assume` is a precise RA lever

The baseline's `__builtin_assume(i < 100)` sits inside the ctorFont
`if (special)` block. It is load-bearing: removing it (or replacing it
with a plain use) drops to **72.5331% / 3328 insns**, because the
assume is what keeps `i` in `r8` and `special` in `sl` (matching the
original's `r8=i`, `sl=special`). Measured:

| assume form / placement | fuzzy |
|---|---:|
| **`__builtin_assume(i < 100)` inside `if (special)` (baseline)** | **95.6928** |
| `__builtin_assume(i < 16/1000/0x1000/0x7fffffff)` | 95.6928 |
| `__builtin_assume(i != 100)` / `i != 0xffffffff` | 95.6928 |
| `__builtin_assume(i <= 100)` / `i < 101` | 95.6928 |
| `__builtin_assume(i < 100u)` / `(unsigned) i < 100u` | 95.6928 |
| `__builtin_assume(i < 100)` in the `else` branch | 95.6928 |
| `__builtin_assume(i < 100)` twice in the block | 95.6928 |
| assume at top of body / after `special` / after `langValue` | 72.5331 |
| assume in the fontId block / after the ctor | 72.5331 |
| no assume; `i = i;` / `i += 0;` / `(void) i;` in the block | 72.5331 |
| `__builtin_expect(i < 100, 1)` | 72.5331 |
| `__builtin_assume(i >= 0)` / `i > 0` | 72.5331 / 70.6325 |
| `__builtin_assume(i < 10u)` | 70.6325 |
| `__builtin_assume(i - 100 < 0)` | 77.2652 |
| `__builtin_assume(i < 100 \|\| i == 1000)` | 72.0602 |
| `__builtin_assume((i & 0xffff) < 100)` | 70.6325 |

The assume must be a direct `__builtin_assume(i < N)` in the *ctorFont*
conditional. `__builtin_expect`, `__builtin_unreachable`, plain uses,
and disjunctive assumes all fail.

## 4. Other re-measured fixed points (all on the baseline)

### 4.1 Language-loop `r8`/`r9` swap and tail order

Original: `lang=r8`, `i=r9` (with a `mov r9,r8` copy and an `i` reload
into `r8` for the second array index), and `id` store before `fontId` /
`ArrayAdd`. Ours: `i=r8`, `lang=r9`, and `fontId` / `ArrayAdd` / `id`
order. Everything below is **neutral at 95.6928 / 110**:

* local loop variable `unsigned int i` / `unsigned int j` / `int i`
* `unsigned int idx = i;` used for one or both array indices
* `copy_idx` before the `special` computation
* `language->id` moved before `if (special) fontId` (worse: 89.7455,
  3321 insns)
* `fontId` after `ArrayAdd` (worse: 92.1988)
* `special` as `int` / `const bool` / `9 == (i|1)` / `(i|1) == 9u` /
  `!((i|1) != 9)`
* `i++` / `++i` / `i += 1`; `(i & 0xf) != 0xa` / `(i & 15) != 10`
* `bw` after `ctorFont`; `langValue` before `fontValue`
* `PaintCanvas *canvas = Globals::Canvas;` local
* `specialFontPtr` after `rowHeight` / `int &` / removed (direct global)
* `(void) MenuTouchWindow_languageIds;` removed
* `showLanguages` as `int` / `unsigned char` / `char`
* `row` as `unsigned int`

Worse: `bw` before the font loads (76.3194), `bw` direct in the ctor
(88.9658), `GameText *gameText` local (91.5688), `row` volatile
(87.515), `row` short (95.6627).

### 4.2 iPad `sliders[2]` y-tree association

The original groups the sum as
`0x2fc + (0xc + 0x20 + 0x154 + 3*0x2c) + 0x164 + (0x15c + 0x1c + 0x2c)*2`.
The current source's left-leaning grouping scores better globally:

| variant | fuzzy |
|---|---:|
| **current (`(0x15c+0x1c)*2 + 0x154 + 0x20 + 0xc + 0x2c*5 + 0x164 + 0x2fc`)** | **95.6928** |
| exact Ghidra tree | 95.3313 |
| exact tree, `0x2fc` first / group first | 95.2711 / 95.241 |
| `0x2fc + 0x164 + group + (0x15c+0x1c+0x2c)*2` | 95.2711 |
| `0x2fc + group + (0x15c+0x1c+0x2c)*2 + 0x164` | 95.3313 |
| `0x2fc + group + 0x164 + (0x15c+0x1c)*2` | 95.5422 |
| flat exact (no group) | 95.241 |

### 4.3 Mission block `adds r4,r5,#2` hoist

Original hoists `row+2` above `getText(395)` into `r4` and keeps the
stack String address in `r5`; ours sinks it into `r3` and uses `r4` for
the String. Every spelling below is **neutral at 95.6928 / 110**:
`int row2 = row + 2;` before/inside the `if`, `const int row2`,
`short row2`, `row2` with `__builtin_assume(row2 > row)`. Worse:
`unsigned char row2` (92.6227), `volatile int row2` (92.2915).

### 4.4 `scrollExtraButton` / slider[4] operand order

`(this->field_0x15c + Globals::layout->field_0x2fc + this->sliderRowHeight) * 2`
variants (`0x2fc` first, `0x15c + (0x2fc + 0x164)`, `0x164 + ...`,
grouped) are neutral or worse (95.6627 / 95.6928).

### 4.5 `rowHeight` ternary

Current `(int)(iPad ? (float)0x34 : (float)0x34 * 0.9f) + 2` is best.
`float rowScale = iPad ? 1.0f : 0.9f;`, `0x34 * (iPad ? 1.0f : 0.9f)`,
and an if/else scale are all 91.6729 / 3321 insns. `rowHeight += 2;`
after is neutral.

## 5. Proven vs hypothesis

**Proven (measured):**
* `i < 10` (and every strong-range spelling) is unusable: it changes
  the select to an address select, moves the loop counter to `fp`, and
  shifts the stack frame by 8 bytes; 9548 B / 3330 insns / 72.06%.
* `(int) i < 10` keeps the body and frame but emits `cmp #9; bls`
  instead of `cmp #10; bcc`; 95.512%, worse than the mask because the
  2-byte-shorter backedge re-opens the downstream pool shift.
* The `__builtin_assume(i < N)` must be a direct assume in the
  ctorFont `if (special)` block; every other placement/form loses the
  `r8=i`, `sl=special` allocation (72.53%).
* The remaining 110 hunks are the r8/r9 swap, the loop-tail order, the
  mission-block ADD hoist and the `sliders[2]`/slider[4] tree
  association. No source spelling found this wave changes them without
  a size/instruction-count change.

**Hypothesis (not proven):**
* The original's `bcc` backedge came from a plain `i < 10` in a source
  shape where clang did not use the `[0,9]` range fact for the
  `special` select. The mask is the closest source-level stand-in that
  preserves the frame; the exact `cmp #10; bcc` encoding appears
  unreachable together with the value select under clang 7.0.2 `-Oz`.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5819
TMPDIR=/dev/shm/w5819/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5819/tmp cmake --build cmake-build-match --target check -j6
TMPDIR=/dev/shm/w5819/tmp cmake --build cmake-build-match --target drift -j6
# per-function score:
bash /dev/shm/w5819/score.sh
# hunk dump:
python3 /dev/shm/w5819/diffhunks.py _ZN15MenuTouchWindowC1Ei
```

Scratch (not tracked): `/dev/shm/w5819/` (`base.cpp`, `score.sh`,
`diffhunks.py`, `dump.py`, `mk.py`, `batch.py`, `exp2.py`..`exp34.py`,
`slider.py`, `orig.asm`, `ours.asm`, `rea.c`, `rea.json`).
