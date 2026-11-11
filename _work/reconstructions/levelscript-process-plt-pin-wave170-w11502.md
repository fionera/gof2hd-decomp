# LevelScript::process — wave 170 / w11502: pin the fff/Vector overload + case-8/7 order

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11502`, branch
`wave170-w11502`, base master `00c403d5a`.  Scratch `/dev/shm/w11502/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    53,970
process aligned insns    17,818
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Call histogram on this base

`callseq.py` (bl/blx target-name diff over the whole function): **orig 2,900 /
ours 2,904 calls (+4 net)**.  Per-callee deltas:

| callee | orig | ours | delta |
|---|---:|---:|---:|
| `AEGeometry::setPosition(const Vector&)` | 23 | 24 | +1 |
| `Level::getStarSystem()` | 15 | 16 | +1 |
| `Level::getPlayer()` | 23 | 24 | +1 |
| `PlayerEgo::getPosition()` | 41 | 42 | +1 |
| `LODManager::forceUpdate(int,bool)` | 66 | 65 | -1 |
| `TargetFollowCamera::setPosition(float,float,float)` | 8 | 9 | +1 |
| `Status::getStanding()` | 1 | 2 | +1 (PLT anchor) |
| `Standing::setStanding(int,int)` | 1 | 2 | +1 (PLT anchor) |
| `PlayerEgo::setAutoTurret(bool)` | 2 | 3 | +1 (PLT anchor) |
| `PlayerEgo::autoTurretIsEnabled()` | 1 | 2 | +1 (PLT anchor) |
| indirect `blx reg` | 544 | 541 | -3 |

`AEGeometry::moveForward` (40/40) and `TargetFollowCamera::setPosition(Vector)`
(59/59) are equal on this base.

## What was found and landed

1. **`LevelScript.cpp:886` was the `-1` Vector / `+1` fff pair.**  The original
   disassembly at `0x13d434` is
   `blx TargetFollowCamera::setPosition(const Vector&)` with `r1 = &m_tempVec`
   (`sl+40`), after `Vector::operator=` + `Vector::operator+=` on `m_tempVec`.
   Our source passed the three components to the fff overload.  Changed to
   `m_pCamera->setPosition(m_tempVec);` — exactly the case-12 block.

2. **Case 8 was missing the state advance and had the music stop in the wrong
   block.**  Original case 8 (`0x13d13c`): `m_nState = 9; m_nScriptTimerA = 0;`
   before `setRumblePercentage(0,0)`, and the `Globals::sound->stop(currentMusicEvent)`
   lives in case 7's `!animating` block (`0x13d124`) immediately before
   `play(141)`, not in case 8.  Moved the stop to case 7 and added the two
   stores.  (The extra `m_pGeometry5->setPosition(player->getPosition())` in
   case 8 is a genuine duplicate — removing it is -8 B — but on its own it
   breaks size-exactness; see below.)

3. **Mission-89 case-3 else block order** now matches the original
   (`0x13c496`..`0x13c4d6`): `startFade(false,-1,10000)`, `(*enemies)[2]->setVisible(true)`,
   `m_nState = 4` (the original stores 4, not 5), loop `setHitpoints(0)`,
   `m_nScriptTimerA = 0`.  Size-neutral, raw -3.

## Measured (full link, `tools/fn_diff.py`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | 53,970 | 17,818 | 57,232 |
| **final (1+2+3)** | **53,947** | **17,820** | **57,232 exact** |

`.rel.plt` byte-identical to the original (verified with `objdump -s -j .rel.plt`).
Gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207, lint CLEAN.

## Negative / not landed

* Moving the `switchSunForSupernovaIntro` call back to the mission-89 case-3
  arm (using the `stars` local, removing the extra `getStarSystem` +1) is
  size-exact only together with the line-886 fix (raw 54,089 / aligned 17,736),
  **but it rotates `.rel.plt`**: our case-3 else block is placed at `0x13d8f2`,
  after the first `PlayerStation::setVisible` reference (`0x13d760`), so
  `switchSunForSupernovaIntro` moves from `.rel.plt` slot 2 to slot 8 and
  `MGame::startJumpScene` loses byte-exactness (`blx` PLT address changes).
  Gate: FAIL byte 4507 -> 4506.  Reverted.
* Removing the duplicate `m_pGeometry5->setPosition(player->getPosition())`
  from case 8 (-8 B, fixes `getPosition` and `setPosition(Vector)` +1s) is
  size -8; with the line-886 fix and the case-8 stop move the total is -24 B
  (57,208) and the tail literal pool loses 3 entries.  Not landed (size is
  sacred).
* `m_nState = 5` -> `4` alone is size-neutral (raw -3).
* The `no st78` tail hoist is +32 on this base (57,248) and aligned +2,540 —
  a clear regression.

## Remaining histogram deltas (unchanged lever)

`AEGeometry::setPosition(Vector)` +1, `PlayerEgo::getPosition()` +1 (the
case-8 duplicate pair), `Level::getPlayer()` +1, `LODManager::forceUpdate` -1,
plus the four deliberate PLT anchors and -3 indirect.  The getStarSystem +1 is
a `.rel.plt`-order anchor (see above).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 /dev/shm/w11502/callseq.py cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```
