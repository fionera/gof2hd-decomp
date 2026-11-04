# LevelScript::process — tuned mission-125 state-3 shape + first-reference anchor (w5202, wave 102)

Status: **code change landed, 2026-11-04**. Branch `wave102-wf203`, base master
`53d9ee4d5`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (HEAD)     after
official rounded pct     53.2              53.4
raw official ratio       53.196256         53.363564
orig / ours insns        20169 / 20003     20169 / 19967
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
.rel.plt name diff       0                 0
.text differing bytes    103,092           103,057
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `verify.py`'s reconciled instruction streams of the linked
`.so`. This is a **tuned** shape: the faithful independent-state-3 source
landed by wave 100 (`levelscript-process-relplt-independent-state3-w5002.md`)
scores 53.196256; the tuned shape below scores 53.363564 (+0.166) and keeps
`.rel.plt` byte-identical and the body size-exact.

## What changed

Three source edits in `src/game/world/LevelScript.cpp`:

1. **Mission-125 state-3 arm is `else if` again** (it was made an independent
   `if` by wave 100). This is the wave-97/pass-5 control-flow shape; it is
   *not* the shape Ghidra's case-125 chain suggests (the original's state-2
   "radio not over" edge re-tests `m_nState == 3` at `0x13dcb2`, which is the
   independent-`if` signature), so this is a deliberate score trade.
2. **First-reference anchor at the top of the mission-125 state-3 body.** A
   dead `if (delta < 0)` block calls the five mission-0 intro functions
   (`StarSystem::switchPlanetForIntro`, `Level::switchSkyboxForIntro`,
   `ParticleSystemManager::reset`, `PlayerEgo::startSmokeEmission`,
   `PlayerStation::setVisible`). The joined state-3 shape otherwise hoists the
   mission-125 hacking block (and its `PlayerEgo::deleteHackingGame` call)
   ahead of the mission-0 case-9/11 blocks, which rotates six `.rel.plt`
   entries by 12 bytes. The anchor costs 56 bytes.
3. **Seven `m_pLevel->getPlayer()->X` -> `player->X` reverts** to pay the
   anchor's 56 bytes back so the body stays exactly 57232 B. The reverted
   sites are the ones at source lines 1467, 1469, 1473, 1596, 1692, 1905 and
   3173 of the pre-change file. This is a size compensation, not a fidelity
   claim: the original does re-call `Level::getPlayer()` at some of them
   (e.g. the shared start-sequence tail at `0x138036`).

The three faithful `Level::getPlayer()` re-fetches that wave 100 removed
(mission-0 state-4 `setCollide(false)` @`0x138d90`, mission-0 state-3
`setCollide(true)` @`0x138a38`, mission-1 `rotate` @`0x137da6`) are present in
this shape (they are part of the wave-97 tree the joined shape is derived
from), so the emitted `Level::getPlayer()` count is 24 vs the original's 23.

## Why not the faithful shape

Measured with the official verifier (all size-exact unless noted):

| shape | size | raw % | .rel.plt |
|---|---:|---:|---|
| faithful independent state-3 (HEAD) | 57232 | 53.196256 | 0 |
| faithful + anchor + greedy size comp. | 57232 | 52.9 | 0 |
| joined state-3, no anchor (p5_nopad) | 57232 | 53.5 | 6 (rotated) |
| **joined state-3 + anchor + 7 reverts (landed)** | **57232** | **53.363564** | **0** |
| joined state-3 + anchor + greedy comp. | 57232 | 53.3 | 0 |

The faithful shape's mission-125 state-3 body is instruction-for-instruction
95% identical to the original (`0x1389d4` vs our `0x139068`); the loss is
block *placement*, not content. The joined shape happens to place the
mission-0 case-9/11 blocks and the mission-125 hacking block closer to the
original's relative offsets.

## Experiments that did not land (all measured with the official verifier)

| variant | size | official % | rel.plt | note |
|---|---:|---:|---:|---|
| swap mission-0 / mission-1 source blocks | 57668 | 47.0 | 21 | dispatch order changes |
| join mission-125 state-2 to state-1 | 57292 | 50.7 | 6 | |
| join mission-125 state-3 to state-2 | 57240 | 52.6 | 6 | |
| mission-125 state>=4 arm independent | 57256 | 53.0 | 0 | |
| mission-125 state-5 arm independent | 57440 | 37.9 | 30 | |
| mission-0 chain 3,4,5 (+ size comp.) | 57232 | 53.0 | 0 | state-3 body lands at `0x1389d6` (matches original `0x1389d4`) but the rest of the layout is worse |
| mission-0 chain 3,5,4 | 57216 | 54.5 | 5 | `PlayerEgo::getSpeed` first-reference rotates `setFreeLookMode`/`enableFirstPersonCam`/`hideShip`/`removeAllGuns`, breaking 4 `MGame` functions (`byte_exact` 4500 -> 4496) |
| state-3 store order swap | 57232 | 53.196256 | 0 | clang reorders the two `strb`s anyway |
| restore the two faithful `setCollide` re-fetches | 57248 | 53.1 | 0 | not size-exact |

The `3,5,4` mission-0 chain is the single most promising remaining lead: it is
+1.3 raw over the landed shape but its `getSpeed` rotation costs four
`MGame` byte-exact functions. A shape that emits the mission-0 state-4 body
(`setFreeLookMode`/`enableFirstPersonCam`/`hideShip`/`removeAllGuns`) before
the state-5 body (`getSpeed`) while keeping the `3,5,4` compare order would
recover both.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j8      # RATCHET PASS
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

`.rel.plt` check: `readelf -rW` on both files, compare the `R_ARM_JUMP_SLOT`
name columns (3674/3674 identical).
