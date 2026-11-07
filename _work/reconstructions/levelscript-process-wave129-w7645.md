# LevelScript::process — wave 129 / w7645 (case-41 body + adjacent case-40/42 corrections)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Owned TU: `src/game/world/LevelScript.cpp`.
Base master `fee528cbe` (wave-128 final; includes w7623 + w7624).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.100000000      60.036804934    (-0.063195066)
official verify pct      60.1              60.0            (rounded)
orig / ours insns        20169 / 20040     20169 / 20043
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54542             54658           (+116)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.rel.plt diffbytes       0                 0               (byte-identical)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams of the linked `.so`
(`/dev/shm/w7645/score.py`).  `process .text diffbytes` is the byte-diff count
over the function's own 57232-byte range (`/dev/shm/w7645/bytediff.py`).

The fuzzy score and raw byte delta dip slightly because the corrected
statements move blocks relative to the original's global
MachineBlockPlacement permutation; every landed statement is confirmed
instruction-for-instruction against the original disassembly.

## Decision on w7623 (case-41 translate placement / play order)

**Keep.**  The w7623 change is faithful:

* The original case-41 state-0 tail is `setTarget`, `getPosition`,
  `setPosition(Vector)`, `translate(-38000, 0, -30200)`, `forceUpdate`
  (`0x13e5e4..0x13e62a`).  Our old source had an extra
  `translate(3000, 1000, 2000)` after `forceUpdate`; removing it is correct.
* The original case-41 state-3 body has
  `translate(3000, 1000, 2000)` immediately after
  `setPosition(freighter->getPosition())` (`0x1412f0..0x1412fe`), so adding it
  there is correct.
* The original state-1 tail is `translate(-3000,-2000,12000)`, then
  `FModSound::play(155)`, then `StopEngineSound`, then `timerA = 0`
  (`0x139dfe..0x139e2c`), so the play-after-translate order is correct.

The -7 B `.text` win and the -0.16 fuzzy dip are both real; the change is a
fidelity improvement and should not be reverted.

## Landed corrections

### Case 41 (`m_nState` 0/1/3/4)

1. **State 0: `translate(0,0,-18*delta)` was a misplaced call.**
   Original `0x13e420` calls `TargetFollowCamera::enableFirstPersonCam(false)`
   after `setFreeLookMode(false)`.  Replaced the translate with that call.
   (The `-18*delta` translate actually belongs to case 42 state 7 — see 6.)
2. **State 1: missing `m_nScriptTimerA = 0`.**
   Original `0x139e2c` stores `strd r8,r8,[r6,#144]` after
   `Player::StopEngineSound`.  Added it.
3. **State 3: bogus `m_nState = 1` removed.**
   The original state-3 body (`0x14129c..0x141338`) has no store to
   `this+0x1c`; it ends with `timerA = 0` and falls to `0x14190c`
   (`m_nState = 4`).  Our source stored 1 between the translate and
   `useTargetsUpVector`.
4. **State 4: faithful body.**
   Original `0x14119e..0x14126a`:
   ```
   updateReferenceMatrix
   translate(delta, delta, -2*delta)          <- was missing
   timerA += delta
   if (timerA >= 15001) {
       sound->stop(156)                       <- was missing
       m_nState = 5
       enemies[0]->player->setHitpoints(100)
       enemies[0]->speed = 0                  <- was missing
       hud visible = 1; radar enabled = 1
       m_bCinematicBreak = 0                  <- was duplicated
       setTarget(player->geometry)
       setLookAtCam(false); useTargetsUpVector(true)
       setComputerControlled(false); setVulnerable(true); freeze = 0
       forceUpdate
   }
   ```
   The bogus `m_nScriptTime = 0` was removed (the original state-4 body has
   no store to `this+8`), and the duplicate `m_bCinematicBreak = 0` was
   collapsed to one.

### Case 42 (`m_nState` 5/7)

5. **State 5: `translate(delta,delta,-2*delta)` was misplaced.**
   Original `0x13abd0..0x13ac3e` is `play(153)`, `m_nScriptTime = 0`, the two
   `Objective` deletes, `landmarks[3]->setPosition`, `open()`, `timerA = 0`,
   `m_nState = 6`.  There is no camera translate.  Replaced the translate
   with the missing `m_nScriptTime = 0` (`strd r6,r6,[r1,#8]` at `0x13abea`).
6. **State 7: wrong camera calls.**
   Original `0x13d8fc..0x13d924` is
   `setRumblePercentage(100.0f, 30)` (constant `0x42c80000` at `0x13dbb8`)
   then `translate(0, 0, -18*delta)` (the `-18` computation is
   `add r0,sl,sl,lsl#3; negs; lsls#1` at `0x13d908`).  Our source had
   `enableFirstPersonCam(false)` and `translate(0, delta*0.2, 0)`.

### Case 40 (adjacent, state 0)

7. **`setVisible(true)` belongs to state 0.**
   Original `0x13a56c..0x13a574` calls `KIPlayer::setVisible(true)` in state 0
   after `awake()`; the state-1 body at `0x13e6c0` starts with
   `translate(0,0,-2*delta)` and has no `setVisible`.  Replaced the bogus
   `m_nScriptTime = 0` in state 0 with `setVisible(true)`.  (State 1 still
   carries the old duplicate `setVisible`; see the size note below.)

## Size-exact compensation

The faithful set is size-changing.  Measured individual deltas (object
symbol size, `/dev/shm/w7645/apply.py` + `objsize.py`):

| correction | size delta |
|---|---:|
| case-41 state-0 `enableFirstPersonCam` | -24 |
| case-41 state-1 `timerA = 0` | 0 |
| case-41 state-3 remove `m_nState = 1` | -8 |
| case-41 state-4 faithful body | +56 |
| case-42 state-5 remove translate / add `m_nScriptTime=0` | -32 |
| case-42 state-7 camera calls | -8 |
| case-40 state-0 `setVisible` | +8 |

The landed subset (`s0 s1 s3 s4 s7 c40 c42s5rm c42s5time`) is exactly
57232 B.  A full search over the 10 candidate corrections found no
size-57232 subset that also includes both remaining case-40 fixes below.

## Remaining (not landed: no size-57232 subset found)

* **Case 40 state 1** still has the duplicate `(*enemies)[0]->setVisible(true)`
  and is missing the original `translate(0,0,-2*delta)` at `0x13e6c0`.
  Applying it (`c40b`) is +16 B.
* **Case 40 state 4** still stores `(*enemies)[0]->speed = 0.0f` instead of
  `m_nState = 5` (`0x13e808 -> 0x141b54`).  This is a real mission-progress
  bug; applying it (`c40c`) is -4 B.  Needs a +4 companion.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7645/score.py                         # 60.036804934
python3 /dev/shm/w7645/bytediff.py                      # process 54658
```

Scratch helpers (`score.py`, `bytediff.py`, `apply.py`, `objsize.py`,
`search.py`, `search2.py`, `LevelScript.base.cpp`) live in `/dev/shm/w7645/`
and are not part of the tree.
