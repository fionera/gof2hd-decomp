# MenuTouchWindow::OnTouchBegin pass 3 — w4203

Date: 2026-11-03. Branch `wave92-w4203` (base master `b379d00d`).
Gate before/after: **byte_exact 4498 / linked_exact 4498 / size-exact 4524**,
data_exact 843/843, imports 207, `.text` `0x1337c8`, `.rodata` `0x41b67`,
`.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26290` (-8), lint `CLEAN ✓`, drift 0.
**No code change landed**; this note records the third source-shape search.

Prior notes (read first):

* `_work/reconstructions/menutouchwindow-ontouchbegin-97-7-modstation-c1-c2-98-6_w3819.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-modstation-pass2-w4019.md`
* `_work/reconstructions/menutouchwindow-ontouchend-case12-leaderboard_w4017.md`
* `_work/reconstructions/menutouchwindow-c1-c2-pass3-value-select-w4016.md`

## Current state (re-measured)

| symbol | size orig/ours | fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | 97.7 % | **27** `\|` lines (26 instruction pairs + header) |

The function is size-exact and semantically complete: no missing code, wrong
constant, wrong field offset or wrong callee.  The whole residual is clang
7.0.2 `-Oz` register allocation / instruction scheduling inside two small
regions.  There is **no `volatile`, `__builtin_expect`, `__builtin_prefetch`,
padding local or size anchor inside `OnTouchBegin`** (the file-level
`__builtin_expect`s at lines ~2889/2916/3022 belong to `OnTouchEnd`; the
prefetches at lines 618–721 belong to the constructor).  Nothing to remove.

## The two residual regions

Addresses are the original VMA (ours is at the same VMA because the function
is size-exact and the section order is pinned).

### 1. Case 8 (`upButtonPressed` / `downButtonPressed`), `0x129fd2`

```
orig  129fd2: f8da c154   ldr.w  ip, [sl, #340]   ; listEntryWidth
      129fd6: 442b        add    r3, r5
      129fd8: 4463        add    r3, ip
ours  129fd2: f8da 4154   ldr.w  r4, [sl, #340]
      129fd6: 442b        add    r3, r5
      129fd8: 4423        add    r3, r4
```

**2 differing bytes:** `0x129fd5 c1→41`, `0x129fd8 63→23`.  `r4` is not used
anywhere in the original's case-8 block; the original allocator still picks
`ip` (r12) for the short-lived `field_0x154` temporary, while ours picks the
callee-saved `r4`.  Pure register-choice tie-break, not a source difference.  Re-ordering the
case-8 condition (`left+top+width`, `top+left+width`, `width+left+top`,
`left+width+top`, `left+(top+width)`, `top+width+left`) and introducing
`listEntryWidth` / `listTopY` locals were all measured: 27 or 30 diff lines,
never better.

### 2. Case 15/16 (`scrollbarHit` + drag init), `0x12a1da..0x12a250`

Original (full block; `r4` was loaded from `[sp,#16]` in the loop preheader at
`0x12a1bc`, `[sp,#16]` holds `&Globals::layout`, `fp` holds `&Globals::Canvas`):

