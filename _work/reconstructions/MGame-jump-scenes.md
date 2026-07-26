# `MGame` jump-scene reconstruction checkpoint

Targets:

- `_ZN5MGame14startJumpSceneEv`, original `0x17c0d4`
- `_ZN5MGame15updateJumpSceneEv`, original `0x17f588`
- Original TU: `Classes/Game/MGame.cpp`
- Current TU: `src/game/menu/MGame.cpp`

## Landed measurements

| Function | Previous | Landed | Original size | Landed size |
|---|---:|---:|---:|---:|
| `startJumpScene` | 54.6% | 83.0% | 896 | 904 |
| `updateJumpScene` | 41.0% | 76.5% | 920 | 912 |

The pass removed the invented `TFC_useTargetsUpVector` and
`FModSound_setProp` imports. The real calls are
`TargetFollowCamera::useTargetsUpVector` and `FModSound::stop`.

## Verified source facts

`updateJumpScene` returns `bool`, not `void`:

- The original caller at `0x17d9dc` calls the function, compares `r0` with
  zero, and branches on a nonzero result.
- The J2ME mapping records `updateJumpScene()Z`.
- The original callee sets `r5 = 0` on both not-ended paths at `0x17f790`,
  sets `r5 = 1` after the two completion paths at `0x17f816` and `0x17f8ac`,
  and moves `r5` to `r0` at `0x17f8ce`.

That caller belongs to the 11,440-byte original `MGame::OnUpdate` monster.
The current 88-byte placeholder still ignores the result; restore its
result-consuming branch only as part of the dedicated per-case monster
reconstruction, not as a local jump-scene codegen tweak.

The animation-time gate is the signed 64-bit expression
`1700LL - transform->currentTime < 0`. It now compiles to the original
sequence at `0x17f5be-0x17f5ca`: load the low/high words, subtract the low
word from 1700, subtract the high word with borrow, and branch on a negative
result.

The two movement vectors are:

- jump drive: `{2 * dt, 5 * dt, -5 * dt}`;
- jump gate: `{2 * dt, 5 * dt, -4 * dt}`.

The completion path uses the real globals
`Level::programmedStation`, `Level::comingFromAlienWorld`,
`Level::initStreamOutPosition`, and
`Globals::switch_to_target_setting`. It switches to application module 2.

Verified `startJumpScene` values and source operations include:

- `field_0x70 = 0x3f9c28f6`;
- `CameraSetPerspective(cameraId, field_0x70-as-float, 20.0f, fov)`;
- FOV values `300000.0f` and `450000.0f`;
- post effect `0x01400002`;
- jump-gate landmark index 1;
- offsets `-10000`, `{-2000, 300, -2000}`, and `+4000`;
- direction scale `3000`;
- sound stops for the player sound id, `0x23`, `0x8d5`, and `0x8d4`,
  followed by play id `0x20`.

The drive-branch camera coordinates are loaded only after the sound calls,
matching the original loads at `0x17c344-0x17c34c`.

## Remaining structural mismatch

`startJumpScene` is eight bytes too large. The original prologue saves
`d3-d8` and has no scalar frame; the landed build saves only `d8` and
allocates 72 bytes. The reconstructed operations and call targets are
substantially aligned, but the Vector temporary lifetimes and stack layout
still differ.

`updateJumpScene` is eight bytes and three decoded instructions short. Its
frame size, 64-bit countdown, boolean return value, and conditional return
epilogue now match. The original saves `fp` in the extended integer set,
while the landed build saves `sl`; the movement/common-tail scheduling and
several later branch shapes still differ.

Requeue only with new source-shape evidence for the Vector lifetimes and
shared control-flow tails. Do not use volatile objects, dummy reads, manual
borrow extraction, inline assembly, or artificial stack forcing.
