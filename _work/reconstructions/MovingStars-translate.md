# MovingStars::translate

Symbol: `_ZN11MovingStars9translateERKN11AbyssEngine6AEMath6VectorE`

Status: deferred after the initial patch, two retries, and an independent
escalation. The remaining mismatch is LLVM stack-slot coloring.

## Verified residual

- Original and current are both 144 bytes and reserve an 84-byte frame.
- All calls, branches, constants, global loads, loop behavior, and matrix/vector
  operations are semantically and structurally identical.
- Original assigns the 60-byte `MatrixSetTranslation` return slot to `sp+8`
  (`r8`) and the 12-byte `Vector pos` slot to `sp+68` (`fp`).
- Current assigns `Vector pos` to `sp+8` and the Matrix return slot to `sp+20`.
  The differing component loads and address materializations follow solely
  from that slot assignment.

## Attempts

1. Removed the named Matrix and discarded the return directly. Codegen was
   unchanged.
2. Used an immediately invoked lambda with the Matrix destination outside and
   `Vector pos` inside the initializer. Codegen was unchanged.
3. Re-derived the natural typed source and checked declaration/lifetime
   alternatives. The Matrix value depends on the fully formed and mutated
   Vector, so beginning its lifetime first requires extra construction and
   assignment behavior.

`Matrix` has a real nontrivial default constructor and assignment function.
Predeclaring it adds calls/stores. Raw lifetime manipulation, unions, volatile,
assembly, attributes, or undefined behavior are prohibited. Requeue only with
authentic source/lifetime evidence or a legitimate compiler-allocation change.
