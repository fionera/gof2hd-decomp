# Status::getFreighterMissionStationBit(int) — deferred compare ordering

Symbol: `_ZN6Status29getFreighterMissionStationBitEi`

Current metric: 89.2%, original 74 bytes, ours 74 bytes, not linked. Original
address: `0xacd74`.

## Target and baseline

The original is a linear Thumb comparison/early-return chain:

```text
15 -> 4
95 -> 6
40 -> 3
45 -> 9
60 -> 5
70 -> 7
80 -> 8
85 -> 1
30 -> 2, otherwise 0
```

Its final test is `cmp r1,#30; ite ne; movne r0,#0; moveq r0,#2`.
The current independent-if source remains 74 bytes but clang canonicalizes the
mapping to comparisons `15,30,40,45,60,70,80,85,95`.

## Source shapes gated

1. Early checks for 15 and 95, a sparse switch for 40 through 85, then a
   default ternary for 30.
2. An ordered `else if` assignment chain feeding one result/return.
3. One ordered, right-associated conditional expression.

All three compiled to the same regressed 78-byte form: comparisons
`15,40,45,60,70,80,85,95`, followed by a four-instruction
`sub/clz/lsrs/lsls` equality result for 30. Thus source order, switch/CFG
partitioning, a PHI result, and a select tree are all optimized into the same
whole-function integer mapping.

## Requeue condition

Do not force ordering with `volatile`, opaque helpers, compiler controls, or
aliasing tricks. Requeue only after recovering an authentic source-level
control/data dependency from Ghidra or a related `Status` representation that
makes the 95 and 30 paths observably distinct before optimization.
