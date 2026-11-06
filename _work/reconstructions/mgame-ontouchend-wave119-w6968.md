# MGame::OnTouchEnd (8080 B) — w6968 (wave 119): mining-block / 127-case / nesting sweep

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Branch `wave119-w6968`, base master
`d512ec24e` (wave-119 notes head; OnTouchEnd source = w6919 best).

## Result

* No source change landed.  Before == after: **8080 B / 81.6429 %**
  (difflib raw ratio; verify.py rounds to 81.6).
* Gate at HEAD and after this pass: `RATCHET PASS`
  (byte_exact **4505**, linked_exact **4505**, size 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0, 27 sections,
  file 2,187,328 B).
* Nothing found this pass beats 81.6429 % at 8080 B with `.rel.plt`
  first-reference order preserved (PLT OK).  Every higher-scoring
  shape is either a different size (which shifts `.text` and collapses
  `byte_exact`) or rotates `.rel.plt` (which makes `MGame::successCheck`
  non-byte-exact: byte_exact 4505 -> 4504 -> RATCHET FAIL).

## Measurement method

All numbers are `raw` = `difflib.SequenceMatcher(None, onorm, unorm).ratio()*100`
from `tools/verify/verify.py`'s normalised instruction streams, at
8080 B unless stated.  `PLT OK` = `resetStartSequenceOver` first
reference address < first `removeObjectives` address in OnTouchEnd's
emitted code (original: reset 0x17b0ae < removeObjectives 0x17b270).
Build: `cmake --build cmake-build-match --target link_so -j6`.

## Mining block (`case 0` of the `_bca` refine window)

Original emits `cmp.w r9,#0 / beq.n` (`amount != 0`) and
`rsb r1,r9,#0` (`String(-amount)`); ours emits
`cmp.w r9,#-1 / bgt.n` (`count > 0`, `count = -amount`).
The first real instruction diff in the whole function is here
(normalised index 407, file offset 0x4a4).  Every faithful spelling
makes clang's Negator flip the induction variable to positive
(`add r8,r8,#1`, `mov r1,r8`) and re-allocates the whole function:

| variant | size | raw | PLT |
|---|---:|---:|---|
| HEAD (`int count = -amount; if (count > 0) ... String(count)`) | 8080 | 81.6429 | OK |
| no `count`, `if (amount != 0) ... String(-amount)` | 8088 | 75.8782 | OK |
| keep `count`, `if (amount != 0) ... String(count)` | 8088 | 75.8782 | OK |
| keep `count`, `if (count != 0) ... String(count)` | 8088 | 75.8782 | OK |
| keep `count`, `if (amount) ... String(count)` | 8088 | 75.8782 | OK |
| keep `count`, `if (count) ... String(count)` | 8088 | 75.8782 | OK |
| keep `count`, `if (amount < 0) ... String(count)` | 8080 | 81.6429 | OK (code-identical) |
| keep `count`, `if (count >= 1) ... String(count)` | 8080 | 81.6429 | OK (code-identical) |
| keep `count`, `if (count > 0) ... String(-amount)` | 8080 | 81.6429 | OK (code-identical) |
| no `count`, `if (amount < 0) ... String(-amount)` | 8084 | 79.2147 | OK |
| `unsigned amount`, `if (amount != 0) ... String((int)-amount)` | 8088 | 75.8782 | OK |
| `bool refinedAny` + `if (amount != 0) ... String(-amount)` | 8084 | 75.5315 | OK |
| `bool refinedAny` + `if (amount < 0) ... String(-amount)` | 8088 | 77.3996 | OK |
| one-line concatenation (same expression, no line break) | 8080 | 81.6429 | OK (code-identical) |
| `String line = ...; report += line;` | 8092 | 79.0564 | OK |
| `report = report + ...` | 8040 | 44.6451 | OK |

