# `PaintCanvas` near-miss deferrals

## `MaterialCreate(unsigned int&, BlendMode, unsigned int, unsigned short)`

The 74-byte body is structurally identical except the constructor ABI alias.
Original `0x7c154` calls weak C2-only `Material::Material()` through PLT
`0x5fc34`; the reconstruction calls the strong C1/C2 alias through PLT
`0x668bc`. Allocation, field stores, Array insertion, and exception cleanup
match.

Ordinary `new Material()` legally selects C1. Requeue only as an atomic
Material header/source ownership pass that confirms and restores inline/weak
constructor provenance, then gates every default Material construction and
dynamic-symbol parity.

## MachineSink residuals

- `TransformRemoveMesh(unsigned int,unsigned short)`, 94.4%, 44/44 bytes:
  original saves the transform index before the count guard; current sinks it
  after the early return.
- `TransformGetTriCount(Transform *)`, 94.1%, 84/84 bytes: original saves
  `this` and the transform and then loads its mesh count; current performs the
  null guard first. The null edge returns zero.
- `MeshChangeShaderAnimValue(Transform *,float,unsigned int)`: the real Mesh
  overload is now restored and the body improved 94.1% to 97.1%, but the
  current compiler sinks all four saved arguments across the null guard.

The saved values in each case are used only on the non-null/in-range edge.
Aliases, cached locals, and condition reshaping retain equivalent SSA; a real
early-edge use would change behavior or require a compiler-control trick.
Requeue only with new type/source-liveness evidence.

Confidence is high for the constructor symbols, real Mesh overload, and
MachineSink classifications.
