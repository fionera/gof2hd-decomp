# LevelScript::process — wave 191b / w13716: case-102 s0, mission-0 s6, case-89 s3 timerB

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13716`, branch
`wave191b-w13716`, base master `c83a0131e` (raw **51,621**, aligned **21,451**,
`.text` diffbytes 72,116, `.ARM.extab` 564, `.rel.plt` md5
`f93278173d9c7a35677f841ff939eab5`).

## Lens

The wave-191b handoff notes cover the tail half (w13561/w13599), the d1–d6
transposition sweeps (w13570), case-14/16/24/29/40/41/42/56/64/67/69/70/73
line buckets, case-92 state 4 (w13631), case-14 state 1 (w13642),
case-158 state 1/2 (w13645/w13667), case-154 state 4 (w13657/w13668),
case-24 state 1 (w13676), case-105 state 6 (w13687), case-70 state 1
(w13697) and case-91 state 5 (w13684, not merged into this base).  This pass
re-derives the case-102 state-0 teardown, the mission-0 state-6 speed-boost /
campaign hand-off, and the second 64-bit timer reset in case-89 state 3 from
the Thumb disassembly, and measures a set of further semantic candidates.

## Landed change 1 — case-102 state 0 (commit c827f9375)

Original `0x13a6xx` order (decomp + disasm):

```
setTurretMode(false)
resetCamera(m_pLevel)
if (isInRocketControl()) { setRocketControl(0,0); killLiberator(); setRumblePercentage(0,0); }
setFreeLookMode(false)
enableFirstPersonCam(false)
hideShipForFirstPersonCameraView(false)
stopShooting(0)
setFreeze(true)
setVisible(false)
setVulnerable(false)
m_bCinematicBreak = 1
m_pHud->visible = 0
m_pRadar->enabled = 0
setLookAtCam(true)
setTarget(enemies[0]->geometry)
setPosition(enemies[0]->getPosition() + {9000,-7000,40000})
sound->stop(currentMusicEvent)
sound->play(2240,0,0,0)
m_nScriptTimerA = 0
m_nState++
```

The whole block is reordered to that order.  All moved statements touch
disjoint state (camera target/transform, player control/visibility/speed
flags, HUD/radar bytes, cinematic flag), so the reorder is
semantics-preserving.

Also landed in the same commit (measured individually on the pristine base):

* case-154 s0 (L3463): reload `getRightVector()` before the `* 0.04f` add.
* case-105 s9 (L3810): `revive()` before the three `nextInt(1000)` draws,
  assigned x, y, z in that order (the old code swapped y/z).
* case-105 s10 (L3861): scale the right-vector term by `1000.0f` before the
  `*camPos += offset`.
* operator spelling: `player->GetDirVector() * 2400.0f` (was
  `2400.0f * ...`) and `right * -30000.0f` (was `-30000.0f * right`).

```
raw        51,621 -> 51,642  (+21)
aligned    21,451 -> 21,434  (-17)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,116 -> 72,137 (+21)
.ARM.extab 564 (unchanged)
.rel.plt   md5 f9327817… (byte-identical)
check      RATCHET PASS
```

## Landed change 2 — mission-0 state 6 + case-89 state 3 timerB (commit a0261c6b2)

Original mission-0 state 6 (`0x141022`..`0x1410de`):

```
m_nScriptTimerA += delta;                      // 64-bit
if (m_nScriptTimerA >= 2001) {
    if (player->getSpeed() < 100.0f)           // getSpeed() called twice
        player->setSpeed(player->getSpeed() * 1.05f);
    if (m_nScriptTimerA >= 8001) {
        Globals::status->nextCampaignMission(true);
        Globals::status->setStation(getStation(113));
        Globals::status->departStation(getStation(113));
        Level::initStreamOutPosition = 1;
        status->field_64 = getHitpoints();
        status->field_5c = getShieldHP();
        status->field_60 = getArmorHP();
        status->field_68 = getGammaHP();
        appManager->SetCurrentApplicationModule(2);
    }
}
```

The old source had `setStation` before `nextCampaignMission`, the
`setSpeed(*1.05)` inside the `>= 8001` arm, a dead `getSpeed()` re-read,
`departStation` after `initStreamOutPosition`, and only one `getSpeed()`
call.  The block is now instruction-identical to the original modulo
register allocation.

The faithful block is 8 B shorter than the old block, so the missing
`m_nScriptTimerB = 0` in **case-89 state 3** (original
`13c4d4: movs r0,#0; 13c4d6: strd r0,r0,[sl,#144]` zeroes both 64-bit
timers) is added as the exact size compensation — and it is a real
semantic fix.

```
raw        51,642 -> 51,964  (+322)
aligned    21,434 -> 21,187  (-247)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,137 -> 72,459 (+322)
.ARM.extab 564 (unchanged)
.rel.plt   md5 f9327817… (byte-identical)
check      RATCHET PASS
```

The raw cost is the global register-allocation shift caused by the
mission-0 block; `.ARM.extab` is unchanged, so this is a net aligned win of
the kind accepted for case-105 s6 (-113) and case-70 s1 (-115), but larger.

