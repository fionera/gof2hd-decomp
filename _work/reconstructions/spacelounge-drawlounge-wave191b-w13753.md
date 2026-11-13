# SpaceLounge::drawLounge — wave-191b: source-shape bimodality (w13753)

Status: research note (2026-11-13). Worktree `wt/w13753`, branch
`wave191b-w13753`, base `32b89cb8a`. **No `src/` change landed** — every
size-exact source shape tested this session is either the committed base
score or the 175-shape score; no shape lands strictly between them and none
beats raw 138. Tree is clean apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` (1948/1948 B).

* baseline: raw **138**, aligned **87**, size **1948/1948**, insn **739/741**,
  `.text` diff **72954**, `.ARM.extab` diff **564**, `.ARM.exidx` 0,
  `.rodata` 0
* after: unchanged (note only)

Prior work read first: `spacelounge-drawlounge-wave191b-w13737.md` (pad
load/reload/clamp sweep), `...-w13709.md`, `...-w13675.md`, `...-w13629.md`,
`...-w13620.md`, `...-w13566.md`, `spacelounge-drawlounge-wave191-w13554.md`.
This note does not repeat the pad-load-placement sweep; it attacks the
*whole-block* source shape (local-variable reuse, clamp-arm association,
`Globals::layout`/`Globals::Canvas` hoisting, `top`/`above` derivation,
`boxW`/`boxH` locals) and reports the resulting bimodality.

## 0. Harness (reproduces the committed base exactly)

`scratch/w13753/measure.sh <cand.cpp> <label>` compiles a full-TU candidate
with the exact `ninja -t commands` clang line (NDK r18b clang 7.0.2) and the
saved gold-link command directly, then runs `tools/fn_diff.py`. Verified this
session: `./measure.sh base.cpp basechk10` → **138 / 87 / 1948 / 739**,
`.text` 72954, extab 564 (matches the gate's committed numbers).

* `measure_inst.sh <cand.cpp> <label> [ENV=V ...]` — same but swaps in the
  instrumented clang `/tmp/llvm_android_702b/build2/bin/clang++`.
* `drv.py <cases.json>` — wraps a body (the `if (this->mode == 0) { ... }`
  overview block only) into the base TU, measures, prints
  raw/aligned/size/insn, deletes `.o/.so`.
* `secdiff.py <so>` — per-section diff vs the original.
* `bytediff.py <so>` — differing byte runs.
* `sbs.py` (run from repo root) — normalised instruction side-by-side.

## 1. The original overview structure (re-confirmed)

The original overview is the **175 shape**: `left`/`above` locals reused by
both `drawBox` and `DrawRectangle`, no `field_0x94` reload in the rect:

```
int left = x - pad;
int above = top - pad;
int lineHeight = layout->field_0x30_rowHeight;
layout->drawBox(2, left, above, pad * 2 + width, lineHeight, String(""));
...
DrawRectangle(left, above, pad * 2 + width, Globals::layout->field_0x30_rowHeight);
```

Original registers across the overview: **pad=r9**, **pad2=sl**
(`mov.w sl,r9,lsl #1` at `170ed2`), **top=r8**, **lineHeight=fp**,
**width=r6**, **x=r5** at the clamp (`vmov r5,s18` at `1712b2`) then
`mov r3,r5` / `mov r9,r3` after pad's last use at `171304`. The original's
clamp arm is two instructions (`itt gt; addgt.w r1,r6,sl; subgt r5,r0,r1`)
because `sl` already holds `pad2`; the original's `DrawRectangle` x is
`mov r1,r6` (= `left`), and the `mov fp,r8` / `ldr.w r8,[sp,#12]` pair at
`17132e/171330` is an agent/top register swap around the call.

Committed base differs only in the rect expression
(`x - Globals::layout->field_0x94`, a redundant load) and consequently in
colouring: **pad=sl, pad2=fp, lineHeight=sl, x=r9**, 4-insn clamp arm.
The 175 shape matches the original's whole conversation branch byte-for-byte
but recolours the overview.

## 2. Measured table (real gold link, `tools/fn_diff.py`)

Base = **138 / 87 / 1948 / 739**, `.text` 72954, extab 564. All rows
size 1948/1948 unless noted.

