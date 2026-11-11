# Radar::draw wave 171 (w11707) — full re-sweep on the post-w11607 base

**Status:** complete, 2026-11-11. Worktree `wt/w11707`, branch `wave171-w11707`,
base `master` `41ef4c8b6`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave171-w11607.md` (the frame-test / enemy-chain /
planet-guard pass) and `radar-draw-wave170-w11507.md`.

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,061 |
| instruction-index alignment | 1,561 |
| `.rel.plt` | byte-identical (0) |
| `.ARM.extab` differing bytes | 711 |
| `.text` differing bytes | 81,661 |

## 1. Landed (two commits)

| # | site | expect | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | outer expect on the planet-lock `isDockingToPlanet()` guard (line 650), i.e. `expect((expect(isDockingToPlanet(),1)),0)` | 0 | 7,061 -> **7,059** | 1,561 -> **1,548** | 9,908 |
| 2 | campaign-125 freighter `getStatusValue() & (1 << bit) == 0` term (line 578) | 0 | 7,059 -> **7,054** | 1,548 -> **1,582** | 9,908 |

Both size-exact, `.rel.plt` byte-identical.  `check` green after each
(byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN).
Whole-`.so` `.text` differing bytes 81,661 -> **81,654**.
Note: site 2's waiver had to be placed on the `__builtin_expect` line with the
`&` moved to the next line (a same-line `// lint: arm_builtin` after the `&`
would swallow it).

## 2. Exhaustive whole-condition re-sweep (392 cells)

Every `if (...)` condition in the function (196), wrapped whole in
`__builtin_expect((cond), 0)` and `(cond), 1)`, built + gold-linked and
measured.  Only one size-exact raw win: line 650 (landed #1).  Everything
else was neutral or worse.  Scratch: `scratch/w11707/{gen_sweep.py,
run_whole.py,whole_results.jsonl}`.

## 3. Exhaustive top-level subterm re-sweep (412 cells)

Every top-level `&&`/`||` operand of every `if` condition, wrapped in
`__builtin_expect((sub), 0/1)`.  Size-exact raw wins:

| site | expect | raw | aligned | size |
|---|---|---:|---:|---:|
| campaign-116 `(uint32_t)(stations->data()[i]-90) <= 4` (line 567) | 0 | 7,059 -> 7,055 | 1,589 | 9,908 |
| campaign-125 freighter status-value term (line 578) | 0 | 7,059 -> **7,054** | 1,582 | 9,908 |

The two are not additive: applying both gives raw 8,420 / size 9,912.  The
line-576 term was landed (lower raw).  Scratch: `scratch/w11707/{gen_sub.py,
run_variants.py,sub_results.jsonl}`.

## 4. Enemy image chains (measured, all raw-neutral)

All remaining `enemyFlagsLo` / `carriesFriendCargoFlag` / `countsAsEnemy…`
chain arms were re-tested on the new base (expect 0/1 each):

| site | raw | aligned | size |
|---|---:|---:|---:|
| `target->enemyFlagsLo` frame/bar image chain (727) | 7,061 | 1,568 | 9,908 |
| `target->carriesFriendCargoFlag` (730) | 7,061 | 1,561 | 9,908 |
| `enemy->player->enemyFlagsLo` lock-image chain (755) | 7,061 | 1,581 | 9,908 |
| `enemy->player->enemyFlagsLo` image chain (770) | 7,061 | 1,572-1,599 | 9,908 |
| `e->countsAsEnemyExcludeFlag` wreck chain (808) | 7,061 | 1,582-1,603 | 9,908 |
| `e->shipGroup == 9` (810) | 7,061 | 1,561 | 9,908 |
| `carriesFriendCargoFlag` off-screen chain (818) | 7,826 | 1,613 | 9,900 (size!) |

No raw win; wave-171's line-776 expect remains the only landed chain arm.

## 5. Lock-landmark `!lockConfirmed` expect (still +8 B)

`if (__builtin_expect(!this->lockConfirmed, 0))` in the station-lock block
(line 500) is still **+8 B** (9,916) on the new base, raw 7,084 (7,089 before
landed #2).  The original has a plain `ldrb r0,[r4,#0x218]; cbnz` with the
play(26) block inline; the expect moves it out of line and adds a landing-pad
entry.  Variants tried (all 9,916):

| variant | raw | size |
|---|---:|---:|
| `expect(!lockConfirmed, 0)` | 7,084 | 9,916 |
| `!expect(lockConfirmed, 1)` | 7,084 | 9,916 |
| `expect(lockConfirmed, 1) == 0` | 7,084 | 9,916 |
| above + `expect(stationLockTimer > field_0x1b8, 1)` | 7,084 | 9,916 |
| above + `expect(stationLockTimer > 0, 1)` | 7,083 | 9,916 |

`expect(!lockConfirmed, 1)` is a codegen no-op (7,054 / 9,908) — the original
has no expect there.  Not landable under the size gate.

## 6. Campaign register-allocation residual (re-confirmed)

The dominant residual is unchanged: original keeps `campaign` in `sl`
(`12e0f8: mov sl,r0`, `12e38a: cmp.w sl,#2`), `missionBlocksPlanets` in `fp`,
and `stations` in `sl` after the campaign check; ours spills `campaign` to
`sp+144` and keeps `missionBlocksPlanets` in `sl`, which cascades through the
landmark/planet loops.  Removing the wave-169 `expect(...,0)` on the campaign
guard is far worse (raw 8,911 / size 9,976), so it stays.  Shape experiments
this pass (all reverted): `marked` as `int` (8,434/9,912), `marked` declared
first (no-op), `isTarget` inlined into either condition (8,428-8,459/9,912),
dropping the `campaign` local (8,499/9,904), `campaign` declared late
(8,499/9,904).

## 7. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: scratch/w11707/{gen_sweep.py,run_whole.py,gen_sub.py,run_variants.py,
#          whole_results.jsonl,sub_results.jsonl,test_*.py}
```
