# MenuTouchWindow::OnTouchBegin pass 4 — w4247

Date: 2026-11-03. Branch `wave92-w4247` (base master `b379d00d`, HEAD `cb47a768`).
Gate before/after: **byte_exact 4499 / linked_exact 4499 / size-exact 4524**,
data_exact 843/843, imports 207, `.text` `0x1337c8`, `.rodata` `0x41b67`,
`.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26290` (-8), lint `CLEAN ✓`, drift 0,
`RATCHET PASS`.  **No code change landed**; this note records the fourth
source-shape search (the new angle: the exact case-15 source order from
Ghidra).

Prior notes (read first):

* `_work/reconstructions/menutouchwindow-ontouchbegin-97-7-modstation-c1-c2-98-6_w3819.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-modstation-pass2-w4019.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass3-w4203.md`

## Current state (re-measured)

| symbol | size orig/ours | official fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **97.7 %** | **27** (`\|` lines: 1 header + 2 case-8 + 24 case-15) |

The function is size-exact and semantically complete.  The whole residual is
clang 7.0.2 `-Oz` register allocation / instruction scheduling inside two
small regions (case 8 and case 15/16).  There is **no `volatile`,
`__builtin_expect`, `__builtin_prefetch`, padding local or size anchor inside
`OnTouchBegin`**.

The current source of the case-15/16 block (`src/game/ui/MenuTouchWindow.cpp`
~2338–2352) is:

```cpp
{
    Layout &layout = *Globals::layout;
    int top = Globals::layout->field_0x20_top;
    if (x > Globals::w - layout.buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
        y < Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]) + top + layout.field_0xc_leftMargin)
        this->scrollbarHit = 1;
    else
        this->scrollbarHit = 0;
    if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
        this->dragLastX = y;
        this->dragStartX = y;
        this->dragVelocity = 0;
        this->dragging = 1;
    }
}
```

## The exact case-15 source order (Ghidra)

`decompile_function` at Ghidra address `0x139bd4` (image base `0x10000`) gives:

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
`&Globals::Canvas`.  The original source is **direct `Globals::layout`
access** (three separate `*(int *)(*piVar3 + …)` loads), and the second
condition loads `leftMargin` and `top` **before** the `GetImage2DHeight`
call.

## Disassembly of the case-15 block (VMA, original vs current)

Original (`0x12a1da..0x12a250`; `r4` = `&Globals::layout` loaded from
`[sp,#16]` in the loop-exit block at `0x12a1bc`, `fp` = `&Globals::Canvas`):

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

Current build (same VMA because the function is size-exact; `fp` =
`&Globals::Canvas`, `r6` = the **value** of `Globals::layout` cached across
the first call, `top` spilled to `[sp,#12]`):

```
12a1da: 484f         ldr    r0, [pc, #316]
12a1dc: 494f         ldr    r1, [pc, #316]
12a1de: 4478         add    r0, pc
12a1e0: 6826         ldr    r6, [r4, #0]      ; Globals::layout (cached value)
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
12a224: 68f1         ldr    r1, [r6, #12]     ; layout->field_0xc_leftMargin
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

The **third** `if` (`0x12a23c..0x12a250`) is byte-identical to the original
except for the final temporary register (`r5` vs `r4`).  The first `if` and
the `GetImage2DHeight` block are the residual.

## The faithful direct shape and the 4-byte shortfall

The faithful source (direct `Globals::layout` access, `leftMargin` + `top`
before the `GetImage2DHeight` call) is:

```cpp
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]))
    this->scrollbarHit = 1;
else
    this->scrollbarHit = 0;
