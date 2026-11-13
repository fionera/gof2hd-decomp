# SpaceLounge::drawLounge — wave-191b: 175/41 original shape re-measured, no net win (w13800)

Status: research note (2026-11-13). Worktree `wt/w13800`, branch
`wave191b-w13800`, base `58f9196da`. **No `src/` change landed** — every
size-exact source shape tested this session is either the committed base
score or the 175-shape score; none beats raw 138. Tree is clean apart from
this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**,
  `.text` diff **75,196**, `.ARM.extab` diff **587**
* after: unchanged (note only)

Prior work read first: `...-w13629.md` (true source shape 175/41),
`...-w13675.md` (SIZEVREG prize 123/72), `...-w13709.md` (per-region byte
split), `...-w13737.md` (pad-load/reload sweep), `...-w13753.md`
(source-shape bimodality), and `status-generator-spacelounge-wave155-w10110.md`
§3. This note re-measures the 175 shape on the wave-191b base, adds ~40 fresh
cells on the clamp arm, declaration order, `lineHeight` placement, local
`layout` vs `Globals::layout` access, and instrumented-clang `NOEVICT` /
`HINTVREG` probes, and confirms the source space is bimodal.

## 1. Baseline reproduction

`bash scratch/w13800/score.sh` (incremental `link_so` + `tools/fn_diff.py`
+ section diff) reproduces the committed gate:

```
_ZN11SpaceLounge10drawLoungeEv  raw=138 aligned=87 size=1948/1948 insn=739/741
.text       diff=75196
.ARM.extab  diff=587
```

## 2. The 175 shape is still the structural original

The original disassembly computes `left = x - pad` and `above = top - pad`
once (`subs r6, r3, r1` / `sub.w r5, r2, r9`) and reuses them in both
`drawBox` (`mov r2, r6` / `mov r3, r5`) and `DrawRectangle`
(`mov r1, r6` / `mov r2, r5`). The `DrawRectangle` height is a *reload* of
`layout->field_0x30_rowHeight`, not the `lineHeight` local. Spelling:

```cpp
int left = x - pad;
int above = top - pad;
int lineHeight = layout->field_0x30_rowHeight;
layout->drawBox(2, left, above, pad * 2 + width, lineHeight, String(""));
Globals::Canvas->SetColor(0x193043ffu);
Globals::Canvas->DrawRectangle(left, above, pad * 2 + width,
                               Globals::layout->field_0x30_rowHeight);
```

Measured on this base: **raw 175 / aligned 41 / 1948 B / 741 insns /
`.text` 75,233 / extab 590**. The conversation branch (`0x1dc..0x576`) is
byte-identical; all 41 aligned diffs are the single `pad`/`x` colouring
(`pad = fp` vs original `r9`, `x = r9` whole vs original `r5 -> r3 -> r9`).
The committed base differs from this shape only in the `DrawRectangle` x
argument (`x - Globals::layout->field_0x94`, a redundant `field_0x94`
reload) and consequently colours `pad = sl, pad2 = fp, lineHeight = sl,
x = r9` with a 4-instruction clamp arm.

## 3. Measured table (real gold link, `tools/fn_diff.py`)

Base = **138 / 87 / 1948 / 739**, `.text` 75,196, extab 587. All rows are
size 1948/1948 unless the size column says otherwise.

