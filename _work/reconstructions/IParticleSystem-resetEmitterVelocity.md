# IParticleSystem::resetEmitterVelocity() — deferred stack-scheduling residual

Symbol: `_ZN15IParticleSystem20resetEmitterVelocityEv`

Current metric: 91.7%, original 92 bytes, ours 92 bytes, not linked. Original
address: `0x181b34`. Both use a 16-byte stack frame with one 12-byte Vector
slot, matching field offsets and calls.

## Remaining shape

The original zeros the slot with `r1 = sp`, assigns it to
`emitterVelocity`, then only later establishes `r8 = sp` for the
`MatrixGetPosition` result and `lastEmitterPosition` assignment. Ours
materializes `r8 = sp` before the zero/canary setup and moves the matrix-field
load relative to the dirty-byte store. The observable operation order and all
calls remain equivalent.

## Source shapes tested

1. Baseline raw 12-byte storage, followed by placement construction of the
   MatrixGetPosition result into the same slot: 92 bytes / 16-byte frame.
2. Two typed Vector locals: 92 bytes but a 32-byte frame and major structural
   regression.
3. One typed Vector reused via assignment: 112 bytes with extra copy calls.
4. One typed Vector with a legal lifetime restart and placement construction:
   restored 92 bytes / 16-byte frame, but generated exactly the baseline
   residual.

The final typed no-op was reverted; the current raw-slot body remains the
closest machine match.

## Hypothesis / requeue condition

The residual is stack-base/local scheduling after the authentic one-slot
lifetime reuse has already been recovered. Do not retry with extra locals,
undefined object lifetimes, artificial liveness, `volatile`, or alias tricks.
Requeue only with stronger original-source lifetime evidence or after another
authentic particle TU change affects register allocation.
