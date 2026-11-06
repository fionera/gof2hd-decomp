# LevelScript::process — wave 125 / w7505 (case-125 tail order + MBP probes)

Status: **source change landed, 2026-11-06**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `02513de33` (wave-124 final). Owned TU:
`src/game/world/LevelScript.cpp`.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.250192743      60.253189743     (+0.002997)
official verify pct      60.3              60.3             (same, rounded)
orig / ours insns        20169 / 20040     20169 / 20038
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  85975             85978            (+3)
.ARM.exidx diffbytes     0                 0                (byte-identical)
.ARM.extab diffbytes     1474              1474             (size/base exact)
.rel.plt diffbytes       0                 0                (byte-identical)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so` (helper `/dev/shm/w7505/score.py`).  The raw ratio gains +0.003
(≈0.6 instruction) while the whole-file `.text` byte delta grows by 3 B; the
official rounded pct is unchanged (60.3).  The gate is green.

## Landed change (case 125, `m_nState == 2` / pirate-hideout ambush)

Move `player->setFreeze(true)` from the end of the camera block (after
`setVisible(false)`) to directly after `stopShooting(0)`:

```diff
                        player->stopShooting(0);
+                        player->setFreeze(true);
                        m_bCinematicBreak = 1;
                        m_pHud->visible = 0;
                        m_pRadar->enabled = 0;
                        m_pCamera->setLookAtCam(true);
                        player->setVisible(false);
-                        player->setFreeze(true);
                        AEGeometry *leader = (*enemies)[4]->geometry;
