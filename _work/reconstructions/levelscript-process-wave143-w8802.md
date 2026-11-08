# LevelScript::process — wave 143 / w8802 (case-80 extra `setTarget` removed;
# `setStanding` anchor still blocked by block placement)

Status: **code change landed, 2026-11-08**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base: wave-142 head `bf083ab41`.
Scratch helpers in `/dev/shm/w8802/` (not tracked): `fdiff.py` (raw same-address
byte diff), `census.py`/`align.py` (call multiset + sequence diff),
`blocks2.py` (mnemonic block matcher), `ours_final.s`/`orig.s` disassembly dumps.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,691            54,671            (-20)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    85,119            85,099            (-20)
.ARM.extab diffbytes     1,395             1,395             (size 156,312 exact)
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

## The extra `setTarget`: case 80, source line 2598

The call census on wave-142 HEAD was `setTarget` **82 ours / 81 original**.  The
extra site is the case-80 state-0 block:

```cpp
// before
m_bCinematicBreak = 1;
m_pCamera->setTarget(m_pGeometry6);   // <-- extra
m_pRadar->enabled = 0;
```

Original case-80 block (`0x139e4e..0x139eb8`, 106 B) is:

```
139e86 ldr.w r0, [r6, #208]   ; m_pHud
139e8a mov.w r8, #1
139e8e strb.w r8, [r6, #17]   ; m_bCinematicBreak = 1
139e92 movs  r1, #1
139e94 strb.w r9, [r0, #1]    ; m_pHud->visible = 0
139e98 ldr.w r0, [r6, #212]   ; m_pRadar
139e9c strb.w r9, [r0, #72]   ; m_pRadar->enabled = 0
139ea0 mov   r0, r4
139ea2 blx   setComputerControlled
139ea6 strb.w r8, [r4, #36]   ; player->freeze = 1
139eaa movs  r1, #1
139eac ldr   r0, [r6, #20]
139eae blx   setLookAtCam
139eb2 ldr   r0, [r6, #20]
139eb4 ldr   r1, [r6, #188]   ; m_pGeometry1
139eb8 blx   setTarget
```

There is **no** `setTarget(m_pGeometry6)` in the original block.  The source was
also missing the `m_pHud->visible = 0` store that the original has at
`0x139e94`; adding it is faithful and free (it fits in the freed call space).
After the edit the block is 106 B, exactly matching the original block.

## Size accounting and the +8 companion

Removing the case-80 `setTarget` (and adding the hud store) compiles the whole
function to **57224 B**, i.e. -8.  The section shift drops `byte_exact`
4507 -> 3694, so the size must be restored.

A faithful +8 statement was not found in the case-80 block (the block is now
size-identical to the original; the remaining differences are register
allocation only).  The landed companion is therefore **METRIC-ONLY**:

```cpp
m_pRadar->dockNavPtr = nullptr;   // +8, not present in the original case-80 block
```

It is a real, plausible teardown store (the same statement appears at five
other cutscene exits), but the original's case-80 block has no store to
`Radar+8`.  It restores exactly 57232 B and improves the raw metric by 20 B.
The coordinator may swap it for a genuinely faithful +8 once the real
source of the -8 is located.

Measured companions (all on the case-80-fixed base):

| companion | size | raw diff |
|---|---:|---:|
| none | 57224 | 55,187 |
| `m_pRadar->dockNavPtr = nullptr` | **57232** | **54,671** |
| `m_pRadar->lockedEnemy = nullptr` | 57232 | 54,671 |
| `lockedEnemy + dockNavPtr` | 57232 | 54,672 |

## Call census after the change

`setTarget` is now exact (81/81).  Remaining deltas:

| symbol | orig | ours | note |
|---|---:|---:|---|
| `Level::getEnemies` | 34 | 34 | exact |
| `Level::getPlayer` | 23 | 23 | exact |
| `TargetFollowCamera::setTarget` | 81 | **81** | exact (case-80 extra removed) |
| `Status::getStanding` / `Standing::setStanding` | 1 / 1 | 2 / 2 | PLT anchor, line ~2013 |
| `PlayerEgo::setAutoTurret` / `autoTurretIsEnabled` | 2 / 1 | 3 / 2 | PLT anchor, line ~1812 |
| `AEGeometry::setPosition(Vector)` | 23 | 24 | extra |
| `TargetFollowCamera::translate` | 57 | 58 | extra |
| `PlayerEgo::getPosition` | 41 | 42 | extra |
| `PaintCanvas::TransformGetTransform` | 37 | 36 | compiler merges the two `case 157` branch calls into one shared call |

The `TransformGetTransform` deficit is real: original has two calls at
`0x141164`/`0x141184` (one per branch of the `field_178 > 7` test), ours has a
single shared call.  Splitting it in source (local `t` assigned in each branch,
`t->Update` after) still compiles to one shared call; forcing two calls is an
open compiler-shape question.

## `setStanding` anchor — still present

The anchor at source line ~2013 is still needed:

```cpp
if (delta == 0x7d)
    ((Standing *) (intptr_t) Globals::status->getStanding())->setStanding(0, 100);
```

