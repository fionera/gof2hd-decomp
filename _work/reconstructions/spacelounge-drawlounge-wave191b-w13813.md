# SpaceLounge::drawLounge — wave-191b w13813: Ghidra confirmation + clamp/declaration sweep (no win)

Status: research note (2026-11-13). Worktree `wt/w13813`, branch
`wave191b-w13813`, base `dcb04a00d`. **No `src/` change landed** — every
size-exact source shape tested this session is either the committed base
score or the already-known 175/41 attractor; none is a raw win. Tree is clean
apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` in `src/game/menu/SpaceLounge.cpp`
(overview block, function starts at line 1184).

Prior work read first: `...-w13785.md` (RA trace, 175/41 attractor),
`...-w13800.md` (bimodality, ~40-cell clamp/declaration table),
`...-w13753.md` (register-colouring map), `...-w13737.md`
(pad-load/reload sweep), `...-w13709.md` (per-region byte split),
`...-w13675.md` (SIZEVREG prize).

## 1. Baseline re-verified on this base

`bash scratch/w13813/score.sh` (incremental `link_so -j2` + `tools/fn_diff.py`
+ section diff) reproduces the committed gate:

```text
_ZN11SpaceLounge10drawLoungeEv  raw=138 aligned=87 size=1948/1948 insn=739/741
.text 1259464/1259464 diff 75904
.ARM.extab 156312/156312 diff 594
.ARM.exidx diff 0   .rodata diff 0   .data diff 0   .bss diff 0
```

(The task brief's `.text` 75,146 and w13785's 74,651 are stale; the wave-191b
base is **75,904**.)

## 2. Ghidra confirmation of the original source shape

`SpaceLounge::drawLounge` is at Ghidra address **0x180d38** (real
`0x170d38`; `0x170d38` is not a function entry in the project). Project:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so_ghidra`,
program `/android_2.0.16_libgof2hdaa.so`. Use the pyghidra venv
(`/tmp/pyghidra-venv/bin/python`) with a **30 GB** `ulimit -v` (12 GB OOMs the
JVM at startup).

The decompile of the overview block is exactly the **175 shape**:

```c
iVar9  = *(int *)(*piVar1 + 0x94);          /* pad */
iVar11 = iVar9 * 2;                          /* pad2, materialised early  */
iVar4  = centerX;  iVar26 = top;  iVar23 = width;
...
pLVar14 = (Layout *)*piVar1;
iVar8 = *(int *)(pLVar14 + 0x28);            /* buttonInsetX */
if (iVar4 - iVar9 < iVar8) {
  iVar4 = iVar8 + iVar9;
}
else {
  iVar8 = **(int **)(DAT_001814c0 + 0x1812d0) - iVar8;   /* w - inset */
  if (iVar8 < iVar9 + iVar4 + iVar23) {                   /* right < pad + x + width */
    iVar4 = iVar8 - (iVar23 + iVar11);                    /* right - (width + pad2)  */
  }
}
uVar17 = *(undefined4 *)(pLVar14 + 0x30);    /* lineHeight */
Layout::drawBox(pLVar14, 2, iVar4 - iVar9, iVar26 - iVar9, iVar11 + iVar23, uVar17, String(""));
SetColor(...);
DrawRectangle(*puVar21, iVar4 - iVar9, iVar26 - iVar9, iVar11 + iVar23,
              *(int *)(*piVar1 + 0x30));     /* height is a reload of field_0x30 */
```

Confirmed points:

* `pad2` is an **integer** `pad * 2` materialised once (asm `170ed2
  mov.w sl, r9, lsl #1`) and reused in `top`, the clamp arm and `boxWidth`
  (asm `1712de addgt.w r1, r6, sl` and `1712fa add sl, r6`).
* The clamp arm is `right - (width + pad2)` (2 instructions:
  `itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`), not the `right + (-width - pad
  - pad)` form the committed source uses.
* The condition sums as `(pad + x) + width` (`1712d4 add.w r1,r9,r5;
  1712d8 add r1,r6`).
