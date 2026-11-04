# MenuTouchWindow C1/C2 + OnTouchBegin — wave 105 (w5319)

Date: 2026-11-04. Worktree `wt/w5319`, branch `wave105-w5319`, base
`835748c18`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000).

Targets:

| symbol | size orig/ours | insns | fuzzy | structural diff lines |
|---|---|---|---|---|
| `_ZN15MenuTouchWindowC1Ei` / `C2` | 9600 / 9600 | 3320 / 3322 | **95.091840** | 2889 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | 734 / 734 | **97.683924** | 26 |

No code change landed: every variant measured this pass was neutral or worse.
The gate stayed green before and after (byte_exact 4500 / linked_exact 4500 /
size 4524 / data 843 / imports 207 / lint CLEAN / drift 0).

This pass adds one new REA datum and a few new source-shape measurements that
close off the remaining obvious routes.

## 1. REA evidence: the ctor language loop is a value select, not an address select

`rea decompile ... --procedure _ZN15MenuTouchWindowC1Ei` (evidence
`ev_5c2683c3de0b3e59e495e94516493b3b5106b7379d148749a66a6fb3791db9ab`)
gives the original loop as:

```c
for (; uVar3 = Globals::fontLangSelect, uVar24 = Globals::font,
       uVar26 < 10; uVar26 = uVar26 + 1) {
    ...
    uVar29 = uVar26 | 1;
    uVar19 = Globals::font;
    if (uVar29 == 9) {
      uVar19 = Globals::fontLangSelect;
    }
    iVar10 = PaintCanvas::FontGetSpacing(Globals::Canvas, uVar19);
    ...
    if (uVar29 == 9) {
      uVar24 = uVar3;
    }
    TouchButton::TouchButton(..., uVar24, iVar10);
    ...
    if (uVar29 == 9) {
      *(uint *)(pTVar8 + 8) = Globals::fontLangSelect;
    }
    ArrayAdd<TouchButton*>(pTVar8, *(Array **)(this + 0xb0));
}
```

So the original source's two selects are **both** `special ? lang : font`
(the ctor arg is *not* the inverted `special ? font : lang` that our current
source uses). The original loads both globals once at loop top and selects the
**values**; it does not select the addresses of the globals.

The current source is semantically inverted at the ctor arg:

```cpp
int spacing = Globals::Canvas->FontGetSpacing(
    special ? (unsigned int) langValue : fontValue);   // correct
...
new TouchButton(..., (i == 9 || i == 8) ? fontValue : (unsigned int) langValue, spacing);
//                    ^ inverted: special ? font : lang
```

The inverted form is what keeps the two selects distinct in the IR and gives
the 9600-byte / 95.09 % local optimum. Fixing the orientation makes clang CSE
the two selects into one value kept in a callee-saved register (`r4`), and the
function shrinks to 9544 B / 72.61 %.

## 2. Correct-orientation measurements (all regress)

Every variant below was applied to the language loop only, the whole `.so`
relinked, and the official verifier re-run. `diffs` is the number of
structural `|` lines excluding the header.

