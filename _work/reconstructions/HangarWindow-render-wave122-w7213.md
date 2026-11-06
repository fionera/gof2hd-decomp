# HangarWindow::render — wave 122 (w7213)

Function: `_ZN12HangarWindow6renderEv` (9700 B @ `0x1490d8`)
Owned TU: `src/game/ui/HangarWindow.cpp` (render() body only)

## Result

| | before | after |
|---|---|---|
| verify pct | 91.655191 % (report 91.7) | **94.043023 %** (report 94.0) |
| size | 9700 B | 9700 B |
| normalized insns | 3626 / 3636 | 3626 / 3626 |
| structural diff ops | 142 | 114 |
| gate byte_exact | 4506 | 4506 |
| gate linked_exact | 4506 | 4506 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

The `.ARM.extab` record (948 B) stays exact; the LSDA shape is untouched.

## Source change (free-credits block, tail of render())

The original keeps `PaintCanvas` in a callee-saved register across the two
`DrawString` calls and keeps the `getWidth()` result in a register (not a
stack spill).  The previous source reloaded `Globals::Canvas` for every call
and evaluated `getWidth()` inline in the `textX` expression, which made the
allocator spill the width and re-load the canvas global.

Old shape:

```cpp
Layout *lay = Globals::layout;
int height = button->getHeight();
int textX = button->getWidth() + baseX + lay->field_0x2c_rowHeight;
Globals::Canvas->SetColor(0xffffffffu);
volatile float textYf = (float) y + (float) height * 0.2f;
int textY = (int) textYf;
Globals::Canvas->DrawString(...);
Globals::Canvas->SetColor(0x777777ffu);
if (i != 0) { ... } else { ... }
```

New shape:

```cpp
int width = button->getWidth();
int textX = width + baseX + Globals::layout->field_0x2c_rowHeight;
int height = button->getHeight();
PaintCanvas *canvas = Globals::Canvas;
canvas->SetColor(0xffffffffu);
volatile int textY = (int) ((float) y + (float) height * 0.2f);
canvas->DrawString(hangarFont(), *Globals::gameText->getText(i != 0 ? i + 112 : 3400),
                   textX, textY, false);
canvas->SetColor(0x777777ffu);
if (i == 0) {
    canvas->DrawString(hangarFont(), String(""), textX, textY + Globals::layout->field_0x4, false);
} else {
    canvas->DrawString(hangarFont(),
                       String("(+") + Globals::layout->formatCredits(FREE_CREDIT_AMOUNTS[i]) + String(")"),
                       textX, textY + Globals::layout->field_0x4, false);
}
```

Also dropped the dead `volatile int pV = 0;` from the `if (this->viewMode == 1)`
block (neutral for codegen, but it was a NoopGuard-era artefact).

Why it works: the named `width` temp is live across the `getHeight()` call, so
clang 7.0.2 must keep it in a callee-saved register; the named `canvas` local
is live across `getText()`/`formatCredits()`, so the canvas pointer is kept in
a callee-saved register instead of being re-loaded from the GOT for every
call.  The `i == 0` arm is written first so the empty-string `DrawString`
falls through to the loop increment, matching the original's block order.
`volatile int textY` (rather than `volatile float textYf` + `int textY`)
matches the original's single `vcvt.s32.f32` + `vstr`/`ldr` spill shape.

## Variants tried (all measured)

| variant | pct | size |
|---|---|---|
| baseline (before) | 91.66 | 9700 |
| `width` first + old `volatile float textYf` + big arm first | 91.07 | 9700 |
| body swap alone | 91.71 | 9720 |
| `volatile int textY` alone (no canvas local) | 88.87 | 9688 |
| canvas local before `width` | 69.9 | — |
| canvas local after `SetColor` | build fail | — |
| canvas local after textY | 92.3 | — |
| canvas local in blueprint branch | 59.8 | 9592 |
| canvas local in ship branch | 70.5 | — |
| canvas local in buy-credits block | 67.5 | — |
| canvas local for whole body | 50.5 | — |
| `volatile unsigned int textY` | 94.043 | 9700 |
| `height*0.2f + y` order | 94.043 | 9700 |
| `0.2F` | 94.043 | 9700 |
| `volatile short textY` | 91.54 | 9700 |
| `volatile double textY` | 91.22 | 9700 |
| `pV` dropped / moved | 94.043 | 9700 |
| hoist `getText` into `String &text` | 91.54 | 9700 |
| hoist `int field4` | 91.84 | 9712 |
| `Layout *volatile lay2` | 92.10 | 9708 |

## Remaining residual (114 diff ops)

- Blueprint branch `0x1496da`: orig `ldrd sl,r9,[sp,#132]` (sl=&Globals::layout,
  r9=this) vs ours `ldrd r9,sl` — cascades to `0x149840`.
- Ship branch `0x149b20`: orig `ldrd r5,r4` + `cbz r0,149b96` vs ours `ldrd r6,r5`
  + `cmp/beq.w` (jump threading of the `tab == 0` edge).
- Buy-credits half `0x14a8xx-0x14ac7x`: `r8`/`sl` role swap
  (`str.w r9,[sp,#132]` vs `strd sl,r9,[sp,#128]`), extra `mov r8,sl`.
- Free-credits tail `0x14ad2c-0x14ae8x`: orig keeps `sl` = width, `r9` = rowHeight,
  `r6` = Canvas; ours keeps `sl` = rowHeight and spills width to `[sp,#116]`.
- Cleanup landing pads shifted (identical code, different offsets).

Root cause of the remaining buy-credits delta: the original keeps
`Globals::layout` live in `sl` from `0x14a85a` (`layout->drawFooter()`) through
`0x14ab02`, which frees `r6` for the `description` String.  Our source reloads
`Globals::layout` from the GOT at each use, so the allocator reuses `sl` for
`description` and spills layout to `[sp,#128]`.  A function-scope
`Layout *layout = Globals::layout;` local was tried but changed the whole
function's allocation (`this` moved from `fp` to `r9`, 62.7 %), so it was
reverted.
