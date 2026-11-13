# LevelScript::process — wave 191b / w13676: case-24 state-1 faithful order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13676`, branch
`wave191b-w13676`, base master `b221b3826` (raw **51,534**, aligned **21,862**,
`.text` diffbytes 71,247, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b handoff notes cover the tail half (w13561/w13599), the first-half
d1–d6 transposition sweeps (w13570), case-14/16/24/29/40/41/42/56/64/67/69/70/73
line buckets (w13570), case-92 state 4 (w13631), case-14 state 1 (w13642),
case-158 state 1/2 (w13645/w13667) and case-154 state 4 (w13657).  Case 24 had
only been transposition-swept, never reconstructed from the disassembly.  This
pass takes the **case-24 state-1 wormhole spin-up** block (source L1832-1843,
original `0x13a960`..`0x13a9b2`) and re-derives its statement order from Thumb.

## Original disassembly (case 24 state 1, `0x13a960`..`0x13a9b2`)

```
13a960  landmarks = m_pLevel->getLandmarks()
13a972  (*landmarks)[3]->setVisible(true)
13a982  ((PlayerWormHole*)(*landmarks)[3])->reset(false)
13a98e  ((PlayerWormHole*)(*landmarks)[3])->open()
13a9a2  Globals::sound->play(34, nullptr, nullptr, 0.0f)
13a9a8  m_nScriptTimerA = 0
13a9ac  m_nState = 2
13a9ae  b.w 0x13c690 (state-2 body)
```

Our source emitted `play`, the auto-turret PLT anchor, `timer = 0`, `reset`,
`open`, `state = 2`, then `setVisible(true)` last.  The faithful order is
`setVisible`, `reset`, `open`, `play`, `timer`, `state`.

## Landed change

The state-1 body is reordered to the original statement order.  The PLT-order
anchor (`player->setAutoTurret(player->autoTurretIsEnabled())`, a no-op) moves
from inside the body to immediately before the state-1 `if`, so the first
auto-turret reference still precedes `PlayerWormHole::open` (the original's
first reference is in the case-154 state-0 block, which
MachineBlockPlacement emits before this arm in the original but after it in
ours).  Removing the anchor entirely shrinks the function to 57,216 B and
changes `.rel.plt`; it is a size/PLT anchor, not just a PLT anchor.

```cpp
+                // PLT-order anchor: ... The call is a no-op (it stores the current value back).
+                player->setAutoTurret(player->autoTurretIsEnabled());
                 if (((RadioMessage *) ((*messages)[4]))->isTriggered() && m_nState == 1) {
-                    Globals::sound->play(34, nullptr, nullptr, 0.0f);
-                    // PLT-order anchor: ...
-                    player->setAutoTurret(player->autoTurretIsEnabled());
-                    m_nScriptTimerA = 0;
+                    (*m_pLevel->getLandmarks())[3]->setVisible(true);
                     ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(false);
                     ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->open();
+                    Globals::sound->play(34, nullptr, nullptr, 0.0f);
+                    m_nScriptTimerA = 0;
                     m_nState = 2;
-                    (*m_pLevel->getLandmarks())[3]->setVisible(true);
```

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,534 -> 51,549  (+15)
aligned    21,862 -> 21,674  (-188)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,247 -> 71,262 (+15); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

This is an aligned-weighted win of the kind accepted for case-154 state 4
(w13657: raw +17 / aligned -262) and case-92 state 4 (w13631: raw -4 /
aligned +25).  It also makes the state-1 statement order faithful.

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab | rel.plt |
|---|---:|---:|---:|---:|---|
| baseline | 51,534 | 21,862 | 57,232 | 577 | ok |
| **case-24 s1 faithful, anchor before the `if` (landed)** | **51,549** | **21,674** | **57,232** | **577** | **ok** |
| case-24 s1 faithful, anchor at block top | 51,550 | 21,686 | 57,232 | 577 | ok |
| case-24 s1 faithful, anchor after `setVisible` | 51,547 | 21,862 | 57,232 | 577 | ok |
| case-24 s1 faithful, anchor before `reset` | 51,548 | 21,862 | 57,232 | 577 | ok |
| case-24 s1 faithful, anchor after `open` | 51,550 | 21,653 | 57,232 | 577 | **no** |
| case-24 s1 faithful, anchor at case top | 52,701 | 21,647 | 57,232 | 577 | ok |
| case-24 s1 faithful, no anchor | 54,324 | 21,539 | 57,216 | 577 | **no** |
| case-24 s1 + s0 full faithful | 51,775 | 21,662 | 57,232 | 577 | ok |
| case-24 s0 full faithful alone | 51,759 | 21,838 | 57,232 | 577 | ok |
| case-29 s0 full faithful + up-vector fix | 51,585 | 21,862 | 57,232 | 577 | ok |
| case-29 up-vector scaling fix alone | 51,579 | 21,862 | 57,232 | 577 | ok |

The anchor after `open` is the best aligned number (21,653) but changes
`.rel.plt`; the landed form is the best `.rel.plt`-identical candidate.

## Other clusters measured (not landed)

**case-24 state 0 (L1802-1829, original `0x13e97e`).**  Original order:
`stopShooting`, HUD/radar, `setLookAtCam`, `setVulnerable`, `setRotation`,
`setComputerControlled`, `removeAllGuns`, `setTarget`, `getPosition`,
`setPosition`, `translate`, then the dir/right/up basis-vector frame, the
landmarks[3] move, the `setEnemies(nullptr)` loop, `forceUpdate`,
`m_bCinematicBreak = 1`, then the shared tail sets `m_nState = 1`.  Full
faithful reorder: raw **51,759** (+225), aligned **21,838** (-24).  Combined
with the landed state-1 reorder: 51,775 / 21,662.  The raw cost is too high.

**case-29 state 0 (L1866-1896, original `0x13e1f6`).**  The original computes
`frame = dir*25000 + right*1000 + up*1000 + pos`; our source had the up-vector
`*= 1000` **after** the add (dead multiply) so the up term was unscaled.  The
faithful order is a real semantic fix and shrinks the block by 2 B, but the
block is at `0x13dcxx` in ours vs `0x13e2xx` in the original, so the same-address
raw metric moves +45 and the aligned metric is unchanged.  Full faithful
reorder of the state-0 teardown is likewise raw +51 / aligned 0.  Recorded for
a future pass that can also fix the block placement.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13676/` (`runs.py`, `rangediff.py`,
`callseq.py`, `callhist.py`, `calladdr.py`, `alignmap.py`, `setblock2.py`,
`base.cpp`, `cand_between.cpp`).
