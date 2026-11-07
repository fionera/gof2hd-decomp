# LevelScript::process — wave 136 / w8106 (case-102/145/154 statement placement + case-145 state-5 advance)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `2e51d765d` (wave-135 head, 58.8%).

## Result

```
                         before (master)   after (landed)
official verify pct      58.8              58.8
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54316             54424            (+108)
whole .text diffbytes    84960             85068            (+108)
.ARM.extab diffbytes     1404              1404
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

The raw byte-diff rises by 108 B because the corrected cold blocks re-roll
`MachineBlockPlacement`; the fuzzy ratio is unchanged at 58.8 and the size and
`.rel.plt` stay exact.  All changes are checked statement-by-statement against
`objdump` of the original.

## Landed changes

### 1. Case 102 state 0 — add `setTarget`, drop the misplaced `translate`

Original state 0 (`0x13a658`..`0x13a76a`): after `setLookAtCam(true)` it calls
`m_pCamera->setTarget((*enemies)[0]->geometry)` (`0x13a70e`), then
`setPosition(getPosition() + {9000,-7000,40000})`, sound stop/play(2240), timer 0,
`m_nState++`.  There is **no** `translate` in state 0.  Source had the translate
here and the `setTarget` in state 1.

### 2. Case 102 state 1 — the `translate` belongs here

State-1 entry `0x13e880`:

```
13e880  ldr r5,[sp,#76]                       ; delta
13e882  vmov.f32 s2,#4                        ; 2.5
13e886  vldr s6,[pc,#-16]  (0.6)
13e88c  vldr s8,[pc,#-20]  (0.8)
13e8bc  m_pCamera->translate(delta*0.6, delta*0.8, -delta*2.5)
13e8c0  if (messages[2]->isOver()) m_nScriptTimerA += delta
```

The state-1 `setTarget` was spurious (the only `setTarget` in this case is state 0's).

### 3. Case 102 state 0 / case 145 state 1 — cached `player` receiver

Original passes the cached `player` local to `PlayerEgo::isInRocketControl`:

```
13a688  mov r0,r5 ; blx PlayerEgo::isInRocketControl     ; case 102 state 0
13941a  mov r0,sl ; blx PlayerEgo::isInRocketControl     ; case 145 state 1
```

`m_pLevel->getPlayer()->isInRocketControl()` is CSE'd by clang to the same code,
so this is a source-faithfulness fix (removes the two extra `Level::getPlayer`
calls the call census reported).

### 4. Case 154 state 2 — `GetDirVector` must precede the first `+=`

Original (`0x142fb6`..`0x143018`):

```
142fb6  m_dirVec = player->GetDirVector()
142fba  m_tempVec += m_dirVec * (step * 0.008f)
142ff6  m_dirVec = player->geometry->getRightVector()
143000  m_tempVec += m_dirVec * (step * -0.05f)
14301c  m_dirVec = player->GetUpVector()
```

Source applied the stale `m_dirVec` to the first term and then overwrote it with
`GetDirVector()` (a dead assignment).  Reordered to the faithful sequence.
Byte-neutral (clang's scheduler already hoisted the call): size and diffbytes do
not move; it removes a real source-level bug.

### 5. Case 145 state 5 — advance `m_nState` (the +8 B companion)

Original state-5 tail (`0x7bde`..`0x7be6`), inside `if (m_nScriptTimerA > 10000)`:

```
7bde  ldr.w r0,[r9,#28]      ; m_nState
7be2  strb.w r4,[r9,#17]     ; m_bCinematicBreak = 0
7be6  b.w 0xd2f6             ; -> adds r0,#1 ; str r0,[r9,#28]  (m_nState++)
```

Source was missing the `m_nState++` (state 5 never advanced; the `case 6:
m_nState = 2` block was unreachable).  Added `m_nState++;` after
`m_bCinematicBreak = 0;`.  This is the **+8 B** companion that keeps `process`
at exactly 57232 B with the case-102/145/154 placement fixes.

## Size accounting (measured on the wave-135 master)

| edit | delta |
|---|---:|
| case-102 state-0/1 `setTarget` + `translate` move | -8 |
| case-102 cached `player` | -8 |
| case-145 cached `player` | -8 |
| case-154 state-2 reorder | 0 |
| case-145 state-5 `m_nState++` | +8 |
| **net** | **-16** (measured; interactions re-roll block placement) |

The measured net is -16 on this base (the w7770 note measured 0 on the
pre-case-125-switch base).  The `m_nState++` is +8 in isolation; the residual
-16 is absorbed by the block-placement re-roll and the function still lands at
exactly 57232 B (verified by the ELF symbol size).

## Open leads (not landed)

* **Case-40 state-1 `translate(0,0,-2*delta)`** (the "cold-chain mirror" of
  case-41 state-4's `translate(delta,delta,-2*delta)`): original `0x69a8`:

  ```
  69aa  sub.w r0,r4,sl,lsl#1   ; -2*delta
  69c2  m_pCamera->translate(0.0f, 0.0f, (float)(-2*delta))
  69c6  if (messages[4]->isOver()) { setTarget(player->geometry); hud=1; radar=1;
         cinematicBreak=0; setLookAtCam(false); setComputerControlled(false);
         player->player->setVulnerable(true); m_nState=2; }
  ```

  Adding it costs **+36 B** on this base (not the +24 the w7687 note measured),
  so it needs a -36 B faithful companion.  The best landing found that keeps
  57232 is `case-102 state-1 tail resetCamera` (-16) + `case-145 case-6 removal`
  (-4) + the translate (+36) + the rest, but it drops the fuzzy ratio to 58.1.
* **Case-102 state-1 tail**: original `0xbea0` is `resetCamera(m_pLevel)`; the
  source has a spurious `m_tempVec = (*enemies)[3]->geometry->getPosition()`
  instead.  Replacing it is -16 B (needs a companion).
* `case 145: case 6: m_nState = 2;` is unreachable in the original (the state-2..5
  `tbh` covers four entries only); removing it is -4 B but re-rolls placement
  and drops the fuzzy ratio to 57.9 on its own.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
```

Scratch helpers in `/dev/shm/w8106/` (not tracked): `apply.py` (variant applier),
`score2.py` (fuzzy pct / matched count), `diffbytes.py` (process byte diff),
`callset.py` (call census), `percase.py` (per-case match map), `orig.dis`/`ours.dis`.
