# LevelScript::process — wave 191b / w13645: case-158 state-1/2 order + operator* overload

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13645`, branch
`wave191b-w13645`, base master `4b2b1c10f` (raw **51,513**, aligned **22,174**,
`.text` 71,197, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

The prior wave-191b notes swept source lines 605-5200.  Case 158 and the
post-switch tail (lines 5227-5486) were outside every previous transposition
sweep, so this pass reconstructed that cluster directly from the Thumb
disassembly.  The state-1 head (`0x138878..0x13898c` original) is already
faithful; the two order divergences and one operator-overload divergence below
were the only mismatches found.

## Original disassembly evidence

### case 158 state 1 tail (original `0x14384e..0x1438d8`)

```
143868  enemies[0]->geometry->setPosition(m_pGeometry4->getPosition())
143876  m_pCamera->setTarget(enemies[0]->geometry)
14388e  player->geometry->rotate(step/2000, step/2000, step/2000)
143892  if (m_nScriptTimerA >= 20000) { ... }
```

Our source emitted `rotate` before `setTarget`.  The original's order is
`setPosition`, `setTarget`, `rotate`, then the timer test.  This is the
faithful order and is semantics-preserving (the camera target and the player
geometry rotation are independent).

### case 158 state 2 (original `0x140414..0x1404c0`)

```
14041c  field_0x24 = 0
140420  m_pLevel->getPlayer()
140424  resetGunDelay()
14042c  setLookAtCam(false)
140436  setTarget(player->geometry)
14043e  setComputerControlled(false)
140446  setFreeze(false)
140452  setVisible(true)
14045c  setSpeed(2.0f)
140466  m_pHud->visible = 1
14046e  m_pRadar->enabled = 1
140474  setVulnerable(true)
14047c  resetCamera(m_pLevel)
140488  lodManager->forceUpdate(delta, false)
140492  m_bCinematicBreak = 0
140498  m_nScriptTimerA/B = 0
1404a2  setCloakingPossible(true)
...
```

Our source had `setTarget(player->geometry)` before `resetCamera`, and
`resetCamera` before `setVulnerable`.  The landed change moves `resetCamera`
before `setTarget` (see below); the full faithful permutation of the whole
state-2 block measures raw +6 / aligned -41 and is not a raw win.

### case 70: `operator*(Vector,float)` vs `operator*(float,Vector)`

The original calls `_ZN11AbyssEngine6AEMathmlERKNS0_6VectorEf` (Vector * float)
at `0x139a66` for `camPos += dir * 3000.0f`.  Our source wrote
`3000.0f * dir`, which clang lowers to `_ZN11AbyssEngine6AEMathmlEfRKNS0_6VectorE`
(float * Vector).  The call multiset over the whole function confirmed the
mismatch: original 29 `Vector*float` / 1 `float*Vector`; ours 26 / 4 (the
remaining three sites are lines 2153, 3182, 3822; 2153 is genuinely
`float*Vector` in the original -- `0x13c2f6` -- the other two are also
`Vector*float` in the original but are not landed here because they measure
raw +1 each).

## Landed changes

```cpp
// case 158 state 1
-                    player->geometry->rotate(step / 2000.0f, step / 2000.0f, step / 2000.0f);
                     m_pCamera->setTarget((*enemies)[0]->geometry);
+                    player->geometry->rotate(step / 2000.0f, step / 2000.0f, step / 2000.0f);

// case 158 state 2
-                        m_pCamera->setTarget(player->geometry);
+                        resetCamera(m_pLevel);
                         ...
-                        resetCamera(m_pLevel);
+                        m_pCamera->setTarget(player->geometry);

// case 70
-                    camPos += 3000.0f * dir;
+                    camPos += dir * 3000.0f;
```

`resetCamera` already sets the camera target to `player->geometry` and then
sets the target/cam offsets; re-issuing `setTarget(player->geometry)` after it
is a no-op on the target and does not touch the offsets.  The rotate/setTarget
swap and the commuted multiplication are exact.

Measured on the real build (`-j2`, NDK r18b `-Oz`, full gold link):

```
raw        51,513 -> 51,513   (tie)
aligned    22,174 -> 22,171   (-3)
size       57,232 / 57,232    (exact)
.text      71,197 (unchanged)
.ARM.extab 577 (size/base exact; md5 6630befd... unchanged)
.ARM.exidx 0, .rodata 0 (unchanged)
.rel.plt   md5 289b2a06... (byte-identical)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Candidate table (official fn_diff metric on the stripped link)

| # | candidate | raw | aligned | size | extab |
|---|---|---:|---:|---:|---:|
| 0 | baseline | 51,513 | 22,174 | 57,232 | 577 |
| 1 | **landed: case-158 rot/setTarget + setTarget/resetCamera + case-70 `dir*3000`** | **51,513** | **22,171** | **57,232** | **577** |
| 2 | case-158 setTarget/resetCamera only | 51,512 | 22,181 | 57,232 | 577 |
| 3 | case-158 rot/setTarget only | 51,514 | 22,165 | 57,232 | 577 |
| 4 | case-70 `dir*3000` only | 51,513 | 22,173 | 57,232 | 577 |
| 5 | case-158 full faithful state-2 order | 51,519 | 22,133 | 57,232 | 577 |
| 6 | rot/setTarget + setTarget/resetCamera | 51,513 | 22,172 | 57,232 | 577 |
| 7 | rot/setTarget + case-70 `dir*3000` | 51,514 | 22,164 | 57,232 | 577 |
| 8 | full faithful state-2 + rot/setTarget + case-70 | 51,520 | 22,123 | 57,232 | 577 |

Candidate 2 is the only raw win (51,512) but costs aligned +7; candidate 1 is
the only size-exact candidate that improves aligned without regressing raw.
The three remaining `float*Vector` sites (lines 3182, 3822) are the same
faithfulness bug but each measures raw +1 on this base, so they are left for a
future pass with a compensating reshape.

## Sweeps run (case 158 + post-switch tail, lines 5227-5486)

* d1-d4 statement transpositions: 145 cells, one raw win (candidate 2).
* d5-d9 statement transpositions: one raw win (candidate 2).
* d1-d6 over lines 5150-5226 (case-157 tail): 124 cells, no win.
* Re-ran d1-d4 and d5-d9 on the landed body: no further raw win.

The parser is `stmts.py` (from w13635); the harness is `scratch/w13645/fast.py`
(exact NDK r18b `-Oz` compile + gold link + GNU strip, then `tools/fn_diff.py`).

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13645/` (`fast.py`, `sweep158.py`,
`diffmap.py`, `callseq.py`, `base.cpp`, `c_*.cpp`, `orig.dis`, `ours.dis`).
