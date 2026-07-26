# Level::collideStation(Vector) — deferred argument-save scheduling

Symbol: `_ZN5Level14collideStationEN11AbyssEngine6AEMath6VectorE`

Current metric: 87.9%, original 84 bytes, ours 84 bytes, not linked. Original
address: `0xc4594`. Field offsets, null guards, status call, success reload,
virtual collide dispatch, and function size match.

## Stuck diff

The original preserves all three by-value `Vector` words immediately after
loading `Level::landmarks` and before the first null guard:

```text
mov r4, r0
ldr.w r0, [r0, #256]
mov r8, r3
mov r5, r2
mov r6, r1
cbz r0, ...
```

Ours performs both landmark null checks first and emits the same three moves
only before the `Status::inEmptyOrbit()` call.

## Source shapes gated

1. Snapshot `this->landmarks` and reuse it for guards and success. This
   regressed to 80 bytes: `r4` held the list and the original success reload
   disappeared.
2. Use the landmark snapshot only for the guards, but reload
   `this->landmarks` on success. This restored the 84-byte structure and
   `this` in `r4`, but the Vector saves remained after both guards.
3. Add a typed `Vector collision = v` before the landmark snapshot and use it
   at the final call. LLVM scalarized and sank the copy; codegen was identical
   to shape 2.

## Requeue condition

Do not use volatile, opaque calls, alias tricks, or compiler barriers. Requeue
only after Ghidra or historical source evidence identifies a genuine pre-guard
use/control dependency that makes all three Vector components live before the
first `cbz`.
