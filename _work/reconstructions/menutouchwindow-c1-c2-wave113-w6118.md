# MenuTouchWindow C1/C2 wave 113 (w6118): narrow `row2` local + `i != 10` loop
# (96.1602% -> 97.2143%, 9600 B)

Date: 2026-11-05. Worktree `wt/w6118`, branch `wave113-w6118`, base
`a979ace18` (wave-112 report sync). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000). Target: `_ZN15MenuTouchWindowC1Ei` /
`_ZN15MenuTouchWindowC2Ei` (weak alias, same address/size, same score).

## TL;DR

* **Before:** 9600 B, 3320 orig / 3321 ours instructions, exact difflib
  ratio **96.160217%**, **101** diff hunks.
* **After:** 9600 B, 3320 / **3321** instructions, exact difflib ratio
  **97.214275%**, **67** diff hunks. C2 scores identically.
* Two-line source change in `src/game/ui/MenuTouchWindow.cpp`:

```diff
         int mission = Globals::status->getCurrentCampaignMission();
         if (mission == 154 || mission == 157 || mission == 158) {
-            this->addButton(18, *Globals::gameText->getText(395), row + 2, this->buttons, 0);
+            char row2 = row + 2;
+            this->addButton(18, *Globals::gameText->getText(395), row2, this->buttons, 0);
         }
@@
-        for (unsigned int i = 0; (i & 0xf) != 10; i++) {
+        for (unsigned int i = 0; i != 10; i++) {
```

