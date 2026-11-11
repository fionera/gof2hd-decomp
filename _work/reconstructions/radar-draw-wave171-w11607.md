# Radar::draw wave 171 (w11607) — exhaustive condition re-sweep + two landed expects

**Status:** complete, 2026-11-11. Worktree `wt/w11607`, branch `wave171-w11607`,
base `master` `50716a763`.  **Final: raw 7,095 -> 7,066, aligned 1,595 -> 1,561,
size 9,908 (two commits).** Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave170-w11507.md` (three landed expects, 7,127 -> 7,095)
and `radar-draw-wave169-w11407.md`.

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,095 |
| instruction-index alignment | 1,595 |
| `.rel.plt` | byte-identical (0) |
| `.ARM.extab` differing bytes | 711 |
| `.text` differing bytes | 81,825 |

## 1. Exhaustive re-sweep on the new base

Full `if`/`while` condition sweep of the whole function: 196 conditions,
524 jobs (whole condition + every top-level `&&`/`||` subterm), both
expect values = **1,048 cells**, each built + gold-linked and measured with
the raw metric.  Scratch: `/dev/shm/w11607/{sweep.py,sweep.json}`.

Raw wins (`size == 9908`, `raw < 7095`): only the lock-landmark frame test
(cond 28, line 507).  All campaign/planetTargets conditions (30-58) are
neutral or worse on this base, confirming the wave-170 finding.

Additional targeted sweeps (not in the `if` sweep): all `for` loop
conditions (9, both values), and ternary conditions (8).  No raw win; all
were neutral or worse.

## 2. Landed (one commit, two sites)

| # | site | expect | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | `field_0x28 != 0` in the lock-landmark frame test (line 507) | 1 | 7,095 -> **7,093** | 1,595 -> **1,599** | 9,908 |
| 2 | `enemy->player->enemyFlagsLo` in the not-locked enemy image chain (line 776) | 1 | 7,093 -> **7,093** | 1,599 -> **1,561** | 9,908 |

Site 1 alone is raw 7,093 / aligned 1,599.  Site 2 alone is raw 7,095 /
aligned 1,557 (raw-neutral, aligned -38); combined with site 1 it is
raw 7,093 / aligned 1,561.  Both are size-exact and `.rel.plt` stays
byte-identical.  `check` green after the commit (byte 4,507 / linked 4,507 /
size 4,524 / data 843 / imports 207 / lint CLEAN).  Whole-`.so` `.text`
differing bytes 81,825 -> **81,823**; `.ARM.extab` unchanged at 711.

The expect on site 2 is a structural (aligned) win: it re-places the
campaign/landmark block chain and removes 34 non-matching instructions while
leaving the raw byte count unchanged.  It is kept because it does not worsen
raw bytes.

## 3. Lock-landmark `!lockConfirmed` expect (still not landable)

`if (__builtin_expect(!this->lockConfirmed, 0))` in the station-lock block
(line 500) is still **+8 B** (9,916) on the new base, raw 7,119.  All
equivalent formulations were measured and give the same +8:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `expect(!lockConfirmed, 0)` | 7,119 | 1,617 | 9,916 |
| `!expect(lockConfirmed, 1)` | 7,119 | 1,617 | 9,916 |
| `expect(lockConfirmed, 1) == 0` | 7,119 | 1,617 | 9,916 |
| empty `if (expect(lockConfirmed,1)) {} else play` | 7,117 | 1,647 | 9,916 |
| `expect(!lockConfirmed, 1)` | 7,095 | 1,595 | 9,908 (no-op) |

The `+8` is the extra `sp+220` `String` destructor site in the exception
cleanup tail (same effect reported in waves 166-168).  Combining it with
site 1 gives 7,121 / 9,916 — still not landable.

## 4. Other measured-but-not-landed rows

| site | expect | raw | aligned | size | note |
|---|---|---:|---:|---:|---|
| `alwaysEnemySeen` in the `<3` sound-ladder arm (line 1268) | 0 or 1 | 7,093 | 1,625 | 9,908 | raw -2 but aligned +30; same effect as the `bossSeen` outer expect |
| outer `expect(bossSeen,1)` in the `<5` arm (line 1278) | 1 | 7,093 | 1,625 | 9,908 | same -2/aligned+30 effect |
| `freelance->getType() == 14` (line 588) | 1 | 7,093 | 1,625 | 9,908 | hoists the marked-draw block; aligned +30 |
| `e->countsAsEnemyExcludeFlag` (line 807) | 0 | 7,095 | 1,565 | 9,908 | aligned -30, but cancels site 2 when combined |
| `enemy->player->enemyFlagsLo` (line 776) | 0 | 7,096 | 1,566 | 9,908 | raw +1 |
| `this->field_0x28 != lockedStation` (line 507) | 1 | 7,093 | 1,599 | 9,908 | same as site 1 |

The `-2 raw` sound-ladder effect is independent of site 1 (7,091 combined)
but costs +30 aligned and is masked by site 2, so it was not landed.

## 5. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11607/{sweep.py,sweep.json,testvar.py,vars_for.py,vars_tern.py,vars_sound.py}
```

## 6. Second commit: planet-lock draw guard clause

The original planet-lock draw block is a **guard clause**, not an `if` around
the draw body.  Original disassembly at `0x130486`:

```
130486 ldr  r0,[r4,#0x1a0]   ; planetLockTimer
13048a movs r1,#0
13048c cmp  r0,#1
13048e it   lt
130490 movlt r1,#1
130492 orrs r0,r5,r1         ; missionBlocksPlanets | (timer < 1)
130496 bne  12ec24           ; -> enemies
13049a ...                   ; draw body is the fallthrough
```

The source shape was reconstructed as:

```cpp
if (missionBlocksPlanets || this->planetLockTimer < 1) {
    goto enemies;
}
int frame;
...
```

(no `__builtin_expect`: the original has none on this guard).  This is
semantically identical to the previous
`if (__builtin_expect(!missionBlocksPlanets && planetLockTimer >= 1, 1)) { draw }`
but produces the original guard-clause layout.

| metric | before | after |
|---|---:|---:|
| raw | 7,093 | **7,066** |
| aligned | 1,561 | **1,561** |
| size | 9,908 | 9,908 |
| insn | 3,537 | 3,538 |

`.rel.plt` stays byte-identical; `.ARM.extab` unchanged at 711; whole-`.so`
`.text` differing bytes 81,823 -> **81,796**.  `check` green (byte 4,507 /
linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN).

## 7. Discarded: semantic-inversion "negated expect" sweep

A sweep of `__builtin_expect(!(E), v)` forms produced apparently large raw
wins (e.g. 7,066 for the planet guard, 7,090 for the `field_0x14` test), but
`expect(!E, v)` returns `!E`, i.e. it inverts the condition.  Those cells are
semantically wrong unless the body/else is swapped.  The guard-clause
restructure above is the one case where the inversion is faithful (body
swapped for `goto enemies`).  Correct negated-hint forms
(`!__builtin_expect(!E, 0)`) are codegen no-ops here.
