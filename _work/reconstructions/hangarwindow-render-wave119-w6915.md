# HangarWindow::render — wave 119 (w6915): pool-offset analysis + 110 variants, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6915`, branch `wave119-w6915`, base `0bbd27a1a`.

Builds on `hangarwindow-render-wave119-w6839.md` (91.337281 landed).
This pass: **91.337281 -> 91.337281** (no source change landed; note only).

## Result

| metric | HEAD `0bbd27a1a` | after this pass |
|---|---:|---:|
| `render` fuzzy (fresh Elf32/Resolver, same normalisation as `verify.py`) | 91.337281 | **91.337281** |
| `render` size | 9700 | 9700 |
| normalised insns (ours/orig) | 3635 / 3626 | 3635 / 3626 |
| byte_exact / linked_exact | 4505 / 4505 | 4505 / 4505 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |
| ELF section count / file size | 27 / 2,187,328 | same |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.
`src/game/ui/HangarWindow.cpp` is byte-identical to HEAD; worktree clean.
No section, base, size or `.ARM.extab` change.

## New analysis: where the 4-byte literal-pool shift comes from

The mid-function literal pool sits at fn-relative **`0x19a4` in the original and
`0x19a8` in ours** (+4).  I enumerated *all* pools in the function with
`verify._pool_bytes`:

```
orig: 0x33c 0x594 0x8f0 0xb94 0xfc4 0x137c 0x1728 0x19a4 0x1da8 ...
ours: 0x33c 0x594 0x8f0 0xb94 0xfc4 0x137c 0x1728 0x19a8 0x1d90 ...
```

Only the `0x19a4` pool (the buy-credits block) is shifted; every earlier pool is
already exact.  The pool is 4-byte aligned: the original's pre-pool code ends
exactly at `0x19a4` (its last instruction is the 2-byte `b.n` at `0x19a2`),
while ours ends at `0x19a6` and is padded with one 2-byte `nop` (`bf00`) to
`0x19a8`.

The 2-byte pre-pool excess of ours can be traced to identifiable items:

| site (fn-rel) | orig | ours | delta |
|---|---|---|---:|
| amount block `0x574` | `ldr r6,[sp,#136]` + `ldr r5,[sp,#132]` | `ldrd r5,r6,[sp,#132]` | 0 |
| amount block `0x58a` | `ldr r0,[r4,#16]` (r4 = item) | `ldr r0,[sp,#120]` + `ldr r0,[r0,#16]` | +2 |
| buy preheader `0x183c` | — | `mov r8,sl` | +2 |
| viewMode block `0x176e` | — | `str r0,[sp,#312]` (`volatile int pV = 0;`) | +2 |
| ship branch `0xa4c` | `cbz r0` (2 B) | `cmp r0,#0` + `beq.w` (6 B) | +4 |
| ship branch `0xa64` | extra `ldrd r5,r4,[sp,#132]` | — | -4 |

Net +2 (the `nop` then rounds the pool to +4).  **Removing `volatile int pV =
0;` alone does not move the pool** (measured: still `0x19a8`): the allocator
re-grows the pre-pool code by 2 bytes elsewhere, so the `nop` padding survives.
To reach `0x19a4` one would have to remove 2 bytes *and* keep the allocator from
re-growing, and both remaining candidates (`mov r8,sl` and the amount-block
`item` reload) are register-allocation artifacts, not source-expressible.

`pV` is a **2-byte** store (`904e` = `str r0,[sp,#312]`), not 4; the earlier note
that attributed the +4 pool shift to it is only correct through alignment.

## Residual map (unchanged from w6839)