* `DrawRectangle` reuses the same `x - pad` / `top - pad` values as
  `drawBox` (`1712fe subs r6,r3,r1` then `171340 mov r1,r6`), and reloads
  `field_0x30` for its height.
* The overview block's name/role/width construction is **byte-identical** in
  our build already: the first differing instruction is `170ebe` (the `pad`
  load), not anything earlier.

## 3. New measurements this session (real gold link, `tools/fn_diff.py`)

Harness: `scratch/w13813/mybatch.py` (base body) and `B.py` (175 body)
splice a body into `src/game/menu/SpaceLounge.cpp`, build `link_so -j2` under
`ulimit -v 12000000`, and report `fn_diff.py` + section diffs. All rows are
size 1948/1948 unless the size column says otherwise.

Base = **138 / 87 / 1948 / 739**, `.text` **75,904**, extab **594**.
175 shape = **175 / 41 / 1948 / 741**, `.text` **75,941**, extab **597**.

| variant (on the 175 shape unless noted) | raw | aligned | size | `.text` | extab |
|---|---:|---:|---:|---:|---:|
| 175 shape (plain) | 175 | **41** | 1948 | 75,941 | 597 |
| + Ghidra clamp as `else { int right = w - inset; if (right < pad + x + width) x = right + (-width - pad - pad); }` | 176 | 42 | 1948 | 75,942 | 597 |
| + arm `- (width + pad + pad)` | 176 | 42 | 1948 | 75,942 | 597 |
| + arm `+ (-(width + pad + pad))` | 176 | 42 | 1948 | 75,942 | 597 |
| + arm `- (width + (pad + pad))` (CSE with `top`) | 445 | 54 | 1948 | 76,211 | 609 |
| + arm `- (pad + pad) - width` | 445 | 54 | 1948 | 76,211 | 609 |
| + arm `+ (-width - (pad + pad))` | 445 | 54 | 1948 | 76,211 | 609 |
| + arm `- width - (pad + pad)` | 445 | 54 | 1948 | 76,211 | 609 |
| + named `int pad2 = pad + pad;` in top/arm/width | 445 | 54 | 1948 | 76,211 | 609 |
| + named `int pad2 = pad * 2;` in top/arm/width | 445 | 54 | 1948 | 76,211 | 609 |
| + named `pad2` but arm keeps `pad + pad` | 175 | 41 | 1948 | 75,941 | 597 |
| + arm `- width - pad - pad` (flat) | 468 | 64 | **1944** | 77,552 | 616 |
| + `int right = ...` else-block **and** rect uses `left` | 491 | 90 | **1944** | 77,575 | 616 |
| + `above` declared before `left` | 265 | 50 | 1948 | 76,031 | 600 |
| + `int left = x - pad, above = top - pad;` (one decl) | 175 | 41 | 1948 | 75,941 | 597 |
| + `above = top - Globals::layout->field_0x94` (reload) | 565 | 83 | **1956** | 237,791 | 1305 |
| + `left = x - Globals::layout->field_0x94` (reload) | 573 | 163 | **1956** | 237,799 | 1301 |
| + `x` declared right after `top` | 175 | 41 | 1948 | 75,941 | 597 |
| + `int x;` then `x = centerX;` before the clamp | 175 | 41 | 1948 | 75,941 | 597 |
| + `top` declared before `centerX` | 175 | 41 | 1948 | 75,941 | 597 |
| + `const int pad` | 175 | 41 | 1948 | 75,941 | 597 |
| + `const int left/above` | 175 | 41 | 1948 | 75,941 | 597 |
| + `int x = (int) centerX;` | 175 | 41 | 1948 | 75,941 | 597 |
| + `int width = (int) GetTextWidth(...)` | 175 | 41 | 1948 | 75,941 | 597 |
| + `above = top - pad` hoisted before the clamp | 175 | 41 | 1948 | 75,941 | 597 |
| + `int left2 = left;` before `drawBox` | 175 | 41 | 1948 | 75,941 | 597 |
| + `Layout *layout` hoisted next to `pad` | 1860 | 276 | **1972** | 240,787 | 1318 |
| + `pad` declared after `SetColor` | 254 | 72 | 1948 | 76,020 | 597 |
| + arm1 `x = pad + layout->buttonInsetX` | 445 | 54 | 1948 | 76,211 | 609 |
| + `x = w - (inset + (width + pad + pad))` | 477 | 77 | **1952** | 237,722 | 1300 |
| + `above` before `left` on the 445 shape | 554 | 72 | **1944** | 77,638 | 616 |

