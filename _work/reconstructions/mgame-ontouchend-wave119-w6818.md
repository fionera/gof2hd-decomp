# MGame::OnTouchEnd (8080 B) — w6818 (wave 119): mission-select hoist, gate-safe win

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Branch `wave119-w6818`, base master
`3981b32c5`.  This pass lands one source change.

## Result

* Before (HEAD): **8080 B / 80.1441 %**, `.rel.plt` OK, `RATCHET PASS`
  (byte_exact 4505, linked_exact 4505, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0).
* After: **8080 B / 80.3964 %**, `.rel.plt` OK, `RATCHET PASS`
  (byte_exact **4505**, linked_exact **4505**, same size/data/imports).
* No section, `.ARM.extab` or file-size change: 27 sections,
  file 2,187,328 B, `.text` 0x1337c8 @0x69440, `.ARM.extab`
  0x26294 @0x1a3b78, `.rodata` 0x41c27, `.rel.plt` 0x72d0 — all
  identical to HEAD.

## The change (one line moved)

In the `else if (... hasWon() ...)` won-branch preamble the mission
selection was declared *after* the `keepObjectives` pair.  Hoisting it
above them is behaviour-neutral (neither `campaign` nor `keepObjectives`
is read by the `mission` initialiser, and all three are pure reads):

```cpp
} else if (Globals::status->getMission()->hasWon() == true ||
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
    Mission *mission = Globals::status->getMission()->hasWon() == true ? Globals::status->getMission() : (Mission *) (intptr_t) Globals::status->getCampaignMission();
    Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
    bool keepObjectives = campaign->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
```

The instruction stream is unchanged in size (8080 B) and the
`resetStartSequenceOver < removeObjectives` first-reference order is
preserved (reset 0x17b160, first `removeObjectives` 0x17b2ee), so
`byte_exact` stays 4505.  The fuzzy score rises because the
mission-select block is emitted earlier:

```
HEAD         won-check 0x17adc4 -> keepObjectives 0x17adde -> ... -> mission select 0x17b022
missionfirst won-check 0x17adc4 -> mission select 0x17adde -> ... -> keepObjectives 0x17b012
original     won-check 0x17adc8 -> keepObjectives 0x17ade0 -> mission select 0x17af5e
```

The stripped (branch-target-normalised) ratio actually *falls*
(87.4595 -> 87.2432) even though the official raw ratio rises
(80.1441 -> 80.3964); the official metric is the raw ratio.

## Why the source order moves the block (LLVM 7 MBP)

`MachineBlockPlacement::buildCFGChains()` builds the whole function
chain greedily from the entry block.  At the `keepObjectives` diamond
(`bb.160` -> `bb.161` false arm / `bb.162` true arm -> `bb.163`
mission-select merge) the edge `bb.162 -> bb.163` is rejected by
`hasBetterLayoutPredecessor()`: `StaticLikelyProb` is 80, so the
backward check `PredEdgeFreq * 0.8 >= CandidateEdgeFreq * 0.2` fires
for the 37.5 % false arm, and `bb.163` is *not* merged with the true
arm.  It is instead placed later by `selectBestCandidateBlock()` (the
highest-frequency ready block).  Which of the two 37.5 %/62.5 % arms
is the "true" arm in the source decides where that ready block lands,
hence the layout change from a pure declaration reorder.

## Measurement table (all on the HEAD base; NDK r18b link + verify)

`OK` = `.rel.plt` first-reference order matches the original.

| variant | size | pct | plt |
|---|---:|---:|---|
| HEAD | 8080 | 80.1441 | OK |
| **mission hoisted before campaign/keepObjectives** | **8080** | **80.3964** | **OK** |
| + `campaign` inlined into `keepObjectives` | 8080 | 80.3964 | OK |
| + `keepObjectives` as `&&` | 8080 | 80.3964 | OK |
| + `keepObjectives` as if/else (3 spellings) | 8080 | 80.3964 | OK |
| + `mission` as if/else / simple `if(!hasWon())` | 8080 | 80.3964 | OK |
| + `isCampaignMission()` hoisted to `bool` | 8080 | 80.3964 | OK |
| + bare top `hasWon()` (D1) | 8084 | 80.9155 | DIFF |
| + bare mission-select `hasWon()` (D2) | 8080 | 80.3964 | OK |
| + `isInstantActionMission() == true` (X) | 8084 | 82.5225 | DIFF |
| + `isInstantActionMission() == 1` | 8084 | 82.5225 | DIFF |
| + `bool refinedAny` (B) | 8088 | 77.8098 | OK |
| + `count != 0` (C) | 8084 | 74.6215 | OK |
| + `m_nTimeLimit = 0` before `changeCredits` (FT) | 8080 | 80.2883 | OK |
| + `goingToWaypoint() == true` | 8084 | 79.1787 | OK |
| + split 96/127 (M127) | 8096 | 79.8631 | OK |
| + `campaign->hasWon() == true` (K) | 8080 | 80.3604 | OK |
| + `menuTouchOpen == true` (MT) | 8072 | 80.1009 | DIFF |
| D1b+D2+K+MT+DJ | 8080 | 81.4841 | DIFF |
| D1+X+K | 8088 | 82.7387 | DIFF |
| X+`supernovaChallenge` | 8084 | 82.5225 | DIFF |
| `keepObjectives` moved to `finishMission` | 8064 | 79.0110 | OK |
| `keepObjectives` moved after dispatch | 8104 | 77.4739 | OK |
| `mission` hoisted above `bool failed` | 8088 | 77.0394 | OK |

Also re-measured on the new base: every single op of the wave-119 pool
(37 ops: D1/D1b/D1d, D2/D2b/D2d, X/MI/MI2/MI3/MI4, B, C, GO, IC/IC0,
FT, K/K2/KAND, NC, W2, F1/F2, M127/M127b, SM, CS, MT, FC, EX, TS, DJ,
HUD0, AP, SUPER) — **none exceeds 80.3964 with 8080 B + PLT OK**; most
are code-identical to the hoisted shape.  A 60-combo random search over
the same pool found no better gate-safe result.

## The residual gap (unchanged by this pass)

The remaining ~20 % is still block placement:

* the original keeps `keepObjectives` at 0x17ade0 and the mission
  select at 0x17af5e (both early); we can match one or the other, not
  both, because of the MBP diamond above;
* the original's campaign chain splits 96/127 (`M127`, 8096 B) and
  orders 96,100,110,120,126,127,134; ours merges 96/127 and emits 127
  before 100;
* the `refinedAny` test is `lsls #31` (bit 0) in the original vs
  `lsls #24` (byte) in ours — `bool refinedAny` fixes it but is 8088 B;
* `count > 0` (`cmp #-1; bgt`) vs the original `cmp #0; beq`;
  `count != 0` fixes the encoding but is 8084 B / 74.6 %.

Every variant that raises the fuzzy score above 80.3964 (D1, X, D1b…)
is 8084/8088 B and/or rotates `.rel.plt` (reset lands after the first
`removeObjectives`), which fails the ratchet by one `byte_exact`.

## Gate

`cmake --build cmake-build-match --target check -j6` on the final tree:
`verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`,
`data: exact 843 of 843`, `sodiff: extra-beyond-allow 0 missing 0
allowed 0 imports 207`, `RATCHET PASS`.
