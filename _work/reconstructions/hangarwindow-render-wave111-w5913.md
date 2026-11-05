# HangarWindow::render — wave 111 (w5913): DrawRegion2D operand order + `pV` placement

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w5913`, branch `wave111-w5913`, base master
`8eea060cc`.

Builds on `_work/reconstructions/hangarwindow-render-wave110-w5813.md`
(88.518263 -> 88.711234). This pass: **88.711234 -> 89.057332**.

## Result

| metric | before (HEAD 8eea060cc) | landed |
|---|---:|---:|
| `render` fuzzy | 88.711234 | **89.057332** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626/3629 | 3626/3630 |
| `render` LSDA record | 156 rows / 948 B | 156 rows / 948 B |
| `.ARM.extab` | `0x26294` @ `0x1a3b78` | same |
| `.text` | `0x1337c8` @ `0x69440` | same |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4500 / 4500 | **4500 / 4500** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. `.ARM.extab` remains 4 B short of
the original's `0x26298` (known wave-110 state, not regressed).

## Landed diff (2 hunks, both inside `render`)

```diff
@@ -750,7 +750,7 @@ void HangarWindow::render() {
                             Globals::Canvas->DrawRegion2D(
                                 this->progressBarFillImage, 0, 0, (int) (rate * (float) this->progressBarWidth),
                                 this->progressBarHeight, 0.0f, 0, 0,
-                                textX + 3 + Globals::layout->buttonInsetX + this->hintOffsetX,
+                                this->hintOffsetX + textX + 3 + Globals::layout->buttonInsetX,
                                 y + 2 + Globals::layout->field_0x70_rowHeight / 2);
@@ -898,10 +898,10 @@ void HangarWindow::render() {
     if (this->viewMode == 1) {
         this->viewMode = 0;
+        volatile int pV = 0;
         this->render();
         this->viewMode = 1;
         this->listItemWindow->draw();
-        volatile int pV = 0;
     }
```

1. **Blueprint progress-bar `DrawRegion2D` posX operand order.** The original
   emits the `this->hintOffsetX` load first (`ldr.w r6,[r9,#244]`), then the
   `textX + 3` addend, then `Globals::layout->buttonInsetX`
   (`ldr r2,[r0,#40]`; `add r2,r4`; `add r2,r6`). Integer `+` is
   commutative, so this is purely a source-order change that steers the
   `-Oz` scheduler. Worth +0.291 on its own (88.711234 -> 89.002205).
2. **`volatile int pV = 0;` moved between `this->viewMode = 0;` and
   `this->render();`** (was after `listItemWindow->draw()`). The dead store
   is a codegen anchor; moving it one statement earlier re-pins the
   recursive-call register allocation. Worth +0.055 (89.002205 ->
   89.057332). Placing it before `viewMode = 0` is neutral; after `render()`
   or after `viewMode = 1` is worse (88.975 / 88.920); giving it a live use
   grows the body to 9704 B (86.77 / 86.83).

All existing codegen anchors are kept: `volatile int pad3 = left;`,
`volatile int *padX = 0;`, `volatile int pV = 0;`, `volatile int *pad0 = &x;`,
`volatile float textYf = ...`, and `int b = item->inTabIndex == -1 ? 4 : 3;`.

## Measured table

Baseline = HEAD `8eea060cc` (`88.711234`). Every row is 9700 B / 3626:3629
insns unless noted; same gate-normalised fuzzy metric.

| variant | fuzzy |
|---|---:|
| HEAD `8eea060cc` | 88.711234 |
| + `hintOffsetX + textX + 3 + buttonInsetX` (LANDED) | 89.002205 |
| **+ `pV` before `render()` (LANDED)** | **89.057332** |
| `pV` before `viewMode = 0` | 89.057332 |
| `pV` after `render()` | 88.974642 |
| `pV` after `viewMode = 1` | 88.919515 |
| `pV` after `draw()` (HEAD position) | 88.711234 |
| `pV` with a live `pV = 1` after `draw()` | 86.773216 (9704 B) |
| `pV` with `(void) pV` after `draw()` | 86.828327 (9704 B) |
| `textX + 2 + buttonInsetX + hintOffsetX` -> `hintOffsetX + textX + 2 + ...` | 87.915116 |
| other six `textX + buttonInsetX + hintOffsetX` sites reordered | 88.947078 |
| final `DrawString` `textX + buttonInsetX + hintOffsetX` reordered | 89.029768 |
| `buttonInsetX + (listWidth - buttonOffset) + hintOffsetX` | 89.057332 |
| `hintOffsetX + buttonInsetX + (listWidth - buttonOffset)` | 89.002205 |
| `int b = item->inTabIndex < 0 ? 4 : 3` | 87.329381 (9696 B) |
| `textX` computed before `height` (original order) | 77.153867 (3640 insns) |
| `textX` split into `width` local, before `height` | 75.302863 (9692 B) |
| `int width`/`int height`/`int textX` (height first) | 89.057332 |
| `textX` = `width + baseX` then `+= rowHeight` | 89.057332 |
| non-volatile `textYf` (direct `int textY`) | 75.313231 (9696 B) |
| `volatile float textYf` + plain `float tf` copy | 89.057332 |
| `volatile float textYf` + `(int)(float)textYf` | 89.057332 |
| `float textYf` + `volatile float tf` | 86.753963 (9696 B) |
| `0.2f * height + y` operand order | 89.057332 |
| `int visible`->`volatile int buttonOffset` | 73.020245 (9704 B) |
| `int buttonOffset = 0` / `= -1` / `unsigned` / `long` / `register` | 89.057332 |
| `buttonOffset` declared at loop top / before loop | 89.057332 |
| `String name;`/`int buttonOffset;` swapped | 89.057332 |
| `int left/right` if-else instead of ternary | 81.010910 (9656 B) |
| `right` if/else, `left` ternary | 89.057332 |
| `hangarFont()` -> `Globals::font` / `int` return | 89.057332 |
| `pad3 = right` | 86.151302 (9704 B) |
| `pad3 = y` | 89.029768 |
| `pad3 = 0` | 86.442546 (9704 B) |
| remove `pad3` | 86.710780 (9696 B, extab 0x26288) |
| remove `padX` | 85.911032 (extab 0x26288) |
| `pad0` moved before `setPosition` | 87.151916 (9696 B, extab 0x2628c) |
| `pad0` + `(void) pad0` | 89.057332 |
| `String name;`/`int buttonOffset;` before `SetColor` | 61.116471 (9664 B) |

## What is proven

1. **The two landed changes are strictly monotone**: 88.711234 ->
   89.002205 -> 89.057332, both at 9700 B / 3626:3630 insns.
2. **The `DrawRegion2D` posX operand order is worth +0.291.** The original
   loads `this->hintOffsetX` before the `Globals::layout` fields; the
   previous source had it last.
3. **`pV` is a genuine codegen anchor for the recursive `render()` call.**
   Its dead store must sit between `viewMode = 0` and `render()`; any live
   use grows the body by 4 B and costs ~2.3 fuzzy points.
4. **The free-credits loop still wants the original's source order but the
   compiler will not produce it.** Declaring `textX` before `height` (the
   original's apparent order) costs ~12 fuzzy points because the whole
   loop's register allocation changes; the tuned order keeps `height` in
   `r6` and spills `width` to `[sp,#124]`.
5. **The full gate stays green**: `check` RATCHET PASS, byte 4500 /
   linked 4500 / size 4524 / data 843 / imports 207, lint CLEAN, drift 0.
   No section size/base/count changed; `.ARM.extab` stays `0x26294`.

## Remaining diff clusters (for future waves)

The remaining ~11 % is register allocation, stack-slot colouring and block
placement. 297 diff blocks: 68 are pure `loc_*` branch-target offsets, 229
are real (403 instructions). The largest:

- **Free-credits loop (`0x14ac7a`-`0x14ae7c`).** The original keeps
  `width` in `sl`, `textY` in `r6`, computes `textX` before `getText`, and
  has no `textYf` spill. Ours spills `width` to `[sp,#124]`, spills
  `textYf` through `[sp,#204]`, and re-issues `vcvt.s32.f32`. The
  `else` (`i == 0`, `String("")`) block is placed after the literal pool in
  ours but before it in the original (the 27-vs-1 block at normalised
  instruction 2817).
- **`buttonOffset` stack slot.** Original `[sp,#80]`, ours `[sp,#308]`.
  This single slot is why every `[sp,#N]` in the list body differs by 4
  (`this` 136 vs 132, `&layout` 132 vs 128, `tab` 108 vs 104, `y` 116 vs
  112, `listWidth` 128 vs 124, String temporaries 144+12k vs 140+12k).
  Declaration position, initialiser, type (`unsigned`/`long`), and
  loop-scope vs function-scope are all neutral; the allocator will not
  colour it low.
- **Blueprint progress block (`0x1496da`-`0x1498xx`).** `sl`/`r9`
  `&Globals::layout`/`this` role swap (ours `r9` = layout, `sl` = this;
  original the reverse). Same for the selected-ship price block
  (`0x149ada`-`0x149ba2`, ours `r5` = this / `r6` = layout, original
  `r4` = this / `r5` = layout).
- **Buy-credits list-mode block.** `r8`/`sl` String-temp swap and the
  `item` pointer reloaded from `sp+0x78` where the original keeps it in
  `r4` across `getStationAmount`/`getAmount`.
- **`.ARM.extab` is 4 B short** (`0x26294` vs `0x26298`). The first
  differing entry is at extab offset `0x1506`; one cleanup region is
  missing. The `padX` LSDA sink is required (removing it drops the extab
  to `0x26288` and the score to 85.91).

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS

# per-function fuzzy score (same normalisation as the gate)
python3 /dev/shm/w5913/score.py _ZN12HangarWindow6renderEv
```

Scratch helpers (`score.py`, `measure.sh`, `blocks.py`, `try.py`,
`try2.py`, `batchA`-`batchS` variants) live under `/dev/shm/w5913/`; they
are not part of the commit.
