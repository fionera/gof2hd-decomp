# MenuTouchWindow::OnTouchBegin pass 5 — w4419

Date: 2026-11-03. Branch `wave94-w4419` (base master `f262bd4b`).
Gate before/after: **byte_exact 4499 / linked_exact 4499 / size-exact 4524**,
data_exact 843/843, imports 207, `.text` `0x1337c8`, `.rodata` `0x41b67`,
`.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26290` (-8), lint `CLEAN ✓`, drift 0,
`RATCHET PASS`.  **No code change landed**; this note records the fifth
source-shape search and the machine-IR evidence that the residual is a
register-allocation tie-break, not a missing source construct.

Prior notes (read first):

* `_work/reconstructions/menutouchwindow-ontouchbegin-97-7-modstation-c1-c2-98-6_w3819.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-modstation-pass2-w4019.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass3-w4203.md`
* `_work/reconstructions/menutouchwindow-ontouchbegin-pass4-w4247.md`

## Current state (re-measured)

| symbol | size orig/ours | official fuzzy | structural diff lines |
|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **97.7 %** | **27** (1 header + 2 case-8 + 24 case-15) |

The function is size-exact and semantically complete.  The whole residual is
clang 7.0.2 `-Oz` register allocation / rematerialisation inside two small
regions (case 8 and case 15/16).  There is **no `volatile`,
`__builtin_expect`, `__builtin_prefetch`, padding local or size anchor inside
`OnTouchBegin`**.  The current source of the case-15/16 block
(`src/game/ui/MenuTouchWindow.cpp` lines 2338–2352) is:

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

Reproduce with the official verifier:

