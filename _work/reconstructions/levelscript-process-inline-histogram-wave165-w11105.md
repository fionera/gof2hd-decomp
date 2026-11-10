# LevelScript::process — wave 165 / w11105: callee inlining histogram

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11105`, base `3633505f8` (wave 164).  Scratch
`/dev/shm/w11105/`.

Baseline (unchanged HEAD):

```
raw 54,247   aligned 17,532   size 57,232/57,232   insn 20,709/20,830
gate byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## Question

Which callees did the original inline into `process`, and which did it call?
Does any callee belong to the original `Classes/Game/LevelScript.cpp` TU, so
that our tree must define it there to reproduce the same inlining?

## TU membership (authority: `_work/original_layout/`)

`tu_symbols['gof2hd_1.1.19_ios']['Classes/Game/LevelScript.cpp']` contains
**only** the `LevelScript` methods (C1/C2/D1/D2, `process`, `resetCamera`,
`render3D`, `skipSequence`, `canSkipCutsceneNow`, `skipCutscene`,
`resetStartSequenceOver`, `startSequenceOver`, `startSequence`, `getEvent`,
`setEvent`, `lookBehind`, `setAutoPilotToProgrammedStation`) plus the weak
template instantiation `ArrayAdd<Player*>(Player*, Array<Player*>&)`.
No other class's functions are in that TU.

Therefore **no cross-class callee can have been inlined into `process`**:
the build has no LTO, and every callee below is a strong (`g`) out-of-line
symbol owned by another TU:

| callee | original TU |
|---|---|
| `Level::getPlayer`, `Level::getStarSystem`, `Level::getEnemies`, `Level::getLandmarks`, `Level::getDockingTarget` | `Classes/Game/Level.cpp` |
| `AEGeometry::getPosition/getDirection/getRightVector/getUpVector/setPosition/moveForward/...` | `Classes/Game/AEGeometry.cpp` |
| `Status::getStanding`, `Status::getCurrentCampaignMission`, … | `Classes/Game/Status.cpp` |
| `Standing::setStanding` | `Classes/Game/Standing.cpp` |
| `PlayerEgo::getPosition`, `setAutoTurret`, `autoTurretIsEnabled`, … | `Classes/Game/PlayerEgo.cpp` |
| `LODManager::forceUpdate` | `Classes/Game/LODManager.cpp` |
| `TargetFollowCamera::setPosition` (both overloads) | `Classes/Game/TargetFollowCamera.cpp` |

The only same-TU callees are `LevelScript::resetCamera` (51 calls, **not**
inlined in the original either — it goes through the PLT) and
`ArrayAdd<Player*>` (inlined in both; no call in either histogram).

So the "inlined-call histogram" is really a **called-callee histogram**;
an inlining difference would show up as a callee present in one histogram
and absent from the other.  There is no such callee.

## Histogram (PLT calls + `blx fp` GOT-indirect calls)

Extracted with `objdump -d` over the whole function, resolving `bl`/`blx`
targets by symbol name.  `0xf` below is the objdump rendering of `blx fp`
(a cached GOT function pointer, e.g. `AEMath::operator*`); it is *not* a
call to address 0xf.

Totals: original 2,402 direct calls, ours 2,404; 247 distinct names in both.
Indirect (`blx rN`) calls: original 544, ours 542 — the GOT-indirect/thunk
call volume is matched to within 2, so no callee was turned into an inline
load on either side.

Every name matches exactly except:

| callee | orig | ours | delta |
|---|---:|---:|---:|
| `blx fp` (GOT-indirect) | 46 | 40 | -6 |
| `AEGeometry::moveForward(float)` | 40 | 41 | +1 |
| `AEGeometry::setPosition(const Vector&)` | 23 | 24 | +1 |
| `LODManager::forceUpdate(int,bool)` | 66 | 65 | -1 |
| `TargetFollowCamera::setPosition(const Vector&)` | 59 | 58 | -1 |
| `TargetFollowCamera::setPosition(float,float,float)` | 8 | 9 | +1 |
| `Level::getStarSystem()` | 15 | 16 | +1 |
| `Level::getPlayer()` | 23 | 24 | +1 |
| `Status::getStanding()` | 1 | 2 | +1 |
| `Standing::setStanding(int,int)` | 1 | 2 | +1 |
| `PlayerEgo::getPosition()` | 41 | 42 | +1 |
| `PlayerEgo::setAutoTurret(bool)` | 2 | 3 | +1 |
| `PlayerEgo::autoTurretIsEnabled()` | 1 | 2 | +1 |
| local (non-exported) callee | 1 (`+0xff125194`) | 1 (`+0xff124f28`) | different target |

