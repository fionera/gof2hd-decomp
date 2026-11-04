# LevelScript::process — wave 106 (w5403): state-16 body, case order, and the
# size / `.rel.plt` double constraint

Status: **note only, no code change landed**. Branch `wave106-w5403`, base master
`486f8a119`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Baseline (HEAD) official linked raw fuzzy: **54.155469 %** (rounded 54.2),
size 57232, insns 20169 / 20019.  Everything below is measured against that
number with `tools/verify/verify.py`'s reconciled instruction streams of the
linked `.so` (`difflib.SequenceMatcher(...).ratio()*100`, unrounded).

No variant that satisfies **both** hard constraints below beat 54.155469.
The best measured shape overall is **54.227333 %** but it is 20 B short of
57232, and any size change breaks `byte_exact` (see §4).

## 1. Proven: the mission-0 state-16 tail in our source is wrong

Original `0x139170..0x1391b8` (state 16, reached from `0x13916c`) plus the
shared tail at `0x13b132..0x13b15a`:

```
0x139170  ldr  r4, [Globals::layout]
0x139176  blx  Layout::isFading
0x13917c  cmp  r0, #0 ; bne 0x13df76
0x139182  blx  Layout::enableFillScreen(true)
0x13918a  blx  Status::nextCampaignMission(true)
0x139194  r5 = 0
0x1391a8  str  r5, [Globals::switch_to_target_setting]
0x1391aa  r1 = 100
0x1391ac  blx  Galaxy::getStation
0x1391b4  blx  Status::departStation
0x1391bc  b.w  0x13b132            ; shared tail
0x13b132  blx  Status::getStation
0x13b138  blx  Station::setAttackedFriends(false)
0x13b13c  strb r5, [Level::comingFromAlienWorld]
0x13b14c  blx  ApplicationManager::SetCurrentApplicationModule(5)
0x13b152  strb r5, [Level::initStreamOutPosition]
```

i.e. the real body is

```cpp
Globals::layout->enableFillScreen(true);
Globals::status->nextCampaignMission(true);
Globals::switch_to_target_setting = 0;
Globals::status->departStation((Station *) (intptr_t) Globals::galaxy->getStation(100));
Globals::status->getStation()->setAttackedFriends(false);
Level::comingFromAlienWorld = 0;
Globals::appManager->SetCurrentApplicationModule(5);
Level::initStreamOutPosition = 0;
return 0;
```

HEAD has the truncated `switch_to_target_setting = 1;
SetCurrentApplicationModule(2);` body (60 B shorter).  This is a real bug.

## 2. Proven: mission-1 state-3 `departStation` argument is 10, not 100

Original `0x13b122: movs r1, #10` (the `Galaxy::getStation` argument for the
mission-1 state-3 tail).  HEAD passes `100` there.  Real bug.

## 3. Proven: the campaign-switch case-body order; case 125 precedes case 92

The dispatch chain at `0x1387ce..0x13885e` gives every case body address
directly.  Sorting by address yields the original source order:

```
158,
[131,145,135,142,144,139],      // tbh @0x138a8a
[64,69,73,67,70],               // tbh @0x138c3e
29, 41, 80, 56, 125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114, 16
```

The two tbh groups were decoded from their jump tables and their body order
matches our source **exactly**.  The only divergence is that our source emits
`92, 157, 125, 40`; the original emits `125, 92, 157, 40`.  Moving the whole
`case 125: { ... }` block (source lines 3506..3652 at HEAD) to just before
`case 92:` reproduces the original order (verified: our `case 125` body then
lands at `0x139d04`, `case 92` at `0x139d68`, i.e. 125 before 92).

## 4. The two hard constraints that make this function a local optimum

**(a) The function must stay exactly 57232 B.**  Any size change shifts
`.text` and therefore the literal-pool / `.rodata` layout, which drops
`byte_exact` for hundreds of unrelated functions:

| size delta | byte_exact |
|---|---:|
| 0 (57232) | 4500 |
| -4 (57228) | 4319 |
| -20 (57212) | 3688 |

**(b) `.rel.plt` must stay byte-identical.**  The two dead anchors in HEAD
(the `delta == 0x7d` Standing anchor and the
`setAutoTurret(autoTurretIsEnabled())` anchor) are *load-bearing for PLT
order*, not just size:

* removing the Standing anchor moves `Standing::setStanding` 13 slots later
  in `.rel.plt`;
* removing the auto-turret anchor moves `PlayerEgo::autoTurretIsEnabled` /
  `setAutoTurret` 8 slots later.

Either rotation changes every caller's `blx <plt>` encoding and costs one
`byte_exact` (4500 -> 4499, RATCHET FAIL) even when the function stays 57232 B.

## 5. Measured table (official linked raw ratio, unrounded)

All builds: `cmake --build cmake-build-match --target link_so -j6`.

