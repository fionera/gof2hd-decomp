# `Sprite::setFrame` backend-commutation checkpoint

Target: `_ZN6Sprite8setFrameEi`

- Original: `0xd0d88`, 72 bytes.
- Baseline: 72 bytes, 92.6%.
- Original TU: `Classes/Game/Sprite.cpp`.
- Current TU: `src/engine/render/Sprite.cpp`.

At baseline, the body was instruction-identical except for the commutative
operand encodings of two multiplies:

```text
original  mul.w r0, r9, r0    baseline  mul.w r0, r0, r9
original  mul.w r1, r8, r1    baseline  mul.w r1, r1, r8
```

Controller-built attempts established:

1. Reversing the multiplication operands in the source did not change the
   generated multiply encodings.
2. Moving the `frameWidth` and `frameHeight` declarations after the
   quotient/remainder computation made the first multiply exact:
   `mul.w r0, r9, r0`.
3. That declaration-lifetime improvement left one residual:
   original `mul.w r1, r8, r1` versus current `mul.w r1, r1, r8`.
4. Expressing the remaining computation with compound assignment did not
   change that encoding.
5. Computing `frameX` before `frameY` also did not eliminate the residual.

The current best natural source form is:

```cpp
int columns = this->columns;
int row = frame / columns;
int column = frame - row * columns;

int frameWidth = this->frameWidth;
int frameHeight = this->frameHeight;
int frameX = frameWidth * column;
int frameY = frameHeight * row;

this->frameSrcX = frameX;
this->frameSrcY = frameY;
```

The remaining instruction has identical integer semantics, registers,
size, and control flow; only the commutative source-register fields in the
Thumb multiply encoding are exchanged. Classify it as backend commutation
unless new authentic source evidence appears. No forbidden steering was
used, and future work must not use volatile, dummy dependencies or reads,
inline assembly, compiler attributes, builtins, or artificial helper calls
to force the encoding.
