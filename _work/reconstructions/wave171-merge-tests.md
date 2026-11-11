# Wave 171 merge tests (20 agents)

Base master `50716a763` (wave 170).  All worktrees branched from it, built with
`-j2` and a 12 GB `ulimit -v`.  Landed rows were re-measured on the combined
master tree after cherry-picking.

## Landed

| worker | target | change | raw before -> after | aligned before -> after | size |
|---|---|---|---:|---:|---:|
| w11601 | `LevelScript::process` | 1,566-cell expect / existing-expect / operand-order sweep | 53,932 -> 53,812 | 17,900 -> 17,932 | 57,232 |
| w11607 | `Radar::draw` | lock-landmark + enemy image-chain expects, planet-lock guard clause | 7,095 -> 7,061 | 1,595 -> 1,561 | 9,908 |
| w11608 | `Radar::draw` | gas-cloud loop index declared in `for` | (part of 7,061) | | 9,908 |
| w11620 | `Radar.cpp` flag | `-Xclang -target-feature -Xclang +cheap-predicable-cpsr` | (part of 7,061) | | 9,908 |
| w11612 | `Level::createMission` | drop cached `Status*` local in raid block | 5,435 -> 5,425 | 767 -> 936 | 17,172 |

Combined wave-171 result: `.text` **81,825 -> 81,661**, `.ARM.extab` 711
unchanged, build-id 20, total differing bytes **82,556 -> 82,392**.
Aligned total (15 functions) **25,618 -> 25,785**.  `check` green: byte_exact
4507/4524, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN.

## Cherry-pick interactions

* w11601 (expects) and w11604 (block-scope aliases) interact: w11601 alone is
  53,812 / 17,932; w11604 alone is 53,829 / 18,339; the combination is
  54,006 / 18,377.  w11601 is better on both metrics, so w11604's source
  changes were reverted and only its note kept.
* w11607's Radar commits stack with w11608's loop-index change and w11620's
  per-TU flag: 7,095 -> 7,061.
* w11601's new `__builtin_expect` cells needed same-line `// lint: arm_builtin`
  waivers; without them `goal_lint` reports 3 violations.

## Not landed (notes kept)

| worker | target | result |
|---|---|---|
| w11602 | `LevelScript::process` | case-8 duplicate removal, size-fatal |
| w11603 | `LevelScript::process` | state-8 duplicate `setPosition` + shared tail, raw 54,462, `.rel.plt` rotates |
| w11604 | `LevelScript::process` | alias set interacts with w11601 (53,812 -> 54,006) |
| w11605 | `LevelScript::process` | case-11 translate arg, raw +12 |
| w11606 | `LevelScript::process` | wingman-loop expect removal, raw +15 |
| w11609 | `Layout` C1/C2 | ascending 0x34..0x44 store order, raw +42 |
| w11610 | `Layout` C1/C2 | tail dispatch reconstruction, no raw win |
| w11611 | `Level::createMission` | bool `alienWave` de-sink, raw 6,769 |
| w11614 | `HangarWindow::render` | row-39/raw shape matrix, no win |
| w11615 | `Level::createShip` | switch/bitmask mission guard, aligned worse |
| w11616 | `Hud::draw` | transfer-block stack-slot permutation, no win |
| w11617 | `Status::missionCompleted` | rank/fallthrough producers, IR reaches 637/59 |
| w11618 | `Generator::createMission` | `volatile b` frees r9, raw 163 |
| w11619 | ModStation/MenuTouchWindow/MGame | re-sweep, no source win |
