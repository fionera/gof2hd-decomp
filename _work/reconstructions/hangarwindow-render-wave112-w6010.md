# HangarWindow::render — wave 112 (w6010): register-role swap is the frontier

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6010`, branch `wave112-w6010`, base master
`5d98bd391`.

Builds on `_work/reconstructions/hangarwindow-render-wave111-w5913.md`
(88.711234 -> 89.057332). This pass: **no landable improvement**; the note
records the measurements and the two structural findings that explain why
the remaining ~11 % resists source-level tuning.

## Result

| metric | HEAD 5d98bd391 (landed) |
|---|---:|
| `render` fuzzy (raw difflib) | **89.057332** |
| `render` fuzzy (gate, rounded) | 89.1 |
| `render` size | 9700 |
| normalised insns | 3626 / 3630 |
| `.ARM.extab` | `0x26298` @ `0x1a3b78` — **already exact** |
| `.ARM.exidx` | `0x6f70` @ `0x19cc08` — exact |
| `.text` | `0x1337c8` @ `0x69440` — exact size/base |
| ELF section count | 27 — exact |
| file size | 2,187,328 — exact |
| byte_exact / linked_exact | 4500 / 4500 |
| size-exact / data / imports | 4524, 843, 207 |
| lint / drift | CLEAN / 0 |

The wave-110/111 prompt hint that `.ARM.extab` is 4 B short is **stale**: both
the original and the current build have `.ARM.extab` size `0x26298` at
`0x1a3b78`. Do not chase it.

## Finding 1 — the faithful free-credits source order scores worse

Ghidra's decompilation of the free-credits loop (`0x1b9c`-`0x1da8`) is
unambiguous about the source order:

```
iVar26 = TouchButton::getWidth(pTVar29);
iVar16 = *(int *)(Globals::layout + 0x2c);      // rowHeight, part of textX
uVar31 = TouchButton::getHeight(pTVar29);
AbyssEngine::PaintCanvas::SetColor(Globals::Canvas);
fVar13 = (float) uVar31;  fVar14 = (float) iVar15;   // height, y
iVar15 = (int) (fVar14 + fVar13 * 0.2);              // textY
iVar16 = iVar26 + iVar32 + iVar16;                   // textX = width + baseX + rowHeight
pSVar19 = GameText::getText(Globals::gameText, iVar26);
DrawString(Globals::Canvas, Globals::font, pSVar19, iVar16, iVar15, false);
```

i.e. the original source is

```cpp
int width  = button->getWidth();
int textX  = width + baseX + Globals::layout->field_0x2c_rowHeight;
int height = button->getHeight();
Globals::Canvas->SetColor(0xffffffffu);
float textYf = (float) y + (float) height * 0.2f;
int textY = (int) textYf;
```

and the original disassembly confirms it (`blx getWidth` at `0x1c50`,
`ldr.w r9,[layout,#44]` at `0x1c5a`, `blx getHeight` at `0x1c60`,
`blx SetColor` at `0x1c6e`). But writing exactly that **lowers** the score:

| free-loop source order | fuzzy | size |
|---|---:|---:|
| current (height first, width inline) | **89.057332** | 9700 |
| faithful (width, textX, height) | 77.153867 | 9700 |
| faithful with `int width` local | 77.153867 | 9700 |
| `const int width` | 77.153867 | 9700 |
| width, height, textX | 75.302863 | 9692 |
| faithful + non-volatile `textYf` | 75.843083 | 9700 |
| faithful + `int textY = (int)((float)y + (float)height*0.2f)` | 75.843083 | 9700 |
| faithful, `textYf` before `SetColor` | 75.165198 | 9696 |

Reason: with the faithful order the allocator spills `width` to `[sp,#124]`
and keeps `&Globals::Canvas` in `sl`; the original keeps `width` in `sl`
(`mov sl, r0` at `0x1c56`) and `&Globals::Canvas` in `r6`
(`ldr r6,[sp,#140]` at `0x1c64`). The call order is right but the register
roles are wrong, and the whole function's allocation cascades (the prologue
region ratio falls from 98.6 to 60.6 purely from pool-offset churn).

Free-credits statement-order permutation (W=width, H=height, C=SetColor,
Y=textYf/textY, X=textX); baseline is `H X C Y`:

| order | fuzzy | size |
|---|---:|---:|
| `H X C Y` (current) | **89.057332** | 9700 |
| `H C Y X` | 88.989941 | 9700 |
| `H C W Y X` / `H C W X Y` / `C H W X Y` | 88.962381 | 9700 |
| `H C X Y` (no width local) | 88.962381 | 9700 |
| `H W C X Y` | 86.773216 | 9704 |
| `H X Y C` | 86.545354 | 9696 |
| `W H C X Y` | 76.128855 | 9696 |
| `W X H Y C` | 75.165198 | 9696 |
| any order with W before H | 74.9–77.2 | 9692–9700 |