| region (fn-rel) | diff elems | what |
|---|---:|---|
| `0x570`-`0x592` | 15 | amount block: orig keeps `item` in `r4` across `getStationAmount`; we rematerialise from `sp+120` |
| `0x602`-`0x768` | 68 | blueprint block: `ldrd sl,r9` vs `ldrd r9,sl` plus pure scheduling |
| `0xa48`-`0xae8` | 38 | ship branch: `cbz` vs `cmp/beq.w` (SimplifyCFG threads the `tab==0` edge past the shared `tab==1` test) |
| `0xbb4`-`0xbd2` | 6 | else-arm `item` reload |
| `0x1700`-`0x1b00` | 129 | buy-credits: `&Globals::layout` in `sl` (orig) vs `r8` (ours); `mov r8,sl` / `mov sl,r8` at the block boundaries; `String` temp address in `r8` (orig) vs `sl` (ours) |
| `0x1c00`-`0x1d00` | 182 | free-credits block: `getWidth`/`getHeight` reversed; `width`/`rowHeight` spilled instead of `sl`/`r9`; `volatile float textYf` round-trip |
| `0x1e00`-`0x2200` | 127 | non-listMode buy block: `&Globals::layout` in `r8` (orig) vs `r5` (ours) — a pure 2-register swap |
| `0x2400`-`0x25e4` | 64 | EH cleanup dispatch table `b.n` shifts |

Call stream identical (258 `blx`) except free-credits `getWidth`/`getHeight`
order.

## Measurements this wave (baseline 91.337281 / 9700 B / 3626:3635)

All were built with `link_so` and scored with the exact `verify.py` pipeline
(fresh `Elf32`/`Resolver` per measurement).

### Byte-identical to baseline (real C++ simplifications, no codegen change)

`String description(""), price("");` (one declaration), `String description =
String("");`, `PaintCanvas *canvas = Globals::Canvas;` cached in the
free-credits block, `Layout *lay = Globals::layout;` in the non-listMode block,
`if (visible == true)`, `if (tab)` for `if (tab != 0)`,
`if (this->buyCreditsActive != 0)`, `if (this->listModeFlag != 0)`,
`if (this->dialogActive == 0)`, `if (items)`, `ListItem *item =
items->data()[i];`, `ListItem *const item`, `int tab = (int) ...getCurrentTab()`,
`int iconSpace = 0;`, `int textX = iconSpace + ...`, `if (0 < barHeight)`,
`unsigned int row = 0`, `int right = 0, left = 0;`, `int left, right;`,
`int baseY`/`int baseX` swapped, `int row; row = 0;`,
`int baseX, baseY;` then assignments, `int padS;` at the top of either credit
block, `HangarAppData *data` before/after `int count`, `volatile int pV =
this->viewMode;`.

### Free-credits block orderings (all worse)

| variant | pct | size |
|---|---:|---:|
| `width, rowHeight, height, SetColor, textX, textYf, textY` | 90.778773 | 9700 |
| same, `textX` after `textY` | 90.778773 | 9700 |
| `width, rowHeight, height, textX, SetColor, ...` | 90.778773 | 9700 |
| `width, height, SetColor, textX(inline layout), ...` | 88.769464 | 9704 |
| `width, height, textX(inline), SetColor, ...` | 90.751206 | 9700 |
| `width, height, SetColor, textYf, textY, textX(inline)` | 88.769464 | 9704 |
| `width, height, SetColor, textX(inline), plain textYf` | 88.166414 | 9704 |
| `width, rowHeight, height, SetColor, textX, plain textYf` | 90.254997 | 9700 |
| `width, height, SetColor, textX(inline), int textY = ...` | 88.166414 | 9704 |
| `width, height, SetColor, textX = rowHeight+baseX+width` | 88.769464 | 9704 |
| `rowHeight, width, height, SetColor, textX, textYf, textY` | 90.738699 | 9700 |
| same, `textX` after `textY` | 90.738699 | 9700 |
| `lay, rowHeight, width, height, ...` | 90.738699 | 9700 |
| `width, height, rowHeight, SetColor, textX, ...` | 90.751206 | 9700 |
| `lay, width, height, textX(lay), ...` (keep `lay`) | 90.751206 | 9700 |
| `width, lay, height, textX(lay), ...` | 90.751206 | 9700 |
| `width, height, rowHeight, SetColor, textX, ...` | 90.751206 | 9700 |
| `int textY2 = textY + layout->field_0x4;` used twice | 88.273391 | 9708 |
| `unsigned int font = hangarFont();` cached | 91.309737 | 9700 |
| `visible = !(i == 4 && options[76])` | 91.254648 | 9700 |
| `if (i)` for `if (i != 0)` | 91.337281 | 9700 |
| `if (i > 0)` | 90.891553 | 9728 |
| `i == 0 ? 3400 : i + 112` ternary | 90.891553 | 9728 |
| `setVisible` after the `y` computation | 88.512397 | 9696 |

