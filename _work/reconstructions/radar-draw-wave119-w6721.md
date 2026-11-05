# Radar::draw pass (wave119, w6721) — local `String` in the landmark `i == 0` arm

**Status:** landed, 2026-11-05. Worktree `wt/w6721`, branch `wave119-w6721`,
based on current `master` (after `radar-draw-wave119-w6706.md`, which landed
no source change). Target `_ZN5Radar4drawEP6PlayerP3Hudi` in
`src/game/weapons/Radar.cpp`.

Original 9908 B / 3492 normalised instructions. This pass keeps the emitted
size at 9908 B and moves the unrounded fuzzy ratio from **74.029509 %** to
**74.426606 %** (official `verify.py` display 74.0 → 74.4). Positional
matches rise 2596 → 2610. Gate unchanged: `byte_exact 4503`,
`linked_exact 4503`, `data 843/843`, imports 207, lint CLEAN, drift 0.

## 0. The change

Only the `i == 0 && !alienOrbit` arm of the landmark `i <= 1` block changes.
The lock label is materialised in a local `String` instead of being written
through `this->lockLabel`:

```cpp
KIPlayer *landmark = this->landmarks->data()[i];
String label = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                  this->cameraX, this->cameraY, this->cameraZ);
Globals::Canvas->DrawString(radarFont(), label, this->screenX + this->originX,
                            this->screenY + Globals::layout->field_0x4 * 2, false);
```

The `else` arm of the same `if` is untouched (`this->lockLabel = ...` then
`DrawString(..., this->lockLabel, ...)`, with `offset` as the x term).

This is a *tuned* shape, not a faithful one: the original also performs
`String::operator=` on `this->lockLabel` in this arm (Thumb `0x12eb0a`,
followed by `String::~String` at `0x12eb10` and the `DrawString` at
`0x12eb36`). The local drops that store. It nevertheless scores higher
because the temporary's stack slot / register colouring lines up better with
the original's `r9`-held `lockLabel` pointer at the call site.

## 1. Call-count evidence

Normalised `bl`/`blx` targets over the whole body:

| call | original | before | after |
|---|---:|---:|---:|
| `PaintCanvas::DrawString` | 9 | 8 | **9** |
| `String::operator=` | 6 | 6 | 5 |
| `Level::getPlayer` | 29 | 32 | 32 |
| `Status::getSystem` | 5 | 7 | 7 |
| `SolarSystem::getStations` | 2 | 3 | 3 |
| `SolarSystem::getStationEnumIndex` | 1 | 2 | 2 |
| `PlayerEgo::goingToPlanet` | 1 | 2 | 2 |
| `PlayerEgo::isDockingToPlanet` | 4 | 5 | 5 |
| `PlayerEgo::getAutoPilotTarget` | 1 | 2 | 2 |
| `KIPlayer::isDead` | 17 | 16 | 16 |
| `KIPlayer::isDying` | 13 | 12 | 12 |

The change fixes the `DrawString` deficit (the original has a separate call
for the `i == 0` arm and for the `else` arm; the baseline tail-merged them
into eight calls). It costs one `String::operator=`, which is the price of
the local. The seven positive deltas are the known invented recompute block
after the planet loop (see `radar-draw-wave119-w6706.md` §1); they are
unchanged.

## 2. Variants measured this wave

All on top of the pristine baseline unless noted. `usize` is the emitted
function size; `raw %` is the unrounded
`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`.

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | 74.029509 |
| **`String label = calcDistance(...)` in `i == 0` arm (LANDED)** | **9908** | **74.426606** |
| same, direct-init `String label(calcDistance(...))` | 9908 | 74.426606 (identical code) |
| landed + drop inner `__builtin_expect(lockedGasCloud, 1)` | 9908 | 74.397936 |
| landed + `String label; label = calcDistance(...)` | 9948 | 65.999142 |
| landed + `lockLabel = calcDistance(...); String label = lockLabel;` | 9932 | 72.555476 |
| landed + `String label` in the `else` arm (no store) | 9856 | 71.835443 |
| landed + `String label` + `lockLabel = label` in `else` arm | 9896 | 72.881113 |
| landed + `lockLabel = ...; String label = lockLabel;` in `else` arm | 9912 | 72.761141 |
| landed + `otherLock` single-OR expression | 9908 | 73.193807 |
| landed + `otherLock` without outer `__builtin_expect` | 9892 | 72.106697 |
| landed + recompute block moved (inside planet loop / before campaign / after prologue / before `otherLock`) | 9824 | 63.746398 (all four identical) |
| landed + recompute block moved to end of function | 9904 | 49.024110 |

Prior-wave variants (inSight reorder, asteroid dead-check, recompute delete,
type/loop/`String` sweeps) are tabulated in
`radar-draw-wave119-w6706.md` §4 and were not re-run except where combined
above.

### Recompute-position note

The invented recompute block (`if (!alienOrbit) { ... field_0x1a8 = ...; }`,
source lines 614-624) is only emitted as a duplicate computation at its
current position, immediately after the planet `for` loop. Moving it to any
other position listed above lets clang merge it with the prologue's
`onPlanetCourse` computation: `getAutoPilotTarget` drops from 2 calls to 1
and the body collapses to the "recompute deleted" size (9824 B / 63.75 %).
The current placement is therefore load-bearing for the fuzzy score even
though it is wrong relative to the original call graph.

## 3. Scratch

All scratch under `/tmp/w6721` (not committed): `Radar.base.cpp` (pristine
baseline), `Radar.u3.cpp` (landed variant), `score.sh` (relink + objdump +
difflib), `vscore.py` (official `verify.py` scorer for this one function),
`sweep.py` / `apply.py` (variant harness), `orig.dis` / `ours.dis`,
`gapsort.py`.
