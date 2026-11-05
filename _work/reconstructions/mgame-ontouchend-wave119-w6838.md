# MGame::OnTouchEnd (8080 B) — w6838 (wave 119): predicate-probability tuning, gate-safe win

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Branch `wave119-w6838`, base master
`fdb68f8ee` (wave-119 notes head; OnTouchEnd source = w6818 hoist).
This pass lands one source change set.

## Result

* Before (HEAD): **8080 B / 80.3964 %**, `RATCHET PASS`
  (byte_exact 4505, linked_exact 4505, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0).
* After: **8080 B / 81.2827 %**, `RATCHET PASS`
  (byte_exact **4505**, linked_exact **4505**, same size/data/imports).
* No section, `.ARM.extab` or file-size change: 27 sections,
  file 2,187,328 B, `.text` 0x1337c8 @0x69440, `.ARM.extab`
  0x26290 @0x1a3b78, `.rodata` 0x41c27, `.rel.plt` 0x72d0 — all
  identical to HEAD.

## The change (five predicate spellings, all behaviour-neutral)

```cpp
if (this->starMap->exitRequested == true) { ... }          // was: if (this->starMap->exitRequested)
if (this->player->goingToWaypoint() == true) { ... }       // was: if (this->player->goingToWaypoint())   (both sites)
Mission *mission = Globals::status->getMission()->hasWon() ? ...   // was: hasWon() == true ?
bool keepObjectives = campaign->hasWon() != false ? ...    // was: campaign->hasWon() ?
if (this->menuTouchOpen == true) { ... }                   // was: if (this->menuTouchOpen)
```

All five are pure boolean reads (`bool` returns / 0-1 flags), so the
predicate rewrite is behaviour-neutral.  The point is not the compare
itself: `== true` / `!= false` makes LLVM 7's branch-probability
analysis treat the branch as 50/50 instead of the `isZero`
37.5/62.5 heuristic, which changes `MachineBlockPlacement`'s
`selectBestCandidateBlock()` choices and therefore the cold-block
layout of the whole 8080-byte function.  The instruction stream stays
the same size (8080 B), so `.rel.plt`, `.text` offsets and every other
function's bytes are untouched.

`.rel.plt` first-reference order is preserved:
`resetStartSequenceOver` 0x17b160 < first `removeObjectives` 0x17b2ee,
so `MGame::successCheck` stays byte-exact and `byte_exact` stays 4505.

## Measurement table (all on the HEAD base; NDK r18b link + verify)

`OK` = `.rel.plt` first-reference order matches the original (reset
before removeObjectives); `ROT` = rotated, which drops `byte_exact`
4505 -> 4504 and fails the ratchet.

| variant | size | pct | plt |
|---|---:|---:|---|
| HEAD (w6818 hoist) | 8080 | 80.3964 | OK |
| D2 (`mission` ternary bare) | 8080 | 80.3964 | OK |
| P (keepObjectives before mission) | 8080 | 80.1441 | OK |
| D1 (top `hasWon()` bare) | 8080 | 81.4481 | ROT |
| D1+EX (top bare + exitRequested==true) | 8080 | 81.5349 | ROT |
| D1 on best2 base | 8080 | 81.5709 | ROT |
| EX alone (`exitRequested == true`) | 8084 | 80.1369 | OK |
| X (`isInstantActionMission() == true`) | 8084 | 82.5225 | ROT |
| best1 = D2 + K + MT + GO | 8080 | 81.2320 | OK |
| **best2 = best1 + EX + K2** | **8080** | **81.2827** | **OK** |
| best2 + FT | 8080 | 81.1746 | OK |
| best2 + SUPER | 8080 | 80.3819 | OK |
| best2 + POPEN / K3 / D2b / HUD0 | 8080 | 81.2827 | OK (code-identical) |
| faithful mining (`amount++`, `amount=-amount`, `!=0`) | 8088 | 79.9 | OK |
| `count != 0` | 8084 | 74.6 | OK |
| `bool refinedAny` | 8088 | 77.8 | OK |
| instant-first dispatch (E) | 8072 | 73.3 | OK |
| split 96/127 (M127) | 8096 | 79.86 | OK |

Ops: D2 = bare mission-select `hasWon()`; K = `campaign->hasWon() == true`;
K2 = `campaign->hasWon() != false`; MT = `menuTouchOpen == true`;
GO = `goingToWaypoint() == true`; EX = `starMap->exitRequested == true`;
POPEN = `pauseOpen == true`; HUD0 = `hudTouchFlags == 0x0`.

A 19-op single sweep and a 171-pair sweep on the best2 base found no
8080 B / PLT-OK variant above 81.2827 %.  Every variant that scores
higher (D1 81.4481, D1+EX 81.5349, D1-on-best2 81.5709, X 82.5225)
rotates `.rel.plt`: the D1/X probability change moves the cold
`resetStartSequenceOver` block after the first `removeObjectives`, so
`MGame::successCheck`'s `blx` offsets change and `byte_exact` drops to
4504.  The reset block could not be pulled back with `else if (!won)`,
a standalone `if`, or any of the 19 single ops (all still ROT).

## Why the faithful mining block is not landed

The original mining loop emits `cmp r9,#0; beq` (i.e. `amount != 0`),
but the current source's `int count = -amount; if (count > 0)` emits
`cmp r9,#-1; bgt`.  The faithful spelling
(`amount++; amount = -amount; if (amount != 0)`) does match the mining
region instruction-for-instruction (verified at 0x17a5b6..0x17a5da)
but the compiler then picks a different global register allocation /
block layout: 8088 B / 79.9 %.  `count != 0` is 8084 B / 74.6 %
(register swap of `this`), `bool refinedAny` is 8088 B / 77.8 %.
None keeps 8080 B, so they cannot be landed (a size change shifts
`.text` and collapses `byte_exact` to ~3100).

## Gate

`cmake --build cmake-build-match --target check -j6` on the final tree:
`verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`,
`data: exact 843 of 843`, `sodiff: extra-beyond-allow 0 missing 0
allowed 0 imports 207`, `RATCHET PASS`.
