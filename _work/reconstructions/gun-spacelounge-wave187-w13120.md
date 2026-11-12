# `Gun::calcCharacterCollision` + `SpaceLounge::drawLounge` wave 187 (w13120):
# both residuals are RA/hoist plateaus; no size-exact raw win

Worker: w13120. Branch `wave187-w13120`, base master `8b5d4281c`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w13120`.

## 0. Result

**No `src/` change landed.**  Every size-exact shape measured this wave is
equal to or worse than the committed body.  The tree is clean apart from
this note.

| symbol | raw | aligned | size | pct |
|---|---:|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | **458** | 63 | 1984/1984 | 93.4 |
| `_ZN11SpaceLounge10drawLoungeEv` | **175** | 41 | 1948/1948 | 95.7 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
**RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, `.rel.plt` byte-identical, 27/27 headers, file size 2,187,328
exact.

## 1. `Gun::calcCharacterCollision` — 458/63/1984

### Root cause confirmed in machine IR: the hoisted slot is the merged
### `playerPosition`/`view` slot

`-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv` on the committed
body shows the frame objects are two 60-byte slots (`fi#10` align 8,
`fi#11` align 4) plus the three 12-byte vectors.  `%stack.10` (the
`fi#10` slot at `sp+104`) is shared by **three** variables:

* `playerPosition` (DBG_VALUE line 606),
* the `mine`-branch `VectorNormalize`/`VectorpL` temporary (bb.52),
* `Matrix view` (DBG_VALUE line 710).

Before Early Machine LICM the scatter block has its own
`%114 = t2ADDri %stack.10, 0` and the view block its own
`%275 = t2ADDri %stack.10, 0`.  Early Machine LICM hoists a new
`%308 = t2ADDri %stack.10, 0` into the i-loop preheader and rewrites
**both** `%114` and `%275` (and the later `%308` uses) to it.  The
hoisted vreg then lives from the loop header to the view block, which is
why `i*3` (`add.w r4, fp, fp, lsl #1`) lands in `r5` instead of the
shipped `r4`, why the scatter block uses `add r4,sp,#104` instead of
`add r4,sp,#44`, and why the view block rematerialises
`add r0/r1/r4,sp,#104` instead of the shipped single
`add r5,sp,#104` + three `mov`s.

The shipped slot layout is the opposite merge: `playerPosition` is at
`sp+44` and the 60-byte slot at `sp+104` is shared by the scatter
`delta`, the `mine` temporary and `view`.  The shipped therefore has
`i*3 = r4`, `pp = r4/sp+44`, `delta = r5/sp+104`, and rematerialises
`&view` at the view block (`add r5,sp,#104`).

The `expr` form (no named `playerPosition`; the `getPosition` sret temp
is not lifetime-marked and so is not merged with `view`) reproduces the
shipped slot layout (`pp = sp+44`, `delta+view = sp+104`), but its
`%308` hoist still steals `r4`: the scatter block becomes
`pp = r5/sp+44`, `delta = r4/sp+104`, `i*3 = r5` (raw 462, aligned 73).

### New sweeps this wave (full link + `tools/fn_diff.py`, `-j2`)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed (`stmt` shape) | **458** | 63 | 1984 |
| `expr` inline (no named `playerPosition`) | 462 | 73 | 1984 |
| `int distance = VectorLength(distance < magnitude)` form, `diff` inside | 463 | **57** | 1984 |
| `if (__builtin_expect(distance < magnitude, 0))` + `diff` inside | 463 | 57 | 1984 |
| `if (distance < magnitude)` / `expect(...,1)` (no cold expect) | 1886 | 257 | 1908 |
| named `Vector delta;` before `pp` + `delta = enemyPosition - pp` | 1645 | 287 | 1964 |
| `Vector pp; Vector delta; pp = ...; delta = ...` | 1408 | 302 | 1980 |
| `Vector delta;` at i-loop top, pp named | 1709 | 250 | 1964 |
| `Vector playerPosition;` at i-loop top | 1614 | 270 | 1964 |
| `Matrix *vp = &view` / `Matrix &vr = view` / `Matrix view2 = view` | 458/458/459 | 63 | 1984 |
| `__builtin_expect` on the `k` loop condition (0/1), `k != size`, `while` | 1714..1799 | 308..361 | 1904..1984 |
| per-TU `-likely-branch-weight` 1..64 on `Gun.cpp` | 458 | 63 | 1984 |
| `lt_e0` + per-TU branch weights 1..64 | 463 | 57 | 1984 |

