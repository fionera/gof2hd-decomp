# LevelScript::process — wave 191b / w13657: case-154 state-4 camera matrix block

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13657`, branch
`wave191b-w13657`, base master `fda02cb7e` (raw **51,507**, aligned **22,183**,
`.text` 1,259,464 = delta 71,201, `.ARM.extab` 156,312 = delta 577, `.rel.plt`
md5 `289b2a069f8504d1e8a3919e43915e04`).

## Lens

The handoff notes (w13642 case-14 state-1, w13631 case-92 state-4, w13634
case-70/105/154, w13599 case-102, w13570 first-half, w13561 tail) leave the
case-154 state-4 camera-matrix block (source L4640-4658) as the largest
*unreconstructed semantic* cluster in the tail.  w13634 had already recorded
that the block is missing the right-vector term and that the faithful form is
size-exact but raw +6 / aligned -14; this pass re-derives the whole block from
the Thumb disassembly and measures the full faithful order.

## Original disassembly (case 154 state 4, `0x14313e`..`0x14329a`)

```
0x14313e  step = delta * 0.25
0x143150  (*enemies)[0]->geometry->moveForward(step)
0x14315a  canvas = Globals::Canvas
0x143164  CameraGetCurrent()
0x14316c  CameraGetLocal(current)
0x143176  local = *(...)                       (60-byte memcpy)
0x143182  m_tempVec = MatrixGetPosition(local)
0x1431a6  m_dirVec  = player->GetDirVector()
0x1431b4  m_tempVec += m_dirVec * (step * 0.008)
0x1431e6  m_dirVec  = player->geometry->getRightVector()
0x1431f0  m_tempVec += m_dirVec * (step * -0.05)
0x143210  m_dirVec  = player->GetUpVector()
0x14321a  m_tempVec += m_dirVec * (step * 0.005)
0x14323a  m_pCamera->setFixed(true)
0x14324a  MatrixSetTranslation(local, m_tempVec)
0x143296  m_pCamera->setLocal(local)
```

Pool words: `0x143124 = 0.008f`, `0x143130 = -0.05f`, `0x143530 = 0.005f`.

Our source had three real differences:

1. `m_dirVec = player->geometry->getRightVector();` was immediately overwritten
   by `m_dirVec = player->GetUpVector();` — the right-vector term
   `m_tempVec += m_dirVec * (step * -0.05f)` was missing, and the `+0.005f`
   term used the **up** vector for the `-0.05` add (both wrong).
2. `m_tempVec = MatrixGetPosition(local)` came after the `GetDirVector` load;
   the original loads the matrix position first.
3. `m_pCamera->setFixed(true)` came before the matrix-position load; the
   original emits it after the three vector adds, immediately before
   `MatrixSetTranslation`.

## Landed change

The full faithful order (the `moveForward` call is kept after the
`CameraGetLocal` copy: it writes `enemies[0]`, not the camera, so its position
relative to the matrix reads is semantically irrelevant, and moving it before
the canvas load costs +2 raw):

```cpp
                     float step = (float) delta * 0.25f;
                     AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                     Matrix local = *reinterpret_cast<Matrix *>(canvas->CameraGetLocal(canvas->CameraGetCurrent()));
+                    (*enemies)[0]->geometry->moveForward(step);
+                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                     m_dirVec = player->GetDirVector();
-                    (*enemies)[0]->geometry->moveForward(step);
-                    m_pCamera->setFixed(true);
-                    m_tempVec = AbyssEngine::AEMath::MatrixGetPosition(local);
                     m_tempVec += m_dirVec * (step * 0.008f);
                     m_dirVec = player->geometry->getRightVector();
-                    m_dirVec = player->GetUpVector();
                     m_tempVec += m_dirVec * (step * -0.05f);
+                    m_dirVec = player->GetUpVector();
                     m_tempVec += m_dirVec * (step * 0.005f);
+                    m_pCamera->setFixed(true);
```

Measured alone on the real build (exact NDK r18b `-Oz` compile + full gold
link + `--strip-all` + `tools/fn_diff.py`):

```
raw        51,507 -> 51,524  (+17)
aligned    22,183 -> 21,921  (-262, fn_diff mnemonic-sequence metric)
size       57,232 / 57,232   (exact)
.text      71,201 delta (unchanged; 1,259,464 B)
.ARM.extab 577 delta, 156,312 B, md5 6630befd… (size/base/content unchanged)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

