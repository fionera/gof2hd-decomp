# LevelScript::process — wave 135 / w7770 (case-102 state-0/1 statement placement + cached-player receivers)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bbffef482` (wave-134 head).

## Result

```
                         before (master)   after (landed)
official verify pct      58.07             58.12
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54359             54383            (+24)
whole .text diffbytes    85022             85046            (+24)
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

The raw byte-diff rises by 24 B because moving the state-0 `translate` into the
state-1 cold block re-rolls `MachineBlockPlacement`, but the structural (fuzzy)
ratio improves 58.07 -> 58.12 and three genuine statement-placement bugs are fixed.
Section sizes and `.rel.plt` are unchanged.

## Landed changes (all checked against objdump of the original)

### 1. Case 102 state 0 — add the missing `setTarget`, drop the misplaced `translate`

Original state 0 (`0x13a658`..`0x13a76a`):

```
13a6fc  m_pCamera->setLookAtCam(true)
13a70e  m_pCamera->setTarget((*enemies)[0]->geometry)     <-- was missing
13a712  m_tempVec = (*enemies)[0]->geometry->getPosition()
13a73e  m_pCamera->setPosition(m_tempVec + Vector{9000,-7000,40000})
13a74e  sound->stop(currentMusicEvent)
13a760  sound->play(2240)
13a766  m_nScriptTimerA = 0
13a76a  b.w 143d0c  (m_nState++)
```

There is **no** `translate` in state 0. The source had the `translate` here and
the `setTarget` in state 1.

### 2. Case 102 state 1 — the `translate` belongs here

The state-1 `tbh` entry decodes to `0x13e880` (base `0x13e822` + 2*0x2f):

```
13e880  ldr r5,[sp,#76]                       ; delta
13e882  vmov.f32 s2,#4                        ; 2.5
13e886  vldr s6,[pc,#-16]  (0.6)
13e88c  vldr s8,[pc,#-20]  (0.8)
13e8bc  m_pCamera->translate(delta*0.6, delta*0.8, -delta*2.5)
13e8c0  if (messages[2]->isOver()) m_nScriptTimerA += delta
13e8e0  b.w 143b1e  (timer >= 2001 check)
```

So the `translate` moves from state 0 to the first statement of state 1; the
state-1 `setTarget` was spurious (the only `setTarget` in this case is state 0's).
This also removes the last extra `setTarget` call (orig 82 / ours 83 before).

### 3. Case 102 state 0 and case 145 state 1 — use the cached `player` receiver

Original both sites pass the already-cached `player` local (`r5`/`sl` loaded from
`[sp,#84]`) to `PlayerEgo::isInRocketControl`, not `m_pLevel->getPlayer()`:

```
13a688  mov r0,r5 ; blx PlayerEgo::isInRocketControl     ; case 102 state 0
13941a  mov r0,sl ; blx PlayerEgo::isInRocketControl     ; case 145 state 1
```

Source now spells `player->isInRocketControl()` at both sites. This is the -8 B
size compensator that keeps `process` at exactly 57232 B after change 2.

### 4. Case 154 state 2 — `GetDirVector` must precede the first `+=`

Original (`0x142fb6`..`0x143018`):

```
142fb6  m_dirVec = player->GetDirVector()
142fba  m_tempVec += m_dirVec * (step * 0.008f)
142ff6  m_dirVec = player->geometry->getRightVector()
143000  m_tempVec += m_dirVec * (step * -0.05f)
14301c  m_dirVec = player->GetUpVector()
```

The source applied the stale `m_dirVec` to the first term and then overwrote it
with `GetDirVector()` (a dead assignment). Reordered to the faithful sequence.
This change is byte-neutral (clang's scheduler already hoisted the call), but it
removes a real source-level bug; size and diffbytes are unchanged by it alone.

## Size accounting (measured on the pristine master)

| edit | delta |
|---|---:|
| case-102 state-0/1 `setTarget` + `translate` move | +8 |
| case-102 `player->isInRocketControl()` | -4 (in this context) |
| case-145 `player->isInRocketControl()` | -4 (in this context) |
| case-154 state-2 reorder | 0 |
| **net** | **0** |

## Open leads (not landed)

* `LevelScript::process` still has one extra `getPlayer()` call vs the original
  (25 vs 27 after these fixes). The mission==1 intro `m_pLevel->getPlayer()->rotate`
  is one candidate, but switching it to the cached `player` local extends its live
  range and grows `process` by 120 B (57232 -> 57352), so it needs a companion.
* One extra `setPosition(Vector)` (orig 82 / ours 83) and one missing
  `TargetFollowCamera::translate` (orig 57 / ours 56) remain. The missing translate
  is not in cases 102/125/145/154 (all four were reconstructed statement-by-statement
  against the objdump in this pass).
* The case-125 `if (delta < 0)` PLT anchor and the case-40 `if (delta == 0x7d)`
  anchor still account for +1 `getStarSystem`/`getLandmarks`/`setStanding` and the
  duplicated particle/sound first-reference calls; they are deliberate `.rel.plt`
  scaffolding.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
```

Scratch helpers in `/dev/shm/w7770/` (not tracked): `score.py` (process size /
diffbytes), `ratio.py` (verify fuzzy ratio), `percase2.py` (per-case diff map),
`orig_proc.dis`, `ours_proc.dis`.

## Post-merge note (wave 135 coordinator)

The source change was **not landed**: on the w7763+w7768 base it grows
`process` to 57,240 B (+8) and needs a size companion.  The case-102/145/154
statement placement is semantically right and remains a lead.
