# HangarWindow::render — wave 110 (w5813): free-credits / condition-order tuning

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w5813`, branch `wave110-w5813`, base master
`390f58052`.

## Result

| metric | before (HEAD 390f58052) | landed |
|---|---:|---:|
| `render` fuzzy | 88.518263 | **88.711234** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626/3629 | 3626/3629 |
| `render` LSDA record | 156 rows / 945 B | **156 rows / 945 B** |
| `.ARM.extab` | `0x26294` @ `0x1a3b78` | same |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.rodata` | `0x41b67` @ `0x1c9e10` | same |
| `.jcr` | `0xc0` | same |
| ELF section count | 28 | 28 |
| file size delta vs original | +44 | +44 |
| byte_exact / linked_exact | 4500 / 4500 | **4500 / 4500** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. The 156-row / 945-byte LSDA record
is kept (LSDA base still `0x1c30f4`); `.ARM.extab` remains 4 B short of the
original's `0x26298`.

## Landed diff (3 hunks, all inside `render`)

```diff
@@ -582,7 +582,7 @@ void HangarWindow::render() {
                                         volatile int pad3 = left;
                                         (*this->buttons)[8]->setPosition(
-                                            this->hintOffsetX + Globals::layout->buttonInsetX,
+                                            Globals::layout->buttonInsetX + this->hintOffsetX,
                                             this->field_0x114 + y, 17);
@@ -793,12 +793,12 @@ void HangarWindow::render() {
                     } else if (item->isMoveToCargoButton()) {
-                        (*this->buttons)[6]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX, y, 17);
+                        (*this->buttons)[6]->setPosition(Globals::layout->buttonInsetX + this->hintOffsetX, y, 17);
@@ -801,7 +801,7 @@ void HangarWindow::render() {
                     } else if (item->isSellButton()) {
-                        (*this->buttons)[5]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX, y, 17);
+                        (*this->buttons)[5]->setPosition(Globals::layout->buttonInsetX + this->hintOffsetX, y, 17);
@@ -827,9 +827,9 @@ void HangarWindow::render() {
-                            if (tab == 1 && this->upgradeMode) {
+                            if (this->upgradeMode && tab == 1) {
                                 (*this->buttons)[2]->setVisible(true);
-                                (*this->buttons)[10]->setPosition(this->hintOffsetX + Globals::layout->buttonInsetX,
+                                (*this->buttons)[10]->setPosition(Globals::layout->buttonInsetX + this->hintOffsetX,
                                                                   this->field_0x114 + y, 17);
@@ -852,7 +852,7 @@ void HangarWindow::render() {
                                         (*this->buttons)[b]->setPosition(
-                                            this->hintOffsetX + Globals::layout->buttonInsetX + listWidth -
+                                            Globals::layout->buttonInsetX + this->hintOffsetX + listWidth -
                                             2 * Globals::layout->field_0x2c_rowHeight,
@@ -865,7 +865,7 @@ void HangarWindow::render() {
                                     (*this->buttons)[7]->setPosition(
-                                        this->hintOffsetX + Globals::layout->buttonInsetX + listWidth,
+                                        Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
                                         this->field_0x114 + y, 18);
@@ -1041,7 +1041,7 @@ void HangarWindow::render() {
             TouchButton *button = (*this->buttons)[18 + i];
-            bool visible;
+            int visible;
```

1. **`if (this->upgradeMode && tab == 1)`** instead of `if (tab == 1 &&
   this->upgradeMode)`. Both operands are side-effect free; the swap changes
   which test is emitted first (`upgradeMode` is a byte load on `this`,
   `tab` is a local) and matches the original's branch order at the selected-
   ship price site.
2. **Six `Globals::layout->buttonInsetX + this->hintOffsetX` normalisations**
   (buttons 8, 6, 5, 10, `b`, 7). The original emits the global load first and
   the `this->hintOffsetX` load second; the previous source had the operands
   reversed. Integer `+` is commutative, so this is purely a source-order
   change that steers the `-Oz` scheduler.
3. **`int visible;`** instead of `bool visible;` in the free-credits loop.
   The original materialises the flag with `movs r6,#0` / `mov r6,r0` and
   tests it with `cmp r6,#0` (a full 32-bit value), not a byte; the `int`
   local reproduces that register class. `char` / `unsigned char` /
   `unsigned int` / explicit `= 0` / `visible != 0` / `visible > 0` are all
   neutral or worse (see table).

All existing codegen anchors are kept: `volatile int pad3 = left;`,
`volatile int *padX = 0;`, `volatile int pV = 0;`, `volatile int *pad0 = &x;`,
`volatile float textYf = ...`.

## Measured table

Baseline = HEAD `390f58052` (`88.518263`). Every row is 9700 B / 3626:3629
insns unless noted; all use the same gate-normalised fuzzy metric.