The `lt_e0` family (`if (distance < magnitude) { int diff = ...; }`)
improves the *aligned* score 63 -> 57 because it reproduces the shipped
`cmp magnitude,distance` / `ble` comparison, but the block placement is
still header-first: the shipped has the loop test at the bottom
(`b.n` entry guard, body fallthrough, increment out of line), ours has
the test at the top and the increment as fallthrough.  The extra entry
`b.n` (2 B) is not compensated, so raw grows 458 -> 463.  No branch
weight in 1..64 changes that placement.

### Conclusion

The residual is one machine-level decision: Early Machine LICM hoists
`&stack.10` because `playerPosition` and `view` were merged into one
60-byte stack slot.  Every source shape that keeps the shipped `ABCDE`
tail keeps `i*3 = r5`; every shape that moves `pp` out of `%stack.10`
(the `expr` form) still hoists the `delta`/`view` slot.  The committed
`stmt` body remains the best measured shape at 458/63/1984.

## 2. `SpaceLounge::drawLounge` — 175/41/1948

### Root cause re-confirmed: `pad` is `fp`, `x` is `r9`

Shipped right-edge clamp:

```
ldr.w  r9, [r6, #148]        ; pad
vmov   r5, s18               ; x = centerX
sub.w  r1, r5, r9
cmp    r1, r0
bge.n  else
add.w  r5, r0, r9            ; x = inset + pad
b      after
...
add.w  r1, r9, r5            ; pad + x
add    r1, r6                ; + width
cmp    r1, r0
itt    gt
addgt.w r1, r6, sl           ; width + 2*pad
subgt  r5, r0, r1            ; x = (w-inset) - (width+2pad)
```

Ours has `pad = fp`, `x = r9`, the 4-instruction negated clamp
(`movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6; addgt.w r9,r1,r0`)
and a `mov r5, fp` copy in each clamp arm, because `fp` must be reused
for `lineHeight` while `pad` is still live.  The tail then keeps
`agent` in `r5` instead of `r8` and `Globals::Canvas` in `r5` instead of
`r8`.

The 2-instruction clamp itself is reachable independently with
`x = Globals::w - layout->buttonInsetX + (-width - (pad + pad));`
(IR `%174 = add width, pad2; %175 = sub (w-inset), %174`), but `pad`
still lands in `fp`, so the two `mov r5, fp` copies remain and raw
grows to 445.

### New sweeps this wave (full link + `tools/fn_diff.py`, `-j2`)

Every measurement below used the committed body; `pad`/`x` are read from
the emitted `ldr.w ...,[r6,#148]` and `vmov ...,s18`.

