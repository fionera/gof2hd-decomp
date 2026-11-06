# MGame::OnTouchEnd — wave 125 (w7513): campaign temp + hudMenu else-if chain

Status: one landed source tweak, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7513`,
base `02513de33` (wave-124 final), owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch: `/dev/shm/w7513*` (not committed).

Target: `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | before (wave 124) | after |
|---|---|---|
| fuzzy match | 78.8 % | **81.3 %** |
| body size (orig/ours) | 8080 / 8080 | 8080 / 8080 |
| `.rel.plt` differing bytes | 0 | **0** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.ARM.extab` differing bytes | 1,474 | **1,436** (size exact `0x26298`) |
| `.text` differing bytes | 85,975 | **85,636** |
| avg match | 99.93 % | **99.94 %** |

## Change (two size-neutral edits: −4 B + 4 B)

```diff
                     if (won) {
-                        bool keepObjectives = ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon() != false ? !Globals::status->getMission()->isCampaignMission() : false;
-                        Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : (Mission *) (intptr_t) Globals::status->getCampaignMission();
+                        Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
+                        bool keepObjectives = campaign->hasWon() != false ? !Globals::status->getMission()->isCampaignMission() : false;
+                        Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : campaign;
                         if (mission->isInstantActionMission()) {
...
             this->hud->closeHudMenu();
             this->pauseOpen = 0;
             resumeSounds();
-        }
-        if (this->hudTouchFlags == 0) {
+        } else if (this->hudTouchFlags == 0) {
             this->hud->closeHudMenu();
```

1. **`campaign` temporary** — one `getCampaignMission()` call is reused for the
   `keepObjectives` test and the `mission` fallback instead of refetching it. This is a
   **−4 B** codegen change (one fewer `blx Status::getCampaignMission`).
2. **`hudMenu` else-if chain** — the trailing `if (hudTouchFlags == 0)` is folded into the
   preceding `else if (wingmanCommand >= 0)` chain. This is a **+4 B** codegen change and is
   the original's shape: the wingman tail at `0x17aae0` branches back into the
   `_bc8`/`resetGunDelay` tail shared with the `hudTouchFlags == 0` block, i.e. the chain form.

The two edits are size-neutral *together*: campaign-temp alone is 8076 B (byte_exact collapses
to 4486), else-if alone is 8084 B (byte_exact collapses to 3106). Together they keep the body
at exactly **8080 B**, so every later `.text` symbol stays put and `byte_exact` stays 4506.
The `.rel.plt` first-reference order is unchanged (`isInstantActionMission` @ `0x17aff2` before
`resetStartSequenceOver` @ `0x17b088`), so `.rel.plt` stays byte-identical.

## Evidence

Original won-body (`0x17ade0`) computes `keepObjectives` first, then the `mission` selection;
the candidate keeps that order. The candidate's `isInstantActionMission` call is at `0x17aff2`
(original `0x17af7e`), the cold `resetStartSequenceOver` block at `0x17b088` (original
`0x17b0ae`) — i.e. the reset block is still *after* `isInstantActionMission`, exactly the PLT
ordering the original has. The shared canary return block is at `0x17b992` (original `0x17b004`);
moving it back is the remaining root cause (see below).

Original `hudMenu` wingman tail (`0x17aae0`):

```
17aad8: ldr r0,[fp,#88]      ; player
17aae0: cmp r0,#0
17aae2: beq.w 17acea
17aae6: blx resetGunDelay
17aaea: b.n 17acea
```

i.e. the wingman block shares the `_bc8`/`resetGunDelay` tail with the `hudTouchFlags == 0`
block — the `else if` chain form.

## Gate (full `check`)

```
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` CLEAN, `drift` 0 fields across 0 classes. Section sizes all exact: `.text` 1,259,464 B,
`.rel.plt` 29,392 B, `.ARM.exidx` 28,528 B, `.ARM.extab` 156,312 B (`0x26298`), `.rodata`
269,159 B. `.ARM.exidx`, `.rel.plt`, `.rodata` content byte-identical; `.ARM.extab` size/base
exact (content differs in 1,436 bytes, down from 1,474).

## Variants tried and rejected

All measured against the landed 81.3 % / 8080 B state unless noted. `pct / size / PLT / exidx /
extab` = fuzzy %, body size, `.rel.plt` differing bytes, `.ARM.exidx` differing bytes,
`.ARM.extab` differing bytes.

| variant | pct | size | PLT | exidx | extab |
|---|---|---|---|---|---|
| **landed (campaign temp + else-if)** | **81.3** | **8080** | **0** | **0** | **1436** |
| campaign temp only (no else-if) | 78.5 | 8076 | 0 | 24 | 1476 |
| else-if only (no campaign temp) | 80.3 | 8084 | 0 | 3586 | 2150 |
| + `isInstantActionMission() == true` (X) | 82.7 | 8084 | 0 | 4015 | 9312 |
| + X + `Mission *current = getMission()` temp | 82.7 | 8080 | 0 | 429 | 8683 |
| + `Mission *current` temp only (no X) | — | 8076 | 0 | 0 | 1436 |
| bare `hasFailed()` | 74.7 | 8072 | 6 | — | — |
| bare `campaign->hasWon()` | 81.3 | 8080 | 0 | — | — |
| bare won `hasWon()` | 76.5 | 8064 | 0 | — | — |
| `goingToWaypoint() == true` | 81.3 | 8080 | 0 | — | — |
| `!wasAutoPilot && isAutoPilot()` reorder | 80.3 | 8076 | 0 | 24 | 1467 |
| `menuTouchOpen == true` | 76.6 | 8064 | 6 | 346 | 1466 |
| `pauseOpen == true` | 81.3 | 8080 | 0 | 0 | 1436 |
| `hudTouchFlags` `!` / `!= -1` / `!= nullptr` spellings | 82.7 (with X) | 8084 | 0 | 4015 | 9312 |

The `X` / mission-temp combinations reach **82.7 %** but are not landable: they shrink
`.ARM.extab` to `0x26294` (4 B short) and break `.ARM.exidx` byte-identity. `X` alone changes
the `isInstantActionMission` branch inside an EH region (`cbz` → `cmp #1; beq`), which changes
the LSDA call-site table length. The mission temp alone keeps `.ARM.extab` size and
`.ARM.exidx` exact, but is 4 B short (8076 B) and needs a +4 B non-EH anchor that was not found.

## Remaining root cause

The shared stack-canary return block sits at `0x17b992` (offset `0x184e`) instead of the
original `0x17b004` (offset `0xec0`). The original lays the cold `hudTouchFlags == 0`, failed
`type == 12` and `resetStartSequenceOver` blocks *after* the shared return; our
`MachineBlockPlacement` keeps them before it. The `else if` chain makes the
`hudTouchFlags == 0` block a fallthrough (hot) rather than a branch target (cold), which is the
opposite of the original's `cbz r0, 0x17b01a`. Reproducing the original block order needs the
cold blocks to be outlined past the return; every 8080 B shape tried keeps at least one of them
before it. The `X`/mission-temp 82.7 % shapes move the layout closer but cost the `.ARM.extab`
size.
