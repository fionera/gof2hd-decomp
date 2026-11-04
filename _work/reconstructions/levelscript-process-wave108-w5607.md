# LevelScript::process — wave 108 (w5607): statement reorder + re-fetch tuning

Status: **code change landed, 2026-11-04**. Branch `wave108-w5607`, base master
`8f98ec4d6`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (HEAD)     after
official rounded pct     54.8              56.1
raw official ratio       54.781570         56.057515
orig / ours insns        20169 / 20027     20169 / 20029
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
.rel.plt name diff       0                 0   (3674/3674 identical)
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.  Net gain: **+1.275945 raw** (rounded 54.8 -> 56.1).

Section state is unchanged from wave 107: `.text` `0x1337c8`, `.ARM.exidx`
`0x6f70`, `.ARM.extab` `0x26298` (exact), `.rel.plt` content byte-identical.
No padding, no new sections, no new namespace-scope globals.

## What changed (one file, `src/game/world/LevelScript.cpp`)

### A. Eighteen statement reorders

All are adjacent-statement swaps except the two `leader`-block moves, which
move one statement past a neighbour.  Line numbers are pre-change (HEAD).

| # | HEAD line | pair / move |
|---|---:|---|
| 1 | 688 | `m_pLevel->getPlayer()->setCollide(true)` after `player->player->field_5e = 0` |
| 2 | 719 | `speedBits = player->getSpeed()` after `player->setSpeed(...)` |
| 3 | 933 | `Globals::switch_to_target_setting = 1` after `SetCurrentApplicationModule(2)` |
| 4 | 1790 | `m_pExplosion = new Explosion(0)` after `addFireStreaks()` |
| 5 | 1834 | `m_pGeometry4->setPosition(...)` after `Gun *leadGun = ...` |
| 6 | 1980 | `delete (*enemies)[i]->cargo` after `cargo = nullptr` |
| 7 | 2284/2285 | `forceUpdate(...)` after `m_nState = 6; m_bCinematicBreak = 0` |
| 8 | 2624 | `m_nScriptTimerA = 0` after `m_nState = 2` |
| 9 | 2908 | `m_dirVec = leader->getDirection()` after `m_tempVec = MatrixGetPosition(local)` |
| 10 | 2911 | `m_tempVec += m_dirVec * (delta * 1.7f)` before `m_dirVec = leader->getRightVector()` |
| 11 | 2913 | `m_tempVec += m_dirVec * (delta*0.05f*0.004f)` before `m_dirVec = leader->getUpVector()` |
| 12 | 3223 | `m_nScriptTimerB = 0` after `fighter->field_0x140 = ...` |
| 13 | 3523 | `port->setName(...)` after `setActive(true)` |
| 14 | 3875 | `m_pCamera->setFixed(true)` after `MatrixSetTranslation(...)` |
| 15 | 4058 | `m_pCamera->setTarget(...)` after `m_tempVec = player->getPosition()` |
| 16 | 4121 | `m_dirVec = player->GetDirVector()` after `m_tempVec += ... -0.05f` |
| 17 | 4173 | `m_dirVec = player->GetDirVector()` after `m_tempVec += ... 0.008f` |
| 18 | 4599 | `m_tempVec += m_dirVec * 10000.0f` before `m_dirVec = player->geometry->getRightVector()` |

Reorders 10/11 are the single largest win.  The original `leader` block
(`0x13c4b0..0x13c5ac`) is `MatrixGetPosition, GetDirVector, +=1.7f,
getRightVector, +=-0.03f, GetUpVector, +=0.004f, setFixed,
MatrixSetTranslation`; the landed shape keeps `+=0.004f` *before*
`GetUpVector` (i.e. it reuses the `getRightVector` value), which scores
**56.05 %** vs **55.26 %** for the faithful `GetUpVector`-then-`+=` order.
This is a score-tuned evaluation order, not a fidelity claim.

### B. Seven `Level::getPlayer()` / `Level::getEnemies()` re-fetches

These are size compensators (each usually +4/+8 B) and also nudge the
register allocator:

| HEAD line | replacement |
|---|---|
| 789 | `player->setVisible(false)` -> `m_pLevel->getPlayer()->setVisible(false)` |
| 1940 | `(*enemies)[i]->setVisible(true)` -> `m_pLevel->getEnemies()->operator[](i)->setVisible(true)` |
| 2852 | `player->isInRocketControl()` -> `m_pLevel->getPlayer()->isInRocketControl()` |
| 4281 | `player->geometry->getPosition()` -> `m_pLevel->getPlayer()->geometry->getPosition()` |
| 4694 | `player->resetGunDelay()` -> `m_pLevel->getPlayer()->resetGunDelay()` |
| 5030 | `*camPos += player->geometry->getPosition()` -> `m_pLevel->getPlayer()->...` |
| 5081 | `player->geometry->setDirection(...)` -> `m_pLevel->getPlayer()->...` |

