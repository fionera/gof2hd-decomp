# MGame::OnTouchEnd — wave 119 (w6977)

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, starts at line 1295).
Size-exact at **8080 B**. Baseline score **81.6429 %** (report rounds to 81.6 %),
`byte_exact` 4505 / `linked_exact` 4505, lint CLEAN, drift 0.

**Result of this pass: no landable improvement.** The best measured variant is
**83.4444 % at 8080 B**, but it rotates the PLT order of
`LevelScript::resetStartSequenceOver` and `Level::removeObjectives`, which costs
`MGame::successCheck` its byte-exactness (4505 -> 4504) and fails the ratchet.
Every 8080 B variant that keeps the PLT order scores <= 81.6069 %.

## The winning (but unlandable) source shape

Replace the won-test block (HEAD, lines ~1553-1556)

```cpp
                } else if (Globals::status->getMission()->hasWon() == true ||
                           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
                    Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : (Mission *) (intptr_t) Globals::status->getCampaignMission();
                    Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
                    bool keepObjectives = campaign->hasWon() != false ? !Globals::status->getMission()->isCampaignMission() : false;
```

with

```cpp
                } else {
                    Mission *campaign = Globals::status->getCampaignMission();
                    Mission *current = Globals::status->getMission();
                    if (current->hasWon() || campaign->hasWon()) {
                    bool keepObjectives = campaign->hasWon() ? !current->isCampaignMission() : false;
                    Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : campaign;
```

and the trailing

```cpp
                } else {
                    this->levelScript->resetStartSequenceOver();
                }
```

with

```cpp
                    } else {
                    this->levelScript->resetStartSequenceOver();
                    }
                }
```

(8-space indentation of the body is intentionally left as-is; only the braces change.)

Measured: **8080 B, 83.4444 %, PLT ROT** (reset blx 0x12f2 > removeObjectives blx 0x1130).
Equivalent spellings that produce byte-identical code and the same measurement:
`if (!(current->hasWon() || campaign->hasWon())) { reset } else { body }`,
`bool won = ...; if (!won) reset; if (won) body;`, and a `goto` form.

## Why the PLT rotates (the blocker)

`resetStartSequenceOver` is referenced only in `OnTouchEnd`; the first
`removeObjectives` reference is the failed-mission cleanup at line 1544.  Gold emits
`.rel.plt` in first-referenced-address order, so the `reset` blx must sit at a **lower
address** than the first `removeObjectives` blx inside `OnTouchEnd`.

* Original: reset blx at `0x17b0ae` (fn+0xf6a) < first removeObjectives at `0x17b270` (fn+0x112c).
* HEAD: reset at fn+0x101c < remove at fn+0x11aa. **PLT OK.**
* tC shape: reset at fn+0x12f2 > remove at fn+0x1130. **PLT ROT.**

Hoisting `current`/`campaign` *inside* the `else` changes LLVM's machine-block
placement: the reset block is moved from before the campaign-chain cold blocks to
after the `... == 155` block (its DILocation is line 1712), i.e. past the failed-cleanup
`removeObjectives`.  Keeping the declarations before `bool failed` (the `hC`/`hCC`
family) preserves the original cold-block order (reset < remove) but loses the hot-code
match.

## Measurement table (all 8080 B unless noted)

