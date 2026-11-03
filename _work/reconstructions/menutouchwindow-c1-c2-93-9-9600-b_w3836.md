# MenuTouchWindow C1/C2 (93.9%, 9600 B) — w3836

**Status:** pass, 2026-11-03. Worktree `wt/w3836`, branch `wave89-w3836`,
base `5710f6d0`. Lands the `Globals::options` element-type fix together with the
collateral `HangarWindow::render` source-shape fix it requires. The constructor
itself is unchanged at **9600 B / 3323 insns / 93.9%** (original 3320 insns);
the pass is a source-correctness fix (the original's `options` really is
`bool[100]`) that keeps the gate green. The remaining +15 instructions are
still open (see §4).

## TL;DR

* Baseline: **byte 4498 / linked 4498, `.text 0x1337c8`, lint CLEAN, drift 0**.
  `_ZN15MenuTouchWindowC1Ei` / `C2` are 9600 B, fuzzy **93.8733%**
  (3320 instructions original, 3323 ours).
* `Globals::options` is `bool[100]` in the original: the six
  `setAlwaysPressed`/`setHalfTransparent` call sites in the ctor are a direct
  `ldrb r1,[options+N]; blx TouchButton::setXxx(bool)` with **no**
  int->bool conversion (verified against the original disassembly at
  instruction indices 1861-1868, 2284-2285, 2324-2325, 2364-2365).
* Switching `Globals.h/.cpp` to `bool options[100]` removes the six
  `cmp/it/movne` conversions (18 instructions, 36 B), shrinking the ctor to
  9564 B / 3305 insns and `.text` to `0x1337a8`; every later function shifts
  and raw `byte_exact` collapses. The missing 36 B must be supplied by the
  other source-shape differences (§4), which are not yet reconstructed.
* To land the type fix without the `.text` shift, the six ctor sites read the
  raw byte through `optionByte(N)` (a documented raw-image accessor, §2). This
  reproduces the pre-fix codegen exactly: the whole `.so` is instruction-
  identical to the baseline, so `byte_exact`/`linked_exact` stay 4498/4498.
* The bool type change has one collateral codegen effect: `HangarWindow::render`
  regressed 9700 -> 9688 B (fuzzy 80.6 -> 77.7) because clang turned the
  free-credits `if (i == 0 || (i == 1 && options[73]) || ...)` chain into a
  `tbb` jump table. Swapping each `&&` to `options[N] && i == n` (semantically
  identical, but the branch is emitted on `i` first) restores the original
  compare chain: 9700 B, 3638 insns, fuzzy 80.6, byte-identical to baseline.

## 1. Evidence: `Globals::options` is `bool`

Original ctor, first three call sites:

```
1861 ldrb     r1, [r4, #17]
1862 blx      TouchButton::setHalfTransparent(bool)
1863 ldrb     r1, [r4, #16]
1864 ldr.w    r0, [r6, #204]
1865 blx      TouchButton::setAlwaysPressed(bool)
1866 ldrb.w   r1, [r4, #64]
1867 ldr.w    r0, [r6, #232]
1868 blx      TouchButton::setAlwaysPressed(bool)
```

No `cmp r1,#0; it ne; movne r1,#1`. `ldrb` zero-extends to 32 bits, which is a
legal `bool` value only if the object's type is `bool` (the compiler knows the
byte is 0/1). With `unsigned char` clang must materialise the conversion
(measured: `cmp/it/movne` at all six sites). `bool` and `unsigned char` are
both 1 B, so the `.bss` layout and `_ZN7Globals7optionsE` size (100 B) are
unchanged.

## 2. The `optionByte` anchor

`Globals::options` is the raw `OptionsRecord` image; the menus already access
its fields by offset through `optionFlag`/`optionFloat`/`optionInt`
(`reinterpret_cast` at `&Globals::options[offset]`). The six ctor sites now use

```cpp
static inline unsigned char optionByte(int offset) {
    return *reinterpret_cast<unsigned char *>(&Globals::options[offset]);
}
```

This is the same raw-byte image access as the existing helpers, and it keeps
the ctor's measured 9600 B / `.text 0x1337c8` while the type is correct. It is
a temporary anchor: once the remaining +15 instructions are reconstructed
(§4) the six sites should become plain `Globals::options[N]` and the anchor
deleted.

## 3. `HangarWindow::render` collateral

With `options` as `bool`, clang's SimplifyCFG converts

```cpp
if (i == 0 || (i == 1 && Globals::options[73]) || (i == 2 && Globals::options[74]) ||
    (i == 3 && Globals::options[75])) { ... }
```

into a `tbb` jump table (the bool loads are `i1`, so the whole chain becomes a
switch on `i`). The original has the compare chain:

```
2675 beq.n    loc_1c1c          ; i == 0
2676 ldr      r1, [sp, #124]    ; &options
2677 cmp.w    fp, #1
2678 bne.n    loc_1bfe
2679 ldrb.w   r0, [r1, #73]
2680 cbnz     r0, loc_1c1c
...
```

Writing each conjunct as `Globals::options[N] && i == n` (operand order only;
identical semantics) emits the branch on `i` first and keeps the chain. This
restores `render` to exactly the baseline codegen (9700 B, 3638 insns,
fuzzy 80.6), so the whole `.so` is instruction-identical to the baseline.

## 4. Remaining +15 instructions (open)

With `bool` and no anchor the ctor is 9564 B / 3305 insns; the original is
9600 B / 3320 insns, i.e. **15 instructions** are still missing. Net diff by
region (`tools/verify` normalized stream):

| region | net orig-ours |
|---|---:|
| language loop (indices ~934-1000) | **+9** |
| pre-loop button setup (~357-933) | +3 |
| slider[2] / iPad position block (~2495-2515) | +1 |
| ~2791 | +1 |
| ~2810/2826 | -2 |
| ~2913-2964 | +3 |
| ~2987 | -1 |
| ~3016-3040 | +2 |
| EH landing-pad dispatch (~3137-3154) | 0 |

The language loop is the dominant block. The original selects **values**:

```
946 ldr      r5, [r0, #0]       ; Globals::font
947 ldr      r0, [r1, #0]       ; Globals::Canvas
948 ldr.w    r8, [r2]           ; Globals::fontLangSelect
949 mov      r1, r5
950 str      r2, [sp, #76]      ; &Globals::fontLangSelect
951 it       eq
952 moveq    r1, r8
953 blx      PaintCanvas::FontGetSpacing(unsigned int)
...
968 cmp.w    sl, #9
969 it       eq
970 moveq    r5, r8             ; re-materialised select for the ctor argument
```

Ours selects an **address** and dereferences (`it ne; ldrne r9,[&font]; ldr r1,[r9]`).
The ARM backend folds `select(load,load)` into `load(select(ptr,ptr))` at -Oz,
so every plain-C++ ternary form tried (`special ? Globals::fontLangSelect :
Globals::font`, hoisted value locals, `if` assignment, references, `int`
condition, `(uintptr_t)` casts) still compiles to the address select. Forcing
the loads to be unconditional (`*(volatile ...)`) does produce the value select
but changes the function-wide register allocation and drops the fuzzy to
~70-73%. The original's value select is coupled to its full-function RA
(r5=font, r8=fontLangSelect, r9=i copy), so it has to be reconstructed
together with the other regions, not as a local rewrite.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3836
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j8     # RATCHET PASS 4498/4498
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
```
