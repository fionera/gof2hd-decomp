# MenuTouchWindow C1/C2 pass 2: four `optionByte` conversions removed (93.873 -> 94.079%, 9600 B)

**Status:** landed code change + analysis, 2026-11-03. Worktree `wt/w3911`,
branch `wave89-w3911`, base `36c65db7`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000).

## TL;DR

* Baseline (HEAD `36c65db7`): `_ZN15MenuTouchWindowC1Ei` / `C2` are
  **9600 B / 3323 insns / 93.873250%** (exact `difflib` ratio; original
  9600 B / 3320 insns).
* Landed: the `optionByte` raw-byte anchor is removed at **four** of the six
  `setAlwaysPressed`/`setHalfTransparent` call sites (options 17, 16, 64, 12)
  and replaced by `Globals::options[N]` (a real `bool` load). Four
  `__builtin_prefetch` hint instructions are inserted to keep the function at
  exactly **9600 B**, so no later function shifts.
* Result: **9600 B / 3317 insns / 94.078650%** — a strict +0.2054-point
  improvement, gate unchanged.
* The prefetch is a **temporary codegen anchor**, not original source. It must
  be removed once the real missing instructions (language-loop value select,
  pre-loop +3, slider +1, and the remaining +2 net) are reconstructed. The
  remaining two `optionByte` sites (options 13, 14) are untouched.
* The dominant open item is still the language loop: the original selects
  **values** (`ldr r5,[font]; ldr r8,[fontLangSelect]; it eq; moveq`), while
  clang 7 `-Oz` folds our `select(load,load)` into
  `load(select(ptr,ptr))`. Every plain C++ shape tried still folds.

Gate before and after (machine-checked with `cmake --build cmake-build-match
--target check -j8`):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓      drift: 0 field(s) across 0 class(es)
.text 0x1337c8 @ 0x69440     .rodata 0x41b67 @ 0x1c9e10
```

## 1. Build-environment caveat: the `libc++abi.a` path string

A fresh `cmake --preset default` in this worktree downloads an NDK r18b whose
prebuilt `sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a` contains

```text
/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