| variant | fuzzy |
|---|---:|
| HEAD `390f58052` | 88.518263 |
| + `upgradeMode && tab == 1` | 88.573398 |
| + all six `buttonInsetX + hintOffsetX` normalisations | 88.656099 |
| **+ `int visible` (LANDED)** | **88.711234** |
| `visible` as `char` / `unsigned char` / `unsigned int` / `= 0` / `!= 0` / `> 0` | 88.711234 |
| `visible` set via `= 0` / `= 1` | 88.711234 |
| `visible = !(i == 4 && options[76])` | 86.297215 (9696 B) |
| `visible = i == 4 ? !options[76] : true` | 88.545830 |
| `visible` if/else form | 88.656099 |
| `visible = i < 4 \|\| !options[76]` | 86.647375 (9704 B) |
| `visible = (i - 4) != 0 \|\| !options[76]` | 88.711234 |
| `if (!visible) continue;` / `if (visible == 0) continue;` | 88.711234 |
| item loop `i` as `int` (was `unsigned int`) | 88.711234 |
| `float rate` -> `const float` | 88.711234 |
| `float rate` -> `double` | 84.494630 (9720 B) |
| hoist `rate` out of the `if` | 85.158621 (9692 B) |
| tab-button loop `i` as `int` | 88.683666 |
| `baseX` operand swap | 88.683666 |
| `baseY` operand swap | 88.711234 |
| `baseY` multiply-first | 88.490696 |
| `baseX`/`baseY` declaration order swap | 88.711234 |
| `textX` = width+rowHeight+baseX | 88.711234 |
| `textX` = baseX+rowHeight+width / rowHeight+baseX+width / rowHeight+width+baseX | 88.699008 |
| `textX` = baseX+width+rowHeight | 88.711234 |
| `textX` computed before `height` | 77.136958 (3639 insns) |
| `textX` first + non-volatile `textYf` | 75.144509 (9704 B) |
| non-volatile `textYf` | 75.296062 (9696 B) |
| `const float textYf` / direct `int textY` | 75.296062 |
| `volatile int textY` | 86.344219 (9696 B) |
| `volatile double textYf` | 71.544715 (9708 B) |
| `volatile int height` | 88.683666 |
| `volatile int height` + non-volatile `textYf` | 75.416609 (9692 B) |
| `volatile float textYf` + plain copy to `float` | 88.711234 |
| `volatile float textYf` + `int textY = (int)(float)textYf` | 88.711234 |
| `float fy/fh` locals feeding the volatile | 88.711234 |
| `const int textY` | 88.711234 |
| `0.2f * height + y` / `y + height * 0.2f` | 88.711234 |
| `i != 0 ? i + 112 : 3400` -> `i == 0 ? 3400 : i + 112` | 87.467255 (9692 B) |
| `String("(+") + ... + String(")")` fully parenthesised | 88.711234 |
| right-associative `String("(+") + (formatCredits + String(")"))` | 85.414943 (9696 B) |

## What is proven

1. **The three landed changes are strictly monotone**: 88.518263 ->
   88.573398 -> 88.656099 -> 88.711234, all at 9700 B / 3626:3629 insns.
2. **`int visible` is worth +0.055** over `bool visible`; the flag is a
   32-bit register value in the original (`cmp r6,#0`, not `lsls`/`cbz` on a
   byte). Other integer widths and comparison spellings are neutral.
3. **The `buttonInsetX` operand order is worth +0.083** across the six sites;
   the original loads `Globals::layout` before `this->hintOffsetX`.
4. **The `upgradeMode`/`tab` test order is worth +0.055**.
5. **The full gate stays green**: `check` RATCHET PASS, byte 4500 /
   linked 4500 / size 4524 / data 843 / imports 207, lint CLEAN, drift 0.
   No section size/base/count changed; `.ARM.extab` stays `0x26294` and the
   `render` LSDA record stays 945 B / 156 rows at base `0x1c30f4`.

## Hypothesis (not proven)

The remaining ~11.3 % gap is still register allocation and block placement.
The single largest cluster is the free-credits loop
(`0x14ac3a`-`0x14ae8e`, ~100 of the ~150 differing instructions):

- The original computes `textX` **before** the `getText` call
  (`0x14ad78`/`0x14ad7e`) and keeps `width` in `sl`; ours spills `width` to
  `[sp,#124]` and recomputes `textX` after `getText` (`0x14ada0`-`0x14ada4`).
  Declaring `textX` before `height` (the original's apparent source order)
  costs ~12 fuzzy points because the whole loop's allocation then changes.
- The original keeps `textY` in `r6` and converts `s2` once
  (`0x14ad70`); ours spills `textYf` through `[sp,#204]` and re-issues
  `vcvt.s32.f32` for the second/third `DrawString`. Removing the `volatile`
  lets clang sink the whole float computation past `getText` (75.3).
- The original keeps `&Globals::Canvas` in `r6` and `width` in `sl`; ours
  swaps them (`sl` = `&Globals::Canvas`, width spilled). This is a global
  allocation choice, not controllable by a single local.

Smaller residual clusters: the blueprint progress-bar block
(`0x1496da`-`0x1498xx`, `sl`/`r9` `&Globals::layout`/`this` swap), the
selected-ship price block (`0x149ada`-`0x149ba2`), and the buy-credits
list-mode block (`0x14a4f4`-`0x14a862`, `r8`/`sl` String-temp swap). The
`item` pointer is still reloaded from `sp+0x78` where the original keeps it
in `r4` across `getStationAmount`/`getAmount`.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint  -j6      # CLEAN

# per-function fuzzy score (same normalisation as the gate)
python3 /dev/shm/w5813/score.py _ZN12HangarWindow6renderEv
# section sizes
arm-linux-androideabi-objdump -h cmake-build-match/libgof2hdaa.so
```

Scratch helpers (`score.py`, `measure.sh`, `blocks.py`, `diffsrc4.py`,
`objlines.py`, `batch100`-`batch112` variants) live under `/dev/shm/w5813/`;
they are not part of the commit.
