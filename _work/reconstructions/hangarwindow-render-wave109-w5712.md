# HangarWindow::render — wave 109 (w5712): sink value/placement tuning

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w5712`, branch `wave109-w5712`, base master
`7c34c4098`.

## Result

| metric | before (HEAD 7c34c4098) | landed |
|---|---:|---:|
| `render` fuzzy | 88.254756 | **88.518263** |
| `render` raw difflib | 88.254756 | **88.518263** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626/3628 | 3626/3629 |
| `render` LSDA record | 945 B / 156 rows | **945 B / 156 rows** |
| `.ARM.extab` | `0x26294` @ `0x1a3b78` | `0x26294` @ `0x1a3b78` |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.rodata` | `0x41b67` @ `0x1c9e10` | same |
| ELF section count | 28 | 28 |
| file size delta vs original | +44 | +44 |
| byte_exact / linked_exact | 4500 / 4500 | **4500 / 4500** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. The 156-row / 945-byte record is
kept, and the LSDA base is still `0x1c30f4`.

## Landed diff (3 hunks, all inside `render`)

```diff
@@ -578,8 +578,9 @@ void HangarWindow::render() {
                                     if (left > 0) {
                                         // Codegen anchor: the dead store keeps the
                                         // list-body register roles (this -> r6) and
-                                        // the 9700-byte body of the original.
-                                        volatile int pad3 = y;
+                                        // the 9700-byte body of the original. Storing
+                                        // `left` (already in r0) avoids a reload of y.
+                                        volatile int pad3 = left;
                                         (*this->buttons)[8]->setPosition(
@@ -952,11 +953,11 @@ void HangarWindow::render() {
                 (*this->buttons)[12 + count]->setPosition(x, y, 68);
+                // Codegen anchor: taking x's address pins the buy-credits
+                // register allocation, and the dead pointer keeps the original's
+                // separate cleanup region (156-row .ARM.extab) at 9700 bytes.
+                volatile int *pad0 = &x;
                 (*this->buttons)[12 + count]->replaceTextKeepSize(description);
-                volatile int pad0 = count;
-                // The original's LSDA has a separate cleanup region covering
-                // this call; the volatile sink keeps that region (and hence
-                // .ARM.extab) at the original size without changing behaviour.
                 (*this->buttons)[12 + count]->setSplitText(price);
@@ -1054,8 +1055,8 @@ void HangarWindow::render() {
             if (visible) {
                 int height = button->getHeight();
                 int textX = button->getWidth() + baseX + Globals::layout->field_0x2c_rowHeight;
-                volatile float textYf = (float) y + (float) height * 0.2f;
                 Globals::Canvas->SetColor(0xffffffffu);
+                volatile float textYf = (float) y + (float) height * 0.2f;
                 int textY = (int) textYf;
```

The third hunk (SetColor before textYf) is the wave-108/109 predecessor change
already present in the handoff baseline; it is included here because the two
sink tweaks were measured on top of it. All three are plain C++ (no asm,
attributes, linker tricks, namespace-scope globals or data anchors).

## What the two sink tweaks actually do

1. `pad3 = left` instead of `pad3 = y`. At the `if (left > 0)` site the
   `getAmount()` / `getCurrentAmount()` result is already in `r0`; storing
   `left` costs no reload, whereas `y` forced a `ldr r4,[sp,#116]` before the
   store. The whole `if (left > 0)` body then matches the original
   instruction-for-instruction (`0x149cac`–`0x149cc6`), with only the single
   `str r0,[sp,#300]` sink store left over.

2. `pad0` moved from *after* `replaceTextKeepSize` to *between*
   `setPosition` and `replaceTextKeepSize`, and retyped from a scalar
   (`volatile int pad0 = count;`) to a dead pointer (`volatile int *pad0 = &x;`).
   The pointer initialiser is dead (the pointer itself is not volatile, so the
   store is dropped), but taking `x`'s address forces `x` to be
   address-taken/spilled, which is what re-pins the buy-credits list-mode
   register allocation. Any in-scope local gives the same effect (see table).

## Measured table (all 9700 B, 156 rows, extab base `0x1c30f4` unless noted)

Baseline = HEAD `7c34c4098` (`88.254756`). All variants below build on the
wave-108/109 `SetColor`-before-`textYf` change (`88.365040`).

