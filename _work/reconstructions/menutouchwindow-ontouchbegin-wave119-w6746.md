# MenuTouchWindow::OnTouchBegin wave 119 (w6746): RA-liveness / operand-order sweep, no source win

Date: 2026-11-05. Worktree `wt/w6746`, branch `wave119-w6746`, base
`b5cbf56bd`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, `bool MenuTouchWindow::OnTouchBegin(int,
int, void*)`). Baseline and best measured: **98.092643 %**, 1896 B,
734/734 normalised instructions, `byte=False`, `linked=False`, **18**
differing instruction lines.

Gate at commit: `byte_exact 4505 / linked_exact 4505`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**. 27
sections, `.so` exactly 2,187,328 B. No section/extab change.

## TL;DR

Same 18 residual lines in 3 regions as waves 118/119-w6716; every one is a
register rename. This wave re-ran the full 192-cell shape sweep plus new
operand-order, exotic-spelling, comma-operator and case-8 association
sweeps. **No legal C++ variant beats 98.092643 %.** The new information is
a *third* codegen family (`Td + (Lc + H)`, 97.956403 %, 18 diffs) that
reproduces the original's address handling exactly (`fp = &Globals::Canvas`,
`&Globals::layout` rematerialised from `[sp,#16]` in both later blocks) but
caches `leftMargin` instead of `top`; the original caches neither.

## 1. The residual (unchanged from wave 119-w6716)

`tools/verify/verify.py --show` diff (orig | ours), function at `0x129bd4`,
1896 B both sides, 734 instructions both sides:

### Region A — `case 8`, first button `if`, 4th `y` condition

```
ldr.w  ip, [sl, #340]        |  ldr.w  r4, [sl, #340]     ; this->listEntryWidth
add    r3, ip                |  add    r3, r4
```

