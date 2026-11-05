# HangarWindow::render — wave 117 (w6512): local-optimum confirmation, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), real
address `0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6512`, branch `wave117-w6512`.

Builds on `_work/reconstructions/hangarwindow-render-wave116-w6412.md`
(91.199559 landed). This pass: **91.199559 -> 91.199559** (no improvement
found). The deliverable is this measurement record.

## Result

| metric | before (HEAD c22678d64) | landed |
|---|---:|---:|
| `render` fuzzy (raw difflib) | 91.199559 | **91.199559** |
| `render` size | 9700 | 9700 |
| normalised insns | 3626 / 3635 | 3626 / 3635 |
| difflib ops / changed tokens | 226 / 639 | 226 / 639 |
| `.text` | `0x1337c8` @ `0x69440` | same |
| `.ARM.extab` | `0x26294` @ `0x1a3b78` | same |
| ELF section count | 27 | 27 |
| file size | 2,187,328 | 2,187,328 |
| byte_exact / linked_exact | 4501 / 4501 | 4501 / 4501 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

No ELF section size, base or count changed. `.ARM.extab` stays 4 B short of the
original (`0x26294` vs `0x26298`), as at the start of the pass. Gate:
**RATCHET PASS**.

## Where the residual actually is (address-mapped)

The diff was re-derived with a resolver that keeps the address of every
normalised entry (so pool words and jump tables are mapped correctly). The
residual is **four** compact regions, not a uniform smear:

| region (function-relative) | absolute | what |
|---|---|---|
| `0x570`-`0x58c` | `0x149648`-`0x149664` | `item`/`this`/`layout` materialisation in the `tab == 1` selected-item block |
| `0x602`-`0x768` | `0x1496da`-`0x149840` | `ldrd sl,r9` vs `ldrd r9,sl` (layout/this swap) in the blueprint branch |
| `0xa48`-`0xae8` | `0x149b20`-`0x149bc0` | `ldrd r5,r4` vs `ldrd r6,r5` + `cbz` vs `cmp/beq.w` in the ship branch |
| `0xbb4`-`0xbd4` | `0x149c8c`-`0x149cac` | else-arm of the `tab == 1` block (`item` reload) |
| `0x1766`-`0x1a2a` | `0x14a83e`-`0x14ab02` | `pV` store (`str r0,[sp,#312]`, 2 B) shifts every later branch target |
| `0x1b0e`-`0x1e60` | `0x14abe6`-`0x14af38` | buy-credits tail + free-credits block |

**Everything between `0x149cac` and `0x14a83e` (2962 bytes: the whole rest of
the list loop, the scrollbar, `drawHeader`, the tab-button loop) is
instruction-identical.** The residual is not a source-shape error; it is four
allocator/scheduling decisions plus the literal-pool placement they cause.

### The `item`-in-`r4` decision is the key to the `tab == 1` block

Original (`0x149648`):

```
149648: ldr r4,[sp,#120]   ; item   (4 B, high reg)
14964a: ldr r1,[sp,#108]   ; tab
14964c: ldr r6,[sp,#136]   ; this
14964e: ldr r0,[r4,#16]    ; item->item
149650: cmp r1,#1
149652: bne.w 149c88
149656: blx getStationAmount
14965a: ldr r5,[sp,#132]   ; layout (lazy, after the call)
...
```

Ours:

```
149648: ldr r0,[sp,#120]   ; item   (2 B)
14964a: ldr r1,[sp,#108]
14964c: ldr r0,[r0,#16]
14964e: cmp r1,#1
149650: ldrd r5,r6,[sp,#132]  ; layout+this together, BEFORE the branch
149654: bne.w 149c88
149658: blx getStationAmount
14965c: ldr.w r8,[sp,#140]
149660: mov r9,r0
149662: ldr r0,[sp,#120]   ; item reloaded
149664: ldr r0,[r0,#16]
```

The original keeps `item` in callee-saved `r4` across the two calls; ours
spills it and reloads. This is a 2-byte difference in the then-arm that is
exactly cancelled by the `volatile int pad3 = left;` store at `0x149caa`
(ours) which the original does not have. **Removing `pad3` alone is worse
(88.855214) precisely because it un-aligns the 2962-byte identical region**;
`pad3` is a byte-count compensation for the missing `ldr r4`, not an
allocation anchor in its own right. If `item` ever lands in `r4`, `pad3`
should be deleted in the same change.

### The `pV` store is a 2-byte tax on the whole second half

`volatile int pV = 0;` in the `viewMode == 1` re-entrant block emits
`movs r0,#0; str r0,[sp,#312]` at `0x14a846`; the original has only the
`movs`. That 2-byte store shifts every branch target from `0x14a85a` to the
end of the function. It is nevertheless load-bearing: dropping `pV` gives
91.006748 (the buy-credits `r8`/`sl` roles flip). Any future fix must remove
the store *without* changing the allocation.

