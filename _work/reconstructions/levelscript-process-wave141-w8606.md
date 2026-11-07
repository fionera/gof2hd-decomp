# LevelScript::process — wave 141 / w8606 (case-105 state 7/9/12 audit;
# the missing `setTarget` call and the PLT-anchor size budget)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-140 head `e18e0c2a4`.
Scratch helpers in `/dev/shm/w8606/` (not tracked): `variant.py` (named-edit
apply/build/measure), `fdiff.py` (raw same-address byte diff), `secdiff.py`
(per-section byte diff), `reldiff.py` (decoded `.rel.plt` order diff),
`stores.py`/`calls.py` (store/call sequence diff), `symname.py`.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,138            54,138            (0)
whole .text diffbytes    86,472            86,472            (0)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.ARM.extab diffbytes     1,411             1,411             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 0   (byte-identical, 29,392 B)
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

The landed source edits are **codegen-neutral on this base** (they compile to
the same bytes as HEAD): they only reorder two spellings to match the original
source shape.  The real finding of this wave is the **size budget for the
missing `setTarget` call** (below), which is blocked by the two PLT-order
anchors.

## The landed set (faithful source shape, byte-neutral)

### 1. Case-105 state-7 tail — `getPosition()` before `GetDirVector()`

Original `0x14296a..0x142980`:

```
14296a add r8,sp,#268
14296e mov r1,r5
142970 ldr.w r9,[r4,#20]
142974 mov r0,r8
142976 blx PlayerEgo::getPosition
14297a add r6,sp,#344
14297c mov r1,r5
14297e mov r0,r6
142980 blx PlayerEgo::GetDirVector
```

The source declared `playerDir` before `playerPos`.  Swapped to
`playerPos` then `playerDir` (matches the original evaluation order).

### 2. Case-105 state-12 — cached `geometry` receiver

Original `0x144db8..0x144dec`:

```
144db8 ldr r6,[sp,#88]        ; this
144dba ldr r1,[r4,#8]         ; player->geometry  (setTarget)
144dbc ldr r0,[r6,#20]
144dbe blx setTarget
144dc2 ldr r4,[r4,#8]         ; player->geometry  (cached once)
144dc4 add r5,sp,#280
144dc6 mov r0,r5
144dc8 mov r1,r4
144dca blx getDirection
...
144de4 mov r0,r4              ; same cached geometry
144dec blx setDirection
```

The source reloaded `player->geometry` three times.  Introduced
`AEGeometry *geo = player->geometry;` while keeping the original
`setTarget(player->geometry)` argument spelling (so the compiler keeps the
`setTarget` load separate but caches `geo` for the two direction calls).

Both edits are size- and byte-neutral on this base (`raw 54,138`, `.text
86,472`), i.e. the compiler already produced the same code; they are landed as
faithfulness fixes only.

## The missing call: case-105 state-7 `m_pCamera->setTarget(player->geometry)`

The original tail (`0x142950..0x142966`) is:

```
142950 ldr r4,[sp,#88]        ; this
142952 movs r1,#0
142954 ldr r0,[r4,#20]        ; m_pCamera
142956 blx setFixed(false)
14295a ldr r1,[r5,#8]         ; player->geometry
14295c ldr r0,[r4,#20]        ; m_pCamera
14295e blx setTarget          <-- MISSING in source
142962 ldr r0,[r4,#20]
142964 movs r1,#0
142966 blx setLookAtCam(false)
```

The call is genuinely absent from the source.  Adding it is **+20 B** (direct)
or **+16 B** with a cached `TargetFollowCamera *cam = m_pCamera;` local, and
the function must stay exactly 57,232 B, so it needs a size-negative
companion.

### Candidate companions measured (all size-exact unless noted)

| edit | Δ size | raw diff | `.rel.plt` | notes |
|---|---:|---:|---:|---|
| `setTarget` direct | +20 | 54,792 | 0 | 57252 B |
| `setTarget` via `cam` local | +16 | 54,944 | 0 | 57248 B |
| reorder mission-157 tail (store `initStreamOutPosition` before `SetCurrentApplicationModule`) | −12 | 55,019 | 0 | matches original store order (`0x145be4`/`0x145be8` then shared tail `0x13d6b6`) |
| drop mission-144 duplicate `setStation(getStation())` | −16 | 55,014 | 0 | original `0x141494..0x1414a8` has only `departStation` |
| drop `autoTurret` PLT anchor (line 1812) | −16 | 54,894 | **+18** | breaks `.rel.plt` |
| drop `setStanding` PLT anchor (line 2010) | 0 | 53,958 | **+26** | breaks `.rel.plt` |

