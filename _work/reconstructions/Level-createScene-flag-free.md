# Level::createScene — the `-mllvm -sink-insts-to-avoid-spills` compensation

Status: **open**. Branch `wave87-w3253` (commit `21cc3026`) removes the flag but scores
86.7% instead of 93.1%; master keeps the flag. The goal is a source shape that reaches
100% with **no** scoped `-mllvm` flag.

## What the flag actually does (pre-RA MIR diff, `-mllvm -print-machineinstrs`)

The only MIR difference in the whole function is one instruction:

```
%540 = t2ADDri %stack.1, 0        ; frame address of the parts-loop Vector{0,pi,0} temp
```

`MachineLICM` hoists it into the loop preheader; `-sink-insts-to-avoid-spills` moves it back
into the loop body. That single move flips the greedy allocator:

| | parts loop | ship loop |
|---|---|---|
| with flag | `r9` = `kHangarMeshes[sceneRace]` row, `r8` = `&Globals::Canvas` | `&Globals::rnd` in `r8` |
| without flag | `r8` = mesh row, `r9` = `&Canvas` | `&Globals::rnd` spilled to `sp#44` |

The original matches the "with flag" column. Without the flag the function is also 2244 B
instead of 2252 B, which shifts the following `b.w <plt>` thunks and costs ~38 `byte_exact`
(the address-sensitive metric), so the flag-free variant must keep the 2252 B size.

## What was tried (≈150 builds, all measured, none flipped the allocation)

- 16 combinations of `nextInt` sites reading a local `AbyssEngine::AERandom *rnd` vs
  `Globals::rnd` directly. Best flag-free shape (86.7%, 2252 B): the two race-selection
  calls read `Globals::rnd` directly (the original reloads `ldr.w r0,[r8]` at each site);
  retry/rotation sites keep the local.
- `Vector` temporary moved before/after `new`, named / `const` / parenthesised forms.
- Hoisting `Globals::Canvas`, `this->enemies`, `kBarChildCounts[sceneRace]`,
  `kHangarMeshes[sceneRace]` into loop locals (all 16 subsets).
- `type`/`capacity`/`shipCount`/`race`/`spot`/`i`/`tries`/`pick` as `short`/`unsigned`.
- Loop forms (`!=`, `<=`, `unsigned`), `spotUsed` as `bool*`, `enemies` hoist, declaration
  placement.

## Next angles for a fresh worker

1. The parts-loop `Vector{0, pi, 0}` temporary is the *only* LICM-relevant value. If the
   original did not materialise a stack temporary there at all (e.g. it calls a helper that
   takes the rotation by value, or reuses a `Vector` local declared outside the loop), the
   `t2ADDri` never exists and the allocator sees the flag-free MIR. Reconstruct the parts
   loop from Thumb and count stack slots before assuming the temporary shape.
2. The ship-loop `&Globals::rnd` in `r8` is a separate 6.9%: the original keeps a live
   pointer across the loop, which means the loop body must not reload `Globals::rnd` at any
   `nextInt` site.
3. Keep the function at exactly 2252 B so the downstream PLT thunk encodings do not move.