The original's `cmp r9,#0` + `rsb r1,r9,#0` is exactly the unflipped
`amount--` / `amount != 0` / `String(-amount)` shape; clang 7.0.2
flips it in every source spelling tried here (a minimal standalone
repro also flips), so the original's non-flip must come from a
surrounding-code difference, not the mining block itself.

## `refinedAny` type

Original tests bit 0 (`lsls r0,r0,#31`); ours tests the low byte
(`lsls r0,r0,#24`), i.e. original is a 0/1 `bool` and ours is `char`.
Changing the type alone does not help:

| type | size | raw | PLT |
|---|---:|---:|---|
| `char` (HEAD) | 8080 | 81.6429 | OK |
| `short` / `unsigned short` / `int8_t` / `uint8_t` / `unsigned char` | 8080 | 81.6429 | OK (code-identical) |
| `int` / `unsigned int` / `long` / `int32_t` | 8080 | 81.6069 | OK |
| `bool` (`= false`, `if (refinedAny)`) | 8084 | 78.4942 | OK |
| `bool` + `if (refinedAny != false)` / `== true` | 8084 | 78.4942 | OK |

## The separate `case 127` (new structural finding)

The original has **23** `Mission::isCampaignMission` calls in
OnTouchEnd; our HEAD has **22**.  The missing one is the 127 case:
the original's chain comparison order is

```
15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120, 126, 127, 134,
144, 155, 161, 162
```

i.e. **127 is a separate `else if` between 126 and 134**, not
`96 || 127` as in our source.  The original's 127 check is at
0x17bc26 (`isCampaignMission`; `cmp r0,#127; beq 0x17b722`) and
jumps to the *same* body as case 96 (0x17b722), so the source
duplicates the 96 body and clang tail-merges it.

Splitting it in source (96-only block + a 127 block after 126,
identical body) reproduces the call count (23) and the shared body,
but is **8100 B** (20 B larger than HEAD):

| variant | size | raw | PLT |
|---|---:|---:|---|
| SPLIT127 after 126 | 8100 | 81.2601 | OK |
| SPLIT127 + `menuTouchOpen == true` | 8088 | 77.6297 | OK |
| SPLIT127 + `jumpActive == true || ...` | 8088 | 75.2746 | OK |
| SPLIT127 + `_bc8 == true` (second) | 8084 | 75.4724 | OK |
| SPLIT127 + modal `== true` | 8096 | 80.4105 | OK |
| SPLIT127 + `cinematicSteerActive == true` | 8104 | 81.3321 | OK |
| SPLIT127 + MT + BC8C | 8084 | 69.4744 | OK |
| SPLIT127 + JUMP + BC8C | 8072 | 73.6766 | OK |
| SPLIT127 + MT + JUMP + BC8C + MODAL | 8076 | 73.6994 | OK |

No combination lands on 8080 B.  Since the original is 8080 B with
the split, the original must be ~20 B smaller than HEAD somewhere
else (block placement / branch-width differences, not a missing
statement).  This is the most promising remaining lead: find the
20 B that the original saves elsewhere, then the split should fit
at 8080 B and the score should rise (the split region is
instruction-faithful).

## Won/failed nesting (PLT experiments)

Re-nesting the `failed` / `won` / `resetStartSequenceOver` blocks
does not move the score, but it does show the PLT constraint is
robust against the faithful bare `hasWon()`:

| variant | size | raw | PLT |
|---|---:|---:|---|
| HEAD | 8080 | 81.6429 | OK |
| A: `if (!failed) { if (won) {...} else reset; } else { failed }` | 8080 | 81.5709 | OK |
| B: `if (failed) {...} else { if (won) {...} else reset; }` | 8080 | 81.6429 | OK (code-identical) |
| C: A with `if (!won) reset; else {...}` | 8080 | 81.5709 | OK |
| D: `if (failed) {...} else if (!won) reset; else {...}` | 8080 | 81.6429 | OK (code-identical) |
| A + bare top `hasWon()` | 8088 | 82.0965 | ROT |
| B + bare top `hasWon()` | 8088 | 82.1686 | ROT |
| C + bare top `hasWon()` | 8088 | 82.0965 | ROT |
| D + bare top `hasWon()` | 8088 | 82.1686 | ROT |
| `bool won = ...; if (failed) ... else if (won) ...` (`== true`) | 8088 | 72.9302 | OK |
| same with bare `hasWon()` | 8088 | 72.9302 | OK |

