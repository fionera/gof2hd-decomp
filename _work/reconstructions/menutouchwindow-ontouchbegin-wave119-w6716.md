# MenuTouchWindow::OnTouchBegin wave 119 (w6716): RA tie-break re-sweep, no source win

Date: 2026-11-05. Worktree `wt/w6716`, branch `wave119-w6716`, base
`47a1396bd` (wave-118 harvest). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, `bool MenuTouchWindow::OnTouchBegin(int,
int, void*)`). Baseline and best measured: **98.092643 %**, 1896 B,
734/734 normalised instructions, `byte=False`, `linked=False`, **18**
differing instruction lines.

Gate at commit: `byte_exact 4503 / linked_exact 4503`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27
sections, `.so` exactly 2,187,328 B. No section/extab change.

## TL;DR

The residual is the same 18 lines in 3 regions as wave 118. Every one is a
pure register rename. This wave re-ran the local source sweep, added
case-order/prologue perturbations, and inspected clang's post-RA MIR. The
HEAD shape is a hard local optimum: **no legal C++ variant found this wave
beats 98.092643 %**. The new information is *why*: HEAD's address
assignment already matches the original (`r4 = &Globals::layout`,
`fp = &Globals::Canvas`); the only difference is that HEAD caches the
`Globals::layout` **value** in `r5` across the first `GetImage2DWidth`
call, while the original reloads it into `r2`. Making the second
condition's `top` access direct forces clang to promote
`&Globals::layout` to `r11`, which changes the code size to 1892 B and
cascades every intra-function branch target (154 diff lines).

## 1. The residual (unchanged)

`tools/verify/verify.py --show` diff (orig | ours), function at `0x129bd4`,
1896 B both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

The duplicate block in `case 3` (iPad branch) is byte-identical and uses
`ip` in both. The second `case 8` `if` uses `r2` in both. Pure RA
tie-break.

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

### Region C — `case 15`, scrollbar `if`, second condition tail

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

## 3. Post-RA MIR evidence (new this wave)

Compiled the TU with the exact CMake command plus
`-mllvm -print-after-all` and extracted the
`IR Dump After Virtual Register Rewriter` section for
`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`.

### HEAD

`bb.42` liveins: `$r4, $r8, $r9, $r10` (`r4 = &Globals::layout`,
`r8 = x`, `r9 = y`, `r10 = this`). `fp = &Globals::Canvas`.

```
$r0 = tLDRLIT_ga_pcrel @Globals::w
$r1 = tLDRLIT_ga_pcrel @Globals::Canvas
$r0 = [$r0]                 ; &Globals::w
$r5 = [$r4]                 ; Globals::layout value   <-- cached in r5
$r2 = [$r10, #312]          ; heapBufB
$r3 = [$r10, #480]          ; field_0x1e0
$r11 = [$r1]                ; &Globals::Canvas
$r4 = [$r0]                 ; Globals::w
$r6 = [$r5, #40]            ; buttonInsetX
$r1 = [$r2 + $r3*4]         ; heapBufB[idx]
$r0 = [$r11]                ; Canvas
$r4 = $r4 - $r6             ; w - insetX
bl GetImage2DWidth
$r0 = $r4 - $r0
...
bb.44 (second condition):
$r2 = [$r10, #312]
$r1 = [$r10, #480]
$r4 = [$r5, #12]            ; leftMargin from cached r5  <-- differs
$r6 = [$r5, #32]            ; top from cached r5         <-- differs
$r2 = [$r11]                ; fresh layout for b
...
```

### Original (from disassembly)

```
r4 = [sp,#16]               ; &Globals::layout
r3 = [sl,#312]              ; heapBufB
r6 = [sl,#480]              ; field_0x1e0
r0 = &Globals::w
r1 = &Globals::Canvas
r0 = [r0]                   ; &Globals::w
fp = [r1]                   ; &Globals::Canvas
r2 = [r4]                   ; layout value  <-- r2, not cached
r1 = [r3 + r6*4]            ; heapBufB[idx]
r5 = [r0]                   ; Globals::w   <-- r5, not r4
r0 = [fp]                   ; Canvas
r2 = [r2, #40]              ; buttonInsetX
r5 = r5 - r2
bl GetImage2DWidth
r0 = r5 - r0
...
second condition:
r2 = [sp,#16]
r2 = [r2]                   ; fresh layout
r5 = [r2, #12]              ; leftMargin (fresh)
r6 = [r2, #32]              ; top (fresh)
```

So HEAD already has the original's `r4 = &Globals::layout` and
`fp = &Globals::Canvas`. The divergence is only that HEAD keeps the
layout *value* in `r5` across the call; the original reloads it into
`r2`, which frees `r5` for `w` and makes `r2` the scratch for
`insetX`/`leftMargin`/`top`.

### Why all-direct is 1892 B / 154 diffs

