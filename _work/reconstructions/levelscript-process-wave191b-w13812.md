# LevelScript::process — wave 191b w13812

Target: `_ZN11LevelScript7processEi`, size 57,232 B, mission-106 region.
Baseline entering this pass: raw 52,993 / aligned 19,776 / size 57,232 exact,
`.text` diffbytes 75,904, `.ARM.extab` diffbytes 594.

## Landed (three independent statement-order reconstructions)

1. **mission-106 state-4 teardown order** (original disasm `0x1439f2..0x143b14`)
   - `setType(4)` hoisted above `m_nState++`
   - `setRocketControl(nullptr, nullptr)` before `killLiberator()`
   - schedule `setFreeLookMode`, `enableFirstPersonCam`, `hideShipForFirstPersonCameraView`,
     `stopShooting`, `setFreeze`, `setVisible`, `setVulnerable`, `m_bCinematicBreak`,
     `m_pHud`, `m_pRadar`, `setLookAtCam`, `setTarget`, `setPosition`, `m_nScriptTimerA`
   - `m_pCamera` read directly instead of a cached `wcamera` local.
2. **mission-106 state-6 tail order** (original disasm `0x140f54..0x140ffe`)
   - `m_pLevel->getPlayer()->resetGunDelay()` first
   - `setFreeze(false)`, `setVisible(true)`, `setVulnerable(true)`
   - `setLookAtCam(false)`, `setTarget(player->geometry)`
   - `setPosition(m_dirVec)` then computed `setPosition(player->getPosition() - player->GetDirVector() * 2400.0f)`
   - `m_pHud`, `m_pRadar`, `resetCamera`, `forceUpdate`, `m_bCinematicBreak = 0`,
     `m_nScriptTimerA = 0`, `m_nState++`.
3. **mission-106 state-2 escort route arrays** (original disasm `0x141f24..0x141faa`)
   - `getRightVector()` before `getDirection()`
   - route expressions rebuilt as `r * 900000 +/- d * 900000` forms.

## Measurements

| experiment | raw | aligned |
|---|---:|---:|
| baseline | 52,993 | 19,776 |
| state-6 tail reorder only | 52,946 | 19,773 |
| state-6 + route rewrites | 52,957 | 19,747 |
| state-6 + route + state-4 reorder (landed) | **52,992** | **19,638** |
| state-4 tail reorder alone | 53,024 | 19,684 |
| route edits alone | 53,004 | 19,750 |

Final: raw **52,993 -> 52,992** (-1), aligned **19,776 -> 19,638** (-138),
size 57,232/57,232 exact, `.text` 75,904 -> 75,903, `.ARM.extab` 594
(content md5 changed, diffbytes unchanged), `.rel.plt` byte-identical.
Gate: byte 4508 / linked 4508 / data 843/843 / imports 207 / lint CLEAN /
drift 0 / parity 5370/5370.
