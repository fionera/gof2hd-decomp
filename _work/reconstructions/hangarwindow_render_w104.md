# HangarWindow::render — wave 104: operand order + SetColor scheduling (w5313)

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`, real `0x1490d8`,
`0x25e4` = 9700 B).  Baseline: wave-103 master `835748c18` (render 84.4 %, 9700 B,
`.ARM.extab` `0x26298`, record 948 B / 156 rows).

## Result

| metric | before (HEAD) | landed |
|---|---:|---:|
| `render` fuzzy | 84.4 | **84.7** |
| `render` size | 9700 | 9700 |
| `.ARM.extab` | 0x26298 | **0x26298** |
| `render` LSDA record | 948 B / 156 rows | **948 B / 156 rows** |
| byte_exact | 4500 | 4500 |
| linked_exact | 4500 | 4500 |
| size_exact | 4524/4524 | 4524/4524 |
| data_exact | 843/843 | 843/843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |

## What changed (all in `HangarWindow::render`, measured one at a time)

1. **`Layout::drawBox` operand order** (4 sites, the selected/row highlight box):
   `this->hintOffsetX + Globals::layout->buttonInsetX` -> `Globals::layout->buttonInsetX + this->hintOffsetX`.
   The original evaluates the `Globals::layout` load first (`0x149252 subs r2,r2,r6; 0x14925e add r2,r3`).
   84.4 -> 84.5.
2. **`tab == 1` selected-item header `DrawString` x expressions** (2 sites):
   `this->hintOffsetX + Globals::layout->buttonInsetX + this->field_0x100.a - ...`
   -> `Globals::layout->buttonInsetX + this->hintOffsetX + this->field_0x100.a - ...`, and
   `listWidth + this->hintOffsetX - ...` -> `this->hintOffsetX + listWidth - ...`.
   84.5 -> 84.6.
3. **`else` (non-`tab == 1`) header `DrawString` x expressions** (2 sites): same reorder
   (`Globals::layout->buttonInsetX + this->hintOffsetX + ...`, `this->hintOffsetX + listWidth - ...`).
   84.6 (removes an `add` mismatch; neutral score).
4. **Free-credits block: `SetColor` scheduling.**  Move `Globals::Canvas->SetColor(0xffffffffu)`
   between the `textYf` float conversion and `int textY = (int) textYf;`:
   ```cpp
   int height = button->getHeight();
   int textX = button->getWidth() + baseX + Globals::layout->field_0x2c_rowHeight;
   volatile float textYf = (float) y + (float) height * 0.2f;
   Globals::Canvas->SetColor(0xffffffffu);
   int textY = (int) textYf;
   ```
   Original `0x14ac34`-`0x14ac6a`: `getHeight` -> `SetColor` -> `vcvt/vmla` -> `vcvt.s32`.
   84.6 -> **84.7**.  `volatile` on `textYf` is still required (plain `float` -> 9716 B, 74.0 %).
5. **`tab != 0` split for the selected ship button** (semantic, neutral score).  The original
   (REA lines 753-775) has `if (iVar11 != 0) { if (iVar11 == 3 && selected->ship == status->getShip()) goto done;
   setPosition/setVisible/draw; buttonOffset = getWidth() + rowHeight; }`, i.e. the `tab == 3` guard
   is nested inside `tab != 0`, not a single `&&`:
   ```cpp
   if (tab != 0) {
       if (!(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) { ... }
   }
   ```

## Experiments that did **not** help (all measured)

* **Free-credits `getWidth` before `getHeight`** (`int textX = button->getWidth() + ...;` first):
  74.2 %, body 9700.  The original's *assembly* does `getWidth` (`0x14ac2a blx 0xfff1bcc8`) before
  `getHeight` (`0x14ac34 blx 0xfff1de34`), but hoisting `getWidth` in source changes global register
  allocation (282 mismatched shape blocks vs 120).  The current `getHeight`-then-`getWidth`
  source shape is the one that reproduces the original's allocation; the compiler schedules the
  `getWidth` call first on its own.
* **`int right, left;` declaration order / `= 0` initialisers / separate declarations**: all
  byte-identical at 84.7 (no LSDA change).
* **Removing `padR`** (the w103 156th-row anchor): 86.6-86.7 % but `.ARM.extab` shrinks to
  `0x26290` (940 B / 155 rows), which shifts `.rodata` and drops `byte_exact` 4500 -> 4040.
  Not gate-safe.
* **Alternative 156th-row anchors** (volatile stores at the start/end of `if (left > 0)`,
  inside `else { buttonOffset = 0; }`, at the top of the item/ship branches, in the buy-credits
  and free-credits blocks): every placement either grew the body to 9704/9708 B or left the
  record at 940 B.  `volatile int padR = 0;` at the top of `if (right > 0)` is the only anchor
  found that keeps **both** 9700 B and 948 B.

## The 948-byte record / why the anchor is needed

Original LSDA rows for the two selected-item amount calls:

```
row 16  start=0x149656 len=4 lp=0x14b5a2   -> Item::getStationAmount
row 17  start=0x149660 len=4 lp=0x14b64c   -> Item::getAmount
```

Both landing pads are the same 2-insn stub (`b.n 0x14b698`, destroy the `String name` at
`sp+0x138`), but clang 7.0.2 emitted two separate stubs.  Our build merges the two adjacent
calls into one 18-byte row (`start=0x149658 len=0x12 lp=0x14b648`), so the call-site table is
one row short and the record rounds to 940 B.  `padR` forces the 156th call-site row;
`table_end` becomes 929 B, which rounds to the original 948 B.

## Remaining diff (regalloc-dominated)

Shape ratio 94.24 %, 126 mismatched blocks / 418 elements.  Dominant residuals:

* `this` lives in `fp` in the original but in `r6`/`r5` in ours; `item` is reloaded from
  `sp+0x78` in ours vs kept in `r4` in the original.
* `ldrd sl, r9, [sp,#132]` (original: `sl = &Globals::layout`, `r9 = this`) vs
  `ldrd r9, sl, [sp,#132]` (ours) in the blueprint progress-bar block (`0x1496da`).  This is the
  only remaining register-permutation-only instruction in the function.
* Literal-pool word counts differ by 2 in the buy-credits area; the original has extra `b.n`
  epilogue stubs at `0x14b4be`-`0x14b4d0`.

## Evidence

* REA/Ghidra decompilation: `/dev/shm/w5313/rea-render.json` (image base 0x10000; real addr
  `0x1490d8`), `/dev/shm/w5313/rea-render.c`.
* LSDA dump: `python3 _work/reconstructions/gameresource_gen/lsda.py <so> _ZN12HangarWindow6renderEv`.
* Side-by-side: `FN=_ZN12HangarWindow6renderEv cmake --build cmake-build-match --target verify-fn`.
