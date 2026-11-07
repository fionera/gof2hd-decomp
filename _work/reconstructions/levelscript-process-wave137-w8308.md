# LevelScript::process — wave 137 / w8308 (case-40 cold-chain block placement)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `96b069792` (wave-137 head).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.9
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  85,535            85,657           (+122)
.ARM.extab diffbytes     1,395             1,395            (size exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 byte-identical    byte-identical   (7348 words, 0 diff)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The raw `.text` diff rises by 122 B because the corrected case-40 cold block
re-rolls `MachineBlockPlacement` across the 57 KB body (the same trade-off
recorded in waves 129/132/134/136); the official fuzzy pct is unchanged at 58.9
and every hard gate number is identical.

## Landed changes (all checked against objdump of the original)

### 1. Case 40 — nest the state-0 test so the state-0 body is laid out inline

Original entry `0x13a4d8`:

```
13a4d8 ldr r0,[sp,#88]
13a4da ldr r0,[r0,#28]        ; m_nState
13a4dc cmp r0,#0
13a4de bne.w 13e6b8           ; non-zero -> cold dispatch at 0x13e6b4
13a4e2 ldr.w r0,[fp,#4]
13a4e6 ldr r0,[r0,#12]        ; messages[3]
13a4e8 blx RadioMessage::isTriggered
13a4ec cmp r0,#0
13a4ee beq.w 13e6b4           ; not triggered -> cold dispatch
13a4f2 ... state-0 body (inline, falls through) ...
```

The cold dispatch `0x13e6b4` re-loads `m_nState` and checks `2`, then `1`,
then `3`, `4`; the state-1 body is the fall-through of that cold block
(`0x13e6c0`).

Our source wrote `if (m_nState == 0 && isTriggered())` and clang chose the
opposite layout: the entry became a pure dispatch (`cmp #0; beq cold`) and the
**state-1** body was laid out inline, with the whole state-0 body (including
its `isTriggered` check) in a cold block at `0x13c1d8`.

Rewriting the test as a nested `if`:

```cpp
if (m_nState == 0) {
    if (((RadioMessage *) ((*messages)[3]))->isTriggered()) {
        ... state 0 ...
    }
} else if (m_nState == 1) { ... }
```

makes clang emit the original shape: the `isTriggered` check is part of the
entry block and the state-0 body is its fall-through, while the state-1 body
moves into the cold block.  Both blocks now match the original's call
sequence exactly:

* state-0 (`0x13a4d8`..`0x13a640` vs ours `0x13a2de`..`0x13a450`): **24/24**
  calls in the same order (only the indirect-call register name differs:
  original `blx r4`, ours `blx r5`);
* state-1 (`0x13e6c0`..`0x13e726` vs ours `0x13d87e`..`0x13d8e4`):
  `translate(0,0,-2*delta)`, `isOver(messages[4])`, `setTarget`, hud=1,
  radar=1, `m_bCinematicBreak=0`, `setLookAtCam(false)`,
  `setComputerControlled(false)`, `setVulnerable(true)`, `m_nState=2` — exact.

This is the "remaining cold-block order" left open by wave-136/w8206.

### 2. `routePoints` — restore the faithful float fold (size-neutral)

Source line 1707 was left as the wave-137 size lever

```cpp
int routePoints[3] = {(int) camPos.x, (int) camPos.y, (int) camPos.z + 30000};
```

The original does `vadd.f32` then `vcvt.s32.f32` (`0x13f170/0x13f180`), i.e.
`(int) (camPos.z + 30000.0f)`.  Restored the faithful form; it is size-neutral
on this base and improves the fuzzy ratio.

### 3. Docking block — `resetGunDelay` on a fresh `getPlayer()` (the +8 companion)

Original `0x140270`..`0x14029a` (the `isDockedToDockingPoint` teardown):

```
140270 ldr r0,[r5,#24] ; blx Level::getPlayer ; blx setAutoPilot
140288 ldr r0,[r5,#24] ; blx Level::getPlayer ; blx resetGunDelay
140290 ldr r0,[r5,#24] ; blx Level::getPlayer ; blx dockToDockingPoint
```

The source used the cached `player` for the middle call.  Changed to
`m_pLevel->getPlayer()->resetGunDelay();` (source line 3065).  This is the
**+8 B** companion that keeps `process` at exactly 57232 B after the nested
case-40 test (-8 B).

## Size accounting (measured on the wave-137 master)

| edit | delta |
|---|---:|
| case-40 nested `if` | -8 |
| docking `resetGunDelay` fresh `getPlayer()` | +8 |
| `routePoints` float fold | 0 |
| **net** | **0** (measured 57232) |

## Case-102 state-1 tail — already landed

Wave-137/w8201 already replaced the spurious
`m_tempVec = (*enemies)[3]->geometry->getPosition()` with
`resetCamera(m_pLevel)` (original `0x143bb8`), and the whole tail
(`0x143b2c`..`0x143bfe`) matches except the final `m_nState++` store, which
ours inlines where the original branches to the shared `0x13999a` helper.  No
further source change is possible there.

## Open lead (not landed)

* `nested + routefold + fixgetplayer + rgd2` measures **57224 B** (-8).  The
  `fixgetplayer` edit is the genuine receiver fix at `0x144db6`
  (`player->geometry->setDirection(-dir, ...)` instead of
  `m_pLevel->getPlayer()->geometry->...`; original uses the cached `player`
  from `[sp,#84]`).  It needs a +8 B faithful companion.
* `routefold + fixgetplayer + rgd2` (without the case-40 nest) is size-exact
  at 57232 and has a higher raw difflib ratio (0.5904 vs 0.5871), but it does
  not carry the case-40 cold-block fix.  Kept the case-40 fix because it is
  the mission target and the official pct is identical.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
```

Scratch helpers in `/dev/shm/w8308/` (not tracked): `measure.py` (size /
insn count / difflib ratio), `relplt.py` (rel.plt word diff), `apply2.py`
(named-edit harness), `extract.py` (objdump symbol extractor).
