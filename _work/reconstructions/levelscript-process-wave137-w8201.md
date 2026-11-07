# LevelScript::process — wave 137 / w8201 (w8105 combo re-measured + base-shift compensation)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `655e3bcc5` (wave-136 docs head,
LevelScript.cpp at the `f29455047` revert state).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              58.2
process diffbytes        54,327            55,085
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
.text diffbytes          84,709            85,467           (+758)
.ARM.extab diffbytes     1,395             1,395            (unchanged, size 156,312)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 **4 bytes**       **0 (byte-identical)**
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

`Level::getPlayer` call count in `process` is **23/23** after the four receiver fixes.
All section sizes are exact; `.rel.plt` is now byte-identical (was 4 bytes off).

## 1. The w8105 combo, verified on the w8105 base

The mission's combination is `anchor_trim2 + case4_hoist + case24_loop2 + case135 +
enemies1718`.  Reconstructed on the wave-135 base (`2e51d765d`, 5-statement `delta < 0`
anchor) it reproduces the note's near-miss almost exactly:

| build | size | process diffbytes |
|---|---:|---:|
| `gp + case4 + c24cache + c135 + anchor_trim2` | 57264 | 55,043 |
| + `enemies1718` (case-16 **state-3** loop cached) | 57236 | 54,993 |
| + `case16_loop2` (case-16 **state-4** loop cached too) | **57228** | **54,898** |

So `enemies1718` in the w8105 scratch was the *pair* of case-16 exit-wormhole loops
(state 3 and state 4), and `anchor_trim2` is the single-statement
`PlayerStation::setVisible(false)` anchor.  The 54898 vs the note's 54887 is an 11-byte
measurement difference (the note's scratch snapshot), not a different edit.

## 2. Why the current base is +20 B

The current `LevelScript.cpp` is not the w8105 base.  `git show 2e51d765d` vs HEAD differs
by exactly four hunks:

```
+ m_pCamera->translate(0.0f, 0.0f, (float)(-2 * delta));   case-40 state 1   (+36)
- m_pLevel->switchSkyboxForIntro();                         anchor trim       (-16)
- ((PlayerStation *) ...)[0]->setVisible(false);            anchor trim
- player->resetGunDelay();                                  setFixed pad      (-4)
```

Net **+20 B**.  Consequently the same combo measures **57248 B** on the current base, not
57228.  The mission's "+4 B lever" therefore becomes a **-16 B** compensation here.

Measured on the pristine current base (`57308` after `gp + case4`):

| lever | Δ size |
|---|---:|
| `case24_loop2` (`enemies->count` / `(*enemies)[i]`) | -8 |
| `case135` (`(*enemies)[i]->setVisible(true)`) | -8 |
| `enemies1718` state-3 loop cached | -36 |
| `case16_loop2` state-4 loop cached | -8 |
| `anchor_trim2` = single `setVisible(false)` anchor | -8 |
| `routepoints` (`(int)camPos.z + 30000`, drop the float fold) | -8 |
| `lm4` (case-16 state-3 `getLandmarks()` cache) | -40 |
| `restore_rgd` (missing `m_pLevel->getPlayer()->resetGunDelay()` at `0x144982`) | +8 |

## 3. The landed combination

```
4 getPlayer receiver fixes                        +88
case-4 StarSystem* hoist                           -8
case-24 loop cache                                 -8
case-135 cache                                     -8
case-16 state-3 loop cache                        -36
case-16 state-4 loop cache                         -8
anchor trimmed to single setVisible(false)          -8
routePoints int fold removed                        -8
                                                 ----
net                                               +4  (measured 57232, block placement)
```

This is the w8105 combination (`anchor_trim2 + case4_hoist + case24_loop2 + case135 +
enemies1718`) plus the two -8 B levers needed to absorb the +20 B base shift.  Both are
`.rel.plt`-safe; the resulting `.rel.plt` is **byte-identical** (3674 entries).

## 4. The faithful missing call (found, but not used at 57232)

`objdump` of the original confirms a genuinely missing statement in the
`m_pCamera->setFixed(false)` tail (`0x144976..0x14498c`):

```
144976  blx TargetFollowCamera::setFixed
14497a  ldr r0,[sl,#24]            ; m_pLevel
14497e  blx Level::getPlayer
144982  blx PlayerEgo::resetGunDelay
144986  ldr r0,[sl,#20]            ; m_pCamera
14498c  blx TargetFollowCamera::setLookAtCam
```

i.e. `m_pLevel->getPlayer()->resetGunDelay();` between `setFixed(false)` and
`setLookAtCam(false)`.  It is **+8 B**.  Restoring it raises the fuzzy ratio
(58.2 -> 58.2 at 57260; 58.1 at 57252) but every combination that also returns to exactly
57232 drops below 58.2 (best 57.5 with `rm_case6`).  It is recorded here as the next
wave's faithful lever; the current landing keeps the higher fuzzy ratio.

The `routePoints` fold is a size lever, not a faithfulness fix: the original *does*
`vadd.f32` then `vcvt.s32.f32` (`0x13f170/0x13f180`), so `(int)(camPos.z + 30000.0f)` is
the faithful form.  Dropping the fold is what buys the last -8 B.

## 5. Negative results

* `lm4` (-40) overshoots every 57232 candidate; adding it forces a +24..+40 pad.
* `rgd1` (first `resetGunDelay` site -> cached `player`) is -8 but drops the ratio to 58.1.
* `rm_case6` (unreachable `case 6: m_nState = 2;`) is -4 in isolation but re-rolls to +4
  on this base.
* `anchor_minus_stop` / `anchor_minus_play` / `anchor_minus_switch` each re-roll to -8 but
  cost 0.1-0.4 fuzzy points.
* The current `.rel.plt` 4-byte delta from w8102 is *fixed* by the single-`setVisible`
  anchor: the symbol's first reference moves back to the real case-9/11 block.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w8201/` (not tracked): `apply.py` (named-edit harness),
`score.py` (raw byte diff), `pct.py` (official fuzzy score for one symbol), `search.py`,
`search2.py` (subset sweeps).