| variant | raw | aligned | size | pad | x |
|---|---:|---:|---:|---|---|
| committed | **175** | 41 | 1948 | fp | r9 |
| 2-insn clamp `+ (-width - (pad + pad))` | 445 | 54 | 1948 | fp | r9 |
| `- (width + pad + pad)` | 176 | 42 | 1948 | fp | r9 |
| `- width - pad - pad` (flat minus) | 468 | 64 | 1944 | fp | r9 |
| `int right = w - inset; int over = width + pad + pad; x = right - over` | 176 | 42 | 1948 | fp | r9 |
| `int right; x = right - (width + pad + pad)` | 176 | 42 | 1948 | fp | r9 |
| 6 declaration orders of `{pad, SetColor, centerX, top}` with pad first | 175 | 41 | 1948 | fp | r9 |
| 400 random 1-3-edit combinations (clamp/condition/type/reorder pool) | 175 | 41 | 1948 | fp | r9 |
| branch-weight grid `l 1..2000 x u 1..7` (105 builds) | 175..982 | 41..147 | 1948..1980 | fp/r5 | r9 |
| `int &pad = Globals::layout->field_0x94` | 1853 | 195 | 1972 | sl | **r5** |
| `int &pad` + 2-insn clamp | 1848 | 208 | 1968 | sl | r5 |
| `volatile` pad load | 230 | 66 | 1948 | fp | r9 |
| `centerX` in place (no `x` local) | 175 | 41 | 1948 | fp | r9 |
| `int x(centerX)` / `int &x = centerX` / `register int pad` | 175 | 41 | 1948 | fp | r9 |
| precompute `left/top2/w2` before `lineHeight` | 175 | 41 | 1948 | fp | r9 |

The branch-weight grid shows `pad` can be `fp` or `r5` but **never**
`r9`; `x` is `r9` in every size-1948 variant.  The only shape that
moves `x` to `r5` is `int &pad` (pad becomes a reloaded reference in
`sl`, frame +24 B).  The colour is a global greedy-RA decision driven by
the whole (741-instruction) function, not by the mode-0 declaration
order, the clamp association, or the `x` spelling.

### Conclusion

