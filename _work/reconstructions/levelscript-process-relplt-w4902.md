# LevelScript::process — `.rel.plt` first-reference rotation (w4902, wave 99)

Status: **note only, no source change landed**. Branch `wave99-w4902`, base master
`44225192c`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

This pass investigated the last known structural defect of the function: the
12-byte rotation in `.rel.plt` around `PlayerEgo::deleteHackingGame`. Every
size-exact shape that repairs the rotation was measured with the official
verifier; all of them regress the official raw score below the current
`54.806399`. The source is therefore left untouched.

## Baseline (confirmed with `check`)

```
byte_exact 4500/4524, linked_exact 4500/4524, size-exact 4524/4524,
stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0,
.text 0x1337c8, .rodata 0x41b67, .ARM.exidx 0x6f70, .ARM.extab 0x26294 (-4)
RATCHET PASS
```

Official raw ratio for the function: **54.806399** (oracle raw `55.874253`).
The oracle is `_work/reconstructions/w3692_oracle.py`; the official number is
`tools/verify/verify.py` on the linked `.so`. The oracle overestimates this
function by roughly 1.07 points and its ranking does not transfer reliably, so
every candidate below was confirmed with the official verifier.

## The rotation

Original `.rel.plt` entries 2831..2836:

```
2831 StarSystem::switchPlanetForIntro
2832 Level::switchSkyboxForIntro
2833 ParticleSystemManager::reset
2834 PlayerEgo::startSmokeEmission
2835 PlayerStation::setVisible(bool)
2836 PlayerEgo::deleteHackingGame
```

Ours (base):

```
2831 PlayerEgo::deleteHackingGame      <-- one slot too early
2832 StarSystem::switchPlanetForIntro
2833 Level::switchSkyboxForIntro
2834 ParticleSystemManager::reset
2835 PlayerEgo::startSmokeEmission
2836 PlayerStation::setVisible(bool)
```

`readelf -rW` reports 3674/3674 entries, `same off 3674`, `same info 3668`,
`same name 3668`; the only difference is the rotation.

First-reference offsets in the linked `.so`:

| symbol | original | ours |
|---|---:|---:|
| `PlayerEgo::deleteHackingGame` | `0x13ded4` | `0x13d154` |
| `StarSystem::switchPlanetForIntro` | `0x13d1ec` | `0x13deca` |
| `Level::switchSkyboxForIntro` | `0x13d1f4` | `0x13ded2` |
| `ParticleSystemManager::reset` | `0x13d212` / `0x13d21c` | `0x13def0` / `0x13defa` |
| `PlayerEgo::startSmokeEmission` | `0x13d396` | `0x13e17c` |
| `PlayerStation::setVisible(bool)` | `0x13da1e` | `0x13e496` |

Object-level first references (what actually determines relocation order):

```
deleteHackingGame        0x543c
switchPlanetForIntro     0x61b2
switchSkyboxForIntro     0x61ba
ParticleSystemManager::reset  0x61d8 / 0x61e2
startSmokeEmission       0x6464
PlayerStation::setVisible 0x677e
```

## Root cause

The five mission-0 calls live in the mission-0 intro choreography
(`case 9` of the start-sequence `switch (m_nState)` at
`src/game/world/LevelScript.cpp` line ~826, plus `startSmokeEmission` in
`case 11` and `PlayerStation::setVisible` later). The `deleteHackingGame` call
lives in the mission-125 hacking block (`case 125`, `m_nState >= 4`).
The block contents are instruction-for-instruction identical between the two
binaries; only their **layout order** differs:

- original: mission-0 `case 9` block at `0x13d1de`, mission-125 hacking block
  at `0x13de80`+ (mission-0 first);
- ours: mission-125 state-3 + hacking blocks at `0x13d0xx`, mission-0 `case 9`
  block at `0x13deca` (mission-125 first).