| variant | size | insns | fuzzy | diffs |
|---|---:|---:|---:|---:|
| **current (inverted ctor arg)** | **9600** | **3322** | **95.091840** | **2889** |
| both selects `special ? lang : font` | 9544 | 3324 | 72.606863 | 2936 |
| both selects direct `Globals::fontLangSelect : Globals::font` | 9556 | 3328 | 73.255114 | 2809 |
| ctor arg `i >= 8 ? lang : font` | 9592 | 3317 | 91.125509 | 2615 |
| ctor arg `i > 7 ? lang : font` | 9592 | 3317 | 91.125509 | 2615 |
| ctor arg `(unsigned)(i - 8) < 2 ? lang : font` | 9568 | 3333 | 74.312340 | 2894 |
| ctor arg `(i & 0xe) == 8 ? lang : font` | 9560 | 3330 | 73.624060 | 2916 |
| ctor arg `i / 8 == 1` / `i >> 3 == 1` | 9568 | 3333 | 74.312340 | 2894 |
| ctor arg `(i == 8) + (i == 9)` | 9564 | 3324 | 70.469597 | 2975 |
| swapped locals + `!special` spacing / `special` ctor | 9544 | 3324 | 72.606863 | 2936 |
| swapped locals + `!special` spacing / `i >= 8` ctor | 9592 | 3317 | 91.125509 | 2615 |
| `fontId = langValue` (instead of `*specialFontPtr`) + `i >= 8` | 9588 | 3314 | 89.810069 | 2865 |
| `fontId = langValue` + both `special` selects | 9540 | 3317 | 73.286123 | 3066 |
| `id`/`fontId` order swapped | 9600 | 3323 | 94.716243 | 2870 |
| `ArrayAdd` before `id`/`fontId` | 9596 | 3322 | 91.930142 | 2897 |
| direct `ArrayAdd(language, *this->buttonsB0)` + `i >= 8` | 9612 | 3325 | 88.216704 | 2693 |
| `langList` declared inside loop after the font locals + `i >= 8` | 9572 | 3332 | 72.970535 | 3108 |
| `fontValue` declared `int`/`unsigned`/`long` + `i >= 8` | 9592 | 3317 | 91.125509 | 2615 |
| all six declaration orders of `{special,fontValue,langValue}` + `i >= 8` | 9592 | 3317 | 91.125509 | 2615 |
| `fontValue`/`langValue` hoisted outside the loop + `i >= 8` | 9592 | 3317 | 91.125509 | 2615 |
| volatile reads of both globals + both `special` selects | 9532 | 3312 | 70.687575 | 2957 |
| volatile read of only the ctor-arg global | 9556 | 3328 | 73.255114 | 2809 |

### Why `i >= 8` is the interesting near miss

`i >= 8` is the only correct-orientation spelling that produces **two value
selects** (`cmp r8,#7; it hi; movhi r6,fp`) and keeps both `Globals::font` /
`Globals::fontLangSelect` in registers across the calls. It is 8 B / 3 insns
short of the original because the allocator keeps `i` in `r8` and `lang` in
`fp`; the original keeps `lang` in `r8` and `i` in `r9`, which forces the
`mov r9,r8` + `str r9,[sp,#80]` + `ldr r8,[sp,#80]` save/restore triad:

```
orig 123400  mov  r9,r8          ; save i
     12341a  ldr.w r8,[r2]       ; r8 = lang
     12343c  str.w r9,[sp,#80]   ; spill i
     12347a  ldr.w r8,[sp,#80]   ; restore i for the loop counter
     1234a2  add.w r8,r8,#1
     1234aa  cmp.w r8,#10
     1234ae  bcc.w
```

Ours (`i >= 8`) keeps `r8 = i` throughout, `fp = lang`, and does not need the
save/restore. No source spelling tried this pass (declaration order, local
hoisting, `fontId` source, `ArrayAdd` source, swapped locals, volatile reads)
moves `lang` into `r8` while keeping the two value selects and the 9600-byte
size.

### 2.1 Where the 2 extra instructions are

The language loop is instruction-for-instruction the same length as the
original; the 3322-vs-3320 delta is in the iPad fire-image / cinematic block
just before `cinematicBtnB`:

```
orig 123ae2  movs r0,#200
     123ae4  blx  operator new
     123ae8  mov  r5,r0
     123aea  ldr  r0,[sp,#96]
     123aec  ldr  r0,[r0,#0]        ; Globals::gameText
     123aee  movw r1,#497
     123af2  blx  GameText::getText

ours 123ad6  mov  sl,r4              ; extra: restore &Globals::Canvas
     123ada  ldr  r0,[sp,#96]
     123adc  ldr.w r9,[r0]           ; extra: cache gameText in r9
     123ae0  movs r0,#200
     123ae2  blx  operator new
     123ae6  mov  r6,r0
     123ae8  mov  r0,r9              ; extra: move gameText back
     123aea  movw r1,#497
     123aee  blx  GameText::getText
```

`sl` is used as a temporary in the preceding `Globals::setCoordsFire(...)`
argument setup, so the compiler must restore `&Globals::Canvas` into `sl`
before the four `Image2DCreate(1701..1704, ...)` calls; the original keeps
`sl = &Globals::Canvas` across that whole block. Adding a local
`PaintCanvas *canvas = Globals::Canvas;` before the four calls makes it worse
(9596 B / 3323 insns / 92.909830); `PaintCanvas *&`, `PaintCanvas *const &`,
`PaintCanvas *volatile &` and `PaintCanvas **canvasPtr` are all neutral
(9600 / 3322 / 95.091840). The fix, if any, is in the `setCoordsFire` argument
setup, not in the `Image2DCreate` calls.

