# MGame::OnTouchEnd — wave 123 (w7308): `won` local + inverted guard, 81.7 -> 82.4%

Target `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`), 8080 B.
Base `1c8c9b20e` (gate byte 4506 / linked 4506).

## Change

Hoist the won-test into a named local and invert the failed/won guard so the
`resetStartSequenceOver()` block is emitted *before* the campaign-mission
dispatch chain:

```diff
                 bool failed = Globals::status->getMission()->hasFailed();
-                if (failed) {
+                bool won = Globals::status->getMission()->hasWon() == true ||
+                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon();
+                if (!failed && !won) {
+                    this->levelScript->resetStartSequenceOver();
+                } else if (failed) {
                     ... failed body ...
-                } else if (Globals::status->getMission()->hasWon() == true ||
-                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
+                } else {
                     ... won body ...
-                } else {
-                    this->levelScript->resetStartSequenceOver();
                 }
```

The named `won` local is what changes codegen. The semantically equivalent
`else if (!(A || B))`, `else if (!A && !B)`, and `if (!failed) { if (!won) ... }`
spellings are canonicalised by clang back to the old block order (measured:
8080/81.7/PLT 3 for `!(A||B)`, 8084/82.3/PLT 7 for De Morgan, 8088/72.9/PLT 5
for the nested form).

## Evidence

- before: **81.7 %**, 8080 B, `.rel.plt` 6 differing bytes (slots 3329-3331:
  ours `resetAnalogStick, inCinematicMode, setCinematicMode` vs original
  `inCinematicMode, setCinematicMode, resetAnalogStick`).
- after: **82.4 %**, 8080 B, `.rel.plt` **4** differing bytes (slots 3323-3324:
  ours `resetStartSequenceOver, isInstantActionMission` vs original
  `isInstantActionMission, resetStartSequenceOver`). The reset block now
  precedes the `isInstantActionMission()` call, so the PLT first-reference
  order moves toward the original and the `.rel.plt` delta shrinks 6 -> 4.
- gate: byte_exact **4506/4524** (unchanged), linked_exact **4506/4524**,
  size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
  symbol parity 5370/5370, avg 99.94 %. `RATCHET PASS`.
- section bytes: `.text` 86,947 differing (unchanged), `.ARM.exidx` 1,327
  (unchanged), `.rodata` 0, `.bss` 1.

## Why the remaining `== true` removals are not landable yet

The original uses bare `hasWon()` (`cbnz` at 0x17adcc), bare
`goingToWaypoint()` (`cbz` at 0x17b8c4) and bare `campaign->hasWon()`
(`cmp #0; beq` at 0x17adf0). Removing `hasWon() == true` (TB) alone gives
82.3 % but grows the body to 8084 B and rotates `.rel.plt` 3324-3327
(`resetStartSequenceOver` moves after `removeObjectives`), which breaks the
byte-exact `MGame::successCheck` (it calls `removeObjectives`,
`goingToWaypoint`, `removeRoute`). Every 8080 B TB variant found (including
`reuse campaign` ckm, 83.7 %) keeps the 7-entry PLT rotation. The blocker is
the cold `resetStartSequenceOver` block: the original places it right after
the failed `type == 12` block (0x17b0aa, before the campaign chain), while our
TB builds place it after the mission-74 block (0x17b436).

Sweeps run this wave: single-lever sweep of 41 predicate/type spellings on the
HEAD base and on TB and on TB+MO; 2-lever sweep of 29x28 combinations on TB;
structural rewrites (`else if (!won)`, `if (!failed && !won)`, De Morgan,
nested `if (!failed)`, `bool won` local); `char/int/bool refinedAny`;
`count != 0`/`count`; `long count/amount`; tail duplication. Best 8080 B
results: `mo_bare + cs_true` 83.0 % but PLT 7; `bool won` + inverted guard
(this change) 82.4 % / PLT 2. `GO1 + MO` reaches PLT 0 but scores 80.4 %.
