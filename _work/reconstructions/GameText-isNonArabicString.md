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
2. `bool keep`: produces the original `lsls` truth test, but swaps `ch` and
   accumulator across `r4/r5` throughout the function.
3. `int keep` with natural low-bit condition `keep & 1`: also feeds back into
   allocation and produces the same `r4/r5` swap as `bool`.

Requeue only with recovered type/provenance evidence that simultaneously
requires integer-like allocation and one-bit condition semantics. Do not spell
an explicit shift or use volatile solely to force the final instruction.
