# MGame::OnTouchEnd — wave 119 (w6954)

Target: `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`, ~line 1295).
Original: 8080 B. Baseline at HEAD: **81.6429 %** (8080 B, gate byte 4505 / linked 4505).
Landed: **81.6790 %** (8080 B, gate byte 4505 / linked 4505) — the only landable win found.

The fuzzy metric is dominated by **basic-block placement** (intra-function branch targets are
compared literally as `loc_<offset>`), not by instruction selection.  The stripped-branch ratio
for the landed variant is 84.81 %, i.e. ~3.1 points of the raw score are pure block-offset noise.

## Landed change

In the `finishMission` tail (second `goingToWaypoint()` occurrence, ~line 1756):

```cpp
- if (this->player->goingToWaypoint() == true) {
+ if (this->player->goingToWaypoint()) {
```

The *first* `goingToWaypoint() == true` (failed branch, ~line 1547) must stay `== true`:
flipping it alone gives 8076 B / 80.5 %; flipping both gives 8076 B / 80.5 %.

## Landable-vs-not summary (all 8080 B unless noted)

| variant | size | pct | gate byte/linked |
|---|---:|---:|---|
| HEAD base | 8080 | 81.6429 | 4505/4505 |
| **base + GO2 (landed)** | **8080** | **81.6790** | **4505/4505** |
| base + `keepObjectives` without `!= false` | 8080 | 81.6790 | 4505/4505 (code-identical) |
| base + `unsigned char refinedAny` | 8080 | 81.6790 | 4505/4505 (code-identical) |
| base + `amount < 0` (keep `count`) | 8080 | 81.6790 | 4505/4505 (code-identical) |
| base + `int refinedAny` | 8080 | 81.6 | 4505/4505 |
| base + top `hasWon()` bare + GO2 | 8084 | 82.3 | 3105/4505 |
| **tb_reuse + GO2** | **8080** | **83.7** | **4504/4505 (RATCHET FAIL)** |
| tb_reuse + GO2 + `!= false` top | 8080 | 83.7 | 4504/4505 |
| tb_reuse + GO2 + `!(!A && !B)` top | 8080 | 83.7 | 4504/4505 |
| tb_reuse + GO2 + inverted/nested branch | 8080 | 83.7 | 4504/4505 |
| tb_reuse + GO2 + eager `!failed && !(A||B)` first | 8080 | 83.7 | 4504/4505 |
| tb_reuse + GO2 + swap changeCredits/m_nTimeLimit | 8080 | 83.6 | 4504/4505 |
| tb_reuse + GO2 + swap campaign cases 15/22 | 8080 | 83.6 | 4504/4505 |
| tb_reuse + GO2 + keepObjectives after m_nTimeLimit | 8080 | 82.4 | 4504/4505 |
| tb_reuse (no GO2) | 8084 | 82.6 | 3105/4505 |
| tb_reuse + GO1 only | 8084 | 82.6 | 3105/4505 |
| hoist+reuse, `== true` top, GO2 | 8072 | 80.2 | 4288/4505 |
| hoist+reuse, `!= false` top, GO2 | 8080 | 83.7 | 4504/4505 |
| hoist, no reuse, bare top, GO2 | 8084 | 82.6 | 3105/4505 |
| hoist, no reuse, `== true` top, GO2 | 8076 | 80.2 | 4485/4505 |
| reuse + instant-first + GO2 | 8068 | 74.1 | 4288/4505 |
| faithful (tb/hoist/plain/no-reuse/instant-first) | 8072 | 74.3 | 4288/4505 |
| faithful + mining `amount != 0` | 8080 | 73.3 | 4505/4505 |
| faithful + mining + GO2 | 8080 | 73.4 | 4505/4505 |
| eager `bool won` variant | 8084 | 81.8 | 3105/4505 |
| `__builtin_expect(won,1)` | 8076 | 73.9 | 4485/4505 |
| `__builtin_expect(won,0)` | 7824 | 59.1 | 3136/4505 |
| move mission-15 case to end of switch | 8152 | 68.8 | 3105/4505 |
| move campaign case 42 to end | 8120 | 71.9 | 3105/4505 |
| move `if (!keepObjectives)` after escort | 8060 | 80.2 | 4288/4505 |
| 96/127 split (prior session) | 8100 | 81.3 | 3105/4505 |