Key observations:

* The **445/54 shape** (`pad2` CSE'd into the arm) is the one where the clamp
  arm becomes structurally exact (`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`)
  and the prologue/`fp`/`sl` colouring matches the original
  (`170d64 ldr.w sl,[r0]`, `170d7e ldrd fp,r8,[r4,#112]`). But it pays
  **+13 aligned** (54 vs 41) because `pad` then lands in `fp` and the merge
  block inserts an extra `mov r5,fp` in each clamp arm, while `x` stays in
  `r9`. Not a win.
* `left = x - Globals::layout->field_0x94` (extra load, 1956 B) is the only
  size-exact-adjacent spelling that reaches **`pad = r9`**; it is size-fatal
  (stack frame grows 120 → 128), confirming w13737's row.
* All declaration-order/`const`/cast spellings are byte-identical no-ops: the
  scheduler and RA are insensitive to them.

## 4. Residual delta (unchanged, three coupled RA tie-breaks)

The 175 shape's 41 aligned diffs are exactly the three items in w13785 §5:

1. `pad = fp` instead of `r9` (original `170ebe ldr.w r9,[r6,#148]`).
2. `x = r9` whole instead of `r5 → r3 → r9` (original `1712b2 vmov r5,s18`
   … `171308 mov r9,r3`).
3. Clamp arm is the 4-instruction `itttt gt; movgt r1,#0; subgt.w r1,r1,pad<<1;
   subgt r1,r1,width; addgt.w x,r1,right` form because `pad*2` is
   rematerialised from `fp` instead of reusing `sl`.

A fourth, independent tie-break is visible in the tail: after the `drawBox`
call the original moves `top` from `r8` to `fp` and restores `agent` into `r8`
(`17132e mov fp,r8; 171330 ldr.w r8,[sp,#12]`), whereas ours keeps `top` in
`r8` and loads `agent` into `r5` (`17135a ldr r5,[sp,#12]`). Both are one
instruction; the allocator breaks the tie differently.

These four are all "which callee-saved register" choices; no source spelling
tested (this session or the six prior wave-191b notes) changes them without
growing the body past 1948 B.

## 5. Conclusion

The committed base remains the best size-exact shape:
**raw 138 / aligned 87 / 1948 B / 739 insns / `.text` 75,904 / extab 594**.
The 175/41 shape is the structurally-correct original but scores 175 raw and
costs +37 `.text` / +3 extab. No `src/` change is landed; the gate is
unchanged (byte_exact 4508/4524, linked_exact 4508/4524, size-exact
4524/4524, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## 6. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/wt/w13813
git checkout -- src/game/menu/SpaceLounge.cpp
rm cmake-build-match/libgof2hdaa.so && bash /home/debian/GalaxyOnFire2/scratch/w13813/score.sh
# 175 shape:
cd /home/debian/GalaxyOnFire2/scratch/w13813 && python3 -c "import B; B.run([], 'plain175')"
# Ghidra:
cd /home/debian/GalaxyOnFire2 && (ulimit -v 30000000; timeout 300 /tmp/pyghidra-venv/bin/python \
  .tmp/ghidra_decomp2.py /home/debian/GalaxyOnFire2/_work/bins android_2.0.16_libgof2hdaa.so_ghidra \
  / android_2.0.16_libgof2hdaa.so 0x180d38)
```
