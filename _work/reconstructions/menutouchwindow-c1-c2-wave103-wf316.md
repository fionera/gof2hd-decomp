# MenuTouchWindow C1/C2 wave 103 (wf316): the 95.0918% local optimum holds;
# the ctor-arg select is semantically inverted in our source

**Status:** analysis only, 2026-11-04. Worktree `wt/wf316`, branch
`wave103-wf316`, base `1d2b4398e`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size).

## TL;DR

* **Before = after:** 9600 B, **3322 instructions** (orig 3320), exact
  difflib ratio **0.9509183980728696** (95.0918%). No code change
  landed: every variant tried this pass was neutral or worse.
* Two new, durable facts:
  1. **Our ctor-arg select is semantically inverted.** The original
     passes `special ? Globals::fontLangSelect : Globals::font` as
     `TouchButton` arg 8; our source passes
     `special ? fontValue : langValue`. This is a real behavioural
     difference (the 8/9 language buttons get the wrong font id). The
     oracle (`game/MenuTouchWindow.c` lines 400-420) agrees with the
     binary: one `uVar19` is used for both `FontGetSpacing` and the
     ctor. Fixing it, however, makes clang CSE the two selects and the
     function drops to 9544 B / 72.61%.
  2. **The faithful `TouchSlider[2]` grouping (pass-10 §5) measures
     94.82%** with the current loop allocation — better than the pass-10
     `94.64%` variant but still below the 95.09% baseline. Its *block*
     is nearly instruction-for-instruction identical to the original;
     the loss is entirely the downstream register-allocation cascade.

## 1. Baseline and gate

```
size 9600 (orig 9600)  insns 3322 (orig 3320)
ratio 95.091840  (report.json pct 95.1)
```

Full `check` before and after: `byte_exact 4500/4524`,
`linked_exact 4500/4524`, `size_exact 4524/4524`, `stub_zero_size 0`,
`missing 0`, `wrong_type 0`, `extra 0`, `data_exact 843/843`,
`imports 207`, `lint CLEAN`, `drift 0`, `RATCHET PASS`.

## 2. The ctor-arg select is inverted (proven against the binary)

Original, language loop (VMA 0x1233fa-0x1234ae). GOT resolution of the
three PIC literals at 0x1236d0/0x1236d4/0x1236d8 gives
`&Globals::font` (0x21029c), `&Globals::Canvas` (0x2101f4) and
`&Globals::fontLangSelect` (0x210474):

```
123416  ldr     r5, [r0, #0]      ; r5 = Globals::font
123418  ldr     r0, [r1, #0]      ; Canvas
12341a  ldr.w   r8, [r2]          ; r8 = Globals::fontLangSelect
12341e  mov     r1, r5            ; r1 = font
123422  it      eq
123424  moveq   r1, r8            ; r1 = lang if special   (spacing arg)
123426  blx     FontGetSpacing
...
123450  cmp.w   sl, #9
123454  it      eq
123456  moveq   r5, r8            ; r5 = lang if special   (ctor arg 8)
```

Both selects are `special ? lang : font`. Our source (line 460) is

```cpp
(i == 9 || i == 8) ? fontValue : (unsigned int) langValue, spacing);
```

i.e. `special ? font : lang` — **inverted**. The oracle confirms the
binary's orientation (`uVar19` is used for both the spacing call and the
ctor).

Why we do not fix it: with both selects in the same orientation clang
CSEs them into one `select` (the original has *two* `cmp sl,#9` +
`moveq`), and the function shrinks to 9544 B / 3324 insns / 72.61%.
The original source must keep the two selects non-identical in the IR
even though they compute the same value; no source spelling found so far
does that without also changing the code size.

Condition spellings tried for the second select (all on top of the
baseline; orientation flipped to `special ? lang : font`):

| second-select condition | size | insns | ratio |
|---|---|---|---|
| `special` (== `(i\|1)==9`) | 9544 | 3324 | 72.6069 |
| `i == 8 \|\| i == 9` | 9544 | 3324 | 72.6069 |
| `(i == 8) \| (i == 9)` | 9544 | 3324 | 72.6069 |
| direct `Globals::fontLangSelect : Globals::font` | 9556 | 3328 | 73.2551 |
| `(i & 0xe) == 8` | 9560 | 3330 | 73.6241 |
| `(unsigned)(i - 8) < 2` | 9568 | 3333 | 74.3123 |
| `(i >> 1) == 4` | 9600 | 3325 | 82.7690 |
| `i >= 8` / `i > 7` | 9592 | 3317 | 91.1255 |

`i >= 8` is the only spelling that yields two same-orientation selects
with the correct `ittt eq; ldreq; ldreq; streq` fontId store, but it is
8 B short and the exception-table tail rotates, costing ~4 points.

## 3. `TouchSlider[2]` y-expression: the faithful grouping (94.82%)

The original grouping (VMA 0x12462e-0x1246b0, decoded from the raw
Thumb):

```
r2 = layout->field_0x1c + layout->field_0x2c_rowHeight
r1 = this->field_0x15c + r2
r2 = layout->field_0x20_top + layout->field_0xc_leftMargin
r2 += this->listEntryWidth                       ; 0x154
r6 = rowHeight*3
r0 = layout->field_0x2fc
r2 += r6
r0 += r2
r0 += this->sliderRowHeight                      ; 0x164
r0 += r1*2
```

i.e.