The shipped needs `pad = r9`, `x = r5` simultaneously.  With the
committed source the allocator always gives `x = r9` and `pad = fp`
(or `r5` under a different branch-weight grid).  No size-1948 source
shape found this wave flips `pad` to `r9`.  The committed body remains
the best measured shape at 175/41/1948.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13120
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv _ZN11SpaceLounge10drawLoungeEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=458/175`, `aligned=63/41`, sizes `1984/1948`,
`RATCHET PASS`, byte 4507 / linked 4507, data 843/843, imports 207,
lint CLEAN, drift 0.

Machine-IR evidence (scratch, not tracked):

```sh
# Gun: %308 = t2ADDri %stack.10, 0 hoisted; %stack.10 is
# playerPosition (line 606) + mine temp + view (line 710)
clang++ <Gun.cpp flags> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv > gun_mir.txt
# SpaceLounge: "After Virtual Register Rewriter" shows $r11 = pad, $r9 = x
clang++ <SpaceLounge.cpp flags> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN11SpaceLounge10drawLoungeEv > sl_mir.txt
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13120/`
(`sweep_sl.py`, `sl2.py`, `sl_rand.py`, `sl_weights2.py`, `sweep_gun.py`,
`gun1.py`..`gun5.py`, `gun_prob.py`, `gun_combo.py`, `gun_view.py`,
`gun_weights*.py`, `gun_mir.txt`, `gun_expr_mir.txt`, `sl_mir.txt`,
`dump.py`, `adiff.py`, `parse_mir.py`).

## 3.5 Second-pass sweeps (same wave, object-level harness)

The full-link measurements above were cross-checked with the single-TU
object harness (`scratch/w13120/measure.py`, `gx.py`, `sx.py`), which
compiles one TU and reports the alignment diff of the object's
`.text.<sym>` section.  Object-level base values are Gun `diff=55`
(full link 458/63) and SpaceLounge `diff=35` (full link 175/41); the
object diff tracks the full-link diff monotonically for these two
functions.  Every shape below kept the function size at 1984 (Gun) /
1948 (SpaceLounge) unless stated.

### Gun

| variant | object diff | size |
|---|---:|---:|
| committed | **55** | 1984 |
| `expr` form (no named `playerPosition`) | 65 | 1984 |
| `Vector position(...)` / `velocity(...)` / `enemyPosition(...)` copy-ctor spelling | 55 | 1984 |
| `swap position`/`velocity` declaration order | 78 | 1984 |
| `radius` declared before `enemyPosition` | 64 | 1984 |
| `const bool bomb` / `const bool mine` / `const bool scatter` | 55 | 1984 |
| `Vector *td` before `lastHitKIPlayer = 0` | 55 | 1984 |
| `int i` / `int e` loop counters (with casts) | 56 | 1984 |
| `int k` / `uint32_t i` | 55 | 1984 |
| `radius / 2` instead of `(int)((float)radius * 0.5f)` | 98 | 1976 |
| `radius * 3` int instead of `(int)(float)(radius * 3)` | 88 | 1976 |
| `(int)((float)radius * 3.0f)` | 58 | 1984 |
| `(int)((float)radius * 2.0f)` in the 10000 branch | 97 | 1980 |
| `Matrix translated = MatrixSetTranslation(...); (void)translated;` | 55 | 1984 |
| `MatrixSetTranslation` result used as the `TransformSetLocal` matrix | 111 | 1988 |
| `MatrixElements view` at loop top / inner (POD 60 B) | 281 | **1948** |
| `Matrix view;` at loop top (default ctor) + assignment | 355 | **2024** |
| global `Vector gtmp` for the k-loop `getPosition()` temp | 270 | **1972** |
| `expr` + `Matrix translated` | 65 | 1984 |
| `expr` + swap position/velocity | 88 | 1984 |
| `expr` + radius before enemyPosition | 74 | 1984 |
| swap `mine`/`scatter` branch order | 269 | **1976** |

The `MatrixElements`/`Matrix view;`/global-`gtmp` shapes were aimed at
forcing `view` or the k-loop temp into a *distinct* stack slot so that
`%stack.10` would not be CSE-hoisted.  They do change the merge (and
sometimes the frame size), but every size-1984 shape still leaves
`i*3` in `r5`; the shapes that do move it are not size-exact.

### SpaceLounge

| variant | object diff | pad | x | clamp |
|---|---:|---|---|---|
| committed | **35** | fp | r9 | 4 |
| `centerX` declared before `pad` | 35 | fp | r9 | 4 |
| `pad`, `centerX`, `SetColor`, `top` order | 35 | fp | r9 | 4 |
| `pad`, `centerX`, `top`, `SetColor` order | 35 | fp | r9 | 4 |
| `SetColor` first | 64 | - | r9 | 4 |
| `centerX`, `pad`, `top` (no `SetColor` between) | 64 | - | r9 | 4 |
| `const int pad` / `const int centerX` / `const int top` | 35 | fp | r9 | 4 |
| `int pad2 = pad + pad;` used by `top` (pad first) | 35 | fp | r9 | 4 |
| `int pad2` with `centerX` first | 64 | - | r9 | 4 |
| `pad << 1`, `pad * 2`, `pad + pad + width`, `(pad << 1) + width` | 35 | fp | r9 | 4 |
| no `lineHeight` local (direct `layout->field_0x30_rowHeight`) | 35 | fp | r9 | 4 |
| `Layout *layout` hoisted before the `pad` load | 232 | - | sl | 4 |
| `top` declared then assigned just before the clamp | 35 | fp | r9 | 4 |
| `top` moved after `width` (before clamp) | 205 | fp | r6 | 4 |
| 2-insn clamp `+ (-width - (pad + pad))` | 45 | fp | r9 | 2 |
| 2-insn clamp + `centerX` first / `const pad` / `pad2` | 45 | fp | r9 | 2 |

Every size-1948 shape still gives `x = r9` and `pad = fp`; the only
variants that move `pad` off `fp` are the `int &pad` reference
(earlier table, size 1972) and the branch-weight grid (`r5`, never
`r9`).  The shipped `pad = r9`, `x = r5` pair remains unreachable from
the measured source shapes.

## 4. Files changed

`_work/reconstructions/gun-spacelounge-wave187-w13120.md` only.
