# Radar::draw wave 165 (w11106) — campaign / space-lounge block `expect` cells

**Status:** two source changes landed, 2026-11-10. Worktree `wt/w11106`,
branch `wave165-w11106`, base master `3633505f8` (wave 164 docs; w11008's
Radar landing already on master). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-campaign-sl-wave164-w11007.md`,
`radar-draw-campaign-sl-wave163-w10909.md`,
`radar-draw-wave164-w11008.md`, `wave164-merge-tests.md`.

## 0. Baseline and result

| metric | wave-164 base | this pass |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 7,578 | **7,549** |
| instruction-index alignment | 1,428 | **1,446** |
| `.text` differing bytes | 83,177 | **83,148** |
| `.ARM.extab` differing bytes | 659 | **659** |
| `.rel.plt` | byte-identical | **byte-identical** |
| byte_exact / linked_exact | 4507 / 4507 | **4507 / 4507** |
| size-exact | 4524 / 4524 | **4524 / 4524** |
| data | 843 / 843 | **843 / 843** |
| imports | 207 | **207** |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |
| parity | 5370 / 5370 | **5370 / 5370** |

`check` green (RATCHET PASS).

## 1. Landed changes

Two `__builtin_expect` cells in the campaign (`planetTargets`) loop, both
same-line waived `// lint: arm_builtin anchor: Radar::draw`:

```cpp
// body1: the campaign target marker draw (cur != 120)
if (__builtin_expect(Globals::status->getCurrentCampaignMission() != 120, 1)) {
    Globals::Canvas->DrawImage2D(this->image_0xc8, ...);
    marked = true;
}
...
// tail: the planet-lock blip animation guard
if (__builtin_expect(!missionBlocksPlanets && this->planetLockTimer >= 1, 1)) {
```

Each is a pure branch-probability hint: no logic change, size stays 9,908 B.

| change | raw | aligned | size |
|---|---:|---:|---:|
| base (w11008) | 7,578 | 1,428 | 9,908 |
| + `expect(cur != 120, 1)` on the body1 marker draw | **7,570** | 1,428 | 9,908 |
| + `expect(!mbp && planetLockTimer >= 1, 1)` on the tail blip | **7,549** | 1,446 | 9,908 |

## 2. What was re-verified from the wave-164 block map

The wave-164 w11007 note's block map and condition chains were re-checked
against the current binary (all addresses are original image base 0x10000):

* The campaign guard is `campaignMission != nullptr && !isEmpty() &&
  isVisible() && (cur < 148 || cur >= 152)`; the range tail is two separate
  `getCurrentCampaignMission()` calls (`0x12e460`, `0x12e46c`). Our source
  matches.
* Body1 is `isTarget = stations->data()[i] == campaignMission->getTargetStation()`,
  then `marked = false`, then `if (stationId != currentStation && isTarget) {
  if (cur != 120) { draw; marked = true; } }`. Our source matches; the only
  difference is that the original keeps body1 inline (guard hot) while the
  w11008 `expect(guard, 0)` outlines the whole campaign block. The `expect`
  is load-bearing for the 9,908 B size (see §3).
* The mission chain is a sequential `if (cur == 116 && stationId-90 <= 4 && ...)`
  / `else if (cur == 120 && stationId == 93)` / `else if (cur == 125 && ...)`
  chain; the original re-normalises `marked` (`marked = marked != 0`) before
  the `cur == 125` test. Our source does not spell the normalisation but the
  generated chain is otherwise equivalent.
* Exit chain `if (!turretMode && !landmarkHit)`; original slots `sp+148`
  (turretMode), `sp+108` (landmarkHit); ours `sp+152`/`sp+112`. Same logic.

## 3. The `campaign`-in-`sl` flip (still not landed)

The raw metric is dominated by one register-allocation difference: the
original keeps `campaign` in `sl` (`cmp.w sl, #2` at `0x12e38a`) while our
build spills it to `[sp,#144]` and keeps the zero / `missionBlocksPlanets` /
loop-index vreg in `sl`. Everything from `0x12e38a` onward is then shifted,
which is why the same-address diff is ~250 bytes per 256-byte bucket from
`0x12e450` to `0x12f750`.

Measured attempts to flip it (all on top of the w11008 base):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base | **7,578** | **1,428** | 9,908 |
| `landmarks != nullptr` expected 0 | 9,609 | 3,430 | 9,892 |
| + `route != nullptr` expected 1 + route `onScreen` 1 | 9,585 | 3,472 | 9,900 |
| move the `mission`-type check down to the campaign block | 9,122 | 1,735 | 9,908 |
| move only the `missionBlocksPlanets` computation down | 9,122 | 1,735 | 9,908 |
| hoist `campaign` before `alienOrbit` | 7,556 | 1,451 | 9,908 |
| hoist `campaign` before `turretMode` | 8,494 | 1,452 | 9,900 |
| hoist `campaign` to the top | 8,635 | 1,471 | 9,900 |

Moving the mission-type check down does flip `campaign` into `sl`
(`cmp.w sl, #2`), but the block layout change costs far more raw than the
register win returns. The original reaches `sl` for `campaign` with the
mission check still at the top, i.e. a different greedy-allocator tie-break,
not a different source range (consistent with w10909 §4).

Removing the campaign-guard `expect(..., 0)` gives the original's inline
body1 layout but is 8,696 raw / 9,964 B (+56 B from high-register encodings);
no companion cell tried recovered the size.

## 4. Other cells swept (no win at 9,908 B)

All measured against the landed 7,549 raw shape; each was reverted.

* Campaign guard: `expect(..., 1)`, `expect(..., 0)` split before the range
  tail, `== 0`, negated, range-first, `expect(campaign >= 2, 1)`,
  `expect(planetTargets != nullptr, 1)`, `expect(!alienOrbit, 1)`,
  `expect(isDockingToPlanet(), 0)` — all worse or identical.
* Campaign body: `expect(stationId != currentStation && isTarget, 0)`
  identical; `int marked`, `int planetHit`, separate/`else if` chain,
  explicit `marked = marked != 0`, both `continue` guards, the hit-test
  bounds, `orbitPlanetIndex`, `field_0x18 != data()[i]` — all worse or
  identical.
* Campaign tail: `expect(planetHit, 1)`, `expect(planetLockTimer > 0x1b8, 1)`,
  `expect(missionBlocksPlanets, 0)`, `expect(field_0x14 != 0, 0/1)`,
  `expect(field_0x14 == field_0x18, 0)`, `expect(field_0x1a8, 1)`,
  `expect(isDockingToPlanet(), 0)` — all worse or identical.
* Landmark / space-lounge block: `expect(landmarks != nullptr, 0)`,
  `expect(i == 2 || data()[i] == nullptr || !isVisible(), 0)`,
  `expect(!landmarkHit && i != 3 && notTurretMode && notOtherLock, 0)`,
  `expect(i != 3, 1)`, `expect(i <= 1, 1)`, `expect(!turretMode, 0)`,
  `expect(lockedStation->isDead(), 0)`, `expect(stationLockTimer > 0x1b8, 1)`,
  `expect(!lockConfirmed, 0)`, `expect(field_0x28 != 0 && ..., 0)` — all
  worse or identical.
* Gas-cloud block: `expect(gasClouds != nullptr, 1)`,
  `expect(!turretMode && !enemyHit, 1)`, `expect(gasHit, 1)`,
  `expect(gasLockTimer > 0x1b8-200, 1)`, `expect(gasLockTimer > 500, 1)` —
  all worse.

## 5. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j8
# scratch harness: /dev/shm/w11106/{measure.sh,runvar.py,align.py,buckets.py}
```
