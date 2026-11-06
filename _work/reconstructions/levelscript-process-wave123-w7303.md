# LevelScript::process — wave 123 / w7303 (mission-chain order + MBP census)

Status: **no source win**, note-only. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Base master `1c8c9b20e`. Source:
`src/game/world/LevelScript.cpp` (unchanged).

## Baseline / gate

```
raw ratio (difflib)      60.254688355
official verify pct      60.3
orig / ours insns        20169 / 20037
orig / ours size         57232 / 57232        (size-exact)
byte_exact               4506
linked_exact             4506
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN ✓
drift                    0
RATCHET                  PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
over `tools/verify/verify.py`'s reconciled instruction streams of the linked
`.so` (helper `/dev/shm/w7303/score.py`).

## What this pass established (all measured, not guessed)

### 1. The residual is ordering, not register naming

| metric | value |
|---|---:|
| raw difflib ratio | 60.254688355 |
| ratio with every register name masked to `R` | 65.502661294 |
| ratio with registers **and** `#imm` masked | 68.263443267 |
| bag-of-instructions overlap (multiset) | 16963 / 20169 = 84.1 % |
| mnemonic-only overlap | 98.1 % |
| `ldr` count orig vs ours | 5660 vs 5551 (orig +230 `ldr`, −120 `ldr.w`) |

So masking registers recovers only ~5 points: **the dominant residual is
instruction ordering, not the register map**. The `ldr` vs `ldr.w` skew is the
`this`-in-`sl` vs `this`-in-`[sp,#88]` allocation (16-bit `ldr rX,[sp,#88]`
vs 32-bit `ldr.w rX,[sl,#N]`).

### 2. The case-entry layout order is already identical

Extracting every `cmp r0,#N ; beq/bne loc_X` from the main
`switch (getCurrentCampaignMission())` dispatch gives the same case→layout
order on both sides:

```
29, 41, 80, 56, 125, 92, 157, 40, 102, 154, 24, 14, 21, 42, 91, 94, 105, 114
```

(orig offsets / ours offsets)
```
29  1f8a / 1d30     56  21fc / 1fe4    157  25f0 / 2718     21  2e0c / 2cf8
41  203e / 1dfe     125 2284 / 202a    40   27c0 / 28e6     42  2e92 / 2d7e
80  211c / 1ef8     92  22e6 / 243a    102  2940 / 2956     91  2f2a / 2e3e
                                       154  2a56 / 29ae     94  2f94 / 2e68
                                       24   2c18 / 2b32     105 318c / 3098
                                       14   2c9a / 2bb6     114 32c6 / 319a
```

The **order is a perfect match**; only the addresses drift. So the top-level
switch lowering and the case-label source order are already faithful. The
reordering is *inside* each case, between sub-blocks.

### 3. The top-level mission chain differs in exactly one place

Following the `bne` targets of the pre-switch chain in the original:

```
137d62 mission==0   -> 137e84 (mission-0 body)
137d76 mission==1   -> 1381b2 (mission-81 test)   [chain A]
1381b2 mission==81  -> 13832c (mission-78 test)
13832c mission==78  -> 138574 (mission-89 test)
138574 mission==89  -> 138c7a (mission-106 test)
...
13812a mission==5   -> 1382dc (mission-26 test)   [chain B]
1382dc mission==26  -> 1383e4 (mission-50 test)
1383e4 mission==50  -> 13876c (mission-51 test)
```

