# `CheatHandler` near-miss deferrals

## Targets

- TU: `src/game/core/CheatHandler.cpp`
- `_ZN11AbyssEngine12CheatHandler12AddCheatCodeERKNS_6StringEi`
  - original `0x74538`, current `0x8d0d8`
  - 98.8%, 214/214 bytes
- `_ZN11AbyssEngine12CheatHandler6UpdateEt`
  - original `0x7465c`, current `0x8d1fc`
  - 96.3%, 66/66 bytes

## `AddCheatCode` residual

The body is structurally identical except for the constructor ABI alias after
the 12-byte allocation:

```text
original 0x7454e  blx CheatCode::CheatCode() C2
current            blx CheatCode::CheatCode() C1
```

The original C1 and C2 aliases both resolve to `0x74378`; the reconstructed
aliases likewise share one implementation. Ordinary `new CheatCode()` must
construct a complete object and therefore selects C1 under the Itanium ABI.
C2 is selected for a base subobject. Direct constructor calls, alias/linker
tricks, or a synthetic derived wrapper are either banned or change the
construction being represented.

This is related to the authoritative TU mapping: the original
`CheatCode.cpp` owns both CheatCode and CheatHandler, while the reconstruction
is split. Requeue only as part of the exclusive CheatHandler/CheatCode TU
membership merge, never as a direct constructor-call workaround.

## `Update` residual

Only the `key` register copy moves across the early exit:

```text
original 0x74668  mov r4, r1
original 0x7466a  cbz r0, ...

current  0x8d208  cbz r0, ...
current  0x8d20a  mov r4, r1
```

Everything after that copy is structurally identical. `r4` is unused on the
null-function edge, so LLVM may sink the copy into the loop successor.
Locals, early-return spelling, and cached function/key variables reduce to
the same SSA liveness. Forcing a pre-branch copy needs volatile or artificial
observable work and is not justified by the binary.

Requeue only with a normal-C++ formulation that makes the value genuinely
live on the null edge without adding behavior or instructions.