* Gate before and after: `byte_exact 4500 / linked_exact 4500`,
  `size 4524/4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
  **RATCHET PASS**. Fuzzy-only change, so no baseline update.
  No section/extab change: 27 sections, `.so` exactly 2,187,328 B,
  `.text` 0x1337c8, `.ARM.extab` 0x26298 (size- and base-exact).

## 1. Mechanism (measured)

The wave-112 tree used the wave-109 loop mask `(i & 0xf) != 10`, which
clang lowers to the 3-instruction backedge

```
1234a6: and.w r0, r8, #15
1234aa: cmp   r0, #10
1234ac: bne.w 123342
```

while the original has the 2-instruction range backedge

```
1234a6: cmp.w r8, #10
1234aa: bcc.w 123346
```

Writing the plain `i != 10` removes the `and` but, on its own, is a
*regression*: 9596 B / 3319 instructions / 93.206808% / 163 hunks. The
range fact lets clang fold the two value selects in the loop body into
an address select, move the loop counter to `fp` and re-colour the whole
frame.

Adding a **narrow unsigned local** for the mission row (`char row2 =
row + 2;`, also `unsigned char` / `uint8_t` / `unsigned short` /
`char16_t`) re-introduces one `uxtb` in the mission block

```
U 366 adds  r0, r5, #2
U 369 uxtb  r3, r0
```

which perturbs register allocation just enough to restore the original
9600-byte frame and most of the wave-112 match: **97.214275%**, 67
hunks. The `uxtb` is the only instruction surplus (3321 vs 3320).

Why a narrow type is load-bearing: `int` / `short` / `signed char` /
`long` / `long long` / `double` all give 9596 B / 3319 / 93.206808%;
`float` gives 91.735662%. Only the *unsigned narrow* forms (which need
`uxtb` at the `int` parameter) reproduce the frame.

## 2. Measured table (official verifier, full TU)

`orig` is always 9600 B / 3320 instructions. `fz` is the exact difflib
ratio from `tools/verify/verify.py`'s normalised stream. All variants
are on top of the wave-112 tree unless noted.

| variant | size | insns | fuzzy | hunks |
|---|---:|---:|---:|---:|
| **wave-112 baseline (`(i & 0xf) != 10`)** | **9600** | **3321** | **96.160217** | **101** |
| **`i != 10` + `char row2` (this pass)** | **9600** | **3321** | **97.214275** | **67** |
| `i != 10` + `int row2` | 9596 | 3319 | 93.206808 | 163 |
| `i != 10` + plain `row + 2` | 9596 | 3319 | 93.206808 | 163 |
| `i != 10` + `signed char` / `short` / `int8_t` / `wchar_t` row2 | 9596 | 3319 | 93.206808 | 163 |
| `i != 10` + `unsigned char` / `uint8_t` / `unsigned short` / `char16_t` row2 | 9600 | 3321 | 97.214275 | 67 |
| `i != 10` + `float` row2 | 9600 | 3321 | 91.735662 | — |
| `i != 10` + `(char)(row + 2)` inline | 9600 | 3321 | 97.214275 | 67 |
| `i != 10` + `(char)row + 2` inline | 9600 | 3320 | 96.957831 | 77 |
| `(i & 0xf) != 10` + `int row2` | 9600 | 3321 | 96.160217 | 101 |
| `(i & 0xf) != 10` + `char row2` | 9604 | 3323 | 93.361433 | 155 |

Other wave-113 dead ends (all measured against the `i != 10` + `char
row2` best; every one is <= 97.214275):

| variant | fuzzy | hunks |
|---|---:|---:|
| iPad y regroupings (temp, `-=`, sum-first, gap-sum, height-first, parens) | 97.214275 | 67 |
| `char row2` declared before `int mission` / `row2 += 2` / `(char)(row+2)` local | 97.214275 | 67 |
| mission-condition permutations (all 6 orders) | 97.214275 | 67 |
| `special = i == 8 \|\| i == 9` | 97.214275 | 67 |
| `special = i >= 8` / `i > 7` | 88.664760 | 170 |
| `__builtin_assume(i < N)` for N in 11..10000 | 97.214275 | 67 |
| no `__builtin_assume` / assume outside `if (special)` | 72.680800 | 489 |
| `fontValue`/`ctorFont`/`langValue` as `int` | 97.214275 | 67 |
| swap `fontValue`/`langValue` declaration order | 97.214275 | 67 |
| drop `specialFontPtr`, use `Globals::fontLangSelect` directly | 97.214275 | 67 |
| `bw` moved after the `if (special)` | 97.214275 | 67 |
| `ctorFont = special ? langValue : fontValue` (no `if`) | 72.680800 | 489 |
| `spacing` after `bw` | 76.462626 | 427 |
| cinematic: `GameText *const cinematicText = Globals::gameText;` | 97.184159 | 67 |
| cinematic: local used for all three `getText` sites | 93.014152 | 155 |
| cinematic: second call direct (`Globals::gameText`) | 95.058753 | 108 |
| cinematic: first call direct | 93.610609 | 95 |
| `Globals::w >> 1` -> `Globals::w / 2` in the iPad branch | 93.574116 | 137 |
| slider[0] `rowHeight*3` first (Ghidra tree) | 96.777108 | 74 |
| slider[1] `0x2fc` first (Ghidra tree) | 96.506024 | 79 |
| slider[2] exact Ghidra tree | 96.852884 | 76 |
| slider[2] `0x2c*2` split out of the doubled term | 97.093811 | 68 |
| slider[3] `((0x15c + 0x1c) + sliderRowHeight) * 2` | 96.852884 | 74 |
| sliders 0+1+2+3 all Ghidra trees | 92.666767 | 155 |
| `scrollExtraButton` exact Ghidra tree | 96.807229 | 71 |
| `scrollExtraButton` `0x2c*5` first | 97.033579 | 68 |
| `unsigned char` on `mainButtonOffset` / `next` / `storeX` / `langValue` | 97.093-97.184 | 68-69 |
| `unsigned char` on `mission` / `windowHeight` / `listX` / `yBase` / `rowPitch` | 92.7-93.8 | 139-170 |
| `unsigned char` on `y` / `x` / `spacing` / `bw` / `rowHeight` / `storeY` | 66.0-93.1 | 162-610 |
| `unsigned char row` in case 0 / case 2 | 97.214275 | 67 |
| `unsigned char row` in case 1 | 82.319363 | 420 |

## 3. Remaining residual (67 hunks)

Of the 172 replaced instruction slots, **57 keep the same opcode** (26
of those differ only in register name) and **57 change opcode**; the
rest are 1-3 instruction inserts/deletes. The clusters:

* **Language loop** (3 hunks). The backedge is now the correct
  `cmp.w r8,#10; bne.w` (original `bcc.w`; same size, flags differ
  only). iPad `y` subtraction order: original
  `subs r1,r6,r2; subs r1,r1,r3` (`yBase - gap - height`), ours
  `subs r1,r6,r3; subs r1,r1,r2`. Every source spelling of that
  expression (temp, `-=`, reordered, parenthesised) produces identical
  IR — LLVM's reassociate pass canonicalises it, so it is not
  source-controllable here.
