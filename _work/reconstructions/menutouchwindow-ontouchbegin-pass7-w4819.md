# MenuTouchWindow::OnTouchBegin pass 7 — w4819

Date: 2026-11-04. Branch `wave98-w4819` (base master `fe4f5e7cf`).
Gate before/after: **byte_exact 4500 / linked_exact 4500 / size-exact 4524**,
data_exact 843/843, imports 207, `.text` `0x1337c8`, `.rodata` `0x41b67`,
`.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26294` (-4), lint `CLEAN ✓`, drift 0,
`RATCHET PASS`.  **No code change landed**; this pass adds ~400 further
source-shape measurements and one new structural observation (the
`PaintCanvas *canvas` local reproduces the original's second/third `if`
exactly but costs a `mov` in the first `if`) to the pass-6 conclusion.

Prior notes (read first):

* `_work/reconstructions/menutouchwindow-ontouchbegin-97-7-modstation-c1-c2-98-6_w3819.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-modstation-pass2-w4019.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass3-w4203.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass4-w4247.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass5-w4419.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass6-w4520.md`

## Current state (re-measured this pass)

| symbol | size orig/ours | official fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **97.7 %** (raw 97.68392370572208) | **26** (2 case-8 + 24 case-15) |

The function is size-exact and semantically complete.  There is no
`volatile`, `__builtin_expect`, `__builtin_prefetch`, padding local or size
anchor inside `OnTouchBegin`.  The current case-15 source is the pass-6
shape:

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

## The exact residual

Original (ground-truth `.so`, `0x12a1da..0x12a266`) vs current
(`cmake-build-match/libgof2hdaa.so`, `0x12a1da..0x12a266`):

```
orig 12a1da: ldr r0,[pc]            ; &Globals::w
     12a1dc: ldr r1,[pc]            ; &Globals::Canvas
     12a1de: add r0,pc
     12a1e0: ldr.w r3,[sl,#312]     ; heapBufB
     12a1e4: add r1,pc
     12a1e6: ldr.w r6,[sl,#480]     ; field_0x1e0
     12a1ea: ldr r0,[r0,#0]         ; &Globals::w
     12a1ec: ldr.w fp,[r1]          ; &Globals::Canvas
     12a1f0: ldr r2,[r4,#0]         ; layout value   (r4 = [sp,#16])
     12a1f2: ldr.w r1,[r3,r6,lsl#2] ; heapBufB[field]
     12a1f6: ldr r5,[r0,#0]         ; w
     12a1f8: ldr r0,[fp]            ; Canvas
     12a1fc: ldr r2,[r2,#40]        ; buttonInsetX
     12a1fe: subs r5,r5,r2
     12a200: blx GetImage2DWidth
     12a204: subs r0,r5,r0
     12a206: cmp r0,r8
     12a208: bge 12a232
     12a20a: ldr.w r0,[sl,#312]     ; heapBufB
     12a20e: ldr.w r1,[sl,#480]     ; field_0x1e0
     12a212: ldr r2,[sp,#16]        ; &Globals::layout  <-- reload
     12a214: ldr.w r1,[r0,r1,lsl#2] ; heapBufB[field]
     12a218: ldr r2,[r2,#0]         ; layout value      <-- reload
     12a21a: ldr r0,[fp]            ; Canvas
     12a21e: ldr r5,[r2,#12]        ; leftMargin
     12a220: ldr r6,[r2,#32]        ; top
     12a222: blx GetImage2DHeight
     12a226: adds r1,r6,r5          ; top + leftMargin
     12a228: add r0,r1              ; height + (top+leftMargin)
     12a22a: cmp r0,r9
     12a22c: ble 12a232
     12a22e: movs r0,#1
     12a230: b 12a234
     12a232: movs r0,#0
     12a234: ldr.w r1,[sl,#308]     ; heapBufA
     12a238: strb.w r0,[sl,#473]    ; scrollbarHit
     12a23c: ldr r0,[sp,#16]        ; &Globals::layout  <-- reload
     12a23e: ldr r1,[r1,#0]         ; heapBufA[0]
     12a240: ldr r2,[r0,#0]         ; layout value      <-- reload
     12a242: ldr r0,[fp]            ; Canvas
     12a246: ldr r5,[r2,#40]        ; buttonInsetX
     12a248: blx GetImage2DWidth
     12a24c: add r0,r5
     12a24e: cmp r0,r8
     12a250: ble 12a2b6
     12a252: movs r0,#0
     12a254: str.w r9,[sl,#544]     ; dragLastX
     12a258: str.w r9,[sl,#524]     ; dragStartX
     12a25c: str.w r0,[sl,#532]     ; dragVelocity
     12a260: movs r0,#1
     12a262: strb.w r0,[sl,#548]    ; dragging
     12a266: b 12a2b6
```