```
12a1da: 484f         ldr    r0, [pc, #316]      ; GOT &Globals::w
12a1dc: 494f         ldr    r1, [pc, #316]      ; GOT &Globals::Canvas
12a1de: 4478         add    r0, pc
12a1e0: f8da 3138    ldr.w  r3, [sl, #312]    ; this->heapBufA
12a1e4: 4479         add    r1, pc
12a1e6: f8da 61e0    ldr.w  r6, [sl, #480]    ; this->field_0x1e0
12a1ea: 6800         ldr    r0, [r0, #0]      ; &Globals::w
12a1ec: f8d1 b000    ldr.w  fp, [r1]           ; &Globals::Canvas
12a1f0: 6822         ldr    r2, [r4, #0]      ; Globals::layout
12a1f2: f853 1026    ldr.w  r1, [r3, r6, lsl #2] ; heapBufB[field_0x1e0]
12a1f6: 6805         ldr    r5, [r0, #0]      ; Globals::w
12a1f8: f8db 0000    ldr.w  r0, [fp]          ; Globals::Canvas
12a1fc: 6a92         ldr    r2, [r2, #40]     ; layout->buttonInsetX
12a1fe: 1aad         subs   r5, r5, r2        ; w - buttonInsetX
12a200: f738 edc0    blx    GetImage2DWidth
12a204: 1a28         subs   r0, r5, r0        ; (w - inset) - width
12a206: 4540         cmp    r0, r8            ; x
12a208: da13         bge.n  12a232
12a20a: f8da 0138    ldr.w  r0, [sl, #312]
12a20e: f8da 11e0    ldr.w  r1, [sl, #480]
12a212: 9a04         ldr    r2, [sp, #16]     ; &Globals::layout (reload)
12a214: f850 1021    ldr.w  r1, [r0, r1, lsl #2]
12a218: 6812         ldr    r2, [r2, #0]      ; Globals::layout
12a21a: f8db 0000    ldr.w  r0, [fp]          ; Globals::Canvas
12a21e: 68d5         ldr    r5, [r2, #12]     ; layout->field_0xc_leftMargin
12a220: 6a16         ldr    r6, [r2, #32]     ; layout->field_0x20_top
12a222: f738 edb6    blx    GetImage2DHeight
12a226: 1971         adds   r1, r6, r5        ; top + leftMargin
12a228: 4408         add    r0, r1            ; height + top + leftMargin
12a22a: 4548         cmp    r0, r9            ; y
12a22c: dd01         ble.n  12a232
12a22e: 2001         movs   r0, #1
12a230: e000         b.n    12a234
12a232: 2000         movs   r0, #0
12a234: f8da 1134    ldr.w  r1, [sl, #308]    ; this->heapBufA
12a238: f88a 01d9    strb.w r0, [sl, #473]    ; this->scrollbarHit
12a23c: 9804         ldr    r0, [sp, #16]     ; &Globals::layout (reload)
12a23e: 6809         ldr    r1, [r1, #0]      ; heapBufA[0]
12a240: 6802         ldr    r2, [r0, #0]      ; Globals::layout
12a242: f8db 0000    ldr.w  r0, [fp]          ; Globals::Canvas
12a246: 6a95         ldr    r5, [r2, #40]     ; layout->buttonInsetX
12a248: f738 ed9c    blx    GetImage2DWidth
12a24c: 4428         add    r0, r5
12a24e: 4540         cmp    r0, r8
12a250: dd31         ble.n  12a2b6
```

Ours (current best source, 27 diff lines):

```
12a1da: 484f         ldr    r0, [pc, #316]
12a1dc: 494f         ldr    r1, [pc, #316]
12a1de: 4478         add    r0, pc
12a1e0: 6826         ldr    r6, [r4, #0]      ; Globals::layout (cached in r6)
12a1e2: 4479         add    r1, pc
12a1e4: f8da 2138    ldr.w  r2, [sl, #312]
12a1e8: 6800         ldr    r0, [r0, #0]
12a1ea: f8da 31e0    ldr.w  r3, [sl, #480]
12a1ee: f8d1 b000    ldr.w  fp, [r1]           ; &Globals::Canvas
12a1f2: 6804         ldr    r4, [r0, #0]      ; Globals::w
12a1f4: 6a30         ldr    r0, [r6, #32]     ; layout->field_0x20_top (hoisted)
12a1f6: f852 1023    ldr.w  r1, [r2, r3, lsl #2]
12a1fa: 9003         str    r0, [sp, #12]     ; spill top
12a1fc: f8db 0000    ldr.w  r0, [fp]
12a200: 6ab5         ldr    r5, [r6, #40]     ; layout->buttonInsetX
12a202: 1b64         subs   r4, r4, r5
12a204: f738 edbe    blx    GetImage2DWidth
12a208: 1a20         subs   r0, r4, r0
12a20a: 4540         cmp    r0, r8
12a20c: da11         bge.n  12a232
12a20e: f8da 0138    ldr.w  r0, [sl, #312]
12a212: f8da 11e0    ldr.w  r1, [sl, #480]
12a216: f850 1021    ldr.w  r1, [r0, r1, lsl #2]
12a21a: f8db 0000    ldr.w  r0, [fp]
12a21e: f738 edb8    blx    GetImage2DHeight
12a222: 9a03         ldr    r2, [sp, #12]     ; top
12a224: 68f1         ldr    r1, [r6, #12]     ; leftMargin (cached layout)
12a226: 4410         add    r0, r2
12a228: 4408         add    r0, r1
12a22a: 4548         cmp    r0, r9
12a22c: dd01         ble.n  12a232
12a22e: 2001         movs   r0, #1
12a230: e000         b.n    12a234
12a232: 2000         movs   r0, #0
12a234: f8da 1134    ldr.w  r1, [sl, #308]
12a238: f88a 01d9    strb.w r0, [sl, #473]
12a23c: 9804         ldr    r0, [sp, #16]     ; &Globals::layout (reload)
12a23e: 6809         ldr    r1, [r1, #0]
12a240: 6802         ldr    r2, [r0, #0]
12a242: f8db 0000    ldr.w  r0, [fp]
12a246: 6a94         ldr    r4, [r2, #40]
12a248: f738 ed9c    blx    GetImage2DWidth
12a24c: 4420         add    r0, r4
12a24e: 4540         cmp    r0, r8
12a250: dd31         ble.n  12a2b6
```

