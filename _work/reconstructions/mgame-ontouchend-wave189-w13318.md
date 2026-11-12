# MGame::OnTouchEnd — wave 189 (w13318): full-body pass, two raw wins

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13318`, branch
`wave189-w13318`, base master `502ed160d`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13318`.

## Result

| metric | base | after |
|---|---:|---:|
| raw same-address diff bytes | 2,928 | **2,920** |
| aligned (instruction) diff | 1,670 | 1,703 |
| function size | 8,080 / 8,080 | 8,080 / 8,080 |
| instructions | 2,861 / 2,865 | 2,861 / 2,865 |
| `.text` differing bytes | 74,032 | **74,024** |
| `.rel.plt` diff | 0 | **0** (byte-identical) |
| `.ARM.extab` size | 156,312 | **156,312** (base `0x1a3b78` exact) |
| `.ARM.extab` diff | 639 | **637** |
| `.ARM.exidx` diff | 0 | **0** |
| build-id diff | 20 | 20 |
| total differing bytes | 74,691 | **74,681** |
| file size | 2,187,328 | **2,187,328** |

Gate (`check`, `RATCHET PASS`): byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact **843/843**, imports **207**, lint **CLEAN ✓**, drift **0**, parity
**5370/5370**, 27/27 section headers, file size 2,187,328 exact.

## Win 1 — `!keepObjectives` finish-mission reorder (raw 2,928 -> 2,921)

Inside the `finishMission:` `if (!keepObjectives)` block, the
`if (this->player->goingToWaypoint()) { this->player->setAutoPilot(nullptr); }`
guard was hoisted above `Globals::status->setMission(Mission::empty);` and
`this->player->setRoute(nullptr);`:

```cpp
if (!keepObjectives) {
    this->level->removeObjectives();
    if (this->player->goingToWaypoint()) {
        this->player->setAutoPilot(nullptr);
    }
    Globals::status->setMission(Mission::empty);
    this->player->setRoute(nullptr);
    this->player->removeRoute();
    this->level->setPlayerRoute(nullptr);
}
```

All six statements are independent:
- `PlayerEgo::goingToWaypoint()` is a pure read of `goingToWaypointFlag`
  (`src/game/ship/PlayerEgo.cpp:1179`), not of the route.
- `PlayerEgo::setAutoPilot()` writes the autopilot target, not the route.
- `Status::setMission()` and `Level::setPlayerRoute()` target different objects.
- The aliasing pair `setRoute(nullptr)` / `removeRoute()` keeps its relative
  order (`setRoute` before `removeRoute`), so the emitted delete-on-null
  behaviour is unchanged.

Function stays exactly 8,080 B; `.rel.plt` byte-identical.

## Win 2 — escort-mission creation order (raw 2,921 -> 2,920)

In the `mission->getType() == 183` escort block the `Mission *escort = new
Mission(183, String("Client"), ...)` construction was moved ahead of
`Item *escortItem = (*Globals::items)[216]->makeItem(10);`.  The two
allocations are independent (the `Mission` ctor reads `Globals::items` only
through `imageFactory`/`status->getStation()`, and `makeItem` does not touch
`Status`), so the reorder is behaviour-preserving.  An 8x8 single-move sweep of
the eight independent statements in that block found this as one of two
raw-2,920 orders; the other (`SetCurrentApplicationModule(2)` moved before
`setMission`) is riskier because the module switch can invalidate `this`, so the
allocation reorder was landed instead.  `.text` 74,025 -> 74,024 and
`.ARM.extab` diff 639 -> 637.

## Search context (full-body pass)

- Re-derived the whole `won` block and campaign chain from the original
  disassembly (`0x17af5a`–`0x17b2c0`) and the Unity-remake decompilation
  (`_work/oracles/.../native/game/MGame.c`). The `won` block source structure is
  `if (instantAction) { switch=2; active=0; module1; return; } if
  (isCampaignMission()) nextCampaignMission(true); else { setFreelanceMission;
  showMissionRewardMessage; }`; the campaign chain source order is
  `15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162`
  (confirmed again from the `cmp r0, #N` sequence at `+0xfd0 … +0x15d8`).
- Rebuilt the whole chain in that order (original bodies and base bodies):
  size 8,084 (+4) or 8,076 (−4) and raw 4,250–6,098 — the epilogue moves off
  `0xec0`, so a 2-byte `b.w`/`b.n` flip at `+0x6ae` shifts every later byte.
  The base's tuned chain order `74,15,162,43,42,22,65,120,96,100,95,110,81,134,
  144,126,155,161` keeps the epilogue at `0xec0` and is still the best raw.
- Full single-move (306) and 3-cycle (1,632) sweeps of the 18 chain blocks from
  the base order: no order beats raw 2,928 while keeping `.rel.plt` clean.
- Full 720-permutation sweep of the six `!keepObjectives` statements
  (`removeObjectives`, `setRoute`, `setMission`, `removeRoute`,
  `setPlayerRoute`, the `goingToWaypoint` guard) with the guard position free:
  best raw is **2,921**, achieved by `A F C B D E` (landed),
  `A F C D B E`, `A F C D E B`, `A F C E D B`. All keep size 8,080 and
  `.rel.plt` byte-identical. `A F C B D E` keeps `setRoute` before
  `removeRoute` (the original's emitted order) so it is the behaviour-faithful
  choice.
- Variants that put `removeRoute` before `setRoute` (e.g. `A D E B F C`) reach
  raw 2,926 but change `.rel.plt` (two PLT relocations swap) and delete the
  route instead of leaking it; rejected.

## Additional probes in this pass (no win, reverted)

- `won`-block preamble single-move sweep (keepObjectives / mission /
  instantAction / campaign-if / changeCredits / m_nTimeLimit, 30 moves):
  base order `K M I C D T` is best at raw 2,920. `K M I C T D` = 2,922;
  everything else >= 3,205 or size-fatal.
- `changeCredits` <-> `m_nTimeLimit` swap: raw 2,922.
- `instantAction` block statement swap (`switch_to_target_setting = 2` before
  `this->active = 0`, matching the original's store order at `+0xe40`):
  raw 2,921, aligned 1,699; reverted (raw is the primary metric).
- The `count >= 0` condition at `+0x48a` is the first differing instruction
  (`bgt.n` vs original `beq.n`). The original source is `if (amount != 0)`
  (loop counter decrements, body prints `String(-amount)`), but any
  `!= 0`/`< 0` spelling makes clang 7.0.2 canonicalise the decrementing
  counter into an incrementing one, flipping the whole function's register
  allocation (`sl`/`r9` swap) and growing the function to 8,088 B (size-fatal).
  `count > 0` is semantically correct but raw 2,922; `count >= 0` (current,
  always true) is the raw-best 2,920. `String(-amount)`/`String(count)` and
  explicit `String(..., false)` variants are byte-identical to the base.
- String-ctor scheduling: the `add.w r8, sp, #80` hoist at `+0x4a4` (22-byte
  region) is not affected by naming the `"t "` temporary, by `String(-amount)`
  vs `String(count)`, or by explicit `false` arguments.

## Residual map (unchanged)

First differing byte is still `count >= 0` emitting `bgt.n` where the original
emits `beq.n` at `+0x48b`. Then the 22-byte String-ctor schedule diff at
`+0x4a4` (the `add.w r8, sp, #80` hoist) and its 4-byte literal-pool
consequence at `+0x6b4`. The dominant residual is still the campaign-chain
block order/placement from `+0xe3e`; the tail-merge direction at `0x17bf60`
still differs (original merges into the `isDockingToAsteroid` site).