| variant | fuzzy | size | rows |
|---|---:|---:|---:|
| HEAD `7c34c4098` | 88.254756 | 9700 | 156 |
| + `SetColor` before `textYf` (predecessor) | 88.365040 | 9700 | 156 |
| + `pad3 = left` | 88.392611 | 9700 | 156 |
| + `pad3 = left`, `pad0 = x` scalar after `replaceTextKeepSize` | 88.407994 | 9700 | 156 |
| + `pad3 = left`, `pad0 = x` scalar before `replaceTextKeepSize` | 88.490696 | 9700 | 156 |
| + `pad3 = left`, `pad0 = y` scalar before `replaceTextKeepSize` | 88.490696 | 9700 | 156 |
| **+ `pad3 = left`, `pad0 = &x` pointer before `replaceTextKeepSize` (LANDED)** | **88.518263** | **9700** | **156** |
| `pad0 = &y` / `&count` / `&col` / `&row` / `&description` pointer | 88.518263 | 9700 | 156 |
| `int *pad0 = &x` (non-volatile pointer) | 88.518263 | 9700 | 156 |
| `pad0 = &x` before `setPosition` | 86.612436 | 9696 | 155 |
| `pad0 = &x` after `replaceTextKeepSize` | 86.143665 | 9696 | 156 |
| `pad0 = count` before `replaceTextKeepSize` | 86.521499 | 9704 | 156 |
| `pad0 = col` before `replaceTextKeepSize` | 88.242591 | 9700 | 156 |
| `pad0 = row` before `replaceTextKeepSize` | 84.164142 | 9720 | 156 |
| `pad3 = right` | 85.584344 | 9704 | 156 |
| `pad3 = y` (no pad3 tweak) | 88.365040 | 9700 | 156 |
| `pad3` removed | 86.143665 | 9696 | 154 |
| `pad3 = y`, `pad0 = &x` before `replaceTextKeepSize` | 88.463129 | 9700 | 156 |
| `pad0` as `volatile int *` (volatile pointer) | 85.170893 | 9704 | 156 |
| `pad0` as `volatile int` holding `(int)&x` | 85.170893 | 9704 | 156 |
| `pad0 = &x` + `padX = &y` | 88.518263 | 9700 | 156 |
| `pad0 = &x` + `textYf` const `1.0f/5.0f` | 88.518263 | 9700 | 156 |
| `pad0 = &x` + `int left, right;` | 88.518263 | 9700 | 156 |
| `pad0 = &x` + `int *pin = &right;` in item block | 88.518263 | 9700 | 156 |

Free-credits block permutations re-tested on the landed base (all worse than
or equal to the predecessor 88.365040): `height,SetColor,textX,textYf`
88.270159; `height,SetColor,textYf,textX` 88.297726;
`height,textYf,textX,SetColor` 88.282327; `height,textYf,SetColor,textX`
88.215024; `textX,height,SetColor,textYf` 76.927313; `int textY = (int)((float)y
+ (float)height*0.2f)` 75.320028 (9704 B); `volatile int textY` 86.214502
(9688 B); `0.2` double constant 86.029209 (9716 B).

## What is proven

1. **`pad3 = left` is strictly better than `pad3 = y`** at the same 9700 B /
   156 rows: 88.392611 vs 88.365040. The `if (left > 0)` body then matches the
   original exactly except for the single sink store.
2. **Moving `pad0` between `setPosition` and `replaceTextKeepSize` is worth
   +0.10 over keeping it after `replaceTextKeepSize`**, and retyping it from
   scalar to pointer is worth another +0.03 (88.518263 vs 88.490696).
3. **The pointer's pointee is irrelevant** (`&x`, `&y`, `&count`, `&col`,
   `&row`, `&description` all give 88.518263); only the address-taking matters.
   The `volatile` on the pointee is also irrelevant (`int *pad0 = &x` gives the
   same score) because the pointer initialiser is a dead store.
4. **The full gate stays green**: `check` RATCHET PASS, byte 4500 /
   linked 4500 / size 4524 / data 843 / imports 207, lint CLEAN, drift 0.
   No section size/base/count changed; `.ARM.extab` stays `0x26294` and the
   `render` LSDA record stays 945 B / 156 rows at base `0x1c30f4`.

## Hypothesis (not proven)

The remaining ~11.5 % gap is still register allocation and block placement.
The largest remaining clusters are the free-credits block
(`0x14ac00`–`0x14ae00`, dominated by the `volatile float textYf` spill and the
`getHeight`-first call order), the buy-credits list-mode block
(`0x14a900`–`0x14ab00`, a systematic `r8`/`sl` String-temp swap), and the
bluePrint progress-bar block (`0x1496da`–`0x1498xx`, the `sl`/`r9`
`&Globals::layout`/`this` swap). The `item` pointer is reloaded from `sp+0x78`
in ours where the original keeps it in `r4` across the
`getStationAmount`/`getAmount` calls; no plain source shape tried so far
reproduces that. The `getWidth`-first free-credits order (the original's
actual call order) still costs ~12 fuzzy points (76.9), so the
`getHeight`-first shape is kept.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint  -j6      # CLEAN

# per-function fuzzy score (same normalisation as the gate)
python3 /dev/shm/w5712/score.py _ZN12HangarWindow6renderEv
# LSDA record size / rows
python3 /dev/shm/w5712/exrec.py cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv

# section sizes
arm-linux-androideabi-objdump -h cmake-build-match/libgof2hdaa.so
```

Scratch helpers (`score.py`, `exrec.py`, `measure.sh`, `diffaddr.py`,
`batch2.py`, variant JSONs) live under `/dev/shm/w5712/`; they are not part of
the commit.
