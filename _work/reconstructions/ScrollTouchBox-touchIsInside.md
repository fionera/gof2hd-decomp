# ScrollTouchBox::touchIsInside — deferred lower-bound IT scheduling

Symbol: `_ZN14ScrollTouchBox13touchIsInsideEii`

Current metric: 82.6%, original 50 bytes, ours 50 bytes, not linked. Original
address: `0x135a22`. Upper-bound offsets, inclusive lower/exclusive upper
semantics, return values, and function size match.

## Stuck prologue

```text
original                                ours
ldr.w   lr, [r0, #4]                    ldr.w   lr, [r0, #4]
mov     r3, r0                        | cmp     lr, r1
cmp     lr, r1                        | bgt     reject
itt     le                            | ldr.w   ip, [r0, #8]
ldrle.w ip, [r3, #8]                 | mov     r3, r0
cmple   ip, r2                        | cmp     ip, r2
ble     upper                           ble     upper
```

## Source shapes gated

1. Existing synthetic `left`/conditional `top` locals and rejection.
2. Direct lower-bound rejection with `||`.
3. Positive lower-bound `&&` guarding the upper checks.
4. Explicit nested x-lower, y-lower, and upper checks with direct return.

All four compile to identical branchy code with the `this` save after the y
load. Requeue only with recovered type/lifetime provenance that explains the
original conditional load/compare IT block; do not use volatile, asm, or
compiler barriers.