## Finding 2 — `int b` is `3 + (inTabIndex < 0)`, but the 4-byte deficit is real

Ghidra (line 737 of the decompilation) shows the selected-item button index as

```
iVar17 = 3 - (*(int *)(this_02 + 0x3c) >> 0x1f);   // 3 + (inTabIndex < 0)
```

and the original instruction at `0x15a8` is `add.w r4, r4, r3, lsr #31`
(`movs r4,#3` at `0x158e`). The current source uses
`item->inTabIndex == -1 ? 4 : 3`, which emits `adds r3,#1; it eq; moveq r4,#4`
(6 B instead of 4 B). Switching to the faithful form:

| `b` source | fuzzy | size |
|---|---:|---:|
| `inTabIndex == -1 ? 4 : 3` (current) | **89.057332** | 9700 |
| `3 + (inTabIndex < 0)` | 87.329381 | 9696 |
| `inTabIndex < 0 ? 4 : 3` | 87.329381 | 9696 |
| `3 - (inTabIndex >> 31)` | 87.301806 | 9696 |
| `3 - (inTabIndex < 0 ? -1 : 0)` | 87.301806 | 9696 |

The faithful form makes the **list body better** (region ratio 92.004 ->
92.850) but the body shrinks 4 B and the whole cleanup/landing-pad region
shifts: cleanup region ratio 83.777 -> 73.684. The 4-byte deficit is not in
the `b` expression itself; the address-delta trace shows it accumulates at
`0x12d6` (the original has an extra `ldr r0,[sp,#120]` before
`blx ListItem::isBluePrint` at `0x12d4`) and at `0x137c` (the original has a
2-byte `nop` after `b.n loc_165c` at `0x137a`). Both are compiler-scheduled
instructions that our source shape does not produce.

Trying to re-add the 4 bytes with a volatile sink near the `b` block was
worse: `b1 + volatile int padBP = 0` in isBluePrint 70.06, isSlot 85.18,
isPendingProduct 85.21, isShip 83.62.

## Finding 3 — the remaining diff is a global `r8`/`sl`/`r9` role swap

The dominant remaining difference is not a source-shape bug; it is the
callee-saved register assignment. Original vs current, same instruction
sequence:

| site | original | current |
|---|---|---|
| function entry (`Globals::appManager`) | `ldr.w r8,[r0]` | `ldr.w sl,[r0]` |
| `Globals::layout` (before `drawBG`) | `ldr.w sl,[r0]` | `ldr.w r8,[r0]` |
| blueprint block (`ldrd` at `0x602`) | `ldrd sl,r9,[sp,#132]` (`sl`=&layout, `r9`=this) | `ldrd r9,sl,[sp,#132]` (`r9`=&layout, `sl`=this) |
| buy-credits String temp (`0x1842`) | `add.w r8,sp,#288` | `mov r8,sl; add.w sl,sp,#288` |
| buy-credits switch `Globals::cItemListDescription_0X` | `ldr.w r0,[r8]` | `ldr r0,[r5,#0]` |

Because the two roles are swapped, ~50 instructions in the blueprint block
and ~100 in the buy-credits/cleanup region differ only by register name.
The buy-credits preheader even pays an extra `mov r8, sl` (2 B) because the
allocator keeps `&Globals::layout` in `sl` and reuses `sl` for the String
temp, while the original uses `r8` for the temp and leaves `sl` alone.

Attempts to force the swap from source (all worse or neutral):

| variant | fuzzy | size |
|---|---:|---:|
| `Layout *layout = Globals::layout;` at top of `viewMode==0` (all 99 uses) | 47.901371 | 9560 |
| `Layout *layout = Globals::layout;` at function top | 74.225100 | 9704 |
| `Layout *layout` inside free-credits block | 68.904788 | 9696 |
| `Layout *layout` inside buy-credits block (all uses) | 86.111877 | 9708 |
| `Layout *layout` unused local (any position) | 89.057332 | 9700 |
| `ApplicationManager *app` local, both uses | 75.914424 | 9640 |
| `ApplicationManager *app` local inside `if` | 76.107660 | 9640 |
| `PaintCanvas *cv` local (unused / used) | 89.057332 | 9700 |
| `HangarWindow *volatile self = this` at top | 70.037195 | 9708 |
| `PaintCanvas *volatile cv` at top | 70.349959 | 9704 |
| `Layout *volatile lay` at top | 67.171578 | 9712 |
| `volatile PaintCanvas *cv` (pointer-to-volatile, optimised away) | 89.057332 | 9700 |

## Other measured dead ends (wave 112)

All 9700 B / 3626:3630 unless noted.