Current:

```
ours 12a1da: ldr r0,[pc]
     12a1dc: ldr r1,[pc]
     12a1de: add r0,pc
     12a1e0: ldr r6,[r4,#0]         ; layout value   <-- cached early
     12a1e2: add r1,pc
     12a1e4: ldr.w r2,[sl,#312]     ; heapBufB
     12a1e8: ldr r0,[r0,#0]         ; &Globals::w
     12a1ea: ldr.w r3,[sl,#480]     ; field_0x1e0
     12a1ee: ldr.w fp,[r1]          ; &Globals::Canvas
     12a1f2: ldr r4,[r0,#0]         ; w
     12a1f4: ldr r0,[r6,#32]        ; top            <-- hoisted
     12a1f6: ldr.w r1,[r2,r3,lsl#2] ; heapBufB[field]
     12a1fa: str r0,[sp,#12]        ; spill top
     12a1fc: ldr r0,[fp]            ; Canvas
     12a200: ldr r5,[r6,#40]        ; buttonInsetX
     12a202: subs r4,r4,r5
     12a204: blx GetImage2DWidth
     12a208: subs r0,r4,r0
     12a20a: cmp r0,r8
     12a20c: bge 12a232
     12a20e: ldr.w r0,[sl,#312]     ; heapBufB
     12a212: ldr.w r1,[sl,#480]     ; field_0x1e0
     12a216: ldr.w r1,[r0,r1,lsl#2] ; heapBufB[field]
     12a21a: ldr r0,[fp]            ; Canvas
     12a21e: blx GetImage2DHeight
     12a222: ldr r2,[sp,#12]        ; top (spilled)
     12a224: ldr r1,[r6,#12]        ; leftMargin (cached)
     12a226: add r0,r2              ; height + top
     12a228: add r0,r1              ; + leftMargin
     12a22a: cmp r0,r9
     12a22c: ble 12a232
     12a22e: movs r0,#1
     12a230: b 12a234
     12a232: movs r0,#0
     12a234: ldr.w r1,[sl,#308]     ; heapBufA
     12a238: strb.w r0,[sl,#473]    ; scrollbarHit
     12a23c: ldr r0,[sp,#16]        ; &Globals::layout  (reload, identical)
     12a23e: ldr r1,[r1,#0]         ; heapBufA[0]
     12a240: ldr r2,[r0,#0]         ; layout value
     12a242: ldr r0,[fp]            ; Canvas
     12a246: ldr r4,[r2,#40]        ; buttonInsetX
     12a248: blx GetImage2DWidth
     12a24c: add r0,r4
     12a24e: cmp r0,r8
     12a250: ble 12a2b6
     ...
```

So the third (`drag`) block is already byte-identical except the final
temporary register (`r5` in the original, `r4` in ours).  The first
condition is the same instruction multiset in a different order.  The whole
24-line case-15 residual is:

1. the current shape caches the layout **value** in `r6` before the first
   call and hoists `top` to `[sp,#12]`;
2. the original re-loads `&Globals::layout` from `[sp,#16]` and the layout
   value after the first call, then loads `leftMargin` and `top` **before**
   the `GetImage2DHeight` call and computes `top + leftMargin` first;
3. the current shape computes `height + top` first and uses the cached
   `r6` / spilled `top`.

There are also the two independent case-8 bytes (`ldr.w ip,[sl,#340]` vs
`ldr.w r4,[sl,#340]`, `add r3,ip` vs `add r3,r4`).

