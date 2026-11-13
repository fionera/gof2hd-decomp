# LevelScript::process — wave 191b / w13839: case-5 teardown + case-92 dock sub-order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13839`, branch
`wave191b-w13839`, base master `44ad5324b` (commit 15 `8cf545c37`).

Session start (HEAD `8cf545c37`): raw **53,423**, aligned **18,840**, `.text`
diffbytes 76,341, `.ARM.extab` 552, `.ARM.exidx`/`.rodata`/`.rel.plt` diff 0,
size 57,232/57,232, insn 20,726 / 20,830.

## Landed

### 1. case-5 (`m_nScriptTimerA > 10000`) teardown order — `0x13f880..0x13f8fe`

Original order: `resetGunDelay` -> `setLookAtCam(false)` -> `setTarget(player->geometry)`
-> `setComputerControlled(false)` -> `setVulnerable(true)` -> `setFreeze(false)` ->
`setVisible(true)` -> `hud->visible = 1` -> `radar->enabled = 1` -> `resetCamera` ->
`forceUpdate` -> radar stores -> `m_bCinematicBreak = 0` -> `m_nState++`.

The full reorder (radar `lockedEnemy` store *after* `forceUpdate`, matching the
original's `strd r4,r4,[r0,#4]`) is semantically right but shrinks the function to
**57,224** (clang merges the two radar stores into one `strd`, and the layout shift
relaxes a branch): gate-fatal.  The landed variant keeps `lockedEnemy` before
`forceUpdate` (so the stores stay separate) and moves only `setTarget`,
`setVisible` and `m_bCinematicBreak`:

```c++
 m_pCamera->setLookAtCam(false);
+m_pCamera->setTarget(player->geometry);
 player->setComputerControlled(false);
 player->player->setVulnerable(true);
-m_pHud->visible = 1;
 player->setFreeze(false);
-m_pCamera->setTarget(player->geometry);
-m_bCinematicBreak = 0;
+player->setVisible(true);
+m_pHud->visible = 1;
 m_pRadar->enabled = 1;
 ...
-m_pRadar->dockNavPtr = nullptr;
-player->setVisible(true);
+m_pRadar->dockNavPtr = nullptr;
+m_bCinematicBreak = 0;
 m_nState++;
```

Result: raw **53,423 -> 53,436** (+13), aligned **18,840 -> 18,835** (-5),
`.text` 76,341 -> 76,354 (+13), `.ARM.extab` 552 unchanged, size exact.
Landed `55cb91424`.

### 2. case-92 state-7 dock sub-order — `0x1402a0`

Original dock block is `setAutoPilot` -> radar stores -> `resetGunDelay` ->
`dockToDockingPoint` -> `setDockingState(3)` -> `setRotationAroundTarget(false)`,
with `m_nScriptTimerA = 0` emitted at the very end of the outer block (the original
zeroes the 8-byte timer with `strd sl,sl,[r4]` at `0x143bd8`).  Also
`setTurretMode(false)` precedes `resetCamera` (`0x1402ae` before `0x1402be`).

Landed: `dockToDockingPoint` before `setDockingState`, `m_nScriptTimerA = 0` after
`setRotationAroundTarget`, `setTurretMode` before `resetCamera`.

Result: raw **53,436 -> 53,429** (-7), aligned **18,835 unchanged**, `.text`
76,354 -> 76,347 (-7), `.ARM.extab` 552 unchanged, size exact.
Landed `0c6f47d1d`.  (The aligned metric is dominated by the large residual
blocks and does not move for this local reorder; the raw/`.text` drop is real.)

## Measured variants (all size 57,232 exact unless noted)

| variant | raw | aligned | .text | .ARM.extab |
|---|---:|---:|---:|---:|
| session start (commit 15) | 53,423 | 18,840 | 76,341 | 552 |
| case-5 full reorder, radar `strd` (dirty from handoff) | 54,191 | 18,829 | 354,161 | 552 |
| case-5 A: only `setTarget` moved | 53,428 | 18,843 | 76,346 | 552 |
| case-5 A+B: `setVisible` before hud | 53,436 | 18,836 | 76,354 | 552 |
| **case-5 A+B + `m_bCinematicBreak` at end (landed)** | 53,436 | **18,835** | 76,354 | 552 |
| case-5 + `lockedEnemy` after `forceUpdate` | 54,191 | 18,829 | 354,161 | 552 |
| case-5 + `m_bCinematicBreak` between radar stores | 54,191 | 18,830 | 354,161 | 552 |
| case-69 st1 (`messages[9]`, state 5) full reorder + `freeze=0` | 55,004 | 18,836 | 358,922 | 555 |
| line-1755 block (`messages[4]`, state 5) reorder | 53,444 | 18,941 | 76,362 | 552 |
| line-2337 block + `freeze=0` | 55,104 | 18,835 | 402,461 | 1,237 |
| case-92 dock sub-order only | 53,432 | 18,835 | 76,350 | 552 |
| + `m_nScriptTimerA` at end only | 53,433 | 18,835 | 76,351 | 552 |
| **+ `setDockingState` after `dockToDockingPoint` (landed)** | 53,429 | **18,835** | 76,347 | 552 |
| + rocket `setRocketControl` before `killLiberator` | 53,429 | 18,835 | 76,347 | 552 |
| case-92 state-7 full main-block reorder | 53,427 | 18,860 | 76,345 | 552 |
| case-102 state-1 (`0x143b2c`) full reorder | 53,445 | 18,851 | 76,363 | 552 |

## Residual / next angles (not landed)

- **case-69 st1 (`messages[9]`, state 5) `0x13fb00`**: original has exactly ONE
  `forceUpdate` (at `0x13fb68`, after `resetCamera`) and a `player->freeze = 0`
  store (`strb r8,[r9,#36]` at `0x13fb16`) that our source lacks; ours has an
  extra `forceUpdate` right after `setVulnerable` and no freeze store.  Fixing it
  is semantically right but shrinks the function to **57,208** (-24): needs a
  compensating +24 elsewhere.  Original order: `setVulnerable` -> `freeze=0` ->
  `setSpeed(8.0f)` -> `setLookAtCam(false)` -> `setTarget` ->
  `setComputerControlled(false)` -> hud -> radar -> `resetCamera` ->
  `forceUpdate` -> `m_nState=6` -> `m_bCinematicBreak=0`.
- **case-102 st1 (`0x143b2c`, `m_nScriptTimerA >= 2001`)**: original order is
  `resetGunDelay` -> `setFreeze(false)` -> `setVisible(true)` -> `setVulnerable(true)`
  -> `setLookAtCam(false)` -> `setTarget` -> `setPosition` -> hud -> radar ->
  `resetCamera` -> `forceUpdate` -> `m_bCinematicBreak=0` -> `m_nScriptTimerA=0`
  -> `sound->stop` -> `sound->play(2241)` -> `m_nState++`.  Ours has
  `resetCamera` first, `setVulnerable` before `setVisible`, hud after
  `forceUpdate`, and `play` before `stop`.  Full reorder landed at aligned 18,851
  (worse) — the global aligned metric pairs this block with the wrong original
  region; needs a local (address-anchored) check, not the global metric.
- **line-2337 block (`0x13c43a`)**: original has `player->freeze = 0` between
  `setVulnerable(true)` and `setLookAtCam(false)`; ours lacks it.  Adding it grows
  the function to 57,240 (+8) and `.ARM.extab` to 1,237 — needs compensation.
- **Mission-1 fill-screen** `0x138a66`: original emits `enableFillScreen(true)` /
  `switch_to_target_setting = 0` / `SetCurrentApplicationModule(5)` *before* the
  camera-local matrix block; ours at `0x138c84` after (src ~960-980).
- **Largest residual aligned blocks**: `0x13e0e6` (n=614), `0x13e962` (423),
  `0x13d74a` (402), `0x13dbd8` (354), `0x141028` (324), `0x145c42` (242),
  `0x13b564` (234, literal pool), `0x13ed5e` (218 delete), `0x144a12` (211).
- **Prologue** `0x137d18`: original `this` in `sl`, `Globals::status` in `fp`;
  ours `status` in `r9`, `this` in `fp`.  Further variants worse.

## Final state

raw **53,429**, aligned **18,835**, size **57,232/57,232**, `.text` **76,347**,
`.ARM.extab` 552, `.ARM.exidx`/`.rodata`/`.rel.plt` diff 0, insn 20,726 / 20,830.
Gate: byte_exact 4508/4524, linked_exact 4508/4524, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