| variant | raw | aligned | size | `.text` | extab |
|---|---:|---:|---:|---:|---:|
| committed base | **138** | **87** | 1948 | **75,196** | **587** |
| 175 shape (`left`/`above` reused, rect x/y = `left`/`above`) | 175 | **41** | 1948 | 75,233 | 590 |
| 175 + arm `- (width + pad + pad)` | 176 | 42 | 1948 | 75,234 | 590 |
| 175 + arm `- (width + pad * 2)` | 445 | 54 | 1948 | 75,503 | 602 |
| 175 + arm `+ (-width - pad * 2)` | 445 | 54 | 1948 | 75,503 | 602 |
| 175 + arm `+ (-width - (pad << 1))` | 445 | 54 | 1948 | 75,503 | 602 |
| 175 + arm `- width - pad - pad` (sequential) | 468 | 64 | **1944** | 76,844 | 609 |
| 175 + named `pad2` in top/arm/width | 445 | 54 | 1948 | 75,503 | 602 |
| 175 + `above` declared before `left` | 265 | 50 | 1948 | 75,323 | 593 |
| 175 + `int left = x - pad, above = top - pad;` | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + `int left(x - pad); int above(top - pad);` | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + `int left = -pad + x;` | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + `x -= pad;` (rect/text use `x + pad`) | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + `lineHeight` declared before the clamp | 588 | 79 | **1952** | — | 1296 |
| 175 + `lineHeight` inline in `drawBox` | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + clamp condition `x + width + pad > …` | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + clamp condition `width + pad + x > …` | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + clamp condition `x + (pad + width) > …` | 175 | 41 | 1948 | 75,233 | 590 |
| 175 + `int drawX = x` for both `DrawString`s | 1567 | 266 | **1956** | — | 1308 |
| base + rect x via local `layout->field_0x94` | 1186 | 177 | **1952** | — | 1304 |
| base + rect h via local `layout->field_0x30_rowHeight` | 142 | 93 | 1948 | 75,200 | 587 |
| base + rect x *and* rect h via local `layout` | 141 | 91 | 1948 | 75,199 | 587 |
| base + `lineHeight` declared before the clamp | 606 | 88 | **1952** | — | 1295 |
| base + named `pad2` in top/arm/width | 1553 | 167 | **1960** | — | 1307 |
| base + arm `- (width + pad * 2)` | 1553 | 167 | **1960** | — | 1307 |
| 175 + clamp condition `x < inset + pad` | 492 | 73 | **1944** | 76,868 | 606 |
| 175 + clamp condition `x + width > w - inset - pad` | 477 | 60 | **1952** | — | 1293 |
| 175 + clamp condition `pad + x + width + inset > w` | 524 | 100 | **1940** | 189,139 | 607 |

The two `lineHeight`-placement rows were re-measured on the 175 shape (the
earlier values were base-shape numbers): declaring `lineHeight` before the
clamp is size-fatal (1952), while inlining the `field_0x30_rowHeight` load in
`drawBox` is an invariant (byte-identical to the 175 base row).

Prior-session cells re-confirmed on this base: 175 − `__builtin_expect`
anchor = 550/76/**1952**/extab 1296; 175 + `expect(...,0)` =
1879/205/**1980**/extab 1316; 175 + expect operand `!=0`/`!!`/`==1`/`(bool)`
= 175/41/1948/75,233/590.

## 4. Instrumented-clang probes (new)

Using the instrumented clang `/tmp/llvm_android_702b/build2/bin/clang++` and
the direct gold-link harness (`scratch/w13800/ilink.sh`, which reproduces the
base at 138/87/1948):

| knob on the 175 shape | raw | aligned | size |
|---|---:|---:|---:|
| none | **175** | **41** | 1948 |
| `NOEVICT=2147483650` (pad may not be evicted) | 1905 | 239 | **1996** |
| `HINTVREG=2147483650:75` (pad hint → r9) | 1883 | 209 | **1980** |

Both are size-fatal: pinning `pad` to a register breaks the allocator's
rematerialisation of the `Globals::layout` GOT temp (`%365`) and the
`pad`/`pad2` CSE, so the body grows. This confirms the 175 shape's `pad = fp`
is a stable local optimum, not a near-miss that a hint can nudge to `r9`.

## 5. Residual analysis (why the 175 shape loses raw)

The 175 shape's 41 aligned diffs are all one coupled fact, and the raw cost
is high because the clamp-arm instruction count differs (boundaries shift),
not just a register field:

1. **`pad → fp` instead of `r9`.** In the greedy-RA trace on the 175 shape
   (`RA_TRACE=1`), `%2` (pad, size 6400) is selected after every
   hint-carrying vreg; all callee-saved registers are taken, so it evicts
   `%365` (the `Globals::layout` GOT temp) from `r11`/fp. `%365` then fails
   to find a register and is rematerialised. Later `%46` (`lineHeight`)
   evicts `%2` from `r11`, so `pad` is rematerialised into `fp` at the use
   site (`ldr.w fp, [r6, #148]`).