Best combinations:
* `cam` setTarget + mission-157 reorder + position/dir reorder + radar-last +
  `setFixed(true)`-before-`MatrixSetTranslation` = **57236 B (+4)**, raw 54,959.
* same with the `setStanding` anchor also dropped = 57228 B (−4), `.rel.plt +26`.
* `setTarget` + mission-157 reorder + mission-144 `setStation` drop = 57224 B
  (−8).

No size-exact (57232) combination containing the missing `setTarget` was found
without removing a PLT anchor.

## Why the PLT anchors cannot be removed yet

`.rel.plt` is byte-identical at HEAD only because of two deliberate
"PLT-order anchor" calls that the original does **not** contain:

* line 1812 `player->setAutoTurret(player->autoTurretIsEnabled());`
  (original first references the pair at `0x13a7fc`/`0x13a80c`, before
  `PlayerWormHole::open` at `0x13a98e`; our mission-154 state-0 block sits at
  `0x13c4ce`, after `open`, so the anchor is needed to keep the pair before
  `PlayerWormHole::open` in the PLT order).
* line 2010 `if (delta == 0x7d) ...Standing::setStanding(0, 100);`
  (original first reference at `0x139b06`, before
  `TargetFollowCamera::setRotationAroundTarget` at `0x139d94`).

Decoded `.rel.plt` order (`reldiff.py`), original:

```
Standing::setStanding
TargetFollowCamera::setRotationAroundTarget
Player::setShootingEnabled
Mission::setType
AEGeometry::isVisible
PlayerEgo::autoTurretIsEnabled
PlayerEgo::setAutoTurret
PlayerWormHole::open
Level::flashScreen
StarSystem::scaleSunDuringSupernovaIntro
StarSystem::updateSupernova
PlayerEgo::isInWormhole
PlayerWormHole::freeMissionLock
StarSystem::switchSunForSupernovaIntro
```

Removing the `autoTurret` anchor rotates the pair to the end of this window
(18 differing `.rel.plt` bytes) and drops `byte_exact 4507 -> 4506` (ratchet
FAIL).  Removing the `setStanding` anchor moves `setStanding` to the end
(26 differing bytes).  Both must stay until the mission-154 block layout is
matched so the real first references land in the original order.

## Subset sweep (all built + linked + measured on HEAD)

| variant | size | raw diff | `.rel.plt` |
|---|---:|---:|---:|
| HEAD | 57232 | 54,138 | 0 |
| **posdir + sd12 cached geo (landed)** | **57232** | **54,138** | **0** |
| mission-157 reorder | 57220 | 55,019 | 0 |
| mission-144 `setStation` drop | 57216 | 55,014 | 0 |
| `cam` setTarget + 157 reorder + posdir + radar + setFixed | 57236 | 54,959 | 0 |
| `setTarget` + 157 reorder + 144 `setStation` drop | 57224 | 54,901 | 0 |
| drop `setStanding` anchor | 57232 | 53,958 | +26 |
| drop `autoTurret` anchor | 57216 | 54,894 | +18 |

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8606/variant.py /dev/shm/w8606/v_safe.py  # 57232, raw 54138
python3 /dev/shm/w8606/secdiff.py                       # .text 86472, rel.plt 0
```

## Open questions

* The missing `setTarget` is a real +16..+20 B statement.  A size-exact landing
  requires either (a) matching the mission-154 block layout so the two PLT
  anchors can be dropped (freeing 16 B) or (b) finding a genuinely redundant
  ~16 B statement in the case-105 block.  The mission-157 tail reorder (−12) is
  a real faithfulness fix but is 4 B short of paying for the `cam` spelling.
* `Level::getEnemies()` is referenced 34× in the original but only 31× in ours
  (3 missing re-fetches); `Level::getPlayer()` 23× vs 22×.  These are the
  remaining call-count deltas outside the PLT pair and are candidates for
  further size-positive faithfulness fixes.
* `.ARM.extab` remains 1,411 differing bytes at an exact 156,312 B size.