So the defect is not a missing call or a wrong receiver; it is LLVM's
`MachineBlockPlacement` ordering the mission-125 cold chain before the
mission-0 cold chain. The whole mission-125 state-2/3/hacking chain is shifted
by about `0xd80` relative to the original, and the mission-0 state-7/8/9 chain
by about `0xd00` in the opposite direction. The rest of the function inherits
the shifted block layout, which is the main reason the raw ratio is only
~54.8%.

## Experiments

All variants keep the function body at exactly `57232 B` unless stated. The
"oracle" column is the unrounded `SequenceMatcher` ratio; "official" is the
linked-`.so` raw ratio. `PLTok` means the object-level first-reference order
is `switchPlanet < switchSkybox < reset < startSmoke < setVisible < delete`.

### 1. Duplicated mission-0 calls at the top of a mission-125 arm (early block)

Inserting a dead guard block

```cpp
if (delta == 0x7d) {
    m_pLevel->getStarSystem()->switchPlanetForIntro();
    m_pLevel->switchSkyboxForIntro();
    m_pLevel->particleRenderBoolPtr->reset();
    player->startSmokeEmission();
    ((PlayerStation *) (*m_pLevel->getLandmarks())[0])->setVisible(false);
}
```

at the top of the mission-125 state-3 body forces the five calls to be emitted
before `deleteHackingGame`. This is **not faithful source** (a never-taken
guard with unrelated-class casts; `delta == 125` is possible at runtime) and
costs 56 bytes:

| variant | size | oracle | PLT |
|---|---:|---:|---|
| base | 57232 | 55.874 | bad |
| early block, mission-125 state 3 | 57288 | 55.732 | ok |
| early block, mission-125 state 2 | 57288 | 55.727 | ok |
| early block, mission-125 state 4 | **57280** | 55.706 | ok |
| early block, mission-0 state 4 | 57288 | 55.736 | ok |
| early block, mission-0 state 3 | 57288 | 55.741 | ok |
| early block, mission-0 state 5 | 57288 | 55.731 | ok |
| early block, mission-0 `switch` top | 57232 | 55.874 | bad |
| early block, after `enemies = ...` | 57296 | 49.669 | ok |

The mission-0 placements are slightly better on the oracle but still cost
56 bytes. The state-4 placement is the cheapest at +48.

### 2. Compensating the early block with `m_pLevel->getPlayer()->` -> `player->`

Each replacement of `m_pLevel->getPlayer()->X` by `player->X` usually saves
8 bytes. A single-replacement sweep on top of the state-4 early block found
the 8-byte savers at source lines 697, 721, 1469, 1471, 1907, 2999, 3002,
3003, 3175, 3968, 4221, 4587, 4812 (occurrences 3, 4, 6, 7, 10, 11, 12, 13,
14, 20, 22, 25, 27). Occurrences 8, 9, 15, 16, 17, 18, 23, 24, 26 save 0;
occurrence 19 saves 4; occurrences 5 and 21 add 32.

The best size-exact PLT-exact combination found is the handoff's
`cand_best.cpp`: the state-3 early block plus nine replacements
`(3,4,6,7,9,10,15,16,25)`:

```
size 57232, oracle 55.808, official 54.732069, rel.plt 3674/3674 exact
```

Other size-exact PLT-exact combinations:

| variant | size | oracle | official |
|---|---:|---:|---:|
| `cand_best` (state-3 block + 9 repl.) | 57232 | 55.808 | **54.732069** |
| state-4 block + `(10,22,25,6,7,20)` | 57232 | 55.731 | 54.657797 |
| state-4 block + `(10,22,25,6,7,14)` | 57232 | 55.721 | 54.647831 |
| state-3 block + `(10,25,6,7,22,11,12)` | 57232 | 55.727 | 54.595281 |
| `v_opt2_auto_r2999.cpp` (early block + anchor changes) | 57232 | — | 54.624277 |

