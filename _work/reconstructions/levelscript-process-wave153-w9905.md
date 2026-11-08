# LevelScript::process — wave 153 / w9905: the mission-0 `(*enemies)[i]`
# status-r4 prologue is +112 on the wave-152 base; the -112 companion
# is not a single genuine statement removal (measured sweep)

Status: **research note, no landed source change**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9905`, base master `4a6104a43` (wave 152).  Scratch in
`/dev/shm/w9905/`.

## Result (HEAD, unchanged)

```
process raw diffbytes    55,347
process aligned insns    17,223
orig / ours size         57232 / 57232   (size-exact)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
```

## 1. The faithful loop shape is +112 on this base, not +132

w9807 measured `(*enemies)[i]` for both calls at **57,364 (+132)** on the
wave-151 base.  On the wave-152 base (w9805's three `__builtin_expect`
levers landed) the same source is **57,344 (+112)**, raw 55,105, aligned
21,745.  The w9805 expects are size-neutral on HEAD but add 20 bytes to the
faithful-loop allocation.  So the companion needed here is **-112**, not
-132.

The loop lever is `setVisible` only, exactly as w9807 found:

| loop shape (setDirection / setVisible) | size |
|---|---:|
| `enemyData[i]` / `enemyData[i]` (HEAD) | 57232 |
| `enemyData[i]` / `(*enemies)[i]` | 57344 |
| `(*enemies)[i]` / `enemyData[i]` | 57232 (byte-identical to HEAD) |
| `(*enemies)[i]` / `(*enemies)[i]` | 57344 |
| `(*enemies)[i]` / `enemies->data_[i]` | 57344 |
| `KIPlayer *e = (*enemies)[i]` (both) | 57344 |

## 2. Prologue register map (the deliverable)

`(*enemies)[i]` for `setVisible` reproduces the original's
`&Globals::status`-in-`r4` prologue.  Full map (orig / faithful-ours):

| value | original | faithful ours |
|---|---|---|
| `this` | `sl` | **`r9`** |
| `&__stack_chk_guard` | `fp` | **`sl`** |
| `&m_pLevel` (temp) | `r4` | `r4` |
| `messages` | `r5` | `r5` |
| `delta` | `r6` | `r6` |
| `enemies` | `r8` | `r8` |
| `player` | spilled from `r0` to `[sp+84]` | **`fp`**, then `stmia` to `[sp+76]` |
| `&Globals::status` | **`r4`** | **`r4`** (match) |
| frame size | `0x1b0` (432) | **`0x1a8` (424)** |
| canary slot | `[sp+428]` | **`[sp+420]`** |
| stack slots | msg/delta `[sp+72]`, &level `[sp+80]`, player `[sp+84]`, this `[sp+88]`, status `[sp+92]`, enemies `[sp+96]` | msg/delta/player `[sp+68/72/76]`, this/status `[sp+80/84]`, enemies `[sp+88]`, &level `[sp+64]` |

So the faithful loop gets `status` into `r4` (the mission lever) but rotates
`this` from `sl` to `r9`, the canary address from `fp` to `sl`, keeps
`player` live in `fp` instead of spilling it straight from `r0`, and moves
the whole spill frame down by 8 bytes.  The mission-0 loop then uses
`r9 = &{0,1,0}`, `fp = 1.0f`, `r4 = enemies` where the original uses
`fp = &{0,1,0}`, `r4 = 1.0f`, `r9 = enemies` — a cyclic rotation of
`r4/r9/fp` driven by the `this`/canary swap.

The prologue+mission-0 block is actually 4 B *shorter* than the original
(0x137d18..0x137f78 = 0x260 vs 0x137d18..0x137f7c = 0x264).  The whole +112
accumulates **after the shared tail** (0x137f7c): the faithful build has
15 `tbh` state tables at addresses within ~±40 B of HEAD's up to
`0x13978e`, then diverges (+80 at `0x13a1b0`).  The per-mission case-entry
order is unchanged (same 31-entry table as HEAD).

## 3. The -112 companion: measured single-statement removals

Every single-line statement in the mission-0 block (lines 615-936) was
removed one at a time on the faithful base (57,344) and rebuilt:

| removed statement | size | delta |
|---|---:|---:|
| `m_pLevel->lodManager->forceUpdate(30, false)` (state 0) | 57220 | **-124** |
| `AbyssEngine::AEMath::MatrixSetTranslation(*local, ...)` | 57272 | -72 |
| `m_pCamera->translate(...)` (state 5 / 12) | 57272 | -72 |
| `m_pCamera->setRumblePercentage(rumble, 20)` (state 6) | 57288 | -56 |
| `m_pCamera->translate(...)` (state 6) | 57288 | -56 |
| `m_pCamera->translate(...)` (state 3) | 57296 | -48 |
| `player->rotate(spin, spin, spin)` | 57304 | -40 |
| `(*enemies)[i]->geometry->translate(0, wobble, 0)` | 57308 | -36 |
| `player->setSpeed(...)` / `updateEvent3DAttributes(161,...)` | 57312 | -32 |
| `(*enemies)[i]->geometry->setVisible(true)` | 57340 | -4 |
| `(*enemies)[i]->geometry->setDirection(...)` | 57368 | **+24** |
| `m_nState = 8;` | 57376 | +32 |

The shared-tail `m_pLevel->lodManager->forceUpdate(30, false)` is worth
**-12**; removing both forceUpdates is **-132** (57,212).

No single removal is -112.  The closest single removals are -124
(state-0 forceUpdate) and -96 (the whole matrix block).

**The state-0 `forceUpdate(30, false)` is genuinely redundant**: the
original calls it at `0x137f72` and then, after restoring `fp`/`sl`,
the shared tail calls the identical `forceUpdate(30, false)` again at
`0x137f90` (both `ldr r0,[sp+80]; movs r1,#30; movs r2,#0; ldr r0,[r0];
ldr r0,[r0]; blx`).  Removing it is a semantic no-op but a faithfulness
regression (the original emits both).

## 4. Combinations tried for exactly 57,232 (none hit)

All built + linked on the faithful base:

| variant | size |
|---|---:|
| remove state-0 forceUpdate (`nofu`) | 57220 |
| `nofu` + duplicate the shared-tail forceUpdate | 57228 |
| `nofu` + `m_nState = 13` -> `++` | 57228 |
| `nofu` + `m_nState = 6` (state 5) -> `++` | 57228 |
| `nofu` + `m_nState = 14` -> `++` | 57236 |
| `nofu` + `m_nState = 8` -> `++` | 57268 |
| move state-0 forceUpdate before the loop | 57228 |
| move it after `m_nState = 1` | 57236 |
| move it before `enemies[0]->setPosition` | 57196 |
| remove both forceUpdates | 57212 |
| `both_fu` + `m_nState = 14` -> `++` | 57228 |
| revert w9805 expects + `both_fu` | 57236 |
| revert w9805 expects + `nofu` | 57244 |
| `Level *level = m_pLevel;` prologue | 57352 |
| `Level *level` + `nofu` | 57220 |
| `ei01n` nested case-125 form | 57356 |
| `__builtin_expect(m_nState <= 2, 1)` | 57224 |

A 2-way brute force over 11 small levers (dup, s13, c6, 650++, 729++, 738++,
667++, 765++, 1022++, `m_bStartSequenceOver=1`, 1011++) found **no pair at
57,232**; every near miss is 57,228 or 57,236.

The case-125 `else if` joins were also measured on the faithful base (they
are independent `if`s at HEAD): `01` (state0/1) 57,376; `12` (1/2) 57,548;
`23` (2/3) 57,292; `01+12` **57,240**; `12+23` 57,344; `01+12+23`
**57,112**; `01+12` + state-0 forceUpdate removal 57,148; `01+12` + the
shared-tail duplicate 57,248.  None is 57,232.

Because `byte_exact` is same-address, a non-size-exact process shifts every
following function and drops `byte_exact` 4507 -> 4329 (measured with the
57,228 variant).  The faithful loop therefore **cannot be landed** until a
true -112 companion is found; the note is committed instead.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9905/fdiff.py _ZN11LevelScript7processEi
python3 /dev/shm/w9905/align_diff.py _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j6
```

Scratch scripts: `/dev/shm/w9905/{base_faithful.cpp,sweep_lines.py,
sweep3.py,incsweep.py,pairbf.py,bothsweep.py,fublsweep.py}`.

## 6. Open questions

1. The -112 companion is not a single mission-0 statement removal.  The
   state-0 forceUpdate removal (-124) is 12 B too big; a genuine +12
   faithfulness fix would land it.  No +12 lever was found among
   `m_nState = N` -> `++` conversions (all are 0/+8/+16/+48).
2. The residual is a whole-function re-colour driven by `this` in `r9`
   instead of `sl` (and `player` in `fp` instead of spilled).  A source
   shape that spills `player` straight from `r0` (as the original does)
   while keeping `status` in `r4` would likely remove the +112 outright.
   `Level *level = m_pLevel;` gets the `fp`/`r4`/`r5` prologue but moves
   `player` to `[sp+88]` and is +120.
3. `nofu` + duplicate shared-tail forceUpdate is 57,228 (4 B short); a
   fourth `m_nState`-increment lever or a 4-byte statement reorder is the
   remaining gap.
