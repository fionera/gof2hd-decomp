# LevelScript::process — wave 190 / w13367: rebase w13338 d2 rotations

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`,
57,232 B, size-exact).  Worktree `/home/debian/GalaxyOnFire2/wt/w13367`,
branch `wave190-w13367`, base master `0f54626b3` (raw **51,820**,
`.text` 73,516, `.ARM.extab` 610, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

Task: rebase the eight 3-statement rotations from w13338's candidate
`2b1318a65` onto current master, one at a time.  The landed d4/d3 passes
overlap at lines 2326/2492/3343/5103, so each site is re-measured on the
current body instead of applying the candidate patch verbatim.

## Landed rotations

| # | source site | rotation | raw before -> after |
|---:|---|---|---:|
| 1 | line 2281 (escort state 1) | `setTarget` + `setComputerControlled` before `setLookAtCam(false)` | 51,820 -> 51,819 |
| 2 | line 2441 (mission-0 state 0) | camera `setTarget` before `setLookAtCam(true)` + `setActive(true)` | 51,819 -> 51,818 |
| 3 | line 3188 (mission state 9) | enemy `setVisible(false)` + player `setVulnerable(true)` before enemy `setActive(false)` | 51,818 -> 51,817 |
| 4 | line 4752 (mission 8) | enemy0 `setVulnerable(true)` before enemy1 `setAlwaysEnemy` + enemy0 `setEnemies(nullptr)` | 51,817 -> 51,816 |
| 5 | line 5103 (mission state 9) | `m_nScriptTimerA/B = 0` before `player->setVisible(false)` | 51,816 -> 51,814 |

Each is size-exact (57,232/57,232) and keeps `.rel.plt` byte-identical.

## Rejected on the current body

The other three candidate sites are already covered by the landed d4/d3
reorderings, and re-testing the candidate's intended order on the current
body is not a win:

| source site | tested order | raw |
|---|---|---:|
| line 2326 | `setFreeLookMode` / `setTurretMode` / `resetCamera` / `enableFirstPersonCam` | 51,820 (base 51,819, +1) |
| line 2326 | candidate's pre-rotation order (`setTurretMode` / `resetCamera` / `setFreeLookMode` / `enableFirstPersonCam`) | 51,816 (base 51,814, +2) |
| line 2492 | candidate's post-rotation order (`setLookAtCam` / `setFreeLookMode` / `setComputerControlled` / `radar` / `setActive`) | 51,814 (neutral) |
| line 2492 | d4 order + `setLookAtCam` before `setComputerControlled` | 51,818 (base 51,814, +4) |
| line 3343 | `resetCamera` / `setFreeze` / `resetGunDelay` (candidate's post-rotation) | 51,814 (neutral) |

## Final state

```
process raw             51,820 -> 51,814  (-6)
process aligned insns   20,981 -> 21,712
orig / ours size        57232 / 57232  (size-exact)
.text diffbytes         73,516 -> 73,510  (-6)
.rel.plt                md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
.ARM.extab              610 (unchanged)
file size               2,187,328 (exact)
```