## New evidence 1 — the `canvas` local reproduces the original B/drag

The pass-6-era shape from commit `539eeb3c4` (the `PaintCanvas *canvas`
local) is:

```cpp
{
    PaintCanvas *canvas = Globals::Canvas;
    if (x > Globals::w - (Globals::layout->buttonInsetX + canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0])) &&
        y < (Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin) + canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]))
        this->scrollbarHit = 1;
    else
        this->scrollbarHit = 0;
    if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
        ...
    }
}
```

It measures **97.208986 / 1896 B / 143 diff lines**, but its second and
third `if` blocks are *instruction-for-instruction identical to the
original* apart from register names:

```
ours (canvas-local) 12a214: ldr r2,[sp,#16]      ; &Globals::layout
                    12a216: ldr.w r1,[r0,r1,lsl#2]
                    12a21a: mov r0,r5            ; <-- extra mov (cached Canvas)
                    12a21c: ldr r2,[r2,#0]       ; layout value
                    12a21e: ldr r4,[r2,#12]      ; leftMargin
                    12a220: ldr r6,[r2,#32]      ; top
                    12a222: blx GetImage2DHeight
                    12a226: adds r1,r4,r6        ; leftMargin + top
                    12a228: add r0,r1            ; height + ...
```

The `canvas` local is live across the `GetImage2DWidth` call (it is used by
the `GetImage2DHeight` call), so it occupies a callee-saved register
(`r5`), and the allocator rematerialises `&Globals::layout` from
`[sp,#16]` instead of keeping it in `fp`.  That is exactly the
rematerialisation the original performs.  The cost is the extra
`mov r0,r5` before each cached-Canvas call (the first `if`'s width call and
the second `if`'s height call), i.e. 4 bytes, which the original does not
pay because it loads `Globals::Canvas` from `fp` at each call.

Removing the `canvas` local (`new1`) makes the first `if` match the
original's `ldr r0,[fp]`, but the compiler then caches the layout **value**
instead (the current shape / `new1`), and the address stays in `fp`:
`new1` = 89.631651 / 1892 B / 161 diff lines.

## New evidence 2 — direct shape with `THL` association

The direct shape with the `top + leftMargin + height` association
(`i1`) is **90.313779 / 1892 B / 154 diff lines**; its second `if` matches
the original's arithmetic (`adds r1,r4,r6; add r0,r1`) but uses `fp` for
the layout address instead of `[sp,#16]`.  The direct shape with the
`height + top + leftMargin` association is **88.403820 / 1892 B / 155
diff lines**.

So the original's B-block association is `(top + leftMargin) + height`
(equivalently `height + (top + leftMargin)`); the current shape's
`height + top + leftMargin` is the one association that keeps the
`r6`/`[sp,#12]` value cache and therefore the 97.7 % score.

## Source-shape sweep this pass (~400 variants)

Every row replaces only the case-15 block (or the case-8 block for the
case-8 rows), rebuilds the whole `.so`
(`cmake --build cmake-build-match --target link_so -j8`, ~1.7 s) and runs
the official verifier (`tools/verify/verify.py`, `autojunk=False`).

### A. 216-variant case-15 sweep (`/dev/shm/w4819/v18`)

Dimensions: first-`if` `buttonInsetX` access ∈ {direct, local `Layout
&layout`}; second-`if` `top` access ∈ {direct, local ref, `int top`};
second-`if` `leftMargin` access ∈ {direct, local ref, `int lm`};
third-`if` `buttonInsetX` access ∈ {direct, local ref}; second-condition
order ∈ all six permutations of H/T/L.  Best rows:

| variant | size | fuzzy | diff lines |
|---|---:|---:|---:|
| `x0197` (`r`,`t`,`r`,`d`,`HLT`) | 1896 | **97.683924** | 26 |
| `x0195` (`r`,`t`,`r`,`d`,`HTL`) | 1896 | **97.683924** | 26 |
| `x0193` (`r`,`t`,`r`,`d`,`THL`) | 1896 | **97.683924** | 26 |
| `x0173` (`r`,`r`,`l`,`d`,`HLT`) | 1896 | **97.683924** | 26 |
| `x0065` (`d`,`r`,`l`,`d`,`HLT`) | 1896 | **97.683924** | 26 |
| `x0060` (`d`,`r`,`l`,`d`,`HTL`) | 1896 | **97.683924** | 26 |
| `x0016` (`d`,`d`,`r`,`d`,`LTH`) | 1896 | 97.138965 | **23** |
| `x0026` (`r`,`r`,`r`,`d`,`LTH`) | 1896 | 97.547684 | 28 |

`x0016` has the fewest diff lines (23) but a lower ratio: its diffs fall in
the long first-condition sequence, while the current shape's 26 diffs are
mostly the two `top`/`[sp,#12]` instructions in the second condition.

### B. Targeted shapes (all measured, all ≤ 97.7)

* `PaintCanvas *canvas` / `PaintCanvas &canvas` local (old `539eeb3c4`
  shape): 97.208986 / 1896 B / 143.
* `canvas` used only in the second condition, direct first: 97.208986 /
  1896 B / 143 (the compiler hoists the Canvas load before the first
  call).
* comma-assigned `canvas` in the second operand: 90.313779 / 1892 B.
* `Layout *layout` / `Layout &layout` used in first + drag, direct
  second: 89.829352 / 1888 B / 150.
* `Layout *&layout = Globals::layout` reference-to-pointer: 88.403820 /
  1892 B (compiler folds it to direct).
* `Layout **layoutPtr = &Globals::layout`: 88.403820 / 1892 B.
* `const Layout &` / `const Layout *` / `Layout *const`: 97.683924 /
  1896 B / 26 (no change).
* `int w = Globals::w` / `int inset = layout.buttonInsetX` locals:
  97.683924 / 1896 B / 26 (no change).
* `int top` + `int lm` both: 90.115883 / 1888 B / 87.
* `__builtin_expect` on either condition (either polarity): 71.7–94.6 %,
  never better.
* swapping the two conditions, swapping the two `if` blocks, hoisting the
  width/height calls into locals: 87.8–94.6 %, never better.

### C. Case-8 sweep (`/dev/shm/w4819/v27`)

All six field orders and all groupings of
`leftMargin + top + listEntryWidth`, `int entryWidth`/`int top`/`int lm`
locals (with braces), and swapping the two `if` blocks: the best is the
current order at **97.683924 / 1896 B / 26**; the `listEntryWidth`
temporary is still `r4`, never `ip`.  The two case-8 bytes are a pure
allocator tie-break and are not reachable from the expression.

## Conclusion

The current shape is the local optimum of the reachable source space
(~900 variants measured across passes 3–7).  The residual is a clang 7.0.2
Greedy-RA live-range-splitting/rematerialisation tie-break on the spilled
`&Globals::layout` address:

* the direct shape keeps `&Globals::layout` live in `fp` across both
  calls (1892 B, 88.4 %);
* the current shape trades that for a layout-value cache in `r6` plus a
  `top` spill (1896 B, 97.7 %);
* the `canvas`-local shape gets the original's B/drag blocks exactly but
  pays a `mov` in the first `if` (1896 B, 97.2 %).

No source shape found in this pass puts `&Globals::Canvas` in `fp`,
rematerialises `&Globals::layout` from `[sp,#16]`, and loads both
`leftMargin` and `top` from the reloaded value while keeping the first
`if`'s `ldr r0,[fp]` — i.e. the exact original.  The note is committed
alone; no gate number changes.

## Reproduction

```sh
# gate
cmake --build cmake-build-match --target check -j8

# official per-function score / side-by-side
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv

# raw (unrounded) score probe
python3 /dev/shm/w4819/score.py cmake-build-match/libgof2hdaa.so
```

Scratch harness (not tracked, `/dev/shm/w4819`): `MenuTouchWindow.cpp.orig`,
`score.py`, `apply.py`, `apply8.py`, `gen*.py`, `v18/` (216-variant sweep),
`v18_results.txt`, `v27/` (case-8 sweep), `mi_current.log`,
`mi_direct.log`, `preRA_direct.txt`, `ghidra_out.txt`.
