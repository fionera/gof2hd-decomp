# LevelScript::process — wave 191 / w13470: mission-158 park-call order (raw 51,679 -> 51,678)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13470`, branch
`wave191-w13470`, base master `f325e2d87` (raw **51,679**, `.text` 71,811,
`.ARM.extab` 575, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13470`.

Task: state-machine dispatch / register-allocation residual.  A fresh `d=1`
adjacent-statement sweep around the wave-190 body changes (the three regions
touched by w13455/w13480) found one new size-exact cell.

## Landed change

Source lines 2957-2958 (mission-158 state-1 enemy park block):

```cpp
                         Vector ref = (*enemies)[3]->getPosition();
                         m_nScriptTimerA = 0;
                         (*enemies)[1]->setPosition((*enemies)[3]->getPosition() + Vector{-79000.0f, 4900.0f, 7500.0f});
-                        (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
                         (*enemies)[2]->setPosition(ref + Vector{-79000.0f, 4900.0f, 2500.0f});
+                        (*enemies)[0]->setPosition(ref + Vector{-76000.0f, 5000.0f, 5000.0f});
```

The two `setPosition` calls write disjoint `KIPlayer` objects and read only the
local `ref` copy (taken from `(*enemies)[3]->getPosition()` before the block),
so the swap is semantics-preserving.  The first call's
`(*enemies)[3]->getPosition()` is a pure read.

Measured alone on the real build:

```
raw        51,679 -> 51,678  (-1)
aligned    22,003 -> 22,003
size       57,232 / 57,232   (exact)
.text      71,811 -> 71,810  (-1)
.ARM.extab 575 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Sweeps run

* Fresh `d=1` sweep over the case-145 block (source lines 4370-4540, 85
  candidates, exact NDK r18b `-Oz` compile + gold link, 4 workers): no
  size-exact winner.  The case-145 `switch (m_nState)` is a single 6-entry
  `tbh` in ours, while the original splits it into `cmp #1 / cbz #0 / b.w`
  plus a 4-entry `tbh` for states 2..5 (original `0x1393a8`/`0x13cacc`).
  Splitting the source into two switches reproduces the original dispatch
  shape exactly (`cmp r4,#1; beq; cbz r4; b.w` + 4-entry `tbh`) but grows
  the function by 224 B (57,456) and raw to 55,548, so it is not landable.
  Adding `default: break;` to the switch is codegen-neutral.
* Fresh `d=1` sweep over the three wave-190 changed regions (lines
  1240-1300, 2930-2980, 3970-4030): one winner (the landed cell).
* Individual original-order restorations measured and rejected as raw
  regressions: case-145 state-0 `addFireStreaks` before `setScaling`
  (+1), case-145 state-1 `setRocketControl`/`killLiberator` before
  `setRumblePercentage` (+1), `m_pHud->visible` before `m_pRadar->enabled`
  (+1), and the full original state-1 call order (+9).
