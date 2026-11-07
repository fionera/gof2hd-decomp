# LevelScript::process — wave 135 / w7764 (mission-0 state 4/5 teardown split)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `bbffef482`
(wave-134 head).

## Result

```
                         before (master)   after (landed)
official verify pct      58.1              57.9
process .text diffbytes  54359             54416   (+57)
whole .text diffbytes    85022             85079   (+57)
orig / ours insns        20830 / 20709     20830 / 20711
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
rel.plt                  identical         identical
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The fuzzy pct dips 0.2 because the corrected state-machine split re-rolls
`MachineBlockPlacement` (the same trade-off recorded for waves 129/132).  The
semantic fix is real: four statements were in the wrong state arm.

## The w7746 hypothesis is a misread

w7746 said the body was missing `translate(-7000,500,17000)` at `0x13a4b8`.
`0x13a4b8` is actually `translate(-10000,5000,25000)` (case 157 state 1,
literals at `0x13a63c/38/40` = `-10000/5000/25000`).  The `-7000,500,17000`
translate is the *next* block's call at `0x13a5fc` (case 40 state 0, literals
`0x13a64c/50/54`), and both are present in our source (lines 4762 and 1925) and
in the emitted code (`0x13a5e2` and `0x13c4a4` in the old build).  Nothing was
missing there.  The real missing statements were found by diffing the Ghidra
pseudocode of the whole function (`rea decompile`, Ghidra 12.1.2 at
`/tmp/ghidra_12.1.2_PUBLIC`) against the source.

## Landed change: mission 0, states 4/5

Original state 4 (`0x138d2a`..`0x138df6`) does the full cutscene-exit teardown:

```
138d5e  setTurretMode(false)
138d68  resetCamera(level)
138d70  PlayerEgo::setFreeLookMode(false)        <-- was in our state 5
138d7a  TargetFollowCamera::enableFirstPersonCam(false)  <-- was in our state 5
138d82  PlayerEgo::hideShipForFirstPersonCameraView(false) <-- was in our state 5
138d8c  m_bCinematicBreak = 1
138d90  getPlayer()->setCollide(false)
138da0  m_pRadar->enabled = 0
138da6  setComputerControlled(true)
138dac  Player::removeAllGuns()                  <-- was missing entirely from state 4
138db6  setLookAtCam(true)
138dc0  setTarget(player->geometry)
138dcc  setRotation(0, pi/2, 0)
138de2  setPosition(playerPos)
138df0  translate(delta*0.3, 0, -delta*0.2)
138df4  m_nState = 5
```

Original state 5 (`0x138fd4`) starts directly with `msg12->isOver()` and then
`getPosition()` / `getSpeed()` -- it has none of the four teardown statements.
Our source had them at the *top of state 5* and was missing `removeAllGuns`
from state 4.  The fix moves the three free-look/camera statements into state 4
and adds `player->player->removeAllGuns()` after `setComputerControlled(true)`.

## Size / .rel.plt constraints (measured)

Moving the statements alone (source order still `3,5,4`) is **57232 B exact**
but reorders `.rel.plt`: the first `PlayerEgo::getSpeed` (state 5) then precedes
the first `PlayerEgo::setFreeLookMode` (state 4), whereas the original has
`setFreeLookMode` first.  Ratchet: `byte_exact 4506 -> 4502`.

The original's block order is state 3 (`0x1389d4`), state 4 (`0x138d3a`), state 5
(`0x138fd4`).  Reordering the source arms was swept over all six permutations
(with the four statements moved):

| source arm order | size | first .rel.plt diff |
|---|---:|---|
| 3,4,5 | 57264 (+32) | 2834 |
| 3,5,4 | 57232 (0) | 2807 (getSpeed vs setFreeLookMode) |
| 4,3,5 | 57256 (+24) | 2834 |
| 4,5,3 | 57240 (+8) | **identical** |
| 5,3,4 | 57232 (0) | 2807 |
| 5,4,3 | 57232 (0) | 2807 |

`4,5,3` is the only order that reproduces the original `.rel.plt`; it is +8 B.
The `delta < 0` first-reference anchor block (case 125 state 3) had a spurious
`player->PlayEngineSound()` size compensator (the original emits only one
`PlayEngineSound`, ours emitted two).  Removing it is exactly -8 B and leaves
`.rel.plt` identical, so the final function is **57232 B exact** with an
unchanged `.rel.plt`.  `StopEngineSound` is kept in the anchor block (either one
of the pair works; removing both is -16 B and would need a different +8).

## Why the diff bytes go up 57

The arm reorder changes `MachineBlockPlacement` for the whole mission-0 region;
the state-4 body now lands at `0x138a08` instead of the original `0x138d3a`, so
the corrected statements are byte-identical locally but the surrounding blocks
are not.  This is the same block-placement trade-off documented in waves
129/132.  The gate is green and the source is semantically faithful; the next
lever is matching the original's state-3/4/5 block order (source order `4,3,5`
gives the right *checks* but +24 B).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC \
  rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure _ZN11LevelScript7processEi --provider ghidra
```

Scratch helpers in `/dev/shm/w7764/` (not tracked): `perm.py`/`perm2.py`
(arm-order sweep), `subset2.py` (anchor-call subset sweep), `regions.py`
(difflib region map), `trans.py`/`tbh.py` (translate/tbh decoders).

## Post-merge note (wave 135 coordinator)

The source change was **not landed**: on the w7763 base it raises `.text`
differing bytes 84,906 -> 85,038 (+132) even at the exact 57,232 B size.  The
state-4 teardown reconstruction is semantically right and remains the lead for
a future pass with a size/block companion.
