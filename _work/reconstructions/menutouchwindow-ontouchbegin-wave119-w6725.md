# MenuTouchWindow::OnTouchBegin wave 119 (w6725): structural re-sweep, no source win

Date: 2026-11-05. Worktree `wt/w6725`, branch `wave119-w6725`, based on
current `master` (wave-118 harvest + the w6716 wave-119 note). Original
binary: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`). Baseline and best measured:
**98.092643 %**, 1896 B, **18** differing instruction lines, 734/734
normalised instructions, `byte=False`, `linked=False`.

Gate at commit: **RATCHET PASS**, `byte_exact 4503 / linked_exact 4503`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0. 27
sections, `.so` exactly 2,187,328 B. `.ARM.extab` unchanged at 0x26290
(8 B short of the original's 0x26298); no section/extab change.

## TL;DR

This wave attacked the residual from a different angle than the w6716
RA sweep: it tried to reproduce the original's *fresh* `Globals::layout`
reload for the second `case 15` condition while keeping the 1896-byte
code size. Every legal C++ shape that makes both `field_0xc_leftMargin`
and `field_0x20_top` come from a post-call `Globals::layout` load
(all-direct) produces **1892 B** and 90.313779 % (154 diff lines),
because clang promotes `&Globals::layout` into the callee-saved high
register `fp` and drops the stack reload. Every shape that keeps
**1896 B** either caches the layout *value* for `field_0xc_leftMargin`
(HEAD, 18 diffs) or caches `field_0x20_top` (GGLG `(top+lm)+h`, 23
diffs). No variant found this wave beats 98.092643 %. The HEAD shape is
a hard local optimum.

## 1. Residual (unchanged, 18 lines in 3 regions)

`tools/verify/verify.py --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(orig | ours), function at `0x129bd4`, 1896 B both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

The duplicate block in `case 3` (iPad branch) is byte-identical and uses
`ip` in both. The second `case 8` `if` uses `r2` in both. Pure RA
tie-break; `r4` is free at that point in both binaries.

### Region B — `case 15`, scrollbar `if`, first condition

```
ldr.w  r3, [sl, #312]        |  ldr.w  r2, [sl, #312]     ; this->heapBufB
ldr.w  r6, [sl, #480]        |  ldr.w  r3, [sl, #480]     ; this->field_0x1e0
ldr    r2, [r4, #0]          |  ldr    r6, [r4, #0]       ; Globals::layout
ldr.w  r1, [r3, r6, lsl #2]  |  ldr.w  r1, [r2, r3, lsl #2] ; heapBufB[idx]
ldr    r5, [r0, #0]          |  ldr    r4, [r0, #0]       ; Globals::w
ldr    r2, [r2, #40]         |  ldr    r5, [r6, #40]      ; layout->buttonInsetX
subs   r5, r5, r2            |  subs   r4, r4, r5
subs   r0, r5, r0            |  subs   r0, r4, r0
```

### Region C — `case 15`, second condition tail

```
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]      ; layout->field_0x20_top
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]
adds   r1, r6, r5            |  adds   r1, r5, r4
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]
add    r0, r5                |  add    r0, r4
```

## 2. HEAD source shape (the 98.092643 % shape)

```cpp
{
    Layout &layout = *Globals::layout;
    if (x > Globals::w - layout.buttonInsetX -
                 Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) &&
        y < layout.field_0x20_top +
                (Globals::layout->field_0xc_leftMargin +
                 Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])))
        this->scrollbarHit = 1;
    else
        this->scrollbarHit = 0;
    if (x < Globals::layout->buttonInsetX +
                Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) {
        this->dragLastX = y;
        this->dragStartX = y;
        this->dragVelocity = 0;
        this->dragging = 1;
    }
}
```

The original binary's post-`GetImage2DWidth` sequence reloads
`Globals::layout` once into `r2` and takes **both**
`field_0xc_leftMargin` (`[r2,#12]`) and `field_0x20_top` (`[r2,#32]`)
from it. HEAD reloads the pointer only for `field_0x20_top`; it takes
`field_0xc_leftMargin` from the layout value cached in `r6` before the
first call.

## 3. Why the faithful all-direct shape loses

The presumed original source is fully direct:

