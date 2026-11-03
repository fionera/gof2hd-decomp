# `HangarWindow::render` — lazy-layout / free-credits regalloc sweep (w3828)

Symbol `_ZN12HangarWindow6renderEv`, original `0x1490d8`, size `0x25e4` (9700 B).
Branch `wave89-w3828`, base master `5710f6d0`. Ground truth:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (Ghidra `android_2.0.16_libgof2hdaa.so`,
image base 0x10000). Build-flag work is settled; this pass is source-shape only.

## TL;DR

No source change beats the current tree. Baseline is **80.6 % / 9700 B**
(raw difflib `0.8064427312775331`, 3626 vs 3638 normalized insns), gate
byte 4498 / linked 4498 / `.text 0x1337c8`, lint CLEAN, drift 0.

This pass localised the residual to **three independent register-allocation
facts**, each measured directly against the original:

1. The original keeps `this` in `fp` and `&Globals::layout` in `sl`, and it
   **rematerialises the `Globals::layout` GOT address a second time at
   `drawFooter`** (`0x14a85a`). Ours materialises it once, in the prologue.
2. The original's **hint loop does not cache `Globals::layout`** (`sl` is
   reused for `rows`; the pointer value is reloaded from `[sp+132]` every
   iteration). Ours caches it in `r9` via the `hintLayout` local.
3. The original's **free-credits loop is width-first and hoists `textX`
   before the `getText` call** (`getWidth` → `rowHeight` → `getHeight`,
   `textX` in `sl`, `height` in `r4`, `Canvas` reloaded from `sp+140`).
   Ours is height-first, spills `width` to `sp+116`, keeps `Canvas` in `sl`,
   and recomputes `textX` after `getText`.

Every faithful shape that fixes (2) or (3) flips `this` out of `fp`
(`this = r8` or `sl`) and loses 1.5–19 points. The two remaining anchor
locals (`hintLayout`, the three `description = description;` self-assigns)
are what currently pin `this = fp` and the exact 9700-byte size.