Chain A (mission-1's else) is `81, 78, 89, 106, 95-group`; chain B (the
`if (m_bStartSequence)` block's else) is `5, 26, 50, 51`.

Our build's chain B is `26, 5, 50, 51` — the source order at lines 1496/1506.
So **mission 5 and mission 26 are swapped relative to the original.**

Also: the original's `if (m_bStartSequence)` block *falls through* to the
mission-5 test at `0x13812a` (the `beq.w 13812a` at `0x137f82` and the
then-block both converge there). Ours ends the then-block with
`b.n 138794` (the switch guard) because MBP placed the mission-81 test
immediately after the then-block's constant island instead.

### 4. Call-set census (real fidelity deltas, not layout)

Counting `bl/blx` targets inside the function:

| callee | orig | ours |
|---|---:|---:|
| `Level::getPlayer` | 23 | **28** |
| `Level::getEnemies` | 34 | 33 |
| `Level::getLandmarks` | 15 | **17** |
| `Level::getStarSystem` | 15 | **17** |
| `ParticleSystemManager::reset` | 2 | **4** |
| `StarSystem::switchPlanetForIntro` | 1 | **2** |
| `Level::switchSkyboxForIntro` | 1 | **2** |
| `PlayerStation::setVisible` | 1 | **2** |
| `Status::getStanding` / `Standing::setStanding` | 1 / 1 | **2 / 2** |
| `PlayerEgo::PlayEngineSound` / `StopEngineSound` | 1 / 1 | **2 / 2** |
| `PlayerEgo::startSmokeEmission` / `autoTurretIsEnabled` | 1 / 1 | **2 / 2** |
| `PaintCanvas::TransformGetTransform` | 37 | 36 |
| `TargetFollowCamera::translate` | 57 | 56 |
| `Status::getStation` | 12 | 11 |
| `Status::departStation` | 9 | 8 |
| `KIPlayer::setActive` | 27 | 26 |

The `getPlayer` surplus (5) is the clearest: the original keeps the prologue's
`PlayerEgo *player` local at `[sp,#84]` and reloads it (`ldr r0,[sp,#84]`,
163 reloads); our source re-calls `m_pLevel->getPlayer()` at several of those
sites instead (31 `m_pLevel->getPlayer()` occurrences in the body).

### 5. Structural block-order census

`difflib` opcodes over the two streams: 12113 instructions match in order
(60.25 %). Unmatched: 8056 orig / 7924 ours instructions in **2278 / 2235
contiguous runs** (largest 280). So it is not a handful of moved blocks — it
is a global MBP permutation of the sub-blocks inside the case bodies.

Example: the 56-call `port->setName(*gameText->getText(3212) + ...)` block
(case 125, source line 3909) is emitted at orig `0x13b446` but at ours
`0x139da0` — a 0x16a6 displacement. In the original it is cold-placed at the
end of the case region; in ours it is inline at the case entry.

## Variants measured (all reverted; 57232 B unless noted)

| variant | raw % | ours size | note |
|---|---:|---:|---|
| **baseline** | **60.254688355** | 57232 | current master |
| swap mission 5 / mission 26 (faithful chain-B order) | 60.175098244 | 57232 | size-exact; chain becomes 81,5,78,89 |
| faithful prologue `messages, player, enemies` | 58.781379842 | 57416 | `messages=r5` ✔ but `status` still `r9`; `strd r6,r5,[sp,#72]` swaps the 72/76 slots |
| faithful prologue + 5/26 swap | 58.701763989 | 57416 | |
| mission-1 `player->rotate(...)` instead of `m_pLevel->getPlayer()->rotate(...)` | 57.425791844 | 57336 | faithful to `0x137da6` (`ldr r0,[sp,#84]`) but +104 B |
| mission-5 as a plain `if` before the mission-26 `else if` | 57.993878617 | 57484 | |
| case-125 `if (a) if (b) if (c)` → `if (a && b && c)` | 60.254688355 | 57232 | byte-identical codegen (neutral) |

The two faithful changes (prologue order, `player` local) both *lose* 1.5–3
points and grow the body, because they perturb the allocator's tie-breaks
without reproducing the original's spill-slot order. The baseline is a strong
local optimum.

## Root cause (unchanged from wave 121/122, now quantified)

1. `messages`/`status` colouring: the original calls
   `getMessages, getPlayer, getEnemies` in that order, so `messages` is
   coloured to `r5` (r4 still holds `&this->m_pLevel` at that point) and the
   later `Globals::status` load gets `r4`. Our source calls
   `getEnemies, getPlayer, getMessages`, so `messages` gets `r4` and
   `status` falls to `r9`, leaving `r5` free as a scratch register.
2. Because `r5` is free, the allocator uses `r5` where the original uses
   `sl`, so `this` stays live in `sl` and is reloaded 195× instead of 327×.
   That is the entire 230-`ldr`/120-`ldr.w` skew.
3. Fixing (1) from source alone also swaps the `[sp,#72]`/`[sp,#76]` spill
   slots (`messages`/`delta`), which costs more than it gains. The slot order
   is a PEI spill-order decision, not a source-order one.
4. The remaining ~40 % of the stream is MBP sub-block permutation inside the
   case bodies (2278 runs). The case-entry order already matches, so this is
   not a case-label ordering problem; it is branch-probability / cold-block
   placement inside each case.

## Next step that would actually move the number

Reproduce the original's *cold-block* placement inside the big cases. The
clearest lever is the case-125 `if (m_nState == 1)` block (the 56-call
`setName` loop): the original places it at `0x13b446` (end of the case
region), ours at `0x139da0` (inline). Any source shape that makes clang's
BranchProbabilityInfo treat that arm as cold (e.g. an early `m_nState`
dispatch that leaves the `== 1` arm as the unlikely successor) should move
it and cascade the rest of the case layout. That was not found in this pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields
cmake --build cmake-build-match --target lint           # CLEAN ✓
python3 /dev/shm/w7303/score.py                         # 60.254688355
```

Scratch helpers (`common.py`, `score.py`, `chain.py`, `switch.py`,
`calldiff.py`, `gaps.py`, `regmask.py`, `try.py`, `variants.py`,
`LevelScript.base.cpp`) live in `/dev/shm/w7303/` and are not part of the
tree. No source change was committed; the worktree is clean.