This is an aligned-weighted win of the kind accepted for `Layout` C1/C2
(w13607: raw +3, aligned -32) and case-70 (w13634: raw +2, aligned -643).
It also fixes two real semantic bugs (missing right-vector term, wrong
up-vector term).

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| baseline | 51,507 | 22,183 | 57,232 | 577 |
| **case-154 s4 full faithful (landed)** | **51,524** | **21,921** | **57,232** | **577** |
| case-154 s4 truly faithful (`moveForward` before canvas) | 51,526 | 21,921 | 57,232 | 577 |
| case-154 s4 faithful vector + `setFixed` late, `moveForward` after `GetDirVector` | 51,517 | 22,170 | 57,232 | 577 |
| case-154 s4 faithful vector order, `setFixed` early | 51,513 | 22,169 | 57,232 | 577 |
| case-154 s4 minimal right-vector add only | 51,513 | 22,169 | 57,232 | 577 |
| case-154 s4 faithful vector, `MatrixGetPosition` before `GetDirVector`, `setFixed` early | 51,513 | 22,169 | 57,232 | 577 |
| case-154 s4 faithful vector, `moveForward` after `GetDirVector`, `setFixed` late | 51,517 | 22,170 | 57,232 | 577 |
| case-154 s4 "swap" order (up dead, right used for both adds) | 51,512 | 21,945 | 57,232 | 577 |
| case-154 s4 `MatrixGetPosition` before `GetDirVector` + `moveForward` before canvas | 52,072 | 21,921 | 57,208 | 577 |
| case-154 s4 full faithful with `setFixed` after `MatrixSetTranslation` | 52,030 | 22,171 | 57,224 | 577 |

The `MatrixGetPosition`-before-`GetDirVector` reorder changes the function size
to 57,208 (-24) unless `moveForward` also moves before the canvas load; the
landed form keeps the size exact.

## Other clusters measured (no landing)

**case-158 state 2 teardown (L5263-5300).**  Original order
(`0x140414`..`0x140512`): `resetGunDelay`, `field_0x24 = 0`, `setLookAtCam`,
`setTarget`, `setComputerControlled`, `setFreeze`, `setVisible`, `setSpeed`,
`hud`, `radar`, `setVulnerable`, `resetCamera`, `forceUpdate`,
`m_bCinematicBreak`, timers, `setCloakingPossible`, `setAlwaysEnemy`,
`setAIDisabled`, `setDirection`, `state = 3`.  Full faithful reorder:
raw **51,512** (+5), aligned **22,146** (-37), size exact, extab unchanged.
Sub-moves: `setSpeed` before `hud` alone 51,512/22,161; `m_bCinematicBreak`
after `forceUpdate` alone 51,511/22,181; `setVulnerable` after `resetCamera`
alone neutral.  Not landed: smaller aligned win than case-154 s4.

**case-158 state 1 (L5230-5255).**  Original (`0x138878`..`0x13898c`) increments
`m_nScriptTimerA` **before** computing `approach = timerA / -34000 + 1`, and
calls `m_pCamera->setTarget` before `player->geometry->rotate`.  Ours computes
`approach` from the pre-increment timer and swaps the target/rotate pair.
- approach-after-increment alone: raw 51,510 (+3), aligned 22,175 (-8).
- setTarget-before-rotate alone: raw 51,508 (+1), aligned 22,174 (-9).
- both: raw 51,511 (+4), aligned 22,166 (-17), size exact.
Not landed: smaller aligned win than case-154 s4.

**case-91 state 5 (L2915-2926).**  Original (`0x13f3a0`..`0x13f538`) uses
`getDirection()` for the `* 10000` add and `getRightVector()` for `* 700`;
ours uses `getRightVector()` for both and leaves a dead `getDirection()` at the
end.  The faithful form removes the dead call and is size 57,224 (-8); it needs
a compensating +8 reshape.  The rocket-control reorder is size-exact but
raw +3 / aligned -5.  Not landed.

**case-16 state 0 (L1670-1688).**  Original order (`0x13b014`..`0x13b0e6`) is
`setTurretMode`, `resetCamera`, `setFreeLookMode`, `enableFirstPersonCam`,
`hideShip`, `stopShooting`, `m_bCinematicBreak`, `hud`, `radar`,
`setComputerControlled`, `setLookAtCam`, `setActive`, `setTarget`,
`getPosition`.  Full faithful reorder: raw 51,532 (+25), aligned 24,192
(+2,009).  Single-statement move sweep (19x19 cells): best raw 51,509
(`enableFirstPersonCam` after `setTurretMode`), no raw win.  Not landed.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13657/` (`measure.sh`, `sec.py`,
`diffmap.py`, `linemap.py`, `alignmap.py`, `c158*.py`, `c154*.py`,
`case16_sweep.py`).
