# MenuTouchWindow C1/C2 (93.9%, 9600 B) — w3816

**Status:** research note, 2026-11-03. Worktree `wt/w3816`, branch `wave88-w3816`,
base `41804126`. No source change landed: the one real source-type defect found
(`Globals::options` is `bool`, not `unsigned char`) removes 36 bytes from the
constructor and therefore cannot be landed alone without a `.text` shift and a
`byte_exact` collapse (4498 -> 2672). This commit is the note only.

## TL;DR

* Baseline: **byte 4498 / linked 4498, size-exact 4524/4524, `.text 0x1337c8`,
  lint CLEAN, drift 0**. `_ZN15MenuTouchWindowC1Ei` / `C2` are 9600 B, fuzzy
  **93.8733%** (3320 instructions original, 3323 ours).
* The single most important remaining root cause is **`Globals::options`'s
  element type**. The original passes `Globals::options[N]` straight to
  `TouchButton::setAlwaysPressed(bool)` / `setHalfTransparent(bool)` with **no
  int->bool conversion**; our `unsigned char options[100]` forces a
  `cmp r1,#0; it ne; movne r1,#1` at six sites (36 bytes).
* The diff is not literal-pool order any more: the only pool-order difference
  left is `Globals::font` vs `Globals::Canvas` swapped in the language-loop
  pool, and that is a *symptom* of the scheduler, not an independent knob.
* Switching the global to `bool` (or casting through a `bool&` accessor)
  removes the six conversions, but shrinks the ctor to 9564 B and `.text` to
  `0x1337a8`; every function after the ctor then shifts by -0x20 and raw
  `byte_exact` drops 4498 -> 2672 (`linked_exact` stays 4498, i.e. the code is
  structurally unchanged). The original must therefore contain 36 B of other
  source-shape code that our body still lacks; the bool fix must be paired
  with it in one pass.

## 1. Evidence: the six missing conversions

`arm-linux-androideabi-objdump` over the 9600 B blobs:

| instruction | original | ours |
|---|---:|---:|
| `cmp`  | 44 | 48 |
| `it`   | 19 | 24 |
| `movne`|  1 |  7 |

The original's six `setAlwaysPressed`/`setHalfTransparent` call sites
(`0x0a8a` region and `0x1a..`/`0x1b..`) are all `ldrb r1,[options+N]; blx
TouchButton::setAlwaysPressed(bool)` directly. Ours inserts
`cmp r1,#0; it ne; movne r1,#1` before each. The six sites:

```
src/game/ui/MenuTouchWindow.cpp:611  setHalfTransparent(Globals::options[17])
:612  setAlwaysPressed(Globals::options[16])
:613  setAlwaysPressed(Globals::options[64])
:702  setAlwaysPressed(Globals::options[13])
:709  setAlwaysPressed(Globals::options[12])
:716  setAlwaysPressed(Globals::options[14])
```

`setHalfTransparent(!Globals::options[x])` sites are already conversion-free
(`!` yields bool -> `clz/lsr #5`), so they are not evidence either way.

The original's `_ZN7Globals7optionsE` is 100 B in `.bss`; `bool` and
`unsigned char` are both 1 B, so the layout is unchanged. `bool` is the only
type that makes the direct `ldrb` call legal.

Measured: changing `Globals.h/.cpp` to `bool options[100]` (plus the two
`unsigned char *options = Globals::options;` aliases in `ModStation.cpp` and
`MenuTouchWindow.cpp` to `bool *`) gives a 9564 B ctor with zero `movne`
conversions. `linked_exact` stays 4498; only raw `byte_exact` collapses
because `.text` shrinks 0x20.

## 2. The remaining 36 bytes

With the conversions removed, the original has 15 more instructions than ours
(3320 vs 3305). The net `orig - ours` per diff block is spread out; the
largest contributors are:

* **language-loop ternary, +8**: the original selects *values*
  (`ldr r5,Globals::font; ldr r8,Globals::fontLangSelect; it eq; moveq`),
  ours selects *addresses*
  (`it ne; ldrne r9,[&font/&fontLangSelect]; ldr r1,[r9]`). The source is
  `special ? *specialFontPtr : Globals::font` twice. Replacing the ternary
  with `Globals::fontLangSelect` (or hoisting a value local) changes the
  function-wide register allocation and drops the fuzzy to ~72-86%, so the
  `specialFontPtr` cache must stay; the value/pointer choice is a clang
  codegen consequence of the surrounding shape, not a local rewrite.
* **slider[2] y-expression, net 0 but re-split (orig 13 vs ours 13)**: the
  original's tree is
  `field_0x2fc + (top + left + listEntryWidth + 3*rowHeight) + sliderRowHeight
   + 2*(field_0x15c + 0x1c + rowHeight)`
  (note `3*rowHeight` and `(0x1c+rowHeight)` grouped together). Our current
  source is `(field_0x15c+0x1c)*2 + ... + rowHeight*5`. Rewriting the source
  with the original's grouping (tried: `top+left+listEntryWidth+3*rowHeight +
  field_0x2fc + sliderRowHeight + (field_0x15c+(0x1c+rowHeight))*2`) makes
  clang emit the same tree but a different evaluation order, and the fuzzy
  falls to 91.3-93.4%. Again the scheduler is coupled to the bool fix.
* **EH landing-pad dispatch, ~-2 net**: orig has 8 extra `b.n/mov r9,r0`
  pairs at 0x3137.. and 10 fewer at 0x3147..; 54 landing pads in both, so the
  destructor set matches. It is a dispatch-order difference, not missing
  temporaries (String ctor/dtor call counts are identical: 56 `StringD1`,
  27 `StringC1(RKS0_b)`, 15 `StringC1(PKcb)`).

## 3. Literal-pool order

Full pool-sequence comparison (resolver-aware, `pool:pic:*`) shows **exactly
one** difference across the whole function:

```
orig[25] Globals::font      ours[25] Globals::Canvas
orig[26] Globals::Canvas    ours[26] Globals::font
```

(the language-loop pool; `fontLangSelect` is third in both). The
`options, globals, GetImage2DWidth, iPadHD` pool is already byte-for-byte in
the same order. The swap is produced by clang scheduling the `font` load
before/after the `Canvas` load, so it is downstream of the ternary/RA shape.
No separate anchor can fix it.

## 4. `int id` shape

`TouchButton::id` is `long long` at offset 0 and `fontId` is `uint32_t` at
offset 8. The original's `quit->id = 20` really is `movs r0,#20;
strd r0,r4,[r5]` (8-byte store), and `language->id = languageIds[i]` really is
`ldr r32; mov r1,r0,asr #31; strd r0,r1,[r6]` (sign-extended int -> long
long). Both match ours; the field shape is correct.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3816
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindowC1Ei
cmake --build cmake-build-match --target check -j8   # RATCHET PASS 4498/4498
```

## 6. Suggested next pass

Land the `Globals::options` -> `bool` change **together with** the +36 B of
source-shape code (language-loop value-select shape + slider[2] tree +
landing-pad order) in one commit, then check that `.text` returns to
`0x1337c8`. Do not land the type change alone: it is a hard `byte_exact`
regression (4498 -> 2672) even though `linked_exact` is unaffected.