| variant | size | pct | PLT |
|---|---:|---:|---|
| HEAD | 8080 | 81.6429 | OK |
| **tC (campaign+current inside else, `ne`)** | 8080 | **83.4444** | **ROT** |
| `in_current` (`xb|kF|mQ`, current only inside else) | 8080 | 82.4568 | ROT |
| `hC|mb|kG|mG` (campaign before `failed`, bare hasWon) | 8080 | 82.3635 | ROT |
| `in_campaign` (campaign only inside else) | 8084 | 82.3275 | ROT |
| `hCC|xb|kG|mC` | 8080 | 82.1898 | ROT |
| `hCC|cb|kC|mC` | 8080 | 82.1538 | ROT |
| `A|kQ|mF` (`hC`, `campaign->hasWon() || getMission()->hasWon() == true`, `keep == true`, `mF`) | 8080 | 81.6069 | OK |
| `hC|ct|kG|mF` | 8080 | 81.5709 | OK |
| `hCC|xt|kG|mG` | 8080 | 81.5268 | OK |
| `hCC|mt|kG|mG` | 8080 | 81.4615 | OK |
| `CKM_mF` / `NOVAR_CKM` (faithful campaign→keep→mission, no hoist) | 8080 | 81.4268 | OK |
| `HEAD_goto_cleanup` | 8080 | 81.2973 | OK |
| `HEAD_duptail` (reset + duplicated tail + return) | 8084 | 80.0793 | OK |
| `PRE` / `earlyreset` (reset as separate if before failed) | 8100 | 82.1454 | OK |
| `PRE_hoist` | 8100 | 80.7485 | OK |
| `dispatch_swap` (isInstantActionMission tested first) | 8084 | 74.7253 | OK |
| `tC_goto_cleanup` | 8084 | 81.8804 | ROT |
| `tC_duptail` | 8084 | 81.8804 | ROT |

## Structural findings (verified instruction-by-instruction against the original)

* The original's won-test hot code is the **campaign -> keepObjectives -> mission**
  order with **no hoisted pointer** (it re-calls `getCampaignMission()` for the mission
  ternary):
  ```
  17ade0 ldr r0,[r8]; 17ade8 blx getCampaignMission; 17adec blx hasWon
  17adf0 cmp r0,#0;   17adf2 beq keep=false
  17adf6 ldr r0,[r8]; 17adfa blx getMission; 17adfe blx isCampaignMission
  17ae02 eor sl,r0,#1; 17ae06 b mission
  ```
  Reproducing this faithfully (`NOVAR_CKM`) is 8080 B / PLT OK but only 81.4268 %,
  i.e. *below* HEAD: the fuzzy metric rewards HEAD's tuned mission-first shape.
* The original's reset block sits at fn+0xf66, immediately before the freelance body
  (fn+0xf72), and before the failed-cleanup `removeObjectives` (fn+0x112c).  HEAD
  reproduces this cold-block order (reset fn+0x1018, freelance fn+0x1022, remove
  fn+0x11a6); the tC shape does not.
* `dispatch` order: the original tests `isInstantActionMission` first
  (0x17af7e) and `isCampaignMission` second (0x17afa6).  Swapping our source to match
  is a large regression (8084 B, 74.7253 %), so the source-level order is evidently not
  what drives that block layout.
* The first `removeObjectives` in the source (line 1544, failed branch) is the one that
  pins the PLT order, not the `finishMission` one (line 1753).

## Dead ends tried this pass

* Declaration-in-`else if` condition (`else if (Mission *campaign = ...; cond)`):
  8080 B / 83.4444 % / ROT.
* `__builtin_expect` on either branch: 8076 B / 73.6121 % / ROT.
* Duplicating the shared tail into the reset arm: 8084 B / worse.
* Moving the failed-branch `removeObjectives` to the end of the cleanup: no layout
  change (still ROT).
* `goto`-based cleanup relocation: no improvement.
* `refinedAny` type (`int`/`signed char`/`short`) and `if (count > 0)` ->
  `if (count != 0)` / `if (amount < 0)`: neutral or worse; `count != 0` grows the
  function to 8088 B.
* Final 60-combination sweep of the `hC` family (5 condition spellings x 3 keep
  spellings x 4 mission spellings, campaign hoisted before `bool failed`): best
  8080 B PLT-OK result is still **81.6069 %** (`campaign->hasWon() || getMission()->hasWon() == true`
  + `keep == true` + `mF`/`mN`); best 8080 B overall is 82.3635 % but ROT.

## Recommendation

The PLT constraint is the binding one.  Any future attempt should try to obtain the
tC hot code while keeping the reset block's machine layout before the failed-cleanup
`removeObjectives`; until then HEAD (81.6429 %, 8080 B, PLT OK) is the best landable
shape.
