# LevelScript::process — wave 137 / w8303 (w8202 companion set re-based on the w8201 landing)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `96b069792` (wave-137 head, which already
contains the w8201 four-`getPlayer`-receiver landing). Scratch helpers in
`/dev/shm/w8303/` (not tracked): `apply.py` (named-edit harness), `measure.sh`
(build + size + official verify pct), `diffbytes.py` (raw process byte diff).

## Result

```
                         before (master)   after (landed)
official verify pct      57.4              57.3
process diffbytes        55,085            55,072
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
whole .text diffbytes    85,535            85,456
.ARM.extab diffbytes     1,395             1,395            (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 **0 (byte-identical)**  **0 (byte-identical)**
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The function stays exactly 57232 B, so every later function keeps its original address
and `byte_exact`/`linked_exact` stay 4507. The whole-`.text` positional diff improves
**85,535 -> 85,456** (-79 B); the process fuzzy drops 0.1 pp because the corrected cold
blocks re-roll `MachineBlockPlacement` (the same trade-off recorded in waves 129/132/134/136).

## The landed set (all re-verified against objdump of the original)

### 1. case-125 state-2 teardown — `setFreeze` after `setVisible`

Original `0x13b716..0x13b740`:

```
13b716  m_bCinematicBreak = 1
13b722  m_pHud->visible = 0
13b728  m_pRadar->enabled = 0
13b730  m_pCamera->setLookAtCam(true)
13b738  player->setVisible(false)
13b740  player->setFreeze(true)
```

The source had `player->setFreeze(true)` *before* `m_bCinematicBreak`. Moving it after
`player->setVisible(false)` gives exactly the original tail
`setLookAtCam(true); setVisible(false); setFreeze(true)`. **-8 B** on this base.

### 2. case-135 / case-131 source order

The original places the case-135 block at `0x1391c0` (its `getStatusValue` call is at
`0x1391f4`), while case-131's state-1 and state-2 `CameraGetLocal` blocks are at
`0x13b9e6` and `0x13c906`. The source emitted case 131 (state 1 + state 2) contiguously
before case 135. Swapping the two `case` blocks puts the case-135 body at `0x139150`
(ours) vs `0x1391f4` (original) — the closest adjacency the source can express.
**Size-neutral on this base** (it was +16 B on the w8106 base; the w8201 companions
already re-rolled the placement).

### 3. case-144 state-3 tail — missing `nextCampaignMission` + faithful statement order

Original `0x13d666..0x13d6bc`:

```
13d66e  m_nState = 4
13d674  Globals::status->nextCampaignMission(true)
13d684  Globals::status->setStation(Globals::status->getStation())
13d694  Globals::status->departStation(Globals::status->getStation())
13d698  if (Level::initStreamOutPositionAfterCutscene) { ... }
13d6b8  Globals::appManager->SetCurrentApplicationModule(2)
```

The source had `SetCurrentApplicationModule(2)` first, `m_nState++`, then
setStation/departStation, the `if`, and `m_nState = 4` last — and **no**
`nextCampaignMission`. Reordering the block to the original statement order and adding
the missing call is **+8 B** on this base. (Just inserting the call without the reorder
is +16 B; the reorder is what makes the companion exactly +8.)

## Size accounting (measured on the w8201 base)

| edit | Δ size |
|---|---:|
| case-144 faithful reorder + `nextCampaignMission(true)` | +8 |
| case-125 `setFreeze` after `setVisible` | -8 |
| case-131 / case-135 swap | 0 |
| **net** | **0** (57232 B, shift 0) |

Measured combinations: `D` alone 57240; `B` alone 57224; `C` alone 57232; `D+B` 57232;
`D+B+C` 57232. The naive `A` (insert the call, keep the old statement order) is +16,
so `A+B+C` is 57240 and does **not** keep shift 0.

## Negative results (do not re-run)

* **case-125 ambush-loop local-declaration reorder** (w8203's `awake/pirate/playerPos/
  sideZ/offsetY/offsetZ/sideX` order): +144 B on the current switch shape (57376), pct
  48.7. It is only -8 B on the else-if dispatch shape, which is not the landed shape.
* **`A` (insert `nextCampaignMission` without the case-144 reorder)**: +16 B (57248).
* The w8201 `routePoints` fold drop is a non-faithful size lever (the original does
  `vadd.f32` then `vcvt.s32.f32`); restoring the faithful `(int)(camPos.z + 30000.0f)`
  is +8 B and would need another -8 companion.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target drift -j6      # 0 fields
```
