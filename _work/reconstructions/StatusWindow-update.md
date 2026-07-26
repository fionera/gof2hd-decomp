# `StatusWindow::update` deferred reconstruction

## Target

- Mangled: `_ZN12StatusWindow6updateEi`
- TU: `src/game/ui/StatusWindow.cpp` (`Classes/Game/StatusWindow.cpp`)
- Original address: `0x158250`
- Current gate: 98.5%, original/current size 202/202, not linked-exact

## Remaining diff

The complete function is structurally identical apart from two independent
zero stores in the bottom-bound reset branch:

```text
original 0x1582ce  movs r0, #0
original 0x1582d0  str  r0, [r4, #76]  ; scrollVelocityF
original 0x1582d2  str  r0, [r4, #56]  ; scrollOffset

current  0x125c9e  movs r0, #0
current  0x125ca0  str  r0, [r4, #56]  ; scrollOffset
current  0x125ca2  str  r0, [r4, #76]  ; scrollVelocityF
```

## Attempts

1. Reversing the two source statements regressed the body from 202 to 208
   bytes and introduced a floating literal load; reverted.
2. Making only the first float lvalue volatile preserved 202 bytes but LLVM
   still moved the neighboring nonvolatile store across it; reverted.
3. Expressing the relationship as
   `scrollOffset = static_cast<int>(scrollVelocityF = 0.0f)` preserved 202
   bytes but still emitted the baseline reversed store order; reverted.

No attempted edit was landed.

## Blocker and requeue condition

The stores are semantically independent under the as-if rule. The exhausted
normal retries did not establish a source-level dependency that survives
optimization. A generic barrier, two volatile accesses, or another opaque
codegen device would be a match-only trick without evidence that the original
fields were volatile.

Requeue only with additional source/type evidence that gives the stores a real
ordering relationship while retaining one integer-zero materialization and the
202-byte control-flow shape.
