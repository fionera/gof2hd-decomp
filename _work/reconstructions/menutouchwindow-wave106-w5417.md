# MenuTouchWindow::OnTouchBegin — wave 106 (w5417)

Date: 2026-11-04. Worktree `wt/w5417`, branch `wave106-w5417`, base
`486f8a119`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA =
Ghidra - 0x10000).

Target: `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`, size 1896 B (size-exact
throughout; the body differs only in register allocation / instruction
scheduling / block placement).

| variant | size | insns | fuzzy | structural diff lines |
|---|---:|---:|---:|---:|
| **wave-105 baseline (`int top` local + `ref`)** | **1896** | **734** | **97.7** | **26** |
| **wave-106 (this pass)** | **1896** | **734** | **98.1** | **18** |

Gate after the change: `RATCHET PASS`, byte_exact 4500 / linked_exact 4500 /
size 4524 / data 843 / imports 207 / lint `CLEAN ✓` / drift 0. No
`tools/verify_baseline.json` change was needed (byte_exact/linked_exact are
unchanged; the gain is fuzzy-only).

## The change

`src/game/ui/MenuTouchWindow.cpp`, case 15 scrollbar block. The wave-105 shape
hoisted `int top = Globals::layout->field_0x20_top;` as a separate statement,
which clang schedules before the first `GetImage2DWidth` call and spills to
`[sp,#12]`. Removing the local and grouping the second condition as
`top + (leftMargin + height)` makes clang:

* keep `fp = &Globals::Canvas` (as in the original),
* keep the cached `layout` value in `r6` (used for `field_0x20_top`),
* reload `Globals::layout` from the `[sp,#16]` address slot for
  `field_0xc_leftMargin` (matching the original's two-instruction
  `ldr r2,[sp,#16]; ldr r2,[r2,#0]` reload),
* avoid the `top` spill entirely.

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

The residual 18 structural lines are:

1. case 8: `ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]` (2 lines) — the
   allocator's choice for `this->listEntryWidth`; `r4` is free on our side.
2. case 15 first condition: a pure register rename of the original's
   `heapBufB` / `field_0x1e0` / layout-value / `w` / `buttonInsetX` loads
   (8 lines). The cached layout value lives in `r6` (callee-saved) because it
   is reused after the first call; the original reloads it into a caller-saved
   temp (`r2`).
3. case 15 second condition: `ldr r4,[r6,#32]` (cached `top`) vs the original's
   `ldr r6,[r2,#32]` (fresh `top`) plus load-order (6 lines).
4. case 15 third condition: `ldr r4,[r2,#40]` vs `ldr r5,[r2,#40]` (2 lines).

## Measured variants (all 1896 B unless noted)

`ref` = `Layout &layout = *Globals::layout;` used for the access; `dir` =
`Globals::layout->...`; `cached` = `layout.field_0xNN`. Second-condition
expression is `y < <expr>`.

| second-condition `<expr>` | decl / first / third | pct | diffs |
|---|---|---:|---:|
| **`Tc + (Ld + H)`** | **ref / ref / dir** | **98.1** | **18** |
| `Tc + (Ld + H)` | ptr local `lp` / `lp` / dir | 98.1 | 18 |
| `Td + (Lc + H)` | ref / ref / dir | 98.0 | 18 |
| `(Tc + Ld) + H` | ref / ref / dir | 97.1 | 22 |
| `(Td + Lc) + H` | ref / ref / dir | 97.1 | 22 |
| `Td + (Ld + H)` (all direct) | ref / ref / dir | 90.3 | 1892 B |
| `H + Td + Ld` (all direct) | ref / ref / dir | 88.4 | 1892 B |
| `H + top + Lc` (wave-105 `int top`) | ref / ref / dir | 97.7 | 26 |
| `H + Td + Lc` | ref / ref / dir | 94.4 | 54 |
| `H + (Td + Lc)` | ref / ref / dir | 94.4 | 54 |
| `Td + Lc + H` | ref / ref / dir | 97.1 | 24 |
| `Tc + (Lc + H)` (all cached) | ref / ref / dir | 90.9 | 1892 B |
| `Tc + (Ld + H)` | ref / dir / dir | 98.1 | 18 |
| `Tc + (Ld + H)` | ref / ref / ref | 90.9 | 1892 B |

where `H = Globals::Canvas->GetImage2DHeight(this->heapBufB[this->field_0x1e0])`,
`Td/Tc = Globals::layout->field_0x20_top` / `layout.field_0x20_top`,
`Ld/Lc = Globals::layout->field_0xc_leftMargin` / `layout.field_0xc_leftMargin`.

Also measured and rejected: `Layout *volatile` locals (1916-1928 B, 75-76 %),
`volatile Layout *` (1880 B, 89.9 %), `Layout *&` / `Layout **` (1892 B,
88.4 %), `const Layout &` (97.7 %), `short`/`long long` `top` (91.5 % / 90.2 %),
declaring the reference before the scrollSlots loop or at case-15 scope
(1916-1924 B, 44-46 %), and hoisting the image id into a local (1884-1888 B,
88-90 %).

## Proven vs hypothesis

* **Proven from disassembly**: the original's case-15 block uses
  `Globals::layout` directly (three fresh value loads after the two
  `GetImage2D*` calls), keeps `fp = &Globals::Canvas`, and keeps the address
  of `Globals::layout` in the `[sp,#16]` slot (two-instruction
  `ldr [sp,#16]; ldr [r2,#0]` reloads). The first condition computes
  `w - buttonInsetX` before the call and subtracts the width after.
* **Proven from measurement**: the exact source shape that reproduces the
  original's *instruction count* and `fp = &Canvas` is
  `Tc + (Ld + H)`; the all-direct shape (`Td + (Ld + H)`) is 4 B short because
  clang keeps `&Globals::layout` in `fp` and needs only one load per value.
* **Hypothesis**: the remaining 18 lines are allocator tie-breaks (cached
  layout value in `r6` vs a caller-saved temp; `ip` vs `r4` for
  `listEntryWidth`). No source spelling tried this pass moved the cached value
  out of `r6` while keeping the 1896 B size.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5417
TMPDIR=/dev/shm/w5417/tmp cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
TMPDIR=/dev/shm/w5417/tmp cmake --build cmake-build-match --target check -j6

python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

Scratch (not tracked): `/dev/shm/w5417/` (`base.cpp`, `one.py`, `try*.py`,
`rea.json`, `show*.txt`, `*_fn.asm`).
