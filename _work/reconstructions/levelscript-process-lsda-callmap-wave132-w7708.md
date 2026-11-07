# LevelScript::process — wave 132 / w7708: LSDA record decode + invoke/call-site map

Status: **research note, no landed source change**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 normalised instructions. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `70e451ffe`.

## Result

```
                         before (master)   after (no change)
raw ratio (difflib)      59.879643905      59.879643905
official verify pct      59.9              59.9
orig / ours insns        20169 / 20045     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
.text differing bytes    85379             85379
.ARM.extab differing     1420              1420
.ARM.exidx               0                 0
.rel.plt                 0                 0
.rodata/.data/.bss       0                 0
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## 1. The single `.ARM.extab` record owned by `process`

`.ARM.exidx` has exactly **one** entry for the function (`0x1a12a0`), pointing at
extab record `0x1c2af8`, record length **348 B** in both. Decoded with
`_work/reconstructions/extab_lsda_decode.py`:

| field | original | ours |
|---|---:|---:|
| personality | `0x5eb60` | `0x5eb60` |
| unwind words | 3 | 3 |
| `lpStart` encoding | `0xff` (omit) | `0xff` |
| `ttype` encoding | `0xff` (cleanup only) | `0xff` |
| call-site encoding | `0x01` (ULEB) | `0x01` |
| call-site table bytes | 324 | 326 |
| **rows** | **46** | **46** |
| rows with a landing pad | **24** | **24** |
| distinct landing pads | **24** | **24** |
| raw differing bytes | — | **291 / 348** |

Same row count, same landing-pad count, same single cleanup-only personality.
The 291 differing bytes are ULEB row starts/lengths/landing-pad offsets, i.e.
code-layout derived.

## 2. Every landing pad resolved to its cleanup action

Each landing pad is a 4–6 byte stub that branches into the shared cleanup tail
at `0x145c34..0x145c6a` (`_ZdlPv` / `StringD1` / `_Unwind_Resume`). The table
below is the full row list; `lp` is the landing-pad offset from the function
start and the action is the decoded stub.

```
ORIGINAL (fn 0x137d18)                          OURS (fn 0x137d18)
row start  len  lp    action                    row start  len  lp    action
  1   5832   6  57116 delete r6                  1   5650   6  57094 delete r5
  3   6586   8  57080 String sp+112              3   6422   8  57080 String sp+112
  5   8226   8  57106 delete r5                  5   7658   8  57096 delete r5
  7   8898   8  57104 delete r5                  7   8298   8  57106 delete r5
  9   9624   8  57074 String sp+220              9   8436  10  57162 String sp+280
 11   9942   6  57114 delete r5                 10   8448  10  57152 String sp+368
 13  12162   8  57098 String sp+232             11   8458  14  57142 String sp+268
 15  14224  14  57162 String sp+280             12   8472   8  57132 String sp+136
 16  14242   8  57152 String sp+368             14   9988   8  57072 String sp+220
 17  14250  12  57142 String sp+268             16  10246   6  57108 delete r5
 18  14262   8  57132 String sp+136             18  19600   8  57100 String sp+232
 20  19988   6  57086 delete r5                 20  23672   8  57058 String sp+124
 22  24978   8  57058 String sp+124             22  25182  12  57098 delete r5
 24  26118  12  57096 delete r5                 24  27726   8  57116 delete r6
 26  28924   8  57108 delete r8                 26  29670  10  57088 delete [sp+72]
 28  29260   8  57064 delete r6                 28  30688   6  57078 delete r5
 30  29820  10  57090 delete [sp+72]            30  31966   8  57110 delete r8
 32  36488   4  57122 delete r5                 32  36784   4  57122 delete r5
 34  39118   6  57088 delete r6                 34  38912   6  57086 delete r6
 36  41624   8  57068 delete r4                 36  41716   8  57066 delete r4
 38  41640   8  57066 delete r5                 38  41732   8  57064 delete r5
 40  47204   8  57056 delete r5                 40  45900   8  57056 delete r5
 42  49682  16  57050 delete fp                 42  49888  16  57054 delete r6
 44  54454   8  57048 delete r5                 44  54430   8  57052 delete r5
