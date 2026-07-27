# `Station::setAgents` deferred scheduling blocker

## Target

- Symbol: `_ZN7Station9setAgentsEP5ArrayIP5AgentE`
- Original address: `0xa6dce`
- Original size: 36 bytes
- Current size: 36 bytes
- Current similarity: 93.3%
- Original TU: `Classes/Game/Station.cpp`
- Current TU: `src/game/world/Station.cpp`

The reconstructed semantics are correct. The member is the `Array<Agent *> *`
at `Station + 0x30`; the retain/release-related callees, delete path, control
flow, final member store, and epilogue are also correct. The single residual is
instruction scheduling around the equality test.

## Exact residual

The original preserves the incoming `newAgents` pointer in `r4` before
comparing it with the old member:

```text
mov r4, r1
cmp old, r1
beq end
```

The best natural reconstruction emits the same 36-byte function but sinks the
preservation move into the unequal successor:

```text
cmp old, r1
beq end
mov r4, r1
```

Thus the mismatch is not a semantic, size, offset, callee, or branch-target
error. It is only the placement of `mov r4,r1`.

## Exhausted legal source shapes

Two exploratory retries plus a final retry tested the natural spelling space:

- A nested inequality guard, with direct uses and with a `newAgents` alias,
  retained the 36-byte body and 93.3% similarity. It compiled as
  `cmp` / `beq` / `mov`, identical to the current residual.
- An early return on equality, with direct operands or aliases for
  `oldAgents` and `newAgents`, also kept `newAgents` live only on the unequal
  edge. The compiler again emitted the same 36-byte
  `cmp` / `beq` / `mov` schedule.
- Reversing comparison operands and swapping positive/negative guard forms did
  not produce the original schedule.
- Reference and pointer aliases did not alter SSA liveness and either compiled
  identically or optimized away.
- Moving the final member assignment to a common tail did keep incoming `r1`
  live and place its preservation move correctly before the comparison.
  However, it changed the branch formation to an IT-block shape containing
  `cmpne` and `bne`, growing the function from 36 to 42 bytes. That is a worse
  structural result and was reverted.

No authentic, legal C++ spelling found in nested guards, early returns,
aliases, operand reversal, or common assignment combines both required
properties: the original pre-comparison `mov r4,r1` and the original 36-byte
control-flow shape.

## Compiler hypothesis

This is most likely an LLVM MachineSink consequence rooted in SSA liveness.
In the natural implementation, the equal edge returns without needing the
incoming pointer, while the unequal edge needs it across calls. The compiler
can therefore sink the callee-saved-register copy into only the successor
where it is live. The original object evidently retained the earlier copy,
whether because of a subtly different source-level live range or a historical
backend scheduling decision.

The common-tail assignment experiment supports this hypothesis: making the
incoming pointer genuinely live after both comparison outcomes prevents the
same sinking, but also forces a different conditional-control-flow lowering.

## Deferral

Leave the current natural 36-byte implementation in place. Do not use
volatile or dummy uses, inline assembly, attributes, builtins, ODR tricks, or
other compiler-forcing constructs to manufacture false-edge liveness.

Requeue this target only when source or compiler evidence supplies an
authentic legal spelling or explains the original backend schedule. Confidence
is high that the remaining 6.7% mismatch is an irreducible scheduling
near-miss under the currently known source constraints.
