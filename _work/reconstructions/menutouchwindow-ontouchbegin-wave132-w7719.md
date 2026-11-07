# MenuTouchWindow::OnTouchBegin wave 132 (w7719): exact original IR identified; residual is one Greedy-RA remat

Date: 2026-11-07. Worktree `wt/w7719`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` (`src/game/ui/MenuTouchWindow.cpp`),
1896 B, 18 structural diff lines at HEAD. Extends
`menutouchwindow-ontouchbegin-wave129-w7659.md` (and waves 118-128).

## 1. New result: the source shape that emits the *exact* original IR

Wave 129 concluded "no legal C++ shape produces `%29` dead + `%99` remat". This wave
found that the shape **does** exist and is simply the all-direct form of the case-15
block:

```cpp
{
    int right = Globals::w - Globals::layout->buttonInsetX;
    if (right - Globals::Canvas->GetImage2DWidth(this->heapBufB[this->field_0x1e0]) < x &&
        y < Globals::layout->field_0x20_top + Globals::layout->field_0xc_leftMargin +
                Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0]))
        this->scrollbarHit = 1;
    else
        this->scrollbarHit = 0;
    if (x < Globals::layout->buttonInsetX + Globals::Canvas->GetImage2DWidth(this->heapBufA[0])) { ... }
}
```

This is exactly the Ghidra decompilation of case 15 (`iVar13 = Globals::w -
*(int *)(Globals::layout + 0x28)` then `iVar13 - iVar9 < param_2`, and a *fresh*
`Globals::layout` load for both `+0xc` and `+0x20`). Its post-RA MIR at `bb.44` is

```
%319 = t2LDRi12 %508, 0     ; fresh Globals::layout
%322 = t2LDRi12 %319, 12    ; leftMargin
%323 = t2LDRi12 %319, 32    ; top
```

which is instruction-for-instruction the original (`ldr r2,[sp,#16]; ldr r2,[r2,#0];
ldr r5,[r2,#12]; ldr r6,[r2,#32]`). **The IR is no longer the residual.**

## 2. The residual is the Greedy-RA choice on `%508` alone

`%508` is the reload `%508 = t2LDRi12 %stack.0, 0` of the spilled `&Globals::layout`
GOT address. It is rematerialisable (a load from a stack slot).

* HEAD (and the original): `%508` is remat'd at `bb.44`/`bb.47`
  (`%511 = [%stack.0]`, `%510 = [%stack.0]`); `fp` = `Globals::Canvas` value.
* The exact-IR shape (variant `D`): the allocator keeps `%508` live in `fp` across both
  calls and moves the Canvas value to `r5`; function shrinks to 1892 B / 164 diffs.

So the last remaining decision is a single Greedy-RA remat-vs-keep tie-break on `%508`,
not a source-shape problem. In HEAD the tie-break goes the right way only because `%29`
(the cached `Globals::layout` value) is live across the first call and raises pressure;
the original has `%29` dead and still remats `%508`.

## 3. Variants measured this wave (all reverted; 1896 B unless noted)

| variant | diffs | note |
|---|---:|---|
| HEAD (committed) | **18** | `%29` live, `top` cached |
| `D` = Ghidra-faithful all-direct + `right` local | 165 (1892 B) | exact original IR; `%508` kept in fp |
| `E` = faithful without `right` local | 165 (1892 B) | same |
| `A` = `right` local, `layout` ref for c1 only | 165 (1892 B) | same |
| `H` = `top` fresh, `leftMargin` from `layout` ref | 18 (1896 B) | `%29` live, RA same as HEAD |
| `J` = `lm` cached before call, `top` fresh | 79 (1892 B) | RA right, extra `add.w r1,r4,fp` |
| `K` = `top` cached before call, `leftMargin` fresh | 79 (1892 B) | mirror of J |
| `I` = comma `(layout.field_0xc_leftMargin, ...)` | 165 (1892 B) | load elided |

## 4. Conclusion

The exact original IR is reachable (`D`). What is left is a single allocator tie-break:
make Greedy-RA remat `%508` while `%29` is dead. Since `%29` liveness is the only lever
that flips it, the remaining search space is "keep `%29` live across the first call
without emitting a use of it". No such shape was found this wave; the committed HEAD
(18 diffs, 1896 B) is unchanged.

Gate before/after: byte 4506 -> 4506, linked 4506 -> 4506, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.
