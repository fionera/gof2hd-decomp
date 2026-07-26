# `TouchSlider::OnTouchMove` canonicalization deferral

## Target

- TU: `src/game/ui/TouchSlider.cpp`
- `_ZN11TouchSlider11OnTouchMoveEii`
- original `0xa2494`, current `0xaf134`
- 96.4%, 72/72 bytes

## Residual

The verified signed-int fields are `x @ +0x00`, `knobX @ +0x08`,
`knobWidth @ +0x14`, and `trackWidth @ +0x1c`; `isDragging` and `isDisabled`
are bytes at `+0x34/+0x35`. The function clamps the touch x coordinate to:

```text
[x + knobWidth / 2, x + trackWidth - knobWidth / 2]
```

Its only executable difference is the algebraic tree for the upper bound:

```text
original  add trackEnd, x, trackWidth
          sub maxX, trackEnd, halfKnob

current   sub maxX, x, halfKnob
          add maxX, trackWidth
```

The current source already spells the original natural expression,
`(x + trackWidth) - halfKnob`, and Ghidra shows the same expression. Under
C++ signed-overflow rules LLVM may reassociate these equivalent trees.
Parentheses, named temporaries, compound assignments, reusing the half-width
local, and alternate clamp spelling all reduce to the same SSA. Unsigned or
wider arithmetic changes recovered semantics or instruction count; escaping
the value, volatile access, barriers, and helper calls are unjustified.

Everything else is structurally identical. Requeue only if original source
provenance establishes a real type, macro, or object-lifetime distinction that
changes the IR semantics. Confidence is high on the layout and residual and
medium-high that ordinary local spellings are exhausted.