`cand_best` is the best PLT-exact shape found, but it is still
`0.074330` below the base official score. Note that it also undoes the two
w4702 mission-0 `setCollide` re-fetches (occurrences 3 and 4) and the
case-139 state-1 size filler (occurrence 9), which is why the oracle gain from
the replacements does not survive the official verifier.

### 3. Non-early-block shapes

Every shape that moves the mission-125 hacking block without a duplicated
call either changes the size or destroys the score:

- **Mission-125 arm separator sweep** (join/split state 2/3/4, 16
  combinations): only `CCAC` (state-2/3 join) gives `PLTok`, at
  `57260 B`, oracle `52.786`. No size-exact PLT-exact shape.
- **Mission-125 arm permutations** (`234`, `243`, `324`, `342`, `423`, `432`):
  all `PLTbad`; sizes `57232/57252/57232/57608/57264/57272`.
- **Move mission-0 `case 9` before the switch / to the function top**: layout
  and score unchanged or worse.
- **Move `case 9` into mission-125 state 3 with a guard**: `57252 B`,
  oracle `53.503`, `PLTok`.
- **Move mission-125 state 4 after the switch**: `57440 B`, oracle `45.225`,
  `PLTok`.
- **`goto` to defer `deleteHackingGame`**: `57360 B`, oracle `43.043`,
  `PLTok`.
- **Pending-delete flag + call at end**: `57928 B`, oracle `42.749`, `PLTok`.
- **`__builtin_expect` on the mission-125 state-4 condition**: mostly no
  change; `expect(...,0)` gives `57236 B`, oracle `49.682`.
- **`__builtin_expect` on the mission-125 state-2/state-3 conditions**:
  `PLTok` but oracle `48.279` / `55.384`.
- **`volatile int pad` anchor moves**: no PLT fix; removing the pad gives
  `57232 B`, official `54.501`.
- **Reordering mission-0 switch cases 6..15**: byte-identical output.
- **Reordering the top-level per-mission `case` blocks** (move `case 125`
  first/last/before/after 40/157/158, reverse order): size stays `57232` for
  most, oracle `55.577..55.874`, but all remain `PLTbad`.
- **Splitting the mission-0 `else if` chain** (states 0..5): the state-4 split
  flips the PLT order but shrinks the body to `57172 B` and drops the oracle
  to `50.147`; the state-3/state-5/state-<=2 splits give `57340/57368/57364 B`
  and oracle `47.8..49.9`.
- **`__builtin_expect` on the mission-0 chain conditions**: the state-5
  `expect(...,0)` gives `57224 B`, oracle `55.784`, object-level `PLTok`, but
  the linked `.rel.plt` is still not exact (`same info 3648`) and the
  official score is `54.713079`.

## Conclusion

The `.rel.plt` rotation is fully explained and can be repaired, but every
repair that keeps the body at exactly `57232 B` also perturbs the block
layout / register allocation enough to lose more official score than the
rotation is worth:

```
base                        57232   official 54.806399   PLTbad
best PLT-exact repair       57232   official 54.732069   PLTok
```

No strict improvement was found, so `src/game/world/LevelScript.cpp` is left
unchanged. The remaining work is to recover the original block-placement
source shape (most likely the real structure of the mission-0 intro state
machine and/or the mission-125 hacking arm) rather than to add a duplicated
call block.

## Reproduction

```
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 /dev/shm/w4902/rawscore.py cmake-build-match/libgof2hdaa.so
python3 /dev/shm/w4902/relplt.py                          # rotation diff
```

Scratch helpers and all candidate sources are under `/dev/shm/w4902/`
(`cand_best.cpp`, `s4_G.cpp`, `s4_H.cpp`, `s3_C.cpp`, `s5exp0.cpp`,
`screen.py`, `rawscore.py`, `relplt.py`). The official verifier is
`tools/verify/verify.py`; the oracle is `_work/reconstructions/w3692_oracle.py`.