The local-callee difference is exactly the ratchet's `#X` blind spot: both
sides make one call into local code, but to different local functions.

## Causes of the +1 call deltas

Three are **deliberate PLT-order anchors** left by earlier passes, not
inlining:

| source | anchor | extra calls |
|---|---|---|
| `LevelScript.cpp:1818` | `player->setAutoTurret(player->autoTurretIsEnabled());` | `autoTurretIsEnabled` +1, `setAutoTurret` +1 |
| `LevelScript.cpp:2023` | `if (delta == 0x7d) status->getStanding()->setStanding(0,100);` | `getStanding` +1, `setStanding` +1 |
| `LevelScript.cpp:2139` | `m_pLevel->getStarSystem()->switchSunForSupernovaIntro();` | `getStarSystem` +1 |

The original has exactly one `switchSunForSupernovaIntro` call (mission-89
state 2, `0x13c496`) and reuses the `StarSystem *stars` already fetched at
`0x13b170`; our anchor re-fetches it in a different mission block.  The
original has exactly one `getStanding`/`setStanding` pair (`0x139b06`) and
one `autoTurretIsEnabled` (`0x13a7fc`).

The remaining deltas (`getPlayer`, `getPosition`, `moveForward`,
`AEGeometry::setPosition`; original's `forceUpdate`, both
`TargetFollowCamera::setPosition` overloads) are source-shape differences in
one or more blocks that were not isolated in this pass: the call-sequence
alignment is destroyed by block reordering, so a per-site mapping is not
recoverable from the linked binary alone.

## Measured experiments (all reverted; tree left at HEAD)

| shape | raw | aligned | size | note |
|---|---:|---:|---:|---|
| HEAD | **54,247** | 17,532 | 57,232 | |
| move `switchSun` back to case 3 + drop anchor | 54,972 | 17,453 | 57,240 (+8) | size regression |
| same, plus original else-branch order (`setVisible` before loop, `m_nState` last) | 54,392 | 17,450 | 57,232 | size exact, raw +145 |
| else-branch reorder only (keep anchor) | 54,481 | 17,525 | 57,224 (-8) | |
| drop auto-turret anchor (`:1818`) | 54,886 | 17,372 | 57,216 (-16) | .rel.plt differs |
| drop Standing anchor (`:2023`) | 54,999 | 17,525 | 57,208 (-24) | |
| `m_pLevel->getPlayer()->getPosition()` → cached `player` (`:2871`) | 54,256 | 17,531 | 57,232 | raw +9 |
| same site → `m_tempVec` | 54,817 | 17,527 | 57,216 (-16) | |

The three anchors are load-bearing: removing any one changes `.rel.plt`
(18 / 26 bytes) and shrinks `.text` by 16–24 B, so removing them requires a
size compensation elsewhere.  Moving the `switchSun` call back to its true
original site is structurally faithful and size-exact with the original
else-branch order, but costs 145 raw bytes; it was not landed because raw
is the byte-identicality ground truth.

## Conclusion

* No callee is inlined in the original that our tree calls: every callee is
  an out-of-line strong symbol in a different original TU.  The histogram
  differs only by call counts, not by callee membership.
* `LevelScript.cpp` already owns exactly the original TU's symbols
  (`LevelScript` methods + `ArrayAdd<Player*>`); no TU merge or declaration
  move is needed for inlining.
* The +1 counts are accounted for by the three PLT-order anchors
  (`getStarSystem`, `getStanding`/`setStanding`, `autoTurretIsEnabled`/
  `setAutoTurret`).  The remaining ±1 counts are source-shape mismatches,
  not inlining.