2. **`x → r9` whole instead of `r5 → r3 → r9`.** `%609` (x) is assigned
   `$r5` first, then evicted by `%45` (`above = top - pad`, size 736,
   hint `r3`) via `TRYASSIGN2` (r9 is free but has `CostPerUse=1`, so the
   allocator prefers the cheap `r5`). `%609` is re-enqueued and takes `r9`
   whole. The original instead keeps `x` in `r5` through the clamp and
   copies it `r5 → r3 → r9` only after `pad`'s last use at `171304`.
3. **Clamp arm is 4 instructions, not 2.** The original's
   `itt gt; addgt.w r1, r6, sl; subgt r5, r0, r1` reuses `sl = pad*2`.
   Every 1948-byte spelling of the arm emits the 4-instruction
   `itttt gt; movgt r1,#0; subgt.w r1,r1,pad<<1; subgt r1,r1,width;
   addgt.w x,r1,(w-inset)` form, because the compiler rematerialises
   `pad*2` from `fp` rather than reusing `sl`. The only spelling that
   produces the 2-instruction arm (`- width - pad - pad`, sequential)
   shrinks the body to 1944 (gate-fatal).
4. **Downstream `sl`/`fp` conversation-branch swap.** In the base shape
   `pad = sl` matches the original's `Globals::Canvas` in `sl`, so the
   conversation branch pays only ~7 raw bytes; in the 175 shape `pad = fp`
   swaps `sl`/`fp` through the whole conversation branch (but there the
   branch is byte-identical because the rest of the overview schedule also
   moved). Per-region split (w13709): base = conv 7 + overview 94 +
   epilogue 37 = 138; 175 = conv 0 + overview 138 + epilogue 37 = 175.
5. **Epilogue / LSDA tail.** Both shapes share the same 37 epilogue bytes
   (branch-displacement shifts from the overview length difference) and the
   same `.ARM.extab` personality/LSDA layout; extab grows 587 → 590 with
   the 175 shape (net loss on top of the raw loss).
6. **Literal pool.** The final `str r1, [sp, #528]` (original) vs
   `[sp, #496]` (ours) is a stack-slot difference from the `String`
   temporaries, unchanged by any overview spelling tested.

The prize remains the instrumented `SIZEVREG=%2:9500` colouring (123/72,
conversation branch exact, `.text` −15 / extab +5 = net −10 B), but no
source spelling raises `pad`'s enqueue size from 6528 past 9500 without
changing the schedule; the 175 shape already has the target `fp`/`sl` pair
but pays 52 more overview bytes.

## 6. Conclusion

The committed base is still the best size-exact source shape:
**raw 138 / aligned 87 / 1948 B / 739 insns / `.text` 75,196 / extab 587**.
The 175/41 shape is the structural original but scores 175 raw and costs
+37 `.text` / +3 extab. No `src/` change is landed; the gate is unchanged
(byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
imports 207, lint CLEAN, drift 0, parity 5370/5370). `cmake --build
cmake-build-match --target check -j2` after restoring pristine `src/`:
`RATCHET PASS` (verify byte 4508 / linked 4508 / stubs 0 / missing 0 /
extra 0, data 843/843, imports 207).

## 7. Reproduction

```bash
# incremental build + score (reproduces 138/87/1948, .text 75196, extab 587)
bash /home/debian/GalaxyOnFire2/scratch/w13800/score.sh

# direct gold-link harness for instrumented clang (NOEVICT/HINTVREG/...)
NOEVICT=2147483650 bash /home/debian/GalaxyOnFire2/scratch/w13800/ilink.sh
HINTVREG=2147483650:75 bash /home/debian/GalaxyOnFire2/scratch/w13800/ilink.sh
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13800` (`score.sh`,
`secdiff.py`, `detail.py`, `rawdiff.py`, `icompile.sh`, `ilink.sh`,
`vtry2.py`, `measure_all.py`, `gen*.py`, `SpaceLounge.base.cpp`,
`SpaceLounge.175.cpp`, `ra175*.log`, `diff138.txt`, `diff175.txt`,
`orig.txt`, `ours.txt`, `ours175.txt`).
