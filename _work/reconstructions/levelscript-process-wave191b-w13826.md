# LevelScript::process — wave 191b / w13826: case-158 reloads, combined timer store, tail expect

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13826`, branch
`wave191b-w13826`, base `e59df5414`.

Session start (HEAD): raw **53,006**, aligned **19,501**, `.text` diffbytes 76,645,
`.ARM.extab` 623, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`.
Session end (landed): raw **53,422**, aligned **19,467**, `.text` 77,061,
`.ARM.extab` **622**, `.rel.plt` unchanged, size 57,232/57,232 exact.

## Landed

### 1. case-158 (mission 0x9e) state 0 — drop the `KIPlayer *enemy` cache (`N`)

Original `0x1437xx` reloads `(*enemies)[0]` for `isVisible`, `setVisible` and
`setActive` instead of caching it in a local.  Removing the cache is faithful and
changes the surrounding register allocation (size -8 before compensation).  Alone:
aligned **19,501 -> 19,497**, size 57,224 (-8).

### 2. case-158 state 1 — combined timer zeroing before `setExhaustVisible(true)` (`P`)

Original `0x1438b8..0x1438d0` zeroes `m_nScriptTimerA` and `m_nScriptTimerB` with a
single `vmov.i32 q8,#0; vst1.64 {d16-d17},[r9]` **before** `setExhaustVisible(true)`.
Moving `m_nScriptTimerB = 0` above that call makes clang 7.0.2 emit the same combined
store.  Alone: aligned **19,501 -> 19,499**, size 57,232 exact.

### 3. shared-tail state-0 success guard — `__builtin_expect(..., 0)` (`H`)

The original places the state-0 success block out of line after the epilogue
(`0x144f80`, reached by `beq.n 144f80`) and skips the `m_nState` reload on that path.
The expect hint reproduces the out-of-line placement.  It costs +8 B on its own and
exactly compensates `N`'s -8 B, restoring 57,232.

### 4. shared-tail distance vectors — single `diff` temp (`T3`)

```c++
-            Vector playerPos = player->getPosition();
-            Vector enemyPos = (*enemies)[0]->getPosition();
-            Vector diff = playerPos - enemyPos;
+            Vector diff = player->getPosition() - (*enemies)[0]->getPosition();
             int distance = (int) AbyssEngine::AEMath::VectorLength(diff);
```

Free aligned -1 (raw and `.text` unchanged); kept because it is a measured win.

### 5. shared-tail station-list compare — re-read `Globals::status->field_90` (`S3`)

The original's loop count load lands in `r2` (`ldr r2,[r1,#0]; cmp r4,r2` at
`0x144f1c`).  Re-reading `Globals::status->field_90` in the compare (while the store
keeps the cached `stationList`) makes clang 7.0.2 allocate the count to `r2`, matching
the original.  Free aligned -2 (raw/`.text`/`.extab` unchanged).

## Measured variants (all size 57,232 exact unless noted)

| variant | raw | aligned | size | .text | .ARM.extab |
|---|---:|---:|---:|---:|---:|
| HEAD | 53,006 | 19,501 | 57,232 | 76,645 | 623 |
| T3 only | 53,006 | 19,500 | 57,232 | 76,645 | 623 |
| P only | 53,017 | 19,499 | 57,232 | 76,656 | 623 |
| P+T3 | 53,017 | 19,498 | 57,232 | 76,656 | 623 |
| N only | 53,352 | 19,497 | 57,224 (-8) | — | — |
| N+P | 53,359 | 19,470 | 57,224 (-8) | — | — |
| **N+P+H+T3+S3 (landed)** | **53,422** | **19,467** | **57,232** | **77,061** | **622** |
| N+P+H+T3 (no S3) | 53,422 | 19,469 | 57,232 | 77,061 | 622 |
| N+P+H (no T3) | 53,422 | 19,470 | 57,232 | 77,061 | 622 |
| N+P+F+H | 53,428 | 19,466 | 57,228 (-4) | — | — |
| N+P+F+H+T3 | 53,428 | 19,465 | 57,228 (-4) | — | — |
| N+P+F+H+T2 | 53,434 | 19,446 | 57,244 (+12) | — | — |
| N+P+H+G (case-154 faithful order) | 53,469 | 19,479 | 57,232 | — | — |

`F` = radio-14 moved into the state-0 success block (faithful, but leaves the block
4 B short of exact with `H`).  `G` = full faithful case-154 state-0 order (landed
separately as the w13826 case-157 pass family; combining it here is a metric loss).

## Notes

* Raw/`.text` rise by +416 for an aligned -32 because `N` shifts the function body;
  the same-address raw metric is dominated by that shift (the function is ~93 %
  byte-different).  `.ARM.extab` content shrinks by one byte (623 -> 622).
* Original shared-tail success block is out of line; ours is inline except on the
  `H` path.  The state-0 success block also sets `m_nState = 1` **after** the
  `setAlwaysEnemy` loop in the original, while our source sets it before the loop.
  Moving it (`F`) is faithful but size-fatal without `H`.
* Prologue register roles still differ (`this` in `sl`/`r4` in the original vs `fp`
  in ours; `Level *` in `r4` vs `fp`), which accounts for many of the remaining
  1-instruction aligned mismatches.