The third `if` (`0x12a23c..0x12a250`) is **byte-identical** to the original
except for the final temporary register (`r5` vs `r4`).  The first `if` and
the `GetImage2DHeight` block are the residual.

**68 differing bytes** (raw) span `0x12a1e0..0x12a24c`.  They are not 68
independent mistakes: the instruction *boundaries* shift because our first
`if` has a different instruction order, so the raw diff is not a useful
metric.  The verifier's `--show` output prints **27 `|` lines**: 1 header
line + 2 case-8 instruction lines + 24 case-15 instruction lines.  The 24
case-15 pairs are:

| # | original | ours | class |
|---|---|---|---|
| 1 | `ldr.w r3,[sl,#312]` | `ldr r6,[r4,#0]` | layout value cached in r6 |
| 2 | `ldr.w r6,[sl,#480]` | `ldr.w r2,[sl,#312]` | heapBufA register |
| 3 | `ldr.w fp,[r1]` | `ldr.w r3,[sl,#480]` | field_0x1e0 register |
| 4 | `ldr r2,[r4,#0]` | `ldr.w fp,[r1]` | load order |
| 5 | `ldr.w r1,[r3,r6,lsl#2]` | `ldr r4,[r0,#0]` | heapBufB index / w |
| 6 | `ldr r5,[r0,#0]` | `ldr r0,[r6,#32]` | w / hoisted top |
| 7 | `ldr.w r0,[fp]` | `ldr.w r1,[r2,r3,lsl#2]` | Canvas / heapBufB |
| 8 | `ldr r2,[r2,#40]` | `str r0,[sp,#12]` | buttonInsetX / top spill |
| 9 | `subs r5,r5,r2` | `ldr.w r0,[fp]` | |
| 10 | `blx GetImage2DWidth` | `ldr r5,[r6,#40]` | |
| 11 | `subs r0,r5,r0` | `subs r4,r4,r5` | |
| 12 | `cmp r0,r8` | `blx GetImage2DWidth` | |
| 13 | `bge` | `subs r0,r4,r0` | |
| 14 | `ldr.w r0,[sl,#312]` | `cmp r0,r8` | |
| 15 | `ldr.w r1,[sl,#480]` | `bge` | |
| 16 | `ldr r2,[sp,#16]` | `ldr.w r0,[sl,#312]` | second `if` reload |
| 17 | `ldr.w r1,[r0,r1,lsl#2]` | `ldr.w r1,[sl,#480]` | |
| 18 | `ldr r2,[r2,#0]` | `ldr.w r1,[r0,r1,lsl#2]` | |
| 19 | `ldr r5,[r2,#12]` | `blx GetImage2DHeight` | |
| 20 | `ldr r6,[r2,#32]` | `ldr r2,[sp,#12]` | leftMargin source |
| 21 | `blx GetImage2DHeight` | `ldr r1,[r6,#12]` | top source |
| 22 | `adds r1,r6,r5` | `add r0,r2` | |
| 23 | `ldr r5,[r2,#40]` | `ldr r4,[r2,#40]` | third `if` temp |
| 24 | `add r0,r5` | `add r0,r4` | third `if` temp |

(Rows 23–24 are the only differences in the third `if`; the third `if` is
otherwise byte-identical.  The 2 case-8 lines are the `ip`/`r4` tie-break
above.)

## Ghidra decompilation of the block

`decompile_function` at Ghidra address `0x139bd4` (image base 0x10000) gives
the faithful direct shape (case `0xf`/`0x10`):