### Anchors (all load-bearing)

| variant | pct | size |
|---|---:|---:|
| baseline (pV + pad3 + volatile textYf) | **91.337281** | 9700 |
| `pV` removed | 91.144470 | 9700 |
| `pad3` removed | 88.882766 | 9696 |
| `volatile` removed from `textYf` | 90.214994 | 9700 |
| `int textY = (int)((float)y + (float)height*0.2f)` (no textYf) | 90.214994 | 9700 |
| `pV` + `pad3` removed | 88.689902 | 9696 |
| `pV` + `volatile` removed | 90.022051 | 9700 |
| `pad3` + `volatile` removed | 87.758478 | 9696 |
| all three removed | 87.565481 | 9696 |
| `pV` moved after `this->render()` | 91.254648 | 9700 |
| `volatile int textYi = textY;` instead of `volatile float textYf` | 91.282193 | 9700 |
| `volatile int pV = this->viewMode;` | 91.337281 | 9700 |

### Buy block / ship branch / misc

| variant | pct | size |
|---|---:|---:|
| `String description(""), price("");` | 91.337281 | 9700 |
| `String description = String("");` | 91.337281 | 9700 |
| `String price("")` declared before `description` | 91.144470 | 9700 |
| `String` declarations after `setVisible(true)` | 89.296046 | 9696 |
| `if (tab > 0)` for `if (tab != 0)` | 91.199559 | 9700 |
| `if (tab == 1 && this->upgradeMode)` (swapped &&) | 91.282193 | 9700 |
| `int i` declared in the `for` | 91.282193 | 9700 |
| `barHeight` declared before `barPos` | 91.199559 | 9700 |
| `int top = Globals::layout->field_0x20_top;` before the buy loop | 88.261229 | 9696 |
| ... same + `pV` removed | 88.151006 | 9696 |
| `Layout *lay` before the buy loop | 87.820336 | 9696 |
| non-listMode `setVisible` before `baseY` | 90.523416 | 9700 |
| `baseY = rowHeight*5 + windowTopInset` (reordered) | 91.199559 | 9700 |

### Dead-store anchor sweep

`volatile int padV = 0;` inserted after every line 950-1084 (the non-listMode
buy block, the buy-offer block and the whole free-credits block): best
**90.198238** (line 987).  Nothing beat 91.337281.  Lines 1073-1082 are
byte-identical to baseline (the store is optimised away or lands in dead code).

## Conclusion

`render` remains a strong local optimum.  The residual is the global
callee-saved register assignment (`&Globals::layout` wants `sl` in the
blueprint/buy blocks and `r8` in the non-listMode block; the `String`
temporary address wants `r8` in the buy loop; `&this->buttons` wants `r8` in the
tail), plus the branch-folder's `cbz` -> `cmp/beq.w` fold on the ship branch and
the free-credits `getWidth`/`getHeight` order.

The one *measured* new fact this wave is the exact decomposition of the
`0x19a4` pool shift: three 2-byte items, of which only `pV` is
source-controllable, and removing it is absorbed by the 4-byte pool alignment.
The three `volatile` anchors (`pV`, `pad3`, `textYf`) are all load-bearing — every
subset removal loses 0.19-3.8 points.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```
