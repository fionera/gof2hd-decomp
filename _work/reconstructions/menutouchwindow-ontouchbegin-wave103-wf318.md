# MenuTouchWindow::OnTouchBegin — wave 103 (wf318)

Date: 2026-11-04. Branch `wave103-wf318` (base master `1d2b4398e`).
Gate before/after: **byte_exact 4500 / linked_exact 4500 / size-exact 4524**,
stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, **RATCHET PASS**. No code change landed;
this note records the wave-103 source-shape search and why the residual is
not reachable from plain C++ with clang 7.0.2 `-Oz`.

## Target state (re-measured)

| symbol | size orig/ours | fuzzy | `|` lines (incl. header) | structural diff lines |
|---|---|---|---|---|
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 / 1896 | **97.683924 %** | 27 | **26** |

`verify.py --show` header line also contains `|`, so the handoff's "27 diff
lines" is 26 structural lines + 1 header. The function is size-exact; there is
no missing code, wrong constant, wrong field offset or wrong callee.

## Commands

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Every variant below was applied to the case-15 block of
`src/game/ui/MenuTouchWindow.cpp`, the whole `.so` was relinked, and the
official verifier was re-run. `diffs` below is the number of `|` structural
lines **excluding** the header (so the current baseline is 26). Ratio is
`difflib.SequenceMatcher(None, orig, ours, autojunk=False).ratio()*100` over
the reconciled instruction lists (734 instructions each at the baseline).

## Residual regions

1. **Case 8** (`upButtonPressed` / `downButtonPressed`), VMA `0x129fd2`:
   original `ldr.w ip,[sl,#340]` / `add r3,ip`; ours `ldr.w r4,[sl,#340]` /
   `add r3,r4`. `r4` is dead in the original's case-8 block; the allocator
   still picks `ip` (r12) for the short-lived `listEntryWidth` temporary.
   Reordering the sum (`top+left+w`, `top+w+left`, `w+top+left`) and a local
   `listEntryWidth` copy do not change the choice (26 or 27 lines).

2. **Case 15/16** (`scrollbarHit` + drag init), `0x12a1da..0x12a24e`.
   Original (three separate `Globals::layout` accesses, all rematerialised
   from the `&Globals::layout` spill at `[sp,#16]`):

