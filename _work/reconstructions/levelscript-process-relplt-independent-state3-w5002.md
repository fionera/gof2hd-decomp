# LevelScript::process — .rel.plt first-reference order via independent state-3 arm (w5002, wave 100)

Status: **code change landed, 2026-11-04**. Branch `wave100-w5002`, base master
`44225192c`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

This pass closes the `.rel.plt` 12-byte rotation left open by wave 95
(`levelscript-process-wave95-w4504.md` §4, `rel_plt_level_levelscript_w3461.md`
group B): `PlayerEgo::deleteHackingGame` was first-referenced too early in the
compiled `LevelScript::process`, so six `.rel.plt` entries rotated by 12 bytes.
The fix is a **faithful source-shape change** — the mission-125 state-3 arm is
an independent `if`, not an `else if` of the state-2 arm — which reproduces the
original block order and moves the `deleteHackingGame` call after the mission-0
state-9/11 blocks. The function stays exactly 57232 B and the whole gate is
unchanged; `.rel.plt` becomes byte-identical (3674/3674 entries).

## Result

```
                         before            after
official rounded pct     54.8              53.2
raw official ratio       54.806399         53.196256
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        20169 / 19965     20169 / 20003
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
.rel.plt name diff       6 (12 bytes)      0
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `verify.py`'s reconciled instruction streams of the linked
`.so`. The function's own fuzzy score drops 54.8 -> 53.2; the gate's
`byte_exact`/`linked_exact` are unchanged because the six rotated PLT symbols
are only called from `LevelScript::process` itself (which is not byte-exact
either way). The improvement is link-level: `.rel.plt` is now byte-identical.

## 1. The `.rel.plt` rotation

`readelf -rW` on both files: 3674 `R_ARM_JUMP_SLOT` entries, identical
`r_offset`s and types. Six names are rotated:

```
index   original                              ours (before)
2831    StarSystem::switchPlanetForIntro      PlayerEgo::deleteHackingGame   <-- moved
2832    Level::switchSkyboxForIntro           StarSystem::switchPlanetForIntro
2833    ParticleSystemManager::reset          Level::switchSkyboxForIntro
2834    PlayerEgo::startSmokeEmission         ParticleSystemManager::reset
2835    PlayerStation::setVisible             PlayerEgo::startSmokeEmission
2836    PlayerEgo::deleteHackingGame          PlayerStation::setVisible
```

Gold emits PLT relocations in input-relocation order, i.e. instruction order
inside `LevelScript::process`. In the base tree the first reference to
`deleteHackingGame` is at `0x13d15c`, before `switchPlanetForIntro`
(`0x13deca`); the original has it at `0x13ded4`, after `PlayerStation::
setVisible` (`0x13da1e`). The fix moves the whole mission-125 state>=4 block
after the mission-0 state-9/11 blocks:

```
                         original      after (ours)
switchPlanetForIntro     0x13d1ec      0x13d9f4
switchSkyboxForIntro     0x13d1f4      0x13d9fc
ParticleSystemManager    0x13d212/1c   0x13da1a/24
startSmokeEmission       0x13d396      0x13dcee
PlayerStation::setVisible 0x13da1e     0x13dfb4
deleteHackingGame        0x13ded4      0x13eb52
```

## 2. The source shape: independent state-3 arm

`src/game/world/LevelScript.cpp`, mission-125 chain (case 125):

```cpp
                }
                if (m_nState == 3) {          // was: } else if (m_nState == 3) {
```

The state-2 arm was already an independent `if` (wave-95 `05e996c88`); the
state-3 and state>=4 arms were joined to it as `else if`s. Making state-3
independent again is the shape Ghidra shows for the original chain (state-2,
state-3 and state>=4 are separate tests, with state>=4 the `else if` of
state-3). It changes clang's block placement: the mission-125 state>=4 arm is
emitted after the mission-0 state-9/11 blocks, so `deleteHackingGame` becomes
the last of the six first references.

The join/independent sweep (official verifier, `link_so`, `.rel.plt` checked):

| state-2 | state-3 | state>=4 | size | raw % | .rel.plt |
|---|---|---|---|---|---|
| indep | else-if | else-if | 57232 | **54.806399** | 6 (base) |
| else-if | else-if | else-if | 57244 | 40.930325 | 0 |
| else-if | indep | else-if | 57324 | 49.932772 | 6 |
| else-if | else-if | indep | 57380 | 36.972040 | 0 |
| else-if | indep | indep | 57340 | 49.800817 | 6 |
| indep | indep | else-if | **57260** | **51.779581** | **0** |
| indep | indep | indep | 57252 | 45.928655 | 0 |

Only the `indep / indep / else-if` shape is both faithful (Ghidra's chain) and
`.rel.plt`-clean. Its natural size is 57260 B, 28 B over the original.

## 3. Size-neutral shape: drop the w4702 re-fetches and the pad anchor

The wave-97 pass (`d37d674fe`, `levelscript-process-mission0-prologue-w4702.md`)
made the function size-exact by adding three `Level::getPlayer()` re-fetches
(two mission-0 `setCollide` sites and the mission-1 `rotate`) and a case-139
`setCollide` filler, replacing the old `m_nState == 0x7f` and `volatile int pad`
anchors. On the independent-state-3 shape those cost bytes we do not have, so
this pass reverts them to the cached `player` local and removes the remaining
`volatile int pad = delta;` anchor:

| change | size delta on the 57260 shape |
|---|---:|
| `m_pLevel->getPlayer()->rotate(...)` -> `player->rotate(...)` | -12 |
| remove `volatile int pad = delta; (void) pad;` | 0 (folded into the above) |
| state-3 `setCollide` re-fetch -> cached `player` | -8 |
| state-4 `setCollide` re-fetch -> cached `player` | -8 |
| case-139 `setCollide` filler -> cached `player` | 0 |
| **total** | **-28 -> 57232** |

The resulting source has no size anchors and no `volatile` locals. The three
re-fetches are faithful to the binary (the original really does re-call
`Level::getPlayer()` at `0x138a38`, `0x138d90` and `0x137da6`), so this is a
deliberate trade: they can be restored when the surrounding 45% of the body is
reconstructed and the natural size is closer to 57232. The case-139 filler was
already documented as a non-recovered size filler; reverting it is strictly
more faithful.

Measured stages (all size-exact unless noted):

| tree | size | raw % | .rel.plt |
|---|---:|---:|---|
| base `44225192c` | 57232 | 54.806399 | 6 |
| independent state-3 only | 57260 | 51.779581 | 0 |
| + cached mission-1 rotate | 57252 | 50.920948 | 0 |
| + pad anchor removed | 57248 | 53.063 | 0 |
| + cached state-3 `setCollide` | 57240 | 53.130 | 0 |
| + cached state-4 `setCollide` (**landed**) | **57232** | **53.196256** | **0** |

The last two rows are the "mission-0 arm swap gives 57240 B (not landable)"
shape from the wave-100 brief; the state-4 re-fetch revert is the missing 8 B.
A 29-way single-`Level::getPlayer()` revert sweep on the 57240 shape found no
better size-exact/`.rel.plt`-clean variant than the landed 53.196256 (best
alternatives: 53.195, 53.189, 53.185).

## 4. Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

`.rel.plt` check: `readelf -rW` on both files, compare the `R_ARM_JUMP_SLOT`
name columns (3674/3674 identical, `name diff 0`).