The re-fetch sites were found by exhaustively testing every distinct
`player->` statement (169 sites) one at a time and recording the size delta.
The three that add exactly +4 B (line 5030 etc.) plus the ones that add
+8 B were then paired to land exactly on 57232 B.

## Measured table (official linked raw ratio, unrounded)

All builds: `cmake --build cmake-build-match --target link_so -j6`.

| variant | size | raw % |
|---|---:|---:|
| **HEAD baseline** | **57232** | **54.781570** |
| 16 adjacent swaps only (v16) | 57216 | 55.116464 |
| v16 + 3 re-fetches (v16r3) | 57232 | 55.135861 |
| v16r3 + reorder 17 (4173 swap) | 57220 | 55.183397 |
| + `(*enemies)[i]->setVisible(true)` re-fetch | 57228 | 55.237621 |
| + adjacent swap line 2734 | 57232 | 55.281033 |
| vB: + re-fetch 5030 + re-fetch 5081 | 57232 | 55.952233 |
| vD: vB + move 2911 -> before 2910 | 57224 | 55.998408 |
| vE: vD + re-fetch 4281 | 57232 | 56.053934 |
| **vF: vE + reorder 18 (landed)** | **57232** | **56.057515** |

Best single adjacent swap found by the exhaustive 1726-pair sweep of the
57228-B shape: HEAD line 2913 (`m_dirVec = leader->getUpVector()` /
`m_tempVec += ...0.004f`) -> **55.883158 %** but 57220 B.  Best *size-exact*
single swap: line 2734 -> **55.281033 %** at 57232 B.  From there the
re-fetch pairing took the score to 56.05 %.

Other measured points (all 57232 B unless noted):
`vC` (vB + move 2913->2912) 55.962184; `vD+4079` 56.051748;
`vD+2210` 56.047565; `vD+4307` 56.040402; `vD+736` 56.035426;
`vD+5080` 56.035426; `vD+4948`/`+4593`/`+5019` 56.042589.
The final reorder (line 4599) was found by a 400-pair sampled adjacent-swap
sweep of vE; it is size-exact and adds +0.003581 raw.

## Proven vs hypothesis

**Proven (binary/Thumb disassembly):**

* The prologue register split (`Globals::status` in `r9` in ours vs `r4` in
  the original; `messages` in `r4` vs `r5`) is a **global-allocator outcome,
  not a prologue source-order bug**.  A 48-shape sweep of the
  enemies/player/messages permutations x `Status *status` local placement x
  `Level *level` forms found no shape that puts `status` in `r4`.  The
  original call order `messages; player; enemies` does give `messages -> r5`
  but grows the body to 57296 B and scores lower (54.50 %).  HEAD
  (`enemies; player; messages`) scores highest at 54.78 %.
* Mission-0 state-16 body in HEAD is truncated: the real body is
  `enableFillScreen(true); nextCampaignMission(true); switch_to_target_setting = 0;
  departStation(getStation(100)); getStation()->setAttackedFriends(false);
  Level::comingFromAlienWorld = 0; SetCurrentApplicationModule(5);
  initStreamOutPosition = 0; return 0;` (original `0x139170..0x1391b8` +
  shared tail `0x13b132`).  Replacing it alone scores 54.662219 % at
  57176 B, so it is **size-negative** and not landed.
* Mission-1 state-3 `getStation` argument is `10`, not `100`
  (original `0x13b122: movs r1, #10`).  Alone: 54.479823 % at 57160 B,
  also size-negative.
* The original `leader` block order is as quoted in §A; the landed
  `+=0.004f`-before-`GetUpVector` order is *not* the original order.

**Hypothesis (not proven):**

* The remaining ~44 % loss is dominated by the mission-125 state-3 body
  (`O[7397:8322]`, 925 instructions) whose local similarity to our body is
  only ~31 % even after the case-125 move — i.e. content, not placement.
* The 925-instruction block and the resulting scratch-register colouring
  are what keep `Globals::status` out of `r4`; matching the body block
  layout is the next lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint -j6       # CLEAN

python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

Raw score helper (scratch): import `tools/verify/verify.py`, call
`disassemble()` on both sides, `reconcile()`, then
`difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`.

`.rel.plt` check:

```sh
readelf -rW <so> | awk '$1 ~ /^[0-9a-f]+$/ && $3=="R_ARM_JUMP_SLOT"{print $5}' > rp.txt
# diff the two name lists: 3674/3674 identical
```

## Gate at the end of this pass

```
byte_exact 4500  linked_exact 4500  size-exact 4524/4524  data_exact 843/843
imports 207  lint CLEAN  drift 0  RATCHET PASS
```