| cell | raw | aligned | size | insn | .text |
|---|---:|---:|---:|---:|---:|
| committed base | **138** | **87** | 1948 | 739 | **72954** |
| 175 (`left`/`above` reused) | 175 | **41** | 1948 | 741 | 72991 |
| 175 + rect x = `x - field_0x94` (= base) | 138 | 87 | 1948 | 739 | 72954 |
| `base_arm_sub_pp` (`- (width + pad + pad)`) | 139 | 88 | 1948 | 739 | 72955 |
| `d_base_boxH_local` (rect h = `lineHeight`) | 141 | 87 | 1948 | 738 | — |

### 2.1 Local-variable / argument-reuse shapes (cases27–35)

Every size-exact cell is either the base attractor or the 175 attractor.
`raw` is shown; `aligned` is in parentheses.

| shape | raw (aligned) | size | insn |
|---|---:|---:|---:|
| `const int pad` | 138 (87) | 1948 | 739 |
| `int pad2 = pad + pad` used only in `top` | 138 (87) | 1948 | 739 |
| `int pad2` used in `top` + box width | 517 (79) | **1952** | 741 |
| `pad2 = pad * 2` / `pad << 1` | 138 (87) | 1948 | 739 |
| `int pad2 = pad + Globals::layout->field_0x94` | 1866 (211) | **1972** | 743 |
| `pad2` used in arm only | 1553 (167) | **1960** | 735 |
| `pad2` used in arm + box | 1553 (167) | **1960** | 735 |
| `pad2` in arm + box + rect x = `x - pad` | 445 (54) | 1948 | 741 |
| `SetColor` moved after `centerX` | 138 (87) | 1948 | 739 |
| `int drawX = x` used in DrawString only | 138 (87) | 1948 | 739 |
| `int clampX = x` copy before clamp | 138 (87) | 1948 | 739 |
| `int drawX = x` used everywhere | 138 (87) | 1948 | 739 |
| `pad` moved after `SetColor` | 1872 (208) | **1976** | 744 |
| `Layout *layout = Globals::layout` first | 1153 (226) | **1936** | 727 |
| `Globals::w` local | 138 (87) | 1948 | 739 |
| clamp `centerX` in place (no `x` local) | 138 (87) | 1948 | 739 |
| clamp `centerX`, then `int x = centerX` | 138 (87) | 1948 | 739 |
| 175 with `centerX` clamped in place | 175 (41) | 1948 | 741 |
| 175 with `x` declared after the clamp | 175 (41) | 1948 | 741 |
| 175 `left`/`above` after `lineHeight` | 175 (41) | 1948 | 741 |
| 175 `left` only | 175 (41) | 1948 | 741 |
| 175 `above` only | 138 (87) | 1948 | 739 |
| 175 `boxW` local | 175 (41) | 1948 | 741 |
| 175 `boxW`+`boxH` locals | 990 (162) | 1948 | 737 |
| 175 `boxH` local (rect h = `lineHeight`) | 990 (162) | 1948 | 737 |
| base `boxW` local | 517 (79) | **1952** | 741 |
| base `boxH` local (rect h = `lineHeight`) | 141 (87) | 1948 | 738 |
| `PaintCanvas *canvas = Globals::Canvas` local | 1855 (264) | **1964** | 736 |
| `drawBox(..., String())` default | 484 (95) | **1940** | 735 |
| `float topF` then `int top = (int) topF` | 138 (87) | 1948 | 739 |
| `above` first, `top = above + pad` | 517 (79) | **1952** | 741 |
| `top = (int) bottomRight.y - pad - pad` | 1860 (216) | **1960** | 737 |

### 2.2 Clamp-arm association (cases30/32)

`pad + x + width` condition with different arm spellings:

| arm | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| `+ (-width - pad - pad)` (base) | **138** | **87** | 1948 | 739 |
| `+ (-(width + pad + pad))` | 139 | 88 | 1948 | 739 |
| `+ (-width - (pad + pad))` | 1553 | 167 | **1960** | 735 |
| `+ (-width - pad2)` (pad2 local) | 1553 | 167 | **1960** | 735 |
| `+ (-(width + pad2))` | 1553 | 167 | **1960** | 735 |
| `x + pad + width` condition (any arm) | 138 | 87 | 1948 | 739 |
| `width + pad + x` condition + `- (width+pad+pad)` | 521 | 103 | **1944** | 737 |

The `-(width + pad + pad)` arm is the only one that reaches the original's
2-instruction shape, but it costs one raw byte (139) and one aligned
instruction (88) — the closest near-miss found.