Hoisting `won` into a variable *does* keep PLT OK (reset stays
before removeObjectives), but costs 8 B and ~9 points.  The
bare-`hasWon()` family is always 8088 B and always rotates
`.rel.plt`, whichever nesting is used.

## Full bare-`hasWon()` single-op sweep (all ROT)

`WON_BARE` = top `getMission()->hasWon() == true` -> bare.
All 8088 B / ROT unless noted; no 8080 B:

`P_PAUSE P_HASCARGO P_SUPER P_TURRET P_SMNULL P_SMOPEN P_LAYOUTCW
P_DOINST P_CUT P_DIALOG P_ISCAMP P_CARGO P_GOTO P_DISPATCH P_KEEP
P_ESCORT P_MT P_MSG P_MUTED P_PLAYERNE P_FCINE P_FCINE2 P_HUDNZ
P_HUD1 P_PAUSENOT P_MWNULL P_STARTSEQ P_HACK P_ROCKET P_AUTOTURRET
P_HUD40 P_ALIEN P_BC8A P_ISAUTO P_BC8B P_DOCKAST P_DOCKPT P_DOCKSTR`
all give 8088 / 82.1686 / ROT (code-identical to WON_BARE alone).
Deviating: `P_CHOICEW` 8072/38.5914, `P_CF` 8096/75.009,
`P_BCA` 8068/50.5656, `P_AUTOP` 8088/73.5363,
`P_EXITREQ` 8088/81.8444, `P_TOUCHSTR` 8088/81.7363,
`P_TOUCHSTR2` 8088/82.1326, `P_INSTANT` 8088/81.4268,
`P_FREECAM` 8096/79.4022, `P_MODAL` 8088/81.4335,
`P_MWONTE` 8092/80.7852, `P_SOUNDSM` 8088/80.6701,
`P_CINE` 8088/82.1326, `P_JUMP` 7952/44.9858,
`P_BC8C`/`P_BC8D` 8072/75.2746.

## Prior-pass numbers carried forward (same HEAD base)

Nothing beat 81.6429 at 8080 B / PLT OK in the earlier w6919 /
w6914 / w6838 sweeps either.  Highest raw shapes seen are all
size/PLT-incompatible:

| variant | size | raw | PLT |
|---|---:|---:|---|
| `isInstantActionMission() == true` (`P_INSTANT`) | 8092 | 82.1 | ROT |
| `menuWindow OnTouchEnd` bare + `P_INSTANT` | 8088 | 82.6878 | ROT |
| `bool refinedAny` + MWONTE + INST | 8088 | 82.2915 | ROT |
| bare top `hasWon()` (`WON_BARE`) | 8088 | 82.2 | ROT |
| `P_CHOICEW` (`choiceWindowOpen == true`) | 8080 | 38.6355 | OK |
| `P_CF` (`field_0xcf == true`) | 8092 | 74.1 | OK |
| `P_BCA` (`_bca == true`) | 8068 | 50.2 | OK |
| `P_AUTOP` | 8088 | 71.1 | OK |
| `P_SMOPEN` | 8076 | 76.5 | OK |
| `P_LAYOUTCW` | 8076 | 79.0 | OK |
| `P_EXITREQ` | 8080 | 81.4 | OK |
| `P_TOUCHSTR` | 8076 | 80.1 | OK |
| `P_CUT` | 8076 | 75.2 | OK |
| `P_GOTO` (bare `goingToWaypoint`) | 8076 | 80.5 | OK |
| `P_MT` | 8076 | 77.9 | OK |
| `P_FREECAM` | 8084 | 77.3 | OK |
| `P_MODAL` | 8076 | 79.7 | OK |
| `P_MWONTE` (bare menuWindow OnTouchEnd) | 8084 | 79.1 | OK |
| `P_SOUNDSM` | 8080 | 79.6 | OK |
| `P_JUMP` | 7956 | 45.8 | OK |
| `P_BC8C` | 8080 | 75.0 | OK |
| `bool refinedAny` | 8084 | 78.4942 | OK |
| ORD1 (campaign/keepObjectives before mission) | 8080 | 81.4268 | OK |
| SWAPA (won/failed swap) | 8080 | 81.5709 | OK |
| SPLIT127 after 134 | 8100 | 81.2601 | OK |

