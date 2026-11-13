# LevelScript::process — wave 191b / w13559: case-67 escort wake order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13559`, branch
`wave191b-w13559`, base master `2baef717d` (raw **51,540**, aligned 22,826,
`.text` 71,240, `.ARM.extab` 576, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Diff-cluster map (first step)

`tools/fn_diff.py` plus the same-address byte runs mapped through the debug
line table (scratch `clusters.py`) put the raw diff in these campaign-case
buckets (LevelScript.cpp lines only):

| case | lines | raw diff bytes |
|---|---|---:|
| 157 | 4796-5226 | 4,932 |
| 105 | 3587-3951 | 3,339 |
| 154 | 4516-4795 | 3,169 |
| 92 | 2946-3201 | 3,117 |
| 102 | 3310-3586 | 2,927 |
| 80 | 2623-2838 | 2,100 |
| pre-campaign | 605-1577 | 7,473 |

An exhaustive adjacent-statement (d1) swap sweep of the current body (915
candidates, exact NDK r18b `-Oz` compile + gold link each) found two
size-exact / `.rel.plt`-identical raw wins.  One is landed here; the other
(`if (m_nScriptTimerA < 75001) break;` before `if (m_nState == 2) m_nState = 3;`
in case 102) is rejected because swapping lets a state-2 ship advance before
the break (the w13524 note's rejected cell 180).

## Landed change

Case-67 escort wake block (source lines 1941/1942): swap the two independent
flag stores so `setFreeLookMode(false)` precedes `setVulnerable(false)`:

```cpp
-                    player->player->setVulnerable(false);
                     player->setFreeLookMode(false);
+                    player->player->setVulnerable(false);
                     m_pHud->visible = 0;
```

`PlayerEgo::setFreeLookMode` writes only the free-look flag; the vulnerable
flag is a separate `Player` field.  Neither call reads the other's state, so
the reorder is semantics-preserving.

Measured alone on the real build:

```
raw        51,540 -> 51,539  (-1)
aligned    22,826 -> 22,826
size       57,232 / 57,232   (exact)
.text      71,240 -> 71,239  (-1)
.ARM.extab 576 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507/4524, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

Scratch harness (`parser.py`, `sweep.py`, `measure.py`, `clusters.py`) lives
in `/home/debian/GalaxyOnFire2/scratch/w13559/`.
