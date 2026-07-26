# `MGame::OnSuspend` header blockers

## Target

- `_ZN5MGame9OnSuspendEv`
- 88.4%, original 192 bytes / current 196 bytes
- TU: `src/game/menu/MGame.cpp`

The wrong-callee pass restored `Globals::recordHandler`,
`RecordHandler::saveOptions()`, `MGame::pauseSounds()`, and the real
`Hud::releaseAllKeys()` tail call. The remaining size and structural
differences require exclusive header work:

| Witness | Original | Current |
| --- | ---: | ---: |
| `new MenuTouchWindow(1)` allocation | 576 (`0x240`) bytes | 572 (`0x23c`) bytes |
| `PlayerEgo::isDead()` result | passed directly as the bool argument | declared `int`, so Clang emits `cmp` / `it` / `movne` normalization |

Verified `MGame` offsets in this body are:

- `player` `+0x58`
- `pauseOpen` `+0x5d`
- `jumpActive` `+0x5f`
- `hud` `+0x74`
- `level` `+0x78`
- `menuWindow` `+0x88`
- `menuTouchOpen` `+0xc9`
- `pauseSnapshot` `+0x1a6`

The original stores `pauseSnapshot` before `pauseOpen`; the current body now
does so as well. One residual `mov r5, r0` versus `cbz r0` scheduling
difference around `Level::getEnemies()` is allocator-only. Two retries found
no honest C++ body change that improves it without adding volatility or an
artificial use.

Requeue only as an exclusive pass that reconciles the complete
`MenuTouchWindow` layout and the `PlayerEgo::isDead()` declaration against
all callers. Do not force either difference with a raw allocation size,
mismatched call type, or other ABI workaround.