```c
puVar19 = *(undefined4 **)(DAT_0013a31c + 0x13a1e8);   /* &Globals::Canvas */
iVar17 = **(int **)(DAT_0013a318 + 0x13a1e2);          /* Globals::w */
iVar10 = *(int *)(*piVar3 + 0x28);                     /* layout->buttonInsetX */
iVar12 = PaintCanvas::GetImage2DWidth((PaintCanvas *)*puVar19,
             *(uint *)(*(int *)(param_1 + 0x138) + *(int *)(param_1 + 0x1e0) * 4));
if (((iVar17 - iVar10) - iVar12 < param_2) &&
    (iVar10 = *(int *)(*piVar3 + 0xc),                 /* layout->leftMargin */
     iVar17 = *(int *)(*piVar3 + 0x20),                /* layout->top */
     iVar12 = PaintCanvas::GetImage2DHeight((PaintCanvas *)*puVar19,
                 *(uint *)(*(int *)(param_1 + 0x138) + *(int *)(param_1 + 0x1e0) * 4)),
     (int)param_3 < iVar12 + iVar17 + iVar10)) {
  uVar1 = 1;
} else {
  uVar1 = 0;
}
*(undefined1 *)(param_1 + 0x1d9) = uVar1;              /* scrollbarHit */
iVar10 = *(int *)(*piVar3 + 0x28);                     /* layout->buttonInsetX */
iVar12 = PaintCanvas::GetImage2DWidth((PaintCanvas *)*puVar19,
             **(uint **)(param_1 + 0x134));
if (param_2 < iVar12 + iVar10) {                       /* drag init */
  *(void **)(param_1 + 0x220) = param_3;
  *(void **)(param_1 + 0x20c) = param_3;
  *(undefined4 *)(param_1 + 0x214) = 0;
  *(undefined1 *)(param_1 + 0x224) = 1;
}
```

`piVar3` is `&Globals::layout`; `*piVar3` is the `Layout *`; `puVar19` is
`&Globals::Canvas`.  So the original source is **direct `Globals::layout`
access** (three separate `*(int *)(*piVar3 + …)` loads), not a local
`Layout &layout`.

## Why the faithful direct shape is 4 bytes short

The faithful source is:

```cpp
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
    y < Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]) + Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin)
    this->scrollbarHit = 1;
else
    this->scrollbarHit = 0;
if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) { … }
```

It compiles to **1892 B / 165 `|` lines**.  The 4-byte shortfall is exactly
the `ldr r2,[sp,#16]` that the original executes before the second
`Globals::layout` access: with direct globals clang 7.0.2 keeps
`&Globals::layout` in `fp` across the `GetImage2DWidth` call, so the second
access is one `ldr r2,[fp]` instead of `ldr r2,[sp,#16]; ldr r2,[r2,#0]`.
The original keeps `&Globals::Canvas` in `fp` and treats the layout address
as a rematerialisable spill (`[sp,#16]`, already live from the
`choiceWindowOpen` check at `0x129bf8..0x129c14`).  This is a
register-allocation choice, not a source-semantics difference.

The current best shape (27 lines) is a *local* `Layout &layout` plus a
hoisted `int top`; it keeps the layout **value** in `r6` and spills `top` to
`[sp,#12]`.  It is a local optimum: the direct shape is faithful but 4 bytes
short and re-colours the whole block.

## Source-shape sweep (all measured with `tools/verify/verify.py --show`, `autojunk=False`)

The block was replaced by each variant below, the whole `.so` re-linked, and
the function re-verified.  "diffs" is the number of `|` structural-diff lines.