```cpp
(int) ((float) (Globals::layout->field_0x2fc +
                (Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top +
                 this->listEntryWidth + Globals::layout->field_0x2c_rowHeight * 3) +
                this->sliderRowHeight +
                (Globals::layout->field_0x1c + Globals::layout->field_0x2c_rowHeight +
                 this->field_0x15c) * 2) +
       (Globals::iPadHD ? 19.6875f : (Globals::iPadLarge ? 28.0f : 14.0f)));
```

(Note: the pass-10 note said `listEntryHeight`; the binary loads
`this+0x154`, which is `listEntryWidth`. The handoff's `0x124636`
block is actually `sliders[2]`, not `sliders[1]`; `sliders[1]` is
0x1245b0-0x12462a and already matches.)

Measured variants of this one expression:

| grouping | size | insns | ratio |
|---|---|---|---|
| current (`(0x15c+0x1c)*2 + 0x154 + 0x20 + 0xc + rowHeight*5 + 0x164 + 0x2fc`) | 9600 | 3322 | **95.0918** |
| faithful, `0x2fc` first (above) | 9600 | 3322 | 94.8208 |
| faithful, `0x2fc` after the group | 9600 | 3322 | 94.6402 |
| oracle order (`0x2fc + 0x154 + 0x20 + 0xc + 0x15c*2 + rowHeight*4 + 0x1c*2`) | 9592 | 3318 | 92.0458 |
| `0xc` first, `0x2fc` middle | 9604 | 3323 | 93.0002 |

The faithful grouping's block is almost identical to the original
(loads `layout` fields first, then `this` fields, same add tree); the
remaining loss is the register permutation (`lr/r9/r3/r6` vs
`lr/r3/r9/r6`) and the whole-function cascade it triggers.

## 4. Everything else tried this pass (all neutral or worse)

Language-loop source permutations (exact verifier, full TU rebuild):

| variant | size | insns | ratio |
|---|---|---|---|
| baseline | 9600 | 3322 | **95.0918** |
| `int bw = this->buttonWidth` removed (`this->buttonWidth` inline) | 9592 | 3315 | 90.9118 |
| `langList` removed (`ArrayAdd(language, *this->buttonsB0)`) | 9604 | 3321 | 89.6251 |
| both of the above | 9612 | 3323 | 88.0927 |
| `fontValue`/`langValue` removed (direct globals, old orientation) | 9556 | 3328 | 73.2250 |
| `fontId = langValue` (drop `specialFontPtr`) | 9588 | 3314 | 89.8402 |
| `specialFontPtr = &langValue` | 9588 | 3314 | 89.8402 |
| `bw` declared before `spacing` (any position) | 9640 | 3334 | 76.1046 |
| `special` declared after `bw` | 9640 | 3334 | 76.1046 |
| all 6 declaration orders of `{special,fontValue,langValue}` | 9600 | 3322 | 95.0918 |
| `language` declared outside the loop | 9600 | 3322 | 95.0918 |
| `spacing`/`bw` declared outside the loop | 9600 | 3322 | 95.0918 |
| `int i` (vs `unsigned int i`) | 9600 | 3322 | 95.0918 |
| `i <= 9` | 9600 | 3322 | 95.0617 |
| `both int` / `both unsigned` fontValue/langValue | 9600 | 3322 | 95.0918 |
| `special = i == 8 \|\| i == 9` | 9600 | 3322 | 95.0918 |
| `missionRow = row + 2` hoisted before `getText(395)` | 9600 | 3322 | 95.0918 |
| `textId` hoisted before `new TouchButton` | 9600 | 3322 | 95.0918 |
| cinematicText first-direct/second-local (local kept at top) | 9600 | 3322 | 95.0918 |
| cinematicText local moved after first `new` | 9596 | 3321 | 93.3594 |
| cinematicText both direct | 9592 | 3319 | 93.4177 |
| `PaintCanvas *canvas` local in loop | 9600 | 3322 | 95.0918 |

Combinations of the faithful slider[2] grouping with the other
candidates (cinematicText first-direct, missionRow hoist, `i >= 8`,
direct `buttonsB0`) were all <= 94.82%.

## 5. Conclusion / next route

The function is at a strong local optimum. The remaining diff is one
register permutation in the language loop (`fp/r5/r6/r8` 4-cycle,
pass-10 §3) plus the 2-instruction size delta that moves every
post-loop branch target and the exception-table tail. Two faithful
source fixes are known but currently lose:

1. ctor arg 8 should be `special ? lang : font` (currently inverted);
2. `TouchSlider[2]` y should use the `rowHeight*3 + (0x15c+0x1c+rowHeight)*2`
   grouping (currently `rowHeight*5 + (0x15c+0x1c)*2`).

Both should be re-applied and re-measured the moment a source shape is
found that reproduces the original loop allocation. Do not land either
in isolation — each is a measured regression on its own.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf316
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump "$OD" --show _ZN15MenuTouchWindowC1Ei

# language loop
$OD -d -M force-thumb --start-address=0x1233fa --stop-address=0x1234b0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
# sliders[2]
$OD -d -M force-thumb --start-address=0x12462e --stop-address=0x1246b4 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

Scratch (not tracked): `/dev/shm/wf316/` (`base.cpp`, `exact.py`,
`measure.sh`, `ndiff.py`, `try.py`, `sl2.py`, `sl3.py`, `v*.json`).