## Structural facts (for the next pass)

1. **Dispatch order**: original emits `isInstantActionMission()`
   (0x17af7e) *before* `isCampaignMission()` (0x17afa8); ours is
   campaign-first.  Faithful instant-first is 8080 B / 75.9416 / OK
   with the ORD1 preamble (campaign, keepObjectives, mission).
2. **Original won-block preamble order**: `campaign =
   getCampaignMission()` (0x17ade8), `campaign->hasWon()`
   (0x17adec), `keepObjectives = ... !getMission()->isCampaignMission()`
   (0x17adfe), then the mission ternary re-fetches
   `getMission`/`getCampaignMission` (0x17af62-0x17af7c).  ORD1
   matches this order and is 8080 / 81.4268 / OK.
3. **Case chain order**: 15, 22, 43, 42, 65, 81, 74, 95, 96, 100,
   110, 120, 126, **127**, 134, 144, 155, 161, 162 (see above).
4. **Branch layout**: original puts the `failed` block at 0x17b374
   (after the campaign chain), ours at 0x17b0e4 (before it);
   original puts the mission-select block at 0x17af5e before the
   hud epilogue 0x17afd8, ours after (0x17b00c/0x17b046).
5. **`.text` size**: original ends 0x17c0d4, ours 0x17c0cc (8 B
   shorter).  First real instruction diff at +0x486 (mining compare).
6. **Inline literal-pool groups** (original / ours, word counts):
   `0x3a4(12) 0x6b4(7) 0xac4(5) 0xe7c(6) 0x1278(9) 0x166c(17)
   0x1a28(16) 0x1e20(7) 0x1f60(12)` /
   `0x3a4(12) 0x6b0(7) 0xac0(5) 0xe84(5) 0x12b0(10) 0x1660(16)
   0x1a40(17) 0x1e20(7) 0x1f60(12)` — same 91 words, different split.
7. **Call multiset**: original has one extra `isCampaignMission`
   (the separate 127 case); ours has one extra `68424`
   (`PlayerEgo::removeRoute`?) and one `6840c` vs `68418`.

## PLT constraint (do not regress)

`.rel.plt` first-reference order is set by OnTouchEnd's own emitted
code: `resetStartSequenceOver` is referenced by no other function,
and `removeObjectives` is referenced later in link order
(`MGame::OnUpdate`, `MGame::successCheck`).  OnTouchEnd must emit
the reset block before the first `removeObjectives` or
`MGame::successCheck`'s `blx` offsets change and `byte_exact`
drops 4505 -> 4504 (RATCHET FAIL).  Any size change to OnTouchEnd
also shifts every later function and collapses `byte_exact` to
~3100, so the landing target is strictly **8080 B + PLT OK**.

## Gate

`cmake --build cmake-build-match --target check -j6` on the final
tree: `verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0
avg 99.94`, `data: exact 843 of 843`, `sodiff: extra-beyond-allow 0
missing 0 allowed 0 imports 207`, `RATCHET PASS`.
