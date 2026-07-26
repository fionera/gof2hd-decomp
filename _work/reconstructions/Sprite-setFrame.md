# `Sprite::setFrame` backend-only checkpoint

Target: `_ZN6Sprite8setFrameEi`

- Original: `0xd0d88`, 72 bytes.
- Current: 72 bytes, 92.6%.
- Original TU: `Classes/Game/Sprite.cpp`.
- Current TU: `src/engine/render/Sprite.cpp`.

The body is instruction-identical except for the commutative operand
encodings of two multiplies:

```text
original  mul.w r0, r9, r0    current  mul.w r0, r0, r9
original  mul.w r1, r8, r1    current  mul.w r1, r1, r8
```

The following legal source shapes were controller-built and produced the
same current encoding:

1. Ordinary `column`, `frameY`, and `frameX` locals.
2. Reversing the multiplication operands in those local expressions.
3. Direct assignments
   `frameSrcX = frameWidth * (frame - row * columns)` and
   `frameSrcY = frameHeight * row`.

The source was restored to the ordinary local form. The residual has
identical integer semantics, register values, size, and control flow.
Requeue only with new natural source-shape evidence. Do not use volatile,
dummy reads, inline assembly, attributes, or artificial helper calls to
steer instruction encoding.