* **Mission block** (3 hunks). Original hoists `adds r4,r5,#2` *before*
  `blx getText` and passes it with `mov r3,r4`; ours sinks it after the
  `strd` and needs `uxtb r3,r0`. Same byte count, different schedule.
  `-mllvm -pre-RA-sched=source` reproduces the hoist but drops the whole
  TU to 47.5% (frame 0x174 -> 0x184), so it is not usable.
* **Cinematic-button block** (~20 hunks). Ours keeps `Globals::gameText`
  live in `r9`/`sl` across the first `TouchButton` ctor and inserts one
  `mov sl,r4`; the original reloads `[sp,#96]` for each `getText` and
  uses `r5`/`r6` the other way round. Every spelling that forces the
  reload also shrinks the frame to 9588-9592 B and scores 93.6-95.1%.
* **Slider `setPosition` y trees** (~12 hunks). The original groups the
  sums as (decoded from the Thumb, matches Ghidra):
  - slider[0]: `((0x2c*3 + (((0x20 + 0xc) + 0x1c) + 0x154)) + 0x15c*2)`
  - slider[1]: `((0x2fc + (((0x20 + 0xc) + 0x154) + 0x2c*4)) + ((0x15c + 0x1c)*2))`
  - slider[2]: `((((0x2fc + (((0x20 + 0xc) + 0x154) + 0x2c*3)) + 0x164) + (((0x15c + 0x1c) + 0x2c)*2))`
  - slider[3]: `((0x2fc + (((0x20 + 0xc) + 0x154) + 0x2c*6)) + (((0x15c + 0x1c) + 0x164)*2))`
  Every faithful tree scores 96.5-96.9; the current left-leaning
  groupings are better. Only the doubled-term split
  (`0x2c*2` pulled out of `(0x15c + 0x1c + 0x2c)*2`) is close
  (97.093811).
* **Literal-pool displacements** (`ldr [pc,#N]` at indices ~1269, 1342,
  1379, 1382, 1781, 1815, 1856, 1960, 2441, 2467, 2479, 2621). These
  are consequences of the pool placement and will follow once the
  opcode/RA hunks above are fixed.
* **Exception cleanup landing pads** (last hunk, indices 3263-3265).
  The two `b.n` at the tail target `loc_2566` in the original and
  `loc_2560` in ours; the delete-path register is `r5` vs `r6`
  (post-RA colouring of the `new TouchButton` result).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6118
TMPDIR=/dev/shm/w6118/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w6118/tmp cmake --build cmake-build-match --target check -j6
TMPDIR=/dev/shm/w6118/tmp cmake --build cmake-build-match --target drift -j6
```

Single-function score (C1 and C2 are identical):

```sh
python3 tools/verify/verify.py --show _ZN15MenuTouchWindowC1Ei
```

Ghidra cross-check used for the slider trees:

```sh
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure _ZN15MenuTouchWindowC1Ei --format json
```
