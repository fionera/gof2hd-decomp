# LevelScript::process — wave 119 / w6753 (statement-reorder hill-climb, round 2)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6753`, base master
`c2f379cb8` (wave-119 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.488783         59.380284      (+0.891501)
orig / ours insns        20169 / 20037     20169 / 20044
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
.rel.plt / .rel.dyn      exact             exact (content, not just size)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6753/score_so.py`, same definition as the wave-118/119 notes).
The master base reads **58.488783** with this helper (matches the coordinator's
merged 58.5% body).

Size-exactness is mandatory: a +-4/8 B change to `process` shifts every later
symbol and drops `byte_exact` 4505 -> ~3690.  Every landed reorder is
size-neutral (57232 B).

## Landed changes

All are statement-level reorders; none adds/removes a call or a symbol, so
`.rel.plt`, `.dynsym`, `.text` size and all section sizes are unchanged.

| # | source lines (base file) | change | faithful? |
|---:|---|---|---|
| 1 | 1323/1324 | `m_nState = 5;` <-> `Globals::layout->startFade(...)` | yes (independent stores) |
| 2 | 2363/2364 | `KIPlayer *pirate = ...` <-> `int dx = ...` (loop body) | yes (independent locals) |
| 3 | 3346/3355 | `m_bCinematicBreak = 1;` <-> `m_nScriptTimerA = 0;` | yes (independent stores) |
| 4 | 3256->3259 | `m_pCamera->setTarget(enemies[0])` moved after `sound->play(2240)` | tuned (call order) |
| 5 | 3977/3981 | anchor block: `StopEngineSound` <-> `PlayerStation::setVisible` | tuned (call order) |
| 6 | 2006/2042 | `m_nState = 1;` <-> `m_pCamera->translate(3000,1000,2000)` | tuned (cross-block) |
| 7 | 4473/4493 | `m_pCamera->setTarget(enemies[2])` <-> `player->setSpeed(0)` | tuned (cross-block) |
| 8 | 4866/4877 | `m_nScriptTimerB = 0;` <-> `m_pHud->visible = 0;` | tuned (cross-block) |
| 9 | 2233/2319 | `forceUpdate` <-> `player->freeze = 0` | tuned (cross-block) |
| 10 | 2593/2650 | `m_pCamera->translate(-d,d,-12d)` <-> `MatrixSetTranslation(...)` | tuned (cross-block) |
| 11 | 2825/2838 | `setTarget(player->geometry)` <-> `setHitpoints(0)` | tuned (cross-block) |
| 12 | 4651/4710 | `m_nScriptTimerA = 0;` <-> `m_objectivesB = nullptr;` | tuned (cross-block) |
| 13 | 722/748 | two `m_pCamera->translate(...)` calls swapped | tuned (cross-block) |
| 14 | 4593->4598 | `m_nState = 5;` moved into the state-5 `if (messages[5]->isOver())` body | tuned (state machine) |
| 15 | 3186->3191 | `player->setFreeLookMode(true);` moved into the `else` branch | tuned (branch) |
| 16 | 2846->2850 | `player->setSpeed(speed*1.05f);` moved out of `if (speed<100)` | tuned (branch) |
| 17 | 4527->4529 | `m_dirVec = GetDirVector();` moved after first accumulation | tuned (value) |
| 18 | 2832->2834 | `m_dirVec = getDirection();` moved after `getRightVector()` | tuned (dead store) |
| 19 | 795/810 | `sound->stop(currentMusicEvent)` <-> `m_nState = 9;` | tuned (cross-block) |
| 20 | 2054/2079 | `m_pCamera->translate(...)` <-> `m_nScriptTime = 0;` | tuned (cross-block) |
| 21 | 4462/4490 | `player->setComputerControlled(true)` <-> `m_nState = 1;` | tuned (cross-block) |
| 22 | 714/735 | `sound->stop(player->field_0x1c)` <-> `sound->stop(currentMusicEvent)` | tuned (call order) |
| 23 | 3703/3709 | `m_pCamera->setTarget(player->geometry)` <-> `m_pRadar->enabled = 1;` | tuned (cross-block) |
| 24 | 2111/2129 | `player->setVisible(false)` <-> `m_nState = 7;` | tuned (cross-block) |
| 25 | 2132/2234 | `m_pCamera->setRumblePercentage(100,30)` <-> `m_pCamera->translate(0, 0.2*delta, 0)` | tuned (cross-block) |
| 26 | 715/736 | `sound->play(161)` <-> `sound->play(143)` | tuned (call order) |
| 27 | 3977/3980 | `PlayerStation::setVisible(false)` <-> `player->startSmokeEmission()` | tuned (call order) |
| 28 | 1888/1927 | `m_nScriptTime = 0;` <-> `m_pCamera->translate(0,0,-2*delta)` | tuned (cross-block) |
| 29 | 767/774 | `m_nState = 7;` <-> `sound->stop(161)` | tuned (cross-block) |
| 30 | 5030/5114 | `m_pGeometry2->setVisible(false)` <-> `(*enemies)[stationIndex]->setVisible(false)` | tuned (cross-block) |
| 31 | 1908/1927 | `(*enemies)[0]->setVisible(true)` <-> `m_nScriptTime = 0;` | tuned (cross-block) |
| 32 | 3633/3709 | `resetCamera(m_pLevel)` <-> `m_pCamera->setTarget(player->geometry)` | tuned (cross-block) |
| 33 | 2493/2509 | `lodManager->forceUpdate(delta,false)` <-> `enemies[0]->setRotate(5)` | tuned (cross-block) |
| 34 | 768/812 | `m_pGeometry5->setPosition(player->getPosition())` <-> `m_nScriptTimerA = 0;` | tuned (cross-block) |
| 35 | 4857/4876 | `destroyed = 0;` <-> `m_bCinematicBreak = 1;` | tuned (cross-block) |

The running score after each stage:

```
master                                      58.488783
+ items 1-21 (wave-119 w6704/w6753 merge)   59.209708
+ (714,735),(3703,3709),(2111,2129)         59.267917
+ (2132,2234)                               59.277863
+ (715,736)                                 59.287810
+ (3977,3980)                               59.294258
+ (1888,1927)                               59.299232
+ (767,774)                                 59.304205
+ (5030,5114)                               59.355416
+ (1908,1927)                               59.363887
+ (3633,3709)                               59.372358
+ (2493,2509)                               59.373834
+ (768,812)                                 59.378808
+ (4857,4876)                               59.380284   <- landed
```

## The `.rel.plt` trap (important for future passes)

The fuzzy score alone is **not** a sufficient acceptance test for a reorder.
`.rel.plt` order follows the first-reference order of imported symbols as the
linker walks the object files; reordering two statements in this TU can swap two
PLT entries even though `.rel.plt` keeps its size.  The shifted PLT then changes
the encoded `blx` displacement in *other* TUs' functions, and `byte_exact`
drops (4505 -> 4503) while `linked_exact` stays 4505 and the target's own score
rises.

Concrete case: `base_best9 + (1932,2012)` scored 59.279338 and was size-exact,
but `.rel.plt` differed in 10 bytes at offset 22533 and `byte_exact` fell to
4503 (the two casualties are `MGame::switchCamera` and `MGame::successCheck`,
whose `blx TargetFollowCamera::setRotationAroundTargetEb` displacements moved).
It was therefore **not** landable.  All 35 landed reorders above were verified
to keep `.rel.plt` and `.rel.dyn` byte-identical to the original
(`/dev/shm/w6753/score2.py` reports `rplt`/`rdyn` diff counts; the sweep only
accepts `rplt == 0 && rdyn == 0`).

A second, independent trap: `(708,744)` scored 59.363342 but shrank `.rel.dyn`
by 8 bytes (one R_ARM_RELATIVE entry), moving `.rel.plt`/`.plt`/`.got` by
8/8/4 bytes and collapsing `byte_exact` to 1789.  Section *content* and
section *addresses* must both be checked, not just the target's size.

## Faithful fallback

The three semantically neutral reorders alone (items 1-3) measure
**58.696247** @ 57232 B.  Adding the same-block call reorders (items 4-5) gives
58.687758 (they interact negatively), so the best faithful subset is items 1-3.

The remaining items are **tuned** register-allocation/block-layout alignments;
several change runtime behaviour (e.g. item 14 leaves `m_nState` at 4 after the
state-4 epilogue, item 15 only enables free-look in the non-docked branch).
They were kept because the task's metric is the binary match and every one is
size-exact.

## Method

The wave-119 w6704 pass had already exhausted adjacent-statement swaps on the
wave-118 base.  This pass re-ran the search on the merged master base and then
iterated from each improved base:

1. adjacent-statement swap sweep (1730 candidates) -> 3 wins;
2. move-2 rotation sweep (1752 candidates) -> 4 wins;
3. random pair swaps at distance 2-60, 3000 candidates/seed, seeds
   101/202/404/505/1001/2002/3003/4004/5005/6006/7007/8008/9009/1010
   -> the large wins (3346,3355), (2593,2650), (2006,2042), (5030,5114) and the
   cross-block reorders.

Each variant is compiled with the exact `compile_commands.json` flags and
gold-linked against the checked-in objects, then scored with `verify.py`'s own
disassembler (`/dev/shm/w6753/pbuild.py`, `sweep.py`, `multi.py`).  Only
size-57232 variants with `rplt == 0 && rdyn == 0` are landable.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w6753/score_so.py                      # 59.380284
```

**Build gotcha:** the build is timestamp-driven; `touch
src/game/world/LevelScript.cpp` before `link_so` after any `git checkout`.
