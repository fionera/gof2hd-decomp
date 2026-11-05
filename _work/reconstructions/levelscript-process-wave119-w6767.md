# LevelScript::process — wave 119 / w6767 (statement-order hill-climb, angle J)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6767`, base master
`415673c43`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Metric = repo's own `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6767/score.py`, unrounded).  Baseline on this tree reads
**58.498122031** (matches the wave-119 w6757 coordinator-merge note).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.498122         58.589125      (+0.091003)
orig / ours insns        20169 / 20034     20169 / 20033
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
.ARM.extab (ours/orig)   0x26290 / 0x26298 0x26290 / 0x26298  (pre-existing 8 B short)
```

All landed changes are reorders of independent statements (or of mutually
exclusive `m_nState` arms); semantics are unchanged.  Every one is size-neutral
in the final combination (57232 B).

## Landed changes

| # | source region | change |
|---:|---|---|
| 1 | case 40 (~734) | `Globals::sound->stop(currentMusicEvent)` moved between the `m_pRadar->enabled = 0` store and `player->setComputerControlled(true)` |
| 2 | anchor guard (~3973) | `if (delta < 0)` -> `if (delta < -1)` (tuned; same codegen as `< -2`, `<= -2`, `-delta > 1`) |
| 3 | anchor block (~3973) | the 8 statements inside the `delta < -1` block permuted to `PlayEngineSound, startSmokeEmission, StopEngineSound, switchPlanetForIntro, landmarks[0]->setVisible(false), particleEmitBoolPtr->reset, switchSkyboxForIntro, particleRenderBoolPtr->reset` |
| 4 | case 70 (~2491) | `((PlayerFighter *) getEnemies()[0])->setRotate(5)` moved to just after `lodManager->forceUpdate(delta, false)`, before the `isOver()` early break |
| 5 | case 16 (~1874) | `m_nScriptTime = 0;` moved to the top of the arm, before `m_nState = 2` |
| 6 | case 145 (~4588) | `m_pCamera->setPosition(m_tempVec)` moved from the state-4 tail into the state-5 arm, after the `messages[5]->isOver()` test |
| 7 | case 158 state 2/3 (~5201) | `setCloakingPossible(true)` moved from the state-2 epilogue into the state-3 `m_nScriptTimerA >= 4001` block |
| 8 | case 158 state 2/3 (~4262) | `enemies[0]->geometry->moveForward(...)` moved from the `m_nScriptTimerA > 4000` block into the `messages[3]->isOver()` arm, before `m_nScriptTimerA = 0` |
| 9 | case 102 (~3251) | `Globals::sound->stop(currentMusicEvent)` moved between `m_pHud->visible = 0` and `m_pRadar->enabled = 0` |

Running score after each landed change (each measured on top of the previous):
`58.498122 -> 58.503097 -> 58.506616 -> 58.516566 -> 58.518629 -> 58.522148 ->
58.538527 -> 58.571286 -> 58.576261 -> 58.589125`.

## The `.plt`-order trap (important for future passes)

Two of the candidate moves were **rejected because they silently reorder the
PLT** and therefore break `byte_exact` for two unrelated MGame functions
(`MGame::gameOverCheck`, `MGame::startJumpScene`), even though `linked_exact`
stays 4505:

| candidate | size | ratio | PLT order | gate |
|---|---:|---:|---|---|
| case-91 state-0 `setActive(true)` move (empty state-1 arm) | 57232 | **58.602592** | `setDockingState` **before** `scaleSunDuringSupernovaIntro` | **FAIL byte 4503** |
| same, plus case-105 radar move | 57240 | 58.5713 | dock-first | FAIL (size) |
| landed set (case-91 move reverted, case-105 radar move reverted) | 57232 | 58.589125 | sun-first (matches original) | **PASS byte 4505** |

The original `.plt` order is `scaleSunDuringSupernovaIntro(2825) <
updateSupernova < isInWormhole < freeMissionLock < switchSunForSupernovaIntro <
setDockingState(2830)`.  The case-91 `setActive` move changes MachineBlockPlacement
so the case-91 state-2 block (first `setDockingState` reference) is emitted at
`0x13ab00` instead of after the case-4 block; the linker then allocates the PLT
slot first and every `blx setDockingState` displacement in the image changes.
Any future move that relocates a call across a block boundary must be checked
with `plt_map()` (helper `/dev/shm/w6767/pltcheck.py`), not just by size/score.

## Faithful case-91 shape (measured, not landed)

The original case-91 state machine (orig `0x13ac42`, `0x13ebe4`) is:

```c++
if (m_nState == 0 && msg3->isTriggered()) {
    m_nScriptTimerB = 0;
    m_nState = 1;                    // NOT 2
    (*enemies)[0]->setActive(true);
    ((PlayerFixedObject *) m_pLevel->getDockingTarget(0))->setName(...);
} else if (m_nState == 1 && player->isDockedToDockingPoint()) {
    m_nState = 2;                    // no setActive here
} else if (m_nState == 2 && msg5->isOver()) { ... }
```

That shape restores the original PLT order but compiles to **57240 B**
(58.317580) — the state-0 check is hoisted into the shared dispatch block in our
build, making the body 8 B shorter than the original's.  Landing it requires
finding 8 B elsewhere; the current master shape (`m_nState = 2` in state 0,
`setActive` in the state-1 arm) is size-exact and PLT-clean, so it was kept.

## Other measured dead ends (all size-exact unless stated)

| variant | % | size |
|---|---:|---:|
| prologue declarations M,P,E (faithful call order) | 57.861980 | 57308 |
| prologue M,P,E + all landed moves | 57.861980 | 57308 |
| case-105 radar store inside `if (field_0xaa)` | 58.589125 -> 58.602592 (with case-91 move) | 57232/57240 |
| case-158 `setCloakingPossible` move alone on base4 | 58.351572 | 57240 |
| case-158 `moveForward` move alone on base4 | 58.324834 | 57220 |
| case-102 sound-stop move alone on base4 | 58.334577 | 57240 |
| case-70 `setRotate` move alone on base4 | 58.521540 | 57232 |
| case-16 `m_nScriptTime` move alone on base4 | 58.518021 | 57232 |
| case-145 `setPosition` move alone on base4 | 58.531489 | 57232 |

## Structural findings (recorded)

* Original prologue call order is **messages, player, enemies** (M,P,E) but the
  stack slots match our E,P,M build; every M-first declaration order compiles to
  57300/57308 B.  The original spills `&m_pLevel` to `sp+80` and reuses `r4` for
  `Globals::status`; our build keeps `m_pLevel` in `r9` (`r4` in the M,P,E
  variant) and stores it at `sp+92`.  This is the single biggest remaining
  register-allocation divergence.
* Original mission switch dispatch and case-body order are unchanged from the
  w6757 note (tbh at `0x138a8a` / `0x138c3e`; bodies 131,64,135,145,142,144,139,
  69,73,67,70,29,41,80,56,125,92,157,40,102,154,24,14,21,42,91,94,105,114,16,158).
* `.ARM.extab` remains 8 B short (`createShip` -8 only); 27 sections, file size
  2,187,328 B, all unchanged by this pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6767/score.py cmake-build-match/libgof2hdaa.so
```
