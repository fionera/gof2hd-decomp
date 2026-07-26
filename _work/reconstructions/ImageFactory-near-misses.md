# `ImageFactory` near-miss deferrals

## Targets

- TU: `src/engine/render/ImageFactory.cpp`
- `_ZN12ImageFactory10createCharEbi`
  - original `0x11c6f4`
  - 98.1%, 128/128 bytes
- `_ZN12ImageFactory8loadCharEPi`
  - original `0x11c774`
  - 97.6%, 112/112 bytes

## `createChar(bool, int)` residual

The executable instructions are structurally identical. The apparent final
differences are raw PIC literal-pool words that the verifier decodes as
garbage halfword instructions:

- original pool `0x11c768` targets the `Globals::rnd` GOT entry `0x2101f8`
- original pool `0x11c76c` targets the 176-byte part-count table at `0x201c24`
- original pool `0x11c770` targets `Globals::rnd` again

The reconstructed loads resolve to its own `Globals::rnd` GOT entry and its
own relocated copy of the same table. The two 176-byte tables are byte
identical (11 rows of four integers). This is not a wrong constant, global,
or table identity.

The verifier recognizes a PIC delta only when the matching `add reg, pc` is
within its short lookahead. Here the table literal load and its PC add are
farther apart, so the relocated address word is scored as a constant. Fixing
that number locally would require repo-wide rodata/link layout manipulation
or a new global, neither of which is a valid function reconstruction.

## `loadChar(int *)` residual

Only one copy is sunk across the null guard:

```text
original 0x11c77c  mov r5, r1
original 0x11c77e  cbz r1, ...
original 0x11c780  movs r0, #12

current             cbz r1, ...
current             movs r0, #12
current             mov r5, r1
```

The remainder and the 112-byte size are structurally identical. `r5` is
unused on the null edge, so LLVM MachineSink places the copy in the non-null
successor. A local alias canonicalizes to the same SSA; volatile would add
stack traffic and fake source semantics.

Requeue `createChar(bool,int)` only if verifier PIC normalization expands to
cover the delayed add. Requeue `loadChar` only with a normal source shape
that changes real liveness without compiler-control tricks.
