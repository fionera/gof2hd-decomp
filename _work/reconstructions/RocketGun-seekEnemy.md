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
