# MenuTouchWindow::OnTouchBegin wave 191b (w13612): RA tie-break persists, no win

Date: 2026-11-13. Worktree `wt/w13612`, branch `wave191b-w13612`, base
`bdf403963`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`
(`src/game/ui/MenuTouchWindow.cpp`, 1,896 B). Baseline and best measured this
wave: **raw 28 / aligned 16 / size 1896/1896 / 754/754 insns**. No source
shape tested beat HEAD; `src/` is unchanged and the gate is green.

Gate at HEAD and after this wave (`check`): `RATCHET PASS` — byte_exact
**4508/4524**, linked_exact 4508/4524, size 4524/4524, data 843/843, imports
207, lint CLEAN, drift 0, symbol parity 5370/5370, stub_zero_size 0. `.text`
size 431,168, `.ARM.extab` 468,168 with **577** differing bytes (unchanged),
`.ARM.exidx` 577, `.rodata` 0. `.rel.plt` byte-identical.

## 1. Residual map at HEAD (28 bytes, 16 aligned lines)

All four regions are pure register-allocation differences; the instruction
streams are otherwise identical.

| region | file offset(s) | original | ours |
|---|---|---|---|
| A (case 8) | `0x401`, `0x404` | `ldr.w ip,[sl,#340]` / `add r3,ip` | `ldr.w r4,...` / `add r3,r4` |
| B (case 15 c1) | `0x60f`..`0x630` | `r3=heapBufB`, `r6=field_0x1e0`, `r5=w`, `r2=layout value` | `r2/r3/r4/r6` permutation |
| C (case 15 c2) | `0x640`..`0x652` | fresh `Globals::layout` reload; both `leftMargin` (off 12) and `top` (off 32) from it | `leftMargin` from cached `Layout&` (`%29`), `top` fresh |
| D (case 15 c3) | `0x672`,`0x678` | `r5=buttonInsetX` | `r4=buttonInsetX` |

The original's `[sp,#16]` spill/reload pattern is **identical to HEAD**:
8 loads (`0x129d20`, `0x129f84`, `0x129ffa`, `0x12a1bc`, `0x12a212`,
`0x12a23c`, `0x12a2b6`, plus the entry store at `0x129c14`). `fp` is
`&Globals::Canvas` in both (loaded at `0x12a1ec`, dereferenced at
`0x12a1f8/0x12a21c/0x12a242`). HEAD's stack layout and `fp` assignment are
already correct; only the cached `%29` (layout pointer value) and the
resulting RA permutation differ.

## 2. Root cause (unchanged from waves 132/191)

`%98` is `&Globals::layout` (GOT-loaded at entry, spilled to `[sp,#16]`).
`%99 = [%98]` is the `Layout*` value. The case-15 block's `Layout &layout =
*Globals::layout;` creates a *second* load `%29 = [%98]` at bb.42 that is live
across the first `GetImage2DWidth` call (used for `leftMargin` at bb.44).

* **HEAD**: `%29` live across the first call -> `%98` remains spilled,
  `fp = &Globals::Canvas`, raw 28; c2 `leftMargin` is cached.
* **D (all-direct, Ghidra-faithful)**: `%29` dead -> Greedy-RA keeps `%98` in
  `fp` from bb.37 across both calls, `&Globals::Canvas` moves to `r5`, only 6
  `[sp,#16]` loads -> 1892 B, raw 362.
* **Original**: `%29` dead **and** `%98` spilled **and**
  `fp = &Globals::Canvas` — the third combination, not reachable by any source
  shape tested so far.

The pre-RA IR of D at bb.44 is instruction-for-instruction the original
(`%319 = [%508]; %322 = [%319,12]; %323 = [%319,32]`), so the remaining gap is
a Greedy-RA remat-vs-keep tie-break on `%508`, not an IR-shape problem.

## 3. Measurements this wave (all reverted; 1896 B unless noted)

### 3.1 case 15 — cached/fresh masks

Bits: 1 = c1 `buttonInsetX` cached, 2 = c2 `leftMargin` cached,
4 = c2 `top` cached, 8 = c3 `buttonInsetX` cached.

| mask | raw | aligned | size |
|---|---:|---:|---:|
| `02` (lm cached) = HEAD | **28** | **16** | 1896 |
| `03` (c1+lm cached) | 28 | 16 | 1896 |
| `04` (top cached) | 29 | 15 | 1896 |
| `05` (c1+top cached) | 29 | 15 | 1896 |
| `06` (lm+top cached) | 308 | 73 | 1892 |
| `08` (c3 cached) | 322 | 74 | 1888 |
| `10` (c1+c3 cached) | 291 | 74 | 1892 |
| `12` (lm+c3 cached) | 292 | 73 | 1892 |
| `00` (all fresh) = D | 362 | 76 | 1892 |
| `01`,`07`,`09`,`11`,`13`,`14`,`15` | 291-362 | 73-76 | 1888-1892 |

Only masks `02/03` (lm cached) and `04/05` (top cached) keep 1896 B. No mask
with both c2 fields fresh stays at 1896 B.

### 3.2 case 15 — preludes / local forms (all reverted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`Layout &layout`, lm cached, top fresh) | **28** | **16** | 1896 |
| `Layout *lp` for c2 lm only (fresh top/c1/c3) | 28 | 16 | 1896 |
| `Layout *lp` for c1+c2 lm | 28 | 16 | 1896 |
| `Layout *lp` for c1 + top | 29 | 15 | 1896 |
| two refs (`Layout &l1`, `Layout &l2`), l1 for lm | 28 | 16 | 1896 |
| `Layout &l1` + `Layout *l2` (l2 for top) | 308 | 73 | 1892 |
| two `Layout *` pointers | 308 | 73 | 1892 |
| `const Layout &` / `Layout *const` / `*(Layout*)&` / ptr-to-member | 28 | 16 | 1896 |
| `volatile Layout *layout` | 35 | 21 | 1896 |
| `Layout *volatile layout` | 1759 | 258 | 1924 (766) |
| `*(Layout**)&Globals::layout` | 362 | 76 | 1892 |
| `reinterpret_cast`/`static_cast`/`(Layout*)` | 362 | 76 | 1892 |
| `Layout *&slot = Globals::layout` (c2 lm) | 362 | 76 | 1892 |
| `Layout *&slot` (c2 top) | 362 | 76 | 1892 |
| `Layout *&slot` (both c2) | 362 | 76 | 1892 |
| lambda `[]()->Layout**{return &Globals::layout;}` | 362 | 76 | 1892 |
| lambda `[]()->Layout*{return Globals::layout;}` | 362 | 76 | 1892 |
| lambda `[](int off){return *(int*)((char*)Globals::layout+off);}` | 362 | 76 | 1892 |
| `Layout &layout2 = layout;` after the calls (dbg.value) | 362 | 76 | 1892 |
| `const Layout *lp2 = &layout;` after the calls | 362 | 76 | 1892 |
| `(void)layout` / dead no-op comparisons | 28 | 16 | 1896 |
| function-top `Layout &layoutRef` / `Layout *lp` used for lm | 1781 | 818 | 1912 |
| function-top `PaintCanvas *canvas` | 1713 | 434 | 1908 |
| `PaintCanvas *canvas` at block start (all uses) | 359 | 75 | 1892 |
| `PaintCanvas **canvasAddr = &Globals::Canvas` at block start | 362 | 76 | 1892 |
| `PaintCanvas *canvas` before the scroll loops | 424 | 102 | 1892 (755) |
| `volatile` read of `Globals::layout` in c2 | 127 | 76 | 1896 (753) |
| `volatile` read of `Globals::layout` in c3 | 364 | 78 | 1892 |
| `volatile` read of `Globals::Canvas` in c1 | 364 | 85 | 1892 |
| `(unsigned)` compare of c2 | 362 | 76 | 1892 |
| `?:` for `scrollbarHit` | 363 | 84 | 1896 (751) |
| nested `if` (c1 then c2) | 363 | 84 | 1896 (751) |
| swap the two `if` statements | 384 | 126 | 1892 |
| comma-expression oracle shape (fresh lm/top) | 362 | 76 | 1892 |
| comma-expression with cached lm (HEAD-like) | 28 | 16 | 1896 |
| comma-expression with cached top | 29 | 15 | 1896 |
| comma-expression with outer `layout` pointer for lm | 1781 | 818 | 1912 |
| outer `layout` pointer for c1 / top / lm | 1773-1781 | 817-818 | 1908-1912 |
| `(unsigned) field_0x1e0` index | 362 | 76 | 1892 |
| `unsigned idx` / `uint32_t *hb` locals | 378 / 379 | 78 / 82 | 1888 |
| `tex` locals | 351-374 | 76-86 | 1888-1892 |

### 3.3 case 15 — c2 association / operand order

| c2 expression (fresh unless noted) | raw | aligned | size |
|---|---:|---:|---:|
| `top + (lm + height)` (D) | 362 | 76 | 1892 |
| `top + lm + height` | 362 | 76 | 1892 |
| `lm + top + height` | 362 | 76 | 1892 |
| `lm + (top + height)` | 362 | 76 | 1892 |
| `height + top + lm` | 369 | 111 | 1892 |
| `height + lm + top` | 370 | 111 | 1892 |
| `lm + height + top` | 380 | 87 | 1892 |
| `top + height + lm` | 380 | 87 | 1892 |
| `top + (lm + height)` with **lm cached** (HEAD) | **28** | **16** | 1896 |
| `lm + (top + height)` with **lm cached** | 28 | 16 | 1896 |
| `top + (lm + height)` with **top cached** | 29 | 15 | 1896 |
| `lm + (top + height)` with **top cached** | 29 | 15 | 1896 |
| `(top + lm) + height` with top cached | 45 | 23 | 1896 |
| `height + ...` variants with lm/top cached | 113-296 | 63-108 | 1892-1896 |
| `y - top - lm < height` / `y - (top+lm) < height` | 358 | 98 | 1892 (751) |

### 3.4 case 8 — region A (`listEntryWidth` r4 vs ip)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`lm + top + listEntryWidth`) | **28** | **16** | 1896 |
| `top + lm + listEntryWidth` | 28 | 16 | 1896 |
| `top + (lw + lm)` | 28 | 16 | 1896 |
| `top + lw + lm > y` (v4) | 28 | 16 | 1896 |
| `lm + (top + lw)` / `(lm + top) + lw` | 28 | 16 | 1896 |
| `lm + top + (int)(unsigned)lw` / `(lw+0)` / `(lw*1)` / `(-(-lw))` / `(lw^0)` / `(lw|0)` | 28 | 16 | 1896 |
| `(unsigned) lw` | 29 | 17 | 1896 |
| `(unsigned short) lw` | 29 | 16 | 1896 |
| `lw + lm + top` (lw first) | 33 | 17 | 1896 |
| `y < lw + top + lm` | 33 | 17 | 1896 |
| `int lw = this->listEntryWidth;` local | 62 | 26 | 1896 |
| swap `listTopY`/`buttonInsetX` declarations | 30 | 18 | 1896 |
| shadow `Layout *layout` inside case 8 | 28 | 16 | 1896 |
| reorder `layout = Globals::layout` before `buttonInsetX = ...` | 28 | 16 | 1896 |
| `y - top - lm < lw` / `y - (top+lm) < lw` | 832 | 46-47 | 1896 (753) |
| `lw` as `long long` | 873 | 111 | 1912 (758) |

The case-8 block does not use `r4` in the original and does not use it before
`0x3fe` in ours either; `ip` and `r4` are both free at that point. The choice
is a pure allocator tie-break, independent of the case-15 `%29` liveness.

## 4. RA-knob sweep (single-TU object, `-mllvm`)

On D (the exact-IR shape) the object-level aligned diff stays 229 and the
`[sp,#16]` load count stays 6 for:

`-spp-rematerialization-threshold=0/1/2/5/10/50/1000`,
`-regalloc-csr-first-time-cost=0/1/2/5/10/100`,
`-remat-pic-stub-load=true/false`, `-split-spill-mode=size/speed`,
`-enable-deferred-spilling`, `-sink-insts-to-avoid-spills`,
`-condsider-local-interval-cost`, `-callsite-splitting-duplication-threshold=0`,
`-misched-postra=false`, `-enable-misched=false`, `-pre-RA-sched=source/list-*`.

Only `-regalloc=basic/pbqp` change the shape (sp16=3) and are not the original
allocator. `-optimize-regalloc=false` explodes the function (aligned 1291).
So the tie-break is not reachable by any of the exposed Greedy-RA cost knobs;
it is a source-shape/IR-liveness property.

## 5. Conclusion

Every shape lands in one of two buckets:

* `%29` live across the first call -> 1896 B, `fp = &Globals::Canvas`, `%98`
  reloaded at all 8 sites; raw 28 (HEAD) or 29 (top cached). The c2
  `leftMargin` is cached, so region C remains.
* `%29` dead -> 1892 B (or smaller), `%98` kept in `fp`, `&Globals::Canvas` in
  `r5`; raw 291-362.

The original's combination (`%29` dead, `%98` reloaded, `fp =
&Globals::Canvas`) was not reachable by the ~90 new shapes tested this wave.
The residual remains the Greedy-RA remat-vs-keep tie-break on `%98`/`%508`
coupled to `%29` liveness, plus the independent case-8 `ip`/`r4` tie-break.
HEAD is unchanged.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13612
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN15MenuTouchWindow12OnTouchBeginEiiPv
# raw=28 aligned=16 size=1896/1896 insn=754/754
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13612` (`try.sh`, `try8.sh`,
`try15top.sh`, `try15wrap.sh`, `tryfntop.sh`, `mk15*.py`, `mk8*.py`,
`countsp.sh`, `sweepflags.sh`, MIR dumps `D_pre.mir`, `H_pre.mir`,
`D_g.mir`, `H_g.mir`).
