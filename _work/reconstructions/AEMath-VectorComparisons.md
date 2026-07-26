# AEMath Vector comparisons — deferred scheduler family

Symbols:

- `_ZN11AbyssEngine6AEMatheqERKNS0_6VectorES3_`
- `_ZN11AbyssEngine6AEMathltERKNS0_6VectorES3_`
- `_ZN11AbyssEngine6AEMathgtERKNS0_6VectorES3_`
- `_ZN11AbyssEngine6AEMathleERKNS0_6VectorES3_`
- `_ZN11AbyssEngine6AEMathgeERKNS0_6VectorES3_`

Each function is 90.9%, original 68 bytes, ours 68 bytes, and not linked.
Original addresses are `0x73c3a`, `0x73cbe`, `0x73d02`, `0x73d46`, and
`0x73d8a`, respectively.

## Shared stuck diff

The predicate changes with the operator, but the structural residual is
identical:

```text
orig                                      ours
vldr    s0, [r1]                          vldr    s0, [r1]
mov     r2, r0                          | vldr    s2, [r0]
vldr    s2, [r0]                        | vcmp/vcmpe ...
vcmp/vcmpe ...                          | vmrs ...
vmrs ...                                | itt <failure>
itt     <failure>                       | mov<failure> r0, #0
mov<failure> r0, #0                    | bx<failure> lr
bx<failure> lr                         | vldr    s0, [r1, #4]
vldr    s0, [r1, #4]                   | mov     r2, r0
movs    r0, #0                         | vldr    s2, [r0, #4]
vldr    s2, [r2, #4]                   | movs    r0, #0
```

The y and z comparisons, early returns, predicate codes, sizes, and return
materialization are otherwise structurally identical. The source already
preserves component evaluation order and IEEE unordered/NaN behavior.

## Source shapes tested

The pinned NDK r18b clang `-Oz` build generated the same machine code for every
function under all tested forms:

1. Baseline direct conjunction: `x op && y op && z op`.
2. Explicit failing x guard followed by the y/z conjunction.
3. Nested `bool result` flow that initializes false, evaluates y only after x
   succeeds, and evaluates z only after y succeeds.

The final Sol retry restored the baseline bodies after finding no binary
evidence for a different semantic organization.

## Hypothesis / requeue condition

The original allocator eagerly preserves the lhs pointer in `r2`; the pinned
compiler defers that copy until the first comparison succeeds. This is a
backend register-allocation/scheduling choice, not a recovered logic
difference. Do not retry with unused aliases, dummy reads, `volatile`,
artificial escapes, cached fields solely for liveness, opaque helpers, or
pointer-arithmetic tricks. Requeue only if original-source evidence reveals a
genuine distinct construct or another authentic `AEMath.cpp` change alters the
TU's register allocation.
