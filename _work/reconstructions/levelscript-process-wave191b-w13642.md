# LevelScript::process — wave 191b / w13642: case-14 state-1 teardown

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13642`, branch
`wave191b-w13642`, base master `cb3afdf0f` (raw **51,513**, aligned **22,816**,
`.text` 71,197, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The handoff notes cover the tail half (w13561), the first-half d1–d6
transposition sweeps (w13570) and the case-102 awake hoist (w13599).  This pass
takes the **case-14 teardown** cluster (source lines 1578–1667, original
`0x13a9e2` state 1 / `0x13c702` state 2 / `0x13ea84` state 0) and the
**mission-0 state-6 timer** cluster (source lines 758–782, original
`0x139b70`), reconstructing the original statement order from the Thumb
disassembly.

All measurements below use the exact NDK r18b `-Oz` compile + full gold link
(`scratch/w13642/fast.py`), then `--strip-all` and `tools/fn_diff.py` so the
`aligned` metric matches the checked-in baseline (the stripped build's branch
targets normalise differently from the unstripped object).

## Original order (case-14 state 1, `0x13a9e2`–`0x13aaf2`)

```
getPosition / zero vector / updateEvent3DAttributes(15,&pos,&zero,false)
setTurretMode(false)
resetCamera(m_pLevel)
setFreeLookMode(false)
enableFirstPersonCam(false)
hideShipForFirstPersonCameraView(false)
m_bCinematicBreak = 1
stopShooting(0)
m_pHud->visible = 0
m_pRadar->enabled = 0
flashScreen(3)
setComputerControlled(true)
setSpeed(0.0f)
removeAllGuns()
setLookAtCam(true)
setTarget(player->geometry)
setPosition(player->getPosition())
translate(1000.0f, 700.0f, 1000.0f)
```

Our source differs in several places; the full permutation to the original
order is a large raw regression (see table), so only the one size-exact,
raw-lowering cell was landed.

## Landed change

case-14 state-1 teardown (source ~1614): move the HUD visibility store ahead of
the camera translate, matching the original's `strb` at `0x13aa44` before the
`translate` at `0x13aa9a`:

```cpp
-                    m_pCamera->translate(1000.0f, 700.0f, 1000.0f);
                     m_pHud->visible = 0;
+                    m_pCamera->translate(1000.0f, 700.0f, 1000.0f);
```

`m_pHud->visible` is a plain byte store; `translate` reads only the camera, so
the swap is semantics-preserving and matches the original statement order.

Measured alone on the real build:

```
raw        51,513 -> 51,512  (-1)
aligned    22,816 -> 22,816  (unchanged)
size       57,232 / 57,232   (exact)
.text      71,197 -> 71,196  (-1)
.ARM.extab 577 (unchanged; size/base exact)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Measured candidates (stripped build, `tools/fn_diff.py`)

| candidate | raw | aligned | size | .text | .ARM.extab | note |
|---|---:|---:|---:|---:|---:|---|
| base | 51,513 | 22,816 | 57,232 | 71,197 | 577 | — |
| **landed: hud before translate** | **51,512** | **22,816** | **57,232** | **71,196** | **577** | **landed** |
| mission-0 s6: timer+=delta moved before rumble | 51,521 | 22,808 | 57,232 | 71,205 | 577 | raw +8, aligned -8 |
| mission-0 s6: `int timer = …; m_nScriptTimerA = timer;` | 51,518 | 22,816 | 57,232 | 71,202 | 577 | raw +5 |
| case-14 s1: full original order | 51,566 | 22,828 | 57,232 | 71,250 | 577 | raw +53, aligned +12 |
| case-14 s2: full original order | 51,518 | 22,816 | 57,232 | 71,202 | 577 | raw +5 |
| case-42 s6: full original order | 53,946 | 22,775 | 57,240 | 399,582 | 1,262 | size +8, extab +685 |
| case-42 s5: timer=0 moved to end | 52,142 | — | 57,232 | 71,826 | 577 | raw +629 |
| case-14 s0: `m_nState=1` hoisted to block top | 51,516 | — | 57,232 | 71,200 | 577 | raw +3 |

## Rejected in the same pass

* case-14 state-1: all 380 single-statement relocations and all 106 statement
  rotations (windows 2–4) were measured.  Only the landed cell and its
  equivalent (`hud` before `setComputerControlled`) reach 51,512; the latter
  measures aligned 22,818 (+2) on the stripped build, so the landed form is
  preferred.  No group-of-2 relocation is a size-exact raw win.
* case-14 state-2 (0x13c702): d1–d6 transpositions, 132 single-statement
  relocations and the full original order all fail to lower raw (best +5).
* case-42 state-6 (0x13c1c6): d1–d6 transpositions and 870 single-statement
  relocations produce no size-exact raw win.  The full original order shrinks
  the function by 88 B, so it needs a compensating +88 B reshape before it can
  land; it is recorded for a future pass.
* mission-0 state-6 (0x139b70): d1–d6 transpositions and the timer moves above
  produce no raw win.  The original fuses the 64-bit timer increment with the
  `/4000.0f` conversion; the current source's early increment is a strong local
  optimum for the same-address byte metric.
* case-14 state-0 (0x13ea84): hoisting `m_nState = 1` to the block top (its
  original position) measures +3.

Scratch harness (`fast.py`, `sweep.py`, `reloc.py`, `greloc.py`, `rot.py`,
`try_*.py`, `secdiff.py`) lives in
`/home/debian/GalaxyOnFire2/scratch/w13642/`.