The same expression in `case 3` (iPad branch) is byte-identical and uses
`ip` in both; the second `case 8` `if` uses `r2` in both. Pure RA
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
ldr.w  r1, [r0, r1, lsl #2]  |  ldr    r4, [r6, #32]      ; layout->field_0x20_top (cached)
ldr    r2, [r2, #0]          |  ldr.w  r1, [r0, r1, lsl #2]
ldr.w  r0, [fp]              |  ldr    r2, [r2, #0]
ldr    r5, [r2, #12]         |  ldr.w  r0, [fp]
ldr    r6, [r2, #32]         |  ldr    r5, [r2, #12]      ; layout->field_0xc_leftMargin
adds   r1, r6, r5            |  adds   r1, r5, r4
ldr    r5, [r2, #40]         |  ldr    r4, [r2, #40]      ; third if: layout->buttonInsetX
add    r0, r5                |  add    r0, r4
```

## 2. HEAD source shape (unchanged)

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

## 3. The three codegen families (post-RA MIR / disassembly)

All three share the same IR *instruction order* in the first condition
(heapBufB, field_0x1e0, Canvas-addr, layout-value, elem, w, Canvas,
buttonInsetX) and in the second condition (heapBufB, field_0x1e0,
&layout, elem, layout-value, Canvas, leftMargin, top, call). They differ
only in which values are kept in callee-saved registers across the calls.

| family | size | fp | layout addr | layout value | Region C |
|---|---:|---|---|---|---|
| **original** | 1896 | `&Globals::Canvas` | remat `[sp,#16]` ×2 | fresh ×3 | both fields fresh |
| **HEAD (`LLGG`/`GLGG`)** | 1896 | `&Globals::Canvas` | remat `[sp,#16]` ×1 | cached `r6` across 1st call | `top` cached, `leftMargin` fresh |
| **`LGLG`/`GGLG` (`Td+(Lc+H)`)** | 1896 | `&Globals::Canvas` | remat `[sp,#16]` ×2 | cached `r6` | `leftMargin` cached, `top` fresh |
| **all-direct (`LGGG`/`GGGG`)** | 1892 | `&Globals::layout` | one load in bb.37, live to bb.47 | fresh ×3 | both fresh |

`Td + (Lc + H)` is the closest structural match to the original: its
address is rematerialised from `[sp,#16]` in both the second and third
blocks (`ldr r2,[sp,#16]; ldr r2,[r2,#0]`) and `fp = &Globals::Canvas`,
exactly like the original. It scores 97.956403 % (18 diffs) because
`leftMargin` is loaded early from the cached `r6` instead of late from
the fresh value.

The all-direct family is 4 B short (1892 B, 732 insns): clang keeps
`&Globals::layout` in `fp` across both calls, so the two
`ldr r2,[sp,#16]` rematerialisations disappear. Post-RA VRR for the
all-direct build:

```
bb.37:  $r11 = t2LDRi12 %stack.0, 0     ; &Globals::layout, live to bb.47
bb.42:  $r2 = t2LDRi12 $r11, 0          ; layout value
bb.44:  $r2 = t2LDRi12 $r11, 0          ; layout value
bb.47:  $r2 = t2LDRi12 $r11, 0          ; layout value
```

HEAD instead caches the layout *value* (`$r6 = [$r4]` in bb.42, reused in
bb.44) and rematerialises the address only for the third `if`. The
original caches neither: it rematerialises the address in bb.42, bb.44
and bb.47 and loads the value fresh each time.

The remaining lever is therefore the greedy allocator's live-range
splitting: with `r4/r5/r6` all occupied (HEAD) or with a cached value in
`r6` (LGLG) the address is split at the calls; with a spare callee-saved
register (all-direct) it is kept in `fp`. No source spelling tested can
occupy a register *without* also changing which field is cached.

## 4. Variants measured this wave

All measurements are full gold link + per-function verify. "diffs" is the
number of `|` structural lines; lower is better. Every variant below was
reverted; HEAD is unchanged.

### Full 192-cell shape sweep

`decl ∈ {none, ref, ptr}` × `(a,b,c,d) ∈ {L,G}^4` (a = first-condition
`buttonInsetX`, b = `field_0x20_top`, c = `field_0xc_leftMargin`,
d = third-if `buttonInsetX`) × `assoc ∈ {paren, flat}` ×
`order ∈ {top-first, leftMargin-first}`. Best cells:

| decl | a b c d | assoc | size | pct | diffs |
|---|---|---|---:|---:|---:|
| ref | LLGG | paren | 1896 | **98.092643** | **18** |
| ref | GLGG | paren | 1896 | 98.092643 | 18 |
| ref | LGLG | paren | 1896 | 97.956403 | 18 |
| ref | GGLG | paren | 1896 | 97.956403 | 18 |
| ref | LLGG/GLGG/LGLG/GGLG | flat | 1896 | 97.138965 | 23 |
| ref | LGGG/GGGG | paren/flat | 1892 | 90.313779 | 154 |
| ref | LGGL/GGGL | paren/flat | 1888 | 90.102389 | 145 |
| ref | LLLG/GLLG | paren | 1892 | 90.859482 | 153 |
| ptr | same ranking as ref | | | | |

`none` (all-direct) is only legal for `GGGG`.

### Second-condition operand order (10 associations × a,d)

`T = Globals::layout->field_0x20_top`, `L = Globals::layout->field_0xc_leftMargin`,
`H = Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])`.

| expression | size | pct | diffs |
|---|---:|---:|---:|
| `T + (L + H)` | 1892 | 90.313779 | 154 |
| `T + L + H` | 1892 | 90.313779 | 154 |
| `L + T + H` | 1892 | 90.313779 | 154 |
| `L + (T + H)` | 1892 | 90.313779 | 154 |
| `(T + L) + H` | 1892 | 90.313779 | 154 |
| `(L + T) + H` | 1892 | 90.313779 | 154 |
| `(L + H) + T` | 1892 | 88.949523 | 159 |
| `L + (H + T)` | 1892 | 88.949523 | 159 |
| `H + (T + L)` | 1892 | 88.403820 | 155 |
| `(H + L) + T` | 1892 | 88.403820 | 155 |

All-direct is always 1892 B regardless of association/order.

### Exotic spellings (all-direct and with a `Layout &layout` decl)

`(*Globals::layout).f`, `Globals::layout[0].f`, `(&Globals::layout)[0]->f`,
`(*&Globals::layout)->f`, plain `Globals::layout->f` — all produce
**1892 B / 90.313779 % / 154 diffs**, with or without the unused
`Layout &layout` declaration. The IR is identical for every spelling.

### Comma-operator / re-load shapes

| shape | size | pct | diffs |
|---|---:|---:|---:|
| `Layout *layout = Globals::layout;` + `(layout = Globals::layout, layout->top + (layout->lm + H))`, d direct | 1892 | 90.995907 | 140 |
| same, d cached | 1892 | 90.313779 | 154 |
| `(layout = &*Globals::layout, ...)` | 1892 | 90.313779 | 154 |
| `((layout = Globals::layout) ? ... : 0)` | 1896 | 88.010899 | 142 |

### `case 8` association variants (4th `y` condition)

`lm = Globals::layout->field_0xc_leftMargin`, `top = ...field_0x20_top`,
`w = this->listEntryWidth`.

| expression | size | pct | diffs |
|---|---:|---:|---:|
| `lm + top + w` (HEAD) | 1896 | **98.092643** | **18** |
| `lm + (top + w)` | 1896 | 98.092643 | 18 |
| `(lm + top) + w` | 1896 | 98.092643 | 18 |
| `top + w + lm` | 1896 | 98.092643 | 18 |
| `w + lm + top` | 1896 | 97.956403 | 19 |
| `w + (lm + top)` | 1896 | 97.956403 | 19 |
| `lm + w + top` | 1896 | 97.956403 | 19 |

Region A's `ip` vs `r4` is unchanged by every one of these.

## 5. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w6746
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
eval "$CMD -mllvm -print-after-all" > /dev/shm/w6746/mir_all.txt 2>&1
# extract the 'IR Dump After Virtual Register Rewriter' section containing
# _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

## 6. Verdict

The residual is a greedy register-allocation/live-range-splitting
tie-break, not a source-shape difference. The original has *neither* the
layout address nor the layout value live across the first call: it
rematerialises `&Globals::layout` from `[sp,#16]` in all three blocks and
loads the value fresh each time. Every legal source shape that makes both
`top` and `leftMargin` fresh (all-direct) also lets clang keep
`&Globals::layout` in `fp` across both calls, costing 4 B and cascading
every intra-function branch target (154 diff lines). Every shape that
keeps the 1896 B size caches one field in `r6` (HEAD caches `top`,
`Td+(Lc+H)` caches `leftMargin`), both 18 diff lines. No legal C++ variant
tested this wave improves 98.092643 %. The next lever would have to
perturb global liveness from outside case 15 while keeping the 700+
already-matching instructions, or a different clang-7 scheduling outcome.
