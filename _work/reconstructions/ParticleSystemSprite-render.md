# ParticleSystemSprite::render(4-arg) — partial landed, eager-save residual

Symbol:
`_ZN20ParticleSystemSprite6renderEPN11AbyssEngine11PaintCanvasEjjNS0_9BlendModeE`

Wave 64 landed the `float m[15]` local after the setup calls, improving the
function from 95.0% to 96.7% at 156/156 bytes. The matrix initialization now
matches, including loading the non-zero constant vector before materializing
the stack destination.

## Remaining diff

The original preserves `blend` and `canvas` before stack-canary setup and the
`handle == 0xffffffff` early return:

```text
orig                                      ours
mov     r5, r0                            mov     r5, r0
ldr     r0, [pc, #136]                    ldr     r0, [pc, #136]
mov     r8, r3                          | add     r0, pc
mov     r4, r1                          | ldr     r6, [r0]
... canary setup ...                     ... canary setup ...
adds    r0, r1, #1                       adds    r0, r1, #1
beq     return                           beq     return
                                          mov     r4, r1
...
                                          mov     r8, r3
```

All code from the setup calls through matrix stores, render calls, canary
check, and epilogue is otherwise structurally identical.

## Source shapes tested

1. Early aliases for canvas, handle, and blend plus a post-call `float m[15]`.
   The aliases were optimized away; the matrix change was the landed win.
2. `float m[15]` before the sentinel guard. This failed to move the arguments
   and regressed matrix initialization order, so it was rejected.
3. Final landed form: ordinary parameters and guard/calls, then
   `float m[15]`.

## Hypothesis / requeue condition

Canvas and blend have no semantic use on the sentinel-return path, so the
pinned compiler sinks their callee-saved copies. Natural aliases do not change
SSA. Do not retry with dummy uses, `volatile`, artificial escapes or
dependencies, helper calls, or compiler forcing. Requeue only with original
source evidence for genuine pre-guard work or after another authentic change
to `ParticleSystemSprite.cpp` changes allocation.
