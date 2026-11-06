# MGame::OnTouchEnd — wave 124 (w7415): `.rel.plt` byte-identical (82.4 -> 78.8 %)

Target `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`), 8080 B.
Base `6f8ee2320` (wave-123 final, gate byte 4506 / linked 4506).

## Result

| | before (HEAD) | after |
|---|---:|---:|
| fuzzy | 82.4 % | **78.8 %** |
| size | 8080 B | **8080 B** |
| `.rel.plt` differing bytes | 4 (2 swapped entries) | **0** |
| `.text` differing bytes | 87,014 | **86,164** |
| `.ARM.extab` differing bytes | 1,446 | 1,480 |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.note.gnu.build-id` | 20 | 20 |

The fuzzy score drops, but the raw `.text` delta improves by 850 bytes and `.rel.plt`
becomes byte-identical. Full `check`: byte_exact **4506/4524**, linked_exact
**4506/4524**, size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN,
drift 0, symbol parity 5370/5370, avg **99.93 %**, `RATCHET PASS`.

## Change

`MGame::OnTouchEnd`, cutsceneActive mission-completion block:

```diff
-                bool failed = Globals::status->getMission()->hasFailed();
-                bool won = Globals::status->getMission()->hasWon() == true ||
-                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon();
-                if (!failed && !won) {
-                    this->levelScript->resetStartSequenceOver();
-                } else if (failed) {
+                bool failed = Globals::status->getMission()->hasFailed() == true;
+                if (failed) {
                     ... failed body ...
+                    if (this->player->goingToWaypoint()) {   // was `== true`
+                        this->player->setAutoPilot(nullptr);
+                    }
                 } else {
-                    Mission *mission = ...hasWon() ? ... : ...;
-                    Mission *campaign = ...;
-                    bool keepObjectives = campaign->hasWon() != false ? ... : false;
-                    if (mission->isCampaignMission()) { ... }
-                    else if (mission->isInstantActionMission()) { ... return; }
-                    else { ... }
+                    bool won = Globals::status->getMission()->hasWon() ||
+                               ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() == true;
+                    if (won) {
+                        bool keepObjectives = ...;   // keepObjectives first
+                        Mission *mission = ...;      // then mission
+                        if (mission->isInstantActionMission()) { ... return; }   // instant first
+                        else if (mission->isCampaignMission()) { ... }          // campaign second
+                        else { ... }
+                        ... common body ...
+                    } else {
+                        this->levelScript->resetStartSequenceOver();
+                    }
                 }
```

Three independent levers, all measured:

1. **`hasFailed() == true`** changes the `if (failed)` branch probability from the
   zero-heuristic 37.5/62.5 to 50/50 (`icmp eq i8 %x, 1` instead of `icmp eq i8 %x, 0`).
   This is what moves the cold `resetStartSequenceOver` block. With a bare
   `hasFailed()` the reset block lands before `isInstantActionMission`; with
   `== true` it lands after it.
2. **failed-first / `won`-local structure** (`if (failed) ... else { bool won = ...;
   if (won) ... else reset; }`). The original tests `failed` first
   (`0x17ab20..0x17ab38`), then `won` (`0x17adc8..0x17addc`), then resets
   (`0x17b0aa`). The `won` local is kept (declared inside the `else`).
3. **`goingToWaypoint()` bare** in the failed body — the original emits `cbz`
   (`0x17b8c4`); the `== true` spelling emitted `cmp #1; bne`. This is a 19-byte
   `.text` improvement and size-neutral in this layout.

## Evidence

Original first-reference order (from `readelf -r` / disassembly):

```
0x17af7e  Mission::isInstantActionMission
0x17b0ae  LevelScript::resetStartSequenceOver
0x17b0be  Status::setFreelanceMission
0x17b0de  Layout::showMissionRewardMessage
0x17b270  Level::removeObjectives
0x17b292  PlayerEgo::goingToWaypoint
0x17b2a6  PlayerEgo::removeRoute
0x17b816  MenuTouchWindow::inCinematicMode
0x17b820  MGame::setCinematicMode
0x17b8ec  Hud::resetAnalogStick
```

After the change our order is identical and `.rel.plt` slots 3323-3331 are byte-identical:

```
3323 isInstantActionMission
3324 resetStartSequenceOver
3325 removeObjectives
3326 goingToWaypoint
3327 removeRoute
3329 inCinematicMode
3330 setCinematicMode
3331 resetAnalogStick
```

The original's won-body head is `keepObjectives` (`0x17ade0`), then `mission`
(`0x17af5e`), then `isInstantActionMission` (`0x17af7e`), then `isCampaignMission`
(`0x17afa6`); the after source reproduces exactly that order (`won_b` body). The
reset block is emitted at `0x17b052` (original `0x17b0aa`), after `isInstantActionMission`
(`0x17afa4`) and before `setFreelanceMission` (`0x17b098`).

## Variants measured (size / pct / PLT diffs / `.text` / `.ARM.extab`)

| variant | size | pct | PLT | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| HEAD (inverted guard, `won` local) | 8080 | 82.4 | 2 | 87,014 | 1,446 |
| failed-first + `won_b` + bare `failed` | 8080 | 76.0 | 3 | 87,221 | 1,450 |
| failed-first + `won_b` + `failed == true` | 8080 | 78.8 | 0 | 86,186 | 1,480 |
| **+ `goingToWaypoint()` bare** | **8080** | **78.8** | **0** | **86,164** | **1,480** |
| inverted guard + `failed == true` | 8088 | 70.0 | 8 | — | — |
| failed-first + `won_a` + `failed == true` | 8088 | 74.7 | 0 | — | — |
| failed-first + `won_c` + `failed == true` | 8088 | 74.6 | 0 | — | — |
| failed-first + `won_d` + `failed == true` | 8080 | 78.2 | 0 | 86,170 | 1,480 |
| failed-first + `won_i` + `failed == true` | 8080 | 78.2 | 0 | 86,170 | 1,480 |
| failed-first + `won_b` + `failed == true` + `menuWindow == true` bare | 8084 | 77.4 | 0 | — | — |
| failed-first + `won_b` + `failed == true` + `split127` | 8068 | 78.0 | 0 | — | — |
| `won = A \|\| B` (both bare) + `failed == true` | 8068 | 76.7 | 0 | — | — |
| `won = A \|\| B == true` + `failed == true` | 8080 | 78.8 | 0 | 86,164 | 1,480 |
| `won = A == true \|\| B` + `failed == true` | 8080 | 78.8 | 0 | 86,167 | 1,480 |
| `__builtin_expect(hasFailed(), 0/1)` | 8072/7816 | 71.9/56.0 | 9/10 | — | — |

`won_a` = mission-first / campaign temp / isCampaign-first; `won_b` = keepObjectives-first /
mission / isInstant-first / isCampaign-second; `won_c` = keepObjectives-first / mission /
isCampaign-first; `won_d` = mission-first / campaign temp / keepObjectives / isInstant-first;
`won_i` = declarations-first variant of `won_a`.

## Why the fuzzy score drops

The after layout matches the original's block *order* but not its block *addresses*.
Two measured causes:

1. **`Hud::initHudMenu` tail-merge is lost.** The original has three `blx initHudMenu`
   sites (`0x17ad24` arg 0, `0x17afde` shared by arg 1 and arg 2, `0x17bfd8`): the
   `initHudMenu(1)` block is `ldrd r0,r2,[fp,#116]; movs r1,#1; b 0x17afde`, i.e. it
   branches into the `initHudMenu(2)` call site. HEAD preserves that merge (sites
   `0x17ad42`, `0x17af5e`, `0x17bfd8`). The after build does **not**: it emits a fourth
   instruction at `0x17adcc` (`ldrd; movs r1,#1; blx; b.w 0x17b93a`), so the whole won
   body starts 4 B late (`hasWon` at `0x17add4` vs `0x17adc8`) and the downstream
   MachineBlockPlacement decisions change.
2. **The shared return block lands elsewhere.** The stack-canary epilogue is at
   `0x17b004` in the original and `0x17af84` in HEAD, but at `0x17b95c` after the change
   — i.e. inside the campaign-chain region instead of before it. Every intra-function
   branch to the return block therefore differs (`b.w 0xec0` vs `b.w 0x1818` at
   `0x17a2b2`, the first structural diff), which is what difflib penalises.

The failed-body head is nevertheless *better*: the `getCurrentCampaignMission() == 42`
test sits at `0x17ab4a`, exactly the original's address (HEAD had it at `0x17ab68`).
The raw `.text` delta is 850 B smaller because the instruction stream and the
literal-pool contents line up better. The ratchet does not gate the fuzzy score; the
section deltas improve.

### Additional variants measured this pass

| variant | size | pct | PLT | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| hoist `won` + `else if (won)` | 8104 | 70.5 | 4 | — | — |
| hoist `won` before `if (failed)`, nested `if (won)` | 8104 | 70.5 | 4 | — | — |
| `failed != false` | 8076 | 74.7 | 6 | 106,831 | 1,480 |
| `true == hasFailed()` | 8080 | 78.8 | 0 | 86,164 | 1,480 |
| `if (hasFailed() == true)` inline (no `failed` local) | 8080 | 78.8 | 0 | 86,164 | 1,480 |
| `if (failed == true)` | 8076 | 74.7 | 6 | 106,831 | 1,480 |
| `else if (!won)` reset | 8080 | 78.8 | 0 | 86,164 | 1,480 |
| `if (won == true)` | 8080 | 78.8 | 0 | 86,164 | 1,480 |
| `won`-first (`if (!failed) { won... } else failed`) | 8080 | 78.8 | 0 | 86,164 | 1,480 |
| `if (!won) reset; else won` | 8080 | 78.8 | 0 | 86,164 | 1,480 |
| `if (!failed && won) ... else if (failed) ... else reset` | 7916 | 54.7 | 19 | — | — |
| cached `Mission *campaign` in `won` | 8088 | 79.2 | 0 | — | — |

All 8080 B / PLT-0 variants above compile to the identical instruction stream; clang
canonicalises the `if`/`else` spellings.

## Files

- `src/game/menu/MGame.cpp` — `MGame::OnTouchEnd` only.
- `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-124 numbers.
- `report.json` — re-synced by the `verify` target (avg 99.94 -> 99.93, OnTouchEnd 82.4 -> 78.8).
