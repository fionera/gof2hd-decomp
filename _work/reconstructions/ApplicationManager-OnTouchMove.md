# `ApplicationManager::OnTouchMove` scheduling checkpoint

Target: `_ZN11AbyssEngine18ApplicationManager11OnTouchMoveEiiPv`

- Original: `0x83bb0`, 104 bytes, 44 decoded instructions.
- Landed: 104 bytes, 44 decoded instructions, 97.7%.
- Previous: 104 bytes, 43 decoded instructions, 94.3%.
- Original TU: `Svn_iPhone_AE2/Classes/ApplicationManager.cpp`.
- Current TU: `src/engine/core/ApplicationManager.cpp`.

Using direct formal parameters `int x, int y, void *touch` instead of copying
`xArg/yArg` into locals restores the original independent x/y stack spills
and the missing decoded instruction. The coordinate conversion, both virtual
touch handlers, final `lastTouchX/lastTouchY` store, frame size, and canary
path otherwise match.

The only structural residual is preservation of the opaque touch handle:

```text
original  str x,[sp+8]; mov r8,r3; ... canary setup ... module/state guards
landed    str x,[sp+8]; ... canary setup ... module/state guards;
          mov r8,r3 immediately before ConvertTouchCoords
```

Ghidra confirms `touch` is semantically consumed only by the
`onTouchMove3` virtual dispatch. `OnTouchBegin` and `OnTouchEnd` also retain
their touch parameter early in the original binary, but a local alias used
only by that one dispatch collapses to the same SSA value and LLVM
MachineSink may legally move its preservation copy into the taken block.

Requeue only with new natural source evidence for a second pre-guard use.
Do not add a false-edge dummy use, volatile alias, asm, attribute, or
compiler-forcing construct.
