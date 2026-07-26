# `Ship` near-miss MachineSink deferrals

## Targets

- TU: `src/game/ship/Ship.cpp`
- `hasEquipment(int,int)`: 96.9%, 76/76 bytes
- `hasCargo(int,int)`: 96.9%, 76/76 bytes
- `getCargo(int)`: 96.2%, 62/62 bytes
- `removeEquipment(Item *)`: 96.2%, 62/62 bytes
- `hasCargoType(int)`: 96.0%, 58/58 bytes
- `getFirstEquipmentOfSort(int)`: 97.6%, 96/96 bytes

## Common residual

Each function tests a nullable Array pointer (`equipment` at `+0x6c` or
`cargo` at `+0x70`) before entering a loop. The original copies the receiver
or saved arguments into callee-saved registers before the null `cbz`; the
current compiler sinks those copies into the non-null successor.

The null edges immediately return false, `nullptr`, or void. The receiver and
saved arguments are used only by the non-null loop. In
`getFirstEquipmentOfSort`, the receiver is also needed by the special
sort/index fallback, but that fallback remains on the non-null edge.

Local aliases, cached Array pointers, positive conditions versus early
returns, alternate null comparisons, member-derived known-null returns,
cast-to-void, and deleting a known-null pointer all preserve the same SSA
liveness or optimize away. A genuine null-edge use requires changed observable
behavior, undefined access, volatile state, a barrier, or an opaque fake use.

All remaining executable instructions are structurally identical or
link-only. Requeue only if recovered source/type evidence gives one of these
values a real null-edge use. Confidence is high.
