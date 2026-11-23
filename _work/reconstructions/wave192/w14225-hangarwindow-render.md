# HangarWindow::render — wave192 w14225

Target: `_ZN12HangarWindow6renderEv`, size 9700, baseline raw 1414 / aligned 395.

## Tested this pass
- `const int textY`: **1414 / 395 / 9700** — byte-identical to baseline; clang
  ignores the qualifier here. Not a win.

## Residual evidence (free-credits loop, ~0x14ad00)
Original:
- `textY` is converted once (`vmov r6, s0` after `vcvt.s32.f32`) and kept in
  callee-saved **r6** across both `DrawString` calls; second call is
  `add r0, r6` + `layout->field_0x4`.
- `this` is loaded from `[sp,#0x80]`; no extra `str` before the `beq` selecting
  the `i==0` empty-string branch.
Ours:
- `textY` stays as float `s18`, is re-converted with `vcvt.s32.f32` at each use
  and spilled to `[sp,#0xc0]`; extra `str r0,[sp,#0x330]` precedes the `beq`.
- `this` is loaded from `[sp,#0x84]` in this loop; the buy-list loop and the
  free-credits loop have swapped `this` stack slots (buy-list orig `[sp,#0x80]`,
  ours `[sp,#0x84]`). Fixing one loop's slot may flip the other.

## Dead ends already measured (from prior waves + this one)
`upgTab` hoist (extab 562->1196), `borderX` accumulation (9704), x-arg reorder
(9704), branch-local `Layout*` (1887/632), `volatile Layout*` (9708),
`setPosition`/`setVisible` swap (9708), buy-list price-first (9692/472),
`C,W,R,H`/`rh` permutations, all 24 free-credit decl permutations,
`textYv` variants (9696/9712/9704/9736), `PaintCanvas *volatile canvas`
(required; plain 9624), nested ship-guard variants (~738), separate upgrade
`if` (736), `const int textY` (no change).

Best remains baseline 1414 / 395 / 9700; tree left clean.
