# `AEMath::MatrixGetInverse` constructor-ABI deferral

## Target

- TU: `src/engine/math/AEMath.cpp`
- `_ZN11AbyssEngine6AEMath16MatrixGetInverseERKNS0_6MatrixE`
- original `0x80e78`, current `0x96f8a`
- 97.8%, 158/158 bytes

## Residual

Every instruction is structurally identical except the default constructor
called for `Matrix result;` at `AEMath.cpp:528`:

```text
original 0x80e82  blx Matrix::Matrix() C2 @plt 0x5f118
current            blx Matrix::Matrix() C1 @plt 0x64804
```

The original PLT entry resolves to a weak, C2-only constructor at `0x6e8d8`
(64 bytes). The reconstruction resolves to the strong C1/C2 alias at
`0x87c30` (40 bytes). Ordinary local declarations, value initialization, and
temporary spellings all construct a complete object and legally select C1;
requesting C2 directly would be an ABI-symbol trick.

This is also a semantic constructor mismatch. The original default constructor
sets ones at matrix elements 0, 5, 10, 13, and 14 and zeros elsewhere. Its
literal pool at `0x6e908` supplies `{1.0f, 0.0f, 0.0f, 1.0f}` for the store at
offset `+0x28`. The current constructor sets ones at elements 0, 5, 12, and
14. `MatrixGetInverse` leaves elements 12 through 14 untouched, so the
returned tails are respectively `{0, 1, 1}` and `{1, 0, 1}`.

## Requeue condition

Do not retry this as an `AEMath.cpp` source-shape pass. Drain the fleet and
perform one atomic Matrix ownership pass:

1. restore the original default-constructor values;
2. test the evidence that the constructor was header-inline/weak and remove
   the strong out-of-line definition if confirmed;
3. gate every default-Matrix call site and C1/C2 dynamic-symbol parity.

The blast radius includes every default Matrix construction. Confidence is
high for the callee identities and values, and medium-high for inline-header
provenance.
