# SpaceLounge::drawLounge — wave-191b w13785 (source-shape / RA probe)

Status: research note (2026-11-13). Worktree `wt/w13785`, branch
`wave191b-w13785`, base `5968bc1ac`. **No `src/` change landed**: no tested
source shape beat the committed base on raw bytes, `.text` or `.ARM.extab`.
Tree is clean; this note is the only artefact.

Target: `_ZN11SpaceLounge10drawLoungeEv` in `src/game/menu/SpaceLounge.cpp`
(overview block is lines 1184–1244 of `5968bc1ac`).

## Gate context (unchanged by this note)

```text
byte_exact 4508/4524  linked_exact 4508/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0
data_exact 843/843  imports 207  lint CLEAN  drift 0  parity 5370/5370
```

Baseline for the target (re-verified twice this session, `basechk11/12`):

| | raw | aligned | size | insn |
|---|---:|---:|---|---:|
| committed base | **138** | **87** | 1948/1948 | 739/741 |

Section deltas at base: `.text` **74651**, `.ARM.extab` **562**,
`.ARM.exidx` 0, `.rodata` 0, `.data` 0, `.bss` 0.

## 1. New finding: the `x - pad` DrawRectangle form is the 175/41 attractor

The committed source writes the rectangle's left edge as
`x - Globals::layout->field_0x94`. The **original reuses the `x - pad` value
already computed for `drawBox`** (`1712fe subs r6,r3,r1` then `171340 mov
r1,r6`), i.e. it does not reload `field_0x94`. Switching the source to
`x - pad` is a one-line change that reaches the previously known 175/41
attractor *and* fixes the whole-function callee-saved colouring:

```text
O 170d64: ldr.w sl, [r0]      U(base) 170d64: ldr.w fp, [r0]
```

With `x - pad` the first differing instruction moves from `170d64` to
`170ebe`: **the entire `mode != 0` half of the function (0x170d38–0x170e80)
becomes byte-identical**, and the instruction count becomes 741/741 (base is
739/741). Only the overview block's `pad`/`x` colouring still differs.

Measured (`tools/fn_diff.py`, `.text`/`.ARM.extab` from a full link):

| shape | raw | aligned | size | insn | .text | .ARM.extab |
|---|---:|---:|---|---:|---:|---:|
| base | **138** | 87 | 1948 | 739 | **74651** | **562** |
| `x - pad` in DrawRectangle (`b_rect`) | 175 | **41** | 1948 | 741 | 74688 | 565 |

So the 175 attractor is *structurally* the better source (41 aligned lines,
741 insns, whole non-overview half exact) but is **not a raw win**: +37 raw,
+37 `.text`, +3 extab. Not committed; it is the recommended base for any
future pass that solves the `pad`/`x` colouring.

## 2. Why `pad` is `fp` and not `r9` (instrumented clang 7.0.2)

`RA_TRACE=1` on the `b_rect` (175) body, `drawLounge` only:

```text
ENQ %2   pad   size=6400 prio=2684360960 (=0xA0000000+6400, no hint bit)
ENQ %609 x     size=4220 prio=3758100604 (=0xE0000000+4220, hint bit set -> $r3)

select %609 x    size=4220 w=2.946140e-04 hint=$r3 -> $r5
select %47  boxW size=768  w=6.080040e-04 hint=$r3 -> $r6
select %46  top-pad size=736 w=6.251309e-04 hint=$r3 -> $r5   (evicts x)
select %609 x    size=4220 w=2.946140e-04 hint=$r3 -> $r9     (r9 free -> whole)
select %45  x-pad size=704  w=6.432506e-04 hint=$r2 -> $r6
select %47  boxW size=768  w=6.080040e-04 hint=$r3 -> $r10
select %2   pad  size=6400 w=2.154168e-04 no hint -> cannot evict x in r9
     -> split; the load part lands in $r11 (fp)
```

`pad` is enqueued with a *larger* size than `x` (6400 > 4220) but **without
the `1<<30` hint bit**, while `x` carries a known preference for `$r3`. So `x`
is dequeued first and takes `r9` whole. When `pad` is finally dequeued,
`RAGreedy::shouldEvict` (RegAllocGreedy.cpp) requires
`pad.weight > x.weight`; `2.154e-04 < 2.946e-04`, so `pad` cannot evict `x`
and is split into `fp`.

