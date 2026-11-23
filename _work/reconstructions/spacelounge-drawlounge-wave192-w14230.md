# SpaceLounge::drawLounge — wave-192 w14230: declaration/literal-order probes (no win)

Status: research note (2026-11-23). Worktree `wt/w14230`, branch
`wave192-w14230`, base `02c4622b7`. **No `src/` change landed** — the committed
base remains the best size-exact shape (175 raw / 41 aligned). Tree is clean
apart from this note.

Target: `_ZN11SpaceLounge10drawLoungeEv` in `src/game/menu/SpaceLounge.cpp`
(function starts at line 1184; overview block lines 1185–1245).

Prior work read first: `...-wave191b-w13846.md`, `...-w13813.md`, `...-w13785.md`,
`...-w13753.md`, `...-w13737.md`, `...-w13709.md`, `...-w13675.md`.

## 1. Baseline (re-verified)

```text
_ZN11SpaceLounge10drawLoungeEv  raw=175 aligned=41 size=1948/1948 insn=741/741
```

Gate unchanged: byte_exact 4508/4524, linked 4508/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
Section deltas: `.text` 74,628; `.ARM.extab` content 562; `.ARM.exidx` 0;
`.rodata` 0.

## 2. Probes this wave (all worse than or equal to base; size-exact unless noted)

| # | source shape | raw | aligned | size | insn |
|---|---|---:|---:|---:|---:|
| p1 | named `String empty("")` reuse in role fallback | 1488 | 239 | 1972 | 752 |
| p2 | `Layout *layout` declaration moved before `width` | 1870 | 194 | 1968 | 743 |
| p3 | `pad2` local used for `top` only | 175 | 41 | 1948 | 741 |
| p4 | `top = (int)(br.y - (float)pad - (float)pad)` | 1845 | 163 | 1968 | 742 |
| p5 | named `int boxX = x-pad; int boxY = top-pad` | 175 | 41 | 1948 | 741 |
| p6 | `int x = centerX` declared right after `top` | 175 | 41 | 1948 | 741 |
| p7 | `pad2 = pad + pad` early, used in top/box/rect widths and clamp | 175 | 41 | 1948 | 741 |
| p8 | `pad2` after `SetColor` + right-assoc clamp `w - inset - (width + pad2)` | 445 | 54 | 1948 | 741 |
| p9 | `pad2` after `SetColor` + arm `+ (-width - pad2)` only | 445 | 54 | 1948 | 741 |
| p10 | single `int x = centerX` (centerX folded away entirely) | 175 | 41 | 1948 | 741 |

Notable: every shape that keeps `1948/1948` collapses to the same 175/41
instruction scheduler/allocator attractor. Local renaming and declaration
position around the overview block are effectively no-ops; materialising
`pad2` as a real local (the shape Ghidra reports as original) costs bytes or
schedules worse.

## 3. Established hunk evidence (carried forward, do not re-derive)

11 hunks (`tools/hunks.py`). Core differences:

- h1/h2: `pad` load colouring, orig `pad=r9`, ours `pad=fp`.
- h3/h4 (clamp + rect): orig emits `itt gt; addgt.w r1,r6,sl;
  subgt r5,r0,r1` — `x` stays **r5**, `pad` **r9**, `pad2` **sl**, `width`
  **r6**. Ours emits `itttt gt; movgt r1,#0; subgt.w r1,r1,fp,lsl#1;
  subgt r1,r1,r6; addgt.w r9,r1,r0` — `x=r9`, `pad=fp`. Original source is
  right-associated `w - inset - (width + pad2)` with explicit
  `pad2 = pad+pad`; committed base folds as `+ (-width - pad - pad)`.
- Original `pad` load precedes `SetColor`; `sl = pad*2` is emitted right after
  the `SetColor` blx at `0x170ed2`; rect reuses `x-pad`/`top-pad` from
  `drawBox`.
- h5: orig `add r0,fp` (top+field_0x2c0) vs ours `add r0,r8`.
- h11: `.ARM.extab` frame slot `[sp,#528]` orig vs `[sp,#496]` ours.

Register-allocator trace (prior waves): `x` `%609` hinted r0→r6/r5, evicted by
`%46`, reselected r9; `pad` `%2`→r10/fp; no NOHINT/HINTVREG knob reproduces
`x=r5, pad=r9, pad2=sl`. `lh_inline` no-op. `__builtin_expect` anchor is
size-critical (plain form grows body 1948→1952).

## 4. Conclusion

No measurable win this wave; the base is a local optimum for size-exact
source. A future pass needs allocator-level evidence (e.g. a shape that makes
`x` live acrpss the whole clamp in r5 without adding instructions), not more
local-declaration motion.
