# LevelScript::process — wave 139 / w8405 (restore_rgd landed + cached-player companion + state-9 tail)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bf123bd47` (wave-138 head).

## Result

```
                         before (master)   after (landed)
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        55,072            54,740           (-332)
whole .text diffbytes    85,476            85,144           (-332)
.ARM.extab diffbytes     1,396             1,396            (size 156,312)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 byte-identical    byte-identical
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## 1. restore_rgd is +12 B on this base, not +8

w8201 recorded the missing `m_pLevel->getPlayer()->resetGunDelay()` at `0x144982` as +8 B.
On the wave-138 head it is **+12 B**: the original emits the full 12-byte sequence

```
144970  ldr r0,[sl,#20]        ; m_pCamera
144974  movs r1,#0
144976  blx TargetFollowCamera::setFixed
14497a  ldr r0,[sl,#24]        ; m_pLevel
14497e  blx Level::getPlayer
144982  blx PlayerEgo::resetGunDelay
144986  ldr r0,[sl,#20]        ; m_pCamera
14498a  movs r1,#0
14498c  blx TargetFollowCamera::setLookAtCam
```

(`ldr` + `blx getPlayer` + `blx resetGunDelay` = 12 B; the preceding `setFixed` uses
`[sl,#20]`, so `m_pLevel` is not live and must be reloaded).  The cached form
`player->resetGunDelay()` is only +8 B but is *not* what the binary does — the original
re-fetches through `Level::getPlayer`.  So the companion must be **-12 B**, not -8.

## 2. The companion: cached `player` in case-154 state-10 (-8) + state-9 tail (-4)

### 2a. case-154 state-10 `*camPos += ...` (-8 B)

Source line ~3805 wrote `m_pLevel->getPlayer()->geometry->getPosition()`.  The original
uses the cached `player` local (spilled at `[sp,#0x54]`, kept in `r8`):

```
142d56  ldr r1,[r8,#8]        ; player->geometry
142d5a  add r8, sp, #280
142d5e  mov r0,r8
142d60  blx AEGeometry::getPosition
```

Replacing it with `player->geometry->getPosition()` removes the
`ldr r0,[sl,#24]; blx Level::getPlayer` pair (-8 B) and makes the block instruction-for-
instruction identical to the original (verified at `0x142d0c..0x142d44`).

### 2b. case-154 state-9 tail: `m_bCinematicBreak = 0`, drop `m_nScriptTimerB = 0` (-4 B)

The original tail is

```
1449f2  ldrb r0,[sl,#170]     ; field_0xaa
1449f6  strb r4,[sl,#17]      ; m_bCinematicBreak = 0   <-- before the if
1449fa  cbz r0, 144a04
1449fc  ldr r0,[sp,#84]
1449fe  movs r1,#1
144a00  blx PlayerEgo::setAutoTurret
144a04  strd r4,r4,[r9]       ; m_nScriptTimerA = 0
144a08  ldr r0,[sl,#28]       ; m_nState++
144a0c  adds r0,#1
144a0e  str r0,[sl,#28]
```

The source had `m_nScriptTimerB = 0;` (a `strd r4,r4,[sl,#152]`, which the original never
emits here) and was missing `m_bCinematicBreak = 0;`.  The fix is a genuine bug fix as well
as a size lever: `m_bCinematicBreak` is now cleared on the state-9 -> 10 transition, and the
spurious 64-bit timer-B zero-store is gone.  Net -4 B.

## 3. Size accounting

| edit | Δ size |
|---|---:|
| `restore_rgd` (`m_pLevel->getPlayer()->resetGunDelay()`) | +12 |
| `gp10` (state-10 cached `player`) | -8 |
| `cine9` (state-9 tail `m_bCinematicBreak = 0`, drop timer B) | -4 |
| **net** | **0** (measured 57232) |

The three edits together also improve the raw diff by 332 B (the state-9 block at
`0x144b50..0x144b6c` and the state-9 tail at `0x144bce..0x144be4` now match the original
byte-for-byte; the state-10 block matches modulo register choice).

## 4. Negative / rejected levers

Measured on this base (each applied alone):

| edit | Δ size | note |
|---|---:|---|
| `gp12` (case-154 state-12 cached `player`) | -8 | faithful, but `rgd+gp12` = 57236 (needs -4) |
| `rgd7` (state-7 `m_pLevel->getPlayer()->resetGunDelay()`) | +8 | faithful (orig has 3 getPlayer calls there) |
| `docknav` (state-7 `m_pRadar->dockNavPtr = nullptr`) | 0 | faithful, size-neutral |
| `rmtimer` (state-7 drop spurious `m_nScriptTimerA = 0`) | -8 | faithful |
| `turret` (drop auto-turret PLT anchor) | -16 | re-orders `.rel.plt` |
| `anchor` (drop Standing PLT anchor) | -8 | re-orders `.rel.plt` |

A subset sweep over `{gp10,gp12,rgd7,docknav,rmtimer,turret,anchor}` + `rgd` produced **no**
exact 57232 B combination (all sizes were 4-aligned but none hit 57232).  The `cine9` edit
supplies the missing -4 and lands exactly.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w8405/` (not tracked): `fdiff.py` (raw byte diff), `apply.py`
(named-edit harness), `callspan.py`, `idiff.py`, `orig.dis`/`ours.dis`.