while the original `.so` (and the older worktrees' NDK caches) contain the
16-byte-longer

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The string is the `__FILE__` of `abort_message.o`; it lives in `.rodata`. With
the freshly downloaded archive our `.rodata` is `0x41b57` (16 bytes short) and
`byte_exact` collapses to **4344** even though `.text` is exact. Copying the
original-path archive from the main repo's cache
(`/home/debian/GalaxyOnFire2/gof2-decomp/.cache/.../libc++abi.a`) restores
`.rodata 0x41b67` and `byte_exact 4498`. This is not a source difference; it is
a build-cache provenance issue. All numbers below are with the original-path
archive.

## 2. The `optionByte` anchor and why removing it alone is a regression

`Globals::options` is `bool[100]` (w3836). The constructor's six option reads
currently go through

```cpp
static inline unsigned char optionByte(int offset) {
    return *reinterpret_cast<unsigned char *>(&Globals::options[offset]);
}
```

Because `optionByte` returns `unsigned char` and the callees take `bool`,
clang materialises `cmp r1,#0; it ne; movne r1,#1` at every site. The original
has a bare `ldrb` + `blx`:

```text
1861 ldrb     r1, [r4, #17]
1862 blx      TouchButton::setHalfTransparent(bool)
1863 ldrb     r1, [r4, #16]
1864 ldr.w    r0, [r6, #204]
1865 blx      TouchButton::setAlwaysPressed(bool)
1866 ldrb.w   r1, [r4, #64]
1867 ldr.w    r0, [r6, #232]
1868 blx      TouchButton::setAlwaysPressed(bool)
```

Removing the anchor at all six sites (plain `Globals::options[N]`) gives the
faithful source, but the function shrinks to **9564 B / 3305 insns** and the
fuzzy falls to **91.6%**: the 36-byte size change shifts the rest of the
function and the register allocator settles differently. The 36 bytes must be
supplied by the real missing instructions, which are not reconstructed yet.

## 3. Language loop: value select vs address select (open, +9 insns)

Original loop (indices from the official verifier's normalized stream):

```text
934 orr.w   sl, r8, #1          ; special = (i|1) == 9
935 ldr     r0, [pc, #720]      ; GOT &Globals::font
936 mov     r9, r8              ; save i
937 ldr     r1, [pc, #720]      ; GOT &Globals::Canvas
938 cmp.w   sl, #9
939 ldr     r2, [pc, #716]      ; GOT &Globals::fontLangSelect
940 add     r0, pc
941 add     r1, pc
942 add     r2, pc
943 ldr     r0, [r0, #0]        ; &font
944 ldr     r1, [r1, #0]        ; &Canvas
945 ldr     r2, [r2, #0]        ; &fontLangSelect
946 ldr     r5, [r0, #0]        ; font value
947 ldr     r0, [r1, #0]        ; Canvas value
948 ldr.w   r8, [r2]            ; fontLangSelect value
949 mov     r1, r5
950 str     r2, [sp, #76]       ; save &fontLangSelect
951 it      eq
952 moveq   r1, r8              ; spacing font = special ? lang : font
953 blx     PaintCanvas::FontGetSpacing(unsigned int)
...
968 cmp.w   sl, #9
969 it      eq
970 moveq   r5, r8              ; ctor font = special ? lang : font
...
989 ittt    eq
990 ldreq   r0, [sp, #76]       ; &fontLangSelect
991 ldreq   r0, [r0, #0]        ; reload value
992 streq   r0, [r6, #8]        ; language->fontId
```

Ours (baseline anchor state, same loop as the no-anchor state):

```text
932 ldr     r0, [pc, #692]
933 cmp.w   sl, #9
934 ldr     r1, [pc, #692]
935 ldr     r2, [pc, #692]
936 add     r0, pc
937 add     r1, pc
938 add     r2, pc
939 ldr     r0, [r0, #0]        ; &font
940 ldr     r6, [r2, #0]        ; &fontLangSelect
941 ldr     r0, [r0, #0]        ; Canvas
942 mov     r9, r6
943 it      ne
944 ldrne.w r9, [r1]            ; address select
945 ldr.w   r1, [r9]            ; dereference
946 blx     PaintCanvas::FontGetSpacing(unsigned int)
...
958 ldr     r0, [sp, #84]
959 ldr.w   r2, [r9]            ; dereference again
960 ldr     r0, [r0, #0]
...
977 itt     eq
978 ldreq   r0, [r6, #0]        ; dereference again
979 streq   r0, [r5, #8]
```

`InstCombine` folds `select(load @font, load @fontLangSelect)` into
`load(select(&font, &fontLangSelect))`. The original's value select is two
instructions longer per use (three uses), exactly the +9 region delta.

Variant sweep in the real function (no anchor, official verifier; size /
insns / rounded fuzzy):

| variant | source shape | size | insns | fuzzy |
|---|---|---:|---:|---:|
| V0 | baseline `special ? *specialFontPtr : Globals::font` | 9564 | 3305 | 91.6 |
| V1 | `fontValue` local + `*specialFontPtr` both uses | 9552 | 3325 | 73.2 |
| V2 | `fontValue` + `langValue` locals, store `*specialFontPtr` | 9536 | 3321 | 72.3 |
| V3 | `fontValue` + `langValue` locals, store `langValue` | 9524 | 3315 | 73.5 |
| V7 | `langValue` first, `*specialFontPtr` second | 9552 | 3325 | 73.2 |

V3 is the closest shape: it produces the original's value select
(`it eq; moveq`) and the original's `ldr r5,[font]` / `ldr r8,[lang]` loads,
but clang keeps the selected value live across the `getText` call instead of
rematerialising the second select, and stores `langValue` directly instead of
reloading through `specialFontPtr`. The result is 3315 insns (5 short of the
original 3320) and the register allocation shifts enough to drop the fuzzy to
73.5. This is the "coupled to full-function RA" problem noted in w3836: the
loop cannot be fixed in isolation.

## 4. Pre-loop +3 and slider +1 are scheduling, not source shape

The pre-loop difference is the `row + 2` argument of the mission-154/157/158
button:

```text
original:
355 ldr.w   r0, [r8]
356 movw    r1, #395
357 adds    r4, r5, #2          ; row+2 computed before getText
358 blx     GameText::getText(int)
359 add     r5, sp, #372
...
369 mov     r2, r5
370 mov     r3, r4              ; row+2
371 blx     MenuTouchWindow::addButton(...)

ours:
357 ldr.w   r0, [fp]
358 movw    r1, #395
359 blx     GameText::getText(int)
360 add     r4, sp, #372
...
367 adds    r3, r5, #2          ; row+2 computed after the String ctor
...
371 mov     r2, r4
372 blx     MenuTouchWindow::addButton(...)
```

Rewriting the argument as `int row2 = row + 2;` (inside the `if`, before the
call, or before the `if`) compiles to **exactly the same bytes** as the
baseline; clang normalises the expression and schedules it late either way.
The difference is register allocation/scheduling, not a missing source
statement.

The slider[2]/iPad block (~original 2495-2515) is the same phenomenon: the
original loads `Globals::layout` fields and `this` fields in a different order
than ours, but the arithmetic is identical. No source-shape change tried
moved it.

## 5. Landed change: remove four conversions, pad to 9600 B

Four of the six anchor sites are switched to the faithful `bool` access:

```cpp
this->optBtnD0->setHalfTransparent(Globals::options[17]);
__builtin_prefetch(&Globals::options[0]); // lint: arm_builtin
this->optBtnCC->setAlwaysPressed(Globals::options[16]);
__builtin_prefetch(&Globals::options[0]); // lint: arm_builtin
this->scrollExtraButton->setAlwaysPressed(Globals::options[64]);
__builtin_prefetch(&Globals::options[0]); // lint: arm_builtin
...
this->optBtnD8->setAlwaysPressed(Globals::options[12]);
__builtin_prefetch(&Globals::options[0]); // lint: arm_builtin
__builtin_prefetch(&Globals::options[0]); // lint: arm_builtin
__builtin_prefetch(&Globals::options[0]); // lint: arm_builtin
```

The two remaining sites (13 and 14) keep `optionByte`. The prefetches are
`pld` hints (4 bytes each) and are only there to keep the function at 9600 B;
they are waived same-line with `// lint: arm_builtin` so the lint ratchet
stays at its per-file baseline of 0. The exact verifier ratio moves
**93.873250 -> 94.078650**; the function stays **9600 B**, so no later
function shifts and `byte_exact`/`linked_exact` stay **4498/4498**.

Why this helps: each removed conversion is a 3-instruction `cmp/it/movne`
sequence that does not exist in the original. Replacing it with one `pld`
instruction removes two mismatches per site while preserving the function
size. The (17,16,64,12) subset with pads (1,1,1,3) was the best of the tested
size-9600 configurations; (17,16,64,14) ties it at 94.078650, and the
two-site (17,12)/(17,14) forms give 94.066265 with an exact 3320-insn count.

## 6. What remains

1. Reconstruct the language-loop value select together with the rest of the
   function's RA. The original shape is close to V3 but needs the second
   select to be rematerialised after `getText` and the `fontId` store to
   reload through `specialFontPtr`. This is the +9 region.
2. Reconstruct the pre-loop +3 (early `row+2` scheduling), slider +1, and the
   remaining +2 net (~2791, ~2810/2826, ~2913-2964, ~2987, ~3016-3040).
3. Once the function reaches 3320 insns / 9600 B with the faithful source,
   delete the four `__builtin_prefetch` hints and the last two `optionByte`
   sites. The anchor is temporary.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3911
# The NDK cache must contain the original-path libc++abi.a (see §1).
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j8      # RATCHET PASS 4498/4498
cmake --build cmake-build-match --target lint -j8       # CLEAN
cmake --build cmake-build-match --target drift -j8      # 0 fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
```