```cpp
if (x > Globals::w - Globals::layout->buttonInsetX - W &&
    y < Globals::layout->field_0x20_top +
            (Globals::layout->field_0xc_leftMargin + H))
```

That shape compiles to the original's region-B semantics (both fields
from a fresh post-call pointer), but at **1892 B**, not 1896 B. The
post-RA MIR shows why:

* HEAD: `&Globals::layout` is loaded from `[sp,#16]` into `r4`, used for
  the first condition, then dead; it is reloaded from `[sp,#16]` after
  the call for the `top` load. The layout *value* is cached in `r6`
  across the call for the `lm` load.
* All-direct: `&Globals::layout` is loaded from `[sp,#16]` into `fp` at
  `12a1bc` and kept live across the whole `case 15` region (uses at
  `12a1f0`, `12a212`, `12a23e`). `fp` is callee-saved, so the stack
  reloads disappear. The 4-byte size delta is exactly
  `ldr.w fp,[sp,#16]` (4 B) vs `ldr r4,[sp,#16]` (2 B) plus the
  corresponding `ldr.w r2,[fp]` vs `ldr r2,[sp,#16]` / `ldr r2,[r2]`
  pair. Every intra-function branch target shifts by 4 B, cascading to
  154 diff lines.

In the original, `fp = &Globals::Canvas` is the value kept across the
call, and `&Globals::layout` is rematerialised from `[sp,#16]`. In the
all-direct build clang makes the opposite choice. This is a global
greedy-RA decision, not a source-shape difference.

## 4. Measurements this wave (all source restored afterwards)

Harness: replace only the `case 15` block (or the `case 8` block) in
`src/game/ui/MenuTouchWindow.cpp`, `cmake --build cmake-build-match
--target link_so -j6`, then `tools/verify/verify.py` / a small
`score.py` wrapper. Baseline: 1896 B, 98.092643 %, 18 diffs.

### 4.1 Layout access spellings and local kinds (case 15)

| variant | size | ratio % | diffs |
|---|---|---|---|
| HEAD (`Layout &layout`, `layout.` for inset/top, direct `lm`) | 1896 | 98.092643 | 18 |
| `GGLG` (`layout.` only for top) | 1896 | 98.092643 | 18 |
| `LGLG` (`layout.` for inset and top) | 1896 | 98.092643 | 18 |
| `GLGG` (`layout.` for `lm`, direct top) | 1896 | 97.956403 | 18 |
| `LLGG` (`layout.` for inset and `lm`) | 1896 | 97.956403 | 18 |
| `Layout *const layout = Globals::layout` | 1892 | 90.859482 | 153 |
| `const Layout *layout = Globals::layout` | 1892 | 90.859482 | 153 |
| `const Layout *const layout = Globals::layout` | 1892 | 90.859482 | 153 |
| `auto layout = Globals::layout` | 1892 | 90.859482 | 153 |
| `auto &layout = *Globals::layout` | 1892 | 90.859482 | 153 |
| `Layout *&layout = Globals::layout` (ref to pointer) | 1892 | 90.313779 | 154 |
| `Layout *const &layout = Globals::layout` | 1892 | 90.313779 | 154 |
| `Layout **lp = &Globals::layout; (*lp)->...` | 1892 | 90.313779 | 154 |
| all direct (`Globals::layout->` everywhere) | 1892 | 90.313779 | 154 |
| `(*Globals::layout).field` / `Globals::layout[0].field` | 1892 | 90.313779 | 154 |

All-direct is insensitive to arrow/dot/index spelling: the IR is
identical.

### 4.2 Association / evaluation order (case 15)

| form | size | ratio % | diffs |
|---|---|---|---|
| `top + (lm + h)` (HEAD) | 1896 | 98.092643 | 18 |
| `(top + lm) + h` with `top` local | 1896 | 97.138965 | 23 |
| `lm + top + h` with `top` local | 1896 | 97.138965 | 23 |
| `top + h + lm` | 1896 | 94.414169 | 53 |
| `h + (top + lm)` | 1896 | 94.414169 | 54 |
| `top + (h + lm)` | 1896 | 94.414169 | 53 |
| all-direct `top + (lm + h)` | 1892 | 90.313779 | 154 |
| all-direct `(top + lm) + h` | 1892 | 90.313779 | 154 |

