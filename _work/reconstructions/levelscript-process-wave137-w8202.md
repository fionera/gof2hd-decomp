# LevelScript::process — wave 137 / w8202 (w8106 set re-landed at 57232 B)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base: wave-136 head `655e3bcc5` (which already
contains the w8102 case-40 state-1 `translate` + anchor trim).

## Result

```
                         before (master)   after (landed)
process fuzzy pct        58.9              58.6
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54327             54325            (-2)
whole .text diffbytes    84709             84707            (-2)
.ARM.extab diffbytes     1395              1395
.ARM.exidx diffbytes     0                 0
.rel.plt diffbytes       4                 4   (size exact 29,392)
.rodata / .data / .bss   0 / 0 / 0         0 / 0 / 0
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The fuzzy pct falls 58.9 -> 58.6 because the corrected cold blocks re-roll
`MachineBlockPlacement` across the 57 KB body (the same trade-off recorded in
waves 129/132/134/136).  The raw byte-diff improves slightly (54,327 -> 54,325)
and every hard gate number is unchanged.

## What was landed

### 1. The w8106 set (A/D/E/F/G) — all re-verified against objdump

| edit | original evidence |
|---|---|
| case-102 state 0: add `setTarget(enemies[0]->geometry)` before `setPosition`, move `translate` to state 1 | `0x13a70e` setTarget; `0x13e880` translate |
| case-102 state 0 + case-145 state 1: `player->isInRocketControl()` (cached `player`, not `m_pLevel->getPlayer()`) | `0x13a688 mov r0,r5`; `0x13941a mov r0,sl` |
| case-145 state 5: add `m_nState++` after `m_bCinematicBreak = 0` | `0x13f8f6` tail branches to the `m_nState++` helper |
| case-154 state 2: `m_dirVec = player->GetDirVector()` before the first `+=` | `0x142fb6..0x143018` |

On the case-40 base this set alone lands at **57224 B** (the -8 the handoff
reported).

### 2. The exact +8 B companion: `nextCampaignMission` relocation + case-125 tail order

Two *real* source bugs were found while searching for the companion, and they
supply the missing +8:

* **`nextCampaignMission(true)` is misplaced.**  The original's case-145 state-5
  teardown (`0x13f880..0x13f8fe`) is
  `resetGunDelay; setLookAtCam(false); setTarget(player->geometry); ...` — there
  is **no** `nextCampaignMission` between `setLookAtCam` and `setTarget`
  (Ghidra pseudocode and objdump agree).  The call belongs to the state-6 block
  at `0x13d674`: `nextCampaignMission(true); setStation(status->getStation());
  departStation(status->getStation())` (source line ~4295).  The source had it
  in the wrong block.  Moving it is **+8 B on the pre-w8106 base** (0 B on top
  of the full w8106 set).
* **case-125 state-2 tail order.**  Original `0x13b730/0x13b738/0x13b740` is
  `setLookAtCam(true); setVisible(false); setFreeze(true)`.  The source had
  `setFreeze(true)` before `m_bCinematicBreak`.  Fixing it is **-8 B**.

These two cancel on the w8106 base, so the +8 still had to come from a layout
lever:

* **case 131 / case 135 source order.**  The original's case-135 block starts at
  `0x1391c0` while case 131's state-2 body sits at `0x13c8fc`; the source emits
  case 131's whole (state 1 + state 2) body contiguously before 135.  Swapping
  the two `case` blocks is **+16 B** and brings the emitted block adjacency much
  closer to the original (process diffbytes 55,220 -> 54,325).  This is a real
  source reorder, not a dummy statement.

Net on the w8106 base: `+16 (case swap) - 8 (case-125 order) + 0 (nextCM move)
= +8`, landing at exactly **57232 B** with `shift = 0` (every later function
keeps its original address, so `byte_exact` stays 4507).

## Open leads evaluated (not landed)

* **case-102 state-1 tail `resetCamera(m_pLevel)`** — confirmed: original
  `0x143bb8` is `resetCamera(this, m_pLevel)`; the source's
  `m_tempVec = (*enemies)[3]->geometry->getPosition()` is spurious.  Replacing
  it is **-16 B** on the w8106 base.  It is faithful but needs another +16
  companion; not landed.
* **unreachable `case 145: case 6: m_nState = 2;`** — the original's state
  2..5 `tbh` covers four entries only.  Removing it is **-4 B on the pristine
  base**, but **-12 B** on the w8106 base (block re-roll); not landed.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
```

Scratch helpers in `/dev/shm/w8202/` (not tracked): `harness.py` (edit/build/
measure), `measure.py` (size/diffbytes/shift), `score.py` (section deltas),
`census.py`/`callseq.py` (call census), `ghidra.c` (Ghidra 12.1.2 pseudocode
for `process`, produced with `GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea
decompile ...`).

## Negative results (do not re-run)

* `switchSkyboxForIntro` anchor restore: +8 B exactly (57232) with diffbytes
  54,411, but it is a duplicate call (orig 1 / ours 2) — a dummy.
* `PlayerStation::setVisible` anchor restore: +24 B.
* faithful `m_pLevel->getPlayer()->resetGunDelay()` at case-154 state 9: +12 B
  on the w8106 base (the original does call `getPlayer()` fresh there).
* `case-4 StarSystem*` hoist / case-24 un-cache / case-135 `setVisible` /
  case-16 `lm4` / case-16 `enemies1718` / `resetGunDelay` site-1 receiver:
  all negative on this base (see w8105 note); none closes the +8.
* case 139/142/144/145 reorder to the original's `145,142,144,139` block order:
  0 B.
