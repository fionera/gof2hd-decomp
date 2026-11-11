# Radar::draw — wave 177 / w12107 (recovered best variant)

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (9,908 B), base master `9445ac5fb`
(wave-176 merge, including w12005/w12006/w12007/w12108).

The w12107 agent timed out before committing; its scratch saved the best
variant as `scratch/w12107/Radar.base5.cpp`. Applied to the combined tree and
re-measured:

- raw **6,659 -> 6,636** (-23)
- aligned 1,712 -> 1,813
- size **9,908 / 9,908** exact; `.rel.plt` unchanged

Six branch-weight edits (all size-neutral):

1. `landmarks != nullptr && ...` whole-condition `expect(..., 0)`.
2. `planetLockTimer > field_0x1b8` `expect(..., 1)`.
3. `enemy->player->enemyFlagsLo` `expect(..., 1)` (the `&&` tail).
4. `cargo == nullptr` — revert the wave-176 `expect(..., 0)`.
5. `getCurrentCampaignMission() != 145` — revert the wave-176 `expect(..., 1)`.
6. `inAlienOrbit()` — revert the wave-176 `expect(..., 0)`.

The variant table and the full combo search are in
`scratch/w12107/run.log` (`combo4`/`combo5`/`combo6`). Re-measure on the
combined tree before merge.
