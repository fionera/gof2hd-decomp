# `AEMath::MatrixGetInverse` constructor-ABI resolution

## Target

- TU: `src/engine/math/AEMath.cpp`
- `_ZN11AbyssEngine6AEMath16MatrixGetInverseERKNS0_6MatrixE`
- original `0x80e78`, current `0x96f8a`
- 100%, 158/158 bytes

## Former residual

Every instruction is structurally identical except the default constructor
called for `Matrix result;` at `AEMath.cpp:528`:

```text
original 0x80e82  blx Matrix::Matrix() C2 @plt 0x5f118
current            blx Matrix::Matrix() C1 @plt 0x64804
```

The original PLT entry resolves to a weak, C2-only constructor at `0x6e8d8`
(64 bytes). The reconstruction previously resolved to the strong C1/C2 alias
at `0x87c30` (40 bytes). Ordinary local declarations, value initialization,
and temporary spellings all construct a complete object and legally select C1;
requesting C2 directly would have been an ABI-symbol trick.

This is also a semantic constructor mismatch. The original default constructor
sets ones at matrix elements 0, 5, 10, 12, 13, and 14 and zeros elsewhere.
The previous constructor set ones only at elements 0, 5, 12, and 14.

## Resolution

The exclusive Matrix ownership pass restored the verified values and moved the
definition inline into `Matrix.h`. Clang now emits the original weak C2-only
constructor, `Matrix::Matrix()` and `MatrixGetInverse` both match byte-for-byte,
and the redundant C1 export is gone. The full ratchet passed with
`byte_exact 1182`, `linked_exact 2725`, and `avg 78.44%`.