## Measured baseline (this tree)

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS; lint CLEAN; drift 0
.text 0x1337c8, .ARM.extab 0x2628c, .rodata 0x1c9e10 (render record 155 rows; original 156)
render: 9700/9700 B, pct 80.6, raw 0.8064427312775331
```

## Section accounting (exact, this build)

Boundaries are the `blx drawBG` and `blx drawScrollBar` call sites.

| region | original | ours | delta |
|---|---|---|---|
| prologue (`0x1490d8`..drawBG) | 39 insn / 102 B / 12 wide | 40 / 104 / 12 | +2 B |
| item loop (drawBG..drawScrollBar) | 2243 / 5774 / **644 wide** | 2244 / **5756** / **634 wide** | **−18 B** |
| tail (drawScrollBar..end) | 1452 / 3824 / 460 | 1461 / 3840 / 459 | +16 B |
| total | 3734 / 9700 | 3745 / 9700 | 0 |

The 18-byte item-loop shortfall is exactly **10 missing 32-bit encodings
(20 B) minus 1 extra 16-bit instruction (2 B)**: ours keeps one fewer value
in a high register through the item loop. The tail is 16 B long because of
the three `description = description;` calls (24 B) minus 8 B elsewhere.

## What the original actually does (Thumb, measured)

Prologue: `mov fp, r0`; two pc-relative globals before `SetColor`
(`__stack_chk_guard`, `Globals::Canvas`); `Globals::appManager` after;
`Globals::layout` only at `drawBG`:

```
149132: ldr r0,[pc,#748]
149134: add r0,pc
149136: ldr.w sl,[r0]        ; sl = &Globals::layout
14913a: ldr.w r0,[sl]
14913e: blx drawBG
```

and again at `drawFooter`:

```
14a85a: ldr r0,[pc,#544]
14a85c: add r0,pc
14a85e: ldr.w sl,[r0]        ; rematerialised
14a862: ldr.w r0,[sl]
14a866: blx drawFooter
```

Ours materialises `&Globals::layout` once at `0x1490f6` and reuses `sl`
(and `r9` after the hint loop) for the whole body; there is no second PIC
site. `this` is `fp` in both.

Hint loop (original):

```
14923c: mov sl, r0           ; sl = rows (clobbers &layout)
14923e: mov.w r9, #0         ; r9 = r*imgH
149242: mov.w fp, #0         ; fp = r  (this spilled to sp+136)
149248: ldr r5,[sp,#132]     ; &Globals::layout from spill
14924a: movs r6,#1
14924c: ldr r2,[r5,#0]       ; layout value reloaded every iteration
```

Ours: `r8 = rows`, `fp = r*imgH`, `sl = r`, and the layout value is cached
in `r9` for the whole loop (`hintLayout`). Removing the local reproduces the
original's reloads but adds 16 B and drops the score to 0.7706.

Free-credits loop (original):

```
14ad28: blx getWidth
14ad2c: mov r1, sl           ; r1 = &Globals::layout
14ad2e: mov sl, r0           ; sl = width
14ad30: ldr r0,[r1,#0]
14ad32: ldr.w r9,[r0,#44]    ; rowHeight
14ad36: mov r0, r4
14ad38: blx getHeight
14ad3c: ldr r6,[sp,#140]     ; Canvas (reloaded)
14ad3e: mov r4, r0           ; r4 = height
...
14ad78: add.w r1, sl, r8     ; textX = width + baseX
14ad7e: add.w r8, r1, r9     ; textX += rowHeight   (before getText)
14ad90: blx getText
```

Ours: `getHeight` first, `width` spilled to `sp+116`, `sl = Canvas`, and
`textX` is recomputed *after* `getText` (`0x14ada2`). The original's early
`textX` is what lets `width` die before the call, so `sl` can hold it.

## Experiments (all measured in this tree; baseline 0.8064427 / 9700)

| variant | raw | size |
|---|---|---|
| baseline | 0.8064427 | 9700 |
| `hintLayout` -> `Globals::layout` (faithful hint loop) | 0.7706069 | 9716 |
| same + lazy `Globals::layout` (no top local) | 0.6258073 | 9712 |
| same + remove 3 self-assigns | 0.7306527 | **9700** |
| same + remove self-assigns + remove `pad3` | 0.7314050 | 9696 |
| same + remove self-assigns + width-first | 0.6570248 | 9668 |
| width-first (keep `hintLayout`) | 0.6198541 | 9744 |
| width-first + non-volatile `textY` | 0.6164648 | 9748 |
| width-first + no hint local | 0.6929004 | 9692 |
| width-first + `Layout *width` temp | 0.6198541 | 9744 |
| width-first + `rowHeight` temp first | 0.6194934 | 9744 |
| selected-item amount block as two ternaries | 0.7805954 | 9676 |
| same + remove `pad3` | 0.7810863 | 9672 |
| `pad3` moved after `buttons[9]->setVisible` | 0.7893142 | 9696 |
| `pad3` before the amount if/else | 0.7937207 | 9696 |
| `pad3` inside the else before `getAmount` | 0.6025924 | 9708 |
| `pad3` moved after `buttons[8]->setVisible` | 0.8050661 | 9700 |
| hint loop: 2nd use via `Globals::layout` | 0.7877787 | 9700 |
| hint loop: 1st use via `Globals::layout` | 0.7586397 | 9692 |
| hint loop: use top `layout` local | 0.6593497 | 9720 |
| all `Globals::layout->` -> top local in `render` | 0.4524008 | 9556 |
| `baseY` operand reordered | 0.8056167 | 9700 |
| `textX` operand reordered | 0.8058921 | 9700 |
| `this->hintOffsetX + buttonInsetX` -> reversed (12 sites) | 0.7892636 | 9704 |
| `if (left > 0)` -> `if (left)` | 0.7898651 | 9696 |
| `if (barHeight > 0)` -> truthiness | 0.8050661 | 9700 |
| `getCompletionRate() > 0.0f` -> `!= 0.0f` | 0.8006608 | 9700 |
| `if (item == selectedItem)` -> reversed | 0.8061674 | 9700 |
| `inTabIndex > -1` -> `!= -1` | 0.8049553 | 9700 |
| `b = inTabIndex == -1 ? 4 : 3` -> `< 0 ? 4 : 3` | 0.8060030 | 9700 |
| `b = ...` -> `3 + (inTabIndex == -1)` | 0.8064427 | 9700 |
| `HangarWindow &self = *this` + lazy | 0.6161242 | 9724 |
| `HangarWindow *self = this` + lazy | 0.6161242 | 9724 |
| `volatile int textXv` | 0.7893142 | 9680 |

Neutral (codegen-identical, 0.8064427 / 9700): `y` operand order,
`listWidth -= A + B`, `int left, right;`, `String name;` after
`int buttonOffset;`, `int i` loop, `unsigned b` loop, `!viewMode` /
`viewMode`, `visible != 0`, `right != 0`, `dialogActive == 0`,
`options[..] == 0`, `showBestDeal != 0`, `Layout *const hintLayout`,
`Layout &hintLayout`, `inTabIndex >= 0` vs `> -1` at all four sites.

## Root cause (single most important)

**RegAllocGreedy's tie between `this` and `&Globals::layout`.** The original
IR lets `&Globals::layout` die/rematerialise (two PIC sites, spilled to
`sp+132`, reloaded per hint-loop iteration) while `this` stays in `fp`.
Clang 7 on every plain-C++ shape we can write either:

* keeps `&Globals::layout` live across the whole body in `sl` (our baseline:
  one PIC site, `hintLayout` cached in `r9`), or
* loads it lazily at `drawBG`, at which point the allocator gives `fp` to
  `&Globals::layout` and moves `this` to `sl`/`r8` (0.61–0.77).

The missing 18 B in the item loop is the same phenomenon: ours has 10 fewer
32-bit encodings because one fewer high-register value is live.

## Commands

```sh
cmake --build cmake-build-match --target check -j8   # RATCHET PASS byte 4498 linked 4498
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

## Proven vs hypothesis

Proven (measured in this tree):

* Baseline 80.6 % / raw 0.8064427312775331 / 9700 B; gate 4498/4498.
* Region accounting: prologue +2 B, item loop −18 B (10 fewer wide
  encodings), tail +16 B (three self-assigns).
* Original has two `Globals::layout` PIC sites (`0x149132`, `0x14a85a`);
  ours has one.
* Original hint loop reloads the layout value per iteration; ours caches it.
* Original free-credits loop is width-first and computes `textX` before
  `getText`; ours is height-first and recomputes `textX` after.
* Every faithful variant above regresses or is neutral.

Hypothesis / open:

* The original source has no `hintLayout` local and no self-assigns; the
  missing 18 B + 16 B must come from the item-loop/free-credits register
  pressure, not from extra statements.
* A shape that makes clang 7 hoist `textX` before `getText` while keeping
  `this = fp` would likely recover most of the free-credits loop and the
  second PIC site. No plain-C++ form found in this pass does both.