The original instead has `pad = r9`, `pad2 = sl`, `x = r5 -> r3 -> r9` (x is
split at the `drawBox` call: `vmov r5,s18` … `mov r3,r5` … `mov r9,r3`), i.e.
`pad` owned `r9` before `x`'s second selection. That is exactly the ordering
the hint bit prevents.

### RA lever results (instrumented clang, `b_rect` body)

| env | raw | aligned | size | insn | note |
|---|---:|---:|---|---:|---|
| `PRIOVREG=pad(0x80000002)` | 1884 | 225 | 1980 | 749 | pad promoted, size grows |
| `HINTVREG=pad:9` | 1882 | 226 | 1980 | 749 | hint grows body |
| `SIZEVREG=pad:99999999` | 290 | 61 | 1948 | 741 | pad still `fp` |
| `NOHINT=x(0x80000261)` | 175 | 41 | 1948 | 741 | no effect (hint bit unchanged) |

`SIZEVREG` proves re-ordering alone is not enough: even with `pad` first in the
queue it still lands in `fp`, because `x` already owns `r9` from its earlier
whole-range selection. `PRIOVREG`/`HINTVREG` reach `pad=r9` but grow the body
to 1980 B.

### Proof that `pad=r9` is reachable at all

Using `centerX` (not `x`) for both `DrawString` calls — i.e. removing `x` from
the tail and giving the box edges their own value — drops `x`'s weight below
`pad`'s and yields **`pad=r9`**:

| shape | raw | aligned | size | insn | pad |
|---|---:|---:|---|---:|---|
| `r_ds_centerx` (both DrawStrings use `centerX`) | 574 | 160 | **1960** | 746 | **r9** |
| `r_ds_centerx1` (name only) | 528 | 163 | 1952 | 746 | fp |
| `r_ds_centerx2` (role only) | 561 | 167 | 1952 | 746 | fp |

So the original colouring is not unreachable in principle — it needs `x`'s
live-range weight below `pad`'s *without* introducing a second live value
(every attempt that does so costs 4–12 B of body).

## 3. Full measured table (all size 1948/1948 unless stated)

Harness: `bt2.py` splices a body into `pristine.cpp` (HEAD), builds
`link_so -j2` under `ulimit -v 12000000`, and reports `fn_diff.py`.

### 3a. Checkpoint carried over from w13753 (base / prior sessions)

| candidate | raw | aligned | insn |
|---|---:|---:|---:|
| base | **138** | **87** | 739 |
| `b_175` (175 attractor) | 175 | 41 | 741 |
| `B_pad2all` | 445 | 54 | 741 |
| `base_arm_sub_pp` (`-(width+pad+pad)`) | 139 | 88 | 739 |
| `above_first` | 265 | 50 | 741 |
| `above_reload` | 537 | 99 | 1956 |
| `la_scope` (nested `{}`) | 1688 | 243 | 1988 |
| `left_before_clamp` | 1870 | 176 | 1964 |
| `lh_before_clamp` | 606 | 88 | 1952 |
| `known_local` | 1538 | 183 | 1960 |
| `y_local` | 1862 | 226 | 1976 |
| `inline_x2` | 1087 | 158 | 1948 |
| `clamp_lt` (`x < inset+pad`) | 542 | 102 | 1944 |
| `clamp_gt_rhs` (`w-inset < pad+x+width`) | 139 | 88 | 739 |
| `arm_w_inset` / `arm_pp_mul` | 1553 | 167 | 1960 |
| `q3_drawxearly`, `r2_cx2draw` | 566 | 169 | 1952 (pad=r9) |
| `n5_abovefirst` | 554 | 72 | 1944 |
| `v175_arm_pp2` | 468 | 64 | 1944 |
| `v175_lh_before_clamp` | 588 | 79 | 1952 |
| `ora_gt` (2-insn `itt gt` arm) | 445 | 54 | 741 |
| `ora_full` / `ora_lt_arm_pp` | 445 | 57 | 741 |

### 3b. This session — n6 (free / local / scope shapes)

| candidate | raw | aligned | size | insn |
|---|---:|---:|---|---:|
| `text_local`, `text_local2` | 1877 | 320 | 1988 | 751 |
| `y_after_top` | 1544 | 245 | 1968 | 737 |
| `clamp_xpadw` | 138 | 87 | 1948 | 739 |
| `pad2_shift` | 138 | 87 | 1948 | 739 |
| `top_int_sub` | 1860 | 216 | 1960 | 737 |
| `half_local` | 138 | 87 | 1948 | 739 |
| `canvas_block` | 138 | 87 | 1948 | 739 |
| `ds_y_before_x` | 138 | 87 | 1948 | 739 |

