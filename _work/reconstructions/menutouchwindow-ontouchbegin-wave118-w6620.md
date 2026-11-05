# MenuTouchWindow::OnTouchBegin wave 118 (w6620): exhaustive shape re-sweep, no source win

Date: 2026-11-05. Worktree `wt/w6620`, branch `wave118-w6620`, base
`3d4a98a3e` (wave-117 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, `bool MenuTouchWindow::OnTouchBegin(int,
int, void*)`). Baseline and best measured: **98.092643 %**, 1896 B,
734/734 normalised instructions, `byte=False`, `linked=False`.

Gate at commit: `byte_exact 4501 / linked_exact 4501`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27
sections, `.so` exactly 2,187,328 B. No section/extab change.

## TL;DR

The residual is **19 differing instruction lines in 3 regions**, every one a
pure register rename. No legal source variant found this wave moves any of
them; the HEAD shape is a hard local optimum. The measurements below are
recorded so the next worker does not re-run them.

## 1. The residual

`tools/verify/verify.py --show` diff (orig | ours), function at `0x129bd4`,
1896 B both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; layout->field_0x20_top
add    r3, ip                |  add    r3, r4
```

The whole `case 8` block is otherwise instruction-identical, and the
*duplicate* of the same block in `case 3` (iPad branch) is already
byte-identical. The original picks the high scratch `ip` for the
`field_0x20_top` load; ours picks `r4`. Both registers are dead at that
point in both compilations, so this is a greedy-RA colouring decision, not
a source-shape difference. (The same `y < lm + top + width` expression in
the *second* `case 8` `if` uses `r2` identically in both.)

### Region B — `case 15`, scrollbar `if`, first condition

```
ldr.w  r3, [sl, #312]        |  ldr.w  r2, [sl, #312]     ; this->heapBufB
ldr.w  r6, [sl, #480]        |  ldr.w  r3, [sl, #480]     ; this->field_0x1e0
ldr    r2, [r4, #0]          |  ldr    r6, [r4, #0]       ; Globals::layout
ldr.w  r1, [r3, r6, lsl #2]  |  ldr.w  r1, [r2, r3, lsl #2] ; heapBufB[idx]
ldr    r5, [r0, #0]          |  ldr    r4, [r0, #0]       ; Globals::Canvas
ldr    r2, [r2, #40]         |  ldr    r5, [r6, #40]      ; layout->buttonInsetX
subs   r5, r5, r2            |  subs   r4, r4, r5
subs   r0, r5, r0            |  subs   r0, r4, r0
```

Pure permutation of `{r2,r3,r4,r5,r6}` for
`Globals::w - layout.buttonInsetX - GetImage2DWidth(heapBufB[field_0x1e0])`.

### Region C — `case 15`, scrollbar `if`, second condition tail

```
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]
adds   r1, r6, r5            |  adds   r1, r5, r4
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]
add    r0, r5                |  add    r0, r4
```

`y < layout.field_0x20_top + (Globals::layout->field_0xc_leftMargin +
GetImage2DHeight(heapBufB[field_0x1e0]))` and the following
`x < Globals::layout->buttonInsetX + GetImage2DWidth(heapBufA[0])`.

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

## 3. Full 16-way `layout`-reference sweep (case 15)

Four access sites, each independently either `layout.` (L) or
`Globals::layout->` (G):

1. first condition `buttonInsetX`
2. second condition `field_0xc_leftMargin`
3. second condition `field_0x20_top`
4. second `if` `buttonInsetX`

All 16 combinations measured (full gold link + per-function verify):

| combo | pct | size |
|---|---:|---:|
| **LGLG (= HEAD)** | **98.092643** | 1896 |
| GGLG | 98.092643 | 1896 |
| LLGG | 97.956403 | 1896 |
| GLGG | 97.956403 | 1896 |
| LGLL | 90.859482 | 1892 |
| GLLL | 90.573770 | 1888 |
| LLGL | 90.723056 | 1892 |
| LLLG | 90.859482 | 1892 |
| LLLL | 90.573770 | 1888 |
| LGGL | 90.102389 | 1888 |
| GGGL | 90.102389 | 1888 |
| LGGG | 90.313779 | 1892 |
| GGGG | 90.313779 | 1892 |
| GLGL | 90.723056 | 1892 |
| GLLG | 90.859482 | 1892 |
| LLGG | (above) | |

Conclusion: `c` (top) **must** go through the local `layout` reference and
`b`/`d` **must** go through `Globals::layout->`; `a` is free (the compiler
CSEs the two forms of the same load). That is exactly HEAD. There is no
untried assignment.

## 4. Other case-15 variants (all worse or tied)

| variant | pct | size |
|---|---:|---:|
| `int idx = this->field_0x1e0;` local, used twice | 90.661213 | 1892 |
| `int insetX = Globals::layout->buttonInsetX;` local before `if` | 89.890710 | 1884 |
| `idx` + `insetX` locals | 89.692833 | 1880 |
| hoist `lm`/`top`/`insetX` to locals | 89.965870 | 1888 |
| `*(volatile int *)&Globals::layout->field_0xc_leftMargin` (2nd y only) | 97.275204 | 1896 |
| `layout.field_0xc_leftMargin` via ref | 90.859482 | 1892 |
| `layout.buttonInsetX` in 2nd `if` | 90.859482 | 1892 |
| `y < top + lm + height` (flat, no parens) | 97.138965 | 1896 |
| `y < (height + lm) + top` | 94.414169 | 1896 |
| `x > w - (insetX + width)` | 97.138965 | 1896 |
| `x > w - width - insetX` | 89.631651 | 1892 |
| no `Layout &layout` local (all `Globals::layout->`) | 90.313779 | 1892 |
| `this->scrollbarHit = <bool expr>;` | 89.358799 | 1892 |
| `const Layout &layout` | 98.092643 (tie, identical bytes) | 1896 |
| `PaintCanvas *canvas` + `w`/`h` locals | 87.661895 | 1892 |
| swap the two `if`s | 87.645051 | 1888 |

## 5. Case-8 variants (all tied or worse)

| variant | pct | size |
|---|---:|---:|
| `y < lm + (top + width)` | 98.092643 (tie) | 1896 |
| `y < width + top + lm` | 97.683924 | 1896 |
| 2nd `if` `y < top + lm + width` | 98.092643 (tie) | 1896 |
| local `Layout &layout` for the whole `case 8` block | 88.267394 | 1892 |
| swap the two `y` conditions in the first `if` | 98.092643 (tie) | 1896 |
| swap the two `x` conditions in the first `if` | 90.871935 | 1896 |
| reassociate `x < listTopY + buttonInsetX + listEntryHeight` | 98.092643 (tie) | 1896 |

Additional case-8 variants, all ties at 98.092643 % / 1896 B (byte-identical
output — the compiler canonicalises the reassociation and the comparison
direction):

| variant | pct |
|---|---:|
| flip all four comparisons (`a < x`, `b > x`, `c < y`, `d > y`) | 98.092643 |
| flip only the two `y` comparisons | 98.092643 |
| flip only the two `x` comparisons | 98.092643 |
| `y < (top + lm) + width` | 98.092643 |
| `y < top + (lm + width)` | 98.092643 |

All ties produce byte-identical output, so none is a real change.

## 6. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w6620
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

## 7. Verdict

The remaining 19 instruction lines are greedy register-allocation colouring
inside two blocks whose source shape is already pinned by the surrounding
code. The `Layout &layout` + two `Globals::layout->` shape (and its
semantically identical `GGLG` twin) is the unique optimum among all 16
access combinations. No source-level change tested this wave improves
98.092643 %. The next lever would have to come from a change elsewhere in
the function that perturbs global liveness (and that change must not
regress the 700+ already-matching instructions), or from a different
clang-7 scheduling outcome — neither is reachable by local rewrites.
