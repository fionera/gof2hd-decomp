# `AEGeometry::setLodChildMeshes` scheduling checkpoint

Target: `_ZN10AEGeometry17setLodChildMeshesEPt`

- Original: `0xa4a98`, 164 bytes.
- Current: 164 bytes, 96.8%.
- Original TU: `Classes/Game/AEGeometry.cpp`.
- Current TU: `src/engine/render/AEGeometry.cpp`.

All allocation, loop, field, and PaintCanvas call instructions are exact or
link-only. The only structural difference is argument/self scheduling around
the empty-count guard:

```text
original  ldr r9,[r0,#0x50]; mov r8,r1; mov r5,r0; cmp r9,#1; blt end
current   ldr r9,[r0,#0x50]; cmp r9,#1; blt end; mov r5,r0; ...; mov r8,r1
```

Honest `lodMeshes`/`geometry` pointer aliases, both declaration orders, and
reference aliases all optimize to the current 164-byte body: LLVM sinks the
moves because neither value is needed on the zero-count edge. Keeping them
live on that edge has no source-semantic justification and would require an
artificial use, so all alias edits were reverted.

Requeue only with a natural source form that genuinely consumes `this` and
`meshes` on the false edge. Do not use volatile, dummy reads, asm, attributes,
or fake helper calls.
