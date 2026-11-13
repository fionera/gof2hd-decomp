# `LevelScript::process` — w13839 note (state-16 block order)

Target `_ZN11LevelScript7processEi` (57,232 B). The candidate moved the
state-16 block statements and set `initStreamOutPosition = 0`.

Measured on the current tree (master `ebc7c7330`, after w13826/w13828):

| | before | candidate |
|---|---:|---:|
| raw | **53,083** | 53,427 (+344) |
| aligned | **19,162** | 19,160 (-2) |
| size | 57,232 | 57,232 |
| `.text` diff | **75,882** | 76,222 (+340) |
| `.ARM.extab` | 552 | 552 |

The aligned gain is only -2 for a +344 `.text` cost, so the code change was
reverted and kept as this note. Re-land it only if a later shape removes the
branch-offset growth.
