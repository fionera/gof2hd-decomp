# MenuTouchWindow C1/C2 wave 119 (w6718): two scheduling reorders

Date: 2026-11-05. Worktree `wt/w6718`, branch `wave119-w6718`, base
`47a1396bd` (wave-118 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Target:
`_ZN15MenuTouchWindowC1Ei` / `_ZN15MenuTouchWindowC2Ei` (weak alias, same
address/size/score).

## TL;DR

* Ctor: **99.819277 %**, 9600 B (was 99.668675 %, 9600 B).
* Two legal source reorderings fixed two of the three remaining regions:
  1. language loop: compute `y` before `rowPitch`, then `y += rowPitch * (i >> 1)`;
  2. `scrollExtraButton` y: move the `(field_0x2fc + ...) * 2` term before
     `sliderRowHeight + sliderRowHeight / 2`.
* Only region 1a (mission `row + 2` materialisation) remains: 8 instructions.
* Gate: `byte_exact 4503 / linked_exact 4503`, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27 sections, `.so`
  exactly 2,187,328 B. `.ARM.extab` 0x26290 @ 0x1a3b78 (unchanged 8 B short);
  `.rodata` 0x41c27 @ 0x1c9e10; `.text` 0x1337c8.

## 1. Language loop `0x12339e` (fixed)

Committed before: clang reassociated `(yBase - gap) - rowHeight` to
`(yBase - rowHeight) - gap`, and the two `h/2` / `w/2` roundings interleaved
differently:

```
orig: subs r1,r6,r2 ; add.w r2,r5,r5,lsr#31 ; subs r1,r1,r3 ; add.w r0,r0,r0,lsr#31
ours: subs r1,r6,r3 ; add.w r0,r0,r0,lsr#31 ; subs r1,r1,r2 ; add.w r2,r5,r5,lsr#31
```

The IR already has `%725 = sub yBase, gap; %726 = sub %725, rowHeight`, but
the DAG combiner swaps it because `field_0x30_rowHeight` is loaded first (for
`rowPitch`).  Moving the phone `y` subtraction *before* the `rowPitch`
computation forces the gap load first and the DAG keeps the original order:

```cpp
int yBase = Globals::h / 2 - this->buttonYBias;
int y = yBase - 12 - Globals::layout->field_0x29c_buttonRowGap;
int rowPitch = rowHeight + Globals::layout->field_0x30_rowHeight;
y += rowPitch * (i >> 1);
```

Measured: 9600 B, **99.759036 %** (was 99.668675 %).  The iPad branch is
unchanged.

## 2. `scrollExtraButton` y `0x123e44` (fixed)

Committed before: `add r0,r3` (`field_0x2fc + sliderRowHeight`) was scheduled
before `str r2,[sp,#4]` (the `66` height), and `ldr r4,[sp,#76]` moved with
it.  The final value is identical; only the scheduler slot differs.

Reordering the y sum so the `(field_0x2fc + ...) * 2` term precedes
`sliderRowHeight + sliderRowHeight / 2` moves the add back after the store:

```cpp
            Globals::layout->field_0x2c_rowHeight * 5 +
            (Globals::layout->field_0x2fc + ((this->field_0x15c + Globals::layout->field_0x2c_rowHeight) & 0x7fffffff) +
             (this->sliderRowHeight & 0x7fffffff)) * 2 +
            this->sliderRowHeight + this->sliderRowHeight / 2,
```

Measured on top of (1): 9600 B, **99.819277 %**.  Other tested orders:
`double_first` 9596/95.345685, `rowheight5_last` 9600/99.638554, inner
operand swaps and extra parentheses 9600/99.759036 (no change).

## 3. Remaining region: mission `row + 2` `0x122da6` (8 insns)

Original materialises `row2 = row + 2` before `getText(395)` into the
callee-saved `r4`, freeing `r5` for the `String` temporary:

```
122da6 adds r4,r5,#2
122da8 blx  getText
122dac add  r5,sp,#372
...
122dc6 mov  r2,r5
122dc8 mov  r3,r4
```

Committed tree (`char row2`) rematerialises the add after the `String` ctor
and truncates:

```
122da6 blx  getText
122daa add  r4,sp,#372
...
122dc0 adds r0,r5,#2
122dc6 uxtb r3,r0
```

Both blocks are 54 B, so the function size (9600 B) and every branch
displacement match; the 8 instructions are pure placement/register
allocation.  `int row2` alone is 9596 B (the ADD rematerialises at the use and
the extra `mov r3,r4` disappears), which shifts every branch target and scores
94.727328 %.

Tried this wave (all keep size 9600 unless noted):

| variant | size | fuzzy |
|---|---:|---:|
| committed `char row2` | 9600 | **99.819277** |
| `int row2` | 9596 | 94.727328 |
| `int row2` declared before `mission` / before the `if` | 9596 | 94.727328 |
| `const int`, `2 + row`, `row; row += 2`, `row + 1 + 1` | 9596 | 94.727328 |
| `String(*getText(395), row2 == 0)` (forces materialisation) | 9604 | 94.925463 |
| `String(*getText(395), row2 & 0x100)` / `& 0x40000000` | 9600 | 99.563187 |
| `String(*getText(395), row2 & 1)` | 9600 | 99.563187 |
| `getText(395 + (row2 == 0))` + bool | 9608 | 94.881060 |
| `getText(395 + (row2 & 0x100))` + bool | 9604 | 94.879518 |
| `getText(395 + (row2 >> 20))` | 9604 | 95.045927 |
| `int row2; (row2 = row + 2, 395)` comma | 9596 | 94.727328 |
| `String *text = getText(395); int row2` | 9596 | 94.727328 |
| `row2 & 0x7fffffff` | 9600 | 99.412562 |

The `row2 == 0` / `row2 & 0x100` forms do force the ADD into a callee-saved
register before the `String` ctor, but the comparison itself costs 4 bytes
(`clz`+`lsr`, or `ubfx`), so the function grows to 9604 or the bool value
changes.  The original's ADD is also *before* `getText`, while every forced
form places it just before the `String` ctor (after `getText`).  No legal
source shape found that both materialises the plain `int` add before
`getText` and keeps `movs r2,#0` for the copy ctor's `reverse` flag.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6718
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
```
