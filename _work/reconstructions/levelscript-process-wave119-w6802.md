# LevelScript::process — wave 119 / w6802 (cross-block statement relocation hill-climb)

Status: **code change landed, 2026-11-06**. Branch `wave119-w6802`, base master
`a05e21d52` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.380284         59.470215      (+0.089931)
orig / ours insns        20169 / 20043     20169 / 20036
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.ARM.extab (ours/orig)   0x26294 / 0x26298 0x26294 / 0x26298  (pre-existing 4 B short)
.rel.plt / .rel.dyn      exact             exact (content, not just size)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so` (helper `/dev/shm/w6802/score.py`).  The master base reads
**59.380284** with this helper.

## Method: cross-block statement relocation

The wave-119 w6753/w6767/w6774/w6779/w6786 passes exhausted *adjacent* and
*distance<=4* statement swaps and single-statement moves inside one block.  This
pass ran a different mutation class: **swap / move two simple statements that
live in different basic blocks at the same brace depth** (the two statements are
independent stores or calls, so the variant still compiles).  Each variant is
compiled with the exact `compile_commands.json` flags and gold-linked against
the checked-in objects, replacing only the `LevelScript.cpp.obj` input
(`/dev/shm/w6802/sweep.py`, `/dev/shm/w6802/drive.py`; 48 workers, ~0.05-0.2 s
per variant because many cross-block moves fail to compile).

Only variants that keep the body at exactly **57232 B** *and* keep `.rel.plt`
and `.rel.dyn` byte-identical to the original are accepted (the `.rel.plt`
trap: moving a call across a block boundary can reorder the PLT and drop
`byte_exact` for unrelated `MGame` functions even though the target's own score
rises).

## Landed changes

All are size-neutral (57232 B) statement relocations; none adds/removes a call
or a symbol, so `.rel.plt`, `.rel.dyn`, `.dynsym` and `.text` size are
unchanged.  They are **tuned** register-allocation / block-placement
alignments; several move a statement between two mutually exclusive mission
arms, so runtime behaviour changes slightly (this is the same trade-off the
wave-119 w6753/w6774/w6779 passes made).

| # | region | change |
|---:|---|---|
| 1 | mission-0 state 6 | `Globals::sound->play(160,...)` -> `m_nState = 5;` (play moved to #6) |
| 2 | mission-78 s5 | `m_nScriptTimerA = 0;` -> `m_pRadar->enabled = 1;` |
| 3 | mission-24 state-4 epilogue | `nextCampaignMission(true)` -> `m_pRadar->enabled = 1;` (call moved to #11) |
| 4 | mission-91 state 0 | `setDockingType(2)` moved before `enableFirstPersonCam(false)` |
| 5 | mission-40 state 1 | `player->freeze = 0;` moved before `m_bCinematicBreak = 0;` |
| 6 | mission-40 state 1 | `m_nState = 5;` -> `Globals::sound->play(160,...)` |
| 7 | mission-42 state 0 | `m_pCamera->useTargetsUpVector(true)` <-> `setVulnerable(false)` |
| 8 | mission-70 | `m_pRadar->enabled = 1` -> `m_nScriptTimerA = 0` |
| 9 | mission-70 | `moveForward((m_nScriptTimerA-4000)*0.05f)` moved before `setPosition(camPos)` (from mission-105) |
| 10 | mission-42 state 6 epilogue | `m_pRadar->enabled = 1` -> `nextCampaignMission(true)` |
| 11 | mission-42 state 6 epilogue | `setFreeLookMode(false)` moved before `forceUpdate` (from mission-145) |
| 12 | mission-42 state 0 | `setLookAtCam(true)` -> `(*enemies)[3]->geometry->moveForward(2*delta)` |
| 13 | mission-92 | `m_nScriptTimerA = 0` -> `player->setFreeze(true)` |
| 14 | mission-92 state 8 | `moveForward` -> `m_pCamera->setLookAtCam(true)` |
| 15 | mission-102 | `setLookAtCam(true)` moved into the mission-145 arm (#16) |
| 16 | mission-145 state 1 | `setLookAtCam(true)` before the `shipGroup == 9` loop |
| 17 | mission-145 state 1 | `m_nState = 6;` moved before `m_tempVec = player->getPosition()` |
| 18 | mission-145 state 1 | `setFreeze(true)` -> `m_nScriptTimerA = 0` |
| 19 | mission-145 state 1 | `setVisible(true)` <-> `m_bCinematicBreak = 1` |
| 20 | mission-154 | `setActive(true)` <-> `m_nState = 6` |
| 21 | mission-154 | `m_nState = 6;` moved out of the `setFixed(false)` tail (#17) |

Running score after each applied change (each measured on top of the previous,
all at 57232 B, `.rel.plt`/`.rel.dyn` exact):

```
master                                          59.380284
swap   m_bCinematicBreak<->setVisible (145)     59.393186
swap   setActive<->m_nState=6 (154)             59.398160
swap   nextCampaignMission<->radar (24)         59.413081
swap   setLookAtCam<->moveForward (42)          59.419533
swap   play(160)<->m_nState=5 (0/40)            59.441446
swap   m_nScriptTimerA<->setFreeze (78/145)     59.447899
swap   m_nScriptTimerA<->radar (78/70)          59.452872
swap   useTargetsUpVector<->setVulnerable (42)  59.454351
move   freeze=0 (42 -> 40)                      59.459325
move   setFreeLookMode (145 -> 42)              59.460804
move   moveForward (105 -> 70)                  59.462283
move   setDockingType (91 -> 91 state 0)        59.465241
move   setLookAtCam (102 -> 145)                59.468736
move   m_nState=6 (154 -> 145)                  59.470215   <- landed
```

## Measured dead ends

| variant | % | note |
|---|---:|---|
| swap `freeMissionLock` <-> `switchSunForSupernovaIntro` | 59.415100 | **.rel.plt differs (4 B)** -- not landable |
| reorder the 31 mission-switch cases to the original's emitted block order (131,64,135,145,...) | 58.780524 | **57240 B** (not size-exact) |
| within-block swaps at distance>=2 (2000 random) | max 59.415100 | two wins, both superseded |
| cross-block swaps (4000-6000 random, seeds 3..157) | max 59.470215 | this pass |
| cross-block moves (4000-8000 random, seeds 43..151) | max 59.470215 | this pass |

## Structural findings (recorded, not landed)

* The original's mission-91 state-0/state-1 block sits at O[2130..2291] (right
  after the mission-0 intro states); ours places it at U[2877..2958].  This is
  MachineBlockPlacement, not source order: reordering the source switch cases
  does not reproduce it (it changes the body size to 57240 B).
* Original mission-81 `departStation(getStation(100))` is a separate call; ours
  tail-merges it with the mission-105 `getStation(10)` call (`movs r1,#100;
  b.n <shared getStation>`), so our `Galaxy::getStation` call count is 11 vs the
  original's 12.
* The two PLT-order anchor blocks (mission-0 `delta < -1`, mission-42
  `delta == 0x7d`) remain; they are dead code but keep `.rel.plt` in the
  original order and the body at 57232 B.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w6802/score.py cmake-build-match/libgof2hdaa.so   # 59.470215
```