| variant | size | raw % |
|---|---:|---:|
| **HEAD baseline** (`getEnemies,getPlayer,getMessages`) | **57232** | **54.155469** |
| prologue `player,enemies,messages` | 57232 | 54.154122 |
| prologue `enemies,messages,player` | 57312 | 53.337982 |
| prologue `messages,enemies,player` | 57312 | 53.342962 |
| prologue `messages,player,enemies` | 57312 | 53.359231 |
| prologue `player,messages,enemies` | 57312 | 53.342962 |
| case 125 moved before case 92 | 57232 | 54.150493 |
| state-16 real body only | 57252 | 54.118467 |
| state-16 + `getStation(10)` | 57172 | 53.700799 |
| state-16 + both anchors removed | 57212 | **54.227333** (best raw, not size-exact) |
| state-16 + auto-turret anchor removed | 57236 | 54.139192 |
| state-16 + Standing anchor removed | 57228 | 54.114719 |
| state-16 + both anchors kept, drop `departStation`+`setAttackedFriends` | 57232 | 54.119286 |
| state-16 + both anchors kept, drop `departStation`+`setAttackedFriends`+`initStreamOutPosition`, one `getPlayer` re-fetch cached | 57232 | 54.142822 |
| state-16 + both anchors kept, drop `initStreamOutPosition`, one `getPlayer` re-fetch cached | 57244 | 54.194865 |
| state-16 body reordered (`departStation` before `comingFromAlienWorld`) | 57144 | 54.037901 |
| state-16 body reordered (other order) | 57240 | 54.075438 |
| mission-0 chain `4,3,5` -> `3,4,5` | 57228 | 54.037375 |
| mission-0 chain `3,5,4` | 57204 | 54.202444 |
| mission-0 chain `4,5,3` | 57172 | 49.226022 |
| `Status *st = Globals::status;` used for the three state-16 status calls | 57232 | 54.114310 |

The `54.227333` shape is the best source shape found (state-16 body + both
anchors removed, prologue unchanged); it is **20 B short** and its `.rel.plt`
is rotated, so it cannot land without breaking `byte_exact`.

## 6. Other verified facts (call-count deltas at HEAD)

`LevelScript::lookBehind` (2) and `LevelScript::setAutoPilotToProgrammedStation`
(1) **are already present** in the source; earlier handoff notes claiming
"ours 0" were stale.  Remaining real call-count deltas at HEAD:

| symbol | orig | ours | note |
|---|---:|---:|---|
| `Galaxy::getStation` | 12 | 11 | fixed by §1 |
| `Status::departStation` | 9 | 8 | fixed by §1 |
| `TransformGetTransform` | 37 | 36 | one missing re-fetch; not localised |
| `RadioMessage::isTriggered` | 32 | 31 | not localised |
| `TargetFollowCamera::translate` | 57 | 56 | not localised |
| `PlayerEgo::setAutoTurret` | 2 | 3 | anchor duplicate |
| `PlayerEgo::autoTurretIsEnabled` | 1 | 2 | anchor duplicate |
| `Status::getStanding` / `Standing::setStanding` | 1 / 1 | 2 / 2 | anchor duplicate |
| `switchPlanetForIntro` / `switchSkyboxForIntro` | 1 / 1 | 2 / 2 | mission-125 anchor |
| `ParticleSystemManager::reset` | 2 | 4 | mission-125 anchor |
| `Level::getLandmarks` / `Level::getStarSystem` | 15 / 15 | 17 / 17 | mission-125 anchor |

The auto-turret site in the original is `if (field_0xaa)
player->setAutoTurret(true);` (reconciled insn 18489-18494), not
`setAutoTurret(autoTurretIsEnabled())`; the latter at source line 4307 is a
pure PLT-order anchor.

## 7. Hypotheses (not proven)

* The 925-instruction `replace` block `O[7397:8322]` (the mission-125
  state-3 body) has only ~30.8 % local similarity with our real body at
  `U[8771:9704]` even after the case-125 move, so the loss in that window is
  *content*, not just placement.  The original's in-body state order (from the
  `m_nState` stores) is `5, 11, 8, 9, 12, 13, 14, 16, 15, 4`, which is clang's
  `MachineBlockPlacement`, not source order.
* The `+20 B` needed to make the `54.227333` shape size-exact is most likely
  the missing `TransformGetTransform` re-fetch (a shared-tail `if/else` whose
  two arms each call `TransformGetTransform` and then fall into one
  `Transform::UpdateExb`; our compiler CSEs one of the two calls).

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
```

Raw score helper (unrounded, official instruction streams):

```python
# /dev/shm/w5403/score2.py (scratch, not tracked)
# same as tools/verify/verify.py --show, but prints
# difflib.SequenceMatcher(None,onorm,unorm,autojunk=False).ratio()*100
```

`.rel.plt` check:

```sh
readelf -rW <so> | awk '$3=="R_ARM_JUMP_SLOT"{print $5}' > rp.txt
# diff the two name lists; HEAD is identical (3674/3674)
```

Case-body order extraction: decode the linear compare chain at
`0x1387ce..0x13885e` (`cmp r0,#N; beq.w <body>`) and the two `tbh` tables at
`0x138a8a` (15 entries, base `0x138a8e`) and `0x138c3e` (10 entries, base
`0x138c42`), then sort the bodies by address.

## 9. Gate at the end of this pass

```
byte_exact 4500  linked_exact 4500  size-exact 4524/4524  data_exact 843/843
imports 207  lint CLEAN  drift 0  RATCHET PASS
```

Worktree clean; no source change committed.
