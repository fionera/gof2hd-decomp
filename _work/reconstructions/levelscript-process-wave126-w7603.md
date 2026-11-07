# LevelScript::process — wave 126 / w7603 (case-154 state-0/1 faithful reorder)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.261639475      60.235270711     (-0.026368764)
official verify pct      60.3              60.2             (rounded)
orig / ours insns        20169 / 20039     20169 / 20040
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
whole .text diffbytes    85552             85496            (-56)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.ARM.exidx diffbytes     0                 0                (byte-identical)
.ARM.extab size          156312            156312           (size-exact)
.rel.plt diffbytes       0                 0                (byte-identical)
.rodata diffbytes        0                 0                (byte-identical)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (helper `/dev/shm/w7603/score.py`).
The fuzzy score dips 0.026 (branch-offset re-encoding) but the raw `.text` byte
delta improves by 56 B and the case-154 block is now instruction-for-instruction
faithful.

## Landed change (case 154, `m_nState == 0` / `== 1`)

`case 154` is the asteroid-station pirate-ambush camera ride.  The original's
state-0 body is one straight-line block that ends with `m_nState++` and falls
through into the state-1 body; our source had the state-1 prologue duplicated
into state 0 and `m_nState = 1` stored at the top.  Five local corrections:

```diff
                     player->stopShooting(0);
-                    m_nState = 1;
+                    player->setComputerControlled(true);
                     player->player->setVulnerable(false);
-                    field_0xaa = player->autoTurretIsEnabled();
                     player->setSpeed(0.0f);
+                    field_0xaa = player->autoTurretIsEnabled();
                     if (field_0xaa) {
                         player->setAutoTurret(false);
                     }
                     m_bCinematicBreak = 1;
                     m_pHud->visible = 0;
                     m_pRadar->enabled = 0;
                     m_pCamera->setLookAtCam(true);
-                    player->setSpeed(0.0f);
+                    m_pCamera->setTarget((*m_pLevel->getEnemies())[2]->geometry);
                     m_tempVec = (*m_pLevel->getEnemies())[2]->geometry->getPosition();
                     ...
                     (*enemies)[1]->field_0x70 = 1;
                     m_nScriptTimerA = 0;
-                    player->setComputerControlled(true);
+                    m_nState++;
                     m_pLevel->lodManager->forceUpdate(delta, false);
                 } else if (m_nState == 1) {
-                    m_pCamera->setTarget((*m_pLevel->getEnemies())[2]->geometry);
+                    player->setSpeed(0.0f);
                     m_pLevel->lodManager->forceUpdate(delta, false);
```

### Evidence (original disassembly)

Original state-0 block `0x13a788..0x13a928`:

```
13a7e0  mov r0,r5
13a7e2  movs r1,#1
13a7e4  movs r4,#1
13a7e6  blx  PlayerEgo::setComputerControlled(bool)   ; before setVulnerable
13a7ea  ldr r0,[r5,#0]
13a7ee  blx  Player::setVulnerable(bool)
13a7f2  mov r0,r5
13a7f6  blx  PlayerEgo::setSpeed(float)               ; ONE setSpeed only
13a7fa  mov r0,r5
13a7fc  blx  PlayerEgo::autoTurretIsEnabled()
...
13a828  blx  TargetFollowCamera::setLookAtCam(bool)
13a82c  ldrd r4,r0,[r5,#20]
13a830  blx  Level::getEnemies()                      ; setTarget target
13a842  blx  Level::getEnemies()                      ; getPosition target
...
13a912  ldr r0,[sp,#88]
13a916  ldrd r0,r1,[r0,#24]
13a91a  strd r2,r2,[r3,#144]                          ; m_nScriptTimerA = 0
13a91e  adds r1,#1
13a920  str  r1,[r3,#28]                              ; m_nState++
13a928  blx  LODManager::forceUpdate(int,bool)
```

The state-1 body is the separate `tbh` case at `0x13e908`, which starts with
`PlayerEgo::setSpeed(0.0f)` + `LODManager::forceUpdate` (no `setTarget`):

```
13e908  ldr r0,[sp,#84]
13e90a  movs r1,#0
13e90e  blx  PlayerEgo::setSpeed(float)
13e912  ldr r0,[sp,#80]
13e91e  blx  LODManager::forceUpdate(int,bool)
13e922  ... step = delta * 0.5f; for (...) geometry->moveForward(step)
```

The resulting ours block at `0x13c7d0` now matches the original call sequence
exactly: `setTurretMode, setFreeLookMode, enableFirstPersonCam,
hideShipForFirstPersonCameraView, stopShooting, setComputerControlled,
setVulnerable, setSpeed, autoTurretIsEnabled, [setAutoTurret], m_bCinematicBreak,
m_pHud, m_pRadar, setLookAtCam, setTarget, getEnemies/getPosition`.

## Other content findings measured this pass (not landed)

* **`m_pGeometry2->setVisible(false)` at `0x145440`** (case 157 state 9): the
  original calls `AEGeometry::setVisible` on `m_pGeometry2` (`[sl,#0xc0]`), our
  source calls `(*enemies)[stationIndex]->setVisible(false)` (`KIPlayer`).  The
  fix is faithful but **-4 B** (57228) and needs a +4 companion.  Measured with
  the fix alone: raw 60.104, `.text` delta unchanged.  Candidate +4 companions
  found by call-count diff (all faithful re-fetches the original performs):
  `Level::getEnemies` (orig 34 / ours 33), `PaintCanvas::TransformGetTransform`
  (37/36), `KIPlayer::setActive` (27/26), `TargetFollowCamera::translate` (57/56),
  and the wave-125 `setPosition(Vector)` compensator (orig `setPosition(fff)` 8 /
  ours 7 + `setPosition(Vector)` 59/60).  Left for a later pass.
* The `if (delta < 0)` first-reference anchor in case 125 state 3 duplicates
  eight calls the original makes once (`switchPlanetForIntro`,
  `switchSkyboxForIntro`, 2× `ParticleSystemManager::reset`,
  `PlayerStation::setVisible`, `PlayEngineSound`, `StopEngineSound`,
  `startSmokeEmission`, `autoTurretIsEnabled`).  It exists to pin `.rel.plt`
  order; removing it changes `.rel.plt` and drops `byte_exact`.
* Ours re-fetches `Level::getPlayer()` 27× vs the original's 23×; every
  single-site un-refetch measured in wave 124 is size-changing.

## Remaining root cause

Unchanged: global MachineBlockPlacement permutation plus the prologue register
map (`messages` r4 vs r5, `&Globals::status` r9 vs r4, `this` in `sl` vs
`[sp,#88]`).  This pass is a local content/order correction.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4506
python3 /dev/shm/w7603/score.py                         # 60.235270711
```

Scratch helpers (`score.py`, `dump5.py`, `od.py`, `callcount.py`, `callset.py`,
`chunks.py`, `apply.py`, `base.cpp`) live in `/dev/shm/w7603/` and are not part
of the tree.
