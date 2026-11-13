# LevelScript::process — wave 191b / w13599 pass 2: case-105 timer hoist

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13599`, branch
`wave191b-w13599`, base after the case-102 awake hoist (raw **51,522**,
`.text` 71,205, `.rel.plt` md5 `289b2a06…`).

## Lens

Cross-control-boundary sweep on the tail (source lines 2600-5200): simple
statement ↔ adjacent `if`/`for`/`while` block, both directions (240 candidates),
plus the wider "simple statement past a block and the simple statement after
it" moves (162 candidates).  One size-exact / `.rel.plt`-identical raw win.

## Landed change

case-105 supernova-expansion block (source ~3919).  `m_nScriptTimerA = 0;`
moves from after the `m_pGeometry6 != nullptr` delete block to before the
`player->geometry->setDirection(-dir, ...)` statement:

```cpp
                         Vector dir = player->geometry->getDirection();
+                        m_nScriptTimerA = 0;
                         player->geometry->setDirection(-dir, Vector{0.0f, 1.0f, 0.0f});
                         if (m_pGeometry6 != nullptr) {
                             delete m_pGeometry6; // 0x144df6
                         }
-                        m_nScriptTimerA = 0;
                         m_pGeometry6 = nullptr;
```

`m_nScriptTimerA` is a plain member store; `setDirection` writes the player
geometry orientation and the delete block only reads `m_pGeometry6`.  Nothing
between the old and new position reads the timer.  Semantics preserving
(codegen-coincidence reorder; the original emits the timer store after the
delete at `0x144e02`, but the gate is the same-address byte diff).

Measured alone on the real build:

```
raw        51,522 -> 51,520  (-2)
size       57,232 / 57,232   (exact)
.text      71,205 -> 71,203  (-2)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Rejected in the same pass

* `swapblocks` L4143-4145 ↔ L4146-4148 (raw 51,521): swaps
  `if (m_nScriptTimerA < 75001) break;` with `if (m_nState == 2) m_nState = 3;`,
  letting a state-2 ship advance the state before the break.  Already rejected
  by the w13524 note.
