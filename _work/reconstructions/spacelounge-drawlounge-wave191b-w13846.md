# SpaceLounge::drawLounge — wave-191b w13846: clamp-association, pad2-local and
# rect-reuse sweeps (no win)

Status: research note (2026-11-13). Worktree `wt/w13846`, branch
`wave191b-w13846`, base `6382c7abf`. **No `src/` change landed** — every
size-exact source shape measured this session scores worse than the committed
base; the tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` in `src/game/menu/SpaceLounge.cpp`
(function starts at line 1184; overview block lines 1185–1245).

Prior work read first: `...-w13813.md` (Ghidra confirmation, clamp table),
`...-w13800.md` (bimodality), `...-w13785.md` (RA trace, 175/41 attractor),
`...-w13753.md` (register-colouring map), `...-w13737.md` (pad load/reload),
`...-w13709.md` (per-region byte split), `...-w13675.md` (SIZEVREG).

## 1. Baseline re-verified

```text
_ZN11SpaceLounge10drawLoungeEv  raw=138 aligned=87 size=1948/1948 insn=739/741
.text diff 75,882   .ARM.extab diff 552   .ARM.exidx 0   .rodata 0
```

`bytediff.py` reports TOTAL 108 differing bytes over 24 real instruction cells:
7 early conversation-block `sl`↔`fp` Canvas-pointer swaps (`@0x02c`–`@0x092`
plus the `ldrd fp/r8` at `@0x046`), the overview `pad`/`pad2` colouring
(`@0x186`–`@0x25e`), the ~14-cell clamp block (`@0x57a`–`@0x5de`), the rect
reuse/tail swap (`@0x5f6`–`@0x60c`), 8 EH branch targets, the epilogue `nop`
(`@0x76a`) and the `.ARM.extab` frame-slot word (`@0x78c`, `[sp,#528]` vs
`[sp,#496]`).

## 2. Original clamp block, decoded

Real `.text` `0x1712b0`–`0x1712e2`:

```text
1712b0  add   r6, r4              ; width += <textWidth>
1712b2  vmov  r5, s18             ; r5 = centerX
1712b6  ldr   r0, [sp,#20] ; ldr r4,[r0] ; ldr r0,[r4,#40]   ; inset
1712bc  sub.w r1, r5, r9          ; x - pad
1712c0  cmp   r1, r0
1712c2  bge   1712ca
1712c4  add.w r5, r0, r9          ; inset + pad
1712c8  b     1712e4
1712ca  ldr   r1, [pc] ; add r1,pc ; ldr r1,[r1] ; ldr r1,[r1]   ; Globals::w
1712d2  subs  r0, r1, r0          ; w - inset
1712d4  add.w r1, r9, r5          ; pad + x
1712d8  add   r1, r6              ; (pad + x) + width
1712da  cmp   r1, r0
1712dc  itt   gt
1712de  addgt.w r1, r6, sl        ; width + pad2   (sl == pad+pad)
1712e2  subgt r5, r0, r1          ; (w - inset) - (width + pad2)
```

So the original source is the **right-associated** subtraction
`x = Globals::w - layout->buttonInsetX - (width + pad2)` with an explicit
`pad2 = pad + pad` local that is also used for `top`. The committed base
instead folds it as `+ (-width - pad - pad)` (clang emits
`movgt r1,#0; subgt.w r1,r1,sl,lsl #1; subgt r1,r1,r6; addgt.w r9,r1,r0`),
and the base's `x` sits in `r9` while the original's sits in `r5`.

The original rect call (`0x17132e`–`0x171344`) reuses the drawBox temporaries:
`mov fp,r8; ldr.w r8,[sp,#12]`, then `r1 = r6 = x-pad`, `r2 = r5 = top-pad`,
`r3 = sl = pad2+width`, `[sp] = layout->field_0x30_rowHeight`. That is the
`rect_rx` shape (rect reuses `x - pad`), which is the known 175/41 attractor.

## 3. New sweeps this session (all `batchfull.py`, base `HEAD` source)

`v11.json` — `pad2 = pad + pad` named early, `top = ... - (float) pad2`,
Ghidra-shaped clamp arm:

| variant | raw | aligned | size | insn | .text | extab |
|---|---:|---:|---:|---:|---:|---:|
| `A_gh_pad2_inplace` (in-place clamp on centerX, rect reuses x-pad) | 445 | 54 | 1948 | 741 | 76,189 | 567 |
| `B_gh_pad2_inplace_reload` (rect x reloads `field_0x94`) | 1553 | 167 | **1960** | 735 | 240,342 | 1272 |
| `C_gh_pad2_inplace_padmul` (`pad*2 + width`) | 445 | 54 | 1948 | 741 | 76,189 | 567 |
| `D_gh_pad2_else` (`int x; else { x = centerX; }`) | 445 | 54 | 1948 | 741 | 76,189 | 567 |
| `E_gh_pad2_tmp` (`int clamped = ...; x = clamped;`) | 445 | 54 | 1948 | 741 | 76,189 | 567 |
| `F_gh_pad2_wfirst` (`width + pad2` operand order) | 445 | 54 | 1948 | 741 | 76,189 | 567 |
| `G_gh_pad2_h_local` (rect height = `lineHeight` local) | 1175 | 144 | **1944** | 735 | 78,237 | 582 |
| `H_gh_pad2_reload_xy` (rect x and y reload `field_0x94`) | 525 | 89 | **1940** | 735 | 189,757 | 574 |