## 3. OnTouchBegin: the address spill is a rematerialisation tie-break

Current case-15 shape (reference + hoisted `top`) is the best at
**1896 B / 26 structural lines / 97.683924 %**. The two residual regions are
unchanged from wave 103:

1. case 8: `ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]` (2 lines);
2. case 15: `[sp,#16]` address reloads + `r6`/`r5` register permutation
   (24 lines).

New shapes measured this pass (all worse than 26 lines):

| variant | size | insns | fuzzy | diffs |
|---|---:|---:|---:|---:|
| **current (reference + hoisted `top`)** | **1896** | **734** | **97.683924** | **26** |
| direct `Globals::layout->...` | 1892 | 732 | 88.403820 | 157 |
| direct + `PaintCanvas *canvas` local in block | 1892 | 732 | 88.130969 | 157 |
| `canvas` at case-15 start + direct | 1888 | 733 | 87.116564 | 194 |
| `Layout **lp = &Globals::layout` before the scrollSlots loop | 1892 | 732 | 88.403820 | 157 |
| `Layout **volatile lp` before the loop | 1932 | 749 | 75.927175 | 736 |
| reference first / direct second+third | 1892 | 732 | 88.403820 | 157 |
| reference first / direct `top`+`leftMargin` only (mixed) | 1896 | 734 | 94.414169 | 54 |
| explicit `layout = Globals::layout` reload after each call (nested `if`) | 1892 | 732 | 89.904502 | 156 |
| explicit reload + `PaintCanvas *canvas` local | 1892 | 732 | 90.040928 | 154 |
| `*(Layout *volatile *)&Globals::layout` for second/third accesses | 1896 | 733 | 96.250852 | 121 |
| `Layout *volatile layout` local | 1936 | 751 | 75.286195 | 739 |
| `Layout **lp = &Globals::layout` at function top | 1892 | 732 | 88.403820 | 157 |

The direct shape already reloads the **value** of `Globals::layout` after each
call (`ldr r2,[fp]` with `fp = &Globals::layout`). The original instead
rematerialises the **address** from the `[sp,#16]` spill
(`ldr r2,[sp,#16]; ldr r2,[r2,#0]`) and reloads it again for the third
condition. The address is in `fp` in every shape where `fp` is free; the
original's `fp` holds `Globals::Canvas`. The `PaintCanvas *canvas` local does
put Canvas in a callee-saved register but the allocator still prefers `fp` for
the layout address, and the extra `mov r0,r5` costs a size/ratio regression.

## 4. Next hypotheses

1. **Ctor**: find a source shape that makes clang choose `r8 = lang`,
   `r9 = i` (i.e. emit the `mov r9,r8` / `str r9,[sp,#80]` triad) while
   keeping two value selects. The `i >= 8` variant is the right control-flow
   shape but the wrong register assignment; the missing lever is likely the
   live range of `lang` vs the loop counter. Try a second post-ctor use of
   `lang` that does not add an instruction (e.g. a different `fontId` store
   spelling), or a local `Array<TouchButton*> *langList` whose load is moved
   before `operator new` so it competes for `fp`.
2. **OnTouchBegin**: force the layout address out of `fp` without adding a
   `mov`. The original's `fp = Globals::Canvas`; if a source shape can make
   Canvas the first callee-saved value loaded in the block (before the layout
   address), the layout address should fall back to the `[sp,#16]` spill and
   reproduce the two address reloads. The `canvas` local is close but adds a
   `mov r0,r5` before each `GetImage2D*` call.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5319
TMPDIR=/dev/shm/w5319/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5319/tmp cmake --build cmake-build-match --target check -j6

GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure _ZN15MenuTouchWindowC1Ei --format json > /dev/shm/w5319/rea-ctor.json
```

Scratch (not tracked): `/dev/shm/w5319/` (`MenuTouchWindow.base.cpp`,
`measure.py`, `rea-ctor.json`, `ctor.c`, `loop-orig.txt`, `loop-ours.txt`).
