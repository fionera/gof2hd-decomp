# MGame::OnTouchEnd — wave 163 (w10910): epilogue shift vs `.rel.plt`

Status: one source change landed, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10910`, base `e51db2fc7` (wave-162 final).
Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch `/dev/shm/w10910`.
Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`,
shared return epilogue block @ `0x17b004` (`popeq` @ `0x17b014`).

The wave-162 w10810 change (23rd `isCampaignMission` call: the 96/127 split
with the faithful chain order) is part of this commit; the base tree did not
contain it yet.

## Result

| | wave-162 master | w10810 (23rd call) | **landed** |
|---|---:|---:|---:|
| size | 8080 | 8080 | **8080** (exact) |
| raw same-address diff | 4127 | 5816 | **4575** (-1241) |
| aligned (`align_diff.py`) | 906 | 1379 | **1373** (-6) |
| `isCampaignMission` calls | 22 | 23 | **23** (== original) |
| shared epilogue (`popeq`) | `0x17af92` | `0x17af82` | **`0x17b74a`** |
| `.rel.plt` differing bytes | 0 | 0 | **0** (byte-identical) |
| `.text` differing bytes | 85,042 | 87,063 | **85,490** |
| `.ARM.extab` differing bytes | 725 | 735 | **735** |
| byte_exact | 4507/4524 | 4507/4524 | **4507/4524** |
| linked_exact | 4507/4524 | 4507/4524 | **4507/4524** |

`check` green: byte 4507, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.

## Landed source shape

Two edits on top of the w10810 split:

```cpp
// line ~1555 (won arm): drop the redundant `== true`
bool won = Globals::status->getMission()->hasWon() ||
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon();
```

```cpp
// line ~2072 (hudTouchFlags tail): cold-mark the initHudMenu(2) arm
} else if (__builtin_expect(this->hudTouchFlags & 0x400, 0)) { // lint: arm_builtin
    this->hud->initHudMenu(2, this->level);
```

Either half alone is not size-exact (`truthy won` alone 8096 B, `expect(0x400,0)`
alone 8076 B); the pair is 8080 B. `expect(failed,0)` + truthy `won` is an
exact codegen twin of this pair (same 4575 / 1373 / `0x17b74a` / relplt 0).

## The epilogue target is reachable, but it rotates `.rel.plt`

`if (__builtin_expect(failed, 0))` (variant C) lands the shared epilogue
**exactly** at the original `0x17b004` (popeq `0x17b014`) and the whole tail
`0x17afe2..0x17b04e` becomes byte-identical:

| variant | raw | aligned | size | epi | relplt |
|---|---:|---:|---:|---|---:|
| **C `expect(failed,0)`** | 5147 | 1196 | 8080 | **0x17b014** | **9** |
| C + `expect(goingToWaypoint,0)` (failed arm) | 5133 | 1205 | 8080 | 0x17b014 | 9 |
| C + `expect(cargo!=nullptr,0)` | 5143 | 1205 | 8080 | 0x17b014 | 9 |
| C + `expect(!keepObjectives,1)` | 5192 | 1337 | 8080 | 0x17b014 | 9 |
| C + swap resetAnalogStick/releaseAllKeys | 5150 | 1199 | 8080 | 0x17b014 | 9 |
| C + `goto` tail moved to function end | 5147 | 1196 | 8080 | 0x17b014 | 9 |
| C + `expect(starMapOpen,1)` | 5150 | 1196 | 8080 | 0x17b014 | 16 |
| C + `expect(starMapOpen,0)` | 5306 | 1211 | 8080 | 0x17b034 | 13 |

Variant C fails the ratchet: `byte_exact 4507 -> 4506`
(`RATCHET FAIL`). The single flipped function is **`MGame::successCheck`**
(linked-exact 100%, raw differs only in the `blx removeObjectives` PLT
address). The rotation is

```
orig: removeObjectives, goingToWaypoint, removeRoute, skipCutscene,
      inCinematicMode, setCinematicMode, resetAnalogStick
ours: resetAnalogStick, removeObjectives, goingToWaypoint, removeRoute,
      skipCutscene, inCinematicMode, setCinematicMode
```

plus `maneuverTouchEnd` <-> `isInstantActionMission` swapped at slots
3322/3323. Cause: variant C emits the cold cutscene-return block
(`resetStartSequenceOver; frameTime=0; resetAnalogStick; releaseAllKeys`)
at `0x17af88`, i.e. **before** `removeObjectives`/`setCinematicMode`, so
`resetAnalogStick`'s first relocation is allocated one slot too early and
the seven-slot run rotates. In the original that block sits at `0x17b8e2`
(shared fall-through of the `!keepObjectives` finishMission path, after
`setCinematicMode` @ `0x17b820`), so the order is correct.

Every 8080 B shape that puts the epilogue at `0x17b014` measured relplt 9 or
worse; no shape with epilogue `0x17b014` **and** relplt 0 was found.

## Why the landed shape wins on the primary metric

The landed pair re-lays the middle region differently: the epilogue moves to
`0x17b74a` (past the original), but the overall instruction stream aligns
much better (raw 4575 vs 5147; the early `0x17a144..0x17ae0e` region matches
byte-for-byte in both). Raw is the ground truth for byte-identicality, so the
landed shape is kept.

## Open question for the next wave

Find a source shape that (a) keeps the 23rd `isCampaignMission` call, (b) is
8080 B, (c) puts the shared epilogue at `0x17b014`, and (d) leaves
`resetAnalogStick`'s first relocation after `setCinematicMode`'s. The lever is
the placement of the cold cutscene-return block: it must be emitted after the
`menuTouchOpen` block (original `0x17b8e2`) instead of before the epilogue
(variant C `0x17af88`). `expect` cells on `cutsceneActive`/`starMapOpen`/
`pauseOpen` change the size off 8080; `goto`-moving the tail is a no-op for
codegen.

## Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10910/fdiff.py _ZN5MGame10OnTouchEndEiiPv       # 4575 / 8080
python3 /dev/shm/w10910/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 1373
python3 /dev/shm/w10910/measure3.py                               # epi 0x17b74a, relplt 0
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Note: the checked-in `_work/reconstructions/align_diff.py` still points at
`/home/debian/GalaxyOnFire2/gof2-decomp`; the scratch copy is patched to this
worktree. Scratch: `/dev/shm/w10910/{fdiff.py,align_diff.py,measure3.py,
secdiff.py,sweep163*.py,MGame.base.cpp,MGame.landed.cpp}`.