All six 445/54 rows are byte-identical to each other: the allocator treats the
in-place/tmp/else spellings and both operand orders identically. The 445
shape's clamp arm is structurally exact (`itt gt; addgt.w r1,r6,sl;
subgt.w r9,r0,r1` vs orig `subgt r5,r0,r1`); its loss is that `pad` lands in
`fp` instead of `r9` and `x` in `r9` instead of `r5`, plus an extra
`mov r5,fp` in the fall-through arm and the tail shifts.

`v12.json` — condition/arm association sweep on the **base** body (36
variants). Every condition spelling (`pad+x+width`, `x+pad+width`,
`width+pad+x`, `pad+width+x`, `x+width+pad`, `width+x+pad`) is a no-op:
138/87/1948/739/75,882/552. Arm spellings:

| arm | raw | aligned | size | insn | .text | extab |
|---|---:|---:|---:|---:|---:|---:|
| `- (width + pad + pad)` (a1) | 139 | 88 | 1948 | 739 | 75,883 | 552 |
| `+ (-(width + pad + pad))` (a5) | 139 | 88 | 1948 | 739 | 75,883 | 552 |
| `- (width + pad * 2)` (a2) | 1553 | 167 | **1960** | 735 | 240,342 | 1272 |
| `- (layout->buttonInsetX + width + pad + pad)` (a3) | 533 | 90 | **1952** | 741 | 238,130 | 1257 |
| `- width - pad - pad` (a4) | 481 | 100 | **1944** | 738 | 77,543 | 573 |

`a1`/`a5` are the same 139/88 near-miss as `gh_inplace`/`right_local`; their
bytediff is cell-for-cell the base diff plus one extra cell, i.e. clang still
reassociates the subtraction to the base form and only the `x`/`pad` colours
differ. No condition×arm combination beats 138.

`v13.json` — rect/drawBox lifetime sweep:

| variant | raw | aligned | size | insn | .text | extab |
|---|---:|---:|---:|---:|---:|---:|
| `lh_inline` (drop `lineHeight` local, inline `layout->field_0x30_rowHeight`) | 138 | 87 | 1948 | 739 | 75,882 | 552 |
| `lh_inline_globals` (`Globals::layout->...` inline) | 138 | 87 | 1948 | 739 | 75,882 | 552 |
| `rect_rx` (rect x = `x - pad`) | 175 | 41 | 1948 | 741 | 75,919 | 555 |
| `rect_w_pad2` (`pad + pad + width` in rect) | 517 | 79 | **1952** | 741 | 238,114 | 1257 |
| `box_w_pad2` (`pad + pad + width` in drawBox) | 517 | 79 | **1952** | 741 | 238,114 | 1257 |
| `rect_rx_w` (both) | 1851 | 250 | **1964** | 743 | 240,581 | 1276 |

`lh_inline`/`lh_inline_globals` being exact no-ops confirms the `lineHeight`
local is irrelevant to codegen (clang already rematerialises the field load).

## 4. Why the base wins on the gate metric

The structurally-correct original source (explicit `pad2` local + right-assoc
`- (width + pad2)` + rect reusing `x - pad`) is the 445/54 shape. The base
deliberately reloads `pad` for the rect x and folds the clamp arm as
`+ (-width - pad - pad)`; that happens to colour `pad`→`sl`, `pad2`→`fp`,
`x`→`r9` and reproduces more literal bytes (138) than the exact structure
(445), at the cost of the 2-instruction `mov fp,r8; ldr.w r8,[sp,#12]` rect
swap and the epilogue `nop`. Every attempt to move `pad2` into a named local
or to reuse `x - pad` in the rect call pushes the allocator off the base
colouring by more bytes than it recovers.

RA trace (`RA_TRACE` instrumented clang, base body): `%609` (`x`) is hinted
`$r0`, selected first → `$r6`/`$r5`, evicted by `%46` (`top - pad`) and
re-selected as `$r9`; `%2` (`pad`) then lands `$r10`/`fp`; `%610` (the
`pad2` value) ends in `$r9` after a re-enqueue. `NOHINT=2147484257` (x) moves
pad to `$r4`; `HINTVREG=2147483650:9` (pad→r9) moves pad to `$r6`/fp; neither
knob alone reproduces orig's `x=r5, pad=r9, pad2=sl`.

## 5. Conclusion

The committed base remains the best size-exact shape:
**raw 138 / aligned 87 / 1948 B / 739 insns / `.text` 75,882 / extab 552**.
The 175/41 and 445/54 shapes are the structurally-correct original but score
worse raw. No `src/` change is landed; the gate is unchanged (byte_exact
4508/4524, linked_exact 4508/4524, size-exact 4524/4524, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370).

## 6. Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13846`:

```bash
cd /home/debian/GalaxyOnFire2/scratch/w13846
python3 batchfull.py v11.json v11_res.json     # pad2-local sweep
python3 batchfull.py v12.json v12_res.json     # condition/arm association
python3 batchfull.py v13.json v13_res.json     # rect/drawBox lifetime
python3 bytediff.py                            # per-instruction byte diff
# RA trace (instrumented clang, must run from cmake-build-match/):
cd /home/debian/GalaxyOnFire2/wt/w13846/cmake-build-match && \
  (ulimit -v 12000000; RA_TRACE=1 timeout 180 bash /home/debian/GalaxyOnFire2/scratch/w13846/cc_inst.sh > log 2>&1)
```

`batchfull.py` applies textual replacements to `git show HEAD:src/game/menu/
SpaceLounge.cpp` (never to the live tree), so it is safe to re-run on a clean
worktree.
