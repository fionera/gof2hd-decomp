# MGame::OnTouchEnd (8080 B) — w6740 (wave 119): angle-B sweep, no gate-safe win

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `6552d3af5`, branch
`wave119-w6740`.

## Result

**No source change landed; HEAD stays at 8080 B / 80.1441 %**
(official verify 80.1).  `check` is `RATCHET PASS` on the untouched
tree: byte_exact **4505**, linked_exact **4505**, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  All 27 ELF sections
keep their original name/address/size and the file stays 2,187,328 B
(`.text` 0x1337c8, `.ARM.extab` 0x2628c, `.rodata` 0x41c27,
`.rel.plt` 0x72d0 — unchanged).

Every variant below is behaviour-neutral and was measured with the
real NDK r18b link (size + unrounded `verify` SequenceMatcher score +
`.rel.plt` first-reference order).

## The hard constraints (unchanged from wave 118)

1. The function **must stay exactly 8080 B**.  Any other size shifts
   `.text` and drops `byte_exact` 4505 -> ~1761/3103 (ratchet FAIL).
2. The first reference to `LevelScript::resetStartSequenceOver` must
   come **before** the first `Level::removeObjectives` in the emitted
   function, otherwise `.rel.plt` indices 3324..3327 rotate and
   `byte_exact` collapses.
3. The same holds for `inCinematicMode` / `setCinematicMode` /
   `resetAnalogStick` at 3329..3331.

## The 84.37 % false lead

Wave 118's note listed `hoist reset + instEqT + failed==true` as
8080 B / 84.3699 % / PLT OK.  That is **not reproducible** as a
gate-safe variant.  The actual 84.3699 % source is
`TOP bare + INST == true + (trailing else deleted)`, i.e. the
`resetStartSequenceOver()` call is **removed entirely**:

```cpp
} else if (Globals::status->getMission()->hasWon() ||
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
    ...
}
this->frameTime = 0;          // no `else { reset(); }`
```

It is 8080 B and scores 84.3699 %, but `resetStartSequenceOver`
vanishes from `.rel.plt` (it has no other caller in the tree), so
`byte_exact` drops to **1761** — `RATCHET FAIL`.  The "PLT OK" in the
old table came from a `find()` that returns -1 when the symbol is
absent (`-1 < removeObjectives`).  This is the central trap of the
angle.

The genuine hoisted shape (`!(won1 || won2) -> reset; else { body }`,
which is what the REA decompiler prints) is **8088 B** and places the
reset call *after* `removeObjectives` (PLT ROT), so it is doubly
disqualified.

## Measured variants (all behaviour-neutral)

Base = HEAD.  "PLT" = first-reference order of
`resetStartSequenceOver` vs `removeObjectives` in the emitted code.

