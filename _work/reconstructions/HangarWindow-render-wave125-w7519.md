# `HangarWindow::render` — wave 125 (w7519): free-credits role re-seat

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7519`,
base master `02513de33` (wave-124 final; `render` 94.057631 %).

This pass: **94.057631 -> 94.388529** (raw difflib over the reconciled
instruction stream). Size stays 9700 B, insn count stays 3626/3627.

## Landed source change

Free-credits block only, 3 insertions / 2 deletions:

```diff
             if (visible) {
+                int rh = Globals::layout->field_0x2c_rowHeight;
                 int width = button->getWidth();
-                int textX = width + baseX + Globals::layout->field_0x2c_rowHeight;
+                int textX = width + baseX + rh;
                 int height = button->getHeight();
-                PaintCanvas *canvas = Globals::Canvas;
+                PaintCanvas *volatile canvas = Globals::Canvas;
                 canvas->SetColor(0xffffffffu);
```

## Evidence

Original free-credits block at `0x14ad26`:

```
ldr.w r9, [r0, #44]      ; layout->field_0x2c_rowHeight, BEFORE getWidth()
...  bl  TouchButton::getWidth
mov   r1, sl             ; width
mov   sl, r0             ; width -> sl
```

The original reads `field_0x2c_rowHeight` (`layout+0x2c`) **before** the
`getWidth()` call and keeps width in `sl`, rowHeight in `r9`. Naming the
rowHeight as a local `rh` declared before `width` re-seats the load ahead of
`getWidth()`, and moves width off the stack spill.

`PaintCanvas *volatile canvas` (volatile **pointer**, not pointee) makes clang
re-load `Globals::Canvas` before each call, which is exactly what the original
does (`ldr r0,[r5,#0]` / `ldr.w r9,[r5]` before each `SetColor`/`DrawString`),
and removes the extra `ldr`/`mov` noise. A plain `PaintCanvas *canvas` keeps
the pointer in a callee-saved register across the calls and loses those
reloads.

The two changes are independent: `volatile` canvas alone gives 94.360954,
`rh` local alone gives 94.057631 (neutral), and the combination gives
94.388529.

## Result

| metric | before (HEAD 02513de33) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 94.057631 | **94.388529** |
| `render` size | 9700 | 9700 |
| normalized insns | 3626 / 3627 | 3626 / 3627 |
| `.ARM.extab` record | 948 B (exact) | 948 B (exact) |
| `.ARM.exidx` | byte-identical | byte-identical |
| byte_exact / linked_exact | 4506 / 4506 | 4506 / 4506 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`check`: **RATCHET PASS** — `verify: byte 4506 linked 4506 stubs 0 missing 0
extra 0 avg 99.93`; `data: exact 843 of 843`; `sodiff: imports 207`; lint
`CLEAN ✓`; drift `0 field(s)`. `exidx_table.py --diff` reports no differing
extab record (total delta +0). All ELF section sizes and bases are unchanged.

## Remaining residual (unchanged root causes)

Still not byte-exact: the original keeps `sl`=rowHeight and `r9`=width with
`mov r1,sl; mov sl,r0` at `0x14ad2c`; ours now has `sl`=width, `r9`=rowHeight
(closer but swapped). The `volatile` canvas also adds a `str`/`ldr` of the
canvas pointer through `[sp,#204]`, which the original does not have.

The four independent Greedy/CFG ties from wave 124 remain:

1. `0x1496da` blueprint `ldrd sl,r9` (orig) vs `ldrd r9,sl` (ours): Greedy
   tie; `this`->r10/layout->r9 in ours, opposite in orig.
2. `0x149b20` ship branch: orig `ldrd r5,r4` + `cbz`; ours `ldrd r6,r5` +
   `cmp/beq.w` (jump-threading of the `tab==0` edge).
3. Buy-credits `0x14a8ec-0x14abf2`: orig keeps `&Globals::layout` in `sl`,
   ours spills it (`strd sl,r9,[sp,#128]`) and puts the `sp+312` String temp
   in `sl` instead of `r6`.
4. Free-credits tail: now much closer, but `sl`/`r9` roles are swapped and
   the volatile canvas pointer spills through `[sp,#204]`.

## Variants measured this wave (all on the wave-124 base)

Free-credits block:

| variant | pct | size |
|---|---:|---:|
| base | 94.057631 | 9700 |
| **`rh` local before width + `PaintCanvas *volatile canvas` (LANDED)** | **94.388529** | 9700 |
| `PaintCanvas *volatile canvas` alone | 94.360954 | 9700 |
| `rh` local before width alone | 94.057631 | 9700 |
| `rh` after width | 94.360954 | 9700 |
| `const int rh` | 94.388529 | 9700 |
| `volatile int rh` | 75.899132 | 9708 |
| `rh` + no canvas local | 92.112521 | 9712 |
| `const int width` / `unsigned width` / `textX` reorder / `textX` split | 94.388529 | 9700 |
| `height` before `width` | 91.179713 | 9708 |
| `volatile int width` | 91.550655 | 9704 |
| `volatile int height` | 91.312741 | 9704 |
| `volatile int textX` | 68.632856 | 9604 |
| canvas local declared first | 69.735391 | 9620 |
| no canvas local | 92.152807 | 9712 |
| `PaintCanvas &canvas` | 94.057631 | 9700 |
| `PaintCanvas *volatile const` | 94.360954 | 9700 |
| `PaintCanvas *volatile &canvas` | 92.112521 | 9712 |
| `volatile PaintCanvas *canvas` | BUILDFAIL | - |
| canvas local only for 2nd half | 91.608103 | 9712 |
| `GameText *text` local | 77.479967 | 9628 |
| `textY` before `SetColor` | 94.250655 | 9700 |
| non-volatile / const / unsigned `textY` | 75.374673 | 9672 |
| `volatile float textYf` + `int textY` | 91.637967 | 9720 |
| `volatile float` + `volatile int` | 91.412819 | 9708 |
| `visible` if/else (wave-122 form) | 94.043023 | 9700 |
| `visible` ternary / `visible = true` + `else if` / De Morgan / `i != 0 &&` chain | 94.057631 | 9700 |
| `visible` `&&` swapped | 91.289967 | 9708 |
| free-credits `i==0`/`i!=0` body swap | 87.131441 | 9664 |
| `i == 0 ? ... : ...` single DrawString | 87.382662 | 9632 |
| `String amount` temp | 83.473932 | 9632 |
| `small_first_nested` (`else if (i != 0)`) | 94.057631 | 9700 |

Buy-credits block:

| variant | pct | size |
|---|---:|---:|
| `Layout *layout` / `Layout &layout` / `Layout *const layout` in block | 73.182888 | 9564 |
| `Layout *&layout = Globals::layout` in block | 94.057631 | 9700 |
| function-scope `Layout *&layout` | 94.057631 | 9700 |
| function-scope `Layout *layout` / `Layout &layout` | 39.020998 | 9584 |
| `setVisible` after switch | 88.141202 | 9704 |
| `setVisible` first | 89.798732 | 9700 |
| `String price` before `String description` | 93.947332 | 9700 |
| `String description; String price;` (default ctor) | 91.072168 | 9688 |
| implicit `description = Globals::cItemListDescription_0N` | 68.708241 | 9452 |
| `String(x, false)` | 94.057631 | 9700 |
| `count < 5` | 94.057631 | 9700 |
| `count != 5` | 79.834369 | 9648 |
| `unsigned count` | 94.057631 | 9700 |
| `String description = ""` / `("", false)` | 94.057631 | 9700 |
| default ctor + assign | 88.843032 | 9756 |

Blueprint branch:

| variant | pct | size |
|---|---:|---:|
| `const float rate` | 94.057631 | 9700 |
| `volatile float rate` | 71.820311 | 9716 |
| `int pw = this->progressBarWidth` | 92.388307 | 9700 |
| `int hint = this->hintOffsetX` | 94.057631 | 9700 |
| `(*this).progressBarBgImage` | 94.057631 | 9700 |
| x-expr grouped `(textX+2)+(inset+hint)` | 94.057631 | 9700 |
| y-expr `y + 1 + rowHeight/2` | 94.057631 | 9700 |
| region x `textX+3+hint+inset` | 94.057631 | 9700 |
| `progress = (a+b)+(c+d)` | 90.404529 | 9676 |
| `HangarWindow &self` / `*self` in branch | 88.705006 | 9704 |
| `progress` implicit `+ "%  ("` / left-assoc / `pct` local / `rate100` local | 94.057631 | 9700 |

Ship branch:

| variant | pct | size |
|---|---:|---:|
| `if (tab != 3 || ship != getShip())` | 94.057631 | 9700 |
| `switch (tab) { case 0: break; default: ... }` | 93.919757 | 9700 |
| `volatile int tab` | 50.551876 | 9684 |
| `tab == 1 && upgradeMode` | 94.002482 | 9700 |
| `padX` as `volatile int` | 86.478291 | 9708 |
| drop `padX` | 86.698828 | 9700 |
| `padX = &tab;` | 94.057631 | 9700 |
| `padX = &tab` in `else` of `tab != 0` | 94.057631 | 9700 |

Loop / misc:

| variant | pct | size |
|---|---:|---:|
| `for (unsigned int i = ...)` inline decl | 94.002482 | 9700 |
| `for (int i = ...)` | 93.974907 | 9700 |
| `const int baseY` / `const int y` | 94.057631 | 9700 |
| `y` computed before `baseY` | 94.002482 | 9700 |
| `item` before the y-range `continue` | 94.057631 | 9700 |
| `const float rate` | 94.057631 | 9700 |

## Conclusion

The free-credits block is now closer to the original's register roles
(`rh` load hoisted before `getWidth()`, per-call canvas reload), but the
`sl`/`r9` roles are still swapped and the volatile pointer adds a spill the
original does not have. The remaining 142-ish diff ops are the same four
independent Greedy/CFG ties documented in wave 124.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint -j6       # CLEAN
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7519/{score.py,batch.py,compile.sh,
HangarWindow.base.cpp,v1..v25.py,mi.txt,w7519_orig.asm,w7519_ours.asm}`.
