# LevelScript::process — wave 191b / w13701: case-105 state-4 geometry reload

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13701`, branch
`wave191b-w13701`, base master `21aadcc74` (raw **51,611**, aligned **21,601**,
`.text` diffbytes 71,339, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

The wave-191b notes cover the tail half (w13561/w13599), the first-half
transposition sweeps (w13570), case-92 s4 (w13631), case-14 s1 (w13642),
case-158 s1/s2 (w13645/w13667), case-154 s4 (w13657/w13668), case-24 s1
(w13676) and case-105 s6 (w13687).  Case 105 **state 4** (source L3649-3653,
original `0x142138`..`0x1421aa`) had never been reconstructed from Thumb and is
one of the few residual blocks that sits at the **same address** in both builds,
so a local change there moves the raw metric directly.

## Original disassembly (case 105 state 4, `0x142138`..`0x1421aa`)

```
142138  m_nScriptTimerA += delta                    (64-bit, [this+0x90])
142152  dir = player->geometry->getDirection()      (r6 = [sp,#84] player, r1 = [r6,#8])
14215c  t1  = dir * 6.08f                           (pool @0x142324)
14216e  right = player->geometry->getRightVector()  (reloads [r6,#8])
142178  t2  = right * 0.2f                          (pool @0x142328)
142182  t3  = t1 + t2                               (AEMath::operator+)
142192  t4  = t3 * 0.4f                             (pool @0x14232c)
14219c  t5  = t4 * (float) delta
1421ae  t6  = t5 * 0.1f                             (pool @0x142334)
1421da  m_pCamera->translate(t6.x, t6.y, t6.z)
1421de  for i in 0..2: (*enemies)[i]->field_0x12c = 1
1421f6  if (m_nScriptTimerA < 3000) goto tail
        ... teardown (resetGunDelay / setComputerControlled / ... / m_nState=5)
```

The block is semantically exactly our source.  The one source-visible
difference is that the original **reloads `player->geometry`** for the
`getRightVector()` call (it does not keep one cached geometry pointer across
both calls), and it schedules the first geometry load *before* the
`m_nScriptTimerA += delta` 64-bit add.  Our source cached
`AEGeometry *pgeom3630 = player->geometry;` and used it for both calls.

## Landed change

```cpp
                 } else if (m_nState == 4) {
-                    AEGeometry *pgeom3630 = player->geometry;
+                    AEGeometry *g = player->geometry;
                     m_nScriptTimerA += delta;
-                    Vector drift = (pgeom3630->getDirection() * 6.08f
-                                    + pgeom3630->getRightVector() * 0.2f) * 0.4f * (float) delta;
+                    Vector drift = (g->getDirection() * 6.08f
+                                    + player->geometry->getRightVector() * 0.2f) * 0.4f * (float) delta;
                     m_pCamera->translate(drift.x * 0.1f, drift.y * 0.1f, drift.z * 0.1f);
```

The right-vector term now re-reads `player->geometry` exactly as the original
does; the single local (declared before the timer add) lets clang schedule the
first geometry load at the original's position.  Semantics are unchanged (the
geometry pointer is loop-invariant here).  The resulting block is
instruction-for-instruction identical to the original modulo register names,
and now starts at `0x142140` (delta +4) instead of `0x142138`.

Measured on the real build (exact NDK r18b `-Oz` compile + full gold link +
`--strip-all`, `tools/fn_diff.py`):

```
raw        51,611 -> 51,608  (-3)
aligned    21,601 -> 21,561  (-40)
size       57,232 / 57,232   (exact)
.text      diffbytes 71,339 -> 71,336 (-3); size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

Both residual metrics improve, so no trade-off is involved.

## Candidate table (all size-exact unless noted)

| candidate | raw | aligned | size | extab | rel.plt |
|---|---:|---:|---:|---:|---|
| baseline | 51,611 | 21,601 | 57,232 | 577 | ok |
| **s4: local for dir, reload for right (landed)** | **51,608** | **21,561** | **57,232** | **577** | **ok** |
| s4: no local at all (both `player->geometry`) | 51,621 | 21,555 | 57,232 | 577 | ok |
| s4: local declared after the timer add | 51,621 | 21,555 | 57,232 | 577 | ok |
| s4: local for right only | 51,611 | 21,601 | 57,232 | 577 | ok |
| s4: two locals (g1/g2) | 51,611 | 21,601 | 57,232 | 577 | ok |
| s4: named `dir`/`right` temporaries | 51,626 | 21,593 | 57,232 | 577 | ok |
| s4: `Vector d2 = drift * 0.1f` then translate | 52,975 | 21,576 | 57,208 | 577 | ok |
| s4: local + `Vector dir = g->getDirection()` | 51,622 | 21,563 | 57,232 | 577 | ok |
| s3: drop cached `geom` local (same trick) | 53,777 | 21,561 | 57,232 | 577 | ok |
| s9: drop cached `geo` local | 52,325 | 21,570 | 57,252 | 577 | ok |
| case-102 s0 faithful reorder alone | 51,620 | 21,596 | 57,232 | 577 | ok |
| s4 (landed) + case-102 s0 faithful reorder | 51,617 | 21,556 | 57,232 | 577 | ok |

Case-105 state 3 has the same cached-geometry shape but removing its local
makes the block 4 B longer and costs +2,166 raw; state 9's `geo` local is also
load-bearing (removing it grows the function).  The case-102 state-0 block is
statement-order divergent from the original (original order: `setTurretMode`,
`resetCamera`, rocket block, `setFreeLookMode`, `enableFirstPersonCam`,
`hideShip`, `stopShooting`, `setFreeze`, `setVisible`, `setVulnerable`,
`m_bCinematicBreak`, HUD, radar, `setLookAtCam`, `setTarget`, `setPosition`,
sound `stop` then `play`), but its faithful reorder is only aligned -5 / raw +9
and does not compose with this win (raw would go positive).

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13701/` (`map.py`, `side.py`, `sweep.py`,
`sweep4.py`, `sweep4b.py`, `sweep9.py`, `base_LevelScript.cpp`, `candB.cpp`,
`orig.dis`, `ours.dis`).