### 3c. This session — n7 (DrawRectangle / lineHeight spellings, on base)

| candidate | raw | aligned | size | insn | pad |
|---|---:|---:|---|---:|---|
| `rect_xpad` (= 175 attractor) | 175 | 41 | 1948 | 741 | fp |
| `lh_inline` (drop `lineHeight` local) | 138 | 87 | 1948 | 739 | sl |
| `rect_xpad_lh_inline` | 175 | 41 | 1948 | 741 | fp |
| `rect_lh_local` | 141 | 87 | 1948 | 738 | sl |
| `rect_both_local` | 990 | 162 | 1948 | 737 | r5 |

### 3d. This session — n8..n14 (all on the `b_rect` = 175 body)

Every one of these is a fixed point at **175/41/1948/741 pad=fp** unless noted:

- `pad + pad + width` in drawBox / DrawRectangle (both, or either)
- `int pad2 = pad + pad;` used only in `top`
- `top` declared before `centerX`
- clamp condition `pad + (x + width)`, `x + pad + width`
- `x = w - inset - (width + pad + pad)` (2-insn spelling)
- `pad` declared before the Vectors / after `centerX`
- `x` declared after `centerX` / after `top`
- `int pad` cast / `+ 0` / split declaration / `const` / `long`
- `inset` local, `Globals::w` local, nested `{}` around the clamp
- in-place `centerX` (oracle form) with `x - pad` reuse
- `int x = centerX;` after the clamp, box on `centerX`

Deviations:

| candidate | raw | aligned | size | insn | pad |
|---|---:|---:|---|---:|---|
| `r_pad_after_color` (pad read after SetColor) | 254 | 72 | 1948 | 741 | fp |
| `r_pad_before_setcolor` | 254 | 72 | 1948 | 741 | fp |
| `r_pad_after_cx` | 254 | 72 | 1948 | 741 | fp |
| `r_pad_uint` | 177 | 43 | 1948 | 741 | fp |
| `r_pp_drawbox` / `r_pp_rect` (one `pad+pad`) | 1851 | 250 | 1964 | 743 | fp |
| `r_pad2_all` (pad2 in arm + box) | 1824 | 237 | 1960 | 740 | fp |
| `r_pad2_box` | 1851 | 250 | 1964 | 743 | fp |
| `r_setcolor_before_vectors` | 1535 | 184 | 1952 | 742 | fp |
| `r_texty_before` | 1678 | 246 | 1964 | 735 | r5 |
| `r_ds_centerx` (both DrawStrings on `centerX`) | 574 | 160 | **1960** | 746 | **r9** |

## 4. Byte-run diagnostics (base, 138 differing bytes / 36 runs)

Largest runs vs the original: overview `0x5a6–0x5c6` (33 B), `0x5c8–0x5d9`
(18 B), `0x5f6–0x5fe` (9 B), `0x600–0x609` (10 B), LSDA tail `0x74d–0x76b`
(31 B), literal `0x78c` (O `0x84` vs U `0x7c`). `.ARM.extab` tail word:
original `0x1714c4` = `9184 0005` (0x210); base ours `0x1f0`; 175 shape
`0x1e0`; ora `0xa4`. Matching the extab requires matching the frame's
spill-slot layout, i.e. the same RA outcome.

## 5. Conclusion

- The committed base (138/87) is still the best **raw** shape and the best
  `.text`/`.ARM.extab` shape.
- The `x - pad` DrawRectangle spelling is the correct source (matches the
  original's `mov r1,r6` reuse) and reaches the 175/41 attractor with the whole
  non-overview half of the function byte-identical and 741/741 insns, but it
  costs +37 raw / +37 `.text` / +3 extab, so it was **not** landed.
- The remaining residual is one greedy-RA decision: `x` carries a `$r3`
  register hint (`prio` bit 30) so it is dequeued before the larger `pad`
  range and takes `r9` whole; `pad` then cannot evict it
  (`pad.weight 2.154e-04 < x.weight 2.946e-04`) and is split to `fp`.
  `r_ds_centerx` proves `pad=r9` is reachable, but only by adding a second
  live value, which costs 4–12 B of body (size no longer 1948).
- No source shape tried this session produced a raw win, so `src/` is clean.
  The 175 body (`b_rect`) is the recommended starting point for a future pass
  that can lower `x`'s weight without a second live value.
