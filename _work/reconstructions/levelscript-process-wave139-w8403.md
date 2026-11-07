# LevelScript::process — wave 139 / w8403 (case-125 state-2 tail + three faithful receiver/statement fixes)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `bf123bd47` (wave-138 head).
Scratch helpers in `/dev/shm/w8403/` (not tracked): `fdiff.py` (raw same-address diff),
`bucket.py`, `calls.py` (whole-function call census), `callseq.py`, `mk3.py`
(named-edit harness), `measure.sh`, `sweep*.sh`.

## Result

```
                         before (master)   after (landed)
process raw diffbytes    55,072            55,035            (-37)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    85,476            85,439            (-37)
.ARM.extab diffbytes     1,396             1,396             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt / .got / .dynsym / .dynstr / .hash / .rel.dyn   exact   exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The function stays exactly 57,232 B, so every later function keeps its original
address and `byte_exact`/`linked_exact` stay 4507. `.rel.plt` stays byte-identical.

## The landed set (verified against objdump of the original)

### 1. `Radar::dockNavPtr` zeroed with `lockedEnemy` (case 94 state 7, source line ~3062)

Original `0x140280..0x140284`:

```
140280  ldr.w r0, [r5, #212]     ; m_pRadar
140284  strd  r8, r8, [r0, #4]   ; lockedEnemy = dockNavPtr = nullptr   (8-byte store)
```

`Radar` is `level@0, lockedEnemy@4, dockNavPtr@8`, so the `strd` zeroes **both**
pointers. The source only wrote `m_pRadar->lockedEnemy = nullptr;`. Adding
`m_pRadar->dockNavPtr = nullptr;` is **size-neutral** (the compiler coalesces the
pair into the same `strd`).

### 2. `m_pLevel->getPlayer()->resetGunDelay()` in the same block (0x140288..0x14028c)

```
140288  ldr  r0, [r5, #24]       ; m_pLevel
14028a  blx  r4                  ; Level::getPlayer
14028c  blx  PlayerEgo::resetGunDelay
```

The source used the cached `player->resetGunDelay()`. Restoring the fresh
`m_pLevel->getPlayer()` receiver is **+8 B** in isolation.

### 3. Two `m_pLevel->getPlayer()->geometry` receivers that the original spells `player->geometry`

* `0x142d56` (case 90 state 10, source line ~3806): `*camPos += player->geometry->getPosition();`
  — original `ldr r1,[r8,#8]` with `r8 = player`, **no** `getPlayer` call.
* `0x144dc2` (case 90 state 12, source line ~3857):
  `player->geometry->setDirection(-dir, Vector{0,1,0});` — original `ldr r4,[r4,#8]`
  with `r4 = player`, **no** `getPlayer` call.

Each is **-8 B** in isolation.

### Size accounting

| edit | Δ size |
|---|---:|
| (1) `dockNavPtr = nullptr` | 0 |
| (2) fresh `getPlayer()` for `resetGunDelay` | +8 |
| (3a) `player->geometry->getPosition()` | -8 |
| (3b) `player->geometry->setDirection()` | -8 |
| **net measured** | **0** (57232 B) |

Measured: `Q` alone 57248 (+16, raw 54,920); `R1` alone 57224 (-8, raw 55,025);
`R2` alone 57224 (-8, raw 55,076); `Q R1 R2` **57232, raw 55,035**.

## Case-125 state machine — verified faithful, no source delta left

The whole case-125 chain was re-disassembled against the original this wave
(`0x139f9c` state 0 inline, `0x13b43e` state 1, `0x13b518` state 2, `0x13dcb6`
state 3, `0x13de44` default 4..6). Every statement and every receiver matches:

* state 0: `m_nState++` **before** the `routePoints[3] = {-70000,0,-130000}`
  literal, `new Route(...,3)`, `setPlayerRoute`, `setRoute`, `m_nScriptTimerA = 0`
  (`strd r0,r0,[r4,#144]`).
* state 1: `getRoute()` called **twice**, `getWaypoint(0)->state`, `m_nState = 2`
  (immediate `movs r0,#2`), the 8..10 port loop with `setName(*getText(3212) +
  String(" ",false) + String(i-7))`, `setActive(true)`, `field_0x74 = 0`.
* state 2: `isOver()`, the 2..5 ambush loop in the original declaration order
  (`awake, pirate, playerPos, sideX, offsetY, offsetZ, sideZ`), then the teardown
  `setTurretMode, resetCamera, if(rocket), setFreeLookMode, enableFirstPersonCam,
  hideShip, stopShooting, m_bCinematicBreak=1, hud->visible=0, radar->enabled=0,
  setLookAtCam(true), setVisible(false), setFreeze(true), leader=…, setTarget,
  m_tempVec=…`, and the tail `setPosition(m_tempVec); m_nScriptTimerA = 0;
  m_nState++` (the `m_nState++` is the shared tail at `0x144a0c`).
* state 3: `step = (float)(2*delta)`, `moveForward` loop, `m_nScriptTimerA += delta`,
  `>= 7001`, `setAIDisabled(false)` loop, **`m_nState = 4` immediate store**
  (`movs r0,#4; str r0,[r5,#28]` at `0x13dd22`), `setVisible(true)`,
  `setFreeze(false)`, `getPlayer()->setAutoPilot`, `isDockedToDockingPoint`,
  `getPlayer()->resetGunDelay`, `getPlayer()->dockToDockingPoint`,
  `geometry->setDirection(normalize(...), {0,1,0})`, camera reframe, `setComputerControlled`,
  `hud->visible=1`, `radar->enabled=1`, `resetCamera`, `forceUpdate`, `m_bCinematicBreak=0`.
* default (4..6): `(unsigned)(m_nState-4) <= 2` guard (`subs r0,#4; cmp r0,#2; bhi`),
  `isDockedToDockingPoint`, `hackingWon`, `getHackingGameDockIndex >= 0`,
  `setDockingType(0)`, `setName(String("",false))`, `field_0x74 = 1`,
  `deleteHackingGame`, `m_nState++`, `if (m_nState == 6)` 6..7 guard loop.

The only remaining case-125 codegen differences are **block placement** (ours
outlines state 1's `getRoute` prologue and orders the switch compares 3,2,1
descending; the original keeps state 1 cold and compares 1,2,3 ascending), which
is `MachineBlockPlacement`, not source.

## Negative results (measured on this base; do not re-run)

| variant | size | raw diff |
|---|---:|---:|
| baseline (HEAD) | 57232 | 55,072 |
| faithful ambush-loop declaration order (`awake,pirate,playerPos,sideX,offsetY,offsetZ,sideZ`) | 57216 | 55,036 |
| `m_nState = 3` in state-2 tail (immediate, not `++`) | 57236 | 54,800 |
| `m_nScriptTimerA = 0` before `m_nState++` swap | 57232 | 55,072 (no codegen change) |
| `setPosition(m_tempVec.x, m_tempVec.y, m_tempVec.z)` in state-2 tail | 57240 | 55,009 |
| `(int)(camPos.z + 30000.0f)` routePoints fold | 57240 | 55,022 |
| missing `m_pLevel->getPlayer()->resetGunDelay()` at `0x144982` | 57244 | 54,788 |
| `m_nState = m_nState + 1` in state-1 (`movs #2` original) | 57232 | 55,060 |
| `m_nState - 4 <= 2` signed guard | 57232 | 55,059 |
| `Q R1 R2` + the two above | 57232 | 55,010 |
| `Q R1 R2` + faithful loop + `resetGunDelay` + `m_nState = 3` | 57232 | 55,053 |
| `Q R1 R2` + `m_pLevel->getPlayer()->setCollide(false)` (`0x138d90`, +8) | 57240 | 55,155 |

The `setCollide` receiver at source line ~740 is the **next** genuine faithful
delta (`player->setCollide(false)` → `m_pLevel->getPlayer()->setCollide(false)`,
original `0x138d86..0x138d96`, +8 B); it needs a -8 companion to land.

## Remaining call-census deltas after this landing

```
orig ours  symbol
  34   28  Level::getEnemies            (the three known uncached loops, +8)
  23   21  Level::getPlayer             (case-154 resetGunDelay +8; line-740 setCollide +8)
  10    9  PlayerEgo::resetGunDelay     (case-154 teardown, +12)
  23   24  AEGeometry::setPosition(Vector&)   \  one extra block in ours,
   9   10  ApplicationManager::SetCurrentApplicationModule  |  all +1 together
  51   52  LevelScript::resetCamera      |
  81   82  TargetFollowCamera::setTarget |
  57   58  TargetFollowCamera::translate |
  41   42  PlayerEgo::getPosition       /
```

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target drift -j6      # 0 fields
```