```

Action multiset:

| action | original | ours |
|---|---:|---:|
| `delete r5` | 9 | 10 |
| `delete r6` | 3 | 3 |
| `delete r4` | 1 | 1 |
| `delete r8` | 1 | 1 |
| `delete [sp+72]` | 1 | 1 |
| `delete fp` | 1 | **0** |
| `String sp+112` | 1 | 1 |
| `String sp+220` | 1 | 1 |
| `String sp+232` | 1 | 1 |
| `String sp+124` | 1 | 1 |
| `String sp+280/368/268/136` cascade | 4 | 4 |

The only action difference is the known register-colouring swap: the original
has one `delete fp` trampoline (row 42, the case-157
`new AEGeometry(0x37a7, Canvas, false)` marker) where ours deletes the same
object through `r6`, and ours has one extra shared `delete r5` stub. The
destructor *chains* are identical: the 4-String cascade destroys
`sp+280 -> sp+368 -> sp+268 -> sp+136` in both, and each single `String`
landing pad calls `StringD1` on the same stack slot.

**Conclusion: the EH cleanup scopes are faithful.** There is no missing or
extra destructor/lifetime scope to recover from source-level scope placement.

## 3. Every invoke resolved to its call (new evidence)

The 24 landing-pad rows are the 24 `invoke` call sites. Decoding the call in
each range gives the *same* 24-call multiset in both binaries:

| call | orig | ours |
|---|---:|---:|
| `Explosion::Explosion(int)` | 2 | 2 |
| `PlayerFixedObject::setName(String)` | 5 | 5 |
| `Objective::Objective(int,int,Level*)` | 2 | 2 |
| `Route::Route(int*,int)` | 6 | 6 |
| `AEGeometry::AEGeometry(PaintCanvas*)` | 3 | 3 |
| `AEGeometry::AEGeometry(unsigned short,PaintCanvas*,bool)` | 2 | 2 |
| `String operator+(String,String)` | 2 | 2 |
| `String::String(int)` | 1 | 1 |
| `Array<Player>::Array()` | 1 | 1 |
| **total** | **24** | **24** |

The *order* differs only because the case bodies are laid out in a different
order by `MachineBlockPlacement` (the first four and the last seven invokes are
in the same order; the middle thirteen are permuted). The invoke-range length
differences (e.g. 8 vs 10 bytes for a `setName`) are argument-setup encodings
(`mov` vs `add.w`), not different statements.

## 4. Whole-function call-count delta (new evidence)

Counting every `bl`/`blx` target over the full 57232 B body exposes the real
structural deltas (positive = ours has more):

```
TargetFollowCamera::setPosition(fff)      orig  8  ours  7   (-1)
TargetFollowCamera::translate(fff)        orig 57  ours 55   (-2)
AEGeometry::setVisible(bool)              orig 12  ours 11   (-1)
KIPlayer::setVisible(bool)                orig 34  ours 35   (+1)
PlayerStation::setVisible(bool)           orig  1  ours  2   (+1)
PlayerEgo::PlayEngineSound()              orig  1  ours  2   (+1)
PlayerEgo::StopEngineSound()              orig  1  ours  2   (+1)
PlayerEgo::startSmokeEmission()           orig  1  ours  2   (+1)
PlayerEgo::autoTurretIsEnabled()          orig  1  ours  2   (+1)
PlayerEgo::setAutoTurret(bool)            orig  2  ours  3   (+1)
ParticleSystemManager::reset()            orig  2  ours  4   (+2)
Level::getPlayer()                        orig 23  ours 27   (+4)
Level::getEnemies()                       orig 34  ours 33   (-1)
Level::getLandmarks()                     orig 15  ours 17   (+2)
Level::getStarSystem()                    orig 15  ours 17   (+2)
Level::switchSkyboxForIntro()             orig  1  ours  2   (+1)
StarSystem::switchPlanetForIntro()        orig  1  ours  2   (+1)
Status::getStanding()                     orig  1  ours  2   (+1)
Standing::setStanding(int,int)            orig  1  ours  2   (+1)
Galaxy::getStation(int)                   orig 12  ours 11   (-1)
Status::departStation(Station*)           orig  9  ours  8   (-1)
LODManager::forceUpdate(int,bool)         orig 66  ours 65   (-1)
PaintCanvas::TransformGetTransform(uint)  orig 37  ours 36   (-1)
FModSound::stop(int)                      orig 10  ours 11   (+1)
ApplicationManager::SetCurrentApplicationModule(uint) orig 9 ours 10 (+1)
AEMath::Vector::operator=(Vector const&)  orig 33  ours 34   (+1)
AEGeometry::getPosition()                 orig 50  ours 51   (+1)
TargetFollowCamera::setPosition(Vector const&) orig 59 ours 60 (+1)
TargetFollowCamera::setRumblePercentage(float,int) orig 39 ours 40 (+1)
TargetFollowCamera::setTarget(AEGeometry*) orig 81 ours 83  (+2)
KIPlayer::setActive(bool)                 orig 27  ours 26   (-1)
```

### 4.1 The `delta < 0` anchor block explains the cluster of `+1`s

The source contains a deliberate dead-code first-reference anchor at
`LevelScript.cpp` ~3966 (inside case 125 state 3):

```cpp
if (delta < 0) {
    m_pLevel->getStarSystem()->switchPlanetForIntro();
    m_pLevel->switchSkyboxForIntro();
    m_pLevel->particleRenderBoolPtr->reset();
    player->startSmokeEmission();
    m_pLevel->particleEmitBoolPtr->reset();
    player->PlayEngineSound();
    player->StopEngineSound();
    ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->setVisible(false);
}
```

The caller never passes a negative `delta`, but the compiler cannot prove it,
so the block is emitted. It accounts exactly for the `+1` deltas of
`switchPlanetForIntro`, `switchSkyboxForIntro`, `ParticleSystemManager::reset`
(+2: two distinct reset pointers), `startSmokeEmission`, `PlayEngineSound`,
`StopEngineSound`, `PlayerStation::setVisible`, and one each of
`getStarSystem`/`getLandmarks`. Removing it would remove ~8 call sites but
changes `.rel.plt` first-reference order and the 57232 B size, so it cannot be
dropped without a replacement anchor.

### 4.2 The missing `setPosition(fff)` is a tail-merge, not a missing statement

The source has 8 `m_pCamera->setPosition(x,y,z)` statements (lines 624, 654,
831, 1074, 1092, 1133, 1862, 3788) but only 7 call sites are emitted. The
missing one is the case-70 state-2 statement at line 1074
(`setPosition(20000,5000,30000)`). In the original it is a **tail call** into
the shared block at `0x13c888`:

```
13baa6  ldr r1,[pc]        ; 20000.0f
13baa8  movs r6,#3
13baaa  ldr r0,[sl,#20]    ; m_pCamera
13baae  ldr r2,[pc]        ; 5000.0f
13bab0  ldr r3,[pc]        ; 30000.0f
13bab2  str r6,[sl,#28]    ; m_nState = 3
13bab6  b.w 13c888         ; tail-merge with state 8's setPosition block
...
13c888  blx TargetFollowCamera::setPosition(fff)
```

So the statement is present in the source; ours just places the call in a
different shared block. This is a `MachineBlockPlacement` decision, not a
missing line.

## 5. Experiment: real case-157 bug fix is size-shrinking

The w7687 note identified `LevelScript.cpp:5035` as a genuine semantic bug:
`(*enemies)[stationIndex]->setVisible(false)` should be
`m_pGeometry2->setVisible(false)` (original `0x145430..0x145440` loads
`this+0xc0`). Applying it aligns the call counts (`AEGeometry::setVisible`
11 -> 12, `KIPlayer::setVisible` 35 -> 34) but:

| variant | size | raw % | extab diff bytes | byte_exact |
|---|---:|---:|---:|---:|
| baseline | 57232 | **59.879644** | 291 | 4506 |
| case-157 `m_pGeometry2->setVisible(false)` | 57228 | 59.328529 | 292 | **4328** |

The -4 B shift moves every following symbol, and `ratchet` reports
`byte_exact 4506 -> 4328`. Not landable without a +4 B faithful companion.

## 6. Why the LSDA cannot be improved from source

The record is already the right size (348 B), right row count (46), right
landing-pad count (24), right personality, and right cleanup-action multiset.
Every differing byte is a ULEB row start/length/landing-pad offset derived from
the code layout. The only action difference (`delete fp` vs an extra
`delete r5`) is register colouring. Therefore the `.ARM.extab` delta is a
*consequence* of the `.text` block-placement delta, not an independent lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                       # record diff
```

Scratch helpers in `/dev/shm/w7708/`: `invmap.py` (invoke -> call map),
`lp_actions.txt` (landing-pad -> action table), `callcount2.py`
(whole-function call-count delta), `callsites.py` (per-symbol call sites),
`fulldiff.py` (normalised instruction diff).

## Open questions for the next wave

* The `delta < 0` anchor block is the single biggest source-level shortcut in
  this function. Replacing it with a *real* first-reference site (e.g. moving
  the mission-0 case-9/11 `switchPlanetForIntro`/`switchSkyboxForIntro`/
  particle-reset calls to their true positions) would remove 8 fake call sites
  and might let the real missing calls (`translate` x2, `setPosition(fff)` x1,
  `forceUpdate`, `TransformGetTransform`, `getStation`, `departStation`,
  `getEnemies`, `KIPlayer::setActive`) be restored at the same total size.
* The case-157 fix (-4) + case-131 state-2 fix (-8) need a +12 B faithful
  companion; the candidate is the state-2 `setPosition(fff)` tail-merge block
  (currently shared) or one of the two missing `translate` calls.
