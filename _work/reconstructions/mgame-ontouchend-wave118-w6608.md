# MGame::OnTouchEnd (8080 B) — w6608 (wave 118): gate-safe 79.0488 -> 80.1441 %

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `3d4a98a3e`, branch
`wave118-w6608`.

## Result

**Gate-safe improvement landed.**  HEAD was **8080 B / 79.0488 %**
(official verify 79.0).  The new source is **8080 B / 80.1441 %**
(official verify 80.1), `.rel.plt` diff **0**, `RATCHET PASS`
(byte_exact 4501, linked_exact 4501, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0).  All 27 ELF sections keep their
original name/address/size and the file stays 2,187,328 B.

The gain is +1.10 pp.  The remaining gap is still block placement and
literal-pool placement; the best *fuzzy* variant found this wave
(83.17 %) is not gate-safe because it rotates `.rel.plt`.

## The three edits

```cpp
} else if (Globals::status->getMission()->hasWon() == true ||
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
    Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
    bool keepObjectives = campaign->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
    Mission *mission = Globals::status->getMission()->hasWon() == true ? Globals::status->getMission() : (Mission *) (intptr_t) Globals::status->getCampaignMission();  // (2)+(3)
    if (mission->isCampaignMission()) {
        Globals::status->nextCampaignMission(true);
    } else if (mission->isInstantActionMission()) {
        ...
    }
    ...
    } else if (mission->isCampaignMission() &&
               (Globals::status->getCurrentCampaignMission() == 96 ||
                Globals::status->getCurrentCampaignMission() == 127)) {   // (1)
```

1. **`merge127`** — drop the redundant inner `mission->isCampaignMission()`
   from the 96/127 branch.  Behaviour-neutral (the outer test already
   established it).
2. **`novar`** — re-call `Globals::status->getCampaignMission()` in the
   mission-selection ternary instead of reusing `campaign`.  This is in
   fact what the original does: the disassembly calls
   `Status::getCampaignMission` twice (0x17ade8 for `keepObjectives`,
   0x17af78 for the ternary).
3. **`selTrue`** — write the ternary test as `hasWon() == true`.
   `hasWon()` returns `unsigned char`, so the bare form is
   `icmp eq i8 %x, 0` and gets the 37.5/62.5 zero heuristic; `== true`
   is `icmp eq i8 %x, 1` and falls back to 50/50.  That probability
   change moves the cold block chains just enough to gain 1.1 pp while
   keeping `resetStartSequenceOver` before `removeObjectives`.

All three are behaviour-neutral.

## 1. Why the 8080 B / PLT-order pairing is a hard constraint

`byte_exact` is extremely sensitive to the function size and to the
`.rel.plt` order:

* any build that is not exactly **8080 B** shifts `.text` and drops
  `byte_exact` to ~3103 (ratchet FAIL);
* the original `.rel.plt` order at indices 3324..3327 is
  `resetStartSequenceOver`, `removeObjectives`, `goingToWaypoint`,
  `removeRoute`.  If the emitted function first references
  `removeObjectives` before `resetStartSequenceOver`, the four entries
  rotate and `byte_exact` drops 4501 -> 3103.

The landed source emits `resetStartSequenceOver` (0x17b160) before the
first `removeObjectives` (0x17b2ee), so the PLT order is preserved.

## 2. Measured variants (all re-measured this wave, base = HEAD)

`score.py` prints size / fuzzy / `.rel.plt` status.

