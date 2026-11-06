# LevelScript::process — wave 123 / w7304 (extra `getPlayer()` call in the mission-0 case-8 block)

Status: **code change landed, 2026-11-06**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `1c8c9b20e` (wave-122 snapshot).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.254688355      60.256665340     (+0.001976985)
official verify pct      60.3              60.3             (rounded)
orig / ours insns        20169 / 20037     20169 / 20039
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so` (helper `/dev/shm/w7304/score.py`). The official verifier rounds
this function to 60.3% before and after; the raw ratio is the tie-breaker.

## Landed change (one call target)

`src/game/world/LevelScript.cpp`, mission-0 `case 8` block (source line 790):

```diff
                     if (m_nScriptTimerA >= 2001) {
                         Globals::sound->stop(158);
-                        m_pLevel->getPlayer()->setVisible(false);
+                        player->setVisible(false);
                         m_pCamera->setRumblePercentage(0.0f, 0);
                     }
```

`player` is the prologue local `m_pLevel->getPlayer()` (source line 612). The
original reuses that local here instead of calling `getPlayer()` again.

### Evidence

Original (`0x13d0e6..0x13d0f0`):

```asm
13d0e6  blx  FModSound::stop(int)          ; stop(158)
13d0ea  ldr  r0, [sp, #84]                 ; player  (prologue local slot 0x54)
13d0ec  movs r1, #0
13d0ee  blx  PlayerEgo::setVisible(bool)
```

Current (before the fix, `0x13d228..0x13d236`):

```asm
13d228  blx  FModSound::stop(int)
13d22c  ldr.w r0, [sl, #24]                ; m_pLevel
13d230  blx  Level::getPlayer()            ; <-- extra call
13d234  movs r1, #0
13d236  blx  PlayerEgo::setVisible(bool)
```

The original's `Level::getPlayer` call count is 23; the pre-fix build had 28
(5 extra). This is one of them. The fix keeps the body exactly 57232 B and does
not touch `.rel.plt` (`getPlayer` is still referenced elsewhere), so all other
functions keep their addresses and `byte_exact` stays 4506.

## Remaining root cause (unchanged)

The dominant residual is still the prologue register allocation:

| value | original | ours (baseline) |
|---|---|---|
| `messages` | `r5` | `r4` |
| `&Globals::status` | `r4` | `r9` |
| `enemies` | `r8` | `r8` |
| `delta` | `r6` | `r6` |
| `this` | `sl` (spilled 328x) | `sl` (spilled 196x) |

The original calls the accessors in the order `getMessages`, `getPlayer`,
`getEnemies`; our source calls `getEnemies`, `getPlayer`, `getMessages`.
Making the source faithful (`messages, player, enemies`) does give
`messages=r5`, but it still leaves `&Globals::status` in `r9` and grows the body
to **57416 B** (delta/messages stack slots swap to 72/76 and the body uses more
high-register 32-bit encodings), so it is not landable under the size-exact
gate. Measured this session:

| variant | raw % | size |
|---|---:|---:|
| baseline | 60.254688 | 57232 |
| baseline + landed `player->setVisible` | **60.256665** | 57232 |
| faithful accessor order (`m,p,e`) | 58.781380 | 57416 |
| faithful + landed fix | 58.783431 | 57416 |
| `emp` / `mep` / `mpe` accessor orders | 58.981380 / 58.764990 / 58.781380 | 57308 / 57416 / 57416 |
| `emp` + landed fix | 58.983422 | 57308 |
| state-3/4/5 arm permutations (`345`, `435`, `453`, `534`, `543`) | 57.22 / 57.23 / 59.23 / 54.99 / 54.99 | 57256 / 57248 / 57240 / 57220 / 57220 |

The `354` arm order (the current source order) is the only size-exact one.

### Other extra `getPlayer()` sites (measured, all reverted)

Every `m_pLevel->getPlayer()` in `process` was replaced one at a time with the
`player` local. The original uses the local at all of these sites (verified by
`ldr rX,[sp,#84]` / `mov rX, sl` with `sl=player` in the original), but only the
landed `case 8` site is size-exact and non-regressing:

| source line | call | raw % | size |
|---|---|---:|---:|
| 790 | `setVisible(false)` | **60.256665** | 57232 |
| 950 | `rotate(0,angle,angle)` | 57.425792 | 57336 |
| 1786 | `Vector camPos = ...getPosition()` | 59.486618 | 57224 |
| 3243 | `isInRocketControl()` (case 102) | 60.251691 | 57232 |
| 3790 | `*camPos += ...getPosition()` | 59.493073 | 57224 |
| 3838 | `geometry->setDirection(-dir,...)` | 59.501542 | 57224 |
| 4327 | `isInRocketControl()` (case 1) | 60.138782 | 57232 |

Fixing the mission-1 `player->rotate` (line 950) alone makes `player` live
across the `LODManager::forceUpdate` call, so the allocator colours it to `r4`
(displacing `messages` to `r5` and `&Globals::status` to `r9`) and the body
grows by 104 B. That is the same `r4`/`r9` tie-break the wave-121/122 notes
describe; it cannot be fixed from the prologue alone.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check  -j6   # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift  -j6   # 0 drifted fields
cmake --build cmake-build-match --target lint         # CLEAN
python3 /dev/shm/w7304/score.py                       # 60.256665340
```

Scratch helpers (`score.py`, `firstdiff.py`, `batch_gp*.py`, `perm_arms.py`,
`mir.sh`) live in `/dev/shm/w7304/` and are not part of the tree.
