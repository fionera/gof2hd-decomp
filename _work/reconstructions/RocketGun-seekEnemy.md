# `RocketGun::seekEnemy` layout blocker

## Target

- `_ZN9RocketGun9seekEnemyEii`
- 73.2%, original 332 bytes / current 340 bytes
- TU: `src/game/weapons/RocketGun.cpp`

The wrong-callee pass replaced all three invented
`VectorRotateToTarget(Vector&, const Vector&)` calls in this method with the
real exported
`AbyssEngine::AEMath::VectorNormalize(const Vector&)` return-value call.
Nested expressions and lexical lifetimes now reproduce the original
44-byte frame and physical Vector slots:

| Stack slot | Value |
| --- | --- |
| `sp+0x04` | enemy position |
| `sp+0x10` | target delta, later final normalized velocity |
| `sp+0x1c` | normalized target, later normalized current velocity |
| `sp+0x28` | stack canary |

Verified object offsets and constants are:

- `RocketGun::gun` `+0x08`
- `RocketGun::radar` `+0xb0`
- steering Vector `+0xb4`
- turn rate `+0xc8`
- `Gun::owner` `+0x04`
- `Gun::positions` `+0x0c`
- `Gun::velocities` `+0x18`
- `Gun::pitchRate` `+0x50`
- turn-rate multiplier `20.0f`

The remaining target-selection mismatch is a header layout witness. The
original reads the KIPlayer enemy index at `+0x38`; the current named
`field_0x34` compiles at `+0x34`. Requeue this only in an exclusive KIPlayer
layout pass that checks every dependent class and method. Do not replace the
named access with a raw byte offset.

The file-local `VectorRotateToTarget` declaration must remain until the
excluded 1908-byte `RocketGun::update(int)` reconstruction removes its three
remaining uses.

## Second angle (w3267, wave 91) — residual is allocator colouring, not source shape

Control flow, constants, call order and every struct offset match; the residual is greedy
allocator colouring + spill-slot ordering:

| | original | ours |
|---|---|---|
| `elapsed` | r4, spilled by one merged `strd r4, sl, [sp,#16]` | r5, two separate `str r5,[sp,#12]` / `str.w sl,[sp,#20]` |
| `player` | r5 | r4 |
| loop `i*60` (Matrix offset) | r4, with `mov r9,r4` / `mov r4,r9` save-restore | r9 |
| `forward` | r8 -> 4-byte `add.w r8,sp,#116` | r4 -> 2-byte `add r4,sp,#116` |
| Sinf result `s` | spilled to `[sp,#8]` | kept in r6 |
| `trailMatrices` | rematerialised from `[sl,#216]`, no slot | spilled to `[sp,#16]` |

MIR frame objects: ours is created `dt, this, trailMatrices, elapsed, c, guard` -> offsets
`24,20,16,12,8,4`; the original needs `guard, s, c, elapsed, this, dt` -> `4,8,12,16,20,24`.
The original has an extra `s` slot and no `trailMatrices` slot; that extra slot pushes
`elapsed` 12 -> 16 and enables the `strd` merge.

~200 variants measured (all <= 86.9%): all 120 declaration permutations of the muzzle block
(best 81.8%; current interleaved order wins), `dt` placement (82.1-84.2%), anchor removal
(85.6-86.7%), `trailMatrices` rematerialised through `this->trailMatrices` (57.6%, 1880 B),
`AEGeometry *geom` hoists (80.6-86.7%), cluster-block declaration order (86.5-86.9%),
`volatile`/`static_cast`/reference forms (byte-identical at 86.9%).

Conclusion: the original's `elapsed`->r4 colouring needs the Sinf result to take its own stack
slot before the Cosf slot (r6 occupied at that point). No faithful source form tried does that
without losing more elsewhere; `-mllvm -stress-regalloc=12` does put `elapsed` in r4 but scores
44.7%, so this is a whole-function colouring state, not a one-register fix.