```
12a1bc  ldr   r4,[sp,#16]         ; &Globals::layout
...
12a1f0  ldr   r2,[r4,#0]          ; layout
12a1fc  ldr   r2,[r2,#40]         ; buttonInsetX
12a1fe  subs  r5,r5,r2            ; w - buttonInsetX
12a200  blx   GetImage2DWidth
12a204  subs  r0,r5,r0            ; (w-inset) - width
...
12a212  ldr   r2,[sp,#16]         ; &Globals::layout (reload)
12a218  ldr   r2,[r2,#0]          ; layout
12a21e  ldr   r5,[r2,#12]         ; leftMargin
12a220  ldr   r6,[r2,#32]         ; top
12a222  blx   GetImage2DHeight
12a226  adds  r1,r6,r5            ; top + leftMargin
12a228  add   r0,r1               ; height + (top+left)
...
12a23c  ldr   r0,[sp,#16]         ; &Globals::layout (reload)
12a240  ldr   r2,[r0,#0]          ; layout
12a246  ldr   r5,[r2,#40]         ; buttonInsetX
12a248  blx   GetImage2DWidth
12a24c  add   r0,r5               ; width + inset
```

   The current source is the best measured shape:

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
        ...
    }
}
```

   Ours keeps the layout **value** in `r6` across the first call and spills
   `top` to `[sp,#12]`; the original keeps `&Globals::layout` in `[sp,#16]`,
   rematerialises it for every access, and keeps `top` in `r6` across the
   height call.

## Wave-103 sweep (new shapes, all measured)

| variant | size | diffs | ratio |
|---|---:|---:|---:|
| **current** (`Layout&` + hoisted `top`, `h+t+l`) | **1896** | **26** | **97.6839** |
| `h+l+t` order, same shape | 1896 | 26 | 97.6839 |
| `t+h+l` order, same shape | 1896 | 26 | 97.6839 |
| `t+l+h` order, same shape | 1896 | 28 | 97.5477 |
| `l+t+h` order, same shape | 1896 | 28 | 97.5477 |
| `l+h+t` order, same shape | 1896 | 27 | 97.6839 |
| `const Layout&`, `Layout *const`, `auto&`, `long top`, `top` before `layout`, `top = layout.top` | 1896 | 26 | 97.6839 |
| `int top` only + direct `Globals::layout->` fields | 1896 | 67 | 93.5967 |
| `Layout&` + direct `leftMargin` in second condition | 1896 | 66 | 93.5967 |
| `Layout&` + `Layout *volatile &lp` for second condition | 1896 | 66 | 93.5967 |
| `Layout *volatile *lp = &Globals::layout` (all accesses) | 1896 | 126 | 95.4329 |
| `Layout *volatile &lp = Globals::layout` (all accesses) | 1896 | 126 | 95.4329 |
| `Layout *volatile *lp` + comma-assigned `top` (`t+l+h`) | 1896 | 126 | 95.4329 |
| `pp_volatile_ptr` (`Layout *volatile *lp`, `t+l+h`) | 1896 | 126 | 95.4329 |
| `ref_first_direct_second_tlh_topvar` (`Layout&` + hoisted `top` + `t+l+h`, direct second) | 1892 | 80 | 89.7820 |
| `ref_volcast_left` (`*(Layout *volatile *)&Globals::layout` for leftMargin) | 1892 | 79 | 90.1907 |
| `volcast_first_ref_left` (volatile cast for buttonInsetX) | 1892 | 79 | 90.1907 |
| `ref_volcast_both` | 1892 | 79 | 90.1907 |
| oracle comma shape (`right`, `width`, `left`, `top`, `height` locals) | 1892 | 156 | 90.3138 |
| oracle comma shape, `bool hit = …` | 1896 | 154 | 90.1024 |
| `right_width_tlh` (`right`/`width` locals, `t+l+h`) | 1892 | 156 | 90.3138 |
| `right_width_htl` (`right`/`width` locals, `h+t+l`) | 1892 | 157 | 88.4038 |
| `nested_right_then_top_left` (nested `if`, fresh locals) | 1896 | 154 | 90.1024 |
| `nested_fresh_local_tlh` (fresh `Layout *layout2` in inner block) | 1896 | 154 | 90.1024 |
| `hoist_top_left` (`h+t+l`, both hoisted) | 1888 | 88 | 90.1159 |
| `hoist_top_left_tlh` (`t+l+h`, both hoisted) | 1892 | 79 | 90.1907 |
| `canvas_ptr_direct` (`PaintCanvas *canvas`, direct layout) | 1892 | 156 | 88.4799 |
| `canvas_volatile_pp` (`canvas` + `Layout *volatile *lp`) | 1892 | 99 | 89.7069 |
| `w_canvas_tlh` (`int w`, `canvas`, direct layout) | 1892 | 93 | 90.5249 |
| `ref_canvas_top_htl` (`Layout&`, `canvas`, hoisted `top`) | 1896 | 151 | 88.4196 |
| `hoist_w_direct_htl` (`int w`, direct layout) | 1892 | 157 | 88.4038 |
| `hoist_right_direct_htl` (`int right`, direct layout) | 1892 | 157 | 88.4038 |
| `ref_hoist_w_htl` (`Layout&`, `int w`, direct second) | 1892 | 157 | 88.4038 |
| `ref_hoist_w_tlh` | 1892 | 156 | 90.3138 |
| `ref_hoist_right_htl` | 1892 | 157 | 88.4038 |
| `ref_hoist_right_tlh` | 1892 | 156 | 90.3138 |

Earlier wave-90 measurements (`Layout *layout`, `Layout *const`, `Layout **`,
`Layout *&`, `volatile Layout&`, `Layout *volatile layout`, `bool hit`,
`scrollbarHit = cond1 && cond2`, `if (!(…))`, local `w`, `PaintCanvas *&`,
`PaintCanvas **`, 4×4 local/direct matrix, case-8 reorder matrix) are in
`menutouchwindow-ontouchbegin-modstation-pass2-w4019.md`; all were re-checked
and none beats 26 lines / 1896 B.

## Why the faithful direct shape is 4 bytes short

The faithful source (three direct `Globals::layout` accesses, `top+left+height`
order) compiles to **1892 B / 156 lines**. Its case-15 code is instruction-for-
instruction identical to the original except for one `ldr`:

```
orig  12a212  ldr r2,[sp,#16]     ; &Globals::layout
      12a218  ldr r2,[r2,#0]      ; layout
ours  12a212  ldr.w r2,[fp]       ; fp = &Globals::layout, kept across the call
```

The original treats `&Globals::layout` as a rematerialisable spill; clang 7.0.2
in our build keeps it in `fp` (direct shape) or keeps the layout value in `r6`
(reference shape). The only shape that reproduces the three `[sp,#16]`
rematerialisations is `ref_first_direct_second_tlh_topvar` (`Layout&` +
hoisted `top` + `top+left+height` order), but it puts `top` in `fp` *before*
the first call and computes `top+left` before the height call, so it is 4 bytes
short (1892) and 80 lines. This is a register-allocation / rematerialisation
choice, not a source-semantics difference: `OnTouchMove` (byte-exact) uses the
same direct `Globals::layout` accesses and clang rematerialises `&layout` from
`[sp,#72]` there, but its layout value is not live across a call.

## Conclusion

`MenuTouchWindow::OnTouchBegin` is source-shape complete. The remaining 26
structural lines are two independent clang 7.0.2 allocator tie-breaks:

- `ip` vs `r4` for a short-lived case-8 temporary;
- `fp`/`r6`/`[sp,#12]` vs `fp`/`r4`/`[sp,#16]` for the case-15 layout
  address and `top` spill.

No plain C++ shape tried (local/direct, reference/pointer/pointer-to-pointer,
volatile pointer/reference, const, declaration order, comma expressions,
nested `if`, fresh locals, canvas/`w`/`right`/`top`/`left` hoists, six
second-condition orders, four first-condition forms) reproduces the original's
exact register assignment while keeping the 1896-byte size. The current shape
is a strict local optimum at **1896 B / 26 structural lines / 97.683924 %**.