All-direct `Globals::layout->` for `a,b,c,d` gives:
`bb.42` liveins `$r8, $r9, $r10, $r11`, with `r11 = &Globals::layout`
promoted across the whole function and `r5 = &Globals::Canvas`. The
address load becomes `ldr.w fp,[sp,#16]` (4 B) instead of
`ldr r4,[sp,#16]` (2 B), the layout value is reloaded into `r2` after
the call, and the function is 4 B shorter. Every intra-function branch
target shifts, producing 154 diff lines. With <=2 direct `Globals::layout`
accesses the address stays in `r4` and the size stays 1896 B
(HEAD, LLGG, `GGLG`).

## 4. Variants measured this wave

All measurements are full gold link + per-function verify. "diffs" is
the number of `|` structural lines; lower is better. Every variant below
was reverted; HEAD is unchanged.

### Case 15 expression / local shapes

| variant | size | pct | diffs |
|---|---:|---:|---:|
| **HEAD** | 1896 | **98.092643** | **18** |
| `Layout *layout = Globals::layout` + c local | 1896 | 98.092643 | 18 |
| `const Layout *layout` + c local | 1896 | 98.092643 | 18 |
| `int w = Globals::w` local | 1896 | 98.092643 | 18 |
| `int *wp = &Globals::w` + `*wp` | 1896 | 98.092643 | 18 |
| `*(this->heapBufB + idx)` / cast index | 1896 | 98.092643 | 18 |
| alias `Layout &layout2 = layout` | 1896 | 98.092643 | 18 |
| `T_M_H`, `M_T_H` associations | 1896 | 98.092643 | 18 |
| `int lm = Globals::layout->field_0xc_leftMargin` local | 1896 | 97.683924 | 27 |
| nested `if (x) { if (y) }` (HEAD shape) | 1896 | 97.547684 | 23 |
| `x > w - (insetX + width)` | 1896 | 97.138965 | 39 |
| flat `top + lm + height` (16 L/G combos) | 1896 | 97.138965 | 23 |
| `(insetX + width) < w` | 1896 | 89.373297 | 155 |
| volatile `top` | 1892 | 89.495225 | 157 |
| volatile `top` + `lm` | 1892 | 89.495225 | 157 |
| volatile pointer inline for b/c | 1896 | 95.841854 | 117 |
| reload assignment `layout = Globals::layout` | 1892 | 90.313779 | 154 |
| `layout = (Layout *) ((char *) Globals::layout + 0)` | 1892 | 90.313779 | 154 |
| `layout = &*Globals::layout` | 1892 | 90.313779 | 154 |
| volatile reload `*(Layout *volatile *) &Globals::layout` | 1892 | 89.768076 | 157 |
| `fresh = Globals::layout` comma before y | 1892 | 90.313779 | 154 |
| `fresh` + second `if` local | 1888 | 90.102389 | 145 |
| `Globals::layout != 0` null check in `&&` | 1892 | 89.495225 | 150 |
| `Globals::layout == Globals::layout` | 1892 | 90.313779 | 154 |
| dead `(void) layout` after block | 1892 | 90.313779 | 154 |
| dead `(void) layout->field_0x20_top` after block | 1892 | 90.313779 | 154 |
| `Layout **layoutAddr = &Globals::layout` | 1892 | 90.313779 | 154 |
| `Layout *&layoutRef = Globals::layout` | 1892 | 90.313779 | 154 |
| `*(Layout **) &Globals::layout` for c | 1892 | 90.313779 | 154 |
| `*(Layout **) ((char *) &Globals::layout)` for c | 1892 | 90.313779 | 154 |
| local ref a + fresh b/c + local d | 1888 | 90.102389 | 145 |
| `Layout **lp = &Globals::layout` + local a/d | 1888 | 90.102389 | 145 |
| `int left = w - insetX` + width local | 1892 | 88.403820 | 155 |
| `int left` + `lm/top/height` locals | 1904 | 83.774610 | 375 |
| `int left` + direct b/c | 1892 | 90.313779 | 154 |
| `int width = GetImage2DWidth(...)` local, direct | 1884 | 88.797814 | 159 |
| `int top = layout.field_0x20_top` local | 1892 | 90.599455 | 78 |
| `int top` + `int lm` locals | 1888 | 90.115883 | 87 |
| `y` condition first | 1892 | 89.085948 | 174 |
| nested `if` with local ref for a only | 1892 | 90.040928 | 154 |
| `bool hit = ...; scrollbarHit = hit` | 1892 | 89.358799 | 158 |
| ternary `scrollbarHit = cond ? 1 : 0` | 1892 | 89.358799 | 158 |
| `Layout *layout` + `PaintCanvas *canvas` locals | 1884 | 89.480874 | 158 |
| `PaintCanvas *canvas` local, direct layout | 1892 | 90.450205 | 147 |
| `PaintCanvas &canvas` local, direct layout | 1892 | 90.450205 | 147 |
| hoist `Layout &layout` above the two loops | 1920 | 46.289993 | 663 |
| hoist `PaintCanvas *canvas` above the loops | 1892 | 87.678693 | 204 |
| function-scope `Layout &layout` | 1912 | 45.671438 | 650 |
| function-scope `Layout *layout` (all four) | 1908 | 44.976077 | 649 |