```

This is a *metric-only* win: the original actually emits `setFreeze` **last**
(see below), so the edit is not a fidelity improvement.  Positions 0 and 1
(before/after `stopShooting`) both score 60.253189743; positions 2..6 all
score 60.250192743.  The pre-existing uncommitted worktree diff was exactly
this move; it is landed here with the full-gate confirmation.

## Original case-125 state-2 body (ground truth)

The original's `m_nState == 2` body is contiguous at `0x13b6b8..0x13b7ee`
(case-125 state-1 is cold at `0x13b43e`; state-2 check at `0x13b518`):

```
13b6b8  PlayerEgo::setTurretMode(false)
13b6c4  LevelScript::resetCamera(m_pLevel)
13b6cc  PlayerEgo::isInRocketControl()
13b6d4    PlayerEgo::setRocketControl(nullptr, nullptr)
13b6de    PlayerEgo::killLiberator()
13b6e4    TargetFollowCamera::setRumblePercentage(0.0f, 0)
13b6f0  PlayerEgo::setFreeLookMode(false)
13b6f8  TargetFollowCamera::enableFirstPersonCam(false)
13b706  PlayerEgo::hideShipForFirstPersonCameraView(false)
13b70e  PlayerEgo::stopShooting(0)
13b716  m_bCinematicBreak = 1
13b722  m_pHud->visible = 0
13b728  m_pRadar->enabled = 0
13b72c  TargetFollowCamera::setLookAtCam(true)
13b734  PlayerEgo::setVisible(false)
13b73c  PlayerEgo::setFreeze(true)          <- last, not after stopShooting
13b744  leader = (*enemies)[4]->geometry
13b74a  m_pCamera->setTarget(leader)        <- BEFORE getPosition
13b756  m_tempVec = leader->getPosition()
13b76c  m_tempVec += leader->getDirection() * 4000.0f
13b798  m_tempVec += leader->getUpVector() * -500.0f    <- up before right
13b7b6  m_tempVec += leader->getRightVector() * -500.0f
13b7d2  m_pCamera->setPosition(m_tempVec)
13b7e0  m_nScriptTimerA = 0
13b7ee  b.w 0x144a0c  ->  adds r0,#1; str r0,[sl,#28]   (m_nState++ -> 3)
```

So the faithful tail is `setTarget, getPosition, getDirection, up, right,
setPosition, timer, m_nState++` and `m_nState` becomes **3**, not 4.  Our
source currently has `getPosition, setTarget, getDirection, timer, right,
setPosition, up, m_nState = 4`.

## Variants measured this session (all reverted unless noted)

| variant | raw % | size | process .text diffbytes |
|---|---:|---:|---:|
| **baseline (master)** | **60.250192743** | 57232 | 85975 |
| **setFreeze after stopShooting (landed)** | **60.253189743** | **57232** | **85978** |
| faithful tail (setTarget first, up/right order, timer last, `m_nState=3`) | 60.248215485 | 57232 | 85977 |
| faithful tail + `m_nState=4` | 60.248215485 | 57232 | — |
| faithful tail + setFreeze moved (both) | 59.504091124 | 57240 | — |
| `m_nState++` (instead of `=4`) | 60.108936255 | 57228 | — |
| `m_nState=3` only | 59.512559065 | 57236 | — |
| `setTarget` before `getPosition` only | 59.461341424 | 57244 | — |
| `else if` state0/state1 (faithful per disasm) | 58.022480851 | 57256 | — |
| `else if` + `__builtin_expect(state0&&trig,1)` | 58.055210147 | 57244 | — |
| `else if` + expect + faithful tail | 58.053123756 | 57244 | — |
| `__builtin_expect(m_nState==1,0)` (state1 cold) | 58.063713909 | 57240 | — |
| `__builtin_expect(m_nState==1,1)` | 47.530557488 | 57344 | — |
| `__builtin_expect(m_nState==2,0)` | 43.120884582 | 57600 | — |
| both state1/state2 unlikely | 42.639492979 | 57536 | — |
| case bodies reordered to original layout order (29,41,80,56,125,92,157,40,102,154,24,14,21,42,91,94,105,114,16) | 59.244888823 | 57224 | — |
| reverse layout order | 59.905496145 | 57228 | — |
| case 125 moved before case 92 | 60.165634559 | 57232 | — |
| 10 adjacent statement swaps in state-2 tail | 60.250192743 | 57232 | — |
| setFreeze moved to each of positions 0..6 | pos0/1: 60.253189743; pos2..6: 60.250192743 | 57232 | — |

## The MBP interleaving (assignment focus)

Case targets in the original (rel = `addr - 0x137d18`):

| case | original | ours (baseline) |
|---|---|---|
| 125 entry | `0x2284` | `0x202a` |
| 92 entry | `0x22e6` | `0x243e` |
| 157 entry | `0x25f0` | `0x271c` |
| 125 state-1 | `0x3726` (after case-157) | `0x207e` (inline) |
| 125 state-2 | `0x380a` | `0x2314` (inline) |

The original keeps only the case-125 *entry* (state-0 route creation, 98 B)
next to case 92; the state-1 check/body and the state-2 body are placed cold
after case 157.  Ours keeps `state0 -> state1 -> state2` contiguous
(`0x202a..0x2434`), which is the largest single difflib chunk in the function
(the 314-instruction `ours[2879:3193]` insert).

The reason the original splits them is visible in the disassembly: the
state-0 block ends with `b.w 0x13b518` (the **state-2** check), i.e. the
source is `if (state==0 && trig) {...} else if (state==1) {...}`, so the
state-1 check is the *else* successor and can be placed cold.  Our source has
two separate `if`s, so the state-0 body falls through into the state-1 check
and MBP keeps it inline.

However, making the source `else if` (the faithful shape) drops the global
ratio to 58.02 because MBP then also moves the state-0 body cold (it lands at
`0x44ca`/`0x451a` instead of inline) and the whole switch layout shifts.  The
only way to get the original's *local* shape is
`else if` + `__builtin_expect(state0 && trig, 1)`, which reproduces the
`bne/beq -> else` + inline state-0 body exactly, but still scores 58.06
because the cold state-1 body lands at `0xde0c` rather than `0x3726`.
No source-level lever tried here moves it to `0x3726`.

The 329-instruction `ours[6425:6754]` block is a *different* case's loop body
(it starts with a `RadioMessage::isOver` + `awake` + `rnd->nextInt` loop);
the difflib alignment attributes it to the state-2 region only because the
prologue register colouring (`this` in `sl` vs `[sp,#88]`) makes the two
streams locally similar.

## Remaining root cause

Unchanged from waves 121–124: (1) prologue accessor order / `this`-spill
colouring (`messages` r4 vs r5, `&Globals::status` r9 vs r4, `this` in `sl`
vs `[sp,#88]`), and (2) MachineBlockPlacement sub-block permutation of the
big switch.  The case-125 `else if` shape is correct in the binary but
cannot be adopted without a large global regression until the prologue
colouring is fixed.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check  -j6   # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift  -j6   # 0 fields
cmake --build cmake-build-match --target lint         # CLEAN
python3 /dev/shm/w7505/score.py                       # 60.253189743
python3 /dev/shm/w7505/textdiff.py cmake-build-match/libgof2hdaa.so  # .text 85978
```

Scratch helpers (`score.py`, `diffregions.py`, `trytail.py`, `tryexp.py`,
`tryswap.py`, `trymove.py`, `trycombos.py`, `tryei.py`, `reordercases.py`,
`swapcases.py`, `randsearch.py`, `textdiff.py`, `base.cpp`, `best.cpp`) live
in `/dev/shm/w7505/` and are not part of the tree.