The original's `adds r1,r6,r5` (top+lm) followed by `add r0,r1`
(+height) is reproduced by both `top+(lm+h)` and `(top+lm)+h` after
reassociation; only the 1896-byte `top+(lm+h)` shape keeps the rest of
the RA.

### 4.3 Extra live values / precomputation (case 15)

| variant | size | ratio % | diffs |
|---|---|---|---|
| `unsigned int img = heapBufB[field_0x1e0]` (both calls) | 1892 | 89.495225 | 164 |
| `int *buf = heapBufB; int idx = field_0x1e0` | 1892 | 89.495225 | 164 |
| `int *buf = heapBufB` only | 1892 | 89.495225 | 164 |
| `int idx = field_0x1e0` only | 1888 | 90.102389 | 156 |
| `int w = Globals::w` | 1892 | 90.313779 | 154 |
| `int inset = Globals::layout->buttonInsetX` | 1892 | 90.313779 | 154 |
| `int right = Globals::w - Globals::layout->buttonInsetX` | 1892 | 90.313779 | 154 |
| `PaintCanvas *canvas = Globals::Canvas` (value local) | 1892 | 90.524881 | 92 |
| `PaintCanvas *&canvas = Globals::Canvas` (ref local) | 1892 | 90.313779 | 154 |
| HEAD + `PaintCanvas *&canvas` | 1896 | 98.092643 | 18 |
| HEAD + canvas local only in last `if` | 1896 | 98.092643 | 18 |
| HEAD + `unsigned int img` local | 1888 | 90.388548 | 161 |
| nested `if` with pointer reload inside | 1896 | 90.102389 | 151 |
| `bool hitX` then pointer reload | 1892 | 89.085948 | 162 |
| pointer reassigned after first call | 1892 | 89.085948 | 162 |
| new reference scope after first call | 1892 | 89.085948 | 162 |
| comma reload `(layout = Globals::layout)->` | 1892 | 90.313779 | 154 |

No extra live value changed the `fp` promotion of `&Globals::layout`.

### 4.4 `case 8` (region A) perturbations

All of these were measured with the HEAD `case 15` block:

| variant | size | ratio % | diffs |
|---|---|---|---|
| baseline | 1896 | 98.092643 | 18 |
| `int width = this->listEntryWidth` in both `if`s | 1896 | 97.956403 | 19 |
| `int topY`, `int height`, `int lm`, `int top`, `int inset` locals | 1896 | 98.092643 | 18 |
| `Layout &layout` / `Layout *layout` for all case-8 accesses | 1896 | 98.092643 | 18 |
| first-if conjunct order `0123` / `0132` | 1896 | 98.092643 | 18 |
| other 22 conjunct orders | 1896 | 64.9–97.5 | 26–553 |
| `y2` arithmetic `lm+top+w`, `lm+(top+w)`, `top+lm+w`, `top+(lm+w)` | 1896 | 98.092643 | 18 |
| `y2` arithmetic `w+lm+top`, `lm+w+top`, `(lm+w)+top` | 1896 | 97.68–97.96 | 19–21 |

Region A's `ip` vs `r4` is a pure allocator tie-break and did not move
under any legal source spelling tried.

## 5. Verdict

The residual is a greedy register-allocation tie-break, not a source
shape difference. The original keeps `&Globals::Canvas` in `fp` and
rematerialises `&Globals::layout` from `[sp,#16]`; HEAD does the same.
The only remaining semantic difference is that HEAD caches the layout
*value* for `field_0xc_leftMargin` while the original reloads it. Making
that access direct (the faithful source) makes clang promote
`&Globals::layout` to `fp`, shrinking the function to 1892 B and
cascading all branch targets. No legal C++ variant tested this wave
(about 60 new builds across sections 4.1–4.4, on top of the ~400
w6716 builds) improves 98.092643 %. The next lever would have to
perturb global liveness from outside `case 15` while preserving the
700+ already-matching instructions, or a different clang-7 scheduling
outcome.

## 6. Reproduction

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# -> RATCHET PASS, byte 4503, linked 4503, lint CLEAN, drift 0

python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Post-RA MIR: compile the TU with the exact CMake command plus
`-mllvm -print-after-all`, then extract the
`IR Dump After Greedy Register Allocator` /
`Virtual Register Rewriter` sections for
`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`.
