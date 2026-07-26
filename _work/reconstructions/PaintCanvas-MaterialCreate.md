# PaintCanvas::MaterialCreate — deferred constructor-provenance mismatch

Symbol: `_ZN11AbyssEngine11PaintCanvas14MaterialCreateERjNS_9BlendModeEjt`

Current metric: 96.2%, original 74 bytes, ours 74 bytes, not linked. The
allocation size (`0x74`), argument/register saves, `textures[0]` and
`blendMode` stores, `ArrayAdd`, output index, and exception cleanup align.
The sole structural mismatch is the `Material` constructor ABI variant.

## Verified callee mismatch

```text
original 0x7c154: blx 0x5fc34 <_ZN11AbyssEngine8MaterialC2Ev@plt>
ours     0x930e0: blx 0x663f0 <_ZN11AbyssEngine8MaterialC1Ev@plt>
```

The original dynamic symbol is a weak, 152-byte C2 constructor at `0x7c189`;
there is no exported default `Material` C1 symbol. Ours emits C1 and C2 as
strong aliases at the same 144-byte address.

## Ownership evidence

`_work/original_layout/tu_mapping.md` records that the `Material` class
constructor lived in original `PaintCanvas.cpp`. The current out-of-line
definition is in `src/engine/render/Material.cpp`, whose original TU contained
`MaterialDraw` and Array helpers instead. The original weak C2-only emission is
consistent with class-body/inline constructor provenance in the
PaintCanvas-owned source shape; ours is the strong complete/base pair produced
by the out-of-line definition.

## Requeue condition

This is not a legal function-body fix. Calling C2 explicitly is a banned direct
constructor/operator-style ABI trick, and introducing a derived wrapper would
invent an unwitnessed type and make deletion through non-virtual `Material *`
undefined.

Requeue only as an exclusive constructor/TU-ownership pass. Restore the
binary-witnessed `Material` constructor provenance atomically, then gate every
`Material` construction site, dynamic-symbol parity, and the full ratchet.