## The `tb_reuse + GO2` dead end — PLT rotation

`tb_reuse + GO2` is the best *shape* found (83.7 %, 8080 B) but the ratchet rejects it because
**`MGame::successCheck` loses byte-exactness** (4505 -> 4504).  Nothing else changes.

Source shape:

```cpp
} else if (Globals::status->getMission()->hasWon() ||            // bare (was `== true ||`)
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
    Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
    bool keepObjectives = campaign->hasWon() != false ? !Globals::status->getMission()->isCampaignMission() : false;
    Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : campaign;
```

Cause: `.rel.plt` order is first-call-site order in `.text`.

* HEAD: `resetStartSequenceOver` @ `0x17b160` < first `removeObjectives` @ `0x17b2ee` -> PLT
  `reset=683d0`, `remove=683dc` (matches the original).
* `tb_reuse + GO2`: first `removeObjectives` @ `0x17b278` < `resetStartSequenceOver` @ `0x17b436`
  -> PLT rotates to `remove=683d0`, `reset=683f4`.  `successCheck` calls `removeObjectives`, so its
  `blx` immediate changes and it stops being byte-exact.

The original has `reset` @ `0x17b0ae` and first `removeObjectives` @ `0x17b270`, i.e. reset first.

Every attempt to move the reset block earlier from source failed because clang canonicalises the
CFG and re-places the cold `else` block at ~`0x17b432` regardless of source order:
inverting the condition, nesting, an eager `!failed && !(A || B)` guard, `!= false`, swapping the
failed/won branches, and reordering the campaign-switch cases all produce the same block.  Only
changing the CFG (`__builtin_expect`, instant-first dispatch, splitting 96/127) moves it, and every
such change also changes the function size away from 8080 (or drops the score by >5 points).

## Other measured dead ends

* **Mining loop.**  Original is `cmp.w r9,#0; beq` (`if (amount != 0)`), ours is
  `cmp.w r9,#4294967295; bgt` (`if (count > 0)` with `count = -amount`).  Same 4-byte size, but
  any source spelling that matches the original (`if (amount)`, `if (amount != 0)` with
  `String(-amount)`) re-schedules the surrounding String construction and costs +4/+8 B
  (8084/8088).  `if (amount < 0)` keeping `count` is code-identical to `count > 0`.
* **`refinedAny`.**  Original is a byte flag (`strb`/`ldrb`, `lsls r0,r0,#31`); our `char` gives
  `lsls r0,r0,#24`.  `bool` gives `#31` but costs +4 B (8084) and drops the score to 78.5 %.
  `unsigned char` is code-identical to `char`.
* **96/127 campaign cases.**  Original has two separate `== 96` / `== 127` cases jumping to the
  same body; our source merges them.  Splitting costs +20 B (8100).
* **Instant-first dispatch.**  REA shows the original tests `isInstantActionMission()` first, but
  instant-first source is 8068-8084 B in our build.
* **Faithful shape is not the best match.**  The REA-faithful source (bare top, hoisted
  campaign/keepObjectives, `getCampaignMission()` re-fetched for `mission`, instant-first) is 8072 B
  and only 74.3 %; adding the mining `amount != 0` brings it to 8080 B but still 73.3 %.

## Literal-pool group layout (HEAD)

```
orig : 0x3a4(12) 0x6b4(7) 0xac4(5) 0xe7c(6) 0x1278(9) 0x166c(17) 0x1a28(16) 0x1e20(7) 0x1f60(12)
ours : 0x3a4(12) 0x6b0(7) 0xac0(5) 0xe84(5) 0x12b0(10) 0x1660(16) 0x1a40(17) 0x1e20(7) 0x1f60(12)
```

First real instruction diff at offset 0x486 (mining loop).  Original `.text` is 8 B longer overall
(our extra padding/scheduling is elsewhere); function size is 8080 B on both sides.

## Gate

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**
(byte 4505, linked 4505, data 843/843, imports 207, lint CLEAN, drift 0).
No section/extab changes: OnTouchEnd is still 8080 B, `.text` size unchanged.
