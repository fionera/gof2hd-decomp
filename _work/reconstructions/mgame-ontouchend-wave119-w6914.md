# MGame::OnTouchEnd (8080 B) — w6914 (wave 119): three branch-probability spellings, gate-safe win

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Branch `wave119-w6914`, base master
`0bbd27a1a`.  This pass lands three behaviour-neutral condition
spellings in one commit.

## Result

* Before (HEAD): **8080 B / 80.3964 %**, `RATCHET PASS`
  (byte_exact 4505, linked_exact 4505, size 4524/4524, data 843/843,
  imports 207, lint CLEAN, drift 0).
* After: **8080 B / 81.2320 %**, `RATCHET PASS`
  (byte_exact **4505**, linked_exact **4505**, same size/data/imports,
  drift 0).
* No section change vs HEAD: 27 sections, file 2,187,328 B,
  `.text` 0x1337c8 @0x69440, `.ARM.extab` 0x26290 @0x1a3b78,
  `.rodata` 0x41c27, `.rel.plt` 0x72d0.  `successCheck` stays at
  0x17ff58 (PLT order unchanged), so it stays byte-exact.

## The change (three `== true` spellings)

```cpp
// failed branch, finishMission (both copies of the waypoint test)
- if (this->player->goingToWaypoint()) {
+ if (this->player->goingToWaypoint() == true) {

// won-branch preamble
- bool keepObjectives = campaign->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
+ bool keepObjectives = campaign->hasWon() == true ? !Globals::status->getMission()->isCampaignMission() : false;

// menu-touch block (OnTouchEnd's own copy only, not OnTouchBegin's)
- if (this->menuTouchOpen) {
+ if (this->menuTouchOpen == true) {
```

All three are pure reads/booleans; the `== true` form changes only
clang's branch-probability heuristic (`isNonZero` 50/50 vs `isZero`
37.5/62.5) and therefore MBP block placement.  Instruction count,
call sequence and size are unchanged (8080 B).

The `menuTouchOpen` edit is deliberately scoped to OnTouchEnd's block
(`if (this->menuTouchOpen) {` followed by `if (this->freeCamMode) {`);
the other `menuTouchOpen` sites in `OnTouchBegin` are untouched.

## Measurement table (all on the HEAD base; NDK r18b link + verify)

`OK` = `.rel.plt` first-reference order matches the original
(reset 3324 < removeObjectives 3325 < goingToWaypoint 3326 <
removeRoute 3327).

| variant | size | pct | plt |
|---|---:|---:|---|
| HEAD | 8080 | 80.3964 | OK |
| **GOTO + KEEP + MTt** | **8080** | **81.2320** | **OK** |
| GOTO alone | 8084 | 79.1427 | OK |
| KEEP alone | 8080 | 80.3604 | OK |
| MTt alone | 8072 | 80.1009 | OK |
| MTt + KEEP | 8072 | 80.0649 | OK |
| MTt + GOTO | 8076 | 80.0000 | OK |
| KEEP + GOTO | 8088 | 79.2582 | OK |
| PAt + KEEP + D2 | 8080 | 80.3604 | OK |
| PAt + D2 + CNT | 8080 | 80.3964 | OK |

(`MTt` = targeted `menuTouchOpen == true`; `PAt` = targeted
`pauseOpen == true`; `KEEP` = `campaign->hasWon() == true`;
`GOTO` = `goingToWaypoint() == true`; `CNT` = `count >= 1`.)

## The 84.3 % false lead (still not gate-safe)

The highest fuzzy shape found is still the wave-119 pool's
`isInstantActionMission() == true` (X) family:

| variant | size | pct | plt |
|---|---:|---:|---|
| X + AP | 8080 | 83.7419 | ROT |
| X + AP + D1 | 8080 | 83.9373 | ROT |
| X + AP + D1 + D2 | 8080 | 83.9524 | ROT |
| X + AP + `nomission` (reuse `campaign` in ternary) | 8080 | 84.1175 | ROT |
| X + AP + `nomission` + D1 | 8080 | 84.3130 | ROT |

(`AP` = `!wasAutoPilot && isAutoPilot()` reorder; `D1` = bare top
`hasWon()`; `D2` = bare mission-select `hasWon()`.)

Every one rotates `.rel.plt` (reset becomes index 3327, remove/
going/removeRoute shift to 3324/3325/3326), which makes
`MGame::successCheck` non-byte-exact (its `blx` offsets to those
three stubs move by -12 B).  `byte_exact` drops 4505 -> 4504 and the
ratchet FAILs.

### Why a size compensation does not rescue it

A 12-byte smaller OnTouchEnd (8068 B, e.g. `nomission + SMt + D1`)
does move `successCheck` to 0x17ff4c, exactly compensating the -12 B
PLT shift for the three rotated stubs.  But the other PLT stubs
called by `successCheck` did **not** move, so their `blx` offsets now
differ by +12 B: `successCheck` still has 124 differing bytes.  The
compensation only works if the whole PLT rotates, which it does not.
The 8068-B variants also score 76-77 % (worse than HEAD).

## Why this is the gate-safe ceiling found

The PLT constraint forces the top `hasWon()` test to stay
`== true` (bare `hasWon()` is +4 B and ROT) and the instant test to
stay bare (`== true` is +4 B and ROT).  Within that constraint the
only lever is the branch-probability heuristic of the *other*
boolean tests.  A 3-deep exhaustive sweep over the safe spelling pool
(`failed`, `hudTouchFlags`, `refinedAny`, `count`, `menuTouchOpen`,
`starMapOpen`, `cutsceneActive`, `pauseOpen`, `freeCamMode`,
`autopilotMenuOpen`, `_bca`, `field_0xc4`, `choiceWindowOpen`,
`jumpActive`, `supernovaChallenge`, `goingToWaypoint`,
`isCampaignMission`, `keepObjectives`, `starMap->exitRequested`,
`touchesStream`) found `GOTO + KEEP + MTt` as the only 8080 B /
PLT-OK shape above 80.3964.  All other 8080/PLT-OK combinations are
code-identical to HEAD or lower.

## Residual gap

The remaining ~19 % is still block placement: the original emits the
mission-select block at 0x17af5e (before the hud-menu epilogue at
0x17afd8), while ours emits the epilogue at 0x17af54 and the
mission-select/dispatch at 0x17b00c/0x17b046.  The original also
places the failed-branch cargo loop at 0x17b374 (after the campaign
chain and `finishMission`), ours at 0x17b0e4 (before it).  Both are
MBP decisions that no behaviour-neutral spelling in this pool moves
without rotating the PLT.
