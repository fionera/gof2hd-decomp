# LevelScript::process — wave 110 / w5804 (case-125 state chain + ascending case order)

Status: **code change landed, 2026-11-05**. Branch `wave110-w5804`, base master
`390f58052`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Raw fuzzy ratio **56.086578 -> 57.565732** (+1.479154), function size stays
**57232 B (size-exact)**, 20169 / 20032 instructions (was 20169 / 20026).
Rounded **56.1 % -> 57.6 %**.  The whole gate is green.

## Result

```
                         before (HEAD)     after
official rounded pct     56.1              57.6
raw official ratio       56.086578         57.565732
orig / ours insns        20169 / 20026     20169 / 20032
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
.text                    0x1337c8           0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26294           0x26290   (see below)
.rodata/.dynsym/.dynamic/.rel.dyn  exact    exact
sections                 28                28
file size                +44               +44
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so` (helper `/dev/shm/w5804/score.py`).

## What changed (one file, `src/game/world/LevelScript.cpp`)

Three things, in order of score impact:

1. **`case 125` state-0 arm de-nested** (the big lever, +1.4 raw).  The source
   was

   ```cpp
   if (m_nState == 0) {
       if (messages[1]->isTriggered()) { m_nState++; ...route... }
   } else if (m_nState == 1) {
       ...setName loop...
   }
   ```

   and is now the faithful two-statement form

   ```cpp
   if (m_nState == 0 && messages[1]->isTriggered()) { m_nState++; ...route... }
   if (m_nState == 1) { ...setName loop... }
   ```

   The state-0 arm sets `m_nState++` (0 -> 1), so the second `if` really does
   fall through into the state-1 body — that is the original's behaviour, and
   it is what the binary's LSDA row layout shows (the case-125 `setName` rows
   move from offset ~19494 down into the same low region as the original's
   `14224..14262`, i.e. below the 16384 ULEB boundary).  This shrinks the body
   by 8 B, compensated by item 3.

2. **Switch case source order changed to ascending**:
   `14,16,21,24,29,40,41,42,56,64,67,69,70,73,80,91,92,94,102,105,114,125,131,
   135,139,142,144,145,154,157,158`
   (was `158,131,64,145,135,142,144,139,69,73,67,70,29,41,80,56,92,157,125,40,
   102,154,24,14,21,42,91,94,105,114,16`).  Worth ~+0.16 raw on its own.

3. **Size compensation**: the 5th `if (player->isInRocketControl())` in the
   file (inside `case 102`, the state-0 arm) is written
   `if (m_pLevel->getPlayer()->isInRocketControl())`.  This is a re-fetch of
   the same pointer (the original also re-calls `Level::getPlayer()` there);
   it is size-neutral for the score and restores 57232 B after item 1 shrank
   the body by 8 B.

Plus five adjacent-statement reorders (all size-exact, found by the
adjacent-statement sweep):

| # | location | reorder |
|---|---|---|
| 1 | mission-0 state 4 | `Globals::sound->play(143, ...)` before `player->setTurretMode(false)` |
| 2 | mission-0 intro (timer >= 2151) | `(*enemies)[1]->setVisible(false)` before `(*enemies)[0]->setVisible(false)` |
| 3 | case 92-ish docked block | `m_pCamera->setRotationAroundTarget(true)` before `lookBehind()` |
| 4 | case 125 tuned-anchor block | `player->StopEngineSound()` before `m_pLevel->particleEmitBoolPtr->reset()` |
| 5 | case 16 tail | `Globals::appManager->SetCurrentApplicationModule(2)` before `Globals::switch_to_target_setting = 1` |

## `.ARM.extab` / section delta change (reported as required)

`LevelScript::process`'s own extab/LSDA record is now **348 B, exactly the
original's** (it was 352 B / +4 at HEAD).  Because that +4 contribution is
gone, the `.ARM.extab` **section** total moves from `0x26294` (4 B short of
the original `0x26298`) to **`0x26290` (8 B short)**.

Remaining per-function extab deltas (unchanged by this pass):
`Hud::draw` +4, `Level::createShip` -8, `Generator::createMission` -4.
So the section delta worsens by 4 B even though the target function's record
becomes exact.  All other section sizes/bases and the +44 file size are
unchanged; 28 sections; `check` still passes.

Every case-order variant that reproduces the 57.565732 score was measured to
also give `0x26290`, so the 4 B is inherent to this code shape, not a tuning
artefact.

## Measured variant table (all 57232 B unless noted)

