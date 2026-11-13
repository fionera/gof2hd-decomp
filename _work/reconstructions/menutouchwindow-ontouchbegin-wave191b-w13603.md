# MenuTouchWindow::OnTouchBegin wave 191b (w13603): fresh case-15/case-8 shapes, no win

Date: 2026-11-13. Worktree `wt/w13603`, branch `wave191b-w13603`, base
`49a44a262`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`). Baseline and best measured this wave:
**raw 28 / aligned 16 / size 1896/1896 / 754/754 insns**. No source shape
tested beat HEAD; the tree is clean and the gate is unchanged.

Gate at HEAD and after this wave: `RATCHET PASS` — byte_exact **4508/4524**,
linked_exact 4508/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, symbol parity 5370/5370. `.text` size 431,168 identical, `.ARM.extab`
size 468,168 with **577** differing bytes (unchanged), `.ARM.exidx` 577,
`.rodata` 0. `.rel.plt` byte-identical.

## 1. Exact byte-diff map at HEAD (28 bytes)

| region | file offset(s) | original | ours |
|---|---|---|---|
| A (case 8) | `0x401`, `0x404` | `ldr.w ip,[sl,#340]` / `add r3,ip` | `ldr.w r4,...` / `add r3,r4` |
| B (case 15 c1) | `0x60f`,`0x615`,`0x61c-0x622`,`0x628-0x62b`,`0x630` | `r3=heapBufB`, `r6=field_0x1e0`, `r5=w`, `r2=layout value` | `r2/r3/r4/r6` permutation |
| C (case 15 c2) | `0x640-0x64d`,`0x652` | fresh `leftMargin`+`top` after the call | `leftMargin` cached before the call, `top` fresh |
| D (case 15 c3) | `0x672`,`0x678` | `r5=buttonInsetX` | `r4=buttonInsetX` |

All four regions are pure register-allocation differences; the instruction
streams are otherwise identical.

## 2. New MIR evidence (pre-RA, `-stop-after=machine-scheduler`)

`%98` is the GOT-loaded address `&Globals::layout` (`tLDRLIT_ga_pcrel` +
`t2LDRi12 ...,0` at bb.1). It is used at **seven** sites across the whole
function (lines 588, 823, 1058, 1098, 1130, 1408, 1590 of the MIR). The
register allocator spills it to `[sp,#16]` and reloads it at each use.

`%304` is `&Globals::Canvas` (`tLDRLIT_ga_pcrel` + `t2LDRi12 ...,0` in bb.42),
used at three sites (bb.42/44/47).

* **HEAD**: `%29` (the cached `Globals::layout` value, used for `leftMargin`)
  is live across the first call. `%98` is reloaded at bb.44 and bb.47
  (`ldr r2,[sp,#16]`), `fp = &Globals::Canvas`. 1896 B.
* **D (all-direct)**: `%29` is dead. `%98` is kept in `fp` (r11) across the
  whole case-15 region, `&Globals::Canvas` moves to `r5`. Only 6 `[sp,#16]`
  loads instead of 8 -> 1892 B.
* **Original**: `%29` dead **and** `%98` reloaded at bb.44/bb.47 **and**
  `fp = &Globals::Canvas`. The third combination.

So the residual is exactly the Greedy-RA remat-vs-keep tie-break on `%98`,
coupled to `%29` liveness, as recorded in wave 132/191. This wave adds the
observation that `%98`'s spill/reload pattern is *identical between HEAD and
the original at all eight `[sp,#16]` sites*; only the case-15 `leftMargin`
read (region C) separates them.

## 3. Variants measured this wave (all reverted; 1896 B unless noted)

Fresh angles (a) fresh-load without a live `Layout&`, (b) pin `Globals::Canvas`
in `fp`, (c) case-8 `listEntryWidth` term.

### 3.1 case 15 -- all-direct / local shapes (angle a,b)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`lm` cached, `top` fresh) | **28** | **16** | 1896 |
| D all-direct (`blk(c1+c2+tail)`) | 362 | 76 | 1892 |
| canvas local at block start, all three uses | 359 | 75 | 1892 |
| canvas local at block start, c3 only | 359 | 77 | 1892 |
| canvas local at block start, c2+c3 | 359 | 75 | 1892 |
| canvas local after the first `if`, c3 only | 362 | 76 | 1892 |
| `PaintCanvas *canvas;` before loops + assign before loops | 424 | 102 | 1892 (755) |
| `PaintCanvas *canvas;` before loops, no assign | 398 | 83 | 1876 (744) |
| `canvas = Globals::Canvas;` after scrollEntries loop | 373 | 88 | 1888 (753) |
| `Layout *lp;` before loops + assign before loops | 1781 | 843 | 1916 (753) |
| `canvas`+`lp` before loops | 1786 | 796 | 1916 (753) |
| `canvas` before loops, used c3 only | 445 | 96 | 1900 (756) |
| function-top `PaintCanvas *canvas`, c3 only | 1711 | 420 | 1908 (755) |
| function-top `PaintCanvas *canvas`, all uses | 1701 | 390 | 1896 (753) |
| function-top `PaintCanvas *canvas`, c1+c2 | 1702 | 348 | 1912 (757) |
| `__builtin_expect` on c1 | 371 | 106 | 1892 |
| `__builtin_expect` on c2 | 362 | 76 | 1892 |
| `Layout &layout` for c3 only (fresh c1/c2) | 322 | 74 | 1888 |
| `Layout &layout` for c3 + `top` | 292 | 73 | 1892 |
| `Layout &layout` for c3 + `lm` | 291 | 74 | 1892 |
| `Layout &layout` for c1 only | 362 | 76 | 1892 |
| `Layout *lp` for c1 + `top` (**angle b best**) | **29** | **15** | 1896 |
| `Layout *lp` for c1 + `lm` | 28 | 16 | 1896 |
| `Layout *lp` for c3 only | 322 | 74 | 1888 |
| `Layout *lp` for c3 + `top` | 292 | 73 | 1892 |
| `Layout *lp` for c3 + `lm` | 291 | 74 | 1892 |
| `volatile` read of `Globals::layout` in c2 | 127 | 76 | 1896 (753) |
| `volatile` read of `Globals::Canvas` in c1 | 364 | 85 | 1892 |
| `(Layout &) layout` / `*(Layout *)&layout` / second ref / ptr-to-member for `lm` | 28 | 16 | 1896 |
| second ref for `top` | 29 | 15 | 1896 |
| `Layout *volatile` for `top` | 1763 | 249 | 1924 (766) |
| `Layout *const volatile` for `top` | 1763 | 249 | 1924 (766) |
| `bool xok/yok` split statements | 874 | 140 | 1904 (756) |
| `x + width + inset > w` form | 355 | 92 | 1884 (750) |
| `x - w + width + inset > 0` form | 376 | 88 | 1892 |
| `w - (inset + width)` | 365 | 85 | 1892 |
| `w - width - inset` (width before inset) | 345 | 85 | 1884 (750) |
| `w - (inset + width)` / `(w - inset) - width` / `w - width - inset` | 362-365 | 76-85 | 1892 |
| `*(heapBufB + field_0x1e0)` | 362 | 76 | 1892 |
| `unsigned tex = heapBufB[field_0x1e0]` local | 374 | 86 | 1892 |
| `unsigned idx` local | 378 | 78 | 1888 (751) |
| `unsigned *hb` local | 379 | 82 | 1888 (751) |
| `canvas`+`tex` locals | 367 | 83 | 1892 |
| `tex` + `lm` via ref | 351 | 76 | 1888 (753) |
| `tex` + `top` via ref | 351 | 76 | 1888 (753) |
| `lp = Globals::layout` reassigned inside the condition | 362 | 76 | 1892 |
| empty `{}` / `do{}while(0)` / `if(1){}` wrappers | 362 | 76 | 1892 |
| no inner braces (all-direct) | 362 | 76 | 1892 |
| `(void) layout;` after a dead ref | 362 | 76 | 1892 |
| `layout.field_0xc_leftMargin == layout.field_0xc_leftMargin` no-op | 362 | 76 | 1892 |

### 3.2 case 15 -- c2 association / operand order

| c2 expression | raw | aligned | size |
|---|---:|---:|---:|
| `top + (lm + height)` (D) | 362 | 76 | 1892 |
| `top + lm + height` | 362 | 76 | 1892 |
| `lm + top + height` | 362 | 76 | 1892 |
| `height + top + lm` | 369 | 111 | 1892 |
| `height + lm + top` | 370 | 111 | 1892 |
| `lm + height + top` | 380 | 87 | 1892 |
| `top + height + lm` | 380 | 87 | 1892 |
| `height + (top + lm)` | 369 | 111 | 1892 |
| `lm + (top + height)` | 362 | 76 | 1892 |
| `y - top - lm < height` | 358 | 98 | 1892 (751) |
| `y - (top + lm) < height` | 358 | 98 | 1892 (751) |

### 3.3 case 15 -- systematic cached/fresh sweep (ref `layout.` vs `Globals::layout->`)

16 masks over `{c1-inset, c2-lm, c2-top, c3-inset}`:

| mask (bits 1/2/4/8) | raw | aligned | size |
|---|---:|---:|---:|
| `02` (lm cached, rest fresh) = HEAD | **28** | **16** | 1896 |
| `03` (c1+lm cached) | 28 | 16 | 1896 |
| `04` (top cached) | 29 | 15 | 1896 |
| `05` (c1+top cached) | 29 | 15 | 1896 |
| `06` (lm+top cached) | 308 | 73 | 1892 |
| `08` (c3 cached) | 322 | 74 | 1888 |
| `10` (c1+c3 cached) | 291 | 74 | 1892 |
| `12` (lm+c3 cached) | 292 | 73 | 1892 |
| `00` (all fresh) = D | 362 | 76 | 1892 |
| `01`,`07`,`09`,`11`,`13`,`14`,`15` | 291-362 | 73-76 | 1888-1892 |

### 3.4 case 8 -- region A (`listEntryWidth` r4 vs ip) (angle c)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **28** | **16** | 1896 |
| `top + lm + listEntryWidth` (top first) | 28 | 16 | 1896 |
| `lm + (top + listEntryWidth)` | 28 | 16 | 1896 |
| `(lm + top) + listEntryWidth` | 28 | 16 | 1896 |
| `(int) listEntryWidth` / `-(-listEntryWidth)` / `* 1` | 28 | 16 | 1896 |
| `(unsigned int) listEntryWidth` | 29 | 17 | 1896 |
| `listEntryWidth + lm + top` (lw first) | 33 | 17 | 1896 |
| `Globals::layout` fresh in the y condition | 28 | 16 | 1896 |
| `x > buttonInsetX + listTopY` (comparison reversed) | 28 | 16 | 1896 |
| y-condition first in the `&&` chain | 73 | 41 | 1896 |
| `y - listEntryWidth < lm + top` | 827 | 46 | 1896 (753) |

The case-8 block does not use `r4` at all in the original, and does not use it
before `0x3fe` in ours either; both `ip` and `r4` are free at that point. The
choice is a pure allocator tie-break, not a liveness difference.

## 4. Conclusion

Every shape tested this wave lands in one of two buckets:

* `%29` live across the first call -> 1896 B, `fp = &Globals::Canvas`, `%98`
  reloaded; raw 28 (HEAD) or 29 (`top` cached).
* `%29` dead -> 1892 B (or smaller), `%98` kept in `fp`, `&Globals::Canvas` in
  `r5`; raw 291-362.

The original's combination (`%29` dead, `%98` reloaded, `fp = &Globals::Canvas`)
was not reachable by any of the ~120 new shapes. The residual remains the
Greedy-RA remat-vs-keep tie-break on `%98` and the independent case-8
`ip`/`r4` tie-break. HEAD is unchanged.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13603
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# raw=28 aligned=16 size=1896/1896 insn=754/754
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13603` (sweep harnesses
`v1b.py`..`v19b.py`, `sweep2.py`, MIR dumps `Dpre.mir`, `D2.ll`).
