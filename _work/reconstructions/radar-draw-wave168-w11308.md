# Radar::draw wave 168 (w11308) — ±24000 bound order in the enemy / locked-enemy guards

**Status:** one source change landed, 2026-11-11. Worktree `wt/w11308`,
branch `wave168-w11308`, based on `master` `631c26f0b`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), size 9,908 B.

Read with `radar-draw-wave167-w11207.md` (residual branch layout / cleanup
pads, no source win) and `radar-draw-wave166-w11107.md` (sound-ladder
expects + decl order).

## 0. Entry state (master 631c26f0b)

raw **7,487**, aligned **1,454**, size **9,908/9,908**, insn 3,536/3,532,
`.ARM.extab` differing **670**, `.rel.plt` **0**. `check` green (byte 4507,
linked 4507, size 4524, data 843, imports 207, lint CLEAN, parity 5370).

## 1. Landed: swap the ±24000 comparison order in the two `||` guards

The original's `lockedEnemy` cargo guard compares against **-24000 first**
(`vcmpe s0,s18` with s18 = -24000, `bmi`), then **+24000** (`vcmpe s0,s16`,
`bgt`).  The enemy on-screen guard compares against **+24000 first**.  Our
source had `> 24000` first in both `||` guards, so the locked-enemy site
emitted `bgt`/`bmi` in the opposite order (semantically identical, but the
branch opcodes are at different addresses and one byte differed).

```cpp
// locked-enemy cargo guard (line ~882)
if (this->egoPos.x - this->targetPos.x < -24000.0f ||
    this->egoPos.x - this->targetPos.x > 24000.0f ||
    ...) { ... }

// enemy on-screen guard (line ~707)
if (!(this->egoPos.x - this->targetPos.x < -24000.0f) && !(this->egoPos.x - this->targetPos.x > 24000.0f) &&
    ...) { ... }
```

Measured (all size 9,908 unless noted):

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| entry base | 7,487 | 1,454 | 9,908 | 670 |
| enemy `<` first only | 7,485 | 1,471 | 9,908 | 670 |
| locked-enemy `<` first only | 7,486 | 1,469 | 9,908 | 670 |
| **both `<` first (LANDED)** | **7,486** | **1,456** | **9,908** | **670** |
| gas-cloud `<` first | 7,487 | 1,458 | 9,908 | 670 |
| enemy `<=`/`>=` respelling | 7,487 | 1,454 | 9,908 | 670 |

Byte-level check: exactly one function byte (offset `0xdfd`) moves into
agreement and **no** byte moves out.  `.ARM.extab` is unchanged (670) and
`.rel.plt` stays byte-identical.  `check` = RATCHET PASS (byte 4507,
linked 4507, size 4524, data 843, imports 207, lint CLEAN, parity 5370).

## 2. Residual: the global `sl`/`fp` swap (campaign spill)

The wave-167 diagnosis reproduces exactly.  The original keeps
`campaign = getCurrentCampaignMission()` in `sl` (0x12e0f8 -> `cmp.w sl,#2`
at 0x12e38a) and `missionBlocksPlanets` in `fp`; ours spills `campaign` to
`sp+144` and puts `missionBlocksPlanets` in `sl`.  That single allocation
choice cascades into the ~40 % instruction mismatch and the 7-vs-6 cleanup
pads (ours' enemy-block `calcDistance` String temp is coloured to `sp+220`
instead of `sp+232`).

Variants tried this pass (all reverted, base 7,487 / 1,454 / 9,908):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `campaign > 1` / `2 <= campaign` | 7,487 | 1,454 | 9,908 |
| `const int campaign` | 7,487 | 1,454 | 9,908 |
| campaign declared before `alienOrbit` | 7,494 | 1,459 | 9,908 |
| campaign declared before `turretMode` | 8,537 | 1,452 | 9,900 |
| campaign computed just before the planet block | 9,330 | 1,521 | 9,900 |
| `int campaign2 = campaign;` + use campaign2 | 7,487 | 1,454 | 9,908 |
| `campaign + 0`, `campaign >= 2 && campaign >= 0` | 7,487 | 1,454 | 9,908 |
| mission bools declared in all 3 orders | 7,487 | 1,454 | 9,908 |
| `missionBlocksPlanets` ternary / `uint8_t` | 7,488 / 7,642 | 1,456 / 1,525 | 9,908 / 9,912 |
| `expect(!enabled, 0)` | 7,517 | 1,450 | 9,908 |
| `expect(current == 143, 0)` | 8,198 | 1,475 | 9,904 |
| `expect(!missionBlocksPlanets && planetLockTimer >= 1, 0)` | 8,613 | 2,698 | 9,912 |
| `expect(isDockingToPlanet(), 0)` | 8,778 | 1,734 | 9,880 |
| enemy `calcDistance` via named `String` temp | 8,749 | 1,729 | 9,896 |
| landmark `calcDistance` before the DrawString expr | 8,465 | 1,543 | 9,900 |
| `(current != 151) & alwaysEnemySeen` | 7,565 | 1,478 | 9,912 |

The `lsls r2,r2,#31` after `eor r2,r2,#1` in the sound condition survives
every spelling (`bool` cast, `== true`, `!= false`, De Morgan); it is the
ARM backend's bit-0 test idiom for the `bool` load and cannot be removed
without changing `alwaysEnemySeen` to `int` (which costs far more).

## 3. Conclusion

The function remains a local optimum at raw 7,486 / aligned 1,456 /
9,908 B.  The dominant residual is still the `campaign` spill; the
next lever would be a source shape that makes `GreedyRA` keep `campaign`
in `sl` across the `onPlanetCourse` block (where the original shuffles
`r9/fp/r8/sl` and spills `autoPilotTarget` to `sp+152`).
