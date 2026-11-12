# `Gun::calcCharacterCollision` + `SpaceLounge::drawLounge` wave 189 (w13306):
# both residuals remain RA/hoist plateaus; no size-exact raw win

Worker: w13306. Branch `wave189-w13306`, base master `247466fde`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w13306`.

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

This note is self-contained and confirms the wave-185..188 conclusions with
two new pieces of evidence: a machine-block-placement (MBP) analysis of the
Gun `k` loop, and a live-range analysis of the SpaceLounge agent spill.

## 1. `Gun::calcCharacterCollision` — 458/63/1984

### New evidence: the `k`-loop layout is an MBP probability decision

The shipped `k` loop is laid out body-first:

```
15343e  movs r5,#0
153440  b.n  1534fc          ; jump to the header placed after the latch
153442  <body>
1534fa  adds r5,#1           ; latch falls through into the header
1534fc  ldr.w r0,[r9,#180]
153500  ldr  r1,[r0]
153502  cmp  r5,r1
153504  bcc.n 153442         ; back to body
153506  b.n  15325a
```

Ours is header-first (`153440` = header, latch `153486` = `adds; b.n 153440`).
This is **not** an IR loop-rotation difference: `-mllvm -print-after-all`
shows the `k` loop is never rotated by `Rotate Loops` in either form (the
header keeps the conditional branch and the latch stays unconditional), and
the shipped layout is the same top-test CFG with the header block placed
*after* the latch.  The block order is chosen by
`Branch Probability Basic Block Placement`.

The MBP dump for the committed body shows why the header is placed first:

```
bb.73 (%ir-block.577):                 ; k-loop header (k < n)
  successors: %bb.74(0x7c000000), %bb.41(0x04000000)
              %bb.74(96.88%), %bb.41(3.12%)   ; body 3.12%, exit 96.88%
bb.74 (%ir-block.587):                 ; body start
  ...
bb.76 (%ir-block.603):                 ; if (diff > 0)
  successors: %bb.91(0x00106035), %bb.95(0x7fef9fcb)
              %bb.91(0.05%), %bb.95(99.95%)   ; __builtin_expect(diff>0,0)
```

The `__builtin_expect(diff > 0, 0)` anchor (1:2000) makes the whole loop
body cold, so MBP keeps the header adjacent to the preheader.  The shipped
body-first order requires the body to be the *likely* successor.  But the
cold `diff>0` hint is exactly what keeps the function at 1984 B: removing it
(or flipping it to likely) drops the size to 1908 B.  The ratio can be
swept with `-mllvm -unlikely-branch-weight=N` (per-TU, object-level test):

| `-unlikely-branch-weight` | raw | size |
|---|---:|---:|
| 1 (default) .. 512 | 458 | 1984 |
| 1000 | 1775 | 1972 |
| 2000 | 1889 | 1908 |
| 4000 | 1892 | 1924 |
| 10000 | 1892 | 1924 |

So no probability in the size-1984 window flips the layout: the shipped
layout and the shipped size are mutually exclusive under this source shape.

### New measurements this wave (full link + `tools/fn_diff.py`, `-j2`)

All variants keep the function at 1984 B unless stated.

| variant | raw | aligned |
|---|---:|---:|
| committed (`stmt` shape) | **458** | 63 |
| `expr` form (`VectorLength(enemyPosition - getPlayer()->getPosition())`) | 462 | 73 |
| `if (__builtin_expect(distance < magnitude, 0)) { int diff = ...; }` | 463 | 57 |
| `if (!__builtin_expect(diff <= 0, 1))` | 458 | 63 |
| `if (__builtin_expect(diff >= 1, 0))` | 458 | 63 |
| `playerPosition` in an explicit `{ }` scope | 458 | 63 |
| `this->positions[i]` / `this->velocities[i]` (`operator[]`) | 458 | 63 |
| local `Vector *posData/velData` before the loads | 480 | 67 |
| `lifetimes[i] = -1000000` before `setHitVector` | 487 | 69 |
| `playerPosition` decl+assign split | 1822 | 2000 |
| `k` condition `__builtin_expect(k < size(), 1)` | 1799 | 1948 |

The `expr` form is the interesting one: it reproduces the **shipped slot
layout** (`playerPosition` at `sp+44`, `delta`/`view` at `sp+104`) and its
only residual in `0x130`-`0x1a0` is the `r4`/`r5` swap

```
shipped: mov r6,r4 ; add r4,sp,#44  ; add r5,sp,#104 ; mov r0,r5
expr   : mov r6,r5 ; add r5,sp,#44  ; add r4,sp,#104 ; mov r0,r4
```

i.e. the hoisted `&stack.10` address lands in `r4` where the shipped has it
in `r5`.  Same Early-Machine-LICM root cause as the committed form; the
`expr` form just moves which slot is hoisted.  Raw is worse (462) because the
`expr` form reorders the `getPosition` sret call into the block.

## 2. `SpaceLounge::drawLounge` — 175/41/1948

### New evidence: the shipped splits `x` into `r5` (pre-clamp) and `r9`
### (post-clamp); ours keeps one `r9` interval, so `r5` is taken by the
### spilled-agent reload

Shipped tail after the clamp:

```
1712f8  mov  r3,r5          ; r3 = x (x is r5 through the clamp)
1712fc  mov  r1,r9          ; r1 = pad (pad is r9)
1712fe  subs r6,r3,r1       ; x - pad
171300  str.w r8,[sp,#12]   ; spill agent
171304  sub.w r5,r2,r9      ; r5 = top - pad
171308  mov  r9,r3          ; x moves to r9 (r5 now free)
...
171330  ldr.w r8,[sp,#12]   ; agent restored into r8
```

Ours:

```
1712ee  mov  r5,fp          ; r5 = pad (pad is fp)
171308  sub.w r6,r9,r1      ; x - pad (x is r9)
17130c  subs r5,r2,r5       ; r5 = top - pad
17130e  mov  r8,r2          ; r8 = top
...
17135c  ldr  r5,[sp,#12]    ; agent reloaded into r5 (r8 still top)
```

So in ours the pre-clamp `x` and the post-clamp phi are coalesced into one
`r9` interval that spans the whole tail; the spilled-agent reload at
`17135c` then takes `r5` (first free in the allocation order).  In the
shipped the pre-clamp `x` is a separate short interval in `r5` that dies at
`171308`, so the reload takes `r8` and `r5` never blocks `x`.  This is the
concrete mechanism behind "`x` is selected first but `r5` is not free".

### New measurements this wave (full link + `tools/fn_diff.py`, `-j2`)

`pad`/`x` are read from `ldr.w ...,[r6,#148]` and `vmov ...,s18`.

| variant | raw | aligned | size | pad | x |
|---|---:|---:|---:|---|---|
| committed | **175** | 41 | 1948 | fp | r9 |
| `int pad2 = pad + pad;` used by `top` only | 175 | 41 | 1948 | fp | r9 |
| `int pad2 = pad + pad;` used by `top` and clamp | 445 | 54 | 1948 | fp | r9 |
| 2-insn clamp `+ (-width - (pad + pad))` | 445 | 54 | 1948 | fp | r9 |
| `int right = w - inset;` used by cond and clamp | 175 | 41 | 1948 | fp | r9 |
| `int left = layout->buttonInsetX;` used by both arms | 175 | 41 | 1948 | fp | r9 |
| reverse clamp order (right edge first) | 493 | 94 | 1952 | fp | r9 |
| `int clampedX = x;` clamp into `clampedX`, then `x = clampedX` | 175 | 41 | 1948 | fp | r9 |
| 3-way `clampedX` phi with `else clampedX = x;` | 175 | 41 | 1948 | fp | r9 |
| `lineHeight` hoisted before the clamp | 588 | 79 | 1952 | r5 | r9 |
| `lineHeight` hoisted + 2-insn clamp | 265 | 47 | 1948 | r5 | r9 |
| `int top = (int)(bottomRight.y - (float)(pad * 2));` | 175 | 41 | 1948 | fp | r9 |
| `int top = (int)bottomRight.y - (pad + pad);` | 1860 | 164 | 1960 | fp | r9 |
| `PaintCanvas *canvas = Globals::Canvas;` | 175 | 41 | 1948 | fp | r9 |
| cached `unsigned font` local | 182 | 45 | 1948 | fp | r9 |

The `lineHeight`-hoist family is the only one that moves `pad` off `fp`, and
it lands in `r5` (never `r9`).  Every size-1948 shape keeps `x = r9`.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13306
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
# Gun: MBP dump after "Branch Probability Basic Block Placement" shows the
# k-loop header's body edge at 3.12% (cold because of expect(diff>0,0)).
clang++ <Gun.cpp flags> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv > gun_mir.txt
# SpaceLounge: VRR dump shows $r9 = COPY $s18 (x), $r11 = pad, and
# $r5 = COPY $r11 (pad) plus the agent reload $r5 = ldr [sp,#12].
clang++ <SpaceLounge.cpp flags> -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN11SpaceLounge10drawLoungeEv > sl_mir.txt
```

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13306/`
(`measure.py`, `objdiff.py`, `adiff.py`, `sltest.py`, `slsweep.py`,
`gunsweep.py`, `gun_mir.txt`, `sl_mir.txt`, `gun.ll`, `gun_fn.ll`,
`Gun.cpp.base`, `SL.cpp.base`, `build.ninja.bak`).

## 4. Files changed

`_work/reconstructions/gun-spacelounge-wave189-w13306.md` only.
