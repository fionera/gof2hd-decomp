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

current  +0x7e  movs r0, #0
current  +0x80  str  r0, [r4, #56]  ; scrollOffset
current  +0x82  str  r0, [r4, #76]  ; scrollVelocityF
```

## Attempts

1. Reversing the two source statements regressed the body from 202 to 208
   bytes and introduced a floating literal load; reverted.
2. Making only the first float lvalue volatile preserved 202 bytes but LLVM
   still moved the neighboring nonvolatile store across it; reverted.
3. Expressing the relationship as
   `scrollOffset = static_cast<int>(scrollVelocityF = 0.0f)` preserved 202
   bytes but still emitted the baseline reversed store order; reverted.
4. The final escalation tested
   `scrollVelocityF = static_cast<float>(scrollOffset = 0)` with the pinned
   NDK r18b Clang 7 `-Oz`. It regressed the body to 208 bytes by loading a
   floating zero literal and using a separate VFP store; reverted.

No attempted edit was landed.

## Blocker and requeue condition

The stores are semantically independent under the as-if rule. An escalation
compiler trace showed that optimized LLVM IR still contains the source's float
store before its integer store, but the first post-SelectionDAG machine dump
has already reordered them to `str #56; str #76`; the later machine scheduler
preserves that order. A diagnostic-only
`-mllvm -pre-RA-sched=source` build emitted the original local order, proving
that backend scheduling policy is the lever, but that forbidden global option
also perturbs unrelated code. A generic barrier, atomic/volatile accesses,
type-punning, or another opaque codegen device would be a match-only trick
without evidence from the binary's source types.

Requeue only with additional source/type evidence that gives the stores a real
ordering relationship while retaining one integer-zero materialization and the
202-byte control-flow shape.