```sh
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

The 27 `|` lines are:

```
ldr.w  ip, [sl, #340]              | ldr.w  r4, [sl, #340]        ; case 8
add    r3, ip                      | add    r3, r4                 ; case 8
ldr.w  r3, [sl, #312]              | ldr    r6, [r4, #0]          ; case 15
ldr.w  r6, [sl, #480]              | ldr.w  r2, [sl, #312]
ldr.w  fp, [r1]                    | ldr.w  r3, [sl, #480]
ldr    r2, [r4, #0]                | ldr.w  fp, [r1]
ldr.w  r1, [r3, r6, lsl #2]        | ldr    r4, [r0, #0]
ldr    r5, [r0, #0]                | ldr    r0, [r6, #32]
ldr.w  r0, [fp]                    | ldr.w  r1, [r2, r3, lsl #2]
ldr    r2, [r2, #40]               | str    r0, [sp, #12]
subs   r5, r5, r2                  | ldr.w  r0, [fp]
blx    GetImage2DWidth             | ldr    r5, [r6, #40]
subs   r0, r5, r0                  | subs   r4, r4, r5
cmp    r0, r8                      | blx    GetImage2DWidth
bge.n  0x65e                       | subs   r0, r4, r0
ldr.w  r0, [sl, #312]              | cmp    r0, r8
ldr.w  r1, [sl, #480]              | bge.n  0x65e
ldr    r2, [sp, #16]               | ldr.w  r0, [sl, #312]
ldr.w  r1, [r0, r1, lsl #2]        | ldr.w  r1, [sl, #480]
ldr    r2, [r2, #0]                | ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]                    | blx    GetImage2DHeight
ldr    r5, [r2, #12]               | ldr    r2, [sp, #12]
ldr    r6, [r2, #32]               | ldr    r1, [r6, #12]
blx    GetImage2DHeight            | add    r0, r2
adds   r1, r6, r5                  | add    r0, r1
add    r0, r1                      | (same)
ldr    r5, [r2, #40]               | ldr    r4, [r2, #40]         ; third if temp
add    r0, r5                      | add    r0, r4                ; third if temp
```

The third `if` (`0x12a23c..0x12a250`) is byte-identical to the original
except for the final temporary register (`r5` vs `r4`).  The first `if` and
the `GetImage2DHeight` block are the residual.  The current shape caches the
**value** of `Globals::layout` in `r6` across the first call and spills
`top` to `[sp,#12]`; the original reloads `&Globals::layout` from `[sp,#16]`
in the second and third `if` and keeps `&Globals::Canvas` in `fp`.

## Machine-IR evidence: the two shapes differ only in rematerialisation

`clang++ -mllvm -print-machineinstrs` on the TU (scratch
`/dev/shm/w4419/mi_direct.log`) shows that the faithful direct source:

```cpp
if (x > Globals::w - Globals::layout->buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
    y < Globals::layout->field_0xc_leftMargin + Globals::layout->field_0x20_top + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]))
    this->scrollbarHit = 1;
else
    this->scrollbarHit = 0;
if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) { … }
```

produces **three separate GOT loads** of `@_ZN7Globals6layoutE`, one in each
`if` block (pre-RA, `bb.38`/`bb.40`/`bb.42`):

```
bb.38 (%ir-block.198):
  %296:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6layoutE
  %298:gpr  = t2LDRi12 %296, 0        ; &Globals::layout
  %301:gpr  = t2LDRi12 %298, 0        ; Globals::layout value
  %306:rgpr = t2LDRi12 %301, 40       ; buttonInsetX
  ...
bb.40 (%ir-block.221):
  %313:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6layoutE
  %315:gpr  = t2LDRi12 %313, 0        ; &Globals::layout
  %319:gpr  = t2LDRi12 %315, 0        ; Globals::layout value
  %322:rgpr = t2LDRi12 %319, 12       ; leftMargin
  %323:gprnopc = t2LDRi12 %319, 32    ; top
```

The direct source compiles to **1892 B / 90.3 %**: the allocator keeps the
first GOT address live across both calls in `fp`
(`ldr.w fp,[sp,#16]` at the loop exit), so the second and third `if` reuse
`fp` instead of reloading `[sp,#16]`.  The original instead keeps
`&Globals::Canvas` in `fp` and rematerialises `&Globals::layout` from
`[sp,#16]` at each `if`; that costs two extra instructions (the `[sp,#16]`
reload plus the value load in the second and third `if`) and makes the
function 1896 B.

The current source (`Layout &layout = *Globals::layout;` + `int top`) is a
different point in the same space: it caches the **value** in `r6`, which
frees `fp` for `&Globals::Canvas` (matching the original) but costs the
`top` spill and the cached-value second `if`.  It is the only shape found
that reaches 97.7 %.

## Source-shape sweep (all re-measured this pass)

Every row replaces the case-15/16 block, rebuilds the whole `.so`
(`cmake --build cmake-build-match --target link_so -j8`, ~1 s) and runs the
official verifier (`tools/verify/verify.py`, `autojunk=False`).

### 1. 192-variant systematic sweep of the current family

Dimensions: first/second/third `if` layout access ∈ {direct, local
reference}; `top` local ∈ {yes,no}; `leftMargin` local ∈ {yes,no}; second
condition order ∈ {LM+TOP+H, H+TOP+LM, TOP+LM+H, LM+H+TOP, H+LM+TOP,
TOP+H+LM}.  Best rows:

| variant | size | fuzzy | rawdiff |
|---|---:|---:|---:|
| `s11010_H_TOP_LM` (current) | 1896 | **97.7** | 27 |
| `s11010_LM_H_TOP` | 1896 | 97.7 | 28 |
| `s11001_TOP_H_LM` | 1896 | 97.7 | 28 |
| `s11010_TOP_LM_H` | 1896 | 97.5 | 29 |
| `s10110_LM_H_TOP` | 1896 | 97.1 | 40 |
| all-direct (`s00000_*`) | 1892 | 90.3 | 155 |

No variant reaches 100 %.

### 2. The `fp = &Globals::Canvas` family (q9/w10)

The only shapes that put `&Globals::Canvas` in `fp` and keep
`&Globals::layout` in `r4` are those where the **first** `if` uses direct
`Globals::layout` and a **local reference value** is used in the second or
third `if`:

```cpp
Layout &layout = *Globals::layout;
if (x > Globals::w - Globals::layout->buttonInsetX - WID &&
    y < Globals::layout->field_0xc_leftMargin + layout.field_0x20_top + HEI)
    …
if (x < Globals::layout->buttonInsetX + WIDA) { … }        // q9: 96.7 %, 26 rawdiff
```

`q9`'s third `if` is byte-identical to the original except `r4`/`r5`, and
its second `if` has the original's instruction count, but it loads `top`
from the cached `r6` instead of the reloaded `[sp,#16]` value.  Making both
`leftMargin` and `top` direct collapses back to the 1892 B direct shape
(the reference becomes dead and the allocator puts `&Globals::layout` in
`fp`).  Variants `q4`, `q8`, `q10`–`q19`, `w1`–`w27`, `x1`–`x6`, `y1`–`y6`,
`z1`–`z5`, `p1`–`p6`, `t1`–`t4`, `v2/a`–`v2/e` cover the same space; the
best non-current result is `q9`/`q11` at 96.7 %.

### 3. Structural variants (nested `if`, bool, ternary, comma assignment)

`bool hit`, `int hit`, `scrollbarHit = cond`, nested `if`, comma-assigned
`Layout *layout` in the second condition, drag condition hoisted before the
scrollbar assignment, block-order swaps: all 83.6–94.7 %, never above the
current 97.7 %.

### 4. Function-scope `layout` reference/pointer

Adding `Layout *&layout = Globals::layout;` at the top of `OnTouchBegin`
and replacing every `Globals::layout` in the function gives 97.7 % (no
change); using `Layout *layout = Globals::layout;` (value copy) gives
42.9 %.  The function-scope reference does not change the case-15
allocation.

### 5. Case 8 (`0x129fd2`) — independent 2-byte tie-break

```
orig  129fd2: f8da c154   ldr.w  ip, [sl, #340]   ; listEntryWidth
      129fd6: 442b        add    r3, r5
      129fd8: 4463        add    r3, ip
ours  129fd2: f8da 4154   ldr.w  r4, [sl, #340]
      129fd6: 442b        add    r3, r5
      129fd8: 4423        add    r3, r4
```

`r4` is not live in the original's case-8 block; the original allocator
picks `ip` (r12) for the short-lived `field_0x154` temporary, ours picks
`r4`.  Reordering the 4th condition (all six field orders), swapping the
two `if` blocks, swapping conditions 1/2, adding `int entryWidth` /
`int bottom` / `int top` / `int lm` locals (shared and per-`if`), and
hoisting `inset` were all measured: 56.4–97.7 %, never better than the
current 97.7 %.  The second half of case 8 already uses `r2` in both
binaries, so only the first half's register tie-break differs.

## Why the current shape is the local optimum

The original source is almost certainly the faithful direct shape (Ghidra
shows three independent `*(int *)(*piVar3 + …)` accesses).  The direct
shape compiles to 1892 B because clang 7.0.2's greedy allocator keeps the
first GOT address live across both calls in `fp`.  The original instead
rematerialises it from `[sp,#16]` and gives `fp` to `&Globals::Canvas`.
That is a live-range-splitting / rematerialisation decision on a
GOT-relative address that is already spilled at function entry; it is not
reachable by changing the C++ expression, because every source form that
makes the first `if` use the address directly also makes the allocator
keep that address live.  The current shape trades the address cache for a
value cache + `top` spill and lands at 97.7 %, the best of the ~400
variants measured across passes 3–5.

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

# pre-RA machine IR (direct shape)
clang++ … -mllvm -print-machineinstrs -o /dev/null src/game/ui/MenuTouchWindow.cpp
```

## What remains

1. **case 8**: 2 bytes, `ip` vs `r4` register tie-break for the
   `field_0x154` temporary (`0x129fd2`/`0x129fd8`).
2. **case 15/16**: 24 instruction pairs.  The original reloads
   `&Globals::layout` from `[sp,#16]` in the second and third `if` and keeps
   `&Globals::Canvas` in `fp`; the current shape caches the layout value in
   `r6` and spills `top` to `[sp,#12]`.  The faithful direct shape is 4 B
   short because clang keeps `&Globals::layout` in `fp`.

No source shape found so far puts `&Globals::Canvas` in `fp` **and**
reloads `[sp,#16]` in the second `if` while staying 1896 B.  The remaining
angle is the same one pass 4 identified: a pre-RA live-range-splitting /
rematerialisation difference on the spilled GOT address, not a missing
source construct.
