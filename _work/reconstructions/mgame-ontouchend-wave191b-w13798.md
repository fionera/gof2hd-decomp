# MGame::OnTouchEnd (8080 B) — w13798 (wave 191b): cinematic-steer `case 0`
# explicit arm

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, 8080 B).
Branch base `2136d291a`; worker w13774 also owns this function — this pass is the
**cinematic-steer switch** slice only.

Prior notes: `mgame-ontouchend-wave99-w4907.md` (newest), wave 94..98 notes.

## 0. Landed change

The pause-menu "skip" cinematic switch was written as

```cpp
switch (Globals::status->getCurrentCampaignMission()) {
    case 154: case 157: case 158: this->levelScript->skipCutscene(); break;
    case 1:  switch_to_target_setting = 0; active = 0; SetCurrentApplicationModule(5); break;
    default:
        if (getCurrentCampaignMission() == 0) {
            nextCampaignMission(true); setKills(3); switch_to_target_setting = 1;
            active = 0; SetCurrentApplicationModule(2); initStreamOutPosition = 0;
        }
        break;
}
```

The original (`17b64c`) tests in the order `cmp r0,#1; beq 17b7f4` →
`cmp r0,#0; bne 17b810` → case-0 body.  Making `case 0:` an explicit arm
placed before `case 1:` reproduces that test order (the 154/157/158 arm is
unchanged, still reached through the `sub #154 / lsl / tst #25` bitmap).

## 1. Measured table (this pass, `tools/fn_diff.py` + `secdiff.py`)

| variant | raw | aligned | size | .text | .ARM.extab |
|---|---:|---:|---:|---:|---:|
| baseline (`2136d291a`) | 3,683 | 544 | 8,080 | 75,119 | 587 |
| **case 0 explicit, before case 1 (landed)** | **3,665** | **545** | **8,080** | **75,101** | **587** |
| case 1 explicit before case 0 | 3,665 | 545 | 8,080 | 75,101 | 587 |
| `setFreelanceMission` before reward message | 3,667 | 545 | 8,080 | 75,103 | 587 |
| instant-action as a separate `if` before campaign | 4,730 | 1,141 | **8,092** | 75,496 | 1,310 |
| 95 body switch-first | 4,071 | 733 | **8,084** | — | — |
| 95 clause moved after 74 | 3,751 | 799 | 8,080 | — | — |
| `default: if(==1) else if(==0)` | 3,692 | 766 | 8,080 | — | — |

`check` on the landed variant: byte_exact 4508, linked_exact 4508, size
4524/4524, data 843/843, imports 207, lint CLEAN, RATCHET PASS.  The function
remains non-exact, so the hard count is unchanged; only raw/.text drop.

## 2. What is left (biggest aligned clusters, current tree)

- `O17b6ec/U17b758` 46/2 — the 95/96 campaign bodies.  The original shares the
  `initStreamOutPosition=1; departStation(galaxy->getStation(N)); active=0;
  SetCurrentApplicationModule(2)` tail between 95 and 96 by loading the station
  id into `r1` before a common `b.n 17b77c`.  The repo *already* shares that tail
  (`b.n 17b76e` from the 95 body), but the 95 body sits at `17b3ba` instead of
  `17b6a8` because clang's machine-block placement put it elsewhere.  Reordering
  the source clause costs +4 B (size 8,084) — not viable.
- `O17b9d8/U17b9c0` 25/9 — weapon-slot body.  Original lays it between the 100
  and 110 campaign bodies (`17b9ca`); ours puts it after 110.  This is cold-block
  placement, not source order.
- `O17b3b4/U17b456` 22/1 — cargo loop.  The original has a 12-word literal pool
  between the loop init and the loop body (`b.n 17b40e` over the pool); ours has
  no pool there.  Literal-pool placement, not source.
- `O17b1e2/U17b21e` 21/1 — `PlayEngineSound` block; same shape at a different
  address, purely a layout shift.
- `O17b910/U17b922` 14/0 — `isMining()` cold block before the 100 body.

## 3. Dead ends measured this pass

- `setFreelanceMission` before `showMissionRewardMessage` (matching the original
  `17b0b6` order): 3,667 — +2 raw, no aligned gain.
- Instant-action as a separate `if (...) { ...; return; }` before the campaign
  `if`: size grows to 8,092 (+12 B) — gate-fatal.
- `case 1` before `case 0`: numerically identical to the landed order.
- `default: if (==1) ... else if (==0) ...`: 3,692 / 766.

The remaining differences are dominated by clang 7.0.2 `-Oz` machine-block and
literal-pool placement, which source reordering does not reach without changing
the emitted size.  No further size-exact win was found in this slice.
