# MGame::OnTouchEnd — wave 125 (w7514): `bool refinedAny` moves the cold reset block

Target `_ZN5MGame10OnTouchEndEiiPv` (`src/game/menu/MGame.cpp`), 8080 B.
Base `02513de33` (wave-124 final, gate byte 4506 / linked 4506, avg 99.93 %,
`.rel.plt` byte-identical).

## Result

| | before (HEAD) | after |
|---|---:|---:|
| fuzzy | 78.8 % | **79.2 %** |
| size | 8080 B | **8080 B** |
| `.text` differing bytes | 85,975 | **85,973** |
| `.ARM.extab` differing bytes | 1,474 | 1,475 |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` differing bytes | 0 | **0** |
| `.note.gnu.build-id` | 20 | 18 |

Full `check`: byte_exact **4506/4524**, linked_exact **4506/4524**, size-exact
4524/4524, data_exact 843/843, imports 207, lint **CLEAN**, drift **0**, symbol
parity 5370/5370, avg **99.94 %**, `RATCHET PASS`.

## Change

`MGame::OnTouchEnd`, mining-plant refine block:

```diff
-                    char refinedAny = 0;
+                    bool refinedAny = false;
```

`refinedAny` is only ever assigned `true` and tested once, so the type change is
behaviour-neutral.  It is also the faithful original type: the original tests
bit 0 (`lsls r0, r0, #31` at `0x17a66c`), while `char` tested bit 7
(`lsls r0, r0, #24`).  The one-byte flag change is enough to flip LLVM 7's
`MachineBlockPlacement` choice for the cold blocks: the mining `getText(292)`
block moves *before* the failed `type == 12` block, so the cold
`resetStartSequenceOver` block (the `else` of `won`) is emitted immediately
after the failed `type == 12` block, exactly as in the original.

## Evidence

Original block order (VMA, from the shipped `.so`):

```
0x17b05c  mining else: getText(292) / ChoiceWindow::set
0x17b07e  failed body: getMission()->getType() == 12
0x17b0aa  resetStartSequenceOver  (reached by beq from the won test at 0x17addc)
0x17b0b6  won body: setFreelanceMission ...
```

Before (HEAD): `failed type==12` `0x17b000`, mining `getText(292)` `0x17b02c`,
reset `0x17b04e` — reset follows the mining block, not the failed block.
After: mining `getText(292)` `0x17b008`, `failed type==12` `0x17b03a`, reset
`0x17b052` — the original relative order.

The `refinedAny` test itself becomes byte-identical:

```
orig 0x17a66c: 07c0  lsls r0, r0, #31
ours 0x17a66c: 07c0  lsls r0, r0, #31   (was 0600 / #24)
```

`.rel.plt` stays byte-identical (all 3674 `JUMP_SLOT` entries).  The first
reference order `isInstantActionMission` -> `resetStartSequenceOver` ->
`removeObjectives` -> `goingToWaypoint` -> `removeRoute` -> `inCinematicMode`
-> `setCinematicMode` -> `resetAnalogStick` is unchanged, so
`MGame::successCheck` remains byte-exact.

## Variants measured (all on the wave-124 base; size / pct / PLT)

| variant | size | pct | PLT |
|---|---:|---:|---|
| HEAD | 8080 | 78.8 | identical |
| **`bool refinedAny`** | **8080** | **79.2** | **identical** |
| `bool refinedAny` + `goto cutsceneTail` early shape | 8080 | 79.2 | identical |
| `bool refinedAny` + `if (!won) reset; else won body` | 8080 | 79.2 | identical |
| `bool refinedAny` + `if (!won) reset; if (won) ...` | 8080 | 79.2 | identical |
| + `menuWindow->OnTouchEnd(...)` bare | 8080 | 77.8 | identical |
| + `menuTouchOpen == true` | 8080 | 80.7 | **rotated** (3 slots) |
| + `cinematicSteerActive == true` | 8080 | 80.0 | **rotated** (3 slots) |
| + `isInstantActionMission() == true` | 8072 | 80.8 | rotated |
| + `cutsceneActive == true` | 8084 | 74.7 | rotated |
| faithful mining `if (amount != 0)` / `String(-amount)` | 8068 | 74.4 | rotated |
| `int count = -amount; if (count != 0)` | 8068 | 74.4 | rotated |
| `won` second `hasWon()` bare / `!= false` | 8068 | 77.2 | rotated |
| `failed` bare (`hasFailed()` without `== true`) | 8084 | 76.3 | rotated |
| `menuTouchOpen == true` + `menuWindow->OnTouchEnd` bare | 8080 | 78.8 | identical |

`goto`/nested/early-return spellings of the reset block are canonicalised to
the same instruction stream by clang; the `bool` type is the only lever that
moves the block while keeping 8080 B and the PLT order.

The `menuTouchOpen == true` / `cinematicSteerActive == true` variants score
higher (80.7 / 80.0) but rotate the last three `.rel.plt` slots
(`Hud::resetAnalogStick`, `MenuTouchWindow::inCinematicMode`,
`MGame::setCinematicMode`), so they are not landable under the byte-identical
`.rel.plt` constraint.

## Remaining root cause

The residual is still cold-block address placement, not code shape.  The
original places the shared return block at `0x17b004` (after the
`initHudMenu`/`isAutoPilot` fast-forward blocks) and the mission-completion
blocks after it; ours places the return block at `0x17b95c` (end of function).
The mining-counter spelling (`count > 0` vs the original `amount != 0`) is the
next faithful mismatch, but every spelling that emits `cmp #0; beq` shrinks
the body to 8068 B and shifts all following `.text` symbols.
