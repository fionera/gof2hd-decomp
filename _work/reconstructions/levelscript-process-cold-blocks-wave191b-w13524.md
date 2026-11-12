# LevelScript::process — wave 191b / w13524: cross-control-boundary sweep rerun

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13524`, branch
`wave191b-w13524`, base master `9fdef1695` (raw **51,613**, aligned 22,143,
`.text` 71,378, `.ARM.extab` 577, `.rel.plt` md5 `289b2a06…`).

## Lens

Re-ran the w13517 cross-control-boundary sweep (simple statement ↔ adjacent
`if`/`for`/`while`/`switch` block, both directions, 471 candidates, exact NDK
r18b `-Oz` compile + gold link) on the post-w13517 base.  The w13517 landings
shifted register allocation, so cells that were neutral on its base are now
wins.  Three cells are size-exact / `.rel.plt`-identical raw wins; two were
landed (the third, cell 385, crosses a value the block reads — see below).

## Landed changes (semantics-preserving: the moved statement touches no state
## read or written by the block it crosses)

| # | source (new order) | move | raw |
|---:|---|---|---:|
| 1 | case-154 (~4546) | `(*enemies)[1]->field_0x70 = 1;` before the `shipGroup == 9` `setDirection`/`setAIDisabled` loop (loop touches neither field_0x70 nor a KIPlayer byte at 0x70) | -4 |
| 2 | case-146 (~3079) | `m_pCamera->setFixed(false);` before the 3-ship `setAIDisabled`/`field_0x38`/`field_0x128` loop | -1 |

Rejected cell 385 (raw -1): it would move `field_0xaa = player->autoTurretIsEnabled();`
*after* the `if (field_0xaa) setAutoTurret(false);` that reads it.  The original
disasm (`0x13a7fc`–`0x13a80c`) stores `field_0xaa` from the call result and
branches on that same value, so the move is a codegen coincidence that changes
behaviour.

## Measured result

```
                         base (9fdef1695)   this pass
process raw diffbytes    51,613             51,608     (-5)
orig / ours size         57232 / 57232      57232 / 57232  (exact)
.text diffbytes          71,378             71,373     (-5)
.ARM.extab diffbytes     577                577        (size/base exact)
.ARM.exidx diffbytes     0                  0          (byte-identical)
.rodata diffbytes        485                485
.rel.plt                 md5 289b2a06…      md5 289b2a06…  (byte-identical)
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN ✓            CLEAN ✓
drift                    0                  0
symbol parity            5370/5370          5370/5370
RATCHET                  PASS               PASS
```

Scratch harness (`parser.py`, `sweep.py`, `harness.py`, `apply.py`,
`measure_w11901.py`) lives in `/home/debian/GalaxyOnFire2/scratch/w13524/`.

## Pass 2 — wider moves (simple↔ctrl↔simple, ctrl↔ctrl)

Generalised the sweep beyond immediate adjacency: (A) move a simple statement
past a control block *and* the simple statement after it, (C) past a control
and two statements, (D) swap two adjacent control blocks.  264 candidates,
five size-exact / `.rel.plt`-identical raw wins; three landed.

| # | source (new order) | move | raw |
|---:|---|---|---:|
| 3 | case-140 (~4058) | `setAutoPilot(nullptr)` after `resetGunDelay()` (both sides of the `!isDockedToDockingPoint()` if; neither the if nor resetGunDelay reads autopilot) | -1 |
| 4 | case-145 (~4258) | swap the two independent `if (m_nState == 1)` body blocks (escort `turnEnemy` loop ↔ docking-hack conclusion) | -1 |
| 5 | case-154 (~4767) | `m_nScriptTimerA = 0;` before the `if (field_0xaa)` auto-turret restore; `m_bCinematicBreak = 0;` after it (flag not read by the if) | -1 |

Rejected: cell 132 (`if (leader == -1) leader = i;` must precede the
`i == leader` dispatch — first group-10 ship would take the wrong branch) and
cell 180 (`if (timer < 75001) break;` must precede `if (m_nState == 2) m_nState = 3;`
or a state-2 ship would advance early).

```
process raw diffbytes    51,608             51,605     (-3; total -8 vs wave base)
.text diffbytes          71,373             71,370     (-3)
size / .rel.plt / gate   exact / identical / RATCHET PASS
```

## Pass 3 — adjacency rerun ripple

Re-ran the immediate-adjacency sweep on the pass-2 base: one new cell.

| # | source (new order) | move | raw |
|---:|---|---|---:|
| 6 | case-146 (~3080) | `m_pCamera->setTarget(player->geometry);` before the 3-ship re-enable loop (follows the pass-1 `setFixed(false)` move) | -1 |

```
process raw diffbytes    51,605             51,604     (-1; total -9 vs wave base)
.text diffbytes          71,370             71,369     (-1)
size / .rel.plt / gate   exact / identical / RATCHET PASS
```
