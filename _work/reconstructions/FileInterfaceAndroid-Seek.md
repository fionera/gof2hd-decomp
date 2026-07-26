# `FileInterfaceAndroid::Seek` scheduling checkpoint

Target: `_ZN20FileInterfaceAndroid4SeekEj`

- Original: `0x6e590`, 80 bytes.
- Current: 80 bytes, 93.9%.
- Current TU: `src/engine/file/FileInterfaceAndroid.cpp`.

The control flow, calls, field offsets, return normalization, and all
instructions otherwise match. Two callee-saved preservation copies are
scheduled after null guards in the current build:

```text
original  mov r4, r1; cbz r1; ldr r5, [r0, #12]
current   cbz r1; ldr r5, [r0, #12]; mov r4, r1

original  malloc call; mov r6, r0; cbz r0
current   malloc call; cbz r0; mov r6, r0
```

The verified member offsets are `file` at `#8` and `zipFile` at `#12`.
Positive nested guards for both `n` and the allocation result were compiled
and gated; they produced the same 80-byte output and the same two residuals.
Two fresh retries found the historical/simple early-return form already at
HEAD to be the most authentic C++. Descriptive aliases collapse to the same
SSA values.

Both preserved values are dead on their zero/null edges, allowing LLVM
MachineSink to place the copies in the live successors. Requeue only with
new authentic source evidence that creates real edge liveness. Do not use
dummy false-edge uses, volatile aliases, asm, attributes, or other
compiler-forcing constructs.