| variant | size | score | PLT |
|---|---:|---:|---|
| HEAD | 8080 | 80.1441 | OK |
| `failed == true` | 8080 | 80.1441 | OK |
| `cutsceneActive != 0` | 8080 | 80.1441 | OK |
| `pauseOpen != 0` | 8080 | 80.1441 | OK |
| `dialogueWindow->OnTouchEnd(...) == false` | 8080 | 80.1441 | OK |
| `starMap->OnTouchEnd(...) == false` | 8080 | 80.1441 | OK |
| `doInstantJump == false` | 8080 | 80.1441 | OK |
| `menuWindow == nullptr` -> `!menuWindow` | 8080 | 80.1441 | OK |
| `player != nullptr` -> `player` | 8080 | 80.1441 | OK |
| `enemies != nullptr` -> `enemies` | 8080 | 80.1441 | OK |
| `cargo != nullptr` -> `cargo` | 8080 | 80.1441 | OK |
| `!keepObjectives` -> `keepObjectives == false` | 8080 | 80.1441 | OK |
| `hudTouchFlags != 0` -> `hudTouchFlags` | 8080 | 80.1441 | OK |
| `autopilotMenuOpen != 0` | 8080 | 80.1441 | OK |
| `field_0xc4 == 0` | 8080 | 80.1441 | OK |
| `mission->isCampaignMission() == true` (dispatch) | 8080 | 80.1441 | OK |
| `mission->isCampaignMission() != 0` (dispatch) | 8080 | 80.1441 | OK |
| `!mission->isCampaignMission()` -> `== false` | 8080 | 80.1441 | OK |
| `(*cargo)[i]->isUnsaleable() == true` | 8080 | 80.1441 | OK |
| `keepObjectives` `? !x : false` -> `? x == false : false` | 8080 | 80.1441 | OK |
| `soundsMuted` local inlined | 8080 | 80.1441 | OK |
| `weaponSlot/wingmanCommand` as `short` | 8080 | 80.1441 | OK |
| `if (hudTouchFlags == 0)` -> `!hudTouchFlags` | 8080 | 80.1441 | OK |
| `jumpActive == true ?` | 8080 | 80.1441 | OK |
| `isDead() == true` | 8080 | 80.1441 | OK |
| `refinedAny = 1` / `unsigned char refinedAny` | 8080 | 80.1441 | OK |
| `int refinedAny` | 8080 | 80.1081 | OK |
| `keepObjectives = campaign->hasWon() & !...` | 8080 | 79.9496 | OK |
| `touchesStream == true` | 8080 | 79.7838 | OK |
| `starMap->exitRequested == true` | 8080 | 79.6396 | OK |
| `choiceWindowOpen == true` | 8080 | 78.4144 | OK |
| hud-menu chain `0x200,0x400,0x800` | 8068 | 77.2449 | OK |
| hud-menu chain `0x400,0x800,0x200` | 8080 | 80.0360 | OK |
| `menuTouchOpen == true` | 8068 | 79.8630 | OK |
| `starMapOpen == true` | 8076 | 75.4867 | OK |
| `freeCamMode == true` | 8076 | 76.6901 | OK |
| `campaign->hasWon() == true ?` | 8076 | 78.9692 | OK |
| `goingToWaypoint() == true` | 8084 | 78.9265 | OK |
| `hasFailed() == true` (variable init) | 8088 | 76.5988 | OK |
| `isInstantActionMission() == true` | 8088 | 83.1712 | ROT |
| top `hasWon() == true` -> bare | 8088 | 81.1532 | ROT |
| selection `hasWon() == true` -> bare | 8076 | 78.9474 | OK |
| `count != 0` / `amount != 0` / `count` | 8084 | 74.5628 | OK |
| `bool refinedAny` | 8088 | 78.0620 | OK |
| instant-first dispatch (REA shape) | 8068 | 73.6709 | OK |
| instant-first + selTrue | 8068 | 73.7070 | OK |
| instant-first + instEqT | 8076 | 75.1262 | ROT |
| genuine hoist (early-out reset) | 8088 | 81.1532 | ROT |
| hoist + instEqT | 8088 | 83.0059 | ROT |
| hoist + instEqT + failed==true | 8088 | 83.0059 | ROT |
| hoist + instEqT + keepObjectives `&` | 8088 | 82.9400 | ROT |
| hoist + merge127 | 8064 | 75.1939 | ROT |
| hoist + merge127 + instEqT | 8060 | 77.7838 | ROT |
| `__builtin_expect(won, 0)` | 7828 | 60.5045 | OK |
| `__builtin_expect(won, 1)` | 8076 | 75.7477 | ROT |
| block reorder `starMap,menuTouch,cutscene` | 8052 | 70.2829 | OK |
| block reorder `cutscene,menuTouch,starMap` | 8076 | 46.5862 | OK |
| block reorder `menuTouch,cutscene,starMap` | 8060 | 59.2539 | OK |
| block reorder `cutscene,starMap,menuTouch` | 8080 | 44.7288 | OK |
| block reorder `menuTouch,starMap,cutscene` | 8056 | 64.6614 | OK |
| `hudTouchFlags == 0` block moved before weaponSlot | 8064 | 76.4017 | OK |
| `TOP bare + INST == true` (no reset call) | 8080 | 84.3699 | BAD* |
| `TOP bare + INST + failed==true` (no reset call) | 8080 | 84.3699 | BAD* |
| `INST == true` + trailing else deleted | 8080 | 84.2826 | BAD* |