## Candidate table (all measured on the real NDK r18b `-Oz` build + gold link)

| candidate | raw | aligned | size | extab | rel.plt | note |
|---|---:|---:|---:|---:|---|---|
| base (c83a0131e) | 51,621 | 21,451 | 57,232 | 564 | ok | — |
| **case-102 s0 + 5 fixes (landed)** | **51,642** | **21,434** | **57,232** | **564** | **ok** | **landed** |
| **mission-0 s6 + case-89 timerB (landed)** | **51,964** | **21,187** | **57,232** | **564** | **ok** | **landed** |
| mission-0 s6 faithful, single getSpeed | 53,231 | 21,438 | 57,216 | 564 | ok | size -16 |
| mission-0 s6 faithful, double getSpeed | 52,854 | 21,198 | 57,224 | 564 | ok | size -8 |
| case-14 s4: nextCampaignMission first | 51,966 | 21,200 | 57,232 | 564 | ok | aligned +13 |
| case-89 s3: startFade/setVisible order, state last | 51,968 | 21,186 | 57,232 | 564 | ok | raw +4 |
| case-89 s3 full faithful (switchSun moved, anchor removed) | 52,353 | 21,200 | 57,232 | 564 | **changed** | rel.plt |
| case-89 s3 full faithful + anchor kept | 53,624 | 21,202 | 57,240 | 564 | ok | size +8 |
| case-91 s5 faithful order (w13684) | 51,946 | 21,414 | 57,232 | 564 | ok | aligned +227 |
| case-135 s0: setPosition/awake/setVisible/setEnemy | 51,967 | 21,188 | 57,232 | 564 | ok | neutral |
| case-135 s0 reorder + timerB | 54,028 | 21,172 | 57,240 | 564 | ok | size +8 |
| case-135 s0 timerB alone | 54,031 | 21,178 | 57,240 | 564 | ok | size +8 |
| case-68 s6 faithful (setTarget + cam/matrix/forceUpdate order + timerB) | 53,530 | 21,185 | 57,248 | 564 | ok | size +16 |
| case-68 s6 setTarget alone | 53,483 | 21,187 | 57,248 | 564 | ok | size +16 |

## Further semantic findings (not landed — size compensation missing)

* **case-135 state 0** (`0x13c50e`..`0x13c5f0`): the original order is
  `getPosition`, `setPosition(playerPos + {xOff,5000,25000})`, `awake()`
  (vtable+0xc), `setVisible(true)`, `setEnemy(player)`, `delete cargo`,
  `cargo = nullptr`; ours has `setVisible` before `setPosition` and `awake`
  after `setEnemy`.  The original also zeroes **both** timers
  (`13c5ee: strd r0,r0,[r1,#144]`); ours only sets `m_nScriptTimerA`.
  Reorder alone is size-exact but aligned-neutral; the timerB store costs
  +8 B.
* **case-68 state 6** (`0x14156a`..`0x141654`): the original has
  `m_pCamera->setTarget(m_pGeometry6)` after `setLookAtCam(true)` (this is
  the `-1 TargetFollowCamera::setTarget` in the call multiset), calls
  `CameraGetCurrent` after `frame->setPosition(m_tempVec)`, runs
  `lodManager->forceUpdate` **before** `m_pGeometry0->setVisible(true)`,
  and zeroes both timers after the visibility store.  Adding the setTarget
  alone grows the function by 16 B.
* **case-14 state 4** (`0x141470`..`0x1414d2`): the original order is
  `nextCampaignMission(true)`, `departStation(getStation(98))`,
  `switch_to_target_setting = 0`, `SetCurrentApplicationModule(5)`,
  `initStreamOutPosition = 0`, `m_nState = 5`; ours starts with
  `departStation` and runs `SetCurrentApplicationModule` before
  `switch_to_target_setting`.
* **case-89 state 3** (`0x13c496`): the original order is
  `switchSunForSupernovaIntro`, `startFade(false,-1,10000)`,
  `enemies[2]->setVisible(true)`, the hitpoint loop, `timerA=0`, `timerB=0`,
  then the shared `m_nState = 4` tail.  Moving the call here changes
  `.rel.plt` because the block is placed before the anchor's
  `setDockingState` reference; the anchor at L2164 must stay.
* **case-91 state 5** (`0x13f3a0`..`0x13f53a`): w13684's faithful order
  (`resetCamera` first inside the rocket if, `setTurretMode`/`resetCamera`/
  `setFreeLookMode`/`enableFirstPersonCam`, radar before `setCollide`,
  `setTarget` before the vector adds, direction*10000 then right*700,
  `setHitpoints`/`m_nState` last) is size-exact and `.rel.plt`-identical
  but measures aligned +227 on this base (the opposite sign from w13684's
  own base, because the mission-0 fix shifts the global allocator).

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13716/` (`gd.py`, `decomp.txt`,
`aligndiff.py`, `linemap2.py`, `objline.dis`, `secdiff.py`, `multiset.py`,
`pairdiff.py`, `gctx.py`, `base_LevelScript.cpp`, `base5b.cpp`,
`base_c89.cpp`, build logs `b1..b31.log`).