| variant | raw % | size |
|---|---:|---:|
| HEAD (wave 109) | 56.086578 | 57232 |
| ascending case order only | 56.247979 | 57232 |
| **landed: ascending + case-125 de-nest + compensation + 5 swaps** | **57.565732** | **57232** |
| descending case order | 57.498943 | 57232 |
| HEAD case order with landed body | 57.343284 | 57232 |
| original body-layout order (derived from jump-table targets) | 57.178390 | 57240 |
| same, `case 158` first | 57.309883 | 57232 |
| case 91 nested `if (m_nState == 0) { if (isTriggered()) ... }` | ~56.41 | 57196 |
| case 102 `else if` -> separate `if` | 45.283301 | 57412 |
| case 40/64/80/91/154 `else if` -> separate `if` | 55.32 / 57.30 / 56.12 / 57.12 / 42.86 | 57320 / 57244 / 57212 / 57220 / 57432 |
| case 42 `else if` -> separate `if` | 57.565732 (identical code) | 57232 |

## Original case-body layout (derived, for future passes)

`clang -Oz` lays switch case bodies out in source order.  Reading the jump
table targets out of the binary gives the original's body layout:

```
131 64 135 145 142 144 139 69 73 67 70 29 41 80 56 125 92 157 40 102 154 24
14 21 42 91 94 105 114 16 158
```

with bodies at
`0x138aac, 0x138c56, 0x1391c0, 0x1392f6, 0x139544, 0x1395e0, 0x13965c,
0x139720, 0x139834, 0x139892, 0x1399a2, 0x139ca2, 0x139d56, 0x139e34,
0x139f14, 0x139f9c, 0x139ffe, 0x13a308, 0x13a4d8, 0x13a658, 0x13a76e,
0x13a930, 0x13a9b2, 0x13ab24, 0x13abaa, 0x13ac42, 0x13acac, 0x13aea4,
0x13afde, 0x13b014, 0x13d724`.

This is *not* the score optimum (57.18 / +8 B), so ascending order was kept.

## Search method / what was swept

`/dev/shm/w5804/sweep2.py <basefile> <lo> <hi>` swaps every pair of
consecutive same-indentation statements in a line range, rebuilds `link_so`
and rescoring with the official verifier, keeping only 57232-B improvements.

Ranges swept this pass (base = landed file): **2300-2776, 4050-4550,
4550-5000, 5294-end** (233 / 177 / 216 / 20 candidates).  **No improvement**
in any of them, so the adjacent-statement order is exhausted.

Also measured and rejected:

* prologue accessor order — the original emits `getMessages(); getPlayer();
  getEnemies()`, ours emits `getEnemies(); getPlayer(); getMessages()`.
  All 6 permutations of the three source lines were built: the current
  `enemies, player, messages` order is the score optimum (57.565732); the
  original's emitted order scores 56.693305 at 57316 B.  So the emitted call
  order is *not* the source order here — clang reorders the three
  independent calls.
* moving the mission-5 `else if` block up next to mission 1 (matching the
  original's dispatch order `0,1,5,81,26,78,50,89,42,106,...`): 40.491501
  at 57352 B.  Block placement is very sensitive; rejected.
* adjacent-case-swap sweep (29 swaps) and 24 random window shuffles: the
  ascending order is a broad plateau at 57.565732; nothing beats it.

## Proven vs hypothesis

Proven (measured, deterministic — fixed compiler + fixed link order):

* The landed file is size-exact (57232 B), `.text`/`.ARM.exidx`/file size
  unchanged, and the gate passes with byte 4500 / linked 4500 / lint CLEAN /
  drift 0.
* `LevelScript::process`'s extab record is now 348 B = the original's.
* The case-125 de-nesting is worth ~+1.4 raw and is the reason the LSDA rows
  move; ascending case order is worth ~+0.16 raw and is a local optimum.
* The original's mission dispatch chain is `0,1,5,81,26,78,50,89,42,106,95,
  99,109,119,126,133,160,161`; ours is `0,1,81,26,78,5,89,42,106,...`
  (mission 50 is folded into the `goto wormholeExitBody` branch).  Reordering
  the source to match costs ~17 raw points, so it is not a pure win.

Hypothesis / not established:

* The remaining ~42 % sequence loss is still global greedy register
  allocation (`this` lives in `r9` in ours but `sl` in the original;
  `&this->m_pLevel` is `fp` in ours vs `r4` in the original) plus
  MachineBlockPlacement in the mission-0/1/78 state machines.  The largest
  unaligned gaps are 297 / 280 / 238 / 230 / 223 instructions, all inside
  those state machines.
* The literal-pool slotting (`pool:pic:got:*` vs float constants at the same
  sequence position) is a consequence of block order, not of a missing
  source statement.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
cmake --build cmake-build-match --target drift -j6    # 0 fields
cmake --build cmake-build-match --target lint -j6     # CLEAN
python3 /dev/shm/w5804/score.py                       # raw ratio
```

Scratch helpers (not tracked): `/dev/shm/w5804/score.py`, `sweep2.py`,
`reorder_cases2.py`, `casesweep.py`, `rand2.py`, `split_if.py`,
`apply_split.py`, `extabsweep.py`, `lsda2.py`, `bigblocks.py`, `gapaddr.py`,
`best9.cpp` (the landed file), `head.cpp` / `head_sorted.cpp` (baseline and
the baseline with ascending case order, used for the minimal diff).
