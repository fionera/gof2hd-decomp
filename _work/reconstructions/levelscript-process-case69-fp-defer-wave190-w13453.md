# LevelScript::process — wave 190 / w13453: case-69 first-person defer

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13453`, branch
`wave190-w13453`, base master `83d52fe52`.  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13453`.

Continues the w13418/w13452 adjacent-statement sweeps.  A fresh `d=6`
rotation sweep of the current 51,699 body (size-filtered) surfaced the
case-69 cutscene-exit cell; it is size-exact and `.rel.plt`-clean.

## Result

```
                         base (HEAD)      this pass
process raw diffbytes    51,699           51,698     (-1)
process aligned insns    21,923           21,923     (unchanged)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          72,255           72,254     (-1)
.ARM.extab diffbytes     584              584        (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
file size                2,187,328        2,187,328  (exact)
RATCHET                  PASS             PASS
```

## Landed change

Case-69 (mission-125 intro teardown) state-0 block, source lines 2441/2446:

```cpp
 m_pCamera->setTarget((*m_pLevel->getEnemies())[0]->geometry);
-m_pCamera->enableFirstPersonCam(false);
 m_pCamera->setLookAtCam(true);
 m_pCamera->setActive(true);
 Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
 Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
 camPos += dir * 10000.0f;
+m_pCamera->enableFirstPersonCam(false);
 camPos.x += 600.0f;
```

`TargetFollowCamera::enableFirstPersonCam(bool)` only writes `firstPerson`,
`fpOffset`, `shakeAccum` and `shakeReference`; the intervening
`setLookAtCam`/`setActive` writes and the `getPosition`/`getDirection` reads
do not observe those fields, so the reorder is semantics-preserving.  The
emitted stream changes only in scheduling/register pressure: raw
**51,699 -> 51,698** (-1), size 57,232/57,232 exact, `.text` 72,255 ->
72,254, `.rel.plt` md5 unchanged.

## Sweep notes

The full `d=4..8` both-direction rotation sweeps of the current body found
no other size-exact `.rel.plt`-clean winner.  `d=8` (1,022 candidates) best
was 51,699 (equal to the base).  The `d=6` cell above is the only hit that
survives the size filter (`0xdf90`) and the `.rel.plt` check.
