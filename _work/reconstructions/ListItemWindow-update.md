# ListItemWindow::update

Symbol: `_ZN14ListItemWindow6updateEi`

Status: deferred after the initial patch, two retries, and an independent
escalation. The remaining mismatch is an equivalent commutative VFP operand
encoding selected by LLVM register allocation.

## Verified residual

- Original and current are both 352 bytes.
- Calls, branches, constants, field offsets, stack layout, register saves, and
  every instruction except one match structurally.
- Original emits `vadd.f32 s16, s0, s16`.
- Current emits `vadd.f32 s16, s16, s0`.
- At that point `s16` holds the long-lived `baseAngle`, `s0` holds the freshly
  loaded angle-table value, and the result remains live in `s16`. The two
  encodings are numerically equivalent.

## Landed reconstruction

- Store the drag-angle division directly in `previewAngle` and reload that
  member at its three later uses.
- Retain the Canvas global through a pointer-to-holder and reload it for each
  transform lookup.
- Compute the shared table angle before the second transform lookup.

Those changes restore the original frame, saved-register set, global reloads,
member reloads, and instruction order.

## Attempts

1. Wrote `table[idx] + baseAngle`.
2. Reversed the source operands to `baseAngle + table[idx]`.
3. Initialized a temporary from `table[idx]` and then used compound `+=`.

Clang/LLVM 7 at `-Oz` canonicalized all three forms to the same commutative
`fadd`. MachineSink and register allocation keep `baseAngle` in `s16`, load the
table value late into `s0`, and coalesce the result into the dying `s16` live
range. Forcing the alternate operand encoding would require a banned
compiler-specific mechanism or extra observable operations that would disturb
the otherwise exact stream. Requeue only with authentic compiler/source
evidence.
