# LevelScript::process — wave 132 / w7723 (case-70 body + case-154/64 subset landed)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `144fc16b0` (landed w7705 semantic set).

## Result

```
                         before (master)   after (landed)
official verify pct      59.2              59.2
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54507             54537
whole .text diffbytes    85244             85274
.ARM.extab diffbytes     1416              1416
.rel.plt                 exact             exact
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The fuzzy/diff-byte metric moves +30 because the corrected cold blocks re-roll
`MachineBlockPlacement`; the gate counts are unchanged.  The case-70 body alone
(commit `cf7ad5d72`) is -6 diffbytes; the case-154/64 subset below is the +36
companion that keeps the function at exactly 57232 B.

## Landed set A — case 70 state 2 (orig `0x13ec3c`..`0x13ece8`)

The state-2 body called `setRotate(5)` **before** the `isOver()` early-out (so it ran every
frame while state 2 was active) and put the `forceUpdate` at the bottom. Original:

```
13ec3c  forceUpdate(delta,false)                 <-- top of the state body
13ec4c  if (messages[2]->isOver()) {
13ec5c    setLookAtCam(false) ... resetCamera ... forceUpdate
13eca0    m_bCinematicBreak = 0
13eca8    m_nState = 3
13ecac    lead = enemies[0]->player
13ecb2    lead->setMaxHitpoints(enemies[0]->player->getMaxHitpoints()*3)
13ecd2    enemies[0]->setSpeed(10.0f)
13ece2    enemies[0]->setRotate(5)              <-- LAST statement, inside the if
13ece8  }
```

Source now matches that order exactly. Size-neutral; `.rel.plt` unchanged.

## Landed set B — case 154 state 4 (creep + tail) and state 6 (creep), case 64 partial

### case 154 state 4 creep, orig `0x143138`

The original is the standard camera-creep sequence, *not* the dead-assignment pattern:

```
1431a6  m_dirVec = player->GetDirVector()
1431b4  m_tempVec += m_dirVec * (step * 0.008)
1431e6  m_dirVec = player->geometry->getRightVector()
1431f0  m_tempVec += m_dirVec * (step * -0.05)
143210  m_dirVec = player->GetUpVector()
14321a  m_tempVec += m_dirVec * (step * 0.005)
```

Our source applied the stale `m_dirVec` twice and then did three dead assignments.
Fixed to the original order.

### case 154 state 4 tail, orig `0x144658`..`0x1446d6`

Original:

```
player->setSpeed(0.0f)
m_pCamera->setFixed(false)
m_pCamera->setTarget(enemies[1]->geometry)      <-- was missing
m_tempVec = enemies[1]->geometry->getPosition()
m_tempVec += {0, 4000, 42000}
m_pCamera->setPosition(m_tempVec)
m_nScriptTimerA = 0
m_nState++                                       <-- was missing (state never advanced)
```

Our source had a duplicate `m_nScriptTimerA = 0` before the `getPosition` and no state
advance at all (the state machine would stall at 4).  Fixed.

### case 154 state 6 creep, orig `0x14330e`

Same dead-assignment bug as state 4: original is
`GetDirVector -> +=dir*0.008 -> getRightVector -> +=right*0.05 -> GetUpVector -> +=up*0.005`;
our source used the stale vector for the first two terms and applied the up vector twice.
Fixed.

### case 64 state 1, orig `0x13ecfc` (partial)

Original body:

```
forceUpdate(delta,false)
translate(0, delta*0.2, 0)            (double math: vcvt.f64.s32 / vmul.f64 / vcvt.f32.f64)
if (messages[2]->isTriggered()) {
  setLookAtCam(false) ... resetCamera ... forceUpdate
  m_bCinematicBreak = 0
  player->freeze = 0
  for (i) enemies[i]->setRoute(nullptr)
  m_nState = 2
}
```

Our source had a spurious `m_pCamera->setRumblePercentage(100.0f, 30)` and
`Globals::sound->stop(156)` and a duplicate `player->freeze = 0` at the top, and no
`m_bCinematicBreak = 0`.  This pass removes the three spurious statements and adds
`m_bCinematicBreak = 0`.  The two missing leading calls (`forceUpdate`, `translate`) are
**not** added yet: the full faithful body measures +24 B on this base and no exact-size
companion was found in the time box (see below).

## Measured but NOT landed (need size companions)

All measured on the landed w7705 base + set A; each keeps `.rel.plt` identical.

| edit | size | delta | process diffbytes |
|---|---:|---:|---:|
| case 154 state 2 creep (dead `GetDirVector`) | 57228 | -4 | 55103 |
| case 154 state 2 tail (spurious break/dup setTarget, missing setSpeed/timer) | 57216 | -16 | 55125 |
| case 154 state 2 creep + tail | 57216 | -16 | 55127 |
| case 154 state 4 creep | 57240 | +8 | 55101 |
| case 154 state 6 creep | 57244 | +12 | 55083 |
| case 64 full (adds forceUpdate + translate) | 57256 | +24 | 55049 |
| case 64 partial (landed) | 57192 | -40 | 55194 |
| case 40 state 1 missing `translate(0,0,-2*delta)` | +24 | | |

A 128-combination sweep over {s2, s2t, s4, s4t, s6, c64, c64p} found exactly two
size-57232 / `.rel.plt`-exact combinations: the empty set (case-70 only, 54501 diffbytes)
and `s4 + s4t + s6 + c64p` (54537), which is the set landed here.  The case-154 state-2
creep/tail fixes are genuine but do not fit any exact-size combination yet.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
```

Scratch helpers in `/dev/shm/w7723/` (not tracked): `measure.py` (process size /
diffbytes / `.rel.plt` compare), `edits.py` (edit set), `sweep.py` (subset search),
`orig.dis`.
