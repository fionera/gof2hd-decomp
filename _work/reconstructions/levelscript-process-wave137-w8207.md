# LevelScript::process — wave 137 / w8207 (case-145 state machine: cached player + state-5 advance)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `655e3bcc5` (wave-136 head, 58.9%).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.9
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54327             54371            (+44)
whole .text diffbytes    84709             84753            (+44)
.ARM.extab diffbytes     1395              1395
.ARM.exidx diffbytes     0                 0
.rodata / .data / .bss   0 / 0 / 0         0 / 0 / 0
.rel.plt diffbytes       4                 4   (size 29392 exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## What was landed

Three statement-level fixes, all re-verified against `objdump` of the original.
The net size is 0 (the two case-145 edits cancel, the case-102 move is free after
block placement); the function stays exactly 57232 B.

### 1. Case-145 state 1 — cached `player` receiver (orig `0x13941a`)

```
13941a  mov r0, sl          ; sl = player (loaded at 0x1393a4 from [sp,#84])
13941c  blx PlayerEgo::isInRocketControl
```

Source had `m_pLevel->getPlayer()->isInRocketControl()`, which compiles to an
extra `Level::getPlayer` call.  Changed to `player->isInRocketControl()`.
This is the same receiver fix wave-136/w8105 identified but reverted (the 4-site
batch re-rolled placement; the single case-145 site is neutral here).

### 2. Case-145 state 5 — missing `m_nState++` (orig `0x13f8f6`..`0x13f8fe`)

```
13f8f6  ldr.w r0,[r9,#28]   ; m_nState
13f8fa  strb.w r4,[r9,#17]  ; m_bCinematicBreak = 0
13f8fe  b.w 14500e          ; -> adds r0,#1 ; str r0,[r9,#28]
```

The source ended state 5 with `m_bCinematicBreak = 0;` and never advanced, so the
state machine stalled at state 5 forever.  Added `m_nState++;` after the flag
clear.  (This is the +8 B companion that keeps the function at 57232 B.)

### 3. Case-102 state 0/1 — `setTarget` / `translate` placement (orig `0x13a70e`, `0x13e880`)

The wave-136/w8106 note proved state 0 has `setTarget(enemies[0]->geometry)`
(0x13a70e) before `setPosition`, and state 1 has the `translate(delta*0.6,
delta*0.8, -delta*2.5)` (0x13e880).  The source had them swapped.  Moved.

## Case-145 statement audit (all states checked against Thumb)

The whole case-145 block was disassembled and compared call-by-call and
instruction-by-instruction.  Everything else already matches:

* pre-loop `step = (float)(delta << 1)`, `moveForward(step)`, and the
  `i != 0 && m_nState >= 2` / `m_nState != 4 || m_nScriptTimerA <= 1000` shoot
  gate — exact.
* state 0 (`0x1393b2`..`0x1393fc`): loop of `setEnemies(nullptr)` +
  `player->setEnemy(enemies[14]->player)`, `new Explosion(0)`, `addFireStreaks`,
  `setScaling(12.0f)`, `m_nState = 1` — exact.
* state 1 body (`0x1393fe`..`0x139540`): all 30 calls and the
  `m_tempVec = pos + dir*18000 + right*3000` sequence — exact apart from the
  cached-player receiver above.
* state 2 (`0x13cae0`): shoot, `new AEGeometry(Globals::Canvas)`,
  `guns[0][0]->velocities[0]`, `setDirection`, `setVisible(false)`,
  `setTarget(m_pGeometry4)` — exact.
* state 3 (`0x13f53c`): `pitchRate * delta`, `getMatrix`, `systemSetMatrix`,
  `enableSystemEmit(true)`, `Explosion::start(MatrixGetPosition, {0,0,0})` — exact.
* state 4 (`0x13f5fc`): `update`, `>= 324`, wreck/boss matrix copy, visible/active
  swap, `field_0x74 = 1`, `SetAnimationState(1)` — exact.
* state 5 (`0x13f6c6`): `timer += delta`, `Transform::Update((long long)(delta*0.3), false)`,
  the 13-ship direction/normalize/moveForward loop, `update`, `> 10000` tail
  (enableSystemEmit(false), 13-ship hide/setPosition(50000), resetGunDelay,
  setLookAtCam(false), setTarget, setComputerControlled(false), setVulnerable(true),
  setFreeze(false), setVisible(true), hud/radar, resetCamera, forceUpdate,
  radar nulls, `m_bCinematicBreak = 0`) — exact apart from the two items below.

## Open leads (measured, need a companion; not landed)

Two more genuine case-145 defects were found but do not fit an exact-size
landing on this base:

### A. `case 6: m_nState = 2; break;` is unreachable

The original's states-2..5 dispatch is `subs r0,r4,#2; cmp r0,#3; bhi tail;
tbh` (0x13cacc) — four entries, states 2..5 only.  State 6 falls to the tail.
Removing `case 6` is **-4 B** (57228) on master.

### B. Case-145 state 5 has a spurious `Globals::status->nextCampaignMission(true)`

Original `0x13f880`..`0x13f8a0`:

```
13f880  getPlayer ; resetGunDelay
13f88c  m_pCamera->setLookAtCam(false)
13f898  m_pCamera->setTarget(player->geometry)     <-- no nextCampaignMission between
13f8a4  player->setComputerControlled(false)
```

The `nextCampaignMission(true)` at 0x13f92e belongs to the adjacent `cmp m_nState,#16`
case, not case 145.  Removing it is **-8 B** (57224) on master.

Measured combinations (all size-exact except where noted):

| edit set | size | pct |
|---|---:|---:|
| **landed: cached player + state-5 advance + case-102 placement** | **57232** | **58.9** |
| + remove case 6 | 57220 | 58.0 |
| + remove nextCampaignMission | 57216 | 58.7 |
| + both | 57212 | 58.1 |
| + both + case-102 tail `resetCamera` (-16) | 57196 | 58.0 |
| cached player + state-5 advance only | 57232 | 58.9 |
| + case-154 state-2 `GetDirVector` reorder (byte-neutral) | 57232 | 58.8 |

So the two remaining case-145 fixes need a **+20 B** faithful companion to land
together (or +12 B for case-6 alone, +16 B for the message alone).  No such
positive lever was found in the time box; the known positive levers
(case-40 state-1 translate +36, case-64 full +24, case-154 state-6 creep +12)
are already landed on this base.

### C. Two-switch dispatch shape

The original's dispatch is a hybrid: `cmp r4,#1; beq state1; cbz r4,state0;
b states2+` followed by a separate `subs #2; cmp #3; bhi; tbh` for states 2..5.
A literal two-`switch` source reproduces that shape exactly (verified:
`cmp #1 / beq / cbz / b` then `subs #2 / cmp #3 / bhi / tbh[4]`) but costs
**+32 B** (57264) and re-rolls placement to 57.7 %.  A single `switch` (the
current source) lowers to one `tbh` over 0..5.  The two-switch form is also
*semantically* wrong for this source: after state 1 increments `m_nState` to 2,
the second switch would run state 2 in the same call, whereas the original
branches to the shared `m_nState++` tail at 0x13edde and returns.  The original
is therefore a single switch whose fallthrough cluster {0,1} happens to be
lowered as compares; reproducing that exact lowering from C++ was not found.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
```

Scratch helpers in `/dev/shm/w8207/` (not tracked): `score.py` (process pct/size),
`apply.py` (variant applier), `calldiff2.py` (blx-sequence diff), `seqdiff.py`
(instruction diff), `orig.dis`/`ours.dis`.
