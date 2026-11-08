# LevelScript::process — wave 145 / w9002: the .rel.plt-preserving
# state-4-before-state-5 placement (order 4,5,3)

Status: **code change landed (faithfulness-first; raw +14)**, 2026-11-08.
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57232 B (0xdf90)**, size-exact. Owned TU:
`src/game/world/LevelScript.cpp`. Base: wave-144 head `eb13ada9b`
(raw 54,909, `.text` 85,994, `.ARM.extab` 1,370). Scratch helpers in
`/dev/shm/w9002/` (not tracked): `fdiff.py`, `reldiff.py`, `regions.py`,
`regioncmp.py`, `harness.py`, `harness2.py`, `LevelScript.base.cpp`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,909            54,923            (+14)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,994            86,008            (+14)
.ARM.extab diffbytes     1,370             1,368             (-2)
.rel.plt                 byte-identical    byte-identical    (6,105 relocs)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

## What changed

The original's mission-0 state-4 teardown (`0x138d2a`) really does own the
four calls our source had at the head of state 5, in this witnessed order
(Thumb disasm `0x138d6c..0x138dac`):

```
resetCamera(this, m_pLevel)
player->setFreeLookMode(false)                0x138d6c
m_pCamera->enableFirstPersonCam(false)        0x138d74
player->hideShipForFirstPersonCameraView(false) 0x138d7e
m_bCinematicBreak = 1                         0x138d8c
m_pLevel->getPlayer()->setCollide(false)      0x138d90
m_pRadar->enabled = 0
player->setComputerControlled(true)
player->player->removeAllGuns()               0x138daa
m_pCamera->setLookAtCam(true)
...
```

The landed edit moves those four calls into state 4 in exactly that order and
drops them from the head of state 5. It then reorders the three mission-0
state tests to **4, 5, 3** (`if (m_nState == 4) ... else if (m_nState == 5 &&
...) ... else if (m_nState == 3) ...`). The arms are mutually exclusive, so the
reorder is semantics-preserving.

## Why 4,5,3 and not 3,4,5

The original's *block layout* is state 3 (`0x1389d4`), state 4 (`0x138d2a`),
state 5 (`0x138fd8`), i.e. source order 3,4,5. But on the current base that
order (`345`) compiles to **57272 B / raw 55,371** and perturbs `.rel.plt` at
idx 5262 (`PlayerStation::setVisible` / `PlayerEgo::deleteHackingGame` move
ahead of `switchPlanetForIntro` / `switchSkyboxForIntro` /
`ParticleSystemManager::reset` / `startSmokeEmission`). All six source
permutations were built and measured:

| order | size | raw | `.rel.plt` |
|---|---:|---:|---|
| 354 (HEAD) | 57232 | 54,909 | exact |
| 345 | 57272 | 55,371 | perturbed idx 5262 |
| 435 | 57256 | 55,060 | perturbed idx 5262 |
| **453 (landed)** | **57232** | **54,923** | **exact** |
| 534 | 57240 | 55,423 | perturbed idx 5238 |
| 543 | 57240 | 55,423 | perturbed idx 5238 |

Only **4,5,3** keeps `.rel.plt` byte-identical at the exact 57,232 B size.
It places state 4 at `0x1389d8` (the fall-through arm) and state 5 at
`0x138db4`, so state 4 is laid out before state 5 as the mission asked.

The four-call placement was also swept: all 13 insertion anchors (after
resetCamera, after the cinematic-break store, after setCollide, after radar,
after setComputerControlled, after setLookAtCam, after setTarget, after
setRotation, after getPosition, after setPosition, after translate, at the
tail) x removeAllGuns anchors. Every faithful placement is 57232 B and raw
54,919..54,929; the landed one (`removeAllGuns` after `setComputerControlled`,
matching `0x138daa`) is 54,923. The best raw at size-exact was 54,919 with
`removeAllGuns` after `translate` (unfaithful), so the faithful placement was
preferred.

## The mission's -244 / 54,585 numbers are stale

`m4bc` alone (four calls into state 4, cached `player` for setCollide, no
branch reorder) is **57224 B / raw 55,312** on this base — not +24 B / -244.
The -244 was measured on the wave-141 base; wave-142/143/144 re-rolls of
`MachineBlockPlacement` erased it (wave-142: -19; wave-143: 0; wave-137:
+132). `D_cached+swap45` (faithful order, source order 3,4,5) is
**57264 / 55,336** here, not 57,256 / 54,585.

## Honest metric note

This is a **faithfulness** change, not a metric win: the original's state-4
teardown genuinely owns the four calls, and state 4 genuinely precedes state 5
in the original's `.text`. The raw same-address diff rises by 14 because the
state-4 block is still placed at `0x1389d8` instead of the original's
`0x138d2a`; `.ARM.extab` improves by 2. If the coordinator prefers the
lower raw, revert to HEAD (54,909). The change is **not** METRIC-ONLY: it does
not contradict the original's witnessed asm order.

## Open questions

* Source order `345` reproduces the original's 3,4,5 block layout but perturbs
  `.rel.plt` (setVisible/deleteHackingGame vs switchPlanetForIntro). Fixing it
  needs the case-42 state-6 block laid out before state 7 and the mission-125
  hacking block; reordering the case-42 arms (all 6 permutations) did not move
  the layout. That variant is 57272 B and would need a -40 B companion.
* The mission-26 / chain-A block placement is still the biggest shift lever
  (the wave-144 note's 1,340 bytes in `0x138500..0x138c00`).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9002/fdiff.py _ZN11LevelScript7processEi   # 54,923
python3 /dev/shm/w9002/reldiff.py                            # all same up to 6105
```