## What was tried this wave (all measured with the real build + verify)

Base = 91.199559 unless noted. Every row keeps 9700 B / `.text 0x1337c8` /
`.ARM.extab 0x26294` unless a different size is shown.

### Free-credits block (getWidth/getHeight order)

The original calls `getWidth` first, then reads
`layout->field_0x2c_rowHeight`, then calls `getHeight`
(`0x14ad28`/`0x14ad32`/`0x14ad38`). The landed source deliberately calls
`getHeight` first, which is what keeps the global allocation.

| variant | fuzzy | size | insns |
|---|---:|---:|---:|
| `int textX = getWidth() + baseX + lay->rowHeight; int height = getHeight();` (with `lay`) | 76.937904 | 9700 | 3626:3637 |
| same, `Globals::layout` direct (no `lay`) | 77.494838 | 9700 | 3626:3639 |
| `int width = getWidth(); int height = getHeight(); int textX = width + ...` | 76.480308 | 9696 | 3626:3636 |
| `int width = getWidth(); int textX = width + ...; int height = getHeight();` | 76.937904 | 9700 | 3626:3637 |
| faithful order + `volatile int height` | 76.145905 | 9704 | 3626:3639 |
| faithful order + `volatile int width` | 76.190476 | 9712 | 3626:3640 |
| faithful order + `volatile int textX` | 65.156507 | 9692 | 3626:3658 |
| faithful order + non-volatile `textY` | 76.569383 | 9700 | 3626:3638 |
| drop `volatile` from `textYf` (landed order) | 76.012118 | 9696 | 3626:3636 |
| `volatile double textYf` | 74.056734 | 9704 | 3626:3636 |
| all 11 valid permutations of `{lay,height,textX,SetColor}` | 76.33-90.72 | 9696-9704 | — |
| `int textX = baseX + getWidth() + lay->rowHeight;` | 91.199559 | 9700 | 3626:3635 |
| `int textX = lay->rowHeight + getWidth() + baseX;` | 91.199559 | 9700 | 3626:3635 |
| drop `lay`, keep landed order | 90.806340 | 9700 | 3626:3629 |
| `height` after `SetColor` | 76.325575 | 9696 | 3626:3635 |
| `textYf` before `SetColor` | 88.392611 | 9696 | 3626:3628 |
| `Layout &lay = *Globals::layout;` | 91.199559 | 9700 | 3626:3635 |
| `const int width`, `volatile int width` | 91.199559 / 89.026573 | 9700 / 9704 | — |

### Selected-item `tab == 1` block

| variant | fuzzy | size |
|---|---:|---:|
| `ListItem *sel = item;` then `sel->item->...` in both arms | 91.199559 | 9700 |
| `sel` only in the then-arm | 91.199559 | 9700 |
| `sel` only in the else-arm | 85.344353 | 9696 |
| `Item *it = item->item;` then `it->...` | 62.351969 | 9680 |
| `int left, right;` (decl order swap) / separate decls | 91.199559 | 9700 |
| `int right = 0, left = 0;` | 91.199559 | 9700 |
| `volatile int *anchor = 0;` before the if | 91.199559 | 9700 |
| `anchor` after the if/else | 85.273454 | 9696 |
| `int right, left;` hoisted above the outer `if` | 65.252357 | 9620 |
| `left`/`right` assignments swapped in then-arm | 86.111493 | 9708 |
| else-arm reordered (`left` then `right`) | 67.962503 | 9712 |
| else-arm via `this->selectedItem` | 87.346826 | 9704 |
| `HangarWindow *self = this;` in else-arm | 85.344353 | 9696 |
| inner `item->isItem()` -> `this->selectedItem->isItem()` | 91.089382 | 9700 |
| outer `this->selectedItem == item` | 91.172015 | 9700 |
| `int curTab = hangarList->getCurrentTab();` | 55.509642 | 9656 |
| `ListItem *item` -> `ListItem *const item` | 91.199559 | 9700 |

### `pad3` anchor (selected-item `left > 0`)

| variant | fuzzy | size |
|---|---:|---:|
| `volatile int pad3 = left;` (landed) | 91.199559 | 9700 |
| `pad3 = left + 0` / `volatile unsigned int pad3` / `pad3 = pad3;` | 91.199559 | 9700 |
| drop `pad3` | 88.855214 | 9696 |
| `int pad3 = left;` (non-volatile) | 90.786393 | 9700 |
| `volatile int *pad3 = &left;` | 90.786393 | 9700 |
| `pad3 = right` | 87.028367 | 9704 |
| `pad3` before the `if` | 88.417573 | 9700 |
| `pad3` after `setVisible` / after `draw` | 86.863123 / 86.942149 | 9708 / 9704 |
| `pad3` in the `right > 0` block / after `buttonOffset` | 86.925406 / 61.186837 | 9716 / 9684 |

