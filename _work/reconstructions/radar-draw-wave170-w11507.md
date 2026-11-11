# Radar::draw wave 170 (w11507) — remaining condition chains + lock-landmark expect

**Status:** complete, 2026-11-11. Worktree `wt/w11507`, branch `wave170-w11507`,
base `master` `00c403d5a`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave169-w11407.md` (the five condition-chain expects that
took the function 7,486 -> 7,127) and the earlier campaign/sl notes.

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,127 |
| instruction-index alignment | 1,606 |
| `.rel.plt` | byte-identical (0) |
| `.ARM.extab` differing bytes | 1,292 |
| `.text` differing bytes | 81,642 |

## 1. Sweeps

Two exhaustive automated sweeps on the current base (`/dev/shm/w11507/sweep.py`,
`whole.py`), each cell built + gold-linked and measured with the raw metric:

* **subterm sweep**: every top-level `&&`/`||` subterm of every `if` in the
  function (328 subterms, 624 variants at expect 0/1), wrapping just that
  subterm in `__builtin_expect((subterm), v)`.
* **whole-condition sweep**: every `if (...)` condition (196 conditions, 392
  variants at expect 0/1), wrapping the whole condition.

Only size-exact (9,908 B) cells with raw < 7,127 were considered wins.

## 2. Landed (three commits)

| # | site | expect | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | `alwaysEnemySeen` in the `field_0x1bc < 5` sound-ladder arm (line 1275) | 1 | 7,127 -> **7,110** | 1,606 -> 1,609 | 9,908 |
| 2 | `!isDockedToDockingPoint()` in the enemy tractor guard (line 826) | 1 | 7,110 -> **7,105** | 1,609 -> 1,594 | 9,908 |
| 3 | outer expect on the final `alwaysEnemySeen` sound-ladder arm (line 1283), i.e. `__builtin_expect((__builtin_expect(alwaysEnemySeen, 1)), 0)` | 0 | 7,105 -> **7,095** | 1,594 -> 1,595 | 9,908 |

All three are size-exact; `.rel.plt` stays byte-identical; `check` green after
each commit (byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN).  Final raw **7,095**, aligned **1,595**, size 9,908.  Whole-`.so`
`.text` differing bytes 81,642 -> **81,610**; `.ARM.extab` unchanged at 1,292.

Note on #3: the *nested* form is load-bearing.  Replacing the inner
`expect(alwaysEnemySeen, 1)` with `expect(alwaysEnemySeen, 0)` measures
8,586 raw (size exact), and the whole-condition sweep only wins when the outer
expect is added around the existing inner one.

## 3. Lock-landmark expect (measured, not landed)

`if (__builtin_expect(!this->lockConfirmed, 0))` in the landmark station-lock
block (the `stationLockTimer > field_0x1b8` arm, line ~507):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (7,105 at the time) | 7,105 | 1,594 | 9,908 |
| `expect(!lockConfirmed, 0)` | 7,117 | 1,616 | **9,916 (+8)** |
| `expect(!lockConfirmed, 1)` | 7,105 | 1,594 | 9,908 (neutral) |

The `+8 B` is the extra `sp+220` `String` destructor site in the exception
cleanup tail (same effect the wave-166/167/168 notes reported).  Not landable
under the size gate; expect 1 is a no-op.

## 4. Other near misses (not landed)

| site | expect | raw | size | note |
|---|---|---:|---:|---|
| `field_0x28 != 0` (landmark frame test, line 507) | 1 | 7,126 | 9,908 | alone; 7,108 on top of landed #1+#2 |
| `field_0x28 != lockedStation` | 1 | 7,126 | 9,908 | same |
| `(current != 151 && alwaysEnemySeen)` (line 1260) | 0 | 7,126 | 9,908 | neutral on top of #1+#2 |
| `(!enemyHit || enemy->field_0x24)` (line 826) | 1 | 7,122 | 9,908 | alone; 8,570 on top of #1+#2 |
| `(!wreck || enemy->isDead())` (line 826) | 1 | 7,122 | 9,908 | alone; 8,570 on top of #1+#2 |
| `topOk` (landmark hit test, line 445) | 1 | 7,127 | 9,908 | neutral |

The campaign / `planetTargets` block is still untouched: every subterm and
whole-condition expect in conds 30-58 measures 7,127 raw / 9,908 B (or worse),
so the `campaign`-in-`sl` vs spill allocation remains the dominant residual.

## 5. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11507/{sweep.py,whole.py,combo.py,sweep.jsonl,whole.jsonl}
```
