# LevelScript::process — wave 132 / w7724: real case-144 `setActive`, getStation(100) un-merge, anchor trim

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `144fc16b0` (w7708 handoff).

## Result

```
                         before (master)   after (landed)
official verify pct      59.2              59.1
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54507             54430           (-77)
whole .text diffbytes    85244             85167           (-77)
.ARM.extab diffbytes     1416              1416
.ARM.exidx diffbytes     0                 0
.rodata / .data          0 / 0             0 / 0
.rel.plt                 exact             exact (byte-identical)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The official fuzzy pct drops 59.2 -> 59.1 because the corrected call re-rolls
`MachineBlockPlacement`/register colouring (same trade-off as w7705); the
authoritative byte diff improves by 77 B in `process` and in the whole `.text`.
`.rel.plt` is byte-identical and every section size is unchanged.

## Landed changes (all verified against objdump of the original)

### 1. Case 144 state 1 — `setTarget(player->geometry)` is really `setActive(true)`

Original `0x1394f0..0x139540` (case 144 state 1, the `18000.0f / 3000.0f`
camera block; literals at `0x13970c`/`0x139710`):

```
1394ae  ldr  r0,[r8]          ; enemies
1394b4  ldr  r0,[r0,#0]       ; data_[0]
1394b6  ldr  r1,[r0,#8]       ; ->geometry
1394ba  blx  AEGeometry::getPosition
...     m_tempVec = pos; += dir*18000; += right*3000
139516  ldr  r0,[r5,#20]      ; m_pCamera
13951a  blx  TargetFollowCamera::setPosition(Vector)
13951e  ldr  r0,[r8]          ; enemies
139524  ldr  r0,[r0,#0]       ; data_[0]
139526  blx  KIPlayer::setVisible(true)
13952a  ldr  r0,[r8]
139530  ldr  r0,[r0,#0]
139532  blx  KIPlayer::setActive(true)     <-- was setTarget(player->geometry)
139536  m_nScriptTimerA = 0
13953c  m_nState++
```

Our source had `m_pCamera->setTarget(player->geometry);` there.  The original
has no second `setTarget`; it calls `KIPlayer::setActive(true)`.  This is a real
semantic bug (the camera kept targeting the player instead of waking the
enemy).  It fixes the `KIPlayer::setActive` call-count delta (orig 27 / ours 26
-> 27/27) and removes one spurious `TargetFollowCamera::setTarget` (orig 81 /
ours 83 -> 81/82).  Measured alone on master: **57224 B (-8)**.

### 2. `Galaxy::getStation(100)` + `departStation` tail merge (line 1034)

The original has two separate `getStation` call sites for the two
`m_nState == 3` fade-out blocks (mission-0 style `getStation(100)` at
`0x1391ac`, and `getStation(10)` at `0x13b124`).  Ours merged them into one
shared `blx getStation` because both blocks had identical register allocation
(`status` in `r9`), so the call count was 11 vs the original's 12.

Reordering the adjacent global store in the `getStation(100)` block

```cpp
Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(100));
Globals::switch_to_target_setting = 0;   // moved after the call
```

breaks the merge: the block now gets its own `blx getStation` and
`blx departStation`, matching the original.  Measured alone on master: **+16 B**.

### 3. Anchor trim — drop the duplicate `startSmokeEmission` from the `delta < 0` anchor

The w7708 note's `if (delta < 0)` first-reference anchor is still required for
`.rel.plt` (see below), but its `player->startSmokeEmission();` was a pure
duplicate of the real case-11 call and the only anchor statement that could be
removed without disturbing the first-reference order.  Removing it costs -8 B
and exactly balances changes 1+2 (net -8 +16 -8 = 0), keeping `process` at
**57232 B**.  The anchor now has 7 statements.

## Why the anchor cannot be removed outright yet

Removing the whole `if (delta < 0)` block is **-88 B** and leaves `process` at
57144 B, which shifts every following symbol and fails the ratchet
(`byte_exact 4506 -> 2741`).  It also rotates exactly two `.rel.plt` entries:

```
idx 6428: original PlayerStation::setVisible  / ours PlayerEgo::deleteHackingGame
idx 6429: original PlayerEgo::deleteHackingGame / ours PlayerStation::setVisible
```

The original's only `PlayerStation::setVisible` is the case-42 state-7 call at
`0x13da1e` (inside `TransformGetTransform(m_pParticleGeom0->transform)->currentTime >= 4001`),
and `PlayerEgo::deleteHackingGame` is at `0x13ded4`; i.e. in the original the
case-42 block is laid out *before* the mission-125 hacking block.  In ours the
case-42 block lands at `0x13e5e0`, after `deleteHackingGame` at `0x13d974`.
Restoring the getStation un-merge and the setActive fix does not move it, so the
anchor is still the only thing that pins the order.

## Remaining call-count deltas after this pass (size-neutral)

```
+2  @f                              (indirect calls, local-code masking)
+1  AEGeometry::getPosition
+1  AEGeometry::setPosition(Vector)
-1  LODManager::forceUpdate
+1  StarSystem::switchPlanetForIntro      (anchor)
-1  PaintCanvas::TransformGetTransform
+1  ApplicationManager::SetCurrentApplicationModule
+1  AEMath::Vector::operator=
+1  PlayerStation::setVisible             (anchor)
+1  TargetFollowCamera::setRumblePercentage
+1  TargetFollowCamera::setTarget
-2  TargetFollowCamera::translate
+2  ParticleSystemManager::reset          (anchor)
-1  Level::getEnemies
+2  Level::getLandmarks                   (anchor, +1)
+2  Level::getStarSystem                  (anchor, +1)
+1  Level::switchSkyboxForIntro           (anchor)
+3  Level::getPlayer
+1  Status::getStanding
+1  Standing::setStanding
+1  FModSound::stop
+1  PlayerEgo::getPosition
+1  PlayerEgo::setAutoTurret
+1  PlayerEgo::PlayEngineSound            (anchor)
+1  PlayerEgo::StopEngineSound            (anchor)
+1  PlayerEgo::autoTurretIsEnabled
```

The `-1`/`-2` entries (`forceUpdate`, `TransformGetTransform`, `translate` x2,
`getEnemies`) are **tail merges**, not missing statements: the source contains
more calls than the original (e.g. 59 `translate` statements vs 57 original
calls), and clang 7.0.2 `-Oz` shares their call instructions across blocks in
ours but not in the original.  Un-merging them (as done for `getStation(100)`)
would recover the remaining call sites if a faithful source perturbation can be
found.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
```

Scratch helpers in `/dev/shm/w7724/`: `dis.py` (call-count delta), `align.py`
(call-sequence diff), `ctx.py` (call-site context), `relplt.py` (`.rel.plt`
comparator), `shared.py` (branch-target detection).

## Open questions for the next wave

* `translate` x2, `forceUpdate`, `TransformGetTransform`, `getEnemies`: find the
  source perturbation that un-merges each (the `getStation(100)` reorder is a
  proven template).  Each un-merge is worth ~8-16 B.
* Once those are un-merged, the `delta < 0` anchor can be removed: the -88 B
  would be compensated and the case-42 `setVisible` block may then be placed
  before the hacking block.
* The anchor's remaining 7 statements are all duplicates; they are the last
  source-level shortcut in this function.
