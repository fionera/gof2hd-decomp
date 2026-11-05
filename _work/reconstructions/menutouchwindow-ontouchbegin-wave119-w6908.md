# MenuTouchWindow::OnTouchBegin wave 119 (w6908): live-range / spelling sweep, no source win

Date: 2026-11-05. Worktree `wt/w6908`, branch `wave119-w6908`, base
`50d2df436`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, `bool MenuTouchWindow::OnTouchBegin(int,
int, void*)`). Baseline and best measured: **98.092643 %**, 1896 B,
734/734 normalised instructions, `byte=False`, `linked=False`, **18**
differing instruction lines (19 including the `orig | ours` header line).

Gate at commit: `byte_exact 4505 / linked_exact 4505`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27
sections, `.so` exactly 2,187,328 B. No section/extab change.

## TL;DR

The residual is the same 18 lines in 3 regions as waves 118/119-w6716/w6746.
This wave tried a new class of shapes: explicit temporaries (live values
across the first call), pointer re-assignment / comma operators, an inline
accessor, hoisted `heapBufB`/`Canvas`, function-scope locals, and loop-form
perturbation. **No legal C++ variant beats 98.092643 %.** The new result is
that *any* extra value kept live across the first `GetImage2DWidth` call
does force the compiler to rematerialise `&Globals::layout` at bb.44 (the
original's address handling), but every such shape also perturbs the
register assignment of the whole case-15 block and/or loses the 4 B that
the original keeps at bb.47, so the fuzzy score drops to 88–91 %.

## 1. The residual (unchanged)

`verify.py --show` diff (orig | ours), function at `0x129bd4`, 1896 B both
sides, 734 instructions both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

### Region B — `case 15`, scrollbar `if`, first condition

```
ldr.w  r3, [sl, #312]        |  ldr.w  r2, [sl, #312]     ; this->heapBufB
ldr.w  r6, [sl, #480]        |  ldr.w  r3, [sl, #480]     ; this->field_0x1e0
ldr    r2, [r4, #0]          |  ldr    r6, [r4, #0]       ; Globals::layout
ldr.w  r1, [r3, r6, lsl #2]  |  ldr.w  r1, [r2, r3, lsl #2]
ldr    r5, [r0, #0]          |  ldr    r4, [r0, #0]       ; Globals::w
ldr    r2, [r2, #40]         |  ldr    r5, [r6, #40]      ; layout->buttonInsetX
subs   r5, r5, r2            |  subs   r4, r4, r5
subs   r0, r5, r0            |  subs   r0, r4, r0
```

### Region C — `case 15`, scrollbar `if`, second condition tail

```
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]      ; layout->field_0x20_top (cached)
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]      ; layout->field_0xc_leftMargin
adds   r1, r6, r5            |  adds   r1, r5, r4
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]      ; third if: layout->buttonInsetX
add    r0, r5                |  add    r0, r4
```

## 2. HEAD source shape

```cpp
{
    Layout &layout = *Globals::layout;
    if (x > Globals::w - layout.buttonInsetX - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
        y < layout.field_0x20_top + (Globals::layout->field_0xc_leftMargin + Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])))
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

## 3. Binary facts (re-confirmed with objdump)

- Literal pool: `0x129f54` → GOT `Globals::layout`; `0x12a318` → GOT
  `Globals::w`; `0x12a31c` → GOT `Globals::Canvas`.
- `&Globals::layout` (the GOT-resolved address of the pointer global) is
  computed once at `0x129bfc..0x129c00` and spilled to `[sp,#16]`
  (`str r1,[sp,#16]` at `0x129c14`). Every later use reloads it.
- Before the `scrollSlots` loop the original does `movs r5,#0;
  ldr r4,[sp,#16]` (`0x12a1ba/0x12a1bc`): the address lives in `r4` up to
  the first condition and is **rematerialised** (`ldr r2,[sp,#16];
  ldr r2,[r2,#0]`) after the first `GetImage2DWidth` call at `0x12a212`.
- `fp = &Globals::Canvas` (`ldr.w fp,[r1]` at `0x12a1ec`), live across both
  calls; the Canvas *value* is reloaded from `fp` each time.
- `case 15` second condition (`0x12a20a..0x12a228`): fresh layout value,
  then `leftMargin` (`[r2,#12]`) and `top` (`[r2,#32]`) from that same
  fresh value, then the height call, then `adds r1,r6,r5; add r0,r1`.
- The only value live across the first call in the original is
  `fp = &Globals::Canvas`. The layout address is split at the call.

## 4. Variants measured this wave

All measurements are full gold link + per-function `verify.py --show`.
"diffs" = `|` data lines (header excluded). Every variant reverted.

| variant | size | pct | diffs |
|---|---:|---:|---:|
| HEAD (`Layout &layout`, top cached) | 1896 | **98.092643** | **18** |
| `leftMargin + (top + H)` (lfirst, a=L,b=L,c=G) | 1896 | 98.092643 | 18 |
| `Layout *layout` function-scope local used in case 1/2 (fs_local) | 1896 | 98.092643 | 18 (same set) |
| `Globals::layout->top + (layout.leftMargin + H)` (LGLG, top fresh) | 1896 | 97.956403 | 18 |
| `layout.leftMargin + (Globals::layout->top + H)` (lglg2) | 1896 | 97.956403 | 18 |
| `layout.buttonInsetX` first + third, both fields fresh (LGGL) | 1888 | 90.102389 | 157 |
| `while` loop instead of `for` (scrollSlots) | 1896 | 97.956403 | 20 |
| `scrollbarHit = <bool expr>` | 1896 | 90.102389 | 163 (731 insns) |
| `scrollbarHit = <cond> ? 1 : 0` | 1896 | 90.102389 | 163 (731 insns) |
| all-direct (`Globals::layout->` everywhere) | 1892 | 90.313779 | 164 |
| `leftMargin + top + H` (all-direct, L first) | 1892 | 90.313779 | 164 |
| `mtwLayout()` static-inline accessor | 1892 | 90.313779 | 164 |
| `(layout = Globals::layout, layout->top + ...)` comma, third if fresh | 1892 | 90.313779 | 164 |
| `int right = Globals::w - Globals::layout->buttonInsetX;` | 1892 | 90.313779 | 164 |
| `*(this->heapBufB + this->field_0x1e0)` / `*this->heapBufA` | 1892 | 90.313779 | 164 |
| `(layout = Globals::layout, ...)` comma, third if cached | 1892 | 90.995907 | 150 |
| `int lm = Globals::layout->field_0xc_leftMargin;` before the `if` | 1892 | 90.599455 | 78 |
| `int top = Globals::layout->field_0x20_top;` before the `if` | 1892 | 90.599455 | 78 |
| `PaintCanvas *canvas = Globals::Canvas;` hoisted | 1892 | 90.524881 | 101 |
| both fields cached (`layout.top + (layout.leftMargin + H)`) | 1892 | 90.859482 | 163 |
| `unsigned idx = this->field_0x1e0;` hoisted | 1888 | 90.102389 | 168 |
| `unsigned *bufB = this->heapBufB;` hoisted | 1888 | 89.556314 | 172 |
| `H + leftMargin + top` (H first) | 1892 | 88.403820 | 165 |
| `int width = GetImage2DWidth(...);` before the `if` | 1884 | 88.797814 | 172 |
| `Layout &layout` hoisted before the `scrollSlots` loop | 1920 | 46.289993 | 683 |

Key new observation: the `lm`/`topi`/`bufB`/`idx` hoists **do** force the
compiler to rematerialise `&Globals::layout` from `[sp,#16]` at bb.44 and
bb.47 (exactly the original's address handling) and give a fresh `top` +
`leftMargin`, but they also pin the hoisted value in `fp`/`r5` across the
calls, renumbering the whole block; the branch targets then shift and the
fuzzy score collapses. Conversely, every 1896 B shape that keeps the
original's address handling caches exactly one of the two fields in `r6`
(HEAD caches `top`, `LGLG` caches `leftMargin`), both 18 diffs.

## 5. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w6908
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Exact score (verify.py prints one decimal):

```python
# /dev/shm/w6908/exact.py — imports tools/verify/verify.py as a module,
# prints f'{ratio:.6f}%  len {len(on)}/{len(un)}'
```

Post-RA MIR: compile the TU from `cmake-build-match/compile_commands.json`
with `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN15MenuTouchWindow12OnTouchBeginEiiPv`; the section
`# *** IR Dump After Greedy Register Allocator ***` shows `%r6 =
t2LDRi12 %r4, 0` (layout value) live across the first call in HEAD.

## 6. Verdict

The residual is a greedy-RA live-range-splitting tie-break, not a source
shape difference. In HEAD the layout value `%29` is live across the first
`GetImage2DWidth` call, which is exactly what makes the spiller split
`&Globals::layout` at bb.44/bb.47 (the original's address handling) — but
it also makes the `top` load come from the cached `%29` (`ldr r4,[r6,#32]`)
instead of the fresh `[r2,#32]`. Removing that cached use (all-direct)
lets the allocator keep `&Globals::layout` in `fp` across both calls and
loses 4 B (1892 B, branch-target cascade). Every shape that keeps one field
cached stays at 1896 B with the same 18 diffs; every shape with both fields
fresh is 1892 B. No legal C++ variant tested (this wave or waves
118/119-w6716/w6746, >230 shapes total) improves 98.092643 %. The next
lever would have to perturb global liveness from *outside* case 15 while
keeping the 700+ already-matching instructions, or a different clang-7
scheduling outcome.