| variant | fuzzy |
|---|---:|
| `SetColor` after station load | 84.458529 (9704) |
| `!(station != nullptr && pending)` | 89.057332 |
| `station->pendingHangarClose == 0` | 89.057332 |
| `drawBG()` after `getCurrentTab()` | 88.947078 |
| `drawBG()` after `getCurrentTabItems()` | 84.568752 (9704) |
| `tab` after `items` | 77.207506 (9664) |
| `bool pending = ...; if (!pending)` | 89.057332 |
| station expression inlined twice | 84.210526 (9708) |
| getText index `i ? i+112 : 3400` | 89.057332 |
| getText index `i == 0 ? 3400 : i+112` | 87.813620 (9692) |
| getText index via `int textId` ternary | 89.057332 |
| getText index via `if/else` | 89.057332 |
| getText index via `if` assignment | 87.813620 (9692) |
| `visible = !(i == 4 && options[76])` | 86.643694 (9696) |
| `visible = i < 4 \|\| !options[76]` | 86.993662 (9704) |
| `row++` outside the `if (visible)` | 68.036971 (9692) |
| `volatile float textYf` declared before `SetColor` | 86.545354 (9696) |
| `unsigned int count` / `unsigned int i` | 89.057332 |
| buy-credits `x` product-first | 86.738351 (9692) |
| buy-credits `x` product-first (both sites) | 82.054813 (9708) |
| buy-credits `x` with explicit `(w/2 - bw - gs)` grouping | 89.057332 |
| `switch (tab)` -> `if (tab == 2) ... else if (tab == 0)` | 83.059959 (9696) |
| `switch (tab)` -> `if (tab == 0) ... else if (tab == 2)` | 86.108049 |
| `switch` with `case 2` first | 86.273429 |
| `switch` on local `int t = tab` | 83.200000 (9684) |
| `unsigned int tab` | 89.057332 |
| `volatile int tab` | 50.365064 (9708) |
| `viewMode == 0` -> `!viewMode` | 89.057332 |
| `viewMode == 1` -> `viewMode` | 88.781698 |
| `items != nullptr` -> `items` | 89.057332 |
| `dialogActive` / `buyCreditsActive` / `freeCreditsActive` forms | 89.057332 |
| drawBox x `hintOffsetX + buttonInsetX` (all 4) | 88.947078 |
| drawBox x (box 3 or 10 only) | 89.029768 |
| buttons[8]/[9] x `hintOffsetX + buttonInsetX` | 89.002205 |
| blueprint bg x `hintOffsetX + textX + 2 + buttonInsetX` | 87.915116 (9704) |
| blueprint bg x `buttonInsetX + textX + 2 + hintOffsetX` | 89.057332 |
| blueprint bg x `textX + 2 + hintOffsetX + buttonInsetX` | 87.915116 (9704) |
| blueprint border x layout-first | 89.057332 |
| blueprint progress x hint-first | 89.029768 |
| blueprint drawItem x iconSpace-first | 89.057332 |
| `pad3` after setPosition / after draw / before `if (left>0)` | 85.71 / 85.79 / 84.90 |
| `volatile int *pad3 = &left` | 88.643881 |
| `pV` at function top | 70.129512 (9704) |
| `pV` before `if (viewMode==1)` | 62.123090 (9688) |
| `pV` after the `viewMode==1` block | 86.855883 (9704) |
| `pV` in `viewMode==0` block | 70.157068 (9704) |
| buy-credits `setVisible` before String declarations | 87.014061 (9696) |
| buy-credits `row`/`col` before switch | 72.569779 (9712) |
| buy-credits `x` before `y` | 89.057332 |

## What is proven

1. `.ARM.extab` is already exact (`0x26298` @ `0x1a3b78`); the "4 B short"
   hint in the wave-110/112 prompt is stale.
2. The original free-credits source order is `getWidth`, `textX`, `getHeight`,
   `SetColor`, `textYf`, `textY`, `getText`, `DrawString` (Ghidra + Thumb
   agree), but writing that order costs ~12 fuzzy points because the allocator
   spills `width` and keeps `&Globals::Canvas` in `sl`.
3. The original `int b` is `3 + (inTabIndex < 0)` (`add.w r4,r4,r3,lsr #31` at
   `0x15a8`). The faithful form improves the list body by 0.85 region points
   but shrinks the body 4 B and destroys the cleanup region match.
4. The remaining ~11 % is dominated by a global `r8`/`sl`/`r9` role swap
   between `Globals::layout`, `this`, and the buy-credits `String` temporary.
   No source-level local/volatile/operand-order variation tried in this pass
   flips it.
5. The full gate stays green on the unmodified source: `check` RATCHET PASS,
   byte 4500 / linked 4500 / size 4524 / data 843 / imports 207, lint CLEAN,
   drift 0. No section size/base/count changed.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS

# raw fuzzy (same normalisation as the gate, finer than the 0.1 rounding)
python3 /dev/shm/w6010/score.py _ZN12HangarWindow6renderEv
```

Scratch helpers (`m.sh`, `score.py`, `analyze2.py`, `region.py`, `batch.py`,
`v1.json`-`v23.json` variant lists) live under `/dev/shm/w6010/`; they are not
part of the commit.