`BAD*` = `resetStartSequenceOver` is absent from `.rel.plt`; the code
is 8080 B but byte_exact is 1761.

## Why the remaining gap is not source-shaped

The call sequence of the function is **identical** to the original
(588 calls, same order, same targets).  The 630/2775 differing
instructions are almost entirely block placement, register allocation
and literal-pool offsets.

The decisive difference is the position of the **won body**
(mission-select + instant/campaign dispatch) relative to the
**hud-menu epilogue** (`initHudMenu(2)` + `if (hudTouchFlags == 0)` +
`isAutoPilot` tail):

```
original  0x17adc8 won-check
          0x17ade0 keepObjectives
          0x17af5a mission select
          0x17af84 instant dispatch
          0x17afa6 campaign dispatch
          0x17afd8 epilogue (initHudMenu(2) + tail)
          0x17b004 return
          0x17b01a hudTouchFlags == 0 block
          0x17b07e failed branch
          0x17b0aa reset block
          0x17b0b6 freelance body / changeCredits

ours      0x17adc4 won-check
          0x17adde keepObjectives
          0x17af58 epilogue (initHudMenu(2) + tail)
          0x17af84 return
          0x17afb4 hudTouchFlags == 0 block
          0x17afea failed branch
          0x17b026 mission select / dispatch
          ...
          0x17b15c reset block
```

The original places the hot won body *before* the cold epilogue; ours
places the cold epilogue first and the won body after the failed
branch.  That swap plus the 59-instruction campaign-dispatch block is
the bulk of the diff.  Both `initHudMenu(1)`/`initHudMenu(2)` share a
call site in both builds (0x17afde vs 0x17af5e), so the source shape
is already right.

Other real (non-placement) differences:

* `refinedAny` test is `lsls r0,r0,#31` (bit 0) in the original but
  `lsls r0,r0,#24` (whole byte) in ours;
* the top `hasWon()` test is `cbnz` in the original but
  `cmp #1; beq` with our `== true` (wave 118 needed it to hold
  8080 B);
* the mission-select ternary is `cbnz` in the original but
  `cmp #1; beq` with wave 118's `selTrue`;
* literal-pool offsets (`ldr r0,[pc,#N]`), which follow the layout.

Changing any of these perturbs the whole allocation: e.g.
`count != 0` makes the compiler swap `sl`/`r9` for `this` from
instruction 7 onward, which is why it scores 74.6 despite fixing the
local compare.  The only lever that reliably moves the score is the
`unsigned char`-return probability heuristic (`== true`), and wave
118 already spent the one that keeps 8080 B + PLT OK.

## Recommendation for the next wave

Attack the won-body/epilogue block order directly.  Candidate source
shapes to try (all must be re-checked for 8080 B + PLT OK):

* move the `if (this->cutsceneActive)` block *after*
  `if (this->menuTouchOpen)` but keep the call order (already tried:
  8052 B / 70.3 % — the size moves, so a compensating edit is
  needed);
* move the `if (hudTouchFlags == 0)` block before the
  `weaponSlot`/`wingmanCommand` handling (tried: 8064 B / 76.4 %);
* restructure the won body so the mission-select block is emitted
  before the hud-menu epilogue MBB (e.g. hoist the mission selection
  into a `bool`/pointer temporary before the `if (failed)` chain —
  this is behaviour-neutral but changes the MBB numbering).
