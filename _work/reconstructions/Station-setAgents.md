# `Station::setAgents` scheduling checkpoint

Target: `_ZN7Station9setAgentsEP5ArrayIP5AgentE`

- Original: `0xa6dce`, 36 bytes.
- Current: 36 bytes, 93.3%.
- Original TU: `Classes/Game/Station.cpp`.
- Current TU: `src/game/world/Station.cpp`.

The only structural residual is the incoming-pointer move:

```text
original  mov r4,r1; cmp old,r1; beq end
current   cmp old,r1; beq end; mov r4,r1
```

The release, delete, member reload, final store, and epilogue are otherwise
exact or link-only.

Two legal attempts were exhausted:

1. A `newAgents` alias inside an inequality guard compiled identically to
   the current 36-byte body; LLVM sank the move after the equal return.
2. `oldAgents`/`newAgents` aliases with a common final assignment moved
   `r4` before the comparison, but restructured the branch and grew the
   function to 42 bytes. It was reverted.

The source is restored to the simple early-return implementation. Requeue
only with new natural control-flow evidence. Do not add a false-edge dummy
use or redundant volatile assignment to defeat MachineSink.
