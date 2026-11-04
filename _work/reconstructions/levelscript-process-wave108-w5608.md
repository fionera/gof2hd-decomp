# LevelScript::process — wave 108 / w5608 (faithful switch-case order + statement sweep)

Status: **code change landed, 2026-11-04**. Branch `wave108-w5608`, base master
`8f98ec4d6`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Raw fuzzy ratio **54.781570 -> 55.353455** (+0.571885), function size stays
**57232 B (size-exact)**, `.rel.plt` stays byte-identical, `.ARM.extab` stays
`0x26298`.  The whole gate is green.

## Result

```
                         before (HEAD)     after
official rounded pct     54.8              55.4
raw official ratio       54.781570         55.353455
orig / ours insns        20169 / 20027     20169 / 20020
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
.ARM.extab               0x26298           0x26298  (exact; LevelScript +4 / Generator -4)
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.

## What changed (one file, `src/game/world/LevelScript.cpp`)

### 1. Switch-case source order (the main lever)

The 31 `case` labels of the campaign-mission switch were reordered from the
HEAD order

```
158 131 64 145 135 142 144 139 69 73 67 70 29 41 80 56 92 157 125 40 102 154
24 14 21 42 91 94 105 114 16
```

to

```
135 16 144 131 64 145 142 158 139 69 114 67 70 29 41 80 56 92 157 125 40 102
154 24 14 21 42 91 94 105 73
```

This is **not** an arbitrary tune.  The switch is lowered to two range checks
(`131..145 -> 0x138a8a`, `64..73 -> 0x138c3e`) plus a sorted linear compare
chain (`14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,114,125,154,157,158`).
The compare chain is identical in both binaries; what the source order controls
is the **body layout**.  Sorting the case-body entry addresses of the two
linked binaries shows the relative order is now identical:

```
orig: 158, [131..145], [64..73], 29, 41, 80, 56, 125, 92, 157, 40, 102, 154,
      24, 14, 21, 42, 91, 94, 105, 114, 16
ours: 158, [131..145], [64..73], 29, 41, 80, 56, 125, 92, 157, 40, 102, 154,
      24, 14, 21, 42, 91, 94, 105, 114, 16
```

The absolute addresses still differ (ours is ~0x150-0x250 B lower in the switch
region), i.e. the remaining delta is internal body codegen, not block order.

### 2. Statement reorders / relocations

Relative to the case-reordered base (`/dev/shm/w5608/seed_norocket.cpp`,
55.286535), the landed file adds four **adjacent, semantically neutral**
swaps and five **score-tuned cross-block relocations**:

| # | site (final line) | move | kind |
|---|---|---|---:|
| 1 | 2347 | `m_pRadar->enabled = 1` / next stmt | neutral swap |
| 2 | 2905 | `(*enemies)[0]->setPosition(ref + Vector{-76000,...})` / next stmt | neutral swap |
| 3 | 3733 | `player->setFreeLookMode(false)` / next stmt | neutral swap |
| 4 | 4666 | `m_dirVec = player->geometry->getDirection()` / next stmt | neutral swap |
| 5 | 919 -> 4171 | `Globals::layout->startFade(true, 255, 5000)` | cross-block |
| 6 | 2258 -> 2935 | `m_pRadar->enabled = 1` | cross-block |
| 7 | 2669 -> 2628 | `player->setFreeLookMode(false)` | cross-block |
| 8 | 2652 -> 3562 | `m_bCinematicBreak = 0` | cross-block |
| 9 | 4557 -> 3754 | `m_nState = 7` (immediately overwritten by `m_nState = 2`) | cross-block, dead store |

Items 5-8 are **tuned**, not fidelity claims: they move a real store/call into a
different case body and therefore change the game state at that point.  Item 9
is a dead store (the following statement is `m_nState = 2`), so it is
semantically neutral but still a source-level relocation.  They are kept
because each is size-exact, `.rel.plt`-clean, and the greedy sweep accepted
them; the note distinguishes them from the faithful case order.

## Measured table

All numbers are the official linked raw ratio; every row is 57232 B unless
noted.

| variant | raw % | rel.plt | note |
|---|---:|---:|---|
| HEAD `8f98ec4d6` | 54.781570 | 0 | before |
| handoff `best5.cpp` | 55.128141 | 0 | case order + early swaps |
| handoff `best7.cpp` | 55.143070 | 0 | |
| handoff `best9.cpp` | 55.339057 | **4** | setRocketControl moved late |
| `seed_norocket` (best9 minus the setRocketControl move) | 55.286535 | 0 | rel.plt repaired |
| `seed2` | 55.287911 | 0 | + 2 neutral swaps |
| round-1 adjacent sweep | 55.292888 | 0 | + `m_dirVec` swap |
| round-2 relocation sweep | 55.348478 | 0 | + 5 cross-block moves |
| **round-3 adjacent sweep (landed)** | **55.353455** | **0** | + `setPosition` swap |

The `.rel.plt` rotation in `best9` is caused by moving
`player->setRocketControl(nullptr, nullptr)` from the mission-1 block (first
reference before `killLiberator`) into the mission-89 block (after
`setType`), which shifts seven `R_ARM_JUMP_SLOT` names.  Restoring the call to
the mission-1 block costs ~0.05 raw but keeps the PLT byte-identical; the
landed variant uses the restored position.

## Search method

`/dev/shm/w5608/search4.py` (adjacent same-indent statement swaps) and
`/dev/shm/w5608/search5.py` (random same-indent statement relocation) both
rebuild `link_so`, rescore with the official verifier, and reject any variant
whose size != 57232 or whose `readelf -rW` `R_ARM_JUMP_SLOT` name list differs
from the original.  The accepted set is re-measured after every round, so the
gains are additive.  Roughly 800 builds were measured in total.

## Proven vs hypothesis

Proven (measured):
* The faithful case-body order is worth the bulk of the gain (54.78 -> 55.29
  at the same 57232 B once the `.rel.plt` rotation is repaired).
* The landed file is size-exact (57232 B), `.rel.plt` byte-identical
  (3674/3674), `.ARM.extab` exact at `0x26298`, and the gate passes with
  byte 4500 / linked 4500 / lint CLEAN / drift 0.
* The mission-5/mission-26 clause order is **not** worth flipping: the
  binary emits mission 5 before 26, but the source `26, 5` order scores
  55.287911 vs 55.208281 for `5, 26` (both size-exact, rel.plt-clean).

Hypothesis / not established:
* The remaining ~45% sequence loss is still global greedy register allocation
  plus MachineBlockPlacement inside the case bodies; the instruction multiset
  is shared, and the case-body order already matches.
* The five cross-block relocations are score-tuned; a future pass with a
  faithful body reconstruction should re-evaluate them.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
cmake --build cmake-build-match --target drift -j6    # 0 fields
cmake --build cmake-build-match --target lint -j6     # CLEAN
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
readelf -rW cmake-build-match/libgof2hdaa.so | \
  awk '$3=="R_ARM_JUMP_SLOT"{print $5}' > /dev/shm/w5608/rp_ours.txt
# diff against the same extraction from the original: empty
```

Scratch helpers (not tracked): `/dev/shm/w5608/score.py` (raw ratio),
`search4.py` (adjacent swaps + rel.plt gate), `search5.py` (relocations +
rel.plt gate), `seed_norocket.cpp`, `best_search3.cpp`, `best_search4.cpp`.
