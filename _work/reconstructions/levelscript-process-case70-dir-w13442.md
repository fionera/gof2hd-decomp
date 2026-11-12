# LevelScript::process — wave 190 / w13442: case-70 `dir` declaration sink (raw 51,708 -> 51,706)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13442`, branch
`wave190-w13442`, base master `f76...` (raw **51,708**, `.text` 72,843,
`.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`).

Lens: declaration-order sweep (all declaration statements moved to every
sibling position inside their block, wave-181 filter).  854 candidates; one
size-exact, `.rel.plt`-identical raw win landed.

## Landed change

Source `case 70` (mission 70, original `0x1399a2`), lines 2498-2503:

```cpp
-                    Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
                     Vector camPos = (*m_pLevel->getEnemies())[0]->getPosition();
                     camPos.x += -600.0f;
                     camPos.y += -300.0f;
+                    Vector dir = (*m_pLevel->getEnemies())[0]->geometry->getDirection();
                     camPos.z += -1000.0f;
                     camPos += 3000.0f * dir;
```

`dir` is a pure getter result and is only consumed by `camPos += 3000.0f * dir`
two statements later, so sinking it past the independent `camPos.x`/`camPos.y`
scalar adds is semantics-preserving.  It also moves the getter closer to the
original's emission order (the original computes `camPos = getPosition()`
before `dir = getDirection()`).

Measured alone on the real build:

```
raw        51,708 -> 51,706  (-2)
aligned    21,914 -> 21,920
size       57,232 / 57,232   (exact)
.text      72,843 -> 72,841  (-2)
.ARM.extab 597 (unchanged)
.rel.plt   md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
```

`check` -> RATCHET PASS (byte_exact 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).

## Sweep context (negative results)

Full single-statement rotation sweeps `d=1..28` on the 51,708 body (14,000+
candidates, exact NDK r18b `-Oz` compile + gold link) found only the five
already-documented `-1` cells: `down 828,829` (`.rel.plt`-fatal) and the four
`m_tempVec` read-after-write rotations (`down 889,890`, `down 3735,3736`,
`down 4542,4543`, `down 4963,4964`), which are semantically unsafe.

The 854-candidate declaration sweep (wave-181 filter: move each declaration to
every sibling position, reject only on declared-name use / RNG order) found
exactly two cells below the base: `down 2497,2500` (51,706, landed) and
`down 2497,2501` (51,707).

## Structural observations (not landed)

* `case 70` full reorder to the original's exact emission order
  (`setTurretMode, resetCamera, setFreeLookMode, enableFirstPersonCam,
  hideShip, stopShooting, cinematic, hud, radar, computerControlled,
  lookAtCam, setActive, setTarget, camPos, dir, camPos += dir*3000, offsets,
  setPosition, forceUpdate, freeze, state=1`) is size-exact but **+36 raw**
  (51,744).  The original also uses `AEMath::mlERKNS0_6VectorEf`
  (`Vector * float`) where our source emits `mlEfRKNS0_6VectorE`
  (`float * Vector`), i.e. the original is `dir * 3000.0f`, but flipping the
  operand alone does not compose to a win.
* The first literal pool is still `+0x1c` (ours `0x3a8`, original `0x38c`):
  the second wingman-reset loop body is emitted before the pool in ours and
  after it in the original.  `__builtin_expect` on the second loop's
  `getWingmen()!=0` / `isWingMan()` guards, `while`/`for`/`do` spellings and
  `if`-inversion all change size or are neutral.
* The prologue register assignment still differs (`this`=`fp` vs original
  `sl`; `&__stack_chk_guard`=`sl` vs `fp`; `&Globals::status`=`r9` vs `r4`);
  all 24 top-level declaration permutations are size-exact but none beats the
  base (best 51,712).