### `pV` anchor (`viewMode == 1` re-entrant block)

| variant | fuzzy | size |
|---|---:|---:|
| `volatile int pV = 0;` (landed) | 91.199559 | 9700 |
| drop `pV` | 91.006748 | 9700 |
| `volatile int pV;` (no init) / `volatile int *pV = 0;` | 91.006748 | 9700 |
| `volatile char/short pV = 0;` | 88.983751 | 9704 |
| `pV` before `viewMode = 0` | 91.199559 | 9700 |
| `pV` after `render()` | 91.116926 | 9700 |
| `pV` after `viewMode = 1` | 91.061837 | 9700 |
| `volatile int pV = this->viewMode;` / `pV = pV;` / `volatile unsigned int` | 91.199559 | 9700 |
| `if (pV) {}` after `render()` | 88.943963 | 9704 |

### Other anchors / declarations

| variant | fuzzy | size |
|---|---:|---:|
| `volatile int *padX` -> `int *padX` / `nullptr` / `(int*)0` / `volatile long*` / `volatile char*` / `&this->viewMode` | 91.199559 | 9700 |
| `volatile int padX = 0;` | 74.697137 | 9724 |
| `padX` moved after `if (tab != 0)` | 86.091986 | 9696 |
| `volatile int *pad0` -> `int *pad0` | 91.199559 | 9700 |
| `int *volatile pad0` | 87.606720 | 9704 |
| `volatile int *anchor = 0;` at function top / items / loop body / buy-credits / free-credits / after loop | 91.199559 | 9700 |
| same anchor in ship branch / item branch / blueprint branch / selected block | 84.429734 / 64.888766 / 86.458190 / 89.409172 | — |
| `if (station == nullptr)` -> `!station` | 91.199559 | 9700 |
| `items` before `tab` | 78.009100 | 9664 |
| `GetApplicationModule` before `SetColor` | 85.336638 | 9704 |
| `viewMode` read into a local first | 85.501859 | 9708 |
| `for (i < items->count)` / `ListItem *const item` | 91.199559 | 9700 |
| `int rowH = layout->field_0x70_rowHeight;` before `y` | 91.199559 | 9700 |
| `baseY` hoisted out of the loop | 65.195808 | 9660 |
| `item` before `y` | 91.199559 | 9700 |
| `float rate = getCompletionRate(); if (rate > 0.0f)` | 86.037216 | 9692 |
| `const float rate` | 91.199559 | 9700 |
| `BluePrint *bp = item->bluePrint;` | 86.265326 | 9704 |
| `Layout *lay = Globals::layout;` in the blueprint branch | 85.170893 | 9704 |
| `int pbarW = this->progressBarWidth;` before the rate if | 86.458190 | 9704 |
| `!(tab == 3 && selectedItem->ship == getShip())` -> `tab != 3 \|\| ...` | 91.199559 | 9700 |
| same, `item->ship` instead of `selectedItem->ship` | 87.670478 | 9696 |
| `if (this->upgradeMode && tab == 1)` block moved inside `if (tab != 0)` | 91.199559 | 9700 |

No variant measured above 91.199559.

## Conclusion

The landed wave-114/115/116 source is a strong local optimum. The residual is
not a source-shape error; it is:

1. `item` spilled instead of kept in `r4` at `0x149648` (compensated for by the
   `pad3` store at `0x149caa`), which also drags `layout`/`this` into one
   `ldrd` before the branch instead of two lazy loads.
2. `ldrd sl,r9` vs `ldrd r9,sl` at `0x1496da` (`&Globals::layout`/`this`),
   which propagates through the blueprint branch.
3. `ldrd r5,r4` vs `ldrd r6,r5` and `cbz` vs `cmp`/`beq.w` at `0x149b20`
   (jump threading of the `tab == 0` edge).
4. The `pV` 2-byte store at `0x14a846`, which shifts every later branch target.
5. The free-credits `getWidth`-before-`getHeight` call order: making it
   faithful re-allocates the whole function and costs ~15 points.

Every relocation/removal of the five `volatile` anchors measured worse.
No source change measured above 91.199559, so no code change was landed; the
gate counts are unchanged (byte 4501 / linked 4501, data 843/843, imports 207,
lint CLEAN, drift 0, 27 sections, `.ARM.extab 0x26294`, file size 2,187,328).