### 2.3 Extra `field_0x94` reload on the 175 shape (cases31)

| reload site | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| rect x (`x - field_0x94`) | 138 | 87 | 1948 | 739 |
| arm (`-width - field_0x94 - field_0x94`) | 196 | 44 | 1948 | 741 |
| box x | 1884 | 167 | **1980** | 747 |
| box y | 1873 | 168 | **1976** | 744 |
| box width | 1880 | 243 | **1988** | 749 |
| rect y | 1848 | 177 | **1964** | 739 |
| rect width | 1861 | 259 | **1964** | 744 |
| `top` | 1869 | 200 | **1972** | 744 |

Only the rect-x reload is size-exact and it collapses to the base attractor.

## 3. Instrumented-clang weight probe (fine sweep, 175 shape)

`VRW=2147483650:<f>` (multiply pad's spill weight; TU-wide, so `.text` is
polluted but the drawLounge body is valid):

| factor | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| 1.36 | 203 | 46 | 1948 | 741 |
| 1.38–2.0 | 194 | 63 | 1948 | 742 |
| 2.5–3.0 | 562 | 173 | **1952** | 742 |

At ≥1.38 pad moves to r9 (`ldr.w r9,[r6,#148]`, `mov.w sl,r9,lsl #1`) but
`top`/`lineHeight` swap to fp/r8 (original is r8/fp) and the clamp arm stays
4 instructions, so raw rises to 194. There is no factor that yields
pad=r9 with top=r8 / lineHeight=fp.

## 4. Why the source space is bimodal

The overview block has exactly two stable colourings under LLVM 7.0.2 greedy
RA at `-Oz`:

* **base attractor** (rect x reloads `field_0x94`): pad=sl, pad2=fp,
  lineHeight=sl, x=r9, 4-insn arm → 138/87, conv branch pays ~19 bytes of
  `sl`/`fp` swaps.
* **175 attractor** (rect x = `left`): pad=fp, pad2=sl, lineHeight=fp,
  x=r9, 4-insn arm → 175/41, conv branch byte-identical.

The original is a third point (pad=r9, pad2=sl, lineHeight=fp, x=r5→r9,
2-insn arm) that neither attractor reaches. The gap is not a pad-load
placement problem (w13737 already swept that); it is that our single `x`
vreg is live from the clamp to the last `DrawString`, so `above` evicts it
to r9, whereas the original's `x` is copied r5→r3→r9 *after* pad's last
use. Any extra `field_0x94` load that forces pad→r9 also re-schedules
`str.w r8,[sp,#12]` earlier and swaps top/lineHeight.

## 5. Section deltas (this session, full gold link)

| shape | .text | .ARM.extab | .ARM.exidx | .rodata |
|---|---:|---:|---:|---:|
| base (138/87) | 72954 | 564 | 0 | 0 |
| 175 (175/41) | 72991 | 567 | 0 | 0 |
| `base_arm_sub_pp` (139/88) | 72955 | 564 | 0 | 0 |

The 175 shape costs +37 `.text` and +3 extab, so it is a net loss even
ignoring raw.

## 6. Conclusion

No size-exact source shape found this session beats the committed base
**raw 138 / aligned 87 / 1948 B / 739 insns / .text 72954 / extab 564**.
The nearest miss is `base_arm_sub_pp` at 139/88 (one raw byte worse).
The 175 shape is the structural original and a large *aligned* win
(87 → 41) but a raw/.text/extab loss (138 → 175, 72954 → 72991, 564 → 567).
No `src/` change is landed.

## 7. Reproduction

```bash
cd /home/debian/GalaxyOnFire2/scratch/w13753
./measure.sh base.cpp basechk10                 # 138/87/1948/739
./measure.sh v_reuse175.cpp v175chk             # 175/41/1948/741
python3 drv.py cases27.json                     # .. cases35.json
python3 secdiff.py basechk10.so
python3 /home/debian/GalaxyOnFire2/wt/w13753/tools/fn_diff.py \
  --ours basechk10.so _ZN11SpaceLounge10drawLoungeEv
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13753` (`measure.sh`,
`measure_inst.sh`, `drv.py`, `cases27.json`..`cases35.json`, `base.cpp`,
`v_reuse175.cpp`, `secdiff.py`, `bytediff.py`, `sbs.py`, `padreg.py`).
