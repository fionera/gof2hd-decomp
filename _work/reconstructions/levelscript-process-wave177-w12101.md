# LevelScript::process — wave 177 / w12101 (recovered cell)

Target `_ZN11LevelScript7processEi` (57,232 B), base master `9445ac5fb`
(wave-176 merge, including w12102/w12103/w12104).

The w12101 agent timed out before committing; its scratch cell table
(`scratch/w12101/cells.tsv`) records the best size-exact cells. On the combined
tree only one of them survives:

- line 5356 `if (level->getPlayer() != 0)` -> `__builtin_expect(..., 0)`
- raw **53,507 -> 53,506** (-1), size **57,232 / 57,232** exact

The `isOver()` cell at line 1692 (`expect(..., 0)`) is size-changing (+8 B) on
the combined tree because of the w12102 `cam1688` alias; it was rejected.
The `m_nFlashCountdown > 0` cell is already in the tree from w12104.
