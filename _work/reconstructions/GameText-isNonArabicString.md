# GameText::isNonArabicString — landed near-exact, deferred truth-test residual

Symbol: `_ZN8GameText17isNonArabicStringEPKtj`

Landed metric: 97.6%, original 92 bytes, ours 92 bytes, not linked. Original
address: `0x7f798`.

The landed source uses an `int` bitwise accumulator and the recovered row-loop
source order. It matches the original register allocation (`ch` in `r5`,
accumulator in `r4`), inner loop, table stride, and pointer-before-counter
assembly. One structural instruction remains:

```text
original: lsls r0, r4, #31
ours:     cmp  r4, #0
```

Both branch on the 0/1 accumulator immediately afterward.

## Source shapes gated

1. `int keep` with ordinary truth test: the landed 97.6% form above.
2. `bool keep` with the recovered combined `for` loop: produces the original
   `lsls` truth test, but swaps `ch` and accumulator across `r4/r5` throughout
   the function. Reversing the two comma-expression increments restores the
   original pointer-before-counter schedule without changing that register
   swap.
3. Moving the `bool` initialization after the character load does not affect
   allocation.
4. Widening `ch` from `unsigned short` to `unsigned int` preserves the same
   `ldrh` but does not affect allocation.
5. Restoring the landed `int`/split-loop shape and using the natural low-bit
   condition `keep & 1` also feeds back into allocation and produces the same
   `r4/r5` swap as `bool`.

A stale verification object initially appeared to prove the `bool`/combined
loop shape exact, but rebuilding that source with the current match toolchain
reproduced the swap above. Treat the stale artifact as having unknown compiler
inputs, not as source authority.

Requeue only with recovered type/provenance evidence that simultaneously
requires integer-like allocation and one-bit condition semantics. Do not spell
an explicit shift or use volatile solely to force the final instruction.
