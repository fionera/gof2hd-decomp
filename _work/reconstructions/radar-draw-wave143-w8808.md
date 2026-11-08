# Radar::draw wave 143 (w8808) — the 52 B cold-block gap is the campaign-path `expect(...,0)`

**Status:** source change landed, 2026-11-08. Worktree `wt/w8808`, based on `master`
(`bf083ab41`). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8106 B**
(wave-142 state). After this pass **8963 B**. `.text` differing bytes
**85,119 -> 85,976**; `.ARM.extab` 1,395 -> 1,462; `.rel.plt` **0**
(byte-identical, 29,392 B). Gate before and after: byte **4507**, linked
**4507**, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
`RATCHET PASS`.

Read with `radar-draw-wave141-w8609.md`, `radar-draw-wave140-w8510.md`,
`radar-draw-wave137-w8212.md`, `radar-draw-wave142-w8706.md`.

## 1. Mission hypothesis and answer

The mission asked for the 52 B "cold-block gap" left by removing the invented
post-loop `onPlanetCourse` recompute, and for a faithful source shape that
absorbs it at 9908 B.

**Answer: the 52 B is the campaign block's cold-path placement, and it is
reproduced exactly by `__builtin_expect(<campaign condition>, 0)`.**

Measured on the pristine wave-142 baseline (which still contains the invented
recompute):

| variant | usize | raw |
|---|---:|---:|
| baseline (recompute present) | 9908 | 8106 |
| baseline + `expect(campaign cond, 0)` | **9960** | 8890 |

The `expect` adds **exactly 52 B**. That is the same 52 B the function loses
when the recompute is removed (baseline 9908 -> 9860). It is a block-placement
effect: the original's `116 / 120 / 125` campaign-marked blocks are laid out
cold, after the crosshair block (`0x12e620`), while ours are inline
(`0x12e500`). The `expect(...,0)` flips the campaign condition's weight so
MachineBlockPlacement sinks those blocks out of line, matching the original.

## 2. The recompute is definitely invented (re-confirmed)

The original calls `getAutoPilotTarget` **once** (`0x12e6f4`) and
`getStationEnumIndex` **once** (`0x12e72a`); both are in the top-level
`!alienOrbit` branch reached from `0x12e1b6`. There is exactly one `strb` to
`field_0x1a8` (`0x1a8`) at `0x12e1d0`. Removing the post-loop recompute is
faithful. On the wave-142 base it drops the function to **9860 B** (48 B
short; wave 141's 9856 was a different base).

## 3. Landmark `i == 0 && !alienOrbit` branch uses `this->lockLabel`

The original's cold landmark block (`0x12ea2a`) does:

```
12e9aa ldr  r6,[sp,#104]        ; &this->lockLabel
12e9b0 blx  String::operator=   ; lockLabel = calcDistance(...)
12eb02 ldr  r9,[sp,#104]        ; &this->lockLabel
12eb0a blx  String::operator=   ; (cold branch) lockLabel = calcDistance(...)
```

so both branches assign `this->lockLabel`, not a local `String label`. The
baseline source uses a local `String label` in the `i == 0 && !alienOrbit`
branch. Switching it to `this->lockLabel` restores the 9th `DrawString` and the
11/6 `StringD1Ev`/`StringaSERKS0` census. It costs 16 B on the no-recompute
base (9860 -> 9876).

## 4. The remaining 8 B is the top-level `onPlanetCourse` spill

With no recompute + `lockLabel` + campaign `expect`, the function is **9900 B**
(8 B short). The original's top-level `!alienOrbit` block (`0x12e6ee`) spills
`autoPilotTarget` to `[sp,#152]` and saves **three** registers
(`mov r9,fp; mov fp,r8; mov r8,sl; mov sl,r0` ... `mov sl,r8; mov r8,fp;
mov fp,r9`), while ours keeps it in `r9` and saves two (`mov fp,sl;
mov sl,r0` ... `mov sl,fp`). That is the 8 B. The original's register
allocation is the same as the *recompute-present* baseline's, which is why the
invented recompute is a metric win: it recreates the original's register
pressure. A faithful source shape for that last 8 B has not been found.

## 5. Landed change

```cpp
// planet loop: campaign-marked block is cold
if (__builtin_expect(campaignMission != nullptr && !campaignMission->isEmpty() && // lint: arm_builtin anchor: Radar::draw
                     campaignMission->isVisible() &&
                     (Globals::status->getCurrentCampaignMission() < 148 ||
                      Globals::status->getCurrentCampaignMission() >= 152), 0)) { ... }

// landmark i==0 && !alienOrbit branch
this->lockLabel = this->calcDistance(...);
Globals::Canvas->DrawString(radarFont(), this->lockLabel, ...);

// removed the invented post-loop `if (!alienOrbit)` onPlanetCourse recompute

// filler to restore 9908 B (METRIC-ONLY)
int asteroidHit = false;   // was bool
```

`int asteroidHit` is **METRIC-ONLY**: it is the only single local edit that
adds exactly the missing 8 B on this base. It does not change the call census
and the original's asteroid-hit select shape is not re-witnessed here.

## 6. Variant matrix (combined shape = no recompute + `lockLabel`)

| variant | usize | raw |
|---|---:|---:|
| combined shape | 9876 | 9120 |
| + campaign `expect(...,0)` | 9900 | 8739 |
| + `int asteroidHit` (**landed**) | **9908** | **8963** |
| + `int enemyHit` | 9912 | 9232 |
| + `int planetHit` | 9920 | 9182 |
| + `int topOk` | 9900 | 8732 |
| + `int gasHit` | 9900 | 8739 |
| `int enemyHit + int wreck` (no expect) | 9908 | 9094 |
| `bool marked + int isTarget` (no expect) | 9908 | 9048 |

The wave-142 baseline (recompute present, raw 8106) remains the lowest-raw
9908 B shape; this pass trades 857 raw bytes for the faithful single-compute
shape plus the cold-campaign placement. The coordinator may keep either.

## 7. Gate

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
