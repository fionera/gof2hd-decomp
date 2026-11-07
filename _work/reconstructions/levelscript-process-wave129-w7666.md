# LevelScript::process — wave 129 / w7666 (re-land w7645 case-40/41/42 at 57232 B)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `9e58f485e`
(w7646 order; w7645 case-40/41/42 deferred).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.102467171      59.894564
official verify pct      60.1              59.9
orig / ours insns        20169 / 20039     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54577             54623
whole .text diffbytes    85441             85487
.ARM.extab diffbytes     1427              1426
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
on `tools/verify/verify.py`'s reconciled streams of the linked `.so`.

## Why w7645 was deferred, and how the 8 B were found

The full w7645 correction set (case-41 state-0 `enableFirstPersonCam`, state-1
`timerA = 0`, state-3 drop `m_nState = 1`, state-4 faithful body, case-42 state-5
`m_nScriptTime`, state-7 camera calls, case-40 state-0 `setVisible`) measures
**57240 B** on top of the w7646 tree (the w7645 note's "57232" was measured on a
pre-w7646 base).  The compensating -8 B is a genuine fidelity bug in the **same
case 42**: our state-6 body carried `player->freeze = 0;`, which is absent from
the original (`0x13c238..0x13c34e` has no store to `player+0x24`).  Removing it is
exactly -8 B and restores 57232.

## Landed changes

### Case 40

* **State 0:** `m_nScriptTime = 0` -> `(*enemies)[0]->setVisible(true)`
  (original `0x13a574`).
* **State 1:** remove the duplicate `(*enemies)[0]->setVisible(true)`; the
  original state-1 body (`0x13e6c0`) starts with `translate(0,0,-2*delta)` and
  has no `setVisible`.

### Case 41

* **State 0:** `translate(0,0,-18*delta)` -> `enableFirstPersonCam(false)`
  (original `0x13e424`).
* **State 1:** add `m_nScriptTimerA = 0` after `StopEngineSound`
  (original `0x139e2c`, `strd r8,r8,[r6,#144]`).
* **State 3:** drop the bogus `m_nState = 1` (original body ends with
  `timerA = 0` then `m_nState = 4`).
* **State 4:** faithful body: `translate(delta,delta,-2*delta)`,
  `timerA += delta` **before** the translate (disasm `0x1411c4..0x1411e0`),
  then `stop(156)`, `m_nState = 5`, `setHitpoints(100)`, `speed = 0`,
  HUD/radar, one `m_bCinematicBreak = 0`, `setTarget`, `setLookAtCam(false)`,
  `useTargetsUpVector(true)`, `setComputerControlled(false)`,
  `setVulnerable(true)`, `freeze = 0`, `forceUpdate`.

### Case 42

* **State 5:** drop the misplaced camera translate; add `m_nScriptTime = 0`
  (original `0x13abea`).  Order the objective deletes faithfully:
  `delete objectivesB; objectivesB = nullptr; delete objectivesA;
  objectivesA = nullptr` (original `0x13abee..0x13ac12`).
* **State 6:** remove the bogus `player->freeze = 0`; move
  `player->setVisible(false)` to its original slot after
  `setComputerControlled(true)` (`0x13c290`); move `freeMissionLock()` after
  the camera `translate` (`0x13c31c`); restore the missing `m_nState = 7`
  state transition (`0x13c348`).
* **State 7:** `enableFirstPersonCam(false)` + `translate(0,delta*0.2,0)` ->
  `setRumblePercentage(100.0f, 30)` + `translate(0,0,-18*delta)`
  (original `0x13d8fc..0x13d924`).

## Remaining (measured, not landed: no size-57232 subset)

* **Case 40 state 1** is still missing the original `translate(0,0,-2*delta)`
  (`0x13e6c0`).  Adding it costs +16..+24 B.
* **Case 40 state 4** still stores `(*enemies)[0]->speed = 0.0f` instead of
  `m_nState = 5` (`0x13e808 -> 0x141b54`).  This is a real mission-progress
  bug (the transport never advances to state 5).  Applying it is -4 B and
  needs a +4 companion; no genuine +4 statement was found in cases 40/41/42.
  A full 32-combination sweep over the optional case-40/41/42 fixes found no
  57232 B subset that includes it.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7666/score.py                         # 59.894564
python3 /dev/shm/w7666/secdiff.py                       # .text 85487, extab 1426
```

Scratch helpers (`score.py`, `secdiff.py`, `apply.py`, `batch.sh`,
`LevelScript.base.cpp`) live in `/dev/shm/w7666/` and are not part of the tree.