| variant | size | diffs | note |
|---|---:|---:|---|
| **current** (`Layout&` + hoisted `top`) | 1896 | **27** | best |
| direct `Globals::layout` (top+left) | 1892 | 165 | faithful, 4 B short |
| direct `Globals::layout` (left+top) | 1892 | 166 | |
| direct, parenthesised `w - (inset + width)` | 1892 | 173 | |
| direct, `w - width - inset` | 1880 | 172 | |
| `Layout *layout = Globals::layout` + hoisted `top` | 1896 | 27 | same codegen as current |
| `Layout *const layout` + hoisted `top` | 1896 | 27 | |
| `const Layout &layout` + hoisted `top` | 1896 | 27 | |
| `int top` only, direct layout fields | 1896 | 67 | no cache, fp/Canvas swapped |
| `Layout **layout = &Globals::layout` | 1892 | 166 | |
| `Layout *&layout = Globals::layout` | 1892 | 166 | |
| `Layout *const &layout` | 1892 | 166 | |
| `Layout *volatile &layout` | 1896 | 138 | forces reload, volatile field loads |
| `Layout *volatile layout` | 1928 | 763 | local volatile pointer |
| `volatile Layout &layout` | 1884 | 174 | volatile field loads |
| `bool hit = cond; scrollbarHit = hit` | 1892 | 165 | |
| `scrollbarHit = cond1 && cond2` | 1892 | 165 | |
| nested `if` (cond1) { if (cond2) … } | 1896 | 113 | `ite` form |
| `if (!(cond1 && cond2)) 0 else 1` | 1892 | 165 | |
| local `int w = Globals::w` | 1892 | 166 | |
| `PaintCanvas *&canvas` reference | 1892 | 166 | |
| `PaintCanvas **canvas = &Globals::Canvas` | 1892 | 166 | |
| local `Layout&` + direct second `if` (current) | 1896 | 27 | |
| local `Layout&` + local second `if` | 1892 | 161 | |
| `ref_nohoist` (`layout.field_0x20_top` inline) | 1892 | 165 | |
| `ptr_nohoist` | 1892 | 165 | |
| `Layout&` + `Globals::layout->top` + `layout.leftMargin` | 1896 | 27 | same as current |
| `Layout&` + `layout.leftMargin + top` order | 1896 | 27 | same as current |
| `Layout&` + direct top + `layout.leftMargin` | 1896 | 55 | |
| `Layout&` + direct `Globals::layout` top + `layout.leftMargin` | 1896 | 55 | |
| `Layout*const` | 1892 | 166 | |
| `int imgW = …Width(...)` first | 1880 | 172 | |
| nested-if with `Layout&` inside | 1888 | 169 | |
| `&layout != 0` trick | 1900 | 166 | |
| `PaintCanvas*&` | 1892 | 166 | |
| case reorder `1,3,4,7,8,9,11,12,14,16,15,17,6,default` | 1896 | 43 | also changes case-7 `blx` regs |
| `Layout **lp = &Globals::layout` (all accesses) | 1896 | 67 | new (w4203) |
| `Layout **lp` + `int top` + direct second/third | 1896 | 67 | new |
| `PaintCanvas *canvas` local copy + direct layout | 1892 | 166 | new |
| `PaintCanvas &canvas` local copy + direct layout | 1892 | 166 | new |
| `PaintCanvas **canvas` + direct layout | 1892 | 166 | new |
| `PaintCanvas *&canvas` + direct layout | 1892 | 166 | new |
| `int wMinusInset = w - inset` + direct layout | 1892 | 166 | new |
| `int inset = layout->buttonInsetX` + direct layout | 1892 | 166 | new |
| `int width = …Width(...)` first + direct layout | 1880 | 172 | new |
| `Layout *layout` assigned after the width call via comma | 1892 | 165 | new |
| `Layout *volatile *lp = &Globals::layout` | 1896 | 136 | new |
| `Layout *const volatile lp = Globals::layout` | 1928 | 762 | new |
| `const Layout *lp = Globals::layout` | 1888 | 167 | new |
| `Layout&` + `int top = 0` + `(top = Globals::layout->top) + layout.leftMargin + height` | 1896 | **24** | new, best diff count |
| same, top from `layout.field_0x20_top` | 1896 | 24 | new |
| same, direct first `if` | 1896 | 24 | new |
| same, both top and leftMargin direct | 1892 | 165 | new |
| same, third `if` local | 1892 | 160 | new |
| `Layout&` + `PaintCanvas *&canvas` + 24-shape | 1896 | 24 | new, same codegen |

### New: the 24-diff shape

The best diff count found this pass is **24 `|` lines** (23 instruction
pairs + header; vs 27/26 for the current shape), with the same 1896-byte size:

```cpp
{
    Layout &layout = *Globals::layout;
    int top = 0;
    if (x > Globals::w - layout.buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
        (top = Globals::layout->field_0x20_top) + layout.field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]) > y)
        this->scrollbarHit = 1;
    else
        this->scrollbarHit = 0;
    if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) { … }
}
```

It compiles to:

```
12a1e0: f8da 2138    ldr.w  r2, [sl, #312]
12a1e4: 4479         add    r1, pc
12a1e6: f8da 31e0    ldr.w  r3, [sl, #480]
12a1ea: 6800         ldr    r0, [r0, #0]
12a1ec: f8d1 b000    ldr.w  fp, [r1]           ; &Globals::Canvas  (correct!)
12a1f0: 6826         ldr    r6, [r4, #0]      ; layout value (still cached)
12a1f2: f852 1023    ldr.w  r1, [r2, r3, lsl #2]
12a1f6: 6804         ldr    r4, [r0, #0]
12a1f8: f8db 0000    ldr.w  r0, [fp]
12a1fc: 6ab5         ldr    r5, [r6, #40]
12a1fe: 1b64         subs   r4, r4, r5
12a200: f738 edc0    blx    GetImage2DWidth
12a204: 1a20         subs   r0, r4, r0
12a206: 4540         cmp    r0, r8
12a208: da13         bge.n  12a232
12a20a: 9804         ldr    r0, [sp, #16]     ; &Globals::layout (reload)
12a20c: f8da 1138    ldr.w  r1, [sl, #312]
12a210: f8da 21e0    ldr.w  r2, [sl, #480]
12a214: 6800         ldr    r0, [r0, #0]      ; Globals::layout
12a216: 68f3         ldr    r3, [r6, #12]     ; leftMargin (cached)
12a218: f851 1022    ldr.w  r1, [r1, r2, lsl #2]
12a21c: 6a06         ldr    r6, [r0, #32]     ; top (fresh)
12a21e: f8db 0000    ldr.w  r0, [fp]
12a222: 199c         adds   r4, r3, r6        ; left + top
12a224: f738 edb4    blx    GetImage2DHeight
12a228: 4420         add    r0, r4
12a22a: 4548         cmp    r0, r9
12a22c: dd01         ble.n  12a232
```

This fixes the `fp` assignment (`&Globals::Canvas`, as in the original) and
the `top + leftMargin` association, and is 1896 B — but it still caches the
layout **value** in `r6` for the first access and for `leftMargin`, so it is
not byte-exact.  It is a better *diff count* but **not a gate improvement**
(`byte_exact` is still 4498), so it is not landed.

## Flag / option sweep

All measured on the whole `.so` (per-TU `-mllvm` options appended to
`MenuTouchWindow.cpp`'s `-likely-branch-weight=5 -unlikely-branch-weight=1`):

* branch weights `3:1`, `4:1`, `6:1`, `7:1`, `10:1`, `12:1`, `20:1`,
  `80:1`: **27 diffs** with the current shape; **165** with the direct shape.
* `-regalloc-csr-first-time-cost=0..20`: no change.
* `-split-spill-mode=*`, `-enable-deferred-spilling`, `-disable-spill-hoist`,
  `-spp-rematerialization-threshold=0`, `-enable-local-reassign`: no change.
* `-stress-regalloc=8..12`: 1916–2148 B, 715+ diffs.
* `-optimize-regalloc=false`: 2932 B, 1138 diffs.
* `-sink-insts-to-avoid-spills`, `-disable-post-ra`, `-misched-postra=false`,
  `-regalloc=basic`, `-fno-strict-aliasing`, `-fno-exceptions`, `-fno-rtti`,
  `-fomit-frame-pointer`: no improvement (165–764 diffs).

This agrees with `worst_function_flag_sensitivity_w3751.md`: no flag is a
strict improvement, so the residual is source-shape-driven.

## Conclusion

* `OnTouchBegin` is source-shape complete: every value, field offset, callee
  and call argument is exact; the 27 residual lines are two independent
  allocator tie-breaks (`ip` vs `r4` in case 8; `fp`/`r6`/`[sp,#12]` vs
  `fp`/`r4`/`[sp,#16]` in case 15).
* The faithful direct shape (Ghidra) is 4 bytes short because clang keeps
  `&Globals::layout` in `fp` across the `GetImage2DWidth` call; the original
  keeps `&Globals::Canvas` in `fp` and rematerialises the layout address from
  `[sp,#16]`.  No plain C++ shape tried (local/direct, pointer/reference,
  pointer-to-pointer, volatile, const, declaration order, comma assignment,
  parenthesisation, control-flow form, 24-shape) reproduces the original's
  exact register assignment while keeping the 1896-byte size.
* The best diff count found this pass is **24** (1896 B, correct `fp`), but
  it is not byte-exact, so `byte_exact` stays 4498.  **No code change is
  landed**; this note is the deliverable.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4203
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Gate after (unchanged): `verify: byte 4498 linked 4498 stubs 0 missing 0
extra 0 avg 99.9`, `data: exact 843 of 843`, `sodiff: extra-beyond-allow 0
missing 0 allowed 0 imports 207`, `RATCHET PASS`.
