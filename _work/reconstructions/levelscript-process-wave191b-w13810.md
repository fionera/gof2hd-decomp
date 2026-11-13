# LevelScript::process — wave 191b / w13810: case-144 statement order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13810`, branch
`wave191b-w13810`, base master `4065bea1b`.

Session start: raw **52,993**, aligned **19,776**, `.text` diffbytes 75,906,
`.ARM.extab` 594, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`.
Session end: raw **53,007**, aligned **19,694**, `.text` diffbytes 75,918,
`.ARM.extab` 594, `.rel.plt` md5 unchanged, size 57,232/57,232 exact.

## Landed (case 144, `src/game/world/LevelScript.cpp` ~line 4378)

Original disassembly 0x1393d8..0x139540 was used as the oracle (PLT addresses
resolved against `.rel.plt`).

1. **State 0**: the original calls `Explosion::addFireStreaks` (0x1393ec)
   before `Explosion::setScaling(12.0f)` (0x1393f6).  Our source had them
   reversed.

2. **State 1**: the original statement order is

   ```
   player->setTurretMode(false);
   if (player->isInRocketControl()) {
       player->setRocketControl(nullptr, nullptr);
       player->killLiberator();
       m_pCamera->setRumblePercentage(0.0f, 0);
   }
   resetCamera(self);
   player->setFreeLookMode(false);
   m_pCamera->enableFirstPersonCam(false);
   player->hideShipForFirstPersonCameraView(false);
   player->stopShooting(0);
   player->player->setVulnerable(false);
   player->setVisible(false);
   player->setFreeze(true);
   m_bCinematicBreak = 1;
   m_pHud->visible = 0;          // emitted first: radar load/store precede hud
   m_pRadar->enabled = 0;
   m_pCamera->setLookAtCam(true);
   ```

   Our source had `setRumblePercentage` first in the if-body, `setVisible`/
   `setFreeze`/`m_bCinematicBreak` before the camera calls, `setLookAtCam`
   before the hud/radar stores, and radar before hud.  Note the source order
   `m_pHud->visible` then `m_pRadar->enabled` is what makes clang emit the
   original radar-then-hud store sequence.

3. **State 1 camera position**: the original accumulates both
   `m_tempVec += ...getDirection() * 18000.0f` and
   `m_tempVec += ...getRightVector() * 3000.0f` *before*
   `m_pCamera->setPosition(m_tempVec)` (0x13951a).  Our source set the camera
   position between the two accumulations.

## Measured variants

| variant | raw | aligned | size |
|---|---:|---:|---:|
| session start | 52,993 | 19,776 | 57,232 |
| state-1 reorder only (`Level *self` kept) | 53,004 | 19,700 | 57,232 |
| + state-0 `addFireStreaks` first | 53,004 | 19,700 | 57,232 |
| + hud/radar source swap | 53,005 | 19,699 | 57,232 |
| + `setPosition` after both accumulations (landed) | **53,007** | **19,694** | 57,232 |
| same but `Level *self` removed (`m_pLevel` re-read) | 53,010 | 19,698 | 57,232 |
| hybrid `resetCamera(m_pLevel)` with `self` kept | 55,198 | 19,694 | **57,240** (size-fatal) |
| hybrid `(*m_pLevel->getEnemies())` with `self` kept | 55,196 | 19,695 | **57,240** (size-fatal) |

The `Level *self` local is kept: the original re-reads `m_pLevel` at
`resetCamera` (0x13943c) and `getEnemies` (0x13949a), but removing the local
costs 5 raw bytes for 1 aligned instruction and the hybrid forms are +8 B
size-fatal.

`cmake --build cmake-build-match --target check` = RATCHET PASS
(byte_exact 4508, linked_exact 4508, size 4524/4524, data 843/843, imports 207,
lint CLEAN).  `.rel.plt` byte-identical.