The full 16-way `{local, direct}` sweep was re-run for the HEAD
association (`top + (lm + height)`), the flat association
(`top + lm + height`), and six height-first associations. Best cells:
HEAD `LGLG`/`GGLG`/`LLGG`/`GLGG` = 98.092643 (18); flat best =
97.138965 (23); height-first best = 94.414169 (54).

### Case 8 variants

| variant | size | pct | diffs |
|---|---:|---:|---:|
| **HEAD** | 1896 | **98.092643** | **18** |
| `Layout *layout` for `field_0x20_top` in first `if` | 1896 | 98.092643 | 18 |
| `int &width = this->listEntryWidth` (first `if`) | 1896 | 98.092643 | 18 |
| `*(int *) ((char *) this + 0x154)` | 1896 | 98.092643 | 18 |
| `((int *) this)[0x55]` | 1896 | 98.092643 | 18 |
| swap the two `y` conditions | 1896 | 98.092643 | 18 |
| `this->listEntryWidth + 0` / `* 1` / `(int)` | 1896 | 98.092643 | 18 |
| `int width` local, second `if` only | 1896 | 97.820163 | 20 |
| `int width` local, first `if` only | 1896 | 97.002725 | 32 |
| `int width` local, both `if`s | 1892 | 88.207226 | 327 |
| `int width` before first `if`, second uses | 1892 | 88.207226 | 327 |
| `int topY` local, both `if`s | 1892 | 86.162236 | 345 |
| `int topY` local, second `if` only | 1892 | 86.162236 | 345 |
| `int eh` local | 1892 | 81.390593 | 541 |
| `Layout &layout` for all case-8 accesses | 1892 | 88.267394 | 336 |
| `Layout &layout` second `if` only | 1892 | 88.267394 | 336 |
| `Layout &layout` second `if` width only | 1912 | 66.576087 | 348 |

### Case-order and prologue perturbations

| move | size | pct | diffs |
|---|---:|---:|---:|
| `case 8` before `case 12`/`14`/`3`/`17` | 1896 | 98.092643 | 18 |
| `case 8` before `case 9`/`11` | 1896 | 96.866485 | 31 |
| `case 8` before `case 1`/`7`/`4` | 1896 | 96.866485 | 33 |
| `case 8` before `case 16` | 1896 | 96.594005 | 35 |
| `case 16/15` before `case 9`/`11` | 1896 | 98.092643 | 18 |
| `case 16/15` before `case 7`/`4` | 1892 | 91.342877 | 74 |
| `case 16/15` before `case 8` | 1896 | 96.866485 | 30 |
| `case 16/15` before `case 12`/`3`/`17` | 1896 | 96.594005 | 35 |
| `case 3` moved anywhere | 1896 | 98.092643 | 18 |
| `case 11` moved | 1896 | 96.866485 | 30-31 |
| `case 4` before `case 7` | 1896 | 97.547684 | 22 |
| `case 7` moved | 1896 | 97.547684 | 22 |
| `case 1`/`6`/`12`/`14`/`17`/`default` moved | 1896 | 98.092643 or worse | 18+ |
| early returns as `else if` | 1896 | 98.092643 | 18 |
| swap the two early returns | 1896 | 59.536785 | 302 |
| final `if` operands swapped | 1896 | 80.381471 | 294 |
| final `if` nested | 1896 | 98.092643 | 18 |

## 5. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w6716
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

Post-RA MIR:

```bash
cd cmake-build-match
CMD=$(python3 -c "import json;d=json.load(open('compile_commands.json'));print([x['command'] for x in d if x['file'].endswith('MenuTouchWindow.cpp')][0])")
eval "$CMD -mllvm -print-after-all" > /dev/shm/w6716/mir_all.txt 2>&1
# extract the 'IR Dump After Virtual Register Rewriter' section containing
# _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

## 6. Verdict

The residual is a greedy register-allocation tie-break, not a source-shape
difference. HEAD already reproduces the original's address registers
(`r4 = &Globals::layout`, `fp = &Globals::Canvas`). The original reloads
the layout value into `r2` after the first call; HEAD caches it in a
callee-saved register (`r6` in the final disassembly; the VRR dump in §3
still shows the pre-scheduling `r5`).
Every source shape that makes the second condition's `top` direct also
makes clang promote `&Globals::layout` to `r11`, changing the function
size to 1892 B and cascading the branch targets (154 diff lines). Every
shape that keeps the size at 1896 B either caches `top` (HEAD) or caches
`leftMargin` (LLGG), both 18 diff lines. No legal C++ variant tested this
wave improves 98.092643 %. The next lever would have to perturb global
liveness from outside case 15 while keeping the 700+ already-matching
instructions, or a different clang-7 scheduling outcome.
