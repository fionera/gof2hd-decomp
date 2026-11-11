# LevelScript::process — wave 177 / w12101 (recovered cells)

Target `_ZN11LevelScript7processEi` (57,232 B), base master `9445ac5fb`
(wave-176 merge, including w12102/w12103/w12104).

The w12101 agent timed out before committing; its scratch cell table
(`scratch/w12101/cells.tsv`) records the candidate cells. On the combined tree:

- `m_nFlashCountdown > 0` `expect(..., 1)` is already in the tree from w12104.
- `if (level->getPlayer() != 0)` at line 5356 is in `resetCamera`, not
  `process`; adding the expect there grows `resetCamera` 124 -> 128 B and
  shifts `.text` by 32 B (section alignment). **Rejected** — `resetCamera`
  stays byte-exact at 124 B.
- `isOver()` `expect(..., 0)` at line 1692 is size-changing (+8 B) on the
  combined tree because of the w12102 `cam1688` alias. **Rejected.**

Net: no additional code change from w12101; the LevelScript raw remains
**53,507** with size 57,232 exact.
