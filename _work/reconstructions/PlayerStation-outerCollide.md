# `PlayerStation::outerCollide(float,float,float)` checkpoint

Target: `_ZN13PlayerStation12outerCollideEfff`

- Original: `0x122778`, 196 bytes.
- Landed: 196 bytes, 92.4%.
- Previous: 196 bytes, 90.9%.
- TU: `src/game/ship/PlayerStation.cpp`.

The landed pass corrects the bounding-volume dispatch from
`BoundingVolume::collide` to `BoundingVolume::outerCollide`. Original vtable
relocations for `BoundingVolume`, `BoundingAAB`, and `BoundingSphere` verify
the address-point slots:

```text
+0x00 getCollisionNormal
+0x04 update
+0x08 collide
+0x0c outerCollide
+0x10 projectCollisionOnSurface
```

The original loads `[vptr, #0x0c]`; the previous reconstruction loaded
`[vptr, #0x08]`. The loop, call arguments, collision-index store, and return
paths now use the real virtual method.

Remaining structural differences are register-allocation scheduling before
the AABB guards. The original eagerly preserves the incoming float bit
patterns:

```text
mov r8, r3    // z
mov r9, r2    // y
mov sl, r1    // x
movs r0, #0
```

The current build keeps the values in VFP registers for the comparisons and
sinks each core-register copy to its latest safe point. All six radius
comparisons, short-circuit branches, the bounding-volume loop, slot `+0x0c`
call, `collisionIndex` store, and false/true tails otherwise match.

Verified fields are position `+0x58/+0x5c/+0x60`, bounding volumes `+0x130`,
collision index `+0x150`, and collision radius `+0x154`.

Two fresh retries found no same-version source evidence for a distinct
semantic local or aggregate. Scalar aliases fold to the same SSA values;
forcing eager copies would require an artificial address escape, volatile,
dummy use, asm, or attribute. Requeue only with authentic original-source
evidence for parameter liveness.
