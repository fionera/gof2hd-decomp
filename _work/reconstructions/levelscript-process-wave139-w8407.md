# LevelScript::process — wave 139 / w8407 (getEnemies re-fetch loops + missing resetGunDelay)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bf123bd47` (wave-138 head).
Scratch helpers in `/dev/shm/w8407/` (not tracked): `diff.py` (address-carrying
normalised instruction diff), `edits.py` (named-edit harness), `toggle.py`
(single-edit neighbourhood sweep), `fdiff.py` (raw same-address byte diff).

## Result

```
                         before (master)   after (landed)
process raw diffbytes    55,072            54,404   (-668)
whole .text diffbytes    85,476            84,808   (-668)
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
.ARM.extab diffbytes     1,396             1,396   (size 156,312 exact)
.ARM.exidx               exact             exact
.rodata / .data / .bss   exact             exact
.rel.plt                 byte-identical    byte-identical (29,392 B)
.note.gnu.build-id       20                20
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
address and `byte_exact`/`linked_exact` are unchanged.  The raw process diff
improves by 668 bytes (the whole `.text` delta moves by the same amount), and
`.rel.plt` stays byte-identical.

## The landed set (all checked against objdump of the original)

### 1. Case 16 state 3 loop — re-fetch `getEnemies()` for the element

Original loop `0x13f1f4`:

```
13f1f4  blx Level::getEnemies
13f1f8  ldr r0,[r0,#4]          ; enemies->data
13f1fa  ldr.w sl,[r0,fp,lsl #2] ; enemies[i]
...
13f2dc  ldr r5,[sp,#80]         ; &m_pLevel
13f2e0  blx Level::getEnemies   ; condition re-fetch
13f2e4  ldr r1,[r1,#0]          ; count
13f2ea  cmp fp,r1
13f2ec  bcc 13f1f4
```

Our source had `KIPlayer *ship = (*enemies)[i];` (cached array).  Changing the
body to `(*m_pLevel->getEnemies())[i]` reproduces the original's per-iteration
element re-fetch.  (The loop condition keeps the cached `enemies->count`, which
is what the compiler emits here; the original's separate condition call is
absorbed by the surrounding block layout.)

### 2. Case 16 state 4 loop — same re-fetch

Original loop `0x140578`/`0x14059c` calls `getEnemies()` both for the element
(`0x140578`) and for the count (`0x14059c`).  Our source used the cached array
for the element.  Changed to `(*m_pLevel->getEnemies())[i]`.

### 3. Case 24 state 0 loop — same re-fetch

Original loop `0x13ea4c`/`0x13ea62` calls `getEnemies()` for the element and for
the count.  Our source used `(*enemies)[j]`.  Changed to
`(*m_pLevel->getEnemies())[j]`.

### 4. Case 105 dock block — use the cached `player` local

Original `0x14026a..0x14029a` loads the `Level::getPlayer` GOT slot **once** into
`r4` and calls it through `blx r4` for `setAutoPilot`, `resetGunDelay` and
`dockToDockingPoint`.  Our source called `m_pLevel->getPlayer()` a second time
for `dockToDockingPoint`, emitting an extra direct `getPlayer@plt`.  Changed to
the cached `player` local (PLT-safe: `getPlayer` is already referenced earlier
in the TU, and `.rel.plt` is unchanged).

### 5. Case 105 camera offset — use the cached `player` local

Original `0x142d24` loads `this->m_pLevel` and calls `getPlayer` once for the
`geometry->getPosition()` offset.  Our source had
`m_pLevel->getPlayer()->geometry->getPosition()` in a block that already holds
the `player` local.  Changed to `player->geometry->getPosition()` (PLT-safe,
−4 B).

### 6. Case 154 state 9 — the missing `resetGunDelay()`

Original `0x14497a..0x14498c`:

```
144976  blx TargetFollowCamera::setFixed(false)
14497a  ldr.w r0,[sl,#24]        ; m_pLevel
14497e  blx Level::getPlayer
144982  blx PlayerEgo::resetGunDelay
144986  ldr.w r0,[sl,#20]        ; m_pCamera
14498c  blx TargetFollowCamera::setLookAtCam(false)
```

Our source went straight from `setFixed(false)` to `setLookAtCam(false)`.  Added
`m_pLevel->getPlayer()->resetGunDelay();` between them.  This is the call
w8201/w8307 identified as the 10th `resetGunDelay` (ours had 9).

## Size-neutrality

Measured individually on the wave-138 head:

| edit | Δ size |
|---|---:|
| case-16 state-3 body re-fetch | −8 |
| case-16 state-4 body re-fetch | +8 |
| case-24 state-0 body re-fetch | +8 |
| case-154 `resetGunDelay` | +12 |
| case-105 `dockToDockingPoint` cached player | −8 |
| case-105 camera offset cached player | −4 |
| **net** | **0** |

The combination is exactly 57,232 B.  Single-edit neighbourhood sweeps
(`toggle.py`) over `{L3,L4,L24,C3,C4,C24,D,D2,N,P,R}` found no exact-size
combination with a lower raw diff than this one.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8407/fdiff.py _ZN11LevelScript7processEi   # 54,404
python3 /dev/shm/w8407/toggle.py L3,L4,L24,R,D,P            # neighbourhood sweep
```

## Open questions

* The three loop **conditions** still use the cached `enemies->count`; the
  original also calls `getEnemies()` there.  Adding a condition re-fetch costs
  +8..+16 B and currently worsens the raw diff (`C3`/`C4`/`C24` all lose), so it
  needs a different size/block companion.
* The `m_nState++` at case 145 state 1 (line 4358) is present in the original
  (`0x13953c`), so it is not a redundant increment.
* `.ARM.extab` remains 1,396 differing bytes at an exact 156,312 B size; the
  remaining `.text` delta is dominated by 16/32-bit encoding width and
  `MachineBlockPlacement`, not missing statements.