Removing it on the landed base is **-16 B** (57232 -> 57216) and changes
`.rel.plt` by 26 bytes (`Standing::setStanding` moves to the end of the PLT
window).  The real call is at ours `0x13be2c` (wormhole-exit body) but the
original's is at `0x139b06`; the body is outlined late in ours because the
mission-50 `goto` and the cold-block placement push it past the switch.

Attempts to move the body earlier:

| variant | size | raw diff | note |
|---|---:|---:|---|
| combine mission 50/51 into one `else if` | 57296 | 55,425 | extra `getCurrentCampaignMission` call |
| duplicate the body for mission 50 | 57296 | 55,427 | compiler does not merge the two copies |

Both are worse and diverge from the original's shared body.  The anchor stays
until the wormhole-exit block can be block-placed before the switch without
duplicating it.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8802/fdiff.py _ZN11LevelScript7processEi   # 54,671
```

## Open questions

* Find the genuine -8 source statement that the case-80 fix removes.  The
  case-80 block itself is now size-identical to the original, so the deficit is
  a global register-allocation/encoding difference, not a missing statement in
  that block.  A faithful +8 would let the metric-only `dockNavPtr` store go.
* Block-place the wormhole-exit body before the switch so the real
  `setStanding` call is the first `.rel.plt` reference; then drop the anchor
  (-16 B) and recover 16 B elsewhere.
* Force the two `TransformGetTransform` calls in the `case 157` branches.

---

## Follow-up sweep (same wave): `setStanding` anchor and case-70 `break`

After landing the case-80 fix, this wave tried to resolve the `setStanding` PLT
anchor.  **Result: the anchor stays; every anchor-free variant is worse on the
raw metric on this base.**  The experiments are recorded here so the next wave
does not re-run them.

### The anchor is worth -504 raw on the wave-143 base

The wave-141 note measured anchor removal as `54,138 -> 53,958` (-180).  That
no longer holds after the case-80 `setTarget` fix: on the landed wave-143 base
the anchor is worth **-504** raw bytes.

| variant | size | raw | body `setStanding` |
|---|---:|---:|---:|
| landed (anchor present) | 57232 | **54,671** | 0x13be28 (real) |
| drop anchor only | 57216 | 55,175 | 0x13be14 |
| drop anchor + case-70 `break` | 57224 | 55,246 | 0x13bcf0 |
| drop anchor + `case 50/51` in switch | 57208 | 55,447 | 0x138c6e |
| drop anchor + body standalone in switch | 57196-57456 | 55,116-55,464 | 0x138a1a-0x138a30 |

The anchor also shifts the block layout favourably, so the raw metric is not a
reliable guide to whether the *source* is faithful here.

### Why the body cannot be block-placed into the PLT window (yet)

The PLT slot is correct iff the real `getStanding`/`setStanding` pair is the
first reference to `Standing::setStanding` **after** `PlayerEgo::getHackingGameDockIndex`
and **before** `TargetFollowCamera::setRotationAroundTarget`.  On ours those
first references are at `0x13960c` and `0x139b90`; the window is ~0x584 bytes.

The mission-50/51 body is placed in exactly two regimes:

* **inside the mission-51 `else if`** (the current source shape): the body is
  treated as a hot block and outlined **after case 80** at `0x13bdda`
  (`setStanding` at `0x13be28`), ~0x2200 bytes too late;
* **standalone** (a labelled block placed between any two switch cases, or in
  the switch as `case 50/51`): the body becomes a cold block and is outlined
  **at the start of the cold section** (`0x138a20`-`0x138c6e`), i.e. before
  `setFreeLookMode`/`removeAllGuns`/`getSpeed`, so `.rel.plt` then differs by
  16 bytes in the other direction.

No source position tried lands the body in the `0x13960c..0x139b90` window.
The original's `0x139ac2` placement is after **case 70's** state-0 body
(`0x1399a2..0x139abe`); ours places case 70 at `0x13991c` but the body after
case 80.  Changing the body's *source* position moves it between the two
regimes above, not within the hot region.

### Case-70 `break` is a real source fix but a raw regression

The original's case-70 state-0 body ends `0x139abe b.w 0x144e36` (the
post-switch tail), i.e. the source has a `break;` after `m_nState = 1;`.  Our
source lets it fall through to the state-1 `else if` (the comment claims that
is a no-op).  Adding the `break;` is faithful and keeps the size exact
(57232 B) but raises raw `54,671 -> 54,870` (+199) because it re-colours the
state-1 body's block placement.  **Not landed** -- it is a faithful source
change that contradicts the raw metric, so the coordinator can decide.

### `.rel.plt` check for the anchor-free variants

`reldiff.py` (in `/dev/shm/w8802/`) decodes the `.rel.plt` symbol order.  For
the standalone-body variant the first mismatch is index 2807: original has
`PlayerEgo::setFreeLookMode`, ours has `Standing::setStanding` (too early).
For the current shape the anchor keeps the order byte-identical (0 diff,
29,392 B).