| variant | size | pct | plt |
|---|---:|---:|---|
| HEAD | 8080 | 79.0488 | OK |
| merge127 | 8072 | 78.9474 | OK |
| novar | 8084 | 77.9679 | OK |
| selTrue | 8080 | 79.0128 | OK |
| merge127+novar | 8076 | 78.9474 | OK |
| merge127+selTrue | 8072 | 78.9113 | OK |
| novar+selTrue | 8088 | 77.8678 | OK |
| **merge127+novar+selTrue** | **8080** | **80.1441** | **OK** |
| merge127+novar+selTrue+campEqT | 8080 | 80.1441 | OK |
| merge127+novar+selTrue+keepAnd | 8080 | 80.1441 | OK |
| merge127+novar+selTrue+keepIf | 8080 | 80.1441 | OK |
| merge127+novar+selTrue+swapKM | 8080 | 80.1441 | OK |
| merge127+novar+selTrue+keepBitAnd | 8080 | 79.9496 | OK |
| merge127+novar+selTrue+instNe0 | 8080 | 80.1441 | OK |
| merge127+novar+selTrue+instEqT | 8088 | 83.1712 | **DIFF** |
| merge127+novar+selTrue+instEqT+condTrue | 8088 | 79.5098 | OK |
| merge127+novar+selTrue+instEqT+wonbare+andnot | 8080 | 84.3699 | **DIFF** |
| merge127+novar+selTrue+instEqT+wonbare+condTrue | 8080 | 84.0454 | **DIFF** |
| merge127+novar+selTrue+countNe0 | 8084 | 74.5628 | OK |
| dispI (instant-first) | 8068 | 74.1889 | OK |
| dispI+novar+merge127 | 8072 | 73.3694 | OK |

The 83–84 % variants all rotate `.rel.plt` (the `instEqT` probability
change moves the cold `reset` chain after the failed-mission
`removeObjectives` block).  `instEqT+condTrue` restores the PLT order
but only scores 79.51 %.

## 3. Block-placement diff that remains

The original's emitted block order (from the Thumb disassembly) is:

```
hasWon check (0x17adc8) -> campaign hasWon/keepObjectives (0x17ade0)
-> mission select (0x17af5e) -> instant dispatch (0x17af7e)
-> campaign dispatch (0x17afa6) -> continuation (0x17afe2)
-> failed branch (0x17b07e) -> reset block (0x17b0aa)
-> freelance body (0x17b0b6) -> changeCredits (0x17b0e2)
...
-> failed removeObjectives (0x17b270) -> setFreelanceMission (0x17b888)
-> shared continuation / frameTime=0 / resetAnalogStick (0x17b8e2)
```

The landed build keeps the shared continuation at 0x17b8fa (close to
the original 0x17b8e2) and `reset` at 0x17b160 (original 0x17b0ae).
The remaining large `difflib` regions are the 59-instruction campaign
dispatch block and a 62-word literal-pool block; both are pure
`MachineBlockPlacement` / pool-placement differences that source
spelling cannot pin down without changing the PLT order.

Ghidra's decompilation confirms the original source shape is
`keepObjectives = campaign->hasWon() ? !getMission()->isCampaignMission() : false;`
followed by a re-fetch of `getCampaignMission()` in the mission
ternary, and an instant-action-first dispatch.  The instant-first
source (`dispI`) is faithful but scores only 74.19 % because its cold
block chain lands differently; the campaign-first source is a tuned
shape that matches the emitted instruction stream better.

## 4. Structural state (unchanged by this pass)

27 sections; file 2,187,328 B.  `.ARM.extab` is base-exact at
0x1a3b78 but **12 B short** (`0x2628c` vs `0x26298`) — this is the
current HEAD state (the wave-110 note's "4 B short" is stale).
`.rodata` is base-exact at 0x1c9e10 and 192 B larger (the appended
`Status::missionCompleted` rank table).  Neither number changed.

## 5. Dead ends measured

* `count > 0` -> `count != 0` / `amount != 0` in the mining-plant
  refine loop fixes the `cmp r9,#0; beq` vs `cmp r9,#-1; bgt`
  encoding difference at 0x17a5ca, but changes register allocation and
  drops the score to 74.56 % (8084 B).
* `failed == true` / `failed != 0` / `failed == false` on the
  `hasFailed()` temp: 76.60 / 80.14 / 72.98 %.
* `keepObjectives` as `&&`, `&`, `*`, `!=0 & ==0`, statement-if, or
  moved to the `finishMission` use site: all <= 80.14 %.
* `instNe0` / `campNe0` / `campEqT` / `keepAnd` / `keepIf` / `swapKM`
  are code-identical to the landed variant.
