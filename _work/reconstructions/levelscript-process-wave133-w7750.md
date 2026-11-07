# LevelScript::process — wave 133 / w7750 (case-64 state-1 body + case-154 state-1/2/9/10 exact; 57232 B)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `ec0acea1e` (wave-133 docs head).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.2
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54465             54407            (-58)
whole .text diffbytes    85143             85085            (-58)
.ARM.extab diffbytes     1416              1416
.ARM.exidx diffbytes     0                 0
.rodata / .data          0 / 0             0 / 0
.rel.plt diffbytes       0                 0   (byte-identical)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The official fuzzy percentage moves 58.9 -> 58.2 because the corrected cold
blocks re-roll `MachineBlockPlacement`; the authoritative byte diff improves by
58 B in `process` and by 58 B in the whole `.text`.  Section sizes and
`.rel.plt` are unchanged.

## Landed changes (all checked against objdump of the original)

### 1. Case 64 state 1 — restore the leading `forceUpdate` + `translate` (orig `0x13ecfc`)

The original body starts with two statements that were missing:

```
13ecfc  m_pLevel->lodManager->forceUpdate(delta, false)
13ed0e  vmov s0, r5 ; vcvt.f64.s32 ; vmul.f64 d16, d16, 0.2 ; vcvt.f32.f64
13ed2e  m_pCamera->translate(0.0f, delta * 0.2, 0.0f)
13ed32  messages[2]->isTriggered()
```

The translate is **double** math (`(double)delta * 0.2`), not `(float)delta * 0.2f`.
Source now spells `m_pCamera->translate(0.0f, delta * 0.2, 0.0f);` so clang emits
`vcvt.f64.s32 / vmul.f64 / vcvt.f32.f64` exactly as the original.  The rest of the
state-1 body (setLookAtCam, setTarget, hud/radar, resetCamera, forceUpdate,
`m_bCinematicBreak = 0`, `player->freeze = 0`, setRoute loop, `m_nState = 2`) was
already in the original order.

### 2. Case 154 state 1 — timer increment and faithful tail (orig `0x13e956`, `0x144444`)

The state-1 body tested `messages[1]->isOver()` but called `setFixed(false)`
instead of advancing the timer.  The original body is:

```
13e956  if (messages[1]->isOver()) m_nScriptTimerA += delta
```

The tail (`0x144444`) is:

```
for shipGroup == 9: setActive(false); setVisible(false)
m_pCamera->setTarget(player->geometry)
m_tempVec = player->getPosition()
m_tempVec += {-350, 400, -1500}
m_pCamera->setPosition(m_tempVec)
m_pCamera->setFixed(false)
m_nState++
m_pLevel->lodManager->forceUpdate(delta, false)
m_nScriptTimerA = 0
```

Our source had the `+=`/`setPosition` after the state/forceUpdate/timer stores,
was missing `setFixed(false)`, and had a spurious `(*enemies)[0]->setActive(true)`.
All fixed.

### 3. Case 154 state 2 — creep order + tail (orig `0x142f34`, `0x144524`)

The state-2 creep had the same dead-assignment bug as states 4/6: it applied the
stale `m_dirVec` to the first term and then overwrote it with `GetDirVector()`.
The original order is `GetDirVector -> +=dir*0.008 -> getRightVector ->
+=right*-0.05 -> GetUpVector -> +=up*0.0051`.

The state-2 tail was also wrong: it had a spurious `m_bCinematicBreak = 0`, a
duplicate `setTarget`, no `player->setSpeed(0.0f)`, no `m_nScriptTimerA = 0`, and
`m_nState = 3`.  The original tail is:

```
player->setSpeed(0.0f)
m_pCamera->setFixed(false)
m_pCamera->setTarget(enemies[1]->geometry)
m_tempVec = enemies[1]->geometry->getPosition()
m_tempVec += {0, 4000, 26000}
m_pCamera->setPosition(m_tempVec)
m_nScriptTimerA = 0
m_pLevel->lodManager->forceUpdate(delta, false)
m_nState++
```

### 4. Case 154 state 3/4/5/6 tails — `m_nState++` (orig `0x144640`, `0x144a08`, `0x14473e`, `0x144840`)

The original advances these states with a load/add/store (`m_nState++`), not a
constant store.  Source now uses `m_nState++` in the state-3, state-4, state-5
and state-6 tails.  The state-4 and state-6 tails already had the correct
`setTarget`/`getPosition`/`+=`/`setPosition`/timer order from w7723.

### 5. Case 154 state 5 body — drop the spurious `m_nState = 5` (orig `0x1432f8`)

The original `if (messages[5]->isOver())` body only does
`m_nScriptTimerA += delta`; it does not store the state.  Removed.

### 6. Case 154 state 9 tail — remove spurious `setSpeed(0)`, fix timerB, advance state (orig `0x1449f2`..`0x144a08`)

Original tail:

```
m_pLevel->lodManager->forceUpdate(delta, false)
if (field_0xaa) player->setAutoTurret(true)
m_nScriptTimerA = 0
m_nScriptTimerB = 0          ; strd r4,r4,[r9] with r9 = &m_nScriptTimerA
m_nState++                   ; shared block 0x144a08
```

Our source had a spurious `player->setSpeed(0.0f)`, zeroed `m_nScriptTime`
(offset 0x8) instead of `m_nScriptTimerB` (offset 0x98), and never advanced the
state (the mission stalled at state 9).  All fixed.

### 7. Case 154 state 10 — move `delete objectivesB`, restore `m_nScriptTime = 0` (orig `0x143562`, `0x1435a2`)

Original first block:

```
m_nScriptTime = 0            ; strd r0,r0,[this,#8]
m_nTimeLimit = 91000
m_pLevel->objectivesB = new Objective(3, 91000, m_pLevel)
```

The `delete m_pLevel->objectivesB;` belongs to the second block, immediately
after `m_nState = 11` (orig `0x1435aa`).  Our source had the delete in the first
block, no `m_nScriptTime = 0`, and a spurious `m_nScriptTimerA = 0` in the second
block.  Fixed.

### 8. Case 92 state 2 — `m_nState = 10`, not `++` (orig `0x13bd1e`) — size companion

The original at `0x13bd1e` is `movs r0,#10 ; str [sl,#28]`, i.e. a constant
store, not the shared `m_nState++` block.  Source now spells `m_nState = 10;`.
This is a genuine faithfulness fix and supplies the final -8 B needed to keep
`process` at exactly 57232 B.

## Size accounting (measured on the landed base)

| edit | delta |
|---|---:|
| case-64 state-1 `forceUpdate` + double `translate` | +64 |
| case-154 state-2 creep + tail | -40 |
| case-154 state-3/5 tails + state-5 body | -16 |
| case-154 state-9 tail | -4 |
| case-154 state-10 tail | -4 |
| case-92 `m_nState = 10` | -8 |
| case-154 state-1 body/tail | 0 |
| **net** | **0** |

The case-154 state-1 edit is net-zero because it removes one `setFixed` call and
one `setActive` call while adding one `setFixed` call and one 64-bit timer add.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
```

Scratch helpers in `/dev/shm/w7750/` (not tracked): `score.py` (process size /
diffbytes / pct), `orig.dis`, `ours_full.dis`.