if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) { … }
```

It compiles to **1892 B / 90.3 %**.  The direct shape's second `if` uses
`fp` (= `&Globals::layout`) for the layout fields and the third `if` also
uses `fp`, so it is **one instruction (4 B) short**: the original reloads
`[sp,#16]` in the third `if` (`ldr r0,[sp,#16]; ldr r2,[r0,#0]`), while the
direct shape uses `ldr.w r2,[fp]`.

Direct-shape second/third `if` (addresses from the same VMA; `fp` =
`&Globals::layout`, `r5` = `&Globals::Canvas`):

```
12a212: f850 1021    ldr.w  r1, [r0, r1, lsl #2]
12a216: 6828         ldr    r0, [r5, #0]      ; Globals::Canvas
12a218: f738 edba    blx    GetImage2DHeight
12a21c: f8db 1000    ldr.w  r1, [fp]          ; Globals::layout
12a220: 6a0b         ldr    r3, [r1, #32]     ; layout->top
12a222: 68ca         ldr    r2, [r1, #12]     ; layout->leftMargin
12a224: 4418         add    r0, r3
12a226: 4410         add    r0, r2
...
12a23a: f8db 2000    ldr.w  r2, [fp]          ; third if: uses fp, no reload
```

## Why the current shape is the local optimum

The current shape caches the **value** of `Globals::layout` in `r6` across
the first call and hoists `top` into a spill at `[sp,#12]`.  That spends two
extra instructions in the first `if` (`ldr r6,[r4,#0]` early and
`str r0,[sp,#12]`) and saves two in the second `if` (no `[sp,#16]` reload
and no pointer deref), so the function stays 1896 B.  It also keeps
`fp` = `&Globals::Canvas` and reloads `[sp,#16]` in the third `if`, matching
the original there.

The original instead keeps `r4` = `&Globals::layout` (dead after the first
`if`), reloads `[sp,#16]` in the second and third `if`, and keeps
`fp` = `&Globals::Canvas`.  The missing shape is therefore:

* first `if`: direct `Globals::layout->buttonInsetX`, `r4` = address (18 insns);
* second `if`: reload `[sp,#16]` + deref + `leftMargin`/`top` before the
  `GetImage2DHeight` call (13 insns);
* third `if`: reload `[sp,#16]` (11 insns);
* `fp` = `&Globals::Canvas`.

The direct shape has the first two but lets the allocator keep the address
in `fp`, so the third `if` is short.  Any source shape that uses
`Globals::layout` after the first call makes the address live across the
call and the allocator gives it `fp`; the current shape avoids this by
caching the value in `r6`, which costs the `top` spill.

## New source-shape sweep (all re-measured this pass)

Each row replaces the case-15/16 block, rebuilds the whole `.so`, and runs
the official verifier (`tools/verify/verify.py --show`, `autojunk=False`).
"fuzzy" is the verifier's official score; a row only matters if it reaches
100 % (`byte_exact`).

| variant | source shape (second condition / layout access) | size | fuzzy |
|---|---|---:|---:|
| **current** | `Layout&` + hoisted `top`; `height + top + layout.leftMargin` | 1896 | **97.7** |
| P | current, `height + layout.leftMargin + top` | 1896 | 97.7 |
| R | current, `top + height + layout.leftMargin` | 1896 | 97.7 |
| S | current, `layout.leftMargin + height + top` | 1896 | 97.7 |
| Z | `int top = layout.field_0x20_top` (cached) | 1896 | 97.7 |
| N | current, `top + layout.leftMargin + height` | 1896 | 97.5 |
| O | current, `layout.leftMargin + top + height` | 1896 | 97.5 |
| AA | `top` from cached ref, `leftMargin + top + height` | 1896 | 97.5 |
| AH | 24-shape `int top=0; (top=Globals::layout->top)+leftMargin+height > y` | 1896 | 97.1 |
| L | `Layout&` + `top` local + direct `leftMargin`, `height+top+leftMargin` | 1896 | 93.6 |
| AB | direct + `top` local, `height + top + leftMargin` | 1896 | 93.6 |
| AG | `Layout&` + `top` local + direct `leftMargin`, `height+top+leftMargin` | 1896 | 93.6 |
| C | `Layout&`, `leftMargin + top + height` | 1892 | 90.9 |
| V | `Layout&`, inline `top`/`leftMargin`, `height + top + leftMargin` | 1892 | 90.9 |
| W | `Layout&`, inline fields, `leftMargin + top + height` | 1892 | 90.9 |
| X | `Layout&` + comma-assigned `leftMargin`/`top` | 1892 | 90.9 |
| Y | same as X, `leftMargin + top + height` | 1892 | 90.9 |
| AI | 24-shape with cached `top` | 1892 | 90.9 |
| AD | local `PaintCanvas *canvas` + direct layout, `leftMargin+top+height` | 1892 | 90.5 |
| B | direct, `leftMargin + top + height` (faithful Ghidra order) | 1892 | 90.3 |
| D | `Layout&` first + direct second, `leftMargin+top+height` | 1892 | 90.3 |
| E | comma-assigned `leftMargin`/`top`, `leftMargin+top+height` | 1892 | 90.3 |
| F | comma-assigned, `height + top + leftMargin` | 1892 | 90.3 |
| J | local `Layout *layout` reassigned after call | 1892 | 90.3 |
| M | `Layout&` + `top` local + direct second, `leftMargin+top+height` | 1892 | 90.3 |
| T | `Layout&` first + comma direct second | 1892 | 90.3 |
| U | same as T, `leftMargin + top + height` | 1892 | 90.3 |
| H | hoisted `leftMargin`/`top` locals, direct | 1892 | 90.2 |
| AC | direct + `top` local, `leftMargin + top + height` | 1892 | 89.8 |
| AF | `Layout&` + `top` local + direct `leftMargin`, `leftMargin+top+height` | 1892 | 89.8 |
| direct | direct, `height + top + leftMargin` (Canvas first) | 1892 | 88.4 |
| K | `Layout&` + `top` local + direct second, `height+top+leftMargin` | 1892 | 88.4 |
| AE | local `PaintCanvas *canvas` + direct, `height+top+leftMargin` | 1892 | 88.1 |

No variant reaches 100 %.  The best official score remains the current
97.7 %; the closest size-exact alternatives (L/AB/AG, 93.6 %) all put
`&Globals::layout` in `fp` and `&Globals::Canvas` in `r5`, i.e. the opposite
of the original.

## Machine-instruction evidence (pre-RA / Greedy RA)

`clang++ -mllvm -print-machineinstrs` on the TU (scratch:
`/dev/shm/w4247/mi_cur.log`) shows the layout address is the stack-slot load
in the loop-exit block `bb.37`:

```
bb.37 (%ir-block.177):
  %505:gpr = t2LDRi12 %stack.0, 0, 14, $noreg :: (load 4 from %stack.0)
  t2B %bb.41, 14, $noreg, debug-location !15957
```

`%stack.0` is `[sp,#16]` (the `str r1,[sp,#16]` at `0x129c14`).  In the
current shape the final code assigns this load to `$r4`; in the direct
shape it is assigned to `$r11` (`fp`).  The original assigns it to `r4` and
then re-loads it from `[sp,#16]` at `0x12a212` and `0x12a23c`.  The
difference is a live-range-splitting / rematerialisation decision (the
address is a GOT-relative constant and is rematerialisable from
`[sp,#16]`), not a source-semantics difference.

## Flag sweep (all no-ops, same 1892–1896 B / 24-diff result)

```
-split-spill-mode=size|speed|default   -enable-deferred-spilling
-disable-spill-hoist                   -spp-rematerialization-threshold=0
-enable-local-reassign                 -regalloc-csr-first-time-cost=0|20
-condsider-local-interval-cost         -callsite-splitting-duplication-threshold=0|1000
-hoist-cheap-insts                     -sink-insts-to-avoid-spills
-early-live-intervals                  -disable-spill-fusing
```

`-spp-print-liveset` produces no output for this TU.

## Case 8 (`0x129fd2`) — unchanged

```
orig  129fd2: f8da c154   ldr.w  ip, [sl, #340]   ; listEntryWidth
      129fd6: 442b        add    r3, r5
      129fd8: 4463        add    r3, ip
ours  129fd2: f8da 4154   ldr.w  r4, [sl, #340]
      129fd6: 442b        add    r3, r5
      129fd8: 4423        add    r3, r4
```

`r4` is not used anywhere in the original's case-8 block; the original
allocator picks `ip` (r12) for the short-lived `field_0x154` temporary,
ours picks the callee-saved `r4`.  Pure register-choice tie-break.  The
case-8 condition reorderings measured in pass 3 (27 or 30 diff lines) were
not improved by any of this pass's variants.

## Reproduction

```sh
# gate
cmake --build cmake-build-match --target check -j8

# official per-function score (verify-fn target is broken: it passes --show
# with no value; use the direct form)
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv

# exact bytes / score
python3 /dev/shm/w4247/exactscore.py
```

## What remains

The function is 1896 B and semantically complete.  The residual is:

1. **case 8**: 2 bytes, `ip` vs `r4` register tie-break for the
   `field_0x154` temporary (`0x129fd2`/`0x129fd8`).
2. **case 15/16**: 24 instruction pairs.  The original reloads
   `&Globals::layout` from `[sp,#16]` in the second and third `if` and keeps
   `&Globals::Canvas` in `fp`; the current shape caches the layout value in
   `r6` and spills `top` to `[sp,#12]`.  The faithful direct shape is 4 B
   short because clang keeps `&Globals::layout` in `fp`.

No source shape found so far puts `&Globals::Canvas` in `fp` **and**
reloads `[sp,#16]` in the second `if` while staying 1896 B.  The next
plausible angle is a pre-RA scheduling difference (e.g. the loop-exit edge
block `bb.37`) rather than another local source permutation; the ~600
variants from pass 3 plus the ~40 new field-order/comma/local-pointer
variants above all land at 97.7 % or below.
